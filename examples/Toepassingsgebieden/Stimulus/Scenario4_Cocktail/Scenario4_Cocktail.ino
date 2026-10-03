// v2.0.0 Stimulus-voorbeeld.
// ============================================================================
// Scenario 4 — Cocktail
// ============================================================================
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
void ToonEindScoreScenario4();
void ToonMenuKiesEnStelLevelIn();
void UitvoerenAlgoritmeCocktailTik();

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
  Input->InputConfigureren();

  Screen->Print(LCD_START_L1, LCD_START_L2, LCD_LEESTIJD_MEDEDELING_KORT_MS);
  Stimulus.ResetAlleTellers();

  Screen->Print(LCD_NULMETING_L1, LCD_NULMETING_L2, LCD_LEESTIJD_MEDEDELING_KORT_MS);
  Stimulus.BepaalSensorOffsets();  
}

// ============================================================================
// LOOP: HET HOOFDPROGRAMMA
// ============================================================================
void loop() {
  UitvoerenAlgoritmeCocktailTik();
}

// ============================================================================
// ALGORITME 4: COCKTAIL TIK (Scenario 4)
// ============================================================================
void UitvoerenAlgoritmeCocktailTik() {
  Stimulus.ResetAlleTellers();
  ToonMenuKiesEnStelLevelIn();
  Screen->Print(LCD_S4_TITEL, "", MINIMALE_WACHTTIJD_MS);
  Stimulus.WachtTotAlleSensorsLosgelatenVoorTest(AANTAL_SENSOREN_AANWEZIG);
  Screen->Print("", "", 0, LCD_S0_NU);
  int aantalSensorenSimultaanTeMeten;

#if AANTAL_SENSOREN_AANWEZIG == 2
  aantalSensorenSimultaanTeMeten = 2;
#else
  Screen->Print(LCD_S4_SENSORS_TEXT, LCD_S4_SENSORS_KEUZE);

  // Wacht tot alle toetsen losgelaten zijn.
  /*-- while (digitalRead(PIN_TOETS_1) == LOW || digitalRead(PIN_TOETS_2) == LOW || digitalRead(PIN_TOETS_3) == LOW || digitalRead(PIN_TOETS_4) == LOW); */

  while (true) {
    InputResultaat invoer = Input->OpvragenHuidigeToetsAanslag(true);
    const char* opschriftToetsAanslag = invoer.opschriftToetsAanslag;
    if (ToetsPositieIngedrukt(opschriftToetsAanslag, 2)) {
      //-- while (digitalRead(PIN_TOETS_2) == LOW);
      aantalSensorenSimultaanTeMeten = 2;
      break;
    } else if (ToetsPositieIngedrukt(opschriftToetsAanslag, 3)) {
      //-- while (digitalRead(PIN_TOETS_3) == LOW);
      aantalSensorenSimultaanTeMeten = 3;
      break;
    } else if (ToetsPositieIngedrukt(opschriftToetsAanslag, 4)) {
      //-- while (digitalRead(PIN_TOETS_4) == LOW);
      aantalSensorenSimultaanTeMeten = 4;
      break;
    }
  }
#endif

  Screen->Print(LCD_S4_TITEL, String(aantalSensorenSimultaanTeMeten) + LCD_S4_SENSOREN, MINIMALE_WACHTTIJD_MS);
  Stimulus.WachtTotAlleSensorsLosgelatenVoorTest(AANTAL_SENSOREN_AANWEZIG);
  Screen->Print("", "", 0, LCD_S0_NU);

  // Eerste meting smijten we weg, geeft een valse waarde.
  for (int sensorNummer = 0; sensorNummer < aantalSensorenSimultaanTeMeten; sensorNummer++) sensorRFP602->RawAnalogRead(sensorRFP602->sensorPin[sensorNummer]);

  // STAP 1: Blijf wachten tot alle te meten sensoren ingedrukt zijn geweest.
  int aantalGestarteSensoren = 0;
  bool sensorGekozen[4] = { false, false, false, false };

  while (aantalGestarteSensoren < aantalSensorenSimultaanTeMeten) {
    for (int sensorNummer = 0; sensorNummer < aantalSensorenSimultaanTeMeten; sensorNummer++) {
      if (!sensorGekozen[sensorNummer] && Stimulus.AnalogReadMetGekorigeerdeOffsets(sensorRFP602->sensorPin[sensorNummer], sensorNummer == 0 ? Stimulus.offsetSensor1 : sensorNummer == 1 ? Stimulus.offsetSensor2 : sensorNummer == 2 ? Stimulus.offsetSensor3 : Stimulus.offsetSensor4) > TIK_MINIMALE_DRUKWAARDE) {
        sensorGekozen[sensorNummer] = true;
        aantalGestarteSensoren++;
      }
    }
  }

  // STAP 2: Blijf wachten zolang minstens één van de gebruikte sensoren nog ingedrukt blijft.
  Stimulus.WachtTotAlleSensorsLosgelatenVoorTest(aantalSensorenSimultaanTeMeten);

  // --- EENMALIGE NULMETING ---
  int aantalNulmetingPogingen = 0;
  bool nulmetingGoedgekeurd   = false;
  String herhalingStr         = "0M";

  // --- START VAN DE TRAININGSLUS ---
  for (int herhaling = 1; herhaling <= TEST_AANTAL_KEER_HERHALEN; herhaling++) {
    int WACHTTIJD_MS = random(MINIMALE_WACHTTIJD_MS, MAXIMALE_WACHTTIJD_MS);

    if (nulmetingGoedgekeurd) {
      Screen->Print(LCD_S0_LABEL_TIJDENS + String(Stimulus.nulmetingTikTijd) + LCD_S0_LABEL_MS, LCD_S0_TIK_AANHOUDEN, WACHTTIJD_MS);
    } else {
      Screen->Print(LCD_S0_NULMETING, LCD_S0_EVEN_GEDULD, WACHTTIJD_MS);
    }

    Stimulus.WachtTotAlleSensorsLosgelatenVoorTest(AANTAL_SENSOREN_AANWEZIG);
    Screen->Print("", "", 0, LCD_S0_NU);

    // Meet alle gebruikte sensoren en hun gezamenlijke synchronisatie.
    int exitStatus = Stimulus.MeetStimulusSimultaan(gemetenStimulus, aantalSensorenSimultaanTeMeten, gemetenSynchronisatie);
    if (exitStatus == EXIT_VOORWAARDE_NO_ACTION_TIMEOUT || exitStatus == EXIT_VOORWAARDE_TIMEOUT) { return; }

    unsigned long totaleTikTijd = 0, totaleTikKracht = 0;

    for (int sensorNummer = 0; sensorNummer < aantalSensorenSimultaanTeMeten; sensorNummer++) {
      totaleTikTijd += gemetenStimulus[sensorNummer].TikTijd;
      totaleTikKracht += gemetenStimulus[sensorNummer].gemiddeldeTikKracht;
    }

    unsigned long gematigdTijd = totaleTikTijd / aantalSensorenSimultaanTeMeten;
    int gematigdKracht = totaleTikKracht / aantalSensorenSimultaanTeMeten;

    unsigned long toegestaneMargeEindTijd = (gematigdTijd * Stimulus.TOEGESTANE_MARGE_TIKTIJD) / (Stimulus.MARGE_FACTOR * 100UL);
    bool startTijdSimultaan = gemetenSynchronisatie[0].verschilStartTijd <= Stimulus.TOEGESTANE_MARGE_SIMULTANE_STARTTIJD_MS;
    bool eindTijdSimultaan = gemetenSynchronisatie[0].verschilEindTijd <= toegestaneMargeEindTijd;

    if (nulmetingGoedgekeurd) {
      for (int sensorNummer = 0; sensorNummer < aantalSensorenSimultaanTeMeten; sensorNummer++) {
        if (gemetenStimulus[sensorNummer].TikTijd > EXIT_TIKTIJD_MS) { return; }
        Stimulus.VergelijkStimulus(nulmetingStimulus[sensorNummer], gemetenStimulus[sensorNummer], Stimulus.TijdCorrect, Stimulus.KrachtCorrect);
      }

      if (startTijdSimultaan) Stimulus.TELLER_SIMULTANE_START_OK++;
      if (eindTijdSimultaan) Stimulus.TELLER_SIMULTANE_EIND_OK++;
      if (startTijdSimultaan && eindTijdSimultaan) Stimulus.TELLER_TIKTIJD_SYNCHROON++;

      // Formatteer herhalingsteller (Twee digits) via String-klasse om pointers te vermijden
      herhalingStr = (herhaling < 10) ? ('0' + String(herhaling)) : String(herhaling);
    } else {
      int resultaatNulmeting = Stimulus.EvalueerNulmeting(gematigdTijd, gematigdKracht, nulmetingGoedgekeurd, Stimulus.nulmetingTikTijd, Stimulus.nulmetingTikKracht, herhaling, aantalNulmetingPogingen);

      if (resultaatNulmeting == -1) {
        return;
      } else if (resultaatNulmeting == 1) {
        if (!startTijdSimultaan) {
          nulmetingGoedgekeurd = false;
          Screen->Print(LCD_S0_NULMETING, LCD_S4_START_VERSCHIL, LCD_LEESTIJD_FEEDBACK_LANG_MS);
        } else if (!eindTijdSimultaan) {
          nulmetingGoedgekeurd = false;
          Screen->Print(LCD_S0_NULMETING, LCD_S4_TIJD_VERSCHILT, LCD_LEESTIJD_FEEDBACK_LANG_MS);
        } else {
          for (int sensorNummer = 0; sensorNummer < aantalSensorenSimultaanTeMeten; sensorNummer++) nulmetingStimulus[sensorNummer] = gemetenStimulus[sensorNummer];
          nulmetingSynchronisatie = gemetenSynchronisatie[0];
        }
      }
    }

#ifdef DEBUG
    GA_DEBUG_PRINTLN("---------------------");
    GA_DEBUG_PRINT("Scenario 4 herhaling = ");
    GA_DEBUG_PRINTLN(herhaling);

    GA_DEBUG_PRINT("Aantal sensoren = ");
    GA_DEBUG_PRINTLN(aantalSensorenSimultaanTeMeten);

    GA_DEBUG_PRINT("Gemiddelde TikTijd = ");
    GA_DEBUG_PRINTLN(gematigdTijd);

    GA_DEBUG_PRINT("Gemiddelde TikKracht = ");
    GA_DEBUG_PRINTLN(gematigdKracht);

    GA_DEBUG_PRINT("Eerste tot laatste start = ");
    GA_DEBUG_PRINTLN(gemetenSynchronisatie[0].verschilStartTijd);

    GA_DEBUG_PRINT("Toegestane marge start = ");
    GA_DEBUG_PRINTLN(Stimulus.TOEGESTANE_MARGE_SIMULTANE_STARTTIJD_MS);

    GA_DEBUG_PRINT("Eerste tot laatste einde = ");
    GA_DEBUG_PRINTLN(gemetenSynchronisatie[0].verschilEindTijd);

    GA_DEBUG_PRINT("Toegestane marge einde = ");
    GA_DEBUG_PRINTLN(toegestaneMargeEindTijd);

    GA_DEBUG_PRINT("Start/einde/synchroon = ");
    GA_DEBUG_PRINT(startTijdSimultaan);
    GA_DEBUG_PRINT(" / ");
    GA_DEBUG_PRINT(eindTijdSimultaan);
    GA_DEBUG_PRINT(" / ");
    GA_DEBUG_PRINTLN(startTijdSimultaan && eindTijdSimultaan);

    GA_DEBUG_PRINT("Aantal sensoren synchroon start = ");
    GA_DEBUG_PRINTLN(gemetenSynchronisatie[0].aantalSensorenSynchroonStart);

    GA_DEBUG_PRINT("Aantal sensoren synchroon einde = ");
    GA_DEBUG_PRINTLN(gemetenSynchronisatie[0].aantalSensorenSynchroonEinde);
#endif

    if (herhalingStr == "0M") {
      Screen->Print(herhalingStr + ' ' + LCD_SCORE_TIJD + String(gematigdTijd), LCD_SCORE_KRACHT + String(gematigdKracht), LCD_LEESTIJD_FEEDBACK_KORT_MS);
    } else {
      if (aantalSensorenSimultaanTeMeten == 2) { // 2 Sensoren, Scherm 1 (identiek aan scenario 2)
        Screen->Print(herhalingStr + ' ' + LCD_SCORE_TIJD + String(gemetenStimulus[0].TikTijd) + '/' + String(gemetenStimulus[1].TikTijd), LCD_SCORE_KRACHT + String(gemetenStimulus[0].gemiddeldeTikKracht) + '/' + String(gemetenStimulus[1].gemiddeldeTikKracht), LCD_LEESTIJD_FEEDBACK_KORT_MS);
      } else if (aantalSensorenSimultaanTeMeten == 3) { // Drie gebruikte sensoren bij vier hardwarematig aanwezige sensoren.
        /*
        Screen.Print(herhalingStr + ' ' + LCD_SCORE_TIJD + String(gemetenStimulus[0].TikTijd) + '/' + String(gemetenStimulus[1].TikTijd), LCD_SCORE_TIJD + String(gemetenStimulus[2].TikTijd), LCD_LEESTIJD_FEEDBACK_KORT_MS);
        Screen.Print(LCD_SCORE_KRACHT + String(gemetenStimulus[0].gemiddeldeTikKracht) + '/' + String(gemetenStimulus[1].gemiddeldeTikKracht), LCD_SCORE_KRACHT + String(gemetenStimulus[2].gemiddeldeTikKracht), LCD_LEESTIJD_FEEDBACK_KORT_MS);
        */
        Screen->Print(herhalingStr + ' ' + LCD_SCORE_TIJD + String(gemetenStimulus[0].TikTijd) + '/' + String(gemetenStimulus[1].TikTijd), LCD_SCORE_TIJD + String(gemetenStimulus[2].TikTijd), LCD_LEESTIJD_FEEDBACK_KORT_MS, "", 
                      LCD_SCORE_KRACHT + String(gemetenStimulus[0].gemiddeldeTikKracht) + '/' + String(gemetenStimulus[1].gemiddeldeTikKracht), LCD_SCORE_KRACHT + String(gemetenStimulus[2].gemiddeldeTikKracht), LCD_LEESTIJD_FEEDBACK_KORT_MS);
      } else { // 4 Sensoren, Scherm 1: tijden & Scherm 2: krachten
        /*
        Screen.Print(herhalingStr + ' ' + LCD_SCORE_TIJD + String(gemetenStimulus[0].TikTijd) + '/' + String(gemetenStimulus[1].TikTijd), LCD_SCORE_TIJD + String(gemetenStimulus[2].TikTijd) + '/' + String(gemetenStimulus[3].TikTijd), LCD_LEESTIJD_FEEDBACK_KORT_MS);
        Screen.Print(LCD_SCORE_KRACHT + String(gemetenStimulus[0].gemiddeldeTikKracht) + '/' + String(gemetenStimulus[1].gemiddeldeTikKracht), LCD_SCORE_KRACHT + String(gemetenStimulus[2].gemiddeldeTikKracht) + '/' + String(gemetenStimulus[3].gemiddeldeTikKracht), LCD_LEESTIJD_FEEDBACK_KORT_MS);
        */
        Screen->Print(herhalingStr + ' ' + LCD_SCORE_TIJD + String(gemetenStimulus[0].TikTijd) + '/' + String(gemetenStimulus[1].TikTijd), LCD_SCORE_TIJD + String(gemetenStimulus[2].TikTijd) + '/' + String(gemetenStimulus[3].TikTijd), LCD_LEESTIJD_FEEDBACK_KORT_MS, "", 
                      LCD_SCORE_KRACHT + String(gemetenStimulus[0].gemiddeldeTikKracht) + '/' + String(gemetenStimulus[1].gemiddeldeTikKracht), LCD_SCORE_KRACHT + String(gemetenStimulus[2].gemiddeldeTikKracht) + '/' + String(gemetenStimulus[3].gemiddeldeTikKracht), LCD_LEESTIJD_FEEDBACK_KORT_MS);
      }

      Screen->Print(LCD_S4_SYNCROON_START + String(gemetenSynchronisatie[0].aantalSensorenSynchroonStart) + '/' + String(aantalSensorenSimultaanTeMeten), LCD_S4_SYNCROON_EINDE + String(gemetenSynchronisatie[0].aantalSensorenSynchroonEinde) + '/' + String(aantalSensorenSimultaanTeMeten), LCD_LEESTIJD_FEEDBACK_KORT_MS);
    }
  }

  ToonEindScoreScenario4();
}

void ToonEindScoreScenario4() {
  int percentageStartOK   = (Stimulus.TELLER_SIMULTANE_START_OK * 100) / TEST_AANTAL_KEER_HERHALEN;
  int percentageEindOK    = (Stimulus.TELLER_SIMULTANE_EIND_OK * 100) / TEST_AANTAL_KEER_HERHALEN;
  int percentageSynchroon = (Stimulus.TELLER_TIKTIJD_SYNCHROON * 100) / TEST_AANTAL_KEER_HERHALEN;

  Screen->Print(LCD_S4_SYNCROON_START + String(percentageStartOK) + LCD_SCORE_PERCENTAGE, LCD_S4_SYNCROON_EINDE + String(percentageEindOK) + LCD_SCORE_PERCENTAGE, LCD_LEESTIJD_ENDSCORE_MS);
  Screen->Print(LCD_SCORE_SYNCHROON + String(percentageSynchroon) + LCD_SCORE_PERCENTAGE, "", LCD_LEESTIJD_ENDSCORE_MS);
  if (Stimulus.TELLER_TIKTIJD_SYNCHROON == TEST_AANTAL_KEER_HERHALEN) Screen->Print(LCD_FINALE_TITEL, LCD_FINALE_SUCCES, LCD_LEESTIJD_ENDSCORE_MS);
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
