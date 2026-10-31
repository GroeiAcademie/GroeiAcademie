# MF52x

## Implementatie

- Broncode: `src/Systeem/Sensor/MF52x.h` (geen `.cpp`).
- C++-component: `MF52x`, afgeleid van `SensorAnaloog`.
- Parent: `GedeeldeBusNode*` in de constructor.
- Componentcode: `MF52A` (0xD7), `MF52D` (0xD8).
- Configuratie: `src/Configuratie/Sensoren/ConfigMF52A_template.h`, `src/Configuratie/Sensoren/ConfigMF52D_template.h`; de optionele persoonlijke versie heet telkens zonder `_template`.
- Data-kanalen: 1 (via SensorAnaloog).

## Uit de broncode

MF52A: NTC thermistor; spanning, niet omgerekende temperatuur. Eén component voor één fysiek apparaat, via de bestaande ADC-Extender. De meetcode zit in SensorAnaloog; deze klasse geeft alleen de pin en het bereik door. Het fysieke type behoudt zijn eigen componentcode en configuratie. De analoge ADC-meetfuncties zijn identiek aan die van MF52A.

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_MF52A/`, `examples/Systeem/GedeeldeBus/Sensoren/Sensor_MF52D/`.
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.
Dit document bevestigt geen geslaagde hardwaretest.
