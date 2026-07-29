#include <Wire.h>

// OPMERKING: De keuze tussen <GroeiAcademie.h> en <Stimulus.h> bepaalt enkel welke namen je sketch mag aanroepen (zichtbaarheid), niet wat er gecompileerd wordt.
// #include <GroeiAcademie.h> // alles van GROEI ACADEMIE mag worden aangeroepen binnen deze sketch/arduino code
#include <Stimulus.h>         // enkel module: Stimulus van GROEI ACADEMIE mag worden aangeroepen binnen deze sketch/arduino code
#include <Configuratie/Examples.h>

#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
#include <Adafruit_ST7789.h>
Adafruit_ST7789 pixelScreen(PIXEL_SCREEN_CS, PIXEL_SCREEN_DC, PIXEL_SCREEN_RST);
#endif

// ============================================================================
// HARDWARE INSTELLINGEN (De pinnen van de Arduino UNO R3/R4)
// ============================================================================
#define SERIAL_BAUDRATE 115200UL

// Hardwarematig aanwezig zijn 2 of 4 sensoren. Scenarios 1, 2 en 3 gebruiken 2 sensoren. Scenario 4 kan bij 4 aanwezige sensoren ook 3 sensoren gebruiken.
// #define AANTAL_SENSOREN_AANWEZIG   2 // MAG MEN WEIZIGEN IN FUNCTIE VAN DE BESCHIKBARE HARDWARE 

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
int instortendOfGradueel = INSTORTEND_SCORING_BINAIR; // Kan verhoogd worden met instortendOfGradueel++ na succesvolle sessies (net als stimulusVersie)

// ============================================================================
// ============================================================================
// Prototypen voor functies om de juiste opbouwvolgorde te garanderen
// ============================================================================
void ToonEindScoreScenario3();
void ToonMenuKiesEnStelLevelIn();
void UitvoerenAlgoritmeIneenstortendeTik();

// ============================================================================

StimulusProfiel nulmetingStimulus[AANTAL_SENSOREN_AANWEZIG], gemetenStimulus[AANTAL_SENSOREN_AANWEZIG];

SynchronisatieProfiel nulmetingSynchronisatie, gemetenSynchronisatie[3];

// ============================================================================

int TEST_AANTAL_KEER_HERHALEN  = 5;

void setup() {
#if ADC_BITS == 14
  analogReadResolution(14);
#endif  

#if (SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER)
  lcd.init();      // set up the LCD's number of columns and rows
  lcd.backlight(); // turn on the backlight
#endif
#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
  pixelScreen.init(ACTIEF_PIXEL_SCREEN_BREEDTE, ACTIEF_PIXEL_SCREEN_HOOGTE);
  pixelScreen.setRotation(PIXEL_SCREEN_ROTATION);
  PixelScreen = &pixelScreen;
  PixelScreenConfigureren();
#endif
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
  UitvoerenAlgoritmeIneenstortendeTik();
}

// ============================================================================
// ALGORITME 3: INEENSTORTENDE TIK (Scenario 3)
// ============================================================================

void UitvoerenAlgoritmeIneenstortendeTik() {
  ResetAlleTellers();
  ToonMenuKiesEnStelLevelIn();
  PrintToScreen(LCD_S3_TITEL, LCD_S0_GEEF_STARTTIK, MINIMALE_WACHTTIJD_MS);
  WachtTotAlleSensorsLosgelatenVoorTest(AANTAL_SENSOREN_AANWEZIG);
  PrintToScreen("", "", 0, LCD_S0_NU);

  // eerste meting smijten we weg, geeft een valse waarde
  RawAnalogRead(PIN_SENSOR_1); RawAnalogRead(PIN_SENSOR_2);

  // STAP A: Blijf wachten tot een vinger sensor 1 AANRAAKT
  while (AnalogReadMetGekorigeerdeOffsets(PIN_SENSOR_1, offsetSensor1) <= TIK_MINIMALE_DRUKWAARDE);

  // STAP B: Blijf wachten tot een vinger sensor 1 LOSLAAT
  while (AnalogReadMetGekorigeerdeOffsets(PIN_SENSOR_1, offsetSensor1) > TIK_MINIMALE_DRUKWAARDE);

  // --- START VAN DE TRAININGSLUS ---
  for (int herhaling = 1; herhaling <= TEST_AANTAL_KEER_HERHALEN; herhaling++) {
#ifdef DEBUG
    DEBUG_PRINTLN("---------------------");
    DEBUG_PRINT("Scenario 3: herhaling = ");
    DEBUG_PRINTLN(herhaling);
#endif

    int aantalStappenSynchroon = 0;
    int exitStatus = EXIT_STATUS_GEEN;

    // STAP 1: SENSOR 1
    PrintToScreen(LCD_S3_STAP_1, LCD_S3_SENSOR_1, LCD_LEESTIJD_MEDEDELING_KORT_MS);
    WachtTotAlleSensorsLosgelatenVoorTest(AANTAL_SENSOREN_AANWEZIG);
    PrintToScreen("", "", 0, LCD_S0_NU);
    exitStatus = MeetStimulus(PIN_SENSOR_1, offsetSensor1, gemetenStimulus[0], PIN_SENSOR_2, offsetSensor2);

    // Alleen wanneer sensor 2 de exitsensor activeert, mag stap 2 starten.
    if (exitStatus != EXIT_VOORWAARDE_EXITSENSOR_INGEDRUKT) { return; }
    if (gemetenStimulus[0].TikTijd > EXIT_TIKTIJD_MS) { return; }

    // Stap 1 wordt de dynamische referentiemeting voor stap 2 binnen dezelfde ronde.
    nulmetingStimulus[0] = gemetenStimulus[0];

#ifdef DEBUG
    DEBUG_PRINTLN("---------------------");
    DEBUG_PRINT("Stap 1 nulmeting tijd/kracht = ");
    DEBUG_PRINT(nulmetingStimulus[0].TikTijd);
    DEBUG_PRINT(" / ");
    DEBUG_PRINTLN(nulmetingStimulus[0].gemiddeldeTikKracht);
#endif

    // STAP 2: SENSOR 1 EN SENSOR 2 SIMULTAAN
    PrintToScreen(LCD_S3_STAP_2, LCD_S3_SENSOR_1en2); // Geen wachttijd tussen stap 1 en stap 2.
    exitStatus = MeetStimulusSimultaan(gemetenStimulus, AANTAL_SENSOREN_ALGORITME3, gemetenSynchronisatie, 0b1100, 0b0100);

#ifdef DEBUG
    DEBUG_PRINTLN("---------------------");
    DEBUG_PRINT("Stap 2 exitStatus = ");
    DEBUG_PRINTLN(exitStatus);
#endif  

    // MaskReedsActieveSensorsBijStart = 0b1100: sensor 1 en sensor 2 zijn reeds actief bij aanvang van stap 2.
    // MaskGewensteActieveSensorsBijExit = 0b0100: exit wanneer enkel sensor 2 nog actief is (sensor 1 losgelaten).
    if (exitStatus != EXIT_VOORWAARDE_SENSOR_LOSGELATEN) { return; }
    VergelijkStimulus(nulmetingStimulus[0], gemetenStimulus[0], TijdCorrect, KrachtCorrect);

    bool Sensor1Correct = TijdCorrect && KrachtCorrect;
    VergelijkStimulus(nulmetingStimulus[0], gemetenStimulus[1], TijdCorrect, KrachtCorrect);

    // Voor sensor 2 telt in stap 2 alleen de TikTijd. De gemeten kracht wordt hieronder zijn eigen referentie voor stap 3.
    bool Sensor2Correct = TijdCorrect;
    nulmetingStimulus[1] = gemetenStimulus[1];
    unsigned long margeSimultaanTijd = (nulmetingStimulus[0].TikTijd * TOEGESTANE_MARGE_TIKTIJD) / (MARGE_FACTOR * 100UL);

    bool SimultaanCorrect = true;
    if (gemetenSynchronisatie[0].verschilStartTijd > margeSimultaanTijd) SimultaanCorrect = false;
    if (gemetenSynchronisatie[0].verschilTikTijd > margeSimultaanTijd) SimultaanCorrect = false;

    if (Sensor1Correct && Sensor2Correct && SimultaanCorrect) {
      aantalStappenSynchroon++;
      TELLER_TIKTIJD_SYNCHROON++; // SYNCHROON in Scenario 3 betekent dat de simultane tweede stap correct werd uitgevoerd.
    }

#ifdef DEBUG
    DEBUG_PRINTLN("---------------------");
    DEBUG_PRINT("Stap 2 sensor1/sensor2/simultaan = ");
    DEBUG_PRINT(Sensor1Correct);
    DEBUG_PRINT(" / ");
    DEBUG_PRINT(Sensor2Correct);
    DEBUG_PRINT(" / ");
    DEBUG_PRINTLN(SimultaanCorrect);
#endif

    // STAP 3 TOT n: SENSOR 2, EEN DOORLOPENDE METING
    PrintToScreen(LCD_S3_STAP_3_n, LCD_S3_SENSOR_2); // Geen wachttijd tussen stap 2 en stap 3.
    exitStatus = MeetStimulus(PIN_SENSOR_2, offsetSensor2, gemetenStimulus[1], -1, 0, INSTORTEND_MAXIMALE_FACTOR * MAXIMALE_TIKTIJD_MS + 1000UL);
    if (exitStatus == EXIT_VOORWAARDE_NO_ACTION_TIMEOUT || exitStatus == EXIT_VOORWAARDE_TIMEOUT) { return; }

    char instortendExtraTeken = '?';
    VergelijkStimulus(nulmetingStimulus[1], gemetenStimulus[1], TijdCorrect, KrachtCorrect, INSTORTEND_MOEILIJKHEIDSGRAAD_1, &instortendExtraTeken); // De minimale en maximale totale tiktijd van stap 3 worden rechtstreeks bepaald op basis van de tiktijd van sensor 2 in stap 2.

    // Scoringsvorm bepaald door level (instortendOfGradueel):
    // BINAIR   (level 1-3): tijd EN kracht beiden correct → volle punten, anders 0
    // GRADUEEL (level 4)  : tijd en kracht apart gescoord → elk de helft van de punten
    if (instortendOfGradueel == INSTORTEND_SCORING_GRADUEEL) {
      // Bij een oneven aantal stappen krijgt tijd het extra punt: bij 5 is tijd 3 en kracht 2.
      if (TijdCorrect)   aantalStappenSynchroon += (INSTORTEND_AANTAL_STAPPEN + 1) / 2;
      if (KrachtCorrect) aantalStappenSynchroon += INSTORTEND_AANTAL_STAPPEN / 2;
    } else {
      if (TijdCorrect && KrachtCorrect) aantalStappenSynchroon += INSTORTEND_AANTAL_STAPPEN;
    }

#ifdef DEBUG
    DEBUG_PRINTLN("---------------------");
    DEBUG_PRINT("Stap 3 tot n tijd/kracht = ");
    DEBUG_PRINT(TijdCorrect);
    DEBUG_PRINT(" / ");
    DEBUG_PRINTLN(KrachtCorrect);
#endif

    // Maximum per ronde = stap2(1) + stap3(INSTORTEND_AANTAL_STAPPEN)
    if (aantalStappenSynchroon == (1 + INSTORTEND_AANTAL_STAPPEN)) TELLER_INSTORTEND_CORRECT++;

#ifdef DEBUG
    DEBUG_PRINTLN("---------------------");
    DEBUG_PRINT("Aantal stappen synchroon = ");
    DEBUG_PRINTLN(aantalStappenSynchroon);
#endif

    String herhalingStr = (herhaling < 10) ? ('0' + String(herhaling)) : String(herhaling);
    // 0 < -> instortend niet geslaagd, tijd te kort
    // 0 > -> instortend niet geslaagd, tijd te lang
    // 0 = -> tijd van stap 3 is correct, maar de volledige ronde is niet geslaagd
    // 1 = -> tijd van stap 3 is correct én de volledige ronde is geslaagd
    PrintToScreen(herhalingStr + ' ' + LCD_SCORE_SYNCHROON + String(aantalStappenSynchroon), LCD_SCORE_INSTORTEND + String(aantalStappenSynchroon == (1 + INSTORTEND_AANTAL_STAPPEN)) + ' ' + instortendExtraTeken, LCD_LEESTIJD_FEEDBACK_KORT_MS);
  }

  ToonEindScoreScenario3();
}

void ToonEindScoreScenario3() {
  // Stap 1 = 1 punt, stap 2 = 1 punt, stap 3 = INSTORTEND_AANTAL_STAPPEN punten
  int totaalAantalKeren     = TEST_AANTAL_KEER_HERHALEN * 3; // VergelijkStimulus() wordt per ronde 3 keer aangeroepen.
  int percentageTikTijdOk   = (TELLER_TIKTIJD_CORRECT * 100) / totaalAantalKeren;
  int percentageTikKrachtOk = (TELLER_TIKKRACHT_CORRECT * 100) / totaalAantalKeren;
  int percentageSynchroon   = (TELLER_TIKTIJD_SYNCHROON * 100) / TEST_AANTAL_KEER_HERHALEN;
  int percentageInstortend  = (TELLER_INSTORTEND_CORRECT * 100) / TEST_AANTAL_KEER_HERHALEN;

  PrintToScreen(LCD_SCORE_TIJD + String(percentageTikTijdOk) + LCD_SCORE_PERCENTAGE, LCD_SCORE_KRACHT + String(percentageTikKrachtOk) + LCD_SCORE_PERCENTAGE, LCD_LEESTIJD_ENDSCORE_MS);
  PrintToScreen(LCD_SCORE_SYNCHROON + String(percentageSynchroon) + LCD_SCORE_PERCENTAGE, LCD_SCORE_INSTORTEND + String(percentageInstortend) + LCD_SCORE_PERCENTAGE, LCD_LEESTIJD_ENDSCORE_MS);
  if (TELLER_INSTORTEND_CORRECT == TEST_AANTAL_KEER_HERHALEN) PrintToScreen(LCD_FINALE_TITEL, LCD_FINALE_SUCCES, LCD_LEESTIJD_ENDSCORE_MS); // Alle rondes perfect gescoord
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
