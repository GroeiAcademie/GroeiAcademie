# ROADMAP

Deze ROADMAP beschrijft de ontwikkelrichting van het GroeiAcademie FrameWork. Een onderdeel is pas voltooid wanneer de code, voorbeelden, documentatie en relevante hardwarecontrole met elkaar overeenstemmen.

## 1. Visie / Doel van het project

Het GroeiAcademie FrameWork vormt één uitbreidbaar framework waarin software, elektronica, hardware, sensormetingen, biofeedback, onderzoek, onderwijs en toepassingen binnen Neuro-Linguïstisch Programmeren samenkomen.

Het project heeft als doel om fysiologische en gedragsmatige reacties zo betrouwbaar mogelijk te meten, te kalibreren, te combineren en begrijpelijk terug te koppelen. Daarbij staat de individuele gebruiker centraal. Het systeem vertrekt niet uitsluitend van gemiddelden, maar leert hoe één persoon reageert, herstelt en evolueert.

De library blijft modulair en uitbreidbaar. Nieuwe toepassingsgebieden worden pas officieel ondersteund wanneer hun meetdoel, code, voorbeelden, elektronische documentatie, kalibratie, foutbronnen, interpretatiegrenzen en hardwarevalidatie voldoende zijn uitgewerkt.

Het project wordt vanaf v1.0.0 publiek ontwikkeld als open-sourceproject. Bijdragen uit code, documentatie, vertaling, elektronica, hardware, testen, onderzoek, meetprotocollen en communitywerk zijn welkom. De maintainers bewaken de architectuur, kwaliteit, licentie, onafhankelijkheid en samenhang van het project.

## 2. Functionele ROADMAP

1. **Verschillende emoties leren kalibreren.**

2. **Meten of het brein die herinnering of beleving oproept overeenkomstig de gevraagde emotie.**

3. **Hartcoherentie.**

4. **Meten of de ademhaling in lijn is met de opgelegde ademhaling.**

5. **Baseline bepalen**

   Iedere meting begint met het bepalen van iemands persoonlijke fysiologische nulmeting.

6. **Stressrespons meten**

   Meten hoe sterk en hoe snel iemand reageert op een stressor of negatieve stimulus.

7. **Herstelvermogen meten**

   Meten hoe snel iemand na stress of een emotionele activatie terugkeert naar zijn persoonlijke baseline.

8. **Effect van interventies objectiveren**

   Meten welk effect bijvoorbeeld NLP-technieken, ademhaling, hartcoherentie, meditatie, ontspanning of andere interventies werkelijk hebben.

9. **Individuele fysiologische profielen opbouwen**

   Leren hoe iemand persoonlijk reageert in plaats van algemene gemiddelden te gebruiken. Dit sluit nauw aan bij het uitgangspunt om niet op "de gemiddelde gebruiker" te vertrouwen.

10. **Sensorfusie**

    Verschillende sensoren combineren zodat conclusies betrouwbaarder worden dan met één enkele sensor.

11. **Betrouwbaarheid van metingen bepalen**

    Automatisch aangeven hoeveel vertrouwen het systeem heeft in een meting, rekening houdend met beweging, slechte sensorcontacten, storingen enzovoort.

12. **Gepersonaliseerde biofeedback**

    Niet alleen meten, maar de feedback automatisch aanpassen aan wat voor die persoon het meest helpt.

13. **Evolutie over langere termijn volgen**

    Objectief opvolgen of iemand over dagen, weken of maanden vooruitgaat.

14. **Rapportering en vergelijking**

    Sessies vergelijken, evolutie tonen en resultaten begrijpelijk visualiseren.

## 3. Technische ROADMAP

### API-stabiliteit vanaf v1.0

Vanaf versie `1.0.0` geldt maximale API-stabiliteit:

- publieke API's worden als stabiel beschouwd;
- nieuwe functionaliteit wordt bij voorkeur toegevoegd zonder bestaande code te breken;
- een vervangen API wordt eerst als deprecated gemarkeerd;
- een deprecated API blijft minimaal één volledige major versie beschikbaar;
- incompatibele wijzigingen en verwijderingen gebeuren uitsluitend in een major release;
- verwijderingen worden vooraf aangekondigd in de changelog en een migratiegids.

Het project volgt Semantic Versioning 2.0.0.

### Platformondersteuning

De actuele compileprofielen omvatten:

- Arduino UNO R3;
- Arduino UNO R4 Minima;
- Arduino UNO R4 WiFi;
- `BOARD_ESP32_UNO`.

Andere ESP32- en niet-Arduino-UNO-platformen worden pas officieel ondersteund nadat pinmapping, ADC-gedrag, timing, geheugen, dependencies en echte hardwarewerking per profiel zijn gevalideerd.

### PixelScreen-uitbreidingen

De veilige tekstruimte en dynamische gridberekening zijn reeds aanwezig. Mogelijke uitbreidingen voor een volgende minor release:

- optioneel zichtbaar kader, afzonderlijk instelbaar van de schermmarges;
- gecontroleerde afkapping of scrolling van tekst die niet binnen het grid past;
- uitgebreidere ondersteuning voor UTF-8 en alternatieve fonts;
- aanvullende standaardweergave voor andere `ScreenData`-typen.

### Nieuwe toepassingsgebieden

Na stabilisatie van Stimulus worden nieuwe technische modules voorzien voor:

- ademhaling;
- hartslag, HRV en coherentie;
- biopotentiële signalen;
- emotie-observatie;
- aanvullende observatievaardigheden.

Zie [Toepassingsgebieden/MODULES.md](Toepassingsgebieden/MODULES.md) voor de volledige, actuele lijst geplande toepassingsgebieden, en [Toepassingsgebieden/SENSOR_INVENTARIS.md](Toepassingsgebieden/SENSOR_INVENTARIS.md) voor de sensoren en modules die daarvoor al ter beschikking zijn.

Iedere nieuwe module krijgt vóór officiële ondersteuning:

- afbakening van het meetdoel;
- elektronische documentatie;
- kalibratie;
- voorbeelden;
- foutbronnen en interpretatiegrenzen;
- board- en geheugentests.

## 4. Hardware ROADMAP

- Native ADC en ADS1115 op de bedoelde hardware valideren;
- Wemos D1 R32 hardwarematig valideren en de status afzonderlijk van compilatieondersteuning documenteren;
- hardwareprofielen per ondersteund moederbord vastleggen;
- pinmapping, werkspanning en logische niveaus per hardwareprofiel valideren;
- sensorcontact, beweging, storing en andere foutbronnen detecteren en rapporteren;
- hardwarevalidatie voor CharacterScreen en PixelScreen behouden;
- hardwarevalidatie uitbreiden wanneer nieuwe sensoren, schermen, keymatrices of moederborden officieel worden toegevoegd;
- configuratie- en validatieresultaten reproduceerbaar documenteren.

## 5. Software ROADMAP

### Reeds voltooid of aantoonbaar uitgevoerd

- Arduino-librarystructuur onder `src/`;
- publieke hoofdheaders;
- centrale optionele gebruikersconfiguratie via `UserConfig.h` met fallback naar `SystemConfig.h`;
- gedeelde voorbeeldconfiguratie via `ExamplesConfig.h`;
- huidige Screen-laag met CharacterScreen, PixelScreen, callbacks en gecombineerde uitvoer;
- PixelScreen-marges, tekstgrootte, tekenafstand, regelafstand en automatisch gecentreerd tekstgrid;
- huidige Stimulusmodule en vijf gewone Stimulusvoorbeelden;
- afzonderlijke Native-ADC- en ADS1115-hardwarevalidatievoorbeelden;
- eerste project-, community-, hardware- en onderzoeksdocumentatie.

### GitHub en community

Vanaf v1.0.0:

- publieke repository zodra inhoud en licentie daarvoor gereed zijn;
- `CONTRIBUTING.md`, `CONTRIBUTORS.md`, `SPONSORS.md` en `TRADEMARKS.md` publiceren;
- `.github/FUNDING.yml` activeren met een werkelijk bereikbaar contact- of financieringskanaal;
- issue- en pull-requesttemplates voorzien;
- bijdrageproces testen;
- enkele duidelijk afgebakende `good first issue`-taken voorzien;
- GitHub Actions voor ondersteunde boards voorzien wanneer deze teststroom definitief is.

### Arduino Library Manager

- publieke GitHub-repository;
- geldige release-tag;
- officiële GNU LGPL v3.0-or-later-licentie;
- `library.properties` en afhankelijkheden valideren;
- `arduino-lint --library-manager submit` uitvoeren;
- aanmelding volgens de actuele officiële Arduino-procedure.

## 6. Validatie & experimenten

Een item is pas afgerond wanneer de relevante code, voorbeelden, documentatie en hardwarecontrole overeenstemmen.

Voor v1.0.0 en volgende releases omvat de validatie waar van toepassing:

- de volledige actuele compilatiematrix uitvoeren na bronaanpassingen die de matrix kunnen beïnvloeden;
- Arduino Lint uitvoeren en het definitieve resultaat vastleggen;
- geheugenrapport voor UNO R3 vastleggen;
- Screen-regressietests uitvoeren voor standaarduitvoer, callbacks, twee en vier regels, `action`, `delayTime` en `delayTussenPaginas`;
- Native ADC en ADS1115 hardwarematig valideren;
- schone installatie vanuit ZIP controleren;
- interne documentatie en voorbeeldpaden controleren;
- testresultaten per board, backend, schermconfiguratie en hardwareprofiel bewaren;
- betrouwbaarheid, foutbronnen en interpretatiegrenzen per meetmodule documenteren;
- experimenten zo opbouwen dat baseline, stimulus, interventie, herstel en vergelijking afzonderlijk beoordeeld kunnen worden.

## 7. Releases / versies / planning

### Nodig vóór definitieve vrijgave van v1.0.0

- de nog lopende actuele testresultaten beoordelen;
- Arduino Lint opnieuw uitvoeren en het definitieve resultaat vastleggen;
- geheugenrapport voor UNO R3 opnieuw vastleggen;
- Native ADC en ADS1115 op de bedoelde hardware valideren;
- Wemos D1 R32 hardwarematig valideren en de status afzonderlijk van compilatieondersteuning documenteren;
- interne documentatie en voorbeeldpaden opnieuw controleren;
- schone installatie vanuit ZIP controleren;
- versie, tag, changelog en `library.properties` gelijk houden;
- `CONTRIBUTORS.md`, `SPONSORS.md`, `TRADEMARKS.md` en de oproepteksten publiceren;
- `.github/FUNDING.yml` activeren;
- release-notities voor v1.0.0 afronden.

### Na v1.0.0

- eerst stabiliseren en regressies oplossen;
- uitbreidingen die geen bestaande API breken in minor releases opnemen;
- nieuwe hardware- en toepassingsgebieden pas als officieel ondersteund aanduiden na hun eigen validatiebeslissing;
- incompatibele wijzigingen uitsluitend in een nieuwe major release uitvoeren.
