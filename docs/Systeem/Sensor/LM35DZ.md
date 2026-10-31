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

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_LM35DZ/`.
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.
Dit document bevestigt geen geslaagde hardwaretest.
