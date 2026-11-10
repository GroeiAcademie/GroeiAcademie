# DHT11

## Sensor en gedeelde implementatie

Fysiek DHT11-type (eventueel KY-015-module). Componentcode `DHT11` (0xD1); C++-klasse blijft `DHTxx`. DATA vereist een native GPIO-pin.

Deze sensor deelt een C++-klasse met andere uitvoeringen. De fysieke aansluitgegevens gelden alleen voor `DHT11` waar dat uitdrukkelijk vermeld is.

---

## Aansluiten: DHT11

### Aansluitgegevens: losse DHT11-vierpinsensor

- **Voeding VCC:** 3 tot 5 V volgens Adafruit.
- **GND:** gemeenschappelijke massa.
- **Fysieke vierpinsversie, van voren op de sensor bekeken volgens het Adafruit-aansluitschema:** pin 1 = VCC; pin 2 = DATA; pin 3 = niet aangesloten; pin 4 = GND.
- **DATA:** verbind met een native digitale ingang van de microcontroller, overeenkomstig de bestaande `np_DATA`-configuratie. Niet via een GPIO-Extender.
- **Pull-up:** Adafruit adviseert een weerstand van 10 kΩ tussen DATA en de passende logische voedingsrail; op sommige moduleprinten is die al ingebouwd.
- **DHT11-module/KY-015:** de exacte header-volgorde en aanwezigheid van een pull-up van het concrete moduleboard is hier niet officieel bevestigd en mag niet zonder controle met de losse vierpinsensor worden gelijkgesteld.

**Officiële bronnen:** [Adafruit, Connecting to a DHTxx Sensor](https://learn.adafruit.com/dht/connecting-to-a-dhtxx-sensor), [Adafruit, Overview](https://learn.adafruit.com/dht)

## Gedeelde C++-klasse `src/Systeem/Sensor/DHTxx.h`

### DHTxx

## Implementatie

- Broncode: `src/Systeem/Sensor/DHTxx.h` en `DHTxx.cpp`.
- C++-component: `DHTxx`, afgeleid van `Sensor`.
- Parent: `GedeeldeBusNode*` in de constructor.
- Componentcode: `DHT11` (0xD1), `DHT22` (0xD2). De componentcode kiest het fysieke type: `DHT22` stelt de DHT-library in op de DHT22, elke andere code op de DHT11.
- Configuratie: `src/Configuratie/Sensoren/ConfigDHTxx_template.h` met `SENSOR_DHT11_AANTAL` en `SENSOR_DHT22_AANTAL` (elk 0 of 1); de optionele persoonlijke versie heet telkens zonder `_template`.
- Data-kanalen: 2 (temperatuur en relatieve vochtigheid).
- Externe library: `DHT.h`.

## Uit de broncode

De DHT11 (module KY-015) en de DHT22 gebruiken hetzelfde protocol op één datapin; alleen het type in de DHT-library verschilt. De DHT22 meet nauwkeuriger en over een groter bereik, maar mag hooguit één meting per 2 seconden doen. Het uitlezen gebeurt met native timing en interrupts uit (ongeveer 5 ms): niet via een generieke GPIO-Extender.

## Aansluiten

**Aansluitwijze in de bestaande FrameWork-code:**

- De signaalverbindingen worden hieronder per onderdeel gespecificeerd waar ze uit de bestaande componentinterface blijken.
- Controleer voedingsspanning, pinvolgorde en externe stroombegrenzing bij de concrete hardware-uitvoering voordat je spanning aanlegt.

- DATA → één **native** Arduino-pin (`np_DATA`), niet via een GPIO-Extender: de DHT-library heeft nauwkeurige timing nodig.
- Controleer of de module al een DATA-pull-up heeft; voor een losse DHT-sensor is een geschikte pull-up nodig.
- DHT11/KY-015 en DHT22 gebruiken dezelfde DATA-topologie; kies het juiste type in de configuratie.

**Voor de hardwaretest:**

- Gebruik de bestaande `.ino` uit **Testen** en de bestaande configuratiebestanden onder `src/Configuratie/`; een `*_template.h` wordt niet rechtstreeks ingelezen.
- **Een vaste Arduino-pintoewijzing is niet algemeen geldig:** de gekozen constructor, configuratie en werkelijke bekabeling moeten overeenkomen. Waar hierboven geen exact spanningsniveau of pinheader is bevestigd, mag dat niet uit deze documentatie worden afgeleid.


**Aanvulling aansluitpinnen en elektrische voorwaarden:**

- DATA→native digitale GPIO; GND→GND; VCC→voeding die de specifieke DHT11/DHT22-uitvoering toestaat. Een losse sensor kan een externe DATA-pull-up vereisen; een moduleboard kan die al hebben. De fysieke pinvolgorde verschilt per losse sensor en breakout.
- De concrete modulevariant bepaalt de **exacte voedingsgrenzen en fysieke pinvolgorde**. Neem die gegevens niet over van een andere module zonder de werkelijke print te controleren.

**Aansluit- en gebruiksgegevens uit de bestaande FrameWork-interface:**

- **DATA** van de DHT11 → de native digitale pin `np_DATA` die aan `DHTxx` wordt gegeven; sensor-GND en host-GND samen. Gebruik de juiste voeding en DATA-pull-up voor de gebruikte losse sensor of module.
- `DHTxx` gebruikt één digitale datapin `np_DATA`; deze fysieke variant wordt geselecteerd door haar eigen `GedeeldeBusComponent`, niet door een andere C++-klasse.
- Gebruik de DATA-aansluiting, de correcte modulevoeding en de bij de gekozen sensoruitvoering vereiste pull-up; houd DHT11 en DHT22 qua timing en meetbereik uit elkaar.

**Bestaande constructor (parameter-/pinvolgorde):**

```cpp
DHTxx(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin np_DATA)
```

**Configuratie voor deze hardware:**

- Gebruik bij een eigen configuratie `src/Configuratie/Sensoren/ConfigDHTxx.h` (het aanwezige `src/Configuratie/Sensoren/ConfigDHTxx_template.h` is een kopieersjabloon en wordt niet rechtstreeks ingelezen).
- Voor één aangesloten exemplaar: `SENSOR_DHT11_AANTAL` op **1**, behalve wanneer de bestaande groeps-/setconfiguratie uitdrukkelijk een andere betekenis heeft.
- De bestaande configuratie bevat deze aansluitrelevante definities (gebruik de **werkelijke aangesloten** pinnen, adressen en referenties):
  - `SENSOR_DHT11_AANTAL`: uitgecommentarieerd sjabloonvoorstel `1`; zonder actieve override geldt de fallback `0` uit `SystemConfig.h`.
  - `SENSOR_DHT22_AANTAL`: uitgecommentarieerd sjabloonvoorstel `1`; zonder actieve override geldt de fallback `0` uit `SystemConfig.h`.

**Letterlijk constructorfragment uit de bestaande voorbeeldsketch** (alle gebruikte parent-objecten moeten zelf correct aangesloten en geïnitialiseerd zijn):

```cpp
GedeeldeBusNewComponent<DHTxx>(&Native, fysiekType, HardwareResourcePin::NP_D2);
```

**Bestaande test-/voorbeeldsketches:**

- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_DHTxx/Sensor_DHTxx.ino`

**Elektrische aansluiting en randvoorwaarden volgens Adafruit (DHTxx-aansluithandleiding):**

- De **losse vierpins DHTxx-sensor** wordt door Adafruit weergegeven als, in de juiste aanzichtoriëntatie, pin 1 = VCC (3,3–5 V), pin 2 = DATA, pin 3 = niet aangesloten en pin 4 = GND. Verbind DATA met `np_DATA` uit de bestaande FrameWorkconstructor.
- Bij een losse sensor hoort gewoonlijk een **10 kΩ-pull-up tussen DATA en VCC**; een kant-en-klare driepinsmodule kan die weerstand reeds op de print hebben.

**Technische handleiding:** https://learn.adafruit.com/dht/connecting-to-a-dhtxx-sensor

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_DHTxx/` (zet `SENSOR_DHT11_AANTAL` of `SENSOR_DHT22_AANTAL` op 1).
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.

