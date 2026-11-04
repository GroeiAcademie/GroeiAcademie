// ============================================================================
// Sensor: MPU6500
// ============================================================================
// Hardwaretest voor MPU6500 binnen de bestaande Sensor-architectuur.
// Configuratie: SENSOR_MPU6500_AANTAL moet 1 zijn voor deze hardwaretest.
// Configuratie: DATA_LAYER_CONFIG moet DATA_TYPE_NONE zijn voor deze hardwaretest.
// Serial toont afzonderlijk de I2C-resource en de Sensor-lifecyclestatus.
// ============================================================================

#include <GroeiAcademie.h>
#if SENSOR_MPU6500_AANTAL != 1
#error SENSOR_MPU6500_AANTAL moet 1 zijn voor deze hardwaretest.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
#error DATA_LAYER_CONFIG moet DATA_TYPE_NONE zijn voor deze hardwaretest.
#endif
HardwareResourceTypeI2C apparaat(&Native, GedeeldeBusComponent::MPU6500, I2C_ADDRESS_SENSOR_MPU6500_1,
 HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, HardwareResourceToegang::GEDEELD);
MPU6500* sensor = nullptr;
void setup() {
 GA_SERIAL.begin(SERIAL_BAUDRATE);
 while (!GA_SERIAL) { ; }
 GA_SERIAL.println(F("TEST: Sensor_MPU6500"));
 const unsigned long beginTijd = millis();
 while (!GA_SERIAL && millis()-beginTijd<SERIAL_CONNECT_TIMEOUT_MS) { ; }
 const bool parentOK = apparaat.aanmelden() && apparaat.controleren() && apparaat.inpluggen() && apparaat.activeren();
 GA_SERIAL.println(parentOK ? F("I2C RESOURCE: OK") : F("I2C RESOURCE: FOUT"));
 if (parentOK) sensor=GedeeldeBusNewComponent<MPU6500>(&apparaat,GedeeldeBusComponent::MPU6500);
 GA_SERIAL.println(sensor ? F("SENSOR IDENTITEIT/LIFECYCLE: OK") : F("SENSOR IDENTITEIT/LIFECYCLE: FOUT"));
}
void loop() {}
