#include <GroeiAcademie.h>
#if SENSOR_MCU6701_AANTAL == 0
#error Zet SENSOR_MCU6701_AANTAL op 1 voor deze test.
#endif
#if EXTENDER_ADS1115_AANTAL == 0
#error Deze analoge voorbeeldsketch gebruikt een ADS1115. Activeer die Extender in de bestaande configuratie.
#endif
ExtenderADS1115 extenderVoorGSR(&Native, GedeeldeBusComponent::ADC_ADS1115,
    I2C_ADDRESS_EXTENDER_ADS1115_1, HardwareResourcePin::NP_SDA,
    HardwareResourcePin::NP_SCL, EXTENDER_ADS1115_1_ALERT_RDY,
    HardwareResourceToegang::GEDEELD);
MCU6701* analogeGSR = nullptr;
void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  // OUT -> AIN0 van ADS1115; CS, MIS en SCK hier niet aansluiten.
  analogeGSR = GedeeldeBusNewComponent<MCU6701>(&extenderVoorGSR,
      GedeeldeBusComponent::MCU6701, static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN0));
  GA_SERIAL.println(analogeGSR ? F("MCU6701 OUT: OK") : F("MCU6701 OUT: FOUT"));
}
void loop() {
  if (!analogeGSR) return;
  float ruw = NAN;
  if (analogeGSR->LeesHoofdwaarde(ruw)) {
    GA_SERIAL.print(F("GSR OUT ADC geschaald: ")); GA_SERIAL.println(ruw);
  }
  delay(200);
}
