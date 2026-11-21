#include "MPU6500.h"
#if SENSOR_MPU6500_AANTAL > 0
// Fabrikant: https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6500-Register-Map2.pdf
bool MPU6500::Activeren() {
#if GEDEELDE_BUS_MIXED_PINS
  // Mixed: hulppinnen op de tweede Extender instellen vóór de library-initialisatie.
  if (viaMixed) {
    if (extenderHulp_[0] == nullptr || !extenderHulp_[0]->actief) return false;
    if (!extenderHulp_[0]->SetExtenderPinMode(np_INT, INPUT)) return false;   // INT: uitgang van de sensor
  }
#endif
  if (!(parent && parent->actief && mpu6500.init())) return false;
#if GEDEELDE_BUS_MIXED_PINS
  if (viaMixed) {
    HardwareResourceTypeI2C* const apparaat = static_cast<HardwareResourceTypeI2C*>(parent);
    // INT_PIN_CFG 0x37 = 0x30: INT hoog actief, vergrendeld tot een lezing (LATCH_INT_EN, INT_ANYRD_2CLEAR); INT_ENABLE 0x38 = 0x01: data klaar.
    Wire.beginTransmission(apparaat->adres); Wire.write(0x37); Wire.write(0x30);
    if (Wire.endTransmission() != 0) return false;
    Wire.beginTransmission(apparaat->adres); Wire.write(0x38); Wire.write(0x01);
    if (Wire.endTransmission() != 0) return false;
  }
#endif
  return true;
}
bool MPU6500::LeesHoofdwaarde(float& waarde) {
  waarde = NAN;
  if (!parent || !actief) return false;
  HardwareResourceTypeI2C* apparaat = static_cast<HardwareResourceTypeI2C*>(parent);
  if (apparaat->ingeplugd && !apparaat->ComponentBeschikbaarTijdensRuntime()) return false;
  // Zoals de MPU6050: hoofdwaarde = versnelling X (m/s²); de library geeft g en graden per seconde, omgerekend naar m/s² en rad/s.
  const xyzFloat accelerationValues = mpu6500.getGValues();
  const xyzFloat gyroscopeValues = mpu6500.getGyrValues();
  const float temperatuur = mpu6500.getTemperature();
  // De library rekent zonder foutcontrole en geeft nooit NaN: na het lezen controleren of de chip nog antwoordt.
  Wire.beginTransmission(apparaat->adres);
  const bool gelukt = Wire.endTransmission() == 0 && !isnan(temperatuur);
  waarde = gelukt ? accelerationValues.x * 9.80665f : NAN;
  apparaat->ResultaatComponentToegangTijdensRuntimeVerwerken(gelukt);
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  if (gelukt) {
    WriteQueueOrStack(1, accelerationValues.x * 9.80665f);
    WriteQueueOrStack(2, accelerationValues.y * 9.80665f);
    WriteQueueOrStack(3, accelerationValues.z * 9.80665f);
    WriteQueueOrStack(4, static_cast<float>(gyroscopeValues.x * DEG_TO_RAD));
    WriteQueueOrStack(5, static_cast<float>(gyroscopeValues.y * DEG_TO_RAD));
    WriteQueueOrStack(6, static_cast<float>(gyroscopeValues.z * DEG_TO_RAD));
    WriteQueueOrStack(7, temperatuur);
  }
#endif
  return gelukt;
}
#endif
