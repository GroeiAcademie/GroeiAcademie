// ============================================================================
// Sturing: A4988_HardwareTest
// ============================================================================
// Begeleide hardwaretest voor A4988. Elke beweging vereist eerst bevestiging via Serial.
// Pinnen zijn afgeleid uit de bestaande constructoren en de boardmapping in src/Configuratie/UserConfig.h en SystemConfig.h.
// A4988 STEP -> D6, DIR -> D7; ENABLE, RESET, SLEEP en motorvoeding volgens de concrete driver.
// Native pinlabels in de code: D6 en D7; boardmapping:
// UNO R4 Minima/WiFi en UNO Q: D6 en D7.
// ESP32 D1 UNO R32: D6=GPIO27, D7=GPIO14.
// ESP32-S3 ARDI-32: D6=GPIO9, D7=GPIO14; ESP32-S3 DEV: D6=GPIO3, D7=GPIO14.
// RP2040 Cytron Maker UNO: D6=GPIO6, D7=GPIO7.
// STM32 F401RE Nucleo: D6=PB10, D7=PA8.
// ============================================================================

#include <GroeiAcademie.h>
#if STURING_A4988_AANTAL == 0
  #error Zet STURING_A4988_AANTAL op minstens 1 voor deze test.
#endif


A4988* sturing = nullptr;

StappenmotorBesturing* motorBesturing = nullptr;

bool testActief = false;
bool startGelukt = false;
bool testGestopt = false;
String invoer;
uint8_t testStap = 0;
uint8_t testFase = 0;
uint16_t stappenGedaan = 0;
unsigned long testStartMillis = 0;
unsigned long laatsteStapMillis = 0;

void toonStart() {
  GA_SERIAL.println(F("RUN42 HARDWARETEST"));
  GA_SERIAL.println(F("BEGELEIDE STAPPENMOTORTEST. Sluit de motor veilig aan en maak het mechanisme vrij."));
  GA_SERIAL.println(F("Bij A4988 blijft de motor mogelijk bekrachtigd na STOP; schakel zo nodig de voeding fysiek uit."));
  GA_SERIAL.println(F("Elke stap start pas na '1' + Enter. STOP + Enter onderbreekt de test."));
}

void toonTest() {
  GA_SERIAL.println();
  switch (testStap) {
    case 0: GA_SERIAL.println(F("TEST 1: StartMotor + VoerEenStapUit. Verwacht: een motorstap in de beginrichting.")); break;
    case 1: GA_SERIAL.println(F("TEST 2: StartMotor + VoerMeerdereStappenUit(4). Verwacht: vier korte motorstappen; mogelijk nauwelijks zichtbaar.")); break;
    case 2: GA_SERIAL.println(F("TEST 3: DraairichtingOmkeren + 80 losse motorstappen. Verwacht: beweging in de andere draairichting, met tussenpozen.")); break;
    case 3: GA_SERIAL.println(F("TEST 4: StartMotor, stappen, PauzeerMotor, HervatMotor, resterende stappen. Verwacht: beweging - stilstand - beweging.")); break;
    default: GA_SERIAL.println(F("ALLE STAPPENMOTORFUNCTIES VAN RUN133 AANGEROEPEN. Controleer zelf het fysieke resultaat.")); return;
  }
  GA_SERIAL.println(F("Controleer de ruimte rond de motor. Typ 1 + Enter om deze test te starten."));
}

void antwoord(bool ok) { GA_SERIAL.println(ok ? F("FUNCTIEAANROEP: OK (geen bevestiging van fysieke beweging)") : F("FUNCTIEAANROEP: FOUT")); }

void rondTestAf(bool ok) {
  motorBesturing->StopMotor();
  GA_SERIAL.println(F("MOTOR: STOP aangeroepen. De motor krijgt geen nieuwe stappen."));
  antwoord(ok);
  testActief = false;
  if (!ok) { testGestopt = true; GA_SERIAL.println(F("TEST GESTOPT NA FOUT. Controleer hardware, herstart Arduino.")); return; }
  ++testStap;
  toonTest();
}

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sturing_A4988_HardwareTest"));
  sturing = GedeeldeBusNewComponent<A4988>(&Native, HardwareResourcePin::NP_D6, HardwareResourcePin::NP_D7);
  if (sturing != nullptr) motorBesturing = new StappenmotorBesturing(*sturing);
  startGelukt = sturing != nullptr && motorBesturing != nullptr;
  if (startGelukt) motorBesturing->StopMotor();
  GA_SERIAL.println(startGelukt ? F("LEVENSCYCLUS: OK. BEGINSTATUS: MOTOR GESTOPT.") : F("LEVENSCYCLUS: FOUT. GEEN TEST UITVOEREN."));
  toonStart();
  if (startGelukt) toonTest();
}

void loop() {
  if (GA_SERIAL.available()) {
    invoer = GA_SERIAL.readStringUntil('\n');
    invoer.trim();
    invoer.toUpperCase();
    if (invoer == "STOP") {
      if (startGelukt) motorBesturing->StopMotor();
      testActief = false;
      testGestopt = true;
      GA_SERIAL.println(F("NOODSTOP: geen nieuwe stappen. Controleer of de aandrijving veilig is; herstart Arduino om opnieuw te testen."));
      return;
    }
    if (invoer == "1" && startGelukt && !testGestopt) {
      if (testActief && testStap == 3 && testFase == 1) {
        const bool ok = motorBesturing->HervatMotor();
        GA_SERIAL.println(F("HervatMotor(): uitvoering gevraagd, resterende motorstappen volgen."));
        antwoord(ok);
        if (!ok) { rondTestAf(false); return; }
        testFase = 2;
        laatsteStapMillis = millis();
        return;
      }
      if (testActief) { GA_SERIAL.println(F("TEST BEZIG. Gebruik STOP voor een onderbreking.")); return; }
      if (testStap >= 4) { toonTest(); return; }
      GA_SERIAL.print(F("START TEST ")); GA_SERIAL.println(testStap + 1);
      testStartMillis = millis();
      laatsteStapMillis = millis();
      stappenGedaan = 0;
      testFase = 0;
      bool ok = motorBesturing->StartMotor();
      if (ok && testStap == 0) ok = motorBesturing->VoerEenStapUit();
      if (ok && testStap == 1) ok = motorBesturing->VoerMeerdereStappenUit(4);
      if (ok && testStap == 2) ok = motorBesturing->DraairichtingOmkeren();
      if (!ok) { rondTestAf(false); return; }
      testActief = true;
      GA_SERIAL.println(F("FUNCTIEAANROEP: OK. Test loopt; de motor wordt nadien gestopt."));
      return;
    }
    GA_SERIAL.println(F("Geen geldige opdracht. Typ 1 + Enter voor de aangekondigde test, of STOP + Enter."));
  }
  if (!testActief) return;
  const unsigned long nu = millis();
  if (testStap < 2) { if (nu - testStartMillis >= 2000UL) rondTestAf(true); return; }
  if (testStap == 2) {
    if (nu - laatsteStapMillis < 50UL) return;
    laatsteStapMillis = nu;
    if (!motorBesturing->VoerEenStapUit()) { rondTestAf(false); return; }
    if (++stappenGedaan >= 80) rondTestAf(true);
    return;
  }
  if (testStap == 3 && testFase == 1) return;
  if (nu - laatsteStapMillis < 50UL) return;
  laatsteStapMillis = nu;
  if (!motorBesturing->VoerEenStapUit()) { rondTestAf(false); return; }
  ++stappenGedaan;
  if (testFase == 0 && stappenGedaan == 30) {
    const bool ok = motorBesturing->PauzeerMotor();
    GA_SERIAL.println(F("PauzeerMotor(): stappen onderbroken; reeds uitgevoerde stappen zijn niet fysiek gemeten."));
    antwoord(ok);
    if (!ok) { rondTestAf(false); return; }
    testFase = 1;
    GA_SERIAL.println(F("Motor staat stil. Typ 1 + Enter om de resterende 50 stappen te hervatten."));
  }
  if (testFase == 2 && stappenGedaan >= 80) rondTestAf(true);
}
