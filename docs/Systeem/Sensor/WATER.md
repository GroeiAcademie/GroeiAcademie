# WATER

## Implementatie

- Broncode: `src/Systeem/Sensor/WATER.h` (geen `.cpp`).
- C++-component: `WATER`, afgeleid van `SensorAnaloog`.
- Parent: `GedeeldeBusNode*` in de constructor.
- Componentcode: `WATER` (0xDA).
- Configuratie: `src/Configuratie/Sensoren/ConfigWATER_template.h`; de optionele persoonlijke versie heet telkens zonder `_template`.
- Data-kanalen: 1 (via SensorAnaloog).

## Uit de broncode

WATER: analoge watersensormodule; exacte modulevariant te controleren. Eén component voor één fysiek apparaat, via de bestaande ADC-Extender. De meetcode zit in SensorAnaloog; deze klasse geeft alleen de pin en het bereik door.

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_WATER/`.
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.
Dit document bevestigt geen geslaagde hardwaretest.
