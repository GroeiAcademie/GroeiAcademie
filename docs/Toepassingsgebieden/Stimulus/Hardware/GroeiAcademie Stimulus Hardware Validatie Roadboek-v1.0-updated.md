******************************************************************************
GroeiAcademie Stimulus Hardware Validatie Roadboek
Documentversie v1.1
Hardware Shield v0.10.1
******************************************************************************

==============================================================================
0. Inleiding
==============================================================================

------------------------------------------------------------------------------
0.1) Doel van dit document
------------------------------------------------------------------------------

Dit document beschrijft de volledige inventarisatie en validatie van het GroeiAcademie Stimulus Hardware Shield v0.10.1.

Het document vormt de functionele en technische specificatie van de validatiesoftware. Het beschrijft stap voor stap hoe de aanwezige hardware eerst zonder spanning visueel geïnventariseerd wordt, daarna onder spanning automatisch gecontroleerd wordt en ten slotte met gerichte gebruikersinteractie functioneel gevalideerd wordt.

Het doel is een reproduceerbare en betrouwbare hardwarevalidatie waarbij de software zelf uitvoert wat betrouwbaar softwarematig vastgesteld kan worden en de gebruiker uitsluitend inschakelt wanneer een visuele beoordeling of fysieke handeling noodzakelijk is.

------------------------------------------------------------------------------
0.2) Drie opeenvolgende rondes
------------------------------------------------------------------------------

De volledige validatie bestaat uit drie strikt gescheiden rondes.

Ronde 1 — Hardware-inventarisatie zonder spanning

• Het Hardware Shield is nog niet op het moederbord geplaatst.
• Het Hardware Shield ontvangt geen voedingsspanning.
• De gebruiker voert uitsluitend visuele controles uit.
• De software stelt vragen via de seriële monitor of een andere invoermethode die niet afhankelijk is van het Hardware Shield.
• Er worden nog geen I²C-, SPI-, ADC-, scherm-, toets- of sensortesten uitgevoerd.
• De vastgestelde hardwareconfiguratie wordt voorlopig opgeslagen.

Ronde 2 — Automatische hardwarevalidatie onder spanning

• Het Hardware Shield wordt op het geselecteerde moederbord geplaatst.
• De software voert alle betrouwbare automatische controles uit waarvoor geen gebruikershandeling nodig is.
• Verwachte apparaten, adressen, rusttoestanden en basiswaarden worden gecontroleerd.
• Hardware die niet softwarematig bevestigd kan worden, wordt nog niet als functioneel goedgekeurd.

Ronde 3 — Hardwarevalidatie met gebruikersinteractie

• De gebruiker voert uitsluitend de noodzakelijke fysieke handelingen uit.
• De Keymatrix, schermweergave en FSR402/RFP602-sensoren worden functioneel getest.
• De software analyseert de reacties en vraagt alleen om een visuele bevestiging wanneer zij het resultaat niet zelf kan waarnemen.
• Pas na het slagen van deze ronde wordt de volledige hardwareconfiguratie als gevalideerd opgeslagen.

------------------------------------------------------------------------------
0.3) Doelgroep
------------------------------------------------------------------------------

Dit document is bedoeld voor:

• ontwikkelaars van het GroeiAcademie Framework;
• ontwikkelaars van het GroeiAcademie Stimulus Hardware Shield;
• testers;
• personen die een nieuw Hardware Shield assembleren;
• personen die wijzigingen aan het Hardware Shield uitvoeren;
• toekomstige ontwikkelaars van nieuwe Hardware Shield-versies.

------------------------------------------------------------------------------
0.4) Uitgangspunten
------------------------------------------------------------------------------

Tijdens het ontwerp en de uitvoering van de validatiesoftware gelden de volgende uitgangspunten.

• De software voert automatisch uit wat zij betrouwbaar zelf kan vaststellen.
• De gebruiker wordt uitsluitend bevraagd over wat software niet betrouwbaar kan bepalen.
• Visuele inventarisatie, automatische controles en interactieve functietesten worden niet met elkaar vermengd.
• Een bevestiging door de gebruiker vervangt nooit een automatische controle die de software zelf betrouwbaar kan uitvoeren.
• Een automatische initialisatie wordt niet ten onrechte beschouwd als bewijs dat visuele uitvoer correct is.
• Niet-aanwezige hardware wordt automatisch overgeslagen.
• Een ongeldige fysieke combinatie wordt niet als configuratie opgeslagen.
• De hardwareconfiguratie krijgt pas de status gevalideerd nadat alle toepasselijke rondes succesvol afgerond zijn.

------------------------------------------------------------------------------
0.5) Bijhorende documenten
------------------------------------------------------------------------------

Dit document maakt deel uit van de documentatie van het GroeiAcademie Stimulus Hardware Shield.

Bijhorende documenten zijn onder andere:

• het elektrisch schema van het GroeiAcademie Stimulus Hardware Shield v0.10.1;
• de PCB-layout;
• de stuklijst;
• de validatiesoftware;
• de technische documentatie van de ondersteunde hardwaremodules.

------------------------------------------------------------------------------
0.6) Bijhorende software
------------------------------------------------------------------------------

De hardwarevalidatie wordt uitgevoerd met de officiële validatiesoftware van het GroeiAcademie Framework.

De software gebruikt de tijdens de inventarisatie opgebouwde configuratie om uitsluitend de toepasselijke controles en testen uit te voeren.

------------------------------------------------------------------------------
0.7) Ondersteunde moederborden
------------------------------------------------------------------------------

Tijdens Hardware Shield-versie v0.10.1 worden de volgende moederborden ondersteund:

• Arduino UNO R3;
• Arduino UNO R4 Minima;
• Arduino UNO R4 WiFi;
• ESP32 D1 R32, experimenteel en tijdens v0.10.1 te valideren.

De Arduino UNO R3, Arduino UNO R4 Minima en Arduino UNO R4 WiFi gebruiken in deze configuratie een werkspanning van 5V voor het FSR402/RFP602-circuit.

De ESP32 D1 R32 gebruikt 3V3-logicaniveaus. Bij rechtstreekse aansluiting op de interne ADC moet het FSR402/RFP602-circuit daarom via H7 op 3V3 ingesteld zijn. Deze moederbordcombinatie blijft experimenteel totdat alle shieldfuncties afzonderlijk gevalideerd zijn.

------------------------------------------------------------------------------
0.8) Ondersteunde hardware
------------------------------------------------------------------------------

De validatiesoftware ondersteunt:

• maximaal vier FSR402/RFP602-druksensoren;
• een ADS1115-module op connector H5;
• directe meting via de interne ADC van het geselecteerde moederbord;
• CharacterScreens LCD 1602, LCD 1604, LCD 2002, LCD 2004 en LCD 4002 via I²C;
• het PixelScreen GMT020.02.7P V1.3 of later;
• een 1x4 Keymatrix;
• Quad Logic Level Shifters;
• de connectoren, jumpers, schakelaars en draadbruggen van het GroeiAcademie Stimulus Hardware Shield v0.10.1.

------------------------------------------------------------------------------
0.9) Terminologie
------------------------------------------------------------------------------

Hardware-inventarisatie

Het zonder spanning visueel vaststellen welke hardware aanwezig is en hoe deze geconfigureerd werd.

Automatische hardwarevalidatie

Het onder spanning softwarematig controleren van alle eigenschappen die zonder fysieke handeling of visuele beoordeling betrouwbaar vastgesteld kunnen worden.

Hardwarevalidatie met gebruikersinteractie

Het functioneel testen van hardware waarvoor de gebruiker een toets of sensor moet bedienen of de zichtbare schermuitvoer moet beoordelen.

Hardwareconfiguratie

De verzameling van alle tijdens de inventarisatie vastgelegde keuzes.

Gevalideerde hardwareconfiguratie

Een hardwareconfiguratie waarvoor alle toepasselijke automatische en interactieve controles succesvol afgerond zijn.

GroeiAcademie Stimulus Hardware Shield

Het Hardware Shield waarop dit document betrekking heeft.

------------------------------------------------------------------------------
0.10) Versiebeheer
------------------------------------------------------------------------------

Dit document is opgesteld voor het GroeiAcademie Stimulus Hardware Shield v0.10.1.

Bij iedere wijziging aan het elektrisch schema, de PCB-layout, de connectorbezetting of de ondersteunde hardware moet gecontroleerd worden welke inventarisatie- en validatiestappen aangepast moeten worden.


******************************************************************************
1. Hardware-inventarisatie
******************************************************************************

==============================================================================
1.1) Doel
==============================================================================

Het doel van de hardware-inventarisatie is om vóór het Hardware Shield op het moederbord geplaatst wordt vast te stellen welke hardware aanwezig is en hoe deze hardware fysiek geconfigureerd werd.

Tijdens deze ronde wordt niet getest of de hardware elektrisch of functioneel correct werkt. Er wordt uitsluitend gekeken, gekozen en geregistreerd.

De inventarisatie bepaalt:

• welk moederbord gebruikt wordt;
• of een 1x4 Keymatrix aanwezig is;
• hoeveel FSR402/RFP602-sensoren aangesloten zijn;
• welke werkspanning via H7 geselecteerd werd;
• of de sensoren via een ADS1115 of via de interne ADC gemeten worden;
• welk ADS1115-adres via SW1 gekozen werd;
• welk CharacterScreen aanwezig is;
• welk PixelScreen aanwezig is;
• waar Quad Logic Level Shifters of vaste draadbruggen gebruikt worden;
• welke controles in ronde 2 uitgevoerd moeten worden;
• welke functietesten in ronde 3 uitgevoerd moeten worden.

==============================================================================
1.2) Voorwaarden vóór de inventarisatie
==============================================================================

Vóór de inventarisatie gelden de volgende voorwaarden.

• Het Hardware Shield is niet op het moederbord geplaatst.
• Het Hardware Shield is niet via USB, een externe voeding of een ander circuit gevoed.
• Alle modules, jumpers, schermen, sensoren en kabels zijn fysiek bereikbaar voor visuele controle.
• Het elektrisch schema van Hardware Shield v0.10.1 is beschikbaar wanneer twijfel bestaat over de connectororiëntatie.
• Gebruikersinvoer verloopt via de seriële monitor of een andere methode die niet afhankelijk is van het Hardware Shield.

Er worden tijdens ronde 1 geen automatische hardwarecontroles uitgevoerd.

==============================================================================
1.3) Algemene werkwijze
==============================================================================

Iedere inventarisatiestap bestaat uitsluitend uit:

1. een doel;
2. een visuele controle;
3. een gebruikersvraag;
4. een logisch vervolg op basis van het antwoord;
5. het registreren van het resultaat.

Wanneer een fysieke configuratie ongeldig of onvolledig is, wordt de inventarisatie onderbroken totdat deze configuratie gecorrigeerd werd.

De software gaat niet verder op basis van een antwoord dat strijdig is met de vereiste fysieke opbouw.

==============================================================================
1.4) Resultaat van ronde 1
==============================================================================

Na ronde 1 beschikt de software over een voorlopige hardwareconfiguratie.

Deze configuratie beschrijft wat volgens de visuele inventarisatie aanwezig is. Zij vormt de verwachte configuratie voor ronde 2 en ronde 3.

De configuratie is op dit moment nog niet gevalideerd.


******************************************************************************
2. Inventariseren van het GroeiAcademie Stimulus Hardware Shield v0.10.1
******************************************************************************

==============================================================================
2.0) Controleren welk moederbord gebruikt wordt
==============================================================================

Doel

Het geselecteerde moederbord bepaalt de beschikbare interne ADC, de gebruikte pinnen, de werkspanning van het FSR402/RFP602-circuit en de toepasselijke validatiestappen.

Gebruikersvraag

Welk moederbord wordt gebruikt?

□ Arduino UNO R3
□ Arduino UNO R4 Minima
□ Arduino UNO R4 WiFi
□ ESP32 D1 R32, experimenteel

Resultaat

Het geselecteerde moederbord wordt in de voorlopige hardwareconfiguratie geregistreerd.

Deze keuze wordt tijdens ronde 2 vergeleken met de softwarebuild en de daadwerkelijk gebruikte platformconfiguratie.

==============================================================================
2.1) Inventariseren van de 1x4 Keymatrix
==============================================================================

Doel

De 1x4 Keymatrix kan tijdens ronde 3 gebruikt worden om keuzes te maken, testen te bevestigen, stappen te herhalen of de validatie af te breken.

Omdat het Hardware Shield tijdens ronde 1 nog niet op het moederbord geplaatst is, wordt de Keymatrix in deze ronde uitsluitend visueel geïnventariseerd.

Visuele controle

Controleer connector H1.

Pin 5 van connector H1 moet met GND verbonden zijn.

Controleer eveneens dat:

• de connector volledig ingestoken is;
• de kabel niet omgekeerd of verdraaid aangesloten is;
• de overige vier aansluitingen in de juiste volgorde met toets 1, toets 2, toets 3 en toets 4 verbonden zijn.

Gebruikersvraag

Is de 1x4 Keymatrix aangesloten op connector H1?

□ Ja
□ Nee

Indien antwoord = Ja

Registreer de Keymatrix als aanwezig.

De elektrische rusttoestand wordt tijdens ronde 2 gecontroleerd.

De werking van iedere afzonderlijke toets wordt aan het begin van ronde 3 getest. Na een succesvolle toetsentest kan de Keymatrix voor de verdere gebruikersinvoer gebruikt worden.

Indien antwoord = Nee

Registreer de Keymatrix als afwezig.

Alle gebruikersinvoer blijft tijdens de verdere validatie via de seriële monitor of een andere beschikbare invoermethode verlopen.

Resultaat

De aanwezigheid of afwezigheid van de 1x4 Keymatrix wordt in de voorlopige hardwareconfiguratie geregistreerd.

==============================================================================
2.2) Inventariseren van de FSR402/RFP602-sensorconfiguratie
==============================================================================

Doel

Het Hardware Shield ondersteunt geen, twee of vier FSR402/RFP602-sensoren.

De software moet weten welke sensorkanalen tijdens ronde 2 en ronde 3 verwacht worden.

Visuele controle

Controleer de sensoraansluitingen:

• U4: Sensor 1;
• U5: Sensor 2;
• U6: Sensor 3;
• U7: Sensor 4.

Bij twee sensoren moeten uitsluitend U4 en U5 gebruikt worden.

Bij vier sensoren moeten U4, U5, U6 en U7 gebruikt worden.

Gebruikersvraag

Hoeveel FSR402/RFP602-sensoren zijn aangesloten?

□ Geen
□ Twee
□ Vier

Indien antwoord = Geen

Controleer dat U4, U5, U6 en U7 leeg zijn.

De sensortesten worden overgeslagen. De aanwezige meetconfiguratie kan desondanks afzonderlijk geïnventariseerd en gevalideerd worden wanneer een ADS1115 of H6-jumpers gemonteerd zijn.

Indien antwoord = Twee

Controleer dat:

• Sensor 1 op U4 aangesloten is;
• Sensor 2 op U5 aangesloten is;
• U6 leeg is;
• U7 leeg is.

Indien antwoord = Vier

Controleer dat:

• Sensor 1 op U4 aangesloten is;
• Sensor 2 op U5 aangesloten is;
• Sensor 3 op U6 aangesloten is;
• Sensor 4 op U7 aangesloten is.

Resultaat

Het aantal sensoren en de verwachte actieve sensorkanalen worden in de voorlopige hardwareconfiguratie geregistreerd.

==============================================================================
2.3) Controleren van de werkspanning van het FSR402/RFP602-circuit
==============================================================================

Doel

Jumper H7 selecteert de werkspanning van het FSR402/RFP602-circuit en de voedingsspanning van een eventueel geplaatste ADS1115-module.

De positie van H7 moet overeenkomen met het geselecteerde moederbord en de gebruikte meetconfiguratie.

Visuele controle

Controleer de positie van de jumper op H7.

Mogelijke situaties:

• 5V geselecteerd;
• 3V3 geselecteerd;
• geen jumper geplaatst.

Gebruikersvraag

In welke positie staat de jumper op H7?

□ 5V
□ 3V3
□ Geen jumper geplaatst

Indien antwoord = 5V

Deze instelling is de voorziene configuratie voor:

• Arduino UNO R3;
• Arduino UNO R4 Minima;
• Arduino UNO R4 WiFi.

Wanneer ESP32 D1 R32 als moederbord geselecteerd werd, is deze combinatie voor de directe interne ADC niet toegestaan. Corrigeer H7 naar 3V3 voordat de inventarisatie verdergaat.

Indien antwoord = 3V3

Deze instelling is de voorziene configuratie voor ESP32 D1 R32.

Wanneer een Arduino UNO R3, Arduino UNO R4 Minima of Arduino UNO R4 WiFi geselecteerd werd, wijkt deze instelling af van de standaardconfiguratie van Hardware Shield v0.10.1 en moet zij vóór verdere validatie expliciet als ondersteunde testconfiguratie vastgelegd zijn.

Indien antwoord = Geen jumper geplaatst

Het FSR402/RFP602-circuit en een eventueel geplaatste ADS1115-module ontvangen via H7 geen voedingsspanning.

Wanneer sensoren of een ADS1115 gebruikt worden, is deze configuratie ongeldig. Plaats H7 eerst in de vereiste positie.

Wanneer geen sensoren en geen ADS1115 geplaatst zijn, kan H7 leeg blijven. De sensor- en ADC-validatie wordt dan overgeslagen.

Resultaat

De visueel vastgestelde positie van H7 wordt in de voorlopige hardwareconfiguratie geregistreerd.

==============================================================================
2.4) Inventariseren van de meetconfiguratie van de FSR402/RFP602-sensoren
==============================================================================

Doel

De FSR402/RFP602-sensoren kunnen op exact één van de volgende manieren gemeten worden:

• via een ADS1115-module op connector H5;
• rechtstreeks via de interne ADC van het geselecteerde moederbord met de vier H6-jumpers.

De ADS1115-module en de vier H6-jumpers mogen nooit gelijktijdig gebruikt worden.

Visuele controle

Controleer eerst connector H5 en de vier H6-posities zonder iets te wijzigen.

Gebruikersvraag

Is een ADS1115-module op connector H5 geplaatst?

□ Ja
□ Nee

Indien antwoord = Ja

Ga uitsluitend verder met:

2.4.1) Controleren van de ADS1115-module op connector H5

Na 2.4.1 gaat de inventarisatie verder met:

2.4.2) Controleren van de adreskeuze van de ADS1115 via SW1

De controle van de H6-jumpers voor directe ADC-metingen wordt niet als afzonderlijk meetpad uitgevoerd. Er wordt uitsluitend gecontroleerd dat alle H6-jumpers verwijderd zijn.

Indien antwoord = Nee

Ga uitsluitend verder met:

2.4.3) Controleren van de H6-jumpers voor directe ADC-metingen

De ADS1115-controles en de adreskeuze via SW1 worden overgeslagen.

Resultaat

De gekozen meetconfiguratie wordt pas geregistreerd nadat het volledige toepasselijke pad zonder fysieke tegenstrijdigheden afgerond is.

------------------------------------------------------------------------------
2.4.1) Controleren van de ADS1115-module op connector H5
------------------------------------------------------------------------------

Doel

Controleer of de ADS1115-module correct georiënteerd en volledig op H5 geplaatst is en of de directe verbindingen met de interne ADC volledig verwijderd zijn.

Visuele controle

Connector H5 heeft de volgende relevante pinvolgorde:

• pin 1: VCC;
• pin 2: GND;
• pin 3: SCL;
• pin 4: SDA;
• pin 5: ADDR;
• pin 6: ALERT;
• pin 7: A0;
• pin 8: A1;
• pin 9: A2;
• pin 10: A3.

Controleer dat:

• VCC van de ADS1115 zich op pin 1 van H5 bevindt;
• GND van de ADS1115 zich op pin 2 van H5 bevindt;
• alle tien pinnen correct uitgelijnd zijn;
• geen pin naast de connector terechtgekomen is;
• de module volledig en recht ingestoken is;
• alle vier de H6-jumpers verwijderd zijn.

Gebruikersvraag

Is de ADS1115-module correct op H5 geplaatst en zijn alle vier de H6-jumpers verwijderd?

□ Ja
□ Nee

Indien antwoord = Ja

Registreer dat het ADS1115-meetpad fysiek geselecteerd is.

Ga verder met 2.4.2.

Indien antwoord = Nee

Corrigeer de plaatsing terwijl het Hardware Shield nog steeds volledig spanningsloos is.

Herhaal daarna deze controle.

De inventarisatie gaat niet verder zolang:

• de ADS1115 verkeerd georiënteerd is;
• één of meerdere pinnen verkeerd geplaatst zijn;
• één of meerdere H6-jumpers aanwezig zijn.

Resultaat

Het ADS1115-meetpad is visueel correct voorbereid. De elektrische bereikbaarheid wordt pas tijdens ronde 2 gecontroleerd.

------------------------------------------------------------------------------
2.4.2) Controleren van de adreskeuze van de ADS1115 via SW1
------------------------------------------------------------------------------

Doel

Via SW1 wordt het I²C-adres van de ADS1115 gekozen. De software moet vóór ronde 2 weten op welk adres de module verwacht wordt.

Visuele controle

Controleer de stand van SW1.

Er mag exact één adrespositie geselecteerd zijn.

Gebruikersvraag

Welke positie is op SW1 geselecteerd?

□ Positie 1
□ Positie 2
□ Positie 3
□ Positie 4
□ Geen geldige enkelvoudige positie

Indien antwoord = Positie 1

ADDR is met GND verbonden.

Verwacht I²C-adres: 0x48.

Indien antwoord = Positie 2

ADDR is met VDD verbonden.

Verwacht I²C-adres: 0x49.

Indien antwoord = Positie 3

ADDR is met SDA verbonden.

Verwacht I²C-adres: 0x4A.

Indien antwoord = Positie 4

ADDR is met SCL verbonden.

Verwacht I²C-adres: 0x4B.

Indien antwoord = Geen geldige enkelvoudige positie

Corrigeer SW1 terwijl het Hardware Shield spanningsloos blijft.

Selecteer exact één positie en herhaal de controle.

Resultaat

Het verwachte ADS1115-adres wordt in de voorlopige hardwareconfiguratie geregistreerd.

De ADS1115 wordt pas tijdens ronde 2 op dit adres softwarematig gezocht en uitgelezen.

------------------------------------------------------------------------------
2.4.3) Controleren van de H6-jumpers voor directe ADC-metingen
------------------------------------------------------------------------------

Doel

Wanneer geen ADS1115-module gebruikt wordt, verbinden de vier H6-jumpers de vier sensorsignalen rechtstreeks met de interne ADC van het geselecteerde moederbord.

Visuele controle

Controleer eerst dat connector H5 volledig leeg is.

Controleer vervolgens dat alle vier de H6-jumpers aanwezig en volledig geplaatst zijn.

De vier verbindingen zijn:

• H5 pin 7 via H6 naar A0;
• H5 pin 8 via H6 naar A1;
• H5 pin 9 via H6 naar A2;
• H5 pin 10 via H6 naar A3.

Gebruikersvraag

Is H5 leeg en zijn alle vier de H6-jumpers correct geplaatst?

□ Ja
□ Nee

Indien antwoord = Ja

Registreer het meetpad via de interne ADC van het geselecteerde moederbord.

Controleer nogmaals dat:

• geen ADS1115-module op H5 aanwezig is;
• alle vier de H6-jumpers aanwezig zijn;
• geen jumper ontbreekt;
• geen jumper één positie verschoven staat.

Indien antwoord = Nee

Corrigeer de fysieke configuratie terwijl het Hardware Shield spanningsloos blijft.

De inventarisatie gaat niet verder zolang H5 niet leeg is of de vier H6-jumpers niet correct geplaatst zijn.

Resultaat

Het meetpad via de interne ADC wordt in de voorlopige hardwareconfiguratie geregistreerd.

Alle ADS1115-controles worden tijdens ronde 2 en ronde 3 overgeslagen.

==============================================================================
2.5) Inventariseren van het CharacterScreen
==============================================================================

Doel

De software ondersteunt de CharacterScreens LCD 1602, LCD 1604, LCD 2002, LCD 2004 en LCD 4002.

Het schermtype wordt eerst gekozen. Daardoor is vóór de controle van H2 reeds vastgesteld dat het aangesloten scherm door deze versie ondersteund wordt.

Gebruikersvraag

Welk CharacterScreen wordt gebruikt?

□ Geen CharacterScreen
□ LCD 1602
□ LCD 1604
□ LCD 2002
□ LCD 2004
□ LCD 4002

Indien antwoord = Geen CharacterScreen

Registreer het CharacterScreen als afwezig.

Sla 2.5.1 en 2.5.2 over.

Indien antwoord = LCD 1602

Registreer 16 karakters en 2 regels.

Ga verder met 2.5.1.

Indien antwoord = LCD 1604

Registreer 16 karakters en 4 regels.

Ga verder met 2.5.1.

Indien antwoord = LCD 2002

Registreer 20 karakters en 2 regels.

Ga verder met 2.5.1.

Indien antwoord = LCD 2004

Registreer 20 karakters en 4 regels.

Ga verder met 2.5.1.

Indien antwoord = LCD 4002

Registreer 40 karakters en 2 regels.

Ga verder met 2.5.1.

Resultaat

Het gekozen CharacterScreen en de bijhorende schermafmetingen worden in de voorlopige hardwareconfiguratie geregistreerd.

------------------------------------------------------------------------------
2.5.1) Controleren van de aansluiting van het CharacterScreen op H2
------------------------------------------------------------------------------

Doel

Controleer de fysieke aansluiting en oriëntatie van het reeds als ondersteund gekozen CharacterScreen.

Visuele controle

Controleer connector H2.

Pin 1 van H2 moet met GND van het CharacterScreen verbonden zijn.

Controleer eveneens dat:

• de connector volledig ingestoken is;
• de connector niet één positie verschoven is;
• de kabel niet omgekeerd aangesloten is;
• VCC, GND, SDA en SCL met de overeenkomstige aansluitingen van het CharacterScreen verbonden zijn.

Gebruikersvraag

Is het gekozen CharacterScreen correct op H2 aangesloten?

□ Ja
□ Nee

Indien antwoord = Ja

Ga verder met 2.5.2.

Indien antwoord = Nee

Corrigeer de aansluiting terwijl het Hardware Shield spanningsloos blijft en herhaal daarna deze controle.

Resultaat

De fysieke aansluiting op H2 is visueel gecontroleerd. De I²C-bereikbaarheid wordt pas tijdens ronde 2 gecontroleerd.

------------------------------------------------------------------------------
2.5.2) Controleren van de niveauconversie voor het CharacterScreen
------------------------------------------------------------------------------

Doel

De I²C-verbinding van het CharacterScreen loopt via de Dupont-header socket voor de Quad Logic Level Shifter, bestaande uit connectoren H3 en H4.

Er wordt ofwel een Quad Logic Level Shifter gebruikt, ofwel worden de noodzakelijke verbindingen met vaste draadbruggen gerealiseerd. Beide configuraties mogen niet gelijktijdig aanwezig zijn.

Visuele controle

Controleer de Dupont-header socket bestaande uit H3 en H4.

Gebruikersvraag

Is een Quad Logic Level Shifter op H3/H4 geplaatst?

□ Ja
□ Nee

Indien antwoord = Ja

Controleer dat:

• de Quad Logic Level Shifter volledig ingestoken is;
• de HV-zijde zich aan H3 bevindt;
• de LV-zijde zich aan H4 bevindt;
• alle pinnen correct uitgelijnd zijn;
• geen draadbruggen tussen H3 en H4 geplaatst zijn.

Indien antwoord = Nee

Controleer de aanwezigheid van de volgende draadbruggen tussen H3 en H4:

• H3 pin 3 met H4 pin 3;
• H3 pin 4 met H4 pin 4;
• H3 pin 5 met H4 pin 5;
• H3 pin 6 met H4 pin 6.

Controleer eveneens dat:

• iedere draadbrug degelijk gesoldeerd is;
• geen ongewenste kortsluitingen tussen aangrenzende pinnen aanwezig zijn;
• geen Quad Logic Level Shifter geplaatst is.

Resultaat

De gebruikte niveauconversie voor het CharacterScreen wordt in de voorlopige hardwareconfiguratie geregistreerd.

==============================================================================
2.6) Inventariseren van het PixelScreen
==============================================================================

Doel

Hardware Shield v0.10.1 ondersteunt het PixelScreen GMT020.02.7P V1.3 of later op connector H8.

Gebruikersvraag

Welk PixelScreen wordt gebruikt?

□ Geen PixelScreen
□ GMT020.02.7P V1.3 of later

Indien antwoord = Geen PixelScreen

Registreer het PixelScreen als afwezig.

Sla 2.6.1 en 2.6.2 over.

Indien antwoord = GMT020.02.7P V1.3 of later

Ga verder met 2.6.1.

Resultaat

De aanwezigheid en het ondersteunde type van het PixelScreen worden in de voorlopige hardwareconfiguratie geregistreerd.

------------------------------------------------------------------------------
2.6.1) Controleren van de aansluiting van het PixelScreen op H8
------------------------------------------------------------------------------

Doel

Controleer de fysieke aansluiting en oriëntatie van het gekozen PixelScreen.

Visuele controle

Controleer connector H8.

Pin 1 van H8 moet met GND van het PixelScreen verbonden zijn.

De overige aansluitingen zijn overeenkomstig het schema bestemd voor VCC, SCL, SDA, RST, DC en CS.

Controleer dat:

• de connector volledig ingestoken is;
• de connector niet één positie verschoven is;
• de kabel niet omgekeerd aangesloten is;
• iedere aansluiting met de overeenkomstige aansluiting van het PixelScreen verbonden is.

Gebruikersvraag

Is het PixelScreen correct op H8 aangesloten?

□ Ja
□ Nee

Indien antwoord = Ja

Ga verder met 2.6.2.

Indien antwoord = Nee

Corrigeer de aansluiting terwijl het Hardware Shield spanningsloos blijft en herhaal daarna deze controle.

Resultaat

De fysieke aansluiting op H8 is visueel gecontroleerd. De zichtbare werking wordt pas tijdens ronde 3 gevalideerd.

------------------------------------------------------------------------------
2.6.2) Controleren van de niveauconversie voor het PixelScreen
------------------------------------------------------------------------------

Doel

De verbinding van het PixelScreen gebruikt de Dupont-header sockets voor Quad Logic Level Shifters, bestaande uit connectorparen H3/H4 en H9/H10.

Er worden ofwel de vereiste Quad Logic Level Shifters gebruikt, ofwel vaste draadbruggen. Beide configuraties mogen niet gelijktijdig aanwezig zijn.

Visuele controle

Controleer:

• de Dupont-header socket bestaande uit H3 en H4;
• de Dupont-header socket bestaande uit H9 en H10.

Gebruikersvraag

Zijn de vereiste Quad Logic Level Shifters geplaatst?

□ Ja
□ Nee

Indien antwoord = Ja

Controleer op H3/H4 dat:

• de Quad Logic Level Shifter volledig ingestoken is;
• de HV-zijde zich aan H3 bevindt;
• de LV-zijde zich aan H4 bevindt;
• alle pinnen correct uitgelijnd zijn.

Controleer op H9/H10 dat:

• de Quad Logic Level Shifter volledig ingestoken is;
• de HV-zijde zich aan H9 bevindt;
• de LV-zijde zich aan H10 bevindt;
• alle pinnen correct uitgelijnd zijn.

Controleer tenslotte dat geen draadbruggen op de betrokken pinnen geplaatst zijn.

Indien antwoord = Nee

Controleer de aanwezigheid van de volgende zeven draadbruggen.

Tussen H3 en H4:

• H3 pin 1 met H4 pin 1.

Tussen H9 en H10:

• H9 pin 1 met H10 pin 1;
• H9 pin 2 met H10 pin 2;
• H9 pin 3 met H10 pin 3;
• H9 pin 4 met H10 pin 4;
• H9 pin 5 met H10 pin 5;
• H9 pin 6 met H10 pin 6.

Controleer eveneens dat:

• iedere draadbrug degelijk gesoldeerd is;
• geen ongewenste kortsluitingen tussen aangrenzende pinnen aanwezig zijn;
• geen Quad Logic Level Shifter op H3/H4 of H9/H10 geplaatst is.

Resultaat

De gebruikte niveauconversie voor het PixelScreen wordt in de voorlopige hardwareconfiguratie geregistreerd.

==============================================================================
2.7) Controleren van gedeelde connectorconfiguraties
==============================================================================

Doel

H3/H4 wordt door de niveauconversie van het CharacterScreen en het PixelScreen gedeeld. Wanneer beide schermen aanwezig zijn, mag voor H3/H4 slechts één fysiek consistente configuratie geregistreerd worden.

Controle

Wanneer zowel een CharacterScreen als een PixelScreen aanwezig is:

• moet de geregistreerde toestand van H3/H4 in beide inventarisatiepaden identiek zijn;
• mag niet één pad een Quad Logic Level Shifter en het andere pad draadbruggen registreren;
• moeten bij draadbruggen alle voor beide schermen vereiste verbindingen aanwezig zijn;
• mogen geen draadbruggen aanwezig zijn op pinnen die door een geplaatste Quad Logic Level Shifter gebruikt worden.

Resultaat

Bij een tegenstrijdige registratie wordt ronde 1 onderbroken en moet H3/H4 opnieuw visueel gecontroleerd worden.

==============================================================================
2.8) Samenvatting van de hardware-inventarisatie
==============================================================================

Na afloop toont de software de volledige voorlopige hardwareconfiguratie.

De samenvatting bevat minstens:

• geselecteerd moederbord;
• aanwezigheid van de 1x4 Keymatrix;
• aantal FSR402/RFP602-sensoren;
• positie van H7;
• ADS1115 of interne ADC;
• verwacht ADS1115-adres indien van toepassing;
• gekozen CharacterScreen;
• aansluiting en niveauconversie van het CharacterScreen;
• gekozen PixelScreen;
• aansluiting en niveauconversie van het PixelScreen;
• eventuele onderdelen die tijdens ronde 2 of ronde 3 overgeslagen worden.

De gebruiker kan de samenvatting bevestigen of terugkeren naar een foutief geïnventariseerde stap.

Na bevestiging wordt de configuratie voorlopig opgeslagen met status Geïnventariseerd, niet gevalideerd.


******************************************************************************
3. Automatische hardwarevalidatie
******************************************************************************

==============================================================================
3.1) Doel
==============================================================================

Tijdens ronde 2 wordt het Hardware Shield op het geselecteerde moederbord geplaatst en onder spanning automatisch gecontroleerd.

In deze ronde worden uitsluitend controles uitgevoerd waarvoor geen toetsdruk, sensordruk of visuele beoordeling van een scherm nodig is.

De automatische hardwarevalidatie vergelijkt de werkelijk softwarematig waarneembare toestand met de tijdens ronde 1 geïnventariseerde configuratie.

==============================================================================
3.2) Plaatsen van het Hardware Shield
==============================================================================

Vóór het plaatsen:

• schakel het moederbord uit;
• verwijder USB en iedere externe voeding;
• controleer dat geen pin van het Hardware Shield verbogen is;
• controleer dat alle moederbordheaders correct uitgelijnd zijn.

Plaats vervolgens het Hardware Shield volledig en recht op het geselecteerde moederbord.

Controleer dat:

• geen header één positie verschoven is;
• geen pin naast een header terechtgekomen is;
• het Hardware Shield nergens ongewenst contact maakt.

Sluit pas daarna de voeding of USB-verbinding aan.

==============================================================================
3.3) Controleren van het geselecteerde moederbord
==============================================================================

De software controleert of de build- en platformconfiguratie overeenkomen met het moederbord dat tijdens ronde 1 geselecteerd werd.

Waar het platform dit betrouwbaar ondersteunt, wordt gecontroleerd:

• microcontrollerarchitectuur;
• beschikbare pinconfiguratie;
• geselecteerde ADC-implementatie;
• beschikbare I²C- en SPI-interface.

Wanneer de gecompileerde software niet voor het geïnventariseerde moederbord bestemd is, wordt de automatische hardwarevalidatie gestopt.

==============================================================================
3.4) Controleren van de I²C-bus
==============================================================================

De software initialiseert de I²C-bus en voert een adresscan uit.

De scan wordt vergeleken met de verwachte configuratie.

Mogelijke verwachte I²C-apparaten zijn:

• de ADS1115 op het tijdens ronde 1 gekozen adres;
• de I²C-interface van het CharacterScreen.

De software registreert:

• ieder gevonden adres;
• ieder verwacht maar ontbrekend adres;
• ieder onverwacht adres;
• een busfout of geblokkeerde I²C-lijn.

Een onverwacht I²C-adres wordt als waarschuwing geregistreerd wanneer het de verdere validatie niet verhindert. Een ontbrekend vereist apparaat wordt als fout geregistreerd.

==============================================================================
3.5) Automatisch valideren van de ADS1115
==============================================================================

Deze stap wordt uitsluitend uitgevoerd wanneer tijdens ronde 1 een ADS1115 geregistreerd werd.

De software controleert:

• of een apparaat op het verwachte adres 0x48, 0x49, 0x4A of 0x4B antwoordt;
• of de ADS1115-configuratieregisters leesbaar en beschrijfbaar zijn;
• of de ingestelde meetmodus aanvaard wordt;
• of de vier meetkanalen uitgelezen kunnen worden;
• of de ruwe meetwaarden binnen het technisch mogelijke bereik liggen.

Wanneer geen ADS1115 op het verwachte adres gevonden wordt, wordt niet automatisch naar de andere drie adressen overgeschakeld alsof de inventarisatie correct was.

De software mag de andere adressen wel scannen om een gerichte foutmelding te geven, bijvoorbeeld dat de ADS1115 waarschijnlijk op een andere SW1-positie staat.

==============================================================================
3.6) Automatisch valideren van het CharacterScreen
==============================================================================

Deze stap wordt uitsluitend uitgevoerd wanneer tijdens ronde 1 een CharacterScreen geregistreerd werd.

De software controleert:

• of de verwachte I²C-interface bereikbaar is;
• of de scherminterface zonder busfout geïnitialiseerd kan worden;
• of opdrachten naar de I²C-interface verzonden kunnen worden.

De software kan op basis van de I²C-interface niet betrouwbaar vaststellen welk LCD-formaat fysiek aangesloten is en kan evenmin automatisch zien of de tekens correct zichtbaar zijn.

Het gekozen type LCD 1602, LCD 1604, LCD 2002, LCD 2004 of LCD 4002 blijft daarom afkomstig uit ronde 1. De zichtbare weergave wordt pas tijdens ronde 3 gevalideerd.

==============================================================================
3.7) Automatisch voorbereiden van het PixelScreen
==============================================================================

Deze stap wordt uitsluitend uitgevoerd wanneer tijdens ronde 1 een PixelScreen geregistreerd werd.

De software initialiseert de voor het GMT020.02.7P V1.3 of later vereiste SPI-interface en verzendt de initialisatieopdrachten naar het scherm.

Bij de gebruikte aansluiting kan de software niet betrouwbaar vaststellen of het PixelScreen fysiek aanwezig is, de opdrachten ontvangen heeft of correct beeld weergeeft wanneer geen bruikbaar terugleeskanaal beschikbaar is.

Een succesvol uitgevoerde initialisatieroutine is daarom geen automatische goedkeuring van het PixelScreen.

De software registreert uitsluitend dat:

• de vereiste SPI-interface beschikbaar is;
• de benodigde pinnen softwarematig geconfigureerd konden worden;
• de initialisatieroutine zonder interne softwarefout uitgevoerd werd.

De zichtbare werking wordt volledig tijdens ronde 3 gevalideerd.

==============================================================================
3.8) Automatisch controleren van de rusttoestand van de 1x4 Keymatrix
==============================================================================

Deze stap wordt uitsluitend uitgevoerd wanneer tijdens ronde 1 een Keymatrix geregistreerd werd.

Zonder dat de gebruiker een toets indrukt, controleert de software:

• of geen toets permanent als ingedrukt gelezen wordt;
• of de vier ingangen een stabiele rusttoestand hebben;
• of geen ingang snel en ongecontroleerd wisselt.

Deze controle bewijst nog niet dat iedere toets werkt. De afzonderlijke toetsen worden tijdens ronde 3 getest.

Wanneer één toets bij aanvang reeds als ingedrukt gelezen wordt, vraagt de software eerst alle toetsen los te laten. Blijft de toestand bestaan, dan wordt een fout geregistreerd.

==============================================================================
3.9) Automatisch controleren van de geselecteerde ADC
==============================================================================

Wanneer de interne ADC geselecteerd werd, configureert de software de toepasselijke analoge ingangen van het geselecteerde moederbord.

Wanneer de ADS1115 geselecteerd werd, gebruikt de software uitsluitend de ADS1115-kanalen.

De software controleert zonder sensoren in te drukken:

• of alle verwachte kanalen uitgelezen kunnen worden;
• of geen kanaal buiten het technisch meetbereik valt;
• of de meetwaarden gedurende een korte rustmeting voldoende stabiel zijn;
• of geen kanaal exact hetzelfde onwaarschijnlijke foutpatroon vertoont als gevolg van een software- of busfout.

De kanaaltoewijzing en respons op druk worden pas tijdens ronde 3 functioneel gevalideerd.

==============================================================================
3.10) Automatisch controleren van de FSR402/RFP602-rustwaarden
==============================================================================

Deze stap wordt uitsluitend uitgevoerd wanneer twee of vier sensoren geregistreerd werden.

Zonder de sensoren aan te raken meet de software per actief kanaal:

• de gemiddelde rustwaarde;
• de minimale en maximale rustwaarde;
• de spreiding en stabiliteit;
• mogelijke verzadiging aan de onder- of bovengrens;
• opvallende verschillen tussen verwachte sensor- en ongebruikte kanalen.

Een geldige rustwaarde bewijst niet dat een sensor correct op druk reageert. De respons, kanaaltoewijzing en terugkeer naar rust worden tijdens ronde 3 getest.

==============================================================================
3.11) Resultaat van de automatische hardwarevalidatie
==============================================================================

Na ronde 2 krijgt iedere toepasselijke automatische controle één van de volgende statussen:

• Geslaagd;
• Waarschuwing;
• Mislukt;
• Niet toepasselijk;
• Niet automatisch controleerbaar.

Ronde 3 mag starten wanneer alle noodzakelijke automatische controles geslaagd zijn en eventuele waarschuwingen de betrouwbaarheid van de functietesten niet aantasten.

Bij een fout wordt vermeld:

• welk onderdeel verwacht werd;
• wat softwarematig vastgesteld werd;
• welke fysieke controle het meest waarschijnlijk nodig is;
• vanaf welke inventarisatie- of validatiestap opnieuw gestart moet worden.


******************************************************************************
4. Hardwarevalidatie met gebruikersinteractie
******************************************************************************

==============================================================================
4.1) Doel
==============================================================================

Tijdens ronde 3 worden de functies getest die alleen door een fysieke handeling of visuele beoordeling betrouwbaar gevalideerd kunnen worden.

De gebruiker krijgt per stap exact één duidelijke opdracht.

De software analyseert zelfstandig ieder meetbaar resultaat en vraagt geen bevestiging voor gegevens die zij zelf kan beoordelen.

==============================================================================
4.2) Invoer tijdens ronde 3
==============================================================================

Totdat de Keymatrix volledig getest is, verloopt de gebruikersinvoer via de seriële monitor of een andere reeds betrouwbare invoermethode.

Wanneer de Keymatrix aanwezig is en de toetsentest slaagt, kan zij vanaf dat moment gebruikt worden voor:

• doorgaan;
• herhalen;
• bevestigen;
• afbreken;
• kiezen tussen aangeboden opties.

Wanneer geen Keymatrix aanwezig is, blijft de alternatieve invoermethode gedurende de volledige validatie actief.

==============================================================================
4.3) Functioneel testen van de 1x4 Keymatrix
==============================================================================

Deze stap wordt uitsluitend uitgevoerd wanneer tijdens ronde 1 een Keymatrix geregistreerd werd.

De software vraagt achtereenvolgens:

1. druk uitsluitend toets 1 in;
2. laat toets 1 volledig los;
3. druk uitsluitend toets 2 in;
4. laat toets 2 volledig los;
5. druk uitsluitend toets 3 in;
6. laat toets 3 volledig los;
7. druk uitsluitend toets 4 in;
8. laat toets 4 volledig los.

Per toets controleert de software:

• of de juiste ingang reageert;
• of geen andere toets gelijktijdig gedetecteerd wordt;
• of slechts één geldige bediening geregistreerd wordt;
• of de toets na loslaten opnieuw naar de rusttoestand terugkeert;
• of geen blijvende of snel herhaalde ongewenste detectie optreedt.

Na een succesvolle test wordt de Keymatrix als betrouwbare invoermethode geactiveerd.

==============================================================================
4.4) Visueel valideren van het CharacterScreen
==============================================================================

Deze stap wordt uitsluitend uitgevoerd wanneer tijdens ronde 1 een CharacterScreen geregistreerd werd en de I²C-interface tijdens ronde 2 bereikbaar was.

De software:

• initialiseert het gekozen schermformaat;
• schakelt de achtergrondverlichting in;
• wist het scherm;
• vult iedere beschikbare regel met een herkenbaar testpatroon;
• toont een test met cijfers, hoofdletters, kleine letters en relevante symbolen;
• toont een tekst op de eerste en laatste beschikbare kolom en regel.

Gebruikerscontrole

Controleer dat:

• alle verwachte regels zichtbaar zijn;
• het aantal zichtbare karakters overeenkomt met het gekozen schermtype;
• de eerste en laatste kolom correct gebruikt worden;
• alle testtekens leesbaar zijn;
• geen tekens ontbreken of op een verkeerde positie verschijnen;
• de achtergrondverlichting werkt;
• geen ongewenste flikkering zichtbaar is.

Gebruikersvraag

Wordt de volledige CharacterScreen-test correct weergegeven?

□ Ja
□ Test herhalen
□ Nee

Indien antwoord = Nee

De software registreert een mislukte visuele schermvalidatie en toont de meest relevante controles voor H2, H3/H4, het gekozen schermtype en de I²C-interface.

==============================================================================
4.5) Visueel valideren van het PixelScreen
==============================================================================

Deze stap wordt uitsluitend uitgevoerd wanneer tijdens ronde 1 een PixelScreen geregistreerd werd.

De software voert achtereenvolgens uit:

• volledig scherm zwart;
• volledig scherm wit;
• volledig scherm rood;
• volledig scherm groen;
• volledig scherm blauw;
• duidelijke teksttest;
• lijnen langs alle schermranden;
• markeringen in alle vier de hoeken;
• een oriëntatietest met een herkenbare bovenzijde, onderzijde, linkerzijde en rechterzijde.

Gebruikerscontrole

Controleer dat:

• het volledige schermoppervlak gebruikt wordt;
• iedere testkleur correct en egaal weergegeven wordt;
• geen willekeurige gekleurde punten of ruis zichtbaar zijn;
• de tekst volledig leesbaar is;
• alle vier de randen en hoeken zichtbaar zijn;
• de oriëntatie correct is;
• geen ongewenste flikkering optreedt.

Gebruikersvraag

Wordt de volledige PixelScreen-test correct weergegeven?

□ Ja
□ Test herhalen
□ Nee

Indien antwoord = Nee

De software registreert een mislukte visuele schermvalidatie en verwijst gericht naar H8, H3/H4, H9/H10, de niveauconversie, de voeding en de gekozen schermconfiguratie.

==============================================================================
4.6) Functioneel testen van de FSR402/RFP602-sensoren
==============================================================================

Deze stap wordt uitsluitend uitgevoerd wanneer twee of vier sensoren geregistreerd werden.

De sensoren worden één voor één getest. De software vraagt nooit meerdere sensoren gelijktijdig in te drukken tenzij een latere specifieke combinatietest dit uitdrukkelijk vereist.

Per sensor verloopt de test als volgt:

1. laat alle sensoren volledig los;
2. wacht tot alle actieve kanalen stabiel zijn;
3. druk uitsluitend de aangeduide sensor geleidelijk in;
4. houd de sensor kort ingedrukt;
5. laat de sensor volledig los;
6. wacht tot de waarde opnieuw gestabiliseerd is.

De software controleert per sensor:

• of het verwachte kanaal reageert;
• of de meetwaarde duidelijk van de rustwaarde afwijkt;
• of de waarde tijdens toenemende druk in de verwachte richting verandert;
• of geen ander sensorkanaal onterecht even sterk reageert;
• of een bruikbare piekwaarde bereikt wordt;
• of de waarde na loslaten opnieuw naar de rustzone terugkeert;
• of de sensor niet permanent verzadigd blijft;
• of de sensor niet wegvalt of onrealistisch springt.

==============================================================================
4.7) Controleren van kanaaltoewijzing en ongewenste koppeling
==============================================================================

Tijdens iedere afzonderlijke sensortest bewaakt de software alle actieve kanalen.

De verwachte toewijzing is:

• U4 naar Sensor 1 en meetkanaal 1;
• U5 naar Sensor 2 en meetkanaal 2;
• U6 naar Sensor 3 en meetkanaal 3;
• U7 naar Sensor 4 en meetkanaal 4.

Wanneer een andere sensor of een ander kanaal reageert dan gevraagd, wordt aangegeven of waarschijnlijk sprake is van:

• verwisselde sensorconnectoren;
• verwisselde meetkanalen;
• een foutieve H6-verbinding;
• een verkeerd geplaatste ADS1115-module;
• een kortsluiting of ongewenste koppeling tussen kanalen.

==============================================================================
4.8) Herhalen en hervatten
==============================================================================

Na een mislukte interactieve test kan de gebruiker:

• de huidige test herhalen;
• terugkeren naar de relevante visuele inventarisatiestap;
• de volledige validatie afbreken.

Wanneer een fysieke wijziging uitgevoerd wordt, bepaalt de software welke eerdere automatische controles opnieuw uitgevoerd moeten worden.

Voorbeelden:

• na wijziging van SW1 wordt de ADS1115-adrescontrole opnieuw uitgevoerd;
• na wijziging van H3/H4 wordt de betrokken schermvalidatie opnieuw vanaf ronde 2 uitgevoerd;
• na wijziging van H6 worden de ADC-rustmetingen opnieuw uitgevoerd;
• na het verwisselen van sensoren wordt de sensortest opnieuw vanaf de kanaaltoewijzing uitgevoerd.

==============================================================================
4.9) Resultaat van ronde 3
==============================================================================

Na ronde 3 krijgt iedere toepasselijke functietest één van de volgende statussen:

• Geslaagd;
• Waarschuwing;
• Mislukt;
• Niet toepasselijk.

De hardwareconfiguratie wordt uitsluitend als gevalideerd beschouwd wanneer alle vereiste automatische en interactieve controles geslaagd zijn.


******************************************************************************
5. Opslaan en gebruiken van de hardwareconfiguratie
******************************************************************************

==============================================================================
5.1) Doel
==============================================================================

De hardwareconfiguratie wordt opgeslagen zodat zij bij een volgende opstart opnieuw gebruikt kan worden zonder alle visuele vragen opnieuw te stellen.

De opslag moet duidelijk onderscheiden tussen:

• geen geldige configuratie;
• geïnventariseerd, nog niet gevalideerd;
• automatisch gevalideerd, interactieve validatie nog niet voltooid;
• volledig gevalideerd;
• ongeldig geworden door een wijziging of fout.

==============================================================================
5.2) Op te slaan gegevens
==============================================================================

Minstens de volgende gegevens worden opgeslagen:

• identificatie van het gegevensformaat;
• versie van het gegevensformaat;
• versie van het Hardware Shield;
• versie van de validatiesoftware;
• geselecteerd moederbord;
• aanwezigheid van de 1x4 Keymatrix;
• aantal FSR402/RFP602-sensoren;
• positie van H7;
• geselecteerde meetconfiguratie;
• verwacht ADS1115-adres indien van toepassing;
• gekozen CharacterScreen en schermafmetingen;
• aansluiting en niveauconversie van het CharacterScreen;
• gekozen PixelScreen;
• aansluiting en niveauconversie van het PixelScreen;
• status van iedere uitgevoerde validatiestap;
• algemene status van de hardwareconfiguratie;
• controlesom voor de integriteit van de opgeslagen gegevens.

Datum en tijd worden alleen opgeslagen wanneer de gebruikte configuratie over een betrouwbare tijdsbron beschikt.

==============================================================================
5.3) Geldigheid van de opgeslagen gegevens
==============================================================================

Bij het lezen van de opgeslagen configuratie controleert de software minstens:

• de identificatie van het gegevensformaat;
• de versie van het gegevensformaat;
• de controlesom;
• de compatibiliteit met de huidige validatiesoftware;
• de compatibiliteit met Hardware Shield v0.10.1;
• de geldigheid van iedere opgeslagen keuzewaarde.

Wanneer één controle mislukt, wordt de opgeslagen configuratie niet gebruikt alsof zij betrouwbaar is.

==============================================================================
5.4) Gebruik bij een volgende opstart
==============================================================================

Wanneer een volledig geldige en gevalideerde configuratie gevonden wordt, kan de gebruiker kiezen:

□ Opgeslagen hardwareconfiguratie gebruiken
□ Automatische hardwarevalidatie opnieuw uitvoeren
□ Volledige hardware-inventarisatie opnieuw uitvoeren

Opgeslagen hardwareconfiguratie gebruiken

De configuratie wordt geladen. Controles die bij iedere opstart noodzakelijk zijn, mogen opnieuw uitgevoerd worden zonder alle visuele inventarisatievragen te herhalen.

Automatische hardwarevalidatie opnieuw uitvoeren

Ronde 2 wordt opnieuw uitgevoerd op basis van de opgeslagen inventarisatie. Ronde 3 wordt alleen opnieuw uitgevoerd wanneer een automatische controle daartoe aanleiding geeft of de gebruiker dit kiest.

Volledige hardware-inventarisatie opnieuw uitvoeren

De bestaande configuratie wordt ongeldig gemarkeerd en ronde 1 start opnieuw.

==============================================================================
5.5) Wijziging van de hardware
==============================================================================

Na iedere fysieke wijziging moet minstens het betrokken inventarisatiepad opnieuw uitgevoerd worden.

Een volledige nieuwe inventarisatie is vereist bij wijziging van:

• het moederbord;
• het aantal sensoren;
• H7;
• ADS1115 tegenover interne ADC;
• de plaatsing van H6;
• SW1;
• het CharacterScreen;
• het PixelScreen;
• een Quad Logic Level Shifter;
• de vaste draadbruggen;
• de relevante connectorbekabeling.

De eerder gevalideerde status wordt bij een dergelijke wijziging onmiddellijk ongeldig.

==============================================================================
5.6) Schrijfmomenten
==============================================================================

Om onnodige EEPROM-schrijfbewerkingen te beperken, wordt niet na iedere schermmelding geschreven.

De software schrijft bij voorkeur:

• na bevestiging van de volledige inventarisatiesamenvatting;
• na voltooiing van ronde 2;
• na voltooiing van ronde 3;
• bij een expliciete wijziging van de configuratie;
• bij het ongeldig verklaren van een eerdere validatie.


******************************************************************************
6. Gebruikersinteractie, foutafhandeling en validatierapport
******************************************************************************

==============================================================================
6.1) Algemene gebruikersinteractie
==============================================================================

Iedere stap geeft één duidelijke opdracht of vraag tegelijk.

De software vermeldt steeds:

• welk onderdeel behandeld wordt;
• wat de gebruiker moet controleren of uitvoeren;
• via welke invoermethode geantwoord kan worden;
• wat het resultaat van de stap is;
• welke volgende stap uitgevoerd wordt.

==============================================================================
6.2) Automatische voortgang
==============================================================================

Wanneer een controle zonder gebruikersbeslissing succesvol afgerond wordt, gaat de software automatisch verder.

Wanneer de gebruiker een visuele beoordeling moet geven, wacht de software op een expliciet antwoord.

De software gaat nooit automatisch verder terwijl nog een toets of sensor ingedrukt gehouden wordt.

==============================================================================
6.3) Statusniveaus
==============================================================================

Geslaagd

De controle voldoet volledig aan de vereisten.

Waarschuwing

Er werd een afwijking vastgesteld die de volgende stap niet noodzakelijk onbetrouwbaar maakt. De afwijking wordt in het rapport opgenomen.

Mislukt

De controle levert geen betrouwbaar resultaat op of de hardware werkt niet volgens de verwachte configuratie. Afhankelijke testen worden niet uitgevoerd.

Kritieke fout

De verdere uitvoering kan schade, onbetrouwbare resultaten of verlies van controle veroorzaken. De validatie wordt onmiddellijk gestopt.

Niet toepasselijk

De controle hoort niet bij de geïnventariseerde configuratie.

Niet automatisch controleerbaar

De software kan het resultaat niet zelf waarnemen. Een interactieve of visuele controle is vereist.

==============================================================================
6.4) Inhoud van een foutmelding
==============================================================================

Iedere foutmelding bevat minstens:

• het betrokken onderdeel;
• de uitgevoerde controle;
• het verwachte resultaat;
• het vastgestelde resultaat;
• mogelijke fysieke oorzaken;
• de eerstvolgende veilige herstelactie;
• de stap vanaf waar opnieuw gevalideerd moet worden.

==============================================================================
6.5) Veilig onderbreken
==============================================================================

De gebruiker kan de validatie afbreken via de actieve invoermethode.

Bij een veilige onderbreking:

• worden actieve scherm- en meettests beëindigd;
• worden uitgangen in een veilige toestand geplaatst;
• wordt de onvoltooide validatie niet als geslaagd opgeslagen;
• blijft de laatst volledig geldige configuratie herkenbaar;
• wordt vermeld vanaf welke ronde hervat kan worden.

==============================================================================
6.6) Validatierapport
==============================================================================

Na iedere afgeronde of afgebroken validatie wordt een rapport opgebouwd.

Het rapport bevat minstens:

• document- en softwareversie;
• Hardware Shield-versie;
• geselecteerd moederbord;
• volledige geïnventariseerde hardwareconfiguratie;
• resultaat van iedere automatische controle;
• resultaat van iedere interactieve functietest;
• waarschuwingen;
• fouten;
• overgeslagen en niet-toepasselijke controles;
• algemene eindstatus.

Mogelijke eindstatussen zijn:

• Hardware volledig gevalideerd;
• Hardware gevalideerd met waarschuwingen;
• Hardwarevalidatie mislukt;
• Hardwarevalidatie afgebroken;
• Alleen geïnventariseerd, nog niet gevalideerd.

==============================================================================
6.7) Voorwaarde voor volledige goedkeuring
==============================================================================

Het GroeiAcademie Stimulus Hardware Shield v0.10.1 is uitsluitend volledig gevalideerd wanneer:

• de visuele inventarisatie zonder tegenstrijdigheden voltooid is;
• alle verplichte automatische controles geslaagd zijn;
• alle toepasselijke interactieve functietesten geslaagd zijn;
• geen kritieke fout actief is;
• de opgeslagen configuratie en controlesom geldig zijn.

Pas daarna wordt de status Volledig gevalideerd opgeslagen.
