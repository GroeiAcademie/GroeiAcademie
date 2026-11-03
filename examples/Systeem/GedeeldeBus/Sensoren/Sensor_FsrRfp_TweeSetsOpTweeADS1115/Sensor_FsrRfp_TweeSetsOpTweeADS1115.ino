// ============================================================================
// Sensor: FsrRfp, twee sets op twee ADS1115's
// ============================================================================
// Lifecycletest voor twee FsrRfp-sets zonder Data-class (1b).
// Set 1 meet via ADS1115 #1, set 2 via ADS1115 #2: elke set heeft een eigen ADS1115 (G2).
// Nodig in je configuratie (of als compileervlag):
//   EXTENDER_ADS1115_AANTAL 2, I2C_ADDRESS_EXTENDER_ADS1115_2 (bv. 0x49)
//   FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_2 (2 of 4)
// ============================================================================

#include <GroeiAcademie.h>

#if EXTENDER_ADS1115_AANTAL < 2
  #error Zet EXTENDER_ADS1115_AANTAL op 2: elke FsrRfp-set meet via een eigen ADS1115.
#endif

#if !defined(FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_2)
  #error Zet FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_2 op 2 of 4 in ConfigFsrRfp.h.
#endif

#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE voor deze test.
#endif

ExtenderADS1115 extenderADS1115set1(
  &Native, GedeeldeBusComponent::ADC_ADS1115,
  I2C_ADDRESS_EXTENDER_ADS1115_1,
  HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL,
  EXTENDER_ADS1115_1_ALERT_RDY,
  HardwareResourceToegang::GEDEELD
);

ExtenderADS1115 extenderADS1115set2(
  &Native, GedeeldeBusComponent::ADC_ADS1115,
  I2C_ADDRESS_EXTENDER_ADS1115_2,
  HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL,
  EXTENDER_ADS1115_2_ALERT_RDY,
  HardwareResourceToegang::GEDEELD
);


void ToonResultaat(const __FlashStringHelper* stap, bool gelukt) {
  GA_SERIAL.print(stap);
  GA_SERIAL.print(F(": "));
  GA_SERIAL.println(gelukt ? F("OK") : F("FOUT"));
}

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && (millis() - startTijd) < SERIAL_CONNECT_TIMEOUT_MS) { ; }

  GA_SERIAL.println(F("=== FsrRfp twee sets op twee ADS1115's ==="));

  // Set 1: standaardkanalen FSR_RFP_ADC_PIN_SENSOR_n_SET_1, set = 1.
  sensorFsrRfpSet1 = GedeeldeBusNewComponent<struct FsrRfp>(
      &extenderADS1115set1, GedeeldeBusComponent::FsrRfp,
      HardwareResourceToegang::GEDEELD, 1);
  ToonResultaat(F("Set 1 aanmelden/controleren/inpluggen/activeren"), sensorFsrRfpSet1 != nullptr);

  // Set 2: standaardkanalen FSR_RFP_ADC_PIN_SENSOR_n_SET_2, set = 2.
  sensorFsrRfpSet2 = GedeeldeBusNewComponent<struct FsrRfp>(
      &extenderADS1115set2, GedeeldeBusComponent::FsrRfp,
      HardwareResourceToegang::GEDEELD, 2);
  ToonResultaat(F("Set 2 aanmelden/controleren/inpluggen/activeren"), sensorFsrRfpSet2 != nullptr);

  if (sensorFsrRfpSet1 == nullptr || sensorFsrRfpSet2 == nullptr) {
    GA_SERIAL.println(F("RESULTAAT: FOUT"));
    return;
  }
  GA_SERIAL.println(F("RESULTAAT: OK"));
}

void loop() {
  if (sensorFsrRfpSet1 == nullptr || sensorFsrRfpSet2 == nullptr) return;
  for (uint8_t sensorNummer = 0; sensorNummer < FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_1; sensorNummer++) {
    GA_SERIAL.print(F("Set 1 sensor "));
    GA_SERIAL.print(sensorNummer + 1);
    GA_SERIAL.print(F(": "));
    GA_SERIAL.println(sensorFsrRfpSet1->RawAnalogRead(sensorFsrRfpSet1->sensorPin[sensorNummer]));
  }
  for (uint8_t sensorNummer = 0; sensorNummer < FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_2; sensorNummer++) {
    GA_SERIAL.print(F("Set 2 sensor "));
    GA_SERIAL.print(sensorNummer + 1);
    GA_SERIAL.print(F(": "));
    GA_SERIAL.println(sensorFsrRfpSet2->RawAnalogRead(sensorFsrRfpSet2->sensorPin[sensorNummer]));
  }
  GA_SERIAL.println();
  delay(250);
}
