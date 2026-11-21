# Testprocedure

## Testlagen beta v2.0.0

De huidige code en examples maken drie niveaus expliciet van elkaar los:

1. **Lifecycle- en resourcecontrole**: `aanmelden()`, `controleren()`, `inpluggen()` en `activeren()`, inclusief conflicten en toegelaten/geweigerde `ExtenderPins`.
2. **Elektronische functietest**: fysieke I/O, ADC, interrupts, UART-loopback, muxkanalen enzovoort. Deze laag is nog niet voor alle componenten geïmplementeerd.
3. **Praktijktest**: echte Sensor of randapparatuur via de volledige keten.

Een geslaagde Extender-lifecycletest bewijst dus niet dat alle fysieke chipfuncties elektrisch doorgemeten zijn.

`GedeeldeBusNode` bevat onder `DEBUG` momenteel alleen een lege virtuele `Diagnose()`-hook. Er bestaan nog geen concrete Diagnose-overrides; elektronische functietests worden dus nog niet automatisch door `Diagnose()` uitgevoerd.

## Testscripts v2.0.0

De huidige teststructuur bestaat uit negen domeinen. Elk domein heeft een gewoon testscript en een Ongeldig-testscript:

```text
TestLibraryData-v2.0.0.cmd
TestLibraryDataOngeldig-v2.0.0.cmd

TestLibraryExtenders-v2.0.0.cmd
TestLibraryExtendersOngeldig-v2.0.0.cmd

TestLibraryFoutinjecties-v2.0.0.cmd
TestLibraryFoutinjectiesOngeldig-v2.0.0.cmd

TestLibraryGedeeldeBus-v2.0.0.cmd
TestLibraryGedeeldeBusOngeldig-v2.0.0.cmd

TestLibraryInput-v2.0.0.cmd
TestLibraryInputOngeldig-v2.0.0.cmd

TestLibraryScreen-v2.0.0.cmd
TestLibraryScreenOngeldig-v2.0.0.cmd

TestLibrarySensoren-v2.0.0.cmd
TestLibrarySensorenOngeldig-v2.0.0.cmd

TestLibrarySturingen-v2.0.0.cmd
TestLibrarySturingenOngeldig-v2.0.0.cmd

TestLibraryTools-v2.0.0.cmd
TestLibraryToolsOngeldig-v2.0.0.cmd
```

Daarnaast worden gebruikt:

```text
TestLibraryCommon.cmd
TestLibraryCommonOngeldig.cmd
TestLibraryDependency.cmd
TestBoardplatformsDependency.cmd
TestLibraryMappingControle.cmd
TestLibraryAllCmdParallel.cmd
TestLibraryGenereerTESTRESULTATEN.cmd
LokalePaden_template.cmd
```

De achttien domeincycli worden afzonderlijk uitgevoerd. Daarna beoordeelt `TestLibraryGenereerTESTRESULTATEN.cmd` de achttien bijbehorende `.txt`-logs en voert het automatisch `TestLibraryMappingControle.cmd --no-pause` uit.

Een releasevalidatie is alleen geslaagd wanneer alle achttien domeincycli volledig zijn afgerond, alle releasecriteria slagen en ook de mappingcontrole slaagt.

## Padkeuze van de Windows-tests

De scripts zoeken `arduino-cli` eerst via Windows `PATH`. Wanneer dat niet lukt, wordt `extras/LokalePaden.cmd` geladen. Maak dat lokale bestand indien nodig door `extras/LokalePaden_template.cmd` naar `extras/LokalePaden.cmd` te kopiëren en daar de paden van de eigen machine in te vullen. Het lokale bestand staat in `.gitignore` en wordt niet gepubliceerd.

De gewone tests zoeken `arduino-lint` eveneens via `PATH`. Wanneer `arduino-lint` daar niet gevonden wordt en `LINT_PATH` niet via `LokalePaden.cmd` is ingesteld, probeert `TestLibraryCommon.cmd` `arduino-lint` in dezelfde map als `arduino-cli`.

## Schone testconfiguratie

De compiletests definiëren `GROEIACADEMIE_IGNORE_USER_CONFIG`. Een eventueel lokaal `src/Configuratie/UserConfig.h` wordt tijdens de compiletests dus bewust genegeerd. De testconfiguratie bestaat uit de expliciete compilerdefinities van het betrokken testgeval plus de fallbacks van `SystemConfig.h`.

Een `#define` in een `.ino` geldt alleen binnen die sketch-translation-unit en configureert geen apart gecompileerd librarybestand. Instellingen die ook voor librarybronbestanden moeten gelden, worden daarom via `TEST_FLAGS` als compilerdefinitie meegegeven.

## Domeineigenaarschap

`CONTROLE_PADEN` bepaalt welk domein eigenaar is van de voorbeelden en controles die door een testscript worden beheerd. Twee aanvullende Ongeldig-testgevallen gebruiken bewust een voorbeeld uit een ander domein: de FsrRfp-test met twee ADS1115-Extenders in `TestLibraryExtendersOngeldig-v2.0.0.cmd` en het microSD-voorbeeld in `TestLibrarySensorenOngeldig-v2.0.0.cmd`. Die voorbeelden blijven ook in het gewone script van hun eigen domein geregistreerd.

De negen gewone domeinscripts gebruiken `TestLibraryCommon.cmd`.

De negen negatieve domeinscripts gebruiken `TestLibraryCommonOngeldig.cmd` en verwijzen via `BRON_TEST_SCRIPT` naar het gewone testscript van hetzelfde domein.

De testarchitectuur blijft daardoor per domein opgesplitst. Een test hoort in het domein dat door de betrokken `CONTROLE_PADEN` wordt beheerd.

## TEST_TYPE

Een gewoon testblok gebruikt één van deze waarden:

- `NIEUW`
- `REGRESSIE`
- `GERELEASED`
- `EXPERIMENTEEL`

`TEST_TYPE` beschrijft de status van het testgeval. De domeinscripts mogen deze statussen naast elkaar bevatten.

Met:

```text
REGRESSIETEST=TRUE
```

worden de aanvullende regressietests uitgevoerd.

Voorbeeld:

```text
TestLibraryInput-v2.0.0.cmd REGRESSIETEST=TRUE
```

## Gemeenschappelijke FQBN-groepen

`TestLibraryCommon.cmd` gebruikt drie FQBN-groepen:

- `BASIC_FQBN`
- `EXTENDED_FQBN`
- `EXPERIMENTAL_FQBN`

Per testblok bepaalt `WELKE_FQBN_TESTEN` op welke groep of groepen het testgeval wordt uitgevoerd.

Voor de releasevalidatie geldt:

- BASIC + EXTENDED bepaalt de release-eindstatus van het gewone domein;
- EXPERIMENTAL_FQBN wordt afzonderlijk beoordeeld en gerapporteerd;
- EXPERIMENTAL_FQBN heeft geen release-impact.

`WELKE_FQBN_INCOMPATIBLE` en `WELKE_FQBN_SKIPPEN` kunnen test/FQBN-combinaties bewust uitsluiten. Zulke combinaties worden niet in de testtotalen opgenomen.

## Gewone controlebestanden `.ino`

Voor elk gewoon domeinscript gebruikt `TestLibraryCommon.cmd` een controlebestand met dezelfde basisnaam.

De controle vergelijkt de voorbeelden binnen `CONTROLE_PADEN` met de aanwezige `TEST_INO`-vermeldingen. Ontbrekende testblokken worden via het controlebestand gegenereerd.

Wanneer het controlebestand aangeeft dat de testlijst moet worden bijgewerkt, worden de gewone tests niet gestart voordat die controle is afgehandeld.

## Ongeldig-controlebestanden `.ino`

`TestLibraryCommonOngeldig.cmd` zoekt de negatieve compilevalidaties die bij het domein horen en vergelijkt die met de bestaande `VERWACHTE_FOUT`-tests.

Ontbrekende negatieve tests worden als invulblok in het controlebestand geschreven:

```bat
set "TEST_INO=..."
set "TEST_NAAM="
set "TEST_FLAGS="
set "VERWACHTE_FOUT=..."
call :EXPECT_FAIL
```

De controle-`.ino` wordt opnieuw opgebouwd zodat een test uit het controlebestand verdwijnt zodra die correct in het betrokken `*Ongeldig-v2.0.0.cmd`-bestand aanwezig is.

Een Ongeldig-test is alleen geslaagd wanneer:

1. de compilatie mislukt;
2. de verwachte fout wordt in de compile-uitvoer gevonden.

Een toevallige andere compilefout is dus geen geslaagde negatieve test.

## Uitvoering zonder pauze

De domeinscripts ondersteunen `--no-pause` voor aanroepen vanuit andere testscripts of automatisering.

De interne `--logged`-werking wordt door de testscripts zelf gebruikt om uitvoer naar het bijbehorende `.txt`-logbestand te sturen. `--logged` is geen normale handmatige testmodus.

## Releaseprocedure

Voer de achttien domeinscripts uit:

```text
TestLibraryData-v2.0.0.cmd
TestLibraryDataOngeldig-v2.0.0.cmd
TestLibraryExtenders-v2.0.0.cmd
TestLibraryExtendersOngeldig-v2.0.0.cmd
TestLibraryFoutinjecties-v2.0.0.cmd
TestLibraryFoutinjectiesOngeldig-v2.0.0.cmd
TestLibraryGedeeldeBus-v2.0.0.cmd
TestLibraryGedeeldeBusOngeldig-v2.0.0.cmd
TestLibraryInput-v2.0.0.cmd
TestLibraryInputOngeldig-v2.0.0.cmd
TestLibraryScreen-v2.0.0.cmd
TestLibraryScreenOngeldig-v2.0.0.cmd
TestLibrarySensoren-v2.0.0.cmd
TestLibrarySensorenOngeldig-v2.0.0.cmd
TestLibrarySturingen-v2.0.0.cmd
TestLibrarySturingenOngeldig-v2.0.0.cmd
TestLibraryTools-v2.0.0.cmd
TestLibraryToolsOngeldig-v2.0.0.cmd
```

Voer daarna uit:

```text
TestLibraryGenereerTESTRESULTATEN.cmd
```

## Beoordeling van een gewoon domein

Voor elk van de negen gewone domeinlogs controleert `TestLibraryGenereerTESTRESULTATEN.cmd`:

- de eindmarker, zodat alleen een volledig afgeronde run geldig is;
- `BASIC_FQBN`;
- `EXTENDED_FQBN`;
- `Eindstatus BASIC + EXTENDED`;
- `EXPERIMENTAL_FQBN`, zonder release-impact;
- de statische configuratiecontroles;
- de `[GEHEUGEN]`-records;
- `Arduino LINT`.

BASIC + EXTENDED, statische controles, geheugengegevens en Arduino LINT moeten slagen voor de release.

EXPERIMENTAL_FQBN wordt wel in het rapport opgenomen maar blokkeert de release niet.

## Beoordeling van een Ongeldig-domein

Voor elk van de negen Ongeldig-logs moet de eindregel de vorm hebben:

```text
Totaal <aantal> - OK <aantal> - FOUT 0
```

`Totaal` en `OK` moeten gelijk zijn. Iedere negatieve test moet dus de bedoelde compilefout hebben veroorzaakt.

## Mappingcontrole

`TestLibraryGenereerTESTRESULTATEN.cmd` voert automatisch uit:

```text
TestLibraryMappingControle.cmd --no-pause
```

Ook deze controle moet slagen.

De volledige releasevalidatie bestaat daardoor uit achttien domeincycli plus de verplichte mappingcontrole.

## TestLibraryStatusReport.txt

`TestLibraryGenereerTESTRESULTATEN.cmd` maakt `TestLibraryStatusReport.txt`.

Het rapport bevat:

- de totaalstatus van de releasevalidatie;
- de status van alle achttien domeincycli;
- per gewoon domein BASIC + EXTENDED, EXPERIMENTAL_FQBN, statische controles, Arduino LINT en geheugengegevens;
- per Ongeldig-domein het negatieve testtotaal;
- de mappingcontrole;
- foutdetails wanneer de releasevalidatie niet slaagt;
- de volledige inhoud van alle aanwezige domeinlogs.

De libraryversie wordt gelezen uit `library.properties`.

Na een volledig geslaagde releasevalidatie worden de achttien losse `.txt`-domeinlogs verwijderd. `TestLibraryStatusReport.txt` blijft behouden.

Wanneer de releasevalidatie niet slaagt, blijven de losse logs behouden en toont de generator welke scripts opnieuw moeten worden uitgevoerd.

## Historische teststatus

Onderstaande vaststellingen betreffen uitsluitend een oudere testsituatie en zijn geen bewijs van de actuele releasevalidatie. De huidige testscripts omvatten naast Data, Extenders, GedeeldeBus, Input, Screen, Sensoren en Tools ook Foutinjecties en Sturingen.

De Ongeldig-infrastructuur is aangepast zodat de controlebestanden opnieuw worden opgebouwd en versiegebonden `*Ongeldig-v2.0.0.cmd`-bestanden worden meegenomen.

De laatst getoonde Data-run eindigde op:

```text
Totaal 25 - OK 24 - FOUT 1
```

De resterende fout zat in de negatieve microSD-test met CS via `ExtenderPCF8574`: de testconfiguratie activeerde de extender niet volledig en faalde daardoor vóór de bedoelde `SD_CHIP_SELECT_MODE`-validatie. Die `TEST_FLAGS` zijn daarna aangepast aan de bestaande werkende microSD-via-PCF8574-configuratie, zonder `SD_CHIP_SELECT_MODE=1`, zodat de bedoelde negatieve validatie kan worden bereikt.

Die laatste correctie moet nog met een nieuwe Data-run worden bevestigd. De overige Ongeldig-domeinen moeten eveneens met de finale testset opnieuw worden uitgevoerd voordat de volledige releasevalidatie bewezen geslaagd is.

## Bestaande aanvullende Sensor-sketches

De volgende bestaande Sensor-sketches worden niet als afzonderlijke sensorklasse in de component-MD's beschreven. Hun paden zijn hier expliciet opgenomen; een sketchnaam die op een fouttest lijkt, bepaalt **niet** zelfstandig of de registratie in Geldig of Ongeldig thuishoort. Die indeling volgt uitsluitend de bestaande CMD-testblokken.

- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_FsrRfp_Test_ADC_Bezet/Sensor_FsrRfp_Test_ADC_Bezet.ino` (geregistreerd: Geldig).
- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_FsrRfp_Test_EigenBuffers/Sensor_FsrRfp_Test_EigenBuffers.ino` (geregistreerd: Geldig).
- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_FsrRfp_Test_Hardwarefout/Sensor_FsrRfp_Test_Hardwarefout.ino` (geregistreerd: Geldig).
- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_FsrRfp_Test_OngeldigePin/Sensor_FsrRfp_Test_OngeldigePin.ino` (geregistreerd: Geldig).
- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_FsrRfp_Test_Timeout_Conversie/Sensor_FsrRfp_Test_Timeout_Conversie.ino` (geregistreerd: Geldig).
- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_FsrRfp_Test_VerkeerdeParent/Sensor_FsrRfp_Test_VerkeerdeParent.ino` (geregistreerd: Geldig).
- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_FsrRfp_Test_ZonderGet/Sensor_FsrRfp_Test_ZonderGet.ino` (geregistreerd: Geldig).
- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_FsrRfp_TweeSetsOpTweeADS1115/Sensor_FsrRfp_TweeSetsOpTweeADS1115.ino` (geregistreerd: Sensoren Geldig én Extenders Ongeldig, met verschillende `TEST_FLAGS`).
- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_MS3625/Sensor_MS3625.ino` (geregistreerd: Geldig en Ongeldig).
- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_MixedPins/Sensor_MixedPins.ino` (geregistreerd: Geldig en Ongeldig).
- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_TestOpDubbeleComponentcodes/Sensor_TestOpDubbeleComponentcodes.ino` (geregistreerd: Geldig).
- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_XYSTICK/Sensor_XYSTICK.ino` (geregistreerd: Geldig).

## Definitieve releaseresultaten

De definitieve releaseresultaten worden bijgehouden in `extras/TESTRESULTATEN.md`.

## Aanvullende werking van de testscripts

### Eindmarker zonder pauze

- Met `--no-pause` schrijven `TestLibraryCommon.cmd` en `TestLibraryCommonOngeldig.cmd` nu ook de eindmarker `[EINDE] Testen voltooid. Druk op een toets om af te sluiten...`, zonder te wachten. Zo herkent de rapportgenerator ook een run zonder pauze als afgerond.

### Strengere beoordeling door de rapportgenerator

- Een Ongeldig-domein slaagt alleen als `Totaal` groter is dan 0, `Totaal` gelijk is aan `OK` en `FOUT` 0 is.
- Een gewoon domein slaagt alleen als het totaal groter is dan 0, en als het totaal gelijk is aan OK plus verwachte geheugenbeperkingen plus onverwacht mislukt.
- Of alle gedefinieerde tests echt uitgevoerd zijn, controleert de generator niet. Een run die voortijdig stopt, mist de eindmarker en wordt daardoor als onvolledig gemeld. Een test die uit een script verdwijnt, valt alleen op bij het nakijken van de scripts.

### Herkomst en behoud van het rapport

- De generator controleert niet of een log van de huidige code komt, en wist `TestLibraryStatusReport.txt` voor hij de logs beoordeelt.
- Daarom vóór elke vrijgave: het vorige `TestLibraryStatusReport.txt` zelf bewaren, en alle 18 domeinlogs vers maken met de code die vrijgegeven wordt.

### `TestLibraryAllCmdParallel.cmd`

- Dit script opent alleen de tabbladen met de domeinscripts en eindigt meteen met code 0. Die 0 betekent "gestart", niet "geslaagd". Het resultaat komt uit `TestLibraryGenereerTESTRESULTATEN.cmd`.

### De 18 lege controlebestanden `.ino`

- De bestanden `extras/TestLibrary<domein>[Ongeldig]-v2.0.0.ino` zijn bewust leeg (0 bytes). Het zijn de controlebestanden uit de secties hierboven: ze tonen of een testlijst volledig is. Verwijder ze niet; de scripts maken ze anders opnieuw aan.

### Testregistraties

- `TestLibraryData-v2.0.0.cmd`: `FLASH_StartStopRead` en `microSD_StartStopRead` (starten, stoppen, opnieuw starten en teruglezen), en HC-05 nu op alle boards via `GA_SENSOR_SERIAL`.
- `TestLibraryGedeeldeBus-v2.0.0.cmd`: `Scenario2_Simultaan`, `Scenario3_Ineenstortend` en `Scenario4_Cocktail` (Stimulus), elk met `INPUT_TYPE_NONE`, zoals `Scenario1_EnkelTik`.
- `TestLibraryDependency.cmd` vermeldt nu PCF8574 0.4.5 als minimumversie; de versie zelf nakijken blijft handwerk.

## EXPERIMENTEEL-tests en Ongeldig-tests

### EXPERIMENTEEL wordt gecompileerd

- **Uitvoering:** `TestLibraryCommon.cmd` compileert in `CONTROLE_MODUS=INCLUDE` ook tests met `TEST_TYPE="EXPERIMENTEEL"`. In `TestLibraryExtenders-v2.0.0.cmd` zijn daarvoor 41 verschillende voorbeelden met 53 testgevallen geregistreerd.
- **Hoe ze tellen:** een EXPERIMENTEEL-test telt op elk board in de tellers `EXPERIMENTAL_*`, nooit in `BASIC_*` of `EXTENDED_*`. Een mislukte EXPERIMENTEEL-test blokkeert de vrijgave dus niet.
- **In het rapport:** het blok "EXPERIMENTAL_FQBN" bevat nu de resultaten van de UNO Q én die van de EXPERIMENTEEL-tests op alle boards. Elke compilatieregel toont het board en `TEST_TYPE`.
- **Testduur:** een run van de Extenders duurt langer, ongeveer 53 testgevallen maal het aantal boards.
- **Vrijgave:** beoordeel na de volledige Windows-run per voorbeeld en per board of het testtype naar `NIEUW` kan worden omgezet; die omzetting vereist afzonderlijke goedkeuring.
- **Uitzondering:** een onbekende `WELKE_FQBN_TESTEN` blijft `BASIC_FAIL` verhogen, ook bij een EXPERIMENTEEL-test. Dat is een fout in het script zelf, geen mislukte compilatie.

### Ongeldig-testgevallen

27 gerichte tests met bestaande voorbeelden:

- **`TestLibrarySensorenOngeldig-v2.0.0.cmd` (4):**
  - `NATIVE_ADC_RESOLUTIE_BITS`;
  - `FSR_RFP_PROBE_SET_2`;
  - de drempels van FsrRfp, algemeen en voor kanaal 1.
- **`TestLibraryDataOngeldig-v2.0.0.cmd` (5):**
  - `DATA_LAYER_SENSOR_QUEUE_OR_STACK_SIZE`;
  - de ondergrens van de FLASH-buffer;
  - de CS-Extender van de FLASH;
  - de CS-Extender van de microSD;
  - het nRF24-adres. Die test gebruikt `__DATE__` als adres: een vaste tekst van 11 tekens, zonder aanhalingstekens in de vlag.
- **`TestLibraryExtendersOngeldig-v2.0.0.cmd` (3):** de drie PLL-instellingen van de MAX14830.
- **`TestLibraryGedeeldeBusOngeldig-v2.0.0.cmd` (3):** de grens van `RUNTIME_WARNING_…`, de oplopende foutladder en de drempel van de ADS1115.
- **`TestLibraryInputOngeldig-v2.0.0.cmd` (12):** de instelbare grenzen van het PCF8574-keypad.
  - direct: geen pin, meer dan 8, de lengte van `KEY_LAYOUT`, een pin buiten 0 tot 7, een dubbele pin;
  - matrix: geen rij, geen kolom, meer dan 8, de lengte van `KEY_LAYOUT`, een pin buiten 0 tot 7, een dubbele pin, een pin die zowel rij als kolom is.

### Wat bewust geen Ongeldig-test heeft

- **Twee `#error` in SystemConfig.h:** de taalkeuze en "board niet ondersteund".
- **Vier `static_assert` die een gebruiker niet kan veroorzaken:**
  - de twee vaste constanten van het keypad (Input.cpp, regels 415 en 418);
  - de EEPROM-grootte van de HX1838 (Input.cpp, regel 762), want die grootte ligt vast;
  - "binaire float vereist 32 bit".
- **Varianten van een getest soort:** bijvoorbeeld _2 naast _1, of de drempels van de andere Extenders. Daarvoor bestaat één representatieve test.
- Het detailoverzicht van de testdekking wordt apart van de library bijgehouden; de daadwerkelijke testregistraties staan in de bestaande Ongeldig-CMD-bestanden.

### Andere foutmeldingen in de uitvoer

- Bij een paar keypad-tests faalt een tweede controle mee, omdat de fout ze allebei raakt. Een voorbeeld: 9 directe pinnen zijn altijd te veel én bevatten altijd een dubbele pin.
- Een test met 0 pinnen geeft ook een fout van de compiler over een lege tabel.
- De verwachte melding staat in elk geval in de uitvoer, en de runner zoekt die tekst.
