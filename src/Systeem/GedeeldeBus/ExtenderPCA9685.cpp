#include "../../Configuratie/SystemConfig.h"

#if EXTENDER_PCA9685_AANTAL >= 2
  #include "GedeeldeBus.h"

constexpr ExtenderPCA9685::Pca9685Pinnen ExtenderPCA9685::ExtenderLijst[];
#endif
