// ============================================================================
// Extender: PCA9685
// ============================================================================
// Driver-/lifecycletest voor ExtenderPCA9685.
// Test: aanmelden() -> controleren() -> inpluggen() -> activeren() en daarna minimaal één echte driverfunctie.
// Configuratie gebeurt via UserConfig.h.
// Aansluiting gebruikte I2C-parent/Extender: SDA -> board-SDA, SCL -> board-SCL; GND -> board-GND.
// UNO R4 Minima/WiFi: fysieke SDA- en SCL-header (bij R4 Minima ook A4=SDA, A5=SCL); UNO Q: board-SDA/SCL.
// ESP32 D1 UNO R32: SDA=GPIO21, SCL=GPIO22 (bestaand FrameWork-boardprofiel).
// ESP32-S3 ARDI-32: SDA=GPIO38, SCL=GPIO39; DEV: SDA=GPIO8, SCL=GPIO9 (FrameWork-profiel).
// RP2040 Cytron Maker UNO: SDA=GPIO20, SCL=GPIO21 (ook Maker/Grove 6).
// STM32 F401RE Nucleo: SDA=PB9, SCL=PB8 (bestaand FrameWork-boardprofiel).
// Voeding VCC en GND volgens het concrete modulebord; controleer I2C-logicaniveau, pull-ups en eventuele levelshifting.
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
  #if defined(DEBUG)
  while (!GA_SERIAL) { ; }
  #else
  unsigned long startTijd = millis();
  while (!GA_SERIAL && (millis() - startTijd) < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  #endif
  GA_SERIAL.println(F("TEST: Extender_PCA9685"));
  bool gelukt = extender.aanmelden() && extender.controleren() && extender.inpluggen() && extender.activeren();
  if (gelukt) gelukt = (extender.setPWM(0, 0, 0) == 0);
  GedeeldeBusPrintEnVerwijderTijdelijkeConflicten(&extender);
  GA_SERIAL.println(gelukt ? F("RESULTAAT: OK") : F("RESULTAAT: FOUT"));
}
// Runtime-herstel bij een eigen oproep van de library: zelf ComponentBeschikbaarTijdensRuntime() en
// ResultaatComponentToegangTijdensRuntimeVerwerken() oproepen. Dan herstelt de PCA9685 vanzelf na een los contact.
void loop() {
  if (!extender.ComponentBeschikbaarTijdensRuntime()) return;
  const bool gelukt = (extender.setPWM(0, 0, 2048) == 0);
  extender.ResultaatComponentToegangTijdensRuntimeVerwerken(gelukt);
  delay(1000);
}
