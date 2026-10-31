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

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_PIEZO/`.
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.
Dit document bevestigt geen geslaagde hardwaretest.
