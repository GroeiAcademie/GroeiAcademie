# ExtenderCD74HC4067

## Implementatie

- Library: **eigen FrameWork-code**.
- Er is in de huidige Extenderklasse geen externe library-API die integraal wordt geërfd.

## FrameWork-integratie

Huidige overerving:

```cpp
struct ExtenderCD74HC4067 : GedeeldeBusNode
```

Eigen FrameWork-/Extenderfunctionaliteit die bovenop de eventuele library-API staat:

- `inpluggen()`
- `selectChannel()`
- `enable()`
- `disable()`

## Bronbestanden

```text
src/Systeem/GedeeldeBus/ExtenderCD74HC4067.h
src/Systeem/GedeeldeBus/ExtenderCD74HC4067.cpp
```

## Librarydocumentatie

- Deze Extender erft in de huidige implementatie geen externe library-API.
- Deze `.md` documenteert daarom de eigen FrameWork-/hardware-integratie die momenteel in de Extender aanwezig is.
