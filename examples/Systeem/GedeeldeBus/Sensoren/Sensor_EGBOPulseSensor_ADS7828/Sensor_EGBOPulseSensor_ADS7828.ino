// Sensor: EGBOPulseSensor via ADS7828.
// Bestaande FrameWork-API en bestaande Extenderconstructor uit de actuele broncode.
// Configuratie via UserConfig.h. Voeding, GND en signaalniveau fysiek controleren.
// Hardwarewerking en compilatie zijn nog niet bewezen.
// Analoge uitgang naar kanaal 0; bij een CD74HC4067 loopt Z via de geconfigureerde native ADC-ingang.

#include <GroeiAcademie.h>

#if SENSOR_EGBOPULSESENSOR_ADC_BEREIK_MILLIVOLT > (EXTENDER_ADS7828_EXTERNE_REFERENTIE ? EXTENDER_ADS7828_REFERENTIE_MILLIVOLT : 2500)
  #error SENSOR_EGBOPULSESENSOR_ADC_BEREIK_MILLIVOLT is groter dan wat de ADS7828 meet (interne referentie 2500 mV): stel EXTENDER_ADS7828_EXTERNE_REFERENTIE en EXTENDER_ADS7828_REFERENTIE_MILLIVOLT in volgens je hardware, of verlaag het bereik tot de werkelijke uitgangsspanning van de sensor.
#endif
#if SENSOR_EGBOPULSESENSOR_AANTAL != 1
  #error Zet SENSOR_EGBOPULSESENSOR_AANTAL op 1 voor deze test.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE voor deze test.
#endif

#if EXTENDER_ADS7828_AANTAL == 0
  #error Zet EXTENDER_ADS7828_AANTAL in UserConfig.h op minstens 1 voor deze test.
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

EGBOPulseSensor* sensor = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_EGBOPulseSensor_ADS7828"));
  GA_SERIAL.println(F("MEETTEST: Controleer het analoge pulssignaal tijdens en zonder contact; geen BPM-berekening."));
  sensor = GedeeldeBusNewComponent<EGBOPulseSensor>(&extender, GedeeldeBusComponent::EGBOPulseSensor, static_cast<uint8_t>(ExtenderADS7828::ExtenderPins::EP_CH0), HardwareResourceToegang::GEDEELD);
  GA_SERIAL.println(sensor ? F("LIFECYCLE: OK") : F("LIFECYCLE: FOUT"));
}

void loop() {
  if (sensor == nullptr) return;
  int32_t waarde = 0;
  if (sensor->RawAnalogRead(waarde)) {
    GA_SERIAL.print(F("EGBOPulseSensor ruwe ADC-sample (geen fysieke eenheid): "));
    GA_SERIAL.println(waarde);
  } else {
    GA_SERIAL.println(F("ADC-UITLEESFOUT: geen geldige nieuwe meetwaarde (bereik, referentie en verbinding controleren)."));
  }
  // Een geslaagde ruwe uitlezing bewijst nog geen kalibratie of correcte fysieke meetfrequentie.
}
