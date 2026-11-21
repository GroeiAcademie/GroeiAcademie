# NeuroSkyTGAM

## Implementatie

- Broncode: `src/Systeem/Sensor/NeuroSkyTGAM.h` en `NeuroSkyTGAM.cpp`.
- C++-component: `NeuroSkyTGAM`, afgeleid van `Sensor`.
- Parent: `GedeeldeBusNode*` in de constructor.
- Componentcode: `NeuroSkyTGAM` (0xBA).
- Configuratie: `src/Configuratie/Sensoren/ConfigNeuroSkyTGAM_template.h`; de optionele persoonlijke versie heet telkens zonder `_template`.
- Data-kanalen: 15.

## Aansluiten

**Aansluitwijze in de bestaande FrameWork-code:**

- **Seriële verbinding:** UART-**TX van de sensor → RX** van de gekozen seriële poort; eventueel sensor-RX ← TX van dezelfde poort.
- **Voeding:** aansluitspanning en UART-logicaniveau volgens de exacte module; gebruik indien nodig een niveauaanpassing.

- UART-TX van TGAM → RX van de geselecteerde seriële interface; alleen TX/RX verbinden die de gebruikte module daadwerkelijk aanbiedt.
- De TGAM gebruikt lage logicaniveaus; nooit zonder gecontroleerde niveauaanpassing op een 5V-uitgang aansluiten.
- Bij lichaamscontact uitsluitend de door de fabrikant voorgeschreven, elektrisch veilige/isolatiegeschikte opstelling gebruiken.

**Voor de hardwaretest:**

- Gebruik de bestaande `.ino` uit **Testen** en de bestaande configuratiebestanden onder `src/Configuratie/`; een `*_template.h` wordt niet rechtstreeks ingelezen.
- **Een vaste Arduino-pintoewijzing is niet algemeen geldig:** de gekozen constructor, configuratie en werkelijke bekabeling moeten overeenkomen. Waar hierboven geen exact spanningsniveau of pinheader is bevestigd, mag dat niet uit deze documentatie worden afgeleid.


**Aanvulling aansluitpinnen en elektrische voorwaarden:**

- TGAM TX→UART RX; GND→systeemreferentie uitsluitend volgens de officiële TGAM-modulehandleiding. EEG-elektrode-, referentie- en voeding/aardingsverbindingen mogen niet uit een willekeurige UART-aansluittabel worden afgeleid; modulevariant en veilige voeding vereist.
- De concrete modulevariant bepaalt de **exacte voedingsgrenzen en fysieke pinvolgorde**. Neem die gegevens niet over van een andere module zonder de werkelijke print te controleren.

**Aansluit- en gebruiksgegevens uit de bestaande FrameWork-interface:**

- **UART-bekabeling voor NeuroSkyTGAM:** `TX` van de sensor → `RX` van de gekozen host-/Extender-UART, en `RX` van de sensor → `TX` van die UART indien de gebruikte interface beide richtingen nodig heeft. `GND` moet gemeenschappelijk zijn.
- Controleer de UART-logicaniveaus én de vereiste baudrate van de fysieke module; een UART-pin is geen RS-232-niveau. Kies exact de door de constructor ondersteunde native seriële poort of UART-Extender-kanaal.
- De driver gebruikt een seriële `Stream` en bevat aparte `LeesMeditatie(float&)` en `LeesEEGBanden(uint32_t banden[8])` naast `LeesHoofdwaarde(float&)`.
- De sensor-TX/RX-verbinding moet naar een passende UART-poort met correcte spanningsniveaus; hou lichaamssensorverbindingen galvanisch veilig ten opzichte van USB/netvoeding.

**Bestaande constructor (parameter-/pinvolgorde):**

```cpp
NeuroSkyTGAM(GedeeldeBusNode* parent, GedeeldeBusComponent component, Stream& serielePoort, HardwareResourcePin np_TX, HardwareResourcePin np_RX)
```

```cpp
NeuroSkyTGAM(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t channel)
```

**Configuratie voor deze hardware:**

- Gebruik bij een eigen configuratie `src/Configuratie/Sensoren/ConfigNeuroSkyTGAM.h` (het aanwezige `src/Configuratie/Sensoren/ConfigNeuroSkyTGAM_template.h` is een kopieersjabloon en wordt niet rechtstreeks ingelezen).
- Voor één aangesloten exemplaar: `SENSOR_NEUROSKYTGAM_AANTAL` op **1**, behalve wanneer de bestaande groeps-/setconfiguratie uitdrukkelijk een andere betekenis heeft.
- De bestaande configuratie bevat deze aansluitrelevante definities (gebruik de **werkelijke aangesloten** pinnen, adressen en referenties):
  - `SENSOR_NEUROSKYTGAM_AANTAL`: sjabloonwaarde `1`.

**Letterlijk constructorfragment uit de bestaande voorbeeldsketch** (alle gebruikte parent-objecten moeten zelf correct aangesloten en geïnitialiseerd zijn):

```cpp
GedeeldeBusNewComponent<NeuroSkyTGAM>(&Native, GedeeldeBusComponent::NeuroSkyTGAM, GA_SENSOR_SERIAL, GA_SENSOR_SERIAL_PIN_TX, GA_SENSOR_SERIAL_PIN_RX);
```
`GA_SENSOR_SERIAL`, `GA_SENSOR_SERIAL_PIN_RX` en `GA_SENSOR_SERIAL_PIN_TX` staan per board in `src/Configuratie/SystemConfig.h`, naast `GA_SERIAL`.
Sensor-TX gaat naar `GA_SENSOR_SERIAL_PIN_RX`; sensor-RX gaat naar `GA_SENSOR_SERIAL_PIN_TX`.

| Board | Poort | RX | TX |
|---|---|---|---|
| UNO R4 Minima, UNO R4 WiFi | Serial1 | D0 | D1 |
| UNO Q | Serial1 | D0 | D1 |
| ESP32-S3 ARDI32 | Serial1 | D0 (GPIO18) | D1 (GPIO17) |
| RP2040 Cytron Maker UNO | Serial1 | D0 (GP1) | D1 (GP0) |
| ESP32 D1 R32 | Serial1 | D2 (GPIO26) | D3 (GPIO25) |
| ESP32-S3 DEV | Serial1 | D2 (GPIO18) | D3 (GPIO17) |
| Nucleo F401RE | Serial1 (USART1) | D2 (PA10) | D8 (PA9) |

- Op de UNO Q stuurt Zephyr bij elke reset een opstartmelding over Serial1.
- Op de ESP32 D1 R32 en de ESP32-S3 DEV dragen D0/D1 de debugpoort `GA_SERIAL`, daarom D2/D3.
- Op een ESP32 geeft het sketch de pinnen mee aan `begin()` of `setPins()`.
- Op de Nucleo F401RE gaan D0/D1 naar de ST-Link, daarom USART1 op D2/D8.
- Op de Nucleo maakt het sketch `Serial1` zelf aan als `Uart Serial1(...)`, want de STM32-core maakt standaard alleen `Serial` aan.
- Sinds STM32duino 3.0.0 heet de seriële klasse `Uart`; de sketches vragen dus STM32duino 3.0.0 of nieuwer.


**Bestaande test-/voorbeeldsketches:**

- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_NeuroSkyTGAM/Sensor_NeuroSkyTGAM.ino`

**Elektrische aansluiting en randvoorwaarden volgens NeuroSky:**

- De **NeuroSky TGAM** is een EEG-sensormodule met seriële gegevensuitvoer. De bestaande FrameWorkconstructor vraagt een `Stream` en heeft afzonderlijke gereserveerde TX/RX-pinnen, of vraagt een kanaal van een bestaande UART-Extender. **De TGAM-seriële uitgang hoort op de ontvangende UART-ingang van de host**, met gemeenschappelijke signaalmassa.
- NeuroSky beschrijft verwerking van EEG-banden, signaalkwaliteit, aandacht, meditatie en knippersignalen. De fabrikantproductpagina specificeert hier **geen universele breakoutconnector, vaste voedingsspanning of pinvolgorde voor elk TGAM-board**. Gebruik de concrete fabrikantmoduleaansluiting, vooral bij elektrodecontact met het lichaam uitsluitend galvanisch veilig/batterijgevoed.

**Technische handleiding:** https://store.neurosky.com/products/eeg-tgam

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_NeuroSkyTGAM/`.
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.


### Checksumcontrole en herstel van UART-pakketten

- Injecteer een afgebroken UART-pakket, verkeerd checksumbyte en een nieuwe geldige frame. Controleer dat ongeldige frames niet als nieuwe meting verschijnen en dat het eerstvolgende geldige frame correct wordt verwerkt.
- **Status:** fysieke proef vereist; nog geen hardwarebewijs.

### Aanvullende Sensor × Extender-testvoorbeelden

De onderstaande `.ino`-bestanden behoren tot de huidige v2.0.0-voorbeelden. Hun aanwezigheid is gecontroleerd; compilatie en fysieke hardwarevalidatie zijn hier nog **niet** aangetoond.

- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_NeuroSkyTGAM_MAX14830I2C/Sensor_NeuroSkyTGAM_MAX14830I2C.ino`
- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_NeuroSkyTGAM_MAX14830SPI/Sensor_NeuroSkyTGAM_MAX14830SPI.ino`
- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_NeuroSkyTGAM_SC16IS752I2C/Sensor_NeuroSkyTGAM_SC16IS752I2C.ino`
- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_NeuroSkyTGAM_SC16IS752SPI/Sensor_NeuroSkyTGAM_SC16IS752SPI.ino`

### Functionele Sensor × Extender-voorbeelden

De onderstaande voorbeelden bevatten **meetaanroepen na de activering**. Een getoonde waarde is geen bewijs van meetnauwkeurigheid of van een geslaagde hardwaretest.

- **Fysieke test:** Controleer valide attention, meditation en EEG-banden bij goed contact.
- **Huidige reikwijdte:** de `.ino` gebruikt uitsluitend reeds bestaande FrameWork-functies; niet-beschikbare meetkanalen, kalibraties of externe functies zijn niet toegevoegd.

**Voorbeelden:**

- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_NeuroSkyTGAM_MAX14830I2C/Sensor_NeuroSkyTGAM_MAX14830I2C.ino`
- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_NeuroSkyTGAM_MAX14830SPI/Sensor_NeuroSkyTGAM_MAX14830SPI.ino`
- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_NeuroSkyTGAM_SC16IS752I2C/Sensor_NeuroSkyTGAM_SC16IS752I2C.ino`
- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_NeuroSkyTGAM_SC16IS752SPI/Sensor_NeuroSkyTGAM_SC16IS752SPI.ino`

**Validatie:** Arduino-compilatie, fysieke test, kalibratie en meetkwaliteitsvalidatie zijn hier nog niet aangetoond.

## Native meetvoorbeeld

`examples/Systeem/GedeeldeBus/Sensoren/Sensor_NeuroSkyTGAM/Sensor_NeuroSkyTGAM.ino` voert na de lifecycle daadwerkelijk uitlezingen uit via de bestaande Sensor-API.
Gemeten of weergegeven in dit voorbeeld: **Attention (0-100)**.
De seriële uitvoer wordt beperkt; de Sensor-uitlezing blijft in `loop()` plaatsvinden.
Dit bevestigt bij geslaagde hardwaretests de ruwe of rechtstreeks beschikbare meetwaarde, niet automatisch alle fysische kanalen, kalibratie of nauwkeurigheid.
Compilatie en fysieke meting zijn nog niet bevestigd.

## Ruwe golf en EEG-banden

- `ReadRawEEGsample(int16_t&)`: laatste ruwe EEG-sample (code 0x80, 512 per seconde, 57600-baudmodus).
- `ReadEEGbandPowersFromRawWave(delta, theta, alpha, beta, gamma)`: met dezelfde functie als de BioAmpEXGPill (EEGfrequencyBands, eerst gemiddeld tot 256 Hz, venster 1 s, 1 Hz per bin). Dit is los van de door de module berekende banden van `LeesEEGBanden()`.
- Ouderdom:
  - `ReadRawEEGsample()` en `ReadEEGbandPowersFromRawWave()` geven false als de laatste ruwe sample ouder is dan 1 s (`lastRawEEGsampleMS`).
  - Na een onderbreking langer dan 1 s begint het EEG-venster opnieuw (`sampleCount`, `decimationSum` en `decimationCount` op 0), zodat oude en nieuwe samples niet mengen.
  - Een onderbreking korter dan 1 s wordt niet herkend. De banden uit de ruwe golf komen dus niet bewezen uit een aaneengesloten venster.
  - `LeesMeditatie()` geeft false als de laatste geldige Meditation ouder is dan 3 s (`lastMeditationMS`).
  - `LeesEEGBanden()` geeft false als de laatste geldige 8 banden ouder zijn dan 3 s (`lastEEGbandsMS`).
  - `LeesHoofdwaarde()` (Attention) geeft alleen true bij een nieuw ontvangen waarde, en heeft daarom geen tijdstempel.
- Signaalkwaliteit: `lastSignalQuality` bewaart de laatst ontvangen kwaliteit (code 0x02), met het tijdstip in `lastSignalQualityMS`. Bij de start is ze 200 ("geen contact"). Het kwaliteitsveld staat niet in elk pakket; volgens NeuroSky komt het ongeveer één keer per seconde.
- Attention, Meditation en de 8 banden tellen alleen als die laatste kwaliteit 0 is en hoogstens 3 s oud.
- `LeesMeditatie()` en `LeesEEGBanden()` geven ook false als de laatste kwaliteit niet 0 is of ouder dan 3 s, zodat een slechte of verouderde kwaliteit ook bewaarde waarden ongeldig maakt.
- Bij een ontvangen kwaliteit verschillend van 0 vervallen meteen de bewaarde Meditation, de ASIC-banden en het venster van de ruwe golf.
  Na herstel van het contact is eerst een nieuwe meting nodig; een oude waarde wordt niet opnieuw geldig.
- Een ruwe sample komt alleen in het venster als de laatste kwaliteit 0 is en hoogstens 3 s oud. `ReadEEGbandPowersFromRawWave()` geeft ook false bij een slechte of verouderde kwaliteit: zonder contact is de ruwe golf ruis.
  `ReadRawEEGsample()` blijft de ruwe sample geven, ook bij slecht contact: dat is het ruwe signaal, geen verwerkte meting.
- `LeesMeditatie()` zet bij false de waarde op NAN, zoals `LeesHoofdwaarde()`.
- `Activeren()` begint een nieuwe sessie: kwaliteit 200, geen Meditation, banden of ruwe golf, een leeg EEG-venster en een lege parser.
- Doorvoer (te meten grens): `LeesHoofdwaarde()` leest hoogstens 96 bytes per oproep; de TGAM stuurt bij 57600 baud tot ongeveer 5,8 kB per seconde.
  In theorie zijn dus minstens ongeveer 60 oproepen per seconde nodig. Pakketbursts, de grootte van de UART-buffer per board en de rekentijd van de EEG-DFT bepalen de werkelijke grens; die is nog niet gemeten.
- Een eSense-waarde 0 ("geen betrouwbare score") telt niet als meting.
- `LeesHoofdwaarde()` verwerkt ook de ruwe pakketten: roep zo vaak mogelijk aan.
- Voorbeeld: `Sensor_NeuroSkyTGAM_Verwerking.ino`.

## Data-kanalen

- Kanaal 11 tot en met 15: delta, theta, alfa, bèta en gamma uit de ruwe golf (float), bij een geslaagde aanroep van `ReadEEGbandPowersFromRawWave()`.
