#include <GroeiAcademie.h>
#if SENSOR_OT3686_AANTAL == 0
#error Zet SENSOR_OT3686_AANTAL op 1, SENSOR_OT3686_AANTAL_SLEUVEN op jouw wiel.
#endif
OT3686* sensorSnelheid = nullptr;
void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
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
