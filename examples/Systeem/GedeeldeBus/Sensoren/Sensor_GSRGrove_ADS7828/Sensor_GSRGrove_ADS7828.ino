// Sensor: GSRGrove via ADS7828; samengesteld uit bestaande Sensor- en Extender-voorbeelden.

#include <GroeiAcademie.h>

#if !EXTENDER_ADS7828_EXTERNE_REFERENTIE
  #error Zet EXTENDER_ADS7828_EXTERNE_REFERENTIE op 1 en EXTENDER_ADS7828_REFERENTIE_MILLIVOLT op de werkelijke REF: het bereik van deze sensor ligt boven 2500 mV.
#endif

#if SENSOR_GSRGROVE_ADC_BEREIK_MILLIVOLT > (EXTENDER_ADS7828_EXTERNE_REFERENTIE ? EXTENDER_ADS7828_REFERENTIE_MILLIVOLT : 2500)
  #error SENSOR_GSRGROVE_ADC_BEREIK_MILLIVOLT is groter dan wat de ADS7828 meet (interne referentie 2500 mV): stel EXTENDER_ADS7828_EXTERNE_REFERENTIE en EXTENDER_ADS7828_REFERENTIE_MILLIVOLT in volgens je hardware, of verlaag het bereik tot de werkelijke uitgangsspanning van de sensor.
#endif
#if SENSOR_GSRGROVE_AANTAL != 1
  #error Zet SENSOR_GSRGROVE_AANTAL op 1 voor deze test.
#endif
#if EXTENDER_ADS7828_AANTAL == 0
  #error Zet EXTENDER_ADS7828_AANTAL op minstens 1 voor deze test.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE voor deze test.
#endif

#if EXTENDER_ADS7828_AANTAL == 1
ExtenderADS7828 extender(
  &Native,
  GedeeldeBusComponent::ADC_ADS7828,
  I2C_ADDRESS_EXTENDER_ADS7828_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL
);
#else
ExtenderADS7828 extender(
  &Native,
  GedeeldeBusComponent::ADC_ADS7828,
  HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, 0
);
#endif

GSRGrove* sensorGSRGrove = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_GSRGrove_ADS7828"));
  GA_SERIAL.println(F("MEETTEST: Controleer verandering van ruwe ADC bij contact; geen huidgeleidingskalibratie."));
  sensorGSRGrove = GedeeldeBusNewComponent<GSRGrove>(
    &extender, GedeeldeBusComponent::GSRGrove,
    static_cast<uint8_t>(ExtenderADS7828::ExtenderPins::EP_CH0),
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
