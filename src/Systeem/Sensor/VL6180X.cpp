#include "VL6180X.h"
#if SENSOR_VL6180X_AANTAL > 0
bool SensorVL6180X::Activeren() {
  if (!parent || !parent->actief) return false;
  HardwareResourceTypeI2C* const apparaat = static_cast<HardwareResourceTypeI2C*>(parent);
  // Hardware-aanmelding via officiële library. Geen fictieve successtatus.
  return vl.begin(&Wire);
}

// Runtime-metingen gebruiken de bestaande I2C-toegangspoort, ook achter TCA9548A.
bool SensorVL6180X::LeesHoofdwaarde(float& waarde) {
  waarde = NAN;
  if (parent == nullptr || !actief) return false;
  HardwareResourceTypeI2C* apparaat = static_cast<HardwareResourceTypeI2C*>(parent);
  if (apparaat->ingeplugd && !apparaat->ComponentBeschikbaarTijdensRuntime()) return false;
  // readRange() en startRange() van Adafruit_VL6180X wachten zonder timeout op een statusbit; bij een busfout blijft dat hangen.
  // Daarom niet-blokkerend: zelf RESULT__RANGE_STATUS (0x004D) lezen, bit 0 = klaar voor een nieuwe meting.
  if (!metingGestart) {
    Wire.beginTransmission(apparaat->adres);
    Wire.write(static_cast<uint8_t>(0x00));
    Wire.write(static_cast<uint8_t>(0x4D));
    const bool geschreven = Wire.endTransmission(false) == 0;
    const bool gelezen = geschreven && Wire.requestFrom(apparaat->adres, static_cast<uint8_t>(1)) == 1;
    const uint8_t status = gelezen ? static_cast<uint8_t>(Wire.read()) : 0;
    if (!gelezen) { apparaat->ResultaatComponentToegangTijdensRuntimeVerwerken(false); return false; }
    if (!(status & 0x01)) return false;   // nog niet klaar: later opnieuw
    metingGestart = vl.startRange();      // de wachtlus in startRange() eindigt meteen, want bit 0 staat al
    return false;
  }
  if (!vl.isRangeComplete()) return false;   // één leesactie, geen wachtlus
  metingGestart = false;
  const uint8_t afstand = vl.readRangeResult();
  const bool gelukt = vl.readRangeStatus() == VL6180X_ERROR_NONE;
  if (gelukt) waarde = static_cast<float>(afstand);
  apparaat->ResultaatComponentToegangTijdensRuntimeVerwerken(gelukt);
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  if (gelukt) WriteQueueOrStack(1, waarde);
#endif
  return gelukt;
}
#endif
