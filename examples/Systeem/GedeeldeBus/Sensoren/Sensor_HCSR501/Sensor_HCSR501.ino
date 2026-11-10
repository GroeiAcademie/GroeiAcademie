// ============================================================================
// Sensor: HCSR501
// ============================================================================
// Hardwaretest voor HCSR501 binnen de bestaande Sensor-architectuur.
// Configuratie: SENSOR_HCSR501_AANTAL moet 1 zijn voor deze hardwaretest.
// Configuratie: DATA_LAYER_CONFIG moet DATA_TYPE_NONE zijn voor deze test.
// De sketch toont de bestaande parent- en Sensor-controles via Serial.
// ============================================================================

#include <GroeiAcademie.h>
#if SENSOR_HCSR501_AANTAL != 1
#error SENSOR_HCSR501_AANTAL moet 1 zijn voor deze hardwaretest.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
#error DATA_LAYER_CONFIG moet DATA_TYPE_NONE zijn voor deze test.
#endif

void setup() {
 GA_SERIAL.begin(SERIAL_BAUDRATE);
 #if defined(DEBUG)
 while (!GA_SERIAL) { ; }
 #else
 const unsigned long startTijd = millis();
 while (!GA_SERIAL && millis()-startTijd<SERIAL_CONNECT_TIMEOUT_MS) { ; }
 #endif
 GA_SERIAL.println(F("TEST: Sensor_HCSR501"));
 bool parentOK = Native.aanmelden() && Native.controleren() && Native.inpluggen() && Native.activeren();
 GA_SERIAL.println(parentOK ? F("PARENT: OK") : F("PARENT: FOUT"));
 if (!parentOK) return;
 HCSR501* sensor = GedeeldeBusNewComponent<HCSR501>(&Native,GedeeldeBusComponent::HCSR501, HardwareResourcePin::NP_D2);
 GA_SERIAL.println(sensor ? F("LIFECYCLE: OK") : F("LIFECYCLE: FOUT"));
 GA_SERIAL.println(F("LET OP: HC-SR501 heeft geen detecteerbare chip-ID; geen bewijs fysieke aanwezigheid."));
}
void loop() {}
