# Changelog

Alle betekenisvolle wijzigingen aan GroeiAcademie FrameWork worden in dit bestand bijgehouden.

De versienummers volgen de versie in `library.properties`.

## 1.0.4

### Screen-laag — CharacterScreen-configuratie, symmetrisch met PixelScreen

- **Nieuw:** `CharacterScreenConfigureren()` — doet een echte I2C-handdruk vóór `lcd.init()`/`lcd.backlight()`, symmetrisch met de bestaande `PixelScreenConfigureren()`. Vervangt de rechtstreekse `lcd.init(); lcd.backlight();`-aanroepen in alle betrokken voorbeelden;
- **Nieuw:** `ScreensConfigureren()` — optionele gemakslaag die, aan de hand van `SCREEN_OUTPUT`, automatisch enkel de nodige configuratiefunctie(s) aanroept. Geen deprecatie van `CharacterScreenConfigureren()`/`PixelScreenConfigureren()`, die blijven de granulaire, expliciete route (zie `docs/DECISION_LOG.md`, D022);
- **Nieuw:** `CHARACTERSCREEN_I2C_ADRES_MODUS` (0/1/2, standaard `1`) — laat `CharacterScreenConfigureren()` een kort lijstje bekende I2C-adressen aftasten wanneer het geconfigureerde `I2C_ADRES` niet reageert, met per modus een ander gevolg (geen scan / scan + rapporteren / scan + automatisch herbouwen). Zie `docs/DECISION_LOG.md`, D023, en de impact-tabel in `UserConfig_template.h`;
- **Breaking change, bewust aanvaard:** de impliciete auto-configuratie van het pixelscherm (`if (pixelScreenGeselecteerd && pixelScreenStatus.pixelScreenActief) PixelScreenConfigureren();`) is volledig verwijderd, zonder fallback. Een sketch die vandaag werkt zonder expliciete `PixelScreenConfigureren()`-aanroep, werkt na deze update niet meer. Aanvaard omdat er nog geen gekende externe gebruikers zijn (zelfde redenering als D020);
- `PrintToScreenIntern()` toont voortaan, maximaal één keer, een duidelijke foutmelding (`CS000`/`PS000` bij een vergeten configuratie-aanroep, de opgeslagen foutcode bij een mislukte configuratie) in plaats van de aanroep stil te laten mislukken;
- nieuwe foutcodes `_FATAL_CS000`, `_FATAL_CS001`, `_FATAL_CS002` en `_FATAL_PS000` toegevoegd aan de vier taalbestanden; bestaande `_FATAL_PS001`–`_FATAL_PS004` ongewijzigd;
- `PixelScreenFoutmeldingWeergeven()` gebruikt voortaan enkel een reeds geconfigureerd en werkend characterscherm als terugvalpad (voorheen onvoorwaardelijk); nieuwe, symmetrische `CharacterScreenFoutmeldingWeergeven()` doet hetzelfde in de andere richting;
- **Vereist een volledige nieuwe testronde** (niet overgenomen van v1.0.0): dit wijzigt echt gedrag, in tegenstelling tot v1.0.2/v1.0.3. Resultaten volgen in `extras/TESTRESULTATEN.md` zodra de compilatiematrix op echte hardware is uitgevoerd — inclusief empirische bevestiging van de placement-new (modus 2) op AVR (UNO R3);
- **Bugfix tijdens eigen validatie ontdekt (vóór publicatie):** `PixelScreenConfigureren()` zette `pixelScreenActief` niet op `true` bij een geslaagde configuratie, waardoor `PrintToScreenIntern()` een geslaagde pixelscherm-configuratie als mislukt beschouwde. Bij CHARACTER+PIXELS samen leidde dit tot een lege foutmelding op het characterscherm en geen enkele uitvoer op het pixelscherm. Gecorrigeerd vóór release;
- `docs/Systeem/PIXELSCREEN_FOUTCODES.md` hernoemd naar `docs/Systeem/SCREEN_FOUTCODES.md` en aangevuld met `CS000`, `CS001`, `CS002` en `PS000` — voorheen enkel PS001–PS004 gedocumenteerd, waardoor de nieuwe CharacterScreen-foutcodes nergens terug te vinden waren voor wie de "ZOEK DIT NU OP"-instructie opvolgde;
- **Naar aanleiding van code review, vóór publicatie:** het configuratiecontroleblok in `PrintToScreenIntern()` geldt voortaan enkel voor de ingebouwde hardware — is er een callback geregistreerd, dan wordt die niet langer geblokkeerd wanneer de ingebouwde hardware niet geconfigureerd is (zie `docs/DECISION_LOG.md`, D024);
- **Naar aanleiding van code review, vóór publicatie:** `PixelScreenPrint()` tekent niet langer buiten het berekende grid — tekst die niet meer past binnen de resterende kolommen/regels wordt afgekapt in plaats van zichtbaar buiten het grid getekend; `cursorRegel` wordt begrensd zodat opeenvolgende te lange teksten nooit onder het grid kunnen belanden;
- **Nieuw:** `CharacterScreenConfigureren()`, `PixelScreenConfigureren()` en `ScreensConfigureren()` krijgen een optionele `opnieuwProberen`-parameter (standaard `false`, dus geen enkel bestaand voorbeeld hoeft aangepast). Enkel bij expliciet `true` wordt een eerder gecontroleerde configuratie opnieuw geprobeerd — nuttig na een fysieke aansluiting tijdens het draaien, zonder reset van de microcontroller. Niet bedoeld om automatisch (bv. in `loop()`) aan te roepen: elke poging doet een echte I2C-transactie.

## 1.0.3

### Documentatie

- terminologie consequent gemaakt: "board"/"moederbord" vervangen door de volledige, exacte term "Arduino Uno R3-vormfactorbord(en)" doorheen de documentatie, met uitzondering van Arduino's eigen productterminologie (`Arduino Boards Manager`, `boardprofiel`) en de al bestaande vaste samenstelling "ESP32-borden in Arduino Uno R3-vormfactor";
- `SPONSORS.md`: nieuwe kandidaat-Arduino Uno R3-vormfactorborden toegevoegd (Cytron Maker Uno RP2040, SB Components Ardi32, STM32 Nucleo-F401RE);
- `docs/ROADMAP.md`: statustabellen toegevoegd voor Arduino Uno R3-vormfactorborden en sensoren, met verwijzing naar `SPONSORS.md`;
- `docs/Toepassingsgebieden/SENSOR_INVENTARIS.md`: piëzo-ademhalingsband toegevoegd; referentie-/validatie-apparaten (NeuroSky MindWave Mobile 2, Mindfield eSense, MindFlex) apart van kandidaat-sensoren vermeld;
- `docs/HARDWARE_SUPPORT.md`: tabel met interne ADC-resolutie per Arduino Uno R3-vormfactorbord toegevoegd;
- geen codewijziging: gebruikt ongewijzigd de codebasis van v1.0.2.

## 1.0.2

### Screen- en Stimulus-laag — API-oppervlak verkleind

- twaalf functies die uitsluitend intern als bouwsteen dienden, zijn niet langer publiek gedeclareerd (interne/`static` zichtbaarheid): `BepaalAantalSensorenSynchroon()`, `BerekenEindStimulus()`, `InitialiseerSensorStart()`, `MaakSensorMask()`, `MaakSynchronisatieProfielAlleSensoren()`, `ResetStimulusProfiel()`, `ResetSynchronisatieProfiel()`, `VerwerkSensor()`, `PixelScreenClear()`, `PixelScreenSetCursor()`, `PixelScreenPrint()` en `PixelScreenFoutmeldingWeergeven()`;
- geen van deze wijzigingen raakt een bestaand voorbeeld: alle twaalf functies werden al uitsluitend intern gebruikt, nooit door een `.ino`;
- `MaakSynchronisatieProfiel()` blijft bewust publiek, als voorbereide bouwsteen voor toekomstige paarsgewijze synchronisatie (zie `docs/DECISION_LOG.md`, D020);
- geen functionele of gedragswijziging: enkel de zichtbaarheid van deze functies is aangepast;
- dit was een bewuste, eenmalige correctie van het te ruime v1.0.0-API-oppervlak, uitgevoerd terwijl er nog geen gekende externe gebruikers waren.

## 1.0.1

### Documentatie

- documentatie onderling afgestemd op de actuele v1.0.1-status;
- foutieve compilatiecijfers gecorrigeerd naar 188 uitgevoerde compilaties, 184 succesvolle compilaties, 4 gekende UNO R3-geheugenbeperkingen en 0 onverwachte compilatiefouten;
- boardprofiel `esp32:esp32:d1_uno32` verduidelijkt voor WEMOS D1 R32, TTGO D1 R32 en compatibele ESP32-borden in Arduino Uno R3-vormfactor;
- WEMOS D1 R32 overal aangeduid als getest en goedgekeurd sinds v1.0.0;
- fysieke teststatus van TTGO D1 R32 en andere compatibele borden onderscheiden van het gedeelde boardprofiel;
- Arduino Library Manager-status bijgewerkt nadat de aanmelding werd geaccepteerd;
- achterhaalde v1.0.0-pre-releasepunten in de ROADMAP vervangen door de voltooide releasestatus;
- gebroken links in het rootbestand `GOVERNANCE.md` gecorrigeerd;
- `SENSOR_INVENTARIS.md` toegevoegd aan de architectuur- en documentatie-indexen;
- handmatige installatiepaden verduidelijkt per besturingssysteem en Arduino-sketchbooklocatie;
- projectnaam consequent geschreven als `GroeiAcademie FrameWork`;
- losse `#define`-regels in Markdown correct als code weergegeven.

## 1.0.0 — eerste publieke alpha-release

### Screen-laag

- `PrintToScreen()` uitgebreid tot vier tekstregels met `delayTussenPaginas`;
- `ScreenCallback` ontvangt de volledige schermopdracht;
- character- en pixelcallbacks worden elk maximaal één keer per `PrintToScreen()`-aanroep uitgevoerd;
- standaardondersteuning voor LCD1602, LCD1604, LCD2002, LCD2004 en LCD4002;
- documentatie van de Screen-laag;
- compile-time PixelScreen-ondersteuning via `SCREEN_TYPE_PIXELS`;
- algemene `Adafruit_GFX* PixelScreen`-koppeling toegevoegd, zodat de concrete displaydriver in het voorbeeld of de toepassing blijft;
- ondersteunde PixelScreen-resoluties en rotatie 0 tot en met 3 toegevoegd;
- standaard PixelScreen-uitvoer voor `ScreenData::TYPE_NONE` toegevoegd;
- afzonderlijke `CharacterScreenCallback` en `PixelScreenCallback` toegevoegd;
- `ScreenData` met informatie-, interactie-, fout-, diagnose- en inhoudstypen toegevoegd;
- `PixelScreenConfigureren()`, `PixelScreenClear()`, `PixelScreenSetCursor()`, `PixelScreenPrint()` en foutafhandeling toegevoegd;
- voorbeelden toegevoegd voor standaard CharacterScreen, standaard PixelScreen, gecombineerde standaarduitvoer en PixelScreen-callback;
- `SCREEN_OUTPUT` en de waarden 0 tot en met 7 gedocumenteerd;
- debuguitvoer beperkt tot builds waarin zowel `DEBUG` als `SCREEN_TYPE_SERIAL` actief zijn;
- alle relevante Markdownbestanden afgestemd op de nieuwe en hernoemde voorbeelden;
- `ScreenTypes.h` toegevoegd zodat schermtypes en schermresoluties vóór `SystemConfig.h` beschikbaar zijn;
- `delayTussenPaginas` wordt bij gelijktijdige standaarduitvoer naar CharacterScreen en PixelScreen slechts één keer uitgevoerd;
- de Background-hulpfunctie heet `PrintToScreenBackground()`;
- `SCREEN_OUTPUT_CONFIG` gebruikt `SCREEN_TYPE_NONE` als veilige standaard;
- voorbeeldcontroles gebruiken `SCREEN_OUTPUT_CONFIG` voor de expliciete gebruikerskeuze;

### Configuratie, boards en taal

- `UserConfig_template.h` bevat alle in deze release rechtstreeks instelbare gebruikersparameters en wordt als `UserConfig.h` vóór de fallbackwaarden uit `SystemConfig.h` geladen;
- `UNO_VERSION` vervangen door `BOARD_VERSION`, met afzonderlijke keuzewaarden voor UNO R3, UNO R4 Minima, UNO R4 WiFi en Wemos D1 R32;
- 12-bit ADC-schaling toegevoegd voor Wemos D1 R32; de Stimulusvoorbeelden passen `analogReadResolution(ADC_BITS)` nu toe bij 12 en 14 bits;
- `src/Configuratie/Examples.h` behoudt zijn naam en laadt per gekozen taal eerst een eventueel `UserExample_XX.h` en daarna `Examples_XX.h`;
- `UserExample_XX_template.h` en `UserLibrary_XX_template.h` zijn voor NL, DE, EN en FR toegevoegd onder `src/Language/`; actieve gebruikersbestanden blijven in dezelfde map;
- `UserLibrary_XX.h` wordt vóór `Library_XX.h` geladen; de officiële configuratiewaarden en taaldefines zijn afzonderlijk als fallback beschermd;
- vaste numerieke waarden voor `ADC_BACKEND_...` en `BOARD_...` zijn uit `UserConfig_template.h` verwijderd; de gebruiker kiest alleen de vaste namen;
- `DEBUG` en `TRACE` worden uitsluitend via `UserConfig.h` geactiveerd; `TRACE` activeert automatisch `DEBUG`;
- gewone examples stellen libraryconfiguratie niet rechtstreeks in de `.ino` in; persoonlijke instellingen worden vanuit `UserConfig.h` geladen en ontbrekende waarden vallen terug op `SystemConfig.h`; globale compilerdefinities kunnen voor gecontroleerde builds vóór deze configuratie worden vastgelegd;
- `FATAL_ZOEK_OP` is verplaatst van `Screen.h` naar de vier `Library_XX.h`-bestanden en de vier `UserLibrary_XX_template.h`-bestanden;
- status van UNO R3, UNO R4 Minima en UNO R4 WiFi verduidelijkt;
- Wemos D1 R32-pintoewijzing toegevoegd aan `ADC_Validatie_Native`;

### Stimulus en ADC

- `ADC_BACKEND` (`ADC_BACKEND_NATIVE` / `ADC_BACKEND_ADS1115`) toegevoegd in `SystemConfig.h`, naast de bestaande `BOARD_VERSION`-keuze;
- `ADC_BITS`/`DELAY_US` uitgebreid met een derde geval voor de ADS1115-backend, zodat bestaande drempel- en margeberekeningen (via de `ADC(x)`-macro) automatisch correct herschalen;
- nieuwe, backend-afhankelijke `RawAnalogRead()` in `Stimulus.cpp`, die alle drie bestaande directe `analogRead()`-aanroepen vervangt (`AnalogReadMetGekorigeerdeOffsets()`, `BepaalSensorOffsets()`, en de wegwerp-meting bij kanaalwissel in `MeetStimulus()`);
- instelbare `WACHT_LOSLATEN_DELAY_MS` in `WachtTotAlleSensorsLosgelatenVoorTest()`, om onafgebroken I2C-bevraging te vermijden wanneer `ADC_BACKEND_ADS1115` actief is;
- I2C-adres `0x48` gereserveerd voor de Stimulus-ADS1115, afgestemd op de geplande emotiemeetmodule (die `0x49`/`0x4A` zal gebruiken) om adresconflicten te vermijden bij gestapelde shields;
- twee validatiescripts toegevoegd: `ADC_Validatie_Native` en `ADC_Validatie_ADS1115`;
- samengesteld Stimulusvoorbeeld hernoemd naar `Tik_Enkele_Samen_Instortend_Cocktail`;
- TRACE-metingen gebruiken `RawAnalogRead()` en behouden daardoor de geselecteerde ADC-backend;
- ADS1115-adreskeuze in `ADC_Validatie_ADS1115` gecorrigeerd van H7 naar SW1;
- de verplichte dependencies in `library.properties` zijn aangevuld met Adafruit GFX Library, Adafruit ST7735 and ST7789 Library en Adafruit ADS1X15;

### Hardware en elektronische schema's

- documentatie voor elektronische schema's per toepassingsgebied toegevoegd;
- elektronisch schema en pinbezetting voor de ADS1115-variant van Stimulus toegevoegd onder `docs/Toepassingsgebieden/Stimulus/Hardware/`;
- hardwaredocumentatie voor H5, H6 en H7 toegevoegd;
- hardwaredocumentatie en schema uitgebreid met TFT-SPI, levelshifter en draadbruggen;
- een hardwarevalidatieprogramma toegevoegd voor de specifieke UNO R3/R4-shieldhardware;
- een validatieprotocol toegevoegd met testvolgorde en goedkeuringscriteria;
- hardwarevalidatie compileert optionele hardware en de bijbehorende libraries alleen wanneer de overeenkomstige validatiekeuze actief is;
- `Handleiding-GroeiAcademie-Stimulus-Hardware-Validatie-v1.0.0.md` toegevoegd;

### Tests en validatie

- compilatie gecontroleerd voor Arduino UNO R3, UNO R4 Minima en UNO R4 WiFi;
- Arduino LINT uitgevoerd zonder fouten, met één bekende onschadelijke waarschuwing.
- alle `.cmd`-testscripts werken vanuit de hoofdmap van de library, ook wanneer ze vanuit `extras` worden gestart;
- testscripts uitgebreid met Wemos D1 R32, schone sequentiële compilatie, Arduino LINT-status en afzonderlijke registratie van de vier gekende UNO R3-geheugenbeperkingen;

### Documentatie, structuur en licentie

- alle `*.md`-bestanden in lijn gebracht met de definitieve keuze van de licentie: GNU LGPL v3.0-or-later;
- `library.properties` gecorrigeerd naar `license=LGPL-3.0-or-later` (stond nog op GPL);
- `LICENSE.md` aangevuld met een paragraaf over de merknaam: "GroeiAcademie" valt als handelsnaam/merk niet onder de LGPL;
- verouderde verwijzingen naar `lib/` vervangen door de werkelijke Arduino-librarystructuur onder `src/`;
- gebroken interne links naar `STIMULUS.md` gecorrigeerd naar het werkelijke pad `Toepassingsgebieden/Stimulus/README.md`;
- huidige en geplande modules duidelijker van elkaar onderscheiden;
- installatie-, configuratie- en gebruiksinformatie uitgebreid;
- documentatieverwijzingen afgestemd op de werkelijk opgenomen v1.0.0-bestanden;
- achtergebleven `.bak`-bestanden zijn verwijderd.
