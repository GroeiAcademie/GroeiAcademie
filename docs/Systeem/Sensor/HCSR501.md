# HCSR501

## Implementatie

- Broncode: `src/Systeem/Sensor/HCSR501.h` (geen `.cpp`).
- C++-component: `HCSR501`, afgeleid van `SensorDigitaal`.
- Parent: `GedeeldeBusNode*` in de constructor.
- Componentcode: `HCSR501` (0x81).
- Configuratie: `src/Configuratie/Sensoren/ConfigHCSR501_template.h`; de optionele persoonlijke versie heet telkens zonder `_template`.
- Data-kanalen: 1 (via SensorDigitaal).

## Uit de broncode

HC-SR501 heeft geen ID of busprotocol. Dit controleert alleen de geconfigureerde ingang.

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_HCSR501/`.
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.
Dit document bevestigt geen geslaagde hardwaretest.
