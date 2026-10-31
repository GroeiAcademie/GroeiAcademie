# SensorAnaloog

## Implementatie

- Broncode: `src/Systeem/Sensor/SensorAnaloog.h` en `SensorAnaloog.cpp`.
- C++-component: `SensorAnaloog`, afgeleid van `Sensor`.
- Parent: `GedeeldeBusNode*` in de constructor.
- Gebruikt door: `AD8232`, `BioAmpEXGPill`, `EGBOPulseSensor`, `GSRGrove`, `LM35DZ`, `MAX4466_9814`, `MF52x`, `PIEZO`, `WATER`.
- Data-kanalen: 1.

## Uit de broncode

SensorAnaloog: gemeenschappelijke basis voor een analoge Sensor met één ingang op een ADC-Extender. StartAnalogRead(), IsAnalogReadKlaar(), GetAnalogRead() en RawAnalogRead() staan hier één keer. Een afgeleide Sensor geeft alleen zijn ADC-pin en het maximale signaal van zijn uitgang (mV) door.

## Testen

Dit document bevestigt geen geslaagde hardwaretest.
