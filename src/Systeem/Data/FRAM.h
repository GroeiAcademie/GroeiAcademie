#ifndef FRAM_H
#define FRAM_H

#include "../GedeeldeBus/GedeeldeBus.h"

// ============================================================================
// FRAM I2C
// ============================================================================
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_FRAM_I2C)

  #include <FRAM.h>

struct DataFRAMviaI2C : HardwareResourceTypeI2C, FRAM {
  DataFRAMviaI2C(GedeeldeBusNode* parent, uint8_t adres, HardwareResourcePin np_SDA, HardwareResourcePin np_SCL, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : HardwareResourceTypeI2C(parent, GedeeldeBusComponent::DATA_FRAM, adres, np_SDA, np_SCL, Extender), FRAM(&Wire) {}

  ~DataFRAMviaI2C() override;   // wist de eigen plaats in de lijst van Data

  bool Activeren() override;
};

#endif

#endif // FRAM_H
