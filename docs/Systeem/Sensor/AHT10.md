# AHT10

## Implementatie

- Broncode: `src/Systeem/Sensor/AHT10.h` en `AHT10.cpp`.
- C++-component: `AHT10`, afgeleid van `Sensor`.
- Parent: `HardwareResourceTypeI2C*` in de constructor.
- Componentcode: `AHT10` (0xD0).
- Configuratie: `src/Configuratie/Sensoren/ConfigAHT10_template.h`; de optionele persoonlijke versie heet telkens zonder `_template`.
- Data-kanalen: 2.
- Externe library: `Adafruit_AHTX0.h`.

## Uit de broncode

Officiële API: https://github.com/adafruit/Adafruit_AHTX0/blob/master/Adafruit_AHTX0.h

## Aansluiten

**Aansluitwijze in de bestaande FrameWork-code:**

- **I²C:** module-**SDA → SDA** en module-**SCL → SCL** van de geselecteerde I²C-bus of het juiste TCA9548A-kanaal.
- **Voeding:** module-VCC en GND verbinden volgens de **specificatie van het concrete breakoutboard** en de logicaspanning van het gebruikte Arduino-board. Controleer level shifting en I²C-pull-ups op de module.
- **Adres:** de I²C-`HardwareResourceTypeI2C`-parent moet hetzelfde hardwareadres gebruiken als de fysieke module. Controleer eventuele adresselectiepinnen of soldeerbruggen.

**Voor de hardwaretest:**

- Gebruik de bestaande `.ino` uit **Testen** en de bestaande configuratiebestanden onder `src/Configuratie/`; een `*_template.h` wordt niet rechtstreeks ingelezen.
- **Een vaste Arduino-pintoewijzing is niet algemeen geldig:** de gekozen constructor, configuratie en werkelijke bekabeling moeten overeenkomen. Waar hierboven geen exact spanningsniveau of pinheader is bevestigd, mag dat niet uit deze documentatie worden afgeleid.


**Aanvulling aansluitpinnen en elektrische voorwaarden:**

- SDA→SDA, SCL→SCL; I²C-standaardadres 0x38. **Adafruit AHT20-breakout**, niet automatisch elke AHT10-module: VIN→3,3V of 5V overeenkomstig controllerlogica, GND→GND; op deze breakout zitten 10kΩ I²C-pull-ups. De exacte AHT10-module en eventuele 0x39-adresoptie blijven variantafhankelijk.
- **Officiële bron voor de hierboven benoemde chip of breakout:** https://learn.adafruit.com/adafruit-aht20/pinouts

**Aansluit- en gebruiksgegevens uit de bestaande FrameWork-interface:**

- **I²C-bekabeling voor AHT10:** `SDA` van de module → `SDA` van de I²C-parent; `SCL` → `SCL` van dezelfde parent; `GND` → gemeenschappelijke massa. De parent krijgt het werkelijk geselecteerde I²C-adres (of de juiste TCA9548A-tak).
- De voedingspin van de module gaat naar een geschikte voedingsrail volgens de **concrete chip- of breakoutvariant**. Een 5V-controller maakt niet automatisch alle sensor-I/O 5V-tolerant; controleer de aanwezige pull-ups en eventuele levelshifters.
- De klasse gebruikt een `HardwareResourceTypeI2C*`-parent en leest de hoofdwaarde via `LeesHoofdwaarde(float&)`. Verbind SDA, SCL, VCC en GND volgens het gebruikte AHT10-breakoutboard.
- Gebruik de I²C-adresinstelling die bij de gebruikte module hoort; een adreswaarde in software maakt een hardwarematig niet aanwezig adres niet beschikbaar.

**Bestaande constructor (parameter-/pinvolgorde):**

```cpp
AHT10(HardwareResourceTypeI2C* parent, GedeeldeBusComponent component)
```

**Configuratie voor deze hardware:**

- Gebruik bij een eigen configuratie `src/Configuratie/Sensoren/ConfigAHT10.h` (het aanwezige `src/Configuratie/Sensoren/ConfigAHT10_template.h` is een kopieersjabloon en wordt niet rechtstreeks ingelezen).
- Voor één aangesloten exemplaar: `SENSOR_AHT10_AANTAL` op **1**, behalve wanneer de bestaande groeps-/setconfiguratie uitdrukkelijk een andere betekenis heeft.
- De bestaande configuratie bevat deze aansluitrelevante definities (gebruik de **werkelijke aangesloten** pinnen, adressen en referenties):
  - `SENSOR_AHT10_AANTAL`: sjabloonwaarde `1`.
  - `I2C_ADDRESS_SENSOR_AHT10_1`: sjabloonwaarde `I2C_ADDRESS_NONE`.

**Letterlijk constructorfragment uit de bestaande voorbeeldsketch** (alle gebruikte parent-objecten moeten zelf correct aangesloten en geïnitialiseerd zijn):

```cpp
GedeeldeBusNewComponent<AHT10>(&apparaat,GedeeldeBusComponent::AHT10);
```

**Bestaande test-/voorbeeldsketches:**

- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_AHT10/Sensor_AHT10.ino`

**Elektrische aansluiting en randvoorwaarden volgens Adafruit (AHT20-breakout en AHTx-interface):**

- De bestaande FrameWork gebruikt een I²C-parent voor AHT10. De officiële Adafruit **AHT20**-breakout heeft VIN/GND/SDA/SCL en adres **0x38**, met SDA/SCL-pull-ups op de breakout. Dit is geen bewijs dat elke **AHT10**-print dezelfde voeding en header heeft: de fysieke AHT10-adresselectie, voeding en pull-ups eerst aan die variant toetsen.

**Technische handleiding:** https://learn.adafruit.com/adafruit-aht20/pinouts

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_AHT10/`.
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.

