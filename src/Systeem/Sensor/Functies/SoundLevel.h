#ifndef SOUNDLEVEL_H
#define SOUNDLEVEL_H

#include <Arduino.h>

// Geluidsniveau van een microfoonsignaal (MAX4466, MAX9814, INMP441, MS3625) over een venster.
// RMS na het aftrekken van de DC-offset (voortschrijdend gemiddelde); piek-tot-piek op de ruwe samples.
// Eenheid: die van de samples (ADC-waarde of I2S-waarde), geen dB SPL.
// Beide leesfuncties geven false als de laatste sample ouder is dan 1 s: dan is er geen actuele meting.
// Na een onderbreking langer dan maximumSampleGapMS begint AddSample() eerst een nieuw venster.
// Standaard 250 ms; de aanroeper geeft een grens die past bij zijn samplefrequentie.
struct SoundLevel {
  bool AddSample(int32_t sampleValue, uint32_t maximumSampleGapMS = 250UL);
  bool ReadRootMeanSquare(float& rootMeanSquare);
  bool ReadPeakToPeak(int32_t& peakToPeak);
  // Begint een nieuw venster; de DC-offset blijft behouden.
  void ResetWindow();
  float runningMean = NAN;           // DC-offset (biasspanning), factor 1/64 per sample
  float sumOfSquares = 0;
  int32_t minimumSample = INT32_MAX;
  int32_t maximumSample = INT32_MIN;
  uint32_t sampleCount = 0;
  uint32_t lastSampleMS = 0;     // tijdstip van de laatste AddSample()
};

#endif
