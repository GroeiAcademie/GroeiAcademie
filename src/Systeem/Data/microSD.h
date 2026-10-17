#ifndef MICROSD_H
#define MICROSD_H

#include "../GedeeldeBus/GedeeldeBus.h"

// ============================================================================
// microSD SPI: SdFat, FAT16/FAT32 via SdFat32. Eén object per kaart.
// Dezelfde SdFat als Adafruit_SPIFlash (SdFat - Adafruit Fork): twee SdFat-versies in één programma botsen (O28).
// ============================================================================
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD_SPI) || REDIRECT_SERIAL == REDIRECT_SERIAL_TO_MICROSD

  #include <SdFat_Adafruit_Fork.h>
  #if defined(ARDUINO_ARCH_RENESAS)
    #undef F   // SdFat maakt F(str) als macro; de R4-core heeft F() als functie, die zo terug geldt
  #endif

struct DataMICROSDviaSPI : HardwareResourceTypeSPI, SdFat32 {
  DataMICROSDviaSPI(GedeeldeBusNode* parent, HardwareResourcePin np_CS, HardwareResourcePin np_SCK, HardwareResourcePin np_MISO, HardwareResourcePin np_MOSI, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : HardwareResourceTypeSPI(parent, GedeeldeBusComponent::DATA_MICROSD, np_CS, np_SCK, np_MISO, np_MOSI, Extender) {}

  ~DataMICROSDviaSPI() override;   // wist de eigen plaats in de lijst van Data

  bool Activeren() override;
};

#endif

#endif // MICROSD_H
