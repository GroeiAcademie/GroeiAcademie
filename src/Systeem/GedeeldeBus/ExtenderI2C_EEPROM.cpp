#include "../../Configuratie/SystemConfig.h"

#if EXTENDER_I2C_EEPROM_AANTAL >= 2
  #include "GedeeldeBus.h"

constexpr ExtenderI2C_EEPROM::I2cEepromPinnen ExtenderI2C_EEPROM::ExtenderLijst[];
#endif
