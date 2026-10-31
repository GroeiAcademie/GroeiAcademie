# SensorDigitaal

## Implementatie

- Broncode: `src/Systeem/Sensor/SensorDigitaal.h` en `SensorDigitaal.cpp`.
- C++-component: `SensorDigitaal`, afgeleid van `Sensor`.
- Parent: `GedeeldeBusNode*` in de constructor.
- Gebruikt door: `HCSR501`, `HW201`.
- Data-kanalen: 1.

## Uit de broncode

SensorDigitaal: gemeenschappelijke basis voor een digitale Sensor met één OUT-ingang, native of via een GPIO-Extender. Een afgeleide Sensor geeft alleen zijn OUT-pin door.

## Testen

Dit document bevestigt geen geslaagde hardwaretest.
