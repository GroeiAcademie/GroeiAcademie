#include "FRAM_I2C.h"

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_FRAM)

bool FRAM_I2C::Activeren() {
  const int fout = FRAM::begin(adres);
  const bool gelukt = fout == FRAM_OK;
  return FoutDiagnoseI2C(gelukt, fout, gelukt && FRAM::isConnected());
}

#endif
