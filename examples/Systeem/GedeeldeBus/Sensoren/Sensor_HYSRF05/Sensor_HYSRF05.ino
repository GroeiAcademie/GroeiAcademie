// ============================================================================
// Sensor: HYSRF05
// ============================================================================
// TRIG=D2; ECHO=D3; GND gemeenschappelijk; controleer 5V/3.3V-levelcompatibiliteit.
// ============================================================================

#include <GroeiAcademie.h>
#if SENSOR_HYSRF05_AANTAL != 1
#error SENSOR_HYSRF05_AANTAL moet 1 zijn voor deze hardwaretest.
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
  while (!GA_SERIAL && millis() - startTijd < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  #endif
  GA_SERIAL.println(F("TEST: Sensor_HYSRF05"));
  const bool parentOK = Native.aanmelden() && Native.controleren() && Native.inpluggen() && Native.activeren();
  GA_SERIAL.println(parentOK ? F("PARENT: OK") : F("PARENT: FOUT"));
  if (!parentOK) return;
  HSR0x* sensor = GedeeldeBusNewComponent<HSR0x>(&Native, GedeeldeBusComponent::HYSRF05, HardwareResourcePin::NP_D2, HardwareResourcePin::NP_D3);
  GA_SERIAL.println(sensor ? F("LIFECYCLE: OK") : F("LIFECYCLE: FOUT"));
  GA_SERIAL.println(F("LET OP: geen fysieke detectie mogelijk zonder ECHO-meting."));
}
void loop() {}
