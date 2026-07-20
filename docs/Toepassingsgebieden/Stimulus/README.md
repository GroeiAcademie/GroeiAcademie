# Module Stimulus

## Status

Stimulus is het eerste geïmplementeerde toepassingsgebied van GroeiAcademie Framework. De module bevindt zich in:

```text
src/Toepassingsgebieden/Stimulus/
```

Publieke opname:

```cpp
#include <Stimulus.h>
```

of:

```cpp
#include <GroeiAcademie.h>
```

## Doel

De module registreert en vergelijkt vingertikken zodat duur, drukverloop en timing reproduceerbaar kunnen worden geoefend.

## Huidige meetgegevens

### Basisprofiel

- `TikTijd`;
- `gemiddeldeTikKracht`.

### Uitgebreid profiel

- `hoogsteTikKracht`;
- `tijdTotPiekTikKracht`;
- `opbouwSnelheid`;
- `afbouwSnelheid`.

### Synchronisatie

- verschil in starttijd;
- verschil in eindtijd;
- verschil in tikduur;
- verschil in gemiddelde en hoogste drukwaarde;
- aantal sensoren dat binnen de marge synchroon start of eindigt.

De benaming `TikKracht` in de API verwijst in de huidige implementatie naar een ADC-gerelateerde drukwaarde. Zonder afzonderlijke kalibratie is dit geen absolute kracht in newton of gramkracht.

## Elektronisch referentieschema

### Standaard pinbezetting

De standaardwaarden uit `src/Configuratie/SystemConfig.h` zijn:

| Sensor | Analoge ingang |
|---|---|
| sensor 1 | `A0` |
| sensor 2 | `A1` |
| sensor 3 | `A2` |
| sensor 4 | `A3` |

`AANTAL_SENSOREN_AANWEZIG` bepaalt of de opstelling twee of vier sensoren gebruikt.

### Aansluiting per resistieve druksensor

Gebruik per sensor een spanningsdeler:

```text
VCC
 |
[ druksensor ]
 |
 +--------------------> analoge ingang Ax
 |
[ vaste weerstand ]
 |
GND
```

Voor sensor 1 wordt `Ax` bijvoorbeeld `A0`, voor sensor 2 `A1`.

De vaste weerstand moet passen bij het weerstandsbereik van de werkelijk gebruikte sensor. De repository legt in versie `0.9.78` nog geen universele weerstandswaarde vast, omdat die afhangt van sensorvariant, gewenste gevoeligheid en voedingsspanning. Noteer de gekozen waarde bij iedere meetopstelling.

Alle sensoren en de Arduino moeten een gemeenschappelijke massa gebruiken.

### Volledige logische aansluiting

```text
Sensorcircuit 1 uitgang  -> A0
Sensorcircuit 2 uitgang  -> A1
Sensorcircuit 3 uitgang  -> A2  (alleen bij 3 of 4 sensoren)
Sensorcircuit 4 uitgang  -> A3  (alleen bij 4 sensoren)
Alle GND-punten          -> Arduino GND
Voeding sensorcircuits   -> geschikte VCC volgens board en sensor
```

Sluit een kale resistieve sensor niet aan alsof zij zelf een analoge spanning genereert. Zij maakt deel uit van een spanningsdeler of geschikte meetmodule.

## Boardconfiguratie

Voor UNO R3:

```cpp
#define UNO_VERSION 3
```

De huidige configuratie gebruikt dan 10-bit ADC-schaal en `DELAY_US 100`.

Voor UNO R4:

```cpp
#define UNO_VERSION 4
```

De huidige configuratie gebruikt dan 14-bit ADC-schaal en `DELAY_US 200`.

Controleer dat de gekozen ADC-resolutie ook werkelijk door de boardcore en initialisatie wordt toegepast. De macro `ADC(x)` schaalt configuratiedrempels van de 10-bit referentiewaarden naar de gekozen ADC-schaal.

## Configuratiegrenzen

Belangrijke waarden in `StimulusConfig.h` zijn onder andere:

```cpp
TIK_MINIMALE_DRUKWAARDE
TIKKRACHT_MINIMALE_COMFORT_GRENS
TIKKRACHT_MAXIMALE_COMFORT_GRENS
MINIMALE_TIKTIJD_MS
MAXIMALE_TIKTIJD_MS
OFFSET_METING_TIJD_MS
OFFSET_VEILIGHEIDSMARGE
```

Deze waarden zijn onderdeel van de huidige softwareconfiguratie en geen universele fysiologische normen. Zij moeten voor een concrete sensor, mechanische opbouw en gebruiker worden gevalideerd.

## Nulmeting en offsets

`BepaalSensorOffsets()` bepaalt sensoroffsets. De nulmeting vormt de referentie voor latere vergelijking van duur en gemiddelde drukwaarde.

Voor reproduceerbare resultaten:

- monteer elke sensor op dezelfde manier;
- wijzig de spanningsdeler niet tussen nulmeting en test;
- gebruik dezelfde board- en ADC-configuratie;
- laat sensoren volledig los vóór de volgende meting;
- voer opnieuw een nulmeting uit na mechanische of elektrische wijzigingen.

## Scenario's en voorbeelden

- `examples/Stimulus/Scenario1_EnkelTik/Scenario1_EnkelTik.ino`
- `examples/Stimulus/Scenario2_Simultaan/Scenario2_Simultaan.ino`
- `examples/Stimulus/Scenario3_Ineenstortend/Scenario3_Ineenstortend.ino`
- `examples/Stimulus/Scenario4_Cocktail/Scenario4_Cocktail.ino`
- `examples/Stimulus/Tik_Enkele_Samen_Instrotend_Coctail/Tik_Enkele_Samen_Instrotend_Coctail.ino`

De bestandsnaam van het samengestelde voorbeeld bevat historische spelfouten. Die naam wordt hier exact weergegeven omdat het pad in de huidige repository zo bestaat.

## Veiligheid en interpretatie

- controleer de maximale mechanische druk die de sensor en gebruiker veilig verdragen;
- plaats geen harde componentranden rechtstreeks tegen de huid;
- voorkom kortsluiting en trekbelasting op kabels;
- gebruik ADC-waarden niet als absolute kracht zonder kalibratie;
- vergelijk resultaten alleen binnen voldoende gelijkwaardige opstellingen;
- zie [../../DISCLAIMER.md](../../DISCLAIMER.md) en [../SCIENTIFIC_INTEGRITY.md](../SCIENTIFIC_INTEGRITY.md).

## Afzonderlijke schemabestanden

In versie `0.9.78` zijn nog geen afzonderlijke KiCad-, Fritzing-, PDF- of PNG-schema's opgenomen. Het schema en de pinbezetting in dit document zijn daarom voorlopig de gezaghebbende elektronische documentatie.
