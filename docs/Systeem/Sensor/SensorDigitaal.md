# SensorDigitaal

## Implementatie

- Broncode: `src/Systeem/Sensor/SensorDigitaal.h` en `SensorDigitaal.cpp`.
- C++-component: `SensorDigitaal`, afgeleid van `Sensor`.
- Parent: `GedeeldeBusNode*` in de constructor.
- Gebruikt door: `HCSR501`, `HW201`.
- Data-kanalen: 1.

## Uit de broncode

SensorDigitaal: gemeenschappelijke basis voor een digitale Sensor met één OUT-ingang, native of via een GPIO-Extender. Een afgeleide Sensor geeft alleen zijn OUT-pin door.

## Aansluiten

**Aansluitwijze in de bestaande FrameWork-code:**

- **Digitale verbinding:** module-OUT naar de gekozen digitale ingang (native Arduino of de door de constructor ondersteunde GPIO-Extender).
- **Voeding/GND:** volgens de concrete module; controleer de actieve uitgangspolariteit en spanningscompatibiliteit.

- Digitale OUT → de door de constructor gekozen native GPIO of ingang van een ondersteunde GPIO-Extender.
- Controleer of OUT actief-hoog of actief-laag is en of de signaalspanning de ingang niet overschrijdt.

**Voor de hardwaretest:**

- Gebruik de bestaande `.ino` uit **Testen** en de bestaande configuratiebestanden onder `src/Configuratie/`; een `*_template.h` wordt niet rechtstreeks ingelezen.
- **Een vaste Arduino-pintoewijzing is niet algemeen geldig:** de gekozen constructor, configuratie en werkelijke bekabeling moeten overeenkomen. Waar hierboven geen exact spanningsniveau of pinheader is bevestigd, mag dat niet uit deze documentatie worden afgeleid.
- De aanwezigheid van deze rubriek is **geen bewijs van een geslaagde hardwaretest**.

## Testen

Dit document bevestigt geen geslaagde hardwaretest.
