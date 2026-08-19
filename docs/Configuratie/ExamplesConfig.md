# ExamplesConfig.h

Bronbestand: [`../../src/Configuratie/ExamplesConfig.h`](../../src/Configuratie/ExamplesConfig.h)

`ExamplesConfig.h` bevat de gedeelde standaardinstellingen van de Stimulus-example- en scenarioscripts. Iedere waarde staat onder `#ifndef`, zodat een afzonderlijk `.ino`-script vóór de include tijdelijk een afwijkende testwaarde kan definiëren.

```cpp
#define AANTAL_SENSOREN_ALGORITME4 3
#include <Configuratie/ExamplesConfig.h>
```

De header bevat onder meer de voorbeeldwachttijden, de leestijden, de aantallen sensoren per testalgoritme en de twee instortende scoringsvormen. `SERIAL_BAUDRATE` behoort vanaf v1.1.0 tot de systeemconfiguratie in `SystemConfig.h`, omdat ook `Screen.cpp` deze instelling gebruikt. De DIGITAL-keypadmapping blijft in `SystemConfig.h`: nieuwe configuraties gebruiken de keypad-specifieke `KEYPAD_PIN_...`-namen; `PIN_TOETS_1` tot en met `PIN_TOETS_4` blijven als backward-compatibilitylaag via `UserConfig.h` overschrijfbaar.
