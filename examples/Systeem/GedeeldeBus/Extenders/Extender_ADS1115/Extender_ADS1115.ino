// ============================================================================
// Extender: ADS1115
// ============================================================================
// Driver-/lifecycletest voor ExtenderADS1115.
// Test: aanmelden() -> controleren() -> inpluggen() -> activeren() en daarna de publieke FrameWork-interface.
// Configuratie #define EXTENDER_ADS1115_AANTAL 1 of 2 gebeurt via UserConfig.h.
// ============================================================================

#include <GroeiAcademie.h>

#if EXTENDER_ADS1115_AANTAL == 0
  #error Zet EXTENDER_ADS1115_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif

#if EXTENDER_ADS1115_AANTAL == 1
ExtenderADS1115 extender(
  &Native,
  GedeeldeBusComponent::ADC_ADS1115,
  I2C_ADDRESS_EXTENDER_ADS1115_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, EXTENDER_ADS1115_1_ALERT_RDY
);
#else
ExtenderADS1115 extender(
  &Native,
  GedeeldeBusComponent::ADC_ADS1115,
  HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, 0
);
#endif

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }

  GA_SERIAL.println(F("=== ExtenderADS1115 driver-/lifecycletest ==="));

  bool gelukt = extender.aanmelden();
  GA_SERIAL.print(F("aanmelden: "));
  GA_SERIAL.println(gelukt ? F("OK") : F("FOUT"));

  if (gelukt) {
    gelukt = extender.controleren();
    GA_SERIAL.print(F("controleren: "));
    GA_SERIAL.println(gelukt ? F("OK") : F("FOUT"));
  }

  if (gelukt) {
    gelukt = extender.inpluggen();
    GA_SERIAL.print(F("inpluggen: "));
    GA_SERIAL.println(gelukt ? F("OK") : F("FOUT"));
  }

  if (gelukt) {
    gelukt = extender.activeren();
    GA_SERIAL.print(F("activeren: "));
    GA_SERIAL.println(gelukt ? F("OK") : F("FOUT"));
  }

  if (gelukt) {
    extender.setGain(ADS1X15_GAIN_6144MV);

    for (uint8_t sensorPin = 0; sensorPin < 4; sensorPin++) {
      const int16_t waarde = extender.readADC(sensorPin);
      GA_SERIAL.print(F("readADC AIN"));
      GA_SERIAL.print(sensorPin);
      GA_SERIAL.print(F(": "));
      GA_SERIAL.print(waarde);
      GA_SERIAL.print(F(" = "));
      GA_SERIAL.print(extender.toVoltage(waarde), 6);
      GA_SERIAL.println(F(" V"));
    }

    extender.requestADC(0);
    while (!extender.isReady()) { ; }
    const int16_t waarde = extender.getValue();
    GA_SERIAL.print(F("requestADC/isReady/getValue AIN0: "));
    GA_SERIAL.println(waarde);
  }

  GedeeldeBusPrintEnVerwijderTijdelijkeConflicten(&extender);
  GA_SERIAL.println(gelukt ? F("RESULTAAT: OK") : F("RESULTAAT: FOUT"));
}

void loop() {
}
