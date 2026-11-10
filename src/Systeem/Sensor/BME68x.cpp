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
  metingGestart = false;   // bij herstel geen oude FrameWork-meetstatus behouden
  return bme68x.begin(apparaat->adres);
}


// Runtime-metingen gebruiken de bestaande I2C-toegangspoort, ook achter TCA9548A.
bool BME68x::LeesHoofdwaarde(float& waarde) {
  waarde = NAN;
  if (parent == nullptr || !actief) return false;
  HardwareResourceTypeI2C* apparaat = static_cast<HardwareResourceTypeI2C*>(parent);
  if (apparaat->ingeplugd && !apparaat->ComponentBeschikbaarTijdensRuntime()) return false;
  // De meting starten en pas na de meetduur uitlezen, zonder de expliciete wachtdelay van performReading().
  if (!metingGestart) {
    if (bme68x.remainingReadingMillis() > 0) return false;   // een nog lopende librarymeting van voor herstel

    if (bme68x.remainingReadingMillis() == 0) (void)bme68x.endReading();   // oude meetgegevens niet doorgeven

    metingGestart = bme68x.beginReading() != 0;
    if (!metingGestart) apparaat->ResultaatComponentToegangTijdensRuntimeVerwerken(false);
    return false;
  }

  if (bme68x.remainingReadingMillis() > 0) return false;   // nieuwe meting loopt nog

  metingGestart = false;
  const bool gelukt = bme68x.endReading();
  if (gelukt) waarde = bme68x.temperature;
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  if (gelukt) {
    WriteQueueOrStack(1, bme68x.temperature);
    WriteQueueOrStack(2, bme68x.pressure);
    WriteQueueOrStack(3, bme68x.humidity);
    if (bme68x.gas_resistance != 0) WriteQueueOrStack(4, static_cast<uint32_t>(bme68x.gas_resistance));   // gaswaarde 0 is niet bruikbaar
  }
#endif
  apparaat->ResultaatComponentToegangTijdensRuntimeVerwerken(gelukt);
  return gelukt;
}
#endif
