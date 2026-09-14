# GroeiAcademie FrameWork

> Waar subjectieve ervaringen en objectief meetbare vaardigheden elkaar ontmoeten.

Het GroeiAcademie FrameWork is een modulaire Arduino-library voor het meten, oefenen en onderzoeken van menselijke vaardigheden. De huidige versie bevat de toepassing **Stimulus** en de gedeelde systeemlagen **Screen** en **Input**.

> **Meten is Weten, in plaats van Denken te Weten.**


## Prototype Stimulus

![GROEI ACADEMIE FrameWork - Prototype Stimulus](docs/Systeem/Screenshots/GROEI%20ACADEMIE%20FrameWork%20-%20Protottype.jpg)

Op deze foto zie je het prototype op **breadboard van het Stimulus v1.1.2 shield**.

## Stimulus Shield v1.1.2

![Stimulus Shield v1.1.2](docs/Uitbreidingskaarten/Stimulus%20Shield%20v1.1.2/Schematic_Stimulus-Shield-(GroeiAcademie-FrameWork)-v1.1.2_2026-09-05.png)

Op deze afbeelding zie je het schema van het Stimulus Shield v1.1.2, het **elektronische schema** van het eerder getoonde breadboardprototype, dat de aansluitingen voor de Stimulus-toepassing samenbrengt op één uitbreidingskaart.

## Ondersteunde boards

![GROEI ACADEMIE FrameWork - Boards](docs/Systeem/Screenshots/GROEI%20ACADEMIE%20FrameWork%20-%20Board.JPG)

Op deze foto zie je alle succesvol geteste en reeds ondersteunde **ARDUINO UNO-vormfactor boards**.

## Inputkanalen

![GROEI ACADEMIE FrameWork - Input](docs/Systeem/Screenshots/GROEI%20ACADEMIE%20FrameWork%20-%20Input.JPG)

Op deze foto zie je de reeds geteste drukknoppen, membranen en afstandsbedieningen. Via het **User Defined Matrix-mechanisme** kunnen we wellicht alle gangbare varianten ondersteunen.

## Schermen

![GROEI ACADEMIE FrameWork - Screen](docs/Systeem/Screenshots/GROEI%20ACADEMIE%20FrameWork%20-%20Screen.JPG)

Op deze foto zie je de geteste schermen, die slechts enkele voorbeelden zijn van de vele mogelijke **Character- en PixelScreens**.

## Huidige status

- versie: `1.1.2`;
- de actuele boardconstanten en hun volgorde staan in `src/Configuratie/SystemConfig.h`, inclusief `BOARD_UNO_Q` en `BOARD_ESP32S3_DEV`;
- voor `BOARD_STM32F4_NUCLEO64_F401RE` is de Arduino Uno-shieldheader-mapping D0-D13, A0-A5, SDA/SCL en SPI nu actief in `SystemConfig.h` en per pin overschrijfbaar via `UserConfig.h`;
- `BOARD_STM32F4_NUCLEO64_F401RE` is hardwarematig getest met CharacterDisplay, ADS1115, PCF8574, PixelScreen en HX1838; selecteer in Arduino IDE **Nucleo-64** met part number **Nucleo F401RE** (`STMicroelectronics:stm32:Nucleo_64:pnum=NUCLEO_F401RE`); details en de vereiste PixelScreen/Adafruit-patch staan in `docs/HARDWARE_SUPPORT.md`;
- de standaard HX1838-pin is `D8` en de standaard PixelScreen-resetpin is `D7`;
- hardwarestatus: alle tien ondersteunde boards zijn **geïmplementeerd en getest**; de volledige bevestigde hardwarestatus staat in `docs/HARDWARE_SUPPORT.md` en `extras/TESTRESULTATEN.md`;
- ontwikkelfase: alpha;
- huidige implementatie: de Stimulusmodule en de gedeelde Screen- en Input-systeemlagen;
- HX1838 met `HX1838_BRON_CODES_DEFINE` is released;
- `HX1838_TOETSENINDELING_REMOTE_USER_DEFINED` laat binnen deze DEFINE-route een eigen afstandsbediening toe via een configureerbaar aantal toetsen en opschriftkoppeling in `UserConfig.h`; de 8-bit commandcodes kunnen via `HX1838_GENERIEK_CODES` worden opgegeven of automatisch worden gekalibreerd wanneer die define ontbreekt;
- deze HX1838-route is hardwarematig bevestigd met beide ontvangstbackends: TinyIRReceiver (`HX1838_USE_TINYIRRECEIVER_INSTEAD_OF_IRREMOTE = 1`) en IRremote (`HX1838_USE_TINYIRRECEIVER_INSTEAD_OF_IRREMOTE = 0`);
- de EEPROM-gebaseerde HX1838-routes blijven experimenteel;
- de interne releasetests worden in vier afzonderlijke testcycli uitgevoerd; de definitieve releaseresultaten worden vastgelegd in `extras/TESTRESULTATEN.md`;
- Arduino LINT wordt afzonderlijk geregistreerd in `extras/TESTRESULTATEN.md`;
- licentie: GNU LGPL v3.0-or-later, zie [LICENSE](LICENSE) en [LICENSE.md](LICENSE.md).

De actuele status van de geïmplementeerde en geplande toepassingsgebieden staat in [docs/Toepassingsgebieden/MODULES.md](docs/Toepassingsgebieden/MODULES.md).

## Installatie

### Via ZIP in Arduino IDE

1. Download een release-ZIP van de repository.
2. Open Arduino IDE.
3. Kies **Sketch > Include Library > Add .ZIP Library**.
4. Selecteer de ZIP.
5. Open daarna een voorbeeld via **File > Examples > GroeiAcademie FrameWork**.

### Handmatig

Plaats de map `GroeiAcademie` in de `libraries`-map van de ingestelde Arduino-sketchbooklocatie. De gebruikelijke standaardlocaties zijn:

```text
Linux:   ~/Arduino/libraries/GroeiAcademie/

macOS:   ~/Documents/Arduino/libraries/GroeiAcademie/
macOS:   /Users/<gebruikersnaam>/Documents/Arduino/libraries/GroeiAcademie/

Windows: Documents/Arduino/libraries/GroeiAcademie/
Windows: C:\Users\<gebruikersnaam>\Documents\Arduino\libraries\GroeiAcademie\
```

Herstart Arduino IDE na de installatie.

### ESP32-borden in Arduino Uno R3-vormfactor

Installeer via Arduino Boards Manager `esp32 by Espressif Systems` en selecteer daarna het boardprofiel `WeMos D1 R32`. De compilatietests gebruiken FQBN `esp32:esp32:d1_uno32`. Dit boardprofiel wordt gebruikt voor WeMos D1 R32 (ESP32-WROOM-32U) en TTGO D1 R32 (ESP32-WROOM-32U), gezamenlijk aangeduid als `ESP32-WROOM-32U` in de dependencycontrole en verzorgt de juiste omzetting van Arduino-pinnamen zoals `A0` naar de overeenkomstige ESP32-GPIO-pinnen. Een toolpakket zoals `esp32:esp-rv32@2601` wordt automatisch met het ESP32-boardpakket geïnstalleerd en wordt niet als Arduino Uno R3-vormfactorbord geselecteerd.

## Kwaliteitscontrole

Elke officiële release van deze library wordt vóór publicatie met de meegeleverde Windows-releasetests gevalideerd. Alleen `extras/LokalePaden.cmd` is machinespecifiek en wordt via `.gitignore` niet gepubliceerd; `extras/LokalePaden_template.cmd` en de gedeelde testscripts maken wel deel uit van de library.

De validatie omvat:

- Arduino LINT (Library Manager)
- volledige gereleasete regressie op Arduino UNO R4 Minima en een minimale gerichte regressie op de overige boards;
- optioneel `extras/TestLibraryGereleasedVolledigeRegresietesten.cmd` voor de volledige historische gereleasete regressiedekking, selecteerbaar per `Screen`, `Input`, `Stimulus` of combinatie;
- afzonderlijke HX1838-validatie met geldige en bewust ongeldige configuraties;
- menu-/functiekoppeling via `MappingTussenToetsaanslagEnUitTeVoerenFunctie`; één vaste mapping en meerdere expliciet doorgegeven mappings gebruiken dezelfde `UitVoerenFunctieVolgensMappingMetToetsAanslag()`-API; een volledig apart, tweede type, `MappingTussenToetsaanslagEnUitTeVoerenFunctieMetArgumenten`, geeft daarnaast een `void*`-argument door aan de gekoppelde functie, zonder het eerste type te raken;
- Arduino UNO R3;
- Arduino Uno R4 Minima;
- Arduino Uno R4 WiFi;
- WeMos D1 R32 (ESP32-WROOM-32U) en TTGO D1 R32 (ESP32-WROOM-32U) via `esp32:esp32:d1_uno32`;
- Arduino UNO Q via `arduino:zephyr:unoq`;
- Paradisetronic ESP32-S3 UNO (ESP32-S3-WROOM-1) en SB Components Ardi-32 (ESP32-S3-WROOM-1) via `esp32:esp32:esp32s3`;
- Cytron Maker UNO RP2040 en STM32F4 Nucleo-F401RE via hun eigen acceptatieprofielen;
- minimale acceptatiecompilaties voor de experimentele boardprofielen, zonder invloed op PASS/FAIL van de officieel ondersteunde boards.

Meer informatie:

- [Testprocedure](extras/TESTEN.md)
- [Testresultaten](extras/TESTRESULTATEN.md)

## Afhankelijkheden

De verplichte afhankelijkheden staan in `library.properties`:

```text
LiquidCrystal I2C
Adafruit GFX Library
Adafruit ST7735 and ST7789 Library
Adafruit ADS1X15
PCF8574 (>=0.4.0)
IRremote
```

Niet iedere build gebruikt al deze libraries. `SCREEN_OUTPUT_CONFIG`, `ADC_BACKEND` en `INPUT_KANAAL_CONFIG` bepalen welke onderdelen werkelijk worden gecompileerd.

## Configuratie

De configuratie- en taalbestanden zijn:

```text
src/Configuratie/SystemConfig.h
src/Configuratie/UserConfig_template.h
src/Configuratie/Examples.h
src/Configuratie/StimulusConfig.h

src/Language/Examples_XX.h
src/Language/Library_XX.h
src/Language/UserExample_XX_template.h
src/Language/UserLibrary_XX_template.h
```

Kopieer `UserConfig_template.h` vóór gebruik naar `UserConfig.h` wanneer je blijvende persoonlijke instellingen nodig hebt. Zonder `UserConfig.h` gebruikt de library de standaardwaarden uit `SystemConfig.h`. Kopieer voor persoonlijke voorbeeldteksten `UserExample_XX_template.h` naar `UserExample_XX.h` en voor persoonlijke libraryteksten `UserLibrary_XX_template.h` naar `UserLibrary_XX.h`. De actieve gebruikersbestanden blijven naast hun templates staan, worden vóór de overeenkomstige officiële fallbackwaarden geladen en staan in `.gitignore`. `src/Configuratie/Examples.h` blijft de wrapper waarmee de voorbeeldprogramma's de gekozen `UserExample_XX.h` en `Examples_XX.h` laden.

Wanneer je een eigen `UserConfig.h` gebruikt, controleer daarin vóór compilatie minstens:

- `DEBUG`;
- `SCREEN_OUTPUT_CONFIG`;
- `SERIAL_BAUDRATE` en `SERIAL_CONNECT_TIMEOUT_MS` wanneer Serial-uitvoer gebruikt wordt;
- `INPUT_KANAAL_CONFIG`;
- `KEYPAD_TYPE` en, bij een UserDefined-keypad, alle vereiste `KEYPAD_GENERIEK_...`-instellingen;
- `I2C_ADDRESS_PCF8574` wanneer `INPUT_TYPE_PCF8574` gebruikt wordt;
- `I2C_ADRES`;
- `ACTIEF_CHARACTER_SCREEN`;
- `ACTIEF_PIXEL_SCREEN`;
- `PIXEL_SCREEN_CS`, `PIXEL_SCREEN_DC` en `PIXEL_SCREEN_RST`;
- `PIXEL_SCREEN_ROTATION`;
- `AANTAL_SENSOREN_AANWEZIG`;
- `PIN_SENSOR_1` tot en met `PIN_SENSOR_4`;
- `BOARD_VERSION`;
- `ADC_BACKEND` (`ADC_BACKEND_NATIVE` of `ADC_BACKEND_ADS1115`): zie hierboven bij Afhankelijkheid.

Op UNO R3 kan de combinatie van alle functionaliteit en uitgebreide debuguitvoer de beschikbare flash overschrijden. Schakel `DEBUG` uit voor de normale UNO R3-build wanneer nodig.

## Snel starten

Gebruik bij voorkeur de publieke hoofdheader:

```cpp
#include <GroeiAcademie.h>
```

Beschikbare voorbeelden:

```text
examples/
├── Systeem/
│   ├── ADC_Backend/
│   │   ├── ADC_Backend_ADS1115/
│   │   └── ADC_Backend_Native/
│   ├── GedeeldeBus/   # experimenteel, in te schakelen via GEDEELDE_BUS_PROTOTYPE in SystemConfig.h
│   │   ├── Default_CharacterScreen/
│   │   ├── Default_CharacterScreen_PixelScreen/
│   │   ├── Default_PixelScreen/
│   │   ├── InputkanalenDIGITAL/
│   │   ├── InputkanalenHX1838/
│   │   └── InputkanalenPCF8574/
│   ├── Input/
│   │   ├── InputkanalenDIGITAL/
│   │   ├── InputkanalenDIGITALmetArgumenten/
│   │   ├── InputkanalenHX1838/
│   │   ├── InputkanalenHX1838UserDefined/
│   │   ├── InputkanalenHX1838metArgumenten/
│   │   ├── InputkanalenPCF8574/
│   │   ├── InputkanalenPCF8574UserDefinedDirect/
│   │   ├── InputkanalenPCF8574UserDefinedMatrix/
│   │   └── InputkanalenPCF8574metArgumenten/
│   └── Screen/
│       ├── Callback_CharacterScreen/
│       ├── Callback_PixelScreen/
│       ├── Default_CharacterScreen/
│       ├── Default_CharacterScreen_PixelScreen/
│       └── Default_PixelScreen/
└── Toepassingsgebieden/
    └── Stimulus/
        ├── Scenario1_EnkelTik/
        ├── Scenario2_Simultaan/
        ├── Scenario3_Ineenstortend/
        ├── Scenario4_Cocktail/
        └── Tik_Enkele_Samen_Instortend_Cocktail/
```

`ADC_Backend_Native` en `ADC_Backend_ADS1115` zijn bewust zelfstandig gehouden en gebruiken niet de volledige Stimulus-librarylogica. Ze dienen om de Arduino-ADC-route en de ADS1115-route afzonderlijk te valideren, niet als gewone gebruikersvoorbeelden. Zie [Stimulus Shield v1.1.2](docs/Uitbreidingskaarten/Stimulus%20Shield%20v1.1.2/Stimulus-Shield-%28GroeiAcademie-FrameWork%29-v1.1.2.md) en [Handleiding Stimulus Shield v1.1.2](docs/Uitbreidingskaarten/Stimulus%20Shield%20v1.1.2/Handleiding-Stimulus-Shield-%28GroeiAcademie-FrameWork%29-v1.1.2.md). De overige voorbeelden (`Scenario*`, `Tik_Enkele_Samen_Instortend_Cocktail`) volgen wel de normale Stimulus-library-aanpak.

## Librarystructuur

```text
GroeiAcademie/
├── src/
│   ├── GroeiAcademie.h
│   ├── Screen.h
│   ├── Stimulus.h
│   ├── Configuratie/
│   ├── Systeem/Screen/        # Screen.h, Screen.cpp en ScreenTypes.h
│   ├── Systeem/Input/         # Input.h, Input.cpp en InputTypes.h
│   ├── Systeem/GedeeldeBus/   # gedeelde I2C/SPI-businitialisatie (experimenteel, GEDEELDE_BUS_PROTOTYPE)
│   └── Toepassingsgebieden/Stimulus/
├── examples/
├── docs/
├── library.properties
├── keywords.txt
└── README.md
```

Zie [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md) voor verantwoordelijkheden en publieke headers.

### Meerdere ScreenCallbacks

Een CharacterScreen-callback en een PixelScreen-callback mogen tegelijk geregistreerd zijn. Ze kunnen dezelfde informatie synchroon weergeven of elk een ander doel hebben. De library dwingt geen synchronisatie tussen beide callbacks af. Wanneer beide callbacks tegelijk actief zijn, is de gebruiker verantwoordelijk voor de gewenste synchronisatie, de onderlinge timing en het voorkomen dat `delayTime` of `delayTussenPaginas` door beide callbacks wordt uitgevoerd.

## Elektronische schema's

De centrale schema-index staat in [docs/Toepassingsgebieden/MODULES.md](docs/Toepassingsgebieden/MODULES.md). Het actuele Stimulus Shield voor deze release is [Stimulus Shield v1.1.2](docs/Uitbreidingskaarten/Stimulus%20Shield%20v1.1.2/Stimulus-Shield-%28GroeiAcademie-FrameWork%29-v1.1.2.md): actuele v1.1.2-hardware met HX1838 DATA op D8 en PixelScreen RST op D7.

Voor de huidige Stimulusmodule bevat [docs/Toepassingsgebieden/Stimulus/README.md](docs/Toepassingsgebieden/Stimulus/README.md):

- het tekstuele aansluitschema;
- de pinbezetting;
- voeding en massa;
- aandachtspunten voor druksensoren;
- de relatie met `SystemConfig.h`.

De volledige hardwarebasis en de bijbehorende JSON-, PDF-, PNG- en SVG-schema-exporten staan onder Stimulus Shield v1.1.2. [Stimulus Shield v1.1.2](docs/Uitbreidingskaarten/Stimulus%20Shield%20v1.1.2/Stimulus-Shield-%28GroeiAcademie-FrameWork%29-v1.1.2.md) bevat de actuele v1.1.2-hardware inclusief de HX1838 IR Receiver; de [hardwarevalidatie v1.1.2](docs/Uitbreidingskaarten/Stimulus%20Shield%20v1.1.2/Handleiding-Stimulus-Shield-%28GroeiAcademie-FrameWork%29-v1.1.2.md) hoort daarbij.

## Geplande uitbreidingen

Aangevraagde en goedgekeurde uitbreidingen voor volgende releases staan in [de roadmap](docs/ROADMAP.md).

## Documentatie

- [Project Constitution](PROJECT_CONSTITUTION.md)
- [Architectuur](docs/ARCHITECTURE.md)
- [Configuratie](docs/Configuratie/README.md)
- [Screen](docs/Systeem/SCREEN.md)
- [Screen-foutcodes](docs/Systeem/SCREEN_FOUTCODES.md)
- [Toepassingsgebieden en schema-index](docs/Toepassingsgebieden/MODULES.md)
- [Sensorinventaris](docs/Toepassingsgebieden/SENSOR_INVENTARIS.md)
- [Stimulus](docs/Toepassingsgebieden/Stimulus/README.md)
- [Hardwareondersteuning](docs/HARDWARE_SUPPORT.md)
- [Wetenschappelijke integriteit](docs/SCIENTIFIC_INTEGRITY.md)
- [Communitygids](docs/COMMUNITY_GUIDE.md)
- [Terminologie](docs/TERMINOLOGY.md)
- [Beslissingslogboek](docs/DECISION_LOG.md)
- [Roadmap](docs/ROADMAP.md)
- [Bijdragen](CONTRIBUTING.md)
- [Beveiligingsbeleid](SECURITY.md)

## Belangrijke grenzen

- ADC-waarden zijn zonder afzonderlijke kalibratie geen absolute krachtwaarden.
- Fysiologische signalen bewijzen niet automatisch een specifieke emotie of mentale toestand.
- Het framework is geen medisch hulpmiddel.
- Controleer altijd voeding, bedrading en componentdatasheets.

Zie [DISCLAIMER.md](DISCLAIMER.md).
