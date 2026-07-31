# Bijdragen aan het GroeiAcademie FrameWork

Dank je voor je interesse om bij te dragen aan het **GroeiAcademie FrameWork**.

Bijdragen kunnen bestaan uit code, `examples`, documentatie, vertalingen, hardware- en elektronicatests, meetresultaten, onderzoek, meetprotocollen, reviews, bugmeldingen en communityondersteuning. Elke bijdrage is welkom, van een typfix tot een nieuwe hardwaremodule — de verwachtingen hieronder schalen mee met de omvang van je bijdrage.

## Snel starten: kleine bijdragen

Gaat het om een  typfix, een verbeterde zin in de documentatie, een kleine verduidelijking in een bestaand `example`, of een vertaling? Dien gewoon een Pull Request in. Een korte, werkende wijziging met een duidelijke titel volstaat — de uitgebreide checklist verderop in dit document is daar niet op van toepassing. 

Twijfel je of iets "klein" is? Dien de Pull Request gewoon in. De maintainers laten je weten of er meer nodig is.

Op zoek naar een instapvriendelijke eerste bijdrage? Kijk naar issues met het label `good first issue`.

## Hoe kun je bijdragen?

Wijzigingen aan code, `examples` en documentatie: via een Pull Request.

Bugs rapporteren: via een GitHub Issue met het formulier `Bug report`.

Reviews van code, documentatie of hardware: via een Pull Request review of een GitHub Issue met het formulier `Review request`.

Voorstellen aangaande ontbrekende functionaliteit: via het formulier `Feature request`.

Hardwaretests en meetresultaten delen: via een GitHub Discussion, een Pull Request wanneer er ook bestanden worden toegevoegd, of rechtstreeks overleg met de maintainers via `support@groeiacademie.be`.

Grote wijzigingen aan de architectuur, publieke API, hardwareondersteuning of projectstructuur: eerst voorstellen via een GitHub Discussion en pas uitvoeren na akkoord van de maintainers. Voor rechtstreeks overleg met de maintainers kun je contact opnemen via `support@groeiacademie.be`.

### Hardware

Bijdragen aan fysieke onderdelen, sensoren, ontwikkelborden, schermen, connectoren, behuizingen en testopstellingen.

### Elektronica

Bijdragen aan schema's, bedrading, voedingen, spanningsniveaus, signaalconditionering, PCB's en elektronische foutanalyse.

### Testers

Testen van code, `examples`, hardware en combinaties daarvan op ondersteunde boards en configuraties. Ook een enkel testresultaat op één board is een waardevolle bijdrage — dat hoeft geen volledige validatie te zijn.

### Vertalingen

Vertalen en controleren van documentatie, meldingen, `examples` en andere gebruikersteksten.

### Onderzoek en meetprotocollen

Bijdragen aan onderzoeksvragen, meetmethodes, kalibratie, validatie, interpretatiegrenzen en reproduceerbare testprotocollen.

### Reviews

Inhoudelijk beoordelen van code, documentatie, hardware, elektronica, `examples` en meetprotocollen.

### Bugmeldingen

Duidelijk beschrijven van fouten, onverwacht gedrag of problemen, met voldoende informatie om het probleem te kunnen onderzoeken en reproduceren. Een duidelijke melding zonder oplossing is al een waardevolle bijdrage.

## Basisvoorwaarden voor een bijdrage

Deze checklist is niet vereist voor kleine bijdragen (zie hierboven).

Bijdragen aan code en `examples` worden volledig, werkend, getest en gedocumenteerd ingediend. De contributor vermeldt duidelijk op welke ondersteunde boards, hardware en configuraties de bijdrage is getest en voegt de testresultaten toe. Wanneer dat relevant is, mag ook een video van de uitgevoerde tests worden toegevoegd.

Een bestaande functienaam wijzigen, een parameter toevoegen of verwijderen of bestaand gedrag aanpassen: eerst voorstellen via een GitHub Discussion.

Een bestaand `example` wordt niet aangepast om ander gedrag te tonen. Daarvoor wordt een nieuw `example` ingediend, met een duidelijke verwijzing naar het bestaande `example` waarop het gebaseerd is.

Elke bijdrage volgt de bestaande naamgeving, code-opmaak, commentaarregels en mappenstructuur van het GroeiAcademie FrameWork. Afwijkingen hierop kunnen worden voorgesteld via een GitHub Discussion.

Wanneer een bijdrage nieuwe code, hardwareondersteuning of functionaliteit toevoegt, past de contributor ook de bijhorende documentatie aan en voegt die toe.

Een contributor dient alleen materiaal in dat hij zelf heeft gemaakt of waarvoor hij uitdrukkelijk toestemming heeft om het te gebruiken. Dit geldt onder meer voor code, schema's, afbeeldingen, meetgegevens, teksten, `examples` en onderdelen van andere projecten.

Elke contributor bevestigt dat zijn bijdrage mag worden opgenomen en verspreid onder de licentie van het GroeiAcademie FrameWork.

Elke bijdrage die meer omvat dan een kleine tekstuele wijziging, bevat duidelijke testinstructies, zodat de maintainers de werking zelf kunnen herhalen en de testresultaten onafhankelijk kunnen controleren.

Bij een codewijziging vermeldt de contributor welke bestanden, functies en `examples` door de wijziging worden geraakt, en welke bestaande werking mogelijk wordt beïnvloed — ook wanneer dat effect niet de bedoeling is.

Een bijdrage die nieuwe dependencies toevoegt, wordt eerst voorgesteld via een GitHub Discussion. Een nieuwe dependency wordt alleen aanvaard wanneer die aantoonbaar noodzakelijk is en er binnen het bestaande GroeiAcademie FrameWork geen bruikbaar alternatief bestaat.

Een bijdrage die het geheugenverbruik, de compilatietijd of de prestaties merkbaar beïnvloedt, documenteert die impact expliciet. Elke technische bijdrage gaat zo efficiënt mogelijk om met het beschikbare geheugen, de opslagruimte en de processortijd.

Een bijdrage die nieuwe hardware ondersteunt, bevat een volledig aansluitschema, de gebruikte spanningen, de pinbezetting en een duidelijke beschrijving van de testopstelling.

Technische bijdragen volgen de gangbare best practices voor foutafhandeling: verwachte fouten, ongeldige invoer en mislukte initialisaties worden waar relevant duidelijk en betrouwbaar afgehandeld, zonder onnodige complexiteit toe te voegen.

Een technische bijdrage breidt bestaande tests uit of voegt nieuwe tests toe wanneer dat relevant is.

Alle compilerwaarschuwingen worden gerapporteerd en tot een minimum beperkt. Nieuwe waarschuwingen die door de bijdrage ontstaan, worden opgelost voordat de bijdrage kan worden aanvaard. Bestaande waarschuwingen die niet door de bijdrage zijn veroorzaakt, worden duidelijk vermeld.

Bij elke technische bijdrage vermeldt de contributor de exacte versienummers van de Arduino IDE, het boardpakket en alle gebruikte externe libraries waarmee de bijdrage werd getest.

Een technische bijdrage toont waar relevant aan dat bestaande functionaliteit niet wordt gebroken, door de relevante bestaande tests opnieuw uit te voeren en de resultaten toe te voegen.

Een technische bijdrage wordt getest op de officieel ondersteunde boards die de contributor zelf ter beschikking heeft. De contributor vermeldt duidelijk welke boards zijn getest en voor welke boards nog validatie nodig is. Voor ontbrekende tests kan een beroep worden gedaan op contributors die zich als tester aanbieden.

Een bijdrage die nieuwe configuratie-opties, `#define`s of instellingen toevoegt, bevat veilige standaardwaarden zodat bestaande gebruikers niets hoeven aan te passen, en documenteert duidelijk waar die instelling thuishoort en wat de gevolgen van elke mogelijke waarde zijn.

Bijdragen volgen waar mogelijk de taal en terminologie van het bestaande bestand. Wanneer dat niet haalbaar is, kunnen de maintainers de tekst redactioneel aanpassen.

## Wat gebeurt er met je Pull Request?

Een Pull Request is een voorstel tot wijziging. De bijdrage wordt niet automatisch opgenomen in het GroeiAcademie FrameWork.

De maintainers:

1. bekijken de inhoud en het doel van de bijdrage;
2. controleren of de wijziging past binnen de architectuur, documentatieafspraken en kwaliteitsvereisten;
3. testen de wijziging wanneer dat nodig en haalbaar is;
4. geven feedback of vragen aanpassingen;
5. beslissen uiteindelijk of de bijdrage wordt samengevoegd.

Een bijdrage kan worden aanvaard wanneer ze inhoudelijk past binnen het doel en de architectuur van het GroeiAcademie FrameWork, voldoende getest en gedocumenteerd is (in verhouding tot haar omvang, zie hierboven), geen onopgeloste kritieke problemen bevat en door de maintainers is goedgekeurd.

Wordt een bijdrage niet aanvaard, dan kan je de maintainers vragen waarom, en waar mogelijk wat er nodig zou zijn om ze wel aanvaardbaar te maken.

Bij technische vragen of twijfel kun je contact opnemen via `support@groeiacademie.be`. Technische bijdragen worden opgevolgd door Alex Peeters als maintainer.
