// Sensor: MCU6701 via ADS1115.
// Bestaande FrameWork-API en bestaande Extenderconstructor uit de actuele broncode.
// Configuratie via UserConfig.h. Voeding, GND en signaalniveau fysiek controleren.
// Hardwarewerking en compilatie zijn nog niet bewezen.
// Analoge OUT; MCP3201-SPI-modus wordt niet gebruikt.

#include <GroeiAcademie.h>

#if SENSOR_MCU6701_AANTAL != 1
  #error Zet SENSOR_MCU6701_AANTAL op 1 voor deze test.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE && DATA_LAYER_CONFIG != DATA_TYPE_DIRECT
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE of DATA_TYPE_DIRECT voor deze test.
#endif

#if EXTENDER_ADS1115_AANTAL == 0
  #error Zet EXTENDER_ADS1115_AANTAL in UserConfig.h op minstens 1 voor deze test.
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

MCU6701* sensor = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_MCU6701_ADS1115"));
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  if (Data == nullptr) Data = GedeeldeBusNewComponent<struct Data>(DATA_LAYER_CONFIG);
  GA_SERIAL.println(Data ? F("DATA: OK") : F("DATA: FOUT"));
  if (Data == nullptr) return;
#else
  GA_SERIAL.println(F("LET OP: extra meetkanalen vereisen DATA_TYPE_DIRECT in UserConfig.h."));
#endif
  GA_SERIAL.println(F("MEETTEST: Verander de sensorbelasting; controleer ruwe waarde en communicatiestatus."));
  sensor = GedeeldeBusNewComponent<MCU6701>(&extender, GedeeldeBusComponent::MCU6701, static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN0));
  GA_SERIAL.println(sensor ? F("LIFECYCLE: OK") : F("LIFECYCLE: FOUT"));
}

void loop() {
  if (sensor == nullptr) return;
  float waarde = NAN;
  if (sensor->LeesHoofdwaarde(waarde)) {
    GA_SERIAL.print(F("Sensorwaarde (ruwe telling): "));
    GA_SERIAL.println(waarde);
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
    GA_SERIAL.print(F("Via Data, sensorwaarde (ruwe telling): "));
    GA_SERIAL.println(sensor->ReadQueueOrStackFloat(1));
#endif

  }
  // Een false-resultaat kan ook betekenen: conversie nog bezig of geen verse sample.
}
