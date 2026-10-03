// Stimulus-voorbeeld v2.0.0
// ============================================================================
// Tik — Enkel, samen, instortend en cocktail
// ============================================================================
// ============================================================================
// Dit example dwingt ADC_BACKEND/SCREEN_OUTPUT_CONFIG NIET zelf af — dat kan een .ino structureel niet: 
// Stimulus.cpp/Screen.cpp worden als aparte bestanden gecompileerd en zien een #define hier nooit. 
// Deze sketch werkt met welke ADC-backend en welk schermtype dan ook actief is via UserConfig.h (kopieer van UserConfig_template.h) of SystemConfig.h, 
// en past haar gedrag/ aan via de #if-controles hieronder — vandaar geen #error nodig hier.
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
#ifndef INPUT_TEST_GEBRUIK_FUNCTIEMAPPING
  #define INPUT_TEST_GEBRUIK_FUNCTIEMAPPING 0
#endif

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
// Prototypen voor functies om de juiste opbouwvolgorde te garanderen
// ============================================================================
void ToonEindScoreScenario1();
void ToonEindScoreScenario2();
void ToonEindScoreScenario3();
void ToonEindScoreScenario4();
void ToonMenuKiesEnStelLevelIn();

void UitvoerenAlgoritmeEnkelTik();
void UitvoerenAlgoritmeSimultaneTik();
void UitvoerenAlgoritmeIneenstortendeTik();
void UitvoerenAlgoritmeCocktailTik();

// ============================================================================

StimulusProfiel nulmetingStimulus[AANTAL_SENSOREN_AANWEZIG], gemetenStimulus[AANTAL_SENSOREN_AANWEZIG];

SynchronisatieProfiel nulmetingSynchronisatie, gemetenSynchronisatie[3];

// ============================================================================

int TEST_AANTAL_KEER_HERHALEN  = 5;

// ============================================================================
// SETUP: DE OPSTARTFASE
// ============================================================================
const MappingTussenToetsaanslagEnUitTeVoerenFunctie mappingKeuzeMenu[] = {
  {_LABEL_OPSCHRIFT_1, UitvoerenAlgoritmeEnkelTik},
  {_LABEL_OPSCHRIFT_2, UitvoerenAlgoritmeSimultaneTik},
  {_LABEL_OPSCHRIFT_3, UitvoerenAlgoritmeIneenstortendeTik},
  {_LABEL_OPSCHRIFT_4, UitvoerenAlgoritmeCocktailTik}
};

void setup() {
#if ADC_BITS == 12 || ADC_BITS == 14
  analogReadResolution(ADC_BITS);
#endif  

  Screen = GedeeldeBusNewComponent<struct Screen>();
//  Screen = GedeeldeBusNewComponent<struct Screen>(MijnCharacterScreen, MijnPixelScreen);  
//  Screen = GedeeldeBusNewComponent<struct Screen>(SCREEN_TYPE_CHARACTER | SCREEN_TYPE_PIXELS, MijnCharacterScreen, MijnPixelScreen);
  if (Screen == nullptr) exit(0);

#ifdef DEBUG
  GA_DEBUG_PRINTLN("=== DEBUG GESTART ===");
#endif
  Screen->Print(LCD_SERIEEL_L1, LCD_SERIEEL_L2);

  sensorRFP602 = GedeeldeBusNewComponent<struct RFP602>();
  if (sensorRFP602 == nullptr) exit(0);

  Input = GedeeldeBusNewComponent<struct Input>();
  if (Input == nullptr) exit(0);

  Screen->Print(LCD_START_L1, LCD_START_L2, LCD_LEESTIJD_MEDEDELING_KORT_MS);
  Stimulus.ResetAlleTellers();

  Screen->Print(LCD_NULMETING_L1, LCD_NULMETING_L2, LCD_LEESTIJD_MEDEDELING_KORT_MS);
  Stimulus.BepaalSensorOffsets();  
}

// ============================================================================
// LOOP: HET HOOFDPROGRAMMA EN MENU
// ============================================================================
void loop() {
  Screen->Print(LCD_MENU_L1, LCD_MENU_L2);

  // Wacht tot alle toetsen losgelaten zijn.
  /*-- while (digitalRead(PIN_TOETS_1) == LOW || digitalRead(PIN_TOETS_2) == LOW || digitalRead(PIN_TOETS_3) == LOW || digitalRead(PIN_TOETS_4) == LOW); */

  while (true) {
    #if INPUT_TEST_GEBRUIK_FUNCTIEMAPPING
      UitVoerenFunctieVolgensMappingMetToetsAanslag(true, mappingKeuzeMenu);
      break;
    #else
      InputResultaat invoer = Input->OpvragenHuidigeToetsAanslag(true);
      const char* opschriftToetsAanslag = invoer.opschriftToetsAanslag;
    if (ToetsPositieIngedrukt(opschriftToetsAanslag, 1)) {
      //-- while (digitalRead(PIN_TOETS_1) == LOW);
      UitvoerenAlgoritmeEnkelTik();
      break;
    } else if (ToetsPositieIngedrukt(opschriftToetsAanslag, 2)) {
      //-- while (digitalRead(PIN_TOETS_2) == LOW);
      UitvoerenAlgoritmeSimultaneTik();
      break;
    } else if (ToetsPositieIngedrukt(opschriftToetsAanslag, 3)) {
      //-- while (digitalRead(PIN_TOETS_3) == LOW);
      UitvoerenAlgoritmeIneenstortendeTik();
      break;
    } else if (ToetsPositieIngedrukt(opschriftToetsAanslag, 4)) {
      //-- while (digitalRead(PIN_TOETS_4) == LOW);
      UitvoerenAlgoritmeCocktailTik();
      break;
    }
    #endif
  }
}

// ============================================================================
// VOORBEELD CALLBACK VOOR EEN CARACTER SCREEN
// ============================================================================

#if (SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER)
void MijnCharacterScreen(const String& eersteRegel, const String& tweedeRegel, unsigned long delayTime, const String& action, const String& derdeRegel, const String& vierdeRegel, unsigned long delayTussenPaginas) {
  if (eersteRegel != "" || tweedeRegel != "") {
    String eersteRegelLC = eersteRegel; eersteRegelLC.toLowerCase();
    String tweedeRegelLC = tweedeRegel; tweedeRegelLC.toLowerCase();

    Screen->Character.display.clear();
    Screen->Character.display.setCursor(0, 0); Screen->Character.display.print(eersteRegelLC);
    Screen->Character.display.setCursor(0, 1); Screen->Character.display.print(tweedeRegelLC);
  }

  if (derdeRegel != "" || vierdeRegel != "") {
    String derdeRegelLC = derdeRegel; derdeRegelLC.toLowerCase();
    String vierdeRegelLC = vierdeRegel; vierdeRegelLC.toLowerCase();

    if (!ACTIEF_CHARACTER_SCREEN_MET_VIER_REGELS) {
      if (delayTussenPaginas) delay(delayTussenPaginas);
      Screen->Character.display.clear();
    }

    Screen->Character.display.setCursor(0, ACTIEF_CHARACTER_SCREEN_MET_VIER_REGELS ? 2 : 0); Screen->Character.display.print(derdeRegelLC);
    Screen->Character.display.setCursor(0, ACTIEF_CHARACTER_SCREEN_MET_VIER_REGELS ? 3 : 1); Screen->Character.display.print(vierdeRegelLC);
  }

  if (delayTime) delay(delayTime);

  if (action != "") {
    String actionLC = action; actionLC.toLowerCase();
    Screen->Character.display.print(actionLC);
  }
}
#endif

// ============================================================================
// VOORBEELD CALLBACK VOOR EEN PIXEL SCREEN
// ============================================================================

void MijnPixelScreen(const String& eersteRegel, const String& tweedeRegel, unsigned long delayTime, const String& action, const String& derdeRegel, const String& vierdeRegel, unsigned long delayTussenPaginas) {
  // Scherm wissen of bestaande inhoud behouden
  // eersteRegel, tweedeRegel, derdeRegel en vierdeRegel tekenen
  // Indien nodig pagineren en delayTussenPaginas uitvoeren
  // Schermbuffer tonen
  // Indien nodig delayTime uitvoeren
  // Daarna action tekenen of verwerken
}

// ============================================================================
// LOCAAL GEBRUIK
// ============================================================================

void VoorbeeldGebruik() {
  Screen->Print(LCD_S3_STAP_2, LCD_S3_SENSOR_1en2);

  Screen->Print(LCD_S3_STAP_1, LCD_S3_SENSOR_1, LCD_LEESTIJD_MEDEDELING_KORT_MS);

  Screen->Print(LCD_S3_STAP_1, LCD_S3_SENSOR_1, LCD_LEESTIJD_MEDEDELING_KORT_MS, LCD_S0_NU);

  Screen->Print(LCD_S3_STAP_1, LCD_S3_SENSOR_1, LCD_LEESTIJD_MEDEDELING_KORT_MS);
  Stimulus.WachtTotAlleSensorsLosgelatenVoorTest(AANTAL_SENSOREN_AANWEZIG);
  Screen->Print("", "", 0, LCD_S0_NU);
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

// ============================================================================
// ALGORITME 2: SIMULTANE TIK (Scenario 2)
// ============================================================================

void UitvoerenAlgoritmeSimultaneTik() {
  const int offsetSensor[4] = { Stimulus.offsetSensor1, Stimulus.offsetSensor2, Stimulus.offsetSensor3, Stimulus.offsetSensor4 };

  Stimulus.ResetAlleTellers();
  ToonMenuKiesEnStelLevelIn();
  Screen->Print(LCD_S2_TITEL, LCD_S2_KIES_SENSOREN, MINIMALE_WACHTTIJD_MS);
  Stimulus.WachtTotAlleSensorsLosgelatenVoorTest(AANTAL_SENSOREN_AANWEZIG);
  Screen->Print("", "", 0, LCD_S0_NU);

  // Eerste meting smijten we weg, geeft een valse waarde.
  for (int sensorNummer = 0; sensorNummer < AANTAL_SENSOREN_ALGORITME2; sensorNummer++) sensorRFP602->RawAnalogRead(sensorRFP602->sensorPin[sensorNummer]);

  // STAP 1: Blijf wachten tot twee sensoren ingedrukt zijn geweest.
  int eersteGekozenSensorNummer = -1, tweedeGekozenSensorNummer = -1;

  while (eersteGekozenSensorNummer == -1 || tweedeGekozenSensorNummer == -1) {
    for (int sensorNummer = 0; sensorNummer < AANTAL_SENSOREN_ALGORITME2; sensorNummer++) {
      if (Stimulus.AnalogReadMetGekorigeerdeOffsets(sensorRFP602->sensorPin[sensorNummer], offsetSensor[sensorNummer]) > TIK_MINIMALE_DRUKWAARDE) {
        if (eersteGekozenSensorNummer == -1) {
          eersteGekozenSensorNummer = sensorNummer;
        } else if (tweedeGekozenSensorNummer == -1 && eersteGekozenSensorNummer != sensorNummer) {
          tweedeGekozenSensorNummer = sensorNummer;
        }
      }
    }
  }

  // STAP 2: Blijf wachten zolang minstens één van de twee gekozen sensoren nog ingedrukt blijft.
  while (Stimulus.AnalogReadMetGekorigeerdeOffsets(sensorRFP602->sensorPin[eersteGekozenSensorNummer], offsetSensor[eersteGekozenSensorNummer]) > TIK_MINIMALE_DRUKWAARDE ||
         Stimulus.AnalogReadMetGekorigeerdeOffsets(sensorRFP602->sensorPin[tweedeGekozenSensorNummer], offsetSensor[tweedeGekozenSensorNummer]) > TIK_MINIMALE_DRUKWAARDE);

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

    // Meet de twee gekozen sensoren en hun onderlinge synchronisatie.
    int exitStatus = Stimulus.MeetStimulusSimultaan(gemetenStimulus, AANTAL_SENSOREN_ALGORITME2, gemetenSynchronisatie);
    if (exitStatus == EXIT_VOORWAARDE_NO_ACTION_TIMEOUT || exitStatus == EXIT_VOORWAARDE_TIMEOUT) { return; }

    // --- EVALUATIEFITTING ---
    if (nulmetingGoedgekeurd) {
      if (gemetenStimulus[eersteGekozenSensorNummer].TikTijd > EXIT_TIKTIJD_MS || gemetenStimulus[tweedeGekozenSensorNummer].TikTijd > EXIT_TIKTIJD_MS) { return; } // Noodstop check

      Stimulus.VergelijkStimulus(nulmetingStimulus[eersteGekozenSensorNummer], gemetenStimulus[eersteGekozenSensorNummer], Stimulus.TijdCorrect, Stimulus.KrachtCorrect);
      Stimulus.VergelijkStimulus(nulmetingStimulus[tweedeGekozenSensorNummer], gemetenStimulus[tweedeGekozenSensorNummer], Stimulus.TijdCorrect, Stimulus.KrachtCorrect);
      Stimulus.VergelijkSynchronisatie(nulmetingSynchronisatie, gemetenSynchronisatie[0]);

      // Formatteer herhalingsteller (Twee digits) via String-klasse om pointers te vermijden
      herhalingStr = (herhaling < 10) ? ('0' + String(herhaling)) : String(herhaling);
    } else {  // Nulmeting evalueert op basis van de gecombineerde gemiddelden van de twee gekozen sensoren
      unsigned long gematigdTijd = (gemetenStimulus[eersteGekozenSensorNummer].TikTijd + gemetenStimulus[tweedeGekozenSensorNummer].TikTijd) / 2;
      int gematigdKracht = (gemetenStimulus[eersteGekozenSensorNummer].gemiddeldeTikKracht + gemetenStimulus[tweedeGekozenSensorNummer].gemiddeldeTikKracht) / 2;

      bool startTijdSimultaan = gemetenSynchronisatie[0].verschilStartTijd <= Stimulus.TOEGESTANE_MARGE_SIMULTANE_STARTTIJD_MS;
      unsigned long toegestaneVerschilTikTijd = gematigdTijd * Stimulus.TOEGESTANE_MARGE_TIKTIJD / 100;
      bool tikTijdSimultaan = gemetenSynchronisatie[0].verschilTikTijd <= toegestaneVerschilTikTijd;

      int resultaatNulmeting = Stimulus.EvalueerNulmeting(gematigdTijd, gematigdKracht, nulmetingGoedgekeurd, Stimulus.nulmetingTikTijd, Stimulus.nulmetingTikKracht, herhaling, aantalNulmetingPogingen);

      if (resultaatNulmeting == -1) {
        return;
      } else if (resultaatNulmeting == 1) {
        if (!startTijdSimultaan) {
          nulmetingGoedgekeurd = false;
          Screen->Print(LCD_S0_NULMETING, LCD_S2_START_VERSCHIL, LCD_LEESTIJD_FEEDBACK_LANG_MS);
        } else if (!tikTijdSimultaan) {
          nulmetingGoedgekeurd = false;
          Screen->Print(LCD_S0_NULMETING, LCD_S2_TIJD_VERSCHILT, LCD_LEESTIJD_FEEDBACK_LANG_MS);
        } else {
          nulmetingStimulus[eersteGekozenSensorNummer] = gemetenStimulus[eersteGekozenSensorNummer];
          nulmetingStimulus[tweedeGekozenSensorNummer] = gemetenStimulus[tweedeGekozenSensorNummer];
          nulmetingSynchronisatie = gemetenSynchronisatie[0];
        }
      }
    }

    // Directe feedback van beide metingen gescheiden door een slash '/' conform jouw opzet
    Screen->Print(herhalingStr + ' ' + LCD_SCORE_TIJD + String(gemetenStimulus[eersteGekozenSensorNummer].TikTijd) + '/' + String(gemetenStimulus[tweedeGekozenSensorNummer].TikTijd), LCD_SCORE_KRACHT + String(gemetenStimulus[eersteGekozenSensorNummer].gemiddeldeTikKracht) + '/' + String(gemetenStimulus[tweedeGekozenSensorNummer].gemiddeldeTikKracht), LCD_LEESTIJD_FEEDBACK_KORT_MS);
  }

  ToonEindScoreScenario2(); 
}     

// ============================================================================
// ALGORITME 3: INEENSTORTENDE TIK (Scenario 3)
// ============================================================================
void UitvoerenAlgoritmeIneenstortendeTik() {
  Stimulus.ResetAlleTellers();
  ToonMenuKiesEnStelLevelIn();
  Screen->Print(LCD_S3_TITEL, LCD_S0_GEEF_STARTTIK, MINIMALE_WACHTTIJD_MS);
  Stimulus.WachtTotAlleSensorsLosgelatenVoorTest(AANTAL_SENSOREN_AANWEZIG);
  Screen->Print("", "", 0, LCD_S0_NU);

  // eerste meting smijten we weg, geeft een valse waarde
  sensorRFP602->RawAnalogRead(sensorRFP602->sensorPin[0]); sensorRFP602->RawAnalogRead(sensorRFP602->sensorPin[1]);

  // STAP A: Blijf wachten tot een vinger sensor 1 AANRAAKT
  while (Stimulus.AnalogReadMetGekorigeerdeOffsets(sensorRFP602->sensorPin[0], Stimulus.offsetSensor1) <= TIK_MINIMALE_DRUKWAARDE);

  // STAP B: Blijf wachten tot een vinger sensor 1 LOSLAAT
  while (Stimulus.AnalogReadMetGekorigeerdeOffsets(sensorRFP602->sensorPin[0], Stimulus.offsetSensor1) > TIK_MINIMALE_DRUKWAARDE);

  // --- START VAN DE TRAININGSLUS ---
  for (int herhaling = 1; herhaling <= TEST_AANTAL_KEER_HERHALEN; herhaling++) {
#ifdef DEBUG
    GA_DEBUG_PRINTLN("---------------------");
    GA_DEBUG_PRINT("Scenario 3: herhaling = ");
    GA_DEBUG_PRINTLN(herhaling);
#endif

    int aantalStappenSynchroon = 0;
    int exitStatus = EXIT_STATUS_GEEN;

    // STAP 1: SENSOR 1
    Screen->Print(LCD_S3_STAP_1, LCD_S3_SENSOR_1, LCD_LEESTIJD_MEDEDELING_KORT_MS);
    Stimulus.WachtTotAlleSensorsLosgelatenVoorTest(AANTAL_SENSOREN_AANWEZIG);
    Screen->Print("", "", 0, LCD_S0_NU);
    exitStatus = Stimulus.MeetStimulus(sensorRFP602->sensorPin[0], Stimulus.offsetSensor1, gemetenStimulus[0], sensorRFP602->sensorPin[1], Stimulus.offsetSensor2);

    // Alleen wanneer sensor 2 de exitsensor activeert, mag stap 2 starten.
    if (exitStatus != EXIT_VOORWAARDE_EXITSENSOR_INGEDRUKT) { return; }
    if (gemetenStimulus[0].TikTijd > EXIT_TIKTIJD_MS) { return; }

    // Stap 1 wordt de dynamische referentiemeting voor stap 2 binnen dezelfde ronde.
    nulmetingStimulus[0] = gemetenStimulus[0];

#ifdef DEBUG
    GA_DEBUG_PRINTLN("---------------------");
    GA_DEBUG_PRINT("Stap 1 nulmeting tijd/kracht = ");
    GA_DEBUG_PRINT(nulmetingStimulus[0].TikTijd);
    GA_DEBUG_PRINT(" / ");
    GA_DEBUG_PRINTLN(nulmetingStimulus[0].gemiddeldeTikKracht);
#endif

    // STAP 2: SENSOR 1 EN SENSOR 2 SIMULTAAN
    Screen->Print(LCD_S3_STAP_2, LCD_S3_SENSOR_1en2); // Geen wachttijd tussen stap 1 en stap 2.
    exitStatus = Stimulus.MeetStimulusSimultaan(gemetenStimulus, AANTAL_SENSOREN_ALGORITME3, gemetenSynchronisatie, 0b1100, 0b0100);

#ifdef DEBUG
    GA_DEBUG_PRINTLN("---------------------");
    GA_DEBUG_PRINT("Stap 2 exitStatus = ");
    GA_DEBUG_PRINTLN(exitStatus);
#endif  

    // MaskReedsActieveSensorsBijStart = 0b1100: sensor 1 en sensor 2 zijn reeds actief bij aanvang van stap 2.
    // MaskGewensteActieveSensorsBijExit = 0b0100: exit wanneer enkel sensor 2 nog actief is (sensor 1 losgelaten).
    if (exitStatus != EXIT_VOORWAARDE_SENSOR_LOSGELATEN) { return; }
    Stimulus.VergelijkStimulus(nulmetingStimulus[0], gemetenStimulus[0], Stimulus.TijdCorrect, Stimulus.KrachtCorrect);

    bool Sensor1Correct = Stimulus.TijdCorrect && Stimulus.KrachtCorrect;
    Stimulus.VergelijkStimulus(nulmetingStimulus[0], gemetenStimulus[1], Stimulus.TijdCorrect, Stimulus.KrachtCorrect);

    // Voor sensor 2 telt in stap 2 alleen de TikTijd. De gemeten kracht wordt hieronder zijn eigen referentie voor stap 3.
    bool Sensor2Correct = Stimulus.TijdCorrect;
    nulmetingStimulus[1] = gemetenStimulus[1];
    unsigned long margeSimultaanTijd = (nulmetingStimulus[0].TikTijd * Stimulus.TOEGESTANE_MARGE_TIKTIJD) / (Stimulus.MARGE_FACTOR * 100UL);

    bool SimultaanCorrect = true;
    if (gemetenSynchronisatie[0].verschilStartTijd > margeSimultaanTijd) SimultaanCorrect = false;
    if (gemetenSynchronisatie[0].verschilTikTijd > margeSimultaanTijd) SimultaanCorrect = false;

    if (Sensor1Correct && Sensor2Correct && SimultaanCorrect) {
      aantalStappenSynchroon++;
      Stimulus.TELLER_TIKTIJD_SYNCHROON++; // SYNCHROON in Scenario 3 betekent dat de simultane tweede stap correct werd uitgevoerd.
    }

#ifdef DEBUG
    GA_DEBUG_PRINTLN("---------------------");
    GA_DEBUG_PRINT("Stap 2 sensor1/sensor2/simultaan = ");
    GA_DEBUG_PRINT(Sensor1Correct);
    GA_DEBUG_PRINT(" / ");
    GA_DEBUG_PRINT(Sensor2Correct);
    GA_DEBUG_PRINT(" / ");
    GA_DEBUG_PRINTLN(SimultaanCorrect);
#endif

    // STAP 3 TOT n: SENSOR 2, EEN DOORLOPENDE METING
    Screen->Print(LCD_S3_STAP_3_n, LCD_S3_SENSOR_2); // Geen wachttijd tussen stap 2 en stap 3.
    exitStatus = Stimulus.MeetStimulus(sensorRFP602->sensorPin[1], Stimulus.offsetSensor2, gemetenStimulus[1], -1, 0, INSTORTEND_MAXIMALE_FACTOR * MAXIMALE_TIKTIJD_MS + 1000UL);
    if (exitStatus == EXIT_VOORWAARDE_NO_ACTION_TIMEOUT || exitStatus == EXIT_VOORWAARDE_TIMEOUT) { return; }

    char instortendExtraTeken = '?';
    Stimulus.VergelijkStimulus(nulmetingStimulus[1], gemetenStimulus[1], Stimulus.TijdCorrect, Stimulus.KrachtCorrect, INSTORTEND_MOEILIJKHEIDSGRAAD_1, &instortendExtraTeken); // De minimale en maximale totale tiktijd van stap 3 worden rechtstreeks bepaald op basis van de tiktijd van sensor 2 in stap 2.

    // Scoringsvorm bepaald door level (instortendOfGradueel):
    // BINAIR   (level 1-3): tijd EN kracht beiden correct → volle punten, anders 0
    // GRADUEEL (level 4)  : tijd en kracht apart gescoord → elk de helft van de punten
    if (instortendOfGradueel == INSTORTEND_SCORING_GRADUEEL) {
      // Bij een oneven aantal stappen krijgt tijd het extra punt: bij 5 is tijd 3 en kracht 2.
      if (Stimulus.TijdCorrect)   aantalStappenSynchroon += (INSTORTEND_AANTAL_STAPPEN + 1) / 2;
      if (Stimulus.KrachtCorrect) aantalStappenSynchroon += INSTORTEND_AANTAL_STAPPEN / 2;
    } else {
      if (Stimulus.TijdCorrect && Stimulus.KrachtCorrect) aantalStappenSynchroon += INSTORTEND_AANTAL_STAPPEN;
    }

#ifdef DEBUG
    GA_DEBUG_PRINTLN("---------------------");
    GA_DEBUG_PRINT("Stap 3 tot n tijd/kracht = ");
    GA_DEBUG_PRINT(Stimulus.TijdCorrect);
    GA_DEBUG_PRINT(" / ");
    GA_DEBUG_PRINTLN(Stimulus.KrachtCorrect);
#endif

    // Maximum per ronde = stap2(1) + stap3(INSTORTEND_AANTAL_STAPPEN)
    if (aantalStappenSynchroon == (1 + INSTORTEND_AANTAL_STAPPEN)) Stimulus.TELLER_INSTORTEND_CORRECT++;

#ifdef DEBUG
    GA_DEBUG_PRINTLN("---------------------");
    GA_DEBUG_PRINT("Aantal stappen synchroon = ");
    GA_DEBUG_PRINTLN(aantalStappenSynchroon);
#endif

    String herhalingStr = (herhaling < 10) ? ('0' + String(herhaling)) : String(herhaling);
    // 0 < -> instortend niet geslaagd, tijd te kort
    // 0 > -> instortend niet geslaagd, tijd te lang
    // 0 = -> tijd van stap 3 is correct, maar de volledige ronde is niet geslaagd
    // 1 = -> tijd van stap 3 is correct én de volledige ronde is geslaagd
    Screen->Print(herhalingStr + ' ' + LCD_SCORE_SYNCHROON + String(aantalStappenSynchroon), LCD_SCORE_INSTORTEND + String(aantalStappenSynchroon == (1 + INSTORTEND_AANTAL_STAPPEN)) + ' ' + instortendExtraTeken, LCD_LEESTIJD_FEEDBACK_KORT_MS);
  }

  ToonEindScoreScenario3();
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

void ToonEindScoreScenario1() {
  // Berekening van de procentuele scores met integer-veiligheid (schaal 0-100%)
  int percentageTikTijdOk = (Stimulus.TELLER_TIKTIJD_CORRECT * 100) / TEST_AANTAL_KEER_HERHALEN;
  int percentageTikKrachtOk = (Stimulus.TELLER_TIKKRACHT_CORRECT * 100) / TEST_AANTAL_KEER_HERHALEN;

  // --- TOON SCORE TIKTIJD & TIKKRACHT ---
  Screen->Print(LCD_SCORE_TIKTIJD + String(percentageTikTijdOk) + LCD_SCORE_PERCENTAGE, LCD_SCORE_TIKKRACHT + String(percentageTikKrachtOk) + LCD_SCORE_PERCENTAGE, LCD_LEESTIJD_ENDSCORE_MS);
}

void ToonEindScoreScenario2() {
  // 1. Berekening van de procentuele scores (schaal 0-100%)
  int totaalAantalStimuli   = TEST_AANTAL_KEER_HERHALEN * 2;
  int percentageTikTijdOk   = (Stimulus.TELLER_TIKTIJD_CORRECT * 100) / totaalAantalStimuli;
  int percentageTikKrachtOk = (Stimulus.TELLER_TIKKRACHT_CORRECT * 100) / totaalAantalStimuli;

  // 2. Berekening van de specifieke coördinatiescores (Synchroon en In Balans)
  int percentageSynchroon = (Stimulus.TELLER_TIKTIJD_SYNCHROON * 100) / TEST_AANTAL_KEER_HERHALEN;
  int percentageInBalans  = (Stimulus.TELLER_TIKKRACHT_IN_BALANS * 100) / TEST_AANTAL_KEER_HERHALEN;

  // --- SCHERM 1: TOON HOOFDSCORE TIKTIJD & TIKKRACHT ---
  Screen->Print(LCD_SCORE_TIJD + String(percentageTikTijdOk) + LCD_SCORE_PERCENTAGE, LCD_SCORE_KRACHT + String(percentageTikKrachtOk) + LCD_SCORE_PERCENTAGE, LCD_LEESTIJD_ENDSCORE_MS);

  // --- SCHERM 2: TOON COÖRDINATIE FEEDBACK (Synchroon en InBalans) ---
  // Dit scherm laat de gebruiker zien in welke mate beide handen synchroon en in balans samenwerkten
  Screen->Print(LCD_SCORE_SYNCHROON + String(percentageSynchroon) + LCD_SCORE_PERCENTAGE, LCD_SCORE_IN_BALANS + String(percentageInBalans) + LCD_SCORE_PERCENTAGE, LCD_LEESTIJD_ENDSCORE_MS);

  // 3. Reset het geheugen (Zorg dat Stimulus.ResetAlleTellers ook de 2 nieuwe variabelen reset!)
  Stimulus.ResetAlleTellers();
}

void ToonEindScoreScenario3() {
  // Stap 1 = 1 punt, stap 2 = 1 punt, stap 3 = INSTORTEND_AANTAL_STAPPEN punten
  int totaalAantalKeren     = TEST_AANTAL_KEER_HERHALEN * 3; // Stimulus.VergelijkStimulus() wordt per ronde 3 keer aangeroepen.
  int percentageTikTijdOk   = (Stimulus.TELLER_TIKTIJD_CORRECT * 100) / totaalAantalKeren;
  int percentageTikKrachtOk = (Stimulus.TELLER_TIKKRACHT_CORRECT * 100) / totaalAantalKeren;
  int percentageSynchroon   = (Stimulus.TELLER_TIKTIJD_SYNCHROON * 100) / TEST_AANTAL_KEER_HERHALEN;
  int percentageInstortend  = (Stimulus.TELLER_INSTORTEND_CORRECT * 100) / TEST_AANTAL_KEER_HERHALEN;

  Screen->Print(LCD_SCORE_TIJD + String(percentageTikTijdOk) + LCD_SCORE_PERCENTAGE, LCD_SCORE_KRACHT + String(percentageTikKrachtOk) + LCD_SCORE_PERCENTAGE, LCD_LEESTIJD_ENDSCORE_MS);
  Screen->Print(LCD_SCORE_SYNCHROON + String(percentageSynchroon) + LCD_SCORE_PERCENTAGE, LCD_SCORE_INSTORTEND + String(percentageInstortend) + LCD_SCORE_PERCENTAGE, LCD_LEESTIJD_ENDSCORE_MS);
  if (Stimulus.TELLER_INSTORTEND_CORRECT == TEST_AANTAL_KEER_HERHALEN) Screen->Print(LCD_FINALE_TITEL, LCD_FINALE_SUCCES, LCD_LEESTIJD_ENDSCORE_MS); // Alle rondes perfect gescoord
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
