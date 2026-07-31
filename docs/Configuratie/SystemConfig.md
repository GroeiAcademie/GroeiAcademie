# SystemConfig.h

Bronbestand: [`../../src/Configuratie/SystemConfig.h`](../../src/Configuratie/SystemConfig.h)

`SystemConfig.h` bevat de officiële standaardwaarden en fallbackwerking. Maak voor blijvende persoonlijke instellingen een kopie van [`../../src/Configuratie/UserConfig_template.h`](../../src/Configuratie/UserConfig_template.h), noem die kopie `UserConfig.h` en laat ze in `src/Configuratie/` staan.

De laadvolgorde voor de systeemconfiguratie is:

```text
vaste keuzewaarden
-> eventueel UserConfig.h
-> officiële fallbackwaarden uit SystemConfig.h
```

De gekozen taal wordt daarna afzonderlijk geladen:

```text
librarycode onder src
-> eventueel UserLibrary_XX.h
-> Library_XX.h als fallback

voorbeeldprogramma's
-> eventueel UserExample_XX.h
-> Examples_XX.h als fallback
```

Iedere gebruikersinstelling uit `UserConfig.h` krijgt voorrang op de fallback uit `SystemConfig.h`. Iedere ontbrekende instelling valt terug op `SystemConfig.h`. De actieve gebruikersbestanden staan in `.gitignore`.

Gewone examples stellen libraryconfiguratie niet rechtstreeks in de `.ino` in. Een globale compilerdefinitie kan een configuratiewaarde vóór `UserConfig.h` vastleggen. Zonder zo'n compilerdefinitie geldt steeds: eventueel `UserConfig.h`, daarna de fallback uit `SystemConfig.h`. `GROEIACADEMIE_IGNORE_USER_CONFIG` kan worden gebruikt voor een gecontroleerde build die het persoonlijke gebruikersbestand bewust overslaat.

## Taalkeuze

Selecteer in `UserConfig.h` exact één taal. Zonder `UserConfig.h` gebruikt `SystemConfig.h` standaard Nederlands.

```cpp
#define LANGUAGE_NL
// #define LANGUAGE_DE
// #define LANGUAGE_EN
// #define LANGUAGE_FR
```

Meer dan één actieve taal veroorzaakt bewust een compilerfout. De gekozen taal bepaalt welke `Library_XX.h`- en `Examples_XX.h`-bestanden worden gebruikt. Een aanwezig `UserLibrary_XX.h` wordt vóór `Library_XX.h` geladen. Een aanwezig `UserExample_XX.h` wordt via `src/Configuratie/Examples.h` vóór `Examples_XX.h` geladen.

## DEBUG en TRACE

`DEBUG` en `TRACE` zijn standaard uitgeschakeld. `UserConfig.h` bepaalt of ze actief zijn. `TRACE` schakelt automatisch ook `DEBUG` in.

Wanneer `DEBUG` actief is, voegt de library automatisch `SCREEN_TYPE_SERIAL` toe aan de effectieve `SCREEN_OUTPUT`.

## SCREEN_OUTPUT_CONFIG en SCREEN_OUTPUT

`SCREEN_OUTPUT_CONFIG` is de instelling die de gebruiker vastlegt. `SCREEN_OUTPUT` is de effectieve waarde die de Screen-laag daarvan afleidt.

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
#define SCREEN_OUTPUT_CONFIG (SCREEN_TYPE_CHARACTER | SCREEN_TYPE_PIXELS)
```

De officiële veilige standaard is:

```cpp
#define SCREEN_OUTPUT_CONFIG SCREEN_TYPE_NONE
```

Kies in `UserConfig.h` bewust de gewenste uitvoertypes. Wat niet in `SCREEN_OUTPUT_CONFIG` geselecteerd wordt, wordt niet gecompileerd. Wanneer `DEBUG` actief is, wordt Serial aanvullend in `SCREEN_OUTPUT` opgenomen.

## Characterscherm

| Define | Betekenis |
|---|---|
| `I2C_ADRES` | I2C-adres van het characterscherm. |
| `ACTIEF_CHARACTER_SCREEN` | Concreet schermtype: `SCREEN_LCD1602`, `SCREEN_LCD1604`, `SCREEN_LCD2002`, `SCREEN_LCD2004` of `SCREEN_LCD4002`. |
| `LCD_LEESTIJD_FOUTMELDING_MS` | Standaardleestijd voor een foutmelding. |

## PixelScreen

| Define | Betekenis |
|---|---|
| `ACTIEF_PIXEL_SCREEN` | Concrete resolutie: `SCREEN_128X32`, `SCREEN_128X64`, `SCREEN_128X160`, `SCREEN_240X240`, `SCREEN_240X320`, `SCREEN_320X480` of `SCREEN_480X320`. |
| `PIXEL_SCREEN_CS` | Chip-selectpin. |
| `PIXEL_SCREEN_DC` | Data/commandpin. |
| `PIXEL_SCREEN_RST` | Resetpin. |
| `PIXEL_SCREEN_ROTATION` | Rotatie 0, 1, 2 of 3. |
| `PIXEL_SCREEN_TEXT_SIZE` | Tekstgrootte voor het ingebouwde vaste Adafruit_GFX-font. |
| `PIXEL_SCREEN_MARGIN` | Minimale vrije ruimte in pixels tussen het tekstgrid en iedere schermrand. |
| `PIXEL_SCREEN_CHARACTER_SPACING` | Extra witruimte in pixels tussen opeenvolgende tekens. |
| `PIXEL_SCREEN_LINE_SPACING` | Extra witruimte in pixels tussen opeenvolgende tekstregels. |
| `PIXEL_SCREEN_TEXT_COLOR` | 16-bits tekstkleur. |
| `PIXEL_SCREEN_BACKGROUND_COLOR` | 16-bits achtergrondkleur. |

Voor een ST7789 op Arduino UNO gebruikt hardware-SPI de officiële functies `MOSI` en `SCK`; op de Uno-shieldheader zijn dit respectievelijk de D11- en D13-posities. Wanneer `SCREEN_TYPE_PIXELS` niet geselecteerd is, is Adafruit_GFX niet nodig en wordt PixelScreen-code niet gecompileerd.

De standaard PixelScreen-laag berekent de bruikbare breedte en hoogte na aftrek van tweemaal `PIXEL_SCREEN_MARGIN`. Het aantal kolommen en regels wordt daarna bepaald met de vaste Adafruit_GFX-karakterafmetingen, `PIXEL_SCREEN_TEXT_SIZE`, `PIXEL_SCREEN_CHARACTER_SPACING` en `PIXEL_SCREEN_LINE_SPACING`. Het resulterende grid wordt binnen de bruikbare ruimte gecentreerd.

Fatale PixelScreen-configuratiefouten gebruiken de codes `PS001` tot en met `PS004`. Zie [PixelScreen-foutcodes](../Systeem/PIXELSCREEN_FOUTCODES.md).

## Sensoren

| Define | Betekenis |
|---|---|
| `AANTAL_SENSOREN_AANWEZIG` | Aantal fysiek aangesloten sensoren: uitsluitend `2` of `4`; een andere waarde veroorzaakt een compilerfout. |
| `PIN_SENSOR_1` .. `PIN_SENSOR_4` | Sensorpin of ADS1115-kanaal, afhankelijk van `ADC_BACKEND`. |

## ADC_BACKEND

De vaste keuzewaarden zijn vóór `UserConfig.h` beschikbaar:

```cpp
#define ADC_BACKEND_NATIVE  0
#define ADC_BACKEND_ADS1115 1
```

De gebruiker kiest:

```cpp
#define ADC_BACKEND ADC_BACKEND_NATIVE
```

`ADC_BACKEND_NATIVE` gebruikt de ingebouwde Arduino-ADC. `ADC_BACKEND_ADS1115` gebruikt een externe ADS1115 via I2C en vereist de optionele library Adafruit ADS1X15.

`ADS1115_I2C_ADDRESS` bepaalt het adres. `WACHT_LOSLATEN_DELAY_MS` voorkomt bij ADS1115 dat de I2C-bus onafgebroken bevraagd wordt.

## BOARD_VERSION en ADC-resolutie

`BOARD_VERSION` vervangt `UNO_VERSION`, is onafhankelijk van de backendkeuze en staat vóór de berekening van de backendafhankelijke resolutie. De vaste keuzewaarden zijn vóór `UserConfig.h` beschikbaar:

```cpp
#define BOARD_UNO_R3        0
#define BOARD_UNO_R4_MINIMA 1
#define BOARD_UNO_R4_WIFI   2
#define BOARD_ESP32_UNO     3
```

De gebruiker kiest:

```cpp
#define BOARD_VERSION BOARD_UNO_R3
```

| Combinatie | `ADC_BITS` | `DELAY_US` | Betekenis |
|---|---:|---:|---|
| `ADC_BACKEND_ADS1115` | 15 | 0 | ADS1115 single-ended: 0-32767 bruikbaar bereik. |
| `ADC_BACKEND_NATIVE` en `BOARD_VERSION == BOARD_UNO_R3` | 10 | 100 | Arduino UNO R3 met 10-bit ADC. |
| `ADC_BACKEND_NATIVE` en `BOARD_VERSION == BOARD_UNO_R4_MINIMA` | 14 | 200 | Arduino UNO R4 Minima met 14-bit ADC. |
| `ADC_BACKEND_NATIVE` en `BOARD_VERSION == BOARD_UNO_R4_WIFI` | 14 | 200 | Arduino UNO R4 WiFi met 14-bit ADC. |
| `ADC_BACKEND_NATIVE` en `BOARD_VERSION == BOARD_ESP32_UNO` | 12 | 0 | Wemos D1 R32 met de standaard 12-bit resolutie van de Arduino-ESP32-core. |

Voor `BOARD_ESP32_UNO` betekent `DELAY_US 0` dat de GroeiAcademie-library geen extra wachttijd tussen samples toevoegt. De conversietijd van de boardcore blijft bestaan. Controleer de werkelijke samplefrequentie, stabiliteit en sensorrespons op de concrete Wemos D1 R32-opstelling voordat deze instelling definitief gevalideerd wordt.

`ADC_MAX` en `ADC(x)` worden door `SystemConfig.h` uit `ADC_BITS` afgeleid. `ADC(x)` ondersteunt 10, 12, 14 en 15 bits. De voorbeeldprogramma's roepen bij 12 en 14 bits `analogReadResolution(ADC_BITS)` aan.

## DEBUG-macro's

```cpp
DEBUG_PRINT(x)
DEBUG_PRINTLN(x)
DEBUG_PRINTLN2(x, f)
```

Deze macro's verwijzen naar `Serial` wanneer `DEBUG` actief is. De Screen-laag voegt in dat geval automatisch `SCREEN_TYPE_SERIAL` aan de effectieve `SCREEN_OUTPUT` toe.


### BOARD_ESP32_UNO

`BOARD_ESP32_UNO` is het configuratieprofiel voor Arduino Uno R3-vormfactor ESP32-borden. De compilatiematrix vermeldt Wemos D1 R32 via `ESP32 Dev Module`. Hardwarevalidatie en compatibiliteit met andere varianten, waaronder TTGO D1 R32, moeten afzonderlijk worden vastgelegd.


## Officiële Arduino-pincodes en gebruikersafwijkingen

`SystemConfig.h` gebruikt `ARDUINO_UNO_SHIELD_PIN_D0` tot en met `ARDUINO_UNO_SHIELD_PIN_D13` als bordonafhankelijke namen voor de fysieke D0-D13-posities van de Arduino Uno-shieldheader. Voor Uno R3 en Uno R4 verwijzen deze standaard naar de numerieke Arduino-pinnummers 0 tot en met 13. Voor `BOARD_ESP32_UNO` verwijzen ze naar de overeenkomstige `D0` tot en met `D13`-namen van de geselecteerde compatibele boardcore. `PIN_TOETS_1` tot en met `PIN_TOETS_4`, `PIXEL_SCREEN_DC` en `PIXEL_SCREEN_RST` gebruiken deze shieldnamen. `PIXEL_SCREEN_CS` blijft `SS`, omdat dit de officiële SPI-functienaam is. Een afwijkende boardcoremapping kan in `UserConfig.h` per shieldpin worden overschreven.
