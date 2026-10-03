// ============================================================================
// ExtenderSC16IS752I2C — Test ExtenderPins Toegelaten
// ============================================================================
// Test alle ExtenderPins die deze extender momenteel aanbiedt.
// De test gebruikt alleen aanmelden() en controleren(); er wordt geen hardware
// ingeplugd of geactiveerd.
// Configuratie gebeurt via UserConfig.h.
// ============================================================================

#include <Systeem/GedeeldeBus/GedeeldeBus.h>
#include <Configuratie/SystemConfig.h>

#if EXTENDER_SC16IS752_I2C_AANTAL == 0
  #error Zet EXTENDER_SC16IS752_I2C_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif

#if EXTENDER_SC16IS752_I2C_AANTAL == 1
ExtenderSC16IS752I2C extender(
  &Native,
  GedeeldeBusComponent::UART_SC16IS752,
  I2C_ADDRESS_EXTENDER_SC16IS752_I2C_1, HardwareResourcePin::SDA, HardwareResourcePin::SCL, EXTENDER_SC16IS752_I2C_1_IRQ, EXTENDER_SC16IS752_I2C_1_RESET
);
#else
ExtenderSC16IS752I2C extender(
  &Native,
  GedeeldeBusComponent::UART_SC16IS752,
  HardwareResourcePin::SDA, HardwareResourcePin::SCL, 0
);
#endif

uint8_t ExtenderPins[] = {
  static_cast<uint8_t>(ExtenderSC16IS752I2C::ExtenderPins::EP_CHANNEL_A),
  static_cast<uint8_t>(ExtenderSC16IS752I2C::ExtenderPins::EP_CHANNEL_B),
  static_cast<uint8_t>(ExtenderSC16IS752I2C::ExtenderPins::EP_GPIO0),
  static_cast<uint8_t>(ExtenderSC16IS752I2C::ExtenderPins::EP_GPIO1),
  static_cast<uint8_t>(ExtenderSC16IS752I2C::ExtenderPins::EP_GPIO2),
  static_cast<uint8_t>(ExtenderSC16IS752I2C::ExtenderPins::EP_GPIO3),
  static_cast<uint8_t>(ExtenderSC16IS752I2C::ExtenderPins::EP_GPIO4),
  static_cast<uint8_t>(ExtenderSC16IS752I2C::ExtenderPins::EP_GPIO5),
  static_cast<uint8_t>(ExtenderSC16IS752I2C::ExtenderPins::EP_GPIO6),
  static_cast<uint8_t>(ExtenderSC16IS752I2C::ExtenderPins::EP_GPIO7)
};

GedeeldeBusNode SensorTest(
  &extender,
  { nullptr, 0, ExtenderPins, sizeof(ExtenderPins) / sizeof(ExtenderPins[0]) },
  GedeeldeBusComponent::SENSOR,
  HardwareResourceToegang::GEDEELD
);

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }

  GA_SERIAL.println(F("=== ExtenderSC16IS752I2C ExtenderPins Toegelaten ==="));

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
