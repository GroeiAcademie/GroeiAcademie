# HeartRateFromBeats

Hartslag (BPM) uit de tijdstippen van slagen. Gebruikt door AD8232, BioAmpEXGPill (ECG) en EGBOPulseSensor.

- `bool AddBeat(unsigned long beatTimeMS)`: een slag; minder dan 250 ms na de vorige telt niet, na meer dan 3 s pauze begint de reeks opnieuw.
- `bool ReadBeatsPerMinute(float& beatsPerMinute)`: gemiddelde over de laatste slagintervallen (hoogstens 7). false met minder dan 2 slagen of als de laatste slag ouder is dan 3 s.
- `bool ReadLastBeatIntervalMS(uint32_t& beatIntervalMS)`: het laatste slaginterval.

Bestanden: `src/Systeem/Sensor/Functies/HeartRateFromBeats.h` en `src/Systeem/Sensor/Functies/HeartRateFromBeats.cpp`.
