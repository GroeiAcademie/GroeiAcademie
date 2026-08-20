# Testprocedure

Elke release van het GroeiAcademie FrameWork wordt vóór publicatie met de meegeleverde testscripts gevalideerd. De gedeelde Windows-`.cmd`-scripts staan onder `extras/` en maken deel uit van de library. Alleen `extras/LokalePaden.cmd` is machinespecifiek en wordt via `.gitignore` niet gepubliceerd; `extras/LokalePaden_template.cmd` wordt wel meegeleverd.

## Testscripts

```text
TestLibraryGereleased.cmd
TestLibraryGereleasedOngeldig.cmd
TestLibraryNieuw.cmd
TestLibraryNieuwOngeldig.cmd
TestLibraryDependency.cmd
TestBoardplatformsDependency.cmd
TestLibraryMappingControle.cmd
TestLibraryStatusReport.cmd
LokalePaden_template.cmd
```

`TestLibraryAllesEnMaakStatusReport.cmd` bestaat niet meer. De vier compilecycli worden afzonderlijk uitgevoerd. Daarna voegt `TestLibraryStatusReport.cmd` hun vier logbestanden samen tot `TestLibraryStatusReport.txt` en voert het automatisch `TestLibraryMappingControle.cmd --no-pause` uit. De mappingcontrole is een verplichte releasevoorwaarde; een fout in die controle verhindert `RELEASEVALIDATIE GESLAAGD`.

## Padkeuze van de Windows-tests

De scripts zoeken `arduino-cli` eerst via Windows `PATH`. Wanneer dat niet lukt, wordt `extras/LokalePaden.cmd` geladen. Maak dat lokale bestand indien nodig door `extras/LokalePaden_template.cmd` naar `extras/LokalePaden.cmd` te kopiëren en daar de paden van de eigen machine in te vullen. Het lokale bestand staat in `.gitignore` en wordt niet gepubliceerd.

`TestLibraryGereleased.cmd` zoekt `arduino-lint` eveneens via `PATH`. Wanneer `arduino-lint` daar niet gevonden wordt en `LINT_PATH` niet via `LokalePaden.cmd` is ingesteld, probeert het script `arduino-lint` in dezelfde map als `arduino-cli`.

## Schone testconfiguratie

Alle echte `arduino-cli compile`-aanroepen in de vier compilecycli definiëren `GROEIACADEMIE_IGNORE_USER_CONFIG`. Een eventueel lokaal `src/Configuratie/UserConfig.h` wordt tijdens die compiletests dus bewust genegeerd. De testconfiguratie bestaat uit de expliciete compilerdefinities van het betrokken testgeval plus de fallbacks van `SystemConfig.h`.

Een `#define` in een `.ino` geldt alleen binnen die sketch-translation-unit en configureert geen apart gecompileerd librarybestand zoals `Input.cpp`, `Screen.cpp` of `Stimulus.cpp`. UserDefined-keypadtests geven de benodigde `KEYPAD_GENERIEK_...`-waarden daarom expliciet als compilerdefinities mee; in normaal gebruik horen die instellingen in `UserConfig.h`.

## Teststappen

### 1. Dependencies en boardplatforms

`TestLibraryDependency.cmd` vergelijkt de vereiste libraries met de geïnstalleerde libraries en kan ontbrekende dependencies desgewenst via `arduino-cli lib install` installeren. De lijst wordt handmatig uit `library.properties` onderhouden.

`TestBoardplatformsDependency.cmd` vergelijkt de vereiste boardplatforms met `arduino-cli core list`. Voor Cytron Maker Uno RP2040 en STMicroelectronics Nucleo-F401RE kan het script bij automatische installatie eerst de benodigde Additional Boards Manager URL toevoegen, daarna de package-index bijwerken en het platform installeren.

Deze twee dependency-scripts zijn voorbereidende controles; zij maken geen deel uit van de vier compilelogs die `TestLibraryStatusReport.cmd` samenvoegt.

### 2. Gereleasede basis — `TestLibraryGereleased.cmd`

Dit script:

- voert `arduino-lint --library-manager update` uit;
- voert statische configuratiecontroles uit;
- test de officiële boards Arduino UNO R3, Arduino UNO R4 Minima, Arduino UNO R4 WiFi en WEMOS D1 R32 (`esp32:esp32:d1_uno32`);
- compileert voor elk officieel board eerst `InputkanalenDIGITAL` zonder geforceerde `-DBOARD_VERSION`, zodat de automatische boarddetectie in `SystemConfig.h` werkelijk wordt getest;
- compileert daarna de bestaande voorbeelden recursief, met `examples/Systeem/Input/` uitgesloten omdat die configuratiematrix afzonderlijk door `TestLibraryNieuw.cmd` wordt behandeld;
- compileert Stimulusvoorbeelden met `SCREEN_OUTPUT_CONFIG` 0 tot en met 7;
- gebruikt `--jobs 1` om iedere compile sequentieel uit te voeren.

De bekende UNO R3-geheugenbeperking van `Tik_Enkele_Samen_Instortend_Cocktail` wordt alleen als verwachte geheugenbeperking geclassificeerd wanneer de compiler letterlijk `text section exceeds available space in board` meldt voor de betrokken PixelScreen-combinaties. Iedere andere fout blijft een testfout.

### 3. Negatieve gereleasede configuraties — `TestLibraryGereleasedOngeldig.cmd`

Deze test compileert bewust ongeldige configuraties. Een negatieve test is alleen geslaagd wanneer:

1. de compilatie faalt; en
2. de output de specifiek verwachte `#error`-tekst bevat.

Een fout pad, ontbrekende dependency of andere toevallige compilerfout kan daardoor niet als vals-positieve `[OK]` doorgaan.

### 4. Input-validatie v1.1.0 — `TestLibraryNieuw.cmd`

De officiële releaseboards zijn:

- Arduino UNO R3;
- Arduino UNO R4 Minima;
- Arduino UNO R4 WiFi;
- WEMOS D1 R32 / hetzelfde `esp32:esp32:d1_uno32`-profiel.

Daarnaast worden als acceptatieboards gecompileerd:

- Cytron Maker Uno RP2040;
- STMicroelectronics Nucleo-F401RE;
- SB Components Ardi32.

Acceptatieboards worden afzonderlijk gerapporteerd en hebben geen invloed op het officiële release-PASS/FAIL.

`TestLibraryNieuw.cmd` compileert de geldige Input-configuratiematrix voor DIGITAL, PCF8574, HX1838, PCF8574+HX1838, de twee UserDefined-keypads en de uitgebreide non-blocking Input-configuratie. Voor de UserDefined-tests worden de benodigde `KEYPAD_GENERIEK_...`-definities expliciet op compiler-/buildniveau meegegeven.

De twee voorbeelden heten:

```text
examples/Systeem/Input/InputkanalenPCF8574UserDefinedDirect/InputkanalenPCF8574UserDefinedDirect.ino
examples/Systeem/Input/InputkanalenPCF8574UserDefinedMatrix/InputkanalenPCF8574UserDefinedMatrix.ino
```

De compiletest met `INPUT_KANAAL_OPVRAGEN_HUIDIGE_TOETSAANSLAG_UITGEBREID` geeft compiledekking aan dit codepad. Gedrag zoals debounce, loslaten, lang indrukken en timeout blijft daarnaast runtime-/hardwaregedrag.

De vijf `Input_Test_...`-Stimulus-testvoorbeelden worden eveneens gecompileerd. De oorspronkelijke Stimulusvoorbeelden blijven daarbij afzonderlijke voorbeelden.

### 5. Negatieve Input-configuraties — `TestLibraryNieuwOngeldig.cmd`

Ook hier geldt: een bewust ongeldige configuratie is alleen `[OK]` wanneer de compilatie faalt én de specifiek verwachte fouttekst voorkomt. Hierdoor wordt de compile-time validatie zelf getest in plaats van alleen het bestaan van eender welke compilerfout.

### 6. Mapping-volledigheidscontrole — `TestLibraryMappingControle.cmd`

`TestLibraryMappingControle.cmd` behoudt voor DIGITAL, PCF8574 en HX1838 een brede tekstuele aanwezigheidstest op `mappingTestMenu[]` in `InputkanalenDIGITAL.ino`, `InputkanalenPCF8574.ino` en `InputkanalenHX1838.ino`. De referentielijsten bevatten de volledige opschriftenset van de gecontroleerde types.

Voor HX1838 wordt aanvullend per `HX1838_TOETSENINDELING` de eigen `#if`/`#elif`-tak gecontroleerd. Binnen die tak controleert het script zowel de aanwezigheid als de verwachte volgorde van de opschriften. De referentietabellen worden handmatig onderhouden en moeten bij nieuwe of gewijzigde keypadtypes/toetsenindelingen mee bijgewerkt worden.

### 7. Statusrapport — `TestLibraryStatusReport.cmd`

De vier compilecycli schrijven elk hun eigen logbestand:

```text
TestLibraryGereleased.txt
TestLibraryGereleasedOngeldig.txt
TestLibraryNieuw.txt
TestLibraryNieuwOngeldig.txt
```

Nadat deze vier scripts afzonderlijk zijn uitgevoerd, voegt `TestLibraryStatusReport.cmd` ze samen tot `TestLibraryStatusReport.txt`, beoordeelt de releasegerichte samenvattingen en voert het de verplichte `TestLibraryMappingControle.cmd` automatisch uit. Alleen wanneer de vier compilecycli, de releasegerichte controles, de geheugengegevens én de mappingcontrole voldoen, kan het statusrapport `RELEASEVALIDATIE GESLAAGD` melden.

## Definitieve releaseresultaten

De definitieve resultaten van een officiële release worden pas na de volledige testcyclus vastgelegd in:

```text
extras/TESTRESULTATEN.md
```
