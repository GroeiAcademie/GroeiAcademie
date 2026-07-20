# GroeiAcademie Framework

> Waar subjectieve ervaringen en objectief meetbare vaardigheden elkaar ontmoeten, dankzij een modulair open-sourceframework waarmee menselijke vaardigheden objectief gemeten, geoefend, ontwikkeld en onderzocht kunnen worden.

Het GroeiAcademie Framework is één modulair open-sourceframework waarmee menselijke vaardigheden objectief gemeten, geoefend, ontwikkeld en onderzocht kunnen worden.

## Missie

Het GroeiAcademie Framework bestaat enerzijds om beoefenaars van Neuro Linguïstisch Programmeren te helpen hun basisvaardigheden veel sneller naar een hoger niveau te brengen. Anderzijds maakt het framework het mogelijk om de impact van NLP-technieken op het brein en op waarneembare fysiologische processen te onderzoeken en zichtbaar te maken voor onderzoekers, biofeedback-onderzoekers, NLP-beoefenaars, NLP-trainers en leken.

> **Meten is weten, in plaats van denken te weten.**

## Eerste modules

### Stimulus

Meten van vingertikken om een unieke stimulus reproduceerbaar te leren zetten en activeren met dezelfde duur, kracht en timing. De eerste versie ondersteunt één tot vier vingers, vier basisscenario's en beoordeling van het juiste activeringsmoment.

### Emotie-observatie

Leren observeren wanneer iemand kenmerken van een emotie vertoont en de eigen observatie vergelijken met objectieve meetgegevens. Gemeten signalen en interpretaties blijven duidelijk gescheiden.

### Ademhaling en hartslag

Ademhaling en hartslag volgen zodat een begeleider kan nagaan of een observatie, zoals trager inademen, overeenkomt met de werkelijkheid. Dit principe wordt later uitgebreid naar andere observatievaardigheden.

## Doelgroepen

Onderzoekers, biofeedback-onderzoekers, NLP-beoefenaars, NLP-trainers en leken.

## Ondersteuning

De eerste officiële omgeving is Arduino IDE. De eerste officieel ondersteunde boards zijn Arduino UNO R3 en Arduino UNO R4.

## Architectuur

Het project blijft één geïntegreerde library met samenwerkende onderdelen binnen de bestaande hoofdindeling:

```text
lib/
├── GroeiAcademie.h
├── Hulpmiddelen/
├── Sturingen/
├── Systeem/
├── Toepassingsgebieden/
└── Uitbreidingskaarten/
```

De eerste functionele module bevindt zich onder `Toepassingsgebieden/Stimulus`.

## Open source

Iedereen mag het framework gebruiken. Verbeteringen aan het framework horen terug te vloeien naar de community. De definitieve licentie wordt gekozen op basis van dit wederkerigheidsprincipe.

## Documentatie

Begin bij:

1. `PROJECT_CONSTITUTION.md`
2. `SCIENTIFIC_INTEGRITY.md`
3. `DESIGN_PHILOSOPHY.md`
4. `ARCHITECTURE.md`
5. `ROADMAP.md`

## Status

Het project bevindt zich vóór versie 1.0. API, mapstructuur en meetprotocollen worden vóór de eerste publieke release gecontroleerd en bevroren.
