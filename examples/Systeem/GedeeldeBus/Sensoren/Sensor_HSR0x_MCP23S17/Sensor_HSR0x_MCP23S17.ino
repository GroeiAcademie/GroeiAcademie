// Sensor: HC-SR04 via MCP23S17: TRIG <- GPA0, ECHO -> GPA1; INTA van de MCP23S17 op native D2.
// Activeren: TRIG laag als uitgang, ECHO als ingang. De ECHO-timing via INTA hoort bij de functies.
// Test tot en met de meting in loop(). Compilatie en hardwarewerking zijn nog niet bewezen.

#include <GroeiAcademie.h>

#if SENSOR_HCSR04_AANTAL != 1
  #error Zet SENSOR_HCSR04_AANTAL op 1 voor deze test.
#endif
#if EXTENDER_MCP23S17_AANTAL != 1
  #error Zet EXTENDER_MCP23S17_AANTAL op 1 voor deze test (constructor met INTA).
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE voor deze test.
#endif

#if EXTENDER_MCP23S17_AANTAL == 1
// INTA van de MCP23S17 op native D2 (interruptpin), geclaimd door de Extender.
ExtenderMCP23S17 extender(
  &Native, GedeeldeBusComponent::DIGITAL_PINS_MCP23S17,
  EXTENDER_MCP23S17_1_CS_PIN, HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, HardwareResourcePin::NP_D2
);
#endif

HSR0x* sensor = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_HSR0x_MCP23S17"));
  const bool gelukt = extender.aanmelden() && extender.controleren() && extender.inpluggen() && extender.activeren();
  GA_SERIAL.println(gelukt ? F("EXTENDERS: OK") : F("EXTENDERS: FOUT"));
  if (!gelukt) return;
  sensor = GedeeldeBusNewComponent<HSR0x>(&extender, GedeeldeBusComponent::HCSR04, static_cast<HardwareResourcePin>(static_cast<uint8_t>(ExtenderMCP23S17::ExtenderPins::EP_GPA0)), static_cast<HardwareResourcePin>(static_cast<uint8_t>(ExtenderMCP23S17::ExtenderPins::EP_GPA1)));
  GA_SERIAL.println(sensor ? F("LIFECYCLE: OK") : F("LIFECYCLE: FOUT"));
}

void loop() {
  if (sensor == nullptr) return;
  // De fabrikant vraagt minstens 60 ms tussen twee metingen.
  static unsigned long lastMeasurementStartMS = 0;
  if (millis() - lastMeasurementStartMS < 100UL) return;
  lastMeasurementStartMS = millis();
  float waarde = NAN;
  if (!sensor->LeesHoofdwaarde(waarde)) return;
  static unsigned long laatsteRapport = 0;
  if (millis() - laatsteRapport < 250UL) return;
  laatsteRapport = millis();
  GA_SERIAL.print(F("Afstand (mm): "));
  GA_SERIAL.println(waarde);
}
