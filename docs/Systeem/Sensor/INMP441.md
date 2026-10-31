# INMP441

## Implementatie

- Broncode: `src/Systeem/Sensor/INMP441.h` en `INMP441.cpp`.
- C++-component: `INMP441`, afgeleid van `Sensor`.
- Parent: `GedeeldeBusNode*` in de constructor.
- Componentcode: `INMP441` (0xB5), `MS3625` (0xB8). De MS3625 is een MEMS-microfoon via I2S die compatibel is met de INMP441: dezelfde klasse, met zijn eigen componentcode zodat de fysieke module herkenbaar blijft (in `SENSOR.DAT` en in meldingen).
- Configuratie: `src/Configuratie/Sensoren/ConfigINMP441_template.h`; de optionele persoonlijke versie heet telkens zonder `_template`.
- Data-kanalen: 1.
- Externe library: `ESP_I2S.h`.

## Uit de broncode

32-bit Philips-I2S-slots: bewaar de 24-bit microfoondata in het oorspronkelijke woord. Bij stereo staan de I2S-slots links en rechts afwisselend in de uitvoer.

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_MS3625/` (alleen ESP32-S3).

Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.
Dit document bevestigt geen geslaagde hardwaretest.
