// ============================================================================
// ExtenderADS7953: Test ExtenderPins Toegelaten
// ============================================================================
// Test alle ExtenderPins die deze extender momenteel aanbiedt.
// De test gebruikt alleen aanmelden() en controleren(); er wordt geen hardware ingeplugd of geactiveerd.
// Configuratie gebeurt via UserConfig.h.
// ============================================================================

#include <GroeiAcademie.h>

#if EXTENDER_ADS7953_AANTAL == 0
  #error Zet EXTENDER_ADS7953_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif

#if EXTENDER_ADS7953_AANTAL == 1
ExtenderADS7953 extender(
  &Native,
  GedeeldeBusComponent::ADC_ADS7953,
  CS_PIN_EXTENDER_ADS7953_1, HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI
);
#else
ExtenderADS7953 extender(
  &Native,
  GedeeldeBusComponent::ADC_ADS7953,
  HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, 0
);
#endif

uint8_t ExtenderPins[] = {
  static_cast<uint8_t>(ExtenderADS7953::ExtenderPins::EP_CH0),
  static_cast<uint8_t>(ExtenderADS7953::ExtenderPins::EP_CH1),
  static_cast<uint8_t>(ExtenderADS7953::ExtenderPins::EP_CH2),
  static_cast<uint8_t>(ExtenderADS7953::ExtenderPins::EP_CH3),
  static_cast<uint8_t>(ExtenderADS7953::ExtenderPins::EP_CH4),
  static_cast<uint8_t>(ExtenderADS7953::ExtenderPins::EP_CH5),
  static_cast<uint8_t>(ExtenderADS7953::ExtenderPins::EP_CH6),
  static_cast<uint8_t>(ExtenderADS7953::ExtenderPins::EP_CH7),
  static_cast<uint8_t>(ExtenderADS7953::ExtenderPins::EP_CH8),
  static_cast<uint8_t>(ExtenderADS7953::ExtenderPins::EP_CH9),
  static_cast<uint8_t>(ExtenderADS7953::ExtenderPins::EP_CH10),
  static_cast<uint8_t>(ExtenderADS7953::ExtenderPins::EP_CH11),
  static_cast<uint8_t>(ExtenderADS7953::ExtenderPins::EP_CH12),
  static_cast<uint8_t>(ExtenderADS7953::ExtenderPins::EP_CH13),
  static_cast<uint8_t>(ExtenderADS7953::ExtenderPins::EP_CH14),
  static_cast<uint8_t>(ExtenderADS7953::ExtenderPins::EP_CH15),
  static_cast<uint8_t>(ExtenderADS7953::ExtenderPins::EP_GPIO0),
  static_cast<uint8_t>(ExtenderADS7953::ExtenderPins::EP_GPIO1),
  static_cast<uint8_t>(ExtenderADS7953::ExtenderPins::EP_GPIO2),
  static_cast<uint8_t>(ExtenderADS7953::ExtenderPins::EP_GPIO3)
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

  GA_SERIAL.println(F("=== ExtenderADS7953 ExtenderPins Toegelaten ==="));

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
