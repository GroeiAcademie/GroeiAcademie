# EGBOPulseSensor

## Implementatie

- Broncode: `src/Systeem/Sensor/EGBOPulseSensor.h` (geen `.cpp`).
- C++-component: `EGBOPulseSensor`, afgeleid van `SensorAnaloog`.
- Parent: `GedeeldeBusNode*` in de constructor.
- Componentcode: `EGBOPulseSensor` (0xA2).
- Configuratie: `src/Configuratie/Sensoren/ConfigEGBOPulseSensor_template.h`; de optionele persoonlijke versie heet telkens zonder `_template`.
- Data-kanalen: 1 (via SensorAnaloog).

## Aansluiten

**Aansluitwijze in de bestaande FrameWork-code:**

- **Meetverbinding:** sensor-uitgang naar het toegewezen **ADC-Extender-kanaal**. Verbind de signaalreferentie/GND met de ADC-Extender volgens de vereisten van die Extender.
- **Voeding:** controleer de benodigde spanning en toegestane OUT-spanning van de **specifieke sensor of module** vóór aansluiten; de FrameWork-code bepaalt de elektrische grenswaarden van het moduleboard niet.

- Analoge uitgang → het gekozen analoge kanaal van de ADC-Extender; controleer de fysieke pinlabels van de gebruikte PulseSensor-uitvoering.

**Voor de hardwaretest:**

- Gebruik de bestaande `.ino` uit **Testen** en de bestaande configuratiebestanden onder `src/Configuratie/`; een `*_template.h` wordt niet rechtstreeks ingelezen.
- **Een vaste Arduino-pintoewijzing is niet algemeen geldig:** de gekozen constructor, configuratie en werkelijke bekabeling moeten overeenkomen. Waar hierboven geen exact spanningsniveau of pinheader is bevestigd, mag dat niet uit deze documentatie worden afgeleid.


**Aanvulling aansluitpinnen en elektrische voorwaarden:**

- SIGNAL→ADC-kanaal; GND→GND; VCC→door fabrikant voor exact EGBO Pulse Sensor-module voorgeschreven voedingsspanning. Sensor en ADC moeten een gemeenschappelijke referentie hebben; geen maximaal OUT-niveau aannemen.
- De concrete modulevariant bepaalt de **exacte voedingsgrenzen en fysieke pinvolgorde**. Neem die gegevens niet over van een andere module zonder de werkelijke print te controleren.

**Aansluit- en gebruiksgegevens uit de bestaande FrameWork-interface:**

- **Analoge bekabeling voor EGBOPulseSensor:** het analoge meetpunt of `OUT` gaat naar het ADC-kanaal `sensorPin` van de opgegeven parent. Gebruik het **kanaal van die ADC-Extender**, niet automatisch een native Arduino-`A0`-pin. Verbind de elektrische meetreferentie met de GND van dezelfde ADC-opstelling.
- De ADC-ingang mag niet buiten zijn werkelijk ingestelde spanningsbereik worden gestuurd. Sensorvoeding en eventuele spanningsdeler horen bij het specifieke sensorbord of het losse element; een C++-kanaalnummer bepaalt geen veilige voedingsspanning.
- Dit sensortype gebruikt de bestaande analoge `SensorAnaloog`-route: OUT naar een geschikte analoge ingang of ondersteunde ADC-parent, plus GND en modulevoeding conform de print.
- Laat het analoge signaal binnen het ingestelde ADC-bereik blijven en maak geen directe verbinding met een onveilig netgevoed meetsysteem bij lichaamscontact.

**Bestaande constructor (parameter-/pinvolgorde):**

```cpp
EGBOPulseSensor(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t sensorPin, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
```

**Configuratie voor deze hardware:**

- Gebruik bij een eigen configuratie `src/Configuratie/Sensoren/ConfigEGBOPulseSensor.h` (het aanwezige `src/Configuratie/Sensoren/ConfigEGBOPulseSensor_template.h` is een kopieersjabloon en wordt niet rechtstreeks ingelezen).
- Voor één aangesloten exemplaar: `SENSOR_EGBOPULSESENSOR_AANTAL` op **1**, behalve wanneer de bestaande groeps-/setconfiguratie uitdrukkelijk een andere betekenis heeft.
- De bestaande configuratie bevat deze aansluitrelevante definities (gebruik de **werkelijke aangesloten** pinnen, adressen en referenties):
  - `SENSOR_EGBOPULSESENSOR_AANTAL`: sjabloonwaarde `1`.
  - `SENSOR_EGBOPULSESENSOR_ADC_BEREIK_MILLIVOLT`: sjabloonwaarde `5000`.

**Letterlijk constructorfragment uit de bestaande voorbeeldsketch** (alle gebruikte parent-objecten moeten zelf correct aangesloten en geïnitialiseerd zijn):

```cpp
GedeeldeBusNewComponent<EGBOPulseSensor>(&extenderADS1115,
    GedeeldeBusComponent::EGBOPulseSensor,
    static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN0),
    HardwareResourceToegang::GEDEELD);
```

**Bestaande test-/voorbeeldsketches:**

- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_EGBOPulseSensor/Sensor_EGBOPulseSensor.ino`


### Praktische aansluiting en gebruik

**Concreet aansluiten:** de FrameWork-voorbeeldsketch sluit de sensor aan op `ExtenderADS1115::ExtenderPins::EP_AIN0` van `extenderADS1115`. Verbind de sensoruitgang `SIGNAL/OUT` met de fysieke **AIN0** van die ADS1115-uitvoering, de sensormassa met de analoge GND en de voedingsdraad met de door de concrete sensor en ADC toegelaten voeding. `SENSOR_EGBOPULSESENSOR_ADC_BEREIK_MILLIVOLT` uit de configuratie is het **meetbereik in software**, geen veilige voedingsspanning.

- **Als** de feitelijke sensor een originele *Pulse Sensor Amped* van World Famous Electronics is, geldt volgens de maker: **paars = analoog signaal**, **rood = voeding**, **zwart = GND**. De maker noemt een voeding van 3 tot 5 V, afhankelijk van het board. Dit is **geen bewezen identificatie** van elke module die in de FrameWork `EGBOPulseSensor` heet. Een andere EGBO-uitvoering moet op eigen printlabels worden aangesloten.
- Bij gebruik op de huid: voor het genoemde Pulse Sensor Amped-bord adviseert de maker isolerende beschermingsfolie op de achterkant en voorkant waar nodig. Gebruik geen open elektroden of ongeïsoleerde, netgevoede schakeling op de huid.

**Technische documentatie voor uitsluitend de genoemde Pulse Sensor Amped-uitvoering:** https://pulsesensor.com/pages/start en https://pulsesensor.com/pages/open-hardware.


**Originele Pulse Sensor Amped, alleen indien dit exact de gebruikte sensor is:** volgens de maker loopt de **paarse draad naar de analoge ADC-ingang**, de **rode draad naar een door de sensor toegelaten 3–5V-voeding** en de **zwarte draad naar GND**. De oorspronkelijke FrameWorksketch gebruikt `EP_AIN0` op ADS1115; sluit dus paars aan op de fysieke **AIN0**, niet op een willekeurige Arduino-D-pin. Controleer dat de spanning op die ingang binnen de toegestane ADS1115-ingangsspanning blijft. Deze fabrikant is **geen** bevestiging dat een andere als EGBOPulseSensor verkochte print dezelfde kabelkleuren heeft.

**Makerhandleiding:** https://pulsesensor.com/pages/start

### Aansluiting van de ronde optische pulssensor

De gebruikte ronde, paarse optische sensorprint heeft **drie aansluitvlakken**, gemarkeerd `S`, `+` en `−`:

- `S` → analoge sensoruitgang → het door de bestaande `EGBOPulseSensor`-constructor gekozen ADC-Extender-kanaal. In de voorbeeldsketch is dat **AIN0 op ADS1115**.
- `+` → positieve sensorvoeding; `−` → gemeenschappelijke massa met de ADC-opstelling. De originele Pulse Sensor Amped, waarop deze bouwvorm lijkt, is volgens de maker ontworpen voor **3–5 V**. Controleer vóór inschakelen dat de gebruikte print dezelfde elektrische uitvoering heeft.
- Een analoge uitgang moet steeds **binnen het toelaatbare ingangsspanningsbereik van de ADC** blijven. De configuratie van het softwaremeetbereik verhoogt de toelaatbare ADC-pinspanning niet.
- Bij huidcontact moeten achterzijde, soldeervlakken en overige elektronica afgeschermd zijn. Gebruik een elektrisch geïsoleerde meetopstelling, niet een onbeveiligde netgevoede voeding.

De FrameWork leest de analoge waarde; het feit dat de print een hartslaggolfsignaal kan leveren betekent **niet** dat elke gemeten waarde al een berekende hartslag in bpm is. Fabrikantinformatie over de analoge Pulse Sensor Amped: https://pulsesensor.com/pages/open-hardware

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_EGBOPulseSensor/`.
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.

