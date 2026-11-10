// ============================================================================
// Extender: SC16IS752 I2C
// ============================================================================
// Basistest voor ExtenderSC16IS72_I2C.
// Deze test valideert alleen de GedeeldeBus-stappen: aanmelden() -> controleren().
// inpluggen() en activeren() worden getest via een echte client/sensor.
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

#if EXTENDER_SC16IS752_I2C_AANTAL == 0
  #error Zet EXTENDER_SC16IS752_I2C_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif

#if EXTENDER_SC16IS752_I2C_AANTAL == 1
ExtenderSC16IS752I2C extender(
  &Native,
  GedeeldeBusComponent::UART_SC16IS752,
  I2C_ADDRESS_EXTENDER_SC16IS752_I2C_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, EXTENDER_SC16IS752_I2C_1_IRQ, EXTENDER_SC16IS752_I2C_1_RESET
);
#else
ExtenderSC16IS752I2C extender(
  &Native,
  GedeeldeBusComponent::UART_SC16IS752,
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
  GA_SERIAL.println(F("TEST: Extender_SC16IS752_I2C"));

  GA_SERIAL.println(F("=== ExtenderSC16IS752I2C aanmelden/controleren-test ==="));

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
