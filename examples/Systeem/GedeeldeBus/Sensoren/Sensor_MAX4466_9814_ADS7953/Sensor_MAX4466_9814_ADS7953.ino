// Sensor: MAX4466 of MAX9814 via ADS7953.
// Dit zijn twee alternatieve fysieke modules, nooit tegelijk op dezelfde Sensor-ingang.
// Stel in UserConfig.h exact één van SENSOR_MAX4466_AANTAL en SENSOR_MAX9814_AANTAL op 1.
// Analoge OUT -> ADC-kanaal 0; voedingsspanning en ADC-referentie controleren.

#include <GroeiAcademie.h>

#if ((SENSOR_MAX4466_AANTAL == 1 ? SENSOR_MAX4466_ADC_BEREIK_MILLIVOLT : SENSOR_MAX9814_ADC_BEREIK_MILLIVOLT) > EXTENDER_ADS7953_REFERENTIE_MILLIVOLT && ((SENSOR_MAX4466_AANTAL == 1 ? SENSOR_MAX4466_ADC_BEREIK_MILLIVOLT : SENSOR_MAX9814_ADC_BEREIK_MILLIVOLT) > 2 * EXTENDER_ADS7953_REFERENTIE_MILLIVOLT || EXTENDER_ADS7953_VOEDING_MILLIVOLT < 2 * EXTENDER_ADS7953_REFERENTIE_MILLIVOLT)) || \
    (EXTENDER_ADS7953_VOEDING_MILLIVOLT != 0 && EXTENDER_ADS7953_VOEDING_MILLIVOLT < EXTENDER_ADS7953_REFERENTIE_MILLIVOLT)
  #error SENSOR_MAX4466_ADC_BEREIK_MILLIVOLT of SENSOR_MAX9814_ADC_BEREIK_MILLIVOLT is groter dan wat de ADS7953 meet: het dubbele bereik (2 x EXTENDER_ADS7953_REFERENTIE_MILLIVOLT) vraagt EXTENDER_ADS7953_VOEDING_MILLIVOLT van minstens 2 x de referentie; stel beide in volgens je hardware, of verlaag het bereik tot de werkelijke uitgangsspanning van de sensor.
#endif
#if (SENSOR_MAX4466_AANTAL + SENSOR_MAX9814_AANTAL) != 1
  #error Kies precies een module: SENSOR_MAX4466_AANTAL=1 OF SENSOR_MAX9814_AANTAL=1.
#endif
#if EXTENDER_ADS7953_AANTAL == 0
  #error Zet EXTENDER_ADS7953_AANTAL op minstens 1 voor deze test.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE voor deze test.
#endif

#if EXTENDER_ADS7953_AANTAL == 1
ExtenderADS7953 extender(
  &Native, GedeeldeBusComponent::ADC_ADS7953, EXTENDER_ADS7953_1_CS_PIN, HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI
);
#else
ExtenderADS7953 extender(
  &Native, GedeeldeBusComponent::ADC_ADS7953, HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, 0
);
#endif

MAX4466_9814* sensor = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_MAX4466_9814_ADS7953"));
  GA_SERIAL.println(F("MEETTEST: Produceer geluid en controleer raw audio ADC, geen amplitude-in-dB."));
#if SENSOR_MAX4466_AANTAL == 1
  sensor = GedeeldeBusNewComponent<MAX4466_9814>(&extender, GedeeldeBusComponent::MAX4466, static_cast<uint8_t>(ExtenderADS7953::ExtenderPins::EP_CH0), HardwareResourceToegang::GEDEELD);
#else
  sensor = GedeeldeBusNewComponent<MAX4466_9814>(&extender, GedeeldeBusComponent::MAX9814, static_cast<uint8_t>(ExtenderADS7953::ExtenderPins::EP_CH0), HardwareResourceToegang::GEDEELD);
#endif
  GA_SERIAL.println(sensor ? F("SENSOR: OK") : F("SENSOR: FOUT"));
}

void loop() {
  if (sensor == nullptr) return;
  int32_t waarde = 0;
  if (sensor->RawAnalogRead(waarde)) {
    GA_SERIAL.print(F("MAX4466 ruwe ADC-sample (geen fysieke eenheid): "));
    GA_SERIAL.println(waarde);
  } else {
    GA_SERIAL.println(F("ADC-UITLEESFOUT: geen geldige nieuwe meetwaarde (bereik, referentie en verbinding controleren)."));
  }
  // Een geslaagde ruwe uitlezing bewijst nog geen kalibratie of correcte fysieke meetfrequentie.
}
