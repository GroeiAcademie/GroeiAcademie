#include <GroeiAcademie.h>
#if SENSOR_LM35DZ_AANTAL != 1
  #error Zet SENSOR_LM35DZ_AANTAL op 1 voor deze hardwaretest.
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
LM35DZ* sensorLM35DZ = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && millis() - startTijd < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  sensorLM35DZ = GedeeldeBusNewComponent<LM35DZ>(
    &extenderADS1115, GedeeldeBusComponent::LM35DZ,
    static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN0),
    HardwareResourceToegang::GEDEELD);
  GA_SERIAL.println(sensorLM35DZ ? F("LIFECYCLE: OK") : F("LIFECYCLE: FOUT"));
}
void loop() {
  if (sensorLM35DZ == nullptr) return;
  // Niet-blokkerend: elke aanroep van loop() bedient hoogstens een ADC-fase.
  static bool bezig = false;
  if (!bezig) bezig = sensorLM35DZ->StartAnalogRead();
  if (bezig && sensorLM35DZ->IsAnalogReadKlaar()) {
    const int waarde = sensorLM35DZ->GetAnalogRead();
    bezig = false;
    GA_SERIAL.print(F("LM35DZ ADC: "));
    GA_SERIAL.println(waarde);
  }
  // Expliciet blokkerend alternatief: RawAnalogRead(waarde).
}
