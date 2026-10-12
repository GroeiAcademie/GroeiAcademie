#ifndef EEPROM_H
#define EEPROM_H

#include "../GedeeldeBus/GedeeldeBus.h"

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_EEPROM)

  #include <I2C_eeprom.h>

struct EEPROM : HardwareResourceTypeI2C, I2C_eeprom {
  EEPROM(GedeeldeBusNode* parent, uint8_t adres, HardwareResourcePin np_SDA, HardwareResourcePin np_SCL, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : HardwareResourceTypeI2C(parent, GedeeldeBusComponent::DATA_EEPROM, adres, np_SDA, np_SCL, Extender), I2C_eeprom(adres, &Wire) {}

  bool Activeren() override;
};

#endif

#endif // EEPROM_H
