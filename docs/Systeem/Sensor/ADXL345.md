# ADXL345

## Implementatie

- Broncode: `src/Systeem/Sensor/ADXL345.h` en `ADXL345.cpp`.
- C++-component: `SensorADXL345`, afgeleid van `Sensor`.
- Parent: `HardwareResourceTypeI2C*` in de constructor.
- Componentcode: `ADXL345` (0x95).
- Configuratie: `src/Configuratie/Sensoren/ConfigADXL345_template.h`; de optionele persoonlijke versie heet telkens zonder `_template`.
- Data-kanalen: 3.
- Externe library: `Adafruit_ADXL345_U.h`.

## Uit de broncode

Alleen identiteit/lifecycle: nog geen volledige sensorinitialisatie of meting. parent moet de voor dit apparaat geregistreerde HardwareResourceTypeI2C-node zijn.

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_ADXL345/`.
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.
Dit document bevestigt geen geslaagde hardwaretest.
