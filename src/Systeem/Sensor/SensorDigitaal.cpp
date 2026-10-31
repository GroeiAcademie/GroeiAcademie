#include "SensorDigitaal.h"
#if SENSOR_HCSR501_AANTAL > 0 || SENSOR_HW201_AANTAL > 0
bool SensorDigitaal::inpluggen() {
  if (ingeplugd) return true;
  if (this->np_OUT == HardwareResourcePin::NONE || this->np_OUT == HardwareResourcePin::CUSTOM) return false;
  if (firstExtenderNode() != nullptr) {
#if GEDEELDE_BUS_EXTENDER_GPIO_ACTIEF
    // Alleen een directe GPIO-Extender-parent; geen willekeurige Extender.
    if (parent == nullptr || parent != firstExtenderNode()) return false;
    // Alleen een GPIO-Extender, niet een willekeurige andere Extender.
    // De Extender controleert zelf de fysieke pinrange.
    switch (parent->component) {
      case GedeeldeBusComponent::DIGITAL_PINS_PCF8574:
      case GedeeldeBusComponent::DIGITAL_PINS_PCF8575:
      case GedeeldeBusComponent::DIGITAL_PINS_MCP23017:
      case GedeeldeBusComponent::DIGITAL_PINS_MCP23S17:
        break;
      default: return false;
    }
    if (!Sensor::inpluggen()) return false;
    parent->SetExtenderPinMode(static_cast<uint8_t>(np_OUT), INPUT);
    return true;
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
    if (parent != firstExtenderNode()) return false;
    if (parent->gedeeldeIdentiteiten && parent->gedeeldeIdentiteiten[0] == HardwareResourceIdentiteit::I2C_SDA) {
      HardwareResourceTypeI2C* apparaat = static_cast<HardwareResourceTypeI2C*>(parent);
      if (apparaat->ingeplugd && !apparaat->ComponentBeschikbaarTijdensRuntime()) return false;
    }
    waarde = static_cast<float>(parent->ReadExtenderPin(static_cast<uint8_t>(np_OUT)));
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
    WriteQueueOrStack(1, waarde);
#endif
    return true; // De bestaande GPIO-interface retourneert geen transportfoutstatus.
  }
  waarde = static_cast<float>(digitalRead(NativeArduinoPinVan(np_OUT)));
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  WriteQueueOrStack(1, waarde);
#endif
  return true;
}
#endif
