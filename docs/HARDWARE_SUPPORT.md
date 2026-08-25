# Hardwareondersteuning

## Huidige validatiestatus

Compilatieondersteuning en hardwarevalidatie zijn afzonderlijke statussen.

| Arduino Uno R3-vormfactorbord | Architectuur | Compilatiestatus in de repository | Hardwarestatus |
|---|---|---|---|
| Arduino UNO R3 | `avr` | voorbeelden opgenomen in de compilatiematrix; vier gekende geheugenbeperkingen bij de grootste gecombineerde sketch | getest en goedgekeurd sinds v1.0.0 |
| Arduino UNO R4 Minima | `renesas_uno` | voorbeelden opgenomen in de compilatiematrix | getest en goedgekeurd sinds v1.0.0 |
| Arduino UNO R4 WiFi | `renesas_uno` | voorbeelden opgenomen in de compilatiematrix | getest en goedgekeurd sinds v1.0.0; netwerkfuncties maken geen deel uit van de library |
| WEMOS D1 R32 via `esp32:esp32:d1_uno32` | `esp32` | voorbeelden opgenomen in de compilatiematrix; compileert sinds v1.0.0 | hardwarematig nog niet bevestigd |
| TTGO D1 R32 via `esp32:esp32:d1_uno32` | `esp32` | hetzelfde boardprofiel als WEMOS D1 R32; geen afzonderlijk fysiek testresultaat vastgelegd | fysieke hardwarevalidatie nog afzonderlijk vastleggen |
| Cytron Maker Uno RP2040 | `rp2040` | minimale acceptatieregressie in `TestLibraryGereleased.cmd` | experimenteel toegevoegd in v1.1.0; nog niet fysiek hardwarematig gevalideerd |
| STMicroelectronics Nucleo-F401RE | `stm32` | minimale acceptatieregressie in `TestLibraryGereleased.cmd` | experimenteel toegevoegd in v1.1.0; nog niet fysiek hardwarematig gevalideerd |
| SB Components Ardi32 | `esp32` | minimale acceptatieregressie in `TestLibraryGereleased.cmd`; expliciete headermapping in `SystemConfig.h` | experimenteel toegevoegd in v1.1.0; nog niet fysiek hardwarematig gevalideerd |

Arduino UNO R3, UNO R4 Minima en UNO R4 WiFi zijn sinds v1.0.0 getest en goedgekeurd. WEMOS D1 R32 compileert sinds v1.0.0, maar is hardwarematig nog niet bevestigd. TTGO D1 R32 deelt hetzelfde boardprofiel; dat bewijst geen fysieke validatie. Cytron Maker Uno RP2040, STMicroelectronics Nucleo-F401RE en SB Components Ardi32 zijn vanaf v1.1.0 experimenteel opgenomen als acceptatieboards en zijn nog niet fysiek hardwarematig gevalideerd.

### HX1838-validatie v1.1.1

In v1.1.1 gaat HX1838 met `HX1838_BRON_CODES_DEFINE` van experimenteel naar released. Deze route is op de geteste hardwareopstelling met ontvanger op Arduino Uno-shieldpin D12 hardwarematig bevestigd: zowel TinyIRReceiver (`HX1838_USE_TINYIRRECEIVER_INSTEAD_OF_IRREMOTE = 1`) als IRremote (`= 0`) werken met deze configuratie. D12 is daarom de standaard `HX1838_ONTVANGER_PIN` in v1.1.1. De EEPROM-gebaseerde HX1838-routes blijven experimenteel.

De gemelde waarschuwing van de externe library `LiquidCrystal I2C` betreft de architectuurmetadata van die library. Een Arduino Uno R3-vormfactorbord geldt pas als volledig hardwarematig gevalideerd wanneer de relevante voorbeelden op echte hardware zijn uitgevoerd en de resultaten zijn vastgelegd.

## Vereiste Arduino IDE-boardselecties

| Boardnaam | FQBN | Platform-ID | Boards Manager package / core | Exacte boardselectie in Arduino IDE | Pinstrategie |
|---|---|---|---|---|---|
| Arduino UNO R3 | `arduino:avr:uno` | `arduino:avr` | `Arduino AVR Boards by Arduino` | `Arduino Uno` | standaardmapping van de boardcore |
| Arduino UNO R4 Minima | `arduino:renesas_uno:minima` | `arduino:renesas_uno` | `Arduino UNO R4 Boards by Arduino` | `Arduino UNO R4 Minima` | standaardmapping van de boardcore |
| Arduino UNO R4 WiFi | `arduino:renesas_uno:unor4wifi` | `arduino:renesas_uno` | `Arduino UNO R4 Boards by Arduino` | `Arduino UNO R4 WiFi` | standaardmapping van de boardcore |
| WeMos D1 R32 | `esp32:esp32:d1_uno32` | `esp32:esp32` | `esp32 by Espressif Systems` | `WEMOS D1 R32` | specifieke `d1_uno32`-mapping van de Espressif-core |
| TTGO D1 R32 | `esp32:esp32:d1_uno32` | `esp32:esp32` | `esp32 by Espressif Systems` | `WEMOS D1 R32` binnen het huidige ondersteunde profiel | dezelfde D1-R32-coremapping; fysieke hardwarevalidatie blijft afzonderlijk |
| Cytron Maker Uno RP2040 | `rp2040:rp2040:cytron_maker_uno_rp2040` | `rp2040:rp2040` | `Raspberry Pi Pico/RP2040/RP2350 by Earle F. Philhower, III` | `Cytron Maker Uno RP2040` | standaardmapping van de specifieke Cytron-boardvariant |
| STMicroelectronics Nucleo-F401RE | `STMicroelectronics:stm32:Nucleo_64:pnum=NUCLEO_F401RE` | `STMicroelectronics:stm32` | `STM32 MCU based boards by STMicroelectronics` | `Nucleo-64` met part number `NUCLEO_F401RE` | standaardmapping van STM32duino |
| SB Components Ardi32 | `esp32:esp32:esp32s3` | `esp32:esp32` | `esp32 by Espressif Systems` | `ESP32S3 Dev Module` | de generieke boardselectie kent de fysieke Arduino-Uno-header van de Ardi32 niet; `BOARD_VERSION BOARD_ARDI32` moet daarom via `UserConfig.h` expliciet gekozen worden; `SystemConfig.h` bevat de expliciete Ardi32-headerpinmapping |

### Referentiebeelden boards

| Boardnaam | Referentiebeeld(en) |
|---|---|
| Arduino UNO R3 | [Arduino UNO R3.jpg](Systeem/Screenshots/Arduino%20UNO%20R3.jpg) |
| Arduino UNO R4 Minima | [Arduino UNO R4 Minima.jpg](Systeem/Screenshots/Arduino%20UNO%20R4%20Minima.jpg) |
| Arduino UNO R4 WiFi | [Arduino UNO R4 WiFi.jpg](Systeem/Screenshots/Arduino%20UNO%20R4%20WiFi.jpg) |
| WeMos D1 R32 | [voorkant](Systeem/Screenshots/WeMos%20D1%20R32-voorkant.png) · [achterkant](Systeem/Screenshots/WeMos%20D1%20R32-achterkant.jpg) |
| Cytron Maker Uno RP2040 | [voorkant](Systeem/Screenshots/Cytron%20Maker%20Uno%20RP2040-voorkant.jpg) · [achterkant](Systeem/Screenshots/Cytron%20Maker%20Uno%20RP2040-achterkant.jpg) |
| STMicroelectronics Nucleo-F401RE | [voorkant](Systeem/Screenshots/STMicroelectronics%20Nucleo-F401RE-voorkant.webp) |
| SB Components Ardi32 | [voorkant](Systeem/Screenshots/SB%20Components%20Ardi32.jpeg) · [achterkant](Systeem/Screenshots/SB%20Components%20Ardi32-achterkant.jpg) |

### Additional Boards Manager URLs voor third-party boardplatforms

De standaard Arduino- en Espressif-platforms worden via de gekende package-indexen van Arduino IDE/Arduino CLI beheerd. Voor de twee third-party boardplatforms hieronder gebruikt de interne releasevalidatie expliciet deze aanvullende package-indexen wanneer automatische installatie nodig is:

| Boardplatform | Platform-ID | Additional Boards Manager URL |
|---|---|---|
| Cytron Maker Uno RP2040 | `rp2040:rp2040` | `https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json` |
| STMicroelectronics Nucleo-F401RE | `STMicroelectronics:stm32` | `https://github.com/stm32duino/BoardManagerFiles/raw/main/package_stmicroelectronics_index.json` |

De meegeleverde maintainer-tooling controleert de geïnstalleerde cores via `arduino-cli core list`. Wanneer een van deze twee third-party platforms ontbreekt en automatische installatie gekozen wordt, wordt eerst de overeenkomstige URL toegevoegd via `arduino-cli config add board_manager.additional_urls`, daarna de package-index bijgewerkt met `arduino-cli core update-index` en vervolgens het platform geïnstalleerd met `arduino-cli core install`. De gedeelde Windows-testscripts worden mee gepubliceerd; alleen het machinespecifieke `extras/LokalePaden.cmd` blijft via `.gitignore` lokaal.

Voor WEMOS D1 R32 gebruikt de GroeiAcademie FrameWork-library de door Espressif geleverde `D0` tot en met `D13`-namen. De core vertaalt deze zelf naar GPIO3, GPIO1, GPIO26, GPIO25, GPIO17, GPIO16, GPIO27, GPIO14, GPIO12, GPIO13, GPIO5, GPIO23, GPIO19 en GPIO18. `A0..A5` zijn respectievelijk GPIO2, GPIO4, GPIO35, GPIO34, GPIO36 en GPIO39. De afzonderlijke I2C-functies zijn `SDA=GPIO21` en `SCL=GPIO22`; `A4` en `A5` zijn op dit board dus niet de I2C-pinnen.


### SB Components Ardi-32 — eigen Arduino-Uno-headermapping

SB Components gebruikt in Arduino IDE de generieke boardselectie `ESP32S3 Dev Module`. Die boardvariant kent de fysieke Arduino-Uno-header van de Ardi-32 niet. `SystemConfig.h` is daarom voor de library de bron van waarheid: bij `BOARD_VERSION == BOARD_ARDI32` gebruikt de code onderstaande expliciete headerpinmapping.

| Arduino-header | ESP32-S3 GPIO |
|---|---:|
| D0 / RX | 18 |
| D1 / TX | 17 |
| D2 | 1 |
| D3 | 42 |
| D4 | 41 |
| D5 | 2 |
| D6 | 9 |
| D7 | 14 |
| D8 | 47 |
| D9 | 21 |
| D10 / SS | 10 |
| D11 / MOSI | 11 |
| D12 / MISO | 13 |
| D13 / SCK | 12 |
| A0 | 4 |
| A1 | 5 |
| A2 | 6 |
| A3 | 7 |
| A4 | 15 |
| A5 | 16 |
| SDA | 38 |
| SCL | 39 |

Omdat de fabrikant een generieke `ESP32S3 Dev Module` laat selecteren, kan de Arduino-toolchain de Ardi-32 niet automatisch als `BOARD_ARDI32` herkennen. Zet daarom voor dit board expliciet `#define BOARD_VERSION BOARD_ARDI32` in `UserConfig.h`. Een instelling in `UserConfig.h` heeft voorrang op automatische boarddetectie.

## ADC-resolutie per Arduino Uno R3-vormfactorbord

| Arduino Uno R3-vormfactorbord | Interne ADC-resolutie | Opmerking |
|---|---|---|
| Arduino UNO R3 | 10-bit | zeer lineair, geen hoogfrequente storing |
| Arduino UNO R4 Minima/WiFi | 14-bit (standaard 10-bit, instelbaar via `analogReadResolution(14)`) | WiFi-variant heeft iets meer stralingsruis; bron: docs.arduino.cc |
| WEMOS D1 R32 / TTGO D1 R32 | 12-bit | sterk niet-lineair (S-curve), dead-zones onder 0,1V en boven 3,2V zonder externe ADC |

Voor precisiemetingen wordt de externe **ADS1115** (16-bit, I2C) aanbevolen boven elk van de bovenstaande interne ADC's — zie `examples/Systeem/ADC_Validatie/ADC_Validatie_ADS1115/`.

## Configuratie

Kopieer voor persoonlijke instellingen `src/Configuratie/UserConfig_template.h` vóór gebruik naar `src/Configuratie/UserConfig.h`. Zonder `UserConfig.h` gebruikt de library de fallback uit `SystemConfig.h`.

```cpp
#define BOARD_VERSION BOARD_UNO_R3
```

Mogelijke waarden:

```cpp
#define BOARD_UNO_R3                     0
#define BOARD_UNO_R4_MINIMA              1
#define BOARD_UNO_R4_WIFI                2
#define BOARD_ARDI32                     3
#define BOARD_CYTRON_MAKER_UNO_RP2040    4
#define BOARD_ESP32_UNO                  5
#define BOARD_NUCLEO_F401RE              6
```

De configuratie bepaalt onder andere `ADC_BITS`, `ADC_MAX`, `DELAY_US` en de schaalmacro `ADC(x)`.

## Geheugen

UNO R3 is de strengste huidige grens. Met uitgebreide functionaliteit kan de grootste gecombineerde Stimulus-sketch de beschikbare flash overschrijden wanneer PixelScreen-code meegecompileerd wordt. Exact gebruik kan wijzigen bij code-, core- of dependencyversies en moet bij releases opnieuw worden gemeten.

## Huidige externe hardware

### Characterscherm

De standaard Screen-laag gebruikt een I2C-characterscherm via `LiquidCrystal_I2C`.

Standaardconfiguratie:

```cpp
#define I2C_ADRES 0x27
#define ACTIEF_CHARACTER_SCREEN SCREEN_LCD1602
```

Controleer het werkelijke I2C-adres van de module. Veelvoorkomende adressen zijn `0x27` en `0x3F`.

### PixelScreen

PixelScreen wordt alleen gecompileerd wanneer `SCREEN_TYPE_PIXELS` in `SCREEN_OUTPUT` staat. De algemene Screen-laag gebruikt `Adafruit_GFX`; de concrete displaydriver wordt door de toepassing geïnitialiseerd. Voor een ST7789 op Arduino UNO zijn `D11` (MOSI) en `D13` (SCK) de hardware-SPI-pinnen. `CS`, `DC` en `RST` zijn configureerbaar.

Voor de experimentele STMicroelectronics Nucleo-F401RE-acceptatiebuild is een externe dependencybeperking gekend: `Adafruit ST7735 and ST7789 Library` kan bij de gebruikte STM32-core stoppen op `wiring_private.h: No such file or directory`. De maintainer-test classificeert uitsluitend die specifieke fout als verwachte dependencybeperking; dit is geen bevestigde PixelScreen-ondersteuning op Nucleo-F401RE en andere compilefouten blijven acceptatiefouten.

Controleer de voeding en logicaniveaus van de concrete displaymodule. Bij de universele shieldopbouw wordt ofwel de levelshifter geplaatst, ofwel worden de overeenkomstige draadbruggen geplaatst. Plaats nooit beide tegelijk.

### Stimulus-sensoren

`AANTAL_SENSOREN_AANWEZIG` ondersteunt uitsluitend twee of vier aangesloten sensoren. Iedere andere configuratiewaarde veroorzaakt een compilerfout.

De huidige standaardpinnen zijn:

| Sensor | Pin |
|---|---|
| sensor 1 | `A0` |
| sensor 2 | `A1` |
| sensor 3 | `A2` |
| sensor 4 | `A3` |

De volledige aansluiting staat in [Toepassingsgebieden/Stimulus/README.md](Toepassingsgebieden/Stimulus/README.md).

### GroeiAcademie Stimulus Shield v1.1.1

Stimulus Shield v1.1.1 is de volledige opvolger van Stimulus Shield v1.1.0 en bevat de volledige v1.1.0-hardwarebasis plus de HX1838 IR Receiver met signaal op Arduino Uno-shieldpin `D12`. De volledige v1.1.1-tekeningset staat in de map Stimulus Shield v1.1.1. Gebruik de [beschrijving van Stimulus Shield v1.1.1](Uitbreidingskaarten/Stimulus%20Shield%20v1.1.1/Stimulus-Shield-%28GroeiAcademie-FrameWork%29-v1.1.1.md) samen met de [hardwarevalidatie v1.1.1](Uitbreidingskaarten/Stimulus%20Shield%20v1.1.1/Handleiding-Stimulus-Shield-%28GroeiAcademie-FrameWork%29-v1.1.1.md).

De TFT-route gebruikt ofwel de Quad Logic Level Shifters, ofwel zeven draadbruggen: zes tussen H9 en H10 en één tussen H3 pin 1 en H4 pin 1. Plaats nooit beide tegelijk.

## ESP32

Voor `BOARD_ESP32_UNO` is de configuratie voorbereid op 12-bit ADC-uitlezing en `DELAY_US 0`. Dit voegt vanuit de GroeiAcademie FrameWork-library geen extra wachttijd tussen samples toe. ESP32-borden met dit profiel gebruiken 3,3 V-logica. Controleer bij TTGO D1 R32 en andere compatibele borden voeding, analoge ingangsspanning, shieldcompatibiliteit, timing, geheugen, callbacks en dependencies tijdens hun afzonderlijke hardwarevalidatie. Netwerkfuncties maken geen deel uit van de huidige library.

## Sensorbeoordeling

Een sensor wordt per meetdoel beoordeeld op:

- betrouwbaarheid;
- resolutie en bereik;
- samplegedrag;
- kalibratie;
- comfort en plaatsing;
- mechanische reproduceerbaarheid;
- elektrische veiligheid;
- compatibiliteit met de ADC en voedingsspanning van het Arduino Uno R3-vormfactorbord.

Geschiktheid voor één meetdoel impliceert geen geschiktheid voor een ander meetdoel.


### SB Components Ardi-32 — bevestigde onboard GPIO's

De officiële SB Components-documentatie bevestigt voor de onboard randapparatuur:

- onboard LED: GPIO8
- onboard buzzer: GPIO40

De Arduino-Uno-header D0-D13/A0-A5/SDA/SCL-mapping is intussen rechtstreeks aan het officiële Ardi-32-schema geverifieerd en actief vastgelegd in `SystemConfig.h`. De bevestigde onboard GPIO's hierboven staan daar los van. De fysieke hardwarevalidatie van het board blijft een afzonderlijke status.

### Acceptance-validatie SB Components Ardi32

De acceptance-compilaties voor `SB Components Ardi32` gebruiken `esp32:esp32:esp32s3` met `BOARD_VERSION=BOARD_ARDI32` en daarmee de actieve Ardi32-headerpinmapping uit `SystemConfig.h`.

Deze acceptance-tests controleren de compileerbaarheid van de experimentele Ardi32-ondersteuning. Zij hebben geen release-impact zolang de fysieke hardwarevalidatie van dit board nog niet is afgerond.

