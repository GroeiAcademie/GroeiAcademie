#include <GroeiAcademie.h>
#if SENSOR_GSRGROVE_AANTAL != 1
  #error Zet SENSOR_GSRGROVE_AANTAL op 1 voor deze hardwaretest.
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
GSRGrove* sensorGSRGrove = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && millis() - startTijd < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  sensorGSRGrove = GedeeldeBusNewComponent<GSRGrove>(
    &extenderADS1115, GedeeldeBusComponent::GSRGrove,
    static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN0),
    HardwareResourceToegang::GEDEELD);
  GA_SERIAL.println(sensorGSRGrove ? F("LIFECYCLE: OK") : F("LIFECYCLE: FOUT"));
}
void loop() {
  if (sensorGSRGrove == nullptr) return;
  // Niet-blokkerend: elke aanroep van loop() bedient hoogstens een ADC-fase.
  static bool bezig = false;
  if (!bezig) bezig = sensorGSRGrove->StartAnalogRead();
  if (bezig && sensorGSRGrove->IsAnalogReadKlaar()) {
    const int waarde = sensorGSRGrove->GetAnalogRead();
    bezig = false;
    GA_SERIAL.print(F("GSRGrove ADC: "));
    GA_SERIAL.println(waarde);
  }
  // Expliciet blokkerend alternatief: RawAnalogRead(waarde).
}
