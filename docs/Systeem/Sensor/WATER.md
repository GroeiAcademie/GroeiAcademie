# WATER

## Implementatie

- Broncode: `src/Systeem/Sensor/WATER.h` (geen `.cpp`).
- C++-component: `WATER`, afgeleid van `SensorAnaloog`.
- Parent: `GedeeldeBusNode*` in de constructor.
- Componentcode: `WATER` (0xDA).
- Configuratie: `src/Configuratie/Sensoren/ConfigWATER_template.h`; de optionele persoonlijke versie heet telkens zonder `_template`.
- Data-kanalen: 1 (via SensorAnaloog).

## Uit de broncode

WATER: analoge watersensormodule; exacte modulevariant te controleren. Eén component voor één fysiek apparaat, via de bestaande ADC-Extender. De meetcode zit in SensorAnaloog; deze klasse geeft alleen de pin en het bereik door.

## Aansluiten

**Aansluitwijze in de bestaande FrameWork-code:**

- **Meetverbinding:** sensor-uitgang naar het toegewezen **ADC-Extender-kanaal**. Verbind de signaalreferentie/GND met de ADC-Extender volgens de vereisten van die Extender.
- **Voeding:** controleer de benodigde spanning en toegestane OUT-spanning van de **specifieke sensor of module** vóór aansluiten; de FrameWork-code bepaalt de elektrische grenswaarden van het moduleboard niet.

- Analoge OUT van de exacte waterdetectiemodule → gekozen ADC-Extender-kanaal.
- De concrete modulevariant eerst identificeren; digitale OUT en analoge OUT mogen niet verwisseld worden.

**Voor de hardwaretest:**

- Gebruik de bestaande `.ino` uit **Testen** en de bestaande configuratiebestanden onder `src/Configuratie/`; een `*_template.h` wordt niet rechtstreeks ingelezen.
- **Een vaste Arduino-pintoewijzing is niet algemeen geldig:** de gekozen constructor, configuratie en werkelijke bekabeling moeten overeenkomen. Waar hierboven geen exact spanningsniveau of pinheader is bevestigd, mag dat niet uit deze documentatie worden afgeleid.


**Aanvulling aansluitpinnen en elektrische voorwaarden:**

- Analoge SIG/AO→ADC-kanaal; GND→GND en VCC→voor de exacte watersensorprint voorgeschreven rail. Indien ook DO beschikbaar: die hoort bij een aparte digitale uitlezing, niet automatisch bij deze analoge driver.
- De concrete modulevariant bepaalt de **exacte voedingsgrenzen en fysieke pinvolgorde**. Neem die gegevens niet over van een andere module zonder de werkelijke print te controleren.

**Aansluit- en gebruiksgegevens uit de bestaande FrameWork-interface:**

- **Analoge bekabeling voor WATER:** het analoge meetpunt of `OUT` gaat naar het ADC-kanaal `sensorPin` van de opgegeven parent. Gebruik het **kanaal van die ADC-Extender**, niet automatisch een native Arduino-`A0`-pin. Verbind de elektrische meetreferentie met de GND van dezelfde ADC-opstelling.
- De ADC-ingang mag niet buiten zijn werkelijk ingestelde spanningsbereik worden gestuurd. Sensorvoeding en eventuele spanningsdeler horen bij het specifieke sensorbord of het losse element; een C++-kanaalnummer bepaalt geen veilige voedingsspanning.
- Deze sensor gebruikt de analoge `SensorAnaloog`-route. De elektrode-/sensorprint wordt gevoed volgens de module, de analoge OUT gaat naar een passende ADC-ingang.
- Voorkom langdurige elektrolyse en corrosie bij continu gevoede geleidende watersensoren. De gemeten ADC-waarde moet voor de concrete watertoepassing worden gekalibreerd.

**Bestaande constructor (parameter-/pinvolgorde):**

```cpp
WATER(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t sensorPin, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
```

**Configuratie voor deze hardware:**

- Gebruik bij een eigen configuratie `src/Configuratie/Sensoren/ConfigWATER.h` (het aanwezige `src/Configuratie/Sensoren/ConfigWATER_template.h` is een kopieersjabloon en wordt niet rechtstreeks ingelezen).
- Voor één aangesloten exemplaar: `SENSOR_WATER_AANTAL` op **1**, behalve wanneer de bestaande groeps-/setconfiguratie uitdrukkelijk een andere betekenis heeft.
- De bestaande configuratie bevat deze aansluitrelevante definities (gebruik de **werkelijke aangesloten** pinnen, adressen en referenties):
  - `SENSOR_WATER_AANTAL`: uitgecommentarieerd sjabloonvoorstel `1`; zonder actieve override geldt de fallback `0` uit `SystemConfig.h`.
  - `SENSOR_WATER_ADC_BEREIK_MILLIVOLT`: sjabloonwaarde `5000`.

**Letterlijk constructorfragment uit de bestaande voorbeeldsketch** (alle gebruikte parent-objecten moeten zelf correct aangesloten en geïnitialiseerd zijn):

```cpp
GedeeldeBusNewComponent<WATER>(
    &extenderADS1115, GedeeldeBusComponent::WATER,
    static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN0),
    HardwareResourceToegang::GEDEELD);
```

**Bestaande test-/voorbeeldsketches:**

- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_WATER/Sensor_WATER.ino`

**Elektrische aansluiting en randvoorwaarden volgens Seeed Studio (Grove Water Sensor v1.1):**

- De **Grove Water Sensor v1.1** werkt volgens Seeed met **4,75–5,25 V** en een geleidbaarheidsspoor met **1 MΩ pull-up**. Bij droog contact blijft het signaal hoog en nat contact beïnvloedt het signaal. Verbind `SIG` met de analoge of digitale ingang die de bestaande FrameWorkconstructor werkelijk gebruikt, en `GND` met massa.
- Een willekeurige driepins WATER-sensor, regenplaat of module met comparator heeft niet automatisch dezelfde connector, voedingsspanning of polariteit als de Grove Water Sensor. Controleer het feitelijk gebruikte model voordat je 5 V aanbiedt.

**Technische handleiding:** https://wiki.seeedstudio.com/Grove-Water_Sensor/


**Alleen bij de Seeed Grove Water Sensor v1.1:** de fabrikant specificeert **4,75–5,25 V** voeding en een geleidbaarheidsspoor met **1 MΩ pull-up**. De sensor is dus niet zonder nadere controle op een 3,3V-rail te voeden. Verbind `SIG` met het door de FrameWorkconstructor geselecteerde **analoge ADC-kanaal** en `GND` met de analoge systeemmassa; beveilig zo nodig de ADC wanneer diens ingang geen 5V-signaal verdraagt. Verwar deze geleidbaarheidssensor niet met de volledig andere I²C-gebaseerde Seeed Grove Water Level Sensor.

**Officiële boardhandleiding:** https://wiki.seeedstudio.com/Grove-Water_Sensor/

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_WATER/`.
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.

