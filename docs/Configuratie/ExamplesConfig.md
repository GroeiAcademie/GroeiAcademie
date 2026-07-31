# ExamplesConfig.h

Bronbestand: [`../../src/Configuratie/ExamplesConfig.h`](../../src/Configuratie/ExamplesConfig.h)

`ExamplesConfig.h` bevat de gedeelde standaardinstellingen van de Stimulus-example- en scenarioscripts. Iedere waarde staat onder `#ifndef`, zodat een afzonderlijk `.ino`-script vóór de include tijdelijk een afwijkende testwaarde kan definiëren.

```cpp
#define AANTAL_SENSOREN_ALGORITME4 3
#include <Configuratie/ExamplesConfig.h>
```

De header bevat onder meer `SERIAL_BAUDRATE`, de voorbeeldwachttijden, de leestijden, de aantallen sensoren per testalgoritme en de twee instortende scoringsvormen. Hardwaredefaults zoals `PIN_TOETS_1` tot en met `PIN_TOETS_4` blijven in `SystemConfig.h` en kunnen via `UserConfig.h` worden overschreven.
