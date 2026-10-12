#include "EEPROM.h"

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_EEPROM)

bool DataEEPROM::Activeren() {
  return I2C_eeprom::begin() && I2C_eeprom::isConnected();
}

#endif
