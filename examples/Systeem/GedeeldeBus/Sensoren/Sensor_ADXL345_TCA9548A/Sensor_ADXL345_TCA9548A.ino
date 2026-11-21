// Sensor: ADXL345 via TCA9548A.
// Bestaande FrameWork-API en bestaande Extenderconstructor uit de actuele broncode.
// Configuratie via UserConfig.h. Voeding, GND en signaalniveau fysiek controleren.
// Hardwarewerking en compilatie zijn nog niet bewezen.
// TCA-kanaal 0. Sensor-I2C-adres moet overeenkomen met de werkelijke module.

#include <GroeiAcademie.h>

#if SENSOR_ADXL345_AANTAL != 1
  #error Zet SENSOR_ADXL345_AANTAL op 1 voor deze test.
#endif
#if EXTENDER_TCA9548A_AANTAL == 0
  #error Zet EXTENDER_TCA9548A_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif

#if EXTENDER_TCA9548A_AANTAL == 1
ExtenderTCA9548A extender(
  &Native,
  GedeeldeBusComponent::I2C_MULTIPLEXER_TCA9548A,
  I2C_ADDRESS_EXTENDER_TCA9548A_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, EXTENDER_TCA9548A_1_RESET
);
#else
ExtenderTCA9548A extender(
  &Native,
  GedeeldeBusComponent::I2C_MULTIPLEXER_TCA9548A,
  HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, 0
);
#endif

HardwareResourceTypeI2C apparaat(&extender, GedeeldeBusComponent::ADXL345, I2C_ADDRESS_SENSOR_ADXL345_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, HardwareResourceToegang::GEDEELD, 0);
SensorADXL345* sensor = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_ADXL345_TCA9548A"));
  GA_SERIAL.println(F("MEETTEST: Draai sensor over drie assen. Voor alle assen: DATA_TYPE_DIRECT activeren."));
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  if (Data == nullptr) Data = GedeeldeBusNewComponent<struct Data>(DATA_LAYER_CONFIG);
  GA_SERIAL.println(Data ? F("DATA: OK") : F("DATA: FOUT"));
  if (Data == nullptr) return;
#else
  GA_SERIAL.println(F("LET OP: extra meetkanalen vereisen DATA_TYPE_DIRECT in UserConfig.h."));
#endif
  const bool gelukt = extender.aanmelden() && extender.controleren() && extender.inpluggen() && extender.activeren();
  GA_SERIAL.println(gelukt ? F("TCA: OK") : F("TCA: FOUT"));
  if (!gelukt) return;
  const bool parentOK = apparaat.aanmelden() && apparaat.controleren() && apparaat.inpluggen() && apparaat.activeren();
  GA_SERIAL.println(parentOK ? F("I2C: OK") : F("I2C: FOUT"));
  if (!parentOK) return;
  sensor = GedeeldeBusNewComponent<SensorADXL345>(&apparaat, GedeeldeBusComponent::ADXL345);
  GA_SERIAL.println(sensor ? F("LIFECYCLE: OK") : F("LIFECYCLE: FOUT"));
}

void loop() {
  if (sensor == nullptr) return;
  float waarde = NAN;
  if (sensor->LeesHoofdwaarde(waarde)) {
    // Sensor blijft uitlezen; alleen de Serial-uitvoer wordt beperkt.
    static unsigned long laatsteRapport = 0;
    if (millis() - laatsteRapport < 200UL) return;
    laatsteRapport = millis();
    GA_SERIAL.print(F("Versnelling X (m/s2): "));
    GA_SERIAL.println(waarde);
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
    GA_SERIAL.print(F("Versnelling X (m/s2): "));
    GA_SERIAL.println(sensor->ReadQueueOrStackFloat(1));
    GA_SERIAL.print(F("Versnelling Y (m/s2): "));
    GA_SERIAL.println(sensor->ReadQueueOrStackFloat(2));
    GA_SERIAL.print(F("Versnelling Z (m/s2): "));
    GA_SERIAL.println(sensor->ReadQueueOrStackFloat(3));
#endif
  }
  // Een false-resultaat kan ook betekenen: conversie nog bezig of geen verse sample.
}
