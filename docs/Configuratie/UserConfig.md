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

De gebruiker stelt `SCREEN_OUTPUT_CONFIG` in. `SCREEN_OUTPUT` is de door de library afgeleide effectieve waarde. Wanneer `DEBUG` actief is, voegt de library automatisch `SCREEN_TYPE_SERIAL` aan `SCREEN_OUTPUT` toe.

```cpp
// #define SCREEN_OUTPUT_CONFIG SCREEN_TYPE_NONE
```

`SCREEN_TYPE_NONE` is de veilige standaard. Kies in `UserConfig.h` bewust de uitvoertypes die jouw toepassing nodig heeft.

## PixelScreen-lay-out

De PixelScreen-instellingen blijven in `UserConfig_template.h` standaard uitgecommentarieerd. Activeer alleen de waarden die bewust van de officiële standaard moeten afwijken:

```cpp
// #define PIXEL_SCREEN_TEXT_SIZE         2
// #define PIXEL_SCREEN_MARGIN            5
// #define PIXEL_SCREEN_CHARACTER_SPACING 1
// #define PIXEL_SCREEN_LINE_SPACING      2
```

`PIXEL_SCREEN_MARGIN` bepaalt de minimale vrije ruimte aan iedere schermrand. `PIXEL_SCREEN_CHARACTER_SPACING` voegt witruimte tussen tekens toe en `PIXEL_SCREEN_LINE_SPACING` voegt witruimte tussen regels toe. Deze drie waarden beïnvloeden rechtstreeks hoeveel kolommen en regels in het automatisch gecentreerde tekstgrid passen.

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
#define BOARD_UNO_R3        0
#define BOARD_UNO_R4_MINIMA 1
#define BOARD_UNO_R4_WIFI   2
#define BOARD_ESP32_UNO     3
```

Selecteer het bord met één regel:

```cpp
#define BOARD_VERSION BOARD_UNO_R3
```

`SystemConfig.h` leidt `ADC_BITS` automatisch af uit `ADC_BACKEND` en `BOARD_VERSION`. `DELAY_US` blijft via `UserConfig.h` aanpasbaar en krijgt anders de board- en backendafhankelijke fallback uit `SystemConfig.h`. Voor `BOARD_ESP32_UNO` blijft `DELAY_US 0` een te valideren keuze voor de concrete meetopstelling; zie `SystemConfig.md`.

## Actief gebruikersbestand

```text
src/Configuratie/UserConfig.h
```

Dit actieve bestand staat in `.gitignore`. Het templatebestand blijft onderdeel van iedere release. `.gitignore` beschermt het actieve bestand bij Git-gebruik; verwijder of vervang je de volledige librarymap handmatig, maak dan eerst een reservekopie van het actieve gebruikersbestand.


### BOARD_ESP32_UNO

`BOARD_ESP32_UNO` is het configuratieprofiel voor Arduino Uno R3-vormfactor ESP32-borden. De compilatiematrix vermeldt Wemos D1 R32 via `ESP32 Dev Module`. Hardwarevalidatie en compatibiliteit met andere varianten, waaronder TTGO D1 R32, moeten afzonderlijk worden vastgelegd.


## Officiële Arduino-pincodes en gebruikersafwijkingen

`SystemConfig.h` gebruikt `ARDUINO_UNO_SHIELD_PIN_D0` tot en met `ARDUINO_UNO_SHIELD_PIN_D13` als bordonafhankelijke namen voor de fysieke D0-D13-posities van de Arduino Uno-shieldheader. Voor Uno R3 en Uno R4 verwijzen deze standaard naar de numerieke Arduino-pinnummers 0 tot en met 13. Voor `BOARD_ESP32_UNO` verwijzen ze naar de overeenkomstige `D0` tot en met `D13`-namen van de geselecteerde compatibele boardcore. `PIN_TOETS_1` tot en met `PIN_TOETS_4`, `PIXEL_SCREEN_DC` en `PIXEL_SCREEN_RST` gebruiken deze shieldnamen. `PIXEL_SCREEN_CS` blijft `SS`, omdat dit de officiële SPI-functienaam is. Een afwijkende boardcoremapping kan in `UserConfig.h` per shieldpin worden overschreven.
