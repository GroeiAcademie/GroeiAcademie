# MR60BHA2

## Implementatie

- Broncode: `src/Systeem/Sensor/MR60BHA2.h` en `MR60BHA2.cpp`.
- C++-component: `MR60BHA2`, afgeleid van `Sensor`.
- Parent: `GedeeldeBusNode*` in de constructor.
- Componentcode: `MR60BHA2` (0xA6).
- Configuratie: `src/Configuratie/Sensoren/ConfigMR60BHA2_template.h`; de optionele persoonlijke versie heet telkens zonder `_template`.
- Data-kanalen: 1.

## Uit de broncode

Seeed MR60BHA2: UART-frame 0x01, ID(2), lengte(2), type(2), checksum, payload, checksum. De officiele ESP32-only library is niet vereist. Stream laat zowel native HardwareSerial als een UART-Extender toe.

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_MR60BHA2/`.
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.
Dit document bevestigt geen geslaagde hardwaretest.
