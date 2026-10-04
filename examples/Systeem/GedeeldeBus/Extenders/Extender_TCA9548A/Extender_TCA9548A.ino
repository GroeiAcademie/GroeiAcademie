// ============================================================================
// Extender — TCA9548A
// ============================================================================
// Basistest voor ExtenderTCA9548A.
// Deze test valideert alleen de GedeeldeBus-stappen:
// aanmelden() -> controleren().
// inpluggen() en activeren() worden getest via een echte client/sensor.
// Configuratie gebeurt via UserConfig.h.
// ============================================================================

#include <Systeem/GedeeldeBus/GedeeldeBus.h>
#include <Configuratie/SystemConfig.h>

#if EXTENDER_TCA9548A_AANTAL == 0
  #error Zet EXTENDER_TCA9548A_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif

#if EXTENDER_TCA9548A_AANTAL == 1
ExtenderTCA9548A extender(
  &Native,
  GedeeldeBusComponent::I2C_MULTIPLEXER_TCA9548A,
  I2C_ADDRESS_EXTENDER_TCA9548A_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, EXTENDER_TCA9548A_1_RESET
);
#else
ExtenderTCA9548A extender(
  &Native,
  GedeeldeBusComponent::I2C_MULTIPLEXER_TCA9548A,
  HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, 0
);
#endif

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }

  GA_SERIAL.println(F("=== ExtenderTCA9548A aanmelden/controleren-test ==="));

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
