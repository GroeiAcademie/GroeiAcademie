#include "BME68x.h"
#if SENSOR_BME680_AANTAL > 0 || SENSOR_BME688_AANTAL > 0

// Officiële referentie voor register en identiteit: https://github.com/adafruit/Adafruit_CircuitPython_BME680/blob/main/adafruit_bme680.py
// Fabrikant: https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bme688-ds000.pdf
bool BME68x::Activeren() {
  if (!parent || !parent->actief) return false;
  // Geen tweede businitialisatie: gebeurt tijdens inpluggen van de parent.
  HardwareResourceTypeI2C* const apparaat = static_cast<HardwareResourceTypeI2C*>(parent);
  // Officiele library-initialisatie. begin() kan Wire.begin() en wachttijden
  // bevatten; die gelden ook bij runtime-herstel binnen de bestaande lifecycle.
  return bme68x.begin(apparaat->adres);
}


// Runtime-metingen gebruiken de bestaande I2C-toegangspoort, ook achter TCA9548A.
bool BME68x::LeesHoofdwaarde(float& waarde) {
  waarde = NAN;
  if (parent == nullptr || !actief) return false;
  HardwareResourceTypeI2C* apparaat = static_cast<HardwareResourceTypeI2C*>(parent);
  if (apparaat->ingeplugd && !apparaat->ComponentBeschikbaarTijdensRuntime()) return false;
  const bool gelukt = bme68x.performReading();
  if (gelukt) waarde = bme68x.temperature;
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  if (gelukt) {
    WriteQueueOrStack(1, bme68x.temperature);
    WriteQueueOrStack(2, bme68x.pressure);
    WriteQueueOrStack(3, bme68x.humidity);
    WriteQueueOrStack(4, static_cast<uint32_t>(bme68x.gas_resistance));
  }
#endif
  apparaat->ResultaatComponentToegangTijdensRuntimeVerwerken(gelukt);
  return gelukt;
}
#endif
