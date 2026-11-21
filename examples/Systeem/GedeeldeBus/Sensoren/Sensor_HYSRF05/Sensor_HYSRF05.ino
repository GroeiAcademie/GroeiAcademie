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
HSR0x* sensor = nullptr;

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
  sensor = GedeeldeBusNewComponent<HSR0x>(&Native, GedeeldeBusComponent::HYSRF05, HardwareResourcePin::NP_D2, HardwareResourcePin::NP_D3);
  GA_SERIAL.println(sensor ? F("LIFECYCLE: OK") : F("LIFECYCLE: FOUT"));
  GA_SERIAL.println(F("LET OP: afstand alleen met aangesloten ECHO; minstens 100 ms tussen twee metingen."));
}
void loop() {
  if (sensor == nullptr) return;
  // De fabrikant vraagt minstens 60 ms tussen twee metingen.
  static unsigned long lastMeasurementStartMS = 0;
  if (millis() - lastMeasurementStartMS < 100UL) return;
  lastMeasurementStartMS = millis();
  float waarde = NAN;
  if (!sensor->LeesHoofdwaarde(waarde)) return;
  static unsigned long laatsteRapport = 0;
  if (millis() - laatsteRapport < 250UL) return;
  laatsteRapport = millis();
  GA_SERIAL.print(F("Afstand (mm): "));
  GA_SERIAL.println(waarde);
}
