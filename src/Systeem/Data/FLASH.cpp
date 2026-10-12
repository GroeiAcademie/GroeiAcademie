#include "FLASH.h"

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_FLASH)

bool DataFLASH::Activeren() {
  return SPIFlash::begin();
}

#endif
