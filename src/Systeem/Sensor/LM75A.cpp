#include "LM75A.h"
#if SENSOR_LM75A_AANTAL > 0
// Fabrikant: https://www.ti.com/lit/ds/symlink/lm75a.pdf
bool LM75A::Activeren() {
#if GEDEELDE_BUS_MIXED_PINS
  // Mixed: hulppinnen op de tweede Extender instellen vóór de library-initialisatie.
  if (viaMixed) {
    if (extenderHulp_[0] == nullptr || !extenderHulp_[0]->actief) return false;
    if (!extenderHulp_[0]->SetExtenderPinMode(np_OS, INPUT)) return false;   // OS: uitgang van de sensor
  }
#endif
  if (!parent || !parent->actief) return false;
  lm75a.begin();
  return lm75a.isConnected();
}
bool LM75A::LeesHoofdwaarde(float& waarde) {
  waarde = NAN;
  if (!parent || !actief) return false;
  HardwareResourceTypeI2C* apparaat = static_cast<HardwareResourceTypeI2C*>(parent);
  if (apparaat->ingeplugd && !apparaat->ComponentBeschikbaarTijdensRuntime()) return false;
  waarde = lm75a.getTemperature();
  const bool gelukt = waarde != LM75A_INVALID_TEMPERATURE && !isnan(waarde);
  apparaat->ResultaatComponentToegangTijdensRuntimeVerwerken(gelukt);
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  if (gelukt) WriteQueueOrStack(1, waarde);
#endif
  return gelukt;
}
#endif
