// ============================================================================
// Sensor: HW201
// ============================================================================
// VCC/GND volgens concrete module; controleer OUT-niveau voor 3,3V-board.
// De sensor heeft geen bus-ID. Dit voorbeeld bevestigt GEEN echte detectie.
// ============================================================================

#include <GroeiAcademie.h>
#if SENSOR_HW201_AANTAL != 1
#error Zet SENSOR_HW201_AANTAL op 1 voor deze test.
#endif
HW201* sensorHW201 = nullptr;
void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  #if defined(DEBUG)
  while (!GA_SERIAL) { ; }
  #else
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && millis() - startTijd < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  #endif
  GA_SERIAL.println(F("TEST: Sensor_HW201"));
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  if (Data == nullptr) Data = GedeeldeBusNewComponent<struct Data>(DATA_LAYER_CONFIG);
  GA_SERIAL.println(Data ? F("DATA: OK") : F("DATA: FOUT"));
  if (Data == nullptr) return;
#else
  GA_SERIAL.println(F("LET OP: extra meetkanalen vereisen DATA_TYPE_DIRECT in UserConfig.h."));
#endif
  sensorHW201 = GedeeldeBusNewComponent<HW201>(&Native, GedeeldeBusComponent::HW201, HardwareResourcePin::NP_D2);
  GA_SERIAL.println(sensorHW201 ? F("HW201: component aangemaakt; controleer lifecycle en OUT tijdens hardwaretest") : F("HW201: FOUT"));
}
void loop() {
  if (sensorHW201 == nullptr) return;
  float waarde = NAN;
  if (!sensorHW201->LeesHoofdwaarde(waarde)) return;
  static unsigned long laatsteRapport = 0;
  if (millis() - laatsteRapport < 250UL) return;
  laatsteRapport = millis();
  GA_SERIAL.print(F("Detectiestatus (0/1): "));
  GA_SERIAL.println(waarde);
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  GA_SERIAL.print(F("Via Data, detectiestatus (0/1): "));
  GA_SERIAL.println(sensorHW201->ReadQueueOrStackFloat(1));
#endif
}
