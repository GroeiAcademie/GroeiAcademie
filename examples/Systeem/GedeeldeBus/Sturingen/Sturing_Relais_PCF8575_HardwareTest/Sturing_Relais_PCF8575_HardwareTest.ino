// ============================================================================
// Sturing: Sturing_Relais_PCF8575_HardwareTest
// ============================================================================
// Begeleide fysieke aan/uit-test van een Relais, met automatische GedeeldeBus-lifecycle.
// De test start uitgeschakeld. Typ 1 + Enter om AAN en daarna UIT te schakelen.
// STOP + Enter schakelt UIT en beëindigt de test; herstart de Arduino voor een nieuwe reeks.
// Test uitsluitend met een geschikte relaismodule en een geïsoleerd laagspanningscontact.
// Extender PCF8575: P0 naar relais IN; PCF8575 SDA naar SDA, SCL naar SCL, GND naar GND.
// Voeding van PCF8575 en relaismodule volgens module-specificaties en veilige logicaniveaus.
// Terugmelding: spanningsvrij NO-contact naar Native D2 en COM naar GND; geen netspanning op D2.
// Adres en optionele interruptpin: bestaande ConfigPCF8575.h.
// Configuratie: ConfigRelais.h (STURING_RELAIS_AANTAL >= 1) en ConfigPCF8575.h.
// ConfigPCF8575.h: EXTENDER_PCF8575_AANTAL 1; I2C_ADDRESS_EXTENDER_PCF8575_1 = echt adres; EXTENDER_PCF8575_1_INT = werkelijk aangesloten pin of NONE.
// Dit voorbeeld gebruikt een LOW-actieve relaismodule; geen ander niveau veronderstellen.
// ============================================================================

#include <GroeiAcademie.h>

#if STURING_RELAIS_AANTAL == 0
  #error Zet STURING_RELAIS_AANTAL op minstens 1 voor deze test.
#endif

#if EXTENDER_PCF8575_AANTAL != 1
  #error Deze test vereist EXTENDER_PCF8575_AANTAL 1.
#endif

ExtenderPCF8575* extender = nullptr;
Relais* sturing = nullptr;

bool testActief = false;
bool startGelukt = false;
bool testGestopt = false;
String invoer;
const uint8_t meetPin = NativeArduinoPinVan(HardwareResourcePin::NP_D2);

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sturing_Relais_PCF8575_HardwareTest"));
  extender = GedeeldeBusNewComponent<ExtenderPCF8575>(&Native, GedeeldeBusComponent::DIGITAL_PINS_PCF8575, I2C_ADDRESS_EXTENDER_PCF8575_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, EXTENDER_PCF8575_1_INT);
  if (extender != nullptr) sturing = GedeeldeBusNewComponent<Relais>(extender, 0, LOW);
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
