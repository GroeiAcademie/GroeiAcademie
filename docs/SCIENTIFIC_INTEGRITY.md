# Wetenschappelijke integriteit

## Doel

Dit document bepaalt hoe GroeiAcademie FrameWork omgaat met metingen, analyses, interpretaties en claims.

## Meetniveaus

Elke module onderscheidt waar mogelijk:

1. fysiek of fysiologisch verschijnsel;
2. sensorsignaal;
3. ADC- of digitale ruwe data;
4. voorbewerking;
5. berekende meetwaarde;
6. gedetecteerd patroon;
7. menselijke of algoritmische interpretatie;
8. conclusie.

Een afgeleide waarde mag niet als rechtstreeks gemeten worden voorgesteld.

## Elektronische meetketen

Resultaten kunnen worden beïnvloed door:

- sensorvariant en toleranties;
- voedingsspanning;
- spanningsdeler en componentwaarden;
- ADC-resolutie en referentie;
- bedrading en contactweerstand;
- mechanische plaatsing;
- samplefrequentie;
- filtering en timing;
- temperatuur en omgeving;
- kalibratie.

Daarom hoort het elektronische schema bij de meetmethode en niet alleen bij de bouwinstructie.

## Stimulus en kracht

De huidige Stimulusmodule registreert analoge drukwaarden. Zonder kalibratie tegen een bekende mechanische referentie mogen die waarden niet als absolute kracht in newton of gramkracht worden gerapporteerd.

Comfort- en veiligheidsgrenzen in ADC-eenheden zijn softwaregrenzen binnen de huidige opstelling. Zij vormen geen medische of universeel fysiologische norm.

## Emoties

Fysiologische of gedragsmatige signalen kunnen samenhangen met emotionele processen. Zij bewijzen op zichzelf niet welke specifieke emotie iemand ervaart. Emotie-observatie moet duidelijk onderscheiden wat gemeten, afgeleid en geïnterpreteerd wordt.

## Breinclaims

Uitspraken over hersenactiviteit vereisen een passende directe meetmethode en een gevalideerd protocol. Hartslag, ademhaling of huidgeleiding zijn geen rechtstreekse hersenmeting.

## Validatie

Voor elke meetmodule worden vastgelegd:

- doel en onderzoeksvraag;
- gebruikte hardware en schema;
- sensorplaatsing;
- kalibratie;
- samplefrequentie;
- filters en algoritmen;
- foutbronnen;
- referentiemethode;
- ondersteunde Arduino Uno R3-vormfactorborden;
- softwareversie;
- grenzen van interpretatie.

## Resultaten

Negatieve, neutrale en onverwachte resultaten worden niet weggelaten. Wijzigingen aan algoritmen of drempelwaarden worden gedocumenteerd wanneer zij vergelijkbaarheid beïnvloeden.

## Gezondheid en privacy

Het framework is geen medisch hulpmiddel tenzij een specifieke uitvoering afzonderlijk is ontworpen, gevalideerd en gecertificeerd. Persoonsgebonden meetgegevens worden alleen met passende toestemming verzameld, bewaard en gedeeld.
