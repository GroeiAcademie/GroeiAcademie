# SoundLevel

Geluidsniveau over een venster. Gebruikt door MAX4466, MAX9814, INMP441 en MS3625.

- `bool AddSample(int32_t sampleValue, uint32_t maximumSampleGapMS = 250UL)`.
- `bool ReadRootMeanSquare(float& rootMeanSquare)`: RMS na het aftrekken van de DC-offset (`runningMean`, factor 1/64).
- `bool ReadPeakToPeak(int32_t& peakToPeak)`: grootste min kleinste ruwe sample in het venster.
- `void ResetWindow()`: nieuw venster; de DC-offset blijft.
- Eenheid: die van de samples (ADC-waarde of 24-bit I2S-waarde), geen dB SPL.

Bestanden: `src/Systeem/Sensor/Functies/SoundLevel.h` en `src/Systeem/Sensor/Functies/SoundLevel.cpp`.
- `lastSampleMS`: tijdstip van de laatste sample. `ReadRootMeanSquare()` en `ReadPeakToPeak()` geven false als die ouder is dan 1 s, zodat een oud venster niet als actuele meting telt.
- MAX4466_9814 en INMP441_MS3625 geven ook false zolang de sensor niet actief is.
- `AddSample(int32_t sampleValue, uint32_t maximumSampleGapMS = 250UL)`: na een onderbreking langer dan `maximumSampleGapMS` begint `AddSample()` eerst een nieuw venster met `ResetWindow()`. Zo mengen oude en nieuwe samples niet.
  - Standaard 250 ms.
  - MAX4466 en MAX9814: 2 sampleperiodes, bij 500 Hz dus 4 ms.
  - INMP441 en MS3625: de standaard van 250 ms, want de DMA buffert de samples tussen twee aanroepen van `LeesSamples()`. Een korte overloop van de DMA-buffer wordt zo niet herkend.
  - Een onderbreking korter dan die grens wordt niet herkend.
