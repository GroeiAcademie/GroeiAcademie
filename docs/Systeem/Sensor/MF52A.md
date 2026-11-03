# MF52A

## Sensor en gedeelde implementatie

Fysieke MF52A NTC-probe. Componentcode `MF52A` (0xD7), klasse `MF52x`. De spanningsdeler wordt op een ADC-kanaal uitgelezen.

Deze sensor deelt een C++-klasse met andere uitvoeringen. De fysieke aansluitgegevens gelden alleen voor `MF52A` waar dat uitdrukkelijk vermeld is.

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

MF52A: NTC thermistor; spanning, niet omgerekende temperatuur. Eén component voor één fysiek apparaat, via de bestaande ADC-Extender. De meetcode zit in SensorAnaloog; deze klasse geeft alleen de pin en het bereik door. Het fysieke type behoudt zijn eigen componentcode en configuratie. De analoge ADC-meetfuncties zijn identiek aan die van MF52A.

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

- **Analoge bekabeling voor MF52A:** het analoge meetpunt of `OUT` gaat naar het ADC-kanaal `sensorPin` van de opgegeven parent. Gebruik het **kanaal van die ADC-Extender**, niet automatisch een native Arduino-`A0`-pin. Verbind de elektrische meetreferentie met de GND van dezelfde ADC-opstelling.
- De ADC-ingang mag niet buiten zijn werkelijk ingestelde spanningsbereik worden gestuurd. Sensorvoeding en eventuele spanningsdeler horen bij het specifieke sensorbord of het losse element; een C++-kanaalnummer bepaalt geen veilige voedingsspanning.
- Een los weerstandselement levert geen eigen analoge uitgangsspanning: bouw de bijbehorende **spanningsdeler**, sluit het middenpunt op het ADC-kanaal aan en houd de hele meetspanning binnen het bereik van de ADC. De keuze van vaste weerstand en voeding bepaalt de meting.
- De gegroepeerde `MF52x`-klasse gebruikt een analoog meetkanaal. Een NTC levert geen directe temperatuurspanning: meet een weerstandsdeler en gebruik de juiste kalibratie- of omzettingsparameters voor het concrete MF52A-element.

**Bestaande constructor (parameter-/pinvolgorde):**

```cpp
MF52x(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t sensorPin, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
```

**Configuratie voor deze hardware:**

- Gebruik bij een eigen configuratie `src/Configuratie/Sensoren/ConfigMF52A.h` (het aanwezige `src/Configuratie/Sensoren/ConfigMF52A_template.h` is een kopieersjabloon en wordt niet rechtstreeks ingelezen).
- Voor één aangesloten exemplaar: `SENSOR_MF52A_AANTAL` op **1**, behalve wanneer de bestaande groeps-/setconfiguratie uitdrukkelijk een andere betekenis heeft.
- De bestaande configuratie bevat deze aansluitrelevante definities (gebruik de **werkelijke aangesloten** pinnen, adressen en referenties):
  - `SENSOR_MF52A_AANTAL`: sjabloonwaarde `1`.
  - `SENSOR_MF52A_ADC_BEREIK_MILLIVOLT`: sjabloonwaarde `5000`.

**Letterlijk constructorfragment uit de bestaande voorbeeldsketch** (alle gebruikte parent-objecten moeten zelf correct aangesloten en geïnitialiseerd zijn):

```cpp
GedeeldeBusNewComponent<MF52x>(
    &extenderADS1115, GedeeldeBusComponent::MF52A,
    static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN0),
    HardwareResourceToegang::GEDEELD);
```

**Bestaande test-/voorbeeldsketches:**

- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_MF52A/Sensor_MF52A.ino`


### Praktische aansluiting en gebruik

**Voor een losse NTC-thermistor:** gebruik een **spanningsdeler**. Een concreet voorbeeld is `VCC → vaste weerstand → meetknooppunt → NTC → GND`; het meetknooppunt gaat naar `sensorPin` op de gekozen ADC-Extender. De NTC heeft geen eigen VCC- of OUT-pin. Wissel je NTC en vaste weerstand om, dan draait de richting van de spanningsverandering om. Kies voedingsrail en weerstandswaarden zodanig dat de ADC-ingang en de dissipatie van de NTC binnen hun grenzen blijven.

- De huidige `MF52x`-klasse geeft de analoge spanning van de ADC door; zij berekent **geen temperatuur in °C**. Voor echte temperatuurconversie zijn de exacte nominale weerstand (`R25`), B-waarde en kalibratie van de werkelijk gekozen thermistor nodig. Het naamdeel `MF52A` legt die parameters niet vast.
- De MF52-serie van Songtian vermeldt onder andere 10/50/100 kΩ R25-uitvoeringen, verschillende B-waarden en 52A als een uitvoering met bedrade aansluitdraden. Deze parameters gelden uitsluitend voor overeenkomende typenummers.

**Fabrikantgegevens voor de beschreven MF52-serie:** https://www.songtian-ste.com/en/products/ntc-thermistors/mf52/


**De juiste temperatuurberekening vraagt het exacte NTC-onderdeelnummer.** `MF52A` duidt niet automatisch een unieke nominale weerstand of B-waarde aan. Bij de gedocumenteerde Songtian-52A-serie bestaan onder meer uitvoeringen met R25 **10, 50 of 100 kΩ**, en B25/50 **3380 of 3950 K** afhankelijk van het bestelnummer. Sluit de losse twee draden zonder polariteit aan als de NTC-tak van de eerder beschreven spanningsdeler; gebruik voor een temperatuurwaarde de weerstand en de R/T-karakteristiek van het daadwerkelijk gemonteerde onderdeel. De FrameWorkdriver levert hier de ADC-meting; die is niet zonder meer een temperatuur in °C.

**Fabrikantdocumentatie voor deze serie:** https://www.songtian-ste.com/en/products/ntc-thermistors/mf52/

### Aansluiting van het gebruikte MF52A-element

Voor het gebruikte **MF52A / MF52AT**-element zijn de nominale weerstand **`R25 = 10 kΩ` bij 25 °C** en **`B = 3950 K`** bevestigd.

- Dit is een **tweepolige NTC-weerstand**, zonder polariteit en zonder eigen `VCC`-, `GND`- of `OUT`-contact.
- Voor de bestaande analoge `MF52x`-route: maak een spanningsdeler, bijvoorbeeld **VCC → vaste weerstand 10 kΩ → meetknooppunt → MF52A → GND**. Verbind het meetknooppunt met het gekozen ADC-Extender-kanaal; GND moet gemeenschappelijk zijn.
- Met een vaste weerstand van 10 kΩ ligt het knooppunt bij een ideaal 10kΩ-NTC-element op **ongeveer de helft van de voedingsspanning bij 25 °C**. Controleer zelfverhitting en het ingangsbereik van de ADC.
- De bestaande FrameWork-driver leest **ADC-spanning**, niet rechtstreeks °C. Voor een eventuele temperatuurconversie van dit element is **`B = 3950 K`** bevestigd; een gemeten R/T-kalibratie kan aanvullend gebruikt worden.

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_MF52A/`, `examples/Systeem/GedeeldeBus/Sensoren/Sensor_MF52D/`.
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.

