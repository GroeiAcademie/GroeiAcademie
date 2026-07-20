# Decision Log

Dit logboek bevat kernbeslissingen. Nieuwe beslissingen krijgen een nieuw nummer en vervangen oudere besluiten niet stilzwijgend.

## D001 Projectnaam

**Besluit:** GroeiAcademie Framework.

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

**Besluit:** het project streeft naar een licentie waarbij verbeteringen aan de kern terugvloeien naar de community. De definitieve juridische licentie is nog niet gekozen.

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
