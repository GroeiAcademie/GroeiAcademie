// ============================================================================
// Extender: MCP23S17
// ============================================================================
// Driver-/lifecycletest voor ExtenderMCP23S17.
// Test: aanmelden() -> controleren() -> inpluggen() -> activeren() en daarna minimaal één echte driverfunctie.
// Configuratie gebeurt via UserConfig.h.
// SPI-parent/Extender: DIN/SDI -> MOSI, DOUT/SDO -> MISO, CLK/SCK -> SCK; CS volgens de bestaande constructor/configuratie.
// UNO R4 Minima/WiFi: MOSI=D11 (COPI), MISO=D12 (CIPO), SCK=D13; CS via de geconfigureerde pin.
// ESP32 D1 UNO R32: MOSI=GPIO23, MISO=GPIO19, SCK=GPIO18 (bestaand FrameWork-profiel).
// ESP32-S3 ARDI-32/DEV: MOSI=GPIO11, MISO=GPIO13, SCK=GPIO12 (FrameWork-profiel).
// RP2040 Cytron Maker UNO: MOSI=GPIO11, MISO=GPIO12, SCK=GPIO10.
// STM32 F401RE Nucleo: MOSI=PA7, MISO=PA6, SCK=PA5 (FrameWork-profiel).
// Module-VCC en GND volgens het gebruikte bord; SPI-niveaus en eventuele levelshifting controleren.
// CS-keuze in deze sketch: EXTENDER_MCP23S17_1_CS_PIN.
// ============================================================================

#include <GroeiAcademie.h>

#if EXTENDER_MCP23S17_AANTAL == 0
  #error Zet EXTENDER_MCP23S17_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif

#if EXTENDER_MCP23S17_AANTAL == 1
ExtenderMCP23S17 extender(&Native, GedeeldeBusComponent::DIGITAL_PINS_MCP23S17, EXTENDER_MCP23S17_1_CS_PIN, HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI);
#else
ExtenderMCP23S17 extender(&Native, GedeeldeBusComponent::DIGITAL_PINS_MCP23S17, HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, 0);
#endif

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  #if defined(DEBUG)
  while (!GA_SERIAL) { ; }
  #else
  unsigned long startTijd = millis();
  while (!GA_SERIAL && (millis() - startTijd) < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  #endif
  GA_SERIAL.println(F("TEST: Extender_MCP23S17"));
  bool gelukt = extender.aanmelden() && extender.controleren() && extender.inpluggen() && extender.activeren();
  if (gelukt) gelukt = extender.pinMode1(0, OUTPUT) && extender.write1(0, LOW);
  GedeeldeBusPrintEnVerwijderTijdelijkeConflicten(&extender);
  GA_SERIAL.println(gelukt ? F("RESULTAAT: OK") : F("RESULTAAT: FOUT"));
}
void loop() {}
