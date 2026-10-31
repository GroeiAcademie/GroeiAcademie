# LM75A

## Implementatie

- Broncode: `src/Systeem/Sensor/LM75A.h` en `LM75A.cpp`.
- C++-component: `LM75A`, afgeleid van `Sensor`.
- Parent: `HardwareResourceTypeI2C*` in de constructor.
- Componentcode: `LM75A` (0xD6).
- Configuratie: `src/Configuratie/Sensoren/ConfigLM75A_template.h`; de optionele persoonlijke versie heet telkens zonder `_template`.
- Data-kanalen: 1.
- Externe library: `M2M_LM75A.h`.

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_LM75A/`.
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.
Dit document bevestigt geen geslaagde hardwaretest.
