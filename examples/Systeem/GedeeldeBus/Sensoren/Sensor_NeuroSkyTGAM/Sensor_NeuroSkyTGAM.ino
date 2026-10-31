#include <GroeiAcademie.h>
#if SENSOR_NEUROSKYTGAM_AANTAL != 1
#error SENSOR_NEUROSKYTGAM_AANTAL moet 1 zijn.
#endif
#if !defined(HAVE_HWSERIAL1) && !defined(ARDUINO_ARCH_RENESAS)
#error Hardwaretest TGAM vereist Serial1; voor andere boards eerst poortintegratie goedkeuren.
#endif
void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && millis() - startTijd < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  Serial1.begin(57600);
  bool basis = Native.aanmelden() && Native.controleren() && Native.inpluggen() && Native.activeren();
  if (!basis) { GA_SERIAL.println(F("PARENT: FOUT")); return; }
  NeuroSkyTGAM* sensor = GedeeldeBusNewComponent<NeuroSkyTGAM>(&Native, GedeeldeBusComponent::NeuroSkyTGAM, Serial1, HardwareResourcePin::NP_D1, HardwareResourcePin::NP_D0);
  GA_SERIAL.println(sensor ? F("INITIALISATIE: OK; SENSORRESPONS: niet bewezen") : F("LIFECYCLE: FOUT"));
}
void loop() {}
