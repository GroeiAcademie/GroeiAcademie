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

## v2.0.0 Sensorroute

De huidige Stimuluscode neemt `src/Systeem/Sensor/RFP602.h` op en leest de analoge kanalen via de globale pointer `sensorRFP602set1`. RFP602 meet via een ADC-Extender (nu: ADS1115), zoals elke Sensor op een Extender wordt ingeplugd. De Stimulusvoorbeelden maken eerst de Extender aan en daarna de Sensor met die Extender als parent:

```cpp
ExtenderADS1115 extenderADS1115(
  &Native, GedeeldeBusComponent::ADC_ADS1115,
  I2C_ADDRESS_EXTENDER_ADS1115_1,
  HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL,
  EXTENDER_ADS1115_1_ALERT_RDY,
  HardwareResourceToegang::GEDEELD
);

sensorRFP602set1 = GedeeldeBusNewComponent<struct RFP602>(&extenderADS1115, GedeeldeBusComponent::RFP602);
if (sensorRFP602set1 == nullptr) exit(0);
```

Zonder kanaalargumenten gebruikt RFP602 de standaardkanalen `RFP602_ADC_PIN_SENSOR_1_SET_1` tot `RFP602_ADC_PIN_SENSOR_4_SET_1` (ADS1115: `EP_AIN0` tot `EP_AIN3`). Een `.ino` kan ook vier eigen kanalen meegeven. `RawAnalogRead()` leest via `ExtenderADS1115::readADC()`.

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

| Sensor | Ingang van de ADS1115 |
|---|---|
| sensor 1 | `AIN0` |
| sensor 2 | `AIN1` |
| sensor 3 | `AIN2` |
| sensor 4 | `AIN3` |

De ADS1115 zelf hangt op de I2C-bus (SDA/SCL), standaard op adres `0x48`.

`RFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_1` bepaalt of de opstelling twee of vier sensoren gebruikt. `SystemConfig.h` weigert iedere andere waarde tijdens het compileren.

### Aansluiting per resistieve druksensor

Gebruik per sensor een spanningsdeler:

```text
VCC
 |
[ druksensor ]
 |
 +--------------------> ADS1115-ingang AINx
 |
[ vaste weerstand ]
 |
GND
```

Voor sensor 1 wordt `AINx` bijvoorbeeld `AIN0`, voor sensor 2 `AIN1`.

De vaste weerstand moet passen bij het weerstandsbereik van de werkelijk gebruikte sensor. De repository legt in versie `1.0.0` nog geen universele weerstandswaarde vast, omdat die afhangt van sensorvariant, gewenste gevoeligheid en voedingsspanning. Noteer de gekozen waarde bij iedere meetopstelling.

Alle sensoren, de ADS1115 en de Arduino moeten een gemeenschappelijke massa gebruiken.

### Volledige logische aansluiting

```text
Sensorcircuit 1 uitgang  -> ADS1115 AIN0
Sensorcircuit 2 uitgang  -> ADS1115 AIN1
Sensorcircuit 3 uitgang  -> ADS1115 AIN2  (alleen bij 4 sensoren)
Sensorcircuit 4 uitgang  -> ADS1115 AIN3  (alleen bij 4 sensoren)
ADS1115 SDA/SCL          -> Arduino SDA/SCL
Alle GND-punten          -> Arduino GND
Voeding sensorcircuits   -> geschikte VCC volgens Arduino Uno R3-vormfactorbord en sensor
```

Sluit een kale resistieve sensor niet aan alsof zij zelf een analoge spanning genereert. Zij maakt deel uit van een spanningsdeler of geschikte meetmodule.

## Boardconfiguratie

Het bord kies je met `BOARD_VERSION`, bijvoorbeeld:

```cpp
#define BOARD_VERSION BOARD_UNO_R4_MINIMA
```

De meting zelf loopt op elk bord via de ADS1115: 15-bit schaal (`ADC_BITS 15`) en geen extra wachttijd tussen samples (`DELAY_US 0`), want de ADS1115 wacht zelf op de conversie. De macro `ADC(x)` schaalt configuratiedrempels van de 10-bit referentiewaarden naar deze schaal.

`Extender_ADS1115_Meetkwaliteit.ino` is een zelfstandig hardwarevalidatiescript voor de ADS1115-Extender. De gewone Stimulusvoorbeelden gebruiken de centrale configuratie uit `UserConfig.h` en `SystemConfig.h`.

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
- gebruik dezelfde Arduino Uno R3-vormfactorbord- en ADC-configuratie;
- laat sensoren volledig los vóór de volgende meting;
- voer opnieuw een nulmeting uit na mechanische of elektrische wijzigingen.

## Scenario's en voorbeelden

- `examples/Systeem/GedeeldeBus/Extenders/Extender_ADS1115_Meetkwaliteit/Extender_ADS1115_Meetkwaliteit.ino`
- `examples/Toepassingsgebieden/Stimulus/Scenario1_EnkelTik/Scenario1_EnkelTik.ino`
- `examples/Toepassingsgebieden/Stimulus/Scenario2_Simultaan/Scenario2_Simultaan.ino`
- `examples/Toepassingsgebieden/Stimulus/Scenario3_Ineenstortend/Scenario3_Ineenstortend.ino`
- `examples/Toepassingsgebieden/Stimulus/Scenario4_Cocktail/Scenario4_Cocktail.ino`
- `examples/Toepassingsgebieden/Stimulus/Tik_Enkele_Samen_Instortend_Cocktail/Tik_Enkele_Samen_Instortend_Cocktail.ino`

Het validatiescript `Extender_ADS1115_Meetkwaliteit.ino` meet 10 s lang de vier ingangen van de ADS1115-Extender en toont per kanaal gemiddelde, standaardafwijking, minimum en maximum. De volledige fysieke shieldopbouw en de bijbehorende validatiestappen staan in de hardwarebeschrijving en de afzonderlijke handleiding.

## Veiligheid en interpretatie

- controleer de maximale mechanische druk die de sensor en gebruiker veilig verdragen;
- plaats geen harde componentranden rechtstreeks tegen de huid;
- voorkom kortsluiting en trekbelasting op kabels;
- gebruik ADC-waarden niet als absolute kracht zonder kalibratie;
- vergelijk resultaten alleen binnen voldoende gelijkwaardige opstellingen;
- zie [../../../DISCLAIMER.md](../../../DISCLAIMER.md) en [../../SCIENTIFIC_INTEGRITY.md](../../SCIENTIFIC_INTEGRITY.md).

## Afzonderlijke schemabestanden

De hardwarelijn staat onder [Uitbreidingskaarten](../../Uitbreidingskaarten/). De actuele versie is [Stimulus Shield v2.0.0](../../Uitbreidingskaarten/Stimulus%20Shield%20v2.0.0/Stimulus-Shield-%28GroeiAcademie-FrameWork%29-v2.0.0.md): ADS1115 op H5, voedingskeuze H7, TFT-SPI op H8, PCF8574 en HX1838. De aanvullende validatiestappen staan in [Handleiding Stimulus Shield v2.0.0](../../Uitbreidingskaarten/Stimulus%20Shield%20v2.0.0/Handleiding-Stimulus-Shield-%28GroeiAcademie-FrameWork%29-v2.0.0.md).


### BOARD_ESP32_D1_UNO_R32

Arduino Uno R3-vormfactor ESP32-boardprofiel.

#### Reeds getest en ondersteund
- TTGO D1 R32 (ESP32-WROOM-32U): geïmplementeerd en getest;
- WeMos D1 R32 (ESP32-WROOM-32U): geïmplementeerd en getest.

Beide gebruiken `BOARD_ESP32_D1_UNO_R32` en worden in de dependencycontrole gezamenlijk aangeduid als `ESP32-WROOM-32U`.

#### Verwacht compatibel
- Andere Arduino Uno R3-vormfactor ESP32-borden met dezelfde Arduino-pinout en een ondersteunde Arduino ESP32-core.

#### Nog niet getest
- Aan te vullen na validatie.
