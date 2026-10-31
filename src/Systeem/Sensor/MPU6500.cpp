#include "MPU6500.h"
#if SENSOR_MPU6500_AANTAL > 0
// Fabrikant: https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6500-Register-Map2.pdf
bool MPU6500::Activeren() {
  return parent && parent->actief && mpu6500.init();
}
bool MPU6500::LeesHoofdwaarde(float& waarde) {
  waarde = NAN;
  if (!parent || !actief) return false;
  HardwareResourceTypeI2C* apparaat = static_cast<HardwareResourceTypeI2C*>(parent);
  if (apparaat->ingeplugd && !apparaat->ComponentBeschikbaarTijdensRuntime()) return false;
  waarde = mpu6500.getTemperature();
  // getTemperature() rekent zonder foutcontrole en geeft nooit NaN: na het lezen controleren of de chip nog antwoordt.
  Wire.beginTransmission(apparaat->adres);
  const bool gelukt = Wire.endTransmission() == 0 && !isnan(waarde);
  if (!gelukt) waarde = NAN;
  apparaat->ResultaatComponentToegangTijdensRuntimeVerwerken(gelukt);
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  if (gelukt) WriteQueueOrStack(1, waarde);
#endif
  return gelukt;
}
#endif
