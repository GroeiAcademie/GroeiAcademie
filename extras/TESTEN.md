# Testprocedure

Elke release van het GroeiAcademie FrameWork wordt vóór publicatie met de meegeleverde testscripts gevalideerd. De gedeelde Windows-`.cmd`-scripts staan onder `extras/` en maken deel uit van de library. Alleen `extras/LokalePaden.cmd` is machinespecifiek en wordt via `.gitignore` niet gepubliceerd; `extras/LokalePaden_template.cmd` wordt wel meegeleverd.

## Testscripts

```text
TestLibraryGereleased.cmd
TestLibraryGereleasedVolledigeRegresietesten.cmd
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

`TestBoardplatformsDependency.cmd` vergelijkt de vereiste boardplatforms met `arduino-cli core list`. Voor Cytron Maker UNO RP2040 en STM32F4 Nucleo-F401RE kan het script bij automatische installatie eerst de benodigde Additional Boards Manager URL toevoegen, daarna de package-index bijwerken en het platform installeren.

Deze twee dependency-scripts zijn voorbereidende controles; zij maken geen deel uit van de vier compilelogs die `TestLibraryStatusReport.cmd` samenvoegt.

### 2. Gereleasede basis: `TestLibraryGereleased.cmd`

Vanaf v1.1.2 gebruikt de gereleasete regressietest één referentieboard voor diepe dekking en minimale gerichte regressietests op de overige boards.

**Arduino UNO R4 Minima is het referentieboard voor de volledige gereleasete regressiematrix.** Op dit board:

- wordt `arduino-lint --library-manager update` uitgevoerd;
- worden de statische configuratiecontroles uitgevoerd;
- worden de bestaande niet-HX1838-voorbeelden recursief getest;
- worden Stimulusvoorbeelden met `SCREEN_OUTPUT_CONFIG` 0 tot en met 7 gecompileerd;
- wordt de volledige gereleasete Input-matrix voor `INPUT_TYPE_NONE`, DIGITAL, PCF8574, UserDefined-keypads, uitgebreide non-blocking Input en de vijf Stimulus-scenario’s onder `examples/Toepassingsgebieden/Stimulus` uitgevoerd;
- blijven de `metArgumenten`-varianten van DIGITAL en PCF8574 onderdeel van de diepe regressie.

De overige officiële boards krijgen een minimale gerichte regressietest:

- Arduino UNO R3;
- Arduino UNO R4 WiFi;
- WeMos D1 R32 (ESP32-WROOM-32U) (`esp32:esp32:d1_uno32`).

Per board worden minimaal automatische boarddetectie, CharacterScreen, PixelScreen, een Stimulus-basisbuild, een representatieve DIGITAL-build, een representatieve PCF8574-build en een representatieve Input+Stimulus-build gecompileerd. Op Arduino UNO R3 wordt aanvullend één geheugengevoelige gecombineerde Stimulus-build met `SCREEN_OUTPUT_CONFIG = 7` uitgevoerd; alleen de gekende melding `text section exceeds available space in board` wordt daar als verwachte geheugenbeperking geclassificeerd.

De acceptatieboards krijgen dezelfde minimale regressietest, met twee expliciete uitzonderingen: bij SB Components Ardi-32 (ESP32-S3-WROOM-1) is automatische boarddetectie niet van toepassing omdat `esp32:esp32:esp32s3` de generieke `ESP32S3 Dev Module` selecteert; de tests gebruiken daarom expliciet `BOARD_VERSION=BOARD_ESP32S3_ARDI32`. Paradisetronic ESP32-S3 UNO (ESP32-S3-WROOM-1) wordt met dezelfde FQBN afzonderlijk als `BOARD_ESP32S3_DEV` getest. Bij STM32F4 Nucleo-F401RE wordt de PixelScreen-build wel uitgevoerd, maar de gekende externe Adafruit ST77xx-fout `wiring_private.h: No such file or directory` wordt als verwachte dependencybeperking geclassificeerd en niet als fout van GroeiAcademie. Een andere PixelScreen-compilefout blijft wel een acceptatiefout.

De acceptatieboards zijn:

- Arduino UNO Q;
- Cytron Maker UNO RP2040;
- Paradisetronic ESP32-S3 UNO (ESP32-S3-WROOM-1);
- SB Components Ardi-32 (ESP32-S3-WROOM-1);
- STM32F4 Nucleo-F401RE.

Acceptatieresultaten worden afzonderlijk gerapporteerd en hebben geen invloed op het officiële release-PASS/FAIL.

De actuele fysieke hardwarestatus staat in `docs/HARDWARE_SUPPORT.md` en `extras/TESTRESULTATEN.md`: alle tien ondersteunde boards zijn geïmplementeerd en getest.

`examples/Systeem/Input/` blijft uitgesloten van de gewone recursieve example-loop, omdat Input bewust via de aparte gereleasete Input-matrix en de nieuwe HX1838-matrix wordt getest.

Alle compile-aanroepen gebruiken `--jobs 1`.

### 2a. Volledige gereleasete regressie: `TestLibraryGereleasedVolledigeRegresietesten.cmd`

Dit script staat bewust **naast** de vier releasecompilecycli. Het wordt niet door `TestLibraryStatusReport.cmd` verwerkt en wijzigt niets aan `extras/TESTRESULTATEN.md`. Het is het afzonderlijke vangnet om de volledige gereleasete regressiedekking opnieuw uit te voeren wanneer dat nodig is.

Bij keuze `ALLES` combineert het:

- de volledige gereleasete Screen- en Stimulusdekking van `TestLibraryGereleased.cmd` uit v1.1.0 op de vier officiële boards;
- de stabiele DIGITAL-, PCF8574-, UserDefined- en non-blocking Input-dekking van `TestLibraryNieuw.cmd` uit v1.1.0 op de officiële en acceptatieboards;
- de vijf Stimulus-scenario’s onder `examples/Toepassingsgebieden/Stimulus` als Input+Stimulus-integratietests wanneer zowel Input als Stimulus geselecteerd zijn;
- Arduino LINT en de bijbehorende statische controles.

De HX1838-tests die in v1.1.1 door `TestLibraryNieuw.cmd` worden uitgevoerd, zijn hier bewust niet dubbel opgenomen. Om de volledige v1.1.1-testdekking te draaien, gebruik je dus deze volledige regressietest samen met de normale v1.1.1-`TestLibraryNieuw.cmd`-cyclus.

Bij dubbelklikken kan gekozen worden uit:

```text
ALLES
SCREEN
INPUT
STIMULUS
SCREEN + INPUT
SCREEN + STIMULUS
INPUT + STIMULUS
```

Voor niet-interactief gebruik zijn dezelfde selecties beschikbaar als argumenten `ALLES`, `SCREEN`, `INPUT`, `STIMULUS`, `SCREEN_INPUT`, `SCREEN_STIMULUS` en `INPUT_STIMULUS`. Met alleen `--no-pause` wordt `ALLES` gebruikt.

### 3. Negatieve gereleasete configuraties: `TestLibraryGereleasedOngeldig.cmd`

Deze test compileert bewust ongeldige configuraties. Een negatieve test is alleen geslaagd wanneer:

1. de compilatie faalt; en
2. de output de specifiek verwachte `#error`-tekst bevat.

Vanaf v1.1.1 bevat dit script naast de bestaande Screen- en SystemConfig-controles ook de gereleasete ongeldige Input-configuraties voor DIGITAL en PCF8574. HX1838-gerelateerde negatieve tests blijven in `TestLibraryNieuwOngeldig.cmd`.

Een fout pad, ontbrekende dependency of andere toevallige compilerfout kan daardoor niet als vals-positieve `[OK]` doorgaan.

### 4. HX1838-validatie v1.1.1: `TestLibraryNieuw.cmd`

`TestLibraryNieuw.cmd` test in v1.1.1 **uitsluitend HX1838**. De released-route is `HX1838_BRON_CODES_DEFINE`; de EEPROM-gebaseerde HX1838-routes blijven experimenteel.

Dezelfde volledige nieuwe HX1838-matrix wordt uitgevoerd op alle vier officiële boards en alle drie acceptatieboards. Daarbij worden getest:

- `HX1838_BRON_CODES = HX1838_BRON_CODES_DEFINE`;
- standaard ontvangerpin D12;
- TinyIRReceiver (`HX1838_USE_TINYIRRECEIVER_INSTEAD_OF_IRREMOTE = 1`);
- IRremote (`HX1838_USE_TINYIRRECEIVER_INSTEAD_OF_IRREMOTE = 0`);
- de drie ingebouwde HX1838-toetsenindelingen plus `HX1838_TOETSENINDELING_REMOTE_USER_DEFINED`; deze UserDefined-route wordt in v1.1.1 uitsluitend met `HX1838_BRON_CODES_DEFINE` getest;
- de drie ingebouwde toetsenindelingen via de bestaande `metArgumenten`-route met beide ontvangstbackends;
- beide ontvangstbackends met `DEBUG`;
- `PCF8574 + HX1838` met beide ontvangstbackends;
- de vijf Stimulus-scenario’s onder `examples/Toepassingsgebieden/Stimulus` binnen de HX1838-configuraties.

De hardwarevalidatie zelf staat afzonderlijk in `extras/TESTRESULTATEN.md`: met `HX1838_BRON_CODES_DEFINE` en ontvanger op D12 werken op de geteste hardwareopstelling zowel TinyIRReceiver (`= 1`) als IRremote (`= 0`), inclusief `HX1838_TOETSENINDELING_REMOTE_USER_DEFINED` en de automatische UserDefined-kalibratie wanneer `HX1838_GENERIEK_CODES` ontbreekt.

### 5. Negatieve HX1838-configuraties: `TestLibraryNieuwOngeldig.cmd`

Dit script bevat vanaf v1.1.1 uitsluitend negatieve configuraties waarin HX1838 betrokken is, waaronder:

- niet-toegelaten combinaties met DIGITAL;
- een ongeldige `HX1838_BRON_CODES`;
- een ongeldige `HX1838_TOETSENINDELING`;
- een ongeldige waarde voor `HX1838_USE_TINYIRRECEIVER_INSTEAD_OF_IRREMOTE`;
- ongeldige nulwaarden in een vaste `HX1838_BRON_CODES_DEFINE`-mapping;
- `HX1838_TOETSENINDELING_REMOTE_USER_DEFINED` met een EEPROM-codebron, ontbrekende verplichte UserDefined-configuratie (`HX1838_GENERIEK_AANTAL_TOETSEN` of `HX1838_GENERIEK_KEY_LAYOUT`), nulcodes in een expliciet opgegeven `HX1838_GENERIEK_CODES`-mapping of een afwijkend aantal codes/layoutentries.

Ook hier is een test alleen `[OK]` wanneer de compilatie faalt én de specifiek verwachte fouttekst voorkomt.

### 6. Mapping-volledigheidscontrole: `TestLibraryMappingControle.cmd`

`TestLibraryMappingControle.cmd` behoudt voor DIGITAL, PCF8574 en HX1838 een brede tekstuele aanwezigheidstest op `mappingTestMenu[]` in `InputkanalenDIGITAL.ino`, `InputkanalenPCF8574.ino` en `InputkanalenHX1838.ino`. De referentielijsten bevatten de volledige opschriftenset van de gecontroleerde types.

Voor de drie ingebouwde HX1838-toetsenindelingen wordt aanvullend per `HX1838_TOETSENINDELING` de eigen `#if`/`#elif`-tak in `InputkanalenHX1838.ino` gecontroleerd. Binnen die tak controleert het script zowel de aanwezigheid als de verwachte volgorde van de opschriften. `HX1838_TOETSENINDELING_REMOTE_USER_DEFINED` is hiervan bewust uitgezonderd omdat de opschriften door de gebruiker worden bepaald; daarvoor wordt `InputkanalenHX1838UserDefined.ino` gecompileerd met een expliciete generieke testconfiguratie. De referentietabellen worden handmatig onderhouden en moeten bij nieuwe of gewijzigde ingebouwde keypadtypes/toetsenindelingen mee bijgewerkt worden.

### 7. Statusrapport: `TestLibraryStatusReport.cmd`

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
