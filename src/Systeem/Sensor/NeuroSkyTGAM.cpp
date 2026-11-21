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
  // Nieuwe sessie: niets uit een vorige sessie blijft geldig (kwaliteit, Meditation, banden, ruwe golf, parser).
  lastSignalQuality = 200;
  lastSignalQualityMS = 0;
  laatsteMeditatie = NAN;
  eegBandenGeldig = false;
  rawEEGsampleValid = false;
  eegFrequencyBands.sampleCount = 0;
  eegFrequencyBands.decimationSum = 0;
  eegFrequencyBands.decimationCount = 0;
  fasePakket = 0;
  pakketPositie = 0;
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
    // Attention alleen bij goed contact: de laatst ontvangen poor signal (0x02) moet 0 zijn en hoogstens 3 s oud.
    // Zonder contact (poor signal 200) stuurt de TGAM attention 0, wat anders als echte meting zou doorkomen.
    // Het kwaliteitsveld staat niet in elk pakket; volgens NeuroSky komt het ongeveer één keer per seconde.
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
        if (extended == 0 && code == 0x02) {
          lastSignalQuality = v;
          lastSignalQualityMS = millis();
          // Slecht contact: de bewaarde Meditation, de ASIC-banden en het venster van de ruwe golf vervallen.
          // Na herstel is eerst een nieuwe meting nodig.
          if (v != 0) {
            laatsteMeditatie = NAN;
            eegBandenGeldig = false;
            eegFrequencyBands.sampleCount = 0;
            eegFrequencyBands.decimationSum = 0;
            eegFrequencyBands.decimationCount = 0;
          }
        }
        if (extended == 0 && code == 0x04 && v >= 1 && v <= 100) waarde = static_cast<float>(v);   // 0: geen betrouwbare score
        if (extended == 0 && code == 0x05 && v >= 1 && v <= 100) meditatie = static_cast<float>(v);
      } else {
        if (i >= pakketLengte) break;
        const uint8_t lengte = pakket[i++];
        if (lengte > pakketLengte - i) break;
        // RAW_WAVE: één 16-bit waarde, big endian (geen poor signal-veld in dit pakket).
        if (extended == 0 && code == 0x80 && lengte == 2) {
          // Een onderbreking langer dan 1 s: eerst een nieuw venster, zodat oude en nieuwe samples niet mengen.
          if (rawEEGsampleValid && millis() - lastRawEEGsampleMS > 1000UL) {
            eegFrequencyBands.sampleCount = 0;
            eegFrequencyBands.decimationSum = 0;
            eegFrequencyBands.decimationCount = 0;
          }
          lastRawEEGsampleMS = millis();
          lastRawEEGsample = static_cast<int16_t>((static_cast<uint16_t>(pakket[i]) << 8) | pakket[i + 1]);
          rawEEGsampleValid = true;
          // Alleen bij goed contact in het venster: de laatst ontvangen kwaliteit is 0 en hoogstens 3 s oud.
          if (lastSignalQuality == 0 && millis() - lastSignalQualityMS <= 3000UL) eegFrequencyBands.AddSample(static_cast<float>(lastRawEEGsample));
        }
        // ASIC_EEG_POWER: 8 waarden van 3 bytes, big endian, zonder eenheid.
        if (extended == 0 && code == 0x83 && lengte == 24) {
          for (uint8_t b = 0; b < 8; ++b)
            banden[b] = (static_cast<uint32_t>(pakket[i + 3 * b]) << 16) | (static_cast<uint32_t>(pakket[i + 3 * b + 1]) << 8) | pakket[i + 3 * b + 2];
          bandenGevonden = true;
        }
        i = static_cast<uint8_t>(i + lengte);
      }
    }
    // Effectieve kwaliteit: de laatst ontvangen waarde, of 200 (geen contact) als die ouder is dan 3 s.
    const uint8_t signaalKwaliteit = (millis() - lastSignalQualityMS <= 3000UL) ? lastSignalQuality : 200;
    if (signaalKwaliteit == 0) {
      if (!isnan(meditatie)) {
        laatsteMeditatie = meditatie;
        lastMeditationMS = millis();
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
        WriteQueueOrStack(2, meditatie);
#endif
      }
      if (bandenGevonden) {
        memcpy(laatsteEEGBanden, banden, sizeof(banden));
        eegBandenGeldig = true;
        lastEEGbandsMS = millis();
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
// Ouder dan 3 s: false, want dan is er geen actuele waarde.
// Ook false als de laatst ontvangen signaalkwaliteit niet 0 is of ouder dan 3 s.
// Bij false is waarde NAN, zoals bij LeesHoofdwaarde().
bool NeuroSkyTGAM::LeesMeditatie(float& waarde) {
  if (!actief || isnan(laatsteMeditatie) || millis() - lastMeditationMS > 3000UL || lastSignalQuality != 0 || millis() - lastSignalQualityMS > 3000UL) { waarde = NAN; return false; }
  waarde = laatsteMeditatie;
  return true;
}
// Laatste geldige 8 EEG-banden (poor signal 0); worden bijgewerkt door LeesHoofdwaarde().
// Ouder dan 3 s: false, want dan zijn er geen actuele banden.
bool NeuroSkyTGAM::LeesEEGBanden(uint32_t banden[8]) {
  // Ook false als de laatst ontvangen signaalkwaliteit niet 0 is of ouder dan 3 s.
  if (!actief || !eegBandenGeldig || millis() - lastEEGbandsMS > 3000UL || lastSignalQuality != 0 || millis() - lastSignalQualityMS > 3000UL) return false;
  memcpy(banden, laatsteEEGBanden, sizeof(laatsteEEGBanden));
  return true;
}
// Laatste ruwe EEG-sample (code 0x80); wordt bijgewerkt door LeesHoofdwaarde().
// Ouder dan 1 s: false, want dan is er geen actuele sample.
bool NeuroSkyTGAM::ReadRawEEGsample(int16_t& waarde) {
  waarde = lastRawEEGsample;
  return actief && rawEEGsampleValid && millis() - lastRawEEGsampleMS <= 1000UL;
}
// Vermogen van de EEG-banden over de laatste seconde ruwe samples (512 samples, gemiddeld tot 256).
// Geen ruwe sample in de laatste seconde: false.
// Ook false als de laatst ontvangen signaalkwaliteit niet 0 is of ouder dan 3 s: zonder contact is de ruwe golf ruis.
bool NeuroSkyTGAM::ReadEEGbandPowersFromRawWave(float& delta, float& theta, float& alpha, float& beta, float& gamma) {
  if (!actief || !rawEEGsampleValid || millis() - lastRawEEGsampleMS > 1000UL || lastSignalQuality != 0 || millis() - lastSignalQualityMS > 3000UL) { delta = theta = alpha = beta = gamma = NAN; return false; }
  const bool succeeded = eegFrequencyBands.ReadBandPowers(delta, theta, alpha, beta, gamma);
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  // Geslaagde berekening: de vijf banden naar Data (kanaal 11 tot en met 15).
  if (succeeded) { WriteQueueOrStack(11, delta); WriteQueueOrStack(12, theta); WriteQueueOrStack(13, alpha); WriteQueueOrStack(14, beta); WriteQueueOrStack(15, gamma); }
#endif
  return succeeded;
}
#endif
