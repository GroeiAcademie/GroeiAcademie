# ExtenderDS2482v800

## Implementatie

- Library: **Adafruit — `Adafruit_DS248x`**.
- De publieke API van `Adafruit_DS248x` wordt rechtstreeks door `ExtenderDS2482v800` geërfd.
- Voor alle beschikbare libraryfuncties, parameters, returnwaarden en library-specifiek gedrag: raadpleeg de officiële documentatie van **Adafruit `Adafruit_DS248x`**.
- Deze `.md` dupliceert die library-API bewust niet.

## FrameWork-integratie

Huidige overerving:

```cpp
struct ExtenderDS2482v800 : HardwareResourceTypeI2C, Adafruit_DS248x
```

Eigen FrameWork-/Extenderfunctionaliteit die bovenop de eventuele library-API staat:

- `Activeren()`

## Bronbestanden

```text
src/Systeem/GedeeldeBus/ExtenderDS2482v800.h
src/Systeem/GedeeldeBus/ExtenderDS2482v800.cpp
```

## Librarydocumentatie

- Gebruikte library: **Adafruit — `Adafruit_DS248x` 1.2.0**.
- Officiële documentatie / bron: https://github.com/adafruit/Adafruit_DS248x
- De publieke API van `Adafruit_DS248x` wordt rechtstreeks geërfd door `ExtenderDS2482v800`.
- Raadpleeg de officiële librarydocumentatie voor **alle beschikbare functies, overloads, parameters, returnwaarden en library-specifiek gedrag**.
- Deze `.md` dupliceert de library-API bewust niet; hier documenteren we alleen de FrameWork-integratie en onze eigen aanvullingen.
