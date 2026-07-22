# Changelog

Alle betekenisvolle wijzigingen aan GroeiAcademie Framework worden in dit bestand bijgehouden.

De versienummers volgen tijdens de pre-1.0-fase de versie in `library.properties`.

## [Unreleased]

### Nog open

- fysieke elektronische schemabestanden toevoegen naast de tekstuele aansluitschema's;
- automatische compilatietests via GitHub Actions;
- eerste publieke release en aanmelding bij Arduino Library Manager.

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
