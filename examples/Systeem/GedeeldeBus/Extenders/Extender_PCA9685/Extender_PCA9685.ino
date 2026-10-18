// ============================================================================
// Extender: PCA9685
// ============================================================================
// Driver-/lifecycletest voor ExtenderPCA9685.
// Test: aanmelden() -> controleren() -> inpluggen() -> activeren() en daarna minimaal één echte driverfunctie.
// Configuratie gebeurt via UserConfig.h.
// ============================================================================

#include <GroeiAcademie.h>

#if EXTENDER_PCA9685_AANTAL == 0
  #error Zet EXTENDER_PCA9685_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif

#if EXTENDER_PCA9685_AANTAL == 1
ExtenderPCA9685 extender(&Native, GedeeldeBusComponent::PWM_PCA9685, I2C_ADDRESS_EXTENDER_PCA9685_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL);
#else
ExtenderPCA9685 extender(&Native, GedeeldeBusComponent::PWM_PCA9685, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, 0);
#endif

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  bool gelukt = extender.aanmelden() && extender.controleren() && extender.inpluggen() && extender.activeren();
  if (gelukt) gelukt = (extender.setPWM(0, 0, 0) == 0);
  GedeeldeBusPrintEnVerwijderTijdelijkeConflicten(&extender);
  GA_SERIAL.println(gelukt ? F("RESULTAAT: OK") : F("RESULTAAT: FOUT"));
}
// Runtime-herstel (D042) bij een eigen oproep van de library: zelf ComponentBeschikbaarTijdensRuntime() en
// ResultaatComponentToegangTijdensRuntimeVerwerken() oproepen. Dan herstelt de PCA9685 vanzelf na een los contact.
void loop() {
  if (!extender.ComponentBeschikbaarTijdensRuntime()) return;
  const bool gelukt = (extender.setPWM(0, 0, 2048) == 0);
  extender.ResultaatComponentToegangTijdensRuntimeVerwerken(gelukt);
  delay(1000);
}
