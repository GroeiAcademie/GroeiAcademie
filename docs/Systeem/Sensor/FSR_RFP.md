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

## Aansluiten

**Aansluitwijze in de bestaande FrameWork-code:**

- **Meetverbinding:** sensor-uitgang naar het toegewezen **ADC-Extender-kanaal**. Verbind de signaalreferentie/GND met de ADC-Extender volgens de vereisten van die Extender.
- **Voeding:** controleer de benodigde spanning en toegestane OUT-spanning van de **specifieke sensor of module** vóór aansluiten; de FrameWork-code bepaalt de elektrische grenswaarden van het moduleboard niet.

- Elk aangesloten FSR_RFP-signaal → zijn eigen, exclusief toegewezen ADC-kanaal; de code ondersteunt maximaal vier kanalen per sensorobject.
- Gebruik de bestaande setconfiguratie voor set 1 en indien van toepassing set 2; controleer welke analoge spanningsdeler of elektronica bij de specifieke druk-/reflectiesensor hoort.

**Voor de hardwaretest:**

- Gebruik de bestaande `.ino` uit **Testen** en de bestaande configuratiebestanden onder `src/Configuratie/`; een `*_template.h` wordt niet rechtstreeks ingelezen.
- **Een vaste Arduino-pintoewijzing is niet algemeen geldig:** de gekozen constructor, configuratie en werkelijke bekabeling moeten overeenkomen. Waar hierboven geen exact spanningsniveau of pinheader is bevestigd, mag dat niet uit deze documentatie worden afgeleid.
- De aanwezigheid van deze rubriek is **geen bewijs van een geslaagde hardwaretest**.

## Testen

Raadpleeg de bestaande FSR_RFP-voorbeelden (`examples/Systeem/GedeeldeBus/Sensoren/Sensor_FSR_RFP*/`) en `extras/TestLibrarySensoren-v2.0.0.cmd`.
Dit document bevestigt geen geslaagde hardwaretest.
