#include "AHT10.h"
#if SENSOR_AHT10_AANTAL > 0
bool AHT10::Activeren() {
  if (!parent || !parent->actief) return false;
  HardwareResourceTypeI2C* const apparaat = static_cast<HardwareResourceTypeI2C*>(parent);
  // Officiële begin()-API, geen fictieve hardwarepresentie.
  return aht.begin(&Wire, 0, apparaat->adres);
}

// Runtime-metingen gebruiken de bestaande I2C-toegangspoort, ook achter TCA9548A.
bool AHT10::LeesHoofdwaarde(float& waarde) {
  waarde = NAN;
  if (parent == nullptr || !actief) return false;
  HardwareResourceTypeI2C* apparaat = static_cast<HardwareResourceTypeI2C*>(parent);
  if (apparaat->ingeplugd && !apparaat->ComponentBeschikbaarTijdensRuntime()) return false;
  // Adafruit_AHTX0::getEvent() wacht zonder timeout zolang BUSY staat. Antwoordt de chip niet, dan leest getStatus() 0xFF
  // (BUSY aan) en zou getEvent() blijven hangen: dan geen meting starten. Valt de bus uit tijdens getEvent(), dan helpt dit niet.
  if (aht.getStatus() == 0xFF) { apparaat->ResultaatComponentToegangTijdensRuntimeVerwerken(false); return false; }
  sensors_event_t vocht, temperatuur;
  const bool gelukt = aht.getEvent(&vocht, &temperatuur);
  if (gelukt) waarde = temperatuur.temperature;
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  if (gelukt) {
    WriteQueueOrStack(1, temperatuur.temperature);
    WriteQueueOrStack(2, vocht.relative_humidity);
  }
#endif
  apparaat->ResultaatComponentToegangTijdensRuntimeVerwerken(gelukt);
  return gelukt;
}
#endif
