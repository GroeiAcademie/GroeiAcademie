// ============================================================================
// ExtenderSC16IS752SPI — Test ExtenderPins Toegelaten
// ============================================================================
// Test alle ExtenderPins die deze extender momenteel aanbiedt.
// De test gebruikt alleen aanmelden() en controleren(); er wordt geen hardware
// ingeplugd of geactiveerd.
// Configuratie gebeurt via UserConfig.h.
// ============================================================================

#include <Systeem/GedeeldeBus/GedeeldeBus.h>
#include <Configuratie/SystemConfig.h>

#if EXTENDER_SC16IS752_SPI_AANTAL == 0
  #error Zet EXTENDER_SC16IS752_SPI_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif

#if EXTENDER_SC16IS752_SPI_AANTAL == 1
ExtenderSC16IS752SPI extender(
  &Native,
  GedeeldeBusComponent::UART_SC16IS752,
  CS_PIN_EXTENDER_SC16IS752_SPI_1, HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, EXTENDER_SC16IS752_SPI_1_IRQ, EXTENDER_SC16IS752_SPI_1_RESET
);
#else
ExtenderSC16IS752SPI extender(
  &Native,
  GedeeldeBusComponent::UART_SC16IS752,
  HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, 0
);
#endif

uint8_t ExtenderPins[] = {
  static_cast<uint8_t>(ExtenderSC16IS752SPI::ExtenderPins::EP_CHANNEL_A),
  static_cast<uint8_t>(ExtenderSC16IS752SPI::ExtenderPins::EP_CHANNEL_B),
  static_cast<uint8_t>(ExtenderSC16IS752SPI::ExtenderPins::EP_GPIO0),
  static_cast<uint8_t>(ExtenderSC16IS752SPI::ExtenderPins::EP_GPIO1),
  static_cast<uint8_t>(ExtenderSC16IS752SPI::ExtenderPins::EP_GPIO2),
  static_cast<uint8_t>(ExtenderSC16IS752SPI::ExtenderPins::EP_GPIO3),
  static_cast<uint8_t>(ExtenderSC16IS752SPI::ExtenderPins::EP_GPIO4),
  static_cast<uint8_t>(ExtenderSC16IS752SPI::ExtenderPins::EP_GPIO5),
  static_cast<uint8_t>(ExtenderSC16IS752SPI::ExtenderPins::EP_GPIO6),
  static_cast<uint8_t>(ExtenderSC16IS752SPI::ExtenderPins::EP_GPIO7)
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

  GA_SERIAL.println(F("=== ExtenderSC16IS752SPI ExtenderPins Toegelaten ==="));

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
