# MR60BHA2

## Implementatie

- Broncode: `src/Systeem/Sensor/MR60BHA2.h` en `MR60BHA2.cpp`.
- C++-component: `MR60BHA2`, afgeleid van `Sensor`.
- Parent: `GedeeldeBusNode*` in de constructor.
- Componentcode: `MR60BHA2` (0xA2).
- Configuratie: `src/Configuratie/Sensoren/ConfigMR60BHA2_template.h`; de optionele persoonlijke versie heet telkens zonder `_template`.
- Data-kanalen: 3.

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


**Aanvulling aansluitpinnen en elektrische voorwaarden:**

- UART TX→RX en RX←TX; GND→GND; VCC→voeding volgens de eigen MR60BHA2-module. Eventuele status- of triggerlijnen uitsluitend aansluiten als de module ze aanbiedt. UART-logicaniveau controleren.
- De concrete modulevariant bepaalt de **exacte voedingsgrenzen en fysieke pinvolgorde**. Neem die gegevens niet over van een andere module zonder de werkelijke print te controleren.

**Aansluit- en gebruiksgegevens uit de bestaande FrameWork-interface:**

- **UART-bekabeling voor MR60BHA2:** `TX` van de sensor → `RX` van de gekozen host-/Extender-UART, en `RX` van de sensor → `TX` van die UART indien de gebruikte interface beide richtingen nodig heeft. `GND` moet gemeenschappelijk zijn.
- Controleer de UART-logicaniveaus én de vereiste baudrate van de fysieke module; een UART-pin is geen RS-232-niveau. Kies exact de door de constructor ondersteunde native seriële poort of UART-Extender-kanaal.
- De bestaande constructors ondersteunen de seriële poort met native TX/RX of, waar beschikbaar, een kanaal van een UART-Extender.
- Kruis de UART-signalen (sensor-TX naar host-RX) en controleer baudrate, voedingsspanning en elektrisch UART-niveau van de concrete radarmodule.

**Bestaande constructor (parameter-/pinvolgorde):**

```cpp
MR60BHA2(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareSerial* poort, HardwareResourcePin np_TX, HardwareResourcePin np_RX)
```

```cpp
MR60BHA2(GedeeldeBusNode* parent, GedeeldeBusComponent component, Stream* poort, HardwareResourcePin np_TX, HardwareResourcePin np_RX)
```

```cpp
MR60BHA2(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t channel)
```

**Configuratie voor deze hardware:**

- Gebruik bij een eigen configuratie `src/Configuratie/Sensoren/ConfigMR60BHA2.h` (het aanwezige `src/Configuratie/Sensoren/ConfigMR60BHA2_template.h` is een kopieersjabloon en wordt niet rechtstreeks ingelezen).
- Voor één aangesloten exemplaar: `SENSOR_MR60BHA2_AANTAL` op **1**, behalve wanneer de bestaande groeps-/setconfiguratie uitdrukkelijk een andere betekenis heeft.
- De bestaande configuratie bevat deze aansluitrelevante definities (gebruik de **werkelijke aangesloten** pinnen, adressen en referenties):
  - `SENSOR_MR60BHA2_AANTAL`: sjabloonwaarde `1`.

**Letterlijk constructorfragment uit de bestaande voorbeeldsketch** (alle gebruikte parent-objecten moeten zelf correct aangesloten en geïnitialiseerd zijn):

```cpp
GedeeldeBusNewComponent<MR60BHA2>(
      &Native, GedeeldeBusComponent::MR60BHA2, &GA_SENSOR_SERIAL, GA_SENSOR_SERIAL_PIN_TX, GA_SENSOR_SERIAL_PIN_RX);
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

- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_MR60BHA2/Sensor_MR60BHA2.ino`

**Elektrische aansluiting en randvoorwaarden volgens Seeed Studio (MR60BHA2-kit):**

- De fabrikant beschrijft de **MR60BHA2-radarmodule** en een afzonderlijke complete kit met XIAO ESP32C6. De huidige FrameWorkconstructor gebruikt een **seriële poort**; verwar die UART-verbinding niet met de voeding/headers van de volledige kit.
- Bij toepassing van de complete Seeed-kit is de officiële kitvoeding **5 V / 1 A**. Dit is **geen bevestigde voedingswaarde van een losse MR60BHA2-module**. Gebruik de seriële TX/RX-pinout van de werkelijk gemonteerde module.

**Technische handleiding:** https://wiki.seeedstudio.com/getting_started_with_mr60bha2_mmwave_kit/

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_MR60BHA2/`.
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.

### Aanvullende Sensor × Extender-testvoorbeelden

De onderstaande `.ino`-bestanden behoren tot de huidige v2.0.0-voorbeelden. Hun aanwezigheid is gecontroleerd; compilatie en fysieke hardwarevalidatie zijn hier nog **niet** aangetoond.

- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_MR60BHA2_MAX14830I2C/Sensor_MR60BHA2_MAX14830I2C.ino`
- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_MR60BHA2_MAX14830SPI/Sensor_MR60BHA2_MAX14830SPI.ino`
- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_MR60BHA2_SC16IS752I2C/Sensor_MR60BHA2_SC16IS752I2C.ino`
- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_MR60BHA2_SC16IS752SPI/Sensor_MR60BHA2_SC16IS752SPI.ino`

### Functionele Sensor × Extender-voorbeelden

De onderstaande voorbeelden bevatten **meetaanroepen na de activering**. Een getoonde waarde is geen bewijs van meetnauwkeurigheid of van een geslaagde hardwaretest.

- **Fysieke test:** Controleer ontvangen radarmeting, niet een zelf afgeleide hartslag.
- **Huidige reikwijdte:** de `.ino` gebruikt uitsluitend reeds bestaande FrameWork-functies; niet-beschikbare meetkanalen, kalibraties of externe functies zijn niet toegevoegd.

**Voorbeelden:**

- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_MR60BHA2_MAX14830I2C/Sensor_MR60BHA2_MAX14830I2C.ino`
- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_MR60BHA2_MAX14830SPI/Sensor_MR60BHA2_MAX14830SPI.ino`
- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_MR60BHA2_SC16IS752I2C/Sensor_MR60BHA2_SC16IS752I2C.ino`
- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_MR60BHA2_SC16IS752SPI/Sensor_MR60BHA2_SC16IS752SPI.ino`

**Validatie:** Arduino-compilatie, fysieke test, kalibratie en meetkwaliteitsvalidatie zijn hier nog niet aangetoond.

## Native meetvoorbeeld

`examples/Systeem/GedeeldeBus/Sensoren/Sensor_MR60BHA2/Sensor_MR60BHA2.ino` voert na de lifecycle daadwerkelijk uitlezingen uit via de bestaande Sensor-API.
Gemeten of weergegeven in dit voorbeeld: **Radarwaarde (eenheid volgens firmware)**.
De seriële uitvoer wordt beperkt; de Sensor-uitlezing blijft in `loop()` plaatsvinden.
Dit bevestigt bij geslaagde hardwaretests de ruwe of rechtstreeks beschikbare meetwaarde, niet automatisch alle fysische kanalen, kalibratie of nauwkeurigheid.
Compilatie en fysieke meting zijn nog niet bevestigd.

## Hulppinnen en routes via Extenders

- Via een CD74HC4067: alleen ontvangen. TX van de radar op een kanaal, Z = native RX; `np_TX` is NONE. Het kanaal blijft open.

## Ademhaling, aanwezigheid en module stil

- `ReadBreathingRatePerMinute(float&)`: frametype 0x0A14.
- `ReadPersonPresent(bool&)`: frametype 0x0F09 (vraagt recente Seeed-firmware).
- `IsModuleSilent()`: true als er langer dan `SENSOR_MR60BHA2_SILENT_TIMEOUT_MS` (standaard 3000) geen geldig frame was.
- Elk resultaat heeft een eigen tijdstip (`lastBreathRateMS`, `lastPersonPresentMS`).
- `ReadBreathingRatePerMinute()` en `ReadPersonPresent()` geven false als hun eigen resultaat ouder is dan `SENSOR_MR60BHA2_SILENT_TIMEOUT_MS`, ook als er wel frames van een ander type binnenkomen.
- `LeesHoofdwaarde()` verwerkt alle frames: roep die doorlopend aan.
- Voorbeeld: `Sensor_MR60BHA2_Verwerking.ino`.

## Data-kanalen

- Kanaal 1: radarwaarde (float), kanaal 2: ademfrequentie per minuut (float), kanaal 3: persoon aanwezig (int, 0 of 1).
- Kanaal 2 en 3 worden geschreven bij elk geldig frame, zolang de toepassing `LeesHoofdwaarde()` aanroept.

## Heractivatie

- `Activeren()` wist ook de ademhaling, de aanwezigheid en hun tijdstippen: geen resultaat uit een vorige sessie.
