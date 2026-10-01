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
bool Screen::Inpluggen()
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

# Lifecycle

```text
Screen.Aanmelden()
↓
Screen.Controleren()
↓
Screen.Inpluggen()
↓
Screen.actief
↓
Screen.Afmelden()
```

`Configureren()` bestaat niet meer voor Screen. Alle Screen-initialisatie loopt via `Screen::Aanmelden()`, `Screen::Controleren()` en `Screen::Inpluggen()`.

---

## `Screen::Aanmelden()`

`Screen::Aanmelden()` meldt alleen het parentobject `Screen` aan.

```cpp
bool Screen::Aanmelden() {
  return GedeeldeBusNode::aanmelden();
}
```

De constructor blijft:

```cpp
Screen::Screen()
  : GedeeldeBusNode(
      &Native,
      { nullptr, 0, nullptr, 0 },
      GedeeldeBusComponent::SCREEN,
      HardwareResourceToegang::GEDEELD
    )
{}
```

Hier gebeurt niet:

```text
CharacterScreen aanmaken
PixelScreen aanmaken
SerialOutput aanmaken
display.init()
Serial.begin()
actief = true
```

---

## `Screen::Controleren()`

`Screen::Controleren()` controleert het parentobject `Screen`.

```cpp
bool Screen::Controleren() {
  return GedeeldeBusNode::controleren();
}
```

`Screen` claimt zelf geen fysieke Screen-resources:

```cpp
{ nullptr, 0, nullptr, 0 }
```

De concrete Character-, Pixel- en Serial-resources worden gecontroleerd door hun eigen GedeeldeBus-object wanneer dat via `ObjectAanmakenEnInpluggenGedeeldeBus()` wordt opgebouwd.

---

## `Screen::Inpluggen()`

Eerst:

```cpp
if (!GedeeldeBusNode::inpluggen()) {
  this->actief = false;
  return false;
}
```

Daarna worden de gecompileerde concrete Screen-uitvoerdoelen verwerkt.

### Character

Correcte leden:

```cpp
this->Character.actief
this->Character.display
this->Character.foutmelding
this->Character.foutmeldingBuffer
this->Character.foutmeldingWeergegeven
this->Character.gedeeldeBus
```

Start:

```cpp
this->Character.actief = false;
this->Character.foutmelding = nullptr;
this->Character.foutmeldingWeergegeven = false;
```

GedeeldeBus-object:

```cpp
if (this->Character.gedeeldeBus == nullptr) {
  this->Character.gedeeldeBus =
    ObjectAanmakenEnInpluggenGedeeldeBus<::CharacterScreen>(
      this,
      GedeeldeBusComponent::CHARACTER_SCREEN,
      I2C_ADRES,
      HardwareResourcePin::SDA,
      HardwareResourcePin::SCL
    );
}
```

Bij:

```cpp
this->Character.gedeeldeBus == nullptr
```

volgt:

```cpp
this->Character.actief = false;
this->actief = false;
return false;
```

Het CharacterScreen gebruikt uitsluitend het geconfigureerde `I2C_ADRES`. Er bestaan geen adresmodi meer en er wordt niet automatisch naar 0x27 of 0x3F uitgeweken.

De fysieke aanwezigheidscontrole gebeurt in `CharacterScreen::Activeren()` op het adres dat reeds door GedeeldeBus is aangemeld en gecontroleerd:

```cpp
if (!HardwareResourceTypeI2C::Activeren()) return false;

Wire.beginTransmission(adres);
return Wire.endTransmission() == 0;
```

Wanneer deze controle faalt, faalt `GedeeldeBusNode::inpluggen()` en wordt de dynamisch aangemaakte `CharacterScreen`-node afgemeld. `ObjectAanmakenEnInpluggenGedeeldeBus()` geeft dan `nullptr` terug. `Screen::Inpluggen()` zet daarop:

```cpp
this->Character.foutmelding = _FATAL_CS001;
this->Character.actief = false;
```

Daarna:

```cpp
this->Character.display.init();
this->Character.display.backlight();
this->Character.actief = true;
```

`this->Character.actief` wordt dus pas `true` nadat de volledige Character-initialisatie geslaagd is.

### Pixel

Correcte leden:

```cpp
this->Pixel.actief
this->Pixel.aantalKolommen
this->Pixel.aantalRegels
this->Pixel.display
this->Pixel.emulateLCDxxx4
this->Pixel.foutmelding
this->Pixel.foutmeldingWeergegeven
this->Pixel.gedeeldeBus
this->Pixel.offsetX
this->Pixel.offsetY
this->Pixel.gfx
```

Start:

```cpp
this->Pixel.actief = false;
this->Pixel.foutmelding = nullptr;
this->Pixel.foutmeldingWeergegeven = false;
```

GedeeldeBus-object:

```cpp
if (this->Pixel.gedeeldeBus == nullptr) {
  this->Pixel.gedeeldeBus =
    ObjectAanmakenEnInpluggenGedeeldeBus<::PixelScreen>(
      this,
      GedeeldeBusComponent::PIXEL_SCREEN,
      PIXEL_SCREEN_CS,
      HardwareResourcePin::SCK,
      HardwareResourcePin::MISO,
      HardwareResourcePin::MOSI,
      PIXEL_SCREEN_DC,
      PIXEL_SCREEN_RST
    );
}
```

Bij falen:

```cpp
this->Pixel.actief = false;
this->actief = false;
return false;
```

Displayinitialisatie:

```cpp
this->Pixel.display.init(
  ACTIEF_PIXEL_SCREEN_BREEDTE,
  ACTIEF_PIXEL_SCREEN_HOOGTE
);

this->Pixel.display.setRotation(PIXEL_SCREEN_ROTATION);
this->Pixel.gfx = &this->Pixel.display;
```

Bij ontbrekende pointer:

```cpp
this->Pixel.actief = false;
this->Pixel.foutmelding = _FATAL_PS001;

if (this->Pixel.gedeeldeBus != nullptr) {
  this->Pixel.gedeeldeBus->afmelden();
  this->Pixel.gedeeldeBus = nullptr;
}

this->actief = false;
return false;
```

Rotatie `1` of `3`:

```cpp
if (
  this->Pixel.gfx->width() != ACTIEF_PIXEL_SCREEN_HOOGTE ||
  this->Pixel.gfx->height() != ACTIEF_PIXEL_SCREEN_BREEDTE
) {
  this->Pixel.actief = false;
  this->Pixel.foutmelding = _FATAL_PS002;

  if (this->Pixel.gedeeldeBus != nullptr) {
    this->Pixel.gedeeldeBus->afmelden();
    this->Pixel.gedeeldeBus = nullptr;
  }

  this->actief = false;
  return false;
}
```

Rotatie `0` of `2` gebruikt dezelfde foutflow met `_FATAL_PS003`.

Gridberekening:

```cpp
int16_t bruikbareBreedte =
  this->Pixel.gfx->width() - (2 * PIXEL_SCREEN_MARGIN);

int16_t bruikbareHoogte =
  this->Pixel.gfx->height() - (2 * PIXEL_SCREEN_MARGIN);

int32_t ruweKolommen =
  (bruikbareBreedte + PIXEL_SCREEN_CHARACTER_SPACING) /
  this->Pixel.KarakterStap();

int32_t ruweRegels =
  (bruikbareHoogte + PIXEL_SCREEN_LINE_SPACING) /
  this->Pixel.RegelStap();
```

Minimum niet gehaald:

```cpp
this->Pixel.actief = false;
this->Pixel.foutmelding = _FATAL_PS004;

if (this->Pixel.gedeeldeBus != nullptr) {
  this->Pixel.gedeeldeBus->afmelden();
  this->Pixel.gedeeldeBus = nullptr;
}

this->actief = false;
return false;
```

Daarna blijven de bestaande berekeningen voor:

```cpp
this->Pixel.aantalKolommen
this->Pixel.aantalRegels
this->Pixel.offsetX
this->Pixel.offsetY
this->Pixel.emulateLCDxxx4
```

en:

```cpp
this->Pixel.gfx->setTextSize(PIXEL_SCREEN_TEXT_SIZE);

this->Pixel.gfx->setTextColor(
  PIXEL_SCREEN_TEXT_COLOR,
  PIXEL_SCREEN_BACKGROUND_COLOR
);

this->Pixel.gfx->setTextWrap(false);
```

Pas daarna:

```cpp
this->Pixel.actief = true;
```

### Serial

Correcte leden:

```cpp
this->Serial.actief
this->Serial.gedeeldeBus
```

Start:

```cpp
this->Serial.actief = false;
```

GedeeldeBus-object:

```cpp
if (this->Serial.gedeeldeBus == nullptr) {
  this->Serial.gedeeldeBus =
    ObjectAanmakenEnInpluggenGedeeldeBus<SerialOutput>(
      this,
      GedeeldeBusComponent::SERIAL_OUTPUT,
      HardwareResourcePin::D1,
      HardwareResourcePin::D0
    );
}
```

Bij falen:

```cpp
this->Serial.actief = false;
this->actief = false;
return false;
```

Serialinitialisatie:

```cpp
::GA_SERIAL.begin(SERIAL_BAUDRATE);

const unsigned long startTijd = millis();

while (
  !::GA_SERIAL &&
  (millis() - startTijd) < SERIAL_CONNECT_TIMEOUT_MS
) {
  ;
}

this->Serial.actief = (bool)::GA_SERIAL;
```

Bij timeout:

```cpp
if (!this->Serial.actief) {
  if (this->Serial.gedeeldeBus != nullptr) {
    this->Serial.gedeeldeBus->afmelden();
    this->Serial.gedeeldeBus = nullptr;
  }

  this->actief = false;
  return false;
}
```

### Einde

Alle gecompileerde uitvoerdoelen moeten volledig actief zijn.

Alleen dan:

```cpp
this->actief = true;
return true;
```

Bij een fout:

```cpp
this->actief = false;
return false;
```

---

# `Screen::Afmelden()`

Afmelden gebeurt child voor child en daarna pas voor `Screen`.

Character:

```cpp
this->Character.actief = false;

if (this->Character.gedeeldeBus != nullptr) {
  if (!this->Character.gedeeldeBus->afmelden()) {
    this->actief = false;
    return false;
  }

  this->Character.gedeeldeBus = nullptr;
}
```

Pixel:

```cpp
this->Pixel.actief = false;
this->Pixel.gfx = nullptr;

if (this->Pixel.gedeeldeBus != nullptr) {
  if (!this->Pixel.gedeeldeBus->afmelden()) {
    this->actief = false;
    return false;
  }

  this->Pixel.gedeeldeBus = nullptr;
}
```

Serial:

```cpp
this->Serial.actief = false;

if (this->Serial.gedeeldeBus != nullptr) {
  if (!this->Serial.gedeeldeBus->afmelden()) {
    this->actief = false;
    return false;
  }

  this->Serial.gedeeldeBus = nullptr;
}
```

Daarna:

```cpp
this->actief = false;
return GedeeldeBusNode::afmelden();
```

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

Fatale activatiefouten van CharacterScreen en PixelScreen worden gemeld met een korte code zoals `CS000` of `PS001`. De volledige betekenis en oplossing staan in [Screen-foutcodes](SCREEN_FOUTCODES.md). Is geen van beide schermtypes beschikbaar, dan forceert de library voor deze melding Serial op `SERIAL_BAUDRATE`.

Vanaf v2.0.0 bezit `Screen` zelf het concrete `Adafruit_ST7789`-object (`Screen.Pixel.display`) en de algemene `Adafruit_GFX*`-pointer (`Screen.Pixel.gfx`). De sketch maakt geen afzonderlijk `pixelScreen`-object buiten `Screen` meer aan. De normale setup is:

```cpp
Screen.Aanmelden();
Screen.Controleren();
Screen.Inpluggen();
```

`Screen::Inpluggen()` bestuurt de concrete Character-, Pixel- en Serial-uitvoer. Er bestaan geen afzonderlijke `Screen.Character.Inpluggen()`, `Screen.Pixel.Inpluggen()` of `Screen.Serial.Inpluggen()`-functies. Na een geslaagde PixelScreen-inplug voert `Screen` intern, in deze volgorde, `Screen.Pixel.display.init(...)`, `Screen.Pixel.display.setRotation(...)` en `Screen.Pixel.gfx = &Screen.Pixel.display` uit. Daarna controleert `Screen.Inpluggen()` de gekoppelde `Adafruit_GFX`-instantie, houdt rekening met rotatie 0 tot en met 3, trekt de ingestelde buitenmarges van de beschikbare schermruimte af, berekent het tekstgrid en stelt tekstgrootte, tekstkleur, achtergrondkleur en tekstomloop in.

Omdat het concrete object onderdeel van `Screen` is, kan de actieve displaydriver nadien rechtstreeks worden aangesproken, bijvoorbeeld `Screen.Pixel.display.setRotation(1);`. Die rechtstreekse driveraanroep wijzigt het fysieke pixelscherm; de standaard `Screen.Print()`-gridberekening blijft gebaseerd op de activatie die door `Screen.Inpluggen()` is uitgevoerd.

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

Een I2C-characterscherm gebruikt `VCC`, `GND`, `SDA` en `SCL`. Controleer `I2C_ADRES` en de spanning van de gebruikte backpack.

Een SPI-PixelScreen op Arduino UNO gebruikt voor hardware-SPI standaard `D11` als MOSI en `D13` als SCK. `CS`, `DC` en `RST` worden ingesteld met `PIXEL_SCREEN_CS`, `PIXEL_SCREEN_DC` en `PIXEL_SCREEN_RST`. De standaard `PIXEL_SCREEN_RST` is Arduino Uno-shieldpin `D7`. Controleer altijd de voedingsspanning en logicaniveaus van de concrete displaymodule.

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

`PIXEL_SCREEN_ROTATION` bepaalt de rotatie waarmee `Screen.Inpluggen()` de standaard PixelScreen-uitvoer configureert. `Screen.Pixel.display` blijft daarnaast rechtstreeks toegankelijk voor driverbewerkingen. De standaard `Screen.Print()`-gridberekening hoort bij de activatie die via `Screen.Inpluggen()` uitgevoerd werd.

## Doel van de Screen-voorbeelden

- `Default_CharacterScreen`: standaard CharacterScreen-uitvoer, twee pagina's, leestijd en action;
- `Default_PixelScreen`: dezelfde standaardopdracht zonder callback op het PixelScreen;
- `Default_CharacterScreen_PixelScreen`: identieke inhoud en één gezamenlijke paginavertraging op beide standaardschermen;
- `Callback_CharacterScreen`: volledige CharacterScreen-opdracht via één callback;
- `Callback_PixelScreen`: volledige getypeerde PixelScreen-opdracht via één callback.

## Regressiematrix

Controleer bij een release minstens deze `SCREEN_OUTPUT`-combinaties: `SCREEN_TYPE_NONE`, `SCREEN_TYPE_SERIAL`, `SCREEN_TYPE_CHARACTER`, `SCREEN_TYPE_PIXELS`, `SCREEN_TYPE_SERIAL | SCREEN_TYPE_CHARACTER`, `SCREEN_TYPE_SERIAL | SCREEN_TYPE_PIXELS`, `SCREEN_TYPE_CHARACTER | SCREEN_TYPE_PIXELS` en `SCREEN_TYPE_SERIAL | SCREEN_TYPE_CHARACTER | SCREEN_TYPE_PIXELS`. Controleer voor de relevante combinaties standaarduitvoer, CharacterScreen-callback, PixelScreen-callback, twee en vier regels, `action`, `delayTime`, `delayTussenPaginas` en PixelScreen-rotatie 0 tot en met 3.
