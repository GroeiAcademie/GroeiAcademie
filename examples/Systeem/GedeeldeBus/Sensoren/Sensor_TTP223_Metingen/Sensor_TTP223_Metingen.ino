// ============================================================================
// Sensor: TTP223_Metingen
// ============================================================================
// Hardwaretest voor TTP223_Metingen binnen de bestaande Sensor-architectuur.
// Configuratie: Zet SENSOR_TTP223_AANTAL op 1 in de bestaande configuratie om deze sketch te testen.
// Serial toont aanraakstatus, aantal gebeurtenissen en duur.
// ============================================================================

#include <GroeiAcademie.h>
#if SENSOR_TTP223_AANTAL == 0
#error Zet SENSOR_TTP223_AANTAL op 1 in de bestaande configuratie om deze sketch te testen.
#endif
TTP223* sensorAanraking = nullptr;
void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_TTP223_Metingen"));
  sensorAanraking = GedeeldeBusNewComponent<TTP223>(&Native, GedeeldeBusComponent::TTP223, HardwareResourcePin::NP_D2);
  GA_SERIAL.println(sensorAanraking ? F("TTP223 SENSOR: OK") : F("TTP223 SENSOR: FOUT"));
}
void loop() {
  if (!sensorAanraking) return;
  float status = NAN;
  uint32_t aantal = 0;
  unsigned long duur = 0;
  if (sensorAanraking->LeesHoofdwaarde(status)) {
    sensorAanraking->LeesAantalAanrakingen(aantal);
    sensorAanraking->LeesLaatsteAanraakduur(duur);
    static unsigned long laatsteRapport = 0;
    if (millis() - laatsteRapport >= 200) {
      laatsteRapport = millis();
      GA_SERIAL.print(F("aanraakstatus=")); GA_SERIAL.print(status);
      GA_SERIAL.print(F(" aantal=")); GA_SERIAL.print(aantal);
      GA_SERIAL.print(F(" duur_ms=")); GA_SERIAL.println(duur);
    }
  }
}
