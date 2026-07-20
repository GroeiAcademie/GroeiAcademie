# Screen

## Doel

De centrale functie `PrintToScreen()` verstuurt één volledige schermopdracht naar alle actieve uitvoerdoelen.

```cpp
void PrintToScreen(
    const String& eersteRegel,
    const String& tweedeRegel,
    unsigned long delayTime = 0,
    const String& action = "",
    const String& derdeRegel = "",
    const String& vierdeRegel = "",
    unsigned long delayTussenPaginas = 0
);
```

Aanroepen met alleen twee tekstregels blijven geldig.

## Publieke opname

```cpp
#include <Screen.h>
```

of via de volledige library:

```cpp
#include <GroeiAcademie.h>
```

## Uitvoerdoelen

De interne bitmaskerwaarden zijn:

```cpp
SCREEN_TYPE_NONE
SCREEN_TYPE_SERIAL
SCREEN_TYPE_CHARACTER
SCREEN_TYPE_PIXELS
```

`SCREEN_OUTPUT` bepaalt welke uitvoerdoelen actief zijn. Meerdere doelen kunnen gelijktijdig actief zijn.

## Characterscherm zonder callback

Zonder geregistreerde charactercallback gebruikt de library `LiquidCrystal_I2C`.

Ondersteunde configuratiewaarden:

```cpp
SCREEN_LCD1602
SCREEN_LCD1604
SCREEN_LCD2002
SCREEN_LCD2004
SCREEN_LCD4002
```

Op een scherm met vier regels worden de vier teksten op regels 0 tot en met 3 geplaatst.

Op een scherm met twee regels verschijnen regel 1 en 2 als eerste pagina. Wanneer regel 3 of 4 niet leeg is, wacht de library `delayTussenPaginas`, wist het scherm en toont regel 3 en 4 als tweede pagina.

Na de volledige tekstuitvoer wordt `delayTime` één keer uitgevoerd. Daarna wordt `action` verwerkt.

## Callbacktype

Character- en pixeluitvoer gebruiken hetzelfde callbacktype:

```cpp
typedef void (*ScreenCallback)(
    const String& eersteRegel,
    const String& tweedeRegel,
    unsigned long delayTime,
    const String& action,
    const String& derdeRegel,
    const String& vierdeRegel,
    unsigned long delayTussenPaginas
);
```

Registratie:

```cpp
RegistreerCallbackScreenTypeCharacter(MijnCharacterScreen);
RegistreerCallbackScreenTypePixel(MijnPixelScreen);
```

Met `nullptr` wordt voor characterschermen de standaardafhandeling gebruikt. Voor pixelschermen betekent `nullptr` dat geen aangepaste pixeluitvoer beschikbaar is.

## Verantwoordelijkheid van de callback

Een geregistreerde callback wordt voor haar schermtype exact één keer per `PrintToScreen()`-aanroep uitgevoerd en ontvangt de volledige opdracht.

De callback beheert zelf:

- initialisatie;
- wissen of behouden van inhoud;
- regelplaatsing;
- paginering;
- `delayTussenPaginas`;
- `delayTime`;
- `action`;
- afkapping, scrolling en cursorlogica.

`PrintToScreen()` voegt voor dat schermtype daarna geen tweede schermbewerking of tweede wachttijd toe.

## Voorbeelden

- `examples/Screen/Default_PrintToScreen/Default_PrintToScreen.ino`
- `examples/Screen/Callback_CharacterScreen/Callback_CharacterScreen.ino`

## Elektronische aansluiting van een I2C-characterscherm

Gebruik de I2C-pinnen die bij het gekozen Arduino-board horen. Sluit minimaal aan:

| LCD-module | Arduino |
|---|---|
| `VCC` | geschikte voedingsspanning volgens module |
| `GND` | `GND` |
| `SDA` | SDA-pin van het board |
| `SCL` | SCL-pin van het board |

Controleer het I2C-adres en pas `I2C_ADRES` in `src/Configuratie/SystemConfig.h` aan. Raadpleeg ook de datasheet van de gebruikte I2C-backpack, omdat voedingsspanning en pull-ups per module kunnen verschillen.

## Beperkingen

De huidige standaardimplementatie gebruikt blokkerende `delay()`-logica. Lange wachttijden onderbreken dus andere verwerking. Een callback kan een andere aanpak kiezen, maar moet dan zelf de volledige schermopdracht correct afhandelen.
