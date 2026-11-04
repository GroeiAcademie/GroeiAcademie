// ============================================================================
// ExtenderMAX14830I2C: Test ExtenderPins Toegelaten
// ============================================================================
// Test alle ExtenderPins die deze extender momenteel aanbiedt.
// De test gebruikt alleen aanmelden() en controleren(); er wordt geen hardware ingeplugd of geactiveerd.
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

#if EXTENDER_MAX14830_I2C_AANTAL == 0
  #error Zet EXTENDER_MAX14830_I2C_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif

#if EXTENDER_MAX14830_I2C_AANTAL == 1
ExtenderMAX14830I2C extender(
  &Native,
  GedeeldeBusComponent::UART_MAX14830,
  I2C_ADDRESS_EXTENDER_MAX14830_I2C_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, EXTENDER_MAX14830_I2C_1_IRQ
);
#else
ExtenderMAX14830I2C extender(
  &Native,
  GedeeldeBusComponent::UART_MAX14830,
  HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, 0
);
#endif

uint8_t ExtenderPins[] = {
  static_cast<uint8_t>(ExtenderMAX14830I2C::ExtenderPins::EP_UART0),
  static_cast<uint8_t>(ExtenderMAX14830I2C::ExtenderPins::EP_UART1),
  static_cast<uint8_t>(ExtenderMAX14830I2C::ExtenderPins::EP_UART2),
  static_cast<uint8_t>(ExtenderMAX14830I2C::ExtenderPins::EP_UART3),
  static_cast<uint8_t>(ExtenderMAX14830I2C::ExtenderPins::EP_GPIO0),
  static_cast<uint8_t>(ExtenderMAX14830I2C::ExtenderPins::EP_GPIO1),
  static_cast<uint8_t>(ExtenderMAX14830I2C::ExtenderPins::EP_GPIO2),
  static_cast<uint8_t>(ExtenderMAX14830I2C::ExtenderPins::EP_GPIO3),
  static_cast<uint8_t>(ExtenderMAX14830I2C::ExtenderPins::EP_GPIO4),
  static_cast<uint8_t>(ExtenderMAX14830I2C::ExtenderPins::EP_GPIO5),
  static_cast<uint8_t>(ExtenderMAX14830I2C::ExtenderPins::EP_GPIO6),
  static_cast<uint8_t>(ExtenderMAX14830I2C::ExtenderPins::EP_GPIO7),
  static_cast<uint8_t>(ExtenderMAX14830I2C::ExtenderPins::EP_GPIO8),
  static_cast<uint8_t>(ExtenderMAX14830I2C::ExtenderPins::EP_GPIO9),
  static_cast<uint8_t>(ExtenderMAX14830I2C::ExtenderPins::EP_GPIO10),
  static_cast<uint8_t>(ExtenderMAX14830I2C::ExtenderPins::EP_GPIO11),
  static_cast<uint8_t>(ExtenderMAX14830I2C::ExtenderPins::EP_GPIO12),
  static_cast<uint8_t>(ExtenderMAX14830I2C::ExtenderPins::EP_GPIO13),
  static_cast<uint8_t>(ExtenderMAX14830I2C::ExtenderPins::EP_GPIO14),
  static_cast<uint8_t>(ExtenderMAX14830I2C::ExtenderPins::EP_GPIO15)
};

GedeeldeBusNode SensorTest(
  &extender,
  { nullptr, 0, ExtenderPins, sizeof(ExtenderPins) / sizeof(ExtenderPins[0]) },
  GedeeldeBusComponent::GC_SENSOR,
  HardwareResourceToegang::GEDEELD
);

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Extender_MAX14830_I2C_Test_ExtenderPins_Toegelaten"));

  GA_SERIAL.println(F("=== ExtenderMAX14830I2C ExtenderPins Toegelaten ==="));

  bool gelukt = extender.aanmelden();
  if (gelukt) gelukt = extender.controleren();
  GA_SERIAL.print(F("Extender aanmelden/controleren: "));
  GA_SERIAL.println(gelukt ? F("OK") : F("FOUT"));

  if (gelukt) {
    gelukt = SensorTest.aanmelden();
    GA_SERIAL.print(F("Sensor aanmelden: "));
    GA_SERIAL.println(gelukt ? F("OK") : F("FOUT"));
  }

  if (gelukt) {
    gelukt = SensorTest.controleren();
    GA_SERIAL.print(F("Alle ExtenderPins toegelaten: "));
    GA_SERIAL.println(gelukt ? F("OK") : F("FOUT"));
  }

  GedeeldeBusPrintEnVerwijderTijdelijkeConflicten(&SensorTest);
  GA_SERIAL.println(gelukt ? F("RESULTAAT: OK") : F("RESULTAAT: FOUT"));
}

void loop() {
}
