// ============================================================================
// Sturing: MG996R_HardwareTest
// ============================================================================
// Begeleide hardwaretest voor MG996R. Elke beweging vereist eerst bevestiging via Serial.
// Pinnen zijn afgeleid uit de bestaande constructoren en de boardmapping in src/Configuratie/UserConfig.h en SystemConfig.h.
// PCA9685 kanaal 1 -> MG996R servosignaal; servo-voeding geschikt voor het model, GND gemeenschappelijk.
// I2C-aansluiting: SDA/SCL en GND op dezelfde bus; modulevoeding volgens het werkelijke Extender-bord.
// UNO R4 Minima/WiFi en UNO Q: SDA/SCL op de gelijknamige boardaansluitingen.
// ESP32 D1 UNO R32: SDA=GPIO21, SCL=GPIO22.
// ESP32-S3 ARDI-32: SDA=GPIO38, SCL=GPIO39; ESP32-S3 DEV: SDA=GPIO8, SCL=GPIO9.
// RP2040 Cytron Maker UNO: SDA=GPIO20, SCL=GPIO21.
// STM32 F401RE Nucleo: SDA=PB9, SCL=PB8.
// ============================================================================

#include <GroeiAcademie.h>
#if STURING_MG996R_AANTAL == 0
  #error Zet STURING_MG996R_AANTAL op minstens 1 voor deze test.
#endif
#if EXTENDER_PCA9685_AANTAL == 0
  #error Schakel de vereiste Extender in voor deze Sturing-test.
#endif

ExtenderPCA9685* extenderPCA9685 = nullptr;
MG996R* mg996r = nullptr;

ServomotorBesturing* motorBesturing = nullptr;

bool testActief = false;
bool startGelukt = false;
bool testGestopt = false;
String invoer;
uint8_t testStap = 0;
uint8_t testFase = 0;
unsigned long testStartMillis = 0;

void toonStartInstructies() {
  GA_SERIAL.println(F("HARDWARETEST: Sturing_MG996R_HardwareTest"));
  GA_SERIAL.println(F("BEGELEIDE SERVOTEST. Controleer voeding, kalibratie en mechanische hoekgrenzen."));
  GA_SERIAL.println(F("STOP schakelt het PWM-signaal uit; de servo houdt zijn positie dan niet gegarandeerd vast."));
  GA_SERIAL.println(F("Elke beweging start pas na '1' + Enter. STOP + Enter stopt de test."));
  GA_SERIAL.println(F("Controleer de beweging fysiek; een geslaagde functieaanroep bewijst geen motorbeweging."));
}

void toonTestInstructies() {
  GA_SERIAL.println();
  switch (testStap) {
    case 0: GA_SERIAL.println(F("TEST 1: PositioneerAbsoluutNaarHoek(45). Verwacht: beweging naar opgegeven 45 graden.")); break;
    case 1: GA_SERIAL.println(F("TEST 2: DraaiRelatiefOverHoek(60). Verwacht: doelhoek 105 graden op basis van de laatst opgedragen 45 graden.")); break;
    case 2: GA_SERIAL.println(F("TEST 3: PositioneerNaarMiddenpositie. Verwacht: naar 90 graden.")); break;
    case 3: GA_SERIAL.println(F("TEST 4: StartMotor. Verwacht: hernieuwde PWM-opdracht voor laatst opgedragen 90 graden.")); break;
    case 4: GA_SERIAL.println(F("TEST 5: Positie 135 graden, PauzeerMotor, vervolgens HervatMotor. Pauzeren = PWM uit, geen gemeten stilstand.")); break;
    case 5: GA_SERIAL.println(F("TEST 6: SchakelMotorIn en SchakelMotorUit. Verwacht: PWM inschakelen en weer uitschakelen.")); break;
    default: GA_SERIAL.println(F("ALLE SERVOMOTORFUNCTIES AANGEROEPEN. Controleer zelf het fysieke resultaat.")); return;
  }
  GA_SERIAL.println(F("Controleer vrije bewegingsruimte. Typ 1 + Enter om deze test te starten."));
}

void toonFunctieResultaat(bool ok) { GA_SERIAL.println(ok ? F("FUNCTIEAANROEP: OK (geen fysieke positiefeedback)") : F("FUNCTIEAANROEP: FOUT")); }

void rondTestAf(bool ok) {

  if (testStap == 5 && ok) {
    ok = motorBesturing->SchakelMotorUit();
    GA_SERIAL.println(F("SchakelMotorUit(): PWM uitgeschakeld."));
    toonFunctieResultaat(ok);
  }

  motorBesturing->StopMotor();
  GA_SERIAL.println(F("MOTOR: STOP aangeroepen. PWM-signaal uit; servo kan kracht verliezen."));
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
  GA_SERIAL.println(F("TEST: Sturing_MG996R_HardwareTest"));
  extenderPCA9685 = GedeeldeBusNewComponent<ExtenderPCA9685>(&Native, GedeeldeBusComponent::PWM_PCA9685, I2C_ADDRESS_EXTENDER_PCA9685_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL);
  if (extenderPCA9685 != nullptr) mg996r = GedeeldeBusNewComponent<MG996R>(extenderPCA9685, 1, 205, 410);
  if (mg996r != nullptr) motorBesturing = new ServomotorBesturing(*mg996r);
  startGelukt = mg996r != nullptr && motorBesturing != nullptr;
  if (startGelukt) motorBesturing->StopMotor();
  GA_SERIAL.println(startGelukt ? F("LEVENSCYCLUS: OK. BEGINSTATUS: PWM UIT.") : F("LEVENSCYCLUS: FOUT. GEEN TEST UITVOEREN."));
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
      GA_SERIAL.println(F("STOP: PWM uit. Ondersteun de mechanische last zo nodig; herstart Arduino voor nieuwe test."));
      return;
    }
    if (invoer == "1" && startGelukt && !testGestopt) {
      if (testActief && testStap == 4 && testFase == 1) {
        const bool ok = motorBesturing->HervatMotor();
        GA_SERIAL.println(F("HervatMotor(): PWM voor bewaarde doelhoek opnieuw ingeschakeld."));
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
        GA_SERIAL.println(F("TEST BEZIG. Wacht op STOP of de automatische stop."));
        return;
      }

      if (testStap >= 6) {
        toonTestInstructies();
        return;
      }

      GA_SERIAL.print(F("START TEST ")); GA_SERIAL.println(testStap + 1);
      bool ok = false;
      switch (testStap) {
        case 0: ok = motorBesturing->PositioneerAbsoluutNaarHoek(45); break;
        case 1: ok = motorBesturing->DraaiRelatiefOverHoek(60); break;
        case 2: ok = motorBesturing->PositioneerNaarMiddenpositie(); break;
        case 3: ok = motorBesturing->StartMotor(); break;
        case 4: ok = motorBesturing->PositioneerAbsoluutNaarHoek(135); break;
        case 5: ok = motorBesturing->SchakelMotorIn(); break;
      }

      if (!ok) {
        rondTestAf(false);
        return;
      }

      testActief = true;
      testFase = 0;
      testStartMillis = millis();
      GA_SERIAL.println(F("FUNCTIEAANROEP: OK. Je krijgt 2 seconden om de reactie te bekijken."));
      return;
    }
    GA_SERIAL.println(F("Geen geldige opdracht. Typ 1 + Enter, of STOP + Enter."));
  }
  if (!testActief) return;
  if (testStap == 4 && testFase == 1) return;
  if (testStap == 4 && testFase == 0 && millis() - testStartMillis >= 500UL) {
    const bool ok = motorBesturing->PauzeerMotor();
    GA_SERIAL.println(F("PauzeerMotor(): PWM uit. De fysieke stand is niet gemeten."));
    toonFunctieResultaat(ok);

    if (!ok) {
      rondTestAf(false);
      return;
    }

    testFase = 1;
    GA_SERIAL.println(F("Motor heeft geen houdsignaal. Typ 1 + Enter om de doelhoek opnieuw aan te sturen."));
    return;
  }
  if (millis() - testStartMillis >= 2000UL) rondTestAf(true);
}
