# VL53L0X

## Implementatie

- Broncode: `src/Systeem/Sensor/VL53L0X.h` en `VL53L0X.cpp`.
- C++-component: `SensorVL53L0X`, afgeleid van `Sensor`.
- Parent: `HardwareResourceTypeI2C*` in de constructor.
- Componentcode: `VL53L0X` (0x93).
- Configuratie: `src/Configuratie/Sensoren/ConfigVL53L0X_template.h`; de optionele persoonlijke versie heet telkens zonder `_template`.
- Data-kanalen: 1.
- Externe library: `Adafruit_VL53L0X.h`.

## Uit de broncode

Officiële API: https://github.com/adafruit/Adafruit_VL53L0X/blob/master/src/Adafruit_VL53L0X.h

## Aansluiten

**Aansluitwijze in de bestaande FrameWork-code:**

- **I²C:** module-**SDA → SDA** en module-**SCL → SCL** van de geselecteerde I²C-bus of het juiste TCA9548A-kanaal.
- **Voeding:** module-VCC en GND verbinden volgens de **specificatie van het concrete breakoutboard** en de logicaspanning van het gebruikte Arduino-board. Controleer level shifting en I²C-pull-ups op de module.
- **Adres:** de I²C-`HardwareResourceTypeI2C`-parent moet hetzelfde hardwareadres gebruiken als de fysieke module. Controleer eventuele adresselectiepinnen of soldeerbruggen.

**Voor de hardwaretest:**

- Gebruik de bestaande `.ino` uit **Testen** en de bestaande configuratiebestanden onder `src/Configuratie/`; een `*_template.h` wordt niet rechtstreeks ingelezen.
- **Een vaste Arduino-pintoewijzing is niet algemeen geldig:** de gekozen constructor, configuratie en werkelijke bekabeling moeten overeenkomen. Waar hierboven geen exact spanningsniveau of pinheader is bevestigd, mag dat niet uit deze documentatie worden afgeleid.


**Aanvulling aansluitpinnen en elektrische voorwaarden:**

- SDA→SDA, SCL→SCL, adres 0x29, SHDN/XSHUT optioneel voor adresinitialisatie. **Adafruit VL53L0X-breakout**: VIN→3–5V, GND→GND, regulator en levelshifters. Let op: GPIO/interrupt is niet level-shifted. Geen 5V op een kale VL53L0X-chip.
- **Officiële bron voor de hierboven benoemde chip of breakout:** https://learn.adafruit.com/adafruit-vl53l0x-micro-lidar-distance-sensor-breakout/pinouts

**Aansluit- en gebruiksgegevens uit de bestaande FrameWork-interface:**

- **I²C-bekabeling voor VL53L0X:** `SDA` van de module → `SDA` van de I²C-parent; `SCL` → `SCL` van dezelfde parent; `GND` → gemeenschappelijke massa. De parent krijgt het werkelijk geselecteerde I²C-adres (of de juiste TCA9548A-tak).
- De voedingspin van de module gaat naar een geschikte voedingsrail volgens de **concrete chip- of breakoutvariant**. Een 5V-controller maakt niet automatisch alle sensor-I/O 5V-tolerant; controleer de aanwezige pull-ups en eventuele levelshifters.
- De bestaande implementatie gebruikt een I²C-parent; module-SDA/SCL gaan naar de gekozen I²C-bus.
- Bij gebruik van meerdere gelijk geadresseerde modules zijn aparte kanalen of een expliciete adresoplossing nodig; controleer of de huidige code werkelijk een adreswijziging uitvoert.

**Bestaande constructor (parameter-/pinvolgorde):**

```cpp
SensorVL53L0X(HardwareResourceTypeI2C* parent, GedeeldeBusComponent component)
```

**Configuratie voor deze hardware:**

- Gebruik bij een eigen configuratie `src/Configuratie/Sensoren/ConfigVL53L0X.h` (het aanwezige `src/Configuratie/Sensoren/ConfigVL53L0X_template.h` is een kopieersjabloon en wordt niet rechtstreeks ingelezen).
- Voor één aangesloten exemplaar: `SENSOR_VL53L0X_AANTAL` op **1**, behalve wanneer de bestaande groeps-/setconfiguratie uitdrukkelijk een andere betekenis heeft.
- De bestaande configuratie bevat deze aansluitrelevante definities (gebruik de **werkelijke aangesloten** pinnen, adressen en referenties):
  - `SENSOR_VL53L0X_AANTAL`: uitgecommentarieerd sjabloonvoorstel `1`; zonder actieve override geldt de fallback `0` uit `SystemConfig.h`.
  - `I2C_ADDRESS_SENSOR_VL53L0X_1`: sjabloonwaarde `I2C_ADDRESS_NONE`.

**Letterlijk constructorfragment uit de bestaande voorbeeldsketch** (alle gebruikte parent-objecten moeten zelf correct aangesloten en geïnitialiseerd zijn):

```cpp
GedeeldeBusNewComponent<SensorVL53L0X>(&apparaat,GedeeldeBusComponent::VL53L0X);
```

**Bestaande test-/voorbeeldsketches:**

- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_VL53L0X/Sensor_VL53L0X.ino`

**Elektrische aansluiting en randvoorwaarden volgens STMicroelectronics:**

- De VL53L0X gebruikt I²C, met een optionele `XSHUT`-resetlijn en optionele interrupt. De huidige FrameWorkklasse vraagt een I²C-parent; de gekozen breakout moet een bij het hostboard passende voeding en levelshifting hebben.

**Technische handleiding:** https://www.st.com/en/imaging-and-photonics-solutions/vl53l0x

## Runtime-meetgedrag

`SensorVL53L0X::LeesHoofdwaarde(float&)` start een meting met `vl.startRange()` en retourneert dan `false` met `waarde = NAN`. Volgende aanroepen wachten zonder blokkerende lus via `vl.isRangeComplete()`. Is de meting nog niet voltooid binnen `tijdslimietMeting`, dan wordt de status gereset en de mislukking als runtime-toegang verwerkt. De tijdslimiet wordt bij `Activeren()` afgeleid van het meetbudget indien geldig, met een terugval van 200 ms. Na voltooiing worden API-status, rangesstatus en resultaat (`!= 0xFFFF`) gecontroleerd; alleen dan levert de functie een afstandswaarde. `false` terwijl de tijdslimiet nog niet is verstreken is een normale wachttoestand.

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_VL53L0X/`.
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.

