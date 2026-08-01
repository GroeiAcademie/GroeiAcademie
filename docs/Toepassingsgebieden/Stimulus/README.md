# Module Stimulus

## Status

Stimulus is het eerste geïmplementeerde toepassingsgebied van GroeiAcademie FrameWork. De module bevindt zich in:

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

`AANTAL_SENSOREN_AANWEZIG` bepaalt of de opstelling twee of vier sensoren gebruikt. `SystemConfig.h` weigert iedere andere waarde tijdens het compileren.

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

De vaste weerstand moet passen bij het weerstandsbereik van de werkelijk gebruikte sensor. De repository legt in versie `1.0.0` nog geen universele weerstandswaarde vast, omdat die afhangt van sensorvariant, gewenste gevoeligheid en voedingsspanning. Noteer de gekozen waarde bij iedere meetopstelling.

Alle sensoren en de Arduino moeten een gemeenschappelijke massa gebruiken.

### Volledige logische aansluiting

```text
Sensorcircuit 1 uitgang  -> A0
Sensorcircuit 2 uitgang  -> A1
Sensorcircuit 3 uitgang  -> A2  (alleen bij 4 sensoren)
Sensorcircuit 4 uitgang  -> A3  (alleen bij 4 sensoren)
Alle GND-punten          -> Arduino GND
Voeding sensorcircuits   -> geschikte VCC volgens board en sensor
```

Sluit een kale resistieve sensor niet aan alsof zij zelf een analoge spanning genereert. Zij maakt deel uit van een spanningsdeler of geschikte meetmodule.

## Boardconfiguratie

Voor UNO R3:

```cpp
#define BOARD_VERSION BOARD_UNO_R3
```

De huidige configuratie gebruikt dan 10-bit ADC-schaal en `DELAY_US 100`.

Voor UNO R4 Minima:

```cpp
#define BOARD_VERSION BOARD_UNO_R4_MINIMA
```

Voor UNO R4 WiFi:

```cpp
#define BOARD_VERSION BOARD_UNO_R4_WIFI
```

Beide UNO R4-keuzes gebruiken 14-bit ADC-schaal en `DELAY_US 200`.

Voor ESP32-borden met boardprofiel `esp32:esp32:d1_uno32`, waaronder WEMOS D1 R32 en TTGO D1 R32:

```cpp
#define BOARD_VERSION BOARD_ESP32_UNO
```

De configuratie gebruikt dan 12-bit ADC-schaal en `DELAY_US 0`. Dit voegt vanuit de GroeiAcademie-library geen extra wachttijd tussen samples toe. WEMOS D1 R32 is sinds v1.0.0 getest en goedgekeurd. Controleer de werkelijke samplefrequentie, stabiliteit, 3,3 V-ingangsniveaus en sensorrespons afzonderlijk op TTGO D1 R32 en andere compatibele borden.

Controleer dat de gekozen ADC-resolutie ook werkelijk door de boardcore en initialisatie wordt toegepast. De voorbeeldprogramma's roepen bij 12 en 14 bits `analogReadResolution(ADC_BITS)` aan. De macro `ADC(x)` schaalt configuratiedrempels van de 10-bit referentiewaarden naar de gekozen ADC-schaal.

`ADC_Validatie_Native.ino` is een zelfstandig hardwarevalidatiescript en gebruikt daarom een eigen `BOARD_VERSION`-keuze in het script zelf. De gewone Stimulusvoorbeelden gebruiken de centrale configuratie uit `UserConfig.h` en `SystemConfig.h`.

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

- `examples/Systeem/ADC_Validatie/ADC_Validatie_Native/ADC_Validatie_Native.ino`
- `examples/Systeem/ADC_Validatie/ADC_Validatie_ADS1115/ADC_Validatie_ADS1115.ino`
- `examples/Toepassingsgebieden/Stimulus/Scenario1_EnkelTik/Scenario1_EnkelTik.ino`
- `examples/Toepassingsgebieden/Stimulus/Scenario2_Simultaan/Scenario2_Simultaan.ino`
- `examples/Toepassingsgebieden/Stimulus/Scenario3_Ineenstortend/Scenario3_Ineenstortend.ino`
- `examples/Toepassingsgebieden/Stimulus/Scenario4_Cocktail/Scenario4_Cocktail.ino`
- `examples/Toepassingsgebieden/Stimulus/Tik_Enkele_Samen_Instortend_Cocktail/Tik_Enkele_Samen_Instortend_Cocktail.ino`

De twee ADC-validatiescripts testen dezelfde meetlijn via twee afzonderlijke backends: directe Arduino-ADC en ADS1115. De volledige fysieke shieldopbouw en de bijbehorende validatiestappen staan in de hardwarebeschrijving en de afzonderlijke handleiding.

## Veiligheid en interpretatie

- controleer de maximale mechanische druk die de sensor en gebruiker veilig verdragen;
- plaats geen harde componentranden rechtstreeks tegen de huid;
- voorkom kortsluiting en trekbelasting op kabels;
- gebruik ADC-waarden niet als absolute kracht zonder kalibratie;
- vergelijk resultaten alleen binnen voldoende gelijkwaardige opstellingen;
- zie [../../../DISCLAIMER.md](../../../DISCLAIMER.md) en [../../SCIENTIFIC_INTEGRITY.md](../../SCIENTIFIC_INTEGRITY.md).

## Afzonderlijke schemabestanden

De ADS1115-hardwarelijn staat onder [Hardware/](Hardware/). De centrale uitleg staat in [Hardware/GroeiAcademie-Stimulus-Hardware-Shield-v1.0.0.md](Hardware/GroeiAcademie-Stimulus-Hardware-Shield-v1.0.0.md). Daarin worden H5, H6, H7 en H8 beschreven, samen met de keuze tussen directe Arduino-ADC en ADS1115 en de TFT-SPI-route. De praktische validatiestappen staan in [Hardware/Handleiding-GroeiAcademie-Stimulus-Hardware-Validatie-v1.0.0.md](Hardware/Handleiding-GroeiAcademie-Stimulus-Hardware-Validatie-v1.0.0.md).


### BOARD_ESP32_UNO

Arduino Uno R3-vormfactor ESP32-boardprofiel.

#### Reeds getest en ondersteund
- WEMOS D1 R32: getest en goedgekeurd sinds v1.0.0;
- TTGO D1 R32: gebruikt hetzelfde boardprofiel `esp32:esp32:d1_uno32`; fysieke hardwarevalidatie afzonderlijk vast te leggen.

#### Verwacht compatibel
- Andere Arduino Uno R3-vormfactor ESP32-borden met dezelfde Arduino-pinout en een ondersteunde Arduino ESP32-core.

#### Nog niet getest
- Aan te vullen na validatie.
