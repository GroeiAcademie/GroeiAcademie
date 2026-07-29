# Hardwareondersteuning

## Huidige validatiestatus

| Board | Architectuur | Status |
|---|---|---|
| Arduino UNO R3 | `avr` | voorbeelden gecompileerd; beperkte flash en SRAM blijven aandachtspunt |
| Arduino UNO R4 Minima | `renesas_uno` | voorbeelden gecompileerd |
| Arduino UNO R4 WiFi | `renesas_uno` | voorbeelden gecompileerd; netwerkfuncties maken geen deel uit van de huidige library |

De enige gemelde compilatiewaarschuwing kwam van de externe `LiquidCrystal_I2C`-library en haar architectuurmetadata, niet van de GroeiAcademie-broncode.

Een board geldt pas als volledig hardwarematig gevalideerd wanneer relevante voorbeelden ook op echte hardware zijn uitgevoerd en de resultaten zijn vastgelegd.

## Configuratie

In `src/Configuratie/SystemConfig.h`:

```cpp
#define UNO_VERSION 3  // UNO R3
```

of:

```cpp
#define UNO_VERSION 4  // UNO R4
```

De configuratie bepaalt onder andere `ADC_BITS`, `ADC_MAX` en de schaalmacro `ADC(x)`.

## Geheugen

UNO R3 is de strengste huidige grens. Met uitgebreide debugfunctionaliteit kan de volledige build te groot worden. Zonder `DEBUG` paste de gecontroleerde build binnen de beschikbare flash en SRAM. Exact gebruik kan wijzigen bij code-, core- of dependencyversies en moet bij releases opnieuw worden gemeten.

## Huidige externe hardware

### Characterscherm

De standaard Screen-laag gebruikt een I2C-characterscherm via `LiquidCrystal_I2C`.

Standaardconfiguratie:

```cpp
#define I2C_ADRES 0x27
#define ACTIEF_CHARACTER_SCREEN SCREEN_LCD1602
```

Controleer het werkelijke I2C-adres van de module. Veelvoorkomende adressen zijn `0x27` en `0x3F`; de configuratie vermeldt ook `0x32`.

### PixelScreen

PixelScreen wordt alleen gecompileerd wanneer `SCREEN_TYPE_PIXELS` in `SCREEN_OUTPUT` staat. De algemene Screen-laag gebruikt `Adafruit_GFX`; de concrete displaydriver wordt door de toepassing geïnitialiseerd. Voor een ST7789 op Arduino UNO zijn `D11` (MOSI) en `D13` (SCK) de hardware-SPI-pinnen. `CS`, `DC` en `RST` zijn configureerbaar.

Controleer de voeding en logicaniveaus van de concrete displaymodule. Bij de universele shieldopbouw wordt ofwel de levelshifter geplaatst, ofwel worden de vijf overeenkomstige draadbruggen geplaatst. Plaats nooit beide tegelijk.

### Stimulus-sensoren

De huidige standaardpinnen zijn:

| Sensor | Pin |
|---|---|
| sensor 1 | `A0` |
| sensor 2 | `A1` |
| sensor 3 | `A2` |
| sensor 4 | `A3` |

De volledige aansluiting staat in [Toepassingsgebieden/Stimulus/README.md](Toepassingsgebieden/Stimulus/README.md).

### Specifiek v0.10.1-shield

Voor het actuele UNO R3/R4-shield zijn de schema-exporten, assemblagekeuzes en validatiestappen opgenomen onder `docs/Toepassingsgebieden/Stimulus/Hardware/`. Gebruik `examples/Stimulus/Hardware_Validatie_Shield_v0_10_1/Hardware_Validatie_Shield_v0_10_1.ino` samen met [VALIDATIE_Shield_v0.10.1.md](Toepassingsgebieden/Stimulus/Hardware/VALIDATIE_Shield_v0.10.1.md).

De TFT-route gebruikt ofwel de levelshifter, ofwel de vijf draadbruggen. Plaats nooit beide tegelijk.

## ESP32

ESP32 is niet officieel ondersteund. Dat er hardware of voorbereidende mappen bestaan, bewijst geen compatibiliteit. Officiële ondersteuning vereist afzonderlijke controle van ADC-gedrag, spanning, timing, geheugen, callbacks, dependencies, documentatie en privacy bij netwerkgebruik.

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
