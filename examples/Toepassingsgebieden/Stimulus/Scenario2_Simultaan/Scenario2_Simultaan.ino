// ============================================================================
// Scenario 2 — Simultaan
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
void ToonEindScoreScenario2();
void ToonMenuKiesEnStelLevelIn();
void UitvoerenAlgoritmeSimultaneTik();

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
