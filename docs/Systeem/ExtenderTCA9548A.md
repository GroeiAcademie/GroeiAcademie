# ExtenderTCA9548A

## Implementatie

- Library: **Rob Tillaart — `TCA9548`**.
- De publieke API van `TCA9548` wordt rechtstreeks door `ExtenderTCA9548A` geërfd.
- Voor alle beschikbare libraryfuncties, parameters, returnwaarden en library-specifiek gedrag: raadpleeg de officiële documentatie van **Rob Tillaart `TCA9548`**.
- Deze `.md` dupliceert die library-API bewust niet.

## FrameWork-integratie

Huidige overerving:

```cpp
struct ExtenderTCA9548A : HardwareResourceTypeI2C, TCA9548
```

Eigen FrameWork-/Extenderfunctionaliteit die bovenop de eventuele library-API staat:

- `Activeren()`

## Bronbestanden

```text
src/Systeem/GedeeldeBus/ExtenderTCA9548A.h
src/Systeem/GedeeldeBus/ExtenderTCA9548A.cpp
```

## Librarydocumentatie

- Gebruikte library: **Rob Tillaart — `TCA9548` 0.3.2**.
- Officiële documentatie / bron: https://github.com/RobTillaart/TCA9548
- De publieke API van `TCA9548` wordt rechtstreeks geërfd door `ExtenderTCA9548A`.
- Raadpleeg de officiële librarydocumentatie voor **alle beschikbare functies, overloads, parameters, returnwaarden en library-specifiek gedrag**.
- Deze `.md` dupliceert de library-API bewust niet; hier documenteren we alleen de FrameWork-integratie en onze eigen aanvullingen.
