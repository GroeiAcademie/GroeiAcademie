#ifndef EXTENDERI2C_EEPROM_H
#define EXTENDERI2C_EEPROM_H

struct ExtenderI2C_EEPROM : HardwareResourceTypeI2C, I2C_eeprom {
  // ============================================================================
  // DEFAULT: I2C_EEPROM #1
  // ============================================================================
  #if EXTENDER_I2C_EEPROM_AANTAL == 1
  ExtenderI2C_EEPROM(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t adres, HardwareResourcePin np_SDA, HardwareResourcePin np_SCL, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : HardwareResourceTypeI2C(parent, component, adres, np_SDA, np_SCL, Extender), I2C_eeprom(adres, &Wire) {}
  #endif

  // ============================================================================
  // EXPERIMENTEEL: I2C_EEPROM #2 en hoger
  // ============================================================================
  #if EXTENDER_I2C_EEPROM_AANTAL >= 2
  struct I2cEepromPinnen { uint8_t adres; };

  #define BUNDEL_EXTENDER(N) { I2C_ADDRESS_EXTENDER_I2C_EEPROM_##N }

  static constexpr I2cEepromPinnen ExtenderLijst[] = {
    BUNDEL_EXTENDER(1),
    BUNDEL_EXTENDER(2)
  };

  #undef BUNDEL_EXTENDER

  ExtenderI2C_EEPROM(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin np_SDA, HardwareResourcePin np_SCL, uint8_t teller, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : HardwareResourceTypeI2C(parent, component, ExtenderLijst[teller].adres, np_SDA, np_SCL, Extender), I2C_eeprom(ExtenderLijst[teller].adres, &Wire) {}
  #endif

  bool Activeren() override {
    return I2C_eeprom::begin() && I2C_eeprom::isConnected();
  }
};

#endif
