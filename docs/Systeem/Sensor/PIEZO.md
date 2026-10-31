# PIEZO

## Implementatie

- Broncode: `src/Systeem/Sensor/PIEZO.h` (geen `.cpp`).
- C++-component: `PIEZO`, afgeleid van `SensorAnaloog`.
- Parent: `GedeeldeBusNode*` in de constructor.
- Componentcode: `PIEZO` (0xBA).
- Configuratie: `src/Configuratie/Sensoren/ConfigPIEZO_template.h`; de optionele persoonlijke versie heet telkens zonder `_template`.
- Data-kanalen: 1 (via SensorAnaloog).

## Uit de broncode

PIEZO: piezo-element; alleen beveiligd/conditioneerd analoog signaal. Eén component voor één fysiek apparaat, via de bestaande ADC-Extender. De meetcode zit in SensorAnaloog; deze klasse geeft alleen de pin en het bereik door.

## Aansluiten

**Aansluitwijze in de bestaande FrameWork-code:**

- **Meetverbinding:** sensor-uitgang naar het toegewezen **ADC-Extender-kanaal**. Verbind de signaalreferentie/GND met de ADC-Extender volgens de vereisten van die Extender.
- **Voeding:** controleer de benodigde spanning en toegestane OUT-spanning van de **specifieke sensor of module** vóór aansluiten; de FrameWork-code bepaalt de elektrische grenswaarden van het moduleboard niet.

- Het piezo-element mag **niet zonder bescherming** rechtstreeks naar de ADC-Extender: gebruik een geschikte begrenzings-/conditioneringsschakeling.
- De beveiligde analoge uitgang → ADC-kanaal; gemeenschappelijke referentie-GND.

**Voor de hardwaretest:**

- Gebruik de bestaande `.ino` uit **Testen** en de bestaande configuratiebestanden onder `src/Configuratie/`; een `*_template.h` wordt niet rechtstreeks ingelezen.
- **Een vaste Arduino-pintoewijzing is niet algemeen geldig:** de gekozen constructor, configuratie en werkelijke bekabeling moeten overeenkomen. Waar hierboven geen exact spanningsniveau of pinheader is bevestigd, mag dat niet uit deze documentatie worden afgeleid.
- De aanwezigheid van deze rubriek is **geen bewijs van een geslaagde hardwaretest**.

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_PIEZO/`.
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.
Dit document bevestigt geen geslaagde hardwaretest.
