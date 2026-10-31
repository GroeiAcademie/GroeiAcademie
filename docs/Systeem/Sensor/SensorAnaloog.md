# SensorAnaloog

## Implementatie

- Broncode: `src/Systeem/Sensor/SensorAnaloog.h` en `SensorAnaloog.cpp`.
- C++-component: `SensorAnaloog`, afgeleid van `Sensor`.
- Parent: `GedeeldeBusNode*` in de constructor.
- Gebruikt door: `AD8232`, `BioAmpEXGPill`, `EGBOPulseSensor`, `GSRGrove`, `LM35DZ`, `MAX4466_9814`, `MF52x`, `PIEZO`, `WATER`.
- Data-kanalen: 1.

## Uit de broncode

SensorAnaloog: gemeenschappelijke basis voor een analoge Sensor met één ingang op een ADC-Extender. StartAnalogRead(), IsAnalogReadKlaar(), GetAnalogRead() en RawAnalogRead() staan hier één keer. Een afgeleide Sensor geeft alleen zijn ADC-pin en het maximale signaal van zijn uitgang (mV) door.

## Aansluiten

**Aansluitwijze in de bestaande FrameWork-code:**

- **Meetverbinding:** sensor-uitgang naar het toegewezen **ADC-Extender-kanaal**. Verbind de signaalreferentie/GND met de ADC-Extender volgens de vereisten van die Extender.
- **Voeding:** controleer de benodigde spanning en toegestane OUT-spanning van de **specifieke sensor of module** vóór aansluiten; de FrameWork-code bepaalt de elektrische grenswaarden van het moduleboard niet.

- Analoge OUT → het door de constructor gekozen kanaal van de ADC-Extender; de huidige code is niet bedoeld voor een willekeurige digitale GPIO-pin.
- Sensor-GND met ADC-Extender-GND verbinden; OUT binnen het ingestelde meetbereik en spanningslimiet houden.

**Voor de hardwaretest:**

- Gebruik de bestaande `.ino` uit **Testen** en de bestaande configuratiebestanden onder `src/Configuratie/`; een `*_template.h` wordt niet rechtstreeks ingelezen.
- **Een vaste Arduino-pintoewijzing is niet algemeen geldig:** de gekozen constructor, configuratie en werkelijke bekabeling moeten overeenkomen. Waar hierboven geen exact spanningsniveau of pinheader is bevestigd, mag dat niet uit deze documentatie worden afgeleid.
- De aanwezigheid van deze rubriek is **geen bewijs van een geslaagde hardwaretest**.

## Testen

Dit document bevestigt geen geslaagde hardwaretest.
