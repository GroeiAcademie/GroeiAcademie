# ADXL345

## Implementatie

- Broncode: `src/Systeem/Sensor/ADXL345.h` en `ADXL345.cpp`.
- C++-component: `SensorADXL345`, afgeleid van `Sensor`.
- Parent: `HardwareResourceTypeI2C*` in de constructor.
- Componentcode: `ADXL345` (0x95).
- Configuratie: `src/Configuratie/Sensoren/ConfigADXL345_template.h`; de optionele persoonlijke versie heet telkens zonder `_template`.
- Data-kanalen: 3.
- Externe library: `Adafruit_ADXL345_U.h`.

## Uit de broncode

De huidige sensor initialiseert en leest via de gekoppelde `HardwareResourceTypeI2C`-parent; het werkelijke adres van die parent moet met de fysieke sensor overeenkomen.

## Aansluiten

**Aansluitwijze in de bestaande FrameWork-code:**

- **I²C:** module-**SDA → SDA** en module-**SCL → SCL** van de geselecteerde I²C-bus of het juiste TCA9548A-kanaal.
- **Voeding:** module-VCC en GND verbinden volgens de **specificatie van het concrete breakoutboard** en de logicaspanning van het gebruikte Arduino-board. Controleer level shifting en I²C-pull-ups op de module.
- **Adres:** de I²C-`HardwareResourceTypeI2C`-parent moet hetzelfde hardwareadres gebruiken als de fysieke module. Controleer eventuele adresselectiepinnen of soldeerbruggen.

**Voor de hardwaretest:**

- Gebruik de bestaande `.ino` uit **Testen** en de bestaande configuratiebestanden onder `src/Configuratie/`; een `*_template.h` wordt niet rechtstreeks ingelezen.
- **Een vaste Arduino-pintoewijzing is niet algemeen geldig:** de gekozen constructor, configuratie en werkelijke bekabeling moeten overeenkomen. Waar hierboven geen exact spanningsniveau of pinheader is bevestigd, mag dat niet uit deze documentatie worden afgeleid.


**Aanvulling aansluitpinnen en elektrische voorwaarden:**

- I²C: SDA→SDA, SCL→SCL; SDO bepaalt 0x53/0x1D, CS moet voor I²C correct hoog liggen. **Uitsluitend Adafruit ADXL345-breakout**: VIN→3,3V bij 3,3V-logica of VIN→5V bij 5V-logica, GND→GND; deze breakout heeft spanningsregelaar en levelshifters. Een kale ADXL345-chip mag niet zomaar op 5V.
- **Officiële bron voor de hierboven benoemde chip of breakout:** https://learn.adafruit.com/adxl345-digital-accelerometer/pinouts

**Aansluit- en gebruiksgegevens uit de bestaande FrameWork-interface:**

- **I²C-bekabeling voor ADXL345:** `SDA` van de module → `SDA` van de I²C-parent; `SCL` → `SCL` van dezelfde parent; `GND` → gemeenschappelijke massa. De parent krijgt het werkelijk geselecteerde I²C-adres (of de juiste TCA9548A-tak).
- De voedingspin van de module gaat naar een geschikte voedingsrail volgens de **concrete chip- of breakoutvariant**. Een 5V-controller maakt niet automatisch alle sensor-I/O 5V-tolerant; controleer de aanwezige pull-ups en eventuele levelshifters.
- De huidige `SensorADXL345`-klasse heeft uitsluitend een `HardwareResourceTypeI2C*`-parent. Gebruik SDA/SCL en geen SPI voor deze klasse, ook al biedt de fysieke ADXL345-chip andere interfaces.
- De huidige `Activeren()` initialiseert de Adafruit-driver op het adres van de I²C-parent. `LeesHoofdwaarde(float&)` geeft de X-versnelling in m/s²; bij Data-opslag worden X, Y en Z afzonderlijk bewaard.

**Bestaande constructor (parameter-/pinvolgorde):**

```cpp
SensorADXL345(HardwareResourceTypeI2C* parent, GedeeldeBusComponent component)
```

**Configuratie voor deze hardware:**

- Gebruik bij een eigen configuratie `src/Configuratie/Sensoren/ConfigADXL345.h` (het aanwezige `src/Configuratie/Sensoren/ConfigADXL345_template.h` is een kopieersjabloon en wordt niet rechtstreeks ingelezen).
- Voor één aangesloten exemplaar: `SENSOR_ADXL345_AANTAL` op **1**, behalve wanneer de bestaande groeps-/setconfiguratie uitdrukkelijk een andere betekenis heeft.
- De bestaande configuratie bevat deze aansluitrelevante definities (gebruik de **werkelijke aangesloten** pinnen, adressen en referenties):
  - `SENSOR_ADXL345_AANTAL`: uitgecommentarieerd sjabloonvoorstel `1`; zonder actieve override geldt de fallback `0` uit `SystemConfig.h`.
  - `I2C_ADDRESS_SENSOR_ADXL345_1`: sjabloonwaarde `I2C_ADDRESS_NONE`.

**Letterlijk constructorfragment uit de bestaande voorbeeldsketch** (alle gebruikte parent-objecten moeten zelf correct aangesloten en geïnitialiseerd zijn):

```cpp
GedeeldeBusNewComponent<SensorADXL345>(&apparaat, GedeeldeBusComponent::ADXL345);
```

**Bestaande test-/voorbeeldsketches:**

- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_ADXL345/Sensor_ADXL345.ino`

**Elektrische aansluiting en randvoorwaarden volgens Analog Devices:**

- De ADXL345-chip ondersteunt zowel SPI als I²C, maar **de huidige FrameWork-SensorADXL345 gebruikt uitsluitend I²C**. Verbind dus SDA/SCL met de opgegeven I²C-parent en pas de I²C-adresselectie op de module toe; geen SPI-constructie voor deze klasse.
- Gebruik een passende laagspanningsvoeding of de regulator/levelshifter van de concrete breakout.

**Technische handleiding:** https://www.analog.com/en/products/adxl345.html

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_ADXL345/`.
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.

