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


**Aanvulling aansluitpinnen en elektrische voorwaarden:**

- VDD→3,3V **voor de INMP441-microfoonchip**, GND→GND; SCK/BCLK→I²S-bitclock, WS/LRCLK→wordselect, SD→I²S-datain; L/R→kanaalselectie volgens het betreffende breakoutboard. Niet rechtstreeks met 5V-logica voeden of aansturen zonder aantoonbare niveauomzetting.
- De concrete modulevariant bepaalt de **exacte voedingsgrenzen en fysieke pinvolgorde**. Neem die gegevens niet over van een andere module zonder de werkelijke print te controleren.

**Aansluit- en gebruiksgegevens uit de bestaande FrameWork-interface:**

- **I²S**: module `SCK/BCLK` → native `np_BCLK`, `WS/LRCLK` → `np_WS`, `SD/DOUT` → `np_SD`, plus geschikte modulevoeding en gemeenschappelijke GND. Dit is niet I²C en geen analoge microfooningang. Alleen de in `INMP441.h` toegestane ESP32-S3-boardvarianten zijn ondersteund.
- De sensor gebruikt **I²S** met drie in de constructor opgegeven native lijnen: `np_BCLK`, `np_WS` en `np_SD`; dit is niet de analoge microfooningang.
- De bestaande interface biedt `LeesSamples(int32_t* samples, size_t aantal)`; de I²S-capaciteit moet door het gekozen board en de geconfigureerde core worden ondersteund. Controleer de L/R-kanaalselectie op de INMP441-print.

**Bestaande constructor (parameter-/pinvolgorde):**

```cpp
INMP441(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin np_BCLK, HardwareResourcePin np_WS, HardwareResourcePin np_SD)
```

**Configuratie voor deze hardware:**

- Gebruik bij een eigen configuratie `src/Configuratie/Sensoren/ConfigINMP441.h` (het aanwezige `src/Configuratie/Sensoren/ConfigINMP441_template.h` is een kopieersjabloon en wordt niet rechtstreeks ingelezen).
- Voor één aangesloten exemplaar: `SENSOR_INMP441_AANTAL` op **1**, behalve wanneer de bestaande groeps-/setconfiguratie uitdrukkelijk een andere betekenis heeft.
- De bestaande configuratie bevat deze aansluitrelevante definities (gebruik de **werkelijke aangesloten** pinnen, adressen en referenties):
  - `SENSOR_INMP441_AANTAL`: sjabloonwaarde `1`.

De constructor van deze component vereist een board- of moduleafhankelijke aansluiting; kies de echte pinnen en de overeenkomende fysieke module voordat je `aanmelden()`, `inpluggen()` en `activeren()` uitvoert.

**Elektrische aansluiting en randvoorwaarden volgens TDK InvenSense:**

- De INMP441 levert **24-bit I²S-audiogegevens**, geen I²C-berichten en geen analoge ADC-spanning. De functionele verbindingen zijn SCK/BCLK, WS en SD, naast voeding, massa en een eventuele links/rechts-kanaalselectie (`L/R`). Gebruik alleen het in de bestaande ESP32-S3-FrameWorkklasse ondersteunde I²S-pinplan.

**Technische handleiding:** https://product.tdk.com/en/search/sw_piezo/mic/mems-mic/info?part_no=INMP441

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_MS3625/` (alleen ESP32-S3).

Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.

