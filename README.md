# GroeiAcademie FrameWork

> Waar subjectieve ervaringen en objectief meetbare vaardigheden elkaar ontmoeten.

Het GroeiAcademie FrameWork is een modulaire Arduino-library voor het meten, oefenen en onderzoeken van menselijke vaardigheden. De huidige versie bevat de toepassing **Stimulus** en een gedeelde **Screen**-laag.

> **Meten is Weten, in plaats van Denken te Weten.**

## Huidige status

- versie: `1.0.4`;
- ontwikkelfase: alpha;
- huidige implementatie: de Stimulusmodule en de gedeelde Screen-systeemlaag;
- compilatiematrix uitgevoerd voor Arduino UNO R3, UNO R4 Minima, UNO R4 WiFi en ESP32-borden met boardprofiel `esp32:esp32:d1_uno32`: 188 compilaties uitgevoerd, waarvan 184 succesvol, 4 gekende UNO R3-geheugenbeperkingen en 0 onverwachte compilatiefouten;
- Arduino LINT wordt afzonderlijk geregistreerd in `extras/TESTRESULTATEN.md`;
- licentie: GNU LGPL v3.0-or-later, zie [LICENSE](LICENSE) en [LICENSE.md](LICENSE.md).

De actuele status van de geïmplementeerde en geplande toepassingsgebieden staat in [docs/Toepassingsgebieden/MODULES.md](docs/Toepassingsgebieden/MODULES.md).

## Installatie

### Via ZIP in Arduino IDE

1. Download een release-ZIP van de repository.
2. Open Arduino IDE.
3. Kies **Sketch > Include Library > Add .ZIP Library**.
4. Selecteer de ZIP.
5. Open daarna een voorbeeld via **File > Examples > GroeiAcademie**.

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

Installeer via Arduino Boards Manager `esp32 by Espressif Systems` en selecteer daarna het boardprofiel `WEMOS D1 R32`. De compilatietests gebruiken FQBN `esp32:esp32:d1_uno32`. Dit boardprofiel wordt gebruikt voor WEMOS D1 R32, TTGO D1 R32 en compatibele ESP32-borden in Arduino Uno R3-vormfactor en verzorgt de juiste omzetting van Arduino-pinnamen zoals `A0` naar de overeenkomstige ESP32-GPIO-pinnen. Een toolpakket zoals `esp32:esp-rv32@2601` wordt automatisch met het ESP32-boardpakket geïnstalleerd en wordt niet als Arduino Uno R3-vormfactorbord geselecteerd.

## Kwaliteitscontrole

Elke officiële release van deze library wordt vóór publicatie met de meegeleverde testscripts gevalideerd.

De validatie omvat:

- Arduino LINT (Library Manager)
- compilatie van alle voorbeelden
- Arduino Uno R3
- Arduino Uno R4 Minima
- Arduino Uno R4 WiFi
- ESP32-borden via boardprofiel WEMOS D1 R32 (`esp32:esp32:d1_uno32`)

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
```

Niet iedere build gebruikt al deze libraries. `SCREEN_OUTPUT_CONFIG` en `ADC_BACKEND` bepalen welke onderdelen werkelijk worden gecompileerd.

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
- `I2C_ADRES`;
- `ACTIEF_CHARACTER_SCREEN`;
- `ACTIEF_PIXEL_SCREEN`;
- `PIXEL_SCREEN_CS`, `PIXEL_SCREEN_DC` en `PIXEL_SCREEN_RST`;
- `PIXEL_SCREEN_ROTATION`;
- `AANTAL_SENSOREN_AANWEZIG`;
- `PIN_SENSOR_1` tot en met `PIN_SENSOR_4`;
- `BOARD_VERSION`;
- `ADC_BACKEND` (`ADC_BACKEND_NATIVE` of `ADC_BACKEND_ADS1115`) — zie hierboven bij Afhankelijkheid.

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
│   ├── ADC_Validatie/
│   │   ├── ADC_Validatie_Native/
│   │   └── ADC_Validatie_ADS1115/
│   └── Screen/
│       ├── Callback_CharacterScreen/
│       ├── Callback_PixelScreen/
│       ├── Default_CharacterScreen/
│       ├── Default_PixelScreen/
│       └── Default_CharacterScreen_PixelScreen/
└── Toepassingsgebieden/
    └── Stimulus/
        ├── Scenario1_EnkelTik/
        ├── Scenario2_Simultaan/
        ├── Scenario3_Ineenstortend/
        ├── Scenario4_Cocktail/
        └── Tik_Enkele_Samen_Instortend_Cocktail/
```

`ADC_Validatie_Native` en `ADC_Validatie_ADS1115` zijn bewust zelfstandig gehouden en gebruiken niet de volledige Stimulus-librarylogica. Ze dienen om de Arduino-ADC-route en de ADS1115-route afzonderlijk te valideren, niet als gewone gebruikersvoorbeelden. Zie [GroeiAcademie Stimulus Hardware Shield v1.0.0](docs/Toepassingsgebieden/Stimulus/Hardware/GroeiAcademie-Stimulus-Hardware-Shield-v1.0.0.md) en [Handleiding GroeiAcademie Stimulus Hardware Validatie v1.0.0](docs/Toepassingsgebieden/Stimulus/Hardware/Handleiding-GroeiAcademie-Stimulus-Hardware-Validatie-v1.0.0.md). De overige voorbeelden (`Scenario*`, `Tik_Enkele_Samen_Instortend_Cocktail`) volgen wel de normale Stimulus-library-aanpak.

## Librarystructuur

```text
GroeiAcademie/
├── src/
│   ├── GroeiAcademie.h
│   ├── Screen.h
│   ├── Stimulus.h
│   ├── Configuratie/
│   ├── Systeem/Screen/        # Screen.h, Screen.cpp en ScreenTypes.h
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

De centrale schema-index staat in [docs/Toepassingsgebieden/MODULES.md](docs/Toepassingsgebieden/MODULES.md).

Voor de huidige Stimulusmodule bevat [docs/Toepassingsgebieden/Stimulus/README.md](docs/Toepassingsgebieden/Stimulus/README.md):

- het tekstuele aansluitschema;
- de pinbezetting;
- voeding en massa;
- aandachtspunten voor druksensoren;
- de relatie met `SystemConfig.h`.

De ADS1115- en TFTSPI-hardwarelijn staat onder [docs/Toepassingsgebieden/Stimulus/Hardware/](docs/Toepassingsgebieden/Stimulus/Hardware/), met de actuele JSON-, PDF-, PNG- en SVG-schema-exporten, de [beschrijving van het GroeiAcademie Stimulus Hardware Shield v1.0.0](docs/Toepassingsgebieden/Stimulus/Hardware/GroeiAcademie-Stimulus-Hardware-Shield-v1.0.0.md) en de [handleiding voor de hardwarevalidatie v1.0.0](docs/Toepassingsgebieden/Stimulus/Hardware/Handleiding-GroeiAcademie-Stimulus-Hardware-Validatie-v1.0.0.md).

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
