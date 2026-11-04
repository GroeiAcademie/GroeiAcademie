// ============================================================================
// Sensor: LD2410D
// ============================================================================
// Fabrikantdocument: standaard 115200 baud (NIET 256000 van andere LD2410-varianten).
// ============================================================================

#include <GroeiAcademie.h>
#if SENSOR_LD2410D_AANTAL != 1
#error SENSOR_LD2410D_AANTAL moet 1 zijn.
#endif
#if !defined(HAVE_HWSERIAL1) && !defined(ARDUINO_ARCH_RENESAS)
#error LD2410D-hardwaretest vereist Serial1.
#endif
void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_LD2410D"));
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && millis() - startTijd < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  Serial1.begin(115200);
  bool basis = Native.aanmelden() && Native.controleren() && Native.inpluggen() && Native.activeren();
  if (!basis) { GA_SERIAL.println(F("PARENT: FOUT")); return; }
  LD2410D* sensor = GedeeldeBusNewComponent<LD2410D>(&Native, GedeeldeBusComponent::LD2410D, Serial1, HardwareResourcePin::NP_D1, HardwareResourcePin::NP_D0);
  GA_SERIAL.println(sensor ? F("COMPONENT: AANGEMAAKT; verifieer Activeren()") : F("COMPONENT: FOUT"));
}
void loop() {}
