# BioAmpEXGPill

## Implementatie

- Broncode: `src/Systeem/Sensor/BioAmpEXGPill.h` en `BioAmpEXGPill.cpp`.
- C++-component: `BioAmpEXGPill`, afgeleid van `SensorAnaloog`.
- Parent: `GedeeldeBusNode*` in de constructor.
- Componentcode: `BioAmpEXGPill` (0xA1).
- Configuratie: `src/Configuratie/Sensoren/ConfigBioAmpEXGPill_template.h`; de optionele persoonlijke versie heet telkens zonder `_template`.
- Data-kanalen: 1 (via SensorAnaloog).

## Aansluiten

**Aansluitwijze in de bestaande FrameWork-code:**

- **Meetverbinding:** sensor-uitgang naar het toegewezen **ADC-Extender-kanaal**. Verbind de signaalreferentie/GND met de ADC-Extender volgens de vereisten van die Extender.
- **Voeding:** controleer de benodigde spanning en toegestane OUT-spanning van de **specifieke sensor of module** vóór aansluiten; de FrameWork-code bepaalt de elektrische grenswaarden van het moduleboard niet.

- Analoge OUT van de signaalversterker → het gekozen kanaal van de ADC-Extender.
- Electrode/sensoraansluitingen uitsluitend volgens de concrete BioAmp EXG Pill-handleiding en met een **elektrisch veilige, geïsoleerde** opstelling.

**Voor de hardwaretest:**

- Gebruik de bestaande `.ino` uit **Testen** en de bestaande configuratiebestanden onder `src/Configuratie/`; een `*_template.h` wordt niet rechtstreeks ingelezen.
- **Een vaste Arduino-pintoewijzing is niet algemeen geldig:** de gekozen constructor, configuratie en werkelijke bekabeling moeten overeenkomen. Waar hierboven geen exact spanningsniveau of pinheader is bevestigd, mag dat niet uit deze documentatie worden afgeleid.
- De aanwezigheid van deze rubriek is **geen bewijs van een geslaagde hardwaretest**.

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_BioAmpEXGPill/` (lifecycle), `Sensor_BioAmpEXGPill_ECG_Hartslag/` (`LeesHartslagPiek()`, met `SENSOR_BIOAMPEXGPILL_MEETMETHODE` `BIOAMPEXGPILL_MEETMETHODE_ECG`), `Sensor_BioAmpEXGPill_EMG_Envelop/` (`LeesEMGEnvelop()`, `_EMG`) en `Sensor_BioAmpEXGPill_EOG_Knipperen/` (`LeesKnipperen()`, `_EOG`), volgens de officiële voorbeelden HeartBeatDetection, EMGEnvelop en EyeBlinkDetection.
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.
Dit document bevestigt geen geslaagde hardwaretest.
