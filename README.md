# GroeiAcademie Framework

> Waar subjectieve ervaringen en objectief meetbare vaardigheden elkaar ontmoeten.

Het GroeiAcademie Framework is een modulaire Arduino-library voor het meten, oefenen en onderzoeken van menselijke vaardigheden. De huidige versie bevat de toepassing **Stimulus** en een gedeelde **Screen**-laag.

> **Meten is Weten, in plaats van Denken te Weten.**

## Huidige status

- versie: `0.10.0`;
- ontwikkelfase: pre-1.0;
- huidige implementatie: Stimulus en Screen;
- gecompileerd voor Arduino UNO R3, UNO R4 Minima en UNO R4 WiFi;
- Arduino Lint: geen fouten en geen waarschuwingen;
- licentie: GNU LGPL v3.0-or-later, zie [LICENSE](LICENSE) en [LICENSE.md](LICENSE.md).

Emotie-observatie, ademhaling, hartslag en andere toepassingsgebieden staan in de inhoudelijke roadmap, maar zijn nog niet als volwaardige modules geïmplementeerd.

## Installatie

### Via ZIP in Arduino IDE

1. Download een release-ZIP van de repository.
2. Open Arduino IDE.
3. Kies **Sketch > Include Library > Add .ZIP Library**.
4. Selecteer de ZIP.
5. Open daarna een voorbeeld via **File > Examples > GroeiAcademie**.

### Handmatig

Plaats de map `GroeiAcademie` in de Arduino-librarymap:

```text
Documents/Arduino/libraries/GroeiAcademie/
```

Herstart Arduino IDE na de installatie.

## Afhankelijkheid

De huidige Screen-implementatie gebruikt:

```text
LiquidCrystal I2C
```

De exacte, verplichte afhankelijkheid staat in `library.properties`.

### Optionele afhankelijkheid: ADS1115-backend (Stimulus)

Wanneer in `SystemConfig.h` `ADC_BACKEND` op `ADC_BACKEND_ADS1115` gezet wordt, is bijkomend de library **Adafruit ADS1X15** nodig (te installeren via Library Manager). Dit is bewust **geen** harde afhankelijkheid in `library.properties`: bij de standaardbackend (`ADC_BACKEND_NATIVE`) is deze library niet nodig, en de meeste gebruikers gebruiken de ingebouwde ADC.

## Configuratie

De belangrijkste configuratiebestanden zijn:

```text
src/Configuratie/SystemConfig.h
src/Configuratie/StimulusConfig.h
src/Configuratie/EmotieConfig.h
```

Controleer vóór compilatie in `SystemConfig.h` minstens:

- `DEBUG`;
- `I2C_ADRES`;
- `ACTIEF_CHARACTER_SCREEN`;
- `AANTAL_SENSOREN_AANWEZIG`;
- `PIN_SENSOR_1` tot en met `PIN_SENSOR_4`;
- `UNO_VERSION`;
- `ADC_BACKEND` (`ADC_BACKEND_NATIVE` of `ADC_BACKEND_ADS1115`) — zie hierboven bij Afhankelijkheid.

Op UNO R3 kan de combinatie van alle functionaliteit en uitgebreide debuguitvoer de beschikbare flash overschrijden. Schakel `DEBUG` uit voor de normale UNO R3-build wanneer nodig.

## Snel starten

Gebruik bij voorkeur de publieke hoofdheader:

```cpp
#include <GroeiAcademie.h>
```

Beschikbare voorbeelden:

```text
examples/
├── Screen/
│   ├── Callback_CharacterScreen/
│   └── Default_PrintToScreen/
└── Stimulus/
    ├── ADC_Validatie_Native/
    ├── ADC_Validatie_ADS1115/
    ├── Scenario1_EnkelTik/
    ├── Scenario2_Simultaan/
    ├── Scenario3_Ineenstortend/
    ├── Scenario4_Cocktail/
    └── Tik_Enkele_Samen_Instortend_Cocktail/
```

## Librarystructuur

```text
GroeiAcademie/
├── src/
│   ├── GroeiAcademie.h
│   ├── Screen.h
│   ├── Stimulus.h
│   ├── Configuratie/
│   ├── Systeem/Screen/
│   └── Toepassingsgebieden/Stimulus/
├── examples/
├── docs/
├── library.properties
├── keywords.txt
└── README.md
```

Zie [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md) voor verantwoordelijkheden en publieke headers.

## Elektronische schema's

De centrale schema-index staat in [docs/Toepassingsgebieden/MODULES.md](docs/Toepassingsgebieden/MODULES.md).

Voor de huidige Stimulusmodule bevat [docs/Toepassingsgebieden/Stimulus/README.md](docs/Toepassingsgebieden/Stimulus/README.md):

- het tekstuele aansluitschema;
- de pinbezetting;
- voeding en massa;
- aandachtspunten voor druksensoren;
- de relatie met `SystemConfig.h`.

De ADS1115-hardwarelijn staat onder [docs/Toepassingsgebieden/Stimulus/Hardware/](docs/Toepassingsgebieden/Stimulus/Hardware/), met schema-exporten en de Markdown-documentatie [Stimulus_uitbreiding_ADS1115_v0.10.0.md](docs/Toepassingsgebieden/Stimulus/Hardware/Stimulus_uitbreiding_ADS1115_v0.10.0.md).

## Documentatie

- [Project Constitution](PROJECT_CONSTITUTION.md)
- [Architectuur](docs/ARCHITECTURE.md)
- [Screen](docs/Systeem/SCREEN.md)
- [Toepassingsgebieden en schema-index](docs/Toepassingsgebieden/MODULES.md)
- [Stimulus](docs/Toepassingsgebieden/Stimulus/README.md)
- [Hardwareondersteuning](docs/HARDWARE_SUPPORT.md)
- [Wetenschappelijke integriteit](docs/SCIENTIFIC_INTEGRITY.md)
- [Roadmap](docs/ROADMAP.md)
- [Bijdragen](CONTRIBUTING.md)
- [Beveiligingsbeleid](SECURITY.md)

## Belangrijke grenzen

- ADC-waarden zijn zonder afzonderlijke kalibratie geen absolute krachtwaarden.
- Fysiologische signalen bewijzen niet automatisch een specifieke emotie of mentale toestand.
- Het framework is geen medisch hulpmiddel.
- Controleer altijd voeding, bedrading en componentdatasheets.

Zie [DISCLAIMER.md](DISCLAIMER.md).
