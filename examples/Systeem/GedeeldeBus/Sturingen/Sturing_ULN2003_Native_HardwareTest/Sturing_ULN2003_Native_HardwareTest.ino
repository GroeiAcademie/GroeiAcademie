// ============================================================================
// Sturing: Sturing_ULN2003_Native_HardwareTest
// ============================================================================
// Native: D2 -> ULN2003 IN1, D3 -> IN2, D4 -> IN3, D5 -> IN4.
// ULN2003A is een open-collector Darlington-array; geen software-PWM vereist.
// COM naar motorvoeding+ bij inductieve belasting, GND gedeeld.
// 28BYJ-48 via het driverbord; EXTERNE geschikte motorvoeding gebruiken.
// Fysieke beweging uitsluitend na 1 + Enter; STOP schakelt alle vier uitgangen LOW.
// Controleer 3,3V-logische stuurstroom bij de concrete motorbelasting.
// ============================================================================

#include <GroeiAcademie.h>
#if STURING_ULN2003_AANTAL == 0
  #error Zet STURING_ULN2003_AANTAL op minstens 1 voor deze test.
#endif
ULN2003* uln2003 = nullptr;

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

void toonStartInstructies() {
  GA_SERIAL.println(F("HARDWARETEST: Sturing_ULN2003_Native_HardwareTest"));
  GA_SERIAL.println(F("BEGELEIDE STAPPENMOTORTEST. Sluit de motor veilig aan en maak het mechanisme vrij."));
  GA_SERIAL.println(F("Bij ULN2003 stopt STOP de stapcommando's; schakel de motorvoeding uit voordat je bedrading wijzigt."));
  GA_SERIAL.println(F("Elke stap start pas na '1' + Enter. STOP + Enter onderbreekt de test."));
  GA_SERIAL.println(F("Controleer de beweging fysiek; een geslaagde functieaanroep bewijst geen motorbeweging."));
}

void toonTestInstructies() {
  GA_SERIAL.println();
  switch (testStap) {
    case 0: GA_SERIAL.println(F("TEST 1: StartMotor + VoerEenStapUit. Verwacht: een stap; markeer de motoras om kleine beweging te herkennen.")); break;
    case 1: GA_SERIAL.println(F("TEST 2: StartMotor + VoerMeerdereStappenUit(4). Verwacht: vier stappen; markeer de motoras om kleine beweging te herkennen.")); break;
    case 2: GA_SERIAL.println(F("TEST 3: DraairichtingOmkeren + 80 losse motorstappen. Verwacht: beweging in de andere draairichting, met tussenpozen.")); break;
    case 3: GA_SERIAL.println(F("TEST 4: StartMotor, stappen, PauzeerMotor, HervatMotor, resterende stappen. Verwacht: beweging - stilstand - beweging.")); break;
    default: GA_SERIAL.println(F("ALLE STAPPENMOTORFUNCTIES AANGEROEPEN. Controleer zelf het fysieke resultaat.")); return;
  }
  GA_SERIAL.println(F("Controleer de ruimte rond de motor. Typ 1 + Enter om deze test te starten."));
}

void toonFunctieResultaat(bool ok) { GA_SERIAL.println(ok ? F("FUNCTIEAANROEP: OK (geen bevestiging van fysieke beweging)") : F("FUNCTIEAANROEP: FOUT")); }

void rondTestAf(bool ok) {
  motorBesturing->StopMotor();
  GA_SERIAL.println(F("MOTOR: STOP aangeroepen. De motor krijgt geen nieuwe stappen."));
  toonFunctieResultaat(ok);
  testActief = false;

  if (!ok) {
    testGestopt = true;
    GA_SERIAL.println(F("TEST GESTOPT NA FOUT. Controleer hardware, herstart Arduino."));
    return;
  }

  ++testStap;
  toonTestInstructies();
}

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  #if defined(DEBUG)
  while (!GA_SERIAL) { ; }
  #else
  unsigned long startTijd = millis();
  while (!GA_SERIAL && (millis() - startTijd) < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  #endif
  GA_SERIAL.println(F("TEST: Sturing_ULN2003_Native_HardwareTest"));
  uln2003 = GedeeldeBusNewComponent<ULN2003>(&Native, static_cast<uint8_t>(HardwareResourcePin::NP_D2), static_cast<uint8_t>(HardwareResourcePin::NP_D3), static_cast<uint8_t>(HardwareResourcePin::NP_D4), static_cast<uint8_t>(HardwareResourcePin::NP_D5));
  if (uln2003 != nullptr) motorBesturing = new StappenmotorBesturing(*uln2003);
  startGelukt = uln2003 != nullptr && motorBesturing != nullptr;
  if (startGelukt) motorBesturing->StopMotor();
  GA_SERIAL.println(startGelukt ? F("LEVENSCYCLUS: OK. BEGINSTATUS: MOTOR GESTOPT.") : F("LEVENSCYCLUS: FOUT. GEEN TEST UITVOEREN."));
  toonStartInstructies();
  if (startGelukt) toonTestInstructies();
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
        toonFunctieResultaat(ok);

        if (!ok) {
          rondTestAf(false);
          return;
        }

        testFase = 2;
        laatsteStapMillis = millis();
        return;
      }

      if (testActief) {
        GA_SERIAL.println(F("TEST BEZIG. Gebruik STOP voor een onderbreking."));
        return;
      }

      if (testStap >= 4) {
        toonTestInstructies();
        return;
      }

      GA_SERIAL.print(F("START TEST ")); GA_SERIAL.println(testStap + 1);
      testStartMillis = millis();
      laatsteStapMillis = millis();
      stappenGedaan = 0;
      testFase = 0;
      bool ok = motorBesturing->StartMotor();
      if (ok && testStap == 0) ok = motorBesturing->VoerEenStapUit();
      if (ok && testStap == 1) ok = motorBesturing->VoerMeerdereStappenUit(4);
      if (ok && testStap == 2) ok = motorBesturing->DraairichtingOmkeren();

      if (!ok) {
        rondTestAf(false);
        return;
      }

      testActief = true;
      GA_SERIAL.println(F("FUNCTIEAANROEP: OK. Test loopt; de motor wordt nadien gestopt."));
      return;
    }
    GA_SERIAL.println(F("Geen geldige opdracht. Typ 1 + Enter voor de aangekondigde test, of STOP + Enter."));
  }
  if (!testActief) return;
  const unsigned long nu = millis();

  if (testStap < 2) {
    if (nu - testStartMillis >= 2000UL) rondTestAf(true);
    return;
  }

  if (testStap == 2) {
    if (nu - laatsteStapMillis < 50UL) return;
    laatsteStapMillis = nu;

    if (!motorBesturing->VoerEenStapUit()) {
      rondTestAf(false);
      return;
    }

    if (++stappenGedaan >= 80) rondTestAf(true);
    return;
  }
  if (testStap == 3 && testFase == 1) return;
  if (nu - laatsteStapMillis < 50UL) return;
  laatsteStapMillis = nu;

  if (!motorBesturing->VoerEenStapUit()) {
    rondTestAf(false);
    return;
  }

  ++stappenGedaan;
  if (testFase == 0 && stappenGedaan == 30) {
    const bool ok = motorBesturing->PauzeerMotor();
    GA_SERIAL.println(F("PauzeerMotor(): stappen onderbroken; reeds uitgevoerde stappen zijn niet fysiek gemeten."));
    toonFunctieResultaat(ok);

    if (!ok) {
      rondTestAf(false);
      return;
    }

    testFase = 1;
    GA_SERIAL.println(F("Motor staat stil. Typ 1 + Enter om de resterende 50 stappen te hervatten."));
  }
  if (testFase == 2 && stappenGedaan >= 80) rondTestAf(true);
}
