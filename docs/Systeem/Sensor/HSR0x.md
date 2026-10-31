# HSR0x

## Implementatie

- Broncode: `src/Systeem/Sensor/HSR0x.h` en `HSR0x.cpp`.
- C++-component: `HSR0x`, afgeleid van `Sensor`.
- Parent: `GedeeldeBusNode*` in de constructor.
- Componentcode: `HCSR04` (0x90), `HCSR04P` (0x91), `HYSRF05` (0x92).
- Configuratie: `src/Configuratie/Sensoren/ConfigHCSR04_template.h`, `src/Configuratie/Sensoren/ConfigHCSR04P_template.h`, `src/Configuratie/Sensoren/ConfigHYSRF05_template.h`; de optionele persoonlijke versie heet telkens zonder `_template`.
- Data-kanalen: 1.

## Uit de broncode

HC-SR04 en HC-SR04P: zelfde TRIG/ECHO-klasse. De componentcode identificeert de fysieke variant zonder een tweede klasse.

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_HCSR04/`, `examples/Systeem/GedeeldeBus/Sensoren/Sensor_HCSR04P/`, `examples/Systeem/GedeeldeBus/Sensoren/Sensor_HYSRF05/`.
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.
Dit document bevestigt geen geslaagde hardwaretest.
