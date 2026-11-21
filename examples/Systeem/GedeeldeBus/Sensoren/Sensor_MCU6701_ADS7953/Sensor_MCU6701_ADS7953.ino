// Sensor: MCU6701 via ADS7953.
// Bestaande FrameWork-API en bestaande Extenderconstructor uit de actuele broncode.
// Configuratie via UserConfig.h. Voeding, GND en signaalniveau fysiek controleren.
// Hardwarewerking en compilatie zijn nog niet bewezen.
// Analoge OUT; MCP3201-SPI-modus wordt niet gebruikt.

#include <GroeiAcademie.h>

#if EXTENDER_ADS7953_VOEDING_MILLIVOLT == 0
  #error Zet EXTENDER_ADS7953_VOEDING_MILLIVOLT op de werkelijke +VA (2700..5250 mV): het bereik van deze sensor vraagt 2xVREF.
#endif

#if (SENSOR_MCU6701_ADC_BEREIK_MILLIVOLT > EXTENDER_ADS7953_REFERENTIE_MILLIVOLT && (SENSOR_MCU6701_ADC_BEREIK_MILLIVOLT > 2 * EXTENDER_ADS7953_REFERENTIE_MILLIVOLT || EXTENDER_ADS7953_VOEDING_MILLIVOLT < 2 * EXTENDER_ADS7953_REFERENTIE_MILLIVOLT)) || \
    (EXTENDER_ADS7953_VOEDING_MILLIVOLT != 0 && EXTENDER_ADS7953_VOEDING_MILLIVOLT < EXTENDER_ADS7953_REFERENTIE_MILLIVOLT)
  #error SENSOR_MCU6701_ADC_BEREIK_MILLIVOLT is groter dan wat de ADS7953 meet: het dubbele bereik (2 x EXTENDER_ADS7953_REFERENTIE_MILLIVOLT) vraagt EXTENDER_ADS7953_VOEDING_MILLIVOLT van minstens 2 x de referentie; stel beide in volgens je hardware, of verlaag het bereik tot de werkelijke uitgangsspanning van de sensor.
#endif
#if SENSOR_MCU6701_AANTAL != 1
  #error Zet SENSOR_MCU6701_AANTAL op 1 voor deze test.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE voor deze test.
#endif

#if EXTENDER_ADS7953_AANTAL == 0
  #error Zet EXTENDER_ADS7953_AANTAL in UserConfig.h op minstens 1 voor deze test.
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

MCU6701* sensor = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_MCU6701_ADS7953"));
  GA_SERIAL.println(F("MEETTEST: Verander de sensorbelasting; controleer ruwe waarde en communicatiestatus."));
  sensor = GedeeldeBusNewComponent<MCU6701>(&extender, GedeeldeBusComponent::MCU6701, static_cast<uint8_t>(ExtenderADS7953::ExtenderPins::EP_CH0));
  GA_SERIAL.println(sensor ? F("LIFECYCLE: OK") : F("LIFECYCLE: FOUT"));
}

void loop() {
  if (sensor == nullptr) return;
  float waarde = NAN;
  if (sensor->LeesHoofdwaarde(waarde)) {
    GA_SERIAL.print(F("Sensorwaarde (ruwe telling): "));
    GA_SERIAL.println(waarde);

  }
  // Een false-resultaat kan ook betekenen: conversie nog bezig of geen verse sample.
}
