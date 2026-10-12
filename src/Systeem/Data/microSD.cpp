#include "microSD.h"

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD) || REDIRECT_SERIAL == REDIRECT_SERIAL_TO_MICROSD

bool DataMicroSD::Activeren() {
#if defined(ARDUINO_ARCH_ESP32)
  if (np_CS == DATA_LAYER_MICROSD_2_CS_PIN) return SDFileSystemClass::begin(NativeArduinoPinVan(np_CS), SPI, 4000000, "/sd2");
  return SDFileSystemClass::begin(NativeArduinoPinVan(np_CS), SPI, 4000000, "/sd1");
#else
  return SDFileSystemClass::begin(NativeArduinoPinVan(np_CS));
#endif
}

#endif
