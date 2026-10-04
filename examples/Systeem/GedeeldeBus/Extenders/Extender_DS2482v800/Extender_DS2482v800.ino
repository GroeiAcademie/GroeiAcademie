// ============================================================================
// Extender — DS2482v800
// ============================================================================
// Basistest voor ExtenderDS2482v800.
// Deze test valideert alleen de GedeeldeBus-stappen:
// aanmelden() -> controleren().
// inpluggen() en activeren() worden getest via een echte client/sensor.
// Configuratie gebeurt via UserConfig.h.
// ============================================================================

#include <Systeem/GedeeldeBus/GedeeldeBus.h>
#include <Configuratie/SystemConfig.h>

#if EXTENDER_DS2482_800_AANTAL == 0
  #error Zet EXTENDER_DS2482_800_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif

#if EXTENDER_DS2482_800_AANTAL == 1
ExtenderDS2482v800 extender(
  &Native,
  GedeeldeBusComponent::ONE_WIRE_DS2482v800,
  I2C_ADDRESS_EXTENDER_DS2482_800_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL
);
#else
ExtenderDS2482v800 extender(
  &Native,
  GedeeldeBusComponent::ONE_WIRE_DS2482v800,
  HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, 0
);
#endif

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }

  GA_SERIAL.println(F("=== ExtenderDS2482v800 aanmelden/controleren-test ==="));

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
