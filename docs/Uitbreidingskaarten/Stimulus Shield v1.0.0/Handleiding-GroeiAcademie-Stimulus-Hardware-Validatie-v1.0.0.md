******************************************************************************
1. Inleiding
******************************************************************************

==============================================================================
1.1) Doel van dit document
==============================================================================

Dit document beschrijft de volledige hardware-inventarisatie, automatische hardwaredetectie en hardwarevalidatie van het GroeiAcademie Stimulus Hardware Shield.

Deze handleiding begeleidt de gebruiker stap voor stap bij het inventariseren, aansluiten en valideren van het GroeiAcademie Stimulus Hardware Shield.

Het doel is een reproduceerbare en betrouwbare hardwarevalidatie uit te voeren waarbij de gebruiker uitsluitend gevraagd wordt wat de software niet zelf betrouwbaar kan vaststellen.

==============================================================================
1.2) Doelgroep
==============================================================================

Dit document is bedoeld voor:

• ontwikkelaars van de GroeiAcademie Stimulus Software;

• ontwikkelaars van het GroeiAcademie Stimulus Hardware Shield;

• testers;

• personen die een nieuw GroeiAcademie Stimulus Hardware Shield assembleren;

• personen die wijzigingen aan het GroeiAcademie Stimulus Hardware Shield uitvoeren;

• toekomstige ontwikkelaars van nieuwe versies van het GroeiAcademie Stimulus Hardware Shield.

==============================================================================
1.3) Uitgangspunten
==============================================================================

Tijdens de hardwarevalidatie worden volgende uitgangspunten toegepast.

De software detecteert automatisch alles wat betrouwbaar softwarematig vastgesteld kan worden.

De gebruiker wordt uitsluitend gevraagd informatie te bevestigen die software niet betrouwbaar kan bepalen.

Tijdens één volledige hardwarevalidatie wordt de hardwareconfiguratie éénmaal geïnventariseerd.

==============================================================================
1.4) Bijhorende documenten
==============================================================================

Dit document maakt deel uit van de documentatie van het GroeiAcademie Stimulus Hardware Shield.

Bijhorende documenten zijn onder andere:

• het elektrisch schema;

• de PCB-layout;

• de stuklijst (BOM);

• de Arduino-validatiesoftware;

• de technische documentatie van de ondersteunde hardwaremodules.

==============================================================================
1.5) Bijhorende software
==============================================================================

De hardwarevalidatie wordt uitgevoerd met de officiële validatiesoftware van het GroeiAcademie FrameWork.

De software bepaalt automatisch welke hardware aanwezig is, welke hardware ondersteund wordt en welke validatiestappen uitgevoerd moeten worden.

==============================================================================
1.6) Ondersteunde Arduino Uno R3-vormfactorborden
==============================================================================

Tijdens versie v1.0.0 worden volgende Arduino Uno R3-vormfactorborden ondersteund:

• Arduino UNO R3;

• Arduino UNO R4 Minima;

• Arduino UNO R4 WiFi;

• WEMOS D1 R32 (ESP32), getest en goedgekeurd sinds v1.0.0.
• TTGO D1 R32 en compatibele ESP32-borden in Arduino Uno R3-vormfactor gebruiken hetzelfde boardprofiel; hun fysieke hardwarevalidatie wordt afzonderlijk vastgelegd.

Ondersteuning voor bijkomende Arduino Uno R3-vormfactorborden kan in latere versies toegevoegd worden.

==============================================================================
1.6.1) Arduino IDE voorbereiden voor ESP32-borden in Arduino Uno R3-vormfactor
==============================================================================

Installeer via Arduino Boards Manager het boardpakket `esp32 by Espressif Systems`.

Selecteer daarna in Arduino IDE het boardprofiel `WEMOS D1 R32`.

De compilatietests gebruiken hiervoor FQBN `esp32:esp32:d1_uno32`. Dit profiel geldt voor WEMOS D1 R32, TTGO D1 R32 en compatibele ESP32-borden in Arduino Uno R3-vormfactor en verzorgt de juiste omzetting van Arduino-pinnamen zoals `A0` naar de overeenkomstige ESP32-GPIO-pinnen.

Een toolpakket zoals `esp32:esp-rv32@2601` wordt automatisch met het ESP32-boardpakket geïnstalleerd en wordt niet als Arduino Uno R3-vormfactorbord geselecteerd.

==============================================================================
1.7) Ondersteunde hardware
==============================================================================

De hardwarevalidatie ondersteunt onder andere:

• FSR402/RFP602-druksensoren;

• ADS1115-module;

• CharacterScreens LCD 1602/1604/2002/2004/4002 via I²C;

• PixelScreens GMT020.02.7P v1.3 of later via SPI;

• 1x4 Keymatrix;

• Quad Logic Level Shifters;

• de connectoren, jumpers en draadbruggen van het GroeiAcademie Stimulus Hardware Shield.

==============================================================================
1.8) Opbouw van dit document
==============================================================================

Dit document bestaat uit drie opeenvolgende rondes.

Ronde 1: Hardware-inventarisatie.

Het GroeiAcademie Stimulus Hardware Shield ligt spanningsloos op tafel naast het Arduino Uno R3-vormfactorbord.

Alle visuele inventarisatie en fysieke controles worden uitgevoerd.

Ronde 2: Automatische hardwarevalidatie.

Het GroeiAcademie Stimulus Hardware Shield wordt op het Arduino Uno R3-vormfactorbord geplaatst en krijgt spanning.

De software voert automatische testen uit zonder gebruikersinteractie.

Deze ronde omvat zowel de automatische hardwaredetectie uit hoofdstuk 4 als de hardwarevalidatie zonder gebruikersinteractie uit hoofdstuk 5.

Ronde 3: Hardwarevalidatie met gebruikersinteractie.

Het GroeiAcademie Stimulus Hardware Shield blijft onder spanning.

De software voert testen uit waarvoor gebruikersinteractie nodig is.

Iedere ronde bouwt verder op de resultaten van de vorige ronde.

Hierdoor wordt uitsluitend de hardware getest die werkelijk aanwezig is.

==============================================================================
1.9) Versiebeheer
==============================================================================

Dit document volgt de ontwikkeling van het GroeiAcademie Stimulus Hardware Shield.

Bij iedere nieuwe hardwareversie wordt nagegaan welke onderdelen gewijzigd werden en welke validatiestappen aangepast moeten worden.

Hierdoor blijft de hardwarevalidatie volledig afgestemd op de overeenkomstige hardwareversie.

==============================================================================
1.10) Terminologie
==============================================================================

In dit document worden volgende begrippen gebruikt.

**Hardware-inventarisatie**

Het vaststellen welke hardware aanwezig is en hoe deze geconfigureerd werd.

**Automatische hardwaredetectie**

Het automatisch controleren welke hardware door de software bereikt kan worden.

**Hardwarevalidatie**

Het controleren of de aanwezige hardware correct functioneert.

**Gebruiker**

De persoon die de hardware assembleert, configureert of valideert.

**GroeiAcademie Stimulus Hardware Shield**

Het GroeiAcademie Stimulus Hardware Shield waarvoor dit document opgesteld werd.

******************************************************************************
2. Hardware-inventarisatie
******************************************************************************

==============================================================================
2.1) Doel
==============================================================================

Het doel van de hardware-inventarisatie is om vóór het GroeiAcademie Stimulus Hardware Shield op de
Arduino geplaatst wordt, automatisch vast te stellen welke hardware aanwezig
is, hoe deze hardware opgebouwd is en welke configuratie gebruikt wordt.

Het GroeiAcademie Stimulus Hardware Shield v1.0.0 werd bewust modulair ontworpen. Niet iedere
gebruiker zal alle uitbreidingen monteren. Sommige toepassingen vereisen
slechts twee FSR402/RFP602-sensoren, andere vier. Sommigen gebruiken een
ADS1115-module, anderen sluiten de sensoren rechtstreeks aan op de analoge
ingangen van de Arduino. Er kan gekozen worden voor een CharacterScreen, een
PixelScreen, beide schermtypes of zelfs geen scherm.

Door de aanwezige hardware vooraf te inventariseren kan de software zich
automatisch aanpassen aan de werkelijk aanwezige configuratie.

De hardware-inventarisatie bepaalt onder andere:

• welke hardware aanwezig is;
• welke hardware ontbreekt;
• welke configuratie gekozen werd;
• welke hardware automatisch getest kan worden;
• welke controles enkel visueel uitgevoerd kunnen worden;
• welke softwarecomponenten geactiveerd moeten worden;
• welke softwarecomponenten uitgeschakeld mogen blijven;
• welke bibliotheken noodzakelijk zijn;
• welke hardwaretesten later uitgevoerd moeten worden;
• welke hardwaretesten automatisch overgeslagen kunnen worden.

De hardware-inventarisatie vormt daardoor de basis voor alle verdere testen.

==============================================================================
2.2) Algemene werkwijze
==============================================================================

De hardware-inventarisatie wordt uitgevoerd vóór de eigenlijke
hardwarevalidatie.

Tijdens deze fase wordt nog niet getest of de hardware correct werkt.
Er wordt uitsluitend vastgesteld welke hardware aanwezig is en hoe deze
hardware geconfigureerd werd.

Omdat niet alle onderdelen softwarematig gedetecteerd kunnen worden,
combineert de inventarisatie automatische detectie met manuele controles.

Wanneer een controle niet automatisch uitgevoerd kan worden,
wordt de gebruiker stap voor stap begeleid.

Iedere vraag beschrijft daarom zo concreet mogelijk:

• welk onderdeel gecontroleerd moet worden;
• op welke connector het onderdeel aangesloten moet zijn;
• welke oriëntatie gecontroleerd moet worden;
• welke jumpers geplaatst moeten zijn;
• welke draadbruggen aanwezig moeten zijn;
• welke instellingen later automatisch gebruikt zullen worden.

Waar mogelijk wordt slechts één eenvoudige vraag tegelijk gesteld.

Na iedere bevestiging wordt de inventarisatie onmiddellijk bijgewerkt zodat
de volgende vragen automatisch aangepast kunnen worden aan de reeds gekende
hardwareconfiguratie.

******************************************************************************
3. Inventarisatie
******************************************************************************

==============================================================================
3.1) Controleren welk Arduino Uno R3-vormfactorbord gebruikt wordt
==============================================================================

Doel

Het GroeiAcademie Stimulus Hardware Shield ondersteunt meerdere Arduino-compatibele Arduino Uno R3-vormfactorborden. Het gekozen Arduino Uno R3-vormfactorbord bepaalt onder andere de beschikbare ADC, de werkspanning van het FSR402/RFP602-circuit, de softwareconfiguratie en welke hardwaretesten automatisch uitgevoerd kunnen worden.

Tijdens versie v1.0.0 worden volgende Arduino Uno R3-vormfactorborden ondersteund:

• Arduino UNO R3
• Arduino UNO R4 Minima
• Arduino UNO R4 WiFi
• WEMOS D1 R32 (ESP32), getest en goedgekeurd sinds v1.0.0

------------------------------------------------------------------------------

Gebruikersvraag

Welk Arduino Uno R3-vormfactorbord wordt gebruikt?

□ Arduino UNO R3
□ Arduino UNO R4 Minima
□ Arduino UNO R4 WiFi
□ WEMOS D1 R32 (ESP32)

------------------------------------------------------------------------------

==============================================================================
3.2) Inventariseren van het GroeiAcademie Stimulus Hardware Shield v1.0.0
==============================================================================

------------------------------------------------------------------------------
3.2.1) Controleren van de 1x4 Keymatrix
------------------------------------------------------------------------------

Doel

De 1x4 Keymatrix vormt de primaire gebruikersinterface tijdens de hardwarevalidatie.

Via deze vier druktoetsen kan de gebruiker keuzes maken, vragen beantwoorden, testen starten of afbreken en door de verschillende validatiestappen navigeren.

Omdat de verdere hardwarevalidatie afhankelijk is van gebruikersinteractie, wordt eerst gecontroleerd of de Keymatrix aanwezig is en correct aangesloten werd.

Visuele controle

Controleer of de 1x4 Keymatrix aangesloten is op connector H1.

Controleer vervolgens de oriëntatie van de connector.

Pin 5 van connector H1 moet verbonden zijn met GND.

Controleer eveneens dat de overige vier aansluitingen in de juiste volgorde verbonden zijn met de vier druktoetsen.

Gebruikersvraag

Is de 1x4 Keymatrix aangesloten op connector H1?

□ Ja
□ Nee

Indien antwoord = Ja

Controleer dat:

• de connector volledig ingestoken is;
• pin 5 met GND verbonden is;
• de vier overige aansluitingen overeenkomen met de vier druktoetsen;
• de kabel niet verdraaid aangesloten werd.

Indien antwoord = Nee

Het GroeiAcademie Stimulus Hardware Shield beschikt tijdens de hardwarevalidatie niet over een lokale gebruikersbediening.

Alle verdere gebruikersvragen worden uitgevoerd via de seriële monitor.

Resultaat

Deze informatie bepaalt later:

• of de automatische toetsentest uitgevoerd wordt;
• of gebruikerskeuzes via de Keymatrix mogelijk zijn;
• of alternatieve invoermethoden gebruikt moeten worden.

------------------------------------------------------------------------------
3.2.2) Controleren van de sensorconfiguratie FSR402/RFP602
------------------------------------------------------------------------------

Doel

Het GroeiAcademie Stimulus Hardware Shield ondersteunt maximaal vier FSR402/RFP602-druksensoren.

Afhankelijk van de toepassing kunnen geen, twee of vier sensoren aangesloten worden.

De verdere hardwarevalidatie moet weten hoeveel sensoren werkelijk aanwezig zijn zodat uitsluitend de relevante sensoren getest en gekalibreerd worden.

Visuele controle

Controleer hoeveel FSR402/RFP602-sensoren aangesloten zijn.

De sensoren worden aangesloten op:

• U4 : Sensor 1
• U5 : Sensor 2
• U6 : Sensor 3
• U7 : Sensor 4

Wanneer twee sensoren gebruikt worden, moeten deze aangesloten zijn op U4 en U5.

Connector U6 en U7 blijven dan leeg.

Wanneer vier sensoren gebruikt worden, moeten alle vier de connectoren (U4, U5, U6 en U7) bezet zijn.

Gebruikersvraag

Hoeveel FSR402/RFP602-sensoren zijn aangesloten?

□ Geen
□ Twee
□ Vier

Indien antwoord = Geen

Er zijn geen FSR402/RFP602-sensoren aangesloten.

De FSR402/RFP602-sensoren vormen de basis voor de werking van het GroeiAcademie Stimulus Hardware Shield.

Gebruikersvraag

Welke actie wil je uitvoeren?

□ Twee FSR402/RFP602-sensoren aansluiten
□ Vier FSR402/RFP602-sensoren aansluiten
□ De initialisering verlaten

Indien antwoord = Twee FSR402/RFP602-sensoren aansluiten

Sluit Sensor 1 aan op U4 en Sensor 2 op U5.

Voer daarna 3.2.2) opnieuw uit.

Indien antwoord = Vier FSR402/RFP602-sensoren aansluiten

Sluit Sensor 1 aan op U4, Sensor 2 op U5, Sensor 3 op U6 en Sensor 4 op U7.

Voer daarna 3.2.2) opnieuw uit.

Indien antwoord = De initialisering verlaten

De initialisering wordt beëindigd.

Indien antwoord = Twee

Controleer dat uitsluitend connector U4 en U5 gebruikt worden.

Controleer eveneens dat:

• Sensor 1 correct en stevig geklemd is in U4;
• Sensor 2 correct en stevig geklemd is in U5;
• U6 en U7 leeg zijn.

Tijdens de hardwarevalidatie worden uitsluitend Sensor 1 en Sensor 2 getest.

Indien antwoord = Vier

Controleer dat alle vier de connectoren correct aangesloten zijn.

Controleer eveneens dat:

• Sensor 1 correct en stevig geklemd is in U4;
• Sensor 2 correct en stevig geklemd is in U5;
• Sensor 3 correct en stevig geklemd is in U6;
• Sensor 4 correct en stevig geklemd is in U7.

Tijdens de hardwarevalidatie worden alle vier de sensoren afzonderlijk getest.

Resultaat

Na de bevestiging van twee of vier correct aangesloten FSR402/RFP602-sensoren wordt het aantal aanwezige sensoren gebruikt tijdens de verdere hardwarevalidatie.

Deze informatie bepaalt later:

• hoeveel sensoren getest worden;
• hoeveel kalibratiestappen uitgevoerd worden;
• hoeveel meetkanalen geactiveerd worden;
• welke foutmeldingen relevant zijn.

------------------------------------------------------------------------------
3.2.3) Controleren van de werkspanning van het FSR402/RFP602-circuit
------------------------------------------------------------------------------

Doel

Het GroeiAcademie Stimulus Hardware Shield ondersteunt zowel 5V- als 3V3-Arduino Uno R3-vormfactorborden.

De werkspanning van het FSR402/RFP602-circuit wordt geselecteerd met jumper H7.

Deze werkspanning bepaalt eveneens de voedingsspanning van een eventueel aanwezige ADS1115-module.

De gekozen spanning moet overeenkomen met het geselecteerde Arduino Uno R3-vormfactorbord en met alle aangesloten uitbreidingsmodules.

Visuele controle

Controleer de positie van de jumper op connector H7.

Er zijn drie mogelijke situaties:

• de jumper staat in de 5V-positie;
• de jumper staat in de 3V3-positie;
• er is geen jumper geplaatst.

Gebruikersvraag

In welke positie staat de jumper op connector H7?

□ 5V
□ 3V3
□ Geen jumper geplaatst

Indien antwoord = 5V

Controleer of deze instelling overeenkomt met het geselecteerde Arduino Uno R3-vormfactorbord.

Voor de Arduino UNO R3 en de Arduino UNO R4 Minima/WiFi is dit momenteel de standaardconfiguratie.

De werkspanning wordt geregistreerd als 5V.

Indien antwoord = 3V3

Controleer of het geselecteerde Arduino Uno R3-vormfactorbord met een logicaniveau van 3V3 werkt.

Voor WEMOS D1 R32, TTGO D1 R32 en compatibele ESP32-borden met boardprofiel `esp32:esp32:d1_uno32` is de werkspanning 3V3.

Controleer eveneens of alle aangesloten uitbreidingsmodules geschikt zijn voor gebruik met 3V3.

Wanneer één of meerdere modules uitsluitend met 5V mogen werken, moet de jumper opnieuw in de 5V-positie geplaatst worden alvorens de hardwarevalidatie verder kan gaan.

De werkspanning wordt geregistreerd als 3V3.

Indien antwoord = Geen jumper geplaatst

Het FSR402/RFP602-circuit ontvangt geen voedingsspanning.

Wanneer een ADS1115-module aanwezig is, ontvangt ook deze geen voedingsspanning.

Hierdoor kunnen geen betrouwbare sensormetingen uitgevoerd worden.

Plaats eerst de jumper in de gewenste positie alvorens verder te gaan.

Resultaat

Deze informatie wordt later gebruikt tijdens:

• de detectie van de ADS1115-module;
• de kalibratie van de FSR402/RFP602-sensoren;
• alle analoge metingen;
• de automatische hardwarevalidatie.

------------------------------------------------------------------------------
3.2.4) Vaststellen of de FSR402/RFP602-sensoren via een ADS1115-module of via directe ADC-metingen worden uitgelezen
------------------------------------------------------------------------------

Doel

Het GroeiAcademie Stimulus Hardware Shield ondersteunt twee verschillende meetconfiguraties voor de FSR402/RFP602-sensoren.

Configuratie 1

De sensoren worden uitgelezen via een ADS1115-module.

Hierbij worden alle sensormetingen uitgevoerd via de externe 16-bit analoog-naar-digitaalomzetter.

Configuratie 2

De sensoren worden rechtstreeks uitgelezen via de analoge ingangen van het geselecteerde Arduino Uno R3-vormfactorbord.

Slechts één van beide configuraties mag gelijktijdig gebruikt worden.

Tijdens deze inventarisatiestap wordt vastgesteld welke meetconfiguratie op het GroeiAcademie Stimulus Hardware Shield aanwezig is.

Visuele controle

Controleer connector H5.

Wanneer op H5 een ADS1115-module geplaatst is, worden de sensoren via de ADS1115 uitgelezen.

Wanneer H5 leeg is, worden de sensoren rechtstreeks verbonden met de analoge ingangen van het Arduino Uno R3-vormfactorbord via de H6-jumpers.

Gebruikersvraag

Hoe worden de FSR402/RFP602-sensoren uitgelezen?

□ Via een ADS1115-module
□ Via directe ADC-metingen

Indien antwoord = Via een ADS1115-module

Het GroeiAcademie Stimulus Hardware Shield gebruikt de ADS1115 als externe ADC.

De inventarisatie gaat verder met:

**3.2.4.1) Controles voor ADS1115-module**

Indien antwoord = Via directe ADC-metingen

Het GroeiAcademie Stimulus Hardware Shield gebruikt de interne ADC van het geselecteerde Arduino Uno R3-vormfactorbord.

De inventarisatie gaat verder met:

**3.2.4.2) Controles voor directe ADC-metingen**

Resultaat

Alle verdere hardwaretesten worden automatisch aangepast aan deze configuratie.

------------------------------------------------------------------------------
3.2.4.1) Controles voor ADS1115-module
------------------------------------------------------------------------------

Wanneer de FSR402/RFP602-sensoren via een ADS1115-module uitgelezen worden, worden de controles 3.2.4.1.1 tot en met 3.2.4.1.3 uitgevoerd.

------------------------------------------------------------------------------
3.2.4.1.1) Controleren of de ADS1115-module correct geplaatst is op connector H5
------------------------------------------------------------------------------

Doel

Wanneer de FSR402/RFP602-sensoren via een ADS1115-module uitgelezen worden, moet eerst gecontroleerd worden of deze module correct op connector H5 geplaatst is.

Een foutief geplaatste ADS1115-module kan ertoe leiden dat de module niet gedetecteerd wordt, verkeerde meetwaarden levert of beschadigd raakt.

Daarom wordt vóór de eigenlijke hardwarevalidatie eerst de fysieke montage gecontroleerd.

Visuele controle

Controleer of de ADS1115-module volledig op connector H5 ingestoken is.

Controleer eveneens dat:

• alle pinnen correct in de connector geplaatst zijn;
• geen enkele pin naast de connector terechtgekomen is;
• de module volledig ingestoken is;
• de module niet onder een hoek geplaatst is;
• de componentenzijde overeenkomt met de voorziene montage op het GroeiAcademie Stimulus Hardware Shield;
• VCC van de ADS1115-module zich op pin 1 van connector H5 bevindt.

Gebruikersvraag

Is de ADS1115-module correct geplaatst op connector H5 en bevindt VCC zich op pin 1 van connector H5?

□ Ja
□ Nee

Indien antwoord = Ja

Controleer nogmaals dat:

• de ADS1115 volledig ingestoken is;
• alle pinnen correct uitgelijnd zijn;
• VCC zich op pin 1 van connector H5 bevindt.

De inventarisatie gaat verder met de configuratie van SW1.

Indien antwoord = Nee

Controleer vervolgens:

• de correcte plaatsing van de ADS1115-module;
• de uitlijning van alle pinnen;
• de volledige insteekdiepte;
• dat VCC zich op pin 1 van connector H5 bevindt.

Corrigeer de opstelling en herhaal daarna deze controle.

Resultaat

De correcte plaatsing en oriëntatie van de ADS1115-module worden geregistreerd.

------------------------------------------------------------------------------
3.2.4.1.2) Controleren van de adreskeuze van de ADS1115 via SW1
------------------------------------------------------------------------------

Doel

De ADS1115 ondersteunt vier verschillende I²C-adressen.

Via schakelaar SW1 wordt bepaald welk adres de ADS1115 gebruikt.

De software moet dit adres kennen om de ADS1115 later automatisch te kunnen detecteren.

Visuele controle

Controleer de stand van SW1.

Er mag slechts één schakelaar geselecteerd zijn.

Gebruikersvraag

Welke positie is op SW1 geselecteerd?

□ Positie 1
□ Positie 2
□ Positie 3
□ Positie 4

Indien antwoord = Positie 1

Controleer dat SW1 overeenkomt met:

ADDR → GND

Standaard I²C-adres:

0x48

Indien antwoord = Positie 2

Controleer dat SW1 overeenkomt met:

ADDR → VDD

Standaard I²C-adres:

0x49

Indien antwoord = Positie 3

Controleer dat SW1 overeenkomt met:

ADDR → SDA

Standaard I²C-adres:

0x4A

Indien antwoord = Positie 4

Controleer dat SW1 overeenkomt met:

ADDR → SCL

Standaard I²C-adres:

0x4B

Resultaat

Deze informatie wordt later gebruikt voor de automatische detectie van de ADS1115.

------------------------------------------------------------------------------
3.2.4.1.3) Controleren dat de H6-jumpers niet geplaatst zijn
------------------------------------------------------------------------------

Doel

Wanneer de ADS1115 gebruikt wordt, mogen de vier H6-jumpers niet geplaatst zijn.

De ADS1115-module en de vier H6-jumpers mogen nooit gelijktijdig gebruikt worden.

Visuele controle

Controleer dat alle vier de H6-jumpers verwijderd werden.

Gebruikersvraag

Zijn alle vier de H6-jumpers verwijderd?

□ Ja
□ Nee

Indien antwoord = Ja

Controleer nogmaals dat geen enkele H6-jumper geplaatst is.

Indien antwoord = Nee

Verwijder alle vier de H6-jumpers en herhaal daarna deze controle.

Resultaat

Het GroeiAcademie Stimulus Hardware Shield wordt geconfigureerd voor gebruik met een ADS1115-module.

De verdere hardwarevalidatie zal alle sensormetingen via de ADS1115 uitvoeren.

------------------------------------------------------------------------------
3.2.4.2) Controles voor directe ADC-metingen
------------------------------------------------------------------------------

Wanneer de FSR402/RFP602-sensoren via directe ADC-metingen uitgelezen worden, wordt controle 3.2.4.2.1 uitgevoerd.

------------------------------------------------------------------------------
3.2.4.2.1) Controleren dat de H6-jumpers zowel aanwezig als correct aangesloten zijn
------------------------------------------------------------------------------

Doel

Wanneer geen ADS1115-module gebruikt wordt, worden de FSR402/RFP602-sensoren rechtstreeks verbonden met de interne ADC van het geselecteerde Arduino Uno R3-vormfactorbord.

Deze verbinding wordt gerealiseerd met de vier H6-jumpers.

Iedere H6-jumper verbindt één sensorlijn met één analoge ingang van het Arduino Uno R3-vormfactorbord.

Visuele controle

Controleer dat geen ADS1115-module geplaatst is op connector H5.

Controleer vervolgens dat alle vier de H6-jumpers geplaatst zijn.

Controleer eveneens dat iedere jumper volledig ingestoken is.

Controleer vervolgens de vier verbindingen afzonderlijk.

• H5 pin 7 wordt via H6 verbonden met A0.
• H5 pin 8 wordt via H6 verbonden met A1.
• H5 pin 9 wordt via H6 verbonden met A2.
• H5 pin 10 wordt via H6 verbonden met A3.

Controleer tenslotte dat de ADS1115-module niet gelijktijdig met de H6-jumpers geplaatst werd.

De ADS1115-module en de vier H6-jumpers mogen nooit gelijktijdig gebruikt worden.

Gebruikersvraag

Zijn de vier H6-jumpers zowel aanwezig als correct aangesloten?

□ Ja
□ Nee

Indien antwoord = Ja

Controleer nogmaals dat:

• geen ADS1115-module aanwezig is op connector H5;
• alle vier de H6-jumpers aanwezig zijn;
• iedere jumper volledig geplaatst is;
• iedere sensorlijn met de juiste analoge ingang verbonden wordt.

Het GroeiAcademie Stimulus Hardware Shield is nu correct voorbereid voor metingen via de interne ADC van het geselecteerde Arduino Uno R3-vormfactorbord.

Indien antwoord = Nee

Controleer vervolgens:

• of geen ADS1115-module geplaatst is;
• of alle vier de H6-jumpers aanwezig zijn;
• of iedere jumper correct geplaatst is;
• of geen enkele jumper één positie verschoven geplaatst werd.

Corrigeer de opstelling en herhaal daarna deze controle.

Resultaat

Het GroeiAcademie Stimulus Hardware Shield wordt geconfigureerd voor gebruik met de interne ADC van het geselecteerde Arduino Uno R3-vormfactorbord.

Deze configuratie wordt gebruikt tijdens alle verdere sensormetingen.

------------------------------------------------------------------------------
3.2.5) CharacterScreen
------------------------------------------------------------------------------

Doel

Het GroeiAcademie Stimulus Hardware Shield ondersteunt verschillende CharacterScreens die via de I²C-bus aangesloten worden.

Tijdens de hardware-inventarisatie wordt vastgesteld:

• of een CharacterScreen aanwezig is;
• welk CharacterScreen gebruikt wordt;
• of de aansluiting correct uitgevoerd werd;
• of een Quad Logic Level Shifter gebruikt wordt;
• of vaste draadbruggen gebruikt worden.

------------------------------------------------------------------------------
3.2.5.1) Controleren van het type CharacterScreen
------------------------------------------------------------------------------

Doel

De software ondersteunt meerdere CharacterScreens.

Door het juiste schermtype tijdens de hardware-inventarisatie vast te leggen, kan de software later automatisch de correcte schermconfiguratie gebruiken.

Gebruikersvraag

Welk CharacterScreen is aangesloten?

□ LCD 1602
□ LCD 1604
□ LCD 2002
□ LCD 2004
□ LCD 4002
□ Ander CharacterScreen
□ Geen CharacterScreen

Indien antwoord = LCD 1602, LCD 1604, LCD 2002, LCD 2004 of LCD 4002

Het geselecteerde CharacterScreen wordt ondersteund.

De inventarisatie gaat steeds verder met de controle van de aansluiting op connector H2 en daarna met 3.2.5.2.

Indien antwoord = Ander CharacterScreen

Dit CharacterScreen wordt niet ondersteund.

Gebruikersvraag

Welke actie wil je uitvoeren?

□ Toch doorgaan, ook al wordt dit CharacterScreen niet ondersteund
□ Stoppen

Indien antwoord = Toch doorgaan, ook al wordt dit CharacterScreen niet ondersteund

De inventarisatie gaat verder met de controle van de aansluiting op connector H2 en daarna met 3.2.5.2.

Indien antwoord = Stoppen

De hardware-inventarisatie wordt onderbroken.

Indien antwoord = Geen CharacterScreen

Er is geen CharacterScreen aanwezig.

Alle CharacterScreen-testen worden tijdens de hardwarevalidatie automatisch overgeslagen.

De hardware-inventarisatie gaat verder met 3.2.6.

Resultaat

Deze informatie bepaalt later:

• hoeveel regels beschikbaar zijn;
• hoeveel karakters per regel weergegeven kunnen worden;
• welke schermconfiguratie automatisch gebruikt wordt.

Visuele controle

Controleer of het geselecteerde CharacterScreen aangesloten is op connector H2.

Controleer vervolgens de oriëntatie van de connector.

Pin 1 van connector H2 moet verbonden zijn met GND.

Controleer eveneens dat de connector volledig ingestoken is en niet één positie verschoven aangesloten werd.

Gebruikersvraag

Is het geselecteerde CharacterScreen correct aangesloten op connector H2?

□ Ja
□ Nee

Indien antwoord = Ja

Controleer dat:

• de connector volledig ingestoken is;
• pin 1 met GND verbonden is;
• de kabel niet omgekeerd aangesloten werd.

De inventarisatie gaat verder met 3.2.5.2.

Indien antwoord = Nee

Controleer de aansluiting op connector H2, corrigeer de opstelling en herhaal daarna deze controle.

Resultaat

Deze informatie bepaalt later:

• of de CharacterScreen-library geladen wordt;
• of een I²C-scan uitgevoerd wordt;
• of de CharacterScreen-test uitgevoerd wordt.

------------------------------------------------------------------------------
3.2.5.2) Controleren van de Quad Logic Level Shifter of vaste draadbruggen voor het CharacterScreen op de Dupont-header socket bestaande uit H3/H4
------------------------------------------------------------------------------

Doel

Afhankelijk van het gebruikte CharacterScreen kan een Quad Logic Level Shifter gebruikt worden om de I²C-signalen veilig tussen verschillende logicaniveaus om te zetten.

Wanneer geen Quad Logic Level Shifter gebruikt wordt, worden de noodzakelijke verbindingen gerealiseerd met vaste draadbruggen.

Tijdens deze stap wordt vastgesteld welke configuratie gebruikt wordt.

Visuele controle

Controleer de Dupont-header socket, bestemd voor de Quad Logic Level Shifter, bestaande uit H3/H4.

Controleer of een Quad Logic Level Shifter geplaatst is.

Gebruikersvraag

Is op de Dupont-header socket bestaande uit H3/H4 een Quad Logic Level Shifter geplaatst?

□ Ja
□ Nee

Indien antwoord = Ja

Controleer dat:

• de Quad Logic Level Shifter volledig ingestoken is;
• de HV-zijde zich aan connector H3 bevindt;
• de LV-zijde zich aan connector H4 bevindt;
• alle pinnen correct uitgelijnd zijn;
• geen enkele draadbrug geplaatst is tussen H3 en H4.

Indien antwoord = Nee

Controleer de aanwezigheid van de volgende vaste draadbruggen tussen de overeenkomstige pinnen van H3 en H4:

• H3 pin 3 met H4 pin 3
• H3 pin 4 met H4 pin 4
• H3 pin 5 met H4 pin 5
• H3 pin 6 met H4 pin 6

Controleer eveneens dat:

• iedere draadbrug degelijk gesoldeerd werd;
• geen ongewenste kortsluitingen aanwezig zijn;
• geen Quad Logic Level Shifter geplaatst is.

Resultaat

De software weet hierdoor of het CharacterScreen via een Quad Logic Level Shifter of via vaste draadbruggen aangesloten werd.

------------------------------------------------------------------------------
3.2.6) Controleren van het PixelScreen via SPI
------------------------------------------------------------------------------

Doel

Het GroeiAcademie Stimulus Hardware Shield ondersteunt een PixelScreen dat via de SPI-bus aangesloten wordt.

Tijdens de hardware-inventarisatie wordt vastgesteld:

• of een PixelScreen aanwezig is;
• of de aansluiting correct uitgevoerd werd;
• of Quad Logic Level Shifters gebruikt worden;
• of vaste draadbruggen gebruikt worden.

Gebruikersvraag

Welk PixelScreen is aangesloten?

□ GMT020.02.7P v1.3 of later
□ Ander PixelScreen
□ Geen PixelScreen

Indien antwoord = GMT020.02.7P v1.3 of later

Het geselecteerde PixelScreen wordt ondersteund.

De inventarisatie gaat verder met de controle van de aansluiting op connector H8 en daarna met 3.2.6.1.

Indien antwoord = Ander PixelScreen

Dit PixelScreen wordt niet ondersteund.

Gebruikersvraag

Welke actie wil je uitvoeren?

□ Toch doorgaan, ook al wordt dit PixelScreen niet ondersteund
□ Stoppen

Indien antwoord = Toch doorgaan, ook al wordt dit PixelScreen niet ondersteund

De inventarisatie gaat verder met de controle van de aansluiting op connector H8 en daarna met 3.2.6.1.

Indien antwoord = Stoppen

De hardware-inventarisatie wordt onderbroken.

Indien antwoord = Geen PixelScreen

Er is geen PixelScreen aanwezig.

Alle PixelScreen-testen worden tijdens de hardwarevalidatie automatisch overgeslagen.

De overige controles van 3.2.6 worden overgeslagen.

Visuele controle

Controleer of het geselecteerde PixelScreen aangesloten is op connector H8.

Controleer vervolgens de oriëntatie van de connector.

Pin 1 van connector H8 moet verbonden zijn met GND.

Controleer eveneens dat de connector volledig ingestoken is en niet één positie verschoven aangesloten werd.

Gebruikersvraag

Is het geselecteerde PixelScreen correct aangesloten op connector H8?

□ Ja
□ Nee

Indien antwoord = Ja

Controleer dat:

• de connector volledig ingestoken is;
• pin 1 met GND verbonden is;
• de kabel niet omgekeerd aangesloten werd.

De inventarisatie gaat verder met 3.2.6.1.

Indien antwoord = Nee

Controleer de aansluiting op connector H8, corrigeer de opstelling en herhaal daarna deze controle.

Resultaat

Deze informatie bepaalt later:

• of de PixelScreen-library geladen wordt;
• of de PixelScreen-test uitgevoerd wordt.

------------------------------------------------------------------------------
3.2.6.1) Controleren van de Quad Logic Level Shifters of vaste draadbruggen voor het PixelScreen op de Dupont-header sockets bestaande uit H3/H4 en H9/H10
------------------------------------------------------------------------------

Doel

Afhankelijk van het gebruikte PixelScreen kunnen Quad Logic Level Shifters gebruikt worden om de SPI-signalen veilig tussen verschillende logicaniveaus om te zetten.

Wanneer geen Quad Logic Level Shifters gebruikt worden, worden de noodzakelijke verbindingen gerealiseerd met vaste draadbruggen.

Tijdens deze stap wordt vastgesteld welke configuratie gebruikt wordt.

Visuele controle

Controleer de twee Dupont-header sockets, bestemd voor de Quad Logic Level Shifters, bestaande uit H3/H4 en H9/H10.

Controleer of beide Quad Logic Level Shifters geplaatst zijn.

Gebruikersvraag

Zijn de Quad Logic Level Shifters geplaatst?

□ Ja
□ Nee

Indien antwoord = Ja

Controleer dat:

• de Quad Logic Level Shifter op H3/H4 volledig ingestoken is;
• de HV-zijde zich aan connector H3 bevindt;
• de LV-zijde zich aan connector H4 bevindt;

Controleer eveneens dat:

• de Quad Logic Level Shifter op H9/H10 volledig ingestoken is;
• de HV-zijde zich aan connector H9 bevindt;
• de LV-zijde zich aan connector H10 bevindt;

Controleer tenslotte dat geen draadbruggen geplaatst zijn.

Indien antwoord = Nee

Controleer de aanwezigheid van de volgende vaste draadbruggen.

Tussen H3 en H4:

• H3 pin 1 met H4 pin 1

Tussen H9 en H10:

• H9 pin 1 met H10 pin 1
• H9 pin 2 met H10 pin 2
• H9 pin 3 met H10 pin 3
• H9 pin 4 met H10 pin 4
• H9 pin 5 met H10 pin 5
• H9 pin 6 met H10 pin 6

Controleer eveneens dat:

• iedere draadbrug degelijk gesoldeerd werd;
• geen ongewenste kortsluitingen aanwezig zijn;
• geen Quad Logic Level Shifters geplaatst zijn.

Resultaat

De software weet hierdoor of het PixelScreen via Quad Logic Level Shifters of via vaste draadbruggen aangesloten werd.

******************************************************************************
4. Automatische hardwaredetectie
******************************************************************************

==============================================================================
4.1) Doel
==============================================================================

Na de hardware-inventarisatie beschikt de software over een volledig overzicht van de aanwezige hardware.

Het GroeiAcademie Stimulus Hardware Shield is tijdens deze tweede ronde op het geselecteerde Arduino Uno R3-vormfactorbord geplaatst en staat onder spanning.

Tijdens de automatische hardwaredetectie controleert de software zonder gebruikersinteractie welke onderdelen daadwerkelijk bereikbaar zijn en correct communiceren.

Hierbij wordt uitsluitend gecontroleerd wat software betrouwbaar kan vaststellen.

Controles die niet softwarematig uitgevoerd kunnen worden, blijven onderdeel van de visuele hardwarevalidatie.

Wanneer een automatische controle mislukt, wordt de gebruiker onmiddellijk geïnformeerd zodat de oorzaak eerst opgelost kan worden voordat de verdere hardwarevalidatie gestart wordt.

==============================================================================
4.2) Automatisch detecteerbare hardware
==============================================================================

Afhankelijk van de hardwareconfiguratie controleert de software automatisch:

• bereikbaarheid van de I²C-bus;

• bereikbaarheid van de SPI-bus;

• aanwezigheid van de ADS1115-module;

• communicatie met het CharacterScreen;

• communicatie met het PixelScreen;

• bereikbaarheid van de geselecteerde ADC;

Hardware die software niet betrouwbaar kan detecteren wordt niet automatisch goedgekeurd.

==============================================================================
4.3) Automatische detectie van de I²C-bus
==============================================================================

Doel

De software controleert of alle verwachte I²C-apparaten bereikbaar zijn.

Hierbij worden uitsluitend de apparaten gecontroleerd die volgens de hardware-inventarisatie aanwezig zouden moeten zijn.

Automatische controles

De software voert een volledige I²C-scan uit.

Per gevonden apparaat wordt gecontroleerd:

• of het apparaat bereikbaar is;

• of het gevonden adres overeenkomt met de hardwareconfiguratie;

• of geen onverwachte I²C-apparaten aanwezig zijn.

Wanneer één of meerdere verwachte apparaten niet gevonden worden, wordt hiervan een foutmelding weergegeven.

Doel

Controleer of alle verwachte I²C-apparaten correct bereikbaar zijn.

Automatische controles

De software voert opnieuw een I²C-scan uit.

Alle tijdens de hardware-inventarisatie geselecteerde I²C-apparaten moeten gevonden worden.

Wanneer een apparaat ontbreekt of een onverwacht adres gevonden wordt, wordt de hardwarevalidatie onderbroken.

==============================================================================
4.4) Automatische detectie van de ADS1115-module
==============================================================================

Doel

Wanneer tijdens de hardware-inventarisatie gekozen werd voor een ADS1115-module, controleert de software automatisch of deze bereikbaar is.

Automatische controles

De software controleert:

• of de ADS1115 reageert;

• of het geselecteerde I²C-adres overeenkomt met het gevonden adres;

• of communicatie met de ADS1115 mogelijk is.

Wanneer de ADS1115 niet gevonden wordt, wordt de hardwarevalidatie onderbroken.

==============================================================================
4.5) Automatische detectie van het CharacterScreen
==============================================================================

Doel

Wanneer een CharacterScreen aanwezig is, controleert de software automatisch of dit scherm correct werkt.

Automatische controles

De software:

• initialiseert het CharacterScreen;

• schrijft een testbericht;

• controleert of geen communicatiefouten optreden.

==============================================================================
4.6) Automatische detectie van het PixelScreen
==============================================================================

Doel

Wanneer een PixelScreen aanwezig is, controleert de software automatisch of het scherm correct communiceert.

Automatische controles

De software:

• initialiseert het PixelScreen;

• vult het scherm met testkleuren;

• toont een testtekst;

• controleert de communicatie met het scherm.

==============================================================================
4.7) Automatische detectie van de ADC
==============================================================================

Doel

De software controleert of de geselecteerde ADC gebruikt kan worden.

Automatische controles

Wanneer gekozen werd voor de interne ADC van het Arduino Uno R3-vormfactorbord, worden de overeenkomstige analoge ingangen gecontroleerd.

Wanneer gekozen werd voor de ADS1115-module, worden de vier ADS1115-kanalen gecontroleerd.

Doel

Controleer de correcte werking van de geselecteerde ADC.

Automatische controles

Wanneer de interne ADC gebruikt wordt, worden alle geselecteerde analoge ingangen gecontroleerd.

Wanneer de ADS1115 gebruikt wordt, worden alle vier ADS1115-kanalen gecontroleerd.

==============================================================================
4.8) Resultaat
==============================================================================

Na afloop van de automatische hardwaredetectie weet de software welke onderdelen correct bereikbaar zijn.

Wanneer alle automatische controles succesvol doorlopen werden, kan de eigenlijke hardwarevalidatie gestart worden.

Wanneer één of meerdere controles mislukken, wordt de hardwarevalidatie onderbroken totdat de oorzaak opgelost is.

******************************************************************************
5. Hardwarevalidatie zonder gebruikersinteractie
******************************************************************************

==============================================================================
5.1) Doel
==============================================================================

Na de hardware-inventarisatie wordt het GroeiAcademie Stimulus Hardware Shield op het geselecteerde Arduino Uno R3-vormfactorbord geplaatst en krijgt het spanning.

Tijdens deze tweede ronde voert de software uitsluitend automatische testen uit waarvoor geen gebruikersinteractie nodig is.

Wanneer een automatische controle mislukt, wordt de automatische hardwarevalidatie onderbroken voordat de derde ronde gestart wordt.

==============================================================================
5.2) Automatische controles na de keuze van het Arduino Uno R3-vormfactorbord
==============================================================================

Automatische controles

Na de keuze van het Arduino Uno R3-vormfactorbord past de software automatisch de verdere hardwarevalidatie aan.

Hierbij worden onder andere ingesteld:

• juiste ADC-configuratie;
• juiste I²C-configuratie;
• juiste SPI-configuratie;
• juiste werkspanning van het FSR402/RFP602-circuit;
• juiste hardwaretesten voor het gekozen Arduino Uno R3-vormfactorbord.

Wanneer later een hardwareconfiguratie gekozen wordt die niet overeenkomt met de mogelijkheden van het geselecteerde Arduino Uno R3-vormfactorbord, wordt de gebruiker hiervan verwittigd.

==============================================================================
5.3) Automatische controles van de sensorconfiguratie FSR402/RFP602
==============================================================================

Automatische controles

Na de inventarisatie controleert de software automatisch:

• hoeveel sensorkanalen geactiveerd moeten worden;
• welke sensoren later getest moeten worden;
• welke kalibratiestappen uitgevoerd moeten worden;
• welke sensorkanalen automatisch genegeerd kunnen worden.

==============================================================================
5.4) Automatische controles van de werkspanning van het FSR402/RFP602-circuit
==============================================================================

Automatische controles

Na de inventarisatie controleert de software automatisch:

• of de gekozen werkspanning overeenkomt met het geselecteerde Arduino Uno R3-vormfactorbord;
• of de gekozen werkspanning overeenkomt met de verdere hardwareconfiguratie;
• of de juiste softwareconfiguratie geselecteerd kan worden.

Wanneer een ongeldige combinatie vastgesteld wordt, ontvangt de gebruiker een duidelijke foutmelding en wordt de hardwarevalidatie onderbroken totdat de configuratie gecorrigeerd werd.

==============================================================================
5.5) Automatische controles van de meetconfiguratie van de FSR402/RFP602-sensoren
==============================================================================

Automatische controles

Na deze keuze past de software automatisch de verdere hardwarevalidatie aan.

Hierbij wordt automatisch bepaald:

• welke ADC gebruikt wordt;
• welke softwarecomponenten geladen worden;
• welke hardwaretesten uitgevoerd worden;
• welke hardwaretesten automatisch overgeslagen worden.

==============================================================================
5.6) Automatische controles voor de ADS1115-module
==============================================================================

Automatische controles

Tijdens de hardwarevalidatie zal de software automatisch controleren:

• of de ADS1115 via de I²C-bus bereikbaar is;
• of het geselecteerde adres gevonden wordt;
• of de communicatie correct verloopt.

Automatische controles

Tijdens de hardwarevalidatie zoekt de software uitsluitend naar het geselecteerde I²C-adres.

Wanneer geen ADS1115 op dit adres gevonden wordt, ontvangt de gebruiker een foutmelding.

==============================================================================
5.7) Automatische controles voor directe ADC-metingen
==============================================================================

Automatische controles

Tijdens de hardwarevalidatie gebruikt de software uitsluitend de interne ADC van het geselecteerde Arduino Uno R3-vormfactorbord.

Alle controles die betrekking hebben op de ADS1115 worden automatisch overgeslagen.

==============================================================================
5.8) Automatische controles van het CharacterScreen
==============================================================================

Automatische controles

Tijdens de hardwarevalidatie zal de software automatisch:

• een I²C-scan uitvoeren;
• controleren of het CharacterScreen bereikbaar is;
• het scherm initialiseren;
• een testtekst weergeven.

Automatische controles

Tijdens de hardwarevalidatie controleert de software automatisch of het CharacterScreen via de I²C-bus bereikbaar is.

Automatische controles

Tijdens de hardwarevalidatie controleert de software automatisch:

• of het CharacterScreen via de I²C-bus bereikbaar is;
• of de initialisatie correct verloopt;
• of een testtekst correct weergegeven wordt.

==============================================================================
5.9) Automatische controles van het PixelScreen
==============================================================================

Automatische controles

Tijdens de hardwarevalidatie zal de software automatisch:

• het PixelScreen initialiseren;
• een kleurentest uitvoeren;
• een teksttest uitvoeren;
• de schermoriëntatie controleren.

Automatische controles

Tijdens de hardwarevalidatie controleert de software automatisch:

• of het PixelScreen geïnitialiseerd kan worden;
• of de SPI-communicatie correct verloopt.

==============================================================================
5.10) Resultaat
==============================================================================

Na afloop van deze automatische controles weet de software of de geïnventariseerde hardware zonder gebruikersinteractie correct kon worden gevalideerd.

Wanneer een automatische controle niet overeenkomt met de geïnventariseerde configuratie, wordt de automatische hardwarevalidatie onderbroken totdat de configuratie gecorrigeerd werd.

******************************************************************************
6. Hardwarevalidatie met gebruikersinteractie
******************************************************************************

==============================================================================
6.1) Doel
==============================================================================

Na de hardware-inventarisatie, de automatische hardwaredetectie en de hardwarevalidatie zonder gebruikersinteractie wordt de hardwarevalidatie met gebruikersinteractie uitgevoerd.

Het GroeiAcademie Stimulus Hardware Shield blijft tijdens deze derde ronde op het geselecteerde Arduino Uno R3-vormfactorbord geplaatst en onder spanning.

Tijdens deze fase worden de hardwaretesten uitgevoerd waarvoor gebruikersinteractie nodig is en wordt gecontroleerd of alle aanwezige hardware correct functioneert.

Hierbij worden uitsluitend de hardwaretesten uitgevoerd die van toepassing zijn op de tijdens de hardware-inventarisatie vastgestelde configuratie.

Hardware die niet aanwezig is of bewust niet gebruikt wordt, wordt automatisch overgeslagen.

Wanneer tijdens een hardwaretest een fout vastgesteld wordt, wordt de hardwarevalidatie onmiddellijk onderbroken.

Na het oplossen van de fout kan de hardwarevalidatie opnieuw gestart worden.

==============================================================================
6.2) Testvolgorde
==============================================================================

Om afhankelijkheden tussen de verschillende hardwareonderdelen te vermijden, worden de testen steeds in dezelfde volgorde uitgevoerd.

1. Voedingsspanningen

2. CharacterScreen

3. PixelScreen

4. Keymatrix

5. FSR402/RFP602-sensoren

6. Samenvatting

Wanneer een test niet uitgevoerd kan worden omdat een vorige test mislukt is, worden de resterende testen automatisch overgeslagen.

==============================================================================
6.3) Controleren van de voedingsspanningen
==============================================================================

Doel

Controleer of alle noodzakelijke voedingsspanningen aanwezig zijn.

Automatische controles

Wanneer het Arduino Uno R3-vormfactorbord dit ondersteunt, controleert de software de beschikbare voedingsspanningen.

Visuele controles

Controleer eveneens:

• correcte plaatsing van H7;

• correcte voedingsspanning van de aangesloten modules;

• geen zichtbare beschadigingen;

• geen oververhitting.

==============================================================================
6.4) Controleren van het CharacterScreen
==============================================================================

Doel

Controleer de correcte werking van het CharacterScreen.

Automatische controles

De software:

• initialiseert het scherm;

• schakelt de achtergrondverlichting in;

• toont een testbericht;

• controleert de communicatie.

Gebruikerscontrole

Controleer dat:

• alle tekens correct weergegeven worden;

• alle regels leesbaar zijn;

• geen ontbrekende karakters zichtbaar zijn;

• geen flikkering optreedt.

==============================================================================
6.5) Controleren van het PixelScreen
==============================================================================

Doel

Controleer de correcte werking van het PixelScreen.

Automatische controles

De software:

• initialiseert het scherm;

• toont meerdere testkleuren;

• toont een testtekst;

• controleert de schermoriëntatie.

Gebruikerscontrole

Controleer dat:

• het volledige scherm gebruikt wordt;

• alle kleuren correct weergegeven worden;

• geen willekeurige gekleurde punten zichtbaar zijn;

• geen flikkering optreedt;

• de tekst volledig leesbaar is.

==============================================================================
6.6) Controleren van de Keymatrix
==============================================================================

Doel

Controleer de correcte werking van alle vier de toetsen.

Automatische controles

De software vraagt de gebruiker iedere toets afzonderlijk in te drukken.

Per toets wordt gecontroleerd:

• juiste detectie;

• geen dubbele detectie;

• correcte vrijgave na loslaten.

Automatische controle

Na de bevestiging voert de software automatisch een functietest uit.

De gebruiker wordt gevraagd om achtereenvolgens:

• toets 1 in te drukken;
• toets 2 in te drukken;
• toets 3 in te drukken;
• toets 4 in te drukken.

De software controleert automatisch of iedere toets afzonderlijk gedetecteerd wordt.

Wanneer één of meerdere toetsen niet correct functioneren, wordt de hardwarevalidatie onderbroken en ontvangt de gebruiker een duidelijke foutmelding.

Doel

Wanneer een Keymatrix aanwezig is, controleert de software automatisch of iedere toets afzonderlijk gedetecteerd wordt.

Automatische controles

De gebruiker wordt gevraagd iedere toets eenmaal in te drukken.

De software controleert automatisch:

• toets 1;

• toets 2;

• toets 3;

• toets 4.

Iedere toets moet afzonderlijk gedetecteerd worden.

==============================================================================
6.7) Controleren van de FSR402/RFP602-sensoren
==============================================================================

Doel

Controleer de correcte werking van iedere aangesloten FSR402/RFP602-sensor.

Automatische controles

Per sensor controleert de software:

• rustwaarde;

• verandering tijdens indrukken;

• maximumwaarde;

• terugkeer naar de rustwaarde.

Gebruikerscontrole

De gebruiker wordt gevraagd iedere sensor afzonderlijk volledig in te drukken en daarna volledig los te laten.

Doel

De software controleert of alle tijdens de hardware-inventarisatie geselecteerde sensorkanalen reageren.

Automatische controles

De gebruiker wordt gevraagd iedere sensor afzonderlijk in te drukken.

Per sensor controleert de software:

• of uitsluitend het juiste kanaal reageert;

• of de gemeten waarde duidelijk verandert;

• of de waarde na loslaten opnieuw stabiliseert.

==============================================================================
6.8) Samenvatting
==============================================================================

Na afloop van de hardwarevalidatie wordt een volledig overzicht weergegeven.

Per hardwareonderdeel wordt aangegeven:

• Geslaagd

• Waarschuwing

• Mislukt

Wanneer alle testen succesvol doorlopen werden, is het GroeiAcademie Stimulus Hardware Shield correct gevalideerd.

Wanneer één of meerdere testen mislukken, wordt aangegeven welke onderdelen gecontroleerd of gecorrigeerd moeten worden voordat de hardware opnieuw gevalideerd wordt.

******************************************************************************
7. Gebruikersinteractie tijdens de hardwarevalidatie
******************************************************************************

==============================================================================
7.1) Doel
==============================================================================

De hardwarevalidatie begeleidt de gebruiker stap voor stap doorheen het volledige validatieproces.

Tijdens iedere stap wordt uitsluitend de informatie gevraagd die de software niet zelf betrouwbaar kan vaststellen.

Alle overige controles worden automatisch uitgevoerd.

Hierdoor verloopt de hardwarevalidatie zo eenvoudig, snel en betrouwbaar mogelijk.

==============================================================================
7.2) Opbouw van iedere validatiestap
==============================================================================

Iedere validatiestap verloopt volgens hetzelfde principe.

1. De software beschrijft welke controle uitgevoerd wordt.

2. Indien nodig voert de gebruiker eerst een visuele controle uit.

3. Wanneer gebruikersinteractie noodzakelijk is, wordt een duidelijke vraag gesteld.

4. Daarna voert de software automatisch de bijbehorende controle uit.

5. Het resultaat wordt onmiddellijk weergegeven.

6. Indien de controle geslaagd is, wordt automatisch naar de volgende stap gegaan.

==============================================================================
7.3) Gebruikersinstructies
==============================================================================

Iedere opdracht wordt zo duidelijk mogelijk omschreven.

Voorbeelden:

• Druk toets 1 in.

• Laat toets 1 los.

• Druk sensor 2 volledig in.

• Laat sensor 2 opnieuw los.

• Controleer of pin 1 met GND verbonden is.

• Bevestig dat de jumper op H7 vóór het onder spanning brengen in de 5V-positie geplaatst werd.

Per stap wordt slechts één opdracht tegelijk gegeven.

==============================================================================
7.4) Automatische voortgang
==============================================================================

Wanneer een stap succesvol afgerond werd, gaat de hardwarevalidatie automatisch verder met de volgende stap.

De gebruiker hoeft hiervoor geen bijkomende bevestiging te geven.

Hierdoor blijft de volledige hardwarevalidatie vlot verlopen.

==============================================================================
7.5) Waarschuwingen
==============================================================================

Wanneer een controle niet succesvol uitgevoerd kan worden, verschijnt een duidelijke waarschuwing.

Een waarschuwing bevat steeds:

• welk onderdeel gecontroleerd werd;

• waarom de controle mislukt is;

• welke mogelijke oorzaken bestaan;

• welke acties uitgevoerd kunnen worden.

Een waarschuwing betekent niet noodzakelijk dat de hardwarevalidatie onmiddellijk stopt.

==============================================================================
7.6) Fouten
==============================================================================

Wanneer een fout vastgesteld wordt waardoor de verdere hardwarevalidatie geen betrouwbare resultaten meer kan opleveren, wordt de hardwarevalidatie onmiddellijk onderbroken.

De gebruiker ontvangt een duidelijke foutmelding met:

• het betrokken hardwareonderdeel;

• de vastgestelde fout;

• mogelijke oorzaken;

• mogelijke oplossingen.

Na het oplossen van de fout kan de hardwarevalidatie opnieuw gestart worden.

==============================================================================
7.7) Herhalen van een validatiestap
==============================================================================

Na het oplossen van een probleem kan de huidige validatiestap opnieuw uitgevoerd worden.

De software hoeft hiervoor niet opnieuw volledig gestart te worden.

Wanneer meerdere opeenvolgende stappen afhankelijk zijn van de gewijzigde hardwareconfiguratie, wordt automatisch bepaald vanaf welke stap de hardwarevalidatie opnieuw moet beginnen.

==============================================================================
7.8) Wijzigen van de hardwareconfiguratie
==============================================================================

Wanneer tijdens de hardwarevalidatie blijkt dat de hardwareconfiguratie gewijzigd werd, kan de eerder geïnventariseerde configuratie niet langer als betrouwbaar beschouwd worden.

De software vraagt daarom of een nieuwe hardware-inventarisatie uitgevoerd moet worden.

==============================================================================
7.9) Beëindigen van de hardwarevalidatie
==============================================================================

De hardwarevalidatie eindigt wanneer:

• alle testen succesvol afgerond werden;

• de gebruiker de hardwarevalidatie onderbreekt;

• een kritieke fout vastgesteld wordt.

Na afloop wordt steeds een volledig validatierapport weergegeven.

==============================================================================
7.10) Validatierapport
==============================================================================

Na iedere hardwarevalidatie wordt een overzicht van alle uitgevoerde controles weergegeven.

Per onderdeel wordt minstens vermeld:

• Geslaagd

• Waarschuwing

• Mislukt

Daarnaast bevat het rapport:

• gebruikte versie van het GroeiAcademie Stimulus Hardware Shield;

• geselecteerd Arduino Uno R3-vormfactorbord;

• gebruikte hardwareconfiguratie;

• datum en tijd van de validatie;

• versie van de validatiesoftware.

Wanneer alle controles succesvol afgerond werden, wordt de hardware als volledig gevalideerd beschouwd.
