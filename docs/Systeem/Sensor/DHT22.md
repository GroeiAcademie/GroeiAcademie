# DHT22

## Sensor en gedeelde implementatie

Fysiek DHT22-type. Componentcode `DHT22` (0xD2); C++-klasse blijft `DHTxx`. DATA vereist een native GPIO-pin. In de bestaande tekst staat maximaal één meting per twee seconden.

Deze sensor deelt een C++-klasse met andere uitvoeringen. De fysieke aansluitgegevens gelden alleen voor `DHT22` waar dat uitdrukkelijk vermeld is.

---

## Aansluiten: DHT22

### Aansluitgegevens: losse DHT22-vierpinsensor

- **Voeding VCC:** 3 tot 5 V volgens Adafruit.
- **GND:** gemeenschappelijke massa.
- **Fysieke vierpinsversie, van voren op de sensor bekeken volgens het Adafruit-aansluitschema:** pin 1 = VCC; pin 2 = DATA; pin 3 = niet aangesloten; pin 4 = GND.
- **DATA:** naar de native digitale ingang die als `np_DATA` is ingesteld, niet via een GPIO-Extender.
- **Pull-up:** Adafruit adviseert 10 kΩ van DATA naar de passende logische voedingsrail. Bij sommige DHT22/AM2302-uitvoeringen is die al aanwezig.
- **Meetinterval:** maximaal één uitlezing per twee seconden volgens Adafruit; deze beperking is ook in de oorspronkelijke FrameWork-documentatie opgenomen.
- Een bekabelde AM2302-uitvoering of moduleboard heeft mogelijk een andere fysieke aansluiting; deze vierpinsvolgorde niet zonder meer toepassen.

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

- **DATA** van de DHT22 → de native digitale pin `np_DATA` die aan `DHTxx` wordt gegeven; sensor-GND en host-GND samen. Controleer de op de gebruikte print aanwezige of nog benodigde DATA-pull-up.
- `DHTxx` gebruikt één digitale datapin `np_DATA`; voor DHT22 blijft de componentcode afzonderlijk van DHT11, ondanks de gedeelde klasse.
- DATA gaat naar de gekozen native digitale pin; controleer of de gebruikte kale sensor of breakout al een pull-up op DATA bevat.

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

- De **losse vierpins DHT22** heeft volgens de Adafruit-handleiding, gezien vanaf de juiste voorkant, pin 1 = VCC, pin 2 = DATA, pin 3 = niet gebruikt en pin 4 = GND. Verbind DATA met de native `np_DATA` van de huidige constructor.
- Een **10 kΩ-DATA-pull-up** is gebruikelijk, tenzij de concrete module die reeds bevat. De pinvolgorde van een driepinsbreakout kan afwijken van de vierpins-sensor.

**Technische handleiding:** https://learn.adafruit.com/dht/connecting-to-a-dhtxx-sensor

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_DHTxx/` (zet `SENSOR_DHT11_AANTAL` of `SENSOR_DHT22_AANTAL` op 1).
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.

