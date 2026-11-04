// ============================================================================
// Sensor: DS18B20
// ============================================================================
// Hardwaretest voor DS18B20 binnen de bestaande Sensor-architectuur.
// Configuratie: SENSOR_DS18B20_AANTAL moet 1 zijn voor deze hardwaretest.
// Configuratie: DATA_LAYER_CONFIG moet DATA_TYPE_NONE zijn voor deze test.
// De sketch toont de bestaande parent- en Sensor-controles via Serial.
// ============================================================================

#include <GroeiAcademie.h>
#if SENSOR_DS18B20_AANTAL != 1
#error SENSOR_DS18B20_AANTAL moet 1 zijn voor deze hardwaretest.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
#error DATA_LAYER_CONFIG moet DATA_TYPE_NONE zijn voor deze test.
#endif

void setup() {
 GA_SERIAL.begin(SERIAL_BAUDRATE);
 while (!GA_SERIAL) { ; }
 GA_SERIAL.println(F("TEST: Sensor_DS18B20"));
 const unsigned long startTijd = millis();
 while (!GA_SERIAL && millis()-startTijd<SERIAL_CONNECT_TIMEOUT_MS) { ; }
 bool parentOK = Native.aanmelden() && Native.controleren() && Native.inpluggen() && Native.activeren();
 GA_SERIAL.println(parentOK ? F("PARENT: OK") : F("PARENT: FOUT"));
 if (!parentOK) return;
 DS18B20* sensor = GedeeldeBusNewComponent<DS18B20>(&Native,GedeeldeBusComponent::DS18B20, HardwareResourcePin::NP_D2);
 GA_SERIAL.println(sensor ? F("LIFECYCLE: OK") : F("LIFECYCLE: FOUT"));
 
}
void loop() {}
