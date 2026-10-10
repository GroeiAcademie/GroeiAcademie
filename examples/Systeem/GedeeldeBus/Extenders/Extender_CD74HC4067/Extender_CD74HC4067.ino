// ============================================================================
// Extender: CD74HC4067
// ============================================================================
// Driver-/lifecycletest voor ExtenderCD74HC4067.
// Test: aanmelden() -> controleren() -> inpluggen() -> activeren() en daarna minimaal één echte driverfunctie.
// Configuratie gebeurt via UserConfig.h.
// ============================================================================

#include <GroeiAcademie.h>

#if EXTENDER_CD74HC4067_AANTAL == 0
  #error Zet EXTENDER_CD74HC4067_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif

#if EXTENDER_CD74HC4067_AANTAL == 1
ExtenderCD74HC4067 extender(
  &Native,
  GedeeldeBusComponent::ADS_MULTIPLEXER_CD74HC4067,
  EXTENDER_CD74HC4067_1_S0, EXTENDER_CD74HC4067_1_S1, EXTENDER_CD74HC4067_1_S2, EXTENDER_CD74HC4067_1_S3, EXTENDER_CD74HC4067_1_EN, EXTENDER_CD74HC4067_1_Z
);
#else
ExtenderCD74HC4067 extender(
  &Native,
  GedeeldeBusComponent::ADS_MULTIPLEXER_CD74HC4067,
  0
);
#endif

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }

  GA_SERIAL.println(F("=== ExtenderCD74HC4067 driver-/lifecycletest ==="));

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
    extender.disable();
    extender.selectChannel(ExtenderCD74HC4067::ExtenderPins::EP_Y0);
    extender.enable();
    GA_SERIAL.println(F("disable/selectChannel(Y0)/enable: OK"));
  }

  GedeeldeBusPrintEnVerwijderTijdelijkeConflicten(&extender);
  GA_SERIAL.println(gelukt ? F("RESULTAAT: OK") : F("RESULTAAT: FOUT"));
}

void loop() {
}
