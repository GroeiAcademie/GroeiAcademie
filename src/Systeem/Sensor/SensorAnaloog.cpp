#include "SensorAnaloog.h"

bool SensorAnaloog::Activeren() {
  // ADC-contract: bereik 0 controleert alleen de ADC-kanaalondersteuning.
  return parent != nullptr && parent->StartExtenderReadAnalog(sensorPin, 0) == 1;
}

bool SensorAnaloog::StartAnalogRead() {
  laatsteStartresultaat = -1;
  if (metingGestart || parent == nullptr) return false;
  // Gebruik het bestaande herstelpad van de I2C-parent vóór de actief-controle.
  if (parent->gedeeldeIdentiteiten != nullptr &&
      parent->gedeeldeIdentiteiten[0] == HardwareResourceIdentiteit::I2C_SDA) {
    HardwareResourceTypeI2C* i2c = static_cast<HardwareResourceTypeI2C*>(parent);
    if (i2c->ingeplugd && !i2c->ComponentBeschikbaarTijdensRuntime()) return false;
  }
  if (!actief) return false;
  const int8_t gestart = parent->StartExtenderReadAnalog(sensorPin, bereikMillivolt);
  laatsteStartresultaat = gestart;
  if (gestart == 1) { metingGestart = true; return true; }
  if (gestart == -1 && parent->gedeeldeIdentiteiten != nullptr &&
      parent->gedeeldeIdentiteiten[0] == HardwareResourceIdentiteit::I2C_SDA) {
    static_cast<HardwareResourceTypeI2C*>(parent)->ResultaatComponentToegangTijdensRuntimeVerwerken(false);
  }
  return false;
}

bool SensorAnaloog::IsAnalogReadKlaar() {
  return !metingGestart || parent == nullptr || parent->IsExtenderReadAnalogReady();
}

int SensorAnaloog::GetAnalogRead() {
  if (!metingGestart) return 0;
  metingGestart = false;
  if (parent == nullptr) return 0;
  int32_t waarde = 0;
  const bool gelukt = parent->GetExtenderReadAnalogValue(waarde);
  if (parent->gedeeldeIdentiteiten != nullptr &&
      parent->gedeeldeIdentiteiten[0] == HardwareResourceIdentiteit::I2C_SDA) {
    static_cast<HardwareResourceTypeI2C*>(parent)->ResultaatComponentToegangTijdensRuntimeVerwerken(gelukt);
  }
  MeetwaardeVerwerken(waarde, gelukt);
  return gelukt ? static_cast<int>(waarde) : 0;
}

bool SensorAnaloog::afmelden() {
  if (metingGestart) (void)GetAnalogRead();
  return Sensor::afmelden();
}

bool SensorAnaloog::RawAnalogRead(int32_t& waarde) {
  waarde = 0;
  const unsigned long begin = millis();
  // Zelfde blokkerende wachtpatroon als FsrRfp: tijdelijk bezet of referentie nog niet klaar.
  bool gestart = StartAnalogRead();
  while (!gestart && laatsteStartresultaat == 0 && millis() - begin <= 150) {
    yield();
    gestart = StartAnalogRead();
  }
  if (!metingGestart) return false;
  while (!IsAnalogReadKlaar() && millis() - begin <= 150) yield();
  const bool klaar = IsAnalogReadKlaar();
  // De expliciet blokkerende lezing haalt de eigen conversie op en bewaart
  // het werkelijke succesresultaat, ook wanneer de ADC nul teruggeeft.
  if (!metingGestart || parent == nullptr) return false;
  metingGestart = false;
  const bool gelukt = parent->GetExtenderReadAnalogValue(waarde);
  if (parent->gedeeldeIdentiteiten != nullptr &&
      parent->gedeeldeIdentiteiten[0] == HardwareResourceIdentiteit::I2C_SDA) {
    static_cast<HardwareResourceTypeI2C*>(parent)->ResultaatComponentToegangTijdensRuntimeVerwerken(gelukt);
  }
  MeetwaardeVerwerken(waarde, klaar && gelukt);
  if (!klaar || !gelukt) { waarde = 0; return false; }
  return true;
}

void SensorAnaloog::MeetwaardeVerwerken(int32_t waarde, bool gelukt) {
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  if (gelukt) WriteQueueOrStack(1, static_cast<int>(waarde));
#else
  (void)waarde; (void)gelukt;
#endif
}
