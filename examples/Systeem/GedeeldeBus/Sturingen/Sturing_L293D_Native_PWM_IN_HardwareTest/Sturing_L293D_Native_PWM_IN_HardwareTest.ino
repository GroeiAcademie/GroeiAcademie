// ============================================================================
// Sturing: Sturing_L293D_Native_PWM_IN_HardwareTest
// ============================================================================
// D5 (PWM) -> L293D IN1, D6 (PWM) -> IN2. ENABLE moet intern actief zijn.
// L293D motoruitgangen en externe motorvoeding correct aansluiten.
// D5 en D6 moeten op het gekozen board PWM ondersteunen.
// Bij STOP worden IN1/IN2 LOW; de motorremwerking is anders dan PWM via EN.
// Start beweging uitsluitend met 1 + Enter.
// ============================================================================

#include <GroeiAcademie.h>
#if STURING_L293D_AANTAL == 0
  #error Zet STURING_L293D_AANTAL op minstens 1 voor deze test.
#endif
L293D* l293d = nullptr;

DCmotorBesturing* motorBesturing = nullptr;

bool testActief = false;
bool startGelukt = false;
bool testGestopt = false;
String invoer;
uint8_t testStap = 0;
uint8_t testFase = 0;
unsigned long testStartMillis = 0;

void toonStartInstructies() {
  GA_SERIAL.println(F("HARDWARETEST: Sturing_L293D_Native_PWM_IN_HardwareTest"));
  GA_SERIAL.println(F("BEGELEIDE DC-MOTORTEST. Zet de motor stevig vast en houd bewegende onderdelen vrij."));
  GA_SERIAL.println(F("PWM 800/4095 is circa 20 procent aansturing, geen gemeten toerental."));
  GA_SERIAL.println(F("Elke beweging start pas na '1' + Enter. STOP + Enter schakelt de IN1/IN2 LOW."));
  GA_SERIAL.println(F("Controleer de beweging fysiek; een geslaagde functieaanroep bewijst geen motorbeweging."));
}

void toonTestInstructies() {
  GA_SERIAL.println();
  switch (testStap) {
    case 0: GA_SERIAL.println(F("TEST 1: StelDraairichtingEnSnelheidIn(vooruit,800), StartMotor. Verwacht: circa 2 seconden in beginrichting.")); break;
    case 1: GA_SERIAL.println(F("TEST 2: StelSnelheidIn(800), BeweegVooruit. Verwacht: circa 2 seconden vooruit.")); break;
    case 2: GA_SERIAL.println(F("TEST 3: StelSnelheidIn(800), BeweegAchteruit. Verwacht: circa 2 seconden achteruit.")); break;
    case 3: GA_SERIAL.println(F("TEST 4: PauzeerMotor en HervatMotor. Eerst 0,8 seconde bewegen, dan wachten op 1 voor hervatten.")); break;
    case 4: GA_SERIAL.println(F("TEST 5: DraairichtingOmkeren terwijl motor gestopt is, daarna StartMotor. Verwacht: omgekeerde draairichting.")); break;
    case 5: GA_SERIAL.println(F("TEST 6: StelSnelheidIn tijdens beweging. Verwacht: na 0,8 seconde PWM van 500 naar 1300.")); break;
    default: GA_SERIAL.println(F("ALLE DC-MOTORFUNCTIES AANGEROEPEN. Controleer zelf het fysieke resultaat.")); return;
  }
  GA_SERIAL.println(F("Controleer de motorruimte. Typ 1 + Enter om deze test te starten."));
}

void toonFunctieResultaat(bool ok) { GA_SERIAL.println(ok ? F("FUNCTIEAANROEP: OK (geen toerental- of positiefeedback)") : F("FUNCTIEAANROEP: FOUT")); }

void rondTestAf(bool ok) {
  motorBesturing->StopMotor();
  GA_SERIAL.println(F("MOTOR: STOP aangeroepen; IN1/IN2 LOW gevraagd."));
  toonFunctieResultaat(ok);
  testActief = false;

  if (!ok) {
    testGestopt = true;
    GA_SERIAL.println(F("TEST GESTOPT NA FOUT. Controleer hardware en herstart Arduino."));
    return;
  }

  ++testStap;
  toonTestInstructies();
}

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sturing_L293D_Native_PWM_IN_HardwareTest"));
  l293d = GedeeldeBusNewComponent<L293D>(&Native, 5, 6);
  if (l293d != nullptr) motorBesturing = new DCmotorBesturing(*l293d);
  startGelukt = l293d != nullptr && motorBesturing != nullptr;
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
      GA_SERIAL.println(F("STOP: IN1/IN2 LOW. Herstart Arduino voor een nieuwe test."));
      return;
    }
    if (invoer == "1" && startGelukt && !testGestopt) {
      if (testActief && testStap == 3 && testFase == 1) {
        const bool ok = motorBesturing->HervatMotor();
        GA_SERIAL.println(F("HervatMotor(): oude richting en PWM opnieuw gevraagd."));
        toonFunctieResultaat(ok);

        if (!ok) {
          rondTestAf(false);
          return;
        }

        testFase = 2;
        testStartMillis = millis();
        return;
      }

      if (testActief) {
        GA_SERIAL.println(F("TEST BEZIG. Wacht op automatische stop of typ STOP."));
        return;
      }

      if (testStap >= 6) {
        toonTestInstructies();
        return;
      }

      GA_SERIAL.print(F("START TEST ")); GA_SERIAL.println(testStap + 1);
      bool ok = false;
      switch (testStap) {
        case 0: ok = motorBesturing->StelDraairichtingEnSnelheidIn(true, 800) && motorBesturing->StartMotor(); break;
        case 1: ok = motorBesturing->StelSnelheidIn(800) && motorBesturing->BeweegVooruit(); break;
        case 2: ok = motorBesturing->StelSnelheidIn(800) && motorBesturing->BeweegAchteruit(); break;
        case 3: ok = motorBesturing->StelDraairichtingEnSnelheidIn(true, 800) && motorBesturing->StartMotor(); break;
        case 4: ok = motorBesturing->StelDraairichtingEnSnelheidIn(true, 800) && motorBesturing->DraairichtingOmkeren() && motorBesturing->StartMotor(); break;
        case 5: ok = motorBesturing->StelDraairichtingEnSnelheidIn(true, 500) && motorBesturing->StartMotor(); break;
      }

      if (!ok) {
        rondTestAf(false);
        return;
      }

      testActief = true;
      testFase = 0;
      testStartMillis = millis();
      GA_SERIAL.println(F("FUNCTIEAANROEP: OK. Beweging heeft een automatische stoptijd."));
      return;
    }
    GA_SERIAL.println(F("Geen geldige opdracht. Typ 1 + Enter, of STOP + Enter."));
  }
  if (!testActief) return;
  const unsigned long verstreken = millis() - testStartMillis;
  if (testStap == 3 && testFase == 1) return;
  if (testStap == 3 && testFase == 0 && verstreken >= 800UL) {
    const bool ok = motorBesturing->PauzeerMotor();
    GA_SERIAL.println(F("PauzeerMotor(): IN1/IN2 LOW, richting/snelheid intern bewaard."));
    toonFunctieResultaat(ok);

    if (!ok) {
      rondTestAf(false);
      return;
    }

    testFase = 1;
    GA_SERIAL.println(F("Motor is gepauzeerd. Typ 1 + Enter om de bewaarde beweging te hervatten."));
    return;
  }
  if (testStap == 5 && testFase == 0 && verstreken >= 800UL) {
    const bool ok = motorBesturing->StelSnelheidIn(1300);
    GA_SERIAL.println(F("StelSnelheidIn(1300): gevraagde PWM verhoogd, fysieke snelheid onbekend."));
    toonFunctieResultaat(ok);

    if (!ok) {
      rondTestAf(false);
      return;
    }

    testFase = 2;
  }
  if (verstreken >= 2000UL) rondTestAf(true);
}
