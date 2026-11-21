# SensorDigitaal

## Implementatie

- Broncode: `src/Systeem/Sensor/SensorDigitaal.h` en `SensorDigitaal.cpp`.
- C++-component: `SensorDigitaal`, afgeleid van `Sensor`.
- Parent: `GedeeldeBusNode*` in de constructor.
- Gebruikt door: `HCSR501`, `HW201`.
- Data-kanalen: 1.

## Uit de broncode

SensorDigitaal: gemeenschappelijke basis voor een digitale Sensor met één OUT-ingang, native of via een GPIO-Extender. Een afgeleide Sensor geeft alleen zijn OUT-pin door.

## Aansluiten

**Aansluitwijze in de bestaande FrameWork-code:**

- **Digitale verbinding:** module-OUT naar de gekozen digitale ingang (native Arduino of de door de constructor ondersteunde GPIO-Extender).
- **Voeding/GND:** volgens de concrete module; controleer de actieve uitgangspolariteit en spanningscompatibiliteit.

- Digitale OUT → de door de constructor gekozen native GPIO of ingang van een ondersteunde GPIO-Extender.
- Controleer of OUT actief-hoog of actief-laag is en of de signaalspanning de ingang niet overschrijdt.

**Voor de hardwaretest:**

- Gebruik de bestaande `.ino` uit **Testen** en de bestaande configuratiebestanden onder `src/Configuratie/`; een `*_template.h` wordt niet rechtstreeks ingelezen.
- **Een vaste Arduino-pintoewijzing is niet algemeen geldig:** de gekozen constructor, configuratie en werkelijke bekabeling moeten overeenkomen. Waar hierboven geen exact spanningsniveau of pinheader is bevestigd, mag dat niet uit deze documentatie worden afgeleid.


**Aanvulling aansluitpinnen en elektrische voorwaarden:**

- Dit is een generieke digitale sensorinterface, geen afzonderlijk fabrikantproduct. OUT→geconfigureerde digitale ingang; GND→gemeenschappelijke GND. VCC, uitgangspolariteit, open-drain/push-pull en pull-up hangen af van de aangesloten sensor.
- De concrete modulevariant bepaalt de **exacte voedingsgrenzen en fysieke pinvolgorde**. Neem die gegevens niet over van een andere module zonder de werkelijke print te controleren.

**Aansluit- en gebruiksgegevens uit de bestaande FrameWork-interface:**

- **Digitale bekabeling voor SensorDigitaal:** `OUT` van de module → de in de constructor toegewezen native ingang `np_OUT`; `GND` naar gemeenschappelijke massa, voeding volgens de fysieke module. De code verwacht een digitale HIGH/LOW-toestand, geen analoge ADC-waarde.
- Controleer vóór de verbinding het **OUT-logicaniveau** en de ingangstolerantie van het Arduino-board; zo nodig een passende levelshifter of pull-up gebruiken.
- De constructor krijgt een digitale `np_OUT`-pin. De klasse leest de ingang via `LeesHoofdwaarde(float&)`; de interpretatie `HIGH/LOW` hangt af van de aangesloten module.
- Gebruik een geschikte native digitale pin en controleer spanningsniveau, eventuele pull-up en uitgangspolariteit.

**Bestaande constructor (parameter-/pinvolgorde):**

```cpp
SensorDigitaal(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin np_OUT)
```



## Testen

## Hulppinnen en routes via Extenders

- `inpluggen()`: een ADC-kanaal (`StartExtenderReadAnalog(pin, 0) == 1`) wordt via de ADC gelezen; anders via `ReadExtenderPin()` op PCF8574, PCF8575, MCP23017, MCP23S17, ADS1158 (GPIO), SC16IS752 (GPIO) of MAX14830 (GPIO).
- `ReadOUTviaAnalogThreshold(float& waarde)`: één meting; millivolt = ruwe waarde x effectief bereik / ADC_MAX; hoog vanaf `analogThresholdForHighMillivolt`.
- Leden: `analogMeasurementRangeMillivolt` (0 = digitaal gelezen), `analogThresholdForHighMillivolt`.

## ADC-route

- `ReadOUTviaAnalogThreshold()` roept na een gestarte meting altijd `GetExtenderReadAnalogValue()` aan, ook na een time-out: die geeft de ADC vrij. Ready beslist alleen of de waarde geldig is .
