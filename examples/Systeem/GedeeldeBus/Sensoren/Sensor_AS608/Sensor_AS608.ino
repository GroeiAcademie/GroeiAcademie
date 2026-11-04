// ============================================================================
// Sensor: AS608
// ============================================================================
// Hardwaretest voor AS608 binnen de bestaande Sensor-architectuur.
// Configuratie: SENSOR_AS608_AANTAL moet 1 zijn.
// Configuratie: Hardwaretest AS608 vereist Serial1. Gebruik op andere boards een apart goedgekeurde seriele poortintegratie.
// Serial meldt initialisatie; een fysieke sensorrespons is niet automatisch bewezen.
// Fysieke UART op UNO R4 Minima: sensor TX -> D0/RX (Serial1); sensor RX -> D1/TX (Serial1).
// GND gemeenschappelijk; voeding en UART-logicaniveau volgens de concrete sensormodule.
// Op andere boards eerst de werkelijke Serial1-pinnen in het bestaande boardprofiel controleren.
// ============================================================================

#include <GroeiAcademie.h>
#if SENSOR_AS608_AANTAL != 1
#error SENSOR_AS608_AANTAL moet 1 zijn.
#endif
#if !defined(HAVE_HWSERIAL1) && !defined(ARDUINO_ARCH_RENESAS)
#error Hardwaretest AS608 vereist Serial1. Gebruik op andere boards een apart goedgekeurde seriele poortintegratie.
#endif
void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_AS608"));
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && millis() - startTijd < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  bool basis = Native.aanmelden() && Native.controleren() && Native.inpluggen() && Native.activeren();
  if (!basis) { GA_SERIAL.println(F("PARENT: FOUT")); return; }
  AS608* sensor = GedeeldeBusNewComponent<AS608>(&Native, GedeeldeBusComponent::AS608, &Serial1, HardwareResourcePin::NP_D1, HardwareResourcePin::NP_D0);
  GA_SERIAL.println(sensor ? F("INITIALISATIE: OK; SENSORRESPONS: niet bewezen") : F("LIFECYCLE: FOUT"));
}
void loop() {}
