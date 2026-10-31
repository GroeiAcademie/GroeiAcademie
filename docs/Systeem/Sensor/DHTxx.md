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

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_DHTxx/` (zet `SENSOR_DHT11_AANTAL` of `SENSOR_DHT22_AANTAL` op 1).
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.
Dit document bevestigt geen geslaagde hardwaretest.
