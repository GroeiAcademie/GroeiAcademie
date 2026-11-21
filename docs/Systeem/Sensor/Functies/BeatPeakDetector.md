# BeatPeakDetector

Slagdetectie voor ECG en PPG. Gebruikt door AD8232 en EGBOPulseSensor.

- `bool AddSample(float sampleValue, unsigned long sampleTimeMS, bool& beatDetected)`: één sample op een vast ritme. `beatDetected` is true bij een slag; een `NAN`-sample levert `false` op en wordt niet verwerkt.
- Werking: de basislijn (`baselineValue`) volgt het signaal traag (factor 0,02); de drempel (`adaptiveThreshold`) volgt de laatste top en zakt 0,5 % per sample. Een slag is een lokaal maximum boven 60 % van de drempel, niet vroeger dan `refractoryPeriodMS` (standaard 300 ms) na de vorige.
- Niet hardwaregetest. Werkt alleen met samples op een regelmatig ritme.

Bestanden: `src/Systeem/Sensor/Functies/BeatPeakDetector.h` en `src/Systeem/Sensor/Functies/BeatPeakDetector.cpp`.
