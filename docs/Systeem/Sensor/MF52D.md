# MF52D

## Sensor en gedeelde implementatie

Fysieke MF52D NTC-probe. Componentcode `MF52D` (0xD8), klasse `MF52x`. De spanningsdeler wordt op een ADC-kanaal uitgelezen.

Deze sensor deelt een C++-klasse met andere uitvoeringen. De fysieke aansluitgegevens gelden alleen voor `MF52D` waar dat uitdrukkelijk vermeld is.

---

## Gedeelde C++-klasse `src/Systeem/Sensor/MF52x.h`

### MF52x

## Implementatie

- Broncode: `src/Systeem/Sensor/MF52x.h` (geen `.cpp`).
- C++-component: `MF52x`, afgeleid van `SensorAnaloog`.
- Parent: `GedeeldeBusNode*` in de constructor.
- Componentcode: `MF52A` (0xD7), `MF52D` (0xD8).
- Configuratie: `src/Configuratie/Sensoren/ConfigMF52A_template.h`, `src/Configuratie/Sensoren/ConfigMF52D_template.h`; de optionele persoonlijke versie heet telkens zonder `_template`.
- Data-kanalen: 1 (via SensorAnaloog).

## Uit de broncode

MF52D: NTC thermistor; spanning, niet omgerekende temperatuur. Eén component voor één fysiek apparaat, via de bestaande ADC-Extender. De meetcode zit in SensorAnaloog; deze klasse geeft alleen de pin en het bereik door. Het fysieke type behoudt zijn eigen componentcode en configuratie. De analoge ADC-meetfuncties zijn identiek aan die van MF52A.

## Aansluiten

**Aansluitwijze in de bestaande FrameWork-code:**

- **Meetverbinding:** sensor-uitgang naar het toegewezen **ADC-Extender-kanaal**. Verbind de signaalreferentie/GND met de ADC-Extender volgens de vereisten van die Extender.
- **Voeding:** controleer de benodigde spanning en toegestane OUT-spanning van de **specifieke sensor of module** vóór aansluiten; de FrameWork-code bepaalt de elektrische grenswaarden van het moduleboard niet.

- NTC-thermistor → **spanningsdeler** met een passende bekende weerstand, spanning van de deler → analoge ingang van de ADC-Extender.
- Een losse MF52A/MF52D is een weerstandssensor, geen module met rechtstreeks gestuurde OUT. Zonder spanningsdeler is de analoge waarde niet betrouwbaar interpreteerbaar.

**Voor de hardwaretest:**

- Gebruik de bestaande `.ino` uit **Testen** en de bestaande configuratiebestanden onder `src/Configuratie/`; een `*_template.h` wordt niet rechtstreeks ingelezen.
- **Een vaste Arduino-pintoewijzing is niet algemeen geldig:** de gekozen constructor, configuratie en werkelijke bekabeling moeten overeenkomen. Waar hierboven geen exact spanningsniveau of pinheader is bevestigd, mag dat niet uit deze documentatie worden afgeleid.


**Aanvulling aansluitpinnen en elektrische voorwaarden:**

- MF52A/MF52D zijn tweepolige NTC-weerstanden: vorm een spanningsdeler met een vaste weerstand en lees het middenpunt op een ADC-kanaal. De NTC heeft geen afzonderlijke VCC- en GND-pinnen; voedingsrail/weerstandswaarde moeten bij het bereik passen.
- De concrete modulevariant bepaalt de **exacte voedingsgrenzen en fysieke pinvolgorde**. Neem die gegevens niet over van een andere module zonder de werkelijke print te controleren.

**Aansluit- en gebruiksgegevens uit de bestaande FrameWork-interface:**

- **Analoge bekabeling voor MF52D:** het analoge meetpunt of `OUT` gaat naar het ADC-kanaal `sensorPin` van de opgegeven parent. Gebruik het **kanaal van die ADC-Extender**, niet automatisch een native Arduino-`A0`-pin. Verbind de elektrische meetreferentie met de GND van dezelfde ADC-opstelling.
- De ADC-ingang mag niet buiten zijn werkelijk ingestelde spanningsbereik worden gestuurd. Sensorvoeding en eventuele spanningsdeler horen bij het specifieke sensorbord of het losse element; een C++-kanaalnummer bepaalt geen veilige voedingsspanning.
- Een los weerstandselement levert geen eigen analoge uitgangsspanning: bouw de bijbehorende **spanningsdeler**, sluit het middenpunt op het ADC-kanaal aan en houd de hele meetspanning binnen het bereik van de ADC. De keuze van vaste weerstand en voeding bepaalt de meting.
- De gegroepeerde `MF52x`-klasse gebruikt een analoog meetkanaal. Gebruik voor het MF52D-element de passende weerstandsdeler en de voor deze variant geldende weerstands-/temperatuurcurve; neem niet automatisch de parameters van MF52A over.

**Bestaande constructor (parameter-/pinvolgorde):**

```cpp
MF52x(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t sensorPin, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
```

**Configuratie voor deze hardware:**

- Gebruik bij een eigen configuratie `src/Configuratie/Sensoren/ConfigMF52D.h` (het aanwezige `src/Configuratie/Sensoren/ConfigMF52D_template.h` is een kopieersjabloon en wordt niet rechtstreeks ingelezen).
- Voor één aangesloten exemplaar: `SENSOR_MF52D_AANTAL` op **1**, behalve wanneer de bestaande groeps-/setconfiguratie uitdrukkelijk een andere betekenis heeft.
- De bestaande configuratie bevat deze aansluitrelevante definities (gebruik de **werkelijke aangesloten** pinnen, adressen en referenties):
  - `SENSOR_MF52D_AANTAL`: uitgecommentarieerd sjabloonvoorstel `1`; zonder actieve override geldt de fallback `0` uit `SystemConfig.h`.
  - `SENSOR_MF52D_ADC_BEREIK_MILLIVOLT`: sjabloonwaarde `5000`.

**Letterlijk constructorfragment uit de bestaande voorbeeldsketch** (alle gebruikte parent-objecten moeten zelf correct aangesloten en geïnitialiseerd zijn):

```cpp
GedeeldeBusNewComponent<MF52x>(
    &extenderADS1115, GedeeldeBusComponent::MF52D,
    static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN0),
    HardwareResourceToegang::GEDEELD);
```

**Bestaande test-/voorbeeldsketches:**

- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_MF52D/Sensor_MF52D.ino`


### Praktische aansluiting en gebruik

**Voor een losse MF52D-NTC volgens de bestaande FrameWorkroute:** maak een **spanningsdeler**, bijvoorbeeld `VCC → vaste weerstand → meetknooppunt → NTC → GND`, en verbind het meetknooppunt met `sensorPin` van de opgegeven ADC-Extender. Beide aansluitdraden van een los NTC-element zijn **weerstandsaansluitingen**, geen VCC en OUT. Een eventuele print met extra elektronica kan andere labels hebben.

- De klasse `MF52x` gebruikt bij componenttype `MF52D` uitsluitend `SENSOR_MF52D_ADC_BEREIK_MILLIVOLT` voor het analoge ADC-bereik; de waarde is nog **geen temperatuur in graden**. De exacte `R25`, B-waarde, zelfopwarming en temperatuurlimieten hangen van de aangekochte MF52D-uitvoering af.
- De algemene MF52-seriegegevens van Songtian mogen niet automatisch als typeblad voor een **MF52D** worden toegepast: `MF52D` is hiermee niet als een specifieke Songtian-variant geïdentificeerd.

**Algemene MF52-serie als technisch achtergrondmateriaal (niet als MF52D-pinout):** https://www.songtian-ste.com/en/products/ntc-thermistors/mf52/


**MF52D is niet automatisch elektrisch gelijk aan een specifieke MF52A-waarde.** De letters A en D kunnen bij NTC-fabrikanten verschillende uitvoeringen van de aansluitdraden aanduiden; Nearer Technology beschrijft bijvoorbeeld MF52A met vertinde aansluitdraden en MF52D met geïsoleerde tweeaderige kabel. De **R25**, **B-waarde**, weerstandstolerantie en maximale toegelaten dissipatie moeten aan het volledige onderdeelnummer worden ontleend. De twee sensordraden vormen een polariteitsloos weerstandselement en moeten als NTC-tak in de spanningsdeler, niet rechtstreeks als `VCC`/`OUT`, worden aangesloten.

**Fabrikantdocumentatie voor de benoemde MF52A/MF52D-uitvoeringen:** https://nearer-tech.com/products%20PDF/NTC/MF52.pdf

### Aansluiting van het gebruikte MF52D-element

Voor het gebruikte **MF52D**-element zijn **`R25 = 10 kΩ`** en **`B = 3950 K`** bevestigd. Het is een **tweepolige NTC-weerstand zonder polariteit**.

- Voor de bestaande analoge `MF52x`-route: bouw bijvoorbeeld **VCC → vaste weerstand 10 kΩ → meetknooppunt → MF52D → GND**. Verbind het meetknooppunt met het gekozen ADC-Extender-kanaal en gebruik gemeenschappelijke GND.
- Bij **25 °C** ligt het meetknooppunt bij twee ideale 10kΩ-elementen op ongeveer **VCC/2**; controleer dat de totale spanning binnen het toegelaten ADC-bereik valt.
- **B3950** dient voor een eventuele latere weerstand-naar-temperatuurconversie; de bestaande `MF52x`-klasse rekent de analoge waarde **niet** zelf om naar °C. Gebruik de B-waarde dus niet als impliciet aanwezige softwarefunctie.
- Vermijd mechanische trekkracht op aansluitdraden en zelfverhitting door een te grote meetstroom.

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_MF52A/`, `examples/Systeem/GedeeldeBus/Sensoren/Sensor_MF52D/`.
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.

