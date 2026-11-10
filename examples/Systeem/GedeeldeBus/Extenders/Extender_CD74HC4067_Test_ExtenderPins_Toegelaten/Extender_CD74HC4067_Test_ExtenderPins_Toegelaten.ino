// ============================================================================
// ExtenderCD74HC4067: Test ExtenderPins Toegelaten
// ============================================================================
// Test alle ExtenderPins die deze extender momenteel aanbiedt.
// De test gebruikt alleen aanmelden() en controleren(); er wordt geen hardware ingeplugd of geactiveerd.
// Configuratie gebeurt via UserConfig.h.
// ============================================================================

#include <GroeiAcademie.h>

#if EXTENDER_CD74HC4067_AANTAL == 0
  #error Zet EXTENDER_CD74HC4067_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif

#if EXTENDER_CD74HC4067_AANTAL == 1
ExtenderCD74HC4067 extender(
  &Native,
  GedeeldeBusComponent::ADS_MULTIPLEXER_CD74HC4067,
  EXTENDER_CD74HC4067_1_S0, EXTENDER_CD74HC4067_1_S1, EXTENDER_CD74HC4067_1_S2, EXTENDER_CD74HC4067_1_S3, EXTENDER_CD74HC4067_1_EN, EXTENDER_CD74HC4067_1_Z
);
#else
ExtenderCD74HC4067 extender(
  &Native,
  GedeeldeBusComponent::ADS_MULTIPLEXER_CD74HC4067,
  0
);
#endif

uint8_t ExtenderPins[] = {
  static_cast<uint8_t>(ExtenderCD74HC4067::ExtenderPins::EP_Y0),
  static_cast<uint8_t>(ExtenderCD74HC4067::ExtenderPins::EP_Y1),
  static_cast<uint8_t>(ExtenderCD74HC4067::ExtenderPins::EP_Y2),
  static_cast<uint8_t>(ExtenderCD74HC4067::ExtenderPins::EP_Y3),
  static_cast<uint8_t>(ExtenderCD74HC4067::ExtenderPins::EP_Y4),
  static_cast<uint8_t>(ExtenderCD74HC4067::ExtenderPins::EP_Y5),
  static_cast<uint8_t>(ExtenderCD74HC4067::ExtenderPins::EP_Y6),
  static_cast<uint8_t>(ExtenderCD74HC4067::ExtenderPins::EP_Y7),
  static_cast<uint8_t>(ExtenderCD74HC4067::ExtenderPins::EP_Y8),
  static_cast<uint8_t>(ExtenderCD74HC4067::ExtenderPins::EP_Y9),
  static_cast<uint8_t>(ExtenderCD74HC4067::ExtenderPins::EP_Y10),
  static_cast<uint8_t>(ExtenderCD74HC4067::ExtenderPins::EP_Y11),
  static_cast<uint8_t>(ExtenderCD74HC4067::ExtenderPins::EP_Y12),
  static_cast<uint8_t>(ExtenderCD74HC4067::ExtenderPins::EP_Y13),
  static_cast<uint8_t>(ExtenderCD74HC4067::ExtenderPins::EP_Y14),
  static_cast<uint8_t>(ExtenderCD74HC4067::ExtenderPins::EP_Y15)
};

GedeeldeBusNode SensorTest(
  &extender,
  { nullptr, 0, ExtenderPins, sizeof(ExtenderPins) / sizeof(ExtenderPins[0]) },
  GedeeldeBusComponent::GC_SENSOR,
  HardwareResourceToegang::GEDEELD
);

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  #if defined(DEBUG)
  while (!GA_SERIAL) { ; }
  #else
  unsigned long startTijd = millis();
  while (!GA_SERIAL && (millis() - startTijd) < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  #endif
  GA_SERIAL.println(F("TEST: Extender_CD74HC4067_Test_ExtenderPins_Toegelaten"));

  GA_SERIAL.println(F("=== ExtenderCD74HC4067 ExtenderPins Toegelaten ==="));

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
