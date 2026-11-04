// ============================================================================
// Sensor: MR60BHA2
// ============================================================================
// MR60BHA2 gebruikt de FrameWork-Stream-driver, ook voor niet-ESP32-borden.
// Aansluiting: radar TX -> MCU RX, radar RX -> MCU TX; GND gemeenschappelijk.
// Voeding volgens de officiele datasheet van de concrete module.
// ============================================================================

#include <GroeiAcademie.h>
#if SENSOR_MR60BHA2_AANTAL != 1
#error Zet SENSOR_MR60BHA2_AANTAL op 1 voor deze hardwaretest.
#endif
MR60BHA2* sensorMR60BHA2 = nullptr;
void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_MR60BHA2"));
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && millis() - startTijd < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  // Gebruik de bestaande Serial1; FrameWork initialiseert deze op 115200 baud.
  // De definitieve native TX/RX-pintoewijzing blijft afhankelijk van het board.
  sensorMR60BHA2 = GedeeldeBusNewComponent<MR60BHA2>(
      &Native, GedeeldeBusComponent::MR60BHA2, &Serial1, HardwareResourcePin::NP_D1, HardwareResourcePin::NP_D0);
  GA_SERIAL.println(sensorMR60BHA2 ? F("INITIALISATIE: component aangemaakt; SENSORRESPONS: niet bewezen") : F("LIFECYCLE: FOUT"));
}
void loop() {}
