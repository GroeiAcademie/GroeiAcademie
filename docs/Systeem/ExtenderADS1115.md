# ExtenderADS1115

## Library

`ExtenderADS1115` gebruikt uitsluitend **ADS1X15 van Rob Tillaart** en erft publiek van `ADS1115`. Er is geen librarykeuze meer.

De publieke functies van `ADS1115` worden rechtstreeks geërfd en worden hier niet opnieuw gewrapt. Voor de volledige library-API geldt de documentatie van ADS1X15 van Rob Tillaart.

## Eigen FrameWork-functionaliteit

- GedeeldeBus-resourcebeheer via `HardwareResourceTypeI2C`.
- `ExtenderPins` voor `EP_AIN0` t.e.m. `EP_AIN3`.
- registratie van `np_ALERT_RDY`.
- `Activeren()` initialiseert eerst de gedeelde I2C-bus en voert daarna `ADS1115::begin()` en `ADS1115::isConnected()` uit.

## Gebruik

Functies zoals `setGain()`, `readADC()`, `requestADC()`, `isReady()`, `getValue()`, `toVoltage()` en de overige publieke ADS1X15-API worden rechtstreeks van Rob Tillaart gebruikt.

## Afhankelijkheid

Arduino-library: `ADS1X15`.

## Librarydocumentatie

- Gebruikte library: **Rob Tillaart — `ADS1X15` 0.6.2**.
- Officiële documentatie / bron: https://github.com/RobTillaart/ADS1X15
- De publieke API van `ADS1115` wordt rechtstreeks geërfd door `ExtenderADS1115`.
- Raadpleeg de officiële Rob Tillaart-librarydocumentatie voor **alle beschikbare functies, overloads, parameters, returnwaarden en library-specifiek gedrag**.
- Deze `.md` dupliceert de library-API bewust niet; hier documenteren we alleen de FrameWork-integratie en onze eigen aanvullingen.
