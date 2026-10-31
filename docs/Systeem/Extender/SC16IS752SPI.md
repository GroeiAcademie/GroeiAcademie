# ExtenderSC16IS752SPI

## Implementatie

- Library: **`Appnostic_SC16IS752`** (Appnostic SC16IS7XX Arduino Library), één object per UART-kanaal (`channelA`, `channelB`).
- De Extenderklasse erft de library niet, maar bevat ze als leden.

## FrameWork-integratie

Huidige overerving:

```cpp
struct ExtenderSC16IS752SPI : HardwareResourceTypeSPI
```

De huidige klasse voegt voornamelijk GedeeldeBus-/resource-integratie toe; er is geen aanvullende publieke chipspecifieke API geïnventariseerd die hier apart moet worden gedocumenteerd.

## UART-kanalen

- Twee UART-kanalen (channel 0 = A, channel 1 = B). Een Sensor op een channel haalt zijn Stream met `GetExtenderStream(channel)`.
- De Sensor zet zijn eigen baud met `StartExtenderUART(channel, baud)` in zijn `Activeren()`; de Extender roept daarvoor `setBaudrate()` van de library aan.

Niet op hardware getest: de baud per channel.

## Bronbestanden

```text
src/Systeem/Extender/SC16IS752SPI.h
src/Systeem/Extender/SC16IS752SPI.cpp
```

## Librarydocumentatie

- Deze Extender erft geen externe library-API; ze gebruikt `Appnostic_SC16IS752` als leden.
- Deze `.md` documenteert daarom de eigen FrameWork-/hardware-integratie die momenteel in de Extender aanwezig is.
