// ============================================================================
// Sturing: Sturing_Relais_Native_HardwareTest
// ============================================================================
// Begeleide fysieke aan/uit-test van een Relais, met automatische GedeeldeBus-lifecycle.
// De test start uitgeschakeld. Typ 1 + Enter om AAN en daarna UIT te schakelen.
// STOP + Enter schakelt UIT en beëindigt de test; herstart de Arduino voor een nieuwe reeks.
// Test uitsluitend met een geschikte relaismodule en een geïsoleerd laagspanningscontact.
// Rechtstreeks relais via Native: aansluiting D6 (relais IN), GND gemeenschappelijk.
// Voeding van de relaismodule volgens haar specificaties, niet vanaf een Arduino-GPIO.
// Terugmelding: spanningsvrij NO-contact naar D2 en COM naar GND. Geen netspanning op meetpin D2.
// De Native pin D6 wordt via NativeArduinoPinVan() naar de effectieve boardpin vertaald.
// Concrete boardmapping uit UserConfig.h (relais IN / NO-contactmeting):
// UNO R4 Minima/WiFi en UNO Q: D6 / D2.
// ESP32 D1 UNO R32: GPIO27 (D6) / GPIO26 (D2).
// ESP32-S3 ARDI-32: GPIO9 (D6) / GPIO1 (D2); ESP32-S3 DEV: GPIO3 (D6) / GPIO18 (D2).
// RP2040 Cytron Maker UNO: GPIO6 (D6) / GPIO2 (D2).
// STM32 F401RE Nucleo: PB10 (D6) / PA10 (D2).
// Configuratie: src/Configuratie/Sturingen/ConfigRelais.h met STURING_RELAIS_AANTAL >= 1.
// Dit voorbeeld gebruikt een LOW-actieve relaismodule; geen ander niveau veronderstellen.
// ============================================================================

#include <GroeiAcademie.h>

#if STURING_RELAIS_AANTAL == 0
  #error Zet STURING_RELAIS_AANTAL op minstens 1 voor deze test.
#endif

Relais* sturing = nullptr;

bool testActief = false;
bool startGelukt = false;
bool testGestopt = false;
String invoer;
const uint8_t meetPin = NativeArduinoPinVan(HardwareResourcePin::NP_D2);

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sturing_Relais_Native_HardwareTest"));
  sturing = GedeeldeBusNewComponent<Relais>(&Native, static_cast<uint8_t>(HardwareResourcePin::NP_D6), LOW);
  startGelukt = sturing != nullptr;
  GA_SERIAL.println(startGelukt ? F("LEVENSCYCLUS: OK") : F("LEVENSCYCLUS: FOUT"));
  if (!startGelukt) return;
  pinMode(meetPin, INPUT_PULLUP);
  sturing->setRelayStatusOnOff(false);
  GA_SERIAL.println(F("UIT: relais niet bekrachtigd; NO-contact moet OPEN zijn."));
  GA_SERIAL.println(F("TEST 1: schakel relais AAN. Verwacht: NO-contact sluit. Typ 1 + Enter."));
}

void loop() {
  if (!startGelukt || !GA_SERIAL.available()) return;
  invoer = GA_SERIAL.readStringUntil('\n');
  invoer.trim();
  invoer.toUpperCase();
  if (invoer == "STOP") {
    sturing->StopSturing();
    testGestopt = true;
    GA_SERIAL.println(F("STOP: relais UIT. Herstart de Arduino om opnieuw te testen."));
    return;
  }
  if (testGestopt) { GA_SERIAL.println(F("Test gestopt: herstart de Arduino.")); return; }
  if (invoer != "1") { GA_SERIAL.println(F("Typ 1 + Enter, of STOP + Enter.")); return; }
  testActief = !testActief;
  if (!sturing->setRelayStatusOnOff(testActief)) { GA_SERIAL.println(F("FUNCTIEAANROEP: FOUT")); sturing->StopSturing(); testGestopt = true; return; }
  delay(80);
  const bool gesloten = digitalRead(meetPin) == LOW;
  GA_SERIAL.print(F("NO-contact: ")); GA_SERIAL.println(gesloten ? F("GESLOTEN") : F("OPEN"));
  GA_SERIAL.println(gesloten == testActief ? F("TERUGMELDING: PASS") : F("TERUGMELDING: FAIL (controleer bedrading en polariteit)"));
  GA_SERIAL.println(testActief ? F("TEST 2: schakel relais UIT. Verwacht: NO-contact opent. Typ 1 + Enter.") : F("TEST 1: schakel relais AAN. Verwacht: NO-contact sluit. Typ 1 + Enter."));
}
