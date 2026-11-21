// ============================================================================
// Sensor: MPU6500
// ============================================================================
// Hardwaretest voor MPU6500 binnen de bestaande Sensor-architectuur.
// Configuratie: SENSOR_MPU6500_AANTAL moet 1 zijn voor deze hardwaretest.
// Configuratie: DATA_LAYER_CONFIG moet DATA_TYPE_NONE of DATA_TYPE_DIRECT zijn voor deze hardwaretest.
// Serial toont afzonderlijk de I2C-resource en de Sensor-lifecyclestatus.
// Aansluiting gebruikte I2C-parent/Extender: SDA -> board-SDA, SCL -> board-SCL; GND -> board-GND.
// UNO R4 Minima/WiFi: fysieke SDA- en SCL-header (bij R4 Minima ook A4=SDA, A5=SCL); UNO Q: board-SDA/SCL.
// ESP32 D1 UNO R32: SDA=GPIO21, SCL=GPIO22 (bestaand FrameWork-boardprofiel).
// ESP32-S3 ARDI-32: SDA=GPIO38, SCL=GPIO39; DEV: SDA=GPIO8, SCL=GPIO9 (FrameWork-profiel).
// RP2040 Cytron Maker UNO: SDA=GPIO20, SCL=GPIO21 (ook Maker/Grove 6).
// STM32 F401RE Nucleo: SDA=PB9, SCL=PB8 (bestaand FrameWork-boardprofiel).
// Voeding VCC en GND volgens het concrete modulebord; controleer I2C-logicaniveau, pull-ups en eventuele levelshifting.
// ============================================================================

#include <GroeiAcademie.h>
#if SENSOR_MPU6500_AANTAL != 1
#error SENSOR_MPU6500_AANTAL moet 1 zijn voor deze hardwaretest.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE && DATA_LAYER_CONFIG != DATA_TYPE_DIRECT
#error DATA_LAYER_CONFIG moet DATA_TYPE_NONE of DATA_TYPE_DIRECT zijn voor deze hardwaretest.
#endif
HardwareResourceTypeI2C apparaat(&Native, GedeeldeBusComponent::MPU6500, I2C_ADDRESS_SENSOR_MPU6500_1,
 HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, HardwareResourceToegang::GEDEELD);
MPU6500* sensor = nullptr;
void setup() {
 GA_SERIAL.begin(SERIAL_BAUDRATE);
 #if defined(DEBUG)
 while (!GA_SERIAL) { ; }
 #else
 const unsigned long beginTijd = millis();
 while (!GA_SERIAL && millis()-beginTijd<SERIAL_CONNECT_TIMEOUT_MS) { ; }
 #endif
 GA_SERIAL.println(F("TEST: Sensor_MPU6500"));
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  if (Data == nullptr) Data = GedeeldeBusNewComponent<struct Data>(DATA_LAYER_CONFIG);
  GA_SERIAL.println(Data ? F("DATA: OK") : F("DATA: FOUT"));
  if (Data == nullptr) return;
#else
  GA_SERIAL.println(F("LET OP: extra meetkanalen vereisen DATA_TYPE_DIRECT in UserConfig.h."));
#endif
 const bool parentOK = apparaat.aanmelden() && apparaat.controleren() && apparaat.inpluggen() && apparaat.activeren();
 GA_SERIAL.println(parentOK ? F("I2C RESOURCE: OK") : F("I2C RESOURCE: FOUT"));
 if (parentOK) sensor=GedeeldeBusNewComponent<MPU6500>(&apparaat,GedeeldeBusComponent::MPU6500);
 GA_SERIAL.println(sensor ? F("SENSOR IDENTITEIT/LIFECYCLE: OK") : F("SENSOR IDENTITEIT/LIFECYCLE: FOUT"));
}
void loop() {
  if (sensor == nullptr) return;
  float waarde = NAN;
  if (!sensor->LeesHoofdwaarde(waarde)) return;
  static unsigned long laatsteRapport = 0;
  if (millis() - laatsteRapport < 250UL) return;
  laatsteRapport = millis();
  GA_SERIAL.print(F("Versnelling X (m/s2): "));
  GA_SERIAL.println(waarde);
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  GA_SERIAL.print(F("Via Data, versnelling X (m/s2): "));
  GA_SERIAL.println(sensor->ReadQueueOrStackFloat(1));
  GA_SERIAL.print(F("Via Data, versnelling Y (m/s2): "));
  GA_SERIAL.println(sensor->ReadQueueOrStackFloat(2));
  GA_SERIAL.print(F("Via Data, versnelling Z (m/s2): "));
  GA_SERIAL.println(sensor->ReadQueueOrStackFloat(3));
  GA_SERIAL.print(F("Via Data, draaisnelheid X (rad/s): "));
  GA_SERIAL.println(sensor->ReadQueueOrStackFloat(4));
  GA_SERIAL.print(F("Via Data, draaisnelheid Y (rad/s): "));
  GA_SERIAL.println(sensor->ReadQueueOrStackFloat(5));
  GA_SERIAL.print(F("Via Data, draaisnelheid Z (rad/s): "));
  GA_SERIAL.println(sensor->ReadQueueOrStackFloat(6));
  GA_SERIAL.print(F("Via Data, temperatuur (C): "));
  GA_SERIAL.println(sensor->ReadQueueOrStackFloat(7));
#endif
}
