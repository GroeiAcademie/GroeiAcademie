# PIEZO

## Implementatie

- Broncode: `src/Systeem/Sensor/PIEZO.h` (geen `.cpp`).
- C++-component: `PIEZO`, afgeleid van `SensorAnaloog`.
- Parent: `GedeeldeBusNode*` in de constructor.
- Componentcode: `PIEZO` (0x83).
- Configuratie: `src/Configuratie/Sensoren/ConfigPIEZO_template.h`; de optionele persoonlijke versie heet telkens zonder `_template`.
- Data-kanalen: 1 (via SensorAnaloog).

## Uit de broncode

PIEZO leest een elektrisch beveiligd/geconditioneerd analoog signaal via `SensorAnaloog`. Daarnaast houdt de klasse een piek sinds de vorige uitlezing bij (`ReadPeakSinceLastRead()`) en detecteert ze tikken via de ingestelde drempel (`ReadTapDetected()`). Bij een MIXED-aansluiting kan ook DO op een tweede Extender worden gelezen met `ReadDO()`; dit vervangt de analoge beveiliging niet.

## Aansluiten

**Aansluitwijze in de bestaande FrameWork-code:**

- **Meetverbinding:** sensor-uitgang naar het toegewezen **ADC-Extender-kanaal**. Verbind de signaalreferentie/GND met de ADC-Extender volgens de vereisten van die Extender.
- **Voeding:** controleer de benodigde spanning en toegestane OUT-spanning van de **specifieke sensor of module** vóór aansluiten; de FrameWork-code bepaalt de elektrische grenswaarden van het moduleboard niet.

- Het piezo-element mag **niet zonder bescherming** rechtstreeks naar de ADC-Extender: gebruik een geschikte begrenzings-/conditioneringsschakeling.
- De beveiligde analoge uitgang → ADC-kanaal; gemeenschappelijke referentie-GND.

**Voor de hardwaretest:**

- Gebruik de bestaande `.ino` uit **Testen** en de bestaande configuratiebestanden onder `src/Configuratie/`; een `*_template.h` wordt niet rechtstreeks ingelezen.
- **Een vaste Arduino-pintoewijzing is niet algemeen geldig:** de gekozen constructor, configuratie en werkelijke bekabeling moeten overeenkomen. Waar hierboven geen exact spanningsniveau of pinheader is bevestigd, mag dat niet uit deze documentatie worden afgeleid.


**Aanvulling aansluitpinnen en elektrische voorwaarden:**

- Ruwe piezo-elementen kunnen hoge spanningstransiënten genereren; niet rechtstreeks onbeschermd op een ADC aansluiten. Gebruik de bestaande analoge ingang met passende begrenzing/ontlading/signaalconditionering en gemeenschappelijke referentie; een passief element heeft doorgaans geen VCC-pin.
- De concrete modulevariant bepaalt de **exacte voedingsgrenzen en fysieke pinvolgorde**. Neem die gegevens niet over van een andere module zonder de werkelijke print te controleren.

**Aansluit- en gebruiksgegevens uit de bestaande FrameWork-interface:**

- **Analoge bekabeling voor PIEZO:** het analoge meetpunt of `OUT` gaat naar het ADC-kanaal `sensorPin` van de opgegeven parent. Gebruik het **kanaal van die ADC-Extender**, niet automatisch een native Arduino-`A0`-pin. Verbind de elektrische meetreferentie met de GND van dezelfde ADC-opstelling.
- De ADC-ingang mag niet buiten zijn werkelijk ingestelde spanningsbereik worden gestuurd. Sensorvoeding en eventuele spanningsdeler horen bij het specifieke sensorbord of het losse element; een C++-kanaalnummer bepaalt geen veilige voedingsspanning.
- De PIEZO-klasse gebruikt `SensorAnaloog` en een geschikt analoog kanaal. Piëzo-elementen kunnen korte, relatief hoge spanningspieken opwekken: bescherm de ADC-ingang elektrisch.

**Bestaande constructor (parameter-/pinvolgorde):**

```cpp
PIEZO(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t sensorPin, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
```

**Configuratie voor deze hardware:**

- Gebruik bij een eigen configuratie `src/Configuratie/Sensoren/ConfigPIEZO.h` (het aanwezige `src/Configuratie/Sensoren/ConfigPIEZO_template.h` is een kopieersjabloon en wordt niet rechtstreeks ingelezen).
- Voor één aangesloten exemplaar: `SENSOR_PIEZO_AANTAL` op **1**, behalve wanneer de bestaande groeps-/setconfiguratie uitdrukkelijk een andere betekenis heeft.
- De bestaande configuratie bevat deze aansluitrelevante definities (gebruik de **werkelijke aangesloten** pinnen, adressen en referenties):
  - `SENSOR_PIEZO_AANTAL`: sjabloonwaarde `1`.
  - `SENSOR_PIEZO_ADC_BEREIK_MILLIVOLT`: sjabloonwaarde `5000`.

**Letterlijk constructorfragment uit de bestaande voorbeeldsketch** (alle gebruikte parent-objecten moeten zelf correct aangesloten en geïnitialiseerd zijn):

```cpp
GedeeldeBusNewComponent<PIEZO>(
    &extenderADS1115, GedeeldeBusComponent::PIEZO,
    static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN0),
    HardwareResourceToegang::GEDEELD);
```

**Bestaande test-/voorbeeldsketches:**

- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_PIEZO/Sensor_PIEZO.ino`


### Praktische aansluiting en gebruik

**Een los tweepolig piezo-element is geen gevoede analoge sensormodule.** Door tikken of buigen kan een piezo-element spanningspieken met beide polariteiten opwekken. Verbind het element daarom **niet rechtstreeks** met een kwetsbare ADC-ingang. Voor de bestaande `SensorAnaloog`-route is een externe conditionering vereist die het signaal begrenst en binnen het toegelaten bereik van de gekozen ADC-Extender brengt.

- Een kant-en-klare piezo-vibratiemodule met `VCC`, `GND`, `AO` of `DO` wordt aangesloten volgens **die specifieke module**. Gebruik voor deze analoge FrameWorkklasse uitsluitend de geconditioneerde analoge uitgang, niet een willekeurig digitaal DO-contact.
- De bestaande constructor registreert het ADC-kanaal als `sensorPin`; daarmee zijn **geen** serieweerstand, ingangsbeveiliging of modulevoeding elektrisch vastgelegd. Gebruik de specificatie van het werkelijk gebruikte piezo-element en ADC-bord om de beveiliging te dimensioneren.


**Uitvoering bepaalt het aansluitschema.** Een passief tweedraads piëzo-element produceert bij buigen of slaan elektrische spanning; het is **geen** sensor die zonder meer op een VCC-pin gevoed wordt. Voor de specifieke Adafruit-piëzofilmsensor met twee aansluitingen toont de leverancier als voorbeeld: één aansluiting naar GND, de andere naar de analoge meetingang, met een **1 MΩ weerstand over de twee sensoraansluitingen**. Dit is uitsluitend een signaaluitleesvoorbeeld voor dat element, **geen gegarandeerd veilige ADC-bescherming** tegen piekspanningen van een willekeurige piezoschijf. Dimensioneer overspanningsbegrenzing en ingangsconditionering voor het werkelijke element en de ADC.

**Fabrikant-/productvoorbeeld:** https://www.adafruit.com/product/4931 ; **ontwerpachtergrond signaalconditionering:** https://www.analog.com/en/resources/technical-articles/signal-conditioning-for-high-impedance-sensors.html

### Aansluiting van de gebruikte piezomodule met comparator

Naast het tweedraads piezo-element is een **blauwe sensormodule** aanwezig waarop `AO`, `DO`, `GND` en `VCC` bij de vierpolige connector zijn gedrukt. De module heeft een instelpotmeter en een piezo-opnemer met twee draden.

- **Voor de bestaande analoge FrameWork-`PIEZO`-klasse:** module-`AO` → gekozen ADC-Extender-kanaal, `GND` → ADC-GND en `VCC` → de voor de **module** toegelaten voeding. `DO` is een **aparte digitale comparatoruitgang** en wordt door de bestaande analoge `PIEZO`-klasse **niet** automatisch gelezen.
- Bij een **los tweepolig piezo-element** zonder deze module is een geschikte ingangsbeveiliging en signaalconditionering nodig; sluit het element dus niet op dezelfde wijze rechtstreeks aan als de `AO`-uitgang van de gevoede module. Met `GEDEELDE_BUS_MIXED_PINS` kan de comparator-DO daarnaast via een tweede geschikte Extender gelezen worden, maar dat vraagt de aparte MIXED-constructor.
- Controleer de maximale module-`AO`-spanning voor de werkelijk ingestelde ADC-range. De instelpotmeter beïnvloedt doorgaans de digitale detectiedrempel, niet een al in de FrameWork aanwezige nieuwe functie.

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_PIEZO/`.
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.

### Aanvullende Sensor × Extender-testvoorbeelden

De onderstaande `.ino`-bestanden behoren tot de huidige v2.0.0-voorbeelden. Hun aanwezigheid is gecontroleerd; compilatie en fysieke hardwarevalidatie zijn hier nog **niet** aangetoond.

- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_PIEZO_ADS1158/Sensor_PIEZO_ADS1158.ino`
- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_PIEZO_ADS7828/Sensor_PIEZO_ADS7828.ino`
- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_PIEZO_ADS7953/Sensor_PIEZO_ADS7953.ino`

### Functionele Sensor × Extender-voorbeelden

De onderstaande voorbeelden bevatten **meetaanroepen na de activering**. Een getoonde waarde is geen bewijs van meetnauwkeurigheid of van een geslaagde hardwaretest.

- **Fysieke test:** Tik op de sensor en observeer de snelle pieken in de ADC-samples.
- **Huidige reikwijdte:** de `.ino` gebruikt uitsluitend reeds bestaande FrameWork-functies; niet-beschikbare meetkanalen, kalibraties of externe functies zijn niet toegevoegd.

**Voorbeelden:**

- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_PIEZO_ADS1158/Sensor_PIEZO_ADS1158.ino`
- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_PIEZO_ADS7828/Sensor_PIEZO_ADS7828.ino`
- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_PIEZO_ADS7953/Sensor_PIEZO_ADS7953.ino`

**Validatie:** Arduino-compilatie, fysieke test, kalibratie en meetkwaliteitsvalidatie zijn hier nog niet aangetoond.

### Uitleesstatus bij de ADC-Extender-tests

De ADC-Extender-voorbeelden gebruiken de reeds bestaande `RawAnalogRead(int32_t& waarde)` om een meting uit te voeren en **het succes van die uitlezing afzonderlijk te controleren**. Een ADC-waarde van `0` blijft zo een geldige meetwaarde wanneer de functie `true` teruggeeft; bij `false` wordt een uitleesfout gemeld. Hiervoor is geen Data-kanaal nodig.

Dit controleert de ADC-uitleesstatus, niet de fysieke meetnauwkeurigheid, de samplefrequentie of de sensorspecifieke kalibratie.

## Hulppinnen en routes via Extenders

- `ReadDO(bool& waarde)`: DO van de comparator lezen via de tweede Extender (mixed). Welk niveau een tik is, hangt af van de module.
- Mixed constructor: `PIEZO(parent, component, sensorPin, extender, np_DO)`. Vereist `GEDEELDE_BUS_MIXED_PINS`.

## Piek en tik

- `ReadPeakSinceLastRead(int32_t&)`: hoogste ADC-waarde sinds de vorige oproep (elke geslaagde meting telt mee).
- `ReadTapDetected(bool&)`: tik als die piek op of boven `SENSOR_PIEZO_TAP_THRESHOLD_MILLIVOLT` (standaard 500) ligt. Beide gebruiken dezelfde piek: kies één van beide.
- Voorbeeld: `Sensor_PIEZO_Verwerking.ino`.

## Heractivatie

- `Activeren()` wist de bewaarde piek (`peakSinceLastRead = INT32_MIN`): `ReadPeakSinceLastRead()` geeft geen piek uit een vorige sessie.

## Na een herstel

- `Activeren()` zet de oude meting in elke configuratie terug, ook zonder MIXED of zonder GPIO op de Extender.
