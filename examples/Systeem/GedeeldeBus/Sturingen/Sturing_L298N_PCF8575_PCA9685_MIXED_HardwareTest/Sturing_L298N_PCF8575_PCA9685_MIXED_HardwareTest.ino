// ============================================================================
// Sturing: Sturing_L298N_PCF8575_PCA9685_MIXED_HardwareTest
// ============================================================================
// Motor 1: PCF8575 P0/P1 -> L298N IN1/IN2, PCA9685 PWM5 -> ENA.
// Motor 2: PCF8575 P2/P3 -> L298N IN3/IN4, PCA9685 PWM8 -> ENB.
// EN(A/B)-jumper(s) verwijderen waar van toepassing; niet beide motoren tegelijk testen.
// GEDEELDE_BUS_MIXED_PINS 1 in UserConfig.h, beide Extenders ingeschakeld.
// Logica- en motorvoeding passend aansluiten; GND gemeenschappelijk.
// Gebruik 1 of 2 voor motorkeuze, daarna de begeleide serietests.
// ============================================================================

#include <GroeiAcademie.h>
#if STURING_L298N_AANTAL == 0
  #error Zet STURING_L298N_AANTAL op minstens 1 voor deze test.
#endif
#if EXTENDER_PCA9685_AANTAL == 0 || EXTENDER_PCF8575_AANTAL == 0 || !GEDEELDE_BUS_MIXED_PINS
  #error Activeer PCA9685, PCF8575 en GEDEELDE_BUS_MIXED_PINS voor deze test.
#endif

ExtenderPCA9685* extenderPCA9685 = nullptr;
ExtenderPCF8575* extenderPCF8575 = nullptr;
L298N* l298n = nullptr;

DCmotorBesturing* motorBesturing = nullptr;

bool testActief = false;
bool startGelukt = false;
bool testGestopt = false;
String invoer;
uint8_t testStap = 0;
uint8_t testFase = 0;
unsigned long testStartMillis = 0;

void toonStartInstructies() {
  GA_SERIAL.println(F("HARDWARETEST: Sturing_L298N_PCF8575_PCA9685_MIXED_HardwareTest"));
  GA_SERIAL.println(F("BEGELEIDE DC-MOTORTEST. Zet de motor stevig vast en houd bewegende onderdelen vrij."));
  GA_SERIAL.println(F("PWM 800/4095 is circa 20 procent aansturing, geen gemeten toerental."));
  GA_SERIAL.println(F("KIES MOTOR: 1 = Motor 1 (IN1/IN2/ENA, OUT1/OUT2), 2 = Motor 2 (IN3/IN4/ENB, OUT3/OUT4)."));
  GA_SERIAL.println(F("Kies een motor met 1 of 2 + Enter. Daarna kies je een test en start je die met 1 + Enter."));
  GA_SERIAL.println(F("STOP + Enter schakelt de driver uit."));
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
  GA_SERIAL.println(F("MOTOR: STOP aangeroepen; driver-ENABLE = 0 gevraagd."));
  toonFunctieResultaat(ok);
  testActief = false;

  if (!ok) {
    testGestopt = true;
    GA_SERIAL.println(F("TEST GESTOPT NA FOUT. Controleer de hardware voordat je verdergaat."));
  } else {
    GA_SERIAL.println(F("TEST UITGEVOERD. Controleer of de motor heeft gedaan wat hierboven beschreven staat."));
  }

  testFase = 13;
  GA_SERIAL.println(F("0 = Terug naar de motorkeuze."));
}

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sturing_L298N_PCF8575_PCA9685_MIXED_HardwareTest"));
  extenderPCA9685 = GedeeldeBusNewComponent<ExtenderPCA9685>(&Native, GedeeldeBusComponent::PWM_PCA9685, I2C_ADDRESS_EXTENDER_PCA9685_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL);
  if (extenderPCA9685 != nullptr) extenderPCF8575 = GedeeldeBusNewComponent<ExtenderPCF8575>(&Native, GedeeldeBusComponent::DIGITAL_PINS_PCF8575, I2C_ADDRESS_EXTENDER_PCF8575_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, EXTENDER_PCF8575_1_INT);
  if (extenderPCF8575 != nullptr) l298n = GedeeldeBusNewComponent<L298N>(extenderPCF8575, extenderPCA9685, 0, 1, 5);
  if (l298n != nullptr) motorBesturing = new DCmotorBesturing(*l298n);
  startGelukt = l298n != nullptr && motorBesturing != nullptr;
  if (startGelukt) motorBesturing->StopMotor();
  GA_SERIAL.println(startGelukt ? F("LEVENSCYCLUS: OK. BEGINSTATUS: MOTOR GESTOPT.") : F("LEVENSCYCLUS: FOUT. GEEN TEST UITVOEREN."));
  testFase = 10;
  if (startGelukt) toonStartInstructies();
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
      testFase = 13;
      GA_SERIAL.println(F("STOP: driver uit. De gekozen test is gestopt."));
      GA_SERIAL.println(F("0 = Terug naar de motorkeuze."));
      return;
    }

    if (!startGelukt) {
      GA_SERIAL.println(F("LEVENSCYCLUS: FOUT. GEEN TEST UITVOEREN."));
      return;
    }

    if (testFase == 13) {
      if (invoer == "0") {
        testGestopt = false;
        testFase = 10;
        toonStartInstructies();
      } else {
        GA_SERIAL.println(F("0 = Terug naar de motorkeuze."));
      }

      return;
    }

    if (testFase == 10) {
      if (invoer != "1" && invoer != "2") {
        GA_SERIAL.println(F("Kies motor 1 of motor 2 + Enter."));
        return;
      }

      if ((invoer == "1" && l298n->in1 != 0) || (invoer == "2" && l298n->in1 != 2)) {
        motorBesturing->StopMotor();
        delete motorBesturing;
        motorBesturing = nullptr;

        if (!l298n->afmelden()) {
          startGelukt = false;
          GA_SERIAL.println(F("LEVENSCYCLUS: FOUT BIJ AFMELDEN. Herstart Arduino."));
          return;
        }

        l298n = nullptr;
        l298n = GedeeldeBusNewComponent<L298N>(extenderPCF8575, extenderPCA9685, invoer == "1" ? 0 : 2, invoer == "1" ? 1 : 3, invoer == "1" ? 5 : 8);
        if (l298n != nullptr) motorBesturing = new DCmotorBesturing(*l298n);
        startGelukt = l298n != nullptr && motorBesturing != nullptr;

        if (!startGelukt) {
          GA_SERIAL.println(F("LEVENSCYCLUS: FOUT BIJ MOTORKEUZE. Herstart Arduino."));
          return;
        }

        motorBesturing->StopMotor();
      }

      GA_SERIAL.print(F("GEKOZEN MOTOR: "));
      GA_SERIAL.println(invoer);
      GA_SERIAL.println(F("KIES EEN TEST:"));
      GA_SERIAL.println(F("1 = StelDraairichtingEnSnelheidIn / StartMotor"));
      GA_SERIAL.println(F("2 = StelSnelheidIn / BeweegVooruit"));
      GA_SERIAL.println(F("3 = StelSnelheidIn / BeweegAchteruit"));
      GA_SERIAL.println(F("4 = PauzeerMotor / HervatMotor"));
      GA_SERIAL.println(F("5 = DraairichtingOmkeren / StartMotor"));
      GA_SERIAL.println(F("6 = StelSnelheidIn tijdens beweging"));
      GA_SERIAL.println(F("Typ 1 t/m 6 + Enter om de testuitleg te zien."));
      testFase = 11;
      return;
    }

    if (testFase == 11) {
      if (invoer == "0") {
        testFase = 10;
        toonStartInstructies();
        return;
      }

      const int gekozenTest = invoer.toInt();
      if (gekozenTest < 1 || gekozenTest > 6 || String(gekozenTest) != invoer) {
        GA_SERIAL.println(F("Ongeldige test. Typ 1 t/m 6 + Enter."));
        return;
      }

      testStap = static_cast<uint8_t>(gekozenTest - 1);
      testFase = 12;
      toonTestInstructies();
      return;
    }

    if (testActief && testStap == 3 && testFase == 1 && invoer == "1") {
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

    if (testFase == 12 && invoer == "1") {
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

    GA_SERIAL.println(F("Typ 1 + Enter om de gekozen test te starten, of STOP + Enter."));
  }

  if (!testActief) return;
  const unsigned long verstreken = millis() - testStartMillis;
  if (testStap == 3 && testFase == 1) return;
  if (testStap == 3 && testFase == 0 && verstreken >= 800UL) {
    const bool ok = motorBesturing->PauzeerMotor();
    GA_SERIAL.println(F("PauzeerMotor(): driver uit, richting/snelheid intern bewaard."));
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
