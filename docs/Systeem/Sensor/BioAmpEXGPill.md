# BioAmpEXGPill

## Implementatie

- Broncode: `src/Systeem/Sensor/BioAmpEXGPill.h` en `BioAmpEXGPill.cpp`.
- C++-component: `BioAmpEXGPill`, afgeleid van `SensorAnaloog`.
- Parent: `GedeeldeBusNode*` in de constructor.
- Componentcode: `BioAmpEXGPill` (0xA1).
- Configuratie: `src/Configuratie/Sensoren/ConfigBioAmpEXGPill_template.h`; de optionele persoonlijke versie heet telkens zonder `_template`.
- Data-kanalen: 1 (via SensorAnaloog).

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

