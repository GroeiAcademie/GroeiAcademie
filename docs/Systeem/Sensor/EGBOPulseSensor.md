# EGBOPulseSensor

## Implementatie

- Broncode: `src/Systeem/Sensor/EGBOPulseSensor.h` (geen `.cpp`).
- C++-component: `EGBOPulseSensor`, afgeleid van `SensorAnaloog`.
- Parent: `GedeeldeBusNode*` in de constructor.
- Componentcode: `EGBOPulseSensor` (0xA2).
- Configuratie: `src/Configuratie/Sensoren/ConfigEGBOPulseSensor_template.h`; de optionele persoonlijke versie heet telkens zonder `_template`.
- Data-kanalen: 1 (via SensorAnaloog).

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_EGBOPulseSensor/`.
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.
Dit document bevestigt geen geslaagde hardwaretest.
