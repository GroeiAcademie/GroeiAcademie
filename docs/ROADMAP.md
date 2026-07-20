# Roadmap

Deze roadmap beschrijft de ontwikkelrichting. Een item is pas voltooid wanneer code, voorbeelden, documentatie en relevante hardwarecontrole overeenstemmen.

## Voltooid of aantoonbaar uitgevoerd

- Arduino-librarystructuur onder `src/`;
- publieke hoofdheaders;
- huidige Screen-laag;
- huidige Stimulusmodule en vijf voorbeelden;
- compilatie voor UNO R3, UNO R4 Minima en UNO R4 WiFi;
- Arduino Lint zonder fouten en waarschuwingen;
- eerste project-, community- en onderzoeksdocumentatie;
- lokale Git-repository en private GitHub-repository.

## Eerstvolgend

- alle Markdown-documentatie harmoniseren;
- elektronische referentieschema's per geïmplementeerd toepassingsgebied vastleggen;
- README en voorbeelden opnieuw controleren tegen de actuele API;
- dependencywaarschuwing van de externe LCD-library documenteren;
- schone installatie vanuit ZIP controleren;
- repository-inhoud vóór eerste publieke commit en release auditen.

## Releasekwaliteit

- alle voorbeelden automatisch compileren;
- geheugenrapport voor UNO R3 vastleggen;
- echte hardwaretests documenteren;
- interne links en voorbeeldpaden controleren;
- definitieve licentie kiezen;
- release notes maken;
- versie, tag en `library.properties` gelijk houden.

## GitHub

- issue- en pull-requesttemplates;
- GitHub Actions voor ondersteunde boards;
- publieke repository zodra inhoud en licentie daarvoor gereed zijn;
- eerste pre-release of release;
- bijdrageproces testen.

## Arduino Library Manager

- publieke GitHub-repository;
- geldige release-tag;
- definitieve licentie;
- `library.properties` en afhankelijkheden valideren;
- `arduino-lint --library-manager submit` uitvoeren;
- aanmelding volgens de actuele officiële Arduino-procedure.

## Nieuwe toepassingsgebieden

Na stabilisatie van Stimulus:

- ademhaling;
- hartslag, HRV en coherentie;
- biopotentiële signalen;
- emotie-observatie;
- aanvullende observatievaardigheden.

Iedere nieuwe module krijgt vóór officiële ondersteuning:

- afbakening van het meetdoel;
- elektronische documentatie;
- kalibratie;
- voorbeelden;
- foutbronnen en interpretatiegrenzen;
- board- en geheugentests.

## Later

ESP32 en andere platformen worden alleen toegevoegd na een formele compatibiliteits- en veiligheidsbeslissing.
