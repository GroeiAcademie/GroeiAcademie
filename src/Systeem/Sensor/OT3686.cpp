#include "OT3686.h"
#if SENSOR_OT3686_AANTAL > 0
OT3686* OT3686::actieveInterruptSensor = nullptr;
void OT3686::OntvangPuls() {
  OT3686* sensor = actieveInterruptSensor;
  if (!sensor) return;
  const unsigned long nu = micros();
  if (sensor->eerstePulsOntvangen) {
    const unsigned long verstreken = nu - sensor->tijdstipLaatstePulsUs;
    if (verstreken < SENSOR_OT3686_MINIMALE_PULSPERIODE_US) return;
    sensor->periodeLaatstePulsUs = verstreken;
  }
  sensor->eerstePulsOntvangen = true;
  sensor->tijdstipLaatstePulsUs = nu;
  ++sensor->aantalPulsen;
}
bool OT3686::inpluggen() {
  if (ingeplugd) return true;
  if (firstExtenderNode() != nullptr || np_OUT == HardwareResourcePin::NONE ||
      np_OUT == HardwareResourcePin::CUSTOM) return false;
  if (!Sensor::inpluggen()) return false;
  pinMode(NativeArduinoPinVan(np_OUT), INPUT);
  return true;
}
bool OT3686::Activeren() {
  if (!ingeplugd || !parent || !parent->actief) return false;
  if (interruptActief) return true; // bij heractiveren de bestaande pulsteller niet wissen
  const int interruptNummer = digitalPinToInterrupt(NativeArduinoPinVan(np_OUT));
  if (interruptNummer < 0 || (actieveInterruptSensor != nullptr && actieveInterruptSensor != this)) return false;
#if defined(NOT_AN_INTERRUPT)
  if (interruptNummer == NOT_AN_INTERRUPT) return false;
#endif
  noInterrupts();
  aantalPulsen = 0;
  tijdstipLaatstePulsUs = 0;
  periodeLaatstePulsUs = 0;
  eerstePulsOntvangen = false;
  interrupts();
  aantalPulsenVorigVenster = 0;
  tijdstipVorigVensterMs = millis();
  laatsteVensterRPM = NAN;
  actieveInterruptSensor = this;
  attachInterrupt(interruptNummer, OntvangPuls, RISING);
  interruptActief = true;
  return true;
}
bool OT3686::afmelden() {
  if (interruptActief) {
    detachInterrupt(digitalPinToInterrupt(NativeArduinoPinVan(np_OUT)));
    interruptActief = false;
    if (actieveInterruptSensor == this) actieveInterruptSensor = nullptr;
  }
  return Sensor::afmelden();
}
bool OT3686::LeesDigitaleToestand(bool& waarde) {
  waarde = false;
  if (!actief) return false;
  waarde = digitalRead(NativeArduinoPinVan(np_OUT)) == HIGH;
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  WriteQueueOrStack(4, static_cast<int>(waarde));
#endif
  return true;
}
bool OT3686::LeesAantalPulsen(uint32_t& waarde) {
  waarde = 0;
  if (!actief || !interruptActief) return false;
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
  const unsigned long verstreken = nu - tijdstipVorigVensterMs;
  if (verstreken >= SENSOR_OT3686_MEETINTERVAL_MS) {
    const uint32_t nieuwePulsen = aantal - aantalPulsenVorigVenster;
    laatsteVensterRPM = (static_cast<float>(nieuwePulsen) * 60000.0f) /
                       (static_cast<float>(verstreken) * SENSOR_OT3686_AANTAL_SLEUVEN);
    aantalPulsenVorigVenster = aantal;
    tijdstipVorigVensterMs = nu;
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
  if (!actief || !interruptActief) return false;
  unsigned long laatste = 0, periode = 0;
  noInterrupts();
  laatste = tijdstipLaatstePulsUs;
  periode = periodeLaatstePulsUs;
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
bool OT3686::LeesHoofdwaarde(float& waarde) {
  return LeesRPMViaTijdvenster(waarde);
}
#endif
