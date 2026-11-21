// ============================================================================
// Sensor: HX711_Verwerking
// Verwerking: tarra en gewicht in gram (SENSOR_HX711_CALIBRATION_FACTOR).
// ============================================================================
// DOUT=D2, PD_SCK=D3, loadcell en module correct voeden.
// ============================================================================

#include <GroeiAcademie.h>
#if SENSOR_HX711_AANTAL != 1
#error SENSOR_HX711_AANTAL moet 1 zijn voor deze hardwaretest.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE && DATA_LAYER_CONFIG != DATA_TYPE_DIRECT
#error DATA_LAYER_CONFIG moet DATA_TYPE_NONE of DATA_TYPE_DIRECT zijn voor deze test.
#endif
SensorHX711* sensor = nullptr;
// Pas na een geslaagde tarra (sensor->tarraSucceeded) wordt het gewicht getoond.

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  #if defined(DEBUG)
  while (!GA_SERIAL) { ; }
  #else
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && millis() - startTijd < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  #endif
  GA_SERIAL.println(F("TEST: Sensor_HX711_Verwerking"));
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
  sensor = GedeeldeBusNewComponent<SensorHX711>(&Native, GedeeldeBusComponent::HX711, HardwareResourcePin::NP_D2, HardwareResourcePin::NP_D3);
  GA_SERIAL.println(sensor ? F("LIFECYCLE: OK") : F("LIFECYCLE: FOUT"));
  // Eén keer tarra bij de start, met een lege weegschaal.
  // Geen automatische nieuwe tarra later: ligt er dan iets op de weegschaal, dan zou dat ongemerkt het nulpunt worden.
  if (sensor) GA_SERIAL.println(sensor->Tare() ? F("Tarra: OK") : F("Tarra: FOUT"));
  GA_SERIAL.println(F("LET OP: stel SENSOR_HX711_CALIBRATION_FACTOR in; bij 0 komt er geen gewicht."));
  GA_SERIAL.println(F("Compilatie en hardwarewerking afzonderlijk controleren."));
}
void loop() {
  if (sensor == nullptr) return;
  static unsigned long laatsteRapport = 0;
  // Geen geldige tarra: mislukt bij de start, of vervallen na een runtime-herstel (Activeren() zet tarraSucceeded op false).
  // Geen gewicht en geen automatische nieuwe tarra, alleen een melding om de 500 ms.
  if (!sensor->tarraSucceeded) {
    if (millis() - laatsteRapport < 500UL) return;
    laatsteRapport = millis();
    GA_SERIAL.println(F("Tarra vereist: weegschaal leegmaken en het board resetten"));
    return;
  }
  float waarde = NAN;
  if (!sensor->ReadWeightGram(waarde)) return;
  if (millis() - laatsteRapport < 500UL) return;
  laatsteRapport = millis();
  GA_SERIAL.print(F("Gewicht (g): "));
  GA_SERIAL.println(waarde);
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  GA_SERIAL.print(F("Via Data, gewicht (g): "));
  GA_SERIAL.println(sensor->ReadQueueOrStackFloat(2));
#endif
}
