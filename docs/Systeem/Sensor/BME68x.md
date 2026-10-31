# BME68x

## Implementatie

- Broncode: `src/Systeem/Sensor/BME68x.h` en `BME68x.cpp`.
- C++-component: `BME68x`, afgeleid van `Sensor`.
- Parent: `HardwareResourceTypeI2C*` in de constructor.
- Componentcode: `BME680` (0xC5), `BME688` (0xC6).
- Configuratie: `src/Configuratie/Sensoren/ConfigBME680_template.h`, `src/Configuratie/Sensoren/ConfigBME688_template.h`; de optionele persoonlijke versie heet telkens zonder `_template`.
- Data-kanalen: 4.
- Externe library: `Adafruit_BME680.h`.

## Uit de broncode

Alleen identiteit/lifecycle: nog geen volledige sensorinitialisatie of meting. parent moet de voor dit apparaat geregistreerde HardwareResourceTypeI2C-node zijn.

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

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_BME680/`, `examples/Systeem/GedeeldeBus/Sensoren/Sensor_BME688/`.
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.
Dit document bevestigt geen geslaagde hardwaretest.
