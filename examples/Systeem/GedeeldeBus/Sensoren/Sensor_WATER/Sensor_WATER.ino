// ============================================================================
// Sensor: WATER
// ============================================================================
// Hardwaretest voor WATER binnen de bestaande Sensor-architectuur.
// Configuratie: Zet SENSOR_WATER_AANTAL op 1 voor deze hardwaretest.
// Configuratie: Zet EXTENDER_ADS1115_AANTAL op minstens 1 voor deze hardwaretest.
// Configuratie: Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE voor deze hardwaretest.
// Serial toont de uitlezing van het in deze sketch gebruikte ADC-kanaal.
// ============================================================================

#include <GroeiAcademie.h>
#if SENSOR_WATER_AANTAL != 1
  #error Zet SENSOR_WATER_AANTAL op 1 voor deze hardwaretest.
#endif
#if EXTENDER_ADS1115_AANTAL == 0
  #error Zet EXTENDER_ADS1115_AANTAL op minstens 1 voor deze hardwaretest.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE voor deze hardwaretest.
#endif

ExtenderADS1115 extenderADS1115(&Native, GedeeldeBusComponent::ADC_ADS1115,
  I2C_ADDRESS_EXTENDER_ADS1115_1, HardwareResourcePin::NP_SDA,
  HardwareResourcePin::NP_SCL, EXTENDER_ADS1115_1_ALERT_RDY,
  HardwareResourceToegang::GEDEELD);
WATER* sensorWATER = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_WATER"));
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && millis() - startTijd < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  sensorWATER = GedeeldeBusNewComponent<WATER>(
    &extenderADS1115, GedeeldeBusComponent::WATER,
    static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN0),
    HardwareResourceToegang::GEDEELD);
  GA_SERIAL.println(sensorWATER ? F("LIFECYCLE: OK") : F("LIFECYCLE: FOUT"));
}
void loop() {
  if (sensorWATER == nullptr) return;
  // Niet-blokkerend: elke aanroep van loop() bedient hoogstens een ADC-fase.
  static bool bezig = false;
  if (!bezig) bezig = sensorWATER->StartAnalogRead();
  if (bezig && sensorWATER->IsAnalogReadKlaar()) {
    const int waarde = sensorWATER->GetAnalogRead();
    bezig = false;
    GA_SERIAL.print(F("WATER ADC: "));
    GA_SERIAL.println(waarde);
  }
  // Expliciet blokkerend alternatief: RawAnalogRead(waarde).
}
