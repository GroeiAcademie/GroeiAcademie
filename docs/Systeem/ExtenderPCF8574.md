# ExtenderPCF8574

## Library

`ExtenderPCF8574` gebruikt uitsluitend **PCF8574 van Rob Tillaart** en erft publiek van `PCF8574`. Er is geen keuze meer tussen een eigen driver en Rob Tillaart.

De publieke functies van `PCF8574`, waaronder `read()`, `write()`, `read8()`, `write8()`, `lastError()` en `isConnected()`, worden rechtstreeks geërfd en worden niet opnieuw gewrapt.

## Eigen FrameWork-functionaliteit

- GedeeldeBus-resourcebeheer via `HardwareResourceTypeI2C`.
- `ExtenderPins` voor `EP_P0` t.e.m. `EP_P7`.
- registratie van `np_INT`.
- `Activeren()` initialiseert eerst de gedeelde I2C-bus en voert daarna `PCF8574::begin()` en `PCF8574::isConnected()` uit.
- `pinMode(ExtenderPins pin, uint8_t modus)` blijft als FrameWork-extra bestaan omdat bestaande FrameWork-code deze semantiek gebruikt. `INPUT` schrijft HIGH; andere modi schrijven LOW, overeenkomstig het bestaande gedrag.

## Gebruik

Alle overige I/O gebruikt rechtstreeks de geërfde Rob Tillaart-API. Er bestaan geen eigen `read8()`, `write8()`, `digitalRead()`, `digitalWrite()` of `digitalReadAll()` wrappers meer.

## Afhankelijkheid

Arduino-library: `PCF8574` (minimaal 0.4.0 volgens de huidige projectdependency).

## Librarydocumentatie

- Gebruikte library: **Rob Tillaart — `PCF8574` 0.4.5**.
- Officiële documentatie / bron: https://github.com/RobTillaart/PCF8574
- De publieke API van `PCF8574` wordt rechtstreeks geërfd door `ExtenderPCF8574`.
- Raadpleeg de officiële Rob Tillaart-librarydocumentatie voor **alle beschikbare functies, overloads, parameters, returnwaarden en library-specifiek gedrag**.
- Deze `.md` dupliceert de library-API bewust niet; hier documenteren we alleen de FrameWork-integratie en onze eigen aanvullingen.
- `pinMode(ExtenderPins, uint8_t)` blijft een eigen FrameWork-extra.
