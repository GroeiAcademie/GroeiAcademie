// ============================================================================
// Sturing: Sturing_Relais_MCP23017_HardwareTest
// ============================================================================
// Begeleide fysieke aan/uit-test van een Relais, met automatische GedeeldeBus-lifecycle.
// Na START: 1 + Enter schakelt AAN, 0 + Enter schakelt UIT.
// STOP + Enter schakelt UIT en beëindigt de test; herstart de Arduino voor een nieuwe reeks.
// Test uitsluitend met een geschikte relaismodule en een geïsoleerd laagspanningscontact.
// Extender MCP23017: GPA0 naar relais IN; MCP23017 SDA naar SDA, SCL naar SCL en GND naar GND.
// Voeding en logicaniveaus volgens het concrete MCP23017- en relaisbord.
// Terugmelding: spanningsvrij NO-contact naar MCP23017 GPA1 en COM naar GND; geen externe spanning op GPA1.
// I2C-adres, INTA, INTB en RESET: bestaande ConfigMCP23017.h.
// Fysieke I2C-pinnen volgens de bestaande boardprofielen:
// UNO R4 Minima/WiFi, UNO Q: de SDA- en SCL-boardaansluitingen.
// ESP32 D1 UNO R32: SDA=GPIO21, SCL=GPIO22.
// ESP32-S3 ARDI-32: SDA=GPIO38, SCL=GPIO39; DEV: SDA=GPIO8, SCL=GPIO9.
// RP2040 Cytron Maker UNO: SDA=GPIO20, SCL=GPIO21.
// STM32 F401RE Nucleo: SDA=PB9, SCL=PB8.
// Configuratie: ConfigRelais.h (STURING_RELAIS_AANTAL >= 1) en ConfigMCP23017.h.
// ConfigMCP23017.h: EXTENDER_MCP23017_AANTAL 1; I2C_ADDRESS_EXTENDER_MCP23017_1 = echt adres; INTA, INTB, RESET volgens werkelijke aansluitingen of NONE.
// Dit voorbeeld gebruikt een LOW-actieve relaismodule; geen ander niveau veronderstellen.
// ============================================================================

// ============================================================================
// CONCREET AANSLUITEN EN TESTEN
// ============================================================================
// Arduino SDA -> SDA van de I2C-Extender; Arduino SCL -> SCL; GND -> GND.
// Voed de Extender-logica op het voor board en I2C-niveau geschikte VCC/VDD.
// Verifieer fysiek I2C-adres en optionele INT/RESET-pinnen met de actieve Config*.h.
// Bij 3,3V-logica op een 5V-bus: controleer een passende I2C-niveauomzetter.
// Gebruik een LOW-actieve relaisMODULE met geschikte stuurspanning en voeding.
// Relaismodule VCC en GND volgens het bord; signaalbron en relais-GND verbinden.
// Alleen een SPANNINGSVRIJ laagspanningscontact testen: COM -> GND, NO -> meetpin.
// Geen netspanning op een Arduino- of Extender-meetpin aansluiten.
// Na START: 1 + Enter = relais AAN; 0 + Enter = UIT; STOP schakelt UIT.
// Verwacht een relaisklik EN overgang van het terugmeldsignaal (NO open/gesloten).
// Actieve configuratie: src/Configuratie/Sturingen/ConfigRelais.h,
//   STURING_RELAIS_AANTAL >= 1.
// MCP23017 GPA0 -> relais IN (actief LOW), GPA1 -> NO; COM -> GND.
// I2C-extender: ConfigMCP23017.h (EXTENDER_MCP23017_AANTAL >= 1).
// ============================================================================

#include <GroeiAcademie.h>

#if STURING_RELAIS_AANTAL == 0
  #error Zet STURING_RELAIS_AANTAL op minstens 1 voor deze test.
#endif

#if EXTENDER_MCP23017_AANTAL != 1
  #error Deze test vereist EXTENDER_MCP23017_AANTAL 1.
#endif

ExtenderMCP23017* extenderMCP23017 = nullptr;
Relais* relais = nullptr;

bool testActief = false;
bool startGelukt = false;
bool testGestopt = false;
String invoer;

// INPUT_PULLUP: NO open = HIGH, NO gesloten = LOW.
// Sluit NO op GPA1 en COM op GND aan. Geen externe spanning op GPA1.

void toonFunctieResultaat(bool ok) {
  GA_SERIAL.println(ok ? F("OPDRACHT: OK") : F("OPDRACHT: FOUT"));
}

void toonStartInstructies() {
  GA_SERIAL.println(F("HARDWARETEST: Sturing_Relais_MCP23017_HardwareTest. Typ START na controle van bedrading en voeding."));
  GA_SERIAL.println(F("Typ 1 + Enter voor relais AAN, 0 + Enter voor relais UIT."));
  GA_SERIAL.println(F("STOP schakelt het relais UIT; herstart de Arduino voor een nieuwe test."));
}

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  #if defined(DEBUG)
  while (!GA_SERIAL) { ; }
  #else
  unsigned long startTijd = millis();
  while (!GA_SERIAL && (millis() - startTijd) < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  #endif

  GA_SERIAL.println(F("TEST: Sturing_Relais_MCP23017_HardwareTest"));
  extenderMCP23017 = GedeeldeBusNewComponent<ExtenderMCP23017>(&Native, GedeeldeBusComponent::DIGITAL_PINS_MCP23017, I2C_ADDRESS_EXTENDER_MCP23017_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, EXTENDER_MCP23017_1_INTA, EXTENDER_MCP23017_1_INTB, EXTENDER_MCP23017_1_RESET);
  if (extenderMCP23017 != nullptr) relais = GedeeldeBusNewComponent<Relais>(extenderMCP23017, 0, LOW);

  startGelukt = relais != nullptr;
  GA_SERIAL.println(startGelukt ? F("LEVENSCYCLUS: OK") : F("LEVENSCYCLUS: FOUT"));

  if (startGelukt) {
    extenderMCP23017->SetExtenderPinMode(1, INPUT_PULLUP);
    relais->setRelayStatusOnOff(false);

    GA_SERIAL.print(F("GPA1 RAW (rust): "));
    GA_SERIAL.println(extenderMCP23017->ReadExtenderPin(1));
  }
  toonStartInstructies();
}

void loop() {
  if (!GA_SERIAL.available()) return;

  invoer = GA_SERIAL.readStringUntil('\n');
  invoer.trim();
  invoer.toUpperCase();

  if (invoer == "STOP") {
    if (startGelukt) relais->StopSturing();

    testActief = false;
    testGestopt = true;
    GA_SERIAL.println(F("STOP: relais UIT. Herstart de Arduino om opnieuw te testen."));
    return;
  }

  if (testGestopt) {
    GA_SERIAL.println(F("Test gestopt: herstart de Arduino."));
    return;
  }

  if (!startGelukt) {
    GA_SERIAL.println(F("FOUT: levenscyclus."));
    return;
  }

  if (invoer == "START") {
    if (!testActief) {
      testActief = true;
      GA_SERIAL.println(F("START: bevestigd."));
    }

    return;
  }

  if (!testActief) {
    GA_SERIAL.println(F("Eerst START invoeren."));
    return;
  }

  if (invoer != "1" && invoer != "0") {
    GA_SERIAL.println(F("Gebruik 1 (AAN) of 0 (UIT)."));
    return;
  }

  bool aan = invoer == "1";
  bool gelukt = relais->setRelayStatusOnOff(aan);

  if (!gelukt) {
    toonFunctieResultaat(false);
    return;
  }

  delay(80);
  bool gesloten = extenderMCP23017->ReadExtenderPin(1) == LOW;

  GA_SERIAL.print(F("GPA1 RAW: "));
  GA_SERIAL.println(gesloten ? LOW : HIGH);
  GA_SERIAL.print(F("NO-contact: "));
  GA_SERIAL.println(gesloten ? F("GESLOTEN") : F("OPEN"));
  GA_SERIAL.println(gesloten == aan ? F("TERUGMELDING: PASS") : F("TERUGMELDING: FAIL (controleer polariteit/bedrading)"));
  GA_SERIAL.println(F("LED visueel controleren (geen automatische LED-feedback)."));
}
