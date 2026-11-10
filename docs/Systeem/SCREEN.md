# Screen

## Doel

Het centrale object `Screen` verstuurt via `Screen.Print()` één volledige schermopdracht naar alle tijdens het compileren geselecteerde uitvoerdoelen.

```cpp
Screen.Print(const String& eersteRegel, const String& tweedeRegel, unsigned long delayTime = 0, const String& action = "", const String& derdeRegel = "", const String& vierdeRegel = "", unsigned long delayTussenPaginas = 0);
```

Wanneer PixelScreen meegecompileerd is, bestaat daarnaast:

```cpp
Screen.Print(ScreenData screenData, const String& eersteRegel, const String& tweedeRegel, unsigned long delayTime = 0, const String& action = "", const String& derdeRegel = "", const String& vierdeRegel = "", unsigned long delayTussenPaginas = 0);
```

`Screen` gebruikt `GedeeldeBus` voor de lifecycle en resourcecontrole.

```text
Native
└─ Screen
   ├─ CharacterScreen
   ├─ PixelScreen
   └─ SerialOutput
```

De publieke Screen-subobjecten zijn:

```text
Screen.Character
Screen.Pixel
Screen.Serial
```

`CharacterScreen`, `PixelScreen` en `SerialOutput` zijn de bijhorende GedeeldeBus-objecten.

---

## Screen-objectstructuur

```text
Screen
├─ actief
├─ Aanmelden()
├─ Afmelden()
├─ Controleren()
├─ Inpluggen()
├─ Print(...)
├─ Print(ScreenData, ...)
├─ PrintPrivate()
│
├─ Character
│  ├─ actief
│  ├─ callback
│  ├─ display
│  ├─ foutmelding
│  ├─ foutmeldingBuffer
│  ├─ foutmeldingWeergegeven
│  ├─ gedeeldeBus
│  ├─ FoutmeldingWeergeven()
│  └─ RegistreerCallback()
│
├─ Pixel
│  ├─ actief
│  ├─ aantalKolommen
│  ├─ aantalRegels
│  ├─ callback
│  ├─ cursorKolom
│  ├─ cursorRegel
│  ├─ display
│  ├─ emulateLCDxxx4
│  ├─ foutmelding
│  ├─ foutmeldingWeergegeven
│  ├─ gedeeldeBus
│  ├─ offsetX
│  ├─ offsetY
│  ├─ gfx
│  ├─ Clear()
│  ├─ FoutmeldingWeergeven()
│  ├─ GridClamp()
│  ├─ KarakterBreedte()
│  ├─ KarakterStap()
│  ├─ Print()
│  ├─ RegelHoogte()
│  ├─ RegelStap()
│  ├─ RegistreerCallback()
│  └─ SetCursor()
│
└─ Serial
   ├─ actief
   ├─ gedeeldeBus
   └─ FoutmeldingWeergeven()
```

Binnen ieder object staan eerst gegevens, statussen en objectreferenties alfabetisch, daarna functies alfabetisch.

---

## Exacte objectpaden

Binnen:

```cpp
bool Screen::inpluggen()
```

is `this` het huidige `Screen`-object.

Daarom zijn de correcte paden:

```cpp
this->actief

this->Character.actief
this->Character.callback
this->Character.display
this->Character.foutmelding
this->Character.foutmeldingBuffer
this->Character.foutmeldingWeergegeven
this->Character.gedeeldeBus

this->Pixel.actief
this->Pixel.aantalKolommen
this->Pixel.aantalRegels
this->Pixel.callback
this->Pixel.cursorKolom
this->Pixel.cursorRegel
this->Pixel.display
this->Pixel.emulateLCDxxx4
this->Pixel.foutmelding
this->Pixel.foutmeldingWeergegeven
this->Pixel.gedeeldeBus
this->Pixel.offsetX
this->Pixel.offsetY
this->Pixel.gfx

this->Serial.actief
this->Serial.gedeeldeBus
```

Buiten een `Screen`-memberfunctie:

```cpp
::Screen.Character
::Screen.Pixel
::Screen.Serial
```

`Screen->Character` is niet correct.

---

## v2.0.0 aanmaken en diagnose

`Screen` is een `GedeeldeBusNode`. De standaardconstructor gebruikt `SCREEN_OUTPUT`; andere constructors laten een runtime-subset en Character-/Pixelcallbacks toe.

```cpp
Screen = GedeeldeBusNewComponent<struct Screen>();
if (Screen == nullptr) exit(0);
```

Met callbacks:

```cpp
Screen = GedeeldeBusNewComponent<struct Screen>(MijnCharacterScreen, MijnPixelScreen);
```

Of met expliciete runtime-subset:

```cpp
Screen = GedeeldeBusNewComponent<struct Screen>(
  SCREEN_TYPE_CHARACTER | SCREEN_TYPE_PIXELS,
  MijnCharacterScreen,
  MijnPixelScreen
);
```

De runtime-selectie staat in `Screen::typesActief`. De concrete children zijn, afhankelijk van `SCREEN_OUTPUT`, `SerialOutput`, `CharacterScreen` en `PixelScreen`.

`Diagnose()` is nog niet specifiek in `Screen` geïmplementeerd. Alleen de lege virtuele DEBUG-hook uit `GedeeldeBusNode` bestaat.

Belangrijk voor de actuele implementatie: niet elke fysieke controle zit vandaag in `inpluggen()`. `CharacterScreen::Activeren()` controleert of het I2C-adres antwoordt. `PixelScreen::inpluggen()` configureert CS/DC/RST; `PixelScreen::Activeren()` retourneert momenteel `true` en bevat een uitgecommentarieerde ST7789-ID-proef. De documentatie hieronder beschrijft daarom de code zoals ze nu is en niet een toekomstige Diagnose-verdeling.

# Lifecycle

De actuele `Screen`-implementatie in `src/Systeem/Screen/Screen.cpp` voert de lifecycle uit via het parentobject én de geselecteerde uitvoerchildren. `SCREEN_OUTPUT` bepaalt welke children kunnen worden gecompileerd; `typesActief` bepaalt welke daarvan voor dit Screen-object worden aangemaakt.

```text
Screen::aanmelden()   → Screen-parent aanmelden; geselecteerde children aanmaken en aanmelden
Screen::controleren() → Parent en aanwezige children controleren; geweigerde children verwijderen
Screen::inpluggen()   → Parent en aanwezige children inpluggen; mislukte children verwijderen
Screen::Activeren()   → Overgebleven children activeren; displayinitialisatie en pixelgrid instellen
Screen::afmelden()    → Children afmelden; daarna Screen-parent afmelden
```

Een mislukte uitvoer hoeft het gehele `Screen`-object **niet** te laten falen. De huidige code meldt de betreffende fout, verwijdert die child via `ChildVerwijderen()` en laat de overige uitvoerdoelen doorwerken. Bij `typesActief != SCREEN_TYPE_NONE` faalt een lifecyclefase wanneer `AantalUitvoeren(*this) == 0`; `SCREEN_TYPE_NONE` blijft een geldige configuratie. `ActivatiefoutWeergeven()` toont waar mogelijk de bewaarde fouten via nog beschikbare uitvoerdoelen.

## `Screen::aanmelden()`

`Screen::aanmelden()` controleert eerst of `typesActief` een subset is van `SCREEN_OUTPUT` en meldt de parent aan met `GedeeldeBusNode::aanmelden()`. Daarna maakt deze functie alleen de geselecteerde `SerialOutput`, `CharacterScreen` en `PixelScreen` aan (met `new`). Elk childobject krijgt `componentCreated = true` en wordt afzonderlijk via `child->aanmelden()` aangemeld.

- **Serial:** gebruikt `NP_D1`/`NP_D0` uitsluitend op de specifiek in `Screen.cpp` opgesomde boards; op de overige boards worden `NONE`/`NONE` doorgegeven. De uitzonderingsstatus voor UNO Q is in het broncommentaar nog niet officieel bevestigd.
- **Character:** krijgt `I2C_ADDRESS_CHARACTER_SCREEN`, `NP_SDA` en `NP_SCL`. De code probeert geen alternatieve LCD-adressen.
- **Pixel:** krijgt `PIXEL_SCREEN_CS`, SPI-pinnen, `PIXEL_SCREEN_DC` en `PIXEL_SCREEN_RST`.

Bij mislukte child-aanmelding wordt het pas aangemaakte object verwijderd en wordt respectievelijk `_ABORT_SS001`, `_ABORT_CS001` of `_ABORT_PS001` onthouden. De andere geselecteerde children worden nog verwerkt.

## `Screen::controleren()`

Deze fase begint met `GedeeldeBusNode::controleren()` en roept daarna `controleren()` aan op elk aanwezig child. Bij een conflict wordt `foutmeldingWeergegeven` gemarkeerd; anders volgt de eigen foutcode `_FATAL_SS101`, `_FATAL_CS101` of `_FATAL_PS101`. Niet-gecontroleerde children worden via `ChildVerwijderen()` verwijderd.

Het parentobject gebruikt zelf `{ nullptr, 0, nullptr, 0 }` als resourcebeschrijving. De fysieke resourceclaims horen bij de concrete Screen-children, niet bij `Screen` als parent.

## `Screen::inpluggen()`

Na `GedeeldeBusNode::inpluggen()` wordt voor elke nog aanwezige child `inpluggen()` opgeroepen. Bij falen krijgt die uitvoer `_CRITICAL_SS201`, `_CRITICAL_CS201` of `_CRITICAL_PS201` en wordt alleen de betrokken child verwijderd. De fysieke activering en scherminitialisatie vinden **niet** in deze functie plaats.

## `Screen::Activeren()`

De huidige implementatie activeert de overgebleven uitvoerchildren afzonderlijk:

- **Serial:** roept `Serial.gedeeldeBus->activeren()` aan en stelt `Serial.actief` in. Bij falen volgt `_CRITICAL_SS301` en verwijdering van de child. De concrete seriële initialisatie staat in `SerialOutput::Activeren()`.
- **Character:** na succesvolle `Character.gedeeldeBus->activeren()` worden `Character.display.init()` en `backlight()` uitgevoerd en wordt `Character.actief = true`. De adrescontrole zit in `CharacterScreen::Activeren()`. Bij falen volgt `_CRITICAL_CS301` en verwijdering van die child.
- **Pixel:** na `Pixel.gedeeldeBus->activeren()` stelt `Screen::Activeren()` de controller, rotatie, eventuele display-inversie, `gfx`, afmetingen en het tekstgrid in. `_CRITICAL_PS301` t/m `_CRITICAL_PS305` dekken respectievelijk activering, ontbrekende `gfx`, onjuiste afmetingen en een te klein grid. Bij falen worden `Pixel.gfx = nullptr`, `Pixel.actief = false` en de child verwijderd.

`Screen::Activeren()` roept vervolgens `ActivatiefoutWeergeven()` aan en retourneert `true` bij `SCREEN_TYPE_NONE` of wanneer nog minimaal één uitvoerchild overblijft. Een eerdere storing op één uitvoer betekent dus niet automatisch dat alle weergave stopt.

**Character-adrescontrole:** de aanwezigheidscontrole in `CharacterScreen::Activeren()` gebruikt het geconfigureerde I²C-adres. `Screen` scant niet zelfstandig naar `0x27` of `0x3F`.

**Pixelcontroller:** voor `SCREEN_ST7735S` gebruikt de bestaande code `display.initR(INITR_BLACKTAB)`, anders `display.init(...)`. Daarna volgen rotatie en, alleen voor `SCREEN_ST7789`, `invertDisplay(ACTIEF_PIXEL_SCREEN_INVERSIE != 0)`. De gridberekening gebruikt de concrete `gfx`-afmetingen, margins, karakter- en regelstappen; bij een te klein grid volgt `_CRITICAL_PS305`.

## `Screen::afmelden()`

De huidige code zet de betrokken `actief`-statussen uit, meldt eerst Serial-, Pixel- en Character-children af (indien aanwezig), zet hun pointers op `nullptr` en roept tenslotte `GedeeldeBusNode::afmelden()` aan. Als een `child->afmelden()` faalt, retourneert deze functie meteen `false`.

---

# Callbacks en lifecycle

Callbacks vervangen de standaardweergave, niet de lifecycle.

```cpp
Screen.Character.RegistreerCallback(...)
Screen.Pixel.RegistreerCallback(...)
```

maakt een fysiek Screen-object niet automatisch aangemeld, gecontroleerd, ingeplugd of actief.

De bestaande callbackwerking en timing blijven behouden.

---

# Bestaande functionaliteit

De lifecycleherwerking verandert niet stilzwijgend:

```text
Screen.Print()
ScreenData
Character-callbacks
Pixel-callbacks
foutcodes
fatale Serial-terugval
Character-paginering
Pixel-grid
delayTime
action
delayTussenPaginas
compile-time SCREEN_OUTPUT
Pixel-rotatie
aansluitingen
regressiematrix
```

De bestaande documentatie daarvoor blijft hieronder volledig onderdeel van dit pakketdocument.


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

Meerdere uitvoerdoelen worden gecombineerd met `|`. De standaard seriële `Screen.Print()`-uitvoer via `SCREEN_TYPE_SERIAL` volgt de bestaande `DEBUG`-werking: wanneer `DEBUG` actief is, voegt `Screen.h` `SCREEN_TYPE_SERIAL` automatisch toe aan de effectieve `SCREEN_OUTPUT` en wordt de seriële debuguitvoer beschikbaar. Bij de eerste normale SerialScreen-verbinding wacht de library maximaal `SERIAL_CONNECT_TIMEOUT_MS`; na timeout wordt Serial voor die sessie als niet beschikbaar beschouwd en wordt `CRITICAL: SS001` rechtstreeks via beschikbare andere schermen/callbacks gemeld. De afzonderlijke foutfallback voor kritieke schermfouten kan Serial bewust rechtstreeks forceren; zie `SCREEN_FOUTCODES.md`.

De concrete seriële interface loopt via `GA_SERIAL`: bij `BOARD_ESP32S3_ARDI32` en `BOARD_ESP32S3_DEV` is dat `Serial0`, op de andere boards `Serial`.

## Characterscherm zonder callback

Zonder geregistreerde charactercallback gebruikt de library `LiquidCrystal_I2C`. Ondersteunde schermen zijn `SCREEN_LCD1602`, `SCREEN_LCD1604`, `SCREEN_LCD2002`, `SCREEN_LCD2004` en `SCREEN_LCD4002`.


Regel 1 en regel 2 worden eerst weergegeven. Daarna wordt `delayTime` één keer uitgevoerd en wordt `action` achter regel 2 verwerkt. Wanneer regel 3 of regel 4 aanwezig is, wacht de library daarna `delayTussenPaginas`. Vervolgens worden regel 3 en regel 4 weergegeven. Op een scherm met twee regels wordt daarvoor het scherm gewist en vormen regel 3 en regel 4 de tweede pagina. Op een scherm met vier regels worden regel 3 en regel 4 op regels 2 en 3 geplaatst.

## PixelScreen zonder callback

Screen-fouten worden gemeld met lifecyclegebonden codes zoals `CS001`, `CS301`, `PS001`, `PS305` of `SS302`. De actuele codes en hun betekenis staan in [Screen-foutcodes](SCREEN_FOUTCODES.md). Waar mogelijk wordt een fout van één uitvoer via een andere nog werkende Screen-uitvoer gemeld.

Vanaf v2.0.0 bezit `Screen` zelf het concrete schermobject (`Screen.Pixel.display`): `Adafruit_ST7789` bij `ACTIEF_PIXEL_SCREEN_CONTROLLER` `SCREEN_ST7789`, of `Adafruit_ST7735` bij `SCREEN_ST7735S` (geïnitialiseerd met `initR(INITR_BLACKTAB)`, de gangbare 1,8″-module van 128x160) en de algemene `Adafruit_GFX*`-pointer (`Screen.Pixel.gfx`). De sketch maakt geen afzonderlijk `pixelScreen`-object buiten `Screen` meer aan. De normale setup is:

```cpp
Screen.Aanmelden();
Screen.Controleren();
Screen.Inpluggen();
```

`Screen::inpluggen()` plugt de concrete Character-, Pixel- en Serial-children via GedeeldeBus in. Er bestaan geen afzonderlijke `Screen.Character.Inpluggen()`, `Screen.Pixel.Inpluggen()` of `Screen.Serial.Inpluggen()`-functies. Pas in `Screen::Activeren()` wordt het pixelscherm geïnitialiseerd: bij ST7735S met `initR(INITR_BLACKTAB)`, bij ST7789 met `init(breedte, hoogte)`. Daarna volgen `setRotation(...)`, uitsluitend bij ST7789 `invertDisplay(ACTIEF_PIXEL_SCREEN_INVERSIE != 0)` en de `Adafruit_GFX`-koppeling. In dezelfde activatiefase worden de afmetingen na rotatie 0 tot en met 3 gecontroleerd, de marges verrekend, het tekstgrid berekend en tekstgrootte, kleuren en tekstomloop ingesteld. ST7789 gebruikt standaard inversie `1` (bestaand Adafruit-gedrag); `0` schakelt die uit. ST7735S blijft ongewijzigd.

Omdat het concrete object onderdeel van `Screen` is, kan de actieve displaydriver nadien rechtstreeks worden aangesproken, bijvoorbeeld `Screen.Pixel.display.setRotation(1);`. Die rechtstreekse driveraanroep wijzigt het fysieke pixelscherm; de standaard `Screen.Print()`-gridberekening blijft gebaseerd op de activatie die door `Screen::Activeren()` is uitgevoerd.

Zonder geregistreerde pixelcallback verwerkt de standaarduitvoer alleen `ScreenData::TYPE_NONE`. De tekst wordt met het ingebouwde vaste Adafruit_GFX-font in een gecentreerd grid van minimaal 16×2 en maximaal 40×4 geplaatst. Bij `SCREEN_128X160` vereist dat `PIXEL_SCREEN_TEXT_SIZE=1`; een grotere tekstgrootte wordt vooraf bij compilatie geweigerd, terwijl de runtimecontrole `PS305` voor andere te kleine grids behouden blijft. `PIXEL_SCREEN_MARGIN` bepaalt de minimale vrije ruimte aan iedere schermrand. `PIXEL_SCREEN_CHARACTER_SPACING` en `PIXEL_SCREEN_LINE_SPACING` bepalen de extra witruimte tussen tekens en regels. Om karakterafstand mogelijk te maken, plaatst de standaarduitvoer ieder teken afzonderlijk op de berekende cursorpositie. Voor andere fonts, vrije pixelposities, grafieken of andere `ScreenData`-typen is een pixelcallback bedoeld. De standaard cursoradministratie gaat uit van eenvoudige tekens; UTF-8-tekens kunnen meerdere bytes tellen en worden niet volledig ondersteund.

Hardwarematig getest en werkend: de 1,8-inch ST7735S met `INITR_BLACKTAB` en rode tekst zonder ruis aan de randen; de 2,8-inch ST7789V via SPI met `ACTIEF_PIXEL_SCREEN_INVERSIE=0`. Dit bevestigt niet automatisch andere PixelScreen-resoluties of -modules.

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
TYPE_FAULT
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

### Kleuren per type (PixelScreen)

`SystemConfig.h` definieert vier `#ifndef`-beschermde RGB565-kleuren, bedoeld om in een zelfgeschreven PixelScreen-callback per `ScreenData`-type een andere tekstkleur te tonen:

```cpp
PIXEL_SCREEN_KLEUR_FATAL    // 0xF800, rood
PIXEL_SCREEN_KLEUR_FAULT    // 0xFC00, oranje
PIXEL_SCREEN_KLEUR_WARNING  // 0xFFE0, geel
PIXEL_SCREEN_KLEUR_INFO     // 0x07FF, cyaan
PIXEL_SCREEN_KLEUR_CRITICAL // 0xF81F, magenta
PIXEL_SCREEN_KLEUR_ABORT    // 0x780F, paars
PIXEL_SCREEN_KLEUR_PANIC    // 0xFFFF, wit
```

Ruwe RGB565-hexwaarden, niet gebonden aan een specifieke driverbibliotheek (zoals `ST77XX_RED`), zodat ze werken ongeacht welk PixelScreen-driver gebruikt wordt. `examples/Systeem/Screen/Callback_PixelScreen/Callback_PixelScreen.ino` toont hoe dit in de callback toegepast wordt via een `switch` op `screenData`.

### TYPE_FATAL, TYPE_PANIC, TYPE_ABORT, TYPE_CRITICAL: gegarandeerde Serial-terugval

Wanneer `Screen.Print()` met een van deze vier types aangeroepen wordt terwijl geen enkel scherm en geen enkele callback beschikbaar is, forceert `Screen.PrintPrivate()` `GA_SERIAL.begin(SERIAL_BAUDRATE)` en toont de melding daar, naar analogie van de bestaande `FATAL: CSxxx`/`PSxxx`-terugval (zie `SCREEN_FOUTCODES.md`). Dit is een bewuste uitzondering: enkel voor deze vier types, en enkel onder deze specifieke voorwaarde. Is er wél een scherm of callback actief, dan lopen ze gewoon via het normale pad hierboven.

## Callbacktypen

Character en Pixel gebruiken afzonderlijke callbacktypen. De Pixel-callback ontvangt als eerste argument ook `ScreenData`.

```cpp
typedef void (*CharacterScreenCallback)(const String& eersteRegel, const String& tweedeRegel, unsigned long delayTime, const String& action, const String& derdeRegel, const String& vierdeRegel, unsigned long delayTussenPaginas);
typedef void (*PixelScreenCallback)(ScreenData screenData, const String& eersteRegel, const String& tweedeRegel, unsigned long delayTime, const String& action, const String& derdeRegel, const String& vierdeRegel, unsigned long delayTussenPaginas);
```

Registratie:

```cpp
Screen.Character.RegistreerCallback(MijnCharacterScreen);
Screen.Pixel.RegistreerCallback(MijnPixelScreen);
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

Een I2C-characterscherm gebruikt `VCC`, `GND`, `SDA` en `SCL`. Controleer `I2C_ADDRESS_CHARACTER_SCREEN` en de spanning van de gebruikte backpack.

Een SPI-PixelScreen op Arduino UNO gebruikt voor hardware-SPI standaard `D11` als MOSI en `D13` als SCK. `CS`, `DC` en `RST` worden ingesteld met `PIXEL_SCREEN_CS`, `PIXEL_SCREEN_DC` en `PIXEL_SCREEN_RST`. De standaard `PIXEL_SCREEN_RST` is Arduino Uno-shieldpin `D7`. Controleer altijd de voedingsspanning en logicaniveaus van de concrete displaymodule.

De geteste 2,8-inch ST7789V SPI-module heeft ook een aanraaklaag: de `T_`-pinnen worden door de huidige Screen-laag niet ondersteund en blijven los. De microSD werkt via een afzonderlijke SD-header van 4 pinnen, met **aangesloten `SD_MISO`**. Zonder die verbinding mislukte de initialisatie in deze testopstelling (`GB304` bij `DEBUG`, anders `GB301`); de code is niet specifiek voor een ontbrekende MISO-verbinding. Ook microSD met CS via PCF8574-uitgang P0 is op hardware getest en werkt.

## Beperkingen

De huidige standaardimplementatie gebruikt blokkerende `delay()`-logica. Lange wachttijden onderbreken andere verwerking.

## Uitvoeringscontract

`Screen.Print()` verwerkt de standaarduitvoer in deze volgorde. Bij Serial-only geldt Serial eveneens als standaardscherm voor `delayTime` en `action`; wanneer CharacterScreen en/of PixelScreen meegecompileerd zijn, blijft hun bestaande timing-/callbackcontract bepalend en verandert een aanvullend Serial-doel dat contract niet.

`Screen.Print()` verwerkt de standaarduitvoer in deze volgorde:

1. regel 1 en regel 2 worden op alle actieve standaardschermen getoond;
2. `delayTime` wordt exact één keer uitgevoerd;
3. `action` wordt daarna achter regel 2 verwerkt;
4. wanneer regel 3 of regel 4 aanwezig is, wordt `delayTussenPaginas` exact één keer uitgevoerd;
5. regel 3 en regel 4 worden daarna op alle actieve standaardschermen getoond.

CharacterScreen en PixelScreen blijven bij gecombineerde standaarduitvoer synchroon. Een geregistreerde callback wordt exact één keer aangeroepen. `action`, `derdeRegel` en `vierdeRegel` worden ongewijzigd doorgegeven. Wanneer een standaardscherm actief is, ontvangt de callback voor `delayTime` en `delayTussenPaginas` de waarde `0`, omdat de standaarduitvoer de toepasselijke wachttijden al afhandelt. Wanneer geen standaardscherm actief is, ontvangt de callback de oorspronkelijke wachttijden. Bij gelijktijdige CharacterScreen- en PixelScreen-callbacks is de gebruiker verantwoordelijk voor de gewenste synchronisatie en voor het voorkomen van dubbele wachttijden.

`PIXEL_SCREEN_ROTATION` bepaalt de rotatie waarmee `Screen::Activeren()` de standaard PixelScreen-uitvoer configureert. `Screen.Pixel.display` blijft daarnaast rechtstreeks toegankelijk voor driverbewerkingen. De standaard `Screen.Print()`-gridberekening hoort bij de activatie die via `Screen::Activeren()` uitgevoerd werd.

## Doel van de Screen-voorbeelden

- `Default_CharacterScreen`: standaard CharacterScreen-uitvoer, twee pagina's, leestijd en action;
- `Default_PixelScreen`: dezelfde standaardopdracht zonder callback op het PixelScreen;
- `Default_CharacterScreen_PixelScreen`: identieke inhoud en één gezamenlijke paginavertraging op beide standaardschermen;
- `Callback_CharacterScreen`: volledige CharacterScreen-opdracht via één callback;
- `Callback_PixelScreen`: volledige getypeerde PixelScreen-opdracht via één callback.

## Regressiematrix

Controleer bij een release minstens deze `SCREEN_OUTPUT`-combinaties: `SCREEN_TYPE_NONE`, `SCREEN_TYPE_SERIAL`, `SCREEN_TYPE_CHARACTER`, `SCREEN_TYPE_PIXELS`, `SCREEN_TYPE_SERIAL | SCREEN_TYPE_CHARACTER`, `SCREEN_TYPE_SERIAL | SCREEN_TYPE_PIXELS`, `SCREEN_TYPE_CHARACTER | SCREEN_TYPE_PIXELS` en `SCREEN_TYPE_SERIAL | SCREEN_TYPE_CHARACTER | SCREEN_TYPE_PIXELS`. Controleer voor de relevante combinaties standaarduitvoer, CharacterScreen-callback, PixelScreen-callback, twee en vier regels, `action`, `delayTime`, `delayTussenPaginas` en PixelScreen-rotatie 0 tot en met 3.
