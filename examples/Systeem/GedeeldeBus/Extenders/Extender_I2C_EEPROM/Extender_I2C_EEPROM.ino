// ============================================================================
// Extender: I2C_EEPROM
// ============================================================================
// Driver-/lifecycletest voor ExtenderI2C_EEPROM.
// Test: aanmelden() -> controleren() -> inpluggen() -> activeren() en daarna minimaal één echte driverfunctie.
// Configuratie gebeurt via UserConfig.h.
// ============================================================================

#include <GroeiAcademie.h>

#if EXTENDER_I2C_EEPROM_AANTAL == 0
  #error Zet EXTENDER_I2C_EEPROM_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif

#if EXTENDER_I2C_EEPROM_AANTAL == 1
ExtenderI2C_EEPROM extender(&Native, GedeeldeBusComponent::GC_EXTENDER, I2C_ADDRESS_EXTENDER_I2C_EEPROM_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL);
#else
ExtenderI2C_EEPROM extender(&Native, GedeeldeBusComponent::GC_EXTENDER, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, 0);
#endif

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  bool gelukt = extender.aanmelden() && extender.controleren() && extender.inpluggen() && extender.activeren();
  if (gelukt) {
    const uint8_t waarde = extender.readByte(0);
    GA_SERIAL.print(F("EEPROM[0]: "));
    GA_SERIAL.println(waarde);
  }
  GA_SERIAL.println(gelukt ? F("RESULTAAT: OK") : F("RESULTAAT: FOUT"));
}
void loop() {}
