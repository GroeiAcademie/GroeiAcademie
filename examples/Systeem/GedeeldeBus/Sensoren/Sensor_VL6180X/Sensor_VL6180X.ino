// Vereist geïnstalleerde officiële library: Adafruit_VL6180X.h.
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
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && millis() - startTijd < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  const unsigned long beginTijd=millis();
  while(!GA_SERIAL && millis()-beginTijd<SERIAL_CONNECT_TIMEOUT_MS) { ; }
  const bool parentOK=apparaat.aanmelden() && apparaat.controleren() && apparaat.inpluggen() && apparaat.activeren();
  GA_SERIAL.println(parentOK ? F("I2C RESOURCE: OK") : F("I2C RESOURCE: FOUT"));
  if(parentOK) sensor=GedeeldeBusNewComponent<SensorVL6180X>(&apparaat,GedeeldeBusComponent::VL6180X);
  GA_SERIAL.println(sensor ? F("SENSOR LIBRARY INIT/LIFECYCLE: OK") : F("SENSOR LIBRARY INIT/LIFECYCLE: FOUT"));
}
void loop() {}
