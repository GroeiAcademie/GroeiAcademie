# Input

## Dependencies

De gecompileerde dependencies hangen af van `INPUT_KANAAL_CONFIG`:

- `INPUT_TYPE_PCF8574`: gebruikt `Wire` en de externe `PCF8574`-library via `ExtenderPCF8574`, dezelfde FrameWork-implementatie die ook door `EXTENDER_PCF8574` wordt gebruikt. `library.properties` vereist `PCF8574 (>=0.4.5)`.
- `INPUT_TYPE_HX1838`: gebruikt TinyIRReceiver of IRremote, afhankelijk van `HX1838_USE_TINYIRRECEIVER_INSTEAD_OF_IRREMOTE`. HX1838 met `HX1838_BRON_CODES_DEFINE` is released en hardwarematig bevestigd voor beide ontvangstbackends. De standaard ontvangerpin is D8. De EEPROM-gebaseerde HX1838-routes blijven experimenteel.
- Niet geselecteerde invoerbackends worden via de preprocessor niet meegecompileerd.

## Lifecycle en runtime-selectie

`Input` is een `GedeeldeBusNode` met `typesActief`. De standaardconstructor gebruikt `INPUT_KANAAL_CONFIG`; de tweede constructor accepteert een runtime-subset.

De compile-time configuratie accepteert in `Input.h` exact:

```text
INPUT_TYPE_NONE
INPUT_TYPE_PCF8574
INPUT_TYPE_HX1838
INPUT_TYPE_PCF8574 | INPUT_TYPE_HX1838
```

Rechtstreeks gelezen digitale pinnen (`INPUT_TYPE_DIGITAL` uit v1.1.2) bestaan in v2.0.0 niet meer: het Stimulus Shield v2.0.0 leest het keypad via de PCF8574, zodat acht native pinnen vrij blijven. Een runtime-subset moet volledig binnen `INPUT_KANAAL_CONFIG` vallen; `Input::aanmelden()` controleert dit.

Normaal aanmaken:

```cpp
Input = GedeeldeBusNewComponent<struct Input>();
if (Input == nullptr) exit(0);
```

Runtime-subset, alleen wanneer die subset compile-time aanwezig is:

```cpp
Input = GedeeldeBusNewComponent<struct Input>(INPUT_TYPE_HX1838);
if (Input == nullptr) exit(0);
```

De concrete GedeeldeBus-children zijn `InputPCF8574` en `InputHX1838`. `aanmelden()`, `controleren()`, `inpluggen()` en `Activeren()` sturen de lifecycle door. Wanneer één child faalt wordt dat child afgemeld; `Input` faalt pas volledig wanneer geen enkel geselecteerd kanaal overblijft (behalve bij `INPUT_TYPE_NONE`). Bij een niet-actieve Input geeft `OpvragenHuidigeToetsAanslag()` geen aanslag terug; fout IN401 wordt hoogstens eenmaal via `Screen` gemeld als er een schermobject beschikbaar is.

De vroegere publieke `InputConfigureren()`-stap bestaat niet meer. `Input::inpluggen()` roept de inplugstap van de PCF8574-child aan en controleert voor HX1838 de ontvangerpin en de IR-initialisatie: TinyIRReceiver moet zijn interruptkoppeling kunnen initialiseren; bij IRremote wordt `begin()` gevolgd door een korte `isIdle()`-stabiliteitscontrole. `Input::Activeren()` activeert de overgebleven GedeeldeBus-kinderen; voor de PCF8574 gebeurt daar de werkelijke I2C-initialisatie via `ExtenderPCF8574::Activeren()` (`PCF8574::begin()`, `isConnected()` en `lastError()`). Een kanaal dat in een van deze stappen faalt wordt afgemeld en op `nullptr` gezet; Input faalt pas als geen enkel geselecteerd kanaal overblijft. Voor HX1838 verwerkt `Input::Activeren()` vervolgens de gekozen mapping- of kalibratiebron.

`Diagnose()` is nog niet specifiek in `Input` geïmplementeerd; alleen de lege DEBUG-hook uit `GedeeldeBusNode` bestaat.

## HX1838-kalibratie


De compile-time controle geeft een gerichte `#warning` wanneer `HX1838_ONTVANGER_PIN` samenvalt met SPI MOSI, MISO of SCK van een actief PixelScreen. De GedeeldeBus controleert daarnaast de native resource die HX1838 bij het inpluggen exclusief gebruikt.
Voor `HX1838_TOETSENINDELING_REMOTE_USER_DEFINED` binnen de released `HX1838_BRON_CODES_DEFINE`-route start automatisch een kalibratie wanneer `HX1838_GENERIEK_CODES` ontbreekt. Iedere te kalibreren toets heeft een wachttijd begrensd door `HX1838_KALIBRATIE_TIMEOUT_MS` (standaard 30000 ms). Bij timeout wordt de kalibratie afgebroken. Na een geslaagde kalibratie wordt de gegenereerde `HX1838_GENERIEK_CODES`-regel aangeboden om in `UserConfig.h` over te nemen. Na die geslaagde UserDefined-kalibratie stopt de code bewust met `while (true) { }`: neem de getoonde codes over en herstart met de ingevulde configuratie.

Voor de experimentele EEPROM-routes start bij ontbrekende of ongeldige EEPROM-kalibratie eveneens automatisch de kalibratieprocedure. De aansluitende verificatiefase gebruikt dezelfde timeout. Bij timeout wordt geen onvolledige mapping opgeslagen. `HX1838_BRON_CODES_EEPROM_ALTIJD` gebruikt EEPROM ongeacht aanwezige code-defines. Bij `HX1838_BRON_CODES_EEPROM_WANNEER_GEEN_DEFINE` wordt de vaste mapping gebruikt zodra minstens één `HX1838_CODE_x` in `UserConfig.h` is gedefinieerd; wanneer geen enkele code is gedefinieerd, wordt EEPROM gebruikt en zo nodig gekalibreerd. Deze EEPROM-gebaseerde routes blijven experimenteel. Voor EEPROM-kalibratie controleert `Input.cpp` tijdens het compileren of `EEPROM_ADRES_CODES + AANTAL_IR_TOETSEN` binnen `HX1838_EEPROM_NATIVE_SIZE` past. Een afzonderlijke gebruikersroute om later bewust opnieuw te kalibreren blijft een toekomstig instellingenwerkpunt.

## Platformafhankelijke opslag

Platformafhankelijke EEPROM-initialisatie en commit horen niet verspreid op losse architectuurmacro's te steunen. De Input-laag gebruikt hiervoor de centrale `BOARD_VERSION`-indeling; ondersteuning voor nieuwe boards wordt per ondersteund board verder ingevuld.


## Configuratie

De Input-laag gebruikt `INPUT_KANAAL_CONFIG` als bitmask. Zonder eigen instelling geldt `INPUT_TYPE_PCF8574`. Wanneer `KEYPAD_TYPE` niet expliciet is ingesteld, wordt `KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4` gebruikt. `KEYPAD_TYPE` kiest het fysieke keypad; het invoerkanaal bepaalt hoe dat keypad wordt gelezen. PCF8574 kan de huidige keypadtypen lezen zolang maximaal acht expanderlijnen nodig zijn; `KEYPAD_TYPE_MEMBRAAN_MATRIX_4x5` (negen lijnen) geeft daarom een compilerfout. Bij een actief CharacterScreen geeft ook een gelijk I2C-adres voor `I2C_ADDRESS_PCF8574` en `I2C_ADDRESS_CHARACTER_SCREEN` een compilerfout.

Het PCF8574-adres voor `INPUT_PCF8574` heet canoniek `I2C_ADDRESS_PCF8574`. `INPUT_PCF8574` blijft als `GedeeldeBusComponent::INPUT_PCF8574` de exclusieve Input-rol en de pinindeling van het shield behouden, maar gebruikt technisch dezelfde `ExtenderPCF8574`-codebasis als een gewone extender. De Input-PCF8574 gebruikt standaard adres `0x20`. Voor een afzonderlijke `EXTENDER_PCF8574_1` is het standaardadres in `SystemConfig.h` juist `I2C_ADDRESS_NONE`, niet automatisch `0x21`. Stel een vrij I2C-adres expliciet in, bijvoorbeeld `0x21` wanneer Input al `0x20` gebruikt, en stel de hardwarejumpers overeenkomstig in. Vaste HX1838-codes kunnen via `HX1838_CODE_1` tot en met `HX1838_CODE_21` in `UserConfig.h` worden ingevuld. De instelling `HX1838_BRON_CODES` in `UserConfig.h` bepaalt expliciet welke codebron voorrang krijgt.

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

`INPUT_KANAAL_CONFIG` bepaalt welke invoerkanalen gecompileerd zijn. `InputKanaal` meldt via welk kanaal een concrete aanslag werkelijk binnenkwam: `PCF8574` of `HX1838`. De globale `Input` is een pointer (`struct Input*`); roep de onderstaande API in een sketch dus aan als `Input->OpvragenHuidigeToetsAanslag(...)`, nadat `Input` succesvol is aangemaakt.

`InputResultaat` bevat altijd `inputKanaal`, `positieToetsAanslag` en `opschriftToetsAanslag`. Met `INPUT_KANAAL_OPVRAGEN_HUIDIGE_TOETSAANSLAG_UITGEBREID` aan bevat het daarnaast `gebeurtenis`. `positieToetsAanslag` is het volgnummer `1..N` binnen de gekozen indeling. `gebeurtenis` is bij een gewone druk `InputGebeurtenis::TOETSAANSLAG` en kan daarnaast `LOSGELATEN`, `LANG_INDRUKKEN` of `TIMEOUT_GEEN_INVOER` zijn.

`Input->OpvragenHuidigeToetsAanslag(true)` bewaart de bestaande Tik-werking: een vorige fysieke aanslag moet eerst los zijn, daarna wordt gewacht op één nieuwe aanslag en bij een fysiek keypad wordt opnieuw op loslaten gewacht vóór het resultaat wordt doorgegeven.

`Input->OpvragenHuidigeToetsAanslag(false)` blokkeert niet. Met de uitgebreide gebeurtenisschakelaar actief loopt een fysiek keypad bij iedere poll door dezelfde debounce-/stabiele-toestandsverwerking; zonder die schakelaar blijft het oorspronkelijke directe non-blocking gedrag behouden. Bij HX1838 wordt een ontvangen IR-gebeurtenis teruggegeven.

De enkelvoudige API `Input->OpvragenHuidigeToetsAanslag()` geeft maximaal één aanslag per aanroep terug.

De publieke meervoudige API is al vastgelegd als `Input->OpvragenHuidigeToetsAanslagen(bool wachten = true, byte aantalSimultaan = 1)`. `MAX_AANTAL_SIMULTANE_TOETSAANSLAGEN` is momenteel `1`. Daardoor is alleen `aantalSimultaan = 1` werkelijk geïmplementeerd en gebruikt die functie intern dezelfde werking als `Input->OpvragenHuidigeToetsAanslag()`. Elke andere waarde retourneert `StatusOpvragenToetsAanslagen::NIET_GEIMPLEMENTEERD`.

Wanneer simultane aanslagen later worden uitgewerkt, blijft hetzelfde publieke prototype behouden. `aantalSimultaan = 2` zal dan exact twee gelijktijdige aanslagen betekenen: minder dan twee is `TE_WEINIG`, exact twee is `GELDIG` en meer dan twee is `TE_VEEL`.

`Input->OpzoekenUitTeVoerenFunctieViaOpschriftToetsAanslag()` zoekt het ontvangen opschrift in een mapping en retourneert een pointer naar de volledige koppeling met de functiepointer. De eenvoudige overload gebruikt de vaste `mappingTussenToetsaanslagEnUitTeVoerenFunctie`; de overload met een mapping-array zoekt uitsluitend in de expliciet doorgegeven mapping.

`Input->UitVoerenFunctieVolgensMappingMetToetsAanslag(bool wachten = true)` leest een toetsaanslag, zoekt die in de vaste mapping en voert de gekoppelde functie uit. Voor meerdere menu's kan dezelfde functienaam gebruikt worden met een expliciete mapping-array, bijvoorbeeld `Input->UitVoerenFunctieVolgensMappingMetToetsAanslag(true, mappingInstellingenMenu)`. Het aantal elementen hoeft niet afzonderlijk te worden doorgegeven: dit wordt compile-time uit de array afgeleid. `InputkanalenPCF8574.ino` en `InputkanalenHX1838.ino` testen deze volledige keten, elk met parameterloze functies (`InputFunctie`/`MappingTussenToetsaanslagEnUitTeVoerenFunctie`) die hun eigen opschrift via `Screen->Print()` tonen.

Daarnaast bestaat een volledig apart, tweede type, `InputFunctieMetArgumenten`/`MappingTussenToetsaanslagEnUitTeVoerenFunctieMetArgumenten`, met een `argumenten`-veld (`void*`) dat aan de functie wordt doorgegeven. Dit tweede type raakt het eerste op geen enkele manier: bestaande mappings met parameterloze functies blijven ongewijzigd werken. `InputkanalenPCF8574metArgumenten.ino` en `InputkanalenHX1838metArgumenten.ino` demonstreren dit tweede type, inclusief gedeelde toestand tussen meerdere functies via hetzelfde `argumenten`-adres.

## Uitgebreide gebeurtenissen (loslaten, lang indrukken, timeout)

Standaard uitgeschakeld, om geen extra geheugen te verbruiken op geheugenarme boards. Inschakelen via `#define INPUT_KANAAL_OPVRAGEN_HUIDIGE_TOETSAANSLAG_UITGEBREID` in `UserConfig.h`. Enkel van toepassing bij `Input->OpvragenHuidigeToetsAanslag(false)`; bij `wachten=true` blokkeert de bestaande code toch al tot loslaten, dus daar valt niets nieuws te detecteren.

- **Loslaten**: geeft de positie terug die net losgelaten werd, als apart `InputResultaat` met `gebeurtenis = LOSGELATEN`. Wanneer voor die toets `functieBijLoslaten` is ingesteld, voert `Input->UitVoerenFunctieVolgensMappingMetToetsAanslag()` die functie uit.
- **Lang indrukken**: per toets optioneel in te stellen via `functieBijLangIndrukken` en `langIndrukkenDrempelMS` in `MappingTussenToetsaanslagEnUitTeVoerenFunctie`. Wanneer `Input->OpvragenHuidigeToetsAanslag(false)` een `LANG_INDRUKKEN`-gebeurtenis teruggeeft, voert `Input->UitVoerenFunctieVolgensMappingMetToetsAanslag()` de gekoppelde `functieBijLangIndrukken` uit. De gewone `functie` blijft gekoppeld aan `TOETSAANSLAG`. Bij een expliciet meegegeven mapping wordt `langIndrukkenDrempelMS` uit die werkelijk gebruikte mapping opgezocht.
- **Timeout bij geen invoer**: `INPUT_KANAAL_TIMEOUT_BIJ_GEEN_TOETSAANSLAG_BINNEN_MS`, standaard `0` (geen timeout, huidig gedrag). Bij een waarde groter dan 0 wordt de timer tijdens `Input::Activeren()` geïnitialiseerd en geeft `Input->OpvragenHuidigeToetsAanslag(false)` na die periode een `InputResultaat` met `gebeurtenis = TIMEOUT_GEEN_INVOER` terug. Ook de huidige enkelvoudig geïmplementeerde `Input->OpvragenHuidigeToetsAanslagen(..., 1)` geeft dit timeoutresultaat door.

`INPUT_KANAAL_OPVRAGEN_HUIDIGE_TOETSAANSLAG_UITGEBREID` is de configuratieschakelaar voor de optionele uitgebreide gebeurtenissen.

## Nieuwe boards in acceptatiefase

Nieuwe boards mogen al meegecompileerd en getest worden vóór ze officieel ondersteund zijn. Hun resultaten worden afzonderlijk gerapporteerd en hebben tijdens de acceptatiefase geen invloed op PASS/FAIL van het officieel ondersteunde pakket.


## Ondersteunde keypadtypen

De fysieke keypadkeuze gebeurt met `KEYPAD_TYPE`.

PCF8574 ondersteunt de gedefinieerde keypadtypen die maximaal acht expanderlijnen nodig hebben, waaronder de directe 1x4/2x4-varianten en de ondersteunde 2x2-, 4x3- en 4x4-matrixvarianten. `KEYPAD_TYPE_MEMBRAAN_MATRIX_4x5` is bewust geweigerd omdat negen signaallijnen nodig zijn. `KEYPAD_TYPE_MEMBRAAN_MATRIX_1x4` heeft in `Input.cpp` nog een veronderstelde fysieke indeling; de concrete referentiehardware is daar niet bevestigd. `KEYPAD_TYPE_TOUCH_TTP229_MATRIX_4x4` gebruikt de acht parallelle uitgangen via PCF8574 P0-P7; daarmee kunnen de eerste acht toetsen uitgelezen worden. Het actieve uitgangsniveau is configureerbaar met `TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED_HIGH` of `TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED_LOW`; standaard is `TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED_HIGH`. De toekomstige `KEYPAD_TYPE_TOUCH_TTP229_SERIAL_4x4` via SCL/SDO is een aparte, nog niet geïmplementeerde 16-toetsenvariant.

### Referentiebeelden keypadtypen

| Keypadtype | Referentiebeeld |
|---|---|
| `KEYPAD_TYPE_DRUKKNOP_DIRECT_1x4` | [KEYPAD_TYPE_DRUKKNOP_DIRECT_1x4.png](Screenshots/KEYPAD_TYPE_DRUKKNOP_DIRECT_1x4.png) |
| `KEYPAD_TYPE_DRUKKNOP_DIRECT_2x4` | [KEYPAD_TYPE_DRUKKNOP_DIRECT_2x4.png](Screenshots/KEYPAD_TYPE_DRUKKNOP_DIRECT_2x4.png) |
| `KEYPAD_TYPE_DRUKKNOP_MATRIX_2x2` | [KEYPAD_TYPE_DRUKKNOP_MATRIX_2x2.png](Screenshots/KEYPAD_TYPE_DRUKKNOP_MATRIX_2x2.png) |
| `KEYPAD_TYPE_DRUKKNOP_MATRIX_4x4` | [KEYPAD_TYPE_DRUKKNOP_MATRIX_4x4.JPG](Screenshots/KEYPAD_TYPE_DRUKKNOP_MATRIX_4x4.JPG) |
| `KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4` | [KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4.png](Screenshots/KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4.png) |
| `KEYPAD_TYPE_MEMBRAAN_DIRECT_4x1` | [KEYPAD_TYPE_MEMBRAAN_DIRECT_4x1.jpg](Screenshots/KEYPAD_TYPE_MEMBRAAN_DIRECT_4x1.jpg) |
| `KEYPAD_TYPE_MEMBRAAN_MATRIX_1x4` | [KEYPAD_TYPE_MEMBRAAN_MATRIX_1x4.jpeg](Screenshots/KEYPAD_TYPE_MEMBRAAN_MATRIX_1x4.jpeg) |
| `KEYPAD_TYPE_MEMBRAAN_MATRIX_2x4` | geen afzonderlijk referentiebeeld beschikbaar |
| `KEYPAD_TYPE_MEMBRAAN_MATRIX_4x3` | [KEYPAD_TYPE_MEMBRAAN_MATRIX_4x3.jpeg](Screenshots/KEYPAD_TYPE_MEMBRAAN_MATRIX_4x3.jpeg) |
| `KEYPAD_TYPE_MEMBRAAN_MATRIX_4x4` | [KEYPAD_TYPE_MEMBRAAN_MATRIX_4x4.jpeg](Screenshots/KEYPAD_TYPE_MEMBRAAN_MATRIX_4x4.jpeg) |
| `KEYPAD_TYPE_MEMBRAAN_MATRIX_4x5` | [KEYPAD_TYPE_MEMBRAAN_MATRIX_4x5.jpeg](Screenshots/KEYPAD_TYPE_MEMBRAAN_MATRIX_4x5.jpeg) |
| `KEYPAD_TYPE_TOUCH_TTP224_DIRECT_1x4` | [KEYPAD_TYPE_TOUCH_TTP224_DIRECT_1x4.png](Screenshots/KEYPAD_TYPE_TOUCH_TTP224_DIRECT_1x4.png) |
| `KEYPAD_TYPE_TOUCH_TTP229_MATRIX_4x4` | [KEYPAD_TYPE_TOUCH_TTP229_MATRIX_4x4.JPG](Screenshots/KEYPAD_TYPE_TOUCH_TTP229_MATRIX_4x4.JPG) |

## Input-testvoorbeelden

Onder `examples/Systeem/Input/` staan `InputkanalenPCF8574.ino`, `InputkanalenHX1838.ino`, hun `metArgumenten`-tegenhangers (`InputkanalenPCF8574metArgumenten.ino`, `InputkanalenHX1838metArgumenten.ino`), `InputkanalenHX1838UserDefined.ino`, `InputkanalenPCF8574UserDefinedDirect.ino` en `InputkanalenPCF8574UserDefinedMatrix.ino`. De vijf actuele Stimulus-scenario's staan uitsluitend onder `examples/Toepassingsgebieden/Stimulus/` en gebruiken de Input-laag.


Beschikbare HX1838-toetsenindelingen:

- `HX1838_TOETSENINDELING_REMOTE_OK_BOVENAAN_17_TOETSEN`: remote met 17 toetsen: (UP, DOWN, OK, LEFT, RIGHT, 1-9, *, 0, #) [UP, DOWN, OK, LEFT, RIGHT, 1, 2, 3, 4, 5, 6, 7, 8, 9, *, 0, #]: [HX1838_TOETSENINDELING_REMOTE_17_TOETSEN_OK_BOVENAAN.png](Screenshots/HX1838_TOETSENINDELING_REMOTE_17_TOETSEN_OK_BOVENAAN.png).
- `HX1838_TOETSENINDELING_REMOTE_OK_ONDERAAN_17_TOETSEN`: remote met 17 toetsen: (1-9, *, 0, #, UP, DOWN, OK, LEFT, RIGHT) [1, 2, 3, 4, 5, 6, 7, 8, 9, *, 0, #, UP, DOWN, OK, LEFT, RIGHT]: [HX1838_TOETSENINDELING_REMOTE_17_TOETSEN_OK_ONDERAAN.png](Screenshots/HX1838_TOETSENINDELING_REMOTE_17_TOETSEN_OK_ONDERAAN.png);
- `HX1838_TOETSENINDELING_REMOTE_21_TOETSEN_MP3`        : remote met 21 toetsen, inclusief de MP3-toetsen; referentiebeeld: [HX1838_TOETSENINDELING_REMOTE_21_TOETSEN_MP3.jpeg](Screenshots/HX1838_TOETSENINDELING_REMOTE_21_TOETSEN_MP3.jpeg).
- `HX1838_TOETSENINDELING_REMOTE_USER_DEFINED`          : gebruiker bepaalt zelf het aantal toetsen, de 8-bit commandcodes en de opschrift/weergavetekstkoppeling. Deze indeling is uitsluitend beschikbaar met `HX1838_BRON_CODES_DEFINE`.

Voor `HX1838_TOETSENINDELING_REMOTE_USER_DEFINED` moeten in `UserConfig.h` `HX1838_GENERIEK_AANTAL_TOETSEN` en `HX1838_GENERIEK_KEY_LAYOUT` ingesteld worden. `HX1838_GENERIEK_CODES` is optioneel: wanneer deze define ontbreekt, start automatisch de UserDefined-kalibratie; wanneer ze wel is ingesteld, moet het aantal codes exact overeenkomen met `HX1838_GENERIEK_AANTAL_TOETSEN` en worden nulcodes geweigerd. Het aantal layoutentries moet altijd exact overeenkomen met `HX1838_GENERIEK_AANTAL_TOETSEN`. De huidige implementatie vergelijkt de door TinyIRReceiver/IRremote gedecodeerde 8-bit `command`-waarde (`uint8_t`), zodat UserDefined niet claimt ieder mogelijk IR-protocol of iedere volledige protocol/address/command-combinatie te ondersteunen. Zie ook `InputkanalenHX1838UserDefined.ino`.

`HX1838_USE_TINYIRRECEIVER_INSTEAD_OF_IRREMOTE` kiest de HX1838-ontvangstbackend: `1` gebruikt TinyIRReceiver en `0` gebruikt IRremote. De standaard `HX1838_ONTVANGER_PIN` is D8. Met `HX1838_BRON_CODES = HX1838_BRON_CODES_DEFINE` zijn beide ontvangstbackends hardwarematig getest en werkend bevonden; deze DEFINE-route is released. De EEPROM-gebaseerde HX1838-routes blijven experimenteel.

Voor de vaste mapping gebruikt `HX1838_TOETSENINDELING_REMOTE_OK_BOVENAAN_17_TOETSEN` `HX1838_CODE_1` t.e.m. `HX1838_CODE_17`, `HX1838_TOETSENINDELING_REMOTE_OK_ONDERAAN_17_TOETSEN` `HX1838_CODE_1` t.e.m. `HX1838_CODE_17` en `HX1838_TOETSENINDELING_REMOTE_21_TOETSEN_MP3` `HX1838_CODE_1` t.e.m. `HX1838_CODE_21`. Niet zelf gedefinieerde codes worden door `SystemConfig.h` met de standaardcodes aangevuld. Een expliciet gedefinieerde code met waarde `0` is ongeldig wanneer de vaste mapping wordt gebruikt.

## Mapping-volledigheidscontrole (ControleerMappingVolledigheid)

Een mapping (`MappingTussenToetsaanslagEnUitTeVoerenFunctie[]`, zoals `mappingTestMenu[]` in de Input-voorbeelden) die niet elk opschrift bevat dat het gecompileerde `KEYPAD_TYPE`/`HX1838_TOETSENINDELING` kan opleveren, geeft geen compilatiefout en geen waarschuwing. Bij een niet-gevonden opschrift gebeurt er gewoon stilzwijgend niets bij die toetsdruk.

`Input->ControleerMappingVolledigheid(mapping)` controleert dit tijdens het draaien. Ze doorloopt `KEY_LAYOUT[]`/`IR_KEY_LAYOUT[]` van het actief gecompileerde type en meldt ontbrekende opschriften via `Screen->Print()`. Daardoor volgt de controle dezelfde geselecteerde uitvoerdoelen als de rest van de Screen-laag. Seriële standaarduitvoer via `Screen->Print()` volgt daarbij de actieve Serial-configuratie van de Screen-laag.

De controle is enkel beschikbaar wanneer `INPUT_MAPPING_EXTRA_CONTROLES_INSCHAKELEN` in `UserConfig.h` gedefinieerd is. Zonder die define bestaat de functie nergens in het gecompileerde programma. Er is dan geen extra flashgebruik en geen extra `Serial`-afhankelijkheid. De functie is bedoeld om tijdens het testen op te roepen, bijvoorbeeld eenmalig in `setup()` nadat `Input` succesvol via `GedeeldeBusNewComponent<struct Input>(...)` is aangemaakt, en niet om in productiecode te laten staan.

De releasevalidatie bevat daarnaast een tekstuele volledigheidscontrole die geen upload of aangesloten board vereist. De gedeelde Windows-testscripts staan onder `extras/` en worden mee gepubliceerd. Alleen `extras/LokalePaden.cmd` is machinespecifiek en blijft via `.gitignore` lokaal; `extras/LokalePaden_template.cmd` wordt wel meegeleverd.

## Generiek keypad (experimenteel)

`KEYPAD_TYPE_USER_DEFINED_DIRECT` en `KEYPAD_TYPE_USER_DEFINED_MATRIX` laten toe om een nieuw fysiek keypad te testen zonder de bibliotheek zelf aan te passen. Alle vereiste `KEYPAD_GENERIEK_...`-instellingen worden in normaal gebruik volledig in `UserConfig.h` vastgelegd; definities in een `.ino` kunnen het apart gecompileerde `Input.cpp` niet configureren. Zie `UserConfig_template.h` en de twee UserDefined-voorbeelden voor een volledige configuratie.

Enkel beschikbaar bij `INPUT_TYPE_PCF8574`. `KEYPAD_GENERIEK_PINNEN`/`_RIJ_PINNEN`/`_KOLOM_PINNEN` zijn bitposities op de PCF8574 (0 t.e.m. 7), geen Arduino-pinnummers. De build controleert voor UserDefined-keypads de vereiste instellingen, het verwachte aantal layoutentries, het pinbereik, dubbele pinnen en bij matrices ook overlap tussen rij- en kolompinnen.

Bij `KEYPAD_TYPE_USER_DEFINED_DIRECT` bepaalt `KEYPAD_GENERIEK_OUTPUT_LEVEL_WHEN_KEY_PRESSED` of een toets active-low of active-high wordt gelezen. `KEYPAD_TYPE_USER_DEFINED_MATRIX` gebruikt de vaste matrixscan van `Input.cpp` en is **altijd active-low**: de geselecteerde rij wordt laag gemaakt en een ingedrukte toets wordt als lage kolomingang gedetecteerd. Een generieke matrix moet dus active-low worden bekabeld.

`examples/Systeem/Input/InputkanalenPCF8574UserDefinedDirect/InputkanalenPCF8574UserDefinedDirect.ino` en `examples/Systeem/Input/InputkanalenPCF8574UserDefinedMatrix/InputkanalenPCF8574UserDefinedMatrix.ino` bevatten de vereiste configuratie als commentaarvoorbeeld, maar definiëren die waarden niet zelf. De expliciete `mappingTestMenu` wordt aangeroepen met `Input->UitVoerenFunctieVolgensMappingMetToetsAanslag(true, mappingTestMenu)`.

Dit blijft bewust een experimenteel pad: eens een configuratie bewezen werkt, kan ze overgezet worden naar een echt, benoemd `KEYPAD_TYPE_...` in `InputTypes.h`, met documentatie en screenshot zoals de bestaande, officieel ondersteunde types.

## Zonder scherm

- De foutmeldingen van Input tonen alleen iets op het scherm als er een scherm is (`Screen != nullptr`): IN402, IN202, IN302, IN303, IN304, IN305 en de meldingen bij het laden van de HX1838-toetsentabel.
- Met `SCREEN_OUTPUT_CONFIG` op `SCREEN_TYPE_NONE` loopt het board bij die fouten dus niet meer vast.
- De HX1838-kalibratie kan zonder scherm doorlopen: prompts worden, wanneer `SCREEN_OUTPUT` seriële uitvoer omvat, via `GA_SERIAL` getoond. Zonder scherm én zonder seriële uitvoer ontbreken bruikbare kalibratieaanwijzingen. De mappingcontrole draait eveneens zonder `Screen`, maar toont dan geen meldingen via `Screen->Print()`.

## Aanmelden, kalibratie en keypad

- **Geen enkel kanaal aangemeld:** `Input::aanmelden()` meldt de Input dan weer af, zonder het object te verwijderen. Een vaste Input kan later opnieuw proberen; een dynamische Input ruimt `GedeeldeBusNewComponent()` zelf op.
- **Gedeeltelijke aanmelding mag:** lukt één kanaal en het andere niet (bv. de HX1838 werkt, de PCF8574 antwoordt niet), dan werkt Input met het kanaal dat lukte. Het ontbrekende kanaal komt pas terug na een herstart.
- **HX1838-kalibratie uit de EEPROM:** een code 0 of een code die twee keer voorkomt, maakt de bewaarde kalibratie ongeldig. Dan volgt een nieuwe kalibratie.
- **PCF8574-keypad, leesfout:** een mislukte lezing geeft "onbekend" (-1), niet "geen toets". De vorige toestand blijft, zodat een leesfout geen losgelaten toets en daarna een dubbele toets wordt. Tijdens een lange uitval blijft de laatste toestand staan tot de PCF8574 terug is.
- **Zonder scherm:** ook de optionele mappingcontrole en de HX1838-kalibratie controleren nu of er een scherm is. Zonder scherm lopen ze niet meer vast; de aanwijzingen van de kalibratie zie je dan alleen op de seriële monitor, waar die er zijn.

## Wachten op het loslaten bij een leesfout

- Een leesfout van het PCF8574-keypad bevestigt geen loslaten. De toets blijft als ingedrukt gelden, zodat dezelfde toets na het herstel niet opnieuw als nieuwe aanslag binnenkomt.
- Blijft het keypad onleesbaar, dan stopt het wachten op het loslaten na `INPUT_KEYPAD_READ_ERROR_TIMEOUT_MS` (standaard 1000 ms; instelbaar in UserConfig.h).
- Tijdens die wachttijd wordt de HX1838 niet gelezen. Een HX1838-toets kan dus tot ongeveer die tijd vertraagd worden. Of een toets in die tijd bewaard blijft, hangt af van de IR-library; dat bewijst alleen een hardwaretest.
- Wie de toets ingedrukt houdt, laat het wachten duren zolang de toets gelezen wordt, zoals voorheen.
