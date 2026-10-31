#include "MPU6050.h"
#if SENSOR_MPU6050_AANTAL > 0

// Officiële referentie voor register en identiteit: https://github.com/adafruit/Adafruit_MPU6050/blob/master/Adafruit_MPU6050.h
bool SensorMPU6050::Activeren() {
  if (!parent || !parent->actief) return false;
  // Geen tweede businitialisatie: gebeurt tijdens inpluggen van de parent.
  HardwareResourceTypeI2C* const apparaat = static_cast<HardwareResourceTypeI2C*>(parent);
  // Officiele library-initialisatie. begin() kan Wire.begin() en wachttijden
  // bevatten; die gelden ook bij runtime-herstel binnen de bestaande lifecycle.
  return mpu6050.begin(apparaat->adres, &Wire);
}


// Runtime-metingen gebruiken de bestaande I2C-toegangspoort, ook achter TCA9548A.
bool SensorMPU6050::LeesHoofdwaarde(float& waarde) {
  waarde = NAN;
  if (parent == nullptr || !actief) return false;
  HardwareResourceTypeI2C* apparaat = static_cast<HardwareResourceTypeI2C*>(parent);
  if (apparaat->ingeplugd && !apparaat->ComponentBeschikbaarTijdensRuntime()) return false;
  sensors_event_t versnelling, gyro, temperatuur;
  const bool gelukt = mpu6050.getEvent(&versnelling, &gyro, &temperatuur);
  if (gelukt) waarde = versnelling.acceleration.x; // m/s², X-as
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  if (gelukt) {
    WriteQueueOrStack(1, versnelling.acceleration.x);
    WriteQueueOrStack(2, versnelling.acceleration.y);
    WriteQueueOrStack(3, versnelling.acceleration.z);
    WriteQueueOrStack(4, gyro.gyro.x);
    WriteQueueOrStack(5, gyro.gyro.y);
    WriteQueueOrStack(6, gyro.gyro.z);
    WriteQueueOrStack(7, temperatuur.temperature);
  }
#endif
  apparaat->ResultaatComponentToegangTijdensRuntimeVerwerken(gelukt);
  return gelukt;
}
#endif
