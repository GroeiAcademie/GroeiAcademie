// ============================================================================
// ExtenderPCF8574 — Test ExtenderPins Toegelaten
// ============================================================================
// Test alle ExtenderPins die deze extender momenteel aanbiedt.
// De test gebruikt alleen aanmelden() en controleren(); er wordt geen hardware
// ingeplugd of geactiveerd.
// Configuratie gebeurt via UserConfig.h.
// ============================================================================

#include <Systeem/GedeeldeBus/GedeeldeBus.h>
#include <Configuratie/SystemConfig.h>

#if EXTENDER_PCF8574_AANTAL == 0
  #error Zet EXTENDER_PCF8574_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif

#if EXTENDER_PCF8574_AANTAL == 1
ExtenderPCF8574 extender(
  &Native,
  GedeeldeBusComponent::DIGITAL_PINS_PCF8574,
  I2C_ADDRESS_EXTENDER_PCF8574_1, HardwareResourcePin::SDA, HardwareResourcePin::SCL, EXTENDER_PCF8574_1_INT
);
#else
ExtenderPCF8574 extender(
  &Native,
  GedeeldeBusComponent::DIGITAL_PINS_PCF8574,
  HardwareResourcePin::SDA, HardwareResourcePin::SCL, 0
);
#endif

uint8_t ExtenderPins[] = {
  static_cast<uint8_t>(ExtenderPCF8574::ExtenderPins::EP_P0),
  static_cast<uint8_t>(ExtenderPCF8574::ExtenderPins::EP_P1),
  static_cast<uint8_t>(ExtenderPCF8574::ExtenderPins::EP_P2),
  static_cast<uint8_t>(ExtenderPCF8574::ExtenderPins::EP_P3),
  static_cast<uint8_t>(ExtenderPCF8574::ExtenderPins::EP_P4),
  static_cast<uint8_t>(ExtenderPCF8574::ExtenderPins::EP_P5),
  static_cast<uint8_t>(ExtenderPCF8574::ExtenderPins::EP_P6),
  static_cast<uint8_t>(ExtenderPCF8574::ExtenderPins::EP_P7)
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

  GA_SERIAL.println(F("=== ExtenderPCF8574 ExtenderPins Toegelaten ==="));

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
