// ============================================================================
// Sensor: EGBOPulseSensor
// ============================================================================
// EGBO Pulse Sensor, analoge variant voorlopig, exact bord bevestigen
// Bron: Niet officieel geverifieerd: identiteit/elektrische eigenschappen te bevestigen
// Biosignalen: nooit op een persoon testen zonder geschikte elektrische isolatie/veiligheidscontrole.
// ============================================================================

#include <GroeiAcademie.h>
#if SENSOR_EGBOPULSESENSOR_AANTAL != 1
#error Zet SENSOR_EGBOPULSESENSOR_AANTAL op 1 voor deze hardwaretest.
#endif
#if EXTENDER_ADS1115_AANTAL < 1
#error Zet EXTENDER_ADS1115_AANTAL op minstens 1.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
#error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE.
#endif
ExtenderADS1115 extenderADS1115(&Native, GedeeldeBusComponent::ADC_ADS1115,
  I2C_ADDRESS_EXTENDER_ADS1115_1, HardwareResourcePin::NP_SDA,
  HardwareResourcePin::NP_SCL, EXTENDER_ADS1115_1_ALERT_RDY,
  HardwareResourceToegang::GEDEELD);
EGBOPulseSensor* sensorEGBOPulseSensor = nullptr;
void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_EGBOPulseSensor"));
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && millis() - startTijd < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  sensorEGBOPulseSensor = GedeeldeBusNewComponent<EGBOPulseSensor>(&extenderADS1115,
    GedeeldeBusComponent::EGBOPulseSensor,
    static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN0),
    HardwareResourceToegang::GEDEELD);
  GA_SERIAL.println(sensorEGBOPulseSensor ? F("EGBOPulseSensor lifecycle: component aangemaakt (fysieke werking nog niet bewezen)") : F("EGBOPulseSensor lifecycle: FOUT"));
}
void loop() {}
