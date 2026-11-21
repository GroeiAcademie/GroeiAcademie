#ifndef SENSOR_NEUROSKYTGAM_H
#define SENSOR_NEUROSKYTGAM_H

#include <Arduino.h>
#include "../../Configuratie/SystemConfig.h"

#if SENSOR_NEUROSKYTGAM_AANTAL > 0
#include "../GedeeldeBus/GedeeldeBus.h"
#include "Functies/EEGFrequencyBands.h"

struct NeuroSkyTGAM : Sensor {
  Stream* poort;
  // De sketch verklaart deze TX/RX-pinnen; de claim controleert niet de werkelijke poortconfiguratie.
  uint8_t exclusief_[2];
  NeuroSkyTGAM(GedeeldeBusNode* parent, GedeeldeBusComponent component, Stream& serielePoort, HardwareResourcePin np_TX, HardwareResourcePin np_RX)
    : Sensor(parent, {nullptr, 0, exclusief_, 2}, component, HardwareResourceToegang::EXCLUSIEF), poort(&serielePoort) { exclusief_[0] = static_cast<uint8_t>(np_TX); exclusief_[1] = static_cast<uint8_t>(np_RX); }
  // Via een UART-Extender (SC16IS752, MAX14830): de Sensor haalt de Stream van zijn channel bij zijn parent.
  NeuroSkyTGAM(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t channel)
    : Sensor(parent, {nullptr, 0, exclusief_, 1}, component, HardwareResourceToegang::EXCLUSIEF),
      poort(parent != nullptr ? parent->GetExtenderStream(channel) : nullptr) { exclusief_[0] = channel; }
  bool inpluggen() override;
  uint8_t AantalChannels() const override { return 15; }   // attention, meditation, 8 EEG-banden, 5 banden uit de ruwe golf   // meetkanalen voor Data
  bool Activeren() override;
  bool LeesOntvangenByte(uint8_t& waarde);
  bool LeesHoofdwaarde(float& waarde); // gevalideerde eSense Attention 0..100
  uint8_t pakket[169] = {0};
  uint8_t fasePakket = 0;
  uint8_t pakketLengte = 0;
  uint8_t pakketPositie = 0;
  uint8_t pakketSom = 0;
  // Laatst ontvangen signaalkwaliteit (poor signal, code 0x02); 200 = geen contact, ook bij de start.
  uint8_t lastSignalQuality = 200;
  uint32_t lastSignalQualityMS = 0;          // tijdstip (millis) van de laatst ontvangen signaalkwaliteit
  // Alleen bij poor signal 0 (zoals de library Brain ze leest).
  float laatsteMeditatie = NAN;              // eSense Meditation 0..100 (code 0x05)
  uint32_t lastMeditationMS = 0;             // tijdstip (millis) van de laatste geldige Meditation
  uint32_t laatsteEEGBanden[8] = {};         // ASIC_EEG_POWER (code 0x83): delta, theta, low alpha, high alpha, low beta, high beta, low gamma, mid gamma
  bool eegBandenGeldig = false;
  uint32_t lastEEGbandsMS = 0;               // tijdstip (millis) van de laatste geldige 8 EEG-banden
  bool LeesMeditatie(float& waarde);
  bool LeesEEGBanden(uint32_t banden[8]);
  // Ruwe EEG-golf (code 0x80, 512 samples per seconde, 16 bit, zonder eenheid), alleen in de 57600-baudmodus van de TGAM.
  // LeesHoofdwaarde() verwerkt ook deze pakketten: roep ze vaak genoeg aan (ongeveer 4 kB per seconde).
  int16_t lastRawEEGsample = 0;
  bool rawEEGsampleValid = false;
  uint32_t lastRawEEGsampleMS = 0;           // tijdstip (millis) van de laatste ruwe EEG-sample
  EEGfrequencyBands eegFrequencyBands{512};
  bool ReadRawEEGsample(int16_t& waarde);
  // Vermogen van de EEG-banden uit de ruwe golf, met dezelfde functie als de BioAmpEXGPill (eerst gemiddeld tot 256 Hz, venster 256 samples = 1 s, 1 Hz per bin).
  bool ReadEEGbandPowersFromRawWave(float& delta, float& theta, float& alpha, float& beta, float& gamma);
};

#endif

#endif
