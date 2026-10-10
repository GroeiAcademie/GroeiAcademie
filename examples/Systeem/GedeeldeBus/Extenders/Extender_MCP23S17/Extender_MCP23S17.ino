// ============================================================================
// Extender: MCP23S17
// ============================================================================
// Driver-/lifecycletest voor ExtenderMCP23S17.
// Test: aanmelden() -> controleren() -> inpluggen() -> activeren() en daarna minimaal één echte driverfunctie.
// Configuratie gebeurt via UserConfig.h.
// ============================================================================

#include <GroeiAcademie.h>

#if EXTENDER_MCP23S17_AANTAL == 0
  #error Zet EXTENDER_MCP23S17_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif

#if EXTENDER_MCP23S17_AANTAL == 1
ExtenderMCP23S17 extender(&Native, GedeeldeBusComponent::GC_EXTENDER, CS_PIN_EXTENDER_MCP23S17_1, HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI);
#else
ExtenderMCP23S17 extender(&Native, GedeeldeBusComponent::GC_EXTENDER, HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, 0);
#endif

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  bool gelukt = extender.aanmelden() && extender.controleren() && extender.inpluggen() && extender.activeren();
  if (gelukt) gelukt = extender.pinMode1(0, OUTPUT) && extender.write1(0, LOW);
  GA_SERIAL.println(gelukt ? F("RESULTAAT: OK") : F("RESULTAAT: FOUT"));
}
void loop() {}
