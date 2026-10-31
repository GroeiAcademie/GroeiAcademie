# LM35DZ

## Implementatie

- Broncode: `src/Systeem/Sensor/LM35DZ.h` en `LM35DZ.cpp`.
- C++-component: `LM35DZ`, afgeleid van `SensorAnaloog`.
- Parent: `GedeeldeBusNode*` in de constructor.
- Componentcode: `LM35DZ` (0xD5).
- Configuratie: `src/Configuratie/Sensoren/ConfigLM35DZ_template.h`; de optionele persoonlijke versie heet telkens zonder `_template`.
- Data-kanalen: 2.

## Uit de broncode

LM35DZ: temperatuursensor; ruwe ADC-waarde plus temperatuur via expliciet ADC-meetbereik. Eén component voor één fysiek apparaat, via de bestaande ADC-Extender. De meetcode zit in SensorAnaloog; deze klasse rekent in MeetwaardeVerwerken() om naar temperatuur.

## Aansluiten

**Aansluitwijze in de bestaande FrameWork-code:**

- **Meetverbinding:** sensor-uitgang naar het toegewezen **ADC-Extender-kanaal**. Verbind de signaalreferentie/GND met de ADC-Extender volgens de vereisten van die Extender.
- **Voeding:** controleer de benodigde spanning en toegestane OUT-spanning van de **specifieke sensor of module** vóór aansluiten; de FrameWork-code bepaalt de elektrische grenswaarden van het moduleboard niet.

- OUT → gekozen ADC-Extender-ingang; V+ en GND volgens de **feitelijke LM35DZ-behuizing**.
- De drie aansluitingen niet op basis van een willekeurige modulefoto omwisselen; pinvolgorde van de gebruikte uitvoering controleren.

**Voor de hardwaretest:**

- Gebruik de bestaande `.ino` uit **Testen** en de bestaande configuratiebestanden onder `src/Configuratie/`; een `*_template.h` wordt niet rechtstreeks ingelezen.
- **Een vaste Arduino-pintoewijzing is niet algemeen geldig:** de gekozen constructor, configuratie en werkelijke bekabeling moeten overeenkomen. Waar hierboven geen exact spanningsniveau of pinheader is bevestigd, mag dat niet uit deze documentatie worden afgeleid.
- De aanwezigheid van deze rubriek is **geen bewijs van een geslaagde hardwaretest**.

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_LM35DZ/`.
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.
Dit document bevestigt geen geslaagde hardwaretest.
