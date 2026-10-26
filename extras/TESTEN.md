# Testprocedure

## Testlagen beta v2.0.0

De huidige code en examples maken drie niveaus expliciet van elkaar los:

1. **Lifecycle- en resourcecontrole**: `aanmelden()`, `controleren()`, `inpluggen()` en `activeren()`, inclusief conflicten en toegelaten/geweigerde `ExtenderPins`.
2. **Elektronische functietest**: fysieke I/O, ADC, interrupts, UART-loopback, muxkanalen enzovoort. Deze laag is nog niet voor alle componenten geïmplementeerd.
3. **Praktijktest**: echte Sensor of randapparatuur via de volledige keten.

Een geslaagde Extender-lifecycletest bewijst dus niet dat alle fysieke chipfuncties elektrisch doorgemeten zijn.

`GedeeldeBusNode` bevat onder `DEBUG` momenteel alleen een lege virtuele `Diagnose()`-hook. Er bestaan nog geen concrete Diagnose-overrides; elektronische functietests worden dus nog niet automatisch door `Diagnose()` uitgevoerd.

## Testscripts v2.0.0

De releasevalidatie bestaat uit zeven domeinen. Elk domein heeft een gewoon testscript en een Ongeldig-testscript:

```text
TestLibraryData-v2.0.0.cmd
TestLibraryDataOngeldig-v2.0.0.cmd

TestLibraryExtenders-v2.0.0.cmd
TestLibraryExtendersOngeldig-v2.0.0.cmd

TestLibraryGedeeldeBus-v2.0.0.cmd
TestLibraryGedeeldeBusOngeldig-v2.0.0.cmd

TestLibraryInput-v2.0.0.cmd
TestLibraryInputOngeldig-v2.0.0.cmd

TestLibraryScreen-v2.0.0.cmd
TestLibraryScreenOngeldig-v2.0.0.cmd

TestLibrarySensoren-v2.0.0.cmd
TestLibrarySensorenOngeldig-v2.0.0.cmd

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
TestLibraryGenereerTESTRESULTATEN.cmd
LokalePaden_template.cmd
```

De veertien domeincycli worden afzonderlijk uitgevoerd. Daarna beoordeelt `TestLibraryGenereerTESTRESULTATEN.cmd` de veertien bijbehorende `.txt`-logs en voert het automatisch `TestLibraryMappingControle.cmd --no-pause` uit.

Een releasevalidatie is alleen geslaagd wanneer alle veertien domeincycli volledig zijn afgerond, alle releasecriteria slagen en ook de mappingcontrole slaagt.

## Padkeuze van de Windows-tests

De scripts zoeken `arduino-cli` eerst via Windows `PATH`. Wanneer dat niet lukt, wordt `extras/LokalePaden.cmd` geladen. Maak dat lokale bestand indien nodig door `extras/LokalePaden_template.cmd` naar `extras/LokalePaden.cmd` te kopiëren en daar de paden van de eigen machine in te vullen. Het lokale bestand staat in `.gitignore` en wordt niet gepubliceerd.

De gewone tests zoeken `arduino-lint` eveneens via `PATH`. Wanneer `arduino-lint` daar niet gevonden wordt en `LINT_PATH` niet via `LokalePaden.cmd` is ingesteld, probeert `TestLibraryCommon.cmd` `arduino-lint` in dezelfde map als `arduino-cli`.

## Schone testconfiguratie

De compiletests definiëren `GROEIACADEMIE_IGNORE_USER_CONFIG`. Een eventueel lokaal `src/Configuratie/UserConfig.h` wordt tijdens de compiletests dus bewust genegeerd. De testconfiguratie bestaat uit de expliciete compilerdefinities van het betrokken testgeval plus de fallbacks van `SystemConfig.h`.

Een `#define` in een `.ino` geldt alleen binnen die sketch-translation-unit en configureert geen apart gecompileerd librarybestand. Instellingen die ook voor librarybronbestanden moeten gelden, worden daarom via `TEST_FLAGS` als compilerdefinitie meegegeven.

## Domeineigenaarschap

`CONTROLE_PADEN` bepaalt welk domein eigenaar is van de voorbeelden en controles die door een testscript worden beheerd.

De zeven gewone domeinscripts gebruiken `TestLibraryCommon.cmd`.

De zeven negatieve domeinscripts gebruiken `TestLibraryCommonOngeldig.cmd` en verwijzen via `BRON_TEST_SCRIPT` naar het gewone testscript van hetzelfde domein.

De testarchitectuur blijft daardoor per domein opgesplitst. Een test hoort in het domein dat door de betrokken `CONTROLE_PADEN` wordt beheerd.

## TEST_TYPE

Een gewoon testblok gebruikt één van deze waarden:

- `NIEUW`
- `REGRESSIE`
- `GERELEASED`

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

Voer de veertien domeinscripts uit:

```text
TestLibraryData-v2.0.0.cmd
TestLibraryDataOngeldig-v2.0.0.cmd
TestLibraryExtenders-v2.0.0.cmd
TestLibraryExtendersOngeldig-v2.0.0.cmd
TestLibraryGedeeldeBus-v2.0.0.cmd
TestLibraryGedeeldeBusOngeldig-v2.0.0.cmd
TestLibraryInput-v2.0.0.cmd
TestLibraryInputOngeldig-v2.0.0.cmd
TestLibraryScreen-v2.0.0.cmd
TestLibraryScreenOngeldig-v2.0.0.cmd
TestLibrarySensoren-v2.0.0.cmd
TestLibrarySensorenOngeldig-v2.0.0.cmd
TestLibraryTools-v2.0.0.cmd
TestLibraryToolsOngeldig-v2.0.0.cmd
```

Voer daarna uit:

```text
TestLibraryGenereerTESTRESULTATEN.cmd
```

## Beoordeling van een gewoon domein

Voor elk van de zeven gewone domeinlogs controleert `TestLibraryGenereerTESTRESULTATEN.cmd`:

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

Voor elk van de zeven Ongeldig-logs moet de eindregel de vorm hebben:

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

De volledige releasevalidatie bestaat daardoor uit veertien domeincycli plus de verplichte mappingcontrole.

## TestLibraryStatusReport.txt

`TestLibraryGenereerTESTRESULTATEN.cmd` maakt `TestLibraryStatusReport.txt`.

Het rapport bevat:

- de totaalstatus van de releasevalidatie;
- de status van alle veertien domeincycli;
- per gewoon domein BASIC + EXTENDED, EXPERIMENTAL_FQBN, statische controles, Arduino LINT en geheugengegevens;
- per Ongeldig-domein het negatieve testtotaal;
- de mappingcontrole;
- foutdetails wanneer de releasevalidatie niet slaagt;
- de volledige inhoud van alle aanwezige domeinlogs.

De libraryversie wordt gelezen uit `library.properties`.

Na een volledig geslaagde releasevalidatie worden de veertien losse `.txt`-domeinlogs verwijderd. `TestLibraryStatusReport.txt` blijft behouden.

Wanneer de releasevalidatie niet slaagt, blijven de losse logs behouden en toont de generator welke scripts opnieuw moeten worden uitgevoerd.

## Status 18-10-2026

De v2.0.0-domeinsplitsing is aanwezig voor Data, Extenders, GedeeldeBus, Input, Screen, Sensoren en Tools.

De Ongeldig-infrastructuur is aangepast zodat de controlebestanden opnieuw worden opgebouwd en versiegebonden `*Ongeldig-v2.0.0.cmd`-bestanden worden meegenomen.

De laatst getoonde Data-run eindigde op:

```text
Totaal 25 - OK 24 - FOUT 1
```

De resterende fout zat in de negatieve microSD-test met CS via `ExtenderPCF8574`: de testconfiguratie activeerde de extender niet volledig en faalde daardoor vóór de bedoelde `SD_CHIP_SELECT_MODE`-validatie. Die `TEST_FLAGS` zijn daarna aangepast aan de bestaande werkende microSD-via-PCF8574-configuratie, zonder `SD_CHIP_SELECT_MODE=1`, zodat de bedoelde negatieve validatie kan worden bereikt.

Die laatste correctie moet nog met een nieuwe Data-run worden bevestigd. De overige zeven Ongeldig-domeinen moeten eveneens met de finale testset opnieuw worden uitgevoerd voordat de volledige releasevalidatie bewezen geslaagd is.

## Definitieve releaseresultaten

De definitieve releaseresultaten worden bijgehouden in `extras/TESTRESULTATEN.md`.
