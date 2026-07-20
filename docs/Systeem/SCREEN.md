# Screen

## Doel

De centrale functie `PrintToScreen()` verstuurt één volledige schermopdracht naar de actieve uitvoerdoelen.

```cpp
void PrintToScreen(const String& eersteRegel, const String& tweedeRegel, unsigned long delayTime = 0, const String& action = "", const String& derdeRegel = "", const String& vierdeRegel = "", unsigned long delayTussenPaginas = 0);
```

De bestaande aanroepen met twee regels blijven geldig. Regel 3, regel 4 en `delayTussenPaginas` zijn optioneel.

## Standaard characterscherm

Zonder geregistreerde charactercallback gebruikt de library rechtstreeks `LiquidCrystal_I2C`.

Op een scherm met vier regels worden de teksten geplaatst op regels `0`, `1`, `2` en `3`.

Op een scherm met twee regels worden regel 1 en regel 2 als eerste pagina getoond. Wanneer regel 3 of regel 4 aanwezig is, wacht de library gedurende `delayTussenPaginas`, wist het scherm en toont beide teksten als tweede pagina op regels `0` en `1`.

Na alle tekst wordt `delayTime` één keer uitgevoerd. Daarna wordt `action` toegevoegd.

## Callbackprototype

Beide registratiefuncties gebruiken hetzelfde callbacktype:

```cpp
typedef void (*ScreenCallback)(const String& eersteRegel, const String& tweedeRegel, unsigned long delayTime, const String& action, const String& derdeRegel, const String& vierdeRegel, unsigned long delayTussenPaginas);
```

Registratie:

```cpp
RegistreerCallbackScreenTypeCharacter(MijnCharacterScreen);
RegistreerCallbackScreenTypePixel(MijnPixelScreen);
```

## Verantwoordelijkheid van een callback

Een geregistreerde callback wordt exact één keer per `PrintToScreen()`-aanroep uitgevoerd. Ze ontvangt de volledige schermopdracht.

Voor haar eigen schermtype neemt de callback alle controle over. Ze is verantwoordelijk voor:

1. initialisatie van het scherm wanneer die niet elders gebeurt;
2. wissen of behouden van de bestaande scherminhoud;
3. tonen van regel 1 en regel 2;
4. tonen van regel 3 en regel 4;
5. paginering wanneer het scherm onvoldoende regels heeft;
6. uitvoeren van `delayTussenPaginas` tussen beide pagina's;
7. uitvoeren van `delayTime` nadat alle tekst werd getoond;
8. daarna tonen of verwerken van `action`;
9. cursorplaatsing, regelafbreking, afkapping en scrolling.

`PrintToScreen()` voert voor een schermtype met geregistreerde callback geen aanvullende schermlogica, wachttijd of `action` uit.

## Character- en pixelcallback

De charactercallback en pixelcallback hebben bewust hetzelfde prototype. Daardoor ontvangen beide uitvoerlagen dezelfde informatie. De implementatie bepaalt hoe die informatie op het betreffende scherm wordt weergegeven.

Een charactercallback kan bijvoorbeeld kiezen tussen twee pagina's op een LCD1602 en vier gelijktijdige regels op een LCD2004.

Een pixelcallback kan de vier tekstregels vrij positioneren, afbreken, centreren of scrollen en kan `action` als tekst, symbool of grafisch element verwerken.

## Meerdere actieve uitvoerdoelen

`SCREEN_OUTPUT` kan Serial, één characterscherm en één pixelscherm combineren. De standaarduitvoer behoudt één centrale tijdlijn. Geregistreerde callbacks worden daarna elk één keer uitgevoerd en beheren hun eigen tijdlijn. Een blokkerende callback met `delay()` wordt daarom volledig afgewerkt voordat de volgende callback start.
