# ADXL345

## Implementatie

- Broncode: `src/Systeem/Sensor/ADXL345.h` en `ADXL345.cpp`.
- C++-component: `SensorADXL345`, afgeleid van `Sensor`.
- Parent: `HardwareResourceTypeI2C*` in de constructor.
- Componentcode: `ADXL345` (0xB0).
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
  - `SENSOR_ADXL345_AANTAL`: sjabloonwaarde `1`.
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

### Aanvullende Sensor × Extender-testvoorbeelden

De onderstaande `.ino`-bestanden behoren tot de huidige v2.0.0-voorbeelden. Hun aanwezigheid is gecontroleerd; compilatie en fysieke hardwarevalidatie zijn hier nog **niet** aangetoond.

- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_ADXL345_TCA9548A/Sensor_ADXL345_TCA9548A.ino`

### Functionele Sensor × Extender-voorbeelden

De onderstaande voorbeelden bevatten **meetaanroepen na de activering**. Een getoonde waarde is geen bewijs van meetnauwkeurigheid of van een geslaagde hardwaretest.

- **Fysieke test:** Draai sensor over drie assen. Voor alle assen: DATA_TYPE_DIRECT activeren.
- **Huidige reikwijdte:** de `.ino` gebruikt uitsluitend reeds bestaande FrameWork-functies; niet-beschikbare meetkanalen, kalibraties of externe functies zijn niet toegevoegd.
- **Meerkanalige meting:** voor alle kanalen `DATA_LAYER_CONFIG` op `DATA_TYPE_DIRECT` configureren. De `.ino` maakt dan de bestaande Data-component aan en leest de kanalen uit. Met de standaardwaarde `DATA_TYPE_NONE` blijft de secundaire kanaaltest onvolledig.

**Voorbeelden:**

- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_ADXL345_TCA9548A/Sensor_ADXL345_TCA9548A.ino`

**Validatie:** Arduino-compilatie, fysieke test, kalibratie en meetkwaliteitsvalidatie zijn hier nog niet aangetoond.

## Native meetvoorbeeld

`examples/Systeem/GedeeldeBus/Sensoren/Sensor_ADXL345/Sensor_ADXL345.ino` voert na de lifecycle daadwerkelijk uitlezingen uit via de bestaande Sensor-API.
Gemeten of weergegeven in dit voorbeeld: **Versnelling X (m/s2)**.
De seriële uitvoer wordt beperkt; de Sensor-uitlezing blijft in `loop()` plaatsvinden.
Dit bevestigt bij geslaagde hardwaretests de ruwe of rechtstreeks beschikbare meetwaarde, niet automatisch alle fysische kanalen, kalibratie of nauwkeurigheid.
Compilatie en fysieke meting zijn nog niet bevestigd.

## Hulppinnen en routes via Extenders

- `ReadINT1(bool& waarde)`: INT1 lezen via de tweede Extender (mixed); hoog actief. `Activeren()` zet DATA_READY op INT1.
- `ReadINT2(bool& waarde)`: INT2 lezen; hoog actief. `Activeren()` zet ACTIVITY (een verandering van meer dan 1 g op x, y of z, ac-gekoppeld) op INT2. Ac-gekoppeld: de zwaartekracht in rust telt niet als beweging. Na een gelezen gebeurtenis leest de functie INT_SOURCE (0x30) om ze vrij te geven.
- Mixed constructor: `SensorADXL345(apparaat, component, extender, np_INT1, np_INT2)`; I2C via de gewone parent (bv. TCA9548A), INT1 en INT2 op een tweede Extender (PCF8574, PCF8575, MCP23017, MCP23S17, SC16IS752 of MAX14830 GPIO). Vereist `GEDEELDE_BUS_MIXED_PINS`.
