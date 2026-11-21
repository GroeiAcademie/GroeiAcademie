#ifndef BEATPEAKDETECTOR_H
#define BEATPEAKDETECTOR_H

#include <Arduino.h>

// Slagdetectie voor ECG en PPG (AD8232, BioAmpEXGPill ECG, EGBOPulseSensor, MAX30102).
// Werkt op samples met een vast ritme: de basislijn volgt het signaal traag, de drempel volgt de laatste top en zakt daarna langzaam.
// Een slag is een lokaal maximum boven 60 % van de drempel, niet vroeger dan refractoryPeriodMS na de vorige slag.
struct BeatPeakDetector {
  explicit BeatPeakDetector(unsigned long refractoryPeriodMS = 300UL) : refractoryPeriodMS(refractoryPeriodMS) {}
  // false: ongeldig sample (NAN); beatDetected is dan false.
  bool AddSample(float sampleValue, unsigned long sampleTimeMS, bool& beatDetected);
  float baselineValue = NAN;          // trage basislijn (DC) van het signaal
  float adaptiveThreshold = 0;        // laatste top boven de basislijn, zakt per sample 0,5 %
  float previousSampleValue = 0;      // vorig sample min de basislijn
  unsigned long lastBeatTimeMS = 0;
  unsigned long refractoryPeriodMS;   // kortste tijd tussen twee slagen (300 ms = 200 BPM)
};

#endif
