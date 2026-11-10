// ============================================================================
// Sensor: DHTxx
// ============================================================================
// DATA=D2, DHT11 (KY-015) of DHT22 en pull-up correct aansluiten.
// ============================================================================

#include <GroeiAcademie.h>
#if SENSOR_DHT11_AANTAL + SENSOR_DHT22_AANTAL != 1
#error Zet SENSOR_DHT11_AANTAL of SENSOR_DHT22_AANTAL op 1 (niet beide) voor deze hardwaretest.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
#error DATA_LAYER_CONFIG moet DATA_TYPE_NONE zijn voor deze test.
#endif
#if SENSOR_DHT22_AANTAL == 1
const GedeeldeBusComponent fysiekType = GedeeldeBusComponent::DHT22;
#else
const GedeeldeBusComponent fysiekType = GedeeldeBusComponent::DHT11;
#endif
void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  #if defined(DEBUG)
  while (!GA_SERIAL) { ; }
  #else
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && millis() - startTijd < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  #endif
  GA_SERIAL.println(F("TEST: Sensor_DHTxx"));
  const bool parentOK = Native.aanmelden() && Native.controleren() && Native.inpluggen() && Native.activeren();
  GA_SERIAL.println(parentOK ? F("PARENT: OK") : F("PARENT: FOUT"));
  if (!parentOK) return;
  DHTxx* sensor = GedeeldeBusNewComponent<DHTxx>(&Native, fysiekType, HardwareResourcePin::NP_D2);
  GA_SERIAL.println(sensor ? F("LIFECYCLE: OK") : F("LIFECYCLE: FOUT"));
  GA_SERIAL.println(F("Compilatie en hardwarewerking afzonderlijk controleren."));
}
void loop() {}
