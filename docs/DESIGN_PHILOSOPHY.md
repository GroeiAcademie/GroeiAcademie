# Ontwerpfilosofie

## Menselijk begrijpelijk

De API, voorbeelden, schema's en feedback moeten begrijpelijk zijn voor zowel technische als inhoudelijke gebruikers. Een gebruiker moet kunnen zien wat wordt gemeten, hoe de hardware is aangesloten en hoe een resultaat tot stand komt.

## Vaardigheid vóór sensor

Modules worden ontworpen rond een leer-, meet- of onderzoeksdoel. Een sensor is een technische afhankelijkheid, niet de hoofdstructuur van het toepassingsgebied.

## Modulair en geïntegreerd

Elke module heeft een afgebakende verantwoordelijkheid en werkt binnen één Arduino-library samen met andere modules.

## Kleine kern

De kern blijft zo onafhankelijk mogelijk. Concrete hardwarelibraries worden alleen gebruikt via duidelijke, beperkte integraties.

## UNO R3 als discipline

Ondersteuning van UNO R3 dwingt zuinig gebruik van flash, SRAM en processortijd af. Zwaardere functionaliteit wordt expliciet gemarkeerd en mag de basisondersteuning niet stilzwijgend verbreken.

## Transparantie

Documentatie onderscheidt:

- ruwe sensorwaarden;
- voorbewerking;
- berekende meetwaarden;
- gedetecteerde patronen;
- menselijke interpretaties;
- conclusies.

Aannames, kalibratie, foutmarges en beperkingen worden zichtbaar gemaakt.

## Elektronica als onderdeel van de specificatie

Software zonder reproduceerbare bedrading is geen volledige meetopstelling. Daarom hoort bij ieder geïmplementeerd toepassingsgebied een raadpleegbaar aansluitschema met componentwaarden, pinnen, voeding en kalibratie.

## Didactische voorbeelden

Een voorbeeld demonstreert bij voorkeur één duidelijk concept. Samengestelde voorbeelden zijn aanvullend en vervangen de afzonderlijke basisvoorbeelden niet.

## Communitygericht

Een bijdrage moet begrijpelijk, testbaar, bespreekbaar en in lijn met de projectconstitutie zijn.
