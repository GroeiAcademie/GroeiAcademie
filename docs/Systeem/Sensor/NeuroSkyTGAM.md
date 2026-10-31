# NeuroSkyTGAM

## Implementatie

- Broncode: `src/Systeem/Sensor/NeuroSkyTGAM.h` en `NeuroSkyTGAM.cpp`.
- C++-component: `NeuroSkyTGAM`, afgeleid van `Sensor`.
- Parent: `GedeeldeBusNode*` in de constructor.
- Componentcode: `NeuroSkyTGAM` (0xA7).
- Configuratie: `src/Configuratie/Sensoren/ConfigNeuroSkyTGAM_template.h`; de optionele persoonlijke versie heet telkens zonder `_template`.
- Data-kanalen: 1.

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_NeuroSkyTGAM/`.
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.
Dit document bevestigt geen geslaagde hardwaretest.
