#include "SensorDigitaal.h"
#if SENSOR_HCSR501_AANTAL > 0 || SENSOR_HW201_AANTAL > 0
bool SensorDigitaal::inpluggen() {
  if (ingeplugd) return true;
  if (this->np_OUT == HardwareResourcePin::NONE || this->np_OUT == HardwareResourcePin::CUSTOM) return false;
  if (firstExtenderNode() != nullptr) {
    if (parent == nullptr || parent != firstExtenderNode()) return false;
    // Een ADC-kanaal (ADS1115, ADS7828, ADS7953, CD74HC4067, AIN van de ADS1158): OUT via de ADC en een drempel.
    // Bereik 0 vraagt alleen of de meting ondersteund wordt (geen busactie).
    if (parent->StartExtenderReadAnalog(static_cast<uint8_t>(np_OUT), 0) == 1) {
      if (!Sensor::inpluggen()) return false;
      analogMeasurementRangeMillivolt = component == GedeeldeBusComponent::HW201 ? SENSOR_HW201_ADC_MEASUREMENT_RANGE_MILLIVOLT : SENSOR_HCSR501_ADC_MEASUREMENT_RANGE_MILLIVOLT;
      return true;
    }
#if GEDEELDE_BUS_EXTENDER_GPIO_ACTIEF
    // Alleen een directe GPIO-Extender-parent; geen willekeurige Extender.
    // Alleen een GPIO-Extender, niet een willekeurige andere Extender.
    // De Extender controleert zelf de fysieke pinrange.
    switch (parent->component) {
      case GedeeldeBusComponent::DIGITAL_PINS_PCF8574:
      case GedeeldeBusComponent::DIGITAL_PINS_PCF8575:
      case GedeeldeBusComponent::DIGITAL_PINS_MCP23017:
      case GedeeldeBusComponent::DIGITAL_PINS_MCP23S17:
      case GedeeldeBusComponent::ADC_ADS1158:      // eigen GPIO0..GPIO7
      case GedeeldeBusComponent::UART_SC16IS752:   // eigen GPIO0..GPIO7
      case GedeeldeBusComponent::UART_MAX14830:    // eigen GPIO0..GPIO15
        break;
      default: return false;
    }
    if (!Sensor::inpluggen()) return false;
    return parent->SetExtenderPinMode(static_cast<uint8_t>(np_OUT), INPUT);
#else
    return false;  // Buiten de bestaande compile-time mixed-pininterface.
#endif
  }
  if (!Sensor::inpluggen()) return false;
  pinMode(NativeArduinoPinVan(this->np_OUT), INPUT);
  return true;
}
bool SensorDigitaal::Activeren() {
  // Geen ID of busprotocol. Dit controleert alleen de geconfigureerde ingang.
  // Sensor::activeren() beheert de bestaande GedeeldeBus-status.
  return parent && parent->actief && ingeplugd;
}

bool SensorDigitaal::LeesHoofdwaarde(float& waarde) {
  waarde = NAN;
  if (!parent || !actief) return false;
  if (firstExtenderNode() != nullptr) {
    if (analogMeasurementRangeMillivolt != 0) return ReadOUTviaAnalogThreshold(waarde);
#if GEDEELDE_BUS_EXTENDER_GPIO_ACTIEF
    if (parent != firstExtenderNode()) return false;
    if (parent->gedeeldeIdentiteiten && parent->gedeeldeIdentiteiten[0] == HardwareResourceIdentiteit::I2C_SDA) {
      HardwareResourceTypeI2C* apparaat = static_cast<HardwareResourceTypeI2C*>(parent);
      if (apparaat->ingeplugd && !apparaat->ComponentBeschikbaarTijdensRuntime()) return false;
    }
    waarde = static_cast<float>(parent->ReadExtenderPin(static_cast<uint8_t>(np_OUT)));
    if (waarde != LOW && waarde != HIGH) { waarde = NAN; return false; }
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
    WriteQueueOrStack(1, waarde);
#endif
    return true;
#else
    return false;  // Buiten de bestaande compile-time mixed-pininterface.
#endif
  }
  waarde = static_cast<float>(digitalRead(NativeArduinoPinVan(np_OUT)));
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  WriteQueueOrStack(1, waarde);
#endif
  return true;
}
bool SensorDigitaal::ReadOUTviaAnalogThreshold(float& waarde) {
  waarde = NAN;
  if (!parent || !actief || analogMeasurementRangeMillivolt == 0) return false;
  // Zelfde blokkerende wachtpatroon als SensorAnaloog::RawAnalogRead(): even wachten als de ADC bezet is.
  const unsigned long startTime = millis();
  int8_t startResult = parent->StartExtenderReadAnalog(static_cast<uint8_t>(np_OUT), analogMeasurementRangeMillivolt);
  while (startResult == 0 && millis() - startTime <= 150) {
    yield();
    startResult = parent->StartExtenderReadAnalog(static_cast<uint8_t>(np_OUT), analogMeasurementRangeMillivolt);
  }
  if (startResult != 1) return false;
  while (!parent->IsExtenderReadAnalogReady() && millis() - startTime <= 150) yield();
  int32_t rawValue = 0;
  // Na een gestarte meting altijd Get: die geeft de ADC vrij, ook na een time-out (ADC-contract in GedeeldeBus.h).
  // Ready beslist alleen of de waarde geldig is.
  const bool klaar = parent->IsExtenderReadAnalogReady();
  const bool gelukt = parent->GetExtenderReadAnalogValue(rawValue) && klaar;
  if (parent->gedeeldeIdentiteiten != nullptr && parent->gedeeldeIdentiteiten[0] == HardwareResourceIdentiteit::I2C_SDA) {
    static_cast<HardwareResourceTypeI2C*>(parent)->ResultaatComponentToegangTijdensRuntimeVerwerken(gelukt);
  }
  if (!gelukt) return false;
  // Zoals LM35DZ: millivolt = ruwe waarde x effectief full-scale bereik / ADC_MAX.
  const uint16_t effectiveRangeMillivolt = parent->GetExtenderReadAnalogEffectiefBereikMillivolt();
  if (effectiveRangeMillivolt == 0) return false;
  const float measuredMillivolt = static_cast<float>(rawValue) * effectiveRangeMillivolt / static_cast<float>(ADC_MAX);
  waarde = measuredMillivolt >= analogThresholdForHighMillivolt ? HIGH : LOW;
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  WriteQueueOrStack(1, waarde);
#endif
  return true;
}
#endif
