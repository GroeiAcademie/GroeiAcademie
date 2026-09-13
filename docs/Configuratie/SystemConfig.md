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

Wanneer `DEBUG` actief is, voegt de library automatisch `SCREEN_TYPE_SERIAL` toe aan de effectieve `SCREEN_OUTPUT`. De standaard seriële `PrintToScreen()`-uitvoer volgt daarbij de bestaande `DEBUG`-werking.

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

Voor SerialScreen gelden daarnaast:

| Define | Betekenis |
|---|---|
| `SERIAL_BAUDRATE` | Baudrate voor de seriële uitvoer; standaard `115200UL`. |
| `GA_SERIAL` | Seriële projectabstractie: `Serial0` bij `BOARD_ESP32S3_ARDI32` en `BOARD_ESP32S3_DEV`, anders `Serial`. |
| `SERIAL_CONNECT_TIMEOUT_MS` | Maximale wachttijd bij de eerste normale SerialScreen-verbinding; standaard `2000UL`. Na timeout wordt Serial voor die sessie als niet beschikbaar beschouwd en wordt `CRITICAL: SS001` op beschikbare andere schermen/callbacks gemeld. |

## Characterscherm

| Define | Betekenis |
|---|---|
| `I2C_ADRES` | I2C-adres van het characterscherm. |
| `ACTIEF_CHARACTER_SCREEN` | Concreet schermtype: `SCREEN_LCD1602`, `SCREEN_LCD1604`, `SCREEN_LCD2002`, `SCREEN_LCD2004` of `SCREEN_LCD4002`. |
| `FATAL_LEESTIJD_MS` | Standaardleestijd voor een FATAL. |

## PixelScreen

| Define | Betekenis |
|---|---|
| `ACTIEF_PIXEL_SCREEN` | Concrete resolutie: `SCREEN_128X32`, `SCREEN_128X64`, `SCREEN_128X160`, `SCREEN_240X240`, `SCREEN_240X320`, `SCREEN_320X480` of `SCREEN_480X320`. |
| `PIXEL_SCREEN_CS` | Chip-selectpin. |
| `PIXEL_SCREEN_DC` | Data/commandpin. |
| `PIXEL_SCREEN_RST` | Resetpin; standaard: `ARDUINO_UNO_SHIELD_PIN_D7`. |
| `PIXEL_SCREEN_ROTATION` | Rotatie 0, 1, 2 of 3. |
| `PIXEL_SCREEN_TEXT_SIZE` | Tekstgrootte voor het ingebouwde vaste Adafruit_GFX-font. |
| `PIXEL_SCREEN_MARGIN` | Minimale vrije ruimte in pixels tussen het tekstgrid en iedere schermrand. |
| `PIXEL_SCREEN_CHARACTER_SPACING` | Extra witruimte in pixels tussen opeenvolgende tekens. |
| `PIXEL_SCREEN_LINE_SPACING` | Extra witruimte in pixels tussen opeenvolgende tekstregels. |
| `PIXEL_SCREEN_TEXT_COLOR` | 16-bits tekstkleur. |
| `PIXEL_SCREEN_BACKGROUND_COLOR` | 16-bits achtergrondkleur. |

Voor een ST7789 op Arduino UNO gebruikt hardware-SPI de officiële functies `MOSI` en `SCK`; op de Uno-shieldheader zijn dit respectievelijk de D11- en D13-posities. Wanneer `SCREEN_TYPE_PIXELS` niet geselecteerd is, is Adafruit_GFX niet nodig en wordt PixelScreen-code niet gecompileerd.

De standaard PixelScreen-laag berekent de bruikbare breedte en hoogte na aftrek van tweemaal `PIXEL_SCREEN_MARGIN`. Het aantal kolommen en regels wordt daarna bepaald met de vaste Adafruit_GFX-karakterafmetingen, `PIXEL_SCREEN_TEXT_SIZE`, `PIXEL_SCREEN_CHARACTER_SPACING` en `PIXEL_SCREEN_LINE_SPACING`. Het resulterende grid wordt binnen de bruikbare ruimte gecentreerd.

Fatale Screen-configuratiefouten gebruiken de codes `CS000`–`CS002` (CharacterScreen) en `PS000`–`PS004` (PixelScreen). Zie [Screen-foutcodes](../Systeem/SCREEN_FOUTCODES.md).

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

De gebruiker kiest:

```cpp
#define BOARD_VERSION BOARD_UNO_R3
```

| Combinatie | `ADC_BITS` | `DELAY_US` | Betekenis |
|---|---:|---:|---|
| `ADC_BACKEND_ADS1115` | 15 | 0 | ADS1115 single-ended: 0-32767 bruikbaar bereik. |
| `ADC_BACKEND_NATIVE` en `BOARD_VERSION == BOARD_UNO_R3` | 10 | 100 | Arduino UNO R3. |
| `ADC_BACKEND_NATIVE` en `BOARD_VERSION == BOARD_UNO_R4_MINIMA` | 14 | 200 | Arduino UNO R4 Minima. |
| `ADC_BACKEND_NATIVE` en `BOARD_VERSION == BOARD_UNO_R4_WIFI` | 14 | 200 | Arduino UNO R4 WiFi. |
| `ADC_BACKEND_NATIVE` en `BOARD_VERSION == BOARD_UNO_Q` | 10 | 0 | Arduino UNO Q. |
| `ADC_BACKEND_NATIVE` en `BOARD_VERSION == BOARD_ESP32_D1_UNO_R32` | 12 | 0 | WeMos D1 R32 (ESP32-WROOM-32U) en TTGO D1 R32 (ESP32-WROOM-32U), dependency-aanduiding `ESP32-WROOM-32U`. |
| `ADC_BACKEND_NATIVE` en `BOARD_VERSION == BOARD_ESP32S3_ARDI32` | 12 | 0 | SB Components Ardi-32 (ESP32-S3-WROOM-1). |
| `ADC_BACKEND_NATIVE` en `BOARD_VERSION == BOARD_ESP32S3_DEV` | 12 | 0 | Paradisetronic ESP32-S3 UNO (ESP32-S3-WROOM-1). |
| `ADC_BACKEND_NATIVE` en `BOARD_VERSION == BOARD_RP2040_CYTRON_MAKER_UNO` | 10 | 0 | Cytron Maker UNO RP2040. |
| `ADC_BACKEND_NATIVE` en `BOARD_VERSION == BOARD_STM32F4_NUCLEO64_F401RE` | 10 | 0 | STM32F4 Nucleo-F401RE. |

Voor `BOARD_ESP32_D1_UNO_R32` betekent `DELAY_US 0` dat de GroeiAcademie FrameWork-library geen extra wachttijd tussen samples toevoegt. De conversietijd van de boardcore blijft bestaan. WeMos D1 R32 (ESP32-WROOM-32U) en TTGO D1 R32 (ESP32-WROOM-32U) zijn geïmplementeerd en getest. Controleer de werkelijke samplefrequentie, stabiliteit en sensorrespons afzonderlijk wanneer de concrete toepassing dat vereist.

`ADC_MAX` en `ADC(x)` worden door `SystemConfig.h` uit `ADC_BITS` afgeleid. `ADC(x)` ondersteunt 10, 12, 14 en 15 bits. De voorbeeldprogramma's roepen bij 12 en 14 bits `analogReadResolution(ADC_BITS)` aan.

### STM32F4 Nucleo-F401RE: Arduino Uno-shieldheader

Voor `BOARD_STM32F4_NUCLEO64_F401RE` bevat `SystemConfig.h` een actieve mapping voor D0-D13, A0-A5, SDA/SCL en de SPI-functienamen SS/MOSI/MISO/SCK. De standaardwaarden gebruiken de Arduino/STM32duino-namen `D0` t.e.m. `D13`, `A0` t.e.m. `A5`, `SDA`, `SCL`, `SS`, `MOSI`, `MISO` en `SCK`. Iedere `ARDUINO_UNO_SHIELD_PIN_...`-definitie heeft een eigen `#ifndef`, zodat een afwijkende mapping per pin in `UserConfig.h` kan worden opgegeven zonder het volledige boardblok te vervangen.

## DEBUG-macro's

```cpp
GA_DEBUG_PRINT(x)
GA_DEBUG_PRINTLN(x)
GA_DEBUG_PRINTLN2(x, f)
```

Deze macro's verwijzen naar `Serial` wanneer `DEBUG` actief is. De Screen-laag voegt in dat geval automatisch `SCREEN_TYPE_SERIAL` aan de effectieve `SCREEN_OUTPUT` toe.


### WeMos D1 R32 (ESP32-WROOM-32U): officiële Espressif-pinmapping

Gebruik in Arduino IDE:

- Boards Manager package: `esp32 by Espressif Systems`
- Board: `WeMos D1 R32`
- FQBN: `esp32:esp32:d1_uno32`
- automatische boardmacro: `ARDUINO_D1_UNO32`

De specifieke Espressif-boardvariant `d1_uno32` levert zelf de vertaling van de Arduino-headernamen naar de echte ESP32-GPIO's:

| Arduino-header | ESP32 GPIO |
|---|---:|
| D0 / RX | 3 |
| D1 / TX | 1 |
| D2 | 26 |
| D3 | 25 |
| D4 | 17 |
| D5 | 16 |
| D6 | 27 |
| D7 | 14 |
| D8 | 12 |
| D9 | 13 |
| D10 / SS | 5 |
| D11 / MOSI | 23 |
| D12 / MISO | 19 |
| D13 / SCK | 18 |

De analoge header is:

| Arduino-header | ESP32 GPIO | ADC-kanaal |
|---|---:|---|
| A0 | 2 | ADC2_CH2 |
| A1 | 4 | ADC2_CH0 |
| A2 | 35 | ADC1_CH7 |
| A3 | 34 | ADC1_CH6 |
| A4 | 36 | ADC1_CH0 |
| A5 | 39 | ADC1_CH3 |

Op dit board zijn `A4` en `A5` niet dezelfde pinnen als de I2C-functies. De specifieke Espressif-variant definieert `SDA` als GPIO21 en `SCL` als GPIO22. GPIO34, GPIO35, GPIO36 en GPIO39 zijn input-only en hebben geen interne pull-up/pull-down. De ESP32-GPIO's werken op 3,3 V-logica.

`WeMos D1 R32` en `DOIT ESPduino32` zijn afzonderlijke boarddefinities in de Espressif Arduino-core en worden in deze documentatie niet als synoniemen behandeld.

Bron voor de board- en pinmapping: de officiële `boards.txt` en `variants/d1_uno32/pins_arduino.h` van `arduino-esp32` door Espressif Systems.

### BOARD_ESP32_D1_UNO_R32

`BOARD_ESP32_D1_UNO_R32` is het configuratieprofiel voor Arduino Uno R3-vormfactor ESP32-borden. De compilatiematrix gebruikt boardprofiel `WeMos D1 R32` met FQBN `esp32:esp32:d1_uno32`. Dit profiel geldt voor WeMos D1 R32 (ESP32-WROOM-32U) en TTGO D1 R32 (ESP32-WROOM-32U). In de dependencycontrole worden beide gezamenlijk aangeduid als `ESP32-WROOM-32U` en gebruiken ze FQBN `esp32:esp32:d1_uno32`. WeMos D1 R32 (ESP32-WROOM-32U) en TTGO D1 R32 (ESP32-WROOM-32U) zijn geïmplementeerd en getest; andere compatibele varianten blijven afzonderlijk te valideren.


## Officiële Arduino-pincodes en gebruikersafwijkingen

`SystemConfig.h` gebruikt `ARDUINO_UNO_SHIELD_PIN_D0` tot en met `ARDUINO_UNO_SHIELD_PIN_D13` als bordonafhankelijke namen voor de fysieke D0-D13-posities van de Arduino Uno-shieldheader. Voor Uno R3 en Uno R4 verwijzen deze standaard naar de numerieke Arduino-pinnummers 0 tot en met 13. Voor `BOARD_ESP32_D1_UNO_R32` verwijzen ze naar de overeenkomstige `D0` tot en met `D13`-namen van de geselecteerde compatibele boardcore. Bij `INPUT_TYPE_DIGITAL` wordt per `KEYPAD_TYPE` vastgelegd welk keypadlabel op welke Arduino Uno-shieldpin aangesloten is. De `KEYPAD_PIN_...`-namen volgen het gekozen keypadtype: drukknoppen direct gebruiken `K1..K4`, matrixvarianten gebruiken hun rij-/kolomlabels, membraan-directtypes gebruiken `1..4` en touch-directtypes gebruiken `OUT1..OUT4`. Drukknop-direct, drukknop-matrix en TTP224 gebruiken standaard D2,D3,D4,D5; de twee membraan-directtypes behouden D3,D2,D5,D4 omdat die softwaremapping de vaste flatkabelvolgorde volgt, zodat er geen draden fysiek gewisseld moeten worden. Daarna worden de bestaande `PIN_TOETS_1` tot en met `PIN_TOETS_4` als backward-compatibilitylaag aan deze mapping gekoppeld. Een expliciete bestaande `PIN_TOETS_...`-definitie uit `UserConfig.h` heeft voorrang, maar dan moeten `PIN_TOETS_1` tot en met `PIN_TOETS_4` alle vier gedefinieerd zijn. `PIXEL_SCREEN_DC` en `PIXEL_SCREEN_RST` gebruiken eveneens de shieldnamen; de standaard `PIXEL_SCREEN_RST` is D7. De standaard `HX1838_ONTVANGER_PIN` is D8. `PIXEL_SCREEN_CS` blijft `SS`, omdat dit de officiële SPI-functienaam is. Een afwijkende boardcoremapping kan in `UserConfig.h` per shieldpin worden overschreven.
