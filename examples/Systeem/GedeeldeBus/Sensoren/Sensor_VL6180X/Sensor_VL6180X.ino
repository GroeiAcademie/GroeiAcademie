// ============================================================================
// Sensor: VL6180X
// ============================================================================
// Vereist geïnstalleerde officiële library: Adafruit_VL6180X.h.
// Aansluiting gebruikte I2C-parent/Extender: SDA -> board-SDA, SCL -> board-SCL; GND -> board-GND.
// UNO R4 Minima/WiFi: fysieke SDA- en SCL-header (bij R4 Minima ook A4=SDA, A5=SCL); UNO Q: board-SDA/SCL.
// ESP32 D1 UNO R32: SDA=GPIO21, SCL=GPIO22 (bestaand FrameWork-boardprofiel).
// ESP32-S3 ARDI-32: SDA=GPIO38, SCL=GPIO39; DEV: SDA=GPIO8, SCL=GPIO9 (FrameWork-profiel).
// RP2040 Cytron Maker UNO: SDA=GPIO20, SCL=GPIO21 (ook Maker/Grove 6).
// STM32 F401RE Nucleo: SDA=PB9, SCL=PB8 (bestaand FrameWork-boardprofiel).
// Voeding VCC en GND volgens het concrete modulebord; controleer I2C-logicaniveau, pull-ups en eventuele levelshifting.
// ============================================================================

#include <GroeiAcademie.h>
#if SENSOR_VL6180X_AANTAL != 1
#error SENSOR_VL6180X_AANTAL moet 1 zijn voor deze hardwaretest.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
#error DATA_LAYER_CONFIG moet DATA_TYPE_NONE zijn voor deze hardwaretest.
#endif
HardwareResourceTypeI2C apparaat(&Native, GedeeldeBusComponent::VL6180X,
  I2C_ADDRESS_SENSOR_VL6180X_1, HardwareResourcePin::NP_SDA,
  HardwareResourcePin::NP_SCL, HardwareResourceToegang::GEDEELD);
SensorVL6180X* sensor = nullptr;
void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  #if defined(DEBUG)
  while (!GA_SERIAL) { ; }
  #else
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && millis() - startTijd < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  #endif
  GA_SERIAL.println(F("TEST: Sensor_VL6180X"));
  const bool parentOK=apparaat.aanmelden() && apparaat.controleren() && apparaat.inpluggen() && apparaat.activeren();
  GA_SERIAL.println(parentOK ? F("I2C RESOURCE: OK") : F("I2C RESOURCE: FOUT"));
  if(parentOK) sensor=GedeeldeBusNewComponent<SensorVL6180X>(&apparaat,GedeeldeBusComponent::VL6180X);
  GA_SERIAL.println(sensor ? F("SENSOR LIBRARY INIT/LIFECYCLE: OK") : F("SENSOR LIBRARY INIT/LIFECYCLE: FOUT"));
}
void loop() {}
