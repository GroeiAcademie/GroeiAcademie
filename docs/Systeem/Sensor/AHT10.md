# AHT10

## Implementatie

- Broncode: `src/Systeem/Sensor/AHT10.h` en `AHT10.cpp`.
- C++-component: `AHT10`, afgeleid van `Sensor`.
- Parent: `HardwareResourceTypeI2C*` in de constructor.
- Componentcode: `AHT10` (0xD0).
- Configuratie: `src/Configuratie/Sensoren/ConfigAHT10_template.h`; de optionele persoonlijke versie heet telkens zonder `_template`.
- Data-kanalen: 2.
- Externe library: `Adafruit_AHTX0.h`.

## Uit de broncode

Officiële API: https://github.com/adafruit/Adafruit_AHTX0/blob/master/Adafruit_AHTX0.h

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_AHT10/`.
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.
Dit document bevestigt geen geslaagde hardwaretest.
