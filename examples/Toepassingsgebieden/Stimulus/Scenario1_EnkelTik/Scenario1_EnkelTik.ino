// ============================================================================
// Scenario 1 — EnkelTik
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

// OPMERKING: De keuze tussen <GroeiAcademie.h> en <Stimulus.h> bepaalt enkel welke namen je sketch mag aanroepen (zichtbaarheid), niet wat er gecompileerd wordt.
// #include <GroeiAcademie.h> // alles van GROEI ACADEMIE mag worden aangeroepen binnen deze sketch/arduino code
#include <Stimulus.h>         // enkel module: Stimulus van GROEI ACADEMIE mag worden aangeroepen binnen deze sketch/arduino code
#include <Configuratie/Examples.h>
#include <Configuratie/ExamplesConfig.h>

#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
#include <Adafruit_ST7789.h>
Adafruit_ST7789 pixelScreen(PIXEL_SCREEN_CS, PIXEL_SCREEN_DC, PIXEL_SCREEN_RST);
#endif

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

#ifdef DEBUG
  Serial.begin(SERIAL_BAUDRATE);
  while (!Serial) { ; } // Wacht hier totdat er een seriële verbinding is
  DEBUG_PRINTLN("=== DEBUG GESTART ===");
#endif

#if (SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER)
  CharacterScreenConfigureren(); // I2C-handdruk + lcd.init()/backlight()
#endif
#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
  pixelScreen.init(ACTIEF_PIXEL_SCREEN_BREEDTE, ACTIEF_PIXEL_SCREEN_HOOGTE);
  pixelScreen.setRotation(PIXEL_SCREEN_ROTATION);
  PixelScreen = &pixelScreen;
  PixelScreenConfigureren();
#endif
  PrintToScreen(LCD_SERIEEL_L1, LCD_SERIEEL_L2);

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
  UitvoerenAlgoritmeEnkelTik();
}

// ============================================================================
// ALGORITME 1: ENKEL TIK (Scenario 1)
// ============================================================================
void UitvoerenAlgoritmeEnkelTik() {
  ResetAlleTellers();
  ToonMenuKiesEnStelLevelIn();
  PrintToScreen(LCD_S1_TITEL, LCD_S0_GEEF_STARTTIK, MINIMALE_WACHTTIJD_MS);   
  WachtTotAlleSensorsLosgelatenVoorTest(AANTAL_SENSOREN_AANWEZIG);
  PrintToScreen("", "", 0, LCD_S0_NU);

  while (TIK_TEST_ACTIEVE_VINGER == -1) {
#ifdef DEBUG
  DEBUG_PRINT("Actieve vinger = ");
  DEBUG_PRINTLN(TIK_TEST_ACTIEVE_VINGER);
#endif

    // eerste meting smijten we weg, geef valse waarde
    for (int sensorNummer = 0; sensorNummer < AANTAL_SENSOREN_AANWEZIG; sensorNummer++) { RawAnalogRead(sensorPin[sensorNummer]); }
     const int offsetSensor[4] = { offsetSensor1, offsetSensor2, offsetSensor3, offsetSensor4 };

    // bepaal welke sensor als eerste actief is
    for (int sensorNummer = AANTAL_SENSOREN_AANWEZIG - 1; sensorNummer >= 0; sensorNummer--) {
      if (AnalogReadMetGekorigeerdeOffsets(sensorPin[sensorNummer], offsetSensor[sensorNummer]) > TIK_MINIMALE_DRUKWAARDE) {
        TIK_TEST_ACTIEVE_VINGER = sensorPin[sensorNummer];
        offsetSensorActief = offsetSensor[sensorNummer];
        break;
      }
    }
  }

#ifdef DEBUG
  DEBUG_PRINT("Actieve vinger = ");
  DEBUG_PRINTLN(TIK_TEST_ACTIEVE_VINGER);
  DEBUG_PRINTLN("---------------------");
#endif

  while (AnalogReadMetGekorigeerdeOffsets(TIK_TEST_ACTIEVE_VINGER, offsetSensorActief) > TIK_MINIMALE_DRUKWAARDE);
  
  // --- EENMALIGE NULMETING ---
  int aantalNulmetingPogingen = 0;
  bool nulmetingGoedgekeurd   = false;
  String herhalingStr         = "0M"; // Nulmeting-label op LCD; wordt vervangen door rondenummer na goedkeuring
  
  // --- START VAN DE TRAININGSLUS ---
  for (int herhaling = 1; herhaling <= TEST_AANTAL_KEER_HERHALEN; herhaling++) {
    int WACHTTIJD_MS = random(MINIMALE_WACHTTIJD_MS, MAXIMALE_WACHTTIJD_MS);

    if (nulmetingGoedgekeurd) {  // Wanneer de nulmeting WEL is goedgekeurd (true)
      PrintToScreen(LCD_S0_LABEL_TIJDENS + String(nulmetingTikTijd) + LCD_S0_LABEL_MS, LCD_S0_TIK_AANHOUDEN, WACHTTIJD_MS);
    } else {
      PrintToScreen(LCD_S0_NULMETING, LCD_S0_EVEN_GEDULD, WACHTTIJD_MS);
    }

    WachtTotAlleSensorsLosgelatenVoorTest(AANTAL_SENSOREN_AANWEZIG);
    PrintToScreen("", "", 0, LCD_S0_NU);

    // Dit vangt te vroeg drukken of te laat loslaten fysiologisch perfect op.
    while (AnalogReadMetGekorigeerdeOffsets(TIK_TEST_ACTIEVE_VINGER, offsetSensorActief) > TIK_MINIMALE_DRUKWAARDE);

    // PAS ALS HET BORD VRIJ IS, meten we de échte, nieuwe reactie-tik als volledig StimulusProfiel:
    int exitStatus = MeetStimulus(TIK_TEST_ACTIEVE_VINGER, offsetSensorActief, gemetenStimulus[0]);
    if (exitStatus == EXIT_VOORWAARDE_NO_ACTION_TIMEOUT || exitStatus == EXIT_VOORWAARDE_TIMEOUT) { return; }

    if (nulmetingGoedgekeurd) {
      if (gemetenStimulus[0].TikTijd > EXIT_TIKTIJD_MS) { return; } // Noodstop check
      VergelijkStimulus(nulmetingStimulus[0], gemetenStimulus[0], TijdCorrect, KrachtCorrect);
      herhalingStr = (herhaling < 10) ? ('0' + String(herhaling)) : String(herhaling);
    } else {
      int resultaatNulmeting = EvalueerNulmeting(gemetenStimulus[0].TikTijd, gemetenStimulus[0].gemiddeldeTikKracht, nulmetingGoedgekeurd, nulmetingTikTijd, nulmetingTikKracht, herhaling, aantalNulmetingPogingen);

      if (resultaatNulmeting == -1) {
        return; // Verlaat direct de trainingslus omdat het maximum aantal pogingen is bereikt
      } else if (resultaatNulmeting == 1) {
        nulmetingStimulus[0] = gemetenStimulus[0];
      }
    }

    PrintToScreen(herhalingStr + ' ' + LCD_SCORE_TIKTIJD + String(gemetenStimulus[0].TikTijd), String(' ') + LCD_SCORE_TIKKRACHT + String(gemetenStimulus[0].gemiddeldeTikKracht), LCD_LEESTIJD_FEEDBACK_KORT_MS);
  }

  ToonEindScoreScenario1();
}

void ToonEindScoreScenario1() {
  // Berekening van de procentuele scores met integer-veiligheid (schaal 0-100%)
  int percentageTikTijdOk = (TELLER_TIKTIJD_CORRECT * 100) / TEST_AANTAL_KEER_HERHALEN;
  int percentageTikKrachtOk = (TELLER_TIKKRACHT_CORRECT * 100) / TEST_AANTAL_KEER_HERHALEN;

  // --- TOON SCORE TIKTIJD & TIKKRACHT ---
  PrintToScreen(LCD_SCORE_TIKTIJD + String(percentageTikTijdOk) + LCD_SCORE_PERCENTAGE, LCD_SCORE_TIKKRACHT + String(percentageTikKrachtOk) + LCD_SCORE_PERCENTAGE, LCD_LEESTIJD_ENDSCORE_MS);
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
