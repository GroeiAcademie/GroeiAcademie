// ============================================================================
// ExtenderMCP23017: Test ExtenderPins Toegelaten
// ============================================================================
// Test alle ExtenderPins die deze extender momenteel aanbiedt.
// De test gebruikt alleen aanmelden() en controleren(); er wordt geen hardware ingeplugd of geactiveerd.
// Configuratie gebeurt via UserConfig.h.
// Aansluiting gebruikte I2C-parent/Extender: SDA -> board-SDA, SCL -> board-SCL; GND -> board-GND.
// UNO R4 Minima/WiFi: fysieke SDA- en SCL-header (bij R4 Minima ook A4=SDA, A5=SCL); UNO Q: board-SDA/SCL.
// ESP32 D1 UNO R32: SDA=GPIO21, SCL=GPIO22 (bestaand FrameWork-boardprofiel).
// ESP32-S3 ARDI-32: SDA=GPIO38, SCL=GPIO39; DEV: SDA=GPIO8, SCL=GPIO9 (FrameWork-profiel).
// RP2040 Cytron Maker UNO: SDA=GPIO20, SCL=GPIO21 (ook Maker/Grove 6).
// STM32 F401RE Nucleo: SDA=PB9, SCL=PB8 (bestaand FrameWork-boardprofiel).
// Voeding VCC en GND volgens het concrete modulebord; controleer I2C-logicaniveau, pull-ups en eventuele levelshifting.
// ============================================================================

#include <GroeiAcademie.h>

#if EXTENDER_MCP23017_AANTAL == 0
  #error Zet EXTENDER_MCP23017_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif

#if EXTENDER_MCP23017_AANTAL == 1
ExtenderMCP23017 extender(
  &Native,
  GedeeldeBusComponent::DIGITAL_PINS_MCP23017,
  I2C_ADDRESS_EXTENDER_MCP23017_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, EXTENDER_MCP23017_1_INTA, EXTENDER_MCP23017_1_INTB, EXTENDER_MCP23017_1_RESET
);
#else
ExtenderMCP23017 extender(
  &Native,
  GedeeldeBusComponent::DIGITAL_PINS_MCP23017,
  HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, 0
);
#endif

uint8_t ExtenderPins[] = {
  static_cast<uint8_t>(ExtenderMCP23017::ExtenderPins::EP_GPA0),
  static_cast<uint8_t>(ExtenderMCP23017::ExtenderPins::EP_GPA1),
  static_cast<uint8_t>(ExtenderMCP23017::ExtenderPins::EP_GPA2),
  static_cast<uint8_t>(ExtenderMCP23017::ExtenderPins::EP_GPA3),
  static_cast<uint8_t>(ExtenderMCP23017::ExtenderPins::EP_GPA4),
  static_cast<uint8_t>(ExtenderMCP23017::ExtenderPins::EP_GPA5),
  static_cast<uint8_t>(ExtenderMCP23017::ExtenderPins::EP_GPA6),
  static_cast<uint8_t>(ExtenderMCP23017::ExtenderPins::EP_GPA7),
  static_cast<uint8_t>(ExtenderMCP23017::ExtenderPins::EP_GPB0),
  static_cast<uint8_t>(ExtenderMCP23017::ExtenderPins::EP_GPB1),
  static_cast<uint8_t>(ExtenderMCP23017::ExtenderPins::EP_GPB2),
  static_cast<uint8_t>(ExtenderMCP23017::ExtenderPins::EP_GPB3),
  static_cast<uint8_t>(ExtenderMCP23017::ExtenderPins::EP_GPB4),
  static_cast<uint8_t>(ExtenderMCP23017::ExtenderPins::EP_GPB5),
  static_cast<uint8_t>(ExtenderMCP23017::ExtenderPins::EP_GPB6),
  static_cast<uint8_t>(ExtenderMCP23017::ExtenderPins::EP_GPB7)
};

GedeeldeBusNode SensorTest(
  &extender,
  { nullptr, 0, ExtenderPins, sizeof(ExtenderPins) / sizeof(ExtenderPins[0]) },
  GedeeldeBusComponent::GC_SENSOR,
  HardwareResourceToegang::GEDEELD
);

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  #if defined(DEBUG)
  while (!GA_SERIAL) { ; }
  #else
  unsigned long startTijd = millis();
  while (!GA_SERIAL && (millis() - startTijd) < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  #endif
  GA_SERIAL.println(F("TEST: Extender_MCP23017_Test_ExtenderPins_Toegelaten"));

  GA_SERIAL.println(F("=== ExtenderMCP23017 ExtenderPins Toegelaten ==="));

  bool gelukt = extender.aanmelden();
  if (gelukt) gelukt = extender.controleren();
  GA_SERIAL.print(F("Extender aanmelden/controleren: "));
  GA_SERIAL.println(gelukt ? F("OK") : F("FOUT"));

  if (gelukt) {
    gelukt = SensorTest.aanmelden();
    GA_SERIAL.print(F("Sensor aanmelden: "));
    GA_SERIAL.println(gelukt ? F("OK") : F("FOUT"));
  }

  if (gelukt) {
    gelukt = SensorTest.controleren();
    GA_SERIAL.print(F("Alle ExtenderPins toegelaten: "));
    GA_SERIAL.println(gelukt ? F("OK") : F("FOUT"));
  }

  GedeeldeBusPrintEnVerwijderTijdelijkeConflicten(&SensorTest);
  GA_SERIAL.println(gelukt ? F("RESULTAAT: OK") : F("RESULTAAT: FOUT"));
}

void loop() {
}
