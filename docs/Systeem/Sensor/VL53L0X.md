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
- De aanwezigheid van deze rubriek is **geen bewijs van een geslaagde hardwaretest**.

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_VL53L0X/`.
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.
Dit document bevestigt geen geslaagde hardwaretest.
