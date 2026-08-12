# Input

## Dependencies

De gecompileerde dependencies hangen af van `INPUT_KANAAL_CONFIG`:

- `INPUT_TYPE_DIGITAL`: geen extra externe library.
- `INPUT_TYPE_PCF8574`: gebruikt `Wire` en de `PCF8574`-library van Rob Tillaart.
- `INPUT_TYPE_HX1838`: gebruikt `IRremote` en `EEPROM`.
- Niet geselecteerde invoerbackends worden via de preprocessor niet meegecompileerd.

## HX1838-kalibratie

De huidige testimplementatie start bij ontbrekende of ongeldige EEPROM-kalibratie automatisch de kalibratieprocedure. Voor de definitieve `System/Input`-laag blijft als werkpunt staan dat kalibratie niet onnodig automatisch mag worden afgedwongen en dat de gewenste expliciete herkalibratieroute nog wordt vastgelegd.
`HX1838_BRON_CODES` bepaalt expliciet welke bron gebruikt wordt. `HX1838_BRON_CODES_DEFINE` vereist een volledige vaste mapping; `HX1838_BRON_CODES_EEPROM_ALTIJD` gebruikt EEPROM en kalibreert wanneer geen geldige EEPROM-mapping aanwezig is; `HX1838_BRON_CODES_EEPROM_WANNEER_GEEN_DEFINE` gebruikt een volledige vaste mapping wanneer die beschikbaar is en valt anders terug op EEPROM en zo nodig kalibratie. Een afzonderlijke gebruikersroute om later bewust opnieuw te kalibreren blijft een toekomstig instellingenwerkpunt.

## Platformafhankelijke opslag

Platformafhankelijke EEPROM-initialisatie en commit horen niet verspreid op losse architectuurmacro's te steunen. De Input-laag gebruikt hiervoor de centrale `BOARD_VERSION`-indeling; ondersteuning voor nieuwe boards wordt per ondersteund board verder ingevuld.


## Configuratie

De Input-laag gebruikt `INPUT_KANAAL_CONFIG` als bitmask. Het huidige elektronische schema valt standaard terug op `INPUT_TYPE_DIGITAL`. Wanneer `KEYPAD_TYPE` niet expliciet is ingesteld, wordt `KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4` gebruikt. `KEYPAD_TYPE` kiest het fysieke keypad; de gekozen backend bepaalt hoe dat keypad wordt gelezen. DIGITAL wordt in deze fase beperkt tot keypads met maximaal vier digitale signaallijnen. PCF8574 kan de huidige keypadtypen lezen zolang maximaal acht expanderlijnen nodig zijn.

Het PCF8574-adres heet canoniek `I2C_ADDRESS_PCF8574`. Vaste HX1838-codes kunnen via `HX1838_CODE_1` tot en met `HX1838_CODE_21` in `UserConfig.h` worden ingevuld. De instelling `HX1838_BRON_CODES` in `UserConfig.h` bepaalt expliciet welke codebron voorrang krijgt.


## Referentiehardware PCF8574

De referentie-/testmodule voor de I2C-naar-IO-uitbreiding is de OTRONIC OT8980. Afhankelijk van de gemonteerde chip kan dit een PCF8574(T) of PCF8574A(T) zijn. Het adresbereik is respectievelijk `0x20..0x27` of `0x38..0x3F`. De code en publieke API blijven chipgeoriënteerd en gebruiken de naam `PCF8574`.

**Jumperinstelling A0/A1/A2 → I2C-adres.** Zoals geleverd staan alle drie de jumpers op de "-"-kant (GND/laag), wat het standaardadres `0x20` (PCF8574) of `0x38` (PCF8574A) geeft. Elke jumper op de "+"-kant (VCC/hoog) telt op: A0 = +1, A1 = +2, A2 = +4.

| A2 | A1 | A0 | PCF8574 | PCF8574A |
|---|---|---|---|---|
| - | - | - | 0x20 (standaard) | 0x38 (standaard) |
| - | - | + | 0x21 | 0x39 |
| - | + | - | 0x22 | 0x3A |
| - | + | + | 0x23 | 0x3B |
| + | - | - | 0x24 | 0x3C |
| + | - | + | 0x25 | 0x3D |
| + | + | - | 0x26 | 0x3E |
| + | + | + | 0x27 | 0x3F |

## Tik-compatibele Input-API

`INPUT_KANAAL_CONFIG` bepaalt welke invoerkanalen gecompileerd zijn. `InputKanaal` meldt via welk kanaal een concrete aanslag werkelijk binnenkwam: `DIGITAL`, `PCF8574` of `HX1838`.

`InputResultaat` bevat altijd `inputKanaal`, `positieToetsAanslag` en `opschriftToetsAanslag`. Met `INPUT_KANAAL_OPVRAGEN_HUIDIGE_TOETSAANSLAG_UITGEBREID` aan bevat het daarnaast `gebeurtenis`. `positieToetsAanslag` is het volgnummer `1..N` binnen de gekozen indeling. `gebeurtenis` is bij een gewone druk `InputGebeurtenis::TOETSAANSLAG` en kan daarnaast `LOSGELATEN`, `LANG_INDRUKKEN` of `TIMEOUT_GEEN_INVOER` zijn.

`OpvragenHuidigeToetsAanslag(true)` bewaart de bestaande Tik-werking: een vorige fysieke aanslag moet eerst los zijn, daarna wordt gewacht op één nieuwe aanslag en bij een fysiek keypad wordt opnieuw op loslaten gewacht vóór het resultaat wordt doorgegeven.

`OpvragenHuidigeToetsAanslag(false)` blokkeert niet. Bij een fysiek keypad wordt de huidige eerste gevonden positie teruggegeven; bij HX1838 wordt een ontvangen IR-gebeurtenis teruggegeven.

De enkelvoudige API `OpvragenHuidigeToetsAanslag()` geeft maximaal één aanslag per aanroep terug.

De publieke meervoudige API is al vastgelegd als `OpvragenHuidigeToetsAanslagen(bool wachten = true, byte aantalSimultaan = 1)`. In v1.1.0 is `MAX_AANTAL_SIMULTANE_TOETSAANSLAGEN` nog `1`. Daardoor is alleen `aantalSimultaan = 1` werkelijk geïmplementeerd en gebruikt die functie intern dezelfde werking als `OpvragenHuidigeToetsAanslag()`. Elke andere waarde retourneert `StatusOpvragenToetsAanslagen::NIET_GEIMPLEMENTEERD`.

Wanneer simultane aanslagen later worden uitgewerkt, blijft hetzelfde publieke prototype behouden. `aantalSimultaan = 2` zal dan exact twee gelijktijdige aanslagen betekenen: minder dan twee is `TE_WEINIG`, exact twee is `GELDIG` en meer dan twee is `TE_VEEL`.

`OpzoekenUitTeVoerenFunctieViaOpschriftToetsAanslag()` zoekt het ontvangen opschrift in een mapping en retourneert een pointer naar de volledige koppeling met de functiepointer. De eenvoudige overload gebruikt de vaste `mappingTussenToetsaanslagEnUitTeVoerenFunctie`; de overload met een mapping-array zoekt uitsluitend in de expliciet doorgegeven mapping.

`ToonMenuEnUitVoerenFunctieVolgensMappingMetToetsAanslag(bool wachten = true)` leest een toetsaanslag, zoekt die in de vaste mapping en voert de gekoppelde functie uit. Voor meerdere menu's kan dezelfde functienaam gebruikt worden met een expliciete mapping-array, bijvoorbeeld `ToonMenuEnUitVoerenFunctieVolgensMappingMetToetsAanslag(true, mappingInstellingenMenu)`. Het aantal elementen hoeft niet afzonderlijk te worden doorgegeven: dit wordt compile-time uit de array afgeleid. `Inputkanalen.ino` test deze volledige keten met vier dummyfuncties die hun eigen functienaam via `PrintToScreen()` tonen.

## Uitgebreide gebeurtenissen (loslaten, lang indrukken, timeout)

Standaard uitgeschakeld, om geen extra geheugen te verbruiken op geheugenarme boards. Inschakelen via `#define INPUT_KANAAL_OPVRAGEN_HUIDIGE_TOETSAANSLAG_UITGEBREID` in `UserConfig.h`. Enkel van toepassing bij `OpvragenHuidigeToetsAanslag(false)`; bij `wachten=true` blokkeert de bestaande code toch al tot loslaten, dus daar valt niets nieuws te detecteren.

- **Loslaten**: geeft de positie terug die net losgelaten werd, als apart `InputResultaat` met `gebeurtenis = LOSGELATEN`. Wanneer voor die toets `functieBijLoslaten` is ingesteld, voert `ToonMenuEnUitVoerenFunctieVolgensMappingMetToetsAanslag()` die functie uit.
- **Lang indrukken**: per toets optioneel in te stellen via `functieBijLangIndrukken` en `langIndrukkenDrempelMs` in `MappingTussenToetsaanslagEnUitTeVoerenFunctie`. Wanneer `OpvragenHuidigeToetsAanslag(false)` een `LANG_INDRUKKEN`-gebeurtenis teruggeeft, voert `ToonMenuEnUitVoerenFunctieVolgensMappingMetToetsAanslag()` de gekoppelde `functieBijLangIndrukken` uit. De gewone `functie` blijft gekoppeld aan `TOETSAANSLAG`.
- **Timeout bij geen invoer**: `INPUT_KANAAL_TIMEOUT_BIJ_GEEN_TOETSAANSLAG_BINNEN_MS`, standaard `0` (geen timeout, huidig gedrag). Bij een waarde groter dan 0 geeft `OpvragenHuidigeToetsAanslag(false)` een `InputResultaat` met `gebeurtenis = TIMEOUT_GEEN_INVOER` terug wanneer gedurende die tijd geen enkel kanaal iets gemeld heeft.

`INPUT_KANAAL_OPVRAGEN_HUIDIGE_TOETSAANSLAG_UITGEBREID` is de v1.1.0-configuratieschakelaar voor de optionele uitgebreide gebeurtenissen.

## Nieuwe boards in acceptatiefase

Nieuwe boards mogen al meegecompileerd en getest worden vóór ze officieel ondersteund zijn. Hun resultaten worden afzonderlijk gerapporteerd en hebben tijdens de acceptatiefase geen invloed op PASS/FAIL van het officieel ondersteunde pakket.


## Ondersteunde keypadtypen in v1.1.0

De fysieke keypadkeuze gebeurt met `KEYPAD_TYPE`.

DIGITAL ondersteunt in deze fase uitsluitend keypadtypen die maximaal vier digitale signaallijnen nodig hebben:

- `KEYPAD_TYPE_DRUKKNOP_DIRECT_1x4`;
- `KEYPAD_TYPE_DRUKKNOP_MATRIX_2x2`;
- `KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4`;
- `KEYPAD_TYPE_MEMBRAAN_DIRECT_4x1`;
- `KEYPAD_TYPE_TOUCH_TTP224_DIRECT_1x4`.

PCF8574 ondersteunt de gedefinieerde keypadtypen die maximaal acht expanderlijnen nodig hebben, waaronder de directe 1x4/2x4-varianten en de ondersteunde 2x2-, 4x3- en 4x4-matrixvarianten. `KEYPAD_TYPE_MEMBRAAN_MATRIX_4x5` is bewust geweigerd omdat negen signaallijnen nodig zijn. `KEYPAD_TYPE_TOUCH_TTP229_MATRIX_4x4` gebruikt de acht parallelle uitgangen via PCF8574 P0-P7; daarmee kunnen de eerste acht toetsen uitgelezen worden. Het actieve uitgangsniveau is configureerbaar met `TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED_HIGH` of `TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED_LOW`; standaard is `TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED_HIGH`. De toekomstige `KEYPAD_TYPE_TOUCH_TTP229_SERIAL_4x4` via SCL/SDO is een aparte, nog niet geïmplementeerde 16-toetsenvariant.

## Input-testvoorbeelden

Onder `examples/Systeem/Input/` staan `Inputkanalen.ino` en de Input-testkopieën van de Stimulus-scenario's. De originele Stimulus-voorbeelden blijven onaangeroerd. De testkopieën zijn bedoeld om te bewijzen dat dezelfde keuzevolgorde via `InputConfigureren()` en `OpvragenHuidigeToetsAanslag(true)` kan verlopen.


Beschikbare HX1838-toetsenindelingen:

- `HX1838_TOETSENINDELING_3x4`: remote met 17 toetsen; enkel de 12 toetsen van de 3x4-matrix (1-9, *, 0, #) worden gebruikt;
- `HX1838_TOETSENINDELING_REMOTE_17_TOETSEN`: alle 17 toetsen, inclusief navigatie (UP, DOWN, OK, LEFT, RIGHT);
- `HX1838_TOETSENINDELING_REMOTE_21_TOETSEN_MP3`: remote met 21 toetsen, inclusief de MP3-toetsen.

Bij `HX1838_BRON_CODES_DEFINE` moet de vaste mapping volledig zijn voor de gekozen indeling: `HX1838_CODE_1` t.e.m. `HX1838_CODE_12` voor `HX1838_TOETSENINDELING_3x4`, `HX1838_CODE_1` t.e.m. `HX1838_CODE_17` voor `HX1838_TOETSENINDELING_REMOTE_17_TOETSEN` en `HX1838_CODE_1` t.e.m. `HX1838_CODE_21` voor `HX1838_TOETSENINDELING_REMOTE_21_TOETSEN_MP3`.
