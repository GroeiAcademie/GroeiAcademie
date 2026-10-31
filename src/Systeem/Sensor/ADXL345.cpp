#include "ADXL345.h"
#if SENSOR_ADXL345_AANTAL > 0

// Officiële referentie voor register en identiteit: https://github.com/adafruit/Adafruit_ADXL345/blob/master/Adafruit_ADXL345_U.h
bool SensorADXL345::Activeren() {
  if (!parent || !parent->actief) return false;
  // Geen tweede businitialisatie: gebeurt tijdens inpluggen van de parent.
  HardwareResourceTypeI2C* const apparaat = static_cast<HardwareResourceTypeI2C*>(parent);
  // Officiele library-initialisatie. begin() kan Wire.begin() en wachttijden
  // bevatten; die gelden ook bij runtime-herstel binnen de bestaande lifecycle.
  return adxl345.begin(apparaat->adres);
}


// Runtime-metingen gebruiken de bestaande I2C-toegangspoort, ook achter TCA9548A.
bool SensorADXL345::LeesHoofdwaarde(float& waarde) {
  waarde = NAN;
  if (parent == nullptr || !actief) return false;
  HardwareResourceTypeI2C* apparaat = static_cast<HardwareResourceTypeI2C*>(parent);
  if (apparaat->ingeplugd && !apparaat->ComponentBeschikbaarTijdensRuntime()) return false;
  sensors_event_t gebeurtenis;
  const bool gelukt = adxl345.getEvent(&gebeurtenis);
  if (gelukt) waarde = gebeurtenis.acceleration.x; // m/s², X-as
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  if (gelukt) {
    WriteQueueOrStack(1, gebeurtenis.acceleration.x);
    WriteQueueOrStack(2, gebeurtenis.acceleration.y);
    WriteQueueOrStack(3, gebeurtenis.acceleration.z);
  }
#endif
  apparaat->ResultaatComponentToegangTijdensRuntimeVerwerken(gelukt);
  return gelukt;
}
#endif
