// ============================================================================
// ExtenderADS1115 — Test ExtenderPins Geweigerd
// ============================================================================
// Test één ExtenderPin direct buiten de momenteel aangeboden set.
// aanmelden() mag slagen; controleren() MOET weigeren met GB111.
// Er wordt geen hardware ingeplugd of geactiveerd.
// Configuratie gebeurt via UserConfig.h.
// ============================================================================

#include <Systeem/GedeeldeBus/GedeeldeBus.h>
#include <Configuratie/SystemConfig.h>

#if EXTENDER_ADS1115_AANTAL == 0
  #error Zet EXTENDER_ADS1115_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif

#if EXTENDER_ADS1115_AANTAL == 1
ExtenderADS1115 extender(
  &Native,
  GedeeldeBusComponent::ADC_ADS1115,
  I2C_ADDRESS_EXTENDER_ADS1115_1, HardwareResourcePin::SDA, HardwareResourcePin::SCL, EXTENDER_ADS1115_1_ALERT_RDY
);
#else
ExtenderADS1115 extender(
  &Native,
  GedeeldeBusComponent::ADC_ADS1115,
  HardwareResourcePin::SDA, HardwareResourcePin::SCL, 0
);
#endif

uint8_t ExtenderPins[] = { 0x04 };

GedeeldeBusNode SensorTest(
  &extender,
  { nullptr, 0, ExtenderPins, 1 },
  GedeeldeBusComponent::SENSOR,
  HardwareResourceToegang::GEDEELD
);

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }

  GA_SERIAL.println(F("=== ExtenderADS1115 ExtenderPins Geweigerd ==="));

  bool gelukt = extender.aanmelden();
  if (gelukt) gelukt = extender.controleren();
  GA_SERIAL.print(F("Extender aanmelden/controleren: "));
  GA_SERIAL.println(gelukt ? F("OK") : F("FOUT"));

  if (!gelukt) {
    GedeeldeBusPrintEnVerwijderTijdelijkeConflicten(&extender);
    GA_SERIAL.println(F("RESULTAAT: FOUT — extender zelf faalt vóór ExtenderPins-test"));
    return;
  }

  const bool aangemeld = SensorTest.aanmelden();
  GA_SERIAL.print(F("Sensor aanmelden: "));
  GA_SERIAL.println(aangemeld ? F("OK") : F("FOUT"));

  bool geweigerd = false;
  if (aangemeld) {
    const bool gecontroleerd = SensorTest.controleren();
    geweigerd = !gecontroleerd;
    GA_SERIAL.print(F("Ongeldige ExtenderPin geweigerd: "));
    GA_SERIAL.println(geweigerd ? F("OK") : F("FOUT"));
  }

  GedeeldeBusPrintEnVerwijderTijdelijkeConflicten(&SensorTest);
  GA_SERIAL.println((aangemeld && geweigerd) ? F("RESULTAAT: OK") : F("RESULTAAT: FOUT"));
}

void loop() {
}
