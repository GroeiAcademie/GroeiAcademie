// Sensor: GSRGrove via ADS7953; samengesteld uit bestaande Sensor- en Extender-voorbeelden.

#include <GroeiAcademie.h>

#if EXTENDER_ADS7953_VOEDING_MILLIVOLT == 0
  #error Zet EXTENDER_ADS7953_VOEDING_MILLIVOLT op de werkelijke +VA (2700..5250 mV): het bereik van deze sensor vraagt 2xVREF.
#endif

#if (SENSOR_GSRGROVE_ADC_BEREIK_MILLIVOLT > EXTENDER_ADS7953_REFERENTIE_MILLIVOLT && (SENSOR_GSRGROVE_ADC_BEREIK_MILLIVOLT > 2 * EXTENDER_ADS7953_REFERENTIE_MILLIVOLT || EXTENDER_ADS7953_VOEDING_MILLIVOLT < 2 * EXTENDER_ADS7953_REFERENTIE_MILLIVOLT)) || \
    (EXTENDER_ADS7953_VOEDING_MILLIVOLT != 0 && EXTENDER_ADS7953_VOEDING_MILLIVOLT < EXTENDER_ADS7953_REFERENTIE_MILLIVOLT)
  #error SENSOR_GSRGROVE_ADC_BEREIK_MILLIVOLT is groter dan wat de ADS7953 meet: het dubbele bereik (2 x EXTENDER_ADS7953_REFERENTIE_MILLIVOLT) vraagt EXTENDER_ADS7953_VOEDING_MILLIVOLT van minstens 2 x de referentie; stel beide in volgens je hardware, of verlaag het bereik tot de werkelijke uitgangsspanning van de sensor.
#endif
#if SENSOR_GSRGROVE_AANTAL != 1
  #error Zet SENSOR_GSRGROVE_AANTAL op 1 voor deze test.
#endif
#if EXTENDER_ADS7953_AANTAL == 0
  #error Zet EXTENDER_ADS7953_AANTAL op minstens 1 voor deze test.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE voor deze test.
#endif

#if EXTENDER_ADS7953_AANTAL == 1
ExtenderADS7953 extender(
  &Native,
  GedeeldeBusComponent::ADC_ADS7953,
  EXTENDER_ADS7953_1_CS_PIN, HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI
);
#else
ExtenderADS7953 extender(
  &Native,
  GedeeldeBusComponent::ADC_ADS7953,
  HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, 0
);
#endif

GSRGrove* sensorGSRGrove = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_GSRGrove_ADS7953"));
  GA_SERIAL.println(F("MEETTEST: Controleer verandering van ruwe ADC bij contact; geen huidgeleidingskalibratie."));
  sensorGSRGrove = GedeeldeBusNewComponent<GSRGrove>(
    &extender, GedeeldeBusComponent::GSRGrove,
    static_cast<uint8_t>(ExtenderADS7953::ExtenderPins::EP_CH0),
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
