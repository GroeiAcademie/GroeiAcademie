#ifndef STIMULUS_CONFIG_H
#define STIMULUS_CONFIG_H

// ============================================================================
// Instellingen die vooraf vaststaan
// ============================================================================

// STIMULUSVERSIE
#define STIMULUS_BASIC               1 // basismeting: tiktijd en gemiddelde tikkracht
#define STIMULUS_EXTENDED            2 // uitgebreide meting: ook piekkracht, tijd tot piek, opbouw- en afbouwsnelheid

// MARGE_FACTOR t.o.v. de nulmeting
// 1 = vb: ±30% voor zowel de onder- als de bovengrens
// 2 = vb: 30% in totaal, dus 15% voor de onder- en 15% voor de bovengrens, ofwel ±15% voor zowel de onder- als de bovengrens
// 4 = vb: 30% in totaal, dus 7,5% voor de onder- en 7,5% voor de bovengrens, ofwel ±7,5% voor zowel de onder- als de bovengrens
#define DEFAULT_MARGE_FACTOR          1

// Standaardwaarden (zie Stimulus.cpp)
#define DEFAULT_TOEGESTANE_MARGE_TIKTIJD      30
#define DEFAULT_TOEGESTANE_MARGE_TIKKRACHT    25

#define DEFAULT_TOEGESTANE_MARGE_SIMULTANE_STARTTIJD_MS 100UL

// Minimale en maximale factor t.o.v. de nulmeting voor de duur van stap 3 (de lange aanhoudtik)
#define INSTORTEND_MINIMALE_FACTOR  3 // kortste toegelaten duur van stap 3 (in veelvouden van de nulmeting)
#define INSTORTEND_MAXIMALE_FACTOR  7 // langste toegelaten duur van stap 3 (in veelvouden van de nulmeting)
// OPGEPAST: INSTORTEND_MINIMALE_FACTOR + INSTORTEND_MAXIMALE_FACTOR moeten een EVEN getal zijn!

#define INSTORTEND_TOV_NULMETING          0
#define INSTORTEND_MOEILIJKHEIDSGRAAD_1  -1
#define INSTORTEND_MOEILIJKHEIDSGRAAD_2  -2
#define INSTORTEND_MOEILIJKHEIDSGRAAD_3  -3

// Drempel om elektronische ruis op de sensoren te filteren)
#define TIK_MINIMALE_DRUKWAARDE ADC(50)            // from 0 to 50 -> no pressure

// De minimale druk die nodig is om een bruikbare stimulus te registreren)
#define TIKKRACHT_MINIMALE_COMFORT_GRENS ADC(200)  // from 200 to 499 -> light squeeze

// De maximale druk om blessures of blauwe plekken te voorkomen)
#define TIKKRACHT_MAXIMALE_COMFORT_GRENS ADC(800)  // from 800 to 1023 -> big squeeze

// De gewenste tikkracht)
#define SCENARIO_GEWENSTE_TIKKRACHT_ADC ADC(400)   // TODO: voor later

// Om de OFFSET te bepalen
#define OFFSET_METING_TIJD_MS   2000UL
#define OFFSET_VEILIGHEIDSMARGE ADC(5)

#define MAX_AANTAL_POGINGEN_NULMETING 3

// Achterdeur met EXIT, zodat je de oefening na 3000 ms kan verlaten.
#define EXIT_TIKTIJD_MS        3000
#define EXIT_NO_ACTION_MS      9000

// TODO: Fysiologisch onderbouwde minimale en maximale tiktijd bepalen.
#define MINIMALE_TIKTIJD_MS    100UL
#define MAXIMALE_TIKTIJD_MS    2000UL

#endif
