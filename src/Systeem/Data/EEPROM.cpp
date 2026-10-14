#include "EEPROM.h"

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_EEPROM)

bool DataEEPROM::Activeren() {
  // I2C_eeprom heeft geen functie die een foutcode teruggeeft: fout = 0.
  const bool gelukt = I2C_eeprom::begin() && I2C_eeprom::isConnected();
  return FoutDiagnoseI2C(gelukt, 0, gelukt);
}

#endif
