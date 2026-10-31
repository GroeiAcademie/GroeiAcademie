#include "HX711.h"
#if SENSOR_HX711_AANTAL > 0
// Official: https://github.com/bogde/HX711/blob/master/src/HX711.h
bool SensorHX711::inpluggen() {
  if (ingeplugd) return true;
  // Deze driver gebruikt native timing/pinnen: geen GPIO- of 1-Wire-Extenderprotocol.
  if (firstExtenderNode() != nullptr) return false;
  if (this->np_DOUT == HardwareResourcePin::NONE || this->np_DOUT == HardwareResourcePin::CUSTOM) return false;
  if (this->np_PD_SCK == HardwareResourcePin::NONE || this->np_PD_SCK == HardwareResourcePin::CUSTOM) return false;
  if (!Sensor::inpluggen()) return false;
  adc.begin(NativeArduinoPinVan(this->np_DOUT), NativeArduinoPinVan(this->np_PD_SCK));
  return true;
}
bool SensorHX711::Activeren() {
  if (!parent || !parent->actief || !ingeplugd) return false;
  // Interface voorbereid; fysieke respons wordt later gecontroleerd.
  return true;
}

bool SensorHX711::LeesHoofdwaarde(float& waarde) {
  waarde = NAN;
  if (!actief || !adc.is_ready()) return false;
  waarde = static_cast<float>(adc.read()); // ruwe brugwaarde, niet gekalibreerd
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  WriteQueueOrStack(1, waarde);
#endif
  return true;
}
#endif
