// Standaard 256000 baud: uitsluitend op een board met geschikte Serial1 en foutloze baudrate.
#include <GroeiAcademie.h>
#if SENSOR_LD2450_AANTAL != 1
#error SENSOR_LD2450_AANTAL moet 1 zijn.
#endif
#if !defined(HAVE_HWSERIAL1) && !defined(ARDUINO_ARCH_RENESAS)
#error LD2450-hardwaretest vereist Serial1 en ondersteuning voor 256000 baud.
#endif
void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && millis() - startTijd < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  Serial1.begin(256000);
  bool basis = Native.aanmelden() && Native.controleren() && Native.inpluggen() && Native.activeren();
  if (!basis) { GA_SERIAL.println(F("PARENT: FOUT")); return; }
  LD2450* sensor = GedeeldeBusNewComponent<LD2450>(&Native, GedeeldeBusComponent::LD2450, Serial1, HardwareResourcePin::NP_D1, HardwareResourcePin::NP_D0);
  GA_SERIAL.println(sensor ? F("INITIALISATIE: OK; SENSORRESPONS: niet bewezen (controleer echte radarframes)") : F("LIFECYCLE: FOUT"));
}
void loop() {}
