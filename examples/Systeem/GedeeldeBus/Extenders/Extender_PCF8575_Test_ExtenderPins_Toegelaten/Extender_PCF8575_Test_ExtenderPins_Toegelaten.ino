// ============================================================================
// ExtenderPCF8575: Test ExtenderPins Toegelaten
// ============================================================================
// Test alle ExtenderPins die deze extender momenteel aanbiedt.
// De test gebruikt alleen aanmelden() en controleren(); er wordt geen hardware ingeplugd of geactiveerd.
// Configuratie gebeurt via UserConfig.h.
// ============================================================================

#include <GroeiAcademie.h>

#if EXTENDER_PCF8575_AANTAL == 0
  #error Zet EXTENDER_PCF8575_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif

#if EXTENDER_PCF8575_AANTAL == 1
ExtenderPCF8575 extender(
  &Native,
  GedeeldeBusComponent::DIGITAL_PINS_PCF8575,
  I2C_ADDRESS_EXTENDER_PCF8575_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, EXTENDER_PCF8575_1_INT
);
#else
ExtenderPCF8575 extender(
  &Native,
  GedeeldeBusComponent::DIGITAL_PINS_PCF8575,
  HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, 0
);
#endif

uint8_t ExtenderPins[] = {
  static_cast<uint8_t>(ExtenderPCF8575::ExtenderPins::EP_P00),
  static_cast<uint8_t>(ExtenderPCF8575::ExtenderPins::EP_P01),
  static_cast<uint8_t>(ExtenderPCF8575::ExtenderPins::EP_P02),
  static_cast<uint8_t>(ExtenderPCF8575::ExtenderPins::EP_P03),
  static_cast<uint8_t>(ExtenderPCF8575::ExtenderPins::EP_P04),
  static_cast<uint8_t>(ExtenderPCF8575::ExtenderPins::EP_P05),
  static_cast<uint8_t>(ExtenderPCF8575::ExtenderPins::EP_P06),
  static_cast<uint8_t>(ExtenderPCF8575::ExtenderPins::EP_P07),
  static_cast<uint8_t>(ExtenderPCF8575::ExtenderPins::EP_P10),
  static_cast<uint8_t>(ExtenderPCF8575::ExtenderPins::EP_P11),
  static_cast<uint8_t>(ExtenderPCF8575::ExtenderPins::EP_P12),
  static_cast<uint8_t>(ExtenderPCF8575::ExtenderPins::EP_P13),
  static_cast<uint8_t>(ExtenderPCF8575::ExtenderPins::EP_P14),
  static_cast<uint8_t>(ExtenderPCF8575::ExtenderPins::EP_P15),
  static_cast<uint8_t>(ExtenderPCF8575::ExtenderPins::EP_P16),
  static_cast<uint8_t>(ExtenderPCF8575::ExtenderPins::EP_P17)
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

  GA_SERIAL.println(F("=== ExtenderPCF8575 ExtenderPins Toegelaten ==="));

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
