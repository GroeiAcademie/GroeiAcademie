# ExtenderMCP23017

## Implementatie

- Library: **Rob Tillaart — `MCP23017`**.
- De publieke API van `MCP23017` wordt rechtstreeks door `ExtenderMCP23017` geërfd.
- Voor alle beschikbare libraryfuncties, parameters, returnwaarden en library-specifiek gedrag: raadpleeg de officiële documentatie van **Rob Tillaart `MCP23017`**.
- Deze `.md` dupliceert die library-API bewust niet.

## FrameWork-integratie

Huidige overerving:

```cpp
struct ExtenderMCP23017 : HardwareResourceTypeI2C, MCP23017
```

Eigen FrameWork-/Extenderfunctionaliteit die bovenop de eventuele library-API staat:

- `Activeren()`

## Bronbestanden

```text
src/Systeem/GedeeldeBus/ExtenderMCP23017.h
src/Systeem/GedeeldeBus/ExtenderMCP23017.cpp
```

## Librarydocumentatie

- Gebruikte library: **Rob Tillaart — `MCP23017` 0.9.3**.
- Officiële documentatie / bron: https://github.com/RobTillaart/MCP23017
- De publieke API van `MCP23017` wordt rechtstreeks geërfd door `ExtenderMCP23017`.
- Raadpleeg de officiële librarydocumentatie voor **alle beschikbare functies, overloads, parameters, returnwaarden en library-specifiek gedrag**.
- Deze `.md` dupliceert de library-API bewust niet; hier documenteren we alleen de FrameWork-integratie en onze eigen aanvullingen.
