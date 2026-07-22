# Bijdragen

Bedankt dat je wilt bijdragen aan GroeiAcademie Framework.

## Besluitvorming over bijdragen

1. Iedereen mag bijdragen via GitHub Pull Requests.
2. Iedereen mag ideeën, voorstellen, vragen en bugs melden via GitHub Issues.
3. Grote wijzigingen worden bij voorkeur vooraf besproken voordat er ontwikkelingswerk wordt verricht.
4. Alle bijdragen worden technisch beoordeeld op kwaliteit, consistentie, documentatie, prestaties en compatibiliteit.
5. Niet elke bijdrage wordt automatisch opgenomen. Ook kwalitatief goede bijdragen kunnen worden afgewezen wanneer ze niet aansluiten bij de doelstellingen of de ontwikkeling van het project.
6. De maintainers nemen de uiteindelijke beslissing over het al dan niet opnemen van een bijdrage.
7. Bijdragen moeten aansluiten bij de visie, architectuur, kwaliteitsnormen en langetermijndoelstellingen van GroeiAcademie Framework.

## Auteursrecht en CLA

Er is geen Contributor License Agreement. Iedere contributor behoudt het copyright op zijn of haar eigen bijdragen. GroeiAcademie Framework blijft beheerd door **GROEI ACADEMIE BV**.

## Mogelijke bijdragen

Bijdragen kunnen onder andere bestaan uit:

- code en foutcorrecties;
- Arduino-voorbeelden;
- documentatie en vertalingen;
- elektronische schema's en aansluitdocumentatie;
- meetprotocollen en kalibratieprocedures;
- hardwaretests op ondersteunde boards;
- reproduceerbare foutmeldingen;
- technische en inhoudelijke reviews.

## Eerst een issue openen

Open vóór een grote wijziging eerst een issue. Beschrijf:

- het concrete leer-, meet- of onderzoeksdoel;
- het probleem of de voorgestelde verbetering;
- wat rechtstreeks wordt gemeten en wat wordt afgeleid;
- de benodigde hardware en externe libraries;
- gevolgen voor UNO R3, UNO R4 Minima en UNO R4 WiFi;
- gevolgen voor RAM, flash, timing en publieke API;
- benodigde aanpassingen aan voorbeelden, schema's en documentatie.

## Werkwijze

1. Werk vanuit de meest recente hoofdbranch.
2. Beperk één bijdrage tot één duidelijk onderwerp.
3. Respecteer de bestaande mapstructuur en naamgeving.
4. Voeg of actualiseer voorbeelden wanneer gedrag wijzigt.
5. Actualiseer relevante Markdown-documenten en `CHANGELOG.md`.
6. Controleer interne links.
7. Compileer de betrokken voorbeelden voor alle relevante ondersteunde boards.
8. Voer Arduino Lint uit voordat je een pull request indient.

## Codeprincipes

- voeg geen onnodige afhankelijkheden toe;
- houd ruwe data, berekeningen, patronen en interpretaties gescheiden;
- wijzig de publieke API alleen na bespreking;
- documenteer kalibratie, grenzen en foutbronnen;
- voorkom dynamisch geheugengebruik waar dat op UNO R3 problemen kan geven;
- houd diagnostische uitvoer uitschakelbaar;
- neem geen geheime sleutels, persoonsgegevens of lokale systeembestanden op.

## Elektronische schema's

Een hardwarebijdrage bevat minimaal:

- doel en toepassingsgebied;
- volledig aansluitschema;
- pinbezetting;
- voedingsspanning en gemeenschappelijke massa;
- componentwaarden;
- gebruikte sensorvariant;
- waarschuwingen en bekende grenzen;
- bijbehorende voorbeeldcode.

De tekstuele basisdocumentatie staat in het betrokken document onder `docs/Toepassingsgebieden/`. Bronbestanden en afbeeldingen kunnen later onder een vaste schema-map worden toegevoegd zodra die in de repository bestaat.

## Communitybeginsel

Bijdragen moeten de transparantie, reproduceerbaarheid en bruikbaarheid van het open framework versterken. Zie ook [PROJECT_CONSTITUTION.md](PROJECT_CONSTITUTION.md), [docs/SCIENTIFIC_INTEGRITY.md](docs/SCIENTIFIC_INTEGRITY.md) en [CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md).
