# Stimulus uitbreiding met ADS1115

**FSR402/RFP602 analoge uitlezing transparant maken voor Arduino ADC of ADS1115**

Figuur 1. Huidige Stimulus-schema met vier FSR402/RFP602-lijnen richting Arduino analoge ingangen, LCD 1602 I2C, keypad en Arduino UNO R4 Minima.

## DEEL 1 — Beslissing en uitgangspunt

## 1. Korte conclusie

Voorzien in een extra Stimulus-schema met ADS1115 en uitbreiden van de software zodat de rest van de Stimulus-code niet hoeft te weten waar de analoge waarden vandaan komen.

In de versie zonder ADS1115 gaan de vier FSR402/RFP602-lijnen naar A0-A1 bij twee sensoren of naar A0-A3 bij vier sensoren van de Arduino.

In de versie met ADS1115 gaan diezelfde vier lijnen naar A0-A3 van de ADS1115. De Arduino leest dan via de I2C-bus.

De gekozen hardware-opzet is geen reeks solder-jumpers per sensorlijn. We gaan voor één schema waarin twee onafhankelijke bestukkingskeuzes mogelijk zijn, via Dupont-connectoren.

## 2. I2C-adres

Je kan per ADS1115-module zelf het I2C-adres kiezen via de ADDR-pin.

Je kiest dus niet per kanaal een adres, wel per ADS1115-chip of module.

| ADDR-pin verbonden met | I2C-adres | Gebruik |
|---|---:|---|
| ADDR naar GND | 0x48 | Standaard voor Stimulus |
| ADDR naar VDD | 0x49 | Tweede ADS1115 op dezelfde I2C-bus, bijvoorbeeld emotiemeting |
| ADDR naar SDA | 0x4A | Derde ADS1115 op dezelfde I2C-bus |
| ADDR naar SCL | 0x4B | Vierde ADS1115 op dezelfde I2C-bus |

Per ADS1115 kan je het adres kiezen met de ADDR-pin. De vier adressen zijn 0x48, 0x49, 0x4A en 0x4B. Daardoor kan je tot vier ADS1115-modules op dezelfde I2C-bus zetten. Voor de Stimulus-uitbreiding is één ADS1115 voldoende voor de vier FSR402/RFP602-lijnen.

De ADDR-adreskeuze gebeurt via een apart jumperveld. Er mag altijd maar één adresjumper tegelijk geplaatst zijn. Voor Stimulus is de standaard ADDR naar GND, dus adres 0x48.

Zonder I2C-multiplexer kan je maximaal vier ADS1115-modules op dezelfde I2C-bus zetten. Dat geeft maximaal 4 modules x 4 single-ended kanalen = 16 analoge ingangen.

## 3. Waarom een apart schema met ADS1115 zinvol is

| Variant | Aansluiting sensorlijnen | Waarom |
|---|---|---|
| Zonder ADS1115 | FSR402/RFP602 naar Arduino A0-A3 | Eenvoudig, snel testbaar, minder onderdelen |
| Met ADS1115 | FSR402/RFP602 naar ADS1115 A0-A3 | Arduino analoge pinnen blijven vrijer, hogere ADC-resolutie, uitbreidbaar |
| Beide tegelijk | Niet doen voor dezelfde sensorlijnen | Voorkomt dubbele belasting, verwarring en meetfouten |

We gaan voor één schema waarin twee onafhankelijke bestukkingskeuzes mogelijk zijn, via Dupont-connectoren.

De keuze voor het ADC-pad gebeurt door ofwel de ADS1115-module op H5 te plaatsen, ofwel geen ADS1115 te plaatsen en de vier analoge lijnen via draadbruggen van H5 naar H6 door te verbinden. H6 loopt daarna naar Arduino A0-A3. Daardoor zijn geen solder-jumpers per sensorlijn nodig.

De keuze voor de I2C-niveauconversie gebeurt apart: ofwel wordt de logic shifter geplaatst, ofwel worden draadbruggen gebruikt wanneer niveauconversie niet nodig is.

## 4. Bestukkingskeuze in het schema

| Bestukking | Verbinding | Codegevolg |
|---|---|---|
| ADS1115 niet bestukt | FSR402/RFP602 OUT1-OUT4 naar Arduino A0-A3 | Arduino leest rechtstreeks met `analogRead()` |
| ADS1115 bestukt | FSR402/RFP602 OUT1-OUT4 naar ADS1115 A0-A3 | Arduino leest via de I2C-bus |
| Keuze via Dupont-connector | Ofwel ADS1115-module op H5, ofwel draadbruggen van H5 naar H6 waarna H6 naar Arduino A0-A3 loopt | Flexibel, geen solder-jumpers per sensorlijn nodig |

## 5. Hardware-aandachtspunten

| Punt | Opmerking |
|---|---|
| Voeding ADS1115 | Als de FSR402/RFP602-spanningsdelers op 5V staan, moet de ADS1115-opstelling 5V-ingangen veilig aankunnen. De analoge ingang mag niet boven VDD komen. |
| I2C-bus | LCD en ADS1115 mogen samen op SDA/SCL, zolang adressen niet botsen en pull-ups niet te zwaar worden. |
| Logic shifter | In het huidige schema staat al een quad logic shifter voor de I2C-bus. Bepaal per buszijde of de ADS1115 op 5V of 3V3 zit. |
| GND | Arduino, ADS1115, sensor-spanningsdelers en eventuele externe voeding moeten een gemeenschappelijke GND hebben. |
| Resolutie | ADS1115 geeft meer meetresolutie dan de gewone Arduino ADC. Dat is nuttig bij subtiele drukverschillen. |
| Sampling | ADS1115 is niet bedoeld voor extreem snelle signalen, wel ruim voldoende voor trage stimulus/drukveranderingen. |

De ADS1115 kan op dezelfde I2C-bus als het LCD. Wanneer de ADS1115 en de LCD beide op 5V werken, is voor de ADS1115 geen extra logic shifter nodig. Als één onderdeel op 3V3 werkt, moet duidelijk vastliggen aan welke zijde van de logic shifter de ADS1115 wordt aangesloten.

De footprint voor de logic shifter mag twee functies krijgen: ofwel wordt de quad logic shifter geplaatst wanneer niveauconversie nodig is, ofwel worden de overeenkomstige kanaalparen rechtstreeks met draadbruggen verbonden wanneer beide zijden dezelfde logicaspanning gebruiken. GND blijft gemeenschappelijk.

## DEEL 2 — Eén schema, twee onafhankelijke bestukkingskeuzes

In dit deel worden de connectorreferenties uit het schema expliciet gebruikt. Zo blijft de Markdown rechtstreeks bruikbaar naast het schema.

| Referentie | Functie in het schema |
|---|---|
| H1 | 1x5 Dupont-connector voor de 1x4 keymatrix |
| H2 | 1x4 Dupont-connector voor LCD via I2C-bus |
| H3 | 1x6 Dupont-connector aan de HV-zijde van de logic-shifter-footprint |
| H4 | 1x6 Dupont-connector aan de LV-zijde van de logic-shifter-footprint |
| H5 | 1x10 Dupont-connector voor de ADS1115-module en de vier analoge sensorlijnen |
| H6 | 1x4 Dupont-connector die naar Arduino A0-A3 loopt voor directe Arduino-ADC |
| H7 | ADDR-jumperveld voor de ADS1115-adreskeuze |

## 6. Twee footprints, telkens met twee bestukkingsopties

### 6.1 Direct ADC ofwel ADS1115

H5 is de centrale 10-polige Dupont-connector voor het ADS1115-pad en voor de vier analoge sensorlijnen. H6 is de 4-polige connector die naar Arduino A0-A3 loopt. H7 is uitsluitend het ADDR-jumperveld voor de ADS1115-adreskeuze.

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

Bij de ADS1115-keuze wordt het ADS1115-bordje op H5 geplaatst. Daarna kies je via H7 het gewenste I2C-adres.

Bij de Arduino-ADC-keuze wordt geen ADS1115-bordje op H5 geplaatst. Dan worden de vier analoge sensorlijnen met draadbruggen van H5 naar H6 doorverbonden. H6 is de connector die naar Arduino A0-A3 loopt.

| Draadbrug bij Arduino-ADC-keuze | Betekenis |
|---|---|
| H5 pin 7 naar H6 pin 1 | Eerste fysieke doorbrug tussen H5 en H6 |
| H5 pin 8 naar H6 pin 2 | Tweede fysieke doorbrug tussen H5 en H6 |
| H5 pin 9 naar H6 pin 3 | Derde fysieke doorbrug tussen H5 en H6 |
| H5 pin 10 naar H6 pin 4 | Vierde fysieke doorbrug tussen H5 en H6 |

Plaats nooit tegelijk de ADS1115-module op H5 én de H5-H6-draadbruggen. Anders kunnen dezelfde sensorlijnen tegelijk aan ADS1115 A0-A3 en Arduino A0-A3 hangen.

### 6.2 Met of zonder logic shifter

De logic-shifter-footprint bestaat uit twee vrouwelijke headers: 2 rijen x 6 pinnen, 2,54mm steek, rijen op 5-pinnen-afstand van elkaar.

| HV-zijde | LV-zijde |
|---|---|
| HV1 | LV1 |
| HV2 | LV2 |
| HV | LV |
| GND | GND |
| HV3 | LV3 |
| HV4 | LV4 |

Deze footprint is altijd fysiek aanwezig. Als niveauconversie nodig is, wordt de quad logic shifter geplaatst. Als beide zijden dezelfde logicaspanning gebruiken, worden draadbruggen geplaatst.

## 7. Bestukkingstabellen

### 7.1 Bestukkingskeuze ADC-pad

| ADC-pad | Wat wordt geplaatst? | Wat wordt gebruikt? | Codekeuze |
|---|---|---|---|
| ADS1115-pad | ADS1115-module op H5 | H5 gebruikt alle 10 pinnen: VCC, GND, SCL, SDA, ADDR, ALRT, A0, A1, A2, A3. Het adres wordt gekozen via H7. | `ADC_BACKEND_ADS1115` |
| Direct Arduino-ADC-pad | Geen ADS1115-module op H5. Vier draadbruggen van H5 naar H6. | H5 pin 7 naar H6 pin 1, H5 pin 8 naar H6 pin 2, H5 pin 9 naar H6 pin 3, H5 pin 10 naar H6 pin 4. H6 loopt naar Arduino A0-A3. | `ADC_BACKEND_NATIVE` |

### 7.2 Bestukkingskeuze logic-shifter-footprint

| I2C-logica | Wat wordt geplaatst? | Verbinding | Gevolg |
|---|---|---|---|
| Met niveauconversie | Quad logic shifter | SDA en SCL lopen via overeenkomstige HV/LV-kanaalparen | Nodig wanneer HV- en LV-zijde verschillende logicaspanning gebruiken |
| Zonder niveauconversie | Draadbruggen in dezelfde headers | HV1 naar LV1, HV2 naar LV2, HV3 naar LV3, HV4 naar LV4 en gedeelde GND | Voor situaties waarin beide zijden dezelfde logicaspanning gebruiken |

## 8. ADDR-jumperveld H7

H7 is het ADDR-jumperveld voor de ADS1115-adreskeuze. H7 is niet bedoeld om sensorlijnen naar Arduino A0-A3 door te verbinden. De H5-H6-draadbruggen horen alleen bij de directe Arduino-ADC-keuze.

Het ADDR-jumperveld is alleen relevant wanneer de ADS1115-module op H5 geplaatst wordt.

| H7 verbindt ADDR met | I2C-adres | Gebruik |
|---|---:|---|
| GND | 0x48 | Standaard voor Stimulus |
| VDD | 0x49 | Tweede ADS1115 op dezelfde bus, bijvoorbeeld emotiemeting |
| SDA | 0x4A | Derde ADS1115 op dezelfde bus |
| SCL | 0x4B | Vierde ADS1115 op dezelfde bus |

Er mag altijd maar één adresjumper tegelijk geplaatst zijn. Voor Stimulus is de standaardkeuze H7: ADDR naar GND, dus adres 0x48.

## 9. Waarschuwingen op silkscreen

Silkscreen-label bij de 10-polige connector:

```text
ADS1115: VCC GND SCL SDA ADDR ALRT A0 A1 A2 A3
```

Silkscreen-label bij de 4-polige doorverbinding:

```text
ARD ADC: A0 A1 A2 A3
```

Waarschuwingen:

- H5: plaats ofwel het ADS1115-bordje, ofwel de H5-H6-draadbruggen voor directe Arduino-ADC. Nooit beide tegelijk.
- H7: plaats altijd maar één ADDR-adresjumper tegelijk. H7 is uitsluitend voor de ADS1115-adreskeuze.
- Logic-shifter-footprint H3/H4: IC = niveauconversie nodig. Draadbrug = zelfde logicaspanning. Nooit shifter-IC en draadbruggen tegelijk in dezelfde headers.
- Standaardkeuze bij eerste testen: directe Arduino-ADC via H5-H6-draadbruggen naar Arduino A0-A3 en draadbruggen voor de I2C-bus wanneer geen niveauconversie nodig is.

## DEEL 3 — Pinaansluitingen

## 10. FSR402/RFP602

### 10.1 Direct ADC via H5-H6-draadbruggen

Bij de directe Arduino-ADC-variant wordt geen ADS1115-bordje op H5 geplaatst. De vier analoge lijnen op H5 worden via draadbruggen naar H6 geleid. H6 loopt daarna naar Arduino A0-A3.

| Draadbrug | Verbinding richting Arduino | Opmerking |
|---|---|---|
| H5 pin 7 naar H6 pin 1 | H6 loopt naar één van de Arduino A0-A3-lijnen volgens het schema | Directe Arduino-ADC-meting |
| H5 pin 8 naar H6 pin 2 | H6 loopt naar één van de Arduino A0-A3-lijnen volgens het schema | Directe Arduino-ADC-meting |
| H5 pin 9 naar H6 pin 3 | H6 loopt naar één van de Arduino A0-A3-lijnen volgens het schema | Directe Arduino-ADC-meting |
| H5 pin 10 naar H6 pin 4 | H6 loopt naar één van de Arduino A0-A3-lijnen volgens het schema | Directe Arduino-ADC-meting |

Controleer in het schema en op de silkscreen dat de fysieke pinvolgorde H5 pin 7-10 naar H6 pin 1-4 gevolgd wordt. H7 hoort hier niet bij; H7 blijft uitsluitend voor de ADDR-adreskeuze.

### 10.2 ADS1115 op 10-polige connector

Adres 0x48 is gereserveerd voor de Stimulus-ADS1115, zodat er geen conflict ontstaat wanneer de emotiemeet-ADS1115's, bijvoorbeeld 0x49 en 0x4A, later op dezelfde bus bijkomen.

De 10-polige ADS1115-connector heeft deze pinnen:

```text
VCC, GND, SCL, SDA, ADDR, ALRT, A0, A1, A2, A3
```

ADDR wordt via H7 gekozen. Standaard is ADDR naar GND voor adres 0x48. ALRT wordt mee voorzien op H5, ook wanneer deze in de huidige Stimulus-code nog niet gebruikt wordt.

| Connector-pin | Verbonden met | Verbinding richting Arduino | Opmerking |
|---|---|---|---|
| VCC | 5V-voedingsrail | Arduino 5V of gedeelde 5V-rail | Voeding ADS1115-module |
| GND | Gemeenschappelijke massa | Arduino GND | Massa gemeenschappelijk met sensoren en I2C-bus |
| SCL | I2C SCL | Arduino SCL via logic shifter of draadbrug | Clocklijn I2C |
| SDA | I2C SDA | Arduino SDA via logic shifter of draadbrug | Datalijn I2C |
| ADDR | H7 ADDR-jumperveld | Geen Arduino-pin nodig | Bepaalt I2C-adres; standaard ADDR naar GND = 0x48 |
| ALRT | Voorzien / reserve | Eventueel later naar digitale interruptpin | Niet gebruikt in huidige Stimulus-code |
| A0 | FSR402/RFP602 OUT1 | Arduino leest via I2C | ADS1115 kanaal 0 |
| A1 | FSR402/RFP602 OUT2 | Arduino leest via I2C | ADS1115 kanaal 1 |
| A2 | FSR402/RFP602 OUT3 | Arduino leest via I2C | ADS1115 kanaal 2 |
| A3 | FSR402/RFP602 OUT4 | Arduino leest via I2C | ADS1115 kanaal 3 |

## 11. Character-LCD via I2C

Met LCD xxxx worden de ondersteunde character-LCD-formaten bedoeld die via een I2C-backpack op dezelfde I2C-bus kunnen hangen.

De LCD gebruikt dezelfde I2C-bus als de ADS1115. Het LCD-adres blijft apart ingesteld via `I2C_ADRES`; het ADS1115-adres via `ADS1115_I2C_ADDRESS` en het ADDR-jumperveld.

### 11.1 LCD/I2C-pinnen

| LCD/I2C-pin | Verbonden met | Verbinding richting Arduino | Opmerking |
|---|---|---|---|
| VCC | 5V-voedingsrail | Arduino 5V of gedeelde 5V-rail | Voeding LCD-backpack |
| GND | Gemeenschappelijke massa | Arduino GND | Massa gemeenschappelijk met ADS1115 en sensoren |
| SCL | I2C SCL | Arduino SCL via logic shifter of draadbrug | Dezelfde SCL-bus als ADS1115 |
| SDA | I2C SDA | Arduino SDA via logic shifter of draadbrug | Dezelfde SDA-bus als ADS1115 |

### 11.2 Ondersteunde LCD-formaten

| Configuratie | Betekenis | Aansluiting |
|---|---|---|
| `SCREEN_LCD1602` | LCD 16 kolommen x 2 regels | Via I2C-backpack op SDA/SCL |
| `SCREEN_LCD1604` | LCD 16 kolommen x 4 regels | Via I2C-backpack op SDA/SCL |
| `SCREEN_LCD2002` | LCD 20 kolommen x 2 regels | Via I2C-backpack op SDA/SCL |
| `SCREEN_LCD2004` | LCD 20 kolommen x 4 regels | Via I2C-backpack op SDA/SCL |
| `SCREEN_LCD4002` | LCD 40 kolommen x 2 regels | Via I2C-backpack op SDA/SCL |

### 11.3 Zonder logic shifter

Gebruik deze bestukking wanneer Arduino, LCD-backpack en ADS1115 op dezelfde I2C-logicaspanning werken. In dat geval worden de overeenkomstige kanaalparen in de logic-shifter-footprint rechtstreeks doorverbonden met draadbruggen.

### 11.4 Met logic shifter

Gebruik deze bestukking wanneer de HV-zijde en LV-zijde een verschillende logicaspanning hebben. SDA en SCL lopen dan elk via één overeenkomstig kanaalpaar van de quad logic shifter.

| Kanaalpaar | Signaal | HV-zijde | LV-zijde | Opmerking |
|---|---|---|---|---|
| HV1 naar LV1 | SDA | Hoge logicaspanning | Lage logicaspanning | I2C-datalijn |
| HV2 naar LV2 | SCL | Hoge logicaspanning | Lage logicaspanning | I2C-clocklijn |
| HV3 naar LV3 | Reserve | Hoge logicaspanning | Lage logicaspanning | Vrij voor later |
| HV4 naar LV4 | Reserve | Hoge logicaspanning | Lage logicaspanning | Vrij voor later |
| HV / LV / GND | Voeding en massa | HV naar hoge spanning | LV naar lage spanning | GND gemeenschappelijk |

### 11.5 Uitwerking LCD-aansluiting en logic-shifter-footprint

De logic-shifter-footprint is geen derde aparte keuze, maar een tweede onafhankelijke bestukkingskeuze: shifter plaatsen bij niveauverschil, of draadbruggen plaatsen wanneer beide zijden dezelfde logicaspanning gebruiken.

De 10-polige ADS1115-connector en de 4-polige Arduino-ADC-doorverbinding worden duidelijk naast elkaar of logisch in elkaars buurt geplaatst, zodat onmiddellijk zichtbaar is welke keuze actief is.

## DEEL 4 — Huidige software-implementatie

## 12. Backend-define

De huidige code gebruikt niet langer `STIMULUS_USE_ADS1115` of `STIMULUS_ADC_BACKEND_*`. De actuele configuratie gebruikt `ADC_BACKEND_NATIVE` en `ADC_BACKEND_ADS1115` in `SystemConfig.h`.

Praktische consequentie: bij `ADC_BACKEND_NATIVE` is Adafruit ADS1X15 niet nodig. Bij `ADC_BACKEND_ADS1115` moet de Adafruit ADS1X15-library geïnstalleerd zijn.

```cpp
// ============================================================================
// ADC BACKEND (naast UNO_VERSION)
// ============================================================================
#define ADC_BACKEND_NATIVE 0 // ingebouwde ADC van de Arduino
#define ADC_BACKEND_ADS1115 1 // externe 16-bit ADC via I2C
#define ADC_BACKEND ADC_BACKEND_NATIVE // wissel dit om van backend te wisselen
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

#if ADC_BACKEND == ADC_BACKEND_ADS1115
  #define ADC_BITS 15 // ADS1115 single-ended: 0-32767 bruikbaar bereik
  #define DELAY_US 0 // niet van toepassing; I2C-conversietijd zit in de leesfunctie zelf
#elif UNO_VERSION == 3
  #define ADC_BITS 10 // R3 = 10
  #define DELAY_US 100 // Vertraging tussen samples in de meetlussen
#elif UNO_VERSION == 4
  #define ADC_BITS 14 // R4 = 10 of 14
  #define DELAY_US 200 // Vertraging tussen samples in de meetlussen
#else
  #error UNO_VERSION moet 3 of 4 zijn.
#endif

#define ADC_MAX ((1UL << ADC_BITS) - 1)

#if ADC_BITS == 10
  #define ADC(x) (x)
#elif ADC_BITS == 14 || ADC_BITS == 15
  #define ADC(x) (((x) * ADC_MAX) / 1023L)
#else
  #error ADC_BITS moet 10, 14 of 15 zijn.
#endif
```

## 13. Transparante uitleesfunctie

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

## 14. Initialisatie bij ADS1115

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

## 15. Mappingtabel voor de huidige code

| Sensorlijn | Zonder ADS1115 | Met ADS1115 | Huidige code |
|---|---|---|---|
| FSR402/RFP602 1 | Arduino A0 | ADS1115 A0 | `sensorPin[0]` naar `RawAnalogRead(sensorPin[0])` |
| FSR402/RFP602 2 | Arduino A1 | ADS1115 A1 | `sensorPin[1]` naar `RawAnalogRead(sensorPin[1])` |
| FSR402/RFP602 3 | Arduino A2 | ADS1115 A2 | `sensorPin[2]` naar `RawAnalogRead(sensorPin[2])` |
| FSR402/RFP602 4 | Arduino A3 | ADS1115 A3 | `sensorPin[3]` naar `RawAnalogRead(sensorPin[3])` |

## 16. Aanbevolen beslissing voor nu

Ik stel voor om nu vier dingen vast te leggen:

| Onderdeel | Naam | Inhoud |
|---|---|---|
| Schema 1 | Stimulus direct ADC | FSR402/RFP602 naar Arduino A0-A3 |
| Schema 2 | Stimulus ADS1115 | FSR402/RFP602 naar ADS1115 A0-A3, ADS1115 via de I2C-bus |
| Schema 3 | Stimulus keuze via connector | ADS1115-module op H5 met adreskeuze via H7, of directe Arduino-ADC via H5-H6-draadbruggen naar Arduino A0-A3 |
| Code | Transparante uitleeslaag | `RawAnalogRead(int sensorPin)` met `ADC_BACKEND`-define, gebruikt door `AnalogReadMetGekorigeerdeOffsets(...)` |

Daarmee blijft de huidige code bruikbaar, kan je later eenvoudig naar ADS1115 omschakelen, en voorkom je pinproblemen wanneer er extra componenten voor emoties bijkomen.

## DEEL 5 — Validatie

## 17. Validatiestap: wanneer is ADS1115 betrouwbaar?

Voorstel voor een objectieve vergelijking, uitvoerbaar met schema 3 zonder herbedrading:

- Zelfde fysieke FSR402/RFP602-sensor, zelfde druk, eerst meten via directe Arduino-ADC met de H5-H6-draadbruggen naar Arduino A0-A3, daarna meten via de ADS1115-module op H5 met adreskeuze via H7.
- Vergelijk ruis: standaardafwijking bij constante druk.
- Vergelijk lineariteit over het volledige drukbereik.
- Controleer sample-timing: ADS1115 via I2C kost meer tijd per lezing dan `analogRead()`. Voor Stimulus, met trage druksignalen, is dat doorgaans geen probleem, maar dit moet wel gemeten worden.
- Pas na deze vergelijking wordt bepaald of schema 2, ADS1115, de standaardkeuze wordt, of dat schema 1, directe ADC, volstaat en de ADS1115 optioneel blijft.

## 18. Validatiescript — Stimulus_ADC_Validatie-v0.10.00.ino

Dit script valideert dezelfde vier FSR402/RFP602-lijnen via de twee mogelijke backends. Het gebruikt schema 3 met verwisselbare connectoren: eerst meten via directe Arduino-ADC met H5-H6-draadbruggen naar Arduino A0-A3, daarna meten via de ADS1115-module op H5 met adreskeuze via H7, en de resultaten vergelijken.

In dit validatiescript wordt dezelfde functiehandtekening gebruikt als in de Stimulus-code:

```cpp
int RawAnalogRead(int sensorPin)
```

Bij `ADC_BACKEND_NATIVE` is `sensorPin` een Arduino-pin. Bij `ADC_BACKEND_ADS1115` is `sensorPin` een ADS1115-kanaalnummer.

```cpp
// ============================================================================
// Stimulus — transparante ADC-laag + validatiesketch (Arduino-ADC vs ADS1115)
// ============================================================================
// Doel: dezelfde 4 FSR402/RFP602-lijnen uitlezen via twee mogelijke backends,
// zonder dat de rest van de Stimulus-code hoeft te weten welke backend actief is.
//
// Gebruik voor validatie (schema 3, verwisselbare connectoren):
// 1. Voor Arduino-ADC: plaats geen ADS1115 op H5 en verbind H5 pin 7-10 met H6 pin 1-4.
// 2. Zet ADC_BACKEND hieronder op dezelfde kant (NATIVE).
// 3. Flash, laat 10 seconden lopen, noteer de Serial-output (gemiddelde/stdev/min/max).
// 4. Verwijder de H5-H6-draadbruggen, plaats de ADS1115 op H5, kies adres via H7, zet de #define om,
//    herflash, herhaal de meting met dezelfde fysieke druk.
// 5. Vergelijk beide resultaten.
//
// Vereist: Adafruit_ADS1X15-library wanneer ADC_BACKEND_ADS1115 actief is.
// ============================================================================

#include <Wire.h>

#define ADC_BACKEND_NATIVE 0
#define ADC_BACKEND_ADS1115 1
#define ADC_BACKEND ADC_BACKEND_NATIVE

#define ADS1115_I2C_ADDRESS 0x48
#define STIMULUS_AANTAL_KANALEN 4
#define DEBUG

#if ADC_BACKEND == ADC_BACKEND_ADS1115
  #include <Adafruit_ADS1X15.h>
  Adafruit_ADS1115 ads;
#endif

#if ADC_BACKEND == ADC_BACKEND_ADS1115
  #define PIN_SENSOR_1 0
  #define PIN_SENSOR_2 1
  #define PIN_SENSOR_3 2
  #define PIN_SENSOR_4 3
#else
  #define PIN_SENSOR_1 A0
  #define PIN_SENSOR_2 A1
  #define PIN_SENSOR_3 A2
  #define PIN_SENSOR_4 A3
#endif

const int sensorPin[STIMULUS_AANTAL_KANALEN] = { PIN_SENSOR_1, PIN_SENSOR_2, PIN_SENSOR_3, PIN_SENSOR_4 };
bool ads1115Aanwezig = false;
bool metingAfgerond = false;

#ifndef PRINTTOSCREEN_BESTAAT_AL
void PrintToScreen(const char* regel1, const char* regel2) {
#ifdef DEBUG
  Serial.print(F("[LCD] ")); Serial.print(regel1); Serial.print(F(" / ")); Serial.println(regel2);
#endif
}
#endif

void InitialiseerADS1115() {
#if ADC_BACKEND == ADC_BACKEND_ADS1115
  if (!ads.begin(ADS1115_I2C_ADDRESS)) {
#ifdef DEBUG
    Serial.println(F("ADS1115 niet gevonden"));
#endif
    PrintToScreen("ADS1115", "niet gevonden");
    ads1115Aanwezig = false;
    return;
  }
  ads.setGain(GAIN_TWOTHIRDS);
  ads1115Aanwezig = true;
#else
  ads1115Aanwezig = true;
#endif
}

int RawAnalogRead(int sensorPin) {
#if ADC_BACKEND == ADC_BACKEND_ADS1115
  if (!ads1115Aanwezig) return 0;
  return ads.readADC_SingleEnded(sensorPin);
#else
  return analogRead(sensorPin);
#endif
}

struct KanaalStats {
  long n = 0;
  double som = 0;
  double somKwadraat = 0;
  int minWaarde = 32767;
  int maxWaarde = -32768;
};

KanaalStats stats[STIMULUS_AANTAL_KANALEN];

void voegMetingToe(uint8_t kanaal, int waarde) {
  KanaalStats &s = stats[kanaal];
  s.n++;
  s.som += waarde;
  s.somKwadraat += (double)waarde * waarde;
  if (waarde < s.minWaarde) s.minWaarde = waarde;
  if (waarde > s.maxWaarde) s.maxWaarde = waarde;
}

void printStats() {
  for (uint8_t k = 0; k < STIMULUS_AANTAL_KANALEN; k++) {
    KanaalStats &s = stats[k];
    if (s.n == 0) continue;
    double gemiddelde = s.som / s.n;
    double variantie = (s.somKwadraat / s.n) - (gemiddelde * gemiddelde);
    double stdev = sqrt(variantie > 0 ? variantie : 0.0);
    Serial.print(F("Kanaal ")); Serial.print(k);
    Serial.print(F(": n=")); Serial.print(s.n);
    Serial.print(F(" gemiddelde=")); Serial.print(gemiddelde, 2);
    Serial.print(F(" stdev=")); Serial.print(stdev, 3);
    Serial.print(F(" min=")); Serial.print(s.minWaarde);
    Serial.print(F(" max=")); Serial.println(s.maxWaarde);
  }
}

const unsigned long VALIDATIE_DUUR_MS = 10000UL;
const unsigned long SAMPLE_INTERVAL_MS = 20UL;
unsigned long tStart = 0;
unsigned long laatsteSample = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  InitialiseerADS1115();

#if ADC_BACKEND == ADC_BACKEND_ADS1115
  Serial.println(F("=== Validatie: backend = ADS1115 ==="));
#else
  Serial.println(F("=== Validatie: backend = Arduino-ADC ==="));
#endif

  Serial.println(F("Controleer dat de fysieke connectorkeuze overeenkomt met deze backend."));
  tStart = millis();
}

void loop() {
  if (metingAfgerond) return;

  unsigned long nu = millis();

  if (nu - laatsteSample >= SAMPLE_INTERVAL_MS) {
    laatsteSample = nu;
    for (uint8_t k = 0; k < STIMULUS_AANTAL_KANALEN; k++) {
      int waarde = RawAnalogRead(sensorPin[k]);
      voegMetingToe(k, waarde);
    }
  }

  if (nu - tStart >= VALIDATIE_DUUR_MS) {
    Serial.println(F("--- Resultaat ---"));
    printStats();
    Serial.println(F("Meting voltooid. Wissel connector + backend-define voor de andere kant, herflash."));
    metingAfgerond = true;
  }
}
```
