# Testprocedure

Elke release van het GroeiAcademie FrameWork wordt vóór publicatie met de meegeleverde testscripts gevalideerd. Hierdoor wordt gecontroleerd dat de library voldoet aan de kwaliteitsvereisten van het project en compatibel blijft met de ondersteunde Arduino-platformen.

## Uit te voeren script

De testscripts staan onder `extras/`:

```text
TestLibraryStage1.cmd
TestLibraryStage2.cmd
TestLibraryStage3.cmd
TestLibraryStage3metLogbestand.cmd
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

#### Geteste boards

- Arduino UNO R3 met FQBN `arduino:avr:uno` en `BOARD_VERSION=BOARD_UNO_R3`;
- Arduino UNO R4 Minima met FQBN `arduino:renesas_uno:minima` en `BOARD_VERSION=BOARD_UNO_R4_MINIMA`;
- Arduino UNO R4 WiFi met FQBN `arduino:renesas_uno:unor4wifi` en `BOARD_VERSION=BOARD_UNO_R4_WIFI`;
- ESP32-borden in Arduino Uno R3-vormfactor via boardprofiel `WEMOS D1 R32`, met FQBN `esp32:esp32:d1_uno32` en `BOARD_VERSION=BOARD_ESP32_UNO`.

Voor WEMOS D1 R32, TTGO D1 R32 en compatibele ESP32-borden in Arduino Uno R3-vormfactor moet via Arduino Boards Manager `esp32 by Espressif Systems` geïnstalleerd zijn. Kies daarna het boardprofiel `WEMOS D1 R32`. De testscripts gebruiken FQBN `esp32:esp32:d1_uno32`. Een toolpakket zoals `esp32:esp-rv32@2601` wordt samen met het ESP32-boardpakket geïnstalleerd en wordt niet als board geselecteerd.

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

`TestLibraryStage3.cmd` en `TestLibraryStage3metLogbestand.cmd` compileren ieder Stimulusvoorbeeld met `SCREEN_OUTPUT_CONFIG` 0 tot en met 7.

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
