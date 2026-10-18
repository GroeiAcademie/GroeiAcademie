# Decision Log

> Dit logboek bewaart ook historische beslissingen. Namen, paden en API’s in oudere beslissingen kunnen dus bewust afwijken van de huidige beta v2.0.0-code. Wanneer een beslissing expliciet is achterhaald, staat dat bij die beslissing vermeld.


Dit logboek bevat kernbeslissingen. Nieuwe beslissingen krijgen een nieuw nummer en vervangen oudere besluiten niet stilzwijgend.

## D001 Projectnaam

**Besluit:** GroeiAcademie FrameWork.

## D002 Eerste doel

**Besluit:** menselijke vaardigheden meetbaar, oefenbaar en onderzoekbaar maken, met de eerste toepassing binnen NLP.

## D003 Motto

**Besluit:** Meten is weten, in plaats van denken te weten.

## D004 Doelgroepen

**Besluit:** onderzoekers, biofeedback-onderzoekers, NLP-beoefenaars, NLP-trainers en geïnteresseerde leken.

## D005 Taal

**Besluit:** de gezaghebbende documentatie wordt eerst in het Nederlands ontwikkeld.

## D006 Eerste boards

**Besluit:** Arduino UNO R3 en UNO R4. De huidige voorbeelden zijn gecompileerd voor UNO R3, UNO R4 Minima en UNO R4 WiFi.

## D007 Ontwikkel- en validatieomgeving

**Besluit:** Arduino IDE is de primaire gebruikersomgeving. Arduino CLI en Arduino Lint worden gebruikt voor reproduceerbare validatie.

## D008 Open-sourcefilosofie

**Besluit:** het project gebruikt de GNU LGPL v3.0-or-later. Iedereen mag de library vrij gebruiken, commercieel gebruik is toegestaan en verbeteringen aan de library moeten beschikbaar blijven voor de gemeenschap.

## D009 Communitygrens

**Besluit:** commerciële activiteiten mogen het open project ondersteunen, maar niet de vrije toegang, transparantie of communitypositie ondergraven.

## D010 Librarystructuur

**Besluit:** één geïntegreerde Arduino-library met broncode onder `src/` en functionele submappen.

## D011 Eerste module

**Besluit:** de eerste geïmplementeerde toepassingsmodule is Stimulus, voor het meten en vergelijken van vingertikken.

## D012 Centrale Screen-opdracht

**Besluit:** één `PrintToScreen()`-aanroep draagt de volledige schermopdracht over. Een geregistreerde callback wordt per schermtype maximaal één keer aangeroepen en beheert zelf de volledige uitvoer voor dat type.

## D013 Elektronische schema's

**Besluit:** elk geïmplementeerd toepassingsgebied krijgt raadpleegbare elektronische documentatie met pinnen, voeding, componenten, sensorvariant, kalibratie en veiligheidsinformatie. Zolang nog geen afzonderlijke tekenbestanden aanwezig zijn, is de Markdown-specificatie gezaghebbend.

## D014 Pre-1.0-status

**Besluit:** geplande modules worden in documentatie duidelijk onderscheiden van reeds geïmplementeerde en gevalideerde modules.


## D015 Bijdragen

**Besluit:** iedereen mag bijdragen via GitHub Pull Requests en ideeën, voorstellen, vragen en bugs melden via GitHub Issues. Grote wijzigingen worden bij voorkeur vooraf besproken. Niet elke bijdrage wordt automatisch opgenomen. De maintainers nemen de uiteindelijke beslissing op basis van visie, architectuur, kwaliteit, documentatie, prestaties, compatibiliteit en langetermijndoelstellingen.

## D016 Contributor License Agreement

**Besluit:** er komt geen Contributor License Agreement. Contributors behouden hun auteursrecht.

## D017 Copyright

**Besluit:** iedere contributor behoudt het copyright op zijn of haar eigen bijdragen. GroeiAcademie FrameWork blijft beheerd door GROEI ACADEMIE BV.

## D018 Ondersteuningsniveaus

**Besluit:** hardware en functionaliteit worden ingedeeld als Officieel ondersteund, Community ondersteund of Experimenteel. Voor elk niveau worden de verwachtingen duidelijk omschreven.

## D019 API-stabiliteit en Semantic Versioning

**Besluit:** `0.x` zijn ontwikkelversies waarin de API nog kan wijzigen. Vanaf `1.0.0` gelden maximale API-stabiliteit, het vastgelegde deprecationbeleid en Semantic Versioning 2.0.0.

## D020 Zichtbaarheid van interne hulpfuncties

**Besluit:** functies die uitsluitend als bouwsteen dienen binnen één bronbestand van de library: en door geen enkel voorbeeld of extern bekende gebruiker rechtstreeks worden aangeroepen: krijgen interne (`static`) zichtbaarheid in plaats van een publiek prototype in de header. Dit geldt met terugwerkende kracht voor twaalf functies uit de Screen- en Stimulus-laag die abusievelijk publiek stonden: `BepaalAantalSensorenSynchroon()`, `BerekenEindStimulus()`, `InitialiseerSensorStart()`, `MaakSensorMask()`, `MaakSynchronisatieProfielAlleSensoren()`, `ResetStimulusProfiel()`, `ResetSynchronisatieProfiel()`, `VerwerkSensor()`, `PixelScreenClear()`, `PixelScreenSetCursor()`, `PixelScreenPrint()` en `PixelScreenFoutmeldingWeergeven()`. `MaakSynchronisatieProfiel()` blijft bewust extern staan, ondanks dat ze momenteel nergens gebruikt wordt, als voorbereide bouwsteen voor toekomstige paarsgewijze synchronisatie.

**Context:** dit wordt uitgevoerd terwijl v1.0.0/v1.0.1 nog maar zeer kort in de Library Manager staan en er geen gekende externe gebruikers zijn: de kost van deze correctie neemt met de tijd toe onder het API-stabiliteitsbeleid (D019).

## D021 Terminologie: "board" vs. "Arduino Uno R3-vormfactorbord"

**Besluit:** doorheen de documentatie wordt de generieke term "board"/"moederbord" vervangen door de volledige, exacte term "Arduino Uno R3-vormfactorbord(en)" wanneer een fysiek bord bedoeld wordt. Uitzonderingen: Arduino's eigen productterminologie (`Arduino Boards Manager`, `boardprofiel`) blijft ongewijzigd, evenals de al bestaande vaste samenstelling "ESP32-borden in Arduino Uno R3-vormfactor" en letterlijke citaten van compiler-uitvoer. Historische titels in dit logboek (bv. D006) worden niet met terugwerkende kracht herschreven.

**Context:** met v1.0.3 voor het eerst consequent doorgevoerd over de volledige documentatie.

## D022 Screen-configuratie: verplicht en expliciet, geen impliciete auto-configuratie

**Status: achterhaald in beta v2.0.0.** De huidige Screen-code gebruikt de GedeeldeBus-objectlifecycle; de hieronder genoemde losse `CharacterScreenConfigureren()`, `PixelScreenConfigureren()` en `ScreensConfigureren()` behoren tot de vroegere architectuur. Onderstaande tekst blijft staan als historische beslissing.

**Besluit:** `CharacterScreenConfigureren()` wordt toegevoegd, symmetrisch met de bestaande `PixelScreenConfigureren()`: beide doen een echte hardwarecontrole (I2C-handdruk resp. resolutiecontrole), bewaren enkel hun eigen status en foutcode, en tonen zelf niets. `PrintToScreenIntern()` controleert voortaan vooraan of de vereiste configuratiefunctie is aangeroepen en toont, maximaal één keer, de gepaste foutmelding (`CS000`/`PS000` bij een vergeten aanroep, de opgeslagen foutcode bij een mislukte configuratie): daarna stopt die aanroep. De bestaande impliciete auto-configuratie van `PixelScreen` (`if (pixelScreenGeselecteerd && pixelScreenStatus.pixelScreenActief) PixelScreenConfigureren();`) wordt volledig verwijderd, zonder fallback. `ScreensConfigureren()` wordt toegevoegd als optionele gemakslaag die, aan de hand van `SCREEN_OUTPUT`, enkel de nodige configuratiefunctie(s) aanroept: de losse `CharacterScreenConfigureren()`/`PixelScreenConfigureren()` blijven gewoon publiek en worden niet gedeprecieerd, want `PixelScreenConfigureren()` wordt al in 8 van de 12 voorbeelden rechtstreeks gebruikt.

**Gevolg, bewust aanvaard:** een sketch die vandaag werkt zonder expliciete `PixelScreenConfigureren()`-aanroep (steunend op de verwijderde impliciete auto-configuratie) werkt na deze wijziging niet meer: dit is een bewuste breaking change.

**Context:** dezelfde redenering als D020: nog geen gekende externe gebruikers, dus nu bijna kosteloos; later zou dit een verplichte deprecatiecyclus vereisen (D019).

## D023 CHARACTERSCREEN_I2C_ADRES_MODUS

**Status: achterhaald in beta v2.0.0.** De huidige code gebruikt `I2C_ADDRESS_CHARACTER_SCREEN`; de beschreven adresmodi en automatische kandidatenlijst bestaan niet meer. Onderstaande tekst blijft staan als historische beslissing.

**Besluit:** `CharacterScreenConfigureren()` krijgt een instelbare `CHARACTERSCREEN_I2C_ADRES_MODUS` (default `1`) om om te gaan met I2C-characterschermen waarvan het adres niet vooraf gekend is:

- **0**: geen scan, enkel de bestaande handdruk-check op het geconfigureerde `I2C_ADRES`. Kleinste footprint; voor boards die tegen hun geheugengrens zitten (bv. gekende UNO R3-beperkingen, zie `extras/TESTRESULTATEN.md`).
- **1** (standaard): scan van een korte kandidatenlijst (`I2C_ADRES`, dan `0x27`, `0x3F`) en rapporteren via de foutmelding (`CS002`, met het gevonden adres erin), geen zelfherstel. Voor normaal gebruik en overal waar reproduceerbaarheid/betrouwbaarheid telt (onderzoek, metingen, meerdere I2C-apparaten op de bus).
- **2**: scan + automatisch herbouwen van het `lcd`-object op het gevonden adres via placement-new (`lcd.~LiquidCrystal_I2C(); new (&lcd) LiquidCrystal_I2C(...);`), zodat `lcd.`-aanroepen in bestaande voorbeelden en gebruikerscode ongewijzigd blijven werken. Nooit meer hercompileren bij een ander scherm/adres. Enkel voor actief ontwikkelen met wisselende schermen, geen ander I2C-apparaat op de bus, geen lopende metingen: de scan controleert enkel of een apparaat op dat adres reageert (ACK), niet of het effectief een LCD-scherm is.

**Overwogen en verworpen:** een generieke `BETATESTER`-vlag die "alles wat nog niet in alpha zit" zou aanzetten: verworpen omdat dit een tweede, parallel classificatiesysteem naast de bestaande ondersteuningsniveaus (D018) zou zetten voor precies één concreet experiment. Bewaard als aantekening in `docs/ROADMAP.md` voor wanneer een tweede, onafhankelijk experiment opduikt. Hergebruik van `DEBUG` voor dit doel werd eveneens verworpen: `DEBUG` betekent vandaag uitsluitend extra Serial-logging, en een DEBUG-build zou daardoor een ander Screen-gedrag krijgen dan de release-build: precies het omgekeerde van wat een DEBUG-build hoort te doen.

**Context:** aanleiding was een concrete situatie (LCD2004-schermen met onbekend/wisselend I2C-adres, wens om niet telkens te moeten hercompileren).

## D024 Configuratiecontrole geldt enkel voor de ingebouwde hardware

**Status: achterhaald in beta v2.0.0.** De huidige Screen-code gebruikt de GedeeldeBus-objectlifecycle; de hieronder genoemde losse `CharacterScreenConfigureren()`, `PixelScreenConfigureren()` en `ScreensConfigureren()` behoren tot de vroegere architectuur. Onderstaande tekst blijft staan als historische beslissing.

**Besluit:** het configuratiecontroleblok in `PrintToScreenIntern()` (dat `CS000`/`PS000` toont bij een vergeten configuratie-aanroep) geldt enkel wanneer er geen callback geregistreerd is voor dat schermtype. Is er een `CallbackScreenTypeCharacter`/`CallbackScreenTypePixel` geregistreerd, dan wordt die callback niet langer geblokkeerd wanneer de ingebouwde hardware niet geconfigureerd is: de callback-gebruiker blijft zelf verantwoordelijk voor het al dan niet configureren van de ingebouwde hardware, indien die callback ze nog gebruikt.

**Context:** `SCREEN_TYPE_CHARACTER`/`SCREEN_TYPE_PIXELS` vertegenwoordigen zo consequent "er is uitvoer van dit type gewenst", niet specifiek "de ingebouwde hardware moet werken": in lijn met hoe de rest van `PrintToScreenIntern()` een callback al als volledig apart pad behandelde. Naar aanleiding van code review, vóór publicatie van v1.0.4.

## D025 GedeeldeBus: centralisatie van businitialisatie

**Besluit:** `Screen.cpp`, `Input.cpp` (PCF8574-tak) en `Stimulus.cpp` (`InitialiseerADS1115()`) riepen voorheen elk apart `Wire.begin()`/`SPI.begin()` aan, met identieke maar afzonderlijk onderhouden board-specifieke logica. Dit is gecentraliseerd in een nieuw `Systeem/GedeeldeBus`-bestand, gedragsbehoudend.

**Context:** concreet risico verholpen: bij gelijktijdig gebruik van bijvoorbeeld Input met PCF8574 én een I2C-scherm was er voorheen geen garantie op conflictvrije businitialisatie. Toegevoegd in v1.1.0.

## D026 Aparte types voor Input met/zonder argumenten

**Besluit:** `InputFunctieMetArgumenten`/`MappingTussenToetsaanslagEnUitTeVoerenFunctieMetArgumenten` worden volledig apart gehouden van het bestaande `InputFunctie`/`MappingTussenToetsaanslagEnUitTeVoerenFunctie`, met een eigen `void* argumenten`-veld. Het eerste type wordt op geen enkele manier geraakt.

**Context:** toegevoegd in v1.1.0, als tweede, onafhankelijk type naast het bestaande.

## D027 Experimentele, gebruikersgedefinieerde keypadtypes

**Besluit:** `KEYPAD_TYPE_USER_DEFINED_DIRECT`/`KEYPAD_TYPE_USER_DEFINED_MATRIX` toegevoegd, experimenteel, enkel bij `INPUT_TYPE_PCF8574`: laat toe een nieuw, fysiek keypad te testen zonder de bibliotheek zelf aan te passen, door pinnen, `KEYPAD_GENERIEK_OUTPUT_LEVEL_WHEN_KEY_PRESSED` en de opschriftkoppeling volledig in `UserConfig.h` in te stellen.

**Context:** toegevoegd in v1.1.0. Ontbrekende, vereiste instellingen geven een leesbare `#error`-tekst tijdens het compileren.

## D028 HX1838 DEFINE-route: van experimenteel naar released

**Besluit:** HX1838 gaat voor `HX1838_BRON_CODES = HX1838_BRON_CODES_DEFINE` van experimenteel (v1.1.0) naar released (v1.1.1). De EEPROM-gebaseerde routes (`HX1838_BRON_CODES_EEPROM_ALTIJD`, `HX1838_BRON_CODES_EEPROM_WANNEER_GEEN_DEFINE`) blijven experimenteel.

**Context:** `HX1838_BRON_CODES_DEFINE` is hardwarematig getest en werkend met zowel TinyIRReceiver als IRremote, telkens met de ontvanger op Arduino Uno-shieldpin `D12`.

## D029 Test-indeling naar regressierisico

**Besluit:** de testindeling wordt opgesplitst volgens regressierisico: `TestLibraryGereleased.cmd` voert de volledige gereleasete regressiematrix uit; `TestLibraryNieuw.cmd` test de volledige nieuwe matrix voor functionaliteit die nog niet released is; `TestLibraryNieuwOngeldig.cmd` controleert de ongeldige configuraties.

**Context:** toegevoegd in v1.1.1. `extras/TestLibraryGereleasedVolledigeRegresietesten.cmd` bestaat daarnaast als aparte, volledige regressietest, geen deel van de vier releasecompilecycli.

## D030 Canonieke boardnamen

**Besluit:** de vier aanvullende boardconstanten gebruiken vanaf v1.1.2 consequent de canonieke architectuur- en boardfamilienamen (`BOARD_ESP32_D1_UNO_R32`, `BOARD_ESP32S3_ARDI32`, `BOARD_RP2040_CYTRON_MAKER_UNO`, `BOARD_STM32F4_NUCLEO64_F401RE`), met behoud van hun bestaande numerieke waarden.

**Context:** alle code, testscripts, voorbeelden en Markdowndocumentatie zijn naar deze vier namen bijgewerkt.

## D031 Stimulus Shield v1.1.2 als enige geldige hardwarebron

**Besluit:** de foutieve voorgangers van de Stimulus Shield zijn uit de actuele release verwijderd. Uitsluitend de tekeningset en documentatie onder `docs/Uitbreidingskaarten/Stimulus Shield v1.1.2/` gelden als hardwarebron.

**Context:** toegevoegd in v1.1.2. HX1838 DATA gebruikt Arduino Uno-shieldpin `D8`; PixelScreen RST gebruikt Arduino Uno-shieldpin `D7`.

## D032 v2.0.0: GedeeldeBus als neutrale lifecycle- en resourcebeheerlaag

**Besluit:** GedeeldeBus blijft zo klein, neutraal en transparant mogelijk. Screen, Input, Stimulus, Extenders en Sensoren blijven onafhankelijke, los te testen subsystemen. GedeeldeBus beheert generiek aanmelden, controleren, inpluggen, activeren, afmelden en resourceconflicten. Concrete hardwarekennis hoort zoveel mogelijk bij de concrete component.

**Context:** beta v2.0.0, nodig om meerdere hardwarebronnen, Extenders en Sensoren binnen één consistente architectuur te kunnen combineren.

## D033 v2.0.0: vaste lifecycle

**Besluit:** de normale component-lifecycle is `aanmelden()` → `controleren()` → `inpluggen()` → `activeren()`. `GedeeldeBusNewComponent<T>(...)` voert deze stappen in deze volgorde uit en geeft `nullptr` terug wanneer de cyclus niet slaagt.

**Context:** Screen, Input, Extenders en Sensoren gebruiken hiermee dezelfde basis.

## D034 ExtenderPins: vaste prefix EP_

**Besluit:** iedere waarde binnen iedere `enum class ExtenderPins` begint met `EP_`; de rest van de naam blijft de concrete fabrikant-/resourcenaam zo dicht mogelijk volgen. Voorbeelden: `EP_AIN0`, `EP_GPIO0`, `EP_P0`, `EP_GPA0`, `EP_CH0`.

**Context:** voorkomt preprocessorconflicten met bestaande macro's zoals `GPIO0` in `SystemConfig.h` en geeft tegelijk overal dezelfde herkenbare ExtenderPin-naamconventie.

## D035 Inpluggen versus Diagnose

**Besluit:** uitgebreide elektronische functietesten horen in `Diagnose()` en `Diagnose()` bestaat uitsluitend wanneer `DEBUG` actief is; de basisfunctie is voorlopig een lege virtuele hook. De bestaande componentcode wordt stapsgewijs naar die grens gebracht; de huidige implementatie bevat nog hardwarecontroles in zowel `inpluggen()` als `Activeren()` (bijvoorbeeld de CharacterScreen-I2C-controle in `Activeren()`).

**Context:** productiecode mag geen uitgebreide testoverhead krijgen. Diagnose moet later per concrete component kunnen worden ingevuld.

## D036 Drie testlagen voor hardwarecomponenten

**Besluit:** hardwarevalidatie wordt onderscheiden in (1) GedeeldeBus-/architectuurtests, (2) elektronische functietests en (3) praktijktests met echte randapparatuur/sensoren. Een geslaagde lifecycle-test is geen bewijs dat alle elektronische functies van een chip zijn doorgemeten.

**Context:** expliciete scheiding voor Screen, Input, Extenders en Sensoren tijdens de beta-v2.0.0-validatie.

## D037 v2.0.0: FRAM als afzonderlijk Data-medium

**Besluit:** `DATA_FRAM` wordt als afzonderlijk Data-component opgenomen naast EEPROM, FLASH en microSD. De eerste concrete route is `FRAM_I2C` via Rob Tillaart `FRAM_I2C`; Fujitsu MB85RC256V gebruikt de libraryclass `FRAM`. Aanmelden, controleren, inpluggen en activeren blijven volledig binnen de bestaande GedeeldeBus-lifecycle. `FRAM_SPI` blijft optioneel voor later en wordt in deze stap niet geïmplementeerd.

**Context:** beta v2.0.0. FRAM biedt niet-vluchtige opslag met zeer veel schrijfcycli en vormt daarom een afzonderlijke opslagoptie; de bestaande STREAMING- en RECORDING-routes worden in deze stap niet gewijzigd.

## D038 v2.0.0: Data-media, één bit per medium en aansluiting

**Besluit:** elk Data-medium krijgt per aansluiting een eigen bit: `DATA_TYPE_EEPROM_NATIVE`, `DATA_TYPE_EEPROM_I2C`, `DATA_TYPE_FLASH_SPI`, `DATA_TYPE_FRAM_I2C`, `DATA_TYPE_MICROSD_SPI` (`DATA_TYPE_EEPROM_SPI` en `DATA_TYPE_FRAM_SPI` gereserveerd); `typesActief` wordt 16 bits. Eén bestand per medium met alle aansluitingen (`EEPROM`, `FLASH`, `FRAM`, `microSD`). Data houdt per medium een lijst van zijn chips bij; elke chip wist zijn plaats in zijn destructor. DIRECT is een kind van Data (`DataDIRECT`). De componentcodes volgen de volgorde van de bits: `GC_DATA` 0x14, `DATA_DIRECT` 0x15, `DATA_EEPROM` 0x16, `DATA_FLASH` 0x17, `DATA_FRAM` 0x18, `DATA_MICROSD` 0x19. FLASH gebruikt `Adafruit SPIFlash` (zonder cache, eigen transport), microSD `SdFat` via de "SdFat - Adafruit Fork".

**Context:** beta v2.0.0, uitgewerkt in de reuzetabel Data-media (16-10-2026) en de zips van 17-10-2026.

## D039 v2.0.0: gemengde pinnen en CS via een Extender

**Besluit:** met `GEDEELDE_BUS_MIXED_PINS` (standaard 0) mag een component per exclusieve pin een eigen gebied hebben, het board of een Extender (`ownerClaim`, `GetOwnerClaim()`). Extenders met digitale pinnen (PCF8574, PCF8575, MCP23017) bieden `SetExtenderPinMode()`, `WriteExtenderPin()` en `ReadExtenderPin()`. FLASH en microSD kunnen CS op zo'n Extender hebben (O8); microSD vraagt daarvoor `SD_CHIP_SELECT_MODE` 1 of 2 in `SdFatConfig.h`. Met 0 wordt niets hiervan gecompileerd.

**Context:** de architectuur kende tot dan één resourcegebied per component; een sensor of SPI-chip met lijnen op het board én op Extenders is gewone praktijk.

## D040 v2.0.0: QueueOrStack alleen per sensor-channel

**Besluit:** de QueueOrStack bestaat alleen per sensor-channel. De nooit gebruikte `DATA_LAYER_<MEDIUM>_<NR>_QUEUE_OR_STACK_SIZE` zijn verwijderd. Voor FLASH bestaat een aparte schrijfbuffer `DATA_LAYER_FLASH_SPI_<NR>_BUFFER_SIZE` (standaard 0; anders minstens `sizeof(int)` en maximaal 256).

**Context:** een tweede QueueOrStack per medium zou dezelfde waarden opnieuw in RAM bewaren; de FLASH-buffer dient alleen om sneller per pagina te schrijven.

## D041 v2.0.0: bewust niet gedaan

**Besluit:** (1) de namen van GedeeldeBus niet naar PROGMEM verplaatsen: dat helpt alleen de UNO R3; (2) zonder Data blijft de naam `DATA_DIRECT` in de namenlijst (12 bytes op de UNO R3, 8 tot 16 bytes flash op de 32-bit boards); (3) geen `TransferFrame`: er is geen gebruiker; (4) config per sensor-pin om de Extender te kiezen pas wanneer een sensor het nodig heeft; (5) de QueueOrStack krijgt geen peek, geen leegmaken, geen andere types dan `int` en geen bewaren na een stroomonderbreking: het kan (bijvoorbeeld met `FRAM_RINGBUFFER` van FRAM_I2C), maar er is nu geen nood aan.

**Context:** maximale snelheid en minimaal geheugen blijven de maatstaf; de UNO R3 is alleen een testboard.

## D042 v2.0.0: runtime-herstel van I2C-componenten

**Besluit:** een fout tijdens het draaien maakt een I2C-component niet meer voor altijd onbruikbaar (vroeger: één fout bij Input PCF8574 = IN402 en nooit meer proberen).
- (1) Foutladder WARNING, ALERT, FAULT, CRITICAL, FATAL op aantal opeenvolgende fouten OF tijd sinds de eerste fout, met een hoogst toegelaten foutniveau en een foutniveau vanaf waar uitgeschakeld wordt (twee aparte tijdvelden, 11 bytes op AVR en 12 op 32-bit per component: met één gedeeld tijdveld (O3z) kon een uitgeschakeld component niet meer van CRITICAL naar FATAL; een mislukte herstelpoging telt als fout).
- (2) Twee assen: de code GB400 tot GB410 zegt WAT, het foutniveau HOE ERNSTIG. De teksten volgen de bestaande werkwijze, zonder niveau in de tekst.
- (3) Herstel = de bestaande `activeren()`, niet opnieuw inpluggen; daarna alle onderliggende nodes opnieuw `activeren()` (een node die niet herstelt: eigen GB401, blijft uit); bij het uitschakelen alle onderliggende nodes `actief = false`.
- (4) Busherstel nu: SDA bleef hangen in de test. Eerst SDA lezen met Wire actief; alleen als SDA laag is: `Wire.end()`, tot 9 klokpulsen, STOP-conditie, opnieuw initialiseren.
- (5) Wire-timeout 25 ms op AVR en UNO R4.
- (6) Melding met vaste tekst (code + I2C-adres, eventueel kanaal), zonder componentnaam (die kost tot +2660 bytes flash op R4) en zonder `snprintf` (±13 KB op R4); ook IN402 bij de opstart gebruikt geen `snprintf` meer.
- (7) Leestijd per niveau, apart voor opstart (GENERAL) en runtime (RUNTIME); `FATAL_LEESTIJD_MS` verdwijnt. Tijdens runtime standaard 0, ook voor FATAL; WARNING tot FAULT alleen met DEBUG of TRACE.
- (8) Overrides per type, per exemplaar, per Sensor en per kanaal, elk als één define met de hele set (K1 keuze 2). Eén effectieve ladder per meting: kanaal, anders Sensor, anders de ladder van de Extender (exemplaar, type, algemeen); nooit twee ladders voor dezelfde meting. Vanaf het uitschakelniveau gaat de Extender uit, met alle kanalen erop (herstel gebeurt op chipniveau); een tolerant kanaal beschermt de chip dus niet tegen een strenger kanaal.
- (9) Het LCD test zelf vóór elke melding, omdat `LiquidCrystal_I2C` geen foutcode teruggeeft.
- (10) `ScreenData` is altijd beschikbaar, ook zonder pixelscherm.
- (11) Eén functionele handeling = één resultaat (keypadscan, sensormeting, Extender-pinread/-write, LCD-melding), niet elke library-oproep: anders wist een geslaagde write een mislukte read en blijft de ladder op fout 1.
- (12) Herstel alleen bij een volgende toegang, nooit vaker dan de pauze; geen herstel op eigen initiatief. GB407 = dit component zelf hersteld. Een node eronder waarvan de parent niet actief is: geen eigen melding; eigen fout: eigen ladder (I2C) of, zonder eigen toestand, het foutniveau van de herstellende I2C-parent (zijn toestand wordt pas na het heractiveren van de nodes eronder gereset, altijd; nagekeken: geen `Activeren()` raakt de resultaatfunctie van de parent). Elke sensor die runtime uitgeschakeld kan worden, heeft een eigen herstelpad nodig.
- (13) Blijft SDA laag na het busherstel: GB409, geen `activeren()` in die poging.
- (13b) GB410 = de functionele meting (Sensor- of kanaalladder) is hersteld, niet noodzakelijk dat de onderliggende Extender net opnieuw geactiveerd is (dat meldt GB407). Paren: GB400/GB407 (communicatie), GB408/GB410 (meting). Zonder GB410 gaf een Sensor-override zonder kanaal exact dezelfde herstelmelding als de ADS1115 zelf.
- (13c) Schakelt een Sensor- of kanaalladder de Extender uit, dan wordt de toestand samengevoegd met die van de Extender: hoogste foutniveau, hoogste aantal fouten (niet optellen: zelfde storing), oudste eerste fout (als duur vergeleken, juist na de overloop van `millis()`). Zonder eigen historiek neemt de Extender de toestand over. Volgorde: eerst samenvoegen, dan het foutniveau begrenzen op het maximum van de Extender (aantal en oudste tijd onaangeroerd); de Extender herstelt daarna volgens zijn eigen drempels. Na dat chipherstel komt alleen GB407, geen GB410.
- (13d) Staat het uitschakelniveau van de Extender boven zijn maximaal toegelaten foutniveau, dan kan geen onderliggende Sensor- of kanaalladder die Extender automatisch uitschakelen of het daaraan gekoppelde herstelpad starten (alleen GB408). Gevolg: zolang de storing duurt, kost elke toegang de Wire-timeout en is er geen busherstel. "Uitschakelen" betekent hier twee dingen tegelijk: niet meer operationeel en het herstel starten. Herstellen zonder uitschakelen is een apart concept voor later, geen uitzondering op deze instelling.
- (14) Drempels worden bij het compileren gecontroleerd (aantallen oplopend; tijden die niet 0 zijn oplopend als één reeks, ook over een 0 heen; foutniveaus WARNING tot FATAL). Uitschakelniveau boven het hoogst toegelaten foutniveau = wel melden, nooit automatisch uitschakelen.
- (15) Het runtime-herstel veronderstelt sequentiële toegang (geen interrupts of callbacks).
- (16) Niet in v2.0.0: FRAM en EEPROM via I2C (Data gebruikt ze tijdens runtime nog niet), Extenders zonder runtime-functies, SPI.

**Context:** hardwaretest 18-10-2026: een los contact zette de PCF8574 van Input meteen en voor altijd in FATAL; SDA bleef hangen. De maatstaf blijft maximale snelheid en minimaal geheugen: in normaal gebruik kost het één vergelijking per toegang.
