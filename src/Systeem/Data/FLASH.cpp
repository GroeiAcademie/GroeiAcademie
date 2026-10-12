#include "FLASH.h"

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_FLASH)

bool FLASH::Activeren() {
  return SPIFlash::begin();
}

#endif
