#ifndef STIMULUS_H
#define STIMULUS_H

#include <Arduino.h>
#include "../../Configuratie/SystemConfig.h"
#include "../../Configuratie/StimulusConfig.h"
#if defined(LANGUAGE_NL)
  #include "../../Language/Library_NL.h"
#elif defined(LANGUAGE_DE)
  #include "../../Language/Library_DE.h"
#elif defined(LANGUAGE_EN)
  #include "../../Language/Library_EN.h"
#elif defined(LANGUAGE_FR)
  #include "../../Language/Library_FR.h"
#endif
#include "../../Systeem/Screen/Screen.h"

// ============================================================================
// CONSTANTEN DIE NIET AANGEPAST MOGEN WORDEN
// ============================================================================

// EXITVOORWAARDEN DIE HET ALGORITME VERWACHT
#define EXIT_VOORWAARDE_GEEN                          0
#define EXIT_VOORWAARDE_TIMEOUT                       1
#define EXIT_VOORWAARDE_STARTMASK_ONGELDIG            2
#define EXIT_VOORWAARDE_EXITMASK_ONGELDIG             3
#define EXIT_VOORWAARDE_VERKEERDE_STARTTOESTAND       4
#define EXIT_VOORWAARDE_SENSOR_INGEDRUKT              5
#define EXIT_VOORWAARDE_SENSOR_LOSGELATEN             6
#define EXIT_VOORWAARDE_MASK_BEREIKT                  7
#define EXIT_VOORWAARDE_EXITSENSOR_INGEDRUKT          8
#define EXIT_VOORWAARDE_NO_ACTION_TIMEOUT             9
#define EXIT_VOORWAARDE_AANTAL_SENSOREN_ONGELDIG     10

// EXITSTATUS DIE DE MEETFUNCTIE TERUGGEEFT
#define EXIT_STATUS_GEEN                             EXIT_VOORWAARDE_GEEN
#define EXIT_STATUS_TIMEOUT                          EXIT_VOORWAARDE_TIMEOUT
#define EXIT_STATUS_STARTMASK_ONGELDIG               EXIT_VOORWAARDE_STARTMASK_ONGELDIG
#define EXIT_STATUS_EXITMASK_ONGELDIG                EXIT_VOORWAARDE_EXITMASK_ONGELDIG
#define EXIT_STATUS_VERKEERDE_STARTTOESTAND          EXIT_VOORWAARDE_VERKEERDE_STARTTOESTAND
#define EXIT_STATUS_SENSOR_INGEDRUKT                 EXIT_VOORWAARDE_SENSOR_INGEDRUKT
#define EXIT_STATUS_SENSOR_LOSGELATEN                EXIT_VOORWAARDE_SENSOR_LOSGELATEN
#define EXIT_STATUS_MASK_BEREIKT                     EXIT_VOORWAARDE_MASK_BEREIKT
#define EXIT_STATUS_EXITSENSOR_INGEDRUKT             EXIT_VOORWAARDE_EXITSENSOR_INGEDRUKT
#define EXIT_STATUS_NO_ACTION_TIMEOUT                EXIT_VOORWAARDE_NO_ACTION_TIMEOUT
#define EXIT_STATUS_AANTAL_SENSOREN_ONGELDIG         EXIT_VOORWAARDE_AANTAL_SENSOREN_ONGELDIG

// ============================================================================
// CONSTANTEN VOOR DE LCD-TEKSTEN (Als pure tekst-pointers)
// ============================================================================

//— MOVED #define _LCD_SCORE_TIKKRACHT        "TIKKRACHT "
//— MOVED #define _LCD_SCORE_TIKTIJD          "TIKTIJD "  

//— MOVED #define _LCD_KRACHT_TE_ZACHT        "TE ZACHT"
//— MOVED #define _LCD_KRACHT_TE_HARD         "TE HARD"

//— MOVED #define _LCD_TIJD_TE_KORT           "TE KORT"
//— MOVED #define _LCD_TIJD_TE_LANG           "TE LANG"
//— MOVED #define _LCD_TIJD_METEN_STOPT       "WE STOPPEN ERMEE"
//— MOVED #define _LCD_TIJD_TEVEEL_FOUT       "SLECHTE START :)"

//— MOVED #define _LCD_ADS1115_FOUT           "ADS1115"
//— MOVED #define _LCD_ADS1115_NIET_GEVONDEN  "NIET GEVONDEN"

// ============================================================================
// CONSTANTEN VOOR DE PAUZETIJDEN (Delays)
// ============================================================================
#define _LCD_LEESTIJD_FEEDBACK_MS                     2000UL

// ============================================================================
// STRUCTUUR VOOR SENSOR MEETSTATUS
// ============================================================================
struct SensorMeetStatus {
  int actueleTikKracht;
  unsigned long startTikTijd, eindTikTijd, tijdHoogsteTikKracht;
  int eersteTikKrachtMeting, laatsteTikKrachtMeting, hoogsteTikKrachtMeting;
  unsigned long somVanTikKrachtMetingen, aantalTikKrachtMetingen;
  bool sensorGestart, sensorKlaar;
};

// ============================================================================
// STRUCTUUR VOOR STIMULUSPROFIEL
// ============================================================================
struct StimulusProfiel {
  // BASIS
  unsigned long TikTijd;                 // Tijd van de stimulus in milliseconden
  int           gemiddeldeTikKracht;     // Gemiddelde kracht van de stimulus

  // UITGEBREID
  int           hoogsteTikKracht;        // Hoogste gemeten kracht tijdens de stimulus
  unsigned long tijdTotPiekTikKracht;    // Tijd vanaf start stimulus tot piekkracht

  int           opbouwSnelheid;          // Gemiddelde stijgsnelheid van startkracht naar piekkracht
  int           afbouwSnelheid;          // Gemiddelde daalsnelheid van piekkracht naar laatste krachtmeting

  // TOEKOMST
  // int krachtCurve[MAX_CURVE_PUNTEN];  // Volledige krachtcurve, nuttig om de vorm van zetten en activeren punt per punt te vergelijken
  // int aantalCurvePunten;              // Aantal opgeslagen meetpunten, nodig omdat niet elke stimulus evenveel samples heeft
  // long oppervlakteOnderCurve;         // Totale kracht over tijd, nuttig als maat voor de totale stimulusbelasting
  // int symmetrie;                      // Vergelijking tussen opbouw en afbouw, nuttig om het motorisch patroon te herkennen
  // unsigned long tijdOpPlateau;        // Tijd dicht bij de piekkracht, nuttig om te meten of de kracht even wordt vastgehouden
  // int plateauKracht;                  // Gemiddelde kracht tijdens het plateau
  // int aantalPieken;                   // Aantal duidelijke pieken, nuttig om trillende of dubbele stimuli te herkennen
  // int krachtVariatie;                 // Variatie tijdens de stimulus, nuttig om stabiliteit te beoordelen
  // int trillingsIndex;                 // Kleine snelle variaties, nuttig om onvaste druk te herkennen
};

// ============================================================================
// STRUCTUUR VOOR SYNCHRONISATIEPROFIEL
// ============================================================================

struct SynchronisatieProfiel {
  unsigned long verschilStartTijd, verschilEindTijd, verschilTikTijd;  // Verschil tussen eerst en laatst gestart, eerst en laatst losgelaten, en kortste en langste TikTijd
  int verschilGemiddeldeTikKracht, verschilHoogsteTikKracht;           // Verschil tussen laagste en hoogste gemiddelde kracht, en tussen laagste en hoogste piekkracht
  int aantalSensorenSynchroonStart, aantalSensorenSynchroonEinde;      // Grootste groep sensoren die binnen de toegestane marge synchroon start of eindigt: 0, 2, 3 of 4
};

// ============================================================================
// Prototypen voor functies om de juiste opbouwvolgorde te garanderen
// ============================================================================
int  AnalogReadMetGekorigeerdeOffsets(int sensorPin, int offset);

int  BepaalAantalSensorenSynchroon(unsigned long tijden[], int aantalSensoren, unsigned long toegestaneMarge);
void BepaalSensorOffsets();
void BerekenEindStimulus(SensorMeetStatus &sensor, StimulusProfiel &gemetenStimulus);

int  EvalueerNulmeting(unsigned long gemetenTikTijd, int gemetenGemiddeldeTikKracht,
                       bool &nulmetingGoedgekeurd, unsigned long &nulmetingTikTijd, int &nulmetingTikKracht,
                       int &herhaling, int &aantalNulmetingPogingen);

void InitialiseerADS1115();
void InitialiseerSensorStart(unsigned long nu, SensorMeetStatus &sensor);

int  MaakSensorMask(SensorMeetStatus sensor[], int aantalSensorenSimultaanTeMeten, bool testOpDRUKWAARDE = true);

SynchronisatieProfiel MaakSynchronisatieProfiel(SensorMeetStatus sensor[], int sensorA, int sensorB, StimulusProfiel gemetenStimulus[]);
SynchronisatieProfiel MaakSynchronisatieProfielAlleSensoren(SensorMeetStatus sensor[], int aantalSensorenSimultaanTeMeten, StimulusProfiel gemetenStimulus[]);

int  MeetStimulus(int sensorPin, int OffsetSensor, StimulusProfiel &gemetenStimulus, int exitPin = -1, int exitOffset = 0, unsigned long timeoutMs = EXIT_TIKTIJD_MS);
int  MeetStimulusSimultaan(StimulusProfiel gemetenStimulus[], int aantalSensorenSimultaanTeMeten, SynchronisatieProfiel synchronisatie[], int MaskReedsActieveSensorsBijStart = 0, int MaskGewensteActieveSensorsBijExit = 0, unsigned long timeoutMs = EXIT_TIKTIJD_MS, bool testOpDRUKWAARDE = true);

int  RawAnalogRead(int sensorPin);

void ResetAlleTellers();
void ResetStimulusProfiel(StimulusProfiel &gemetenStimulus);
void ResetSynchronisatieProfiel(SynchronisatieProfiel &synchronisatie);

// doelTikTijd: > 0 = expliciete milliseconden, 0 = nulmeting, -1/-2/-3 = instortende moeilijkheidsgraad.
void VergelijkStimulus(StimulusProfiel &nulmeting, StimulusProfiel &gemeten, bool &TijdCorrect, bool &KrachtCorrect, long doelTikTijd = INSTORTEND_TOV_NULMETING, char *instortendExtraTeken = nullptr);

void VergelijkSynchronisatie(SynchronisatieProfiel &nulmeting, SynchronisatieProfiel &gemeten);
void VerwerkSensor(unsigned long nu, int sensorPin, int offsetSensor, SensorMeetStatus &sensor);

void WachtTotAlleSensorsLosgelatenVoorTest(int aantalSensoren);

// ============================================================================
// STATUSTELLERS EN VARIABELEN
// ============================================================================

extern int stimulusVersie;

extern int MARGE_FACTOR;

extern int TOEGESTANE_MARGE_TIKTIJD;
extern int TOEGESTANE_MARGE_TIKKRACHT;

extern unsigned long TOEGESTANE_MARGE_SIMULTANE_STARTTIJD_MS;

extern const int sensorPin[4];

extern unsigned long nulmetingTikTijd;
extern int nulmetingTikKracht;

extern int offsetSensor1, offsetSensor2, offsetSensor3, offsetSensor4, offsetSensorActief;

extern bool TijdCorrect, KrachtCorrect;

extern int TIK_TEST_ACTIEVE_VINGER;

extern int TELLER_TIKTIJD_CORRECT, TELLER_TIKTIJD_TE_SNEL, TELLER_TIKTIJD_TE_TRAAG, TELLER_TIKTIJD_SYNCHROON; 
extern int TELLER_TIKKRACHT_CORRECT, TELLER_TIKKRACHT_TE_ZACHT, TELLER_TIKKRACHT_TE_HARD, TELLER_TIKKRACHT_IN_BALANS; 
extern int TELLER_INSTORTEND_CORRECT, TELLER_SIMULTANE_START_OK, TELLER_SIMULTANE_EIND_OK; 

#endif
