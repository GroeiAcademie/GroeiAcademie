// ============================================================================
// Extender: DS2482v800
// ============================================================================
// Driver-/lifecycletest voor ExtenderDS2482v800.
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
  #if defined(DEBUG)
  while (!GA_SERIAL) { ; }
  #else
  unsigned long startTijd = millis();
  while (!GA_SERIAL && (millis() - startTijd) < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  #endif
  GA_SERIAL.println(F("TEST: Extender_DS2482v800"));

  GA_SERIAL.println(F("=== ExtenderDS2482v800 driver-/lifecycletest ==="));

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
    gelukt = extender.selectChannel(0);
    GA_SERIAL.print(F("selectChannel(0): "));
    GA_SERIAL.println(gelukt ? F("OK") : F("FOUT"));
  }

  if (gelukt) {
    const bool oneWireAanwezig = extender.OneWireReset();
    GA_SERIAL.print(F("OneWireReset: "));
    GA_SERIAL.println(oneWireAanwezig ? F("DEVICE") : F("GEEN DEVICE"));
  }

  GedeeldeBusPrintEnVerwijderTijdelijkeConflicten(&extender);
  GA_SERIAL.println(gelukt ? F("RESULTAAT: OK") : F("RESULTAAT: FOUT"));
}

void loop() {
}
