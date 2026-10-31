#include "NeuroSkyTGAM.h"
#if SENSOR_NEUROSKYTGAM_AANTAL > 0
// Fabrikant: https://developer.neurosky.com/docs/doku.php?id=thinkgear_communications_protocol
// Geen UART-herinitialisatie: caller initialiseert de aangeboden Stream met modulebaudrate.
bool NeuroSkyTGAM::inpluggen() {
  if (ingeplugd) return true;
  // Native: TX en RX moeten aangesloten zijn. Via een UART-Extender: één channel-claim.
  if (aangemeldePinnen.aantalExclusief == 2 && (exclusief_[0] == static_cast<uint8_t>(HardwareResourcePin::NONE) ||
      exclusief_[0] == static_cast<uint8_t>(HardwareResourcePin::CUSTOM) ||
      exclusief_[1] == static_cast<uint8_t>(HardwareResourcePin::NONE) ||
      exclusief_[1] == static_cast<uint8_t>(HardwareResourcePin::CUSTOM))) return false;
  if (poort == nullptr) return false;
  return Sensor::inpluggen();
}
bool NeuroSkyTGAM::Activeren() {
  // Alleen interfacecontrole; een geldig frame moet bij het uitlezen worden gecontroleerd.
  if (parent == nullptr || !parent->actief || !ingeplugd) return false;
  // Via een UART-Extender: de baud van deze Sensor (57600) op zijn channel instellen.
  return aangemeldePinnen.aantalExclusief != 1 || parent->StartExtenderUART(exclusief_[0], 57600UL);
}

// Een byte uit een bestaande Stream is geen geinterpreteerde sensormeting.
bool NeuroSkyTGAM::LeesOntvangenByte(uint8_t& waarde) {
  waarde = 0;
  if (!actief || !parent) return false;
  if (parent->gedeeldeIdentiteiten != nullptr &&
      parent->gedeeldeIdentiteiten[0] == HardwareResourceIdentiteit::I2C_SDA) {
    HardwareResourceTypeI2C* apparaat = static_cast<HardwareResourceTypeI2C*>(parent);
    if (apparaat->ingeplugd && !apparaat->ComponentBeschikbaarTijdensRuntime()) return false;
  }
  if (poort->available() <= 0) return false;
  const int ontvangen = poort->read();
  if (ontvangen < 0) return false;
  waarde = static_cast<uint8_t>(ontvangen);
  return true;
}

// Officieel ThinkGear-pakket: AA AA, lengte 0..169, payload, ~sum(payload).
// Lees enkel de single-byte CODE 0x04 met EXCODE-level 0 na checksumvalidatie.
bool NeuroSkyTGAM::LeesHoofdwaarde(float& waarde) {
  waarde = NAN;
  if (!actief || !parent) return false;
  if (parent->gedeeldeIdentiteiten &&
      parent->gedeeldeIdentiteiten[0] == HardwareResourceIdentiteit::I2C_SDA) {
    HardwareResourceTypeI2C* i2c = static_cast<HardwareResourceTypeI2C*>(parent);
    if (i2c->ingeplugd && !i2c->ComponentBeschikbaarTijdensRuntime()) return false;
  }
  uint8_t maximaal = 96;
  while (maximaal-- && poort->available() > 0) {
    const int ontvangen = poort->read();
    if (ontvangen < 0) break;
    const uint8_t b = static_cast<uint8_t>(ontvangen);
    if (fasePakket == 0) { if (b == 0xAA) fasePakket = 1; continue; }
    if (fasePakket == 1) { fasePakket = b == 0xAA ? 2 : 0; continue; }
    if (fasePakket == 2) {
      if (b == 0xAA) continue;
      if (b > sizeof(pakket)) { fasePakket = 0; continue; }
      pakketLengte = b;
      pakketPositie = 0;
      pakketSom = 0;
      fasePakket = 3;
      continue;
    }
    if (pakketPositie < pakketLengte) {
      pakket[pakketPositie++] = b;
      pakketSom = static_cast<uint8_t>(pakketSom + b);
      continue;
    }
    fasePakket = 0;
    if (b != static_cast<uint8_t>(~pakketSom)) continue;
    // Attention alleen bij goed contact: poor signal (0x02) moet 0 zijn in hetzelfde pakket.
    // Zonder contact (poor signal 200) stuurt de TGAM attention 0, wat anders als echte meting zou doorkomen.
    uint8_t signaalKwaliteit = 0;
    waarde = NAN;
    float meditatie = NAN;
    uint32_t banden[8] = {};
    bool bandenGevonden = false;
    uint8_t i = 0;
    while (i < pakketLengte) {
      uint8_t extended = 0;
      while (i < pakketLengte && pakket[i] == 0x55) { ++extended; ++i; }
      if (i >= pakketLengte) break;
      const uint8_t code = pakket[i++];
      if (code < 0x80) {
        if (i >= pakketLengte) break;
        const uint8_t v = pakket[i++];
        if (extended == 0 && code == 0x02) signaalKwaliteit = v;
        if (extended == 0 && code == 0x04 && v <= 100) waarde = static_cast<float>(v);
        if (extended == 0 && code == 0x05 && v <= 100) meditatie = static_cast<float>(v);
      } else {
        if (i >= pakketLengte) break;
        const uint8_t lengte = pakket[i++];
        if (lengte > pakketLengte - i) break;
        // ASIC_EEG_POWER: 8 waarden van 3 bytes, big endian, zonder eenheid.
        if (extended == 0 && code == 0x83 && lengte == 24) {
          for (uint8_t b = 0; b < 8; ++b)
            banden[b] = (static_cast<uint32_t>(pakket[i + 3 * b]) << 16) | (static_cast<uint32_t>(pakket[i + 3 * b + 1]) << 8) | pakket[i + 3 * b + 2];
          bandenGevonden = true;
        }
        i = static_cast<uint8_t>(i + lengte);
      }
    }
    if (signaalKwaliteit == 0) {
      if (!isnan(meditatie)) {
        laatsteMeditatie = meditatie;
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
        WriteQueueOrStack(2, meditatie);
#endif
      }
      if (bandenGevonden) {
        memcpy(laatsteEEGBanden, banden, sizeof(banden));
        eegBandenGeldig = true;
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
        for (uint8_t b = 0; b < 8; ++b) WriteQueueOrStack(static_cast<uint8_t>(3 + b), banden[b]);
#endif
      }
    }
    if (!isnan(waarde) && signaalKwaliteit == 0) {
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
      WriteQueueOrStack(1, waarde);
#endif
      return true;
    }
    waarde = NAN;
  }
  return false;
}
// Laatste geldige meditation (poor signal 0); wordt bijgewerkt door LeesHoofdwaarde().
bool NeuroSkyTGAM::LeesMeditatie(float& waarde) {
  waarde = laatsteMeditatie;
  return actief && !isnan(waarde);
}
// Laatste geldige 8 EEG-banden (poor signal 0); worden bijgewerkt door LeesHoofdwaarde().
bool NeuroSkyTGAM::LeesEEGBanden(uint32_t banden[8]) {
  if (!actief || !eegBandenGeldig) return false;
  memcpy(banden, laatsteEEGBanden, sizeof(laatsteEEGBanden));
  return true;
}
#endif
