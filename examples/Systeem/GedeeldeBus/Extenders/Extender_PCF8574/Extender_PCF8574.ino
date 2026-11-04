// ============================================================================
// Extender: PCF8574
// ============================================================================
// Driver-/lifecycletest voor ExtenderPCF8574.
// Test: aanmelden() -> controleren() -> inpluggen() -> activeren() en daarna de publieke FrameWork-interface.
// Configuratie #define EXTENDER_PCF8574_AANTAL 1 of 2 gebeurt via UserConfig.h.
// Aansluiting gebruikte I2C-parent/Extender: SDA -> board-SDA, SCL -> board-SCL; GND -> board-GND.
// UNO R4 Minima/WiFi: fysieke SDA- en SCL-header (bij R4 Minima ook A4=SDA, A5=SCL); UNO Q: board-SDA/SCL.
// ESP32 D1 UNO R32: SDA=GPIO21, SCL=GPIO22 (bestaand FrameWork-boardprofiel).
// ESP32-S3 ARDI-32: SDA=GPIO38, SCL=GPIO39; DEV: SDA=GPIO8, SCL=GPIO9 (FrameWork-profiel).
// RP2040 Cytron Maker UNO: SDA=GPIO20, SCL=GPIO21 (ook Maker/Grove 6).
// STM32 F401RE Nucleo: SDA=PB9, SCL=PB8 (bestaand FrameWork-boardprofiel).
// Voeding VCC en GND volgens het concrete modulebord; controleer I2C-logicaniveau, pull-ups en eventuele levelshifting.
// ============================================================================

#include <GroeiAcademie.h>

#if EXTENDER_PCF8574_AANTAL == 0
  #error Zet EXTENDER_PCF8574_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif

#if EXTENDER_PCF8574_AANTAL == 1
ExtenderPCF8574 extender(
  &Native,
  GedeeldeBusComponent::DIGITAL_PINS_PCF8574,
  I2C_ADDRESS_EXTENDER_PCF8574_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, EXTENDER_PCF8574_1_INT
);
#else
ExtenderPCF8574 extender(
  &Native,
  GedeeldeBusComponent::DIGITAL_PINS_PCF8574,
  HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, 0
);
#endif

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Extender_PCF8574"));

  GA_SERIAL.println(F("=== ExtenderPCF8574 driver-/lifecycletest ==="));

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
    extender.write8(0xFF);
    const uint8_t waarde = extender.read8();
    GA_SERIAL.print(F("write8/read8: 0x"));
    GA_SERIAL.println(waarde, HEX);

    extender.pinMode(ExtenderPCF8574::ExtenderPins::EP_P0, INPUT);
    extender.write(static_cast<uint8_t>(ExtenderPCF8574::ExtenderPins::EP_P0), HIGH);
    const uint8_t pinWaarde = extender.read(static_cast<uint8_t>(ExtenderPCF8574::ExtenderPins::EP_P0));
    GA_SERIAL.print(F("pinMode/write/read P0: "));
    GA_SERIAL.println(pinWaarde);

    gelukt = extender.lastError() == 0;
  }

  GedeeldeBusPrintEnVerwijderTijdelijkeConflicten(&extender);
  GA_SERIAL.println(gelukt ? F("RESULTAAT: OK") : F("RESULTAAT: FOUT"));
}

void loop() {
}
