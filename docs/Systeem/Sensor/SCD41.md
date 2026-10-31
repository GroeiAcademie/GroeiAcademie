# SCD41

## Implementatie

- Broncode: `src/Systeem/Sensor/SCD41.h` en `SCD41.cpp`.
- C++-component: `SCD41`, afgeleid van `Sensor`.
- Parent: `HardwareResourceTypeI2C*` in de constructor.
- Componentcode: `SCD41` (0xC8).
- Configuratie: `src/Configuratie/Sensoren/ConfigSCD41_template.h`; de optionele persoonlijke versie heet telkens zonder `_template`.
- Data-kanalen: 3.
- Externe library: `SensirionI2cScd4x.h`.

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_SCD41/`.
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.
Dit document bevestigt geen geslaagde hardwaretest.
