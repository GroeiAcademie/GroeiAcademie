
# Stimulus uitbreiding met ADS1115 en TFTSPI

> Status: werkdocument v1.0.0. ADS1115 blijft ondersteund als hardwarematig geteste uitbreiding uit v0.10.0.
> Deze versie documenteert aanvullend de TFTSPI-uitbreiding en de gewijzigde connectorfuncties in het v1.0.0-shieldschema.

> **Gezaghebbende hardwarebron**: `Schematic_GroeiAcademie-Stimulus-Hardware-Shield-v1.0.0_2026-07-29` (shield-PCB). Dit document beschrijft H6 conform die shield-uitvoering: 4x2 jumpers voor directe Arduino-ADC. De ADDR-keuze gebeurt via SW1. De voedingskeuze voor het FSR/ADS1115-pad gebeurt via H7, de 3-pin jumper 5V/3V3. H8 is in dit v1.0.0-schema de 1x7 TFTSPI-displayconnector. Figuur 1 hieronder (het generieke connectorschema uit v0.10.0) is een **historische/conceptuele referentie — niet gebouwd, niet onderhouden**. Het toont de redenering achter de ADS1115-uitbreiding, maar wijkt af van de effectief te bouwen print en mag niet als bouwinstructie gebruikt worden.

**FSR402/RFP602 analoge uitlezing transparant maken voor Arduino ADC of ADS1115, met TFTSPI-uitbreiding**

Figuur 1 (historisch/conceptueel, niet onderhouden). Vroeger Stimulus-schema met vier FSR402/RFP602-lijnen richting Arduino analoge ingangen, LCD 1602 I2C, keypad en Arduino UNO R4 Minima. H6 stond hier nog als 1x4-pins vrouwelijke Dupont-connector — zie de gezaghebbende shield-PCB voor de actuele 4x2-jumpers-uitvoering.

Figuur 1 — historisch/conceptueel schema: niet opgenomen in deze release; gebruik voor bouw en validatie uitsluitend de actuele v1.0.0-tekeningset.

## Deel 1 — Beslissing en uitgangspunt

## 1. Korte conclusie

Voorzien in een extra Stimulus-schema met ADS1115 en uitbreiden van de software zodat de rest van de Stimulus-code niet hoeft te weten waar de analoge waarden vandaan komen.

In de versie zonder ADS1115 gaan de vier FSR402/RFP602-lijnen naar A0-A1 bij twee sensoren of naar A0-A3 bij vier sensoren van de Arduino.

In de versie met ADS1115 gaan diezelfde vier lijnen naar A0-A3 van de ADS1115. De Arduino leest dan via de I2C-bus.

De gekozen hardware-opzet is geen reeks solder-jumpers per sensorlijn. We gaan voor één schema waarin meerdere onafhankelijke bestukkingskeuzes mogelijk zijn, via Dupont-connectoren.

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

De ADDR-adreskeuze gebeurt in het v1.0.0-shieldschema via SW1. Er mag altijd maar één SW1-schakelaar tegelijk ON staan. Voor Stimulus is de standaard ADDR naar GND, dus adres 0x48.

Zonder I2C-multiplexer kan je maximaal vier ADS1115-modules op dezelfde I2C-bus zetten. Dat geeft maximaal 4 modules x 4 single-ended kanalen = 16 analoge ingangen.

## 3. Waarom een apart schema met ADS1115 zinvol is

| Variant | Aansluiting sensorlijnen | Waarom |
|---|---|---|
| Zonder ADS1115 | FSR402/RFP602 naar Arduino A0-A3 | Eenvoudig, snel testbaar, minder onderdelen |
| Met ADS1115 | FSR402/RFP602 naar ADS1115 A0-A3 | Arduino analoge pinnen blijven vrijer, hogere ADC-resolutie, uitbreidbaar |
| Beide tegelijk | Niet doen voor dezelfde sensorlijnen | Voorkomt dubbele belasting, verwarring en meetfouten |

We gaan voor één schema waarin meerdere onafhankelijke bestukkingskeuzes mogelijk zijn, via Dupont-connectoren.

De keuze voor het ADC-pad gebeurt door ofwel de ADS1115-module op H5 te plaatsen, ofwel geen ADS1115-module op H5 te plaatsen en de vier jumpers op jumperveld H6 te plaatsen. H6 verbindt dan H5 pin 7 tot en met pin 10 met Arduino A0 tot en met A3.

De keuze voor de I2C-niveauconversie gebeurt apart: ofwel wordt de Quad Logic Level Shifter geplaatst, ofwel worden draadbruggen gebruikt wanneer niveauconversie niet nodig is.

## 4. Bestukkingskeuze in het schema

| Bestukking | Verbinding | Codegevolg |
|---|---|---|
| ADS1115 niet bestukt | FSR402/RFP602 OUT1-OUT4 naar Arduino A0-A3 | Arduino leest rechtstreeks met `analogRead()` |
| ADS1115 bestukt | FSR402/RFP602 OUT1-OUT4 naar ADS1115 A0-A3 | Arduino leest via de I2C-bus |
| Keuze via connector/jumpers | Ofwel de ADS1115-module op H5, ofwel geen ADS1115-module op H5 en vier jumpers op jumperveld H6. H6 verbindt H5 pin 7-10 met Arduino A0-A3. | Flexibel, geen solder-jumpers per sensorlijn nodig |

## 5. Hardware-aandachtspunten

| Punt | Opmerking |
|---|---|
| Voeding ADS1115 | Als de FSR402/RFP602-spanningsdelers op 5V staan, moet de ADS1115-opstelling 5V-ingangen veilig aankunnen. De analoge ingang mag niet boven de voedingsspanning van de ADS1115 komen. |
| I2C-bus | LCD en ADS1115 mogen samen op SDA/SCL, zolang adressen niet botsen en pull-ups niet te zwaar worden. |
| Quad Logic Level Shifter | In het huidige schema staat al een quad Quad Logic Level Shifter voor de I2C-bus. Bepaal per buszijde of de ADS1115 op 5V of 3V3 zit. |
| GND | Arduino, ADS1115, sensor-spanningsdelers en eventuele externe voeding moeten een gemeenschappelijke GND hebben. |
| Resolutie | ADS1115 geeft meer meetresolutie dan de gewone Arduino ADC. Dat is nuttig bij subtiele drukverschillen. |
| Sampling | ADS1115 is niet bedoeld voor extreem snelle signalen, wel ruim voldoende voor trage stimulus/drukveranderingen. |

De ADS1115 kan op dezelfde I2C-bus als het LCD. Wanneer de ADS1115 en de LCD beide op 5V werken, is voor de ADS1115 geen extra Quad Logic Level Shifter nodig. Als één onderdeel op 3V3 werkt, moet duidelijk vastliggen aan welke zijde van de Quad Logic Level Shifter de ADS1115 wordt aangesloten.

De footprints voor de Quad Logic Level Shifters mogen twee functies krijgen: ofwel wordt de quad Quad Logic Level Shifter geplaatst wanneer niveauconversie nodig is, ofwel worden de overeenkomstige kanaalparen rechtstreeks met draadbruggen verbonden wanneer beide zijden dezelfde logicaspanning gebruiken. GND blijft gemeenschappelijk. U3 wordt gedeeld gebruikt voor LCD-I2C en TFT-RST. U8 wordt gebruikt voor TFTSPI SCL, SDA, DC en CS.

## Deel 2 — Eén schema, meerdere onafhankelijke bestukkingskeuzes

In dit deel worden de connectorreferenties uit het schema expliciet gebruikt. Zo blijft de Markdown rechtstreeks bruikbaar naast het schema. In v1.0.0 zijn er drie onafhankelijke keuzes: het ADC-pad, de I2C-niveauconversie en de TFTSPI-niveauconversie.

| Referentie | Functie in het schema |
|---|---|
| H1 | 1x5 Dupont-connector van de keypadlijnen naar U2 |
| H2 | 1x4 Dupont-connector van LCD1 naar de LCD-I2C-bus |
| H3 | 1x6 Dupont-connector aan één zijde van U3. Bevat GND, VCC, RST, SDA en SCL. U3 wordt kanaal per kanaal gebruikt; H3/H4 zijn dus niet zuiver “LCD-zijde” versus “Arduino-zijde”. |
| H4 | 1x6 Dupont-connector aan de andere zijde van U3. Bevat de overeenkomstige GND-, VCC-, RST-, SDA- en SCL-lijnen via U3 of draadbruggen. |
| H5 | 1x10 Dupont-connector voor de ADS1115-module en de vier analoge sensorlijnen |
| H6 | Afzonderlijk 4x2-jumperveld. Met vier geplaatste jumpers verbindt H6 H5 pin 7-10 met Arduino A0-A3 voor directe Arduino-ADC wanneer geen ADS1115-module op H5 geplaatst is. |
| H7 | 3-pin jumper voor voedingskeuze van het FSR/ADS1115-pad: 5V of 3V3 |
| H8 | 1x7 Dupont-connector voor GMT020-02 / 2.0 inch TFTSPI-display |
| H9 | 1x6 Dupont-connector aan de TFT-zijde van de TFTSPI footprint voor de Quad Logic Level Shifter U8 |
| H10 | 1x6 Dupont-connector aan de Arduino-zijde van de TFTSPI footprint voor de Quad Logic Level Shifter U8 |
| SW1 | 4-kanaals DIP-switch voor ADS1115 ADDR-keuze |
| U1 | Arduino UNO R3/R4 |
| U2 | 1x4 keymatrix |
| U3 | Quad Quad Logic Level Shifter voor LCD-I2C en TFT-RST |
| U8 | Quad Quad Logic Level Shifter voor TFTSPI SCL, SDA, DC en CS |

## 6. Bestukkingsopties per footprint

### 6.1 Directe Arduino-ADC of ADS1115

H5 is de centrale 10-polige Dupont-connector voor het ADS1115-pad en de vier analoge sensorlijnen. H6 is een afzonderlijk 4x2-jumperveld. Met vier geplaatste jumpers verbindt H6 H5 pin 7 tot en met pin 10 met Arduino A0 tot en met A3. H7, de 3-pin jumper 5V/3V3, is in v1.0.0 uitsluitend de voedingskeuze voor het FSR/ADS1115-pad. SW1 is uitsluitend de ADDR-keuze voor de ADS1115-adreskeuze. H8 is de TFTSPI-displayconnector.

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

Bij de keuze voor directe Arduino-ADC wordt geen ADS1115-module op H5 geplaatst. In plaats daarvan worden vier jumpers op jumperveld H6 geplaatst. H6 verbindt dan H5 pin 7 tot en met pin 10 met Arduino A0 tot en met A3. De 3-pin jumper 5V/3V3 kiest alleen de voeding van het FSR/ADS1115-pad en verandert niets aan de keuze tussen ADS1115 en directe Arduino-ADC.

| H6-jumper op H5 pin 7-10 | Sensorlijn op H5 | Arduino-lijn via H6 | Betekenis |
|---|---|---|---|
| H5 pin 7 (H6 pin 1) | ADS1115 A0-lijn | Arduino A0 | OUT1 naar Arduino A0 bij directe ADC |
| H5 pin 8 (H6 pin 2) | ADS1115 A1-lijn | Arduino A1 | OUT2 naar Arduino A1 bij directe ADC |
| H5 pin 9 (H6 pin 3) | ADS1115 A2-lijn | Arduino A2 | OUT3 naar Arduino A2 bij directe ADC |
| H5 pin 10 (H6 pin 4) | ADS1115 A3-lijn | Arduino A3 | OUT4 naar Arduino A3 bij directe ADC |

Plaats nooit tegelijk de ADS1115-module op H5 én de vier jumpers op jumperveld H6. Anders kunnen dezelfde sensorlijnen tegelijk aan ADS1115 A0-A3 en Arduino A0-A3 hangen.

### 6.2 Met of zonder Quad Logic Level Shifter

De footprints voor de Quad Logic Level Shifters bestaan uit twee 1x6 Dupont-headers rond de quad Quad Logic Level Shifter. U3 wordt gebruikt voor LCD-I2C en TFT-RST. U8 wordt gebruikt voor TFTSPI SCL, SDA, DC en CS.

| HV-zijde | LV-zijde |
|---|---|
| HV1 | LV1 |
| HV2 | LV2 |
| HV | LV |
| GND | GND |
| HV3 | LV3 |
| HV4 | LV4 |

De footprints zijn fysiek aanwezig. Als niveauconversie nodig is, wordt de quad Quad Logic Level Shifter geplaatst. Als beide zijden dezelfde logicaspanning gebruiken, worden draadbruggen geplaatst. Plaats nooit tegelijk een Quad Logic Level Shifter én draadbruggen op hetzelfde kanaalpaar.

## 7. Bestukkingstabellen

### 7.1 Keuze tussen directe Arduino-ADC en ADS1115

| ADC-pad | Wat wordt geplaatst? | Wat wordt gebruikt? | Codekeuze |
|---|---|---|---|
| ADS1115-pad | ADS1115-module op H5 | H5 gebruikt alle 10 pinnen: VCC, GND, SCL, SDA, ADDR, ALRT, A0, A1, A2, A3. Het adres wordt gekozen via SW1. | `ADC_BACKEND_ADS1115` |
| Direct Arduino-ADC-pad | Geen ADS1115-module op H5. De vier H6-jumpers geplaatst op H5 pin 7-10. | H5 pin 7/H6 pin 1 naar Arduino A0, H5 pin 8/H6 pin 2 naar Arduino A1, H5 pin 9/H6 pin 3 naar Arduino A2, H5 pin 10/H6 pin 4 naar Arduino A3. | `ADC_BACKEND_NATIVE` |

### 7.2 Bestukking van U3 voor LCD-I2C en TFT-RST

U3 wordt gedeeld gebruikt. Daarom worden H3 en H4 niet als één vaste “LCD-zijde” en één vaste “Arduino-zijde” beschreven. De functie ligt per kanaal vast.

| Keuze | Wat wordt geplaatst? | Verbinding | Gevolg |
|---|---|---|---|
| Met niveauconversie | Quad Quad Logic Level Shifter U3 | U3 HV3/HV4 lopen naar LCD1 via H2 voor SDA/SCL. U3 HV1 wordt gebruikt aan de Arduino-zijde van TFT RST. | LCD-I2C en TFT-RST kunnen via U3 over verschillende logicaspanningen lopen |
| Zonder niveauconversie | Draadbruggen in de overeenkomstige H3/H4-kanaalparen | De gebruikte kanaalparen voor RST, SDA en SCL worden rechtstreeks doorverbonden. | Alleen gebruiken wanneer beide zijden dezelfde logicaspanning gebruiken |

### 7.3 Bestukking van U8 voor TFTSPI

| Keuze | Wat wordt geplaatst? | Verbinding | Gevolg |
|---|---|---|---|
| Met niveauconversie | Quad Quad Logic Level Shifter U8, plus U3-kanaal HV1/LV1 voor RST | U8 verwerkt TFTSPI SCL, SDA, DC en CS. U3 verwerkt TFT RST. | Alle vijf TFTSPI-stuurlijnen krijgen niveauconversie |
| Zonder niveauconversie | Zeven draadbruggen | Zes draadbruggen van H9 naar H10 voor GND, VCC, SCL, SDA, DC en CS, plus één draadbrug van H3 pin 1 naar H4 pin 1 voor RST. | Alleen gebruiken wanneer Arduino en TFT dezelfde logicaspanning gebruiken |

## 8. ADDR-keuze via SW1 en voedingskeuze via H7

### 8.1 ADDR-keuze via SW1

SW1 bepaalt het I2C-adres van de ADS1115-module op H5. SW1 verbindt de ADDR-pin met één van vier mogelijke signalen.

| SW1 verbindt ADDR met | I2C-adres | Gebruik |
|---|---:|---|
| GND | 0x48 | Standaard voor Stimulus |
| VCC | 0x49 | Tweede ADS1115 op dezelfde I2C-bus, bijvoorbeeld emotiemeting |
| SDA | 0x4A | Derde ADS1115 op dezelfde I2C-bus |
| SCL | 0x4B | Vierde ADS1115 op dezelfde I2C-bus |

Er mag altijd maar één SW1-schakelaar tegelijk ON staan. Voor Stimulus is de standaardkeuze ADDR naar GND, dus adres 0x48.

SW1 is alleen relevant wanneer de ADS1115-module op H5 geplaatst wordt. SW1 is niet bedoeld om sensorlijnen met Arduino A0-A3 te verbinden. De vier jumpers op jumperveld H6 horen uitsluitend bij de keuze voor directe Arduino-ADC.

### 8.2 Voedingskeuze via H7, de 3-pin jumper 5V/3V3

H7, de 3-pin jumper naast het FSR/ADS1115-pad, kiest de voedingsspanning van dat pad. H7 is niet de ADS1115-adreskeuze en is ook niet de TFTSPI-displayconnector H8.

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

Silkscreen-label bij het H6-jumperveld:

```text
H6 ARD-ADC: A0 A1 A2 A3
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

- H5/H6: plaats ofwel de ADS1115-module op H5, ofwel geen ADS1115-module op H5 en vier jumpers op jumperveld H6 voor directe Arduino-ADC. Nooit beide tegelijk.
- SW1: plaats altijd maar één ADDR-schakelaar tegelijk ON. SW1 is uitsluitend voor de ADS1115-adreskeuze.
- H7 voedingskeuze 5V/3V3: plaats één jumper voor de voedingskeuze van het FSR/ADS1115-pad: 5V of 3V3.
- Quad Logic Level Shifter-footprint U3/H3/H4: IC = niveauconversie nodig. Draadbrug = zelfde logicaspanning. Nooit Quad Logic Level Shifter en draadbruggen tegelijk in dezelfde kanaalparen.
- TFTSPI U8/U3: gebruik ofwel de Quad Logic Level Shifters, ofwel zeven draadbruggen wanneer geen niveauconversie nodig is: zes van H9 naar H10 en één van H3 pin 1 naar H4 pin 1. Nooit een Quad Logic Level Shifter en draadbruggen tegelijk op dezelfde verbindingen.
- Standaardkeuze bij eerste testen: directe Arduino-ADC met vier jumpers op jumperveld H6, waardoor H5 pin 7-10 met Arduino A0-A3 verbonden worden, en draadbruggen voor de I2C-bus wanneer geen niveauconversie nodig is.

## Deel 3 — Pinaansluitingen

## 10. FSR402/RFP602

### 10.1 Directe Arduino-ADC via jumperveld H6

Bij de directe Arduino-ADC-variant wordt geen ADS1115-module op H5 geplaatst. In plaats daarvan worden vier jumpers op het afzonderlijke jumperveld H6 geplaatst. H6 verbindt dan H5 pin 7 tot en met pin 10 met Arduino A0 tot en met A3.

| H6-jumper | Sensorlijn op H5 | Verbonden met Arduino via H6 | Opmerking |
|---|---|---|---|
| H5 pin 7 (H6 pin 1) | ADS1115 A0-lijn / OUT1 | Arduino A0 | Directe Arduino-ADC-meting |
| H5 pin 8 (H6 pin 2) | ADS1115 A1-lijn / OUT2 | Arduino A1 | Directe Arduino-ADC-meting |
| H5 pin 9 (H6 pin 3) | ADS1115 A2-lijn / OUT3 | Arduino A2 | Directe Arduino-ADC-meting |
| H5 pin 10 (H6 pin 4) | ADS1115 A3-lijn / OUT4 | Arduino A3 | Directe Arduino-ADC-meting |

Controleer in het schema en op de silkscreen dat de vier posities van H6 respectievelijk H5 pin 7 met Arduino A0, H5 pin 8 met Arduino A1, H5 pin 9 met Arduino A2 en H5 pin 10 met Arduino A3 verbinden. SW1 hoort hier niet bij; SW1 blijft uitsluitend voor de ADDR-adreskeuze. H7 hoort hier ook niet bij; H7 is uitsluitend de voedingskeuze voor het FSR/ADS1115-pad. H8 is de TFTSPI-displayconnector.

### 10.2 ADS1115-module op de 10-polige connector H5

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
| SCL | I2C SCL | Arduino SCL via Quad Logic Level Shifter of draadbrug | Clocklijn I2C |
| SDA | I2C SDA | Arduino SDA via Quad Logic Level Shifter of draadbrug | Datalijn I2C |
| ADDR | SW1 ADDR-keuze | Geen Arduino-pin nodig | Bepaalt I2C-adres; standaard ADDR naar GND = 0x48 |
| ALRT | Voorzien / reserve | Eventueel later naar digitale interruptpin | Niet gebruikt in huidige Stimulus-code |
| A0 | FSR402/RFP602 OUT1 | Arduino leest via I2C | ADS1115 kanaal 0 |
| A1 | FSR402/RFP602 OUT2 | Arduino leest via I2C | ADS1115 kanaal 1 |
| A2 | FSR402/RFP602 OUT3 | Arduino leest via I2C | ADS1115 kanaal 2 |
| A3 | FSR402/RFP602 OUT4 | Arduino leest via I2C | ADS1115 kanaal 3 |

## 11. Character-LCD via I2C

Met LCD xxxx worden de ondersteunde character-LCD-formaten bedoeld die via een I2C-backpack op dezelfde I2C-bus kunnen hangen.

De LCD gebruikt dezelfde I2C-bus als de ADS1115. Het LCD-adres blijft apart ingesteld via `I2C_ADRES`; het ADS1115-adres via `ADS1115_I2C_ADDRESS` en SW1.

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

Gebruik deze bestukking wanneer Arduino, LCD-backpack en ADS1115 op dezelfde I2C-logicaspanning werken. In dat geval worden de overeenkomstige kanaalparen in de footprint voor de Quad Logic Level Shifter rechtstreeks doorverbonden met draadbruggen.

### 11.4 Met Quad Logic Level Shifter U3

Gebruik deze bestukking wanneer de componentzijde en Arduino-zijde een verschillende logicaspanning hebben. U3 wordt gedeeld gebruikt voor LCD-I2C en TFT-RST. Daarom worden H3 en H4 niet beschreven als één vaste “LCD-zijde” en één vaste “Arduino-zijde”; de functie ligt per kanaal vast. De onderstaande toewijzing is geen voorstel meer, maar de vast bedrade kanaaltoewijzing in het v1.0.0-schema.

| U3-kanaal | Functie in dit schema |
|---|---|
| HV1/LV1 | TFT RST-lijn. HV1 loopt naar de Arduino-zijde van de RST-sturing. |
| HV2/LV2 | Reserve / niet gebruikt |
| HV3/LV3 | LCD SDA-lijn. HV3 gaat naar LCD1 via H2; de overeenkomstige zijde loopt naar de Arduino-I2C-bus. |
| HV4/LV4 | LCD SCL-lijn. HV4 gaat naar LCD1 via H2; de overeenkomstige zijde loopt naar de Arduino-I2C-bus. |
| HV / LV / GND | Voeding en gemeenschappelijke massa van U3 |

Hoewel de kanalen van een quad Quad Logic Level Shifter elektrisch vergelijkbaar zijn, geldt voor deze print de bovenstaande vaste schema-toewijzing.

### 11.5 LCD-aansluiting en Quad Logic Level Shifter U3

U3 is geen derde aparte keuze, maar een tweede onafhankelijke bestukkingskeuze: shifter plaatsen bij niveauverschil, of draadbruggen plaatsen wanneer beide zijden dezelfde logicaspanning gebruiken.

Voor LCD-I2C worden U3-kanaal HV3/LV3 voor SDA en U3-kanaal HV4/LV4 voor SCL gebruikt. Voor TFTSPI wordt U3-kanaal HV1/LV1 gebruikt voor RST.

De 10-polige ADS1115-connector H5 en het H6-jumperveld worden duidelijk naast elkaar of logisch in elkaars buurt geplaatst, zodat onmiddellijk zichtbaar is welke keuze actief is.

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
| RST | Reset | D8 |

### 12.1 Pinconflictcontrole TFTSPI en keymatrix

De keymatrix via H1 gebruikt Arduino D2, D3, D4 en D5, met GND als gemeenschappelijke lijn naar U2.

Het TFTSPI-display via H8 gebruikt Arduino D8, D9, D10, D11 en D13.

Er is daardoor geen pinconflict tussen de keymatrix en TFTSPI.

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
| RST | D8 | via U3 |

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

## Deel 4 — Huidige software-implementatie

## 13. Backend-define

De huidige code gebruikt niet langer `STIMULUS_USE_ADS1115` of `STIMULUS_ADC_BACKEND_*`. De actuele configuratie gebruikt `ADC_BACKEND_NATIVE` en `ADC_BACKEND_ADS1115` in `SystemConfig.h`.

Praktische consequentie: bij `ADC_BACKEND_NATIVE` is Adafruit ADS1X15 niet nodig. Bij `ADC_BACKEND_ADS1115` moet de Adafruit ADS1X15-library geïnstalleerd zijn.

Voor TFTSPI zijn de grafische bibliotheken alleen nodig wanneer de TFTSPI-route effectief gebruikt wordt. In dat geval zijn `Adafruit GFX Library` en `Adafruit ST7735 and ST7789 Library` de bedoelde optionele afhankelijkheden voor een ST7789V-compatibel GMT020-02-display.

```cpp
// ============================================================================
// ADC BACKEND (naast BOARD_VERSION)
// ============================================================================
#define ADC_BACKEND_NATIVE 0 // ingebouwde ADC van de Arduino
#define ADC_BACKEND_ADS1115 1 // externe 16-bit ADC via I2C
#define ADC_BACKEND ADC_BACKEND_NATIVE // wissel dit om van backend te wisselen
#if ADC_BACKEND != ADC_BACKEND_NATIVE && ADC_BACKEND != ADC_BACKEND_ADS1115
  #error Selecteer een geldige ADC_BACKEND.
#endif
#define ADS1115_I2C_ADDRESS 0x48 // gereserveerd adres voor Stimulus

// Instelbare vertraging in de busy-wait loop van WachtTotAlleSensorsLosgelatenVoorTest().
// Bij ADC_BACKEND_NATIVE blijft dit 0. Bij ADC_BACKEND_ADS1115 voorkomt dit dat de
// I2C-bus zonder onderbreking bevraagd wordt.
#if ADC_BACKEND == ADC_BACKEND_ADS1115
  #define WACHT_LOSLATEN_DELAY_MS 5UL
#else
  #define WACHT_LOSLATEN_DELAY_MS 0UL
#endif

// Definieer de pinnen voor de sensoren op jouw sensorboard
#if ADC_BACKEND == ADC_BACKEND_ADS1115
  #define PIN_SENSOR_1 0 // ADS1115-kanaal 0
  #define PIN_SENSOR_2 1 // ADS1115-kanaal 1
  #define PIN_SENSOR_3 2 // ADS1115-kanaal 2
  #define PIN_SENSOR_4 3 // ADS1115-kanaal 3
#else
  #define PIN_SENSOR_1 A0 // Analoge pin voor de 1ste test-sensor
  #define PIN_SENSOR_2 A1 // Analoge pin voor de 2de test-sensor
  #define PIN_SENSOR_3 A2 // Analoge pin voor de 3de test-sensor
  #define PIN_SENSOR_4 A3 // Analoge pin voor de 4de test-sensor
#endif

#define BOARD_UNO_R3        0
#define BOARD_UNO_R4_MINIMA 1
#define BOARD_UNO_R4_WIFI   2
#define BOARD_ESP32_UNO     3

#define BOARD_VERSION BOARD_UNO_R3 // wissel dit om van bord te wisselen
#if BOARD_VERSION != BOARD_UNO_R3 && BOARD_VERSION != BOARD_UNO_R4_MINIMA && BOARD_VERSION != BOARD_UNO_R4_WIFI && BOARD_VERSION != BOARD_ESP32_UNO
  #error Selecteer een geldige BOARD_VERSION.
#endif

#if ADC_BACKEND == ADC_BACKEND_ADS1115
  #define ADC_BITS 15 // ADS1115 single-ended: 0-32767 bruikbaar bereik
  #define DELAY_US 0 // niet van toepassing; I2C-conversietijd zit in de leesfunctie zelf
#elif BOARD_VERSION == BOARD_UNO_R3
  #define ADC_BITS 10 // R3 = 10
  #define DELAY_US 100 // Vertraging tussen samples in de meetlussen
#elif (BOARD_VERSION == BOARD_UNO_R4_MINIMA || BOARD_VERSION == BOARD_UNO_R4_WIFI)
  #define ADC_BITS 14 // R4 = 10 of 14
  #define DELAY_US 200 // Vertraging tussen samples in de meetlussen
#elif BOARD_VERSION == BOARD_ESP32_UNO
  #define ADC_BITS 12 // zie kanttekening in docs/Configuratie/SystemConfig.md
  #define DELAY_US 0 // zie kanttekening in docs/Configuratie/SystemConfig.md
#else
  #error Selecteer een geldige BOARD_VERSION.
#endif

#define ADC_MAX ((1UL << ADC_BITS) - 1)

#if ADC_BITS == 10
  #define ADC(x) (x)
#elif ADC_BITS == 12 || ADC_BITS == 14 || ADC_BITS == 15
  #define ADC(x) (((x) * ADC_MAX) / 1023L)
#else
  #error ADC_BITS moet 10, 12, 14 of 15 zijn.
#endif
```

## 14. Transparante uitleesfunctie

De rest van de Stimulus-code hoeft niet rechtstreeks te weten of de waarde via Arduino ADC of via ADS1115 gelezen wordt. De huidige transparante laag bestaat uit `RawAnalogRead(int sensorPin)`, `sensorPin[4]` en `AnalogReadMetGekorigeerdeOffsets(...)`.

```cpp
#if ADC_BACKEND == ADC_BACKEND_ADS1115
  #include <Adafruit_ADS1X15.h>
  static Adafruit_ADS1115 ads;
  static bool ads1115Aanwezig = false;

  int RawAnalogRead(int sensorPin) {
    if (!ads1115Aanwezig) return 0;
    return ads.readADC_SingleEnded(sensorPin);
  }
#else
  int RawAnalogRead(int sensorPin) {
    return analogRead(sensorPin);
  }
#endif

const int sensorPin[4] = { PIN_SENSOR_1, PIN_SENSOR_2, PIN_SENSOR_3, PIN_SENSOR_4 };

int AnalogReadMetGekorigeerdeOffsets(int sensorPin, int offsetSensor) {
  int waarde = RawAnalogRead(sensorPin) - offsetSensor;
  if (waarde < 0) waarde = 0;
  return waarde;
}
```

Bij `ADC_BACKEND_NATIVE` bevat `sensorPin[]` Arduino-pinnen A0-A3. Bij `ADC_BACKEND_ADS1115` bevat `sensorPin[]` ADS1115-kanaalnummers 0-3. Daardoor kan de rest van de code dezelfde uitleeslaag blijven gebruiken.

## 15. Initialisatie bij ADS1115

Bij ADS1115 controleert de code bij het opstarten of de module aanwezig is. Bij niet aanwezig: foutmelding op het scherm, geen `while(1)`-blokkade, en terug naar de gewone flow.

```cpp
void InitialiseerADS1115() {
  if (!ads.begin(ADS1115_I2C_ADDRESS)) {
    ads1115Aanwezig = false;
    PrintToScreen(_LCD_ADS1115_FOUT, _LCD_ADS1115_NIET_GEVONDEN, _LCD_LEESTIJD_FEEDBACK_MS);
    return;
  }

  ads.setGain(GAIN_TWOTHIRDS);
  ads1115Aanwezig = true;
}
```

In `Stimulus.h` staat het prototype:

```cpp
void InitialiseerADS1115();
```

In `setup()` van de voorbeelden gebeurt de initialisatie vóór `BepaalSensorOffsets()`:

```cpp
#if ADC_BACKEND == ADC_BACKEND_ADS1115
  InitialiseerADS1115();
#endif
```

Er staat in deze ADS1115-initialisatie geen `while(1)`. De fout wordt getoond, daarna keert `InitialiseerADS1115()` terug zodat de gewone `loop()` verder kan lopen.

`GAIN_TWOTHIRDS` is logisch wanneer je single-ended spanningen tot ongeveer 5V wil meten. De exacte gain kies je definitief op basis van de hoogste sensoruitgang in het schema.

Let op: de analoge ingang van de ADS1115 mag niet boven de voedingsspanning van de ADS1115 komen. Als de FSR402/RFP602-spanningsdelers op 5V werken, moet de ADS1115 dus ook passend gevoed en aangesloten worden.

## 16. Mappingtabel voor de huidige code

| Sensorlijn | Zonder ADS1115 | Met ADS1115 | Huidige code |
|---|---|---|---|
| FSR402/RFP602 1 | Arduino A0 | ADS1115 A0 | `sensorPin[0]` naar `RawAnalogRead(sensorPin[0])` |
| FSR402/RFP602 2 | Arduino A1 | ADS1115 A1 | `sensorPin[1]` naar `RawAnalogRead(sensorPin[1])` |
| FSR402/RFP602 3 | Arduino A2 | ADS1115 A2 | `sensorPin[2]` naar `RawAnalogRead(sensorPin[2])` |
| FSR402/RFP602 4 | Arduino A3 | ADS1115 A3 | `sensorPin[3]` naar `RawAnalogRead(sensorPin[3])` |

## 17. Aanbevolen beslissing voor nu

Ik stel voor om nu vier dingen vast te leggen:

| Onderdeel | Naam | Inhoud |
|---|---|---|
| Schema 1 | Stimulus direct ADC | FSR402/RFP602 naar Arduino A0-A3 |
| Schema 2 | Stimulus ADS1115 | FSR402/RFP602 naar ADS1115 A0-A3, ADS1115 via de I2C-bus |
| Schema 3 | Stimulus keuze via connector/jumpers | ADS1115-module op H5 met adreskeuze via SW1, of geen ADS1115-module op H5 en vier jumpers op jumperveld H6 voor de verbinding van H5 pin 7-10 met Arduino A0-A3 |
| Schema 4 | Stimulus TFTSPI | GMT020-02 / 2.0 inch TFTSPI via H8. SCL, SDA, CS en DC via U8 en RST via U3, of zeven draadbruggen wanneer geen niveauconversie nodig is: zes van H9 naar H10 en één van H3 pin 1 naar H4 pin 1. |
| Code | Transparante uitleeslaag | `RawAnalogRead(int sensorPin)` met `ADC_BACKEND`-define, gebruikt door `AnalogReadMetGekorigeerdeOffsets(...)` |

Daarmee blijft de huidige code bruikbaar, kan je later eenvoudig naar ADS1115 omschakelen, en voorkom je pinproblemen wanneer er extra componenten voor emoties bijkomen.

## Deel 5 — Validatie

## 18. Validatiestap: wanneer is ADS1115 betrouwbaar?

Voorstel voor een objectieve vergelijking, uitvoerbaar met schema 3 zonder herbedrading:

- Zelfde fysieke FSR402/RFP602-sensor, zelfde druk, eerst meten via directe Arduino-ADC met de H6-jumpers geplaatst op H5 pin 7-10 naar Arduino A0-A3, daarna meten via de ADS1115-module op H5 met adreskeuze via SW1.
- Vergelijk ruis: standaardafwijking bij constante druk.
- Vergelijk lineariteit over het volledige drukbereik.
- Controleer sample-timing: ADS1115 via I2C kost meer tijd per lezing dan `analogRead()`. Voor Stimulus, met trage druksignalen, is dat doorgaans geen probleem, maar dit moet wel gemeten worden.
- Pas na deze vergelijking wordt bepaald of schema 2, ADS1115, de standaardkeuze wordt, of dat schema 1, directe ADC, volstaat en de ADS1115 optioneel blijft.

## 19. Validatiescripts

### Status van de validatiescripts

De validatiescripts zijn bewust zelfstandig gehouden. Ze gebruiken niet de volledige Stimulus-librarylogica, zodat de Arduino-ADC-route en ADS1115-route zuiver hardwarematig vergeleken kunnen worden.

Gebruik:
- `ADC_Validatie_Native` voor de directe Arduino-ADC-route.
- `ADC_Validatie_ADS1115` voor de ADS1115-route.

Deze scripts zijn bedoeld voor hardwarevalidatie, niet als gewone gebruikersvoorbeelden van de Stimulus-library.

Er zijn twee aparte validatiescripts, zodat de Arduino-ADC-route en de ADS1115-route elk afzonderlijk getest kunnen worden zonder telkens dezelfde sketch handmatig om te zetten.

| Script | Backend | Fysieke keuze |
|---|---|---|
| `examples/Systeem/ADC_Validatie/ADC_Validatie_Native/ADC_Validatie_Native.ino` | `ADC_BACKEND_NATIVE` | Geen ADS1115-module op H5. Vier jumpers op jumperveld H6. H6 verbindt H5 pin 7-10 met Arduino A0-A3. |
| `examples/Systeem/ADC_Validatie/ADC_Validatie_ADS1115/ADC_Validatie_ADS1115.ino` | `ADC_BACKEND_ADS1115` | ADS1115-bordje op H5. Gewenst adres kiezen via SW1. Standaard: ADDR naar GND, adres `0x48`. |

In beide validatiescripts wordt dezelfde functiehandtekening gebruikt als in de Stimulus-code:

```cpp
int RawAnalogRead(int sensorPin)
```

Bij `ADC_BACKEND_NATIVE` is `sensorPin` een Arduino-pin. Bij `ADC_BACKEND_ADS1115` is `sensorPin` een ADS1115-kanaalnummer.

### 19.1 Arduino-ADC-validatie

Gebruik `ADC_Validatie_Native.ino` om de directe Arduino-ADC-route te testen.

```cpp
// Zie examples/Systeem/ADC_Validatie/ADC_Validatie_Native/ADC_Validatie_Native.ino
#define ADC_BACKEND ADC_BACKEND_NATIVE
```

### 19.2 ADS1115-validatie

Gebruik `ADC_Validatie_ADS1115.ino` om de ADS1115-route te testen.

```cpp
// Zie examples/Systeem/ADC_Validatie/ADC_Validatie_ADS1115/ADC_Validatie_ADS1115.ino
#define ADC_BACKEND ADC_BACKEND_ADS1115
```


## Quad Logic Level Shifters of draadbruggen

Plaats voor het TFTSPI-pad ofwel de benodigde Quad Logic Level Shifters, ofwel zeven draadbruggen: zes van H9 naar H10 en één van H3 pin 1 naar H4 pin 1. Plaats nooit een Quad Logic Level Shifter en draadbruggen tegelijk op dezelfde verbindingen. De HV-zijde hoort aan de 5 V-Arduinozijde en de LV-zijde aan de 3,3 V-displayzijde.


## 20. Actuele bestandenset v1.0.0

De tekeningset en documentatie voor deze hardwareversie zijn:

- `SCH_GroeiAcademie-Stimulus-Hardware-Shield-v1.0.0_2026-07-29.json`;
- `Schematic_GroeiAcademie-Stimulus-Hardware-Shield-v1.0.0_2026-07-29.pdf`;
- `Schematic_GroeiAcademie-Stimulus-Hardware-Shield-v1.0.0_2026-07-29.png`;
- `Schematic_GroeiAcademie-Stimulus-Hardware-Shield-v1.0.0_2026-07-29.svg`;
- `GroeiAcademie-Stimulus-Hardware-Shield-v1.0.0.md`;
- `Handleiding-GroeiAcademie-Stimulus-Hardware-Validatie-v1.0.0.md`.

De twee afzonderlijke voorbeelden `ADC_Validatie_Native` en `ADC_Validatie_ADS1115` testen respectievelijk de directe ADC-route en de ADS1115-route. De volledige fysieke controle en validatievolgorde staan in de bijbehorende handleiding.


### BOARD_ESP32_UNO

Arduino Uno R3-vormfactor ESP32-boardprofiel.

#### Reeds getest en ondersteund
- Wemos D1 R32
- TTGO D1 R32

#### Verwacht compatibel
- Andere Arduino Uno R3-vormfactor ESP32-borden met dezelfde Arduino-pinout en een ondersteunde Arduino ESP32-core.

#### Nog niet getest
- Aan te vullen na validatie.
