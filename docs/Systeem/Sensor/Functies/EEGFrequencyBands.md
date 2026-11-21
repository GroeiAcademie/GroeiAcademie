# EEGfrequencyBands

Vermogen van de EEG-banden uit een ruw EEG-signaal. Eén functie voor BioAmpEXGPill (EEG, 256 Hz) en de ruwe golf van de NeuroSkyTGAM (512 Hz).

- `EEGfrequencyBands(uint16_t sampleRateHz)`.
- `bool AddSample(float sampleValue)`: één sample; venster van 256 samples (1 KB). Geeft `false` bij `NAN`, anders `true`; een deel van de invoersamples wordt bij een samplefrequentie boven 256 Hz uitsluitend voor decimatie verzameld.
- Boven 256 Hz wordt eerst gemiddeld tot 256 Hz met `decimationSum` en `decimationCount` (512 Hz: telkens 2 samples samen).
- `bool ReadBandPowers(float& delta, float& theta, float& alpha, float& beta, float& gamma)`: DFT per frequentiebin na het aftrekken van het gemiddelde; vermogen opgeteld per band. false zolang er geen 256 samples zijn.
- Banden: delta 0,5-4 Hz, theta 4-8 Hz, alfa 8-13 Hz, bèta 13-30 Hz, gamma 30-45 Hz. Resolutie: 1 Hz per bin bij 256 en 512 Hz; delta telt de bins van 1, 2 en 3 Hz.
- Elke bin dekt 1 Hz rond zijn frequentie (bin 1: 0,5 tot 1,5 Hz), dus delta beslaat 0,5 tot 3,5 Hz.
- Eenheid: kwadraat van de eenheid van de samples; vergelijk banden onderling, geen absolute µV².
- Rekentijd: 45 x 256 sinus- en cosinusberekeningen per oproep; niet op het ritme van de samples oproepen.

Bestanden: `src/Systeem/Sensor/Functies/EEGFrequencyBands.h` en `src/Systeem/Sensor/Functies/EEGFrequencyBands.cpp`.
