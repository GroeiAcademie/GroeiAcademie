#include "Data.h"

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_FRAM_I2C)

DataFRAMviaI2C::~DataFRAMviaI2C() {
  struct Data* data = static_cast<struct Data*>(parent);   // de ouder (Data) van deze chip
  if (data == nullptr) return;
  for (uint8_t teller = 0; teller < DATA_LAYER_FRAM_I2C_AANTAL; teller++) {
    if (data->FRAMviaI2C[teller] == this) data->FRAMviaI2C[teller] = nullptr;
  }
}

bool DataFRAMviaI2C::Activeren() {
  const int fout = FRAM::begin(adres);
  const bool gelukt = fout == FRAM_OK;
  return FoutDiagnoseI2C(gelukt, fout, gelukt && FRAM::isConnected());
}

#endif
