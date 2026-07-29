# GroeiAcademie Framework

> Waar subjectieve ervaringen en objectief meetbare vaardigheden elkaar ontmoeten.

Het GroeiAcademie Framework is een modulaire Arduino-library voor het meten, oefenen en onderzoeken van menselijke vaardigheden. De huidige versie bevat de toepassing **Stimulus** en een gedeelde **Screen**-laag.

> **Meten is Weten, in plaats van Denken te Weten.**

## Huidige status

- versie: `0.10.1`;
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

## Kwaliteitscontrole

Elke officiële release van deze library wordt automatisch gevalideerd.

De validatie omvat:

- Arduino LINT (Library Manager)
- compilatie van alle voorbeelden
- Arduino Uno R3
- Arduino Uno R4 Minima
- Arduino Uno R4 WiFi

Meer informatie:

- `extras/test/TESTEN.md`
- `extras/test/TESTRESULTATEN.md`

## Afhankelijkheid

De standaard CharacterScreen-implementatie gebruikt:

```text
LiquidCrystal I2C
```

De exacte, verplichte afhankelijkheid staat in `library.properties`. Wanneer `SCREEN_TYPE_PIXELS` geselecteerd wordt, zijn daarnaast `Adafruit GFX Library` en de concrete displaydriver nodig, bijvoorbeeld `Adafruit ST7735 and ST7789 Library`. Deze PixelScreen-afhankelijkheden zijn compile-time optioneel en daarom niet als harde afhankelijkheid in `library.properties` opgenomen.

### Optionele afhankelijkheid: ADS1115-backend (Stimulus)

Wanneer in `SystemConfig.h` `ADC_BACKEND` op `ADC_BACKEND_ADS1115` gezet wordt, is bijkomend de library **Adafruit ADS1X15** nodig (te installeren via Library Manager). Dit is bewust **geen** harde afhankelijkheid in `library.properties`: bij de standaardbackend (`ADC_BACKEND_NATIVE`) is deze library niet nodig, en de meeste gebruikers gebruiken de ingebouwde ADC.

## Configuratie

De belangrijkste configuratiebestanden zijn:

```text
src/Configuratie/SystemConfig.h
src/Configuratie/StimulusConfig.h
```

Controleer vóór compilatie in `SystemConfig.h` minstens:

- `DEBUG`;
- `SCREEN_OUTPUT`;
- `I2C_ADRES`;
- `ACTIEF_CHARACTER_SCREEN`;
- `ACTIEF_PIXEL_SCREEN`;
- `PIXEL_SCREEN_CS`, `PIXEL_SCREEN_DC` en `PIXEL_SCREEN_RST`;
- `PIXEL_SCREEN_ROTATION`;
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
│   ├── Callback_PixelScreen/
│   ├── Default_CharacterScreen/
│   ├── Default_PixelScreen/
│   └── Default_CharacterScreen_PixelScreen/
└── Stimulus/
    ├── ADC_Validatie_Native/
    ├── ADC_Validatie_ADS1115/
    ├── Hardware_Validatie_Shield_v0_10_1/
    ├── Scenario1_EnkelTik/
    ├── Scenario2_Simultaan/
    ├── Scenario3_Ineenstortend/
    ├── Scenario4_Cocktail/
    └── Tik_Enkele_Samen_Instortend_Cocktail/
```

`ADC_Validatie_Native`, `ADC_Validatie_ADS1115` en `Hardware_Validatie_Shield_v0_10_1` zijn bewust zelfstandig gehouden en gebruiken niet de volledige Stimulus-librarylogica — ze dienen voor hardwarevalidatie (Arduino-ADC-route versus ADS1115-route vergelijken en het volledige v0.10.1-shield controleren), niet als gewone gebruikersvoorbeelden. Zie `docs/Toepassingsgebieden/Stimulus/Hardware/Stimulus_uitbreiding_ADS1115_TFTSPI_v0.10.1.md`, hoofdstuk 18, voor het gebruik. De overige voorbeelden (`Scenario*`, `Tik_Enkele_Samen_Instortend_Cocktail`) volgen wel de normale Stimulus-library-aanpak.

## Librarystructuur

```text
GroeiAcademie/
├── src/
│   ├── GroeiAcademie.h
│   ├── Screen.h
│   ├── Stimulus.h
│   ├── Configuratie/
│   ├── Systeem/Screen/        # Screen.h, Screen.cpp en ScreenTypes.h
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

De ADS1115- en TFTSPI-hardwarelijn staat onder [docs/Toepassingsgebieden/Stimulus/Hardware/](docs/Toepassingsgebieden/Stimulus/Hardware/), met de actuele JSON-, PDF-, PNG- en SVG-schema-exporten, de centrale Markdown-documentatie [Stimulus_uitbreiding_ADS1115_TFTSPI_v0.10.1.md](docs/Toepassingsgebieden/Stimulus/Hardware/Stimulus_uitbreiding_ADS1115_TFTSPI_v0.10.1.md) en het validatieprotocol [VALIDATIE_Shield_v0.10.1.md](docs/Toepassingsgebieden/Stimulus/Hardware/VALIDATIE_Shield_v0.10.1.md).

## Documentatie

- [Project Constitution](PROJECT_CONSTITUTION.md)
- [Architectuur](docs/ARCHITECTURE.md)
- [Screen](docs/Systeem/SCREEN.md)
- [PixelScreen-foutcodes](docs/Systeem/PIXELSCREEN_FOUTCODES.md)
- [Voorstel voor leesbare PixelScreen-oriëntaties](docs/Systeem/PIXELSCREEN_ORIENTATIE_VOORSTEL.txt)
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
