# Project Constitution

## 1. Naam en karakter

De officiële projectnaam is **GroeiAcademie Framework**.

Het GroeiAcademie Framework is één geïntegreerd, modulair open-sourceframework voor het objectief meten, oefenen, ontwikkelen en onderzoeken van menselijke vaardigheden. Arduino is het eerste implementatieplatform. De technologie ondersteunt de missie en is geen doel op zich.

## 2. Missie

Het GroeiAcademie Framework bestaat enerzijds om beoefenaars van Neuro Linguïstisch Programmeren te helpen hun basisvaardigheden veel sneller naar een hoger niveau te brengen. Anderzijds maakt het framework het mogelijk om de impact van NLP-technieken op het brein en op waarneembare fysiologische processen te onderzoeken en zichtbaar te maken voor onderzoekers, biofeedback-onderzoekers, NLP-beoefenaars, NLP-trainers en leken.

Het framework wil de afstand verkleinen tussen wat iemand denkt waar te nemen of beheersen en wat objectief kan worden gemeten. Het ondersteunt leren, oefenen, kalibreren, onderzoeken en aantonen.

## 3. Visie

> Waar subjectieve ervaringen en objectief meetbare vaardigheden elkaar ontmoeten, dankzij een modulair open-sourceframework waarmee menselijke vaardigheden objectief gemeten, geoefend, ontwikkeld en onderzocht kunnen worden.

Het project brengt subjectieve ervaring en objectieve feedback samen zonder één van beide te reduceren. Ervaring blijft betekenisvol. Meting voegt controleerbare informatie toe.

## 4. Centraal principe

> **Meten is weten, in plaats van denken te weten.**

Dit motto betekent dat aannames zo veel mogelijk worden getoetst aan meetgegevens. Het betekent niet dat elke menselijke ervaring volledig tot één meetwaarde kan worden herleid.

## 5. Doelgroepen

De primaire doelgroepen zijn:

1. onderzoekers
2. biofeedback-onderzoekers
3. NLP-beoefenaars
4. NLP-trainers

Het framework richt zich eveneens tot leken. Zij moeten op een begrijpelijke manier kunnen ervaren en onderzoeken wat er tijdens een oefening of techniek verandert.

## 6. Unieke positie

Het GroeiAcademie Framework is geen losse verzameling sensoren. Het beoogt een modulair totaalpakket rond menselijke basisvaardigheden. Elke module verbindt een concrete vaardigheid met reproduceerbare oefenscenario's, objectieve metingen en bruikbare feedback.

De eerste toepassing ligt binnen NLP. De architectuur blijft ruim genoeg voor verwante toepassingen in biofeedback, observatie, communicatie, coaching, leren en onderzoek.

## 7. Eerste toepassingsgebieden

### 7.1 Stimulusvaardigheden

De eerste module ondersteunt het leren zetten en activeren van een unieke stimulus met een reproduceerbare duur, kracht en timing. De module meet vingertikken met één tot vier vingers, ondersteunt vier basisscenario's en beoordeelt het activeringsmoment.

### 7.2 Emotie-observatie

Deze module helpt deelnemers sneller kenmerken van emoties leren observeren. De eigen observatie wordt naast objectieve meetgegevens geplaatst. Het framework maakt duidelijk onderscheid tussen gemeten signalen, afgeleide patronen en interpretaties.

### 7.3 Ademhaling, hartslag en observatie

Deze module volgt ademhaling en hartslag tijdens oefeningen. Wanneer iemand gevraagd wordt trager in te ademen, kan de begeleider controleren of de waarneming overeenkomt met de meting. Hetzelfde leerprincipe wordt uitgebreid naar andere observatievaardigheden.

## 8. Open-sourcebeginselen

Iedereen mag het framework gebruiken. Verbeteringen aan het framework horen terug te vloeien naar de community. De definitieve licentie moet deze wederkerigheid juridisch passend ondersteunen.

Bijdragen kunnen bestaan uit code, documentatie, vertalingen, voorbeelden, nieuwe modules, hardwaretests, meetprotocollen en code reviews.

## 9. Wetenschappelijke integriteit

Het framework is niet gebouwd om vooraf vastgelegde overtuigingen te bevestigen. Het moet metingen mogelijk maken die transparant, reproduceerbaar en controleerbaar zijn.

Daarom geldt:

- ruwe meetgegevens blijven onderscheiden van interpretaties
- aannames en beperkingen worden expliciet vermeld
- conclusies mogen niet verder gaan dan de gebruikte meetmethode toelaat
- biofeedbackgegevens vormen geen medische diagnose
- claims over hersenactiviteit, emoties of effectiviteit vereisen passende validatie
- negatieve, neutrale en onverwachte resultaten zijn even waardevol als bevestigende resultaten

## 10. Ontwerpprincipes

Het framework is:

- modulair
- menselijk begrijpelijk
- didactisch bruikbaar
- technisch transparant
- reproduceerbaar
- uitbreidbaar
- zo onafhankelijk mogelijk van concrete hardware
- bruikbaar op beperkte microcontrollers

De publieke API wordt pas uitgebreid wanneer de verantwoordelijkheid, naamgeving en samenwerking met bestaande modules duidelijk zijn.

## 11. Technische basis

De eerste officiële ontwikkelomgeving is Arduino IDE.

De eerste officieel ondersteunde boards zijn:

- Arduino UNO R3
- Arduino UNO R4

Het framework blijft één geïntegreerde library. De bestaande functionele hoofdindeling blijft het uitgangspunt:

```text
lib/
├── GroeiAcademie.h
├── Hulpmiddelen/
├── Sturingen/
├── Systeem/
├── Toepassingsgebieden/
└── Uitbreidingskaarten/
```

Een eventuele aanpassing naar de formele Arduino-librarystructuur wordt als afzonderlijke architectuur- en releasebeslissing vastgelegd. Functionele modules worden niet willekeurig als losse libraries afgesplitst.

## 12. Taal

De officiële documentatie wordt eerst in het Nederlands ontwikkeld en inhoudelijk gestabiliseerd. Vertalingen volgen vanuit de gevalideerde Nederlandstalige bron.

## 13. Community boven commercie

Het GroeiAcademie Framework mag nooit een commerciële instelling worden ten koste van de community.

Commerciële diensten, hardware, trainingen of ondersteuning mogen het open project ondersteunen. Zij mogen de vrije toegang, wederkerigheid, transparantie of invloed van de community niet ondergraven.

## 14. Succes

Het project is inhoudelijk geslaagd wanneer NLP-beoefenaars aantoonbaar sneller en consistenter basisvaardigheden ontwikkelen en dit bevestigen in hun feedback, terwijl onderzoekers en leken de resultaten op een transparante manier kunnen onderzoeken.

Downloads, sterren en zichtbaarheid zijn ondersteunende indicatoren. Zij zijn geen vervanging voor leerwaarde, meetkwaliteit of gemeenschapswaarde.

## 15. Wijziging van deze constitutie

Wijzigingen aan missie, kernwaarden, open-sourcebeginselen of de verhouding tussen community en commercie vereisen een expliciete, gedocumenteerde beslissing. Technische keuzes mogen nooit stilzwijgend de grondbeginselen van het project veranderen.
