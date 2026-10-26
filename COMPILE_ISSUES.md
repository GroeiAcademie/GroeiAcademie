# COMPILE_ISSUES.md

## Externe libraries: noodzakelijke lokale correcties

Hier staan uitsluitend concrete wijzigingen in **externe libraries** die na een herinstallatie of update opnieuw gecontroleerd moeten worden.

### Herstelscript: FIX_Libraries_COMPILE_ISSUES.ps1

Het script voert alle drie correcties hieronder uit. Het wijzigt geen bestanden van GroeiAcademie FrameWork.

**Starten:** Windows blokkeert een `.ps1` standaard. Start het daarom vanuit de map waar het script staat met:

```
powershell -ExecutionPolicy Bypass -File FIX_Libraries_COMPILE_ISSUES.ps1
ofwel
powershell.exe -NoProfile -ExecutionPolicy Bypass -File .\FIX_Libraries_COMPILE_ISSUES.ps1
```

**Map:** het script werkt uitsluitend in `%USERPROFILE%\Documents\Arduino\libraries`. Staat de Arduino-sketchbookmap elders, dan werkt het script niet en voer je de correcties met de hand uit.

**Bestanden die het script controleert en zo nodig wijzigt:**

| Bestand | Correctie |
|---|---|
| `I2C_SCANNER\I2C_SCANNER.h` | declaratie `releaseI2CBus`: `SDA`/`SCL` wordt `sdaPin`/`sclPin` |
| `I2C_SCANNER\I2C_SCANNER.cpp` | definitie `releaseI2CBus`: `SDA`/`SCL` wordt `sdaPin`/`sclPin`, ook binnen die functie |
| `Adafruit_ST7735_and_ST7789_Library\Adafruit_ST7796S.h` | software-SPI-constructor: `MOSI` wordt `mosiPin` |
| `Adafruit_ST7735_and_ST7789_Library\Adafruit_ST7796S.cpp` | software-SPI-constructor en initializer: `MOSI` wordt `mosiPin` |
| `Adafruit_ST7735_and_ST7789_Library\Adafruit_ST77xx.cpp` | STM32-regel: `&& !defined(ARDUINO_ARCH_STM32)` erbij |

**Werking:**

1. Ontbreekt een van de vijf bestanden, dan stopt het script en past het niets aan.
2. Herkent het script de inhoud van een bestand niet (geen originele en geen al gecorrigeerde versie), dan stopt het en past het niets aan.
3. Een al gecorrigeerd bestand laat het ongemoeid. Het script mag dus opnieuw uitgevoerd worden; een correctie wordt nooit dubbel toegepast.
4. Van elk bestand dat het wijzigt, maakt het eerst een back-up met de extensie `.RUN33.bak` naast het origineel. Bestaat die back-up al, dan stopt het en past het niets aan.
5. Het schrijft de bestanden byte voor byte terug; codering en regeleinden blijven gelijk.
6. Het meldt elk gecorrigeerd bestand met `GECORRIGEERD:` en sluit af met het aantal gecorrigeerde bestanden.

**Terugdraaien:** vervang een gewijzigd bestand door zijn `.RUN33.bak`-back-up.

### Arduino UNO Q, I2C_SCANNER 0.3.3 en Adafruit 1.12.0

**Gecontroleerd:** acht Tools-fouten door de `SDA`/`SCL`-parameteridentificatoren van `I2C_SCANNER::releaseI2CBus()`; zes Screen-fouten door de `MOSI`-parameteridentificator in de software-SPI-constructor van `Adafruit_ST7796S`. De versies en constructies zijn getoetst aan de officiële 0.3.3- en 1.12.0-bronnen.

**Correctie per bestaand bibliotheekbestand:** gebruik hieronder relatieve paden vanaf de Arduino-werkmap. De werkelijke locatie van de Arduino-sketchbookmap kan per computer verschillen. Wijzig geen bestanden binnen `GroeiAcademie/src`.

**Bestand:** `Arduino\libraries\I2C_SCANNER\I2C_SCANNER.h`

**Oorspronkelijke declaratie:**

```cpp
void releaseI2CBus(uint8_t SDA, uint8_t SCL);
```

**Vervang uitsluitend door:**

```cpp
void releaseI2CBus(uint8_t sdaPin, uint8_t sclPin);
```

**Bestand:** `Arduino\libraries\I2C_SCANNER\I2C_SCANNER.cpp`

**Zoek:** de bestaande definitie `I2C_SCANNER::releaseI2CBus(uint8_t SDA, uint8_t SCL)`.

**Vervang in uitsluitend deze functiedefinitie:** parameter `SDA` door `sdaPin` en parameter `SCL` door `sclPin`. Vervang ook de verwijzingen naar die parameters **binnen dezelfde functie**. Wijzig niet de boardmacro's of andere functies. De originele functienaam en de argumenttypen blijven ongewijzigd.

**Bestand:** `Arduino\libraries\Adafruit_ST7735_and_ST7789_Library\Adafruit_ST7796S.h`

**Oorspronkelijke software-SPI-constructordeclaratie:**

```cpp
Adafruit_ST7796S(int16_t CS, int16_t RS,
                int16_t MOSI, int16_t SCLK,
                int16_t RST = -1);
```

**Vervang uitsluitend de parameternaam `MOSI` door `mosiPin`:**

```cpp
Adafruit_ST7796S(int16_t CS, int16_t RS,
                int16_t mosiPin, int16_t SCLK,
                int16_t RST = -1);
```

**Bestand:** `Arduino\libraries\Adafruit_ST7735_and_ST7789_Library\Adafruit_ST7796S.cpp`

**Zoek:** de software-SPI-constructordefinitie `Adafruit_ST7796S::Adafruit_ST7796S` met de parameters `CS`, `RS`, `MOSI`, `SCLK`, `RST` en de bijhorende `Adafruit_ST77xx(...)`-initializer.

**Vervang uitsluitend:** de parameternaam `MOSI` door `mosiPin` in de constructordefinitie én de doorgegeven parameter `MOSI` door `mosiPin` in die initializer. Laat andere constructors, typen, macro's en broncode ongemoeid.

**Na correctie:** voer `extras\TestLibraryTools-v2.0.0.cmd` en `extras\TestLibraryScreen-v2.0.0.cmd` opnieuw uit, met de Arduino UNO Q als experimenteel platform, en controleer of respectievelijk de acht Tools- en zes Screen-fouten verdwenen zijn. Blijf alle bestaande tests uitvoeren; verlies geen enkele test uit de matrix.

**Ongewijzigd:** alle FrameWork-bestanden, API-functienamen, parametertypen, tests, boardmacro's en STM32-correctie. Geen `#undef`.

**Validatie:** scriptuitvoering op de Windows-machine en hertesten zijn nog niet uitgevoerd in deze omgeving. De correcte status is daarom: *correctie uitvoerbaar aangeleverd, lokale compilatie nog niet bevestigd*.

**Officiële versies:** https://github.com/RobTillaart/I2C_SCANNER/tree/0.3.3 en https://github.com/adafruit/Adafruit-ST7735-Library/tree/1.12.0.

**Officiële bron Adafruit:** [Adafruit issue #221](https://github.com/adafruit/Adafruit-ST7735-Library/issues/221), [PR #222](https://github.com/adafruit/Adafruit-ST7735-Library/pull/222).

**Officiële bron I2C_SCANNER:** voor dit probleem bestaat nog geen upstream issue.

### Adafruit ST7735 and ST7789 Library 1.12.0: STM32 Nucleo F401RE

**Foutmelding:** `fatal error: wiring_private.h: No such file or directory`.

**Bestand:** `Arduino\libraries\Adafruit_ST7735_and_ST7789_Library\Adafruit_ST77xx.cpp`

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

**Na installatie of update:** voer het herstelscript uit, of controleer met de hand eerst of de extra voorwaarde al aanwezig is. Pas ze niet dubbel toe. Voer daarna `extras\TestLibraryScreen-v2.0.0.cmd` uit en controleer dat **alle zes** Screen-compilaties op de Nucleo F401RE slagen. Een ZIP-controle is geen compilatietest.

**Wanneer verwijderen:** pas nadat de **lokaal geïnstalleerde** nieuwe Adafruit-versie de upstreamcorrectie bevat én de zes Nucleo Screen-tests zonder lokale aanpassing slagen. Een samengevoegde PR alleen volstaat niet.

**Officiële bron:** [Adafruit issue #225](https://github.com/adafruit/Adafruit-ST7735-Library/issues/225), [PR #226](https://github.com/adafruit/Adafruit-ST7735-Library/pull/226).

**Status:** aangetoond op 2026-10-25: met deze correctie slagen alle zes Screen-compilaties op de Nucleo F401RE op de Windows-compilerbox.

