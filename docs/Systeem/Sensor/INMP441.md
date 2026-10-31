# INMP441

## Implementatie

- Broncode: `src/Systeem/Sensor/INMP441.h` en `INMP441.cpp`.
- C++-component: `INMP441`, afgeleid van `Sensor`.
- Parent: `GedeeldeBusNode*` in de constructor.
- Componentcode: `INMP441` (0xB5), `MS3625` (0xB8). De MS3625 is een MEMS-microfoon via I2S die compatibel is met de INMP441: dezelfde klasse, met zijn eigen componentcode zodat de fysieke module herkenbaar blijft (in `SENSOR.DAT` en in meldingen).
- Configuratie: `src/Configuratie/Sensoren/ConfigINMP441_template.h`; de optionele persoonlijke versie heet telkens zonder `_template`.
- Data-kanalen: 1.
- Externe library: `ESP_I2S.h`.

## Uit de broncode

32-bit Philips-I2S-slots: bewaar de 24-bit microfoondata in het oorspronkelijke woord. Bij stereo staan de I2S-slots links en rechts afwisselend in de uitvoer.

## Aansluiten

**Aansluitwijze in de bestaande FrameWork-code:**

- De signaalverbindingen worden hieronder per onderdeel gespecificeerd waar ze uit de bestaande componentinterface blijken.
- Controleer voedingsspanning, pinvolgorde en externe stroombegrenzing bij de concrete hardware-uitvoering voordat je spanning aanlegt.

- BCLK/SCK → native I²S-klokpin `np_BCLK`; WS/LRCL → native I²S-wordselect `np_WS`; SD → native I²S-data-ingang `np_SD`.
- Voed de INMP441-module met de geschikte **3,3V-logica**. L/R kanaalselectie volgens de modulebeschrijving aansluiten; geen I²C of analoge ingang gebruiken.
- Controleer of het geselecteerde board I²S-ontvangst ondersteunt; niet ieder Arduino-board kan deze component gebruiken.

**Voor de hardwaretest:**

- Gebruik de bestaande `.ino` uit **Testen** en de bestaande configuratiebestanden onder `src/Configuratie/`; een `*_template.h` wordt niet rechtstreeks ingelezen.
- **Een vaste Arduino-pintoewijzing is niet algemeen geldig:** de gekozen constructor, configuratie en werkelijke bekabeling moeten overeenkomen. Waar hierboven geen exact spanningsniveau of pinheader is bevestigd, mag dat niet uit deze documentatie worden afgeleid.
- De aanwezigheid van deze rubriek is **geen bewijs van een geslaagde hardwaretest**.

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_MS3625/` (alleen ESP32-S3).

Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.
Dit document bevestigt geen geslaagde hardwaretest.
