# DHT11

## Sensor en gedeelde implementatie

Fysiek DHT11-type (eventueel KY-015-module). Componentcode `DHT11` (0xCA); C++-klasse blijft `DHTxx`. DATA wordt rechtstreeks op een native GPIO-pin gelezen, of via een directe CD74HC4067-parent op diens native Z-pin. Een gewone GPIO-Extender wordt niet ondersteund.

Deze sensor deelt een C++-klasse met andere uitvoeringen. De fysieke aansluitgegevens gelden alleen voor `DHT11` waar dat uitdrukkelijk vermeld is.

---

## Aansluiten: DHT11

### Aansluitgegevens: losse DHT11-vierpinsensor

- **Voeding VCC:** 3 tot 5 V volgens Adafruit.
- **GND:** gemeenschappelijke massa.
- **Fysieke vierpinsversie, van voren op de sensor bekeken volgens het Adafruit-aansluitschema:** pin 1 = VCC; pin 2 = DATA; pin 3 = niet aangesloten; pin 4 = GND.
- **DATA:** verbind rechtstreeks met de native digitale ingang `np_DATA`, of met kanaal `np_DATA` van een directe CD74HC4067-parent waarvan de Z-pin op een native ingang aangesloten is. Niet via een gewone GPIO-Extender.
- **Pull-up:** Adafruit adviseert een weerstand van 10 kΩ tussen DATA en de passende logische voedingsrail; op sommige moduleprinten is die al ingebouwd.
- **DHT11-module/KY-015:** de exacte header-volgorde en aanwezigheid van een pull-up van het concrete moduleboard is hier niet officieel bevestigd en mag niet zonder controle met de losse vierpinsensor worden gelijkgesteld.

**Officiële bronnen:** [Adafruit, Connecting to a DHTxx Sensor](https://learn.adafruit.com/dht/connecting-to-a-dhtxx-sensor), [Adafruit, Overview](https://learn.adafruit.com/dht)

## Gedeelde C++-klasse `src/Systeem/Sensor/DHTxx.h`

### DHTxx

## Implementatie

- Broncode: `src/Systeem/Sensor/DHTxx.h` en `DHTxx.cpp`.
- C++-component: `DHTxx`, afgeleid van `Sensor`.
- Parent: `GedeeldeBusNode*` in de constructor.
- Componentcode: `DHT11` (0xCA), `DHT22` (0xCB). De componentcode kiest het fysieke type: `DHT22` stelt de DHT-library in op de DHT22, elke andere code op de DHT11.
- Configuratie: `src/Configuratie/Sensoren/ConfigDHTxx_template.h` met `SENSOR_DHT11_AANTAL` en `SENSOR_DHT22_AANTAL` (elk 0 of 1); de optionele persoonlijke versie heet telkens zonder `_template`.
- Data-kanalen: 2 (temperatuur en relatieve vochtigheid).
- Externe library: `DHT.h`.

## Uit de broncode

De DHT11 (module KY-015) en de DHT22 gebruiken hetzelfde protocol op één datapin; alleen het type in de DHT-library verschilt. De DHT22 meet nauwkeuriger en over een groter bereik, maar mag hooguit één meting per 2 seconden doen. Het uitlezen gebeurt met native timing en interrupts uit (ongeveer 5 ms): niet via een generieke GPIO-Extender. De bestaande uitzondering is een directe CD74HC4067, die het signaal fysiek naar een native Z-pin doorschakelt.

## Aansluiten

**Aansluitwijze in de bestaande FrameWork-code:**

- De signaalverbindingen worden hieronder per onderdeel gespecificeerd waar ze uit de bestaande componentinterface blijken.
- Controleer voedingsspanning, pinvolgorde en externe stroombegrenzing bij de concrete hardware-uitvoering voordat je spanning aanlegt.

- DATA → native Arduino-pin (`np_DATA`), of via een directe CD74HC4067-parent naar diens native Z-pin. Een gewone GPIO-Extender is wegens de DHT-timing niet geschikt.
- Controleer of de module al een DATA-pull-up heeft; voor een losse DHT-sensor is een geschikte pull-up nodig.
- DHT11/KY-015 en DHT22 gebruiken dezelfde DATA-topologie; kies het juiste type in de configuratie.

**Voor de hardwaretest:**

- Gebruik de bestaande `.ino` uit **Testen** en de bestaande configuratiebestanden onder `src/Configuratie/`; een `*_template.h` wordt niet rechtstreeks ingelezen.
- **Een vaste Arduino-pintoewijzing is niet algemeen geldig:** de gekozen constructor, configuratie en werkelijke bekabeling moeten overeenkomen. Waar hierboven geen exact spanningsniveau of pinheader is bevestigd, mag dat niet uit deze documentatie worden afgeleid.


**Aanvulling aansluitpinnen en elektrische voorwaarden:**

- DATA→native digitale GPIO, of via het geselecteerde kanaal van een directe CD74HC4067-parent naar diens native Z-pin; GND→GND; VCC→passende modulevoeding. Een losse sensor kan een externe DATA-pull-up vereisen; een moduleboard kan die al hebben. De fysieke pinvolgorde verschilt per losse sensor en breakout.
- De concrete modulevariant bepaalt de **exacte voedingsgrenzen en fysieke pinvolgorde**. Neem die gegevens niet over van een andere module zonder de werkelijke print te controleren.

**Aansluit- en gebruiksgegevens uit de bestaande FrameWork-interface:**

- **DATA** van de DHT11 → de native digitale pin `np_DATA`, of bij een directe CD74HC4067-parent naar het gekozen kanaal `np_DATA` met een native aangesloten Z-pin; sensor-GND en host-GND samen. Gebruik de juiste voeding en DATA-pull-up.
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
  - `SENSOR_DHT11_AANTAL`: sjabloonwaarde `1`.
  - `SENSOR_DHT22_AANTAL`: sjabloonwaarde `1`.

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

### Meting en Data-kanalen

`LeesHoofdwaarde(float&)` leest de temperatuur in °C met de gekozen DHT-driver. `false` betekent dat de teruggegeven temperatuur geen geldige meting is (`NAN`). Bij een geldige meting bewaart Data de temperatuur op kanaal 1; de functie leest daarna ook de relatieve vochtigheid en bewaart die op kanaal 2, mits de vochtigheidswaarde niet `NAN` is. De code gebruikt geen afzonderlijke openbare FrameWork-uitleesfunctie voor vochtigheid.

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_DHTxx/` (zet `SENSOR_DHT11_AANTAL` of `SENSOR_DHT22_AANTAL` op 1).
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.

## Hulppinnen en routes via Extenders

- Via een CD74HC4067: DATA op een kanaal; de driver werkt op de native Z-pin. Het kanaal blijft open: één digitale child per CD74HC4067. De library meet hooguit om de 2 s.
