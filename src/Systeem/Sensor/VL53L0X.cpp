#include "VL53L0X.h"
#if SENSOR_VL53L0X_AANTAL > 0
bool SensorVL53L0X::Activeren() {
  if (!parent || !parent->actief) return false;
  HardwareResourceTypeI2C* const apparaat = static_cast<HardwareResourceTypeI2C*>(parent);
  // Officiële begin()-API, geen fictieve hardwarepresentie.
  return vl.begin(apparaat->adres, false, &Wire);
}

// Runtime-metingen gebruiken de bestaande I2C-toegangspoort, ook achter TCA9548A.
bool SensorVL53L0X::LeesHoofdwaarde(float& waarde) {
  waarde = NAN;
  if (parent == nullptr || !actief) return false;
  HardwareResourceTypeI2C* apparaat = static_cast<HardwareResourceTypeI2C*>(parent);
  if (apparaat->ingeplugd && !apparaat->ComponentBeschikbaarTijdensRuntime()) return false;
  VL53L0X_RangingMeasurementData_t meting;
  const bool gelukt = vl.rangingTest(&meting, false) == VL53L0X_ERROR_NONE &&
                       meting.RangeStatus == 0;
  if (gelukt) waarde = static_cast<float>(meting.RangeMilliMeter);
  apparaat->ResultaatComponentToegangTijdensRuntimeVerwerken(gelukt);
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  if (gelukt) WriteQueueOrStack(1, waarde);
#endif
  return gelukt;
}
#endif
