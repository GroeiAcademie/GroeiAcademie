// ============================================================================
// Sensor: RFP602, test verkeerde parent (T2-7)
// ============================================================================
// Contract: Activeren() van RFP602 vraagt StartExtenderReadAnalog(pin, 0); een parent zonder ADC geeft -2.
// RFP602 wordt hier op een PCF8574 ingeplugd in plaats van op een ADC-Extender.
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
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && (millis() - startTijd) < SERIAL_CONNECT_TIMEOUT_MS) { ; }

  GA_SERIAL.println(F("=== RFP602 test verkeerde parent (T2-7) ==="));
  const bool extenderGelukt = extenderPCF8574.aanmelden() && extenderPCF8574.controleren() && extenderPCF8574.inpluggen() && extenderPCF8574.activeren();
  ToonResultaat(F("PCF8574 aanmelden/controleren/inpluggen/activeren"), extenderGelukt);
  GedeeldeBusPrintEnVerwijderTijdelijkeConflicten(&extenderPCF8574);

  sensorRFP602set1 = GedeeldeBusNewComponent<struct RFP602>(&extenderPCF8574, GedeeldeBusComponent::RFP602, 0, 1, 2, 3,
      HardwareResourceToegang::GEDEELD, 1);
  ToonResultaat(F("RFP602 op PCF8574 geweigerd (verwacht)"), sensorRFP602set1 == nullptr);
  GA_SERIAL.println(sensorRFP602set1 == nullptr ? F("RESULTAAT: OK") : F("RESULTAAT: FOUT"));
}

void loop() {
}
