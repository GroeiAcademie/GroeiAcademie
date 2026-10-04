#include "../../Configuratie/SystemConfig.h"

#if EXTENDER_MAX14830_I2C_AANTAL >= 2
  #include "GedeeldeBus.h"

constexpr ExtenderMAX14830I2C::Max14830I2cPinnen ExtenderMAX14830I2C::ExtenderLijst[];
#endif
