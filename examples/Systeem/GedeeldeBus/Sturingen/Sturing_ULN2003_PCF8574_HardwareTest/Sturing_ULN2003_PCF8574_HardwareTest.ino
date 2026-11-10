// ============================================================================
// Sturing: Sturing_ULN2003_PCF8574_HardwareTest
// ============================================================================
// Begeleide fysieke test van ULN2003 met 28BYJ-48 stappenmotor.
// Iedere test start met 1 + Enter; STOP + Enter voorkomt nieuwe stapcommando's.
// De motor draait pas als de test actief is en de sturing daadwerkelijk werkt.
// GPIO-Extender PCF8574: P0 -> ULN2003 IN1, P1 -> IN2,
// P2 -> IN3 en P3 -> IN4. Motorstekker -> ULN2003-motorconnector.
// ULN2003-motorvoeding: geschikte EXTERNE 5V, GND gedeeld met Extender en Arduino.
// Controleer voeding bij het ULN2003-bord (V+ en GND). Gebruik geen Arduino-5V als motorvoeding.
// Extender-pinnen horen bij dezelfde GPIO-parent; GEDEELDE_BUS_MIXED_PINS is niet nodig.
// Actieve configuratie: ConfigULN2003.h (STURING_ULN2003_AANTAL >= 1)
// en ConfigPCF8574.h (EXTENDER_PCF8574_AANTAL >= 1, correct adres/pininstellingen).
// ============================================================================
// I2C-aansluiting: SDA/SCL en GND op dezelfde bus; modulevoeding volgens het gebruikte bord.
// VOORBEELD Arduino UNO R4 Minima / WiFi: SDA -> SDA, SCL -> SCL, GND -> GND.
// ESP32 D1 UNO R32: SDA=GPIO21, SCL=GPIO22.
// ESP32-S3 ARDI-32: SDA=GPIO38, SCL=GPIO39; ESP32-S3 DEV: SDA=GPIO8, SCL=GPIO9.
// RP2040 Cytron Maker UNO: SDA=GPIO20, SCL=GPIO21.
// STM32 F401RE Nucleo: SDA=PB9, SCL=PB8.
// I2C-adres en optionele INT/RESET-pinnen: bestaande actieve Config*.h.
// ============================================================================
// OPTIONELE SIGNALBUFFER: SN74AHCT125N (DIP-14)
// ============================================================================
// PCF-uitgang 0 -> SN74AHCT125N pin 2 (1A); pin 3 (1Y)  -> ULN2003 IN1.
// PCF-uitgang 1 -> SN74AHCT125N pin 5 (2A); pin 6 (2Y)  -> ULN2003 IN2.
// PCF-uitgang 2 -> SN74AHCT125N pin 9 (3A); pin 8 (3Y)  -> ULN2003 IN3.
// PCF-uitgang 3 -> SN74AHCT125N pin 12 (4A); pin 11 (4Y) -> ULN2003 IN4.
// SN74AHCT125N pin 14 (VCC) -> 5V logica; pin 7 (GND) -> gemeenschappelijke GND.
// SN74AHCT125N /OE: pinnen 1, 4, 10 en 13 -> samen naar 5V via 10k pull-up.
// /OE HIGH = buffer uit (hoogohmig); /OE LOW = buffer aan (geen inversie).
// Voorzie een afzonderlijke schakelaar van de /OE-lijn naar GND.
// Plaats 10k pull-down op elk van de ULN2003-ingangen IN1..IN4 naar GND,
// zodat de driver UIT blijft wanneer de buffer hoogohmig is.
// Laat /OE bij inschakelen HIGH. Zet de buffer pas AAN (/OE naar GND) NA
// 'LEVENSCYCLUS: OK. BEGINSTATUS: MOTOR GESTOPT.' in Serial.
// Zet /OE weer HIGH VOOR reset of uitschakelen van de Arduino.
// De buffer vervangt de motorvoeding NIET. De ULN2003 blijft de spoelen schakelen.
// Laat de buffer weg voor de rechtstreekse proef; PCF-HIGH kan te zwak zijn.
// Test nooit zonder geschikte externe motorvoeding. Maak geen bedrading onder spanning.
// ============================================================================

// ============================================================================
// CONCREET AANSLUITEN EN TESTEN
// ============================================================================
// 28BYJ-48 motorstekker -> 5-polige motorconnector van ULN2003-driverprint.
// ULN2003 V+ -> geschikte EXTERNE motorvoeding; GND -> motorvoeding (-).
// Arduino-, Extender- en ULN2003-GND gemeenschappelijk; geen motorstroom via GPIO.
// Extenderuitgangen -> ULN2003 IN1, IN2, IN3, IN4 volgens onderstaande volgorde.
// Zorg dat de motor mechanisch vrij kan draaien; markeer de motoras.
// Elke fysieke test pas starten met 1 + Enter; STOP voorkomt nieuwe stappen.
// Verwacht 1 stap, 4 stappen, achteruit en pauzeren/hervatten.
// Actieve configuratie: src/Configuratie/Sturingen/ConfigULN2003.h,
//   STURING_ULN2003_AANTAL >= 1.
// PCF8574 P0..P3 in indexvolgorde 0,1,2,3 -> ULN2003 IN1,IN2,IN3,IN4.
// Arduino SDA -> SDA van de I2C-Extender; Arduino SCL -> SCL; GND -> GND.
// Voed de Extender-logica op het voor board en I2C-niveau geschikte VCC/VDD.
// Verifieer fysiek I2C-adres en optionele INT/RESET-pinnen met de actieve Config*.h.
// Bij 3,3V-logica op een 5V-bus: controleer een passende I2C-niveauomzetter.
// MCP GPIO-uitgangen sturen de driver direct; een extra AHCT125 is niet vereist.
// PCF-poorten zijn quasi-bidirectioneel; controleer of HIGH het ULN2003
//   betrouwbaar activeert. De reeds beschreven SN74AHCT125N-buffer is optioneel.
//   Bij gebruik van buffer: /OE eerst HIGH en motorinputs veilig LOW houden.
// Extenderconfig: src/Configuratie/Extenders/ConfigPCF8574.h, echt adres/CS.
// ============================================================================

#include <GroeiAcademie.h>
#if STURING_ULN2003_AANTAL == 0
  #error Zet STURING_ULN2003_AANTAL op minstens 1 voor deze test.
#endif
#if EXTENDER_PCF8574_AANTAL == 0
  #error Schakel de vereiste Extender in voor deze Sturing-test.
#endif

ExtenderPCF8574* extenderPCF8574 = nullptr;
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
  GA_SERIAL.println(F("HARDWARETEST: Sturing_ULN2003_PCF8574_HardwareTest"));
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
  GA_SERIAL.println(F("TEST: Sturing_ULN2003_PCF8574_HardwareTest"));
  extenderPCF8574 = GedeeldeBusNewComponent<ExtenderPCF8574>(&Native, GedeeldeBusComponent::DIGITAL_PINS_PCF8574, I2C_ADDRESS_EXTENDER_PCF8574_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, EXTENDER_PCF8574_1_INT);
  if (extenderPCF8574 != nullptr) uln2003 = GedeeldeBusNewComponent<ULN2003>(extenderPCF8574, 0, 1, 2, 3);
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
