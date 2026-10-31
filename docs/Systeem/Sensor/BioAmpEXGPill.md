# BioAmpEXGPill

## Implementatie

- Broncode: `src/Systeem/Sensor/BioAmpEXGPill.h` en `BioAmpEXGPill.cpp`.
- C++-component: `BioAmpEXGPill`, afgeleid van `SensorAnaloog`.
- Parent: `GedeeldeBusNode*` in de constructor.
- Componentcode: `BioAmpEXGPill` (0xA1).
- Configuratie: `src/Configuratie/Sensoren/ConfigBioAmpEXGPill_template.h`; de optionele persoonlijke versie heet telkens zonder `_template`.
- Data-kanalen: 1 (via SensorAnaloog).

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_BioAmpEXGPill/` (lifecycle), `Sensor_BioAmpEXGPill_ECG_Hartslag/` (`LeesHartslagPiek()`, met `SENSOR_BIOAMPEXGPILL_MEETMETHODE` `BIOAMPEXGPILL_MEETMETHODE_ECG`), `Sensor_BioAmpEXGPill_EMG_Envelop/` (`LeesEMGEnvelop()`, `_EMG`) en `Sensor_BioAmpEXGPill_EOG_Knipperen/` (`LeesKnipperen()`, `_EOG`), volgens de officiële voorbeelden HeartBeatDetection, EMGEnvelop en EyeBlinkDetection.
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.
Dit document bevestigt geen geslaagde hardwaretest.
