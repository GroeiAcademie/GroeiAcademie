// ============================================================================
// Sturing: Sturing_A4988_PCF8575_HardwareTest
// ============================================================================
// PCF8575 P0 -> A4988 STEP, P1 -> DIR, P2 -> ENABLE (active LOW).
// A4988 RESET/SLEEP/MS1/MS2/MS3 extern correct aansluiten.
// Controleer LOGICA-spanning en uitgangs-HIGH van de gekozen Extender.
// Externe motorvoeding en stroomlimiet instellen; gemeenschappelijke GND.
// Fysieke test pas na 1 + Enter; STOP voorkomt nieuwe stappen en zet ENABLE HIGH.
// STEP-pulsen via een Extender zijn beperkt door de werkelijke I2C/SPI-buslatentie.
// Stel STURING_A4988_AANTAL en EXTENDER_PCF8575_AANTAL minstens op 1.
// ============================================================================

#include <GroeiAcademie.h>
#if STURING_A4988_AANTAL == 0
  #error Zet STURING_A4988_AANTAL op minstens 1 voor deze test.
#endif
#if EXTENDER_PCF8575_AANTAL == 0
  #error EXTENDER_PCF8575_AANTAL moet minstens 1 zijn.
#endif

ExtenderPCF8575* extenderPCF8575 = nullptr;
A4988* a4988 = nullptr;

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
  GA_SERIAL.println(F("HARDWARETEST: Sturing_A4988_PCF8575_HardwareTest"));
  GA_SERIAL.println(F("BEGELEIDE STAPPENMOTORTEST. Sluit de motor veilig aan en maak het mechanisme vrij."));
  GA_SERIAL.println(F("ENABLE P2: active LOW tijdens stappen, HIGH bij STOP; controleer de werkelijke driver-uitgangen."));
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
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sturing_A4988_PCF8575_HardwareTest"));
  extenderPCF8575 = GedeeldeBusNewComponent<ExtenderPCF8575>(&Native, GedeeldeBusComponent::DIGITAL_PINS_PCF8575, I2C_ADDRESS_EXTENDER_PCF8575_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, EXTENDER_PCF8575_1_INT);
  if (extenderPCF8575 != nullptr) a4988 = GedeeldeBusNewComponent<A4988>(extenderPCF8575, 0, 1, 2);
  if (a4988 != nullptr) motorBesturing = new StappenmotorBesturing(*a4988);
  startGelukt = a4988 != nullptr && motorBesturing != nullptr;
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
