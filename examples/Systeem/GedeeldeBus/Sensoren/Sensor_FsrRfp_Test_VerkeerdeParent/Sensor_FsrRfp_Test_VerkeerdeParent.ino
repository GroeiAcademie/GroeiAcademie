// ============================================================================
// Sensor: FsrRfp, test verkeerde parent (T2-7)
// ============================================================================
// Contract: Activeren() van FsrRfp vraagt StartExtenderReadAnalog(pin, 0); een parent zonder ADC geeft -2.
// FsrRfp wordt hier op een PCF8574 ingeplugd in plaats van op een ADC-Extender.
// Verwacht: aanmelden mislukt (sensor = nullptr), er wordt niets gemeten. RESULTAAT: OK betekent: correct geweigerd.
// ============================================================================

#include <GroeiAcademie.h>

#if EXTENDER_PCF8574_AANTAL == 0
  #error Zet EXTENDER_PCF8574_AANTAL op minstens 1 voor deze test.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE voor deze test.
#endif

ExtenderPCF8574 extenderPCF8574(&Native, GedeeldeBusComponent::DIGITAL_PINS_PCF8574, I2C_ADDRESS_EXTENDER_PCF8574_1,
                                HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, EXTENDER_PCF8574_1_INT);

void ToonResultaat(const __FlashStringHelper* stap, bool gelukt) {
  GA_SERIAL.print(stap);
  GA_SERIAL.print(F(": "));
  GA_SERIAL.println(gelukt ? F("OK") : F("FOUT"));
}

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_FsrRfp_Test_VerkeerdeParent"));
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && (millis() - startTijd) < SERIAL_CONNECT_TIMEOUT_MS) { ; }

  GA_SERIAL.println(F("=== FsrRfp test verkeerde parent (T2-7) ==="));
  const bool extenderGelukt = extenderPCF8574.aanmelden() && extenderPCF8574.controleren() && extenderPCF8574.inpluggen() && extenderPCF8574.activeren();
  ToonResultaat(F("PCF8574 aanmelden/controleren/inpluggen/activeren"), extenderGelukt);
  GedeeldeBusPrintEnVerwijderTijdelijkeConflicten(&extenderPCF8574);

  sensorFsrRfpSet1 = GedeeldeBusNewComponent<struct FsrRfp>(&extenderPCF8574, GedeeldeBusComponent::FsrRfp, 0, 1, 2, 3,
      HardwareResourceToegang::GEDEELD, 1);
  ToonResultaat(F("FsrRfp op PCF8574 geweigerd (verwacht)"), sensorFsrRfpSet1 == nullptr);
  GA_SERIAL.println(sensorFsrRfpSet1 == nullptr ? F("RESULTAAT: OK") : F("RESULTAAT: FOUT"));
}

void loop() {
}
