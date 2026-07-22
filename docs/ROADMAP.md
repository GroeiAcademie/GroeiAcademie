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
- officiële GNU LGPL v3.0-or-later-licentie opnemen;
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
- officiële GNU LGPL v3.0-or-later-licentie;
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


## API-stabiliteit vanaf v1.0

De versies binnen `0.x` blijven ontwikkelversies waarin de publieke API nog kan wijzigen.

Vanaf versie `1.0.0` geldt maximale API-stabiliteit:

- publieke API's worden als stabiel beschouwd;
- nieuwe functionaliteit wordt bij voorkeur toegevoegd zonder bestaande code te breken;
- een vervangen API wordt eerst als deprecated gemarkeerd;
- een deprecated API blijft minimaal één volledige major versie beschikbaar;
- incompatibele wijzigingen en verwijderingen gebeuren uitsluitend in een major release;
- verwijderingen worden vooraf aangekondigd in de changelog en een migratiegids.

Het project volgt Semantic Versioning 2.0.0.
