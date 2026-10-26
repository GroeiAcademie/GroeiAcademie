// ============================================================================
// Sensor: RFP602
// ============================================================================
// Basis-lifecycletest voor RFP602 zonder Data-class.
// De ADC-Extender wordt hieronder expliciet gekozen.
// ============================================================================

#include <GroeiAcademie.h>

#if EXTENDER_ADS1115_AANTAL == 0
  #error Zet EXTENDER_ADS1115_AANTAL in UserConfig.h op minstens 1: RFP602 meet via een ADS1115-Extender.
#endif

ExtenderADS1115 extenderADS1115(
  &Native, GedeeldeBusComponent::ADC_ADS1115,
  I2C_ADDRESS_EXTENDER_ADS1115_1,
  HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL,
  EXTENDER_ADS1115_1_ALERT_RDY,
  HardwareResourceToegang::GEDEELD
);


#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE voor deze test.
#endif


void ToonResultaat(const __FlashStringHelper* stap, bool gelukt) {
  GA_SERIAL.print(stap);
  GA_SERIAL.print(F(": "));
  GA_SERIAL.println(gelukt ? F("OK") : F("FOUT"));
}

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && (millis() - startTijd) < SERIAL_CONNECT_TIMEOUT_MS) { ; }

  GA_SERIAL.println(F("=== RFP602 lifecycle-test ==="));
  GA_SERIAL.println(F("Extender: ADS1115"));

  sensorRFP602set1 = GedeeldeBusNewComponent<struct RFP602>(
      &extenderADS1115, GedeeldeBusComponent::RFP602,
      RFP602_ADC_PIN_SENSOR_1_SET_1, RFP602_ADC_PIN_SENSOR_2_SET_1, RFP602_ADC_PIN_SENSOR_3_SET_1, RFP602_ADC_PIN_SENSOR_4_SET_1,
      HardwareResourceToegang::GEDEELD);
  const bool gelukt = sensorRFP602set1 != nullptr;
  ToonResultaat(F("Sensor aanmelden/controleren/inpluggen/activeren"), gelukt);

  if (!gelukt) {
    GA_SERIAL.println(F("RESULTAAT: FOUT"));
    return;
  }

  for (uint8_t sensorNummer = 0; sensorNummer < RFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_1; sensorNummer++) {
    GA_SERIAL.print(F("Sensor "));
    GA_SERIAL.print(sensorNummer + 1);
    GA_SERIAL.print(F(": "));
    GA_SERIAL.println(sensorRFP602set1->RawAnalogRead(sensorRFP602set1->sensorPin[sensorNummer]));
  }

  GA_SERIAL.println(F("RESULTAAT: OK"));
}

void loop() {
  if (sensorRFP602set1 == nullptr) return;
  for (uint8_t sensorNummer = 0; sensorNummer < RFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_1; sensorNummer++) {
    GA_SERIAL.print(F("Sensor "));
    GA_SERIAL.print(sensorNummer + 1);
    GA_SERIAL.print(F(": "));
    GA_SERIAL.println(sensorRFP602set1->RawAnalogRead(sensorRFP602set1->sensorPin[sensorNummer]));
  }

  GA_SERIAL.println();
  delay(250);
}
