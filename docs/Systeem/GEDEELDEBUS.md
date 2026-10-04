# GedeeldeBus

## Status

Deze pagina beschrijft de actuele implementatie in `src/Systeem/GedeeldeBus/GedeeldeBus.h/.cpp` en de 14 concrete Extenderbestanden van beta v2.0.0.

## Doel

GedeeldeBus verzorgt de gemeenschappelijke componentboom, lifecycle, businitialisatie en resourceconflictcontrole. Screen, Input, Sensoren en concrete Extenders blijven hun eigen hardware- en functielogica bezitten.

## Bestanden

```text
src/Systeem/GedeeldeBus/
├── GedeeldeBus.h
├── GedeeldeBus.cpp
├── ExtenderADS1115.h/.cpp
├── ExtenderADS1158.h/.cpp
├── ExtenderADS7828.h/.cpp
├── ExtenderADS7953.h/.cpp
├── ExtenderCD74HC4067.h/.cpp
├── ExtenderDS2482v800.h/.cpp
├── ExtenderMAX14830I2C.h/.cpp
├── ExtenderMAX14830SPI.h/.cpp
├── ExtenderMCP23017.h/.cpp
├── ExtenderPCF8574.h/.cpp
├── ExtenderPCF8575.h/.cpp
├── ExtenderSC16IS752I2C.h/.cpp
├── ExtenderSC16IS752SPI.h/.cpp
└── ExtenderTCA9548A.h/.cpp
```

## Boom

`Native` is de statische rootnode. `GedeeldeBusNode` bevat `parent`, `firstChild`, `nextChild` en `prevChild`; er is geen centrale node-array en geen vast maximum. Iedere node krijgt een uniek `id` via `volgendeGedeeldeBusId`.

`GedeeldeBusNode` wordt gebruikt voor root, parents en leaves. De rol wordt niet door een aparte subclass-hiërarchie afgedwongen, maar door de concrete component en de parent/childrelatie.

## Lifecycle

`GedeeldeBusNewComponent<T>(...)` voert uit:

```text
new T(...)
→ componentCreated = true
→ aanmelden()
→ controleren()
→ inpluggen()
→ activeren()
```

Bij een fout wordt `nullptr` teruggegeven en het dynamisch aangemaakte object opgeruimd. Na de cyclus worden tijdelijke conflictmeldingen weergegeven en verwijderd.

`activeren()` is de generieke lifecyclemethode. Deze zorgt dat de parent eerst actief is en roept vervolgens de concrete virtuele hook `Activeren()` aan.

`afmelden()` weigert wanneer de node nog children heeft. Dynamisch aangemaakte componenten worden verwijderd; statische componenten niet.

## Huidige betekenis per stap

- `aanmelden()`: node aan de boom koppelen en claims beschikbaar maken.
- `controleren()`: configuratie/resourceconflicten controleren.
- `inpluggen()`: basisvoorwaarden voor de component/bus uitvoeren; de concrete inhoud verschilt per component.
- `activeren()`: via `Activeren()` de component operationeel maken.

De code bevat nog component-specifieke hardwarecontroles in verschillende stappen. Bijvoorbeeld: `CharacterScreen::Activeren()` controleert het I2C-adres; `HardwareResourceTypeI2C::inpluggen()` initialiseert de bus; `PixelScreen::inpluggen()` configureert CS/DC/RST.

## Diagnose

In `GedeeldeBusNode` staat momenteel:

```cpp
#ifdef DEBUG
  virtual void Diagnose() {}
#endif
```

Dit is nog een dummy. Er zijn in de huidige code geen concrete `Diagnose()`-overrides. Daardoor heeft een releasebuild geen Diagnose-interface en voert een DEBUG-build nog geen uitgebreide elektronische componenttests uit.

## Native resources

`HardwareResourcePin` omvat `D0..D13`, `A0..A5`, `SDA`, `SCL`, `MISO`, `MOSI`, `SCK`, `SS`, `CUSTOM` en `NONE`. `NativeArduinoPinVan()` vertaalt die naar de boardmapping in `SystemConfig.h`.

ExtenderPins zijn geen `HardwareResourcePin` en worden niet via `NativeArduinoPinVan()` vertaald.

## Claims en conflicten

`BezettingPinnen` bevat gedeelde en exclusieve claims. I2C en SPI gebruiken daarnaast `HardwareResourceIdentiteit` voor de gedeelde lijnen.

De conflictvergelijking werkt alleen binnen hetzelfde resourcegebied. `firstExtenderNode()` bepaalt of twee nodes Native resources of resources van dezelfde Extender gebruiken.

I2C-adressen worden intern met bit `0x80` gemarkeerd en als exclusieve claim behandeld. Daardoor kan de generieke claimvergelijking pinnen en adressen onderscheiden.

## I2C en SPI

`HardwareResourceTypeI2C`:

- gedeelde claims: SDA, SCL;
- exclusieve claim: I2C-adres;
- `inpluggen()` initialiseert `Wire` via `InitialiserenGedeeldeBus()`;
- bevat generieke byte-/register-read/write helpers.

`HardwareResourceTypeSPI`:

- gedeelde claims: SCK, MISO, MOSI;
- exclusieve claim: CS;
- `inpluggen()` vereist CS en initialiseert `SPI`;
- bevat `Transfer()`.

## Businitialisatie

`InitialiserenGedeeldeBus()` bewaart aparte interne flags voor I2C en SPI. `ResettenGedeeldeBus()` zet beide flags terug op `false`. Op `BOARD_ESP32S3_ARDI32` en `BOARD_ESP32S3_DEV` wordt I2C expliciet gestart met de geconfigureerde SDA/SCL-pinnen; andere boards gebruiken `Wire.begin()`.

## Extenders en ExtenderPins

Alle 14 concrete Extenders staan in afzonderlijke `.h/.cpp`-bestanden. Iedere waarde in `enum class ExtenderPins` begint met `EP_`.

| Extender | actuele `ExtenderPins` uit de header |
|---|---|
| `ExtenderADS1115` | `EP_AIN0..EP_AIN3` |
| `ExtenderADS1158` | `EP_AIN0..EP_AIN15`, `EP_GPIO0..EP_GPIO7` |
| `ExtenderADS7828` | `EP_CH0..EP_CH7` |
| `ExtenderADS7953` | `EP_CH0..EP_CH15`, `EP_GPIO0..EP_GPIO3` |
| `ExtenderCD74HC4067` | `EP_Y0..EP_Y15` |
| `ExtenderDS2482v800` | `EP_IO0..EP_IO7` |
| `ExtenderMAX14830I2C` | `EP_UART0..EP_UART3`, `EP_GPIO0..EP_GPIO15` |
| `ExtenderMAX14830SPI` | `EP_UART0..EP_UART3`, `EP_GPIO0..EP_GPIO15` |
| `ExtenderMCP23017` | `EP_GPA0..EP_GPA7`, `EP_GPB0..EP_GPB7` |
| `ExtenderPCF8574` | `EP_P0..EP_P7` |
| `ExtenderPCF8575` | `EP_P00..EP_P07`, `EP_P10..EP_P17` |
| `ExtenderSC16IS752I2C` | `EP_CHANNEL_A`, `EP_CHANNEL_B`, `EP_GPIO0..EP_GPIO7` |
| `ExtenderSC16IS752SPI` | `EP_CHANNEL_A`, `EP_CHANNEL_B`, `EP_GPIO0..EP_GPIO7` |
| `ExtenderTCA9548A` | `EP_CH0..EP_CH7` |

De concrete controlpinnen van een Extender, zoals CS, IRQ, RESET, START, PWDN of de selectorlijnen van de CD74HC4067, blijven Native `HardwareResourcePin`-claims wanneer ze aan het Arduino-board gekoppeld zijn.

## ADC-parents

`ADC_NATIVE` en `ADC_ADS1115` zijn statische parents voor Sensoren. Welke van beide wordt aangemaakt is compile-time afhankelijk van `ADC_BACKEND`.

## Screen, Input en Sensor

`GedeeldeBus.h` definieert de generieke childtypes die deze subsystemen gebruiken:

- `CharacterScreen`, `PixelScreen`, `SerialOutput`;
- `InputDigital`, `InputPCF8574`, `InputHX1838`;
- `Sensor`.

De volledige Screen- en Inputlogica staat niet in GedeeldeBus maar in `src/Systeem/Screen/` en `src/Systeem/Input/`. Concrete Sensorcode staat onder `src/Systeem/Sensor/`.

## Tests die nu aanwezig zijn

Onder `examples/Systeem/GedeeldeBus/Extenders/` bestaan per Extender:

- `Extender_<naam>.ino`: basis lifecycle/resource-test;
- `Extender_<naam>_Test_ExtenderPins_Toegelaten.ino`;
- `Extender_<naam>_Test_ExtenderPins_Geweigerd.ino`.

Onder `examples/Systeem/GedeeldeBus/Sensoren/` staat `Sensor_RFP602.ino`.

Deze tests moeten niet worden geïnterpreteerd als een volledige elektronische functietest van alle chipfuncties. Zie `extras/TESTEN.md`.
