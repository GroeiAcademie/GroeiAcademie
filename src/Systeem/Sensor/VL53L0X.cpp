#include "VL53L0X.h"
#if SENSOR_VL53L0X_AANTAL > 0
bool SensorVL53L0X::Activeren() {
  if (!parent || !parent->actief) return false;
  HardwareResourceTypeI2C* const apparaat = static_cast<HardwareResourceTypeI2C*>(parent);
  // Officiële begin()-API, geen fictieve hardwarepresentie.
  metingGestart = false;
  tijdslimietMeting = 200UL;
  if (!vl.begin(apparaat->adres, false, &Wire)) return false;

  // De officiële getter kan bij een API-fout een onbepaalde waarde teruggeven: alleen met successtatus gebruiken.
  tijdslimietMeting = vl.getMeasurementTimingBudgetMicroSeconds();
  if (vl.Status == VL53L0X_ERROR_NONE && tijdslimietMeting >= 1000UL && tijdslimietMeting <= 715000000UL) {
    tijdslimietMeting = 6UL * ((tijdslimietMeting + 999UL) / 1000UL);
  } else {
    tijdslimietMeting = 200UL;   // veilige terugval, zonder geslaagde initialisatie ongedaan te maken
  }

  return true;
}

// Runtime-metingen gebruiken de bestaande I2C-toegangspoort, ook achter TCA9548A.
bool SensorVL53L0X::LeesHoofdwaarde(float& waarde) {
  waarde = NAN;
  if (parent == nullptr || !actief) return false;
  HardwareResourceTypeI2C* apparaat = static_cast<HardwareResourceTypeI2C*>(parent);
  if (apparaat->ingeplugd && !apparaat->ComponentBeschikbaarTijdensRuntime()) return false;
  // Niet-blokkerend starten, later pas een gereed meetresultaat ophalen.
  if (!metingGestart) {
    metingGestart = vl.startRange();
    tijdstipMetingGestart = millis();
    if (!metingGestart) apparaat->ResultaatComponentToegangTijdensRuntimeVerwerken(false);
    return false;
  }

  if (!vl.isRangeComplete()) {
    if (millis() - tijdstipMetingGestart < tijdslimietMeting) return false;

    metingGestart = false;
    apparaat->ResultaatComponentToegangTijdensRuntimeVerwerken(false);
    return false;
  }

  metingGestart = false;
  if (vl.Status != VL53L0X_ERROR_NONE) {   // isRangeComplete() geeft ook true bij een API-fout
    apparaat->ResultaatComponentToegangTijdensRuntimeVerwerken(false);
    return false;
  }

  const uint16_t gemetenAfstand = vl.readRangeResult();
  const bool gelukt = vl.Status == VL53L0X_ERROR_NONE && vl.readRangeStatus() == 0 && gemetenAfstand != 0xFFFF;
  if (gelukt) waarde = static_cast<float>(gemetenAfstand);
  apparaat->ResultaatComponentToegangTijdensRuntimeVerwerken(gelukt);
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  if (gelukt) WriteQueueOrStack(1, waarde);
#endif
  return gelukt;
}
#endif
