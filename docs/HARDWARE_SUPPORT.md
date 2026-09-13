# Hardwareondersteuning

## Huidige validatiestatus

Compilatieondersteuning en hardwarevalidatie zijn afzonderlijke controles. De statuskolom gebruikt uitsluitend de twee afgesproken projectstatussen.

| Arduino Uno R3-vormfactorbord | Architectuur | Compilatiestatus in de repository | Status |
|---|---|---|---|
| Arduino UNO R3 | `avr` | opgenomen in de compilatiematrix | geïmplementeerd en getest |
| Arduino UNO R4 Minima | `renesas_uno` | opgenomen in de compilatiematrix | geïmplementeerd en getest |
| Arduino UNO R4 WiFi | `renesas_uno` | opgenomen in de compilatiematrix | geïmplementeerd en getest |
| Arduino UNO Q | `zephyr` | acceptatiecompilaties via `arduino:zephyr:unoq` | geïmplementeerd en getest |
| Cytron Maker UNO RP2040 | `rp2040` | acceptatiecompilaties via `rp2040:rp2040:cytron_maker_uno_rp2040` | geïmplementeerd en getest |
| Paradisetronic ESP32-S3 UNO (ESP32-S3-WROOM-1) | `esp32` | acceptatiecompilaties via `esp32:esp32:esp32s3` met `BOARD_ESP32S3_DEV` | geïmplementeerd en getest |
| SB Components Ardi-32 (ESP32-S3-WROOM-1) | `esp32` | acceptatiecompilaties via `esp32:esp32:esp32s3` met `BOARD_ESP32S3_ARDI32` | geïmplementeerd en getest |
| STM32F4 Nucleo-F401RE | `stm32` | acceptatiecompilaties via `STMicroelectronics:stm32:Nucleo_64:pnum=NUCLEO_F401RE` | geïmplementeerd en getest |
| TTGO D1 R32 (ESP32-WROOM-32U) | `esp32` | opgenomen via `esp32:esp32:d1_uno32` | geïmplementeerd en getest |
| WeMos D1 R32 (ESP32-WROOM-32U) | `esp32` | opgenomen via `esp32:esp32:d1_uno32` | geïmplementeerd en getest |

Voor de dependencycontrole delen WeMos D1 R32 (ESP32-WROOM-32U) en TTGO D1 R32 (ESP32-WROOM-32U) de technische aanduiding `ESP32-WROOM-32U` en dezelfde FQBN `esp32:esp32:d1_uno32`. Paradisetronic ESP32-S3 UNO (ESP32-S3-WROOM-1) en SB Components Ardi-32 (ESP32-S3-WROOM-1) delen in diezelfde dependencycontrole `ESP32-S3-WROOM-1`, FQBN `esp32:esp32:esp32s3` en het package `esp32 by Espressif Systems`.

### HX1838-validatie

HX1838 met `HX1838_BRON_CODES_DEFINE` is released. Zowel TinyIRReceiver (`HX1838_USE_TINYIRRECEIVER_INSTEAD_OF_IRREMOTE = 1`) als IRremote (`= 0`) zijn hardwarematig bevestigd. De standaard `HX1838_ONTVANGER_PIN` is Arduino Uno-shieldpin `D8`. De standaard `PIXEL_SCREEN_RST` is Arduino Uno-shieldpin `D7`. De EEPROM-gebaseerde HX1838-routes blijven experimenteel.

De gemelde waarschuwing van de externe library `LiquidCrystal I2C` betreft de architectuurmetadata van die library. Een Arduino Uno R3-vormfactorbord geldt pas als volledig hardwarematig gevalideerd wanneer de relevante voorbeelden op echte hardware zijn uitgevoerd en de resultaten zijn vastgelegd.

## Vereiste Arduino IDE-boardselecties

| Boardnaam | FQBN | Platform-ID | Boards Manager package / core | Exacte boardselectie in Arduino IDE | Pinstrategie |
|---|---|---|---|---|---|
| Arduino UNO R3 | `arduino:avr:uno` | `arduino:avr` | `Arduino AVR Boards by Arduino` | `Arduino Uno` | standaardmapping van de boardcore |
| Arduino UNO R4 Minima | `arduino:renesas_uno:minima` | `arduino:renesas_uno` | `Arduino UNO R4 Boards by Arduino` | `Arduino UNO R4 Minima` | standaardmapping van de boardcore |
| Arduino UNO R4 WiFi | `arduino:renesas_uno:unor4wifi` | `arduino:renesas_uno` | `Arduino UNO R4 Boards by Arduino` | `Arduino UNO R4 WiFi` | standaardmapping van de boardcore |
| Arduino UNO Q | `arduino:zephyr:unoq` | `arduino:zephyr` | `Arduino Zephyr Boards by Arduino` | `Arduino UNO Q` | `SystemConfig.h` gebruikt `BOARD_UNO_Q` |
| Cytron Maker UNO RP2040 | `rp2040:rp2040:cytron_maker_uno_rp2040` | `rp2040:rp2040` | `Raspberry Pi Pico/RP2040/RP2350 by Earle F. Philhower, III` | `Cytron Maker UNO RP2040` | expliciete mapping in `SystemConfig.h` |
| Paradisetronic ESP32-S3 UNO (ESP32-S3-WROOM-1) | `esp32:esp32:esp32s3` | `esp32:esp32` | `esp32 by Espressif Systems` | `ESP32S3 Dev Module` | `BOARD_ESP32S3_DEV`; huidige automatische detectie gebruikt `ARDUINO_ESP32S3_DEV` |
| SB Components Ardi-32 (ESP32-S3-WROOM-1) | `esp32:esp32:esp32s3` | `esp32:esp32` | `esp32 by Espressif Systems` | `ESP32S3 Dev Module` | `BOARD_ESP32S3_ARDI32` expliciet kiezen wanneer de generieke boardselectie wordt gebruikt |
| STM32F4 Nucleo-F401RE | `STMicroelectronics:stm32:Nucleo_64:pnum=NUCLEO_F401RE` | `STMicroelectronics:stm32` | `STM32 MCU based boards by STMicroelectronics` | `Nucleo-64` met part number `NUCLEO_F401RE` | expliciete mapping in `SystemConfig.h` |
| TTGO D1 R32 (ESP32-WROOM-32U) | `esp32:esp32:d1_uno32` | `esp32:esp32` | `esp32 by Espressif Systems` | `WeMos D1 R32` binnen het gedeelde profiel | gedeelde `BOARD_ESP32_D1_UNO_R32`-mapping |
| WeMos D1 R32 (ESP32-WROOM-32U) | `esp32:esp32:d1_uno32` | `esp32:esp32` | `esp32 by Espressif Systems` | `WeMos D1 R32` | gedeelde `BOARD_ESP32_D1_UNO_R32`-mapping |

### Paradisetronic ESP32-S3 UNO (ESP32-S3-WROOM-1): Arduino IDE

De Paradisetronic ESP32-S3 UNO (ESP32-S3-WROOM-1) is op echte hardware getest en werkend met `BOARD_ESP32S3_DEV`. Voor de seriële uitvoer gebruikt het FrameWork op dit board `GA_SERIAL`, dat naar `Serial0` verwijst.

**De Juiste Boardinstellingen Selecteren**

Ga in de Arduino IDE naar **Tools** (Hulpmiddelen) en neem de volgende instellingen exact over:

- **Board:** Kies **ESP32S3 Dev Module**
- **Flash Size:** Kies **16MB (128Mb)**
- **Partition Scheme:** Kies **16MB Flash (3MB APP/9.9MB FATFS)**
- **PSRAM:** Kies **OPI PSRAM** ⚠️ *(Cruciaal! Jouw N16R8 module gebruikt Octal/OPI RAM).*
- **USB CDC On Boot:** Kies **Enabled** *(Zorgt ervoor dat de seriële monitor werkt via de USB-C poort).*

**Belangrijk bij uploaden met PixelScreen**

Bij gebruik van een PixelScreen is **jumper U11 op pin 9 de enige oplossing die we hiervoor aanhouden**. Open jumper U11 tijdens het uploaden en sluit jumper U11 opnieuw na het uploaden.

### Referentiebeelden boards

| Boardnaam | Referentiebeeld(en) |
|---|---|
| Arduino UNO R3 | [Arduino UNO R3.jpg](Systeem/Screenshots/Arduino%20UNO%20R3.jpg) |
| Arduino UNO R4 Minima | [Arduino UNO R4 Minima.jpg](Systeem/Screenshots/Arduino%20UNO%20R4%20Minima.jpg) |
| Arduino UNO R4 WiFi | [Arduino UNO R4 WiFi.jpg](Systeem/Screenshots/Arduino%20UNO%20R4%20WiFi.jpg) |
| Cytron Maker UNO RP2040 | [voorkant](Systeem/Screenshots/Cytron%20Maker%20Uno%20RP2040-voorkant.jpg) · [achterkant](Systeem/Screenshots/Cytron%20Maker%20Uno%20RP2040-achterkant.jpg) |
| SB Components Ardi-32 (ESP32-S3-WROOM-1) | [voorkant](Systeem/Screenshots/SB%20Components%20Ardi32.jpeg) · [achterkant](Systeem/Screenshots/SB%20Components%20Ardi32-achterkant.jpg) |
| STM32F4 Nucleo-F401RE | [voorkant](Systeem/Screenshots/STMicroelectronics%20Nucleo-F401RE-voorkant.webp) |
| WeMos D1 R32 (ESP32-WROOM-32U) | [voorkant](Systeem/Screenshots/WeMos%20D1%20R32-voorkant.png) · [achterkant](Systeem/Screenshots/WeMos%20D1%20R32-achterkant.jpg) |

### Additional Boards Manager URLs voor third-party boardplatforms

De standaard Arduino- en Espressif-platforms worden via de gekende package-indexen van Arduino IDE/Arduino CLI beheerd. Voor de twee third-party boardplatforms hieronder gebruikt de interne releasevalidatie expliciet deze aanvullende package-indexen wanneer automatische installatie nodig is:

| Boardplatform | Platform-ID | Additional Boards Manager URL |
|---|---|---|
| Cytron Maker UNO RP2040 | `rp2040:rp2040` | `https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json` |
| STM32F4 Nucleo-F401RE | `STMicroelectronics:stm32` | `https://github.com/stm32duino/BoardManagerFiles/raw/main/package_stmicroelectronics_index.json` |

De meegeleverde maintainer-tooling controleert de geïnstalleerde cores via `arduino-cli core list`. Wanneer een van deze twee third-party platforms ontbreekt en automatische installatie gekozen wordt, wordt eerst de overeenkomstige URL toegevoegd via `arduino-cli config add board_manager.additional_urls`, daarna de package-index bijgewerkt met `arduino-cli core update-index` en vervolgens het platform geïnstalleerd met `arduino-cli core install`. De gedeelde Windows-testscripts worden mee gepubliceerd; alleen het machinespecifieke `extras/LokalePaden.cmd` blijft via `.gitignore` lokaal.

Voor WeMos D1 R32 (ESP32-WROOM-32U) en TTGO D1 R32 (ESP32-WROOM-32U) gebruikt de GroeiAcademie FrameWork-library de door Espressif geleverde `D0` tot en met `D13`-namen. De core vertaalt deze zelf naar GPIO3, GPIO1, GPIO26, GPIO25, GPIO17, GPIO16, GPIO27, GPIO14, GPIO12, GPIO13, GPIO5, GPIO23, GPIO19 en GPIO18. `A0..A5` zijn respectievelijk GPIO2, GPIO4, GPIO35, GPIO34, GPIO36 en GPIO39. De afzonderlijke I2C-functies zijn `SDA=GPIO21` en `SCL=GPIO22`; `A4` en `A5` zijn op dit board dus niet de I2C-pinnen.


### SB Components Ardi-32 (ESP32-S3-WROOM-1): eigen Arduino-Uno-headermapping

Wanneer er geen COM-poort verschijnt, moet mogelijk de **CP2102-driver** voor het gebruikte besturingssysteem worden geïnstalleerd.

SB Components gebruikt in Arduino IDE de generieke boardselectie `ESP32S3 Dev Module`. Die boardvariant kent de fysieke Arduino-Uno-header van de Ardi-32 niet. `SystemConfig.h` is daarom voor de library de bron van waarheid: bij `BOARD_VERSION == BOARD_ESP32S3_ARDI32` gebruikt de code onderstaande expliciete headerpinmapping.

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

Omdat de fabrikant een generieke `ESP32S3 Dev Module` laat selecteren, kan de Arduino-toolchain de Ardi-32 niet automatisch als `BOARD_ESP32S3_ARDI32` herkennen. Zet daarom voor dit board expliciet `#define BOARD_VERSION BOARD_ESP32S3_ARDI32` in `UserConfig.h`. Een instelling in `UserConfig.h` heeft voorrang op automatische boarddetectie.

### STM32F4 Nucleo-F401RE: Arduino-Uno-shieldheader

Voor `BOARD_VERSION == BOARD_STM32F4_NUCLEO64_F401RE` is de Arduino Uno-shieldheader in `SystemConfig.h` actief vastgelegd met de STM32duino/Arduino-namen: D0-D13, A0-A5, SDA/SCL en SS/MOSI/MISO/SCK. De bijbehorende MCU-pinnen zijn in de mapping als commentaar opgenomen (onder meer D0/PA3, D1/PA2, D11/PA7, D12/PA6, D13/PA5, SDA/PB9 en SCL/PB8). Iedere `ARDUINO_UNO_SHIELD_PIN_...`-fallback is afzonderlijk met `#ifndef` beschermd en kan daardoor per pin vanuit `UserConfig.h` worden overschreven.

Hardwarevalidatie voor dit board:

| Onderdeel | Status |
|---|---|
| CharacterDisplay | getest en werkt |
| ADS1115 | getest en werkt |
| PCF8574 | getest en werkt |
| PixelScreen | getest en werkt |
| HX1838 | getest en werkt |

#### Arduino IDE-boardselectie

Gebruik voor de Nucleo F401RE **niet** `Generic STM32 series`. Selecteer:

- **Tools > Board > STM32MCU based boards > Nucleo-64**
- **Tools > Board part number > Nucleo F401RE**

De overeenkomstige Arduino CLI/FQBN-configuratie is:

`STMicroelectronics:stm32:Nucleo_64:pnum=NUCLEO_F401RE`

Dezezelfde FQBN wordt ook door de meegeleverde testscripts gebruikt. Bij selectie via `Generic STM32 series` ontbreekt de specifieke Nucleo-boardconfiguratie en kunnen boardfuncties, waaronder seriële communicatie via de boardconfiguratie, anders of onjuist werken.

#### PixelScreen: Adafruit ST7735/ST7789 librarypatch voor STM32

Voor de geteste PixelScreen-opstelling op de Nucleo F401RE was in de externe Adafruit-library een kleine compatibiliteitsaanpassing nodig in:

`C:\Users\alexp\Documents\Arduino\libraries\Adafruit_ST7735_and_ST7789_Library\Adafruit_ST77xx.cpp`

Zoek bovenaan naar:

```cpp
#if !defined(ARDUINO_STM32_FEATHER) && !defined(ARDUINO_UNOR4_WIFI)
#if !defined(ARDUINO_UNOR4_MINIMA)
#include "pins_arduino.h"
#include "wiring_private.h"
#endif
#endif
```

Vervang uitsluitend de eerste `#if`-regel door:

```cpp
#if !defined(ARDUINO_STM32_FEATHER) && !defined(ARDUINO_UNOR4_WIFI) && !defined(ARDUINO_ARCH_STM32)
```

Het volledige blok wordt dan:

```cpp
#if !defined(ARDUINO_STM32_FEATHER) && !defined(ARDUINO_UNOR4_WIFI) && !defined(ARDUINO_ARCH_STM32)
#if !defined(ARDUINO_UNOR4_MINIMA)
#include "pins_arduino.h"
#include "wiring_private.h"
#endif
#endif
```

De wijziging bestaat dus uitsluitend uit het achteraan toevoegen van `&& !defined(ARDUINO_ARCH_STM32)` aan de eerste `#if`-regel. Dit is een wijziging in een **externe library**, niet in de GroeiAcademie FrameWork-broncode; na een update van de Adafruit-library moet gecontroleerd worden of deze lokale patch nog nodig is of opnieuw moet worden toegepast.

Het pakket bevat daarnaast de gebruikte Nucleo-F401RE-referentie onder `docs/ArduinoUnoVormfactorBoards/`.

## ADC-resolutie per Arduino Uno R3-vormfactorbord

| Arduino Uno R3-vormfactorbord | Interne ADC-resolutie volgens `SystemConfig.h` |
|---|---:|
| Arduino UNO R3 | 10-bit |
| Arduino UNO R4 Minima | 14-bit |
| Arduino UNO R4 WiFi | 14-bit |
| Arduino UNO Q | 10-bit |
| Cytron Maker UNO RP2040 | 10-bit |
| Paradisetronic ESP32-S3 UNO (ESP32-S3-WROOM-1) | 12-bit |
| SB Components Ardi-32 (ESP32-S3-WROOM-1) | 12-bit |
| STM32F4 Nucleo-F401RE | 10-bit |
| TTGO D1 R32 (ESP32-WROOM-32U) | 12-bit |
| WeMos D1 R32 (ESP32-WROOM-32U) | 12-bit |

Voor precisiemetingen kan de externe **ADS1115** worden gebruikt. `SystemConfig.h` gebruikt daarvoor `ADC_BITS 15` voor het single-ended bereik. Zie `examples/Systeem/ADC_Validatie/ADC_Validatie_ADS1115/`.

## Configuratie

Kopieer voor persoonlijke instellingen `src/Configuratie/UserConfig_template.h` vóór gebruik naar `src/Configuratie/UserConfig.h`. Zonder `UserConfig.h` gebruikt de library de fallback uit `SystemConfig.h`.

```cpp
#define BOARD_VERSION BOARD_UNO_R3
```

Mogelijke waarden:

```cpp
#define BOARD_UNO_R3                       0
#define BOARD_UNO_R4_MINIMA                1
#define BOARD_UNO_R4_WIFI                  2
#define BOARD_UNO_Q                        7
#define BOARD_ESP32_D1_UNO_R32             5
#define BOARD_ESP32S3_ARDI32               3
#define BOARD_ESP32S3_DEV                  8
#define BOARD_RP2040_CYTRON_MAKER_UNO      4
#define BOARD_STM32F4_NUCLEO64_F401RE      6
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

Voor de STM32F4 Nucleo-F401RE-acceptatiebuild is een externe dependencybeperking gekend: `Adafruit ST7735 and ST7789 Library` kan bij de gebruikte STM32-core stoppen op `wiring_private.h: No such file or directory`. De maintainer-test classificeert uitsluitend die specifieke fout als verwachte dependencybeperking; dit is geen bevestigde PixelScreen-ondersteuning op Nucleo-F401RE en andere compilefouten blijven acceptatiefouten.

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

### GroeiAcademie Stimulus Shield v1.1.2

Stimulus Shield v1.1.2 is de actuele en enige geldige hardwareversie in deze release. De HX1838 DATA-lijn gebruikt Arduino Uno-shieldpin `D8` en de PixelScreen-resetlijn gebruikt `D7`. De volledige v1.1.2-tekeningset staat in de map Stimulus Shield v1.1.2. Gebruik de [beschrijving van Stimulus Shield v1.1.2](Uitbreidingskaarten/Stimulus%20Shield%20v1.1.2/Stimulus-Shield-%28GroeiAcademie-FrameWork%29-v1.1.2.md) samen met de [hardwarevalidatie v1.1.2](Uitbreidingskaarten/Stimulus%20Shield%20v1.1.2/Handleiding-Stimulus-Shield-%28GroeiAcademie-FrameWork%29-v1.1.2.md).

De TFT-route gebruikt ofwel de Quad Logic Level Shifters, ofwel zeven draadbruggen: zes tussen H9 en H10 en één tussen H3 pin 1 en H4 pin 1. Plaats nooit beide tegelijk.

## ESP32

Voor `BOARD_ESP32_D1_UNO_R32` is de configuratie voorbereid op 12-bit ADC-uitlezing en `DELAY_US 0`. Dit voegt vanuit de GroeiAcademie FrameWork-library geen extra wachttijd tussen samples toe. ESP32-borden met dit profiel gebruiken 3,3 V-logica. TTGO D1 R32 (ESP32-WROOM-32U) en WeMos D1 R32 (ESP32-WROOM-32U) zijn geïmplementeerd en getest. In de dependencycontrole worden ze gezamenlijk aangeduid als `ESP32-WROOM-32U`. Controleer bij andere compatibele borden voeding, analoge ingangsspanning, shieldcompatibiliteit, timing, geheugen, callbacks en dependencies tijdens hun afzonderlijke hardwarevalidatie. Netwerkfuncties maken geen deel uit van de huidige library.

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


### SB Components Ardi-32 (ESP32-S3-WROOM-1): bevestigde onboard GPIO's

De officiële SB Components-documentatie bevestigt voor de onboard randapparatuur:

- onboard LED: GPIO8
- onboard buzzer: GPIO40

De Arduino-Uno-header D0-D13/A0-A5/SDA/SCL-mapping is intussen rechtstreeks aan het officiële Ardi-32-schema geverifieerd en actief vastgelegd in `SystemConfig.h`. De bevestigde onboard GPIO's hierboven staan daar los van. SB Components Ardi-32 (ESP32-S3-WROOM-1) is fysiek geïmplementeerd en getest.

### Acceptance-validatie SB Components Ardi-32 (ESP32-S3-WROOM-1)

De acceptance-compilaties voor SB Components Ardi-32 (ESP32-S3-WROOM-1) gebruiken `esp32:esp32:esp32s3` met `BOARD_VERSION=BOARD_ESP32S3_ARDI32` en daarmee de actieve Ardi32-headerpinmapping uit `SystemConfig.h`.

Deze acceptance-tests controleren de compileerbaarheid van de Ardi-32-ondersteuning. De fysieke hardwarevalidatie van SB Components Ardi-32 (ESP32-S3-WROOM-1) is afgerond; de boardstatus is geïmplementeerd en getest.

