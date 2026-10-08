// ============================================================================
// Extender — MCP23017
// ============================================================================
// Driver-/lifecycletest voor ExtenderMCP23017.
// Test: aanmelden() -> controleren() -> inpluggen() -> activeren() en daarna minimaal één echte driverfunctie.
// Configuratie gebeurt via UserConfig.h.
// ============================================================================

#include <Systeem/GedeeldeBus/GedeeldeBus.h>
#include <Configuratie/SystemConfig.h>

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

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }

  GA_SERIAL.println(F("=== ExtenderMCP23017 driver-/lifecycletest ==="));

  bool gelukt = extender.aanmelden();
  GA_SERIAL.print(F("aanmelden: "));
  GA_SERIAL.println(gelukt ? F("OK") : F("FOUT"));

  if (gelukt) {
    gelukt = extender.controleren();
    GA_SERIAL.print(F("controleren: "));
    GA_SERIAL.println(gelukt ? F("OK") : F("FOUT"));
  }

  if (gelukt) {
    gelukt = extender.inpluggen();
    GA_SERIAL.print(F("inpluggen: "));
    GA_SERIAL.println(gelukt ? F("OK") : F("FOUT"));
  }

  if (gelukt) {
    gelukt = extender.activeren();
    GA_SERIAL.print(F("activeren: "));
    GA_SERIAL.println(gelukt ? F("OK") : F("FOUT"));
  }

  if (gelukt) {
    gelukt = extender.pinMode1(0, OUTPUT);
    if (gelukt) gelukt = extender.write1(0, LOW);
    const uint8_t waarde = extender.read1(0);
    GA_SERIAL.print(F("pinMode1/write1/read1 P0: "));
    GA_SERIAL.println(waarde);
  }

  GedeeldeBusPrintEnVerwijderTijdelijkeConflicten(&extender);
  GA_SERIAL.println(gelukt ? F("RESULTAAT: OK") : F("RESULTAAT: FOUT"));
}

void loop() {
}
