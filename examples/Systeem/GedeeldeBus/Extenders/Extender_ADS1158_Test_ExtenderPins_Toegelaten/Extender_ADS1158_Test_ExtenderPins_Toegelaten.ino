// ============================================================================
// ExtenderADS1158: Test ExtenderPins Toegelaten
// ============================================================================
// Test alle ExtenderPins die deze extender momenteel aanbiedt.
// De test gebruikt alleen aanmelden() en controleren(); er wordt geen hardware ingeplugd of geactiveerd.
// Configuratie gebeurt via UserConfig.h.
// SPI-parent/Extender: DIN/SDI -> MOSI, DOUT/SDO -> MISO, CLK/SCK -> SCK; CS volgens de bestaande constructor/configuratie.
// UNO R4 Minima/WiFi: MOSI=D11 (COPI), MISO=D12 (CIPO), SCK=D13; CS via de geconfigureerde pin.
// ESP32 D1 UNO R32: MOSI=GPIO23, MISO=GPIO19, SCK=GPIO18 (bestaand FrameWork-profiel).
// ESP32-S3 ARDI-32/DEV: MOSI=GPIO11, MISO=GPIO13, SCK=GPIO12 (FrameWork-profiel).
// RP2040 Cytron Maker UNO: MOSI=GPIO11, MISO=GPIO12, SCK=GPIO10.
// STM32 F401RE Nucleo: MOSI=PA7, MISO=PA6, SCK=PA5 (FrameWork-profiel).
// Module-VCC en GND volgens het gebruikte bord; SPI-niveaus en eventuele levelshifting controleren.
// CS-keuze in deze sketch: EXTENDER_ADS1158_1_CS_PIN.
// ============================================================================

#include <GroeiAcademie.h>

#if EXTENDER_ADS1158_AANTAL == 0
  #error Zet EXTENDER_ADS1158_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif

#if EXTENDER_ADS1158_AANTAL == 1
ExtenderADS1158 extender(
  &Native,
  GedeeldeBusComponent::ADC_ADS1158,
  EXTENDER_ADS1158_1_CS_PIN, HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, EXTENDER_ADS1158_1_START, EXTENDER_ADS1158_1_RESET, EXTENDER_ADS1158_1_PWDN
);
#else
ExtenderADS1158 extender(
  &Native,
  GedeeldeBusComponent::ADC_ADS1158,
  HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, 0
);
#endif

uint8_t ExtenderPins[] = {
  static_cast<uint8_t>(ExtenderADS1158::ExtenderPins::EP_AIN0),
  static_cast<uint8_t>(ExtenderADS1158::ExtenderPins::EP_AIN1),
  static_cast<uint8_t>(ExtenderADS1158::ExtenderPins::EP_AIN2),
  static_cast<uint8_t>(ExtenderADS1158::ExtenderPins::EP_AIN3),
  static_cast<uint8_t>(ExtenderADS1158::ExtenderPins::EP_AIN4),
  static_cast<uint8_t>(ExtenderADS1158::ExtenderPins::EP_AIN5),
  static_cast<uint8_t>(ExtenderADS1158::ExtenderPins::EP_AIN6),
  static_cast<uint8_t>(ExtenderADS1158::ExtenderPins::EP_AIN7),
  static_cast<uint8_t>(ExtenderADS1158::ExtenderPins::EP_AIN8),
  static_cast<uint8_t>(ExtenderADS1158::ExtenderPins::EP_AIN9),
  static_cast<uint8_t>(ExtenderADS1158::ExtenderPins::EP_AIN10),
  static_cast<uint8_t>(ExtenderADS1158::ExtenderPins::EP_AIN11),
  static_cast<uint8_t>(ExtenderADS1158::ExtenderPins::EP_AIN12),
  static_cast<uint8_t>(ExtenderADS1158::ExtenderPins::EP_AIN13),
  static_cast<uint8_t>(ExtenderADS1158::ExtenderPins::EP_AIN14),
  static_cast<uint8_t>(ExtenderADS1158::ExtenderPins::EP_AIN15),
  static_cast<uint8_t>(ExtenderADS1158::ExtenderPins::EP_GPIO0),
  static_cast<uint8_t>(ExtenderADS1158::ExtenderPins::EP_GPIO1),
  static_cast<uint8_t>(ExtenderADS1158::ExtenderPins::EP_GPIO2),
  static_cast<uint8_t>(ExtenderADS1158::ExtenderPins::EP_GPIO3),
  static_cast<uint8_t>(ExtenderADS1158::ExtenderPins::EP_GPIO4),
  static_cast<uint8_t>(ExtenderADS1158::ExtenderPins::EP_GPIO5),
  static_cast<uint8_t>(ExtenderADS1158::ExtenderPins::EP_GPIO6),
  static_cast<uint8_t>(ExtenderADS1158::ExtenderPins::EP_GPIO7)
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
  GA_SERIAL.println(F("TEST: Extender_ADS1158_Test_ExtenderPins_Toegelaten"));

  GA_SERIAL.println(F("=== ExtenderADS1158 ExtenderPins Toegelaten ==="));

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
