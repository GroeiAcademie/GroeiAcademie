# INMP441

## Implementatie

- Broncode: `src/Systeem/Sensor/INMP441_MS3625.h` en `INMP441_MS3625.cpp`.
- C++-component: `INMP441_MS3625`, afgeleid van `Sensor`; gedeeld met de MS3625 (zie `MS3625.md`).
- Parent: `GedeeldeBusNode*` in de constructor.
- Componentcode: `INMP441` (0x98), `MS3625` (0x9B). De MS3625 is een MEMS-microfoon via I2S die compatibel is met de INMP441: dezelfde klasse, met zijn eigen componentcode zodat de fysieke module herkenbaar blijft (in `SENSOR.DAT` en in meldingen).
- Eigen instellingen per uitvoering: `SENSOR_INMP441_AANTAL` en `SENSOR_INMP441_SAMPLEFREQUENTIE_HZ` voor de INMP441, `SENSOR_MS3625_AANTAL` en `SENSOR_MS3625_SAMPLEFREQUENTIE_HZ` voor de MS3625.
- Configuratie: `src/Configuratie/Sensoren/ConfigINMP441_template.h`; de optionele persoonlijke versie heet telkens zonder `_template`.
- Data-kanalen: 3.
- Externe library: `ESP_I2S.h` (ESP32-S3 en ESP32) of `I2S.h` van de arduino-pico-core (RP2040).

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

- **I²S**: module `SCK/BCLK` → native `np_BCLK`, `WS/LRCLK` → `np_WS`, `SD/DOUT` → `np_SD`, plus geschikte modulevoeding en gemeenschappelijke GND. Dit is niet I²C en geen analoge microfooningang. Alleen de boards uit de lijst "Boards" onderaan zijn ondersteund.
- De sensor gebruikt **I²S** met drie in de constructor opgegeven native lijnen: `np_BCLK`, `np_WS` en `np_SD`; dit is niet de analoge microfooningang.
- De bestaande interface biedt `LeesSamples(int32_t* samples, size_t aantal)`; de I²S-capaciteit moet door het gekozen board en de geconfigureerde core worden ondersteund. Controleer de L/R-kanaalselectie op de INMP441-print.

**Bestaande constructor (parameter-/pinvolgorde):**

```cpp
INMP441_MS3625(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin np_BCLK, HardwareResourcePin np_WS, HardwareResourcePin np_SD)
```

**Configuratie voor deze hardware:**

- Gebruik bij een eigen configuratie `src/Configuratie/Sensoren/ConfigINMP441.h` (het aanwezige `src/Configuratie/Sensoren/ConfigINMP441_template.h` is een kopieersjabloon en wordt niet rechtstreeks ingelezen).
- Voor één aangesloten exemplaar: `SENSOR_INMP441_AANTAL` op **1**, behalve wanneer de bestaande groeps-/setconfiguratie uitdrukkelijk een andere betekenis heeft.
- De bestaande configuratie bevat deze aansluitrelevante definities (gebruik de **werkelijke aangesloten** pinnen, adressen en referenties):
  - `SENSOR_INMP441_AANTAL`: sjabloonwaarde `1`.

De constructor van deze component vereist een board- of moduleafhankelijke aansluiting; kies de echte pinnen en de overeenkomende fysieke module voordat je `aanmelden()`, `inpluggen()` en `activeren()` uitvoert.

**Elektrische aansluiting en randvoorwaarden volgens TDK InvenSense:**

- De INMP441 levert **24-bit I²S-audiogegevens**, geen I²C-berichten en geen analoge ADC-spanning. De functionele verbindingen zijn SCK/BCLK, WS en SD, naast voeding, massa en een eventuele links/rechts-kanaalselectie (`L/R`). Gebruik alleen het I²S-pinplan van de boards uit de lijst "Boards" onderaan.

**Technische handleiding:** https://product.tdk.com/en/search/sw_piezo/mic/mems-mic/info?part_no=INMP441

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_INMP441_CD74HC4067/` en `Sensor_INMP441_CD74HC4067_Verwerking/` (boards: zie hieronder); voor de MS3625 zie `MS3625.md`.

Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.


### I²S-timing en sampleverlies

- Meet bij werkelijke I²S samplefrequentie de maximale blokkeertijd van `readBytes()` en het aantal gemiste samples wanneer een andere Sturing of Sensor gelijktijdig actief is. Beoordeel buffer-overrun en herstel na onderbroken opname.
- **Status:** fysieke proef vereist; nog geen hardwarebewijs.

## Hulppinnen en routes via Extenders

- Mixed: BCLK en WS native, SD via een kanaal van een CD74HC4067 (Z = native SD-pin). Vereist `GEDEELDE_BUS_MIXED_PINS`.

## Verwerking

- `ReadSoundLevelRootMeanSquare(float&)` en `ReadSoundLevelPeakToPeak(int32_t&)` (SoundLevel): uit de samples van `LeesSamples()`, linkerslot (L/R-pin aan GND), 24-bit waarde, venster een kwart seconde. Roep `LeesSamples()` doorlopend aan.
- Voorbeeld: `Sensor_INMP441_CD74HC4067_Verwerking.ino`.

## Boards
- ESP32-S3 ARDI32, ESP32-S3 DEV en ESP32 D1 R32: via `ESP_I2S.h` van de ESP32-core.
- RP2040 Cytron Maker UNO: via `I2S.h` van de arduino-pico-core, met de PIO van de chip. WS moet de pin direct na BCLK zijn; met D3 (GP3) en D4 (GP4) klopt dat. Anders geeft `Activeren()` false.
- UNO R4 Minima en WiFi: niet ondersteund. De RA4M1 van de UNO R4 heeft geen I2S-interface.
- UNO Q: momenteel niet ondersteund. Er is nog geen werkende I2S-implementatie aangetoond voor de gebruikte Arduino-core.
- Nucleo F401RE: niet ondersteund. De STM32-core heeft geen standaard I2S-library.
- Op een niet-ondersteund board geeft het compileren een `#error` met de reden.

## Data-kanalen

- Kanaal 1: elk ruw I2S-woord (int), kanaal 2: RMS (float), kanaal 3: piek-tot-piek (int).
- Kanaal 2 en 3 worden geschreven bij elk vol venster van 250 ms in `LeesSamples()`, ook als de toepassing de RMS niet opvraagt.
- Het record ontstaat op het moment dat het venster vol raakt, direct na de laatste sample. Het venster blijft voor de leesfuncties beschikbaar tot de volgende sample het wist. Daardoor toont Data het laatst afgesloten venster, en de rechtstreekse RMS kan het venster tonen dat nog loopt.
- Continue audio-opname via Data is niet aangetoond: 16000 Hz stereo zijn 32.000 woorden per seconde, en of een buffer, FLASH of microSD dat zonder verlies bijhoudt, vraagt een meting.

## Heractivatie en Mixed

- `Activeren()` wist het geluidsvenster (`soundLevel.ResetWindow()`).
- Mixed (SD via een CD74HC4067): `LeesSamples()` geeft 0 samples als die CD74HC4067 niet actief is.
