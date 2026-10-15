#ifndef FRAM_I2C_H
#define FRAM_I2C_H

#include "../GedeeldeBus/GedeeldeBus.h"

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_FRAM)

  #include <FRAM.h>

struct FRAM_I2C : HardwareResourceTypeI2C, FRAM {
  FRAM_I2C(GedeeldeBusNode* parent, uint8_t adres, HardwareResourcePin np_SDA, HardwareResourcePin np_SCL, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : HardwareResourceTypeI2C(parent, GedeeldeBusComponent::DATA_FRAM, adres, np_SDA, np_SCL, Extender), FRAM(&Wire) {}

  bool Activeren() override;
};

#endif

#endif // FRAM_I2C_H
