#include "microSD.h"

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD) || REDIRECT_SERIAL == REDIRECT_SERIAL_TO_MICROSD

bool DataMicroSD::Activeren() {
  // De SD-bibliotheken van de boards geven geen foutcode terug: fout = 0.
  bool gelukt = false;
#if defined(ARDUINO_ARCH_ESP32)
  #if DATA_LAYER_MICROSD_AANTAL >= 2
    if (np_CS == DATA_LAYER_MICROSD_2_CS_PIN) gelukt = SDFileSystemClass::begin(NativeArduinoPinVan(np_CS), SPI, 4000000, "/sd2");
    else
  #endif
  gelukt = SDFileSystemClass::begin(NativeArduinoPinVan(np_CS), SPI, 4000000, "/sd1");
#elif defined(ARDUINO_ARCH_RP2040)
  gelukt = SDClass::begin(NativeArduinoPinVan(np_CS));
#else
  gelukt = SDFileSystemClass::begin(NativeArduinoPinVan(np_CS));
#endif
  return FoutDiagnoseSPI(gelukt, 0, gelukt);
}

#endif
