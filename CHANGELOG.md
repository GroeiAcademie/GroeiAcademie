# Changelog

Alle betekenisvolle wijzigingen aan GroeiAcademie Framework worden in dit bestand bijgehouden.

De versienummers volgen tijdens de pre-1.0-fase de versie in `library.properties`.

## Unreleased

### Added
- Voorbereiding van ADS1115 als alternatieve ADC-backend voor Stimulus.
- Hardwaredocumentatie voor H5, H6 en H7.
- Validatiesketch voor vergelijking tussen Arduino-ADC en ADS1115.

### Status
Deze ADS1115-lijn is aanwezig als werkversie richting v0.10.00 en is nog niet als definitief ondersteunde hardware gevalideerd.

## 0.10.0

### Toegevoegd en gewijzigd

- `ADC_BACKEND` (`ADC_BACKEND_NATIVE` / `ADC_BACKEND_ADS1115`) toegevoegd in `SystemConfig.h`, naast de bestaande `UNO_VERSION`-keuze;
- `ADC_BITS`/`DELAY_US` uitgebreid met een derde geval voor de ADS1115-backend, zodat bestaande drempel- en margeberekeningen (via de `ADC(x)`-macro) automatisch correct herschalen;
- nieuwe, backend-afhankelijke `RawAnalogRead()` in `Stimulus.cpp`, die alle drie bestaande directe `analogRead()`-aanroepen vervangt (`AnalogReadMetGekorigeerdeOffsets()`, `BepaalSensorOffsets()`, en de wegwerp-meting bij kanaalwissel in `MeetStimulus()`);
- instelbare `WACHT_LOSLATEN_DELAY_MS` in `WachtTotAlleSensorsLosgelatenVoorTest()`, om onafgebroken I2C-bevraging te vermijden wanneer `ADC_BACKEND_ADS1115` actief is;
- I2C-adres `0x48` gereserveerd voor de Stimulus-ADS1115, afgestemd op de geplande emotiemeetmodule (die `0x49`/`0x4A` zal gebruiken) om adresconflicten te vermijden bij gestapelde shields;
- optionele afhankelijkheid **Adafruit ADS1X15** gedocumenteerd in `README.md` (bewust niet in `library.properties`, zie afweging aldaar) — enkel vereist bij `ADC_BACKEND_ADS1115`;
- elektronisch schema en pinbezetting voor de ADS1115-variant van Stimulus (schema 1/2/3) toegevoegd als apart document, ter aanvulling op het bestaande tekstuele aansluitschema.

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
