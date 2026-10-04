// ============================================================================
// Scenario 1 — EnkelTik
// ============================================================================
// Dit example dwingt ADC_BACKEND/SCREEN_OUTPUT_CONFIG NIET zelf af — dat kan
// een .ino structureel niet: Stimulus.cpp/Screen.cpp worden als aparte
// bestanden gecompileerd en zien een #define hier nooit. Deze sketch werkt
// met welke ADC-backend en welk schermtype dan ook actief is via UserConfig.h
// (kopieer van UserConfig_template.h) of SystemConfig.h, en past haar gedrag
// aan via de #if-controles hieronder — vandaar geen #error nodig hier.
// ============================================================================

#include <Wire.h>

// INPUT TESTEN:
// Standaard wordt KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4 gebruikt.
//
// Kies in UserConfig.h het te testen invoerkanaal via INPUT_KANAAL_CONFIG:
// #define INPUT_KANAAL_CONFIG INPUT_TYPE_DIGITAL
// #define INPUT_KANAAL_CONFIG INPUT_TYPE_PCF8574
// #define INPUT_KANAAL_CONFIG INPUT_TYPE_HX1838
// #define INPUT_KANAAL_CONFIG (INPUT_TYPE_PCF8574 | INPUT_TYPE_HX1838)
//
// Alleen de toetsen met opschrift 1 t.e.m. 4, of S1 t.e.m. S4, worden in deze test gebruikt.
// Dit geldt ook wanneer het gekozen keypad of de HX1838-remote meer toetsen heeft.
#include <GroeiAcademie.h>
#include <Configuratie/Examples.h>
#include <Configuratie/ExamplesConfig.h>


// Vergelijkt het opschrift van de ingedrukte toets met een positie (1-4), ongeacht of het
// actieve keypad de cijfernotatie ("1") of de S-notatie ("S1") gebruikt voor die positie.
bool ToetsPositieIngedrukt(const char* opschrift, int positie) {
  if (opschrift == nullptr) return false;
  char cijferNotatie[3];
  char sNotatie[4];
  snprintf(cijferNotatie, sizeof(cijferNotatie), "%d", positie);
  snprintf(sNotatie, sizeof(sNotatie), "S%d", positie);
  return strcmp(opschrift, cijferNotatie) == 0 || strcmp(opschrift, sNotatie) == 0;
}

// INSTORTEND SCORINGSVORM (enkel gebruikt bij Scenario 3, stap 3)
int instortendOfGradueel = INSTORTEND_SCORING_BINAIR; // Kan verhoogd worden met instortendOfGradueel++ na succesvolle sessies (net als stimulusVersie)

// ============================================================================
// ============================================================================
// Prototypen voor functies om de juiste opbouwvolgorde te garanderen
// ============================================================================
void ToonEindScoreScenario1();
void ToonMenuKiesEnStelLevelIn();
void UitvoerenAlgoritmeEnkelTik();

// ============================================================================

StimulusProfiel nulmetingStimulus[AANTAL_SENSOREN_AANWEZIG], gemetenStimulus[AANTAL_SENSOREN_AANWEZIG];

SynchronisatieProfiel nulmetingSynchronisatie, gemetenSynchronisatie[3];

// ============================================================================

int TEST_AANTAL_KEER_HERHALEN  = 5;

void setup() {
#if ADC_BITS == 12 || ADC_BITS == 14
  analogReadResolution(ADC_BITS);
#endif  

  Screen = GedeeldeBusNewComponent<struct Screen>();
  if (Screen == nullptr) exit(0);

#ifdef DEBUG
  GA_DEBUG_PRINTLN("=== DEBUG GESTART ===");
#endif
  Screen->Print(LCD_SERIEEL_L1, LCD_SERIEEL_L2);

  sensorRFP602 = GedeeldeBusNewComponent<struct RFP602>();
  if (sensorRFP602 == nullptr) exit(0);

  // Activeer de interne pull-up weerstanden voor de 4 toetsen en zet deze pinnen as input
  //-- pinMode(PIN_TOETS_1, INPUT_PULLUP);
  //-- pinMode(PIN_TOETS_2, INPUT_PULLUP);
  //-- pinMode(PIN_TOETS_3, INPUT_PULLUP);
  //-- pinMode(PIN_TOETS_4, INPUT_PULLUP);
  Input = GedeeldeBusNewComponent<struct Input>();
  if (Input == nullptr) exit(0);

  Screen->Print(LCD_START_L1, LCD_START_L2, LCD_LEESTIJD_MEDEDELING_KORT_MS);
  Stimulus.ResetAlleTellers();

  Screen->Print(LCD_NULMETING_L1, LCD_NULMETING_L2, LCD_LEESTIJD_MEDEDELING_KORT_MS);
  Stimulus.BepaalSensorOffsets();  
}

// ============================================================================
// LOOP: HET HOOFDPROGRAMMA
// ============================================================================
void loop() {
  UitvoerenAlgoritmeEnkelTik();
}

// ============================================================================
// ALGORITME 1: ENKEL TIK (Scenario 1)
// ============================================================================
void UitvoerenAlgoritmeEnkelTik() {
  Stimulus.ResetAlleTellers();
  ToonMenuKiesEnStelLevelIn();
  Screen->Print(LCD_S1_TITEL, LCD_S0_GEEF_STARTTIK, MINIMALE_WACHTTIJD_MS);   
  Stimulus.WachtTotAlleSensorsLosgelatenVoorTest(AANTAL_SENSOREN_AANWEZIG);
  Screen->Print("", "", 0, LCD_S0_NU);

  while (Stimulus.TIK_TEST_ACTIEVE_VINGER == -1) {
#ifdef DEBUG
  GA_DEBUG_PRINT("Actieve vinger = ");
  GA_DEBUG_PRINTLN(Stimulus.TIK_TEST_ACTIEVE_VINGER);
#endif

    // eerste meting smijten we weg, geef valse waarde
    for (int sensorNummer = 0; sensorNummer < AANTAL_SENSOREN_AANWEZIG; sensorNummer++) { sensorRFP602->RawAnalogRead(sensorRFP602->sensorPin[sensorNummer]); }
     const int offsetSensor[4] = { Stimulus.offsetSensor1, Stimulus.offsetSensor2, Stimulus.offsetSensor3, Stimulus.offsetSensor4 };

    // bepaal welke sensor als eerste actief is
    for (int sensorNummer = AANTAL_SENSOREN_AANWEZIG - 1; sensorNummer >= 0; sensorNummer--) {
      if (Stimulus.AnalogReadMetGekorigeerdeOffsets(sensorRFP602->sensorPin[sensorNummer], offsetSensor[sensorNummer]) > TIK_MINIMALE_DRUKWAARDE) {
        Stimulus.TIK_TEST_ACTIEVE_VINGER = sensorRFP602->sensorPin[sensorNummer];
        Stimulus.offsetSensorActief = offsetSensor[sensorNummer];
        break;
      }
    }
  }

#ifdef DEBUG
  GA_DEBUG_PRINT("Actieve vinger = ");
  GA_DEBUG_PRINTLN(Stimulus.TIK_TEST_ACTIEVE_VINGER);
  GA_DEBUG_PRINTLN("---------------------");
#endif

  while (Stimulus.AnalogReadMetGekorigeerdeOffsets(Stimulus.TIK_TEST_ACTIEVE_VINGER, Stimulus.offsetSensorActief) > TIK_MINIMALE_DRUKWAARDE);
  
  // --- EENMALIGE NULMETING ---
  int aantalNulmetingPogingen = 0;
  bool nulmetingGoedgekeurd   = false;
  String herhalingStr         = "0M"; // Nulmeting-label op LCD; wordt vervangen door rondenummer na goedkeuring
  
  // --- START VAN DE TRAININGSLUS ---
  for (int herhaling = 1; herhaling <= TEST_AANTAL_KEER_HERHALEN; herhaling++) {
    int WACHTTIJD_MS = random(MINIMALE_WACHTTIJD_MS, MAXIMALE_WACHTTIJD_MS);

    if (nulmetingGoedgekeurd) {  // Wanneer de nulmeting WEL is goedgekeurd (true)
      Screen->Print(LCD_S0_LABEL_TIJDENS + String(Stimulus.nulmetingTikTijd) + LCD_S0_LABEL_MS, LCD_S0_TIK_AANHOUDEN, WACHTTIJD_MS);
    } else {
      Screen->Print(LCD_S0_NULMETING, LCD_S0_EVEN_GEDULD, WACHTTIJD_MS);
    }

    Stimulus.WachtTotAlleSensorsLosgelatenVoorTest(AANTAL_SENSOREN_AANWEZIG);
    Screen->Print("", "", 0, LCD_S0_NU);

    // Dit vangt te vroeg drukken of te laat loslaten fysiologisch perfect op.
    while (Stimulus.AnalogReadMetGekorigeerdeOffsets(Stimulus.TIK_TEST_ACTIEVE_VINGER, Stimulus.offsetSensorActief) > TIK_MINIMALE_DRUKWAARDE);

    // PAS ALS HET BORD VRIJ IS, meten we de échte, nieuwe reactie-tik als volledig StimulusProfiel:
    int exitStatus = Stimulus.MeetStimulus(Stimulus.TIK_TEST_ACTIEVE_VINGER, Stimulus.offsetSensorActief, gemetenStimulus[0]);
    if (exitStatus == EXIT_VOORWAARDE_NO_ACTION_TIMEOUT || exitStatus == EXIT_VOORWAARDE_TIMEOUT) { return; }

    if (nulmetingGoedgekeurd) {
      if (gemetenStimulus[0].TikTijd > EXIT_TIKTIJD_MS) { return; } // Noodstop check
      Stimulus.VergelijkStimulus(nulmetingStimulus[0], gemetenStimulus[0], Stimulus.TijdCorrect, Stimulus.KrachtCorrect);
      herhalingStr = (herhaling < 10) ? ('0' + String(herhaling)) : String(herhaling);
    } else {
      int resultaatNulmeting = Stimulus.EvalueerNulmeting(gemetenStimulus[0].TikTijd, gemetenStimulus[0].gemiddeldeTikKracht, nulmetingGoedgekeurd, Stimulus.nulmetingTikTijd, Stimulus.nulmetingTikKracht, herhaling, aantalNulmetingPogingen);

      if (resultaatNulmeting == -1) {
        return; // Verlaat direct de trainingslus omdat het maximum aantal pogingen is bereikt
      } else if (resultaatNulmeting == 1) {
        nulmetingStimulus[0] = gemetenStimulus[0];
      }
    }

    Screen->Print(herhalingStr + ' ' + LCD_SCORE_TIKTIJD + String(gemetenStimulus[0].TikTijd), String(' ') + LCD_SCORE_TIKKRACHT + String(gemetenStimulus[0].gemiddeldeTikKracht), LCD_LEESTIJD_FEEDBACK_KORT_MS);
  }

  ToonEindScoreScenario1();
}

void ToonEindScoreScenario1() {
  // Berekening van de procentuele scores met integer-veiligheid (schaal 0-100%)
  int percentageTikTijdOk = (Stimulus.TELLER_TIKTIJD_CORRECT * 100) / TEST_AANTAL_KEER_HERHALEN;
  int percentageTikKrachtOk = (Stimulus.TELLER_TIKKRACHT_CORRECT * 100) / TEST_AANTAL_KEER_HERHALEN;

  // --- TOON SCORE TIKTIJD & TIKKRACHT ---
  Screen->Print(LCD_SCORE_TIKTIJD + String(percentageTikTijdOk) + LCD_SCORE_PERCENTAGE, LCD_SCORE_TIKKRACHT + String(percentageTikKrachtOk) + LCD_SCORE_PERCENTAGE, LCD_LEESTIJD_ENDSCORE_MS);
}

void ToonMenuKiesEnStelLevelIn() {
  Screen->Print(LCD_KEUZE_LEVELS_L1, LCD_KEUZE_LEVELS_L2);
  int gekozenLevel = 0;

  // Wacht tot alle toetsen losgelaten zijn.
  /*-- while (digitalRead(PIN_TOETS_1) == LOW || digitalRead(PIN_TOETS_2) == LOW || digitalRead(PIN_TOETS_3) == LOW || digitalRead(PIN_TOETS_4) == LOW); */

  while (true) {
    InputResultaat invoer = Input->OpvragenHuidigeToetsAanslag(true);
    const char* opschriftToetsAanslag = invoer.opschriftToetsAanslag;
    if (ToetsPositieIngedrukt(opschriftToetsAanslag, 1)) {  // Start
      //-- while (digitalRead(PIN_TOETS_1) == LOW);
      Stimulus.TOEGESTANE_MARGE_SIMULTANE_STARTTIJD_MS = 150UL;
      Stimulus.TOEGESTANE_MARGE_TIKTIJD   = 30;
      Stimulus.TOEGESTANE_MARGE_TIKKRACHT = 25;
      TEST_AANTAL_KEER_HERHALEN  = 5;
      Stimulus.stimulusVersie             = STIMULUS_BASIC;
      instortendOfGradueel       = INSTORTEND_SCORING_BINAIR;   // Stap 3: tijd EN kracht beiden correct
      gekozenLevel               = 1;
      break;
    } else if (ToetsPositieIngedrukt(opschriftToetsAanslag, 2)) {  // Basic
      //-- while (digitalRead(PIN_TOETS_2) == LOW);
      Stimulus.TOEGESTANE_MARGE_SIMULTANE_STARTTIJD_MS = 100UL;
      Stimulus.TOEGESTANE_MARGE_TIKTIJD   = 20;
      Stimulus.TOEGESTANE_MARGE_TIKKRACHT = 15;
      TEST_AANTAL_KEER_HERHALEN  = 10;
      Stimulus.stimulusVersie             = STIMULUS_BASIC;
      instortendOfGradueel       = INSTORTEND_SCORING_BINAIR;   // Stap 3: tijd EN kracht beiden correct
      gekozenLevel               = 2;
      break;
    } else if (ToetsPositieIngedrukt(opschriftToetsAanslag, 3)) {  // Expert
      //-- while (digitalRead(PIN_TOETS_3) == LOW);
      Stimulus.TOEGESTANE_MARGE_SIMULTANE_STARTTIJD_MS = 75UL;
      Stimulus.TOEGESTANE_MARGE_TIKTIJD   = 15;
      Stimulus.TOEGESTANE_MARGE_TIKKRACHT = 10;
      TEST_AANTAL_KEER_HERHALEN  = 15;
      Stimulus.stimulusVersie             = STIMULUS_BASIC;
      instortendOfGradueel       = INSTORTEND_SCORING_BINAIR;   // Stap 3: tijd EN kracht beiden correct
      gekozenLevel               = 3;
      break;
    } else if (ToetsPositieIngedrukt(opschriftToetsAanslag, 4)) {  // Elite
      //-- while (digitalRead(PIN_TOETS_4) == LOW);
      Stimulus.TOEGESTANE_MARGE_SIMULTANE_STARTTIJD_MS = 50UL;
      Stimulus.TOEGESTANE_MARGE_TIKTIJD   = 10;
      Stimulus.TOEGESTANE_MARGE_TIKKRACHT = 5;
      TEST_AANTAL_KEER_HERHALEN  = 20;
      Stimulus.stimulusVersie             = STIMULUS_EXTENDED;
      instortendOfGradueel       = INSTORTEND_SCORING_GRADUEEL; // Stap 3: tijd en kracht apart gescoord
      gekozenLevel               = 4;
      break;
    }
  }

  String tweedeRegel = '#' + String(TEST_AANTAL_KEER_HERHALEN) + " T" + String(Stimulus.TOEGESTANE_MARGE_TIKTIJD) + "% K" + String(Stimulus.TOEGESTANE_MARGE_TIKKRACHT) + "% SV" + Stimulus.stimulusVersie;
  Screen->Print(LCD_KEUZE_LEVELS_L3 + String(gekozenLevel), tweedeRegel, LCD_LEESTIJD_FEEDBACK_LANG_MS);
}
