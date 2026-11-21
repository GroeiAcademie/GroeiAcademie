# BioAmpEXGPill

## Implementatie

- Broncode: `src/Systeem/Sensor/BioAmpEXGPill.h` en `BioAmpEXGPill.cpp`.
- C++-component: `BioAmpEXGPill`, afgeleid van `SensorAnaloog`.
- Parent: `GedeeldeBusNode*` in de constructor.
- Componentcode: `BioAmpEXGPill` (0xB9).
- Configuratie: `src/Configuratie/Sensoren/ConfigBioAmpEXGPill_template.h`; de optionele persoonlijke versie heet telkens zonder `_template`.
- Data-kanalen: 9 (kanaal 1 via SensorAnaloog; zie "Data-kanalen" verderop).

## Aansluiten

**Aansluitwijze in de bestaande FrameWork-code:**

- **Meetverbinding:** sensor-uitgang naar het toegewezen **ADC-Extender-kanaal**. Verbind de signaalreferentie/GND met de ADC-Extender volgens de vereisten van die Extender.
- **Voeding:** controleer de benodigde spanning en toegestane OUT-spanning van de **specifieke sensor of module** vóór aansluiten; de FrameWork-code bepaalt de elektrische grenswaarden van het moduleboard niet.

- Analoge OUT van de signaalversterker → het gekozen kanaal van de ADC-Extender.
- Electrode/sensoraansluitingen uitsluitend volgens de concrete BioAmp EXG Pill-handleiding en met een **elektrisch veilige, geïsoleerde** opstelling.

**Voor de hardwaretest:**

- Gebruik de bestaande `.ino` uit **Testen** en de bestaande configuratiebestanden onder `src/Configuratie/`; een `*_template.h` wordt niet rechtstreeks ingelezen.
- **Een vaste Arduino-pintoewijzing is niet algemeen geldig:** de gekozen constructor, configuratie en werkelijke bekabeling moeten overeenkomen. Waar hierboven geen exact spanningsniveau of pinheader is bevestigd, mag dat niet uit deze documentatie worden afgeleid.


**Aanvulling aansluitpinnen en elektrische voorwaarden:**

- OUT→toegewezen ADC-ingang; GND→signaalreferentie; elektrodecontacten volgens de exacte BioAmp EXG Pill-uitvoering. De polariteit/voedingsrails en maximale OUT-spanning moeten vóór gebruik tegen de officiële modulehandleiding worden gecontroleerd. Bij meting op het lichaam alleen met veilige, geïsoleerde voeding.
- De concrete modulevariant bepaalt de **exacte voedingsgrenzen en fysieke pinvolgorde**. Neem die gegevens niet over van een andere module zonder de werkelijke print te controleren.

**Aansluit- en gebruiksgegevens uit de bestaande FrameWork-interface:**

- **Analoge bekabeling voor BioAmpEXGPill:** het analoge meetpunt of `OUT` gaat naar het ADC-kanaal `sensorPin` van de opgegeven parent. Gebruik het **kanaal van die ADC-Extender**, niet automatisch een native Arduino-`A0`-pin. Verbind de elektrische meetreferentie met de GND van dezelfde ADC-opstelling.
- De ADC-ingang mag niet buiten zijn werkelijk ingestelde spanningsbereik worden gestuurd. Sensorvoeding en eventuele spanningsdeler horen bij het specifieke sensorbord of het losse element; een C++-kanaalnummer bepaalt geen veilige voedingsspanning.
- Naast de analoge hoofdmeting bevat de bestaande klasse aparte leesfuncties `LeesEMGEnvelop(float&)`, `LeesHartslagPiek(bool&)` en `LeesKnipperen(bool&)`. De gewenste meting bepaalt welk sensorsignaal en welke elektrodenopstelling nodig zijn.
- Verbind een persoon alleen via een voor lichaamsmetingen veilig, galvanisch gescheiden meetsysteem; gebruik geen direct netgevoede of via USB aan onveilige apparatuur gekoppelde elektroden.

**Bestaande constructor (parameter-/pinvolgorde):**

```cpp
BioAmpEXGPill(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t sensorPin, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
```

**Configuratie voor deze hardware:**

- Gebruik bij een eigen configuratie `src/Configuratie/Sensoren/ConfigBioAmpEXGPill.h` (het aanwezige `src/Configuratie/Sensoren/ConfigBioAmpEXGPill_template.h` is een kopieersjabloon en wordt niet rechtstreeks ingelezen).
- Voor één aangesloten exemplaar: `SENSOR_BIOAMPEXGPILL_AANTAL` op **1**, behalve wanneer de bestaande groeps-/setconfiguratie uitdrukkelijk een andere betekenis heeft.
- De bestaande configuratie bevat deze aansluitrelevante definities (gebruik de **werkelijke aangesloten** pinnen, adressen en referenties):
  - `SENSOR_BIOAMPEXGPILL_AANTAL`: sjabloonwaarde `1`.
  - `SENSOR_BIOAMPEXGPILL_ENVELOP_VENSTER`: sjabloonwaarde `128`.
  - `SENSOR_BIOAMPEXGPILL_ADC_BEREIK_MILLIVOLT`: sjabloonwaarde `5000`.

**Letterlijk constructorfragment uit de bestaande voorbeeldsketch** (alle gebruikte parent-objecten moeten zelf correct aangesloten en geïnitialiseerd zijn):

```cpp
GedeeldeBusNewComponent<BioAmpEXGPill>(&extenderADS1115,
    GedeeldeBusComponent::BioAmpEXGPill,
    static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN0),
    HardwareResourceToegang::GEDEELD);
```

**Bestaande test-/voorbeeldsketches:**

- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_BioAmpEXGPill/Sensor_BioAmpEXGPill.ino`
- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_BioAmpEXGPill_ECG_Hartslag/Sensor_BioAmpEXGPill_ECG_Hartslag.ino`
- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_BioAmpEXGPill_EMG_Envelop/Sensor_BioAmpEXGPill_EMG_Envelop.ino`
- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_BioAmpEXGPill_EOG_Knipperen/Sensor_BioAmpEXGPill_EOG_Knipperen.ino`

**Elektrische aansluiting en randvoorwaarden volgens Upside Down Labs:**

- De originele **BioAmp EXG Pill** heeft aan de hostkant `VCC`, `GND` en `OUT`. Verbind `OUT` met het bestaande analoge ADC-kanaal in de FrameWorkconstructor. De fabrikant noemt **3,3 V of 5 V** gebruik, maar de uitgangsspanning moet overeenkomen met de werkelijke ADC-ingangslimiet.
- De elektrodeconnector/ingangen `IN+`, `IN−` en `REF` zijn bedoeld voor biopotentiaalmetingen en mogen niet als digitale host-pinnen worden gebruikt. **Bij contact met het lichaam een galvanisch veilige batterijgevoede opstelling gebruiken; geen netgevoede, niet-geïsoleerde USB-computer rechtstreeks aansluiten.** Fout ompolen van VCC/GND kan de print beschadigen.

**Technische handleiding:** https://docs.upsidedownlabs.tech/hardware/bioamp/bioamp-exg-pill/index.html

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_BioAmpEXGPill/` (lifecycle), `Sensor_BioAmpEXGPill_ECG_Hartslag/` (`LeesHartslagPiek()`, met `SENSOR_BIOAMPEXGPILL_MEETMETHODE` `BIOAMPEXGPILL_MEETMETHODE_ECG`), `Sensor_BioAmpEXGPill_EMG_Envelop/` (`LeesEMGEnvelop()`, `_EMG`) en `Sensor_BioAmpEXGPill_EOG_Knipperen/` (`LeesKnipperen()`, `_EOG`), volgens de officiële voorbeelden HeartBeatDetection, EMGEnvelop en EyeBlinkDetection.
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.

### Betekenis van gemeten waarden
- Maak in hardwareverslagen strikt onderscheid tussen **ruw sensorsignaal**, een door code berekende waarde en een klinisch gevalideerde meting. De aanwezigheid van een meetwaarde in Data bewijst geen medische validiteit.
- **Proef:** registreer nul-/referentie- of gecontroleerde testinput, juiste eenheden en invloed van kabels/contactkwaliteit. Geen medische conclusie afleiden. **Status:** metrologie/kalibratie open.

### Controle van de haalbare samplefrequentie

De vaste frequentie verschilt per meetmethode: ECG 125 Hz, EMG 500 Hz, EOG 75 Hz en EEG 256 Hz. De volledige ADC-keten moet die frequentie in de praktijk kunnen halen; de ingestelde ADC-datarate alleen is geen garantie. Bij gefilterde meetmethoden telt `aantalTeLateSamplesNaElkaar` metingen die meer dan 10 % te laat zijn. Vanaf drie opeenvolgende te late samples wordt `samplefrequentieNietHaalbaar` `true` en geven `LeesHoofdwaarde()` en de toepasselijke gespecialiseerde leesfuncties `false` terug. Als het ritme opnieuw gehaald wordt, wordt de status gewist. De RAW-meetmethode gebruikt deze filtercontrole niet. Bij een ADS1115 met 128 SPS zijn met name de vaste EMG- en EEG-frequenties niet haalbaar; 860 SPS moet nog op de werkelijke opstelling worden getest.

### Aanvullende Sensor × Extender-testvoorbeelden

De onderstaande `.ino`-bestanden behoren tot de huidige v2.0.0-voorbeelden. Hun aanwezigheid is gecontroleerd; compilatie en fysieke hardwarevalidatie zijn hier nog **niet** aangetoond.

- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_BioAmpEXGPill_ADS1158/Sensor_BioAmpEXGPill_ADS1158.ino`
- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_BioAmpEXGPill_ADS7828/Sensor_BioAmpEXGPill_ADS7828.ino`
- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_BioAmpEXGPill_ADS7953/Sensor_BioAmpEXGPill_ADS7953.ino`
- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_BioAmpEXGPill_CD74HC4067/Sensor_BioAmpEXGPill_CD74HC4067.ino`

### Functionele Sensor × Extender-voorbeelden

De onderstaande voorbeelden bevatten **meetaanroepen na de activering**. Een getoonde waarde is geen bewijs van meetnauwkeurigheid of van een geslaagde hardwaretest.

- **Fysieke test:** Controleer signaal via de geconfigureerde ECG/EMG/EOG/EEG-meetmethode; geen medische diagnose.
- **Huidige reikwijdte:** de `.ino` gebruikt uitsluitend reeds bestaande FrameWork-functies; niet-beschikbare meetkanalen, kalibraties of externe functies zijn niet toegevoegd.
- Bij lichaamsgebonden metingen uitsluitend een geschikte galvanisch veilige, geïsoleerde batterijgevoede meetopstelling gebruiken; Serial-/USB-verbinding naar onveilige apparatuur vermijden.

**Voorbeelden:**

- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_BioAmpEXGPill_ADS1158/Sensor_BioAmpEXGPill_ADS1158.ino`
- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_BioAmpEXGPill_ADS7828/Sensor_BioAmpEXGPill_ADS7828.ino`
- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_BioAmpEXGPill_ADS7953/Sensor_BioAmpEXGPill_ADS7953.ino`
- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_BioAmpEXGPill_CD74HC4067/Sensor_BioAmpEXGPill_CD74HC4067.ino`

**Validatie:** Arduino-compilatie, fysieke test, kalibratie en meetkwaliteitsvalidatie zijn hier nog niet aangetoond.

## Native meetvoorbeeld

`examples/Systeem/GedeeldeBus/Sensoren/Sensor_BioAmpEXGPill/Sensor_BioAmpEXGPill.ino` voert na de lifecycle daadwerkelijk uitlezingen uit via de bestaande Sensor-API.
Gemeten of weergegeven in dit voorbeeld: **Gefilterd biosignaal**.
De seriële uitvoer wordt beperkt; de Sensor-uitlezing blijft in `loop()` plaatsvinden.
Dit bevestigt bij geslaagde hardwaretests de ruwe of rechtstreeks beschikbare meetwaarde, niet automatisch alle fysische kanalen, kalibratie of nauwkeurigheid.
Compilatie en fysieke meting zijn nog niet bevestigd.

## Verwerking

- Data: kanaal 1 ruwe ADC-waarde, kanaal 2 gefilterde waarde van de meetmethode (alleen als de samplefrequentie haalbaar is).
- ECG: `ReadHeartRateBeatsPerMinute(float&)` uit de pieken die `LeesHoofdwaarde()` bijhoudt (HeartRateFromBeats). Voorbeeld: `Sensor_BioAmpEXGPill_ECG_Verwerking.ino`.
- EEG: `ReadEEGbandPowers(delta, theta, alpha, beta, gamma)` uit de gefilterde samples (EEGfrequencyBands, 256 Hz, venster 1 s, 1 Hz per bin). Voorbeeld: `Sensor_BioAmpEXGPill_EEG_Verwerking.ino`.
- Ouderdom: `LeesEMGEnvelop()` en `ReadEEGbandPowers()` geven false als de laatste geslaagde ADC-lezing ouder is dan 1 s (`lastSuccessfulSampleUS`). `tijdstipLaatsteSample` blijft het geplande sampletijdstip voor het vaste ritme.
- Het vaste ritme blijft dat van de meetmethode (ECG 125 Hz, EMG 500 Hz, EOG 75 Hz, EEG 256 Hz): de filtercoëfficiënten gelden alleen bij die frequentie. Er is daarom geen aparte instelling voor de samplefrequentie.

- `Sensor_BioAmpEXGPill_ECG_Verwerking.ino` en `Sensor_BioAmpEXGPill_EEG_Verwerking.ino` stoppen de compilatie met `#error` als de vaste frequentie (ECG 125 Hz, EEG 256 Hz) hoger is dan wat `EXTENDER_ADS1115_DATARATE` toelaat (4 = 128 SPS, 5 = 250, 6 = 475, 7 = 860).

## Data-kanalen

- Vaste kanalen, ongeacht de meetmethode: 1 ruwe ADC-waarde (int), 2 gefilterde waarde (float), 3 EMG-envelop, 4 ECG-hartslag, 5 tot en met 9 delta, theta, alfa, bèta en gamma (float).
- Kanaal 3 (EMG): bij elke sample waarvoor de samplefrequentie haalbaar is, dus 500 records per seconde. Of FLASH en microSD dat bijhouden zonder de samplefrequentie te verstoren, is niet getest.
- Kanaal 4 (ECG): bij elke aanvaarde slag met een geldige hartslag.
- Kanaal 5 tot en met 9 (EEG): bij een geslaagde aanroep van `ReadEEGbandPowers()`.
- De meetmethode staat niet in een Data-record. Bij een opname moet ze bekend blijven, vooral voor kanaal 2: het filter verschilt per meetmethode.

## Heractivatie

- Eigen `Activeren()`: `tijdstipLaatsteSample` 0 laat `LeesHoofdwaarde()` opnieuw beginnen (filters, EMG-envelop, pieken, EEG-venster).
- Ook `lastSuccessfulSampleUS`, `samplefrequentieNietHaalbaar` en `aantalTeLateSamplesNaElkaar` beginnen opnieuw; bij EMG is `laatsteEnvelop` NAN, bij ECG begint de hartslaggeschiedenis opnieuw.
