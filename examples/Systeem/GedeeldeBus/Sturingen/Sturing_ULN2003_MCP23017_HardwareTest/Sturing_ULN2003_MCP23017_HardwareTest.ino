// ============================================================================
// Sturing: Sturing_ULN2003_MCP23017_HardwareTest
// ============================================================================
// Begeleide fysieke test van ULN2003 met 28BYJ-48 stappenmotor.
// Iedere test start met 1 + Enter; STOP + Enter voorkomt nieuwe stapcommando's.
// De motor draait pas als de test actief is en de sturing daadwerkelijk werkt.
// GPIO-Extender MCP23017: GPA0 -> ULN2003 IN1, GPA1 -> IN2,
// GPA2 -> IN3 en GPA3 -> IN4. Motorstekker -> ULN2003-motorconnector.
// ULN2003-motorvoeding: geschikte EXTERNE 5V, GND gedeeld met Extender en Arduino.
// Controleer voeding bij het ULN2003-bord (V+ en GND). Gebruik geen Arduino-5V als motorvoeding.
// Extender-pinnen horen bij dezelfde GPIO-parent; GEDEELDE_BUS_MIXED_PINS is niet nodig.
// Actieve configuratie: ConfigULN2003.h (STURING_ULN2003_AANTAL >= 1)
// en ConfigMCP23017.h (EXTENDER_MCP23017_AANTAL >= 1, correct adres/pininstellingen).
// ============================================================================
// I2C-aansluiting: SDA/SCL en GND op dezelfde bus; modulevoeding volgens het gebruikte bord.
// VOORBEELD Arduino UNO R4 Minima / WiFi: SDA -> SDA, SCL -> SCL, GND -> GND.
// ESP32 D1 UNO R32: SDA=GPIO21, SCL=GPIO22.
// ESP32-S3 ARDI-32: SDA=GPIO38, SCL=GPIO39; ESP32-S3 DEV: SDA=GPIO8, SCL=GPIO9.
// RP2040 Cytron Maker UNO: SDA=GPIO20, SCL=GPIO21.
// STM32 F401RE Nucleo: SDA=PB9, SCL=PB8.
// I2C-adres en optionele INT/RESET-pinnen: bestaande actieve Config*.h.
// Deze test gebruikt rechtstreeks de push-pull-GPIO-uitgangen van de MCP-Extender.
// Een externe SN74AHCT125N is hiervoor niet vereist; de bedrading moet wel correct zijn.
// ============================================================================

#include <GroeiAcademie.h>
#if STURING_ULN2003_AANTAL == 0
  #error Zet STURING_ULN2003_AANTAL op minstens 1 voor deze test.
#endif
#if EXTENDER_MCP23017_AANTAL == 0
  #error Schakel de vereiste Extender in voor deze Sturing-test.
#endif

ExtenderMCP23017* extenderMCP23017 = nullptr;
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
  GA_SERIAL.println(F("HARDWARETEST: Sturing_ULN2003_MCP23017_HardwareTest"));
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
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sturing_ULN2003_MCP23017_HardwareTest"));
  extenderMCP23017 = GedeeldeBusNewComponent<ExtenderMCP23017>(&Native, GedeeldeBusComponent::DIGITAL_PINS_MCP23017, I2C_ADDRESS_EXTENDER_MCP23017_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, EXTENDER_MCP23017_1_INTA, EXTENDER_MCP23017_1_INTB, EXTENDER_MCP23017_1_RESET);
  if (extenderMCP23017 != nullptr) uln2003 = GedeeldeBusNewComponent<ULN2003>(extenderMCP23017, 0, 1, 2, 3);
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
