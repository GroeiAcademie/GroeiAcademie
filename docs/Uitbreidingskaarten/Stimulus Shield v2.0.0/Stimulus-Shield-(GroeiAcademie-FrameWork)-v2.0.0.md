> **Versiegrens:** dit document hoort bij de Stimulus Shield **hardware v2.0.0**. Voor de huidige softwarearchitectuur gelden `README.md`, `docs/ARCHITECTURE.md` en de code onder `src/`.

# Stimulus uitbreiding met ADS1115, TFTSPI en HX1838

> Status: actuele hardwaredocumentatie v2.0.0. ADS1115 blijft ondersteund als hardwarematig geteste uitbreiding.
> Deze versie beschrijft uitsluitend de actuele Stimulus Shield v2.0.0-hardware.

> **Gezaghebbende hardwarebron**: `Schematic_Stimulus-Shield-(GroeiAcademie-FrameWork)-v2.0.0_2026-10-22` (shield-PCB). De ADDR-keuze gebeurt via SW1. De voedingskeuze voor het FSR/ADS1115-pad gebeurt via H7, de 3-pin jumper 5V/3V3. H8 is in het v2.0.0-schema de 1x7 TFTSPI-displayconnector.

**FSR402/RFP602 analoge uitlezing transparant maken voor ADS1115, met TFTSPI-uitbreiding**

Gebruik voor bouw en validatie uitsluitend de actuele v2.0.0-tekeningset in deze map.

## Deel 1: Beslissing en uitgangspunt

## 1. Korte conclusie

Op het Stimulus Shield v2.0.0 worden de vier FSR402/RFP602-sensorlijnen uitsluitend via een ADS1115-module op H5 gemeten. De ADS1115 hangt op de I2C-bus van de Arduino. De analoge ingangen A0-A3 van de Arduino worden niet meer gebruikt, net zoals de vroegere rechtstreekse keypadaansluiting: het keypad loopt via de PCF8574. Zo blijven acht native pinnen vrij.

In de software is de ADS1115 een gewone Extender (`ExtenderADS1115`). De rest van de Stimulus-code hoeft niet te weten waar de analoge waarden vandaan komen: RFP602 meet via de ADC-Extender die de `.ino` als parent meegeeft.

De hardware-opzet bestaat uit één schema met meerdere onafhankelijke bestukkingskeuzes via Dupont-connectoren, zonder solder-jumpers per sensorlijn.

## 2. I2C-adres

Je kan per ADS1115-module zelf het I2C-adres kiezen via de ADDR-pin.

Je kiest dus niet per kanaal een adres, wel per ADS1115-chip of module.

| ADDR-pin verbonden met | I2C-adres | Gebruik |
|---|---:|---|
| ADDR naar GND | 0x48 | Standaard voor Stimulus |
| ADDR naar VCC | 0x49 | Tweede ADS1115 op dezelfde I2C-bus, bijvoorbeeld emotiemeting |
| ADDR naar SDA | 0x4A | Derde ADS1115 op dezelfde I2C-bus |
| ADDR naar SCL | 0x4B | Vierde ADS1115 op dezelfde I2C-bus |

Per ADS1115 kan je het adres kiezen met de ADDR-pin. De vier adressen zijn 0x48, 0x49, 0x4A en 0x4B. Daardoor kan je tot vier ADS1115-modules op dezelfde I2C-bus zetten. Voor de Stimulus-uitbreiding is één ADS1115 voldoende voor de vier FSR402/RFP602-lijnen.

De ADDR-adreskeuze gebeurt in het v2.0.0-shieldschema via SW1 (zie §8.1 voor de posities). Er mag altijd maar één SW1-schakelaar tegelijk ON staan. Voor Stimulus is de standaard ADDR naar GND (SW1-positie 2), dus adres 0x48.

Zonder I2C-multiplexer kan je maximaal vier ADS1115-modules op dezelfde I2C-bus zetten. Dat geeft maximaal 4 modules x 4 single-ended kanalen = 16 analoge ingangen.

## 3. Waarom de ADS1115 de enige meetroute is

| Voordeel | Toelichting |
|---|---|
| Vrije pinnen | De analoge pinnen van de Arduino blijven vrij voor andere uitbreidingen |
| Resolutie | 15 bit single-ended (0-32767) in plaats van 10 tot 14 bit, afhankelijk van het bord |
| Uitbreidbaar | Tot vier ADS1115-modules op dezelfde I2C-bus |
| Bordonafhankelijk | Op elk ondersteund Arduino Uno R3-vormfactorbord dezelfde meting |

De ADS1115-module wordt op H5 geplaatst. De keuze voor de I2C-niveauconversie van het CharacterScreen gebeurt apart: ofwel wordt de Quad Logic Level Shifter geplaatst, ofwel worden draadbruggen gebruikt wanneer niveauconversie niet nodig is.

## 4. Bestukkingskeuze in het schema

| Bestukking | Verbinding | Codegevolg |
|---|---|---|
| ADS1115 bestukt | FSR402/RFP602 OUT1-OUT4 naar ADS1115 A0-A3 | Arduino leest via de I2C-bus |
| Keuze via connector/jumpers | De ADS1115-module op H5. | Flexibel, geen solder-jumpers per sensorlijn nodig |

## 5. Hardware-aandachtspunten

| Punt | Opmerking |
|---|---|
| Voeding ADS1115 | Als de FSR402/RFP602-spanningsdelers op 5V staan, moet de ADS1115-opstelling 5V-ingangen veilig aankunnen. De analoge ingang mag niet boven de voedingsspanning van de ADS1115 komen. |
| I2C-bus | LCD en ADS1115 mogen samen op SDA/SCL, zolang adressen niet botsen en pull-ups niet te zwaar worden. |
| Quad Logic Level Shifter | De ADS1115 hangt rechtstreeks op SDA/SCL van de Arduino, niet via U3. U3 zet alleen het CharacterScreen (HV-zijde, 5V) om naar de Arduino-bus (LV-zijde). |
| GND | Arduino, ADS1115, sensor-spanningsdelers en eventuele externe voeding moeten een gemeenschappelijke GND hebben. |
| Resolutie | ADS1115 geeft meer meetresolutie dan de gewone Arduino ADC. Dat is nuttig bij subtiele drukverschillen. |
| Sampling | ADS1115 is niet bedoeld voor extreem snelle signalen, wel ruim voldoende voor trage stimulus/drukveranderingen. |

De ADS1115 deelt de I2C-bus met het LCD. In het v2.0.0-schema zitten de ADS1115 (H5) en de PCF8574 (H11) rechtstreeks op SDA/SCL van de Arduino; het LCD zit aan de HV-zijde van U3 (of via draadbruggen). De ADS1115 en de PCF8574 worden gevoed via H7 (5V of 3V3): kies H7 dus volgens de logicaspanning van het Arduino-bord.

De footprints voor de Quad Logic Level Shifters mogen twee functies krijgen: ofwel wordt de Quad Logic Level Shifter geplaatst wanneer niveauconversie nodig is, ofwel worden de overeenkomstige kanaalparen rechtstreeks met draadbruggen verbonden wanneer beide zijden dezelfde logicaspanning gebruiken. GND blijft gemeenschappelijk. U3 wordt gedeeld gebruikt voor LCD-I2C en TFT-RST. U8 wordt gebruikt voor TFTSPI SCL, SDA, DC en CS.

## Deel 2: Eén schema, meerdere onafhankelijke bestukkingskeuzes

In dit deel worden de connectorreferenties uit het schema expliciet gebruikt. Zo blijft de Markdown rechtstreeks bruikbaar naast het schema. Er zijn vier onafhankelijke keuzes: het ADC-pad, de I2C-niveauconversie, de TFTSPI-niveauconversie en het input-pad. Voor Input zijn de PCF8574-uitbreiding via de gedeelde I2C-bus en de HX1838 IR Receiver met DATA op D8 beschikbaar.

| Referentie | Functie in het schema |
|---|---|
| H2 | 1x4 Dupont-connector van LCD1 naar de LCD-I2C-bus |
| H3 | 1x6 Dupont-connector aan één zijde van U3. Bevat GND, VCC, RST, SDA en SCL. U3 wordt kanaal per kanaal gebruikt; H3/H4 zijn dus niet zuiver “LCD-zijde” versus “Arduino-zijde”. |
| H4 | 1x6 Dupont-connector aan de andere zijde van U3. Bevat de overeenkomstige GND-, VCC-, RST-, SDA- en SCL-lijnen via U3 of draadbruggen. |
| H5 | 1x10 Dupont-connector voor de ADS1115-module en de vier analoge sensorlijnen |
| H7 | 3-pin jumper voor voedingskeuze van het FSR/ADS1115-pad: 5V of 3V3 |
| H8 | 1x7 Dupont-connector voor GMT020-02 / 2.0 inch TFTSPI-display |
| H9 | 1x6 Dupont-connector aan de TFT-zijde van de TFTSPI footprint voor de Quad Logic Level Shifter U8 |
| H10 | 1x6 Dupont-connector aan de Arduino-zijde van de TFTSPI footprint voor de Quad Logic Level Shifter U8 |
| H11 | 1x4 Dupont-connector voor de I2C-aansluiting van de PCF8574-uitbreiding |
| U9 | PCF8574-board voor optionele I2C-naar-digitale-I/O-uitbreiding |
| H12 | 1x9 vrouwelijke Dupont-connector voor P0-P7 en INT van het PCF8574-board |
| H13 | 1x3 Dupont-connector tussen het shield en de HX1838 IR Receiver |
| U10 | HX1838 IR Receiver; DATA is verbonden met Arduino D8 |
| SW1 | 4-kanaals DIP-switch voor ADS1115 ADDR-keuze |
| U1 | Arduino UNO R3/R4 |
| U3 | Quad Logic Level Shifter voor LCD-I2C en TFT-RST |
| U8 | Quad Logic Level Shifter voor TFTSPI SCL, SDA, DC en CS |

## 6. Bestukkingsopties per footprint

### 6.1 ADS1115

H5 is de centrale 10-polige Dupont-connector voor het ADS1115-pad en de vier analoge sensorlijnen. H5 pin 7 tot en met pin 10 zijn uitsluitend met de sensorlijnen verbonden; de analoge pinnen van de Arduino worden niet gebruikt. H7, de 3-pin jumper 5V/3V3, is de voedingskeuze voor het FSR/ADS1115-pad en voor het PCF8574-board U9. SW1 is uitsluitend de ADDR-keuze voor de ADS1115-adreskeuze. H8 is de TFTSPI-displayconnector.

| Pinvolgorde H5, 10-polige ADS1115-connector | Functie |
|---|---|
| Pin 1 | VCC |
| Pin 2 | GND |
| Pin 3 | SCL |
| Pin 4 | SDA |
| Pin 5 | ADDR |
| Pin 6 | ALRT |
| Pin 7 | A0 |
| Pin 8 | A1 |
| Pin 9 | A2 |
| Pin 10 | A3 |

Bij de ADS1115-keuze wordt het ADS1115-bordje op H5 geplaatst. Daarna kies je via SW1 het gewenste I2C-adres.

### 6.2 Met of zonder Quad Logic Level Shifter

De footprints voor de Quad Logic Level Shifters bestaan uit twee 1x6 Dupont-headers rond de Quad Logic Level Shifter. U3 wordt gebruikt voor LCD-I2C en TFT-RST. U8 wordt gebruikt voor TFTSPI SCL, SDA, DC en CS.

| HV-zijde | LV-zijde |
|---|---|
| HV1 | LV1 |
| HV2 | LV2 |
| HV  | LV  |
| GND | GND |
| HV3 | LV3 |
| HV4 | LV4 |

De footprints zijn fysiek aanwezig. Als niveauconversie nodig is, wordt de Quad Logic Level Shifter geplaatst. Als beide zijden dezelfde logicaspanning gebruiken, worden draadbruggen geplaatst. Plaats nooit tegelijk een Quad Logic Level Shifter én draadbruggen op hetzelfde kanaalpaar.

### 6.3 Input-pad: via PCF8574 of via HX1838

Het v2.0.0-schema bevat de PCF8574-uitbreiding en de HX1838 IR Receiver.

De referentie-/testmodule voor de software is de OTRONIC OT8980. De software blijft chipgeoriënteerd en gebruikt `INPUT_TYPE_PCF8574` en `I2C_ADDRESS_PCF8574`; vanaf v2.0.0 gebruikt Input dezelfde interne `ExtenderPCF8574`-codebasis als de gewone PCF8574-extender.

| Onderdeel | Functie |
|---|---|
| U9 | PCF8574-board |
| H12 | P0-P7 en INT van de expander |
| U10 | HX1838 IR Receiver |
| H13 | 1x3 aansluiting voor GND, VCC en DATA van de HX1838 |
| H11 | I2C-aansluiting SCL, SDA, GND en VCC naar de gedeelde I2C-bus |

De PCF8574-route blijft beschikbaar voor fysieke keypads. De HX1838 vormt daarnaast een IR-invoerroute. De softwarekeuze gebeurt via `INPUT_KANAAL_CONFIG`; `KEYPAD_TYPE` beschrijft het aangesloten keypad. De HX1838 DATA-lijn is op het shield verbonden met Arduino D8. Ieder I2C-apparaat op dezelfde bus moet een uniek adres hebben.

## 7. Bestukkingstabellen

### 7.1 Bestukking van U3 voor LCD-I2C en TFT-RST

U3 wordt gedeeld gebruikt. Daarom worden H3 en H4 niet als één vaste “LCD-zijde” en één vaste “Arduino-zijde” beschreven. De functie ligt per kanaal vast.

| Keuze | Wat wordt geplaatst? | Verbinding | Gevolg |
|---|---|---|---|
| Met niveauconversie | Quad Logic Level Shifter U3 | U3 HV3/HV4 lopen naar LCD1 via H2 voor SDA/SCL. U3 HV1 wordt gebruikt aan de Arduino-zijde van TFT RST. | LCD-I2C en TFT-RST kunnen via U3 over verschillende logicaspanningen lopen |
| Zonder niveauconversie | Draadbruggen in de overeenkomstige H3/H4-kanaalparen | De gebruikte kanaalparen voor RST, SDA en SCL worden rechtstreeks doorverbonden. | Alleen gebruiken wanneer beide zijden dezelfde logicaspanning gebruiken |

### 7.2 Bestukking van U8 voor TFTSPI

| Keuze | Wat wordt geplaatst? | Verbinding | Gevolg |
|---|---|---|---|
| Met niveauconversie | Quad Logic Level Shifter U8, plus U3-kanaal HV1/LV1 voor RST | U8 verwerkt TFTSPI SCL, SDA, DC en CS. U3 verwerkt TFT RST. | Alle vijf TFTSPI-stuurlijnen krijgen niveauconversie |
| Zonder niveauconversie | Zeven draadbruggen | Zes draadbruggen van H9 naar H10 voor GND, VCC, SCL, SDA, DC en CS, plus één draadbrug van H3 pin 1 naar H4 pin 1 voor RST. | Alleen gebruiken wanneer Arduino en TFT dezelfde logicaspanning gebruiken |

### 7.3 Bestukking van de HX1838 IR Receiver

De HX1838 is in v2.0.0 als Input-hardware opgenomen.

| Onderdeel | Functie |
|---|---|
| U10 | HX1838 IR Receiver |
| H13 | 1x3 Dupont-connector voor GND, VCC en DATA |
| DATA | Verbonden met Arduino D8 |
| VCC | Verbonden met de VCC-lijn van het shield |
| GND | Verbonden met de gemeenschappelijke GND |

De HX1838 kan als Input-route gebruikt worden zonder de PCF8574-hardware uit het schema te verwijderen.

## 8. ADDR-keuze via SW1 en voedingskeuze via H7

### 8.1 ADDR-keuze via SW1

SW1 bepaalt het I2C-adres van de ADS1115-module op H5. SW1 verbindt de ADDR-pin met één van vier mogelijke signalen.

| SW1-positie | SW1 verbindt ADDR met | I2C-adres | Gebruik |
|---:|---|---:|---|
| 2 | GND (H5 pin 2) | 0x48 | Standaard voor Stimulus |
| 1 | VCC (H5 pin 1) | 0x49 | Tweede ADS1115 op dezelfde I2C-bus, bijvoorbeeld emotiemeting |
| 4 | SDA (H5 pin 4) | 0x4A | Derde ADS1115 op dezelfde I2C-bus |
| 3 | SCL (H5 pin 3) | 0x4B | Vierde ADS1115 op dezelfde I2C-bus |

De posities volgen het v2.0.0-schema (SW1 schakelaar 1 tot 4 naar H5 pin 1 tot 4). Er mag altijd maar één SW1-schakelaar tegelijk ON staan. Voor Stimulus is de standaardkeuze ADDR naar GND (positie 2), dus adres 0x48. Kies je een ander adres, geef dan in `UserConfig.h` `I2C_ADDRESS_EXTENDER_ADS1115_1` dezelfde waarde.

SW1 is alleen relevant wanneer de ADS1115-module op H5 geplaatst wordt. 

### 8.2 Voedingskeuze via H7, de 3-pin jumper 5V/3V3

H7, de 3-pin jumper naast het FSR/ADS1115-pad, kiest de voedingsspanning van dat pad en van het PCF8574-board U9 (via H11). H7 pin 3 is 5V, pin 1 is 3V3, pin 2 is de gevoede rail. H7 is niet de ADS1115-adreskeuze en is ook niet de TFTSPI-displayconnector H8.

| Jumperkeuze | Betekenis |
|---|---|
| 5V | FSR-spanningsdelers en ADS1115-pad werken op 5V |
| 3V3 | FSR-spanningsdelers en ADS1115-pad werken op 3V3 |

Let op: de analoge ingang van de ADS1115 mag nooit boven de voedingsspanning van de ADS1115 komen. Gebruik dus geen 5V-sensoruitgang wanneer de ADS1115 op 3V3 gevoed wordt.

## 9. Waarschuwingen op silkscreen

Silkscreen-label bij de 10-polige connector:

```text
ADS1115: VCC GND SCL SDA ADDR ALRT A0 A1 A2 A3
```

Silkscreen-label bij TFTSPI:

```text
TFTSPI: SCL=SCK SDA=MOSI
U8: SCL SDA CS DC
U3: RST
U8 OR 6 BRIDGES H9-H10
U3 RST OR 1 BRIDGE H3-H4
NEVER BOTH ON THE SAME CONNECTIONS
```

Waarschuwingen:

- SW1: plaats altijd maar één ADDR-schakelaar tegelijk ON. SW1 is uitsluitend voor de ADS1115-adreskeuze.
- H7 voedingskeuze 5V/3V3: plaats één jumper voor de voedingskeuze van het FSR/ADS1115-pad: 5V of 3V3.
- Quad Logic Level Shifter-footprint U3/H3/H4: IC = niveauconversie nodig. Draadbrug = zelfde logicaspanning. Nooit Quad Logic Level Shifter en draadbruggen tegelijk in dezelfde kanaalparen.
- TFTSPI U8/U3: gebruik ofwel de Quad Logic Level Shifters, ofwel zeven draadbruggen wanneer geen niveauconversie nodig is: zes van H9 naar H10 en één van H3 pin 1 naar H4 pin 1. Nooit een Quad Logic Level Shifter en draadbruggen tegelijk op dezelfde verbindingen.
- HX1838: controleer dat U10 via H13 op GND, VCC en DATA aangesloten is en dat DATA naar Arduino D8 loopt.
- Standaardkeuze bij eerste testen: ADS1115 op H5 met SW1-positie 2 (adres 0x48), H7 volgens de logicaspanning van het bord, en draadbruggen voor het CharacterScreen wanneer geen niveauconversie nodig is.

## Deel 3: Pinaansluitingen

## 10. FSR402/RFP602

### 10.1 ADS1115-module op de 10-polige connector H5

Adres 0x48 is gereserveerd voor de Stimulus-ADS1115, zodat er geen conflict ontstaat wanneer de emotiemeet-ADS1115's, bijvoorbeeld 0x49 en 0x4A, later op dezelfde bus bijkomen.

De 10-polige ADS1115-connector heeft deze pinnen:

```text
VCC, GND, SCL, SDA, ADDR, ALRT, A0, A1, A2, A3
```

ADDR wordt via SW1 gekozen. Standaard is ADDR naar GND voor adres 0x48. De 3-pin jumper 5V/3V3 kiest de voedingsspanning voor het FSR/ADS1115-pad. ALRT wordt mee voorzien op H5, ook wanneer deze in de huidige Stimulus-code nog niet gebruikt wordt.

| Connector-pin | Verbonden met | Verbinding richting Arduino | Opmerking |
|---|---|---|---|
| VCC | Voedingsrail gekozen via de 3-pin jumper 5V/3V3 | Arduino 5V of 3V3, volgens jumperkeuze | Voeding ADS1115-module en FSR/ADS1115-pad |
| GND | Gemeenschappelijke massa | Arduino GND | Massa gemeenschappelijk met sensoren en I2C-bus |
| SCL | I2C SCL | Rechtstreeks naar Arduino SCL | Clocklijn I2C, zelfde bus als de PCF8574 |
| SDA | I2C SDA | Rechtstreeks naar Arduino SDA | Datalijn I2C, zelfde bus als de PCF8574 |
| ADDR | SW1 ADDR-keuze | Geen Arduino-pin nodig | Bepaalt I2C-adres; standaard SW1-positie 2, ADDR naar GND = 0x48 |
| ALRT | Voorzien / reserve | Eventueel later naar digitale interruptpin | Niet gebruikt in huidige Stimulus-code |
| A0 | FSR402/RFP602 OUT1 | Arduino leest via I2C | `EP_AIN0`, standaard `RFP602_ADC_PIN_SENSOR_1_SET_1` |
| A1 | FSR402/RFP602 OUT2 | Arduino leest via I2C | `EP_AIN1`, standaard `RFP602_ADC_PIN_SENSOR_2_SET_1` |
| A2 | FSR402/RFP602 OUT3 | Arduino leest via I2C | `EP_AIN2`, standaard `RFP602_ADC_PIN_SENSOR_3_SET_1` |
| A3 | FSR402/RFP602 OUT4 | Arduino leest via I2C | `EP_AIN3`, standaard `RFP602_ADC_PIN_SENSOR_4_SET_1` |

## 11. Character-LCD via I2C

Met LCD xxxx worden de ondersteunde character-LCD-formaten bedoeld die via een I2C-backpack op dezelfde I2C-bus kunnen hangen.

De LCD gebruikt dezelfde I2C-bus als de ADS1115. Het LCD-adres wordt ingesteld via `I2C_ADDRESS_CHARACTER_SCREEN`; het ADS1115-adres via `I2C_ADDRESS_EXTENDER_ADS1115_1` en SW1.

### 11.1 LCD/I2C-pinnen

| LCD/I2C-pin | Verbonden met | Verbinding richting Arduino | Opmerking |
|---|---|---|---|
| VCC | 5V-voedingsrail | Arduino 5V of gedeelde 5V-rail | Voeding LCD-backpack |
| GND | Gemeenschappelijke massa | Arduino GND | Massa gemeenschappelijk met ADS1115 en sensoren |
| SCL | I2C SCL | Arduino SCL via Quad Logic Level Shifter of draadbrug | Dezelfde SCL-bus als ADS1115 |
| SDA | I2C SDA | Arduino SDA via Quad Logic Level Shifter of draadbrug | Dezelfde SDA-bus als ADS1115 |

### 11.2 Ondersteunde LCD-formaten

| Configuratie | Betekenis | Aansluiting |
|---|---|---|
| `SCREEN_LCD1602` | LCD 16 kolommen x 2 regels | Via I2C-backpack op SDA/SCL |
| `SCREEN_LCD1604` | LCD 16 kolommen x 4 regels | Via I2C-backpack op SDA/SCL |
| `SCREEN_LCD2002` | LCD 20 kolommen x 2 regels | Via I2C-backpack op SDA/SCL |
| `SCREEN_LCD2004` | LCD 20 kolommen x 4 regels | Via I2C-backpack op SDA/SCL |
| `SCREEN_LCD4002` | LCD 40 kolommen x 2 regels | Via I2C-backpack op SDA/SCL |

### 11.3 Zonder Quad Logic Level Shifter

Gebruik deze bestukking wanneer de Arduino en de LCD-backpack op dezelfde I2C-logicaspanning werken. De ADS1115 en de PCF8574 zitten altijd rechtstreeks op de Arduino-bus. In dat geval worden de overeenkomstige kanaalparen in de footprint voor de Quad Logic Level Shifter rechtstreeks doorverbonden met draadbruggen.

### 11.4 Met Quad Logic Level Shifter U3

Gebruik deze bestukking wanneer de componentzijde en Arduino-zijde een verschillende logicaspanning hebben. U3 wordt gedeeld gebruikt voor LCD-I2C en TFT-RST. Daarom worden H3 en H4 niet beschreven als één vaste “LCD-zijde” en één vaste “Arduino-zijde”; de functie ligt per kanaal vast. De onderstaande toewijzing is geen voorstel meer, maar de vast bedrade kanaaltoewijzing in het v2.0.0-schema.

| U3-kanaal | Functie in dit schema |
|---|---|
| HV1/LV1 | TFT RST-lijn. HV1 loopt naar de Arduino-zijde van de RST-sturing. |
| HV2/LV2 | Reserve / niet gebruikt |
| HV3/LV3 | LCD SDA-lijn. HV3 gaat naar LCD1 via H2; de overeenkomstige zijde loopt naar de Arduino-I2C-bus. |
| HV4/LV4 | LCD SCL-lijn. HV4 gaat naar LCD1 via H2; de overeenkomstige zijde loopt naar de Arduino-I2C-bus. |
| HV / LV / GND | Voeding en gemeenschappelijke massa van U3 |

Hoewel de kanalen van een Quad Logic Level Shifter elektrisch vergelijkbaar zijn, geldt voor deze print de bovenstaande vaste schema-toewijzing.

### 11.5 LCD-aansluiting en Quad Logic Level Shifter U3

U3 is geen derde aparte keuze, maar een tweede onafhankelijke bestukkingskeuze: shifter plaatsen bij niveauverschil, of draadbruggen plaatsen wanneer beide zijden dezelfde logicaspanning gebruiken.

Voor LCD-I2C worden U3-kanaal HV3/LV3 voor SDA en U3-kanaal HV4/LV4 voor SCL gebruikt. Voor TFTSPI wordt U3-kanaal HV1/LV1 gebruikt voor RST.

## 12. TFTSPI GMT020-02 via H8

H8 is de 1x7 Dupont-connector voor het GMT020-02 / 2.0 inch TFTSPI-display.

| H8-pin | TFT-signaal | Betekenis |
|---|---|---|
| 1 | GND | Massa |
| 2 | VCC | Voeding van het TFT-display |
| 3 | SCL | SPI-clock, Arduino SCK |
| 4 | SDA | SPI-data, Arduino MOSI |
| 5 | RST | Resetlijn van het TFT-display |
| 6 | DC | Data/Command-select |
| 7 | CS | Chip Select |

Bij dit display zijn SDA en SCL geen I2C-signalen. Ze worden gebruikt als SPI-signalen.

| TFT-label | SPI-betekenis | Arduino UNO-lijn |
|---|---|---|
| SCL | SCK / clock | D13 / SCK |
| SDA | MOSI / data out | D11 / MOSI |
| CS | Chip Select | D10 |
| DC | Data/Command | D9 |
| RST | Reset | D7 |

### 12.1 Gebruikte en vrije native pinnen

| Native pin | Gebruik op het shield |
|---|---|
| D7 | TFT RST (via U3 of draadbrug H3-H4 pin 1) |
| D8 | HX1838 DATA |
| D9 | TFT DC (via jumper U11) |
| D10 | TFT CS |
| D11 | TFT SDA (MOSI) |
| D13 | TFT SCL (SCK) |
| SDA, SCL | I2C-bus: ADS1115, PCF8574 en (via U3) CharacterScreen |

Vrij voor andere uitbreidingen: D2 tot D6, D12 en A0 tot A3. Het keypad loopt via de PCF8574 en de sensoren via de ADS1115, dus geen pinconflict met TFTSPI.

### 12.2 Optionele TFTSPI-afhankelijkheid

Voor het GMT020-02 / 2.0 inch TFTSPI-display wordt uitgegaan van een ST7789V-compatibele driverchip met 4-wire SPI.

Gebruik bijvoorbeeld:
- `Adafruit GFX Library`
- `Adafruit ST7735 and ST7789 Library`

Deze bibliotheken worden door de TFTSPI-route gebruikt. Ze staan in `library.properties`, zodat de meegeleverde PixelScreen-voorbeelden na installatie van de library over hun driverafhankelijkheden beschikken. Een build zonder `SCREEN_TYPE_PIXELS` compileert de PixelScreen-code zelf niet mee.

Bij gebruik van een andere TFTSPI-module moet de driverchip opnieuw gecontroleerd worden.

### 12.3 Niveauconversie voor TFTSPI via U8 en U3

Het GMT020-02 TFTSPI-display gebruikt vijf stuurlijnen.

| TFT-signaal | Arduino-lijn | Level-shifting |
|---|---|---|
| SCL | D13 / SCK | via U8 |
| SDA | D11 / MOSI | via U8 |
| CS | D10 | via U8 |
| DC | D9 | via U8 |
| RST | D7 | via U3 |

U8 is de belangrijkste Quad Logic Level Shifter voor de vier TFTSPI-lijnen SCL, SDA, CS en DC. H9 is de TFT-zijde van U8. H10 is de Arduino-zijde van U8. De vijfde TFTSPI-lijn, RST, loopt via kanaal HV1/LV1 van U3.

Wanneer niveauconversie nodig is, worden U8 en U3 als logic shifters geplaatst.

Wanneer geen niveauconversie nodig is, worden geen Quad Logic Level Shifters geplaatst. Plaats dan exact deze zeven draadbruggen:

- van H9 pin 1 naar H10 pin 1 voor GND;
- van H9 pin 2 naar H10 pin 2 voor VCC;
- van H9 pin 3 naar H10 pin 3 voor SCL;
- van H9 pin 4 naar H10 pin 4 voor SDA;
- van H9 pin 5 naar H10 pin 5 voor DC;
- van H9 pin 6 naar H10 pin 6 voor CS;
- van H3 pin 1 naar H4 pin 1 voor RST.

| Keuze | Bestukking | Gevolg |
|---|---|---|
| Met niveauconversie | U8 geplaatst voor SCL, SDA, CS, DC. U3-kanaal HV1/LV1 gebruikt voor RST. | Arduino-logica en TFT-logica mogen verschillend zijn |
| Zonder niveauconversie | Zes draadbruggen van H9 naar H10 en één draadbrug van H3 pin 1 naar H4 pin 1 | Alleen gebruiken wanneer Arduino en TFT dezelfde logicaspanning gebruiken |

Plaats nooit tegelijk een Quad Logic Level Shifter én draadbruggen op dezelfde kanaalparen.

## Deel 4: Huidige software-implementatie

## 13. Configuratie

ADS1115 is in de software een gewone Extender. Er bestaat geen globale ADC-keuze meer (`ADC_BACKEND` is vervallen). De ADS1115-code gebruikt de library **ADS1X15 van Rob Tillaart** (0.6.2).

| Instelling | Standaard | Betekenis |
|---|---|---|
| `EXTENDER_ADS1115_AANTAL` | `1` | Aantal ADS1115-Extenders in de build |
| `I2C_ADDRESS_EXTENDER_ADS1115_1` | `0x48` | Moet overeenkomen met SW1 |
| `EXTENDER_ADS1115_1_ALERT_RDY` | `NONE` | ALRT van H5 wordt niet gebruikt |
| `RFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_1` | `2` | `2` of `4` aangesloten sensoren |
| `RFP602_ADC_PIN_SENSOR_1_SET_1` .. `_4` | `EP_AIN0` .. `EP_AIN3` | Standaardkanalen van de vier sensoren |
| `WACHT_LOSLATEN_DELAY_MS` | `5` | Pauze in de wachtlus, zodat de I2C-bus niet onafgebroken bevraagd wordt |
| `ADC_BITS` / `DELAY_US` | `15` / `0` | ADS1115 single-ended; de ADS1115 wacht zelf op de conversie |

Voor TFTSPI zijn `Adafruit GFX Library` en `Adafruit ST7735 and ST7789 Library` de bedoelde afhankelijkheden voor een ST7789V-compatibel GMT020-02-display.

## 14. ADS1115 en RFP602 in de schets

De `.ino` maakt de ADS1115-Extender aan en geeft die als parent aan RFP602:

```cpp
ExtenderADS1115 extenderADS1115(
  &Native, GedeeldeBusComponent::ADC_ADS1115,
  I2C_ADDRESS_EXTENDER_ADS1115_1,
  HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL,
  EXTENDER_ADS1115_1_ALERT_RDY,
  HardwareResourceToegang::GEDEELD
);

sensorRFP602set1 = GedeeldeBusNewComponent<struct RFP602>(&extenderADS1115, GedeeldeBusComponent::RFP602);
```

Zonder kanaalargumenten gebruikt RFP602 `RFP602_ADC_PIN_SENSOR_1_SET_1` tot `_4`. Per instantie kunnen ook vier eigen kanalen meegegeven worden.

## 15. Lezen en activeren

`RFP602::Activeren()` stelt de gain van de ADS1115 in op `ADS1X15_GAIN_6144MV` (bereik ±6,144 V, passend voor spanningsdelers tot 5V). `RawAnalogRead(sensorPin)` leest via `ExtenderADS1115::readADC()`. Is de parent geen ADS1115, dan meet RFP602 niets en mislukt `Activeren()`.

Ontbreekt de ADS1115 bij het opstarten, dan mislukt de lifecycle (aanmelden, controleren, inpluggen, activeren) en geeft `GedeeldeBusNewComponent` `nullptr` terug. Valt de ADS1115 tijdens de werking weg, dan neemt het runtime-herstel van GedeeldeBus het over (D042).

Let op: de analoge ingang van de ADS1115 mag niet boven de voedingsspanning van de ADS1115 komen. Als de FSR402/RFP602-spanningsdelers op 5V werken, moet de ADS1115 via H7 ook op 5V gevoed worden.

## 16. Mappingtabel

| Sensorlijn | Connector | ADS1115-ingang | Software |
|---|---|---|---|
| FSR402/RFP602 1 | U4 | A0 | `EP_AIN0` = `RFP602_ADC_PIN_SENSOR_1_SET_1` |
| FSR402/RFP602 2 | U5 | A1 | `EP_AIN1` = `RFP602_ADC_PIN_SENSOR_2_SET_1` |
| FSR402/RFP602 3 | U6 | A2 | `EP_AIN2` = `RFP602_ADC_PIN_SENSOR_3_SET_1` |
| FSR402/RFP602 4 | U7 | A3 | `EP_AIN3` = `RFP602_ADC_PIN_SENSOR_4_SET_1` |

## Deel 5: Validatie

## 17. Validatiestap: meetkwaliteit van de ADS1115

Voorstel voor een objectieve controle:

- Zelfde fysieke FSR402/RFP602-sensor, zelfde druk meten via de ADS1115-module op H5.
- Ruis: standaardafwijking bij constante druk.
- Lineariteit over het volledige drukbereik.
- Sample-timing: één ADS1115-meting kost ongeveer 9,4 ms (gemeten op UNO R4 Minima). Voor Stimulus, met trage druksignalen, is dat ruim voldoende.

## 18. Validatiescript

`examples/Systeem/GedeeldeBus/Extenders/Extender_ADS1115_Meetkwaliteit/Extender_ADS1115_Meetkwaliteit.ino` meet 10 s lang de vier ingangen en toont per kanaal gemiddelde, standaardafwijking, minimum en maximum. Het script is bewust zelfstandig gehouden en gebruikt niet de volledige Stimulus-librarylogica. Het dient voor hardwarevalidatie, niet als gewoon gebruikersvoorbeeld.

Fysieke opstelling: ADS1115-bordje op H5, adres via SW1 (standaard positie 2, `0x48`), H7 volgens de werkspanning.

## 19. Quad Logic Level Shifters of draadbruggen

Plaats voor het TFTSPI-pad ofwel de benodigde Quad Logic Level Shifters, ofwel zeven draadbruggen: zes van H9 naar H10 en één van H3 pin 1 naar H4 pin 1. Plaats nooit een Quad Logic Level Shifter en draadbruggen tegelijk op dezelfde verbindingen. De HV-zijde hoort aan de 5 V-Arduinozijde en de LV-zijde aan de 3,3 V-displayzijde.

## 20. Actuele bestandenset v2.0.0

De tekeningset en documentatie voor deze hardwareversie zijn:

- `SCH_Stimulus-Shield-(GroeiAcademie-FrameWork)-v2.0.0_2026-10-22.json`;
- `Schematic_Stimulus-Shield-GroeiAcademie-FrameWork-v2.0.0_2026-10-22.pdf`;
- `Schematic_Stimulus-Shield-(GroeiAcademie-FrameWork)-v2.0.0_2026-10-22.png`;
- `Schematic_Stimulus-Shield-(GroeiAcademie-FrameWork)-v2.0.0_2026-10-22.svg`;
- `Stimulus-Shield-(GroeiAcademie-FrameWork)-v2.0.0.md`;
- `Handleiding-Stimulus-Shield-(GroeiAcademie-FrameWork)-v2.0.0.md`.

Voor bouw, aansluiting en validatie geldt uitsluitend deze v2.0.0-bestandenset. De foutieve voorgangers maken geen deel meer uit van de actuele documentatie.


**Paradisetronic ESP32-S3 UNO (ESP32-S3-WROOM-1) met PixelScreen:** jumper U11 op pin 9 is de enige oplossing die we hiervoor aanhouden tijdens het uploaden. Open U11 tijdens het uploaden en sluit U11 opnieuw na het uploaden.
