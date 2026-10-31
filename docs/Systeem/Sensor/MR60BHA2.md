# MR60BHA2

## Implementatie

- Broncode: `src/Systeem/Sensor/MR60BHA2.h` en `MR60BHA2.cpp`.
- C++-component: `MR60BHA2`, afgeleid van `Sensor`.
- Parent: `GedeeldeBusNode*` in de constructor.
- Componentcode: `MR60BHA2` (0xA6).
- Configuratie: `src/Configuratie/Sensoren/ConfigMR60BHA2_template.h`; de optionele persoonlijke versie heet telkens zonder `_template`.
- Data-kanalen: 1.

## Uit de broncode

Seeed MR60BHA2: UART-frame 0x01, ID(2), lengte(2), type(2), checksum, payload, checksum. De officiele ESP32-only library is niet vereist. Stream laat zowel native HardwareSerial als een UART-Extender toe.

## Aansluiten

**Aansluitwijze in de bestaande FrameWork-code:**

- **Seriële verbinding:** UART-**TX van de sensor → RX** van de gekozen seriële poort; eventueel sensor-RX ← TX van dezelfde poort.
- **Voeding:** aansluitspanning en UART-logicaniveau volgens de exacte module; gebruik indien nodig een niveauaanpassing.

- UART-TX van de radarsensor → RX van de gekozen native HardwareSerial of UART-Extender; UART-RX van de sensor ← TX.
- Voeding, signaalniveau en seriële instellingen volgens het concrete Seeed-moduleboard; de code verwerkt UART-frames.

**Voor de hardwaretest:**

- Gebruik de bestaande `.ino` uit **Testen** en de bestaande configuratiebestanden onder `src/Configuratie/`; een `*_template.h` wordt niet rechtstreeks ingelezen.
- **Een vaste Arduino-pintoewijzing is niet algemeen geldig:** de gekozen constructor, configuratie en werkelijke bekabeling moeten overeenkomen. Waar hierboven geen exact spanningsniveau of pinheader is bevestigd, mag dat niet uit deze documentatie worden afgeleid.
- De aanwezigheid van deze rubriek is **geen bewijs van een geslaagde hardwaretest**.

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_MR60BHA2/`.
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.
Dit document bevestigt geen geslaagde hardwaretest.
