# Hardwareondersteuning

## Huidige validatiestatus

Compilatieondersteuning en hardwarevalidatie zijn afzonderlijke statussen.

| Arduino Uno R3-vormfactorbord | Architectuur | Compilatiestatus in de repository | Hardwarestatus |
|---|---|---|---|
| Arduino UNO R3 | `avr` | voorbeelden opgenomen in de compilatiematrix; vier gekende geheugenbeperkingen bij de grootste gecombineerde sketch | getest en goedgekeurd sinds v1.0.0 |
| Arduino UNO R4 Minima | `renesas_uno` | voorbeelden opgenomen in de compilatiematrix | getest en goedgekeurd sinds v1.0.0 |
| Arduino UNO R4 WiFi | `renesas_uno` | voorbeelden opgenomen in de compilatiematrix | getest en goedgekeurd sinds v1.0.0; netwerkfuncties maken geen deel uit van de library |
| WEMOS D1 R32 via `esp32:esp32:d1_uno32` | `esp32` | voorbeelden opgenomen in de compilatiematrix | relevante hardwaretests blijven per opstelling vast te leggen |
| TTGO D1 R32 via `esp32:esp32:d1_uno32` | `esp32` | hetzelfde boardprofiel als WEMOS D1 R32; geen afzonderlijk fysiek testresultaat vastgelegd | fysieke hardwarevalidatie nog afzonderlijk vastleggen |

Arduino UNO R3, UNO R4 Minima en UNO R4 WiFi zijn sinds v1.0.0 getest en goedgekeurd. WEMOS D1 R32 en TTGO D1 R32 delen hetzelfde boardprofiel; dat bewijst niet automatisch dat elk van deze fysieke borden afzonderlijk is gevalideerd, en relevante hardwaretests blijven per opstelling vast te leggen.

De gemelde waarschuwing van `LiquidCrystal_I2C` betreft de architectuurmetadata van die externe library. Een Arduino Uno R3-vormfactorbord geldt pas als volledig hardwarematig gevalideerd wanneer de relevante voorbeelden op echte hardware zijn uitgevoerd en de resultaten zijn vastgelegd.

## Vereiste Arduino IDE-boardselecties

| Ondersteund board | Boards Manager package / core | Exacte boardselectie in Arduino IDE | Pinstrategie |
|---|---|---|---|
| Arduino Uno R3 | `Arduino AVR Boards by Arduino` | `Arduino Uno` | standaardmapping van de boardcore |
| Arduino UNO R4 Minima | `Arduino UNO R4 Boards by Arduino` | `Arduino UNO R4 Minima` | standaardmapping van de boardcore |
| Arduino UNO R4 WiFi | `Arduino UNO R4 Boards by Arduino` | `Arduino UNO R4 WiFi` | standaardmapping van de boardcore |
| WEMOS D1 R32 | `esp32 by Espressif Systems` | `WEMOS D1 R32` | specifieke `d1_uno32`-mapping van de Espressif-core |
| TTGO D1 R32 | `esp32 by Espressif Systems` | `WEMOS D1 R32` binnen het huidige ondersteunde profiel | dezelfde D1-R32-coremapping; fysieke hardwarevalidatie blijft afzonderlijk |
| Cytron Maker Uno RP2040 | `Raspberry Pi Pico/RP2040 by Earle F. Philhower, III` | `Cytron Maker Uno RP2040` | standaardmapping van de specifieke Cytron-boardvariant |
| STM32 Nucleo-F401RE | `STM32 MCU based boards by STMicroelectronics` | `Nucleo-64` met part number `NUCLEO_F401RE` | standaardmapping van STM32duino |
| SB Components Ardi-32 | `esp32 by Espressif Systems` | `ESP32S3 Dev Module` | de generieke boardselectie kent de fysieke Arduino-Uno-header van de Ardi-32 niet; `BOARD_VERSION BOARD_ARDI32` moet daarom via `UserConfig.h` expliciet gekozen worden en de eigen Ardi-32-headerpinmapping moet afzonderlijk vastgelegd worden |

Voor WEMOS D1 R32 gebruikt de GroeiAcademie FrameWork-library de door Espressif geleverde `D0` tot en met `D13`-namen. De core vertaalt deze zelf naar GPIO3, GPIO1, GPIO26, GPIO25, GPIO17, GPIO16, GPIO27, GPIO14, GPIO12, GPIO13, GPIO5, GPIO23, GPIO19 en GPIO18. `A0..A5` zijn respectievelijk GPIO2, GPIO4, GPIO35, GPIO34, GPIO36 en GPIO39. De afzonderlijke I2C-functies zijn `SDA=GPIO21` en `SCL=GPIO22`; `A4` en `A5` zijn op dit board dus niet de I2C-pinnen.


### SB Components Ardi-32 — eigen Arduino-Uno-headermapping

SB Components schrijft voor om in Arduino IDE de boardselectie `ESP32S3 Dev Module` uit `esp32 by Espressif Systems` te gebruiken. Die generieke boardvariant kent de fysieke Arduino-Uno-header van de Ardi-32 niet. Daarom gebruikt `SystemConfig.h` bij `BOARD_VERSION == BOARD_ARDI32` een eigen mapping.

| Arduino-header | ESP32-S3 GPIO |
|---|---:|
| D0 / RX | 44 |
| D1 / TX | 43 |
| D2 | 5 |
| D3 | 6 |
| D4 | 7 |
| D5 | 8 |
| D6 | 9 |
| D7 | 10 |
| D8 | 11 |
| D9 | 12 |
| D10 / SS | 13 |
| D11 / MOSI | 14 |
| D12 / MISO | 21 |
| D13 / SCK | 47 |
| A0 | 1 |
| A1 | 2 |
| A2 | 3 |
| A3 | 4 |
| A4 / SDA | 17 |
| A5 / SCL | 18 |

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

### GroeiAcademie Stimulus Hardware Shield v1.0.0

Voor het actuele shield zijn de schema-exporten, assemblagekeuzes en validatiestappen opgenomen onder `docs/Toepassingsgebieden/Stimulus/Hardware/`. Gebruik de [beschrijving van het GroeiAcademie Stimulus Hardware Shield v1.0.0](Toepassingsgebieden/Stimulus/Hardware/GroeiAcademie-Stimulus-Hardware-Shield-v1.0.0.md) samen met de [handleiding voor de hardwarevalidatie v1.0.0](Toepassingsgebieden/Stimulus/Hardware/Handleiding-GroeiAcademie-Stimulus-Hardware-Validatie-v1.0.0.md).

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

Deze waarden bevestigen niet automatisch de Arduino-Uno-header D0-D13/A0-A5-mapping. De volledige headermapping blijft daarom voorlopig als uitgeschakeld in `SystemConfig.h` totdat elke lijn rechtstreeks aan de officiële SB Components-hardwarebron is geverifieerd.

