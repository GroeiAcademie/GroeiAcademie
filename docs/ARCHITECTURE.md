# Architectuur

## Eén geïntegreerde library

GroeiAcademie Framework wordt als één samenwerkende Arduino-library ontwikkeld. Functionele onderdelen worden niet zonder expliciete reden opgesplitst in afzonderlijke libraries.

## Bestaande hoofdstructuur

```text
GroeiAcademie/
├── lib/
│   ├── GroeiAcademie.h
│   ├── Hulpmiddelen/
│   ├── Sturingen/
│   ├── Systeem/
│   ├── Toepassingsgebieden/
│   └── Uitbreidingskaarten/
├── user_config/
├── docs/
├── examples/
├── keywords.txt
├── library.properties
└── README.md
```

De precieze omzetting naar een Arduino Library Manager-compatibele distributiestructuur wordt vóór publicatie technisch gevalideerd. De functionele indeling blijft behouden, ook wanneer bestanden uiteindelijk onder `src/` moeten worden geplaatst.

## Verantwoordelijkheden

### Hulpmiddelen

Herbruikbare technische hulpmiddelen zoals joystick- of servomotorondersteuning.

### Sturingen

Besturing van samengestelde systemen zoals voertuigen, drones of robotarmen.

### Systeem

Frameworkbrede voorzieningen zoals schermuitvoer, timing, configuratie en logging.

### Toepassingsgebieden

Modules die rechtstreeks een menselijke vaardigheid, meting of onderzoeksvraag ondersteunen, waaronder Stimulus, Ademhaling, Biopotentiële Signalen en Hartritme, Coherentie en Hartslag.

### Uitbreidingskaarten

Ondersteuning voor concrete uitbreidingshardware die meerdere onderdelen kan combineren.

## Configuratie

Gebruikersconfiguratie blijft gescheiden van implementatiecode. Configuratiebestanden mogen geen verborgen afhankelijkheden creëren en worden gecontroleerd op geldige combinaties.

## Samenwerking tussen modules

Modules delen gegevens via expliciete interfaces. Rechtstreekse afhankelijkheden worden beperkt om testbaarheid en vervangbaarheid te behouden.

## Publieke API

Publieke headers bevatten alleen wat gebruikers nodig hebben. Interne functies, globale variabelen en implementatiedetails blijven intern. API-wijzigingen worden vóór versie 1.0 geregistreerd en na versie 1.0 volgens semantisch versiebeheer behandeld.
