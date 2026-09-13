# VEILIGHEID.md

Werkdocument, voorbereidend werk voor het GroeiAcademie FrameWork. Dit document behandelt elektrische veiligheid van elektroden op de huid, isolatie, lekstroom en de relevante medische normen. Geen enkel ander studiedocument uit de zip behandelt dit; het is met websearch samengesteld.

**Belangrijke afbakening, geen aanname:** dit is geen certificering en geen juridisch advies. Ik ben geen elektrotechnisch of medisch bevoegd persoon. Alles hieronder is publiek toegankelijke technische informatie, samengevat en met bron. Voor je met deze hardware op mensen meet, moet iemand met de juiste bevoegdheid dit nakijken.

---

## 1. Waarom dit relevant is voor dit project

Toekomstige toepassingsgebieden binnen het GroeiAcademie FrameWork gaan gebruik maken van elektroden die rechtstreeks op de huid geplakt worden: GSR-vingerelektroden, ECG-elektroden op de borst (AD8232, BioAmp EXG Pill als ECG), EMG-elektroden op frons, kaak, schouder of specifieke gezichtsspieren. Zodra een elektrode op de huid zit én die elektrode elektrisch verbonden is met een microcontroller die op zijn beurt via USB met een computer verbonden kan zijn, ontstaat een mogelijk pad waarlangs stroom vanuit het lichtnet, via de computer, via de microcontroller, naar de huid van de proefpersoon kan lopen. Dat pad bestaat ongeacht of het project ooit als medisch hulpmiddel verkocht wordt.

## 2. De relevante norm

De internationale norm voor elektrische medische apparatuur is **IEC 60601-1**, "Medical Electrical Equipment: Part 1: General Requirements for Safety and Essential Performance". Voor ECG specifiek bestaat een aanvullende deelnorm, **IEC 60601-2-25**.

De norm classificeert het deel van het toestel dat met de patiënt in contact komt (het "applied part") in drie types:

- **Type B**: niet-cardiale toepassing, mag geaard zijn. Minst strenge categorie.
- **Type BF**: "body floating", elektrisch losgekoppeld (floating) van de aarde, voor toepassingen die met de patiënt in contact komen maar niet rechtstreeks op het hart of via de bloedbaan werken. Dit is de categorie waarin GSR-, EMG- en de meeste ECG-toepassingen in dit project vallen.
- **Type CF**: "cardiac floating", de strengste categorie, voor toepassingen met een directe cardiale weg.

## 3. Concrete lekstroomlimieten uit de norm

Voor Type B en Type BF (de categorie die hier van toepassing is):

- Patiëntlekstroom: **100 µA (0,1 mA) onder normale omstandigheden**, oplopend tot **500 µA (0,5 mA) bij één enkele storing** (single fault condition).
- Voor Type CF liggen deze limieten tien keer strenger: 10 µA normaal, 50 µA bij storing.

Ter referentie, waarom dit zo streng is: onderzoek toont aan dat stromen vanaf ongeveer **10 tot 100 µA al kamerfibrillatie kunnen veroorzaken wanneer ze rechtstreeks op de hartspier inwerken**. Dat is de reden waarom cardiale toepassingen (Type CF) de strengste limieten krijgen.

Daarnaast gelden aparte limieten voor de omhulsel- en aardlekstroom van het toestel zelf (los van de patiënt): typisch 100 µA normaal en 500 µA bij storing voor de omhulsellekstroom, en 500 µA normaal/1 mA bij storing voor de aardlekstroom.

## 4. Wat dit concreet betekent voor dit project

Uit praktijkbronnen over hobbymatige AD8232-opstellingen (dezelfde module die dit project voor meerdere toepassingsgebieden inzet), samengevat:

- **Nooit de microcontroller met elektroden aangesloten gebruiken terwijl die via USB met een computer verbonden is die op het stopcontact staat.** Een gewone pc-voeding voldoet niet aan IEC 60601-1. Zolang het toestel op batterijen of een powerbank werkt, is er geen elektrisch pad naar het lichtnet, en dat is de kern van de bescherming.
- Bij de AD8232 zelf zorgen de ingebouwde 180 kΩ-weerstanden voor een intrinsieke stroombegrenzing: bij een falende chip op 3,3V is de maximale foutstroom ongeveer 18 µA, ruim onder de 100 µA-limiet voor Type BF. Dat is echter een eigenschap van deze specifieke chip, geen garantie die automatisch voor andere sensoren (BioAmp EXG Pill, Grove GSR, MCU-6701) geldt; dat is per module apart te controleren, en dat heb ik niet gedaan.
- Een "applied part" moet volgens de norm dubbel geïsoleerd zijn tot 4000 VAC. Geen van de bordjes in de sensorinventaris is daarvoor ontworpen of gecertificeerd.
- Als er toch met een computer gelogd moet worden (voor dataopslag, wat bij meetprotocollen vaak een vereiste is), bestaat er een praktische tussenoplossing: een **galvanisch geïsoleerde USB-verbinding**, gebaseerd op bijvoorbeeld de ADuM3160- of ADuM4160-chip, die de datalijnen en de voeding tussen computer en microcontroller scheidt met een isolatiewaarde van typisch 1500 tot 5000 V. Dit is geen IEC 60601-1-certificering, maar het sluit wel het meest voor de hand liggende risicopad af.

## 4bis. Galvanische scheiding als alternatief voor de batterij-regel

**[TOEGEVOEGD DOOR OPUS]** De batterij-regel uit punt 4 is niet de enige manier om het risicopad naar het net af te sluiten. Galvanische scheiding doorbreekt datzelfde pad elektrisch, ook wanneer het toestel wél op een net-hangende computer aangesloten is. Twee technieken die dit doen:

- **Optocouplers** (optische koppeling): een LED aan de ene kant, een fototransistor aan de andere, geen elektrische verbinding tussen beide zijden, enkel licht steekt de barrière over.
- **iCoupler-chips** zoals de ADuM3160/ADuM4160 die eerder genoemd zijn: geen licht maar een kleine transformator, functioneel hetzelfde resultaat, een elektrisch onderbroken pad met enkel signaaloverdracht.

Als zo'n isolator tussen de computer en de microcontroller zit, en die isolator dekt zowel de datalijnen als de voeding, is de elektrische verbinding naar het net effectief doorbroken. De regel "nooit op een net-hangende computer" is dan in principe niet meer nodig.

**Twee kanttekeningen:**

1. De isolatiewaarde van consumentenmodules (1500 tot 5000V bij de ADuM-isolators) is een elektrische spanningswaarde, geen IEC 60601-1-certificering. De medische norm vraagt ook specifieke kruipafstanden op de printplaat en getest gedrag bij één enkele storing; dat garandeert een USB-isolator op zich niet.
2. De isolatie moet **elk** pad dekken, niet enkel USB. Als het sensorbordje apart gevoed wordt met een adapter die niet ook geïsoleerd is, blijft daar een onbeschermd pad over, ook al is de USB-kant wel geïsoleerd.

**Herziene richtlijn 1, ter vervanging van de eerdere formulering in punt 6:** elke opstelling met huidcontact werkt óf op batterij/powerbank, óf via een galvanisch geïsoleerde verbinding die alle paden (data én voeding) dekt, nooit rechtstreeks op een net-hangende computer zonder isolatie.

## 4ter. Druksensor versus elektrode, twee verschillende risiconiveaus

**[TOEGEVOEGD DOOR OPUS]** Niet elke sensor met huidcontact valt onder hetzelfde risiconiveau. Er is een reëel onderscheid tussen twee categorieën in dit project:

**Categorie A, elektroden (biopotentiaal-meting)**: GSR-vingerelektroden, ECG-elektroden (AD8232, BioAmp EXG Pill als ECG), EMG-elektroden (BioAmp EXG Pill als EMG, op frons, kaak, schouder of specifieke gezichtsspieren). Deze vormen elektrisch deel uit van de meetketen zelf: er loopt een meetstroom door het lichaam, en bij een storing kan daar in principe ook een foutstroom lopen. Dit is exact het scenario waarvoor de IEC 60601-1 Type BF-lekstroomlimieten (sectie 3 hierboven) geschreven zijn. Sectie 3 en 4 (en 4bis) van dit document zijn op deze categorie van toepassing.

**Categorie B, druksensoren (mechanische meting)**: FSR400/402, RFP602, en de HX711 met loadcells. Deze meten een weerstands- of gewichtsverandering onder mechanische druk. Er is contact met de huid, maar geen elektrisch pad de huid in als onderdeel van de meting zelf, enkel een spanningsdeler of brugschakeling die uitgelezen wordt. Het lekstroomrisico dat specifiek voor "applied parts" geldt, is hier veel kleiner, want deze sensoren zijn niet ontworpen om stroom door het lichaam te sturen.

**Wat wel voor beide categorieën geldt**: elk USB-verbonden bordje kan in theorie een pad naar het net vormen als er iets defect raakt in de voeding zelf, ongeacht of het bordje een elektrode of een druksensor aanstuurt. De algemene regel uit sectie 6 (batterij, of volledig geïsoleerde verbinding) blijft daarom voor beide categorieën gelden, maar de ernst van het risico bij falen is voor Categorie B aanzienlijk lager dan voor Categorie A.

**Concreet voor de bestaande code**: de Stimulus-module gebruikt vandaag al FSR402 en RFP602, dus uitsluitend Categorie B. Zodra Stimulus of een andere module een sensor uit Categorie A gaat aansturen, wordt de volledige lekstroomanalyse uit sectie 3 relevant, niet enkel de algemene regel.

## 5. Wat dit niet oplost

- Isolatieafstanden op de printplaat zelf (creepage/clearance) zijn bij hobbymodules niet gegarandeerd. Een bron noemt expliciet dat de 0603-weerstanden op een AD8232-module onvoldoende kruipafstand hebben voor medische isolatie.
- Connector-type telt mee: een module met een 3,5mm jack-aansluiting kan in theorie in een stopcontact gestoken worden. Dat is een ontwerpfout die softwarematig niet op te lossen is.
- Single-fault-gedrag (wat gebeurt er als één component faalt) is voor geen van de sensoren in de inventaris gedocumenteerd of getest.

## 6. Voorstel voor een minimale, niet-gecertificeerde veiligheidsrichtlijn voor dit project

Dit is geen norm, enkel een praktisch voorstel op basis van bovenstaande bronnen, ter beoordeling:

1. Elke opstelling met elektroden op de huid werkt uitsluitend op batterij of powerbank, nooit op een netvoeding of een aan het stopcontact hangende computer.
2. Als datalogging naar een computer nodig is, gebeurt dat via een galvanisch geïsoleerde USB-verbinding, of draadloos (WiFi/Bluetooth), nooit via een rechtstreekse USB-kabel naar een op het net aangesloten computer.
3. Elektrodekabels en connectoren mogen niet in een stopcontact passen of daar toevallig op lijken.
4. Voor elke nieuwe sensor die met de huid in contact komt, wordt nagegaan wat de foutstroom bij falen is, naar analogie van de AD8232-berekening hierboven, voor die sensor als klaar voor implementatie beschouwd wordt.
5. Dit project claimt op geen enkel moment IEC 60601-1-conformiteit of medische certificering, en dat wordt ook zo naar gebruikers gecommuniceerd.

---

## Bronnen

- IEC 60601-1, lekstroomlimieten en applied part-classificatie: meerdere technische samenvattingen (Astrodyne TDI, Advanced Energy, MDDI, Mean Well, xppower.com)
- AD8232-specifieke praktijkrichtlijnen: ShillehTek, Probots, CircuitSchools, duino4projects/duino-projects
- USB-isolatie: Analog Devices ADuM4160-datasheet, Adafruit USB Isolator, Hackaday

Geen van deze bronnen is een officiële norminstantie; voor de volledige, juridisch geldende tekst van IEC 60601-1 is de officiële IEC-publicatie nodig, die niet vrij toegankelijk is.
