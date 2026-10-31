#include <GroeiAcademie.h>
#if SENSOR_BME688_AANTAL != 1
#error SENSOR_BME688_AANTAL moet 1 zijn voor deze hardwaretest.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
#error DATA_LAYER_CONFIG moet DATA_TYPE_NONE zijn voor deze hardwaretest.
#endif
HardwareResourceTypeI2C apparaat(&Native, GedeeldeBusComponent::BME688, I2C_ADDRESS_SENSOR_BME688_1,
 HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, HardwareResourceToegang::GEDEELD);
BME68x* sensor = nullptr;
void setup() {
 GA_SERIAL.begin(SERIAL_BAUDRATE);
 const unsigned long beginTijd = millis();
 while (!GA_SERIAL && millis()-beginTijd<SERIAL_CONNECT_TIMEOUT_MS) { ; }
 const bool parentOK = apparaat.aanmelden() && apparaat.controleren() && apparaat.inpluggen() && apparaat.activeren();
 GA_SERIAL.println(parentOK ? F("I2C RESOURCE: OK") : F("I2C RESOURCE: FOUT"));
 if (parentOK) sensor=GedeeldeBusNewComponent<BME68x>(&apparaat,GedeeldeBusComponent::BME688);
 GA_SERIAL.println(sensor ? F("SENSOR IDENTITEIT/LIFECYCLE: OK") : F("SENSOR IDENTITEIT/LIFECYCLE: FOUT"));
}
void loop() {}
