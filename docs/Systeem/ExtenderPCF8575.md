# ExtenderPCF8575

## Implementatie

- Library: **Rob Tillaart — `PCF8575`**.
- De publieke API van `PCF8575` wordt rechtstreeks door `ExtenderPCF8575` geërfd.
- Voor alle beschikbare libraryfuncties, parameters, returnwaarden en library-specifiek gedrag: raadpleeg de officiële documentatie van **Rob Tillaart `PCF8575`**.
- Deze `.md` dupliceert die library-API bewust niet.

## FrameWork-integratie

Huidige overerving:

```cpp
struct ExtenderPCF8575 : HardwareResourceTypeI2C, PCF8575
```

Eigen FrameWork-/Extenderfunctionaliteit die bovenop de eventuele library-API staat:

- `Activeren()`

## Bronbestanden

```text
src/Systeem/GedeeldeBus/ExtenderPCF8575.h
src/Systeem/GedeeldeBus/ExtenderPCF8575.cpp
```

## Librarydocumentatie

- Gebruikte library: **Rob Tillaart — `PCF8575` 0.3.0**.
- Officiële documentatie / bron: https://github.com/RobTillaart/PCF8575
- De publieke API van `PCF8575` wordt rechtstreeks geërfd door `ExtenderPCF8575`.
- Raadpleeg de officiële librarydocumentatie voor **alle beschikbare functies, overloads, parameters, returnwaarden en library-specifiek gedrag**.
- Deze `.md` dupliceert de library-API bewust niet; hier documenteren we alleen de FrameWork-integratie en onze eigen aanvullingen.
