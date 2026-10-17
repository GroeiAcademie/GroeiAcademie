#ifndef EEPROM_H
#define EEPROM_H

#include "../GedeeldeBus/GedeeldeBus.h"

// ============================================================================
// EEPROM NATIVE: de native EEPROM van het board, via EEPROM.h van de core.
// Geen claims. Data krijgt DATA_LAYER_EEPROM_NATIVE_SIZE bytes, vanaf adres HX1838_EEPROM_NATIVE_SIZE.
// De UNO Q heeft geen EEPROM.h in de core: daar wordt dit medium niet gecompileerd en niet aangemaakt.
// ============================================================================
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_EEPROM_NATIVE) && BOARD_VERSION != BOARD_UNO_Q

  #include <EEPROM.h>

struct DataEEPROMviaNative : GedeeldeBusNode {
  DataEEPROMviaNative(GedeeldeBusNode* parent)
    : GedeeldeBusNode(parent, { nullptr, 0, nullptr, 0 }, GedeeldeBusComponent::DATA_EEPROM, HardwareResourceToegang::GEDEELD) {}

  ~DataEEPROMviaNative() override;   // wist de eigen plaats in de lijst van Data

  bool Activeren() override;
};

#endif

// ============================================================================
// EEPROM I2C
// ============================================================================
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_EEPROM_I2C)

  #include <I2C_eeprom.h>

struct DataEEPROMviaI2C : HardwareResourceTypeI2C, I2C_eeprom {
  DataEEPROMviaI2C(GedeeldeBusNode* parent, uint8_t adres, HardwareResourcePin np_SDA, HardwareResourcePin np_SCL, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : HardwareResourceTypeI2C(parent, GedeeldeBusComponent::DATA_EEPROM, adres, np_SDA, np_SCL, Extender), I2C_eeprom(adres, &Wire) {}

  ~DataEEPROMviaI2C() override;   // wist de eigen plaats in de lijst van Data

  bool Activeren() override;
};

#endif

#endif // EEPROM_H
