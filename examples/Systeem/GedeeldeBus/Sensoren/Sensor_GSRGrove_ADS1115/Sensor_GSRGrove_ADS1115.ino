// Sensor: GSRGrove via ADS1115; samengesteld uit bestaande Sensor- en Extender-voorbeelden.

#include <GroeiAcademie.h>

#if SENSOR_GSRGROVE_AANTAL != 1
  #error Zet SENSOR_GSRGROVE_AANTAL op 1 voor deze test.
#endif
#if EXTENDER_ADS1115_AANTAL == 0
  #error Zet EXTENDER_ADS1115_AANTAL op minstens 1 voor deze test.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE voor deze test.
#endif

#if EXTENDER_ADS1115_AANTAL == 1
ExtenderADS1115 extender(
  &Native,
  GedeeldeBusComponent::ADC_ADS1115,
  I2C_ADDRESS_EXTENDER_ADS1115_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, EXTENDER_ADS1115_1_ALERT_RDY
);
#else
ExtenderADS1115 extender(
  &Native,
  GedeeldeBusComponent::ADC_ADS1115,
  HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, 0
);
#endif

GSRGrove* sensorGSRGrove = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_GSRGrove_ADS1115"));
  GA_SERIAL.println(F("MEETTEST: Controleer verandering van ruwe ADC bij contact; geen huidgeleidingskalibratie."));
  sensorGSRGrove = GedeeldeBusNewComponent<GSRGrove>(
    &extender, GedeeldeBusComponent::GSRGrove,
    static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN0),
    HardwareResourceToegang::GEDEELD);
  GA_SERIAL.println(sensorGSRGrove ? F("LIFECYCLE: OK") : F("LIFECYCLE: FOUT"));
}
void loop() {
  if (sensorGSRGrove == nullptr) return;
  int32_t waarde = 0;
  if (sensorGSRGrove->RawAnalogRead(waarde)) {
    GA_SERIAL.print(F("GSRGrove ruwe ADC: ")); GA_SERIAL.println(waarde);
  } else {
    GA_SERIAL.println(F("GSRGrove: ADC-UITLEESFOUT (controleer bereik/referentie)"));
  }
}
