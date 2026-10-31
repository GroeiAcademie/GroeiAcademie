#include "SCD41.h"
#if SENSOR_SCD41_AANTAL > 0
// Officieel voorbeeld: https://github.com/Sensirion/arduino-i2c-scd4x/blob/master/examples/exampleUsage/exampleUsage.ino
bool SCD41::Activeren() {
  if (!parent || !parent->actief) return false;
  HardwareResourceTypeI2C* const apparaat = static_cast<HardwareResourceTypeI2C*>(parent);
  scd4x.begin(Wire, apparaat->adres);
  // Begin meetmodus via officiele API; de eerste CO2-waarde komt pas na de updateperiode.
  return scd4x.startPeriodicMeasurement() == 0;
}

// Runtime-metingen gebruiken de bestaande I2C-toegangspoort, ook achter TCA9548A.
bool SCD41::LeesHoofdwaarde(float& waarde) {
  waarde = NAN;
  if (parent == nullptr || !actief) return false;
  HardwareResourceTypeI2C* apparaat = static_cast<HardwareResourceTypeI2C*>(parent);
  if (apparaat->ingeplugd && !apparaat->ComponentBeschikbaarTijdensRuntime()) return false;
  bool beschikbaar = false;
  const int16_t gereed = scd4x.getDataReadyStatus(beschikbaar);
  if (gereed != 0) {
    apparaat->ResultaatComponentToegangTijdensRuntimeVerwerken(false);
    return false;
  }
  if (!beschikbaar) return false; // geen storing, enkel nog geen nieuwe CO2-meting
  uint16_t koolstofdioxide;
  float temperatuur, vocht;
  const bool gelukt = scd4x.readMeasurement(koolstofdioxide, temperatuur, vocht) == 0;
  if (gelukt) waarde = static_cast<float>(koolstofdioxide); // ppm
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  if (gelukt) {
    WriteQueueOrStack(1, static_cast<uint32_t>(koolstofdioxide));
    WriteQueueOrStack(2, temperatuur);
    WriteQueueOrStack(3, vocht);
  }
#endif
  apparaat->ResultaatComponentToegangTijdensRuntimeVerwerken(gelukt);
  return gelukt;
}
#endif
