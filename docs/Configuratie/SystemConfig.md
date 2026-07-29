# SystemConfig.h

Bronbestand: [`../../src/Configuratie/SystemConfig.h`](../../src/Configuratie/SystemConfig.h)

Instellingen die het hele framework raken: scherm, sensorpinnen, ADC-backend en debug-uitvoer.

## DEBUG en TRACE

```cpp
#define DEBUG
```

`TRACE` schakelt automatisch ook `DEBUG` in. `DEBUG` alleen maakt nog geen seriële uitvoer mogelijk: daarvoor moet ook `SCREEN_TYPE_SERIAL` in `SCREEN_OUTPUT` aanwezig zijn. Wanneer `SCREEN_OUTPUT` nog niet door de gebruiker werd ingesteld, kiest de standaardconfiguratie bij `DEBUG` voor Serial + CharacterScreen. Een zelf gedefinieerde `SCREEN_OUTPUT` wordt altijd gerespecteerd.

## SCREEN_OUTPUT

`SCREEN_OUTPUT` bepaalt tijdens het compileren welke uitvoertypen in de build aanwezig zijn. Wat niet geselecteerd wordt, wordt niet gecompileerd.

| Waarde | Uitvoer |
|---:|---|
| 0 | geen uitvoer |
| 1 | Serial |
| 2 | CharacterScreen |
| 3 | Serial + CharacterScreen |
| 4 | PixelScreen |
| 5 | Serial + PixelScreen |
| 6 | CharacterScreen + PixelScreen |
| 7 | Serial + CharacterScreen + PixelScreen |

Meerdere typen worden gecombineerd met `|`, bijvoorbeeld:

```cpp
#define SCREEN_OUTPUT (SCREEN_TYPE_SERIAL | SCREEN_TYPE_CHARACTER | SCREEN_TYPE_PIXELS)
```

De standaardkeuze is bij `DEBUG` Serial + CharacterScreen en zonder `DEBUG` alleen CharacterScreen.

## Characterscherm

| Define | Betekenis |
|---|---|
| `I2C_ADRES` | I2C-adres van het characterscherm. |
| `ACTIEF_CHARACTER_SCREEN` | Concreet schermtype: `SCREEN_LCD1602`, `SCREEN_LCD1604`, `SCREEN_LCD2002`, `SCREEN_LCD2004` of `SCREEN_LCD4002`. |

## PixelScreen

| Define | Betekenis |
|---|---|
| `ACTIEF_PIXEL_SCREEN` | Concrete resolutie: `SCREEN_128X32`, `SCREEN_128X64`, `SCREEN_128X160`, `SCREEN_240X240`, `SCREEN_240X320`, `SCREEN_320X480` of `SCREEN_480X320`. |
| `PIXEL_SCREEN_CS` | Chip-selectpin. |
| `PIXEL_SCREEN_DC` | Data/commandpin. |
| `PIXEL_SCREEN_RST` | Resetpin. |
| `PIXEL_SCREEN_ROTATION` | Rotatie 0, 1, 2 of 3. |
| `PIXEL_SCREEN_TEXT_SIZE` | Tekstgrootte voor het ingebouwde vaste Adafruit_GFX-font. |
| `PIXEL_SCREEN_TEXT_COLOR` | 16-bits tekstkleur. |
| `PIXEL_SCREEN_BACKGROUND_COLOR` | 16-bits achtergrondkleur. |

Voor een ST7789 op Arduino UNO gebruikt hardware-SPI standaard `D11` als MOSI en `D13` als SCK. Wanneer `SCREEN_TYPE_PIXELS` niet geselecteerd is, is Adafruit_GFX niet nodig en wordt PixelScreen-code niet gecompileerd.

Fatale PixelScreen-configuratiefouten gebruiken de codes `PS001` tot en met `PS004`. Zie [PixelScreen-foutcodes](../Systeem/PIXELSCREEN_FOUTCODES.md) voor het overzicht, de triggers en de oplossingen. Het nog open voorstel om numerieke rotaties later te vervangen of aan te vullen met leesbare oriëntatienamen staat in [PixelScreen-oriëntatievoorstel](../Systeem/PIXELSCREEN_ORIENTATIE_VOORSTEL.txt).

## Sensoren

| Define | Betekenis |
|---|---|
| `AANTAL_SENSOREN_AANWEZIG` | Aantal fysiek aangesloten sensoren. |
| `PIN_SENSOR_1` .. `PIN_SENSOR_4` | Sensorpin of ADS1115-kanaal, afhankelijk van `ADC_BACKEND`. |

## ADC_BACKEND

```cpp
#define ADC_BACKEND_NATIVE   0
#define ADC_BACKEND_ADS1115  1
#define ADC_BACKEND ADC_BACKEND_NATIVE
```

`ADC_BACKEND_NATIVE` gebruikt de ingebouwde Arduino-ADC. `ADC_BACKEND_ADS1115` gebruikt een externe ADS1115 via I2C en vereist de optionele library Adafruit ADS1X15.

`ADS1115_I2C_ADDRESS` bepaalt het adres. `WACHT_LOSLATEN_DELAY_MS` voorkomt bij ADS1115 dat de I2C-bus onafgebroken bevraagd wordt.

## UNO_VERSION en ADC-resolutie

```cpp
#define UNO_VERSION 3 // R3=3, R4=4
```

| Combinatie | `ADC_BITS` | `DELAY_US` | Betekenis |
|---|---|---|---|
| `ADC_BACKEND_ADS1115` | 15 | 0 | ADS1115 single-ended: 0-32767 bruikbaar bereik. `DELAY_US` niet van toepassing — de I2C-conversietijd zit in de leesfunctie zelf. |
| `UNO_VERSION == 3` | 10 | 100 | Klassieke 10-bit ADC. |
| `UNO_VERSION == 4` | 14 | 200 | UNO R4 Minima's ingebouwde 14-bit ADC. |

`ADC_MAX` en `ADC(x)` herschalen drempelwaarden naar de actieve resolutie.

## DEBUG-macro's

```cpp
DEBUG_PRINT(x)       // Serial.print(x), enkel actief als DEBUG gedefinieerd is
DEBUG_PRINTLN(x)     // Serial.println(x)
DEBUG_PRINTLN2(x, f) // Serial.println(x, f) — bv. voor hexadecimale uitvoer
```

Deze macro's verwijzen alleen naar `Serial` wanneer zowel `DEBUG` als `SCREEN_TYPE_SERIAL` actief zijn.
## Taalkeuze

Selecteer in `SystemConfig.h` exact één taal. De vaste volgorde is NL, DE, EN en FR.

```cpp
#define LANGUAGE_NL
// #define LANGUAGE_DE
// #define LANGUAGE_EN
// #define LANGUAGE_FR
```

De gekozen taal bepaalt welke `Library_XX.h`- en `Examples_XX.h`-tekstbestanden worden gebruikt. Datum-, getal-, land- en locale-instellingen behoren niet tot deze taalbestanden.

