# Toepassingsgebieden en schema-index

Dit document onderscheidt de huidige implementatie van geplande toepassingsgebieden en vormt de centrale index voor elektronische schema's.

## Statusoverzicht

| Toepassingsgebied | Softwarestatus | Elektronische documentatie |
|---|---|---|
| Stimulus | geïmplementeerd | [Stimulus/README.md](Stimulus/README.md) + [Stimulus/Hardware/Stimulus_uitbreiding_ADS1115_TFTSPI_v0.10.1.md](Stimulus/Hardware/Stimulus_uitbreiding_ADS1115_TFTSPI_v0.10.1.md) |
| Screen | gedeelde systeemlaag | [../Systeem/SCREEN.md](../Systeem/SCREEN.md) |
| Ademhaling | gepland | nog geen gevalideerd schema |
| Hartritme, HRV en coherentie | gepland | nog geen gevalideerd schema |
| Biopotentiële signalen | gepland | nog geen gevalideerd schema |
| Emotie-observatie | inhoudelijk gepland | afhankelijk van de uiteindelijk gekozen meetmodules |
| Overige observatievaardigheden | toekomstig | nog niet bepaald |

Een gepland toepassingsgebied mag niet als werkende libraryfunctionaliteit worden voorgesteld.

## 1. Stimulus

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

### Scenario's

De repository bevat afzonderlijke voorbeelden voor:

1. enkele tik;
2. simultane tik;
3. ineenstortend patroon;
4. cocktail;
5. een samengesteld demonstratievoorbeeld.

Zie [Stimulus/README.md](Stimulus/README.md) voor aansluiting, configuratie en interpretatiegrenzen. Zie [Stimulus/Hardware/Stimulus_uitbreiding_ADS1115_TFTSPI_v0.10.1.md](Stimulus/Hardware/Stimulus_uitbreiding_ADS1115_TFTSPI_v0.10.1.md) voor de actuele ADS1115- en TFTSPI-hardwarelijn met H5, H6, H7 en H8. Het validatieprotocol staat in [Stimulus/Hardware/VALIDATIE_Shield_v0.10.1.md](Stimulus/Hardware/VALIDATIE_Shield_v0.10.1.md).

## 2. Ademhaling

Geplande meetdoelen zijn onder meer ademfrequentie, ritme, diepte, ademfase en waar mogelijk mond- of neusademhaling. De definitieve sensoren en elektronische schema's zijn nog niet vastgesteld. Een schema wordt pas opgenomen nadat de opstelling praktisch en inhoudelijk is gevalideerd.

## 3. Hartritme, HRV en coherentie

Geplande meetdoelen zijn hartslag en, mits een geldige meet- en analysemethode, HRV en coherentie. Sensor, samplekwaliteit, artefactdetectie en algoritmen moeten vóór implementatie expliciet worden gekozen en beschreven.

## 4. Biopotentiële signalen

Dit toepassingsgebied is gereserveerd voor signalen zoals ECG, EMG of andere elektrische lichaamssignalen. Vanwege veiligheid, signaalkwaliteit en interpretatierisico's is nog geen schema als officieel referentieontwerp opgenomen.

## 5. Emotie-observatie

Emotie-observatie is geen afzonderlijke enkelvoudige sensor. Zij kan later gebruikmaken van meerdere gevalideerde meetmodules. De documentatie zal steeds gemeten signalen, afgeleide patronen en menselijke interpretatie scheiden.

## Vereisten voor ieder toekomstig schema

Een nieuw schema wordt pas als raadpleegbaar referentieschema opgenomen wanneer het minimaal bevat:

- doel en versie;
- board en sensorvariant;
- volledig aansluitschema;
- pinbezetting;
- voeding en gemeenschappelijke massa;
- componentwaarden;
- kalibratie;
- voorbeeldcode;
- foutbronnen;
- veiligheids- en interpretatiegrenzen.
