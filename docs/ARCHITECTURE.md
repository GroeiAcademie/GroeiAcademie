# Architectuur

## Eén geïntegreerde library

GroeiAcademie FrameWork wordt als één samenwerkende Arduino-library ontwikkeld. Functionele onderdelen worden niet zonder expliciete reden opgesplitst in afzonderlijke libraries.

## Huidige hoofdstructuur

```text
GroeiAcademie/
├── src/
│   ├── GroeiAcademie.h
│   ├── Screen.h
│   ├── Stimulus.h
│   ├── SystemConfig.h
│   ├── Configuratie/
│   │   ├── Examples.h
│   │   ├── ExamplesConfig.h
│   │   ├── StimulusConfig.h
│   │   ├── SystemConfig.h
│   │   └── UserConfig_template.h
│   ├── Language/
│   │   ├── Examples_XX.h
│   │   ├── Library_XX.h
│   │   ├── UserExample_XX_template.h
│   │   └── UserLibrary_XX_template.h
│   ├── Hulpmiddelen/
│   ├── Sturingen/
│   ├── Systeem/
│   │   └── Screen/
│   │       ├── Screen.h
│   │       ├── Screen.cpp
│   │       └── ScreenTypes.h
│   ├── Toepassingsgebieden/
│   │   └── Stimulus/
│   │       ├── Stimulus.h
│   │       └── Stimulus.cpp
│   └── Uitbreidingskaarten/
├── docs/
│   ├── Configuratie/
│   │   ├── ExamplesConfig.md
│   │   ├── README.md
│   │   ├── StimulusConfig.md
│   │   ├── SystemConfig.md
│   │   ├── UserConfig.md
│   │   └── UserLanguage.md
│   ├── Hulpmiddelen/
│   ├── Sturingen/
│   ├── Systeem/
│   │   ├── PIXELSCREEN_FOUTCODES.md
│   │   └── SCREEN.md
│   ├── Toepassingsgebieden/
│   │   ├── MODULES.md
│   │   ├── SENSOR_INVENTARIS.md
│   │   └── Stimulus/
│   │       ├── Hardware/
│   │       └── README.md
│   ├── Uitbreidingskaarten/
│   ├── ARCHITECTURE.md
│   ├── COMMUNITY_GUIDE.md
│   ├── DECISION_LOG.md
│   ├── DESIGN_PHILOSOPHY.md
│   ├── HARDWARE_SUPPORT.md
│   ├── PROJECT_VALUES.md
│   ├── RESEARCH_PHILOSOPHY.md
│   ├── ROADMAP.md
│   ├── SCIENTIFIC_INTEGRITY.md
│   └── TERMINOLOGY.md
├── examples/
│   ├── Systeem/
│   │   ├── ADC_Validatie/
│   │   │   └── ...
│   │   └── Screen/
│   │       └── ...
│   └── Toepassingsgebieden/
│       └── Stimulus/
│           └── ...
├── extras/
├── .github/
│   ├── FUNDING.yml
│   └── ISSUE_TEMPLATE/
├── keywords.txt
├── library.properties
├── CITATION.cff
├── LICENSE
├── AUTHORS.md
├── CHANGELOG.md
├── CODE_OF_CONDUCT.md
├── CONTRIBUTING.md
├── CONTRIBUTORS.md
├── DISCLAIMER.md
├── GOVERNANCE.md
├── LICENSE.md
├── LICENSE_KEUZEGIDS.md
├── PROJECT_CONSTITUTION.md
├── README.md
├── oproep-sponsors-contributors.md
├── SECURITY.md
├── SPONSORS.md
└── TRADEMARKS.md
```

Niet elke gereserveerde domeinmap bevat al broncode. De huidige implementatie bestaat uit de Screen-laag en de Stimulusmodule.

## Publieke headers

Gebruikers kunnen de volledige library opnemen met:

```cpp
#include <GroeiAcademie.h>
```

Daarnaast worden volgens `library.properties` ook deze publieke headers aangeboden:

```cpp
#include <Screen.h>
#include <Stimulus.h>
```

De headers direct onder `src/` leiden door naar de interne moduleheaders. Interne code vereist geen paden uit voorbeeldprojecten.

## Configuratiemodel

`src/Configuratie/SystemConfig.h` bevat de officiële standaardwaarden en validaties. Voor blijvende persoonlijke instellingen kopieert de gebruiker `UserConfig_template.h` vóór gebruik naar `UserConfig.h` in dezelfde map. `UserConfig.h` is optioneel.

De centrale laadvolgorde is:

```text
vaste keuzewaarden
→ eventueel UserConfig.h
→ fallbackwaarden en validaties uit SystemConfig.h
```

Globale compilerdefinities kunnen waarden vóór deze laadvolgorde vastleggen. Gewone examples stellen libraryconfiguratie niet rechtstreeks in de `.ino` in, omdat `Screen.cpp` en `Stimulus.cpp` afzonderlijk van de sketch worden gecompileerd.

`ExamplesConfig.h` bevat uitsluitend gedeelde instellingen van de voorbeeldprogramma's. `StimulusConfig.h` bevat de vaste en configureerbare grenzen van de Stimulusmodule.

## Verantwoordelijkheden

### Configuratie

`src/Configuratie/` bevat boardkeuze, ADC-backend, sensorpinnen, schermconfiguratie, voorbeeldinstellingen en Stimulusgrenzen.

### Systeem

`src/Systeem/` bevat frameworkbrede voorzieningen. De huidige systeemmodule is `Screen`.

### Toepassingsgebieden

`src/Toepassingsgebieden/` bevat modules rond een concreet leer-, meet- of onderzoeksdoel. De huidige module is `Stimulus`.

### Gereserveerde domeinen

`Hulpmiddelen`, `Sturingen` en `Uitbreidingskaarten` zijn gereserveerde functionele domeinen. Lege mappen vormen geen ondersteunde functionaliteit.

## Afhankelijkheden

De in `library.properties` gedeclareerde dependencies zijn:

- LiquidCrystal I2C;
- Adafruit GFX Library;
- Adafruit ST7735 and ST7789 Library;
- Adafruit ADS1X15.

Niet iedere build gebruikt alle code uit deze libraries. `SCREEN_OUTPUT_CONFIG` en `ADC_BACKEND` bepalen welke GroeiAcademie-broncode werkelijk wordt gecompileerd. De dependencies blijven gedeclareerd zodat de meegeleverde CharacterScreen-, PixelScreen- en ADS1115-voorbeelden na installatie beschikbaar zijn.

## Samenwerking tussen modules

Modules communiceren via expliciete headers, functies en datastructuren. Rechtstreekse afhankelijkheden worden beperkt. Screen is een gedeelde uitvoerlaag die door toepassingsmodules kan worden gebruikt.

## Elektronische documentatie

Elektronische schema's worden per toepassingsgebied beschreven. De index staat in [Toepassingsgebieden/MODULES.md](Toepassingsgebieden/MODULES.md). De beschikbare sensoren en modules staan afzonderlijk in [Toepassingsgebieden/SENSOR_INVENTARIS.md](Toepassingsgebieden/SENSOR_INVENTARIS.md).

Een geldig schema vermeldt:

- board en boardvariant;
- pinbezetting;
- voeding en gemeenschappelijke massa;
- componentwaarden;
- sensorvariant;
- externe modules en adressen;
- kalibratievoorwaarden;
- waarschuwingen en beperkingen.

## Geheugen en timing

UNO R3 blijft een belangrijke ondergrens. Nieuwe code wordt beoordeeld op flash, SRAM, blokkerende wachttijden en samplegedrag. Debugfunctionaliteit moet uitschakelbaar blijven.
