#ifndef EEGFREQUENCYBANDS_H
#define EEGFREQUENCYBANDS_H

#include <Arduino.h>

// Vermogen van de EEG-banden uit een ruw EEG-signaal (BioAmpEXGPill EEG, ruwe golf van de NeuroSkyTGAM).
// Eén functie voor beide sensoren: een venster van 256 samples, DFT per frequentiebin, vermogen opgeteld per band.
// Banden: delta 0,5-4 Hz, theta 4-8 Hz, alfa 8-13 Hz, bèta 13-30 Hz, gamma 30-45 Hz (begrensd door de helft van de samplefrequentie).
// Boven 256 Hz wordt eerst gemiddeld tot 256 Hz (512 Hz: telkens 2 samples samen).
// Resolutie: 1 Hz per bin bij 256 en 512 Hz; delta telt zo de bins van 1, 2 en 3 Hz.
// Eenheid: kwadraat van de eenheid van de samples.
// Geheugen: 1 KB voor het venster.
struct EEGfrequencyBands {
  explicit EEGfrequencyBands(uint16_t sampleRateHz) : sampleRateHz(sampleRateHz) {}
  bool AddSample(float sampleValue);
  // false zolang er nog geen 256 samples zijn. Rekentijd: 45 x 256 sinus- en cosinusberekeningen per oproep.
  bool ReadBandPowers(float& delta, float& theta, float& alpha, float& beta, float& gamma);
  float sampleWindow[256] = {};
  uint32_t sampleCount = 0;
  uint16_t sampleRateHz;
  // Som en aantal van de samples die samen één sample van het venster worden (alleen boven 256 Hz).
  float decimationSum = 0;
  uint8_t decimationCount = 0;
};

#endif
