# BME68x

## Implementatie

- Broncode: `src/Systeem/Sensor/BME68x.h` en `BME68x.cpp`.
- C++-component: `BME68x`, afgeleid van `Sensor`.
- Parent: `HardwareResourceTypeI2C*` in de constructor.
- Componentcode: `BME680` (0xC5), `BME688` (0xC6).
- Configuratie: `src/Configuratie/Sensoren/ConfigBME680_template.h`, `src/Configuratie/Sensoren/ConfigBME688_template.h`; de optionele persoonlijke versie heet telkens zonder `_template`.
- Data-kanalen: 4.
- Externe library: `Adafruit_BME680.h`.

## Uit de broncode

Alleen identiteit/lifecycle: nog geen volledige sensorinitialisatie of meting. parent moet de voor dit apparaat geregistreerde HardwareResourceTypeI2C-node zijn.

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_BME680/`, `examples/Systeem/GedeeldeBus/Sensoren/Sensor_BME688/`.
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.
Dit document bevestigt geen geslaagde hardwaretest.
