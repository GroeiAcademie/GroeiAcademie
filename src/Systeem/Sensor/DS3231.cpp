#include "DS3231.h"
#if SENSOR_DS3231_AANTAL > 0
// Fabrikant: https://www.analog.com/media/en/technical-documentation/data-sheets/DS3231.pdf
// Official API: https://github.com/adafruit/RTClib
#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD_SPI) || REDIRECT_SERIAL == REDIRECT_SERIAL_TO_MICROSD
#include "../Data/Data.h"
#define DS3231_BESTANDSDATUM 1   // alleen in dit bestand
namespace {
  struct DS3231* klokVoorBestandsdatum = nullptr;
  // SdFat roept deze functie op wanneer een bestand gemaakt of gewijzigd wordt.
  // SdFat initialiseert datum en tijd niet: eerst altijd de standaardwaarden van SdFat invullen.
  void BestandsdatumVanDS3231(uint16_t* datum, uint16_t* tijd) {
    *datum = FS_DEFAULT_DATE;
    *tijd = FS_DEFAULT_TIME;
    if (klokVoorBestandsdatum == nullptr || !klokVoorBestandsdatum->actief) return;
    const DateTime nu = klokVoorBestandsdatum->ds3231.now();
    const bool verlorenTijd = klokVoorBestandsdatum->ds3231.lostPower();
    // Dezelfde controle als in LeesDatumTijd(): antwoordt de chip niet, dan blijft de standaarddatum staan.
    Wire.beginTransmission(static_cast<HardwareResourceTypeI2C*>(klokVoorBestandsdatum->parent)->adres);
    if (Wire.endTransmission() != 0) return;
    // Dezelfde geldigheid als in LeesDatumTijd(): een onbetrouwbare klok levert geen bestandsdatum.
    if (!nu.isValid() || nu.year() < 2000 || verlorenTijd) return;
    *datum = FS_DATE(nu.year(), nu.month(), nu.day());
    *tijd = FS_TIME(nu.hour(), nu.minute(), nu.second());
  }
}
#endif
DS3231::~DS3231() {
#if DS3231_BESTANDSDATUM
  if (klokVoorBestandsdatum == this) {
    klokVoorBestandsdatum = nullptr;
    FsDateTime::clearCallback();
  }
#endif
}
bool DS3231::Activeren() {
  if (!parent || !parent->actief) return false;
  if (!ds3231.begin(&Wire)) return false;
#if DS3231_BESTANDSDATUM
  klokVoorBestandsdatum = this;
  FsDateTime::setCallback(BestandsdatumVanDS3231);
#endif
  return true;
}
bool DS3231::LeesDatumTijd(DateTime& datumTijd) {
  if (!parent || !actief) return false;
  HardwareResourceTypeI2C* apparaat = static_cast<HardwareResourceTypeI2C*>(parent);
  if (apparaat->ingeplugd && !apparaat->ComponentBeschikbaarTijdensRuntime()) return false;
  datumTijd = ds3231.now();
  const bool verlorenTijd = ds3231.lostPower();
  // RTClib geeft geen I2C-resultaat terug: na het lezen controleren of de chip nog antwoordt (zoals bij de MPU6500).
  // Dit bewijst dat de chip antwoordt, niet dat de voorafgaande leesactie zelf geslaagd is.
  Wire.beginTransmission(apparaat->adres);
  const bool busGelukt = Wire.endTransmission() == 0;
  apparaat->ResultaatComponentToegangTijdensRuntimeVerwerken(busGelukt);
  // Na batterijverlies of zonder ZetDatumTijd() is de tijd niet betrouwbaar: geen busfout, wel geen geldige tijd.
  const bool gelukt = busGelukt && datumTijd.isValid() && datumTijd.year() >= 2000 && !verlorenTijd;
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  if (gelukt) {
    WriteQueueOrStack(1, static_cast<uint32_t>(datumTijd.unixtime()));   // Unix-tijd
    WriteQueueOrStack(2, ds3231.getTemperature());                          // temperatuur van de chip
  }
#endif
  return gelukt;
}
bool DS3231::ZetDatumTijd(const DateTime& datumTijd) {
  if (!parent || !actief || !datumTijd.isValid()) return false;
  HardwareResourceTypeI2C* apparaat = static_cast<HardwareResourceTypeI2C*>(parent);
  // Zelfde volgorde als LeesDatumTijd(): niet beschikbaar, dan geen adjust().
  if (apparaat->ingeplugd && !apparaat->ComponentBeschikbaarTijdensRuntime()) return false;
  ds3231.adjust(datumTijd);   // wist ook de vlag voor batterijverlies
  // adjust() geeft geen I2C-resultaat terug: daarna controleren of de chip nog antwoordt.
  Wire.beginTransmission(apparaat->adres);
  const bool busGelukt = Wire.endTransmission() == 0;
  apparaat->ResultaatComponentToegangTijdensRuntimeVerwerken(busGelukt);
  return busGelukt;
}
#endif
