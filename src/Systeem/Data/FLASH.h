#ifndef FLASH_H
#define FLASH_H

#include "../GedeeldeBus/GedeeldeBus.h"

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_FLASH)

  #include <SPIMemory.h>

struct DataFLASH : HardwareResourceTypeSPI, SPIFlash {
  DataFLASH(GedeeldeBusNode* parent, HardwareResourcePin np_CS, HardwareResourcePin np_SCK, HardwareResourcePin np_MISO, HardwareResourcePin np_MOSI, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : HardwareResourceTypeSPI(parent, GedeeldeBusComponent::DATA_FLASH, np_CS, np_SCK, np_MISO, np_MOSI, Extender), SPIFlash(NativeArduinoPinVan(np_CS)) {}

  bool Activeren() override;
};

#endif

#endif // FLASH_H
