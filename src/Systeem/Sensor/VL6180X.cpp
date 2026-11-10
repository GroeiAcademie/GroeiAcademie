#include "VL6180X.h"
#if SENSOR_VL6180X_AANTAL > 0
bool SensorVL6180X::Activeren() {
  if (!parent || !parent->actief) return false;
  // Hardware-aanmelding via officiële library. Geen fictieve successtatus.
  metingGestart = false;
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
    const bool registeradresVerzonden = Wire.endTransmission(false) == 0;
    const bool gereedstatusOntvangen = registeradresVerzonden && Wire.requestFrom(apparaat->adres, static_cast<uint8_t>(1)) == 1;
    const uint8_t gereedstatus = gereedstatusOntvangen ? static_cast<uint8_t>(Wire.read()) : 0;
    if (!gereedstatusOntvangen) { apparaat->ResultaatComponentToegangTijdensRuntimeVerwerken(false); return false; }
    if (!(gereedstatus & 0x01)) return false;   // nog niet klaar: later opnieuw

    // Het officiële single-shot-startregister schrijven zonder de onbegrensde wachtlus van startRange().
    Wire.beginTransmission(apparaat->adres);
    Wire.write(static_cast<uint8_t>(0x00));
    Wire.write(static_cast<uint8_t>(0x18));
    Wire.write(static_cast<uint8_t>(0x01));
    metingGestart = Wire.endTransmission() == 0;
    tijdstipMetingGestart = millis();
    if (!metingGestart) apparaat->ResultaatComponentToegangTijdensRuntimeVerwerken(false);
    return false;
  }

  if (!vl.isRangeComplete()) {   // één leesactie, geen wachtlus
    if (millis() - tijdstipMetingGestart < 200UL) return false;

    metingGestart = false;
    apparaat->ResultaatComponentToegangTijdensRuntimeVerwerken(false);
    return false;
  }

  metingGestart = false;
  // Elke registerselectie wordt afgesloten, ook wanneer een voorafgaande lezing mislukt.
  Wire.beginTransmission(apparaat->adres);
  Wire.write(static_cast<uint8_t>(0x00));
  Wire.write(static_cast<uint8_t>(0x62));   // RESULT__RANGE_VAL
  bool registeradresVerzonden = Wire.endTransmission(false) == 0;
  const bool afstandOntvangen = registeradresVerzonden && Wire.requestFrom(apparaat->adres, static_cast<uint8_t>(1)) == 1;
  const uint8_t gemetenAfstand = afstandOntvangen ? static_cast<uint8_t>(Wire.read()) : 0;

  Wire.beginTransmission(apparaat->adres);
  Wire.write(static_cast<uint8_t>(0x00));
  Wire.write(static_cast<uint8_t>(0x4D));   // RESULT__RANGE_STATUS
  registeradresVerzonden = Wire.endTransmission(false) == 0;
  const bool afstandStatusOntvangen = registeradresVerzonden && Wire.requestFrom(apparaat->adres, static_cast<uint8_t>(1)) == 1;
  const uint8_t afstandStatus = afstandStatusOntvangen ? static_cast<uint8_t>(Wire.read()) : 0;

  Wire.beginTransmission(apparaat->adres);
  Wire.write(static_cast<uint8_t>(0x00));
  Wire.write(static_cast<uint8_t>(0x15));   // SYSTEM__INTERRUPT_CLEAR
  Wire.write(static_cast<uint8_t>(0x07));
  const bool interruptWiscommandoVerzonden = Wire.endTransmission() == 0;
  const bool gelukt = afstandOntvangen && afstandStatusOntvangen && interruptWiscommandoVerzonden && (afstandStatus >> 4) == VL6180X_ERROR_NONE;
  if (gelukt) waarde = static_cast<float>(gemetenAfstand);
  apparaat->ResultaatComponentToegangTijdensRuntimeVerwerken(gelukt);
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  if (gelukt) WriteQueueOrStack(1, waarde);
#endif
  return gelukt;
}
#endif
