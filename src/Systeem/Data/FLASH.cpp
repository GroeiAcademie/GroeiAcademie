#include "FLASH.h"

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_FLASH)

bool DataFLASH::Activeren() {
  const bool gelukt = SPIFlash::begin();
  return FoutDiagnoseSPI(gelukt, SPIFlash::error(), gelukt);
}

#endif
