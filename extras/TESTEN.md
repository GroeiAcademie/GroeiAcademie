# Testprocedure

Elke release van het GroeiAcademie FrameWork wordt vóór publicatie met de meegeleverde testscripts gevalideerd. De gedeelde Windows-`.cmd`-scripts staan onder `extras/` en maken deel uit van de library. Alleen `extras/LokalePaden.cmd` is machinespecifiek en wordt via `.gitignore` niet gepubliceerd; `extras/LokalePaden_template.cmd` wordt wel meegeleverd.

## Testscripts

```text
TestLibraryGereleased.cmd
TestLibraryGereleasedOngeldig.cmd
TestLibraryNieuw.cmd
TestLibraryNieuwOngeldig.cmd
TestLibraryGedeeldeBus.cmd
TestLibraryGedeeldeBusOngeldig.cmd
TestLibraryDependency.cmd
TestBoardplatformsDependency.cmd
TestLibraryMappingControle.cmd
TestLibraryGenereerTESTRESULTATEN.cmd
LokalePaden_template.cmd
```

De vier releasecompilecycli worden afzonderlijk uitgevoerd: `TestLibraryGereleased.cmd`, `TestLibraryGereleasedOngeldig.cmd`, `TestLibraryNieuw.cmd` en `TestLibraryNieuwOngeldig.cmd`. Daarna voegt `TestLibraryGenereerTESTRESULTATEN.cmd` hun vier logbestanden samen tot `TestLibraryStatusReport.txt` en voert het automatisch `TestLibraryMappingControle.cmd --no-pause` uit. De mappingcontrole is een verplichte releasevoorwaarde; een fout in die controle verhindert `RELEASEVALIDATIE GESLAAGD`.

`TestLibraryGedeeldeBus.cmd` en `TestLibraryGedeeldeBusOngeldig.cmd` maken geen deel uit van deze vier releasecompilelogs.

## Padkeuze van de Windows-tests

De scripts zoeken `arduino-cli` eerst via Windows `PATH`. Wanneer dat niet lukt, wordt `extras/LokalePaden.cmd` geladen. Maak dat lokale bestand indien nodig door `extras/LokalePaden_template.cmd` naar `extras/LokalePaden.cmd` te kopiëren en daar de paden van de eigen machine in te vullen. Het lokale bestand staat in `.gitignore` en wordt niet gepubliceerd.

`TestLibraryGereleased.cmd` zoekt `arduino-lint` eveneens via `PATH`. Wanneer `arduino-lint` daar niet gevonden wordt en `LINT_PATH` niet via `LokalePaden.cmd` is ingesteld, probeert het script `arduino-lint` in dezelfde map als `arduino-cli`.

## Schone testconfiguratie

Alle echte `arduino-cli compile`-aanroepen in de vier compilecycli definiëren `GROEIACADEMIE_IGNORE_USER_CONFIG`. Een eventueel lokaal `src/Configuratie/UserConfig.h` wordt tijdens die compiletests dus bewust genegeerd. De testconfiguratie bestaat uit de expliciete compilerdefinities van het betrokken testgeval plus de fallbacks van `SystemConfig.h`.

Een `#define` in een `.ino` geldt alleen binnen die sketch-translation-unit en configureert geen apart gecompileerd librarybestand zoals `Input.cpp`, `Screen.cpp` of `Stimulus.cpp`. UserDefined-keypadtests geven de benodigde `KEYPAD_GENERIEK_...`-waarden daarom expliciet als compilerdefinities mee; in normaal gebruik horen die instellingen in `UserConfig.h`.

### Gemeenschappelijke testgroepen

`TestLibraryCommon.cmd` gebruikt drie FQBN-groepen:

- `BASIC_FQBN`: Arduino UNO R4 Minima en Arduino UNO R4 WiFi;
- `EXTENDED_FQBN`: Arduino UNO Q, ESP32 D1 UNO32, ESP32-S3 (`BOARD_ESP32S3_ARDI32` en `BOARD_ESP32S3_DEV`), Cytron Maker UNO RP2040 en STM32F4 Nucleo-F401RE;
- `EXPERIMENTAL_FQBN`: Arduino UNO R3.

Per testblok bepaalt `WELKE_FQBN_TESTEN` of de test op `BIJ_BASIC_FQBN`, `BIJ_EXTENDED_FQBN`, `BIJ_EXPERIMENTAL_FQBN` of `BIJ_ALLE_FQBN` wordt uitgevoerd. In het eindresultaat bepalen BASIC + EXTENDED de release-eindstatus. EXPERIMENTAL wordt afzonderlijk gerapporteerd en heeft geen release-impact.

Optioneel kan een testblok daarnaast één of beide van deze lijsten bevatten:

```bat
set WELKE_FQBN_INCOMPATIBLE="fqbn1,fqbn2"
set WELKE_FQBN_SKIPPEN="fqbn1,fqbn2"
```

De regels worden alleen opgenomen wanneer er daadwerkelijk FQBN's voor die test moeten worden overgeslagen. Zowel `WELKE_FQBN_INCOMPATIBLE` als `WELKE_FQBN_SKIPPEN` zorgt ervoor dat de betrokken test/FQBN-combinatie niet wordt uitgevoerd en niet in de testtotalen of het testrapport terechtkomt. `INCOMPATIBLE` wordt gebruikt wanneer de combinatie technisch niet wordt ondersteund; `SKIPPEN` wanneer een combinatie om een andere gedocumenteerde reden bewust niet wordt getest.

### Controlebestand `.ino`

Voor elk gewoon testscript wordt naast het `.cmd`- en `.txt`-bestand een controlebestand met dezelfde basisnaam gebruikt, bijvoorbeeld `TestLibraryNieuw.ino`.

- bestaat het controlebestand nog niet, dan maakt `TestLibraryCommon.cmd` het aan en schrijft het ontbrekende `TEST_INO`-vermeldingen erin;
- bij `CONTROLE_MODUS=INCLUDE` krijgen gegenereerde blokken `TEST_TYPE="NIEUW"` en `WELKE_FQBN_TESTEN="BIJ_ALLE_FQBN"`;
- bij `CONTROLE_MODUS=EXCLUDE` krijgen gegenereerde blokken `TEST_TYPE="REGRESSIE"`;
- bestaat het controlebestand al en is het groter dan 0 byte, dan wordt de inhoud getoond en worden geen tests gestart;
- bestaat het controlebestand al en is het 0 byte, dan wordt niets opnieuw gegenereerd en gaat de testcyclus verder.

Hierdoor kan de gegenereerde inhoud gecontroleerd en desgewenst overgenomen worden in het betrokken `.cmd`-script. Zodra het controlebestand daarna bewust leeg is, wordt geen nieuwe generatie gestart.

## Teststappen

### 1. Dependencies en boardplatforms

`TestLibraryDependency.cmd` vergelijkt de vereiste libraries met de geïnstalleerde libraries en kan ontbrekende dependencies desgewenst via `arduino-cli lib install` installeren. De lijst wordt handmatig uit `library.properties` onderhouden.

`TestBoardplatformsDependency.cmd` vergelijkt de vereiste boardplatforms met `arduino-cli core list`. Voor Cytron Maker UNO RP2040 en STM32F4 Nucleo-F401RE kan het script bij automatische installatie eerst de benodigde Additional Boards Manager URL toevoegen, daarna de package-index bijwerken en het platform installeren.

Deze twee dependency-scripts zijn voorbereidende controles; zij maken geen deel uit van de vier compilelogs die `TestLibraryGenereerTESTRESULTATEN.cmd` samenvoegt.

### 2. Gereleasede tests: `TestLibraryGereleased.cmd`

`TestLibraryGereleased.cmd` gebruikt `CONTROLE_MODUS=EXCLUDE`. De gewone directorycontrole omvat `examples/`, met `examples\Systeem\Input\` en `examples\Systeem\GedeeldeBus\` uitgesloten.

De testblokken gebruiken drie statussen:

- `TEST_TYPE="GERELEASED"`: wordt bij een gewone uitvoering uitgevoerd;
- `TEST_TYPE="REGRESSIE"`: wordt alleen aanvullend uitgevoerd wanneer het script met `REGRESSIETEST=TRUE` wordt gestart;
- `TEST_TYPE="NIEUW"`: is in een EXCLUDE-script niet toegestaan als uitvoerbare released test. Wanneer zo'n blok wordt aangetroffen, stopt de testlijstcontrole voordat de tests starten.

De huidige regressiepaden worden ingesteld met:

```bat
set REGRESSIE_PADEN="examples\"
```

Een gewone uitvoering is dus:

```text
TestLibraryGereleased.cmd
```

De aanvullende regressietests worden uitgevoerd met:

```text
TestLibraryGereleased.cmd REGRESSIETEST=TRUE
```

Alle compile-aanroepen lopen via `TestLibraryCommon.cmd` en gebruiken `--jobs 1`. De eindtabel rapporteert afzonderlijk `BASIC_FQBN`, `EXTENDED_FQBN` en `EXPERIMENTAL_FQBN`; de regel `Eindstatus BASIC + EXTENDED` is de releasegerichte eindstatus van deze testcyclus.

### 3. Negatieve gereleasete configuraties: `TestLibraryGereleasedOngeldig.cmd`

Deze test compileert bewust ongeldige configuraties. Een negatieve test is alleen geslaagd wanneer:

1. de compilatie faalt; en
2. de output de specifiek verwachte `#error`-tekst bevat.

`TestLibraryGereleasedOngeldig.cmd` volgt dezelfde `EXCLUDE`-scope als `TestLibraryGereleased.cmd`: `examples\Systeem\Input\` en `examples\Systeem\GedeeldeBus\` vallen buiten deze negatieve released tests. De bestaande Screen- en SystemConfig-controles blijven hier staan.

Een fout pad, ontbrekende dependency of andere toevallige compilerfout kan daardoor niet als vals-positieve `[OK]` doorgaan.

### 4. Nieuwe tests: `TestLibraryNieuw.cmd`

`TestLibraryNieuw.cmd` gebruikt `CONTROLE_MODUS=INCLUDE` met:

```text
examples\Systeem\Input\
```

Alle uitvoerbare testblokken in dit script hebben `TEST_TYPE="NIEUW"`. Een ander `TEST_TYPE` in een INCLUDE-script wordt door `TestLibraryCommon.cmd` als ongeldig beschouwd en stopt de testlijstcontrole voordat de tests starten.

De huidige testlijst bevat de nieuwe Input-tests en de bijbehorende Input+Stimulus-integratietests. Per blok bepalen `TEST_FLAGS` de concrete configuratie en `WELKE_FQBN_TESTEN` op welke FQBN-groep de test wordt uitgevoerd.

De HX1838-tests die `IRremote` gebruiken, bevatten momenteel:

```bat
set WELKE_FQBN_INCOMPATIBLE="arduino:zephyr:unoq"
```

Deze combinatie wordt overgeslagen omdat IRremote voor de UNO Q met Zephyr geen timerimplementatie levert. De HX1838-tests met TinyIRReceiver blijven op de UNO Q uitgevoerd worden.

De eindtabel gebruikt dezelfde drie groepen als de gereleasete tests:

```text
[BASIC_FQBN]
[EXTENDED_FQBN]
Eindstatus BASIC + EXTENDED
[EXPERIMENTAL_FQBN]
Status EXPERIMENTAL_FQBN
```

BASIC + EXTENDED telt mee voor de releasevalidatie. EXPERIMENTAL wordt afzonderlijk gerapporteerd en heeft geen release-impact.

### 5. Negatieve HX1838-configuraties: `TestLibraryNieuwOngeldig.cmd`

`TestLibraryNieuwOngeldig.cmd` volgt dezelfde `INCLUDE`-scope als `TestLibraryNieuw.cmd`: `examples\Systeem\Input\`. Het script bevat de negatieve nieuwe Input-configuraties, waaronder:

- niet-toegelaten combinaties met DIGITAL;
- een ongeldige `HX1838_BRON_CODES`;
- een ongeldige `HX1838_TOETSENINDELING`;
- een ongeldige waarde voor `HX1838_USE_TINYIRRECEIVER_INSTEAD_OF_IRREMOTE`;
- ongeldige nulwaarden in een vaste `HX1838_BRON_CODES_DEFINE`-mapping;
- `HX1838_TOETSENINDELING_REMOTE_USER_DEFINED` met een EEPROM-codebron, ontbrekende verplichte UserDefined-configuratie (`HX1838_GENERIEK_AANTAL_TOETSEN` of `HX1838_GENERIEK_KEY_LAYOUT`), nulcodes in een expliciet opgegeven `HX1838_GENERIEK_CODES`-mapping of een afwijkend aantal codes/layoutentries.

Ook hier is een test alleen `[OK]` wanneer de compilatie faalt én de specifiek verwachte fouttekst voorkomt.

### Controlebestand voor Ongeldig

De Ongeldig-scripts gebruiken eveneens een controlebestand met dezelfde basisnaam. De lijst met mogelijke `TEST_INO`-bestanden wordt daarbij uitsluitend gelezen uit het overeenkomstige gewone testscript:

- `TestLibraryNieuwOngeldig.cmd` → `TestLibraryNieuw.cmd` (`INCLUDE`: `examples\Systeem\Input\`);
- `TestLibraryGedeeldeBusOngeldig.cmd` → `TestLibraryGedeeldeBus.cmd` (`INCLUDE`: `examples\Systeem\GedeeldeBus\`);
- `TestLibraryGereleasedOngeldig.cmd` → `TestLibraryGereleased.cmd` (`EXCLUDE`: `examples\Systeem\Input\` en `examples\Systeem\GedeeldeBus\`).

Ontbrekende `#error`-validaties waarvoor een relevante `TEST_INO` in die scope gevonden wordt, worden als invulblok geschreven in het controlebestand:

```bat
set "TEST_INO=..."
set "TEST_NAAM="
set "TEST_FLAGS="
set "VERWACHTE_FOUT=..."
call :EXPECT_FAIL
```

Bestaat het controlebestand niet, dan wordt het opgebouwd. Is het groter dan 0 byte, dan stoppen de tests zodat de gegenereerde blokken gecontroleerd en desgewenst overgenomen kunnen worden. Is het bestand 0 byte, dan wordt niet opnieuw gegenereerd en starten de bestaande Ongeldig-tests.

### 6. GedeeldeBus-tests: `TestLibraryGedeeldeBus.cmd` en `TestLibraryGedeeldeBusOngeldig.cmd`

`TestLibraryGedeeldeBus.cmd` gebruikt `CONTROLE_MODUS=INCLUDE` voor:

```text
examples\Systeem\GedeeldeBus\
```

Het script geeft daarnaast deze extra compileflag mee aan de betrokken testblokken:

```text
-DGEDEELDE_BUS_PROTOTYPE
```

De gewone GedeeldeBus-tests gebruiken `TestLibraryCommon.cmd`. `TestLibraryGedeeldeBusOngeldig.cmd` gebruikt `TestLibraryCommonOngeldig.cmd` met dezelfde INCLUDE-scope. Beide maken momenteel geen deel uit van de vier logs die `TestLibraryGenereerTESTRESULTATEN.cmd` voor de officiële releasevalidatie verwerkt.

### 7. Mapping-volledigheidscontrole: `TestLibraryMappingControle.cmd`

`TestLibraryMappingControle.cmd` behoudt voor DIGITAL, PCF8574 en HX1838 een brede tekstuele aanwezigheidstest op `mappingTestMenu[]` in `InputkanalenDIGITAL.ino`, `InputkanalenPCF8574.ino` en `InputkanalenHX1838.ino`. De referentielijsten bevatten de volledige opschriftenset van de gecontroleerde types.

Voor de drie ingebouwde HX1838-toetsenindelingen wordt aanvullend per `HX1838_TOETSENINDELING` de eigen `#if`/`#elif`-tak in `InputkanalenHX1838.ino` gecontroleerd. Binnen die tak controleert het script zowel de aanwezigheid als de verwachte volgorde van de opschriften. `HX1838_TOETSENINDELING_REMOTE_USER_DEFINED` is hiervan bewust uitgezonderd omdat de opschriften door de gebruiker worden bepaald; daarvoor wordt `InputkanalenHX1838UserDefined.ino` gecompileerd met een expliciete generieke testconfiguratie. De referentietabellen worden handmatig onderhouden en moeten bij nieuwe of gewijzigde ingebouwde keypadtypes/toetsenindelingen mee bijgewerkt worden.

### 8. Testresultaten genereren: `TestLibraryGenereerTESTRESULTATEN.cmd`

De vier compilecycli schrijven elk hun eigen logbestand:

```text
TestLibraryGereleased.txt
TestLibraryGereleasedOngeldig.txt
TestLibraryNieuw.txt
TestLibraryNieuwOngeldig.txt
```

Nadat deze vier scripts afzonderlijk zijn uitgevoerd, voegt `TestLibraryGenereerTESTRESULTATEN.cmd` ze samen tot `TestLibraryStatusReport.txt`, leest het de nieuwe BASIC/EXTENDED/EXPERIMENTAL-eindtabellen uit, beoordeelt het de statische controles en geheugengegevens en voert het de verplichte `TestLibraryMappingControle.cmd` automatisch uit. Alleen wanneer de vier compilecycli, de releasegerichte BASIC + EXTENDED-resultaten, de statische controles, de geheugengegevens én de mappingcontrole voldoen, kan het statusrapport `RELEASEVALIDATIE GESLAAGD` melden. `EXPERIMENTAL_FQBN` wordt afzonderlijk gerapporteerd en heeft geen release-impact.

## Definitieve releaseresultaten

De definitieve resultaten van een officiële release worden pas na de volledige testcyclus vastgelegd in:

```text
extras/TESTRESULTATEN.md
```
