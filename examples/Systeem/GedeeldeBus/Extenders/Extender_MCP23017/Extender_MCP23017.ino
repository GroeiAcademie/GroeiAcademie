// ============================================================================
// Extender — MCP23017
// ============================================================================
// Basistest voor ExtenderMCP23017.
// Deze test valideert alleen de GedeeldeBus-stappen:
// aanmelden() -> controleren().
// inpluggen() en activeren() worden getest via een echte client/sensor.
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
  I2C_ADDRESS_EXTENDER_MCP23017_1, HardwareResourcePin::SDA, HardwareResourcePin::SCL, EXTENDER_MCP23017_1_INTA, EXTENDER_MCP23017_1_INTB, EXTENDER_MCP23017_1_RESET
);
#else
ExtenderMCP23017 extender(
  &Native,
  GedeeldeBusComponent::DIGITAL_PINS_MCP23017,
  HardwareResourcePin::SDA, HardwareResourcePin::SCL, 0
);
#endif

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }

  GA_SERIAL.println(F("=== ExtenderMCP23017 aanmelden/controleren-test ==="));

  bool gelukt = extender.aanmelden();
  GA_SERIAL.print(F("aanmelden: "));
  GA_SERIAL.println(gelukt ? F("OK") : F("FOUT"));

  if (gelukt) {
    gelukt = extender.controleren();
    GA_SERIAL.print(F("controleren: "));
    GA_SERIAL.println(gelukt ? F("OK") : F("FOUT"));
  }

  GedeeldeBusPrintEnVerwijderTijdelijkeConflicten(&extender);
  GA_SERIAL.println(gelukt ? F("RESULTAAT: OK") : F("RESULTAAT: FOUT"));
}

void loop() {
}
