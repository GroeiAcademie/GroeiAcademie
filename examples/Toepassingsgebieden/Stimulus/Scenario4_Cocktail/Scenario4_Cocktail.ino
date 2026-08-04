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
  UitvoerenAlgoritmeCocktailTik();
}

// ============================================================================
// ALGORITME 4: COCKTAIL TIK (Scenario 4)
// ============================================================================
void UitvoerenAlgoritmeCocktailTik() {
  ResetAlleTellers();
  ToonMenuKiesEnStelLevelIn();
  PrintToScreen(LCD_S4_TITEL, "", MINIMALE_WACHTTIJD_MS);
  WachtTotAlleSensorsLosgelatenVoorTest(AANTAL_SENSOREN_AANWEZIG);
  PrintToScreen("", "", 0, LCD_S0_NU);
  int aantalSensorenSimultaanTeMeten;

#if AANTAL_SENSOREN_AANWEZIG == 2
  aantalSensorenSimultaanTeMeten = 2;
#else
  PrintToScreen(LCD_S4_SENSORS_TEXT, LCD_S4_SENSORS_KEUZE);

  // Wacht tot alle toetsen losgelaten zijn.
  while (digitalRead(PIN_TOETS_1) == LOW || digitalRead(PIN_TOETS_2) == LOW || digitalRead(PIN_TOETS_3) == LOW || digitalRead(PIN_TOETS_4) == LOW);

  while (true) {
    if (digitalRead(PIN_TOETS_2) == LOW) {
      while (digitalRead(PIN_TOETS_2) == LOW);
      aantalSensorenSimultaanTeMeten = 2;
      break;
    } else if (digitalRead(PIN_TOETS_3) == LOW) {
      while (digitalRead(PIN_TOETS_3) == LOW);
      aantalSensorenSimultaanTeMeten = 3;
      break;
    } else if (digitalRead(PIN_TOETS_4) == LOW) {
      while (digitalRead(PIN_TOETS_4) == LOW);
      aantalSensorenSimultaanTeMeten = 4;
      break;
    }
  }
#endif

  PrintToScreen(LCD_S4_TITEL, String(aantalSensorenSimultaanTeMeten) + LCD_S4_SENSOREN, MINIMALE_WACHTTIJD_MS);
  WachtTotAlleSensorsLosgelatenVoorTest(AANTAL_SENSOREN_AANWEZIG);
  PrintToScreen("", "", 0, LCD_S0_NU);

  // Eerste meting smijten we weg, geeft een valse waarde.
  for (int sensorNummer = 0; sensorNummer < aantalSensorenSimultaanTeMeten; sensorNummer++) RawAnalogRead(sensorPin[sensorNummer]);

  // STAP 1: Blijf wachten tot alle te meten sensoren ingedrukt zijn geweest.
  int aantalGestarteSensoren = 0;
  bool sensorGekozen[4] = { false, false, false, false };

  while (aantalGestarteSensoren < aantalSensorenSimultaanTeMeten) {
    for (int sensorNummer = 0; sensorNummer < aantalSensorenSimultaanTeMeten; sensorNummer++) {
      if (!sensorGekozen[sensorNummer] && AnalogReadMetGekorigeerdeOffsets(sensorPin[sensorNummer], sensorNummer == 0 ? offsetSensor1 : sensorNummer == 1 ? offsetSensor2 : sensorNummer == 2 ? offsetSensor3 : offsetSensor4) > TIK_MINIMALE_DRUKWAARDE) {
        sensorGekozen[sensorNummer] = true;
        aantalGestarteSensoren++;
      }
    }
  }

  // STAP 2: Blijf wachten zolang minstens één van de gebruikte sensoren nog ingedrukt blijft.
  WachtTotAlleSensorsLosgelatenVoorTest(aantalSensorenSimultaanTeMeten);

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

    // Meet alle gebruikte sensoren en hun gezamenlijke synchronisatie.
    int exitStatus = MeetStimulusSimultaan(gemetenStimulus, aantalSensorenSimultaanTeMeten, gemetenSynchronisatie);
    if (exitStatus == EXIT_VOORWAARDE_NO_ACTION_TIMEOUT || exitStatus == EXIT_VOORWAARDE_TIMEOUT) { return; }

    unsigned long totaleTikTijd = 0, totaleTikKracht = 0;

    for (int sensorNummer = 0; sensorNummer < aantalSensorenSimultaanTeMeten; sensorNummer++) {
      totaleTikTijd += gemetenStimulus[sensorNummer].TikTijd;
      totaleTikKracht += gemetenStimulus[sensorNummer].gemiddeldeTikKracht;
    }

    unsigned long gematigdTijd = totaleTikTijd / aantalSensorenSimultaanTeMeten;
    int gematigdKracht = totaleTikKracht / aantalSensorenSimultaanTeMeten;

    unsigned long toegestaneMargeEindTijd = (gematigdTijd * TOEGESTANE_MARGE_TIKTIJD) / (MARGE_FACTOR * 100UL);
    bool startTijdSimultaan = gemetenSynchronisatie[0].verschilStartTijd <= TOEGESTANE_MARGE_SIMULTANE_STARTTIJD_MS;
    bool eindTijdSimultaan = gemetenSynchronisatie[0].verschilEindTijd <= toegestaneMargeEindTijd;

    if (nulmetingGoedgekeurd) {
      for (int sensorNummer = 0; sensorNummer < aantalSensorenSimultaanTeMeten; sensorNummer++) {
        if (gemetenStimulus[sensorNummer].TikTijd > EXIT_TIKTIJD_MS) { return; }
        VergelijkStimulus(nulmetingStimulus[sensorNummer], gemetenStimulus[sensorNummer], TijdCorrect, KrachtCorrect);
      }

      if (startTijdSimultaan) TELLER_SIMULTANE_START_OK++;
      if (eindTijdSimultaan) TELLER_SIMULTANE_EIND_OK++;
      if (startTijdSimultaan && eindTijdSimultaan) TELLER_TIKTIJD_SYNCHROON++;

      // Formatteer herhalingsteller (Twee digits) via String-klasse om pointers te vermijden
      herhalingStr = (herhaling < 10) ? ('0' + String(herhaling)) : String(herhaling);
    } else {
      int resultaatNulmeting = EvalueerNulmeting(gematigdTijd, gematigdKracht, nulmetingGoedgekeurd, nulmetingTikTijd, nulmetingTikKracht, herhaling, aantalNulmetingPogingen);

      if (resultaatNulmeting == -1) {
        return;
      } else if (resultaatNulmeting == 1) {
        if (!startTijdSimultaan) {
          nulmetingGoedgekeurd = false;
          PrintToScreen(LCD_S0_NULMETING, LCD_S4_START_VERSCHIL, LCD_LEESTIJD_FEEDBACK_LANG_MS);
        } else if (!eindTijdSimultaan) {
          nulmetingGoedgekeurd = false;
          PrintToScreen(LCD_S0_NULMETING, LCD_S4_TIJD_VERSCHILT, LCD_LEESTIJD_FEEDBACK_LANG_MS);
        } else {
          for (int sensorNummer = 0; sensorNummer < aantalSensorenSimultaanTeMeten; sensorNummer++) nulmetingStimulus[sensorNummer] = gemetenStimulus[sensorNummer];
          nulmetingSynchronisatie = gemetenSynchronisatie[0];
        }
      }
    }

#ifdef DEBUG
    DEBUG_PRINTLN("---------------------");
    DEBUG_PRINT("Scenario 4 herhaling = ");
    DEBUG_PRINTLN(herhaling);

    DEBUG_PRINT("Aantal sensoren = ");
    DEBUG_PRINTLN(aantalSensorenSimultaanTeMeten);

    DEBUG_PRINT("Gemiddelde TikTijd = ");
    DEBUG_PRINTLN(gematigdTijd);

    DEBUG_PRINT("Gemiddelde TikKracht = ");
    DEBUG_PRINTLN(gematigdKracht);

    DEBUG_PRINT("Eerste tot laatste start = ");
    DEBUG_PRINTLN(gemetenSynchronisatie[0].verschilStartTijd);

    DEBUG_PRINT("Toegestane marge start = ");
    DEBUG_PRINTLN(TOEGESTANE_MARGE_SIMULTANE_STARTTIJD_MS);

    DEBUG_PRINT("Eerste tot laatste einde = ");
    DEBUG_PRINTLN(gemetenSynchronisatie[0].verschilEindTijd);

    DEBUG_PRINT("Toegestane marge einde = ");
    DEBUG_PRINTLN(toegestaneMargeEindTijd);

    DEBUG_PRINT("Start/einde/synchroon = ");
    DEBUG_PRINT(startTijdSimultaan);
    DEBUG_PRINT(" / ");
    DEBUG_PRINT(eindTijdSimultaan);
    DEBUG_PRINT(" / ");
    DEBUG_PRINTLN(startTijdSimultaan && eindTijdSimultaan);

    DEBUG_PRINT("Aantal sensoren synchroon start = ");
    DEBUG_PRINTLN(gemetenSynchronisatie[0].aantalSensorenSynchroonStart);

    DEBUG_PRINT("Aantal sensoren synchroon einde = ");
    DEBUG_PRINTLN(gemetenSynchronisatie[0].aantalSensorenSynchroonEinde);
#endif

    if (herhalingStr == "0M") {
      PrintToScreen(herhalingStr + ' ' + LCD_SCORE_TIJD + String(gematigdTijd), LCD_SCORE_KRACHT + String(gematigdKracht), LCD_LEESTIJD_FEEDBACK_KORT_MS);
    } else {
      if (aantalSensorenSimultaanTeMeten == 2) { // 2 Sensoren, Scherm 1 (identiek aan scenario 2)
        PrintToScreen(herhalingStr + ' ' + LCD_SCORE_TIJD + String(gemetenStimulus[0].TikTijd) + '/' + String(gemetenStimulus[1].TikTijd), LCD_SCORE_KRACHT + String(gemetenStimulus[0].gemiddeldeTikKracht) + '/' + String(gemetenStimulus[1].gemiddeldeTikKracht), LCD_LEESTIJD_FEEDBACK_KORT_MS);
      } else if (aantalSensorenSimultaanTeMeten == 3) { // Drie gebruikte sensoren bij vier hardwarematig aanwezige sensoren.
        /*
        PrintToScreen(herhalingStr + ' ' + LCD_SCORE_TIJD + String(gemetenStimulus[0].TikTijd) + '/' + String(gemetenStimulus[1].TikTijd), LCD_SCORE_TIJD + String(gemetenStimulus[2].TikTijd), LCD_LEESTIJD_FEEDBACK_KORT_MS);
        PrintToScreen(LCD_SCORE_KRACHT + String(gemetenStimulus[0].gemiddeldeTikKracht) + '/' + String(gemetenStimulus[1].gemiddeldeTikKracht), LCD_SCORE_KRACHT + String(gemetenStimulus[2].gemiddeldeTikKracht), LCD_LEESTIJD_FEEDBACK_KORT_MS);
        */
        PrintToScreen(herhalingStr + ' ' + LCD_SCORE_TIJD + String(gemetenStimulus[0].TikTijd) + '/' + String(gemetenStimulus[1].TikTijd), LCD_SCORE_TIJD + String(gemetenStimulus[2].TikTijd), LCD_LEESTIJD_FEEDBACK_KORT_MS, "", 
                      LCD_SCORE_KRACHT + String(gemetenStimulus[0].gemiddeldeTikKracht) + '/' + String(gemetenStimulus[1].gemiddeldeTikKracht), LCD_SCORE_KRACHT + String(gemetenStimulus[2].gemiddeldeTikKracht), LCD_LEESTIJD_FEEDBACK_KORT_MS);
      } else { // 4 Sensoren, Scherm 1: tijden & Scherm 2: krachten
        /*
        PrintToScreen(herhalingStr + ' ' + LCD_SCORE_TIJD + String(gemetenStimulus[0].TikTijd) + '/' + String(gemetenStimulus[1].TikTijd), LCD_SCORE_TIJD + String(gemetenStimulus[2].TikTijd) + '/' + String(gemetenStimulus[3].TikTijd), LCD_LEESTIJD_FEEDBACK_KORT_MS);
        PrintToScreen(LCD_SCORE_KRACHT + String(gemetenStimulus[0].gemiddeldeTikKracht) + '/' + String(gemetenStimulus[1].gemiddeldeTikKracht), LCD_SCORE_KRACHT + String(gemetenStimulus[2].gemiddeldeTikKracht) + '/' + String(gemetenStimulus[3].gemiddeldeTikKracht), LCD_LEESTIJD_FEEDBACK_KORT_MS);
        */
        PrintToScreen(herhalingStr + ' ' + LCD_SCORE_TIJD + String(gemetenStimulus[0].TikTijd) + '/' + String(gemetenStimulus[1].TikTijd), LCD_SCORE_TIJD + String(gemetenStimulus[2].TikTijd) + '/' + String(gemetenStimulus[3].TikTijd), LCD_LEESTIJD_FEEDBACK_KORT_MS, "", 
                      LCD_SCORE_KRACHT + String(gemetenStimulus[0].gemiddeldeTikKracht) + '/' + String(gemetenStimulus[1].gemiddeldeTikKracht), LCD_SCORE_KRACHT + String(gemetenStimulus[2].gemiddeldeTikKracht) + '/' + String(gemetenStimulus[3].gemiddeldeTikKracht), LCD_LEESTIJD_FEEDBACK_KORT_MS);
      }

      PrintToScreen(LCD_S4_SYNCROON_START + String(gemetenSynchronisatie[0].aantalSensorenSynchroonStart) + '/' + String(aantalSensorenSimultaanTeMeten), LCD_S4_SYNCROON_EINDE + String(gemetenSynchronisatie[0].aantalSensorenSynchroonEinde) + '/' + String(aantalSensorenSimultaanTeMeten), LCD_LEESTIJD_FEEDBACK_KORT_MS);
    }
  }

  ToonEindScoreScenario4();
}

void ToonEindScoreScenario4() {
  int percentageStartOK   = (TELLER_SIMULTANE_START_OK * 100) / TEST_AANTAL_KEER_HERHALEN;
  int percentageEindOK    = (TELLER_SIMULTANE_EIND_OK * 100) / TEST_AANTAL_KEER_HERHALEN;
  int percentageSynchroon = (TELLER_TIKTIJD_SYNCHROON * 100) / TEST_AANTAL_KEER_HERHALEN;

  PrintToScreen(LCD_S4_SYNCROON_START + String(percentageStartOK) + LCD_SCORE_PERCENTAGE, LCD_S4_SYNCROON_EINDE + String(percentageEindOK) + LCD_SCORE_PERCENTAGE, LCD_LEESTIJD_ENDSCORE_MS);
  PrintToScreen(LCD_SCORE_SYNCHROON + String(percentageSynchroon) + LCD_SCORE_PERCENTAGE, "", LCD_LEESTIJD_ENDSCORE_MS);
  if (TELLER_TIKTIJD_SYNCHROON == TEST_AANTAL_KEER_HERHALEN) PrintToScreen(LCD_FINALE_TITEL, LCD_FINALE_SUCCES, LCD_LEESTIJD_ENDSCORE_MS);
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
