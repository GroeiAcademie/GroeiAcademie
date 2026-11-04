// ============================================================================
// Sensor: HX711
// ============================================================================
// DOUT=D2, PD_SCK=D3, loadcell en module correct voeden.
// ============================================================================

#include <GroeiAcademie.h>
#if SENSOR_HX711_AANTAL != 1
#error SENSOR_HX711_AANTAL moet 1 zijn voor deze hardwaretest.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
#error DATA_LAYER_CONFIG moet DATA_TYPE_NONE zijn voor deze test.
#endif
void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_HX711"));
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && millis() - startTijd < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  const bool parentOK = Native.aanmelden() && Native.controleren() && Native.inpluggen() && Native.activeren();
  GA_SERIAL.println(parentOK ? F("PARENT: OK") : F("PARENT: FOUT"));
  if (!parentOK) return;
  SensorHX711* sensor = GedeeldeBusNewComponent<SensorHX711>(&Native, GedeeldeBusComponent::HX711, HardwareResourcePin::NP_D2, HardwareResourcePin::NP_D3);
  GA_SERIAL.println(sensor ? F("LIFECYCLE: OK") : F("LIFECYCLE: FOUT"));
  GA_SERIAL.println(F("Compilatie en hardwarewerking afzonderlijk controleren."));
}
void loop() {}
