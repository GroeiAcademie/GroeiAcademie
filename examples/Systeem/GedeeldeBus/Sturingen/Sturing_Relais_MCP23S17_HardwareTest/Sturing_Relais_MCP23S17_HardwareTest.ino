// ============================================================================
// Sturing: Sturing_Relais_MCP23S17_HardwareTest
// ============================================================================
// Begeleide fysieke aan/uit-test van een Relais, met automatische GedeeldeBus-lifecycle.
// De test start uitgeschakeld. Typ 1 + Enter om AAN en daarna UIT te schakelen.
// STOP + Enter schakelt UIT en beëindigt de test; herstart de Arduino voor een nieuwe reeks.
// Test uitsluitend met een geschikte relaismodule en een geïsoleerd laagspanningscontact.
// Extender MCP23S17: GPA0 naar relais IN; SPI SCK, MISO en MOSI op de SPI-pinnen van het gekozen board.
// Chipselect van MCP23S17: de bestaande EXTENDER_MCP23S17_1_CS_PIN uit ConfigMCP23S17.h.
// Voeding, GND en logicaniveaus volgens het concrete MCP23S17- en relaisbord.
// Terugmelding: spanningsvrij NO-contact naar Native D2 en COM naar GND; geen netspanning op D2.
// De fysieke boardmapping van SPI en CS komt uit SystemConfig.h en het gekozen boardprofiel.
// Configuratie: ConfigRelais.h (STURING_RELAIS_AANTAL >= 1) en ConfigMCP23S17.h.
// ConfigMCP23S17.h: EXTENDER_MCP23S17_AANTAL 1; EXTENDER_MCP23S17_1_CS_PIN = aangesloten Native CS-resource (bv. NP_D10).
// SPI-pinnen van de ondersteunde boards volgens UserConfig.h:
// UNO R4 Minima/WiFi: MOSI D11, MISO D12, SCK D13; CS volgens configuratie, bijvoorbeeld D10.
// ESP32 D1 UNO R32: MOSI GPIO23, MISO GPIO19, SCK GPIO18; CS volgens configuratie, D10 is GPIO5.
// ESP32-S3 ARDI-32/DEV: MOSI GPIO11, MISO GPIO13, SCK GPIO12; CS volgens configuratie, D10 is GPIO10.
// RP2040 Cytron Maker UNO: MOSI GPIO11, MISO GPIO12, SCK GPIO10; CS volgens configuratie, D10 is GPIO13.
// STM32 F401RE Nucleo: MOSI D11/PA7, MISO D12/PA6, SCK D13/PA5; CS volgens configuratie.
// UNO Q: MOSI D11, MISO D12, SCK D13; CS volgens configuratie.
// Dit voorbeeld gebruikt een LOW-actieve relaismodule; geen ander niveau veronderstellen.
// ============================================================================

#include <GroeiAcademie.h>

#if STURING_RELAIS_AANTAL == 0
  #error Zet STURING_RELAIS_AANTAL op minstens 1 voor deze test.
#endif

#if EXTENDER_MCP23S17_AANTAL != 1
  #error Deze test vereist EXTENDER_MCP23S17_AANTAL 1.
#endif

ExtenderMCP23S17* extender = nullptr;
Relais* sturing = nullptr;

bool testActief = false;
bool startGelukt = false;
bool testGestopt = false;
String invoer;
const uint8_t meetPin = NativeArduinoPinVan(HardwareResourcePin::NP_D2);

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sturing_Relais_MCP23S17_HardwareTest"));
  extender = GedeeldeBusNewComponent<ExtenderMCP23S17>(&Native, GedeeldeBusComponent::DIGITAL_PINS_MCP23S17, EXTENDER_MCP23S17_1_CS_PIN, HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI);
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
