# Toepassingsgebieden en schema-index

Dit document onderscheidt de huidige implementatie van geplande toepassingsgebieden en vormt de centrale index voor elektronische schema's.

Zie [SENSOR_INVENTARIS.md](SENSOR_INVENTARIS.md) voor de sensoren en modules die al ter beschikking zijn. Die inventaris is geen gevalideerd schema en dient enkel om te tonen welke hardware al aanwezig is om onderstaande toepassingsgebieden te verwezenlijken.

## Statuslegende

Een toepassingsgebied doorloopt de volgende statussen, van eerste denkspoor tot uitvoering:

1. **kandidaat-idee**, een richting die nog niet besproken is;
2. **ter-discussie**, wordt inhoudelijk besproken;
3. **ter-beoordeling**, een concreet voorstel ligt er en wordt beoordeeld;
4. **goedgekeurd**, aanvaard als richting, nog niet op de agenda van een release;
5. **gepland**, staat effectief op de agenda, zie [ROADMAP](../ROADMAP.md);
6. **geïmplementeerd**.

Zie [Governance](../../GOVERNANCE.md) voor hoe een nieuw voorstel deze statussen doorloopt.

## Statusoverzicht

| Toepassingsgebied | Softwarestatus | Elektronische documentatie |
|---|---|---|
| Stimulus en conditionering | geïmplementeerd | [Stimulus/README.md](Stimulus/README.md) + [Stimulus/Hardware/GroeiAcademie-Stimulus-Hardware-Shield-v1.0.0.md](Stimulus/Hardware/GroeiAcademie-Stimulus-Hardware-Shield-v1.0.0.md) |
| Screen | gedeelde systeemlaag | [../Systeem/SCREEN.md](../Systeem/SCREEN.md) |
| Ademhaling | kandidaat-idee | nog geen gevalideerd schema |
| Hartslag, HRV en hartcoherentie | kandidaat-idee | nog geen gevalideerd schema |
| Bio-elektrische en biopotentiële signalen | kandidaat-idee | nog geen gevalideerd schema |
| Hart en autonome reacties | kandidaat-idee | nog geen gevalideerd schema |
| Huidgeleiding en autonome activatie | kandidaat-idee | nog geen gevalideerd schema |
| Huidtemperatuur en omgevingsmetingen | kandidaat-idee | nog geen gevalideerd schema |
| Druk en beweging | kandidaat-idee | nog geen gevalideerd schema |
| Druk, kracht en aanraking | kandidaat-idee | nog geen gevalideerd schema |
| Beweging, houding en lichaamsbeweging | kandidaat-idee | nog geen gevalideerd schema |
| Aanraking, capacitieve bediening en gebruikersinput | kandidaat-idee | nog geen gevalideerd schema |
| Aanwezigheid, afstand en locatie | kandidaat-idee | nog geen gevalideerd schema |
| Stem en ademgeluid | kandidaat-idee | nog geen gevalideerd schema |
| Visuele observatie en pupilgedrag | kandidaat-idee | nog geen gevalideerd schema |
| Licht en optische metingen | kandidaat-idee | nog geen gevalideerd schema |
| Omgevingsfactoren | kandidaat-idee | nog geen gevalideerd schema |
| Sensorfusie en betrouwbaarheid van metingen | methodologisch, geen sensor op zich | nog geen gevalideerd schema |
| Emotie-observatie | goedgekeurd | afhankelijk van de uiteindelijk gekozen meetmodules |
| Overige observatievaardigheden | ter-discussie | nog niet bepaald |

Een gepland toepassingsgebied mag niet als werkende libraryfunctionaliteit worden voorgesteld.

## 1. Stimulus en conditionering

### Leerdoel

Een stimulus met vingertikken reproduceerbaar leren zetten en activeren.

### Huidige meetgegevens

- één tot vier analoge sensoren;
- duur per stimulus;
- gemiddelde drukwaarde;
- piekdrukwaarde in uitgebreide modus;
- tijd tot piek;
- opbouw- en afbouwsnelheid;
- start- en eindsynchronisatie;
- afwijking tegenover een nulmeting of doel.

### Geplande uitbreiding

- één unieke stimulus conditioneren;
- twee unieke stimuli simultaan conditioneren;
- een ineenstortend anker;
- een cocktail van ankers.

### Scenario's

De repository bevat afzonderlijke voorbeelden voor:

1. enkele tik;
2. simultane tik;
3. ineenstortend patroon;
4. cocktail;
5. een samengesteld demonstratievoorbeeld.

Zie [Stimulus/README.md](Stimulus/README.md) voor aansluiting, configuratie en interpretatiegrenzen. Zie [Stimulus/Hardware/GroeiAcademie-Stimulus-Hardware-Shield-v1.0.0.md](Stimulus/Hardware/GroeiAcademie-Stimulus-Hardware-Shield-v1.0.0.md) voor de actuele ADS1115- en TFTSPI-hardwarelijn met H5, H6, H7 en H8. Het validatieprotocol staat in [Stimulus/Hardware/Handleiding-GroeiAcademie-Stimulus-Hardware-Validatie-v1.0.0.md](Stimulus/Hardware/Handleiding-GroeiAcademie-Stimulus-Hardware-Validatie-v1.0.0.md).

## 2. Ademhaling

Geplande meetdoelen zijn onder meer ademfrequentie, ritme, diepte, ademfase en waar mogelijk mond- of neusademhaling. De definitieve sensoren en elektronische schema's zijn nog niet vastgesteld. Een schema wordt pas opgenomen nadat de opstelling praktisch en inhoudelijk is gevalideerd.

Geplande meetdoelen, in detail:

- ademhalingsfrequentie;
- ademhalingsritme;
- ademhalingsdiepte;
- inademing en uitademing;
- borst- en buikbeweging;
- luchtstroom.

## 3. Hartslag, HRV en hartcoherentie

Geplande meetdoelen zijn hartslag en, mits een geldige meet- en analysemethode, HRV en coherentie. Sensor, samplekwaliteit, artefactdetectie en algoritmen moeten vóór implementatie expliciet worden gekozen en beschreven.

## 4. Hart en autonome reacties

Overkoepelend toepassingsgebied voor het autonome zenuwstelsel als geheel: hartslag, hartritmevariabiliteit, hartcoherentie, huidgeleiding en huidtemperatuur samen. Doel is te kunnen tonen of iemand fysiologisch écht ontspant tijdens een oefening, in plaats van louter te melden ontspannen te zijn.

- hartslag;
- hartritmevariabiliteit;
- hartcoherentie;
- huidgeleiding;
- huidtemperatuur: koude handen bij stress, warmere handen bij ontspanning (perifere meting).

## 5. Bio-elektrische en biopotentiële signalen

Dit toepassingsgebied is gereserveerd voor signalen zoals ECG, EMG, EEG en EOG. Vanwege veiligheid, signaalkwaliteit en interpretatierisico's is nog geen schema als officieel referentieontwerp opgenomen.

- EEG: elektrische activiteit van de hersenen, koppeling aan trance- en ontspanningsstaten;
- EMG: spieractiviteit;
- ECG: elektrische activiteit van het hart;
- EOG: oogbewegingen via elektrische signalen rond de ogen; NLP's eigen oogtoegangssignalen (visueel, auditief, kinesthetisch representeren via oogrichting).

Belangrijk onderscheid: alleen EEG meet rechtstreeks elektrische hersenactiviteit. EMG, ECG en EOG meten respectievelijk spieren, hart en oogbewegingen.

## 6. Huidgeleiding en autonome activatie

Geplande meetdoelen zijn elektrodermale activiteit, zweetklieractiviteit en veranderingen in sympathische activatie, als kern van anchoring.

## 7. Huidtemperatuur en omgevingsmetingen

Geplande meetdoelen zijn huidtemperatuur, met een expliciet onderscheid tussen sensoren geschikt voor direct huidcontact en sensoren die vooral de omgevingstemperatuur meten. Zie [SENSOR_INVENTARIS.md](SENSOR_INVENTARIS.md) voor dat onderscheid per sensor.

## 8. Druk en beweging

- druksensoren;
- krachtverdeling;
- gewichtsverdeling;
- aanraking;
- beweging en houding;
- druksensoren: ankersterkte meten via onbewuste grijpdruk.

## 9. Druk, kracht en aanraking

Geplande meetdoelen zijn druk, kracht, grijpkracht, voetdruk, aanraking, houding en verandering in spierspanning via uitgeoefende kracht.

## 10. Beweging, houding en lichaamsbeweging

Geplande meetdoelen zijn borst- en buikbeweging, lichaamshouding, beweging, tremor, reactietijd en bewegingsartefacten, onder meer als objectieve maatstaf voor fysiologische congruentie tijdens een anker.

## 11. Aanraking, capacitieve bediening en gebruikersinput

Geen fysiologische meting, maar een alternatieve inputmethode naast het bestaande `PIN_TOETS`-systeem: stimuli, gebruikersinput, keuzes, bevestigingen en het registreren van aanraking.

## 12. Aanwezigheid, afstand en locatie

Sessiebeheer: aanwezigheid, afstand, beweging, lichaamshouding en locatie in een ruimte. Geen fysiologische NLP-meting op zich.

## 13. Stem en ademgeluid

- toonhoogte;
- luidheid;
- spreektempo;
- ritme;
- pauzes;
- stemkwaliteit;
- spanning of trilling in de stem.

## 14. Visuele observatie en pupilgedrag

- oogbewegingen;
- pupilverwijding;
- knippergedrag;
- fixaties en kijkrichting.

Een enkele camera garandeert nog niet dat pupil- of oogmetingen voldoende betrouwbaar uitgevoerd kunnen worden. Belichting, optiek en beeldverwerking zijn mee bepalend. Zie [SENSOR_INVENTARIS.md](SENSOR_INVENTARIS.md).

## 15. Licht en optische metingen

Nog te bepalen welke exacte module hiervoor geschikt is. Zie [SENSOR_INVENTARIS.md](SENSOR_INVENTARIS.md) voor een openstaand aandachtspunt hierover.

## 16. Omgevingsfactoren

Meet de omstandigheden van de ruimte, niet van de student zelf. Nodig om te weten of een meetverschil tussen twee sessies door de student komt, of door bijvoorbeeld een warmere kamer die dag.

- temperatuur;
- luchtdruk;
- luchtvochtigheid;
- luchtkwaliteit;
- CO₂;
- VOC (vluchtige organische stoffen).

## 17. Sensorfusie en betrouwbaarheid van metingen

Geen fysiek sensortype, maar een overkoepelend methodologisch uitgangspunt: meerdere sensoren die hetzelfde signaal onafhankelijk meten, kunnen elkaar bevestigen of tegenspreken. Dit sluit rechtstreeks aan bij de kernwaarden wetenschappelijke integriteit en reproduceerbaarheid van metingen uit `PROJECT_CONSTITUTION.md`.

## 18. Emotie-observatie

Emotie-observatie is geen afzonderlijke enkelvoudige sensor. Zij kan later gebruikmaken van meerdere gevalideerde meetmodules. De documentatie zal steeds gemeten signalen, afgeleide patronen en menselijke interpretatie scheiden.

## Vereisten voor ieder toekomstig schema

Een nieuw schema wordt pas als raadpleegbaar referentieschema opgenomen wanneer het minimaal bevat:

- doel en versie;
- Arduino Uno R3-vormfactorbord en sensorvariant;
- volledig aansluitschema;
- pinbezetting;
- voeding en gemeenschappelijke massa;
- componentwaarden;
- kalibratie;
- voorbeeldcode;
- foutbronnen;
- veiligheids- en interpretatiegrenzen.
