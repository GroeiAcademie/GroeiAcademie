// ============================================================================
// Sensor: MR60BHA2
// ============================================================================
// MR60BHA2 gebruikt de FrameWork-Stream-driver, ook voor niet-ESP32-borden.
// Aansluiting volgens constructor: radar TX -> Native D0/RX (Serial1),
// radar RX -> Native D1/TX (Serial1), GND gemeenschappelijk.
// UNO R4 Minima/WiFi: fysieke header D0/RX en D1/TX (Serial1).
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
  // Op andere boards blijven de fysieke GPIO-nummers afhankelijk van de gekozen boardmapping.
  sensorMR60BHA2 = GedeeldeBusNewComponent<MR60BHA2>(
      &Native, GedeeldeBusComponent::MR60BHA2, &Serial1, HardwareResourcePin::NP_D1, HardwareResourcePin::NP_D0);
  GA_SERIAL.println(sensorMR60BHA2 ? F("INITIALISATIE: component aangemaakt; SENSORRESPONS: niet bewezen") : F("LIFECYCLE: FOUT"));
}
void loop() {}
