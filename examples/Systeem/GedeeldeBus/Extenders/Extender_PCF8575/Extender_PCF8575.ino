// ============================================================================
// Extender — PCF8575
// ============================================================================
// Basistest voor ExtenderPCF8575.
// Deze test valideert alleen de GedeeldeBus-stappen:
// aanmelden() -> controleren().
// inpluggen() en activeren() worden getest via een echte client/sensor.
// Configuratie gebeurt via UserConfig.h.
// ============================================================================

#include <Systeem/GedeeldeBus/GedeeldeBus.h>
#include <Configuratie/SystemConfig.h>

#if EXTENDER_PCF8575_AANTAL == 0
  #error Zet EXTENDER_PCF8575_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif

#if EXTENDER_PCF8575_AANTAL == 1
ExtenderPCF8575 extender(
  &Native,
  GedeeldeBusComponent::DIGITAL_PINS_PCF8575,
  I2C_ADDRESS_EXTENDER_PCF8575_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, EXTENDER_PCF8575_1_INT
);
#else
ExtenderPCF8575 extender(
  &Native,
  GedeeldeBusComponent::DIGITAL_PINS_PCF8575,
  HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, 0
);
#endif

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }

  GA_SERIAL.println(F("=== ExtenderPCF8575 aanmelden/controleren-test ==="));

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
