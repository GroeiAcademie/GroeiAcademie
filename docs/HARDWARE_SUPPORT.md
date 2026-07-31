# Hardwareondersteuning

## Huidige validatiestatus

Compilatieondersteuning en hardwarevalidatie zijn afzonderlijke statussen.

| Board | Architectuur | Compilatiestatus in de repository | Hardwarestatus |
|---|---|---|---|
| Arduino UNO R3 | `avr` | voorbeelden opgenomen in de compilatiematrix; vier gekende geheugenbeperkingen bij de grootste gecombineerde sketch | relevante hardwaretests blijven per opstelling vast te leggen |
| Arduino UNO R4 Minima | `renesas_uno` | voorbeelden opgenomen in de compilatiematrix | relevante hardwaretests blijven per opstelling vast te leggen |
| Arduino UNO R4 WiFi | `renesas_uno` | voorbeelden opgenomen in de compilatiematrix | relevante hardwaretests blijven per opstelling vast te leggen; netwerkfuncties maken geen deel uit van de library |
| Wemos D1 R32 via ESP32 Dev Module | `esp32` | voorbeelden opgenomen in de compilatiematrix | volledige hardwarevalidatie blijft nodig |
| TTGO D1 R32 | `esp32` | geen afzonderlijk resultaat vastgelegd in `extras/TESTRESULTATEN.md` | compatibiliteit en hardwarewerking nog afzonderlijk vastleggen |

De gemelde waarschuwing van `LiquidCrystal_I2C` betreft de architectuurmetadata van die externe library. Een board geldt pas als volledig hardwarematig gevalideerd wanneer de relevante voorbeelden op echte hardware zijn uitgevoerd en de resultaten zijn vastgelegd.

## Configuratie

Kopieer voor persoonlijke instellingen `src/Configuratie/UserConfig_template.h` vóór gebruik naar `src/Configuratie/UserConfig.h`. Zonder `UserConfig.h` gebruikt de library de fallback uit `SystemConfig.h`.

```cpp
#define BOARD_VERSION BOARD_UNO_R3
```

Mogelijke waarden:

```cpp
#define BOARD_UNO_R3        0
#define BOARD_UNO_R4_MINIMA 1
#define BOARD_UNO_R4_WIFI   2
#define BOARD_ESP32_UNO     3
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

Voor `BOARD_ESP32_UNO` is de configuratie voorbereid op 12-bit ADC-uitlezing en `DELAY_US 0`. Dit voegt vanuit de GroeiAcademie-library geen extra wachttijd tussen samples toe. De Wemos D1 R32 gebruikt 3,3 V-logica; controleer voeding, analoge ingangsspanning, shieldcompatibiliteit, timing, geheugen, callbacks en dependencies tijdens de hardwarevalidatie. Netwerkfuncties maken geen deel uit van de huidige library.

## Sensorbeoordeling

Een sensor wordt per meetdoel beoordeeld op:

- betrouwbaarheid;
- resolutie en bereik;
- samplegedrag;
- kalibratie;
- comfort en plaatsing;
- mechanische reproduceerbaarheid;
- elektrische veiligheid;
- compatibiliteit met de ADC en voedingsspanning van het board.

Geschiktheid voor één meetdoel impliceert geen geschiktheid voor een ander meetdoel.
