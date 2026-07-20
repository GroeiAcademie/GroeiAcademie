# Architectuur

## Eén geïntegreerde library

GroeiAcademie Framework wordt als één samenwerkende Arduino-library ontwikkeld. Functionele onderdelen worden niet zonder expliciete reden opgesplitst in afzonderlijke libraries.

## Bestaande hoofdstructuur

```text
GroeiAcademie/
├── src/
│   ├── GroeiAcademie.h
│   ├── Configuratie/
│   │   ├── StimulusConfig.h
│   │   └── SystemConfig.h
│   ├── Hulpmiddelen/
│   ├── Sturingen/
│   ├── Systeem/
│   │   ├── Screen/
│   │   │   ├── Screen.h
│   │   │   └── Screen.cpp
│   │   └── ...
│   ├── Toepassingsgebieden/
│   │   ├── Stimulus/
│   │   │   ├── Stimulus.h
│   │   │   └── Stimulus.cpp
│   │   └── ...
│   └── Uitbreidingskaarten/
├── docs/
│   ├── Configuratie/
│   ├── Hulpmiddelen/
│   ├── Sturingen/
│   ├── Systeem/
│   │   └── SCREEN.md
│   ├── Toepassingsgebieden/
│   │   ├── Stimulus/
│   │   │   ├── Hardware/
│   │   │   └── README.md
│   │   └── ...
│   ├── Uitbreidingskaarten/
│   ├── ARCHITECTURE.md
│   ├── COMMUNITY_GUIDE.md
│   ├── DECISION_LOG.md
│   ├── DESIGN_PHILOSOPHY.md
│   ├── GOVERNANCE.md
│   ├── HARDWARE_SUPPORT.md
│   ├── PROJECT_VALUES.md
│   ├── RESEARCH_PHILOSPHY.md
│   ├── ROADMAP.md
│   ├── SCIENTIC_INTEGRITY.md
│   └── TERMINOLOGY.md
├── examples/
│   ├── Screen/
│   │   └── ...
│   ├── Stimulus/
│   │   └── ...
├── extras/
├── keywords.txt
├── library.properties
├── AUTHORS.md
├── CHANGELOG.md
├── CODE_OF_CONDUCT.md
├── CONTRIBUTING.md
├── DISCLAIMER.md
├── keywords.txt
├── library.proporties
├── LICENSE.md
├── LICENSE_KEUZEGIDS.md
├── PROJECT_CONSTITUTION.md
├── README.md
└── SECURITY.md

```

Niet elke gereserveerde map bevat al broncode. De huidige implementatie bestaat uit de Screen-laag en de Stimulusmodule.

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

De headers direct onder `src/` leiden door naar de interne moduleheaders. Interne code hoort geen paden uit de voorbeeldprojecten te vereisen.

## Verantwoordelijkheden

### Configuratie

`src/Configuratie/` bevat compile-time-instellingen zoals boardkeuze, ADC-resolutie, sensorpinnen, schermconfiguratie en meetgrenzen.

### Systeem

`src/Systeem/` bevat frameworkbrede voorzieningen. De huidige systeemmodule is `Screen`.

### Toepassingsgebieden

`src/Toepassingsgebieden/` bevat modules rond een concreet leer-, meet- of onderzoeksdoel. De huidige module is `Stimulus`.

### Gereserveerde domeinen

`Hulpmiddelen`, `Sturingen` en `Uitbreidingskaarten` zijn gereserveerde functionele domeinen. Lege mappen vormen geen ondersteunde functionaliteit.

## Afhankelijkheden

Externe libraries worden uitsluitend toegevoegd wanneer zij functioneel nodig zijn en in `library.properties` worden gedeclareerd. De huidige afhankelijkheid is `LiquidCrystal I2C`.

## Samenwerking tussen modules

Modules communiceren via expliciete headers, functies en datastructuren. Rechtstreekse afhankelijkheden worden beperkt. Screen is een gedeelde uitvoerlaag die door toepassingsmodules kan worden gebruikt.

## Configuratie en draagbaarheid

De huidige configuratie gebruikt preprocessorinstellingen in de library zelf. Dat is bruikbaar voor de pre-1.0-fase, maar wijzigingen door de gebruiker kunnen bij een library-update worden overschreven. Een stabieler configuratiemodel blijft een architectuurpunt voor vóór versie 1.0.

## Elektronische documentatie

Elektronische schema's worden per toepassingsgebied beschreven, niet in de softwarearchitectuur verborgen. De index staat in [Toepassingsgebieden/MODULES.md](Toepassingsgebieden/MODULES.md).

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
