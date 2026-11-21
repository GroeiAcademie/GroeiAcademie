// Sensor: MCU6701 via CD74HC4067.
// Bestaande FrameWork-API en bestaande Extenderconstructor uit de actuele broncode.
// Configuratie via UserConfig.h. Voeding, GND en signaalniveau fysiek controleren.
// Hardwarewerking en compilatie zijn nog niet bewezen.
// Analoge OUT; MCP3201-SPI-modus wordt niet gebruikt.

#include <GroeiAcademie.h>

#if EXTENDER_CD74HC4067_NATIVE_REFERENTIE_MILLIVOLT == 0
  #error Zet EXTENDER_CD74HC4067_NATIVE_REFERENTIE_MILLIVOLT op de werkelijke analoge referentie van het board: Z hangt aan een native analoge pin.
#endif

#if SENSOR_MCU6701_AANTAL != 1
  #error Zet SENSOR_MCU6701_AANTAL op 1 voor deze test.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE voor deze test.
#endif

#if EXTENDER_CD74HC4067_AANTAL == 0
  #error Zet EXTENDER_CD74HC4067_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif

#if EXTENDER_CD74HC4067_AANTAL == 1
ExtenderCD74HC4067 extender(
  &Native,
  GedeeldeBusComponent::ADS_MULTIPLEXER_CD74HC4067,
  EXTENDER_CD74HC4067_1_S0, EXTENDER_CD74HC4067_1_S1, EXTENDER_CD74HC4067_1_S2, EXTENDER_CD74HC4067_1_S3, EXTENDER_CD74HC4067_1_EN, EXTENDER_CD74HC4067_1_Z
);
#else
ExtenderCD74HC4067 extender(
  &Native,
  GedeeldeBusComponent::ADS_MULTIPLEXER_CD74HC4067,
  0
);
#endif

MCU6701* sensor = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_MCU6701_CD74HC4067"));
  GA_SERIAL.println(F("MEETTEST: Verander de sensorbelasting; controleer ruwe waarde en communicatiestatus."));
  sensor = GedeeldeBusNewComponent<MCU6701>(&extender, GedeeldeBusComponent::MCU6701, static_cast<uint8_t>(ExtenderCD74HC4067::ExtenderPins::EP_Y0));
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
