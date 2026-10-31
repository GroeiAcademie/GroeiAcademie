# FSR_RFP

## Implementatie

- Broncode: `src/Systeem/Sensor/FSR_RFP.h` en `FSR_RFP.cpp`.
- C++-component: `FSR_RFP`, afgeleid van `Sensor`.
- Eén klasse voor de probes RFP602, FSR400 en FSR402; de probe per set kies je met `FSR_RFP_PROBE_SET_1` en `FSR_RFP_PROBE_SET_2` (waarden `FSR_RFP_PROBE_RFP602`, `FSR_RFP_PROBE_FSR400`, `FSR_RFP_PROBE_FSR402`).
- Componentcode: `FSR_RFP` (0xB0).
- Parent: analoge ADC-Extender, via de generieke analoge parent-functies.
- Kanalen: maximaal vier ingangen per sensorobject, met exclusieve pinclaims.
- Configuratie: `src/Configuratie/Sensoren/ConfigFSR_RFP_template.h`; de optionele persoonlijke versie heet `ConfigFSR_RFP.h`.
- Configuratie per set (`_SET_1` en optioneel `_SET_2`) en eigen databuffers per object.
- Metingen verlopen via de bestaande lifecycle en eventuele Data-koppeling; recording op microSD schrijft naar `SENSOR.DAT`.

## Testen

Raadpleeg de bestaande FSR_RFP-voorbeelden (`examples/Systeem/GedeeldeBus/Sensoren/Sensor_FSR_RFP*/`) en `extras/TestLibrarySensoren-v2.0.0.cmd`.
Dit document bevestigt geen geslaagde hardwaretest.
