# SCD41

## Implementatie

- Broncode: `src/Systeem/Sensor/SCD41.h` en `SCD41.cpp`.
- C++-component: `SCD41`, afgeleid van `Sensor`.
- Parent: `HardwareResourceTypeI2C*` in de constructor.
- Componentcode: `SCD41` (0xA3).
- Configuratie: `src/Configuratie/Sensoren/ConfigSCD41_template.h`; de optionele persoonlijke versie heet telkens zonder `_template`.
- Data-kanalen: 3.
- Externe library: `SensirionI2cScd4x.h`.

## Aansluiten

**Aansluitwijze in de bestaande FrameWork-code:**

- **I²C:** module-**SDA → SDA** en module-**SCL → SCL** van de geselecteerde I²C-bus of het juiste TCA9548A-kanaal.
- **Voeding:** module-VCC en GND verbinden volgens de **specificatie van het concrete breakoutboard** en de logicaspanning van het gebruikte Arduino-board. Controleer level shifting en I²C-pull-ups op de module.
- **Adres:** de I²C-`HardwareResourceTypeI2C`-parent moet hetzelfde hardwareadres gebruiken als de fysieke module. Controleer eventuele adresselectiepinnen of soldeerbruggen.

**Voor de hardwaretest:**

- Gebruik de bestaande `.ino` uit **Testen** en de bestaande configuratiebestanden onder `src/Configuratie/`; een `*_template.h` wordt niet rechtstreeks ingelezen.
- **Een vaste Arduino-pintoewijzing is niet algemeen geldig:** de gekozen constructor, configuratie en werkelijke bekabeling moeten overeenkomen. Waar hierboven geen exact spanningsniveau of pinheader is bevestigd, mag dat niet uit deze documentatie worden afgeleid.


**Aanvulling aansluitpinnen en elektrische voorwaarden:**

- SDA→SDA, SCL→SCL, GND→GND; I²C-adres 0x62. **Sensirion SCD41-chip**: toegestane voeding 2,4–5,5V volgens officiële productspecificatie; I²C-logicaniveaus/pull-ups en breakout-inrichting afzonderlijk controleren.
- **Officiële bron voor de hierboven benoemde chip of breakout:** https://sensirion.com/products/catalog/SCD41

**Aansluit- en gebruiksgegevens uit de bestaande FrameWork-interface:**

- **I²C-bekabeling voor SCD41:** `SDA` van de module → `SDA` van de I²C-parent; `SCL` → `SCL` van dezelfde parent; `GND` → gemeenschappelijke massa. De parent krijgt het werkelijk geselecteerde I²C-adres (of de juiste TCA9548A-tak).
- De voedingspin van de module gaat naar een geschikte voedingsrail volgens de **concrete chip- of breakoutvariant**. Een 5V-controller maakt niet automatisch alle sensor-I/O 5V-tolerant; controleer de aanwezige pull-ups en eventuele levelshifters.
- Deze sensor gebruikt I²C en de hoofdwaarde via `LeesHoofdwaarde(float&)`. Controleer de correcte SDA/SCL-lijnen, modulevoeding en de voor de gebruikte SCD41-uitvoering vereiste meetopstarttijd.

**Bestaande constructor (parameter-/pinvolgorde):**

```cpp
SCD41(HardwareResourceTypeI2C* parent, GedeeldeBusComponent component)
```

**Configuratie voor deze hardware:**

- Gebruik bij een eigen configuratie `src/Configuratie/Sensoren/ConfigSCD41.h` (het aanwezige `src/Configuratie/Sensoren/ConfigSCD41_template.h` is een kopieersjabloon en wordt niet rechtstreeks ingelezen).
- Voor één aangesloten exemplaar: `SENSOR_SCD41_AANTAL` op **1**, behalve wanneer de bestaande groeps-/setconfiguratie uitdrukkelijk een andere betekenis heeft.
- De bestaande configuratie bevat deze aansluitrelevante definities (gebruik de **werkelijke aangesloten** pinnen, adressen en referenties):
  - `SENSOR_SCD41_AANTAL`: sjabloonwaarde `1`.
  - `I2C_ADDRESS_SENSOR_SCD41_1`: sjabloonwaarde `I2C_ADDRESS_NONE`.

**Letterlijk constructorfragment uit de bestaande voorbeeldsketch** (alle gebruikte parent-objecten moeten zelf correct aangesloten en geïnitialiseerd zijn):

```cpp
GedeeldeBusNewComponent<SCD41>(&apparaat,GedeeldeBusComponent::SCD41);
```

**Bestaande test-/voorbeeldsketches:**

- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_SCD41/Sensor_SCD41.ino`

**Elektrische aansluiting en randvoorwaarden volgens Sensirion:**

- De SCD41 is een I²C-CO₂-sensor; de bestaande FrameWorkklasse gebruikt de I²C-parent, niet een analoog kanaal. Voor het losse Sensirion-element moeten de daadwerkelijke voedings- en logicaspanningen uit het bij de aangekochte behuizing passende datasheet worden toegepast.

**Technische handleiding:** https://sensirion.com/products/catalog/SCD41

### Periodieke CO₂-meting

`Activeren()` stopt een eventueel nog lopende periodieke meting, herinitialiseert de sensor en start de periodieke modus. `LeesHoofdwaarde(float&)` geeft alleen bij `getDataReadyStatus()` met nieuwe beschikbare gegevens een CO₂-waarde in ppm terug; geen nieuwe waarde betekent `false`, maar is niet op zichzelf een communicatiefout. Een geldige uitlezing schrijft bij actieve Data CO₂ (kanaal 1), temperatuur (kanaal 2) en vochtigheid (kanaal 3).

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_SCD41/`.
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.

### Herstart tijdens ingeschakelde sensorvoeding

Bij `Activeren()` stopt de bestaande periodieke meetmodus eerst via `stopPeriodicMeasurement()` en herlaadt `reinit()` de in de sensor bewaarde instellingen, vóór `startPeriodicMeasurement()` opnieuw start. De Sensirion-library wacht daarbij ongeveer 500 ms respectievelijk 30 ms. Zowel een koude start als een herstart terwijl de sensor gevoed blijft moeten fysiek worden getest; de eerste CO₂-waarde volgt pas na de meetperiode.

### Aanvullende Sensor × Extender-testvoorbeelden

De onderstaande `.ino`-bestanden behoren tot de huidige v2.0.0-voorbeelden. Hun aanwezigheid is gecontroleerd; compilatie en fysieke hardwarevalidatie zijn hier nog **niet** aangetoond.

- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_SCD41_TCA9548A/Sensor_SCD41_TCA9548A.ino`

### Functionele Sensor × Extender-voorbeelden

De onderstaande voorbeelden bevatten **meetaanroepen na de activering**. Een getoonde waarde is geen bewijs van meetnauwkeurigheid of van een geslaagde hardwaretest.

- **Fysieke test:** Controleer nieuwe CO2-waarde; Data toont ook temp en vocht.
- **Huidige reikwijdte:** de `.ino` gebruikt uitsluitend reeds bestaande FrameWork-functies; niet-beschikbare meetkanalen, kalibraties of externe functies zijn niet toegevoegd.
- **Meerkanalige meting:** voor alle kanalen `DATA_LAYER_CONFIG` op `DATA_TYPE_DIRECT` configureren. De `.ino` maakt dan de bestaande Data-component aan en leest de kanalen uit. Met de standaardwaarde `DATA_TYPE_NONE` blijft de secundaire kanaaltest onvolledig.

**Voorbeelden:**

- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_SCD41_TCA9548A/Sensor_SCD41_TCA9548A.ino`

**Validatie:** Arduino-compilatie, fysieke test, kalibratie en meetkwaliteitsvalidatie zijn hier nog niet aangetoond.

## Native meetvoorbeeld

`examples/Systeem/GedeeldeBus/Sensoren/Sensor_SCD41/Sensor_SCD41.ino` voert na de lifecycle daadwerkelijk uitlezingen uit via de bestaande Sensor-API.
Gemeten of weergegeven in dit voorbeeld: **CO2 (ppm)**.
De seriële uitvoer wordt beperkt; de Sensor-uitlezing blijft in `loop()` plaatsvinden.
Dit bevestigt bij geslaagde hardwaretests de ruwe of rechtstreeks beschikbare meetwaarde, niet automatisch alle fysische kanalen, kalibratie of nauwkeurigheid.
Compilatie en fysieke meting zijn nog niet bevestigd.
