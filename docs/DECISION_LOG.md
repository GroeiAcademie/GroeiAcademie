# Decision Log

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

**Besluit:** functies die uitsluitend als bouwsteen dienen binnen één bronbestand van de library — en door geen enkel voorbeeld of extern bekende gebruiker rechtstreeks worden aangeroepen — krijgen interne (`static`) zichtbaarheid in plaats van een publiek prototype in de header. Dit geldt met terugwerkende kracht voor twaalf functies uit de Screen- en Stimulus-laag die abusievelijk publiek stonden: `BepaalAantalSensorenSynchroon()`, `BerekenEindStimulus()`, `InitialiseerSensorStart()`, `MaakSensorMask()`, `MaakSynchronisatieProfielAlleSensoren()`, `ResetStimulusProfiel()`, `ResetSynchronisatieProfiel()`, `VerwerkSensor()`, `PixelScreenClear()`, `PixelScreenSetCursor()`, `PixelScreenPrint()` en `PixelScreenFoutmeldingWeergeven()`. `MaakSynchronisatieProfiel()` blijft bewust extern staan, ondanks dat ze momenteel nergens gebruikt wordt, als voorbereide bouwsteen voor toekomstige paarsgewijze synchronisatie.

**Context:** dit wordt uitgevoerd terwijl v1.0.0/v1.0.1 nog maar zeer kort in de Library Manager staan en er geen gekende externe gebruikers zijn — de kost van deze correctie neemt met de tijd toe onder het API-stabiliteitsbeleid (D019).

## D021 Terminologie: "board" vs. "Arduino Uno R3-vormfactorbord"

**Besluit:** doorheen de documentatie wordt de generieke term "board"/"moederbord" vervangen door de volledige, exacte term "Arduino Uno R3-vormfactorbord(en)" wanneer een fysiek bord bedoeld wordt. Uitzonderingen: Arduino's eigen productterminologie (`Arduino Boards Manager`, `boardprofiel`) blijft ongewijzigd, evenals de al bestaande vaste samenstelling "ESP32-borden in Arduino Uno R3-vormfactor" en letterlijke citaten van compiler-uitvoer. Historische titels in dit logboek (bv. D006) worden niet met terugwerkende kracht herschreven.

**Context:** met v1.0.3 voor het eerst consequent doorgevoerd over de volledige documentatie.

## D022 Screen-configuratie: verplicht en expliciet, geen impliciete auto-configuratie

**Besluit:** `CharacterScreenConfigureren()` wordt toegevoegd, symmetrisch met de bestaande `PixelScreenConfigureren()` — beide doen een echte hardwarecontrole (I2C-handdruk resp. resolutiecontrole), bewaren enkel hun eigen status en foutcode, en tonen zelf niets. `PrintToScreenIntern()` controleert voortaan vooraan of de vereiste configuratiefunctie is aangeroepen en toont, maximaal één keer, de gepaste foutmelding (`CS000`/`PS000` bij een vergeten aanroep, de opgeslagen foutcode bij een mislukte configuratie) — daarna stopt die aanroep. De bestaande impliciete auto-configuratie van `PixelScreen` (`if (pixelScreenGeselecteerd && pixelScreenStatus.pixelScreenActief) PixelScreenConfigureren();`) wordt volledig verwijderd, zonder fallback. `ScreensConfigureren()` wordt toegevoegd als optionele gemakslaag die, aan de hand van `SCREEN_OUTPUT`, enkel de nodige configuratiefunctie(s) aanroept — de losse `CharacterScreenConfigureren()`/`PixelScreenConfigureren()` blijven gewoon publiek en worden niet gedeprecieerd, want `PixelScreenConfigureren()` wordt al in 8 van de 12 voorbeelden rechtstreeks gebruikt.

**Gevolg, bewust aanvaard:** een sketch die vandaag werkt zonder expliciete `PixelScreenConfigureren()`-aanroep (steunend op de verwijderde impliciete auto-configuratie) werkt na deze wijziging niet meer — dit is een bewuste breaking change.

**Context:** dezelfde redenering als D020 — nog geen gekende externe gebruikers, dus nu bijna kosteloos; later zou dit een verplichte deprecatiecyclus vereisen (D019).

## D023 CHARACTERSCREEN_I2C_ADRES_MODUS

**Besluit:** `CharacterScreenConfigureren()` krijgt een instelbare `CHARACTERSCREEN_I2C_ADRES_MODUS` (default `1`) om om te gaan met I2C-characterschermen waarvan het adres niet vooraf gekend is:

- **0** — geen scan, enkel de bestaande handdruk-check op het geconfigureerde `I2C_ADRES`. Kleinste footprint; voor boards die tegen hun geheugengrens zitten (bv. gekende UNO R3-beperkingen, zie `extras/TESTRESULTATEN.md`).
- **1** (standaard) — scan van een korte kandidatenlijst (`I2C_ADRES`, dan `0x27`, `0x3F`) en rapporteren via de foutmelding (`CS002`, met het gevonden adres erin), geen zelfherstel. Voor normaal gebruik en overal waar reproduceerbaarheid/betrouwbaarheid telt (onderzoek, metingen, meerdere I2C-apparaten op de bus).
- **2** — scan + automatisch herbouwen van het `lcd`-object op het gevonden adres via placement-new (`lcd.~LiquidCrystal_I2C(); new (&lcd) LiquidCrystal_I2C(...);`), zodat `lcd.`-aanroepen in bestaande voorbeelden en gebruikerscode ongewijzigd blijven werken. Nooit meer hercompileren bij een ander scherm/adres. Enkel voor actief ontwikkelen met wisselende schermen, geen ander I2C-apparaat op de bus, geen lopende metingen — de scan controleert enkel of een apparaat op dat adres reageert (ACK), niet of het effectief een LCD-scherm is.

**Overwogen en verworpen:** een generieke `BETATESTER`-vlag die "alles wat nog niet in alpha zit" zou aanzetten — verworpen omdat dit een tweede, parallel classificatiesysteem naast de bestaande ondersteuningsniveaus (D018) zou zetten voor precies één concreet experiment. Bewaard als aantekening in `docs/ROADMAP.md` voor wanneer een tweede, onafhankelijk experiment opduikt. Hergebruik van `DEBUG` voor dit doel werd eveneens verworpen: `DEBUG` betekent vandaag uitsluitend extra Serial-logging, en een DEBUG-build zou daardoor een ander Screen-gedrag krijgen dan de release-build — precies het omgekeerde van wat een DEBUG-build hoort te doen.

**Context:** aanleiding was een concrete situatie (LCD2004-schermen met onbekend/wisselend I2C-adres, wens om niet telkens te moeten hercompileren).

## D024 Configuratiecontrole geldt enkel voor de ingebouwde hardware

**Besluit:** het configuratiecontroleblok in `PrintToScreenIntern()` (dat `CS000`/`PS000` toont bij een vergeten configuratie-aanroep) geldt enkel wanneer er geen callback geregistreerd is voor dat schermtype. Is er een `CallbackScreenTypeCharacter`/`CallbackScreenTypePixel` geregistreerd, dan wordt die callback niet langer geblokkeerd wanneer de ingebouwde hardware niet geconfigureerd is — de callback-gebruiker blijft zelf verantwoordelijk voor het al dan niet configureren van de ingebouwde hardware, indien die callback ze nog gebruikt.

**Context:** `SCREEN_TYPE_CHARACTER`/`SCREEN_TYPE_PIXELS` vertegenwoordigen zo consequent "er is uitvoer van dit type gewenst", niet specifiek "de ingebouwde hardware moet werken" — in lijn met hoe de rest van `PrintToScreenIntern()` een callback al als volledig apart pad behandelde. Naar aanleiding van code review, vóór publicatie van v1.0.4.
