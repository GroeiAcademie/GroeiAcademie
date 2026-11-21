// Sensor: PIEZO via ADS7828; samengesteld uit bestaande Sensor- en Extender-voorbeelden.

#include <GroeiAcademie.h>

#if SENSOR_PIEZO_ADC_BEREIK_MILLIVOLT > (EXTENDER_ADS7828_EXTERNE_REFERENTIE ? EXTENDER_ADS7828_REFERENTIE_MILLIVOLT : 2500)
  #error SENSOR_PIEZO_ADC_BEREIK_MILLIVOLT is groter dan wat de ADS7828 meet (interne referentie 2500 mV): stel EXTENDER_ADS7828_EXTERNE_REFERENTIE en EXTENDER_ADS7828_REFERENTIE_MILLIVOLT in volgens je hardware, of verlaag het bereik tot de werkelijke uitgangsspanning van de sensor.
#endif
#if SENSOR_PIEZO_AANTAL != 1
  #error Zet SENSOR_PIEZO_AANTAL op 1 voor deze test.
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

PIEZO* sensorPIEZO = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_PIEZO_ADS7828"));
  GA_SERIAL.println(F("MEETTEST: Tik op de sensor en observeer de snelle pieken in de ADC-samples."));
  sensorPIEZO = GedeeldeBusNewComponent<PIEZO>(
    &extender, GedeeldeBusComponent::PIEZO,
    static_cast<uint8_t>(ExtenderADS7828::ExtenderPins::EP_CH0),
    HardwareResourceToegang::GEDEELD);
  GA_SERIAL.println(sensorPIEZO ? F("LIFECYCLE: OK") : F("LIFECYCLE: FOUT"));
}
void loop() {
  if (sensorPIEZO == nullptr) return;
  int32_t waarde = 0;
  if (sensorPIEZO->RawAnalogRead(waarde)) {
    GA_SERIAL.print(F("PIEZO ruwe ADC-sample (geen fysieke eenheid): "));
    GA_SERIAL.println(waarde);
  } else {
    GA_SERIAL.println(F("ADC-UITLEESFOUT: geen geldige nieuwe meetwaarde (bereik, referentie en verbinding controleren)."));
  }
  // Een geslaagde ruwe uitlezing bewijst nog geen kalibratie of correcte fysieke meetfrequentie.
}
