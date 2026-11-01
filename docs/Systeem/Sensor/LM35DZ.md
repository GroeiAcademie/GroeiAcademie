# LM35DZ

## Implementatie

- Broncode: `src/Systeem/Sensor/LM35DZ.h` en `LM35DZ.cpp`.
- C++-component: `LM35DZ`, afgeleid van `SensorAnaloog`.
- Parent: `GedeeldeBusNode*` in de constructor.
- Componentcode: `LM35DZ` (0xD5).
- Configuratie: `src/Configuratie/Sensoren/ConfigLM35DZ_template.h`; de optionele persoonlijke versie heet telkens zonder `_template`.
- Data-kanalen: 2.

## Uit de broncode

LM35DZ: temperatuursensor; ruwe ADC-waarde plus temperatuur via expliciet ADC-meetbereik. Eén component voor één fysiek apparaat, via de bestaande ADC-Extender. De meetcode zit in SensorAnaloog; deze klasse rekent in MeetwaardeVerwerken() om naar temperatuur.

## Aansluiten

**Aansluitwijze in de bestaande FrameWork-code:**

- **Meetverbinding:** sensor-uitgang naar het toegewezen **ADC-Extender-kanaal**. Verbind de signaalreferentie/GND met de ADC-Extender volgens de vereisten van die Extender.
- **Voeding:** controleer de benodigde spanning en toegestane OUT-spanning van de **specifieke sensor of module** vóór aansluiten; de FrameWork-code bepaalt de elektrische grenswaarden van het moduleboard niet.

- OUT → gekozen ADC-Extender-ingang; V+ en GND volgens de **feitelijke LM35DZ-behuizing**.
- De drie aansluitingen niet op basis van een willekeurige modulefoto omwisselen; pinvolgorde van de gebruikte uitvoering controleren.

**Voor de hardwaretest:**

- Gebruik de bestaande `.ino` uit **Testen** en de bestaande configuratiebestanden onder `src/Configuratie/`; een `*_template.h` wordt niet rechtstreeks ingelezen.
- **Een vaste Arduino-pintoewijzing is niet algemeen geldig:** de gekozen constructor, configuratie en werkelijke bekabeling moeten overeenkomen. Waar hierboven geen exact spanningsniveau of pinheader is bevestigd, mag dat niet uit deze documentatie worden afgeleid.


**Aanvulling aansluitpinnen en elektrische voorwaarden:**

- Bij losse LM35DZ: +Vs→passende voeding, OUT→ADC-ingang, GND→gemeenschappelijke GND. De fysieke TO-92-pinvolgorde en voedingslimieten moeten op de officiële TI-datasheet en de werkelijk gebruikte behuizing worden gecontroleerd.
- De concrete modulevariant bepaalt de **exacte voedingsgrenzen en fysieke pinvolgorde**. Neem die gegevens niet over van een andere module zonder de werkelijke print te controleren.

**Aansluit- en gebruiksgegevens uit de bestaande FrameWork-interface:**

- **Analoge bekabeling voor LM35DZ:** het analoge meetpunt of `OUT` gaat naar het ADC-kanaal `sensorPin` van de opgegeven parent. Gebruik het **kanaal van die ADC-Extender**, niet automatisch een native Arduino-`A0`-pin. Verbind de elektrische meetreferentie met de GND van dezelfde ADC-opstelling.
- De ADC-ingang mag niet buiten zijn werkelijk ingestelde spanningsbereik worden gestuurd. Sensorvoeding en eventuele spanningsdeler horen bij het specifieke sensorbord of het losse element; een C++-kanaalnummer bepaalt geen veilige voedingsspanning.
- De klasse heeft een eigen `TemperatuurC()` en verwerkt de ADC-waarde via `MeetwaardeVerwerken(int32_t, bool)`. De omzetting hangt af van het **effectieve ADC-millivoltbereik**.
- Verbind de analoge uitgang alleen met een ADC-ingang waarvan het spanningsbereik met voeding en uitgangsspanning van de concrete LM35DZ-uitvoering overeenkomt.

**Bestaande constructor (parameter-/pinvolgorde):**

```cpp
LM35DZ(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t sensorPin, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
```

**Configuratie voor deze hardware:**

- Gebruik bij een eigen configuratie `src/Configuratie/Sensoren/ConfigLM35DZ.h` (het aanwezige `src/Configuratie/Sensoren/ConfigLM35DZ_template.h` is een kopieersjabloon en wordt niet rechtstreeks ingelezen).
- Voor één aangesloten exemplaar: `SENSOR_LM35DZ_AANTAL` op **1**, behalve wanneer de bestaande groeps-/setconfiguratie uitdrukkelijk een andere betekenis heeft.
- De bestaande configuratie bevat deze aansluitrelevante definities (gebruik de **werkelijke aangesloten** pinnen, adressen en referenties):
  - `SENSOR_LM35DZ_AANTAL`: sjabloonwaarde `1 // standaard waarde: 0`.
  - `SENSOR_LM35DZ_ADC_BEREIK_MILLIVOLT`: sjabloonwaarde `1500 // standaard waarde: 1500 (LM35DZ: 10 mV/°C; meetbereik tot 150 °C)`.

**Letterlijk constructorfragment uit de bestaande voorbeeldsketch** (alle gebruikte parent-objecten moeten zelf correct aangesloten en geïnitialiseerd zijn):

```cpp
GedeeldeBusNewComponent<LM35DZ>(
    &extenderADS1115, GedeeldeBusComponent::LM35DZ,
    static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN0),
    HardwareResourceToegang::GEDEELD);
```

**Bestaande test-/voorbeeldsketches:**

- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_LM35DZ/Sensor_LM35DZ.ino`

**Elektrische aansluiting en randvoorwaarden volgens Texas Instruments:**

- De **LM35-familie** geeft een analoge temperatuurspanning van **10 mV per °C**. TI noemt voor de familie een voeding van **4–30 V**; controleer de precieze LM35DZ-uitvoering en het **package-aanzicht** voordat je V+, OUT en GND op de fysieke drie pootjes aansluit.
- Sluit OUT aan op het geselecteerde ADC-Extender-kanaal en pas de spanningsreferentie en meetresolutie in de FrameWork aan; voedingsspanning is geen synoniem voor uitgangsspanning.

**Technische handleiding:** https://www.ti.com/product/LM35?keyMatch=LM35&tisearch=Search-EN-Everything

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_LM35DZ/`.
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.

