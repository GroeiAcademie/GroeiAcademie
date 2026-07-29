# Validatie GroeiAcademie Arduino UNO R3/R4 Shield v0.10.1

## Doel

Dit protocol valideert de volledige specifieke hardware uit de v0.10.1-tekeningset. Eén zelfstandig script voert alle automatiseerbare controles uit. Controles die software niet betrouwbaar kan uitvoeren, worden in hetzelfde protocol met een multimeter en visuele inspectie afgetekend.

De twee ADC-routes kunnen fysiek nooit tegelijk geplaatst worden. Daarom wordt hetzelfde script tweemaal gebruikt:

1. met de vier H6-jumpers en `VALIDATIE_ADC_ROUTE_NATIVE`;
2. zonder de H6-jumpers, met ADS1115 op H5 en `VALIDATIE_ADC_ROUTE_ADS1115`.

Plaats nooit tegelijk de ADS1115-module op H5 en de H6-jumpers op H5 pin 7-10.

## Bijbehorend script

```text
examples/Stimulus/Hardware_Validatie_Shield_v0_10_1/Hardware_Validatie_Shield_v0_10_1.ino
```

## Conditionele onderdelen en libraries

Bovenaan het script worden alleen de aanwezige onderdelen geactiveerd:

```cpp
#define VALIDATIE_I2C_BUS          1
#define VALIDATIE_CHARACTER_SCREEN 1
#define VALIDATIE_PIXEL_SCREEN     1
#define VALIDATIE_KEYMATRIX        1
#define VALIDATIE_ADC_ROUTE VALIDATIE_ADC_ROUTE_NATIVE
```

Een onderdeel op `0` wordt niet getest en de bijbehorende objecten en libraries worden niet gecompileerd. De gekozen ADC-route bepaalt of `Adafruit ADS1X15` nodig is.

| Keuze | Extra library |
|---|---|
| `VALIDATIE_CHARACTER_SCREEN 1` | `LiquidCrystal I2C` |
| `VALIDATIE_PIXEL_SCREEN 1` | `Adafruit GFX Library` en `Adafruit ST7735 and ST7789 Library` |
| `VALIDATIE_ADC_ROUTE_ADS1115` | `Adafruit ADS1X15` |
| alleen Native ADC | geen ADS1115-library nodig |

## Bijbehorende tekeningset

- `SCH_GroeiAcademie-Arduino_Uno_R3-R4_Shield-v0.10.1_2026-07-27.json`;
- `Schematic_GroeiAcademie-Arduino_Uno_R3-R4_Shield-v0.10.1_2026-07-27.pdf`;
- `Schematic_GroeiAcademie-Arduino_Uno_R3-R4_Shield-v0.10.1_2026-07-27.png`;
- `Schematic_GroeiAcademie-Arduino_Uno_R3-R4_Shield-v0.10.1_2026-07-27.svg`.

Gebruik uitsluitend deze v0.10.1-tekeningset bij continuïteits- en isolatiemetingen.

## Verplichte voorbereiding zonder aangesloten modules

1. Controleer visueel op soldeerbruggen, koude solderingen, beschadigde pads en onderbroken banen.
2. Controleer connectororiëntatie, pinnummers en silkscreen tegen het schema.
3. Meet op de spanningsloze print op kortsluiting tussen 5 V en GND en tussen 3,3 V en GND.
4. Schakel de voeding in zonder displays of ADS1115 en meet Arduino 5 V, Arduino 3,3 V, H7-uitgang, TFT-VCC en de HV- en LV-voedingsrails.
5. Schakel de voeding opnieuw uit voordat modules, jumpers, draadbruggen of shifters worden geplaatst.
6. Plaats voor TFT-SPI ofwel de levelshifters, ofwel de vijf draadbruggen. Plaats nooit beide tegelijk.
7. Plaats op SW1 maximaal één ADDR-schakelaar ON.
8. Plaats op H7 exact één voedingskeuze: 5 V of 3,3 V.

## Run 1: directe Arduino-ADC

Gebruik deze fysieke opstelling:

- geen ADS1115 op H5;
- vier H6-jumpers op H5 pin 7-10;
- `#define VALIDATIE_ADC_ROUTE VALIDATIE_ADC_ROUTE_NATIVE`.

Compileer en upload het script. Open de Serial Monitor op 115200 baud.

## Run 2: ADS1115

Schakel eerst alle voeding uit en wijzig daarna de fysieke opstelling:

- verwijder de vier H6-jumpers;
- plaats de ADS1115-module op H5;
- kies via SW1 het bedoelde adres, standaard ADDR naar GND voor `0x48`;
- `#define VALIDATIE_ADC_ROUTE VALIDATIE_ADC_ROUTE_ADS1115`.

Compileer en upload hetzelfde script opnieuw.

## Testvolgorde in het script

### 0. Visuele en multimetercontrole

Het script herhaalt de verplichte veiligheidscontroles. Deze worden niet automatisch goedgekeurd; ze moeten afzonderlijk worden afgetekend.

### 1. I2C-bus

De Serial Monitor toont alle gevonden adressen. Standaard worden verwacht:

- CharacterScreen op `0x27`, wanneer geactiveerd;
- ADS1115 op `0x48`, tijdens de ADS1115-run.

Een gevonden adres bewijst communicatie op de bus. Het bewijst niet dat alle voedings- of shifterverbindingen veilig zijn.

### 2. CharacterScreen

Het scherm moet twee leesbare regels tonen:

```text
GA Shield 0.10.1
LCD OK?
```

Controleer ook backlight, contrast, stabiele tekst en afwezigheid van resets.

### 3. TFT-SPI

Het volledige scherm moet achtereenvolgens rood, groen, blauw, wit en zwart worden. Daarna verschijnt witte tekst op zwarte achtergrond. Controleer:

- volledige beeldvulling;
- correcte kleuren;
- correcte oriëntatie;
- stabiele tekst;
- correcte resetwerking;
- geen flikkering;
- geen willekeurige gekleurde punten.

Een scherm met gekleurde ruis of punten is niet goedgekeurd.

### 4. Keymatrix D2-D5

Gedurende vijftien seconden meldt de Serial Monitor elke elektrische statuswijziging op D2, D3, D4 en D5. Bedien alle aangesloten toetsen.

De automatische test slaagt zodra minstens één wijziging gemeten wordt. Daarna moet handmatig worden gecontroleerd dat iedere toets de bedoelde lijn of combinatie activeert. Een enkele gemeten wijziging keurt dus niet automatisch de volledige keymatrix goed.

### 5. ADC-route

Tijdens de Native-run worden Arduino A0 tot en met A3 uitgelezen. Tijdens de ADS1115-run worden ADS1115 A0 tot en met A3 uitgelezen.

Bedien iedere sensor afzonderlijk en controleer:

- het bedoelde kanaal verandert duidelijk;
- de andere kanalen blijven zoveel mogelijk stabiel;
- geen kanaal blijft permanent op minimum of maximum staan;
- waarden keren na loslaten terug naar de uitgangssituatie;
- beide ADC-routes zien dezelfde fysieke sensorlijnen na de juiste fysieke ombouw.

Gebruik daarna `ADC_Validatie_Native` en `ADC_Validatie_ADS1115` voor de afzonderlijke meetkundige vergelijking en kalibratie.

### 6. Continuïteit, isolatie en bestukking

Software kan geen volledige printcontinuïteit of isolatie bewijzen. Controleer daarom met spanningsloze print:

- H1 tot en met H10 pin voor pin tegen het actuele schema;
- isolatie tussen naburige connectorpinnen;
- isolatie tussen alle netten die volgens het schema niet verbonden zijn;
- U3-kanaaltoewijzing voor SDA, SCL en TFT-RST;
- U8-kanaaltoewijzing voor TFT SCL, SDA, CS en DC;
- H5, H6, H7, H8 en SW1 tegen schema en silkscreen;
- geen gelijktijdige shifter en draadbrug op hetzelfde kanaalpaar;
- geen gelijktijdige ADS1115 en H6-jumpers.

## Aftekenlijst

| Controle | Native-run | ADS1115-run | Resultaat/opmerking |
|---|---|---|---|
| 5 V, 3,3 V en GND correct |  |  |  |
| H7-uitgang correct |  |  |  |
| HV/LV-rails correct |  |  |  |
| Geen voedingskortsluiting |  |  |  |
| I2C-scan correct |  |  |  |
| CharacterScreen correct |  |  |  |
| TFT kleuren, tekst en reset correct |  |  |  |
| Keymatrix iedere toets correct |  |  |  |
| A0-A3 alle vier correct |  | n.v.t. |  |
| ADS1115 A0-A3 alle vier correct | n.v.t. |  |  |
| H1-H10 continuïteit correct |  |  |  |
| Niet-verbonden netten geïsoleerd |  |  |  |
| U3/U8 bestukking correct |  |  |  |
| H5/H6/SW1/H7 bestukking correct |  |  |  |
| Geen reset, oververhitting of instabiliteit |  |  |  |

## Goedkeuringscriterium

Het shield is pas volledig goedgekeurd wanneer:

- beide ADC-runs zijn uitgevoerd voor zover beide routes ondersteund moeten worden;
- de automatische samenvatting geen fouten toont;
- alle handmatige controles zijn afgetekend;
- alle vier sensorlijnen afzonderlijk correct reageren;
- alle toetsen correct reageren;
- beide schermroutes correct werken;
- alle continuïteits-, isolatie- en voedingsmetingen overeenkomen met de actuele v0.10.1-tekeningset;
- geen busconflict, reset, oververhitting of instabiele voeding optreedt.

## Reproduceerbaar validatierecord

Noteer voor iedere finale validatie minstens:

- datum;
- Arduino IDE- of Arduino CLI-versie;
- board en FQBN;
- Arduino core en versie;
- gebruikte libraryversies;
- actieve validatiekeuzes bovenaan het script;
- gebruikte schermen, ADS1115-module en voedingskeuze;
- resultaat van Native-run en ADS1115-run;
- compileerresultaat, waarschuwingen, flashgebruik en SRAM-gebruik;
- naam of initialen van de uitvoerder.

| Gegeven | Waarde |
|---|---|
| Datum |  |
| IDE/CLI |  |
| Board/FQBN |  |
| Coreversie |  |
| Libraryversies |  |
| Validatiekeuzes |  |
| Native-run |  |
| ADS1115-run |  |
| Flash/SRAM |  |
| Uitvoerder |  |

## Grenzen van automatische validatie

Geen Arduino-sketch kan zelfstandig bewijzen dat een voedingsspanning veilig is, HV en LV fysiek juist zijn aangesloten, een niet-aangesloten printbaan werkelijk doorloopt, twee niet-verbonden netten voldoende geïsoleerd zijn of shifters en draadbruggen niet gelijktijdig geplaatst zijn. Daarom bestaat volledige shieldvalidatie uit de automatische testen én de beschreven visuele en multimetercontroles.
