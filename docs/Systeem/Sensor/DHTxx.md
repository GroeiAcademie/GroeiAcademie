# DHTxx

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
- De aanwezigheid van deze rubriek is **geen bewijs van een geslaagde hardwaretest**.

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_DHTxx/` (zet `SENSOR_DHT11_AANTAL` of `SENSOR_DHT22_AANTAL` op 1).
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.
Dit document bevestigt geen geslaagde hardwaretest.
