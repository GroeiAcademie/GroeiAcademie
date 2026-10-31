#include "AS608.h"
#if SENSOR_AS608_AANTAL > 0
// Officieel: https://adafruit.github.io/Adafruit-Fingerprint-Sensor-Library/html/class_adafruit___fingerprint.html
bool AS608::inpluggen() {
  if (ingeplugd) return true;
  // Native: TX en RX moeten aangesloten zijn. Via een UART-Extender: één channel-claim.
  if (aangemeldePinnen.aantalExclusief == 2 && (exclusief_[0] == static_cast<uint8_t>(HardwareResourcePin::NONE) ||
      exclusief_[0] == static_cast<uint8_t>(HardwareResourcePin::CUSTOM) ||
      exclusief_[1] == static_cast<uint8_t>(HardwareResourcePin::NONE) ||
      exclusief_[1] == static_cast<uint8_t>(HardwareResourcePin::CUSTOM))) return false;
  if (aangemeldePinnen.aantalExclusief == 1 && (parent == nullptr || parent->GetExtenderStream(exclusief_[0]) == nullptr)) return false;
  if (!Sensor::inpluggen()) return false;
  vingerafdruk.begin(57600);
  return true;
}
bool AS608::Activeren() {
  if (!parent || !parent->actief || !ingeplugd) return false;
  // Via een UART-Extender: de baud van deze Sensor (57600) op zijn channel instellen.
  if (aangemeldePinnen.aantalExclusief == 1 && !parent->StartExtenderUART(exclusief_[0], 57600UL)) return false;
  // Wachtwoordcontrole volgt later in de expliciete verificatiefase.
  return true;
}
// Vingerherkenning: echte opgeslagen template-ID, geen aanwezigheid als vervangende waarde.
bool AS608::LeesHoofdwaarde(float& waarde) {
  waarde = NAN;
  if (!actief || !parent) return false;
  if (parent->gedeeldeIdentiteiten != nullptr &&
      parent->gedeeldeIdentiteiten[0] == HardwareResourceIdentiteit::I2C_SDA) {
    HardwareResourceTypeI2C* apparaat = static_cast<HardwareResourceTypeI2C*>(parent);
    if (apparaat->ingeplugd && !apparaat->ComponentBeschikbaarTijdensRuntime()) return false;
  }
  const uint8_t status = vingerafdruk.getImage();
  if (status != FINGERPRINT_OK) return false; // geen vinger of fout is geen match
  if (vingerafdruk.image2Tz(1) != FINGERPRINT_OK) return false;
  if (vingerafdruk.fingerSearch(1) != FINGERPRINT_OK) return false;
  waarde = static_cast<float>(vingerafdruk.fingerID);
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  WriteQueueOrStack(1, waarde);
#endif
  return true;
}
#endif
