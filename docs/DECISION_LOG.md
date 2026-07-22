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

**Besluit:** iedere contributor behoudt het copyright op zijn of haar eigen bijdragen. GroeiAcademie Framework blijft beheerd door GROEI ACADEMIE BV.

## D018 Ondersteuningsniveaus

**Besluit:** hardware en functionaliteit worden ingedeeld als Officieel ondersteund, Community ondersteund of Experimenteel. Voor elk niveau worden de verwachtingen duidelijk omschreven.

## D019 API-stabiliteit en Semantic Versioning

**Besluit:** `0.x` zijn ontwikkelversies waarin de API nog kan wijzigen. Vanaf `1.0.0` gelden maximale API-stabiliteit, het vastgelegde deprecationbeleid en Semantic Versioning 2.0.0.
