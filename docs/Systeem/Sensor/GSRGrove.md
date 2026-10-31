# GSRGrove

## Implementatie

- Broncode: `src/Systeem/Sensor/GSRGrove.h` (geen `.cpp`).
- C++-component: `GSRGrove`, afgeleid van `SensorAnaloog`.
- Parent: `GedeeldeBusNode*` in de constructor.
- Componentcode: `GSRGrove` (0xA3).
- Configuratie: `src/Configuratie/Sensoren/ConfigGSRGrove_template.h`; de optionele persoonlijke versie heet telkens zonder `_template`.
- Data-kanalen: 1 (via SensorAnaloog).

## Uit de broncode

GSRGrove: Grove GSR analoge uitgang, uitsluitend ADC-waarde. Eén component voor één fysiek apparaat, via de bestaande ADC-Extender. De meetcode zit in SensorAnaloog; deze klasse geeft alleen de pin en het bereik door.

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_GSRGrove/`.
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.
Dit document bevestigt geen geslaagde hardwaretest.
