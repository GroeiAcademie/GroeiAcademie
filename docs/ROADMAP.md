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
- Arduino Uno R3-vormfactorbord- en geheugentests.

## 4. Hardware ROADMAP

- Native ADC en ADS1115 op de bedoelde hardware valideren;
- WEMOS D1 R32 is sinds v1.0.0 hardwarematig getest en goedgekeurd; fysieke validatie van TTGO D1 R32 en andere compatibele borden afzonderlijk vastleggen;
- hardwareprofielen per ondersteund Arduino Uno R3-vormfactorbord vastleggen;
- pinmapping, werkspanning en logische niveaus per hardwareprofiel valideren;
- sensorcontact, beweging, storing en andere foutbronnen detecteren en rapporteren;
- hardwarevalidatie voor CharacterScreen en PixelScreen behouden;
- hardwarevalidatie uitbreiden wanneer nieuwe sensoren, schermen, keymatrices of Arduino Uno R3-vormfactorborden officieel worden toegevoegd;
- configuratie- en validatieresultaten reproduceerbaar documenteren.

### Arduino Uno R3-vormfactorborden

Status volgens de [statuslegende in MODULES.md](Toepassingsgebieden/MODULES.md#statuslegende).

| Arduino Uno R3-vormfactorbord | Status |
|---|---|
| Arduino UNO R3 | geïmplementeerd |
| Arduino UNO R4 Minima | geïmplementeerd |
| Arduino UNO R4 WiFi | geïmplementeerd |
| Arduino ESP32-S3 38pin V4 Expension Board (ESP32-WROOM-32U) | kandidaat-idee |
| Cytron Maker Uno RP2040 | kandidaat-idee |
| SB Components Ardi32 | kandidaat-idee |
| STM32 Nucleo-F401RE | kandidaat-idee |
| TTGO D1 R32 | geïmplementeerd |
| WEMOS D1 R32 | geïmplementeerd |

Zie ook [SPONSORS.md](../SPONSORS.md) voor de Arduino Uno R3-vormfactorborden die momenteel als kandidaat open staan voor sponsoring.

### Schermen

Status volgens dezelfde [statuslegende in MODULES.md](Toepassingsgebieden/MODULES.md#statuslegende).

Selectiecriterium: parallelle (8-bits) interfaces worden uitgesloten wegens te veel benodigde pinnen voor gelijktijdig gebruik met een meetopstelling. SPI en I2C zijn beide toegestane alternatieven, geen van beide wordt op voorhand uitgesloten.

| Scherm | Interface | Status |
|---|---|---|
| 2.4 inch TFT 240x320 LCD-scherm ST7789V v1.3 | op zoek naar SPI/I2C-alternatief (huidig exemplaar: parallel) | ter-discussie |
| 2.8 inch TFT 240x320 LCD-scherm ST7789V v1.1 | op zoek naar SPI/I2C-alternatief (huidig exemplaar: parallel) | ter-discussie |
| 3.5 inch TFT 480x320 LCD-scherm ILI9486 met touchpen en SD Card Socket for Arduino Board Module - TFT LCD SHIELD | op zoek naar SPI/I2C-alternatief (huidig exemplaar: parallel) | ter-discussie |

De drie momenteel beschikbare exemplaren zijn alle drie parallelle uitvoeringen en voldoen op dit moment niet aan het selectiecriterium. De zoektocht gaat dus niet over deze exemplaren zelf, maar over het vinden van SPI- of I2C-varianten van hetzelfde schermtype, of een alternatief scherm dat wél SPI of I2C gebruikt.

I2C als alternatieve, nog te testen interface voor PixelScreen (naast SPI), bijvoorbeeld een SSD1306 OLED-scherm. Kan dezelfde I2C-bus delen met het characterscherm, mits een verschillend adres.

### Input-apparaten

Deze reeks wordt getest en geëvalueerd. Er bestaat vandaag geen generieke input-laag in de library; deze lijst maakt zichtbaar waarmee gewerkt wordt, zonder dat al beslist is wat, of in welke vorm, dit als gedeelde functionaliteit wordt uitgewerkt.

| Apparaat | Status |
|---|---|
| JoyStick Shield Funduino v1.A | ter-discussie |
| PS2 JoyStick Shield Funduino V3_200907.S | ter-discussie |
| meArm.WDT - XY-joystickmodule met dubbele as | ter-discussie |
| 1x4 button TTP224 | ter-discussie |
| 4x4 button TTP229 / HW136 | ter-discussie |
| 4x1 buttons (enkel toetsen, GND, K1-K4) | ter-discussie |
| 4x2 buttons (enkel toetsen, GND, K1-K8) | ter-discussie |
| 4x4 buttons (enkel toetsen, C1-C4 en R1-R4) | ter-discussie |
| 4x3 Membraan Schakelaar Toetsenbord (flat 7) | ter-discussie |
| 4x5 Membraan Schakelaar Toetsenbord (flat 9) | ter-discussie |
| HW-139 (touch sensors) | ter-discussie |
| IR HX1838 Infrarood Afstandsbediening, IR ontvanger voor 38kHz IR signalen | ter-discussie |

### Bouwstenen

Generieke elektronica-bouwstenen, niet gebonden aan één specifiek toepassingsgebied. Kunnen zowel de input-laag als toekomstige sensor- of schermuitbreidingen ondersteunen.

| Bouwsteen | Status |
|---|---|
| PCF8574 Remote 8-Bit I/O Expander for I2C Bus | ter-discussie |
| AT41 (quad logic level shifter) | ter-discussie |
| CD74HC4067 (16 kanaals multiplexor) | ter-discussie |

### Sensoren

Status volgens dezelfde [statuslegende in MODULES.md](Toepassingsgebieden/MODULES.md#statuslegende).

| Sensor | Status |
|---|---|
| AD8232 | ter-beoordeling |
| ADXL345 | ter-beoordeling |
| BioAmp EXG Pill | ter-beoordeling |
| BME688 | ter-beoordeling |
| DS18B20 | ter-beoordeling |
| ECG EMG Shield DuinoPeak | ter-beoordeling |
| Grove GSR | ter-beoordeling |
| INMP441 | ter-beoordeling |
| MAX30102 | ter-beoordeling |
| MCU-6701 | ter-beoordeling |
| MR60BHA2 | ter-beoordeling |
| MPU6050 | ter-beoordeling |
| MPU6500 | ter-beoordeling |
| NeuroSky TGAM | ter-beoordeling |
| Piëzo-ademhalingsband (×2, borst + buik) | ter-beoordeling |
| Pulse Sensor | ter-beoordeling |
| Seed Studio 60 GHz mmWave Sensor | ter-beoordeling |
| SCD41 | ter-beoordeling |
| SDP810-125PA | ter-beoordeling |
| VL6180X | ter-beoordeling |
| VL53L0X | kandidaat-idee |

Referentie-/validatie-apparaten (geen kandidaat-sensoren, dienen om te controleren of onze eigen hardware dezelfde signaalpatronen kan tonen als een gevestigd consumentenapparaat): NeuroSky MindWave Mobile 2, Mindfield eSense, MindFlex.

Naast deze lijst bevinden zich in [SENSOR_INVENTARIS.md](Toepassingsgebieden/SENSOR_INVENTARIS.md) nog andere, alternatieve sensoren die nog verder onderzocht moeten worden voordat hun status bepaald kan worden.

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

Reeds voltooid:

- publieke repository;
- `CONTRIBUTING.md`, `CONTRIBUTORS.md`, `SPONSORS.md` en `TRADEMARKS.md` gepubliceerd;
- `.github/FUNDING.yml` opgenomen;
- issue- en pull-requesttemplates opgenomen.

Nog gepland:

- bijdrageproces in de praktijk verder testen;
- enkele duidelijk afgebakende `good first issue`-taken voorzien;
- GitHub Actions voor ondersteunde Arduino Uno R3-vormfactorborden voorzien wanneer deze teststroom definitief is.

### Arduino Library Manager

Voltooid:

- publieke GitHub-repository beschikbaar;
- geldige release-tag gepubliceerd;
- officiële GNU LGPL v3.0-or-later-licentie opgenomen;
- `library.properties` en afhankelijkheden gevalideerd;
- `arduino-lint --library-manager submit` uitgevoerd;
- aanmelding geaccepteerd en opgenomen in de Arduino Library Manager-index.

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
- testresultaten per Arduino Uno R3-vormfactorbord, backend, schermconfiguratie en hardwareprofiel bewaren;
- betrouwbaarheid, foutbronnen en interpretatiegrenzen per meetmodule documenteren;
- experimenten zo opbouwen dat baseline, stimulus, interventie, herstel en vergelijking afzonderlijk beoordeeld kunnen worden.

## 7. Releases / versies / planning

### v1.0.0 (voltooid)

- eerste publieke alpha-release vrijgegeven;
- definitieve compilatiematrix en Arduino LINT uitgevoerd;
- WEMOS D1 R32 getest en goedgekeurd;
- versie, tag, changelog en `library.properties` gelijkgetrokken;
- project-, community-, hardware- en release-documentatie gepubliceerd;
- Arduino Library Manager-aanmelding ingediend en nadien geaccepteerd.

### Na v1.0.0

- eerst stabiliseren en regressies oplossen;
- uitbreidingen die geen bestaande API breken in minor releases opnemen;
- nieuwe hardware- en toepassingsgebieden pas als officieel ondersteund aanduiden na hun eigen validatiebeslissing;
- incompatibele wijzigingen uitsluitend in een nieuwe major release uitvoeren.

### Aantekening: generiek opt-in-mechanisme voor experimentele functies

Bij `CHARACTERSCREEN_I2C_ADRES_MODUS` (zie `docs/DECISION_LOG.md`, D023) werd een generieke
`BETATESTER`-vlag overwogen om functies die nog niet in alpha zitten toch al testbaar te
maken. Bewust niet gebouwd: voor precies één concreet experiment een brede infrastructuur
optuigen is voorbarig, en het zou een tweede, parallel classificatiesysteem naast de
bestaande ondersteuningsniveaus (D018) creëren. Wanneer een tweede, onafhankelijk
experiment opduikt dat hetzelfde soort opt-in nodig heeft, is dit het moment om een
generiek mechanisme alsnog te overwegen, met dan twee concrete gevallen om de opzet aan
te toetsen in plaats van één.
