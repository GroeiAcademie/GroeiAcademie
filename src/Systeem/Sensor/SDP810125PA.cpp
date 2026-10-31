#include "SDP810125PA.h"
#if SENSOR_SDP810125PA_AANTAL > 0
// Sensirion arduino-i2c-sdp: examples/exampleUsage/exampleUsage.ino.
// SDP8x0 adres 0x25, SDP8x1 adres 0x26; foutcodes komen rechtstreeks van de driver.
bool SDP810125PA::Activeren() {
  if (!parent || !parent->actief) return false;
  HardwareResourceTypeI2C* apparaat = static_cast<HardwareResourceTypeI2C*>(parent);
  sdp.begin(Wire, apparaat->adres);
  // Na herstel of heractivering kan een vorige continue meting nog lopen.
  (void)sdp.stopContinuousMeasurement();
  // Verifieer het product zoals in de officiele voorbeeldcode.
  uint32_t productNumber = 0;
  uint8_t serialNumber[8];
  if (sdp.readProductIdentifier(productNumber, serialNumber, sizeof(serialNumber)) != 0) return false;
  if ((productNumber & 0xFFFFFF00UL) != 0x03020B00UL) return false;
  return sdp.startContinuousMeasurementWithDiffPressureTCompAndAveraging() == 0;
}

bool SDP810125PA::LeesHoofdwaarde(float& waarde) {
  waarde = NAN;
  if (!parent || !actief) return false;
  HardwareResourceTypeI2C* apparaat = static_cast<HardwareResourceTypeI2C*>(parent);
  if (apparaat->ingeplugd && !apparaat->ComponentBeschikbaarTijdensRuntime()) return false;
  float temperatuur = NAN;
  const bool gelukt = sdp.readMeasurement(waarde, temperatuur) == 0;
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  if (gelukt) {
    WriteQueueOrStack(1, waarde);          // drukverschil
    WriteQueueOrStack(2, temperatuur);     // temperatuur
  }
#endif
  apparaat->ResultaatComponentToegangTijdensRuntimeVerwerken(gelukt);
  if (!gelukt) waarde = NAN;
  return gelukt;
}
#endif
