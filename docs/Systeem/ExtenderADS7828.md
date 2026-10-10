# ExtenderADS7828

## Implementatie

- Library: **geen externe library in de huidige Extenderklasse**.
- Er is in de huidige Extenderklasse geen externe library-API die integraal wordt geërfd.

## FrameWork-integratie

Huidige overerving:

```cpp
struct ExtenderADS7828 : HardwareResourceTypeI2C
```

De huidige klasse voegt voornamelijk GedeeldeBus-/resource-integratie toe; er is geen aanvullende publieke chipspecifieke API geïnventariseerd die hier apart moet worden gedocumenteerd.

## Bronbestanden

```text
src/Systeem/GedeeldeBus/ExtenderADS7828.h
src/Systeem/GedeeldeBus/ExtenderADS7828.cpp
```

## Librarydocumentatie

- Deze Extender erft in de huidige implementatie geen externe library-API.
- Deze `.md` documenteert daarom de eigen FrameWork-/hardware-integratie die momenteel in de Extender aanwezig is.
