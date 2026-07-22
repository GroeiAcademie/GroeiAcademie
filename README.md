# GroeiAcademie Framework

> Waar subjectieve ervaringen en objectief meetbare vaardigheden elkaar ontmoeten.

Het GroeiAcademie Framework is een modulaire Arduino-library voor het meten, oefenen en onderzoeken van menselijke vaardigheden. De huidige versie bevat de toepassing **Stimulus** en een gedeelde **Screen**-laag.

> **Meten is weten, in plaats van denken te weten.**

## Huidige status

- versie: `0.9.79`;
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

De exacte afhankelijkheid staat in `library.properties`.

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
- `UNO_VERSION`.

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
    ├── Scenario1_EnkelTik/
    ├── Scenario2_Simultaan/
    ├── Scenario3_Ineenstortend/
    ├── Scenario4_Cocktail/
    └── Tik_Enkele_Samen_Instrotend_Coctail/
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

Er zijn in versie `0.9.78` nog geen afzonderlijke KiCad-, Fritzing-, PDF- of afbeeldingsbestanden in de repository opgenomen. De Markdown-documentatie is daarom voorlopig de gezaghebbende schema-informatie.

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
