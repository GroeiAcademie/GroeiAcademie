#include <Wire.h>

// OPMERKING: De keuze tussen <GroeiAcademie.h> en <Stimulus.h> bepaalt enkel welke namen je sketch mag aanroepen (zichtbaarheid), niet wat er gecompileerd wordt.
// #include <GroeiAcademie.h> // alles van GROEI ACADEMIE mag worden aangeroepen binnen deze sketch/arduino code
#include <Stimulus.h>         // enkel module: Stimulus van GROEI ACADEMIE mag worden aangeroepen binnen deze sketch/arduino code


// Alex Peeters, 20260718, v0.975

// ============================================================================
// HARDWARE INSTELLINGEN (De pinnen van de Arduino UNO R3/R4)
// ============================================================================
#define SERIAL_BAUDRATE 115200UL

// Hardwarematig aanwezig zijn 2 of 4 sensoren. Scenarios 1, 2 en 3 gebruiken 2 sensoren. Scenario 4 kan bij 4 aanwezige sensoren ook 3 sensoren gebruiken.
// #define AANTAL_SENSOREN_AANWEZIG   2 // MAG MEN WEIZIGEN IN FUNCTIE VAN DE BESCHIKBARE HARDWARE  // TODO WAAR?

// Definieer de pinnen voor het 1x4 keypad
#define PIN_TOETS_1   3  // Gekoppeld aan Digitale Pin 3 (vb: Level 1: Start)
#define PIN_TOETS_2   2  // Gekoppeld aan Digitale Pin 2 (vb: Level 2: Basic)
#define PIN_TOETS_3   5  // Gekoppeld aan Digitale Pin 5 (vb: Level 3: Expert)
#define PIN_TOETS_4   4  // Gekoppeld aan Digitale Pin 4 (vb: Level 4: Elite)

// doelduur van stap 3 = gemiddelde van MIN en MAX vb: MIN=3, MAX=5 → doel = 4x de nulmeting, marge dekt automatisch 3x tot 5x
#define INSTORTEND_AANTAL_STAPPEN   ((INSTORTEND_MINIMALE_FACTOR + INSTORTEND_MAXIMALE_FACTOR) / 2)

#define MINIMALE_WACHTTIJD_MS  500UL
#define MAXIMALE_WACHTTIJD_MS  1500UL

// ============================================================================
// CONSTANTEN VOOR DE LCD-TEKSTEN (Als pure tekst-pointers)
// ============================================================================
#define LCD_SERIEEL_L1         "WACHT OP SERIELE"
#define LCD_SERIEEL_L2         "VERBINDING ..."

#define LCD_START_L1           " GROEI ACADEMIE"
#define LCD_START_L2           " INITIALISEREN!"

#define LCD_NULMETING_L1       "NULMETING:"
#define LCD_NULMETING_L2       "Niets aanraken"

#define LCD_KEUZE_LEVELS_L1    "1 Start  2 Basic"
#define LCD_KEUZE_LEVELS_L2    "3 Expert 4 Elite"
#define LCD_KEUZE_LEVELS_L3    "Gekozen level: "

#define LCD_FINALE_TITEL       "EINDE INOEFENEN"
#define LCD_FINALE_SUCCES      "SUCCESVOL GEDAAN"

#define LCD_S0_GEEF_STARTTIK   "Geef starttik"
#define LCD_S0_TIK_AANHOUDEN   "Tik aanhouden"
#define LCD_S0_NU              " NU"
#define LCD_S0_NULMETING       "Nulmeting Tik"
#define LCD_S0_EVEN_GEDULD     "Wees alert..."
#define LCD_S0_LABEL_TIJDENS   "Tijdens "
#define LCD_S0_LABEL_MS        "ms"

#define LCD_S2_TITEL           "Exp. SIMULTAAN"
#define LCD_S2_START_VERSCHIL  "START VERSCHILT"
#define LCD_S2_TIJD_VERSCHILT  "TIJD VERSCHILT"
#define LCD_S2_KIES_SENSOREN   "Tik Sensors 1&2"

#define LCD_SCORE_TIKKRACHT    "TIKKRACHT "
#define LCD_SCORE_TIKTIJD      "TIKTIJD "
#define LCD_SCORE_KRACHT       "KRACHT "
#define LCD_SCORE_TIJD         "TIJD "
#define LCD_SCORE_SYNCHROON    "SYNCHROON "
#define LCD_SCORE_IN_BALANS    "IN BALANS "
#define LCD_SCORE_INSTORTEND   "INSTORTEND "
#define LCD_SCORE_PERCENTAGE   "%"

// ============================================================================
// CONSTANTEN VOOR DE PAUZETIJDEN (Delays)
// ============================================================================

#define LCD_LEESTIJD_ENDSCORE_MS        5000UL
#define LCD_LEESTIJD_FEEDBACK_KORT_MS   2000UL
#define LCD_LEESTIJD_FEEDBACK_LANG_MS   4000UL
#define LCD_LEESTIJD_MEDEDELING_KORT_MS 1500UL
#define LCD_LEESTIJD_MEDEDELING_LANG_MS 3000UL

// ============================================================================
// CONSTANTEN DIE NIET AANGEPAST MOGEN WORDEN
// ============================================================================

#define AANTAL_SENSOREN_ALGORITME1   2  // AANTAL_SENSOREN_SIMULTAAN_2
#define AANTAL_SENSOREN_ALGORITME2   2  // AANTAL_SENSOREN_SIMULTAAN_2
#define AANTAL_SENSOREN_ALGORITME3   2  // AANTAL_SENSOREN_SIMULTAAN_2

#if AANTAL_SENSOREN_AANWEZIG == 2 
  #define AANTAL_SENSOREN_ALGORITME4 2  // Documentaire standaard; Scenario 4 gebruikt lokaal aantalSensorenSimultaanTeMeten.
#else
  #define AANTAL_SENSOREN_ALGORITME4 4  // Documentaire standaard; Scenario 4 kiest lokaal 2, 3 of 4 gebruikte sensoren.
#endif

#define AANTAL_SENSOREN_SIMULTAAN_2 2
#define AANTAL_SENSOREN_SIMULTAAN_4 4

// INSTORTEND SCORINGSVORM (enkel gebruikt bij Scenario 3, stap 3)
#define INSTORTEND_SCORING_BINAIR    1  // tijd en kracht moeten beiden correct zijn voor punten (level 1, 2, 3)
#define INSTORTEND_SCORING_GRADUEEL  2  // tijd en kracht worden apart gescoord, elk goed voor de helft van de stappenpunten, geeft meer nuance bij hogere levels (level 4)

// INSTORTEND SCORINGSVORM (enkel gebruikt bij Scenario 3, stap 3)
int instortendOfGradueel = INSTORTEND_SCORING_BINAIR; // Kan verhoogd worden met instortendOfGradueel++ na succesvolle sessies (net als stimulusVersie) // TODO WAAROM HIER?

// ============================================================================
// ============================================================================
// Prototypen voor functies om de juiste opbouwvolgorde te garanderen
// ============================================================================
void ToonEindScoreScenario2();
void ToonMenuKiesEnStelLevelIn();
void UitvoerenAlgoritmeSimultaneTik();

// ============================================================================

StimulusProfiel nulmetingStimulus[AANTAL_SENSOREN_AANWEZIG], gemetenStimulus[AANTAL_SENSOREN_AANWEZIG];

SynchronisatieProfiel nulmetingSynchronisatie, gemetenSynchronisatie[3];

// ============================================================================

int TEST_AANTAL_KEER_HERHALEN  = 5;

void setup() {
#if ADC_BITS == 14
  analogReadResolution(14);
#endif  

  lcd.init();      // set up the LCD's number of columns and rows
  lcd.backlight(); // turn on the backlight
  PrintToScreen(LCD_SERIEEL_L1, LCD_SERIEEL_L2);

#ifdef DEBUG
  Serial.begin(SERIAL_BAUDRATE);
  while (!Serial) { ; } // Wacht hier totdat er een seriële verbinding is
  DEBUG_PRINTLN("=== DEBUG GESTART ===");
#endif

#if ADC_BACKEND == ADC_BACKEND_ADS1115
  InitialiseerADS1115();
#endif

  // Activeer de interne pull-up weerstanden voor de 4 toetsen en zet deze pinnen as input
  pinMode(PIN_TOETS_1, INPUT_PULLUP);
  pinMode(PIN_TOETS_2, INPUT_PULLUP);
  pinMode(PIN_TOETS_3, INPUT_PULLUP);
  pinMode(PIN_TOETS_4, INPUT_PULLUP);

  PrintToScreen(LCD_START_L1, LCD_START_L2, LCD_LEESTIJD_MEDEDELING_KORT_MS);
  ResetAlleTellers();

  PrintToScreen(LCD_NULMETING_L1, LCD_NULMETING_L2, LCD_LEESTIJD_MEDEDELING_KORT_MS);
  BepaalSensorOffsets();  
}

// ============================================================================
// LOOP: HET HOOFDPROGRAMMA
// ============================================================================
void loop() {
  UitvoerenAlgoritmeSimultaneTik();
}

// ============================================================================
// ALGORITME 2: SIMULTANE TIK (Scenario 2)
// ============================================================================
void UitvoerenAlgoritmeSimultaneTik () {
  const int offsetSensor[4] = { offsetSensor1, offsetSensor2, offsetSensor3, offsetSensor4 };

  ResetAlleTellers();
  ToonMenuKiesEnStelLevelIn();
  PrintToScreen(LCD_S2_TITEL, LCD_S2_KIES_SENSOREN, MINIMALE_WACHTTIJD_MS);
  WachtTotAlleSensorsLosgelatenVoorTest(AANTAL_SENSOREN_AANWEZIG);
  PrintToScreen("", "", 0, LCD_S0_NU);

  // Eerste meting smijten we weg, geeft een valse waarde.
  for (int sensorNummer = 0; sensorNummer < AANTAL_SENSOREN_ALGORITME2; sensorNummer++) RawAnalogRead(sensorPin[sensorNummer]);

  // STAP 1: Blijf wachten tot twee sensoren ingedrukt zijn geweest.
  int eersteGekozenSensorNummer = -1, tweedeGekozenSensorNummer = -1;

  while (eersteGekozenSensorNummer == -1 || tweedeGekozenSensorNummer == -1) {
    for (int sensorNummer = 0; sensorNummer < AANTAL_SENSOREN_ALGORITME2; sensorNummer++) {
      if (AnalogReadMetGekorigeerdeOffsets(sensorPin[sensorNummer], offsetSensor[sensorNummer]) > TIK_MINIMALE_DRUKWAARDE) {
        if (eersteGekozenSensorNummer == -1) {
          eersteGekozenSensorNummer = sensorNummer;
        } else if (tweedeGekozenSensorNummer == -1 && eersteGekozenSensorNummer != sensorNummer) {
          tweedeGekozenSensorNummer = sensorNummer;
        }
      }
    }
  }

  // STAP 2: Blijf wachten zolang minstens één van de twee gekozen sensoren nog ingedrukt blijft.
  while (AnalogReadMetGekorigeerdeOffsets(sensorPin[eersteGekozenSensorNummer], offsetSensor[eersteGekozenSensorNummer]) > TIK_MINIMALE_DRUKWAARDE ||
         AnalogReadMetGekorigeerdeOffsets(sensorPin[tweedeGekozenSensorNummer], offsetSensor[tweedeGekozenSensorNummer]) > TIK_MINIMALE_DRUKWAARDE);

  // --- EENMALIGE NULMETING ---
  int aantalNulmetingPogingen = 0;
  bool nulmetingGoedgekeurd   = false;
  String herhalingStr         = "0M";
  
  // --- START VAN DE TRAININGSLUS ---
  for (int herhaling = 1; herhaling <= TEST_AANTAL_KEER_HERHALEN; herhaling++) {
    int WACHTTIJD_MS = random(MINIMALE_WACHTTIJD_MS, MAXIMALE_WACHTTIJD_MS);

    if (nulmetingGoedgekeurd) {
      PrintToScreen(LCD_S0_LABEL_TIJDENS + String(nulmetingTikTijd) + LCD_S0_LABEL_MS, LCD_S0_TIK_AANHOUDEN, WACHTTIJD_MS);
    } else {
      PrintToScreen(LCD_S0_NULMETING, LCD_S0_EVEN_GEDULD, WACHTTIJD_MS);
    }

    WachtTotAlleSensorsLosgelatenVoorTest(AANTAL_SENSOREN_AANWEZIG);
    PrintToScreen("", "", 0, LCD_S0_NU);

    // Meet de twee gekozen sensoren en hun onderlinge synchronisatie.
    int exitStatus = MeetStimulusSimultaan(gemetenStimulus, AANTAL_SENSOREN_ALGORITME2, gemetenSynchronisatie);
    if (exitStatus == EXIT_VOORWAARDE_NO_ACTION_TIMEOUT || exitStatus == EXIT_VOORWAARDE_TIMEOUT) { return; }

    // --- EVALUATIEFITTING ---
    if (nulmetingGoedgekeurd) {
      if (gemetenStimulus[eersteGekozenSensorNummer].TikTijd > EXIT_TIKTIJD_MS || gemetenStimulus[tweedeGekozenSensorNummer].TikTijd > EXIT_TIKTIJD_MS) { return; } // Noodstop check

      VergelijkStimulus(nulmetingStimulus[eersteGekozenSensorNummer], gemetenStimulus[eersteGekozenSensorNummer], TijdCorrect, KrachtCorrect);
      VergelijkStimulus(nulmetingStimulus[tweedeGekozenSensorNummer], gemetenStimulus[tweedeGekozenSensorNummer], TijdCorrect, KrachtCorrect);
      VergelijkSynchronisatie(nulmetingSynchronisatie, gemetenSynchronisatie[0]);

      // Formatteer herhalingsteller (Twee digits) via String-klasse om pointers te vermijden
      herhalingStr = (herhaling < 10) ? ('0' + String(herhaling)) : String(herhaling);
    } else {  // Nulmeting evalueert op basis van de gecombineerde gemiddelden van de twee gekozen sensoren
      unsigned long gematigdTijd = (gemetenStimulus[eersteGekozenSensorNummer].TikTijd + gemetenStimulus[tweedeGekozenSensorNummer].TikTijd) / 2;
      int gematigdKracht = (gemetenStimulus[eersteGekozenSensorNummer].gemiddeldeTikKracht + gemetenStimulus[tweedeGekozenSensorNummer].gemiddeldeTikKracht) / 2;

      bool startTijdSimultaan = gemetenSynchronisatie[0].verschilStartTijd <= TOEGESTANE_MARGE_SIMULTANE_STARTTIJD_MS;
      unsigned long toegestaneVerschilTikTijd = gematigdTijd * TOEGESTANE_MARGE_TIKTIJD / 100;
      bool tikTijdSimultaan = gemetenSynchronisatie[0].verschilTikTijd <= toegestaneVerschilTikTijd;

      int resultaatNulmeting = EvalueerNulmeting(gematigdTijd, gematigdKracht, nulmetingGoedgekeurd, nulmetingTikTijd, nulmetingTikKracht, herhaling, aantalNulmetingPogingen);

      if (resultaatNulmeting == -1) {
        return;
      } else if (resultaatNulmeting == 1) {
        if (!startTijdSimultaan) {
          nulmetingGoedgekeurd = false;
          PrintToScreen(LCD_S0_NULMETING, LCD_S2_START_VERSCHIL, LCD_LEESTIJD_FEEDBACK_LANG_MS);
        } else if (!tikTijdSimultaan) {
          nulmetingGoedgekeurd = false;
          PrintToScreen(LCD_S0_NULMETING, LCD_S2_TIJD_VERSCHILT, LCD_LEESTIJD_FEEDBACK_LANG_MS);
        } else {
          nulmetingStimulus[eersteGekozenSensorNummer] = gemetenStimulus[eersteGekozenSensorNummer];
          nulmetingStimulus[tweedeGekozenSensorNummer] = gemetenStimulus[tweedeGekozenSensorNummer];
          nulmetingSynchronisatie = gemetenSynchronisatie[0];
        }
      }
    }

    // Directe feedback van beide metingen gescheiden door een slash '/' conform jouw opzet
    PrintToScreen(herhalingStr + ' ' + LCD_SCORE_TIJD + String(gemetenStimulus[eersteGekozenSensorNummer].TikTijd) + '/' + String(gemetenStimulus[tweedeGekozenSensorNummer].TikTijd), LCD_SCORE_KRACHT + String(gemetenStimulus[eersteGekozenSensorNummer].gemiddeldeTikKracht) + '/' + String(gemetenStimulus[tweedeGekozenSensorNummer].gemiddeldeTikKracht), LCD_LEESTIJD_FEEDBACK_KORT_MS);
  }

  ToonEindScoreScenario2(); 
}

void ToonEindScoreScenario2() {
  // 1. Berekening van de procentuele scores (schaal 0-100%)
  int totaalAantalStimuli   = TEST_AANTAL_KEER_HERHALEN * 2;
  int percentageTikTijdOk   = (TELLER_TIKTIJD_CORRECT * 100) / totaalAantalStimuli;
  int percentageTikKrachtOk = (TELLER_TIKKRACHT_CORRECT * 100) / totaalAantalStimuli;

  // 2. Berekening van de specifieke coördinatiescores (Synchroon en In Balans)
  int percentageSynchroon = (TELLER_TIKTIJD_SYNCHROON * 100) / TEST_AANTAL_KEER_HERHALEN;
  int percentageInBalans  = (TELLER_TIKKRACHT_IN_BALANS * 100) / TEST_AANTAL_KEER_HERHALEN;

  // --- SCHERM 1: TOON HOOFDSCORE TIKTIJD & TIKKRACHT ---
  PrintToScreen(LCD_SCORE_TIJD + String(percentageTikTijdOk) + LCD_SCORE_PERCENTAGE, LCD_SCORE_KRACHT + String(percentageTikKrachtOk) + LCD_SCORE_PERCENTAGE, LCD_LEESTIJD_ENDSCORE_MS);

  // --- SCHERM 2: TOON COÖRDINATIE FEEDBACK (Synchroon en InBalans) ---
  // Dit scherm laat de gebruiker zien in welke mate beide handen synchroon en in balans samenwerkten
  PrintToScreen(LCD_SCORE_SYNCHROON + String(percentageSynchroon) + LCD_SCORE_PERCENTAGE, LCD_SCORE_IN_BALANS + String(percentageInBalans) + LCD_SCORE_PERCENTAGE, LCD_LEESTIJD_ENDSCORE_MS);

  // 3. Reset het geheugen (Zorg dat ResetAlleTellers ook de 2 nieuwe variabelen reset!)
  ResetAlleTellers();
}

void ToonMenuKiesEnStelLevelIn() {
  PrintToScreen(LCD_KEUZE_LEVELS_L1, LCD_KEUZE_LEVELS_L2);
  int gekozenLevel = 0;

  // Wacht tot alle toetsen losgelaten zijn.
  while (digitalRead(PIN_TOETS_1) == LOW || digitalRead(PIN_TOETS_2) == LOW || digitalRead(PIN_TOETS_3) == LOW || digitalRead(PIN_TOETS_4) == LOW);

  while (true) {
    if (digitalRead(PIN_TOETS_1) == LOW) {  // Start
      while (digitalRead(PIN_TOETS_1) == LOW);
      TOEGESTANE_MARGE_SIMULTANE_STARTTIJD_MS = 150UL;
      TOEGESTANE_MARGE_TIKTIJD   = 30;
      TOEGESTANE_MARGE_TIKKRACHT = 25;
      TEST_AANTAL_KEER_HERHALEN  = 5;
      stimulusVersie             = STIMULUS_BASIC;
      instortendOfGradueel       = INSTORTEND_SCORING_BINAIR;   // Stap 3: tijd EN kracht beiden correct
      gekozenLevel               = 1;
      break;
    } else if (digitalRead(PIN_TOETS_2) == LOW) {  // Basic
      while (digitalRead(PIN_TOETS_2) == LOW);
      TOEGESTANE_MARGE_SIMULTANE_STARTTIJD_MS = 100UL;
      TOEGESTANE_MARGE_TIKTIJD   = 20;
      TOEGESTANE_MARGE_TIKKRACHT = 15;
      TEST_AANTAL_KEER_HERHALEN  = 10;
      stimulusVersie             = STIMULUS_BASIC;
      instortendOfGradueel       = INSTORTEND_SCORING_BINAIR;   // Stap 3: tijd EN kracht beiden correct
      gekozenLevel               = 2;
      break;
    } else if (digitalRead(PIN_TOETS_3) == LOW) {  // Expert
      while (digitalRead(PIN_TOETS_3) == LOW);
      TOEGESTANE_MARGE_SIMULTANE_STARTTIJD_MS = 75UL;
      TOEGESTANE_MARGE_TIKTIJD   = 15;
      TOEGESTANE_MARGE_TIKKRACHT = 10;
      TEST_AANTAL_KEER_HERHALEN  = 15;
      stimulusVersie             = STIMULUS_BASIC;
      instortendOfGradueel       = INSTORTEND_SCORING_BINAIR;   // Stap 3: tijd EN kracht beiden correct
      gekozenLevel               = 3;
      break;
    } else if (digitalRead(PIN_TOETS_4) == LOW) {  // Elite
      while (digitalRead(PIN_TOETS_4) == LOW);
      TOEGESTANE_MARGE_SIMULTANE_STARTTIJD_MS = 50UL;
      TOEGESTANE_MARGE_TIKTIJD   = 10;
      TOEGESTANE_MARGE_TIKKRACHT = 5;
      TEST_AANTAL_KEER_HERHALEN  = 20;
      stimulusVersie             = STIMULUS_EXTENDED;
      instortendOfGradueel       = INSTORTEND_SCORING_GRADUEEL; // Stap 3: tijd en kracht apart gescoord
      gekozenLevel               = 4;
      break;
    }
  }

  String tweedeRegel = '#' + String(TEST_AANTAL_KEER_HERHALEN) + " T" + String(TOEGESTANE_MARGE_TIKTIJD) + "% K" + String(TOEGESTANE_MARGE_TIKKRACHT) + "% SV" + stimulusVersie;
  PrintToScreen(LCD_KEUZE_LEVELS_L3 + String(gekozenLevel), tweedeRegel, LCD_LEESTIJD_FEEDBACK_LANG_MS);
}
