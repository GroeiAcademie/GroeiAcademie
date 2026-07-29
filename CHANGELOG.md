# Changelog

Alle betekenisvolle wijzigingen aan GroeiAcademie Framework worden in dit bestand bijgehouden.

De versienummers volgen tijdens de pre-1.0-fase de versie in `library.properties`.

## 0.10.1

### Toegevoegd en gewijzigd

- compile-time PixelScreen-ondersteuning via `SCREEN_TYPE_PIXELS`;
- algemene `Adafruit_GFX* PixelScreen`-koppeling toegevoegd, zodat de concrete displaydriver in het voorbeeld of de toepassing blijft;
- ondersteunde PixelScreen-resoluties en rotatie 0 tot en met 3 toegevoegd;
- standaard PixelScreen-uitvoer voor `ScreenData::TYPE_NONE` toegevoegd;
- afzonderlijke `CharacterScreenCallback` en `PixelScreenCallback` toegevoegd;
- `ScreenData` met informatie-, interactie-, fout-, diagnose- en inhoudstypen toegevoegd;
- `PixelScreenConfigureren()`, `PixelScreenClear()`, `PixelScreenSetCursor()`, `PixelScreenPrint()` en foutafhandeling toegevoegd;
- voorbeelden toegevoegd voor standaard CharacterScreen, standaard PixelScreen, gecombineerde standaarduitvoer en PixelScreen-callback;
- `SCREEN_OUTPUT` en de waarden 0 tot en met 7 gedocumenteerd;
- debuguitvoer beperkt tot builds waarin zowel `DEBUG` als `SCREEN_TYPE_SERIAL` actief zijn;
- hardwaredocumentatie en schema uitgebreid met TFT-SPI, levelshifter en draadbruggen;
- documentatie bijgewerkt naar versie `0.10.1`;
- alle relevante Markdownbestanden afgestemd op de nieuwe en hernoemde voorbeelden;
- alle verwijzingen afgestemd op de actuele v0.10.1-tekeningset;
- `Hardware_Validatie_Shield_v0_10_1` toegevoegd voor de specifieke UNO R3/R4-shieldhardware;
- `VALIDATIE_Shield_v0.10.1.md` toegevoegd met testvolgorde en goedkeuringscriteria;
- `ScreenTypes.h` toegevoegd zodat schermtypes en schermresoluties vóór `SystemConfig.h` beschikbaar zijn;
- callbackfuncties ontvangen opnieuw altijd de volledige oorspronkelijke schermopdracht;
- `delayTussenPaginas` wordt bij gelijktijdige standaarduitvoer naar CharacterScreen en PixelScreen slechts één keer uitgevoerd;
- de Background-hulpfunctie heet `PrintToScreenBackground()`;
- hardwarevalidatie compileert optionele hardware en de bijbehorende libraries alleen wanneer de overeenkomstige validatiekeuze actief is.

### Gevalideerd

- Arduino LINT: OK
- Compilatietesten: 39/39 succesvol
- Arduino Uno R3
- Arduino Uno R4 Minima
- Arduino Uno R4 WiFi

## 0.10.0

### Toegevoegd en gewijzigd

- `ADC_BACKEND` (`ADC_BACKEND_NATIVE` / `ADC_BACKEND_ADS1115`) toegevoegd in `SystemConfig.h`, naast de bestaande `UNO_VERSION`-keuze;
- `ADC_BITS`/`DELAY_US` uitgebreid met een derde geval voor de ADS1115-backend, zodat bestaande drempel- en margeberekeningen (via de `ADC(x)`-macro) automatisch correct herschalen;
- nieuwe, backend-afhankelijke `RawAnalogRead()` in `Stimulus.cpp`, die alle drie bestaande directe `analogRead()`-aanroepen vervangt (`AnalogReadMetGekorigeerdeOffsets()`, `BepaalSensorOffsets()`, en de wegwerp-meting bij kanaalwissel in `MeetStimulus()`);
- instelbare `WACHT_LOSLATEN_DELAY_MS` in `WachtTotAlleSensorsLosgelatenVoorTest()`, om onafgebroken I2C-bevraging te vermijden wanneer `ADC_BACKEND_ADS1115` actief is;
- I2C-adres `0x48` gereserveerd voor de Stimulus-ADS1115, afgestemd op de geplande emotiemeetmodule (die `0x49`/`0x4A` zal gebruiken) om adresconflicten te vermijden bij gestapelde shields;
- optionele afhankelijkheid **Adafruit ADS1X15** gedocumenteerd in `README.md` (bewust niet in `library.properties`, zie afweging aldaar) — enkel vereist bij `ADC_BACKEND_ADS1115`;
- elektronisch schema en pinbezetting voor de ADS1115-variant van Stimulus toegevoegd onder `docs/Toepassingsgebieden/Stimulus/Hardware/`;
- hardwaredocumentatie voor H5, H6 en H7 toegevoegd;
- twee validatiescripts toegevoegd: `ADC_Validatie_Native` en `ADC_Validatie_ADS1115`;
- samengesteld Stimulusvoorbeeld hernoemd naar `Tik_Enkele_Samen_Instortend_Cocktail`.

## 0.9.79

### Toegevoegd en gewijzigd

- alle `*.md`-bestanden in lijn gebracht met de definitieve keuze van de licentie: GNU LGPL v3.0-or-later;
- `library.properties` gecorrigeerd naar `license=LGPL-3.0-or-later` (stond nog op GPL);
- `LICENSE.md` aangevuld met een paragraaf over de merknaam: "GroeiAcademie" valt als handelsnaam/merk niet onder de LGPL;
- verouderde verwijzingen naar `lib/` vervangen door de werkelijke Arduino-librarystructuur onder `src/`;
- gebroken interne links naar `STIMULUS.md` gecorrigeerd naar het werkelijke pad `Toepassingsgebieden/Stimulus/README.md`;
- huidige en geplande modules duidelijker van elkaar onderscheiden;
- documentatie voor elektronische schema's per toepassingsgebied toegevoegd;
- installatie-, configuratie- en gebruiksinformatie uitgebreid;
- status van UNO R3, UNO R4 Minima en UNO R4 WiFi verduidelijkt.

## 0.9.78

### Toegevoegd en gewijzigd

- `PrintToScreen()` uitgebreid tot vier tekstregels met `delayTussenPaginas`;
- `ScreenCallback` ontvangt de volledige schermopdracht;
- character- en pixelcallbacks worden elk maximaal één keer per `PrintToScreen()`-aanroep uitgevoerd;
- standaardondersteuning voor LCD1602, LCD1604, LCD2002, LCD2004 en LCD4002;
- voorbeelden voor standaarduitvoer en een aangepaste charactercallback;
- documentatie van de Screen-laag;
- compilatie gecontroleerd voor Arduino UNO R3, UNO R4 Minima en UNO R4 WiFi;
- Arduino Lint uitgevoerd zonder fouten of waarschuwingen.
