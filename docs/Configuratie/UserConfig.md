# UserConfig.h

Brontemplate: [`../../src/Configuratie/UserConfig_template.h`](../../src/Configuratie/UserConfig_template.h)

Kopieer `UserConfig_template.h` vóór gebruik naar `UserConfig.h` in dezelfde map `src/Configuratie/`. `UserConfig.h` is optioneel: zonder dit bestand gebruikt de library de officiële standaardwaarden uit `SystemConfig.h`. De template bevat alle parameters die de gebruiker in deze release kan instellen. De volledige oorspronkelijke structuur, sectiekoppen, commentaren, volgorde en witregels uit het configureerbare gedeelte van `SystemConfig.h` zijn in de template behouden.

Alle aanpasbare instellingen staan in het template standaard als uitgecommentarieerde `// #define`-regels. Activeer uitsluitend de regels die bewust van de officiële standaard moeten afwijken. De geactiveerde instellingen in `UserConfig.h` zijn gewone `#define`-regels. `SystemConfig.h` laadt een aanwezig `UserConfig.h` vóór de officiële standaardwaarden. Iedere overeenkomstige standaardwaarde in `SystemConfig.h` staat onder `#ifndef`:

- staat een instelling in `UserConfig.h`, dan blijft die gebruikerswaarde behouden tegenover `SystemConfig.h`;
- ontbreekt een instelling in `UserConfig.h`, dan vult `SystemConfig.h` de officiële standaardwaarde in;
- geeft een globale compilerdefinitie de waarde eerder op, dan slaat het overeenkomstige `#ifndef` in `UserConfig.h` die instelling over; gewone voorbeelden stellen libraryconfiguratie niet rechtstreeks in de `.ino` in.

Je mag na het kopiëren dus instellingen verwijderen die je niet persoonlijk wilt vastleggen. Alleen de overblijvende instellingen wijken dan blijvend af van de officiële standaardconfiguratie.

## Taal

Activeer desgewenst exact één taal:

```cpp
// #define LANGUAGE_NL
// #define LANGUAGE_DE
// #define LANGUAGE_EN
// #define LANGUAGE_FR
```

Verwijder `//` uitsluitend voor de gewenste taal. Wanneer geen taalregel actief is, gebruikt `SystemConfig.h` Nederlands als officiële standaard. Meer dan één actieve taal veroorzaakt bewust een compilerfout. Wanneer geen `UserConfig.h` aanwezig is, gebruikt `SystemConfig.h` Nederlands als officiële standaard.

## DEBUG en TRACE

In het volledige template zijn `DEBUG` en `TRACE` standaard uitgeschakeld:

```cpp
// #define DEBUG
// #define TRACE
```

Een actief `UserConfig.h` bepaalt zelf of `DEBUG` en `TRACE` actief zijn. Verwijder de commentaartekens voor de gewenste instelling. Wanneer `TRACE` actief is, schakelt `SystemConfig.h` automatisch ook `DEBUG` in.

Zonder actief `UserConfig.h` blijven `DEBUG` en `TRACE` uitgeschakeld.

## Schermuitvoer

De gebruiker stelt `SCREEN_OUTPUT_CONFIG` in. `SCREEN_OUTPUT` is de door de library afgeleide effectieve waarde. Wanneer `DEBUG` actief is, voegt de library automatisch `SCREEN_TYPE_SERIAL` aan `SCREEN_OUTPUT` toe. De standaard seriële `PrintToScreen()`-uitvoer volgt de bestaande `DEBUG`-werking.

```cpp
// #define SCREEN_OUTPUT_CONFIG SCREEN_TYPE_NONE
// #define SERIAL_BAUDRATE 115200UL
// #define SERIAL_CONNECT_TIMEOUT_MS 2000UL
```

`SCREEN_TYPE_NONE` is de veilige standaard. `SERIAL_CONNECT_TIMEOUT_MS` begrenst alleen de eerste normale SerialScreen-verbindingspoging; na timeout blijft de toepassing doorgaan en kan `CRITICAL: SS001` via een beschikbaar CharacterScreen of PixelScreen/callback worden gemeld. Kies in `UserConfig.h` bewust de uitvoertypes die jouw toepassing nodig heeft.

## PixelScreen-lay-out

De PixelScreen-instellingen blijven in `UserConfig_template.h` standaard uitgecommentarieerd. Activeer alleen de waarden die bewust van de officiële standaard moeten afwijken:

```cpp
// #define PIXEL_SCREEN_TEXT_SIZE         2
// #define PIXEL_SCREEN_MARGIN            5
// #define PIXEL_SCREEN_CHARACTER_SPACING 1
// #define PIXEL_SCREEN_LINE_SPACING      2
```

`PIXEL_SCREEN_MARGIN` bepaalt de minimale vrije ruimte aan iedere schermrand. `PIXEL_SCREEN_CHARACTER_SPACING` voegt witruimte tussen tekens toe en `PIXEL_SCREEN_LINE_SPACING` voegt witruimte tussen regels toe. Deze drie waarden beïnvloeden rechtstreeks hoeveel kolommen en regels in het automatisch gecentreerde tekstgrid passen.

## Input

De Input-laag wordt via `UserConfig.h` geconfigureerd met dezelfde voorrangsregel als de overige gebruikersinstellingen.

Belangrijkste instellingen:

```cpp
// #define INPUT_KANAAL_CONFIG INPUT_TYPE_DIGITAL
// #define KEYPAD_TYPE KEYPAD_TYPE_DRUKKNOP_DIRECT_1x4
// #define I2C_ADDRESS_PCF8574 0x20
// #define HX1838_ONTVANGER_PIN ARDUINO_UNO_SHIELD_PIN_D8
// #define HX1838_USE_TINYIRRECEIVER_INSTEAD_OF_IRREMOTE 1
// #define HX1838_TOETSENINDELING HX1838_TOETSENINDELING_REMOTE_OK_BOVENAAN_17_TOETSEN
// #define HX1838_TOETSENINDELING HX1838_TOETSENINDELING_REMOTE_OK_ONDERAAN_17_TOETSEN
// #define HX1838_TOETSENINDELING HX1838_TOETSENINDELING_REMOTE_21_TOETSEN_MP3
// #define HX1838_TOETSENINDELING HX1838_TOETSENINDELING_REMOTE_USER_DEFINED
// #define HX1838_BRON_CODES HX1838_BRON_CODES_EEPROM_WANNEER_GEEN_DEFINE
```

Geldige huidige `INPUT_KANAAL_CONFIG`-keuzes zijn `INPUT_TYPE_NONE`, `INPUT_TYPE_DIGITAL`, `INPUT_TYPE_PCF8574`, `INPUT_TYPE_HX1838` en `INPUT_TYPE_PCF8574 | INPUT_TYPE_HX1838`.

`HX1838_BRON_CODES_DEFINE` is released en is hardwarematig bevestigd voor zowel TinyIRReceiver (`HX1838_USE_TINYIRRECEIVER_INSTEAD_OF_IRREMOTE = 1`) als IRremote (`= 0`); beide routes werken. De standaard `HX1838_ONTVANGER_PIN` is D8. De EEPROM-gebaseerde HX1838-routes blijven experimenteel.

Voor de drie ingebouwde HX1838-toetsenindelingen kunnen `HX1838_CODE_1` tot en met `HX1838_CODE_21` als vaste mapping worden ingevuld. Bij `HX1838_TOETSENINDELING_REMOTE_USER_DEFINED` zijn `HX1838_GENERIEK_AANTAL_TOETSEN` en `HX1838_GENERIEK_KEY_LAYOUT` verplicht. `HX1838_GENERIEK_CODES` mag als vaste DEFINE-mapping worden ingevuld; wanneer deze ontbreekt, start automatisch de UserDefined-kalibratie om de codes te bepalen. Voor deze UserDefined-route worden geen EEPROM-codebronnen ondersteund. De huidige UserDefined-codes zijn 8-bit commandwaarden (`uint8_t`).

Voor de ingebouwde indelingen geldt verder: `HX1838_BRON_CODES_DEFINE` gebruikt de vaste mapping; codes die niet in `UserConfig.h` zijn gedefinieerd, worden door `SystemConfig.h` met de standaardcodes aangevuld. `HX1838_BRON_CODES_EEPROM_ALTIJD` gebruikt EEPROM ongeacht aanwezige code-defines. Bij `HX1838_BRON_CODES_EEPROM_WANNEER_GEEN_DEFINE` wordt de vaste mapping gebruikt zodra minstens één `HX1838_CODE_x` in `UserConfig.h` is gedefinieerd; wanneer geen enkele code is gedefinieerd, wordt EEPROM gebruikt. Een expliciet gedefinieerde code met waarde `0` is ongeldig wanneer de vaste mapping wordt gebruikt.

De logische keypadpinnen en de Arduino Uno-shieldpin-overrides staan eveneens in `UserConfig_template.h`; activeer alleen de regels die bewust van de standaardconfiguratie moeten afwijken. Voor `INPUT_TYPE_DIGITAL` gebeurt dit per `KEYPAD_TYPE`: drukknop-direct, drukknop-matrix en TTP224 gebruiken standaard D2,D3,D4,D5; de twee membraan-directtypes behouden standaard D3,D2,D5,D4. De oude `PIN_TOETS_1` tot en met `PIN_TOETS_4` blijven voor backward compatibility ondersteund en hebben voorrang wanneer ze expliciet in `UserConfig.h` zijn ingesteld.

## ADC-backend

De vaste keuzewaarden `ADC_BACKEND_NATIVE` en `ADC_BACKEND_ADS1115` worden door `SystemConfig.h` beschikbaar gemaakt vóór `UserConfig.h` wordt geladen. Daardoor kunnen de gewone en backendafhankelijke instellingen in `UserConfig.h` deze namen veilig gebruiken.

Het template bevat de huidige instelbare waarden voor:

- `AANTAL_SENSOREN_AANWEZIG`, uitsluitend `2` of `4`;
- `ADC_BACKEND` en `ADS1115_I2C_ADDRESS`;
- `WACHT_LOSLATEN_DELAY_MS`;
- `PIN_SENSOR_1` tot en met `PIN_SENSOR_4`;
- `BOARD_VERSION`;
- `DELAY_US`.

`BOARD_VERSION` blijft een algemene boardkeuze en staat buiten de keuze tussen de Native ADC en ADS1115. De beschikbare waarden zijn:

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

Selecteer het bord met één regel:

```cpp
#define BOARD_VERSION BOARD_UNO_R3
```

`SystemConfig.h` leidt `ADC_BITS` automatisch af uit `ADC_BACKEND` en `BOARD_VERSION`. `DELAY_US` blijft via `UserConfig.h` aanpasbaar en krijgt anders de van het Arduino Uno R3-vormfactorbord en de backend afhankelijke fallback uit `SystemConfig.h`. Voor `BOARD_ESP32_D1_UNO_R32` blijft `DELAY_US 0` een te valideren keuze voor de concrete meetopstelling; zie `SystemConfig.md`.

## Actief gebruikersbestand

```text
src/Configuratie/UserConfig.h
```

Dit actieve bestand staat in `.gitignore`. Het templatebestand blijft onderdeel van iedere release. `.gitignore` beschermt het actieve bestand bij Git-gebruik; verwijder of vervang je de volledige librarymap handmatig, maak dan eerst een reservekopie van het actieve gebruikersbestand.


### BOARD_ESP32_D1_UNO_R32

`BOARD_ESP32_D1_UNO_R32` is het configuratieprofiel voor Arduino Uno R3-vormfactor ESP32-borden. De compilatiematrix gebruikt boardprofiel `WeMos D1 R32` met FQBN `esp32:esp32:d1_uno32`. Dit profiel geldt voor WeMos D1 R32 (ESP32-WROOM-32U) en TTGO D1 R32 (ESP32-WROOM-32U). In de dependencycontrole worden beide gezamenlijk aangeduid als `ESP32-WROOM-32U` en gebruiken ze FQBN `esp32:esp32:d1_uno32`. WeMos D1 R32 (ESP32-WROOM-32U) en TTGO D1 R32 (ESP32-WROOM-32U) zijn geïmplementeerd en getest; andere compatibele varianten blijven afzonderlijk te valideren.


## Officiële Arduino-pincodes en gebruikersafwijkingen

`SystemConfig.h` gebruikt `ARDUINO_UNO_SHIELD_PIN_D0` tot en met `ARDUINO_UNO_SHIELD_PIN_D13` als bordonafhankelijke namen voor de fysieke D0-D13-posities van de Arduino Uno-shieldheader. Voor Uno R3 en Uno R4 verwijzen deze standaard naar de numerieke Arduino-pinnummers 0 tot en met 13. Voor `BOARD_ESP32_D1_UNO_R32` verwijzen ze naar de overeenkomstige `D0` tot en met `D13`-namen van de geselecteerde compatibele boardcore. De keypad-specifieke `KEYPAD_PIN_...`-mapping voor `INPUT_TYPE_DIGITAL` gebruikt deze shieldnamen en wordt daarna gekoppeld aan de bestaande `PIN_TOETS_1` tot en met `PIN_TOETS_4`; `PIXEL_SCREEN_DC` en `PIXEL_SCREEN_RST` gebruiken eveneens de shieldnamen; de standaard `PIXEL_SCREEN_RST` is D7. De standaard `HX1838_ONTVANGER_PIN` is D8. `PIXEL_SCREEN_CS` blijft `SS`, omdat dit de officiële SPI-functienaam is. Een afwijkende boardcoremapping kan in `UserConfig.h` per shieldpin worden overschreven. Dit geldt ook voor de actieve `BOARD_STM32F4_NUCLEO64_F401RE`-mapping: D0-D13, A0-A5, SDA/SCL en SS/MOSI/MISO/SCK zijn elk afzonderlijk met `#ifndef` beschermd.


Beschikbare HX1838-toetsenindelingen:

- `HX1838_TOETSENINDELING_REMOTE_OK_BOVENAAN_17_TOETSEN`: remote met 17 toetsen: (UP, DOWN, OK, LEFT, RIGHT, 1-9, *, 0, #) [UP, DOWN, OK, LEFT, RIGHT, 1, 2, 3, 4, 5, 6, 7, 8, 9, *, 0, #]
- `HX1838_TOETSENINDELING_REMOTE_OK_ONDERAAN_17_TOETSEN`: remote met 17 toetsen: (1-9, *, 0, #, UP, DOWN, OK, LEFT, RIGHT) [1, 2, 3, 4, 5, 6, 7, 8, 9, *, 0, #, UP, DOWN, OK, LEFT, RIGHT]
- `HX1838_TOETSENINDELING_REMOTE_21_TOETSEN_MP3`        : remote met 21 toetsen, inclusief de MP3-toetsen.
- `HX1838_TOETSENINDELING_REMOTE_USER_DEFINED`          : eigen toetsenindeling; uitsluitend met `HX1838_BRON_CODES_DEFINE`. Stel `HX1838_GENERIEK_AANTAL_TOETSEN` en `HX1838_GENERIEK_KEY_LAYOUT` in `UserConfig.h` in. `HX1838_GENERIEK_CODES` is optioneel: ontbreekt deze define, dan start automatisch de UserDefined-kalibratie.

## Input-specifieke gebruikersinstellingen

UserDefined PCF8574-keypads moeten al hun vereiste `KEYPAD_GENERIEK_...`-instellingen in `UserConfig.h` krijgen; een `.ino`-define bereikt `Input.cpp` niet. Voor HX1838 begrenst `HX1838_KALIBRATIE_TIMEOUT_MS` (standaard 30000 ms) de wachttijd tijdens kalibratie en verificatie.
