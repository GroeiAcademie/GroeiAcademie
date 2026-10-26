# COMPILE_ISSUES.md

## Externe libraries: noodzakelijke lokale correcties

Hier staan uitsluitend concrete wijzigingen in **externe libraries** die na een herinstallatie of update opnieuw gecontroleerd moeten worden.

### Adafruit ST7735 and ST7789 Library 1.12.0: STM32 Nucleo F401RE

**Foutmelding:** `fatal error: wiring_private.h: No such file or directory`.

**Bestand:** `C:\Users\alexp\Documents\Arduino\libraries\Adafruit_ST7735_and_ST7789_Library\Adafruit_ST77xx.cpp`

**Oorspronkelijke regel (zoek bovenaan in het bestand):**

```cpp
#if !defined(ARDUINO_STM32_FEATHER) && !defined(ARDUINO_UNOR4_WIFI)
```

**Vervang uitsluitend deze regel door:**

```cpp
#if !defined(ARDUINO_STM32_FEATHER) && !defined(ARDUINO_UNOR4_WIFI) && !defined(ARDUINO_ARCH_STM32)
```

**Het volledige blok ziet er dan zo uit:**

```cpp
#if !defined(ARDUINO_STM32_FEATHER) && !defined(ARDUINO_UNOR4_WIFI) && !defined(ARDUINO_ARCH_STM32)
#if !defined(ARDUINO_UNOR4_MINIMA)
#include "pins_arduino.h"
#include "wiring_private.h"
#endif
#endif
```

**Waarom:** op de STM32-core 3.0.0 ontbreekt `wiring_private.h`; door STM32 uit te sluiten wordt deze ongeschikte include daar niet uitgevoerd. Wijzig geen bestand binnen `GroeiAcademie/src` en maak geen vervangend `wiring_private.h` aan.

**Na installatie of update:** controleer eerst of de extra voorwaarde al aanwezig is. Pas ze niet dubbel toe. Voer daarna `extras\TestLibraryScreen-v2.0.0.cmd` uit en controleer dat **alle zes** Screen-compilaties op de Nucleo F401RE slagen. Een ZIP-controle is geen compilatietest.

**Wanneer verwijderen:** pas nadat de **lokaal geïnstalleerde** nieuwe Adafruit-versie de upstreamcorrectie bevat én de zes Nucleo Screen-tests zonder lokale aanpassing slagen. Een samengevoegde PR alleen volstaat niet.

**Officiële bron:** [Adafruit issue #225](https://github.com/adafruit/Adafruit-ST7735-Library/issues/225), [PR #226](https://github.com/adafruit/Adafruit-ST7735-Library/pull/226).

**Status:** instructie vastgelegd; werking op de Windows-compilerbox nog niet opnieuw aangetoond.
