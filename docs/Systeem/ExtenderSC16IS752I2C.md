# ExtenderSC16IS752I2C

## Implementatie

- Library: **geen externe library in de huidige Extenderklasse**.
- Er is in de huidige Extenderklasse geen externe library-API die integraal wordt geërfd.

## FrameWork-integratie

Huidige overerving:

```cpp
struct ExtenderSC16IS752I2C : HardwareResourceTypeI2C
```

De huidige klasse voegt voornamelijk GedeeldeBus-/resource-integratie toe; er is geen aanvullende publieke chipspecifieke API geïnventariseerd die hier apart moet worden gedocumenteerd.

## Bronbestanden

```text
src/Systeem/GedeeldeBus/ExtenderSC16IS752I2C.h
src/Systeem/GedeeldeBus/ExtenderSC16IS752I2C.cpp
```

## Librarydocumentatie

- Deze Extender erft in de huidige implementatie geen externe library-API.
- Deze `.md` documenteert daarom de eigen FrameWork-/hardware-integratie die momenteel in de Extender aanwezig is.
