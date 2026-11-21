#include "ADXL345.h"
#if SENSOR_ADXL345_AANTAL > 0

// Officiële referentie voor register en identiteit: https://github.com/adafruit/Adafruit_ADXL345/blob/master/Adafruit_ADXL345_U.h
bool SensorADXL345::Activeren() {
#if GEDEELDE_BUS_MIXED_PINS
  // Mixed: hulppinnen op de tweede Extender instellen vóór de library-initialisatie.
  if (viaMixed) {
    if (extenderHulp_[0] == nullptr || !extenderHulp_[0]->actief) return false;
    if (!extenderHulp_[0]->SetExtenderPinMode(np_INT1, INPUT)) return false;   // INT1: uitgang van de sensor
    if (extenderHulp_[1] == nullptr || !extenderHulp_[1]->actief) return false;
    if (!extenderHulp_[1]->SetExtenderPinMode(np_INT2, INPUT)) return false;   // INT2: uitgang van de sensor
  }
#endif
  if (!parent || !parent->actief) return false;
  // Geen tweede businitialisatie: gebeurt tijdens inpluggen van de parent.
  HardwareResourceTypeI2C* const apparaat = static_cast<HardwareResourceTypeI2C*>(parent);
  // Officiele library-initialisatie. begin() kan Wire.begin() en wachttijden
  // bevatten; die gelden ook bij runtime-herstel binnen de bestaande lifecycle.
  if (!adxl345.begin(apparaat->adres)) return false;
#if GEDEELDE_BUS_MIXED_PINS
  // INT1 = DATA_READY, INT2 = ACTIVITY (beweging). Beide hoog actief.
  // THRESH_ACT 0x24 = 0x10 (16 x 62,5 mg = 1 g); ACT_INACT_CTL 0x27 = 0xF0 (x, y en z, ac-gekoppeld).
  // Ac-gekoppeld: alleen de verandering telt, zodat de zwaartekracht (1 g in rust) geen beweging meldt.
  // INT_MAP 0x2F = 0x10 (ACTIVITY naar INT2, de rest naar INT1); INT_ENABLE 0x2E = 0x90 (DATA_READY en ACTIVITY).
  if (viaMixed) {
    adxl345.writeRegister(0x24, 0x10);
    adxl345.writeRegister(0x27, 0xF0);
    adxl345.writeRegister(0x2F, 0x10);
    adxl345.writeRegister(0x2E, 0x90);
  }
#endif
  return true;
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
