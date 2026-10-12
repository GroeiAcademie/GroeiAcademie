#include "microSD.h"

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD) || REDIRECT_SERIAL == REDIRECT_SERIAL_TO_MICROSD

bool microSD::Activeren() {
  return SDFileSystemClass::begin(NativeArduinoPinVan(np_CS));
}

#endif
