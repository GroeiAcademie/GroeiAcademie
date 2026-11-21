// ============================================================================
// Sensor: DHTxx
// ============================================================================
// DATA=D2, DHT11 (KY-015) of DHT22 en pull-up correct aansluiten.
// ============================================================================

#include <GroeiAcademie.h>
#if SENSOR_DHT11_AANTAL + SENSOR_DHT22_AANTAL != 1
#error Zet SENSOR_DHT11_AANTAL of SENSOR_DHT22_AANTAL op 1 (niet beide) voor deze hardwaretest.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE && DATA_LAYER_CONFIG != DATA_TYPE_DIRECT
#error DATA_LAYER_CONFIG moet DATA_TYPE_NONE of DATA_TYPE_DIRECT zijn voor deze test.
#endif
#if SENSOR_DHT22_AANTAL == 1
const GedeeldeBusComponent fysiekType = GedeeldeBusComponent::DHT22;
#else
const GedeeldeBusComponent fysiekType = GedeeldeBusComponent::DHT11;
#endif
DHTxx* sensor = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  #if defined(DEBUG)
  while (!GA_SERIAL) { ; }
  #else
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && millis() - startTijd < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  #endif
  GA_SERIAL.println(F("TEST: Sensor_DHTxx"));
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  if (Data == nullptr) Data = GedeeldeBusNewComponent<struct Data>(DATA_LAYER_CONFIG);
  GA_SERIAL.println(Data ? F("DATA: OK") : F("DATA: FOUT"));
  if (Data == nullptr) return;
#else
  GA_SERIAL.println(F("LET OP: extra meetkanalen vereisen DATA_TYPE_DIRECT in UserConfig.h."));
#endif
  const bool parentOK = Native.aanmelden() && Native.controleren() && Native.inpluggen() && Native.activeren();
  GA_SERIAL.println(parentOK ? F("PARENT: OK") : F("PARENT: FOUT"));
  if (!parentOK) return;
  sensor = GedeeldeBusNewComponent<DHTxx>(&Native, fysiekType, HardwareResourcePin::NP_D2);
  GA_SERIAL.println(sensor ? F("LIFECYCLE: OK") : F("LIFECYCLE: FOUT"));
  GA_SERIAL.println(F("Compilatie en hardwarewerking afzonderlijk controleren."));
}
void loop() {
  if (sensor == nullptr) return;
  float waarde = NAN;
  if (!sensor->LeesHoofdwaarde(waarde)) return;
  static unsigned long laatsteRapport = 0;
  if (millis() - laatsteRapport < 250UL) return;
  laatsteRapport = millis();
  GA_SERIAL.print(F("Temperatuur (C): "));
  GA_SERIAL.println(waarde);
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  GA_SERIAL.print(F("Via Data, temperatuur (C): "));
  GA_SERIAL.println(sensor->ReadQueueOrStackFloat(1));
  GA_SERIAL.print(F("Via Data, relatieve vochtigheid (%RH): "));
  GA_SERIAL.println(sensor->ReadQueueOrStackFloat(2));
#endif
}
