# GSRGrove

## Implementatie

- Broncode: `src/Systeem/Sensor/GSRGrove.h` (geen `.cpp`).
- C++-component: `GSRGrove`, afgeleid van `SensorAnaloog`.
- Parent: `GedeeldeBusNode*` in de constructor.
- Componentcode: `GSRGrove` (0xA3).
- Configuratie: `src/Configuratie/Sensoren/ConfigGSRGrove_template.h`; de optionele persoonlijke versie heet telkens zonder `_template`.
- Data-kanalen: 1 (via SensorAnaloog).

## Uit de broncode

GSRGrove: Grove GSR analoge uitgang, uitsluitend ADC-waarde. Eén component voor één fysiek apparaat, via de bestaande ADC-Extender. De meetcode zit in SensorAnaloog; deze klasse geeft alleen de pin en het bereik door.

## Aansluiten

**Aansluitwijze in de bestaande FrameWork-code:**

- **Meetverbinding:** sensor-uitgang naar het toegewezen **ADC-Extender-kanaal**. Verbind de signaalreferentie/GND met de ADC-Extender volgens de vereisten van die Extender.
- **Voeding:** controleer de benodigde spanning en toegestane OUT-spanning van de **specifieke sensor of module** vóór aansluiten; de FrameWork-code bepaalt de elektrische grenswaarden van het moduleboard niet.

- Analoge signaaluitgang van de Grove GSR → gekozen ADC-Extender-kanaal.
- Grove-modulevoeding/GND volgens de concrete module; gebruik de juiste pinvolgorde van de Grove-kabel.

**Voor de hardwaretest:**

- Gebruik de bestaande `.ino` uit **Testen** en de bestaande configuratiebestanden onder `src/Configuratie/`; een `*_template.h` wordt niet rechtstreeks ingelezen.
- **Een vaste Arduino-pintoewijzing is niet algemeen geldig:** de gekozen constructor, configuratie en werkelijke bekabeling moeten overeenkomen. Waar hierboven geen exact spanningsniveau of pinheader is bevestigd, mag dat niet uit deze documentatie worden afgeleid.


**Aanvulling aansluitpinnen en elektrische voorwaarden:**

- Grove-connector: **Seeed Grove GSR** zwart→GND, rood→5V in hun Arduino-voorbeeld, geel→analoge A0, wit→NC. De fabrikant specificeert bedrijf op 3,3V of 5V; gebruik de passende voeding en zorg dat de ADC-ingang de analoge OUT-spanning verdraagt. Elektrodes worden aan de GSR-module aangesloten.
- **Officiële bron voor de hierboven benoemde chip of breakout:** https://wiki.seeedstudio.com/Grove-GSR_Sensor/

**Aansluit- en gebruiksgegevens uit de bestaande FrameWork-interface:**

- **Analoge bekabeling voor GSRGrove:** het analoge meetpunt of `OUT` gaat naar het ADC-kanaal `sensorPin` van de opgegeven parent. Gebruik het **kanaal van die ADC-Extender**, niet automatisch een native Arduino-`A0`-pin. Verbind de elektrische meetreferentie met de GND van dezelfde ADC-opstelling.
- De ADC-ingang mag niet buiten zijn werkelijk ingestelde spanningsbereik worden gestuurd. Sensorvoeding en eventuele spanningsdeler horen bij het specifieke sensorbord of het losse element; een C++-kanaalnummer bepaalt geen veilige voedingsspanning.
- De sensor gebruikt de analoge `SensorAnaloog`-route. Gebruik de analoge uitgang van de concrete GSR-module, een geschikte ADC-ingang en gemeenschappelijke massa.
- Voor elektroden aan het lichaam gelden isolatie en veilige voeding; de analoge meetwaarde is niet zonder kalibratie een medische of klinische grootheid.

**Bestaande constructor (parameter-/pinvolgorde):**

```cpp
GSRGrove(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t sensorPin, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
```

**Configuratie voor deze hardware:**

- Gebruik bij een eigen configuratie `src/Configuratie/Sensoren/ConfigGSRGrove.h` (het aanwezige `src/Configuratie/Sensoren/ConfigGSRGrove_template.h` is een kopieersjabloon en wordt niet rechtstreeks ingelezen).
- Voor één aangesloten exemplaar: `SENSOR_GSRGROVE_AANTAL` op **1**, behalve wanneer de bestaande groeps-/setconfiguratie uitdrukkelijk een andere betekenis heeft.
- De bestaande configuratie bevat deze aansluitrelevante definities (gebruik de **werkelijke aangesloten** pinnen, adressen en referenties):
  - `SENSOR_GSRGROVE_AANTAL`: uitgecommentarieerd sjabloonvoorstel `1`; zonder actieve override geldt de fallback `0` uit `SystemConfig.h`.
  - `SENSOR_GSRGROVE_ADC_BEREIK_MILLIVOLT`: sjabloonwaarde `5000`.

**Letterlijk constructorfragment uit de bestaande voorbeeldsketch** (alle gebruikte parent-objecten moeten zelf correct aangesloten en geïnitialiseerd zijn):

```cpp
GedeeldeBusNewComponent<GSRGrove>(
    &extenderADS1115, GedeeldeBusComponent::GSRGrove,
    static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN0),
    HardwareResourceToegang::GEDEELD);
```

**Bestaande test-/voorbeeldsketches:**

- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_GSRGrove/Sensor_GSRGrove.ino`

**Elektrische aansluiting en randvoorwaarden volgens Seeed Studio:**

- Bij de **Grove-GSR-module** zijn de vier Grove-aders volgens Seeed: zwart = GND, rood = voeding, geel = **analoge uitgang**, wit = niet verbonden. De module ondersteunt 3,3 V/5 V en heeft instelbare gevoeligheid. Geel gaat in dit FrameWork naar het gekozen **ADC-Extender-kanaal**, niet automatisch naar native A0.
- Seeed verduidelijkt dat de sensor in deze opstelling **weerstand en niet rechtstreeks huidgeleiding** meet. Gebruik elektroden enkel volgens de instructies van de werkelijk gebruikte module.

**Technische handleiding:** https://wiki.seeedstudio.com/Grove-GSR_Sensor/

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_GSRGrove/`.
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.

### Schaal en interpretatie
- De bestaande ADC-waarde is geen automatisch gekalibreerde huidgeleiding in µS. Controleer spanning, effectieve ADC-referentie, bekende testweerstand en plausibele meetrange voordat een eenheid of interpretatie wordt toegekend.
- **Status:** referentiemetingen open.
