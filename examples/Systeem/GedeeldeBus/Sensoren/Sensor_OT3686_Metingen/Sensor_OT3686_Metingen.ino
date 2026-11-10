// ============================================================================
// Sensor: OT3686_Metingen
// ============================================================================
// Hardwaretest voor OT3686_Metingen binnen de bestaande Sensor-architectuur.
// Configuratie: Zet SENSOR_OT3686_AANTAL op 1, SENSOR_OT3686_AANTAL_SLEUVEN op jouw wiel.
// Serial toont de pulstelling en de bestaande RPM-metingen.
// Signaaluitgang van de sensor -> fysieke Arduino D2 (NP_D2); GND gemeenschappelijk.
// VCC en eventuele levelshifting volgens de concrete module-uitvoering.
// ============================================================================

#include <GroeiAcademie.h>
#if SENSOR_OT3686_AANTAL == 0
#error Zet SENSOR_OT3686_AANTAL op 1, SENSOR_OT3686_AANTAL_SLEUVEN op jouw wiel.
#endif
OT3686* sensorSnelheid = nullptr;
void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  #if defined(DEBUG)
  while (!GA_SERIAL) { ; }
  #else
  unsigned long startTijd = millis();
  while (!GA_SERIAL && (millis() - startTijd) < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  #endif
  GA_SERIAL.println(F("TEST: Sensor_OT3686_Metingen"));
  sensorSnelheid = GedeeldeBusNewComponent<OT3686>(&Native, GedeeldeBusComponent::OT3686, HardwareResourcePin::NP_D2);
  GA_SERIAL.println(sensorSnelheid ? F("OT3686: OK") : F("OT3686: FOUT"));
}
void loop() {
  if (!sensorSnelheid) return;
  float venster = NAN, periode = NAN;
  uint32_t aantal = 0;
  bool toestand = false;
  sensorSnelheid->LeesRPMViaTijdvenster(venster);
  static unsigned long laatsteRapport = 0;
  if (millis() - laatsteRapport < 250) return;
  laatsteRapport = millis();
  sensorSnelheid->LeesRPMViaPeriode(periode);
  sensorSnelheid->LeesAantalPulsen(aantal);
  sensorSnelheid->LeesDigitaleToestand(toestand);
  GA_SERIAL.print(F("DO=")); GA_SERIAL.print(toestand);
  GA_SERIAL.print(F(" pulsen=")); GA_SERIAL.print(aantal);
  GA_SERIAL.print(F(" RPM_venster=")); GA_SERIAL.print(venster);
  GA_SERIAL.print(F(" RPM_periode=")); GA_SERIAL.println(periode);
}
