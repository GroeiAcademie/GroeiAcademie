#include "BMP280.h"
#if SENSOR_BMP280_AANTAL > 0

// Officiële referentie voor register en identiteit: https://github.com/adafruit/Adafruit_BMP280_Library/blob/master/Adafruit_BMP280.h
bool BMP280::Activeren() {
  if (!parent || !parent->actief) return false;
  // Geen tweede businitialisatie: gebeurt tijdens inpluggen van de parent.
  HardwareResourceTypeI2C* const apparaat = static_cast<HardwareResourceTypeI2C*>(parent);
  // Officiele library-initialisatie. begin() kan Wire.begin() en wachttijden
  // bevatten; die gelden ook bij runtime-herstel binnen de bestaande lifecycle.
  return bmp280.begin(apparaat->adres);
}


// Runtime-metingen gebruiken de bestaande I2C-toegangspoort, ook achter TCA9548A.
bool BMP280::LeesHoofdwaarde(float& waarde) {
  waarde = NAN;
  if (parent == nullptr || !actief) return false;
  HardwareResourceTypeI2C* apparaat = static_cast<HardwareResourceTypeI2C*>(parent);
  if (apparaat->ingeplugd && !apparaat->ComponentBeschikbaarTijdensRuntime()) return false;
  waarde = bmp280.readTemperature();
  const bool gelukt = !isnan(waarde);
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  if (gelukt) {
    WriteQueueOrStack(1, waarde);                       // temperatuur (°C)
    const float luchtdruk = bmp280.readPressure();     // luchtdruk (Pa), officiele API
    if (!isnan(luchtdruk)) WriteQueueOrStack(2, luchtdruk);   // eigen controle: de druk kan mislukken terwijl de temperatuur lukt
  }
#endif
  apparaat->ResultaatComponentToegangTijdensRuntimeVerwerken(gelukt);
  return gelukt;
}
#endif
