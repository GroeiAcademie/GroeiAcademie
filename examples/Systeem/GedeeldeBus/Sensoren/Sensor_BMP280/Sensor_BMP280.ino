#include <GroeiAcademie.h>
#if SENSOR_BMP280_AANTAL != 1
#error SENSOR_BMP280_AANTAL moet 1 zijn voor deze hardwaretest.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
#error DATA_LAYER_CONFIG moet DATA_TYPE_NONE zijn voor deze hardwaretest.
#endif
HardwareResourceTypeI2C apparaat(&Native, GedeeldeBusComponent::BMP280,
  I2C_ADDRESS_SENSOR_BMP280_1, HardwareResourcePin::NP_SDA,
  HardwareResourcePin::NP_SCL, HardwareResourceToegang::GEDEELD);
BMP280* sensor = nullptr;
void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  const unsigned long beginTijd = millis();
  while (!GA_SERIAL && millis() - beginTijd < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  const bool parentOK = apparaat.aanmelden() && apparaat.controleren() && apparaat.inpluggen() && apparaat.activeren();
  GA_SERIAL.println(parentOK ? F("I2C RESOURCE: OK") : F("I2C RESOURCE: FOUT"));
  if (parentOK) sensor = GedeeldeBusNewComponent<BMP280>(&apparaat, GedeeldeBusComponent::BMP280);
  GA_SERIAL.println(sensor ? F("SENSOR IDENTITEIT/LIFECYCLE: OK") : F("SENSOR IDENTITEIT/LIFECYCLE: FOUT"));
}
void loop() {}
