// ============================================================================
// Sensor: MAX4466
// ============================================================================
// Microfoonversterker, analoge OUT via ADC
// Bron: https://learn.adafruit.com/adafruit-microphone-amplifier-breakout/overview
// Biosignalen: nooit op een persoon testen zonder geschikte elektrische isolatie/veiligheidscontrole.
// ============================================================================

#include <GroeiAcademie.h>
#if SENSOR_MAX4466_AANTAL != 1
#error Zet SENSOR_MAX4466_AANTAL op 1 voor deze hardwaretest.
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
MAX4466_9814* sensorMAX4466 = nullptr;
void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_MAX4466"));
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && millis() - startTijd < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  sensorMAX4466 = GedeeldeBusNewComponent<MAX4466_9814>(&extenderADS1115,
    GedeeldeBusComponent::MAX4466,
    static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN0),
    HardwareResourceToegang::GEDEELD);
  GA_SERIAL.println(sensorMAX4466 ? F("MAX4466 lifecycle: component aangemaakt (fysieke werking nog niet bewezen)") : F("MAX4466 lifecycle: FOUT"));
}
void loop() {}
