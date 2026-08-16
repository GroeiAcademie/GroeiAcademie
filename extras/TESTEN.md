# Testprocedure

Elke release van het GroeiAcademie FrameWork wordt vóór publicatie met de meegeleverde testscripts gevalideerd. Hierdoor wordt gecontroleerd dat de library voldoet aan de kwaliteitsvereisten van het project en compatibel blijft met de ondersteunde Arduino-platformen.

## Uit te voeren script

De testscripts staan onder `extras/`:

```text
TestLibraryGereleased.cmd
TestLibraryGereleasedOngeldig.cmd
TestLibraryNieuw.cmd
TestLibraryNieuwOngeldig.cmd
TestLibraryDependency.cmd
TestLibraryMappingControle.cmd
TestLibraryStatusReport.cmd
TestLibraryAllesEnMaakStatusReport.cmd
```

De scripts laten een aanwezige `UserConfig.h` actief. De configuratievolgorde tijdens de tests is: compilerdefinitie, actieve definitie in het `.ino`-voorbeeld, `UserConfig.h` en ten slotte de standaardwaarde uit `SystemConfig.h`. De actieve exampledefaults die door de compiler kunnen worden overschreven staan daarom onder `#ifndef`.

## Teststappen

### 1. Arduino LINT

Controle:

```text
arduino-lint --library-manager submit
```

Doel:

- controle op de vereisten van de Arduino Library Manager;
- controle van metadata;
- controle van de directorystructuur;
- controle van de librarybestanden.

Een mislukte Arduino LINT wordt afzonderlijk bijgehouden en maakt de volledige releasevalidatie ongeldig.

### 2. Compilatietesten

Alle voorbeelden worden automatisch gecompileerd. Iedere compilatie gebruikt `--clean --jobs 1`, zodat geen oude buildcache wordt hergebruikt en de compilaties na elkaar worden uitgevoerd.

#### Geteste Arduino Uno R3-vormfactorborden

- Arduino UNO R3 met FQBN `arduino:avr:uno` en `BOARD_VERSION=BOARD_UNO_R3`;
- Arduino UNO R4 Minima met FQBN `arduino:renesas_uno:minima` en `BOARD_VERSION=BOARD_UNO_R4_MINIMA`;
- Arduino UNO R4 WiFi met FQBN `arduino:renesas_uno:unor4wifi` en `BOARD_VERSION=BOARD_UNO_R4_WIFI`;
- ESP32-borden in Arduino Uno R3-vormfactor via boardprofiel `WEMOS D1 R32`, met FQBN `esp32:esp32:d1_uno32` en `BOARD_VERSION=BOARD_ESP32_UNO`.

Voor WEMOS D1 R32, TTGO D1 R32 en compatibele ESP32-borden in Arduino Uno R3-vormfactor moet via Arduino Boards Manager `esp32 by Espressif Systems` geïnstalleerd zijn. Kies daarna het boardprofiel `WEMOS D1 R32`. De testscripts gebruiken FQBN `esp32:esp32:d1_uno32`. Een toolpakket zoals `esp32:esp-rv32@2601` wordt samen met het ESP32-boardpakket geïnstalleerd en wordt niet als Arduino Uno R3-vormfactorbord geselecteerd.

De batch koppelt iedere FQBN expliciet aan de overeenkomstige `BOARD_VERSION`.

#### Geteste voorbeelden

Alle `.ino`-bestanden onder:

```text
examples/
```

worden recursief gevonden. Nieuwe examples en nieuwe submappen worden daardoor automatisch meegenomen.

### 3. Schermconfiguratie

Tijdens de compilatie wordt `SCREEN_OUTPUT_CONFIG` waar nodig door het testscript opgegeven. Een actieve `UserConfig.h` blijft geladen; het `#ifndef` rond `SCREEN_OUTPUT_CONFIG` voorkomt dat die testwaarde opnieuw wordt gedefinieerd.

| Voorbeeld | SCREEN_OUTPUT_CONFIG |
|-----------|----------------------|
| CharacterScreen | SCREEN_TYPE_CHARACTER |
| PixelScreen | SCREEN_TYPE_PIXELS |
| CharacterScreen_PixelScreen | SCREEN_TYPE_CHARACTER \| SCREEN_TYPE_PIXELS |

`TestLibraryGereleased.cmd` en `TestLibraryGereleased.cmd` compileren ieder Stimulusvoorbeeld met `SCREEN_OUTPUT_CONFIG` 0 tot en met 7.

Wanneer `DEBUG` actief is, voegt de library automatisch `SCREEN_TYPE_SERIAL` toe.

### 4. Gekende UNO R3-geheugenbeperking

Het samengestelde voorbeeld `Tik_Enkele_Samen_Instortend_Cocktail` overschrijdt op Arduino UNO R3 het beschikbare programmageheugen bij `SCREEN_OUTPUT_CONFIG` 4, 5, 6 en 7. Deze vier combinaties worden uitsluitend als verwachte geheugenbeperking geregistreerd wanneer de compiler meldt:

```text
text section exceeds available space in board
```

De vier combinaties tellen niet als succesvol gecompileerd en evenmin als onverwachte compilatiefout. Iedere andere fout, ook binnen deze combinaties, blijft een onverwachte compilatiefout.

### 5. Foutafhandeling

Bij een onverwachte compilatiefout:

- wordt de fout weergegeven;
- pauzeert het script zodat de fout bekeken kan worden;
- gaat de test verder met het volgende voorbeeld.

Hierdoor worden altijd alle testen uitgevoerd.

### 6. Samenvatting

Na afloop wordt automatisch weergegeven:

- totaal aantal uitgevoerde compilaties;
- aantal succesvolle compilaties;
- aantal verwachte UNO R3-geheugenbeperkingen;
- aantal onverwachte compilatiefouten;
- resultaat van Arduino LINT.

De resultaten van elke officiële release worden bijgehouden in:

```text
extras/TESTRESULTATEN.md
```


### BOARD_ESP32_UNO

Arduino Uno R3-vormfactor ESP32-boardprofiel.

#### Getest en ondersteund sinds v1.0.0
- WEMOS D1 R32.

#### Hetzelfde boardprofiel
- TTGO D1 R32 gebruikt eveneens FQBN `esp32:esp32:d1_uno32`.

#### Verwacht compatibel
- Andere Arduino Uno R3-vormfactor ESP32-borden met dezelfde Arduino-pinout en een ondersteunde Arduino-ESP32-core.

De fysieke hardwarevalidatie van TTGO D1 R32 en andere compatibele borden wordt afzonderlijk vastgelegd zodra die is uitgevoerd.

## Input-validatie voor v1.1.0

`TestLibraryNieuw.cmd` test geldige Input-configuraties en rapporteert officiële en acceptatieboards afzonderlijk. Acceptatieboards hebben geen invloed op release-PASS/FAIL. Dit omvat ook `KEYPAD_TYPE_USER_DEFINED_DIRECT`/`KEYPAD_TYPE_USER_DEFINED_MATRIX`, via de eigen voorbeelden `InputkanalenUserDefinedDirect.ino`/`InputkanalenUserDefinedMatrix.ino` (die hun eigen, ingebouwde standaardwaarden gebruiken, geen `UserConfig.h`-aanpassing nodig om te compileren).

`TestLibraryNieuwOngeldig.cmd` test configuraties die bewust door compile-time validatie geweigerd moeten worden.

`TestLibraryAllesEnMaakStatusReport.cmd` combineert de nieuwe Input-tests van deze release met de volledige, reeds gereleasede basis.

### Padkeuze van de Windows-tests

De nieuwe Input-tests gebruiken dezelfde padkeuze als `TestLibraryGereleased.cmd`:

Het eerste bestaande pad wordt `BASE_PATH`; `arduino-cli` wordt vervolgens als `%BASE_PATH%\arduino-cli` gebruikt.

Elk van de vier testscripts (`TestLibraryGereleased.cmd`, `TestLibraryGereleasedOngeldig.cmd`, `TestLibraryNieuw.cmd`, `TestLibraryNieuwOngeldig.cmd`) schrijft zijn eigen volledige console-uitvoer naar een gelijknamig `.txt`-logbestand. `TestLibraryStatusReport.cmd` voegt die vier logbestanden samen tot één `TestLibraryStatusReport.txt`, met een overzicht vooraan.


### Volledige testsuite in één opdracht

Start `TestLibraryAllesEnMaakStatusReport.cmd` om de volledige v1.1.0-validatie uit te voeren. Dit script roept na elkaar `TestLibraryGereleased.cmd`, `TestLibraryGereleasedOngeldig.cmd`, `TestLibraryNieuw.cmd` en `TestLibraryNieuwOngeldig.cmd` op (telkens met `--no-pause`), en roept vervolgens `TestLibraryStatusReport.cmd` op om alle vier de logbestanden samen te voegen tot `TestLibraryStatusReport.txt`. Het script slaagt enkel wanneer alle vijf de onderliggende aanroepen slagen.

De in `INPUT_STIMULUS_TESTS` opgesomde Stimulus-testvoorbeelden onder `examples/Systeem/Input/Input_Test_...` worden door `TestLibraryNieuw.cmd` mee gecompileerd. De originele Stimulus-voorbeelden blijven ongewijzigd.



### Input_Test conversie

De `Input_Test_...`-voorbeelden zijn kopieën van de bestaande Stimulus-voorbeelden. De originele Stimulus-bestanden blijven ongewijzigd. In de testkopieën worden de vier fysieke toetsaanslagen via `InputConfigureren()` en `OpvragenHuidigeToetsAanslag(true)` ingelezen. De bestaande keuzevolgorde en toepassingsfuncties blijven behouden.

`InputTestConversieControle.txt` rapporteert per testvoorbeeld hoeveel Input-aanroepen aanwezig zijn en of nog actieve rechtstreekse `digitalRead(PIN_TOETS_x)`-aanroepen overblijven.

Start `TestLibraryAllesEnMaakStatusReport.cmd` voor de volledige testsuite. Alle deeltestuitvoer wordt samengebracht in `TestLibraryStatusReport.txt`.

## Mapping-volledigheidscontrole

`TestLibraryMappingControle.cmd` controleert of `mappingTestMenu[]` in elk van de drie
Input-voorbeelden (`InputkanalenDIGITAL.ino`, `InputkanalenPCF8574.ino`, `InputkanalenHX1838.ino`)
alle opschriften bevat die het bijhorende `KEYPAD_TYPE`/`HX1838_TOETSENINDELING` via `KEY_LAYOUT[]`/
`IR_KEY_LAYOUT[]` in `Input.cpp` kan opleveren. Een ontbrekend opschrift meldt zich stilzwijgend
als "niets gebeurt bij een toetsdruk", zonder compilatiefout of waarschuwing: dit script vangt dat
alsnog op, vóór een release.

Werking: het script doorloopt een handmatig opgebouwde referentietabel (welk type welke opschriften
verwacht) en controleert per combinatie of die opschriften letterlijk voorkomen in het betrokken
`.ino`-bestand.

**Belangrijke beperking:** de controle is tekstueel, niet `#if`/`#elif`-bewust. Ze bevestigt dat een
opschrift ergens in het bestand voorkomt, niet dat het in de juiste voorwaardelijke tak staat. Een
opschrift dat per ongeluk in de verkeerde tak terechtkomt, wordt dus niet gedetecteerd.

**Onderhoud:** bij een nieuw `KEYPAD_TYPE`, een nieuwe `HX1838_TOETSENINDELING`, of een wijziging aan
een bestaande `KEY_LAYOUT[]`/`IR_KEY_LAYOUT[]` in `Input.cpp`, moet de referentietabel in dit script
zelf mee bijgewerkt worden. Er is geen automatische koppeling tussen dit script en `Input.cpp`.
