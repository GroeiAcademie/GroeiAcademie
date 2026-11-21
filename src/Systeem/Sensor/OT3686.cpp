#include "OT3686.h"
#if SENSOR_OT3686_AANTAL > 0
OT3686* OT3686::actieveInterruptSensor = nullptr;
void OT3686::OntvangPuls() {
  OT3686* sensor = actieveInterruptSensor;
  if (!sensor) return;
  const unsigned long nu = micros();
  if (sensor->eerstePulsOntvangen) {
    const unsigned long verstreken = nu - sensor->tijdstipLaatstePulsUS;
    if (verstreken < SENSOR_OT3686_MINIMALE_PULSPERIODE_US) return;
    sensor->periodeLaatstePulsUS = verstreken;
  }
  sensor->eerstePulsOntvangen = true;
  sensor->tijdstipLaatstePulsUS = nu;
  ++sensor->aantalPulsen;
}
bool OT3686::inpluggen() {
  if (ingeplugd) return true;
  if (np_OUT == HardwareResourcePin::NONE || np_OUT == HardwareResourcePin::CUSTOM) return false;
  if (firstExtenderNode() != nullptr) {
    // Alleen een directe Extender-parent.
    if (parent == nullptr || parent != firstExtenderNode()) return false;
#if EXTENDER_CD74HC4067_AANTAL > 0
    // Via de CD74HC4067: np_OUT is het kanaal EP_Yn; de interrupt komt op de native Z-pin (zie Activeren()).
    if (parent->component == GedeeldeBusComponent::ADS_MULTIPLEXER_CD74HC4067) {
      if (!Sensor::inpluggen()) return false;
      pinMode(NativeArduinoPinVan(static_cast<ExtenderCD74HC4067*>(parent)->np_Z), INPUT);
      return true;
    }
#endif
#if GEDEELDE_BUS_EXTENDER_GPIO_ACTIEF
    // Via een Extender met digitale pinnen (MCP23S17, MAX14830 ...): ingang, geen interrupt.
    // Pulsen tellen via die Extender hoort bij de functies, niet bij Activeren().
    if (!Sensor::inpluggen()) return false;
    return parent->SetExtenderPinMode(static_cast<uint8_t>(np_OUT), INPUT);
#else
    return false;
#endif
  }
  if (!Sensor::inpluggen()) return false;
  pinMode(NativeArduinoPinVan(np_OUT), INPUT);
  return true;
}
bool OT3686::Activeren() {
  if (!ingeplugd || !parent || !parent->actief) return false;
  if (interruptActief || pulseCountingByPollingActive) return true; // bij heractiveren de bestaande pulsteller niet wissen
  if (firstExtenderNode() != nullptr) {
#if EXTENDER_CD74HC4067_AANTAL > 0
    if (parent->component != GedeeldeBusComponent::ADS_MULTIPLEXER_CD74HC4067)
#endif
    {
#if !GEDEELDE_BUS_EXTENDER_GPIO_ACTIEF
      return false;
#else
      // GPIO-Extender: geen interrupt; de telling loopt via PollPulseCount().
      const uint8_t niveau = parent->ReadExtenderPin(static_cast<uint8_t>(np_OUT));
      if (niveau != LOW && niveau != HIGH) return false;
      previousStateOUTforRisingEdge = niveau == HIGH;
      aantalPulsen = 0;
      tijdstipLaatstePulsUS = 0;
      periodeLaatstePulsUS = 0;
      eerstePulsOntvangen = false;
      aantalPulsenVorigVenster = 0;
      tijdstipVorigVensterMS = millis();
      laatsteVensterRPM = NAN;
      pulseCountingByPollingActive = true;
      return true;
#endif
    }
  }
  const int interruptNummer = digitalPinToInterrupt(
#if EXTENDER_CD74HC4067_AANTAL > 0
      firstExtenderNode() != nullptr ? NativeArduinoPinVan(static_cast<ExtenderCD74HC4067*>(parent)->np_Z) :
#endif
      NativeArduinoPinVan(np_OUT));
  if (interruptNummer < 0 || (actieveInterruptSensor != nullptr && actieveInterruptSensor != this)) return false;
#if defined(NOT_AN_INTERRUPT)
  if (interruptNummer == NOT_AN_INTERRUPT) return false;
#endif
  noInterrupts();
  aantalPulsen = 0;
  tijdstipLaatstePulsUS = 0;
  periodeLaatstePulsUS = 0;
  eerstePulsOntvangen = false;
  interrupts();
  aantalPulsenVorigVenster = 0;
  tijdstipVorigVensterMS = millis();
  laatsteVensterRPM = NAN;
  actieveInterruptSensor = this;
  attachInterrupt(interruptNummer, OntvangPuls, RISING);
  interruptActief = true;
  return true;
}
bool OT3686::afmelden() {
  if (interruptActief) {
    detachInterrupt(digitalPinToInterrupt(
#if EXTENDER_CD74HC4067_AANTAL > 0
        firstExtenderNode() != nullptr ? NativeArduinoPinVan(static_cast<ExtenderCD74HC4067*>(parent)->np_Z) :
#endif
        NativeArduinoPinVan(np_OUT)));
    interruptActief = false;
    if (actieveInterruptSensor == this) actieveInterruptSensor = nullptr;
  }
  return Sensor::afmelden();
}
bool OT3686::LeesDigitaleToestand(bool& waarde) {
  waarde = false;
  if (!actief) return false;
  if (firstExtenderNode() != nullptr) {
#if EXTENDER_CD74HC4067_AANTAL > 0
    // Via de CD74HC4067: Z lezen (het kanaal blijft open).
    if (parent->component == GedeeldeBusComponent::ADS_MULTIPLEXER_CD74HC4067) {
      waarde = digitalRead(NativeArduinoPinVan(static_cast<ExtenderCD74HC4067*>(parent)->np_Z)) == HIGH;
    } else
#endif
    {
#if GEDEELDE_BUS_EXTENDER_GPIO_ACTIEF
      const uint8_t niveau = parent->ReadExtenderPin(static_cast<uint8_t>(np_OUT));
      if (niveau != LOW && niveau != HIGH) return false;
      waarde = niveau == HIGH;
#else
      return false;
#endif
    }
  } else {
    waarde = digitalRead(NativeArduinoPinVan(np_OUT)) == HIGH;
  }
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  WriteQueueOrStack(4, static_cast<int>(waarde));
#endif
  return true;
}
bool OT3686::LeesAantalPulsen(uint32_t& waarde) {
  waarde = 0;
  if (!actief || (!interruptActief && !pulseCountingByPollingActive)) return false;
  noInterrupts();
  waarde = aantalPulsen;
  interrupts();
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  WriteQueueOrStack(3, waarde);
#endif
  return true;
}
bool OT3686::LeesRPMViaTijdvenster(float& waarde) {
  waarde = NAN;
  uint32_t aantal = 0;
  if (!LeesAantalPulsen(aantal)) return false;
  const unsigned long nu = millis();
  const unsigned long verstreken = nu - tijdstipVorigVensterMS;
  if (verstreken >= SENSOR_OT3686_MEETINTERVAL_MS) {
    const uint32_t nieuwePulsen = aantal - aantalPulsenVorigVenster;
    laatsteVensterRPM = (static_cast<float>(nieuwePulsen) * 60000.0f) /
                       (static_cast<float>(verstreken) * SENSOR_OT3686_AANTAL_SLEUVEN);
    aantalPulsenVorigVenster = aantal;
    tijdstipVorigVensterMS = nu;
  }
  if (isnan(laatsteVensterRPM)) return false;
  waarde = laatsteVensterRPM;
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  WriteQueueOrStack(1, waarde);
#endif
  return true;
}
bool OT3686::LeesRPMViaPeriode(float& waarde) {
  waarde = NAN;
  if (!actief || (!interruptActief && !pulseCountingByPollingActive)) return false;
  unsigned long laatste = 0, periode = 0;
  noInterrupts();
  laatste = tijdstipLaatstePulsUS;
  periode = periodeLaatstePulsUS;
  interrupts();
  if (periode == 0) return false; // minstens twee pulsen vereist
  if (micros() - laatste >= static_cast<unsigned long>(SENSOR_OT3686_STILSTAND_TIMEOUT_MS) * 1000UL) {
    waarde = 0.0f;
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
    WriteQueueOrStack(2, waarde);
#endif
    return true;
  }
  waarde = 60000000.0f / (static_cast<float>(periode) * SENSOR_OT3686_AANTAL_SLEUVEN);
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  WriteQueueOrStack(2, waarde);
#endif
  return true;
}
bool OT3686::PollPulseCount() {
#if !GEDEELDE_BUS_EXTENDER_GPIO_ACTIEF
  return false;
#else
  if (!actief || !pulseCountingByPollingActive) return false;
  const uint8_t niveau = parent->ReadExtenderPin(static_cast<uint8_t>(np_OUT));
  if (niveau != LOW && niveau != HIGH) return false;
  const bool hoog = niveau == HIGH;
  // Stijgende flank (zoals RISING bij de interrupt): dezelfde verwerking als OntvangPuls().
  if (hoog && !previousStateOUTforRisingEdge) {
    const unsigned long nu = micros();
    bool tellen = true;
    if (eerstePulsOntvangen) {
      const unsigned long verstreken = nu - tijdstipLaatstePulsUS;
      if (verstreken < SENSOR_OT3686_MINIMALE_PULSPERIODE_US) tellen = false;
      else periodeLaatstePulsUS = verstreken;
    }
    if (tellen) {
      eerstePulsOntvangen = true;
      tijdstipLaatstePulsUS = nu;
      ++aantalPulsen;
    }
  }
  previousStateOUTforRisingEdge = hoog;
  return true;
#endif
}
bool OT3686::LeesHoofdwaarde(float& waarde) {
  return LeesRPMViaTijdvenster(waarde);
}
#endif
