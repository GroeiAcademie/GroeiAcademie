# Input

## Dependencies

De gecompileerde dependencies hangen af van `INPUT_KANAAL_CONFIG`:

- `INPUT_TYPE_DIGITAL`: geen extra externe library.
- `INPUT_TYPE_PCF8574`: gebruikt `Wire` en de `PCF8574`-library van Rob Tillaart.
- `INPUT_TYPE_HX1838`: gebruikt TinyIRReceiver of IRremote, afhankelijk van `HX1838_USE_TINYIRRECEIVER_INSTEAD_OF_IRREMOTE`. In v1.1.1 is HX1838 met `HX1838_BRON_CODES_DEFINE` released; dit is op de geteste hardwareopstelling met ontvanger op D12 werkend bevestigd voor beide ontvangstbackends. De EEPROM-gebaseerde HX1838-routes blijven experimenteel.
- Niet geselecteerde invoerbackends worden via de preprocessor niet meegecompileerd.

## HX1838-kalibratie

Voor `HX1838_TOETSENINDELING_REMOTE_USER_DEFINED` binnen de released `HX1838_BRON_CODES_DEFINE`-route start automatisch een kalibratie wanneer `HX1838_GENERIEK_CODES` ontbreekt. Iedere te kalibreren toets heeft een wachttijd begrensd door `HX1838_KALIBRATIE_TIMEOUT_MS` (standaard 30000 ms). Bij timeout wordt de kalibratie afgebroken. Na een geslaagde kalibratie wordt de gegenereerde `HX1838_GENERIEK_CODES`-regel aangeboden om in `UserConfig.h` over te nemen.

Voor de experimentele EEPROM-routes start bij ontbrekende of ongeldige EEPROM-kalibratie eveneens automatisch de kalibratieprocedure. De aansluitende verificatiefase gebruikt dezelfde timeout. Bij timeout wordt geen onvolledige mapping opgeslagen. `HX1838_BRON_CODES_EEPROM_ALTIJD` gebruikt EEPROM ongeacht aanwezige code-defines. Bij `HX1838_BRON_CODES_EEPROM_WANNEER_GEEN_DEFINE` wordt de vaste mapping gebruikt zodra minstens één `HX1838_CODE_x` in `UserConfig.h` is gedefinieerd; wanneer geen enkele code is gedefinieerd, wordt EEPROM gebruikt en zo nodig gekalibreerd. Deze EEPROM-gebaseerde routes blijven in v1.1.1 experimenteel. Een afzonderlijke gebruikersroute om later bewust opnieuw te kalibreren blijft een toekomstig instellingenwerkpunt.

## Platformafhankelijke opslag

Platformafhankelijke EEPROM-initialisatie en commit horen niet verspreid op losse architectuurmacro's te steunen. De Input-laag gebruikt hiervoor de centrale `BOARD_VERSION`-indeling; ondersteuning voor nieuwe boards wordt per ondersteund board verder ingevuld.


## Configuratie

De Input-laag gebruikt `INPUT_KANAAL_CONFIG` als bitmask. Het huidige elektronische schema valt standaard terug op `INPUT_TYPE_DIGITAL`. Wanneer `KEYPAD_TYPE` niet expliciet is ingesteld, wordt `KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4` gebruikt. `KEYPAD_TYPE` kiest het fysieke keypad; de gekozen backend bepaalt hoe dat keypad wordt gelezen. DIGITAL wordt in deze fase beperkt tot keypads met maximaal vier digitale signaallijnen. PCF8574 kan de huidige keypadtypen lezen zolang maximaal acht expanderlijnen nodig zijn.

Het PCF8574-adres heet canoniek `I2C_ADDRESS_PCF8574`. Vaste HX1838-codes kunnen via `HX1838_CODE_1` tot en met `HX1838_CODE_21` in `UserConfig.h` worden ingevuld. De instelling `HX1838_BRON_CODES` in `UserConfig.h` bepaalt expliciet welke codebron voorrang krijgt.

### DIGITAL-pinmapping per keypadtype

Bij `INPUT_TYPE_DIGITAL` wordt de fysieke Arduino Uno-shieldpinmapping per `KEYPAD_TYPE` vastgelegd. De niet-membraantypes volgen de natuurlijke volgorde D2,D3,D4,D5. Alleen de twee membraan-directtypes behouden de bestaande gedraaide volgorde D3,D2,D5,D4.

Die gedraaide D3,D2,D5,D4-volgorde hoort bij het membraan-directtype dat beschikbaar was om te testen. Deze softwaremapping volgt de vaste flatkabelvolgorde, zodat er geen draden fysiek gewisseld of gekruist moeten worden.

| `KEYPAD_TYPE` | Keypadlabel / aansluiting | Arduino Uno-shieldpin |
|---|---|---|
| `KEYPAD_TYPE_DRUKKNOP_DIRECT_1x4` | K1, K2, K3, K4 | D2, D3, D4, D5 |
| `KEYPAD_TYPE_DRUKKNOP_MATRIX_2x2` | L1, L2, R1, R2 | D2, D3, D4, D5 |
| `KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4` | 1, 2, 3, 4 | D3, D2, D5, D4 |
| `KEYPAD_TYPE_MEMBRAAN_DIRECT_4x1` | 1, 2, 3, 4 | D3, D2, D5, D4 |
| `KEYPAD_TYPE_TOUCH_TTP224_DIRECT_1x4` | OUT1, OUT2, OUT3, OUT4 | D2, D3, D4, D5 |

De bestaande namen `PIN_TOETS_1` tot en met `PIN_TOETS_4` blijven behouden als backward-compatibilitylaag. `PIN_TOETS_1..4` zijn niet noodzakelijk de fysieke connectorlabels van elk keypadtype, maar de bestaande legacylaag waarop de huidige Input-code blijft werken. Wanneer een bestaande `UserConfig.h` deze namen expliciet definieert, hebben die waarden voorrang. Voor nieuwe DIGITAL-configuraties wordt bij voorkeur de keypad-specifieke `KEYPAD_PIN_...`-mapping gebruikt. De vorm van die namen volgt het gekozen keypadtype: drukknoppen direct gebruiken `KEYPAD_PIN_K1..K4`, membraan-directtypes gebruiken `KEYPAD_PIN_1..4`, touch-directtypes gebruiken `KEYPAD_PIN_OUT1..OUT4` en matrixvarianten gebruiken hun rij-/kolomlabels. `Input.cpp` blijft de bestaande `PIN_TOETS_1..4`-laag gebruiken; `SystemConfig.h` koppelt deze automatisch aan de actieve keypad-specifieke mapping.


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

`OpvragenHuidigeToetsAanslag(false)` blokkeert niet. Met de uitgebreide gebeurtenisschakelaar actief loopt een fysiek keypad bij iedere poll door dezelfde debounce-/stabiele-toestandsverwerking; zonder die schakelaar blijft het oorspronkelijke directe non-blocking gedrag behouden. Bij HX1838 wordt een ontvangen IR-gebeurtenis teruggegeven.

De enkelvoudige API `OpvragenHuidigeToetsAanslag()` geeft maximaal één aanslag per aanroep terug.

De publieke meervoudige API is al vastgelegd als `OpvragenHuidigeToetsAanslagen(bool wachten = true, byte aantalSimultaan = 1)`. In v1.1.0 is `MAX_AANTAL_SIMULTANE_TOETSAANSLAGEN` nog `1`. Daardoor is alleen `aantalSimultaan = 1` werkelijk geïmplementeerd en gebruikt die functie intern dezelfde werking als `OpvragenHuidigeToetsAanslag()`. Elke andere waarde retourneert `StatusOpvragenToetsAanslagen::NIET_GEIMPLEMENTEERD`.

Wanneer simultane aanslagen later worden uitgewerkt, blijft hetzelfde publieke prototype behouden. `aantalSimultaan = 2` zal dan exact twee gelijktijdige aanslagen betekenen: minder dan twee is `TE_WEINIG`, exact twee is `GELDIG` en meer dan twee is `TE_VEEL`.

`OpzoekenUitTeVoerenFunctieViaOpschriftToetsAanslag()` zoekt het ontvangen opschrift in een mapping en retourneert een pointer naar de volledige koppeling met de functiepointer. De eenvoudige overload gebruikt de vaste `mappingTussenToetsaanslagEnUitTeVoerenFunctie`; de overload met een mapping-array zoekt uitsluitend in de expliciet doorgegeven mapping.

`UitVoerenFunctieVolgensMappingMetToetsAanslag(bool wachten = true)` leest een toetsaanslag, zoekt die in de vaste mapping en voert de gekoppelde functie uit. Voor meerdere menu's kan dezelfde functienaam gebruikt worden met een expliciete mapping-array, bijvoorbeeld `UitVoerenFunctieVolgensMappingMetToetsAanslag(true, mappingInstellingenMenu)`. Het aantal elementen hoeft niet afzonderlijk te worden doorgegeven: dit wordt compile-time uit de array afgeleid. `InputkanalenDIGITAL.ino`, `InputkanalenPCF8574.ino` en `InputkanalenHX1838.ino` testen deze volledige keten, elk met parameterloze functies (`InputFunctie`/`MappingTussenToetsaanslagEnUitTeVoerenFunctie`) die hun eigen opschrift via `PrintToScreen()` tonen.

Daarnaast bestaat een volledig apart, tweede type, `InputFunctieMetArgumenten`/`MappingTussenToetsaanslagEnUitTeVoerenFunctieMetArgumenten`, met een `argumenten`-veld (`void*`) dat aan de functie wordt doorgegeven. Dit tweede type raakt het eerste op geen enkele manier: bestaande mappings met parameterloze functies blijven ongewijzigd werken. `InputkanalenDIGITALmetArgumenten.ino`, `InputkanalenPCF8574metArgumenten.ino` en `InputkanalenHX1838metArgumenten.ino` demonstreren dit tweede type, inclusief gedeelde toestand tussen meerdere functies via hetzelfde `argumenten`-adres.

## Uitgebreide gebeurtenissen (loslaten, lang indrukken, timeout)

Standaard uitgeschakeld, om geen extra geheugen te verbruiken op geheugenarme boards. Inschakelen via `#define INPUT_KANAAL_OPVRAGEN_HUIDIGE_TOETSAANSLAG_UITGEBREID` in `UserConfig.h`. Enkel van toepassing bij `OpvragenHuidigeToetsAanslag(false)`; bij `wachten=true` blokkeert de bestaande code toch al tot loslaten, dus daar valt niets nieuws te detecteren.

- **Loslaten**: geeft de positie terug die net losgelaten werd, als apart `InputResultaat` met `gebeurtenis = LOSGELATEN`. Wanneer voor die toets `functieBijLoslaten` is ingesteld, voert `UitVoerenFunctieVolgensMappingMetToetsAanslag()` die functie uit.
- **Lang indrukken**: per toets optioneel in te stellen via `functieBijLangIndrukken` en `langIndrukkenDrempelMs` in `MappingTussenToetsaanslagEnUitTeVoerenFunctie`. Wanneer `OpvragenHuidigeToetsAanslag(false)` een `LANG_INDRUKKEN`-gebeurtenis teruggeeft, voert `UitVoerenFunctieVolgensMappingMetToetsAanslag()` de gekoppelde `functieBijLangIndrukken` uit. De gewone `functie` blijft gekoppeld aan `TOETSAANSLAG`. Bij een expliciet meegegeven mapping wordt `langIndrukkenDrempelMs` uit die werkelijk gebruikte mapping opgezocht.
- **Timeout bij geen invoer**: `INPUT_KANAAL_TIMEOUT_BIJ_GEEN_TOETSAANSLAG_BINNEN_MS`, standaard `0` (geen timeout, huidig gedrag). Bij een waarde groter dan 0 start de timer in `InputConfigureren()` en geeft `OpvragenHuidigeToetsAanslag(false)` na die periode een `InputResultaat` met `gebeurtenis = TIMEOUT_GEEN_INVOER` terug. Ook de huidige enkelvoudig geïmplementeerde `OpvragenHuidigeToetsAanslagen(..., 1)` geeft dit timeoutresultaat door.

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

Onder `examples/Systeem/Input/` staan `InputkanalenDIGITAL.ino`, `InputkanalenPCF8574.ino`, `InputkanalenHX1838.ino`, hun `metArgumenten`-tegenhangers (`InputkanalenDIGITALmetArgumenten.ino`, `InputkanalenPCF8574metArgumenten.ino`, `InputkanalenHX1838metArgumenten.ino`) en de Input-testkopieën van de Stimulus-scenario's. De originele Stimulus-voorbeelden blijven onaangeroerd. De testkopieën zijn bedoeld om te bewijzen dat dezelfde keuzevolgorde via `InputConfigureren()` en `OpvragenHuidigeToetsAanslag(true)` kan verlopen.


Beschikbare HX1838-toetsenindelingen:

- `HX1838_TOETSENINDELING_REMOTE_OK_BOVENAAN_17_TOETSEN`: remote met 17 toetsen: (UP, DOWN, OK, LEFT, RIGHT, 1-9, *, 0, #) [UP, DOWN, OK, LEFT, RIGHT, 1, 2, 3, 4, 5, 6, 7, 8, 9, *, 0, #]: [HX1838_TOETSENINDELING_REMOTE_17_TOETSEN_OK_BOVENAAN.png](Screenshots/HX1838_TOETSENINDELING_REMOTE_17_TOETSEN_OK_BOVENAAN.png).
- `HX1838_TOETSENINDELING_REMOTE_OK_ONDERAAN_17_TOETSEN`: remote met 17 toetsen: (1-9, *, 0, #, UP, DOWN, OK, LEFT, RIGHT) [1, 2, 3, 4, 5, 6, 7, 8, 9, *, 0, #, UP, DOWN, OK, LEFT, RIGHT]: [HX1838_TOETSENINDELING_REMOTE_17_TOETSEN_OK_ONDERAAN.png](Screenshots/HX1838_TOETSENINDELING_REMOTE_17_TOETSEN_OK_ONDERAAN.png);
- `HX1838_TOETSENINDELING_REMOTE_21_TOETSEN_MP3`        : remote met 21 toetsen, inclusief de MP3-toetsen; referentiebeeld: [HX1838_TOETSENINDELING_REMOTE_21_TOETSEN_MP3.jpeg](Screenshots/HX1838_TOETSENINDELING_REMOTE_21_TOETSEN_MP3.jpeg).
- `HX1838_TOETSENINDELING_REMOTE_USER_DEFINED`          : gebruiker bepaalt zelf het aantal toetsen, de 8-bit commandcodes en de opschrift/weergavetekstkoppeling. In v1.1.1 is deze indeling uitsluitend beschikbaar met `HX1838_BRON_CODES_DEFINE`.

Voor `HX1838_TOETSENINDELING_REMOTE_USER_DEFINED` moeten in `UserConfig.h` `HX1838_GENERIEK_AANTAL_TOETSEN` en `HX1838_GENERIEK_KEY_LAYOUT` ingesteld worden. `HX1838_GENERIEK_CODES` is optioneel: wanneer deze define ontbreekt, start automatisch de UserDefined-kalibratie; wanneer ze wel is ingesteld, moet het aantal codes exact overeenkomen met `HX1838_GENERIEK_AANTAL_TOETSEN` en worden nulcodes geweigerd. Het aantal layoutentries moet altijd exact overeenkomen met `HX1838_GENERIEK_AANTAL_TOETSEN`. De huidige implementatie vergelijkt de door TinyIRReceiver/IRremote gedecodeerde 8-bit `command`-waarde (`uint8_t`), zodat UserDefined in v1.1.1 niet claimt ieder mogelijk IR-protocol of iedere volledige protocol/address/command-combinatie te ondersteunen. Zie ook `InputkanalenHX1838UserDefined.ino`.

`HX1838_USE_TINYIRRECEIVER_INSTEAD_OF_IRREMOTE` kiest de HX1838-ontvangstbackend: `1` gebruikt TinyIRReceiver en `0` gebruikt IRremote. De standaard `HX1838_ONTVANGER_PIN` is in v1.1.1 D12. Met `HX1838_BRON_CODES = HX1838_BRON_CODES_DEFINE` zijn op de geteste hardwareopstelling zowel waarde `1` als waarde `0` met D12 getest en werkend bevonden; deze DEFINE-route is vanaf v1.1.1 released. De EEPROM-gebaseerde HX1838-routes blijven experimenteel.

Voor de vaste mapping gebruikt `HX1838_TOETSENINDELING_REMOTE_OK_BOVENAAN_17_TOETSEN` `HX1838_CODE_1` t.e.m. `HX1838_CODE_17`, `HX1838_TOETSENINDELING_REMOTE_OK_ONDERAAN_17_TOETSEN` `HX1838_CODE_1` t.e.m. `HX1838_CODE_17` en `HX1838_TOETSENINDELING_REMOTE_21_TOETSEN_MP3` `HX1838_CODE_1` t.e.m. `HX1838_CODE_21`. Niet zelf gedefinieerde codes worden door `SystemConfig.h` met de standaardcodes aangevuld. Een expliciet gedefinieerde code met waarde `0` is ongeldig wanneer de vaste mapping wordt gebruikt.

## Mapping-volledigheidscontrole (ControleerMappingVolledigheid)

Een mapping (`MappingTussenToetsaanslagEnUitTeVoerenFunctie[]`, zoals `mappingTestMenu[]` in de
Input-voorbeelden) die niet elk opschrift bevat dat het gecompileerde `KEYPAD_TYPE`/
`HX1838_TOETSENINDELING` kan opleveren, geeft geen compilatiefout en geen waarschuwing: bij een
niet-gevonden opschrift gebeurt er gewoon stilzwijgend niets bij die toetsdruk.

`ControleerMappingVolledigheid(mapping)` controleert dit tijdens het draaien: ze doorloopt
`KEY_LAYOUT[]`/`IR_KEY_LAYOUT[]` van het actief gecompileerde type en meldt ontbrekende
opschriften via `PrintToScreen()`. Daardoor volgt de controle dezelfde geselecteerde uitvoerdoelen
als de rest van de Screen-laag; seriële standaarduitvoer via `PrintToScreen()` volgt daarbij de bestaande
`DEBUG`-werking van de Screen-laag.

Enkel beschikbaar wanneer `INPUT_MAPPING_EXTRA_CONTROLES_INSCHAKELEN` in `UserConfig.h` gedefinieerd
is. Zonder die define bestaat de functie nergens in het gecompileerde programma: geen extra
flashgebruik, geen `Serial`-afhankelijkheid. Bedoeld om tijdens het testen op te roepen (bijvoorbeeld
eenmalig in `setup()`, ná `InputConfigureren()`), niet om in productiecode te laten staan.

De releasevalidatie bevat daarnaast een tekstuele volledigheidscontrole die geen upload of aangesloten board vereist. De gedeelde Windows-testscripts staan onder `extras/` en worden mee gepubliceerd. Alleen `extras/LokalePaden.cmd` is machinespecifiek en blijft via `.gitignore` lokaal; `extras/LokalePaden_template.cmd` wordt wel meegeleverd.

## Generiek keypad (experimenteel)

`KEYPAD_TYPE_USER_DEFINED_DIRECT` en `KEYPAD_TYPE_USER_DEFINED_MATRIX` laten toe om een nieuw fysiek keypad te testen zonder de bibliotheek zelf aan te passen. Alle vereiste `KEYPAD_GENERIEK_...`-instellingen worden in normaal gebruik volledig in `UserConfig.h` vastgelegd; definities in een `.ino` kunnen het apart gecompileerde `Input.cpp` niet configureren. Zie `UserConfig_template.h` en de twee UserDefined-voorbeelden voor een volledige configuratie.

Enkel beschikbaar bij `INPUT_TYPE_PCF8574`. `KEYPAD_GENERIEK_PINNEN`/`_RIJ_PINNEN`/`_KOLOM_PINNEN` zijn bitposities op de PCF8574 (0 t.e.m. 7), geen Arduino-pinnummers. De build controleert voor UserDefined-keypads de vereiste instellingen, het verwachte aantal layoutentries, het pinbereik, dubbele pinnen en bij matrices ook overlap tussen rij- en kolompinnen.

Bij `KEYPAD_TYPE_USER_DEFINED_DIRECT` bepaalt `KEYPAD_GENERIEK_OUTPUT_LEVEL_WHEN_KEY_PRESSED` of een toets active-low of active-high wordt gelezen. `KEYPAD_TYPE_USER_DEFINED_MATRIX` gebruikt de vaste matrixscan van `Input.cpp` en is **altijd active-low**: de geselecteerde rij wordt laag gemaakt en een ingedrukte toets wordt als lage kolomingang gedetecteerd. Een generieke matrix moet dus active-low worden bekabeld.

`examples/Systeem/Input/InputkanalenPCF8574UserDefinedDirect/InputkanalenPCF8574UserDefinedDirect.ino` en `examples/Systeem/Input/InputkanalenPCF8574UserDefinedMatrix/InputkanalenPCF8574UserDefinedMatrix.ino` bevatten de vereiste configuratie als commentaarvoorbeeld, maar definiëren die waarden niet zelf. De expliciete `mappingTestMenu` wordt aangeroepen met `UitVoerenFunctieVolgensMappingMetToetsAanslag(true, mappingTestMenu)`.

Dit blijft bewust een experimenteel pad: eens een configuratie bewezen werkt, kan ze overgezet worden naar een echt, benoemd `KEYPAD_TYPE_...` in `InputTypes.h`, met documentatie en screenshot zoals de bestaande, officieel ondersteunde types.
