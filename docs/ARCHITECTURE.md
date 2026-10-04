# Architectuur

## Status en bron van waarheid

Dit document beschrijft de actuele broncode van **beta v2.0.0**. Voor de technische architectuur geldt de code onder `src/` als bron van waarheid. Historische release-informatie blijft in `CHANGELOG.md`, `docs/DECISION_LOG.md` en `extras/TESTRESULTATEN.md` staan.

## Eén geïntegreerde library

GroeiAcademie FrameWork wordt als één samenwerkende Arduino-library ontwikkeld. Functionele onderdelen worden niet zonder expliciete reden opgesplitst in afzonderlijke libraries.

## Huidige hoofdstructuur

```text
GroeiAcademie/
├── src/
│   ├── GroeiAcademie.h
│   ├── Input.h
│   ├── Screen.h
│   ├── Stimulus.h
│   ├── SystemConfig.h
│   ├── Configuratie/
│   │   ├── Examples.h
│   │   ├── ExamplesConfig.h
│   │   ├── StimulusConfig.h
│   │   ├── SystemConfig.h
│   │   ├── UserConfig.h
│   │   └── UserConfig_template.h
│   ├── Language/
│   │   ├── Examples_DE.h / Examples_EN.h / Examples_FR.h / Examples_NL.h
│   │   ├── Library_DE.h / Library_EN.h / Library_FR.h / Library_NL.h
│   │   ├── UserExample_*_template.h
│   │   └── UserLibrary_*_template.h
│   ├── Systeem/
│   │   ├── GedeeldeBus/
│   │   │   ├── GedeeldeBus.h
│   │   │   ├── GedeeldeBus.cpp
│   │   │   ├── ExtenderADS1115.h/.cpp
│   │   │   ├── ExtenderADS1158.h/.cpp
│   │   │   ├── ExtenderADS7828.h/.cpp
│   │   │   ├── ExtenderADS7953.h/.cpp
│   │   │   ├── ExtenderCD74HC4067.h/.cpp
│   │   │   ├── ExtenderDS2482v800.h/.cpp
│   │   │   ├── ExtenderMAX14830I2C.h/.cpp
│   │   │   ├── ExtenderMAX14830SPI.h/.cpp
│   │   │   ├── ExtenderMCP23017.h/.cpp
│   │   │   ├── ExtenderPCF8574.h/.cpp
│   │   │   ├── ExtenderPCF8575.h/.cpp
│   │   │   ├── ExtenderSC16IS752I2C.h/.cpp
│   │   │   ├── ExtenderSC16IS752SPI.h/.cpp
│   │   │   └── ExtenderTCA9548A.h/.cpp
│   │   ├── Input/
│   │   │   ├── Input.h
│   │   │   ├── Input.cpp
│   │   │   └── InputTypes.h
│   │   ├── Screen/
│   │   │   ├── Screen.h
│   │   │   ├── Screen.cpp
│   │   │   └── ScreenTypes.h
│   │   └── Sensor/
│   │       ├── RFP602.h
│   │       └── RFP602.cpp
│   ├── Toepassingsgebieden/
│   │   └── Stimulus/
│   │       ├── Stimulus.h
│   │       └── Stimulus.cpp
│   └── Uitbreidingskaarten/
├── examples/
│   ├── Systeem/
│   │   ├── ADC_Backend/
│   │   ├── GedeeldeBus/
│   │   │   ├── Extenders/
│   │   │   └── Sensoren/
│   │   ├── Input/
│   │   └── Screen/
│   └── Toepassingsgebieden/
│       └── Stimulus/
├── docs/
│   ├── Configuratie/
│   ├── Systeem/
│   ├── Toepassingsgebieden/
│   ├── Uitbreidingskaarten/
│   ├── ARCHITECTURE.md
│   ├── DECISION_LOG.md
│   ├── HARDWARE_SUPPORT.md
│   └── ROADMAP.md
├── extras/
├── .github/
├── keywords.txt
├── library.properties
├── CITATION.cff
└── README.md
```

`src/Uitbreidingskaarten/` is momenteel een gereserveerde broncodemap. De hardwaredocumentatie van de Stimulus Shield staat wel onder `docs/Uitbreidingskaarten/`.

## Publieke headers

Gebruikers kunnen de volledige library opnemen met:

```cpp
#include <GroeiAcademie.h>
```

`src/GroeiAcademie.h` neemt momenteel `Screen.h`, `Stimulus.h` en `Input.h` op. Daarnaast zijn de publieke moduleheaders rechtstreeks beschikbaar:

```cpp
#include <Input.h>
#include <Screen.h>
#include <Stimulus.h>
#include <SystemConfig.h>
```

De headers direct onder `src/` leiden door naar de interne moduleheaders. GedeeldeBus en de concrete Sensor- en Extenderheaders zijn in de huidige code interne systeemheaders; voorbeelden die een concrete Sensor rechtstreeks testen nemen die interne header expliciet op.

## Architectuurgrens van GedeeldeBus

GedeeldeBus is de gemeenschappelijke boom- en resourcebeheerlaag. De generieke kern staat in `src/Systeem/GedeeldeBus/GedeeldeBus.h/.cpp`. Concrete Extenders staan voor onderhoud en leesbaarheid in afzonderlijke bestanden, maar blijven architecturaal onderdeel van GedeeldeBus.

De actuele boom gebruikt één `GedeeldeBusNode`-structuur voor root-, parent- en childnodes. `Native` is de root. Top-level componenten hangen aan `Native`; een Sensor kan als child van een ADC-extender hangen; Input en Screen maken zelf children aan voor hun concrete kanalen.

De code gebruikt geen centrale array met een vast maximum. Elke node bevat `parent`, `firstChild`, `nextChild` en `prevChild` en krijgt bij constructie een uniek `id` via `volgendeGedeeldeBusId`.

## Lifecycle

De generieke lifecycle bestaat uit:

```text
aanmelden()
→ controleren()
→ inpluggen()
→ activeren()
```

`GedeeldeBusNewComponent<T>(...)` maakt een object met `new`, zet `componentCreated = true` en voert exact deze vier stappen uit. Wanneer een stap faalt, wordt de tijdelijke conflictinformatie afgehandeld en wordt het object afgemeld/verwijderd of rechtstreeks verwijderd. Bij succes wordt de pointer teruggegeven.

`afmelden()` verwijdert een node uit de boom. Een node met nog children kan niet worden afgemeld. Alleen objecten die door `GedeeldeBusNewComponent()` als dynamisch component zijn aangemaakt worden via `delete this` verwijderd; statisch aangemaakte objecten niet.

De basisstatus in `GedeeldeBusNode` bestaat uit `aangemeld`, `gecontroleerd`, `ingeplugd`, `actief`, `activerenBezig` en `conflictGevonden`.

### Diagnose

De code bevat momenteel uitsluitend deze voorbereide DEBUG-hook:

```cpp
#ifdef DEBUG
  virtual void Diagnose() {}
#endif
```

Er zijn in de huidige code nog geen concrete `Diagnose()`-overrides. Uitgebreide elektronische functietesten zijn dus nog niet geïmplementeerd als componentdiagnose.

## Resourcecontrole

### Native resources

`HardwareResourcePin` gebruikt voor de fysieke resources van de UNO-vormfactor de prefix `NP_`: `NP_D0..NP_D13`, `NP_A0..NP_A5`, `NP_SDA`, `NP_SCL`, `NP_MISO`, `NP_MOSI`, `NP_SCK` en `NP_SS`. `CUSTOM` en `NONE` blijven zonder `NP_`: dit zijn speciale waarden en geen fysieke pinnen.

`NativeArduinoPinVan()` vertaalt zo'n logische resource naar het board-specifieke Arduino-pinnummer uit `SystemConfig.h`. ExtenderPins gaan nooit door deze functie.

De waarden van `HardwareResourceType` gebruiken de prefix `RT_`, bijvoorbeeld `RT_I2C`, `RT_SPI`, `RT_GPIO` en `RT_INTERRUPT`. De groepswaarden binnen `GedeeldeBusComponent` zijn `GC_INPUT`, `GC_SCREEN`, `GC_EXTENDER` en `GC_SENSOR`; concrete componentwaarden behouden hun eigen naam.

### Gedeelde en exclusieve claims

`BezettingPinnen` bewaart gedeelde en exclusieve claims afzonderlijk. Voor gedeelde I2C- en SPI-lijnen gebruikt GedeeldeBus bovendien `HardwareResourceIdentiteit`, zodat twee claims op dezelfde fysieke pin alleen compatibel zijn wanneer hun protocolrol overeenkomt (`I2C_SDA`, `I2C_SCL`, `SPI_SCK`, `SPI_MISO`, `SPI_MOSI`).

I2C-adressen worden intern als exclusieve claim in hetzelfde mechanisme behandeld. In `HardwareResourceTypeI2C` wordt het adres opgeslagen als `adres | 0x80`, zodat het niet met normale pinwaarden samenvalt.

### Resourcegebied per Extender

`firstExtenderNode()` bepaalt in welk resourcegebied een node zit. Claims worden alleen rechtstreeks vergeleken wanneer ze tot hetzelfde resourcegebied behoren. Native claims vergelijken dus met Native claims; children van dezelfde Extender vergelijken binnen die Extender.

## I2C en SPI

`HardwareResourceTypeI2C` en `HardwareResourceTypeSPI` zijn de twee generieke tussenlagen voor concrete buscomponenten.

- I2C claimt SDA en SCL gedeeld en het adres exclusief; `inpluggen()` initialiseert I2C via `InitialiserenGedeeldeBus(GedeeldeBusType::I2C)`.
- SPI claimt SCK, MISO en MOSI gedeeld en CS exclusief; `inpluggen()` vereist een geldige CS en initialiseert SPI.
- `InitialiserenGedeeldeBus()` bewaakt met interne flags dat `Wire.begin()` en `SPI.begin()` niet telkens opnieuw worden uitgevoerd.

## Concrete Extenders

De huidige code bevat 14 concrete Extenders. Alleen een Extender die compile-time effectief nodig is, wordt mee gecompileerd. Een `EXTENDER_<NAAM>_AANTAL` groter dan 0 activeert de overeenkomstige Extender; daarnaast gebruikt de ADC-route `ExtenderADS1115` wanneer `ADC_BACKEND == ADC_BACKEND_ADS1115` en gebruikt Input `ExtenderPCF8574` wanneer `INPUT_KANAAL_CONFIG` `INPUT_TYPE_PCF8574` bevat.


1. `ExtenderADS1115`
2. `ExtenderADS1158`
3. `ExtenderADS7828`
4. `ExtenderADS7953`
5. `ExtenderCD74HC4067`
6. `ExtenderDS2482v800`
7. `ExtenderMAX14830I2C`
8. `ExtenderMAX14830SPI`
9. `ExtenderMCP23017`
10. `ExtenderPCF8574`
11. `ExtenderPCF8575`
12. `ExtenderSC16IS752I2C`
13. `ExtenderSC16IS752SPI`
14. `ExtenderTCA9548A`

Elke concrete Extender heeft een eigen `.h/.cpp`. Elke `enum class ExtenderPins` gebruikt de vaste naamconventie `EP_<pinnaam>`.

De actuele resources uit de code zijn:

| Extender | `ExtenderPins` |
|---|---|
| ADS1115 | `EP_AIN0..EP_AIN3` |
| ADS1158 | `EP_AIN0..EP_AIN15`, `EP_GPIO0..EP_GPIO7` |
| ADS7828 | `EP_CH0..EP_CH7` |
| ADS7953 | `EP_CH0..EP_CH15`, `EP_GPIO0..EP_GPIO3` |
| CD74HC4067 | `EP_Y0..EP_Y15` |
| DS2482v800 | `EP_IO0..EP_IO7` |
| MAX14830 I2C/SPI | `EP_UART0..EP_UART3`, `EP_GPIO0..EP_GPIO15` |
| MCP23017 | `EP_GPA0..EP_GPA7`, `EP_GPB0..EP_GPB7` |
| PCF8574 | `EP_P0..EP_P7` |
| PCF8575 | `EP_P00..EP_P07`, `EP_P10..EP_P17` |
| SC16IS752 I2C/SPI | `EP_CHANNEL_A`, `EP_CHANNEL_B`, `EP_GPIO0..EP_GPIO7` |
| TCA9548A | `EP_CH0..EP_CH7` |

De fabrikant-/functiebenaming blijft in de comments naast elke enumwaarde staan.

## ADC-laag en Sensor

De code bevat twee ADC-parentvarianten:

- `ADC_NATIVE : GedeeldeBusNode` voor de native analoge resources;
- `ADC_ADS1115 : ExtenderADS1115` voor de externe ADS1115-route.

Welke statische ADC-parent bestaat, wordt compile-time gekozen met `ADC_BACKEND`.

`Sensor` is momenteel een lege tussenstruct boven `GedeeldeBusNode`. De eerste concrete Sensor is `RFP602` onder `src/Systeem/Sensor/`. Eén `RFP602`-object beheert maximaal vier kanalen en hangt, afhankelijk van `ADC_BACKEND`, onder `ADC_NATIVE` of `ADC_ADS1115`. `RFP602::Activeren()` stelt bij ADS1115 de gain in; `RawAnalogRead()` leest via ADS1115 of `analogRead()`.

## Screen

`Screen` is een `GedeeldeBusNode` met runtime-selectie via `typesActief`. De concrete children zijn, afhankelijk van compile-time `SCREEN_OUTPUT`, `SerialOutput`, `CharacterScreen` en `PixelScreen`.

De constructors ondersteunen de compile-time standaardselectie, een runtime-subset en optioneel Character-/Pixelcallbacks. `Screen::aanmelden()`, `controleren()`, `inpluggen()`, `Activeren()` en `afmelden()` sturen de lifecycle door naar de geselecteerde children.

De concrete hardwarecontrole is niet overal in dezelfde lifecyclefase geplaatst. Zo controleert `CharacterScreen::Activeren()` momenteel of het I2C-adres antwoordt; `PixelScreen::inpluggen()` configureert de controlpinnen en `PixelScreen::Activeren()` retourneert in de huidige code nog `true` terwijl een uitgebreidere ID-controle uitgecommentarieerd staat. Dit is de actuele implementatie.

Zie [Systeem/SCREEN.md](Systeem/SCREEN.md).

## Input

`Input` is eveneens een `GedeeldeBusNode` met `typesActief`. Geldige compile-time waarden zijn exact:

```text
INPUT_TYPE_NONE
INPUT_TYPE_DIGITAL
INPUT_TYPE_PCF8574
INPUT_TYPE_HX1838
INPUT_TYPE_PCF8574 | INPUT_TYPE_HX1838
```

`INPUT_TYPE_DIGITAL` wordt dus niet gecombineerd met PCF8574 of HX1838. De runtime-subset moet volledig binnen `INPUT_KANAAL_CONFIG` vallen; dit wordt in `Input::aanmelden()` gecontroleerd.

De concrete children zijn `InputDigital`, `InputPCF8574` en `InputHX1838`. De vroegere publieke `InputConfigureren()`-stap bestaat niet meer. `Input::inpluggen()` beslist eerst welke geselecteerde kanalen fysiek/minimaal bruikbaar zijn: PCF8574 moet zijn `begin(0xFF)`-I2C-transactie succesvol uitvoeren; TinyIRReceiver moet zijn interruptkoppeling kunnen initialiseren; IRremote wordt gestart en met de bestaande korte `isIdle()`-stabiliteitscontrole gecontroleerd. Een falend kanaal wordt vóór de overlevingscontrole afgemeld en op `nullptr` gezet. `Input::Activeren()` werkt daarna alleen met de overgebleven kanalen, configureert de digitale pinmodes en verwerkt de HX1838-mapping- of kalibratiebron.

Zie [Systeem/INPUT.md](Systeem/INPUT.md).

## Stimulus

`Stimulus` blijft een toepassingsmodule onder `src/Toepassingsgebieden/Stimulus/`. De directe sensoruitlezing loopt in de huidige code via de globale pointer `sensorRFP602`; de voorbeelden maken `Screen`, `Input` en `sensorRFP602` via `GedeeldeBusNewComponent()` aan voordat de Stimuluslogica wordt gebruikt.

## Configuratiemodel

`src/Configuratie/SystemConfig.h` bevat vaste keuzewaarden, automatische boarddetectie, standaardwaarden, boardmapping en compile-time validaties. `UserConfig.h` wordt, tenzij `GROEIACADEMIE_IGNORE_USER_CONFIG` is gedefinieerd, vroeg in `SystemConfig.h` geladen wanneer het bestand bestaat.

De laadvolgorde is in de code:

```text
vaste keuzewaarden die vóór UserConfig nodig zijn
→ eventueel UserConfig.h
→ overige defaults
→ boardmapping
→ validaties en afgeleide configuratie
```

`ExamplesConfig.h` bevat gedeelde instellingen voor examples. `StimulusConfig.h` bevat Stimulusgrenzen.

## Verantwoordelijkheden

### Configuratie

`src/Configuratie/` bevat vaste keuzewaarden, gebruikersconfiguratie, boardkeuze en -mapping, ADC-backend, Screen-, Input-, Extender- en Sensorconfiguratie, voorbeeldinstellingen en Stimulusgrenzen.

### Systeem

`src/Systeem/` bevat frameworkbrede voorzieningen. De huidige systeemdomeinen zijn `GedeeldeBus`, `Screen`, `Input` en `Sensor`. GedeeldeBus beheert de generieke boom/lifecycle/resources; concrete Screen-, Input-, Sensor- en Extenderlogica blijft bij de betreffende component.

### Toepassingsgebieden

`src/Toepassingsgebieden/` bevat modules rond een concreet leer-, meet- of onderzoeksdoel. De huidige module is `Stimulus`.

### Gereserveerde domeinen

`src/Uitbreidingskaarten/` is momenteel een gereserveerde broncodemap. `docs/Uitbreidingskaarten/` bevat wel hardwaredocumentatie, waaronder de Stimulus Shield v1.1.2.

## Afhankelijkheden

`library.properties` declareert momenteel:

- LiquidCrystal I2C;
- Adafruit GFX Library;
- Adafruit ST7735 and ST7789 Library;
- Adafruit ADS1X15;
- PCF8574 (>=0.4.0);
- IRremote.

Niet iedere build gebruikt alle dependencycode. Preprocessorconfiguratie bepaalt welke subsystemen werkelijk worden gecompileerd.

## Samenwerking tussen modules

Screen en Input zijn frameworkbrede systeemlagen. GedeeldeBus verzorgt hun resourceboom en lifecycle, maar de concrete Screen- en Inputlogica blijft in de eigen module. Stimulus gebruikt de systeemlagen en `RFP602`, maar GedeeldeBus bevat geen Stimulus-algoritmen.

## Elektronische documentatie

Elektronische schema's worden per toepassingsgebied of uitbreidingskaart beschreven. De index staat in [Toepassingsgebieden/MODULES.md](Toepassingsgebieden/MODULES.md). De beschikbare sensoren en modules staan in [Toepassingsgebieden/SENSOR_INVENTARIS.md](Toepassingsgebieden/SENSOR_INVENTARIS.md).

Een geldig schema vermeldt ten minste boardvariant, pinbezetting, voeding/massa, componentwaarden, sensorvariant, externe modules/adressen, kalibratievoorwaarden en beperkingen.

## Testarchitectuur

De huidige examples onder `examples/Systeem/GedeeldeBus/Extenders/` bevatten per Extender een basisvoorbeeld en aparte `ExtenderPins`-tests voor toegelaten en geweigerde resources. `examples/Systeem/GedeeldeBus/Sensoren/Sensor_RFP602/` bevat de eerste Sensor-lifecycletest.

Deze tests bewijzen niet automatisch de volledige elektronische werking van elk kanaal. Zie `extras/TESTEN.md` voor het onderscheid tussen lifecycle-/resourcecontrole, elektronische functietests en praktijktests.

## Geheugen en timing

UNO R3 blijft een belangrijke ondergrens. Nieuwe code wordt beoordeeld op flash, SRAM, blokkerende wachttijden en samplegedrag. DEBUG- en TRACE-functionaliteit moet uitschakelbaar blijven. `Diagnose()` bestaat uitsluitend in DEBUG-builds.
