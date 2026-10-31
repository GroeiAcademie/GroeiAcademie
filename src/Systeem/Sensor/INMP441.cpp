#include "INMP441.h"
#if SENSOR_INMP441_AANTAL > 0
bool INMP441::inpluggen() {
  if (ingeplugd) return true;
  if (firstExtenderNode() != nullptr || np_BCLK == HardwareResourcePin::NONE ||
      np_WS == HardwareResourcePin::NONE || np_SD == HardwareResourcePin::NONE ||
      np_BCLK == HardwareResourcePin::CUSTOM || np_WS == HardwareResourcePin::CUSTOM ||
      np_SD == HardwareResourcePin::CUSTOM || np_BCLK == np_WS || np_BCLK == np_SD || np_WS == np_SD)
    return false;
  return Sensor::inpluggen();
}

bool INMP441::Activeren() {
  if (!ingeplugd || !parent || !parent->actief) return false;
  I2S.setPins(NativeArduinoPinVan(np_BCLK), NativeArduinoPinVan(np_WS), -1,
              NativeArduinoPinVan(np_SD));
  return I2S.begin(I2S_MODE_STD, SENSOR_INMP441_SAMPLEFREQUENTIE_HZ,
                   I2S_DATA_BIT_WIDTH_32BIT, I2S_SLOT_MODE_STEREO);
}

size_t INMP441::LeesSamples(int32_t* samples, size_t aantal) {
  if (!actief || samples == nullptr || aantal == 0 || aantal > (static_cast<size_t>(-1) / sizeof(int32_t))) return 0;
  // Officiële API levert het aantal gelezen bytes, niet het aantal samples.
  const size_t gelezen = I2S.readBytes(reinterpret_cast<char*>(samples), aantal * sizeof(int32_t)) / sizeof(int32_t);
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  for (size_t index = 0; index < gelezen; ++index)
    WriteQueueOrStack(1, static_cast<int>(samples[index])); // ESP32-S3: 32-bit int, geen conversieverlies
#endif
  return gelezen;
}

bool INMP441::afmelden() {
  if (actief) I2S.end();
  return Sensor::afmelden();
}
#endif
