#ifndef HEARTRATEFROMBEATS_H
#define HEARTRATEFROMBEATS_H

#include <Arduino.h>

// Hartslag (BPM) uit de tijdstippen van de laatste slagen (AD8232, BioAmpEXGPill ECG, EGBOPulseSensor, MAX30102).
// Een slag minder dan 250 ms na de vorige telt niet (meer dan 240 BPM); na meer dan 3 s zonder slag begint de reeks opnieuw.
struct HeartRateFromBeats {
  bool AddBeat(unsigned long beatTimeMS);
  // Gemiddelde over de laatste slagintervallen (hoogstens 7); false met minder dan 2 slagen of als de laatste slag ouder is dan 3 s.
  bool ReadBeatsPerMinute(float& beatsPerMinute);
  bool ReadLastBeatIntervalMS(uint32_t& beatIntervalMS);
  unsigned long beatTimesMS[8] = {};
  uint32_t beatCount = 0;
};

#endif
