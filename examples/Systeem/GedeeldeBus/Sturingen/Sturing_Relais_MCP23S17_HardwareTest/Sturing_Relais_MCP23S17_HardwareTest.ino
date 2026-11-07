// ============================================================================
// Sturing: Sturing_Relais_MCP23S17_HardwareTest
// ============================================================================
// Begeleide fysieke aan/uit-test van een Relais, met automatische GedeeldeBus-lifecycle.
// Na START: 1 + Enter schakelt AAN, 0 + Enter schakelt UIT.
// STOP + Enter schakelt UIT en beëindigt de test; herstart de Arduino voor een nieuwe reeks.
// Test uitsluitend met een geschikte relaismodule en een geïsoleerd laagspanningscontact.
// Extender MCP23S17: GPA0 naar relais IN; SPI SCK, MISO en MOSI op de SPI-pinnen van het gekozen board.
// MCP23S17 DIN/SI -> MOSI, SO -> MISO, SCK -> SCK, CS -> de fysiek ingestelde EXTENDER_MCP23S17_1_CS_PIN (bij NP_D10: de board-D10-header).
// Voeding, GND en logicaniveaus volgens het concrete MCP23S17- en relaisbord.
// Terugmelding: spanningsvrij NO-contact naar MCP23S17 GPA1 en COM naar GND; geen externe spanning op GPA1.
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

// ============================================================================
// CONCREET AANSLUITEN EN TESTEN
// ============================================================================
// Gebruik een LOW-actieve relaisMODULE met geschikte stuurspanning en voeding.
// Relaismodule VCC en GND volgens het bord; signaalbron en relais-GND verbinden.
// Alleen een SPANNINGSVRIJ laagspanningscontact testen: COM -> GND, NO -> meetpin.
// Geen netspanning op een Arduino- of Extender-meetpin aansluiten.
// Na START: 1 + Enter = relais AAN; 0 + Enter = UIT; STOP schakelt UIT.
// Verwacht een relaisklik EN overgang van het terugmeldsignaal (NO open/gesloten).
// Actieve configuratie: src/Configuratie/Sturingen/ConfigRelais.h,
//   STURING_RELAIS_AANTAL >= 1.
// MCP23S17 GPA0 -> relais IN (actief LOW), GPA1 -> NO; COM -> GND.
// SPI-extender: SI -> MOSI, SO -> MISO, SCK -> SCK,
//   CS -> EXTENDER_MCP23S17_1_CS_PIN uit ConfigMCP23S17.h.
// VDD, VSS, RESET, adresselectie A0/A1/A2 volgens de echte MCP23S17-print.
// ConfigMCP23S17.h: EXTENDER_MCP23S17_AANTAL >= 1.
// ============================================================================

#include <GroeiAcademie.h>

#if STURING_RELAIS_AANTAL == 0
  #error Zet STURING_RELAIS_AANTAL op minstens 1 voor deze test.
#endif

#if EXTENDER_MCP23S17_AANTAL != 1
  #error Deze test vereist EXTENDER_MCP23S17_AANTAL 1.
#endif

ExtenderMCP23S17* extenderMCP23S17 = nullptr;
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
  GA_SERIAL.println(F("HARDWARETEST: Sturing_Relais_MCP23S17_HardwareTest. Typ START na controle van bedrading en voeding."));
  GA_SERIAL.println(F("Typ 1 + Enter voor relais AAN, 0 + Enter voor relais UIT."));
  GA_SERIAL.println(F("STOP schakelt het relais UIT; herstart de Arduino voor een nieuwe test."));
}

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }

  GA_SERIAL.println(F("TEST: Sturing_Relais_MCP23S17_HardwareTest"));
  extenderMCP23S17 = GedeeldeBusNewComponent<ExtenderMCP23S17>(&Native, GedeeldeBusComponent::DIGITAL_PINS_MCP23S17, EXTENDER_MCP23S17_1_CS_PIN, HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI);
  if (extenderMCP23S17 != nullptr) relais = GedeeldeBusNewComponent<Relais>(extenderMCP23S17, 0, LOW);

  startGelukt = relais != nullptr;
  GA_SERIAL.println(startGelukt ? F("LEVENSCYCLUS: OK") : F("LEVENSCYCLUS: FOUT"));

  if (startGelukt) {
    extenderMCP23S17->SetExtenderPinMode(1, INPUT_PULLUP);
    relais->setRelayStatusOnOff(false);

    GA_SERIAL.print(F("GPA1 RAW (rust): "));
    GA_SERIAL.println(extenderMCP23S17->ReadExtenderPin(1));
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
  bool gesloten = extenderMCP23S17->ReadExtenderPin(1) == LOW;

  GA_SERIAL.print(F("GPA1 RAW: "));
  GA_SERIAL.println(gesloten ? LOW : HIGH);
  GA_SERIAL.print(F("NO-contact: "));
  GA_SERIAL.println(gesloten ? F("GESLOTEN") : F("OPEN"));
  GA_SERIAL.println(gesloten == aan ? F("TERUGMELDING: PASS") : F("TERUGMELDING: FAIL (controleer polariteit/bedrading)"));
  GA_SERIAL.println(F("LED visueel controleren (geen automatische LED-feedback)."));
}
