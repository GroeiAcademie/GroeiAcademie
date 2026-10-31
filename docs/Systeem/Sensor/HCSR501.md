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

## Aansluiten

**Aansluitwijze in de bestaande FrameWork-code:**

- **Digitale verbinding:** module-OUT naar de gekozen digitale ingang (native Arduino of de door de constructor ondersteunde GPIO-Extender).
- **Voeding/GND:** volgens de concrete module; controleer de actieve uitgangspolariteit en spanningscompatibiliteit.

- Digitale OUT → gekozen digitale ingang (native of ondersteunde GPIO-Extender).
- Voed volgens de specificaties van de gebruikte PIR-module; beweging of stabilisatietijd niet verwarren met elektrische fout.

**Voor de hardwaretest:**

- Gebruik de bestaande `.ino` uit **Testen** en de bestaande configuratiebestanden onder `src/Configuratie/`; een `*_template.h` wordt niet rechtstreeks ingelezen.
- **Een vaste Arduino-pintoewijzing is niet algemeen geldig:** de gekozen constructor, configuratie en werkelijke bekabeling moeten overeenkomen. Waar hierboven geen exact spanningsniveau of pinheader is bevestigd, mag dat niet uit deze documentatie worden afgeleid.
- De aanwezigheid van deze rubriek is **geen bewijs van een geslaagde hardwaretest**.

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_HCSR501/`.
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.
Dit document bevestigt geen geslaagde hardwaretest.
