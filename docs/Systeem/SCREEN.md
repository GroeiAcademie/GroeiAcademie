# Screen

## Doel

De centrale functie `PrintToScreen()` verstuurt één volledige schermopdracht naar alle tijdens het compileren geselecteerde uitvoerdoelen.

```cpp
void PrintToScreen(const String& eersteRegel, const String& tweedeRegel, unsigned long delayTime = 0, const String& action = "", const String& derdeRegel = "", const String& vierdeRegel = "", unsigned long delayTussenPaginas = 0);
```

Wanneer PixelScreen meegecompileerd is, bestaat daarnaast:

```cpp
void PrintToScreen(ScreenData screenData, const String& eersteRegel, const String& tweedeRegel, unsigned long delayTime = 0, const String& action = "", const String& derdeRegel = "", const String& vierdeRegel = "", unsigned long delayTussenPaginas = 0);
```

## Publieke opname

```cpp
#include <Screen.h>
```

of via de volledige library:

```cpp
#include <GroeiAcademie.h>
```

## Compile-time uitvoerdoelen

`SCREEN_OUTPUT` bepaalt welke uitvoercode in de build aanwezig is. Wat niet geselecteerd wordt, wordt niet gecompileerd.

```cpp
SCREEN_TYPE_NONE
SCREEN_TYPE_SERIAL
SCREEN_TYPE_CHARACTER
SCREEN_TYPE_PIXELS
```

Meerdere uitvoerdoelen worden gecombineerd met `|`. `DEBUG` geeft alleen seriële uitvoer wanneer `SCREEN_TYPE_SERIAL` meegecompileerd is.

## Characterscherm zonder callback

Zonder geregistreerde charactercallback gebruikt de library `LiquidCrystal_I2C`. Ondersteunde schermen zijn `SCREEN_LCD1602`, `SCREEN_LCD1604`, `SCREEN_LCD2002`, `SCREEN_LCD2004` en `SCREEN_LCD4002`.

Op een scherm met vier regels worden de vier teksten op regels 0 tot en met 3 geplaatst. Op een scherm met twee regels verschijnen regel 1 en 2 als eerste pagina. Wanneer regel 3 of 4 niet leeg is, wacht de library `delayTussenPaginas`, wist het scherm en toont regel 3 en 4 als tweede pagina. Na de volledige tekstuitvoer wordt `delayTime` één keer uitgevoerd. Daarna wordt `action` verwerkt.

## PixelScreen zonder callback

Fatale configuratiefouten van CharacterScreen en PixelScreen worden gemeld met een korte code zoals `CS000` of `PS001`. De volledige betekenis en oplossing staan in [Screen-foutcodes](SCREEN_FOUTCODES.md). Is geen van beide schermtypes beschikbaar, dan forceert de library voor deze melding Serial op 115200 baud.

De toepassing initialiseert de concrete displaydriver en registreert die daarna via de algemene `Adafruit_GFX*`-pointer:

```cpp
pixelScreen.init(ACTIEF_PIXEL_SCREEN_BREEDTE, ACTIEF_PIXEL_SCREEN_HOOGTE);
pixelScreen.setRotation(PIXEL_SCREEN_ROTATION);
PixelScreen = &pixelScreen;
PixelScreenConfigureren();
```

`PixelScreenConfigureren()` initialiseert niet de concrete hardwaredriver. De functie controleert de geregistreerde `Adafruit_GFX`-instantie, houdt rekening met rotatie 0 tot en met 3, trekt de ingestelde buitenmarges van de beschikbare schermruimte af, berekent het tekstgrid en stelt tekstgrootte, tekstkleur, achtergrondkleur en tekstomloop in.

Zonder geregistreerde pixelcallback verwerkt de standaarduitvoer alleen `ScreenData::TYPE_NONE`. De tekst wordt met het ingebouwde vaste Adafruit_GFX-font in een gecentreerd grid van minimaal 16×2 en maximaal 40×4 geplaatst. `PIXEL_SCREEN_MARGIN` bepaalt de minimale vrije ruimte aan iedere schermrand. `PIXEL_SCREEN_CHARACTER_SPACING` en `PIXEL_SCREEN_LINE_SPACING` bepalen de extra witruimte tussen tekens en regels. Om karakterafstand mogelijk te maken, plaatst de standaarduitvoer ieder teken afzonderlijk op de berekende cursorpositie. Voor andere fonts, vrije pixelposities, grafieken of andere `ScreenData`-typen is een pixelcallback bedoeld. De standaard cursoradministratie gaat uit van eenvoudige tekens; UTF-8-tekens kunnen meerdere bytes tellen en worden niet volledig ondersteund.

## ScreenData

`ScreenData` wordt alleen gecompileerd wanneer `SCREEN_TYPE_PIXELS` geselecteerd is. De huidige typen zijn:

```cpp
TYPE_NONE
TYPE_INFO
TYPE_MESSAGE
TYPE_NOTIFY
TYPE_SUCCESS
TYPE_PROMPT
TYPE_CONFIRM
TYPE_WARNING
TYPE_ALERT
TYPE_ERROR
TYPE_CRITICAL
TYPE_FATAL
TYPE_ABORT
TYPE_PANIC
TYPE_DEBUG
TYPE_TRACE
TYPE_TEXT
TYPE_GRAPHICS
TYPE_VIDEO
```

## Callbacktypen

Character en Pixel gebruiken afzonderlijke callbacktypen. De Pixel-callback ontvangt als eerste argument ook `ScreenData`.

```cpp
typedef void (*CharacterScreenCallback)(const String& eersteRegel, const String& tweedeRegel, unsigned long delayTime, const String& action, const String& derdeRegel, const String& vierdeRegel, unsigned long delayTussenPaginas);
typedef void (*PixelScreenCallback)(ScreenData screenData, const String& eersteRegel, const String& tweedeRegel, unsigned long delayTime, const String& action, const String& derdeRegel, const String& vierdeRegel, unsigned long delayTussenPaginas);
```

Registratie:

```cpp
RegistreerCallbackScreenTypeCharacter(MijnCharacterScreen);
RegistreerCallbackScreenTypePixel(MijnPixelScreen);
```

Met `nullptr` wordt voor beide schermtypen de standaardafhandeling gebruikt. Voor PixelScreen geldt die standaardafhandeling alleen voor `ScreenData::TYPE_NONE`.

Een CharacterScreen-callback en een PixelScreen-callback mogen tegelijk geregistreerd zijn. Ze kunnen dezelfde informatie synchroon weergeven of elk een ander doel hebben. De library dwingt tussen beide callbacks geen synchronisatie af. Wanneer synchroon gedrag gewenst is, is de gebruiker verantwoordelijk voor de onderlinge timing en voor het voorkomen dat `delayTime` of `delayTussenPaginas` door beide callbacks wordt uitgevoerd.

Een callback beheert zelf wissen, regelplaatsing, paginering, wachttijden, `action`, afkapping, scrolling en cursorlogica. De callback wordt exact één keer aangeroepen. Wanneer een standaardscherm actief is, voert de standaarduitvoer de toepasselijke wachttijden uit en ontvangen de callbacks daarvoor `0`. Wanneer geen standaardscherm actief is, ontvangen de callbacks de oorspronkelijke waarden.

## Voorbeelden

- `examples/Systeem/Screen/Default_CharacterScreen/Default_CharacterScreen.ino`
- `examples/Systeem/Screen/Default_PixelScreen/Default_PixelScreen.ino`
- `examples/Systeem/Screen/Default_CharacterScreen_PixelScreen/Default_CharacterScreen_PixelScreen.ino`
- `examples/Systeem/Screen/Callback_CharacterScreen/Callback_CharacterScreen.ino`
- `examples/Systeem/Screen/Callback_PixelScreen/Callback_PixelScreen.ino`

## Aansluitingen

Een I2C-characterscherm gebruikt `VCC`, `GND`, `SDA` en `SCL`. Controleer `I2C_ADRES` en de spanning van de gebruikte backpack.

Een SPI-PixelScreen op Arduino UNO gebruikt voor hardware-SPI standaard `D11` als MOSI en `D13` als SCK. `CS`, `DC` en `RST` worden ingesteld met `PIXEL_SCREEN_CS`, `PIXEL_SCREEN_DC` en `PIXEL_SCREEN_RST`. Controleer altijd de voedingsspanning en logicaniveaus van de concrete displaymodule.

## Beperkingen

De huidige standaardimplementatie gebruikt blokkerende `delay()`-logica. Lange wachttijden onderbreken andere verwerking.

## Uitvoeringscontract

`PrintToScreen()` verwerkt de standaarduitvoer in deze volgorde:

1. regel 1 en regel 2 worden op alle actieve standaardschermen getoond;
2. wanneer regel 3 of regel 4 een tweede pagina nodig maakt, wordt `delayTussenPaginas` exact één keer uitgevoerd;
3. regel 3 en regel 4 worden daarna op alle actieve standaardschermen getoond;
4. `delayTime` wordt exact één keer voor de standaarduitvoer uitgevoerd;
5. `action` wordt daarna op alle actieve standaardschermen getoond.

CharacterScreen en PixelScreen blijven bij gecombineerde standaarduitvoer synchroon. Een geregistreerde callback wordt exact één keer aangeroepen. `action`, `derdeRegel` en `vierdeRegel` worden ongewijzigd doorgegeven. Wanneer een standaardscherm actief is, ontvangt de callback voor `delayTime` en `delayTussenPaginas` de waarde `0`, omdat de standaarduitvoer de toepasselijke wachttijden al afhandelt. Wanneer geen standaardscherm actief is, ontvangt de callback de oorspronkelijke wachttijden. Bij gelijktijdige CharacterScreen- en PixelScreen-callbacks is de gebruiker verantwoordelijk voor de gewenste synchronisatie en voor het voorkomen van dubbele wachttijden.

`PixelScreenConfigureren()` wordt na de hardware-initialisatie en rotatie één keer aangeroepen. De gekozen hardware, driver, resolutie en rotatie liggen voor die gecompileerde sketch vast. Bij andere schermhardware wordt de sketch aangepast, opnieuw gecompileerd en opnieuw geüpload.

## Doel van de Screen-voorbeelden

- `Default_CharacterScreen`: standaard CharacterScreen-uitvoer, twee pagina's, leestijd en action;
- `Default_PixelScreen`: dezelfde standaardopdracht zonder callback op het PixelScreen;
- `Default_CharacterScreen_PixelScreen`: identieke inhoud en één gezamenlijke paginavertraging op beide standaardschermen;
- `Callback_CharacterScreen`: volledige CharacterScreen-opdracht via één callback;
- `Callback_PixelScreen`: volledige getypeerde PixelScreen-opdracht via één callback.

## Regressiematrix

Controleer bij een release minstens deze `SCREEN_OUTPUT`-combinaties: `SCREEN_TYPE_NONE`, `SCREEN_TYPE_SERIAL`, `SCREEN_TYPE_CHARACTER`, `SCREEN_TYPE_PIXELS`, `SCREEN_TYPE_SERIAL | SCREEN_TYPE_CHARACTER`, `SCREEN_TYPE_SERIAL | SCREEN_TYPE_PIXELS`, `SCREEN_TYPE_CHARACTER | SCREEN_TYPE_PIXELS` en `SCREEN_TYPE_SERIAL | SCREEN_TYPE_CHARACTER | SCREEN_TYPE_PIXELS`. Controleer voor de relevante combinaties standaarduitvoer, CharacterScreen-callback, PixelScreen-callback, twee en vier regels, `action`, `delayTime`, `delayTussenPaginas` en PixelScreen-rotatie 0 tot en met 3.
