// ============================================================================
// Sensor: DS18B20
// ============================================================================
// Hardwaretest voor DS18B20 binnen de bestaande Sensor-architectuur.
// Configuratie: SENSOR_DS18B20_AANTAL moet 1 zijn voor deze hardwaretest.
// Configuratie: DATA_LAYER_CONFIG moet DATA_TYPE_NONE of DATA_TYPE_DIRECT zijn voor deze test.
// De sketch toont de bestaande parent- en Sensor-controles via Serial.
// DS18B20 DATA -> fysieke Arduino D2 (NP_D2), GND -> GND, VDD volgens de sensoruitvoering.
// Gebruik de pull-up voor de 1-Wire-datalijn volgens de officiële sensor-/breakoutspecificatie.
// ============================================================================

#include <GroeiAcademie.h>
#if SENSOR_DS18B20_AANTAL != 1
#error SENSOR_DS18B20_AANTAL moet 1 zijn voor deze hardwaretest.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE && DATA_LAYER_CONFIG != DATA_TYPE_DIRECT
#error DATA_LAYER_CONFIG moet DATA_TYPE_NONE of DATA_TYPE_DIRECT zijn voor deze test.
#endif

DS18B20* sensor = nullptr;

void setup() {
 GA_SERIAL.begin(SERIAL_BAUDRATE);
 #if defined(DEBUG)
 while (!GA_SERIAL) { ; }
 #else
 const unsigned long startTijd = millis();
 while (!GA_SERIAL && millis()-startTijd<SERIAL_CONNECT_TIMEOUT_MS) { ; }
 #endif
 GA_SERIAL.println(F("TEST: Sensor_DS18B20"));
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  if (Data == nullptr) Data = GedeeldeBusNewComponent<struct Data>(DATA_LAYER_CONFIG);
  GA_SERIAL.println(Data ? F("DATA: OK") : F("DATA: FOUT"));
  if (Data == nullptr) return;
#else
  GA_SERIAL.println(F("LET OP: extra meetkanalen vereisen DATA_TYPE_DIRECT in UserConfig.h."));
#endif
 bool parentOK = Native.aanmelden() && Native.controleren() && Native.inpluggen() && Native.activeren();
 GA_SERIAL.println(parentOK ? F("PARENT: OK") : F("PARENT: FOUT"));
 if (!parentOK) return;
 sensor = GedeeldeBusNewComponent<DS18B20>(&Native,GedeeldeBusComponent::DS18B20, HardwareResourcePin::NP_D2);
 GA_SERIAL.println(sensor ? F("LIFECYCLE: OK") : F("LIFECYCLE: FOUT"));
 
}
void loop() {
  if (sensor == nullptr) return;
  if (!sensor->temperatuurmetingGestart && !sensor->StartTemperatuurmeting()) return;
  if (!sensor->IsTemperatuurmetingKlaar()) return;
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
#endif
}
