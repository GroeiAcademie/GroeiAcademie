// Sensor: GSRGrove via ADS1158; samengesteld uit bestaande Sensor- en Extender-voorbeelden.

#include <GroeiAcademie.h>

#if SENSOR_GSRGROVE_AANTAL != 1
  #error Zet SENSOR_GSRGROVE_AANTAL op 1 voor deze test.
#endif
#if EXTENDER_ADS1158_AANTAL == 0
  #error Zet EXTENDER_ADS1158_AANTAL op minstens 1 voor deze test.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE voor deze test.
#endif

#if EXTENDER_ADS1158_AANTAL == 1
ExtenderADS1158 extender(
  &Native,
  GedeeldeBusComponent::ADC_ADS1158,
  EXTENDER_ADS1158_1_CS_PIN, HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, EXTENDER_ADS1158_1_START, EXTENDER_ADS1158_1_RESET, EXTENDER_ADS1158_1_PWDN
);
#else
ExtenderADS1158 extender(
  &Native,
  GedeeldeBusComponent::ADC_ADS1158,
  HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, 0
);
#endif

GSRGrove* sensorGSRGrove = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_GSRGrove_ADS1158"));
  GA_SERIAL.println(F("MEETTEST: Controleer verandering van ruwe ADC bij contact; geen huidgeleidingskalibratie."));
  sensorGSRGrove = GedeeldeBusNewComponent<GSRGrove>(
    &extender, GedeeldeBusComponent::GSRGrove,
    static_cast<uint8_t>(ExtenderADS1158::ExtenderPins::EP_AIN0),
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
