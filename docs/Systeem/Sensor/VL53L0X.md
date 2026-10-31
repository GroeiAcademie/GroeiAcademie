# VL53L0X

## Implementatie

- Broncode: `src/Systeem/Sensor/VL53L0X.h` en `VL53L0X.cpp`.
- C++-component: `SensorVL53L0X`, afgeleid van `Sensor`.
- Parent: `HardwareResourceTypeI2C*` in de constructor.
- Componentcode: `VL53L0X` (0x93).
- Configuratie: `src/Configuratie/Sensoren/ConfigVL53L0X_template.h`; de optionele persoonlijke versie heet telkens zonder `_template`.
- Data-kanalen: 1.
- Externe library: `Adafruit_VL53L0X.h`.

## Uit de broncode

Officiële API: https://github.com/adafruit/Adafruit_VL53L0X/blob/master/src/Adafruit_VL53L0X.h

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_VL53L0X/`.
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.
Dit document bevestigt geen geslaagde hardwaretest.
