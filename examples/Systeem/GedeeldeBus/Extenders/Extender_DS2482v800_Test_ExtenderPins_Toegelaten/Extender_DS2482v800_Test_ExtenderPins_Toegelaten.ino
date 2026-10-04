// ============================================================================
// ExtenderDS2482v800 — Test ExtenderPins Toegelaten
// ============================================================================
// Test alle ExtenderPins die deze extender momenteel aanbiedt.
// De test gebruikt alleen aanmelden() en controleren(); er wordt geen hardware
// ingeplugd of geactiveerd.
// Configuratie gebeurt via UserConfig.h.
// ============================================================================

#include <Systeem/GedeeldeBus/GedeeldeBus.h>
#include <Configuratie/SystemConfig.h>

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

uint8_t ExtenderPins[] = {
  static_cast<uint8_t>(ExtenderDS2482v800::ExtenderPins::EP_IO0),
  static_cast<uint8_t>(ExtenderDS2482v800::ExtenderPins::EP_IO1),
  static_cast<uint8_t>(ExtenderDS2482v800::ExtenderPins::EP_IO2),
  static_cast<uint8_t>(ExtenderDS2482v800::ExtenderPins::EP_IO3),
  static_cast<uint8_t>(ExtenderDS2482v800::ExtenderPins::EP_IO4),
  static_cast<uint8_t>(ExtenderDS2482v800::ExtenderPins::EP_IO5),
  static_cast<uint8_t>(ExtenderDS2482v800::ExtenderPins::EP_IO6),
  static_cast<uint8_t>(ExtenderDS2482v800::ExtenderPins::EP_IO7)
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

  GA_SERIAL.println(F("=== ExtenderDS2482v800 ExtenderPins Toegelaten ==="));

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
