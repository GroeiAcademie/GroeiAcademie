// Sensor: HC-SR04 via MCP23017: TRIG <- GPA0, ECHO -> GPA1; INTA van de MCP23017 op een native interruptpin (EXTENDER_MCP23017_1_INTA).
// Activeren: TRIG laag als uitgang, ECHO als ingang. De ECHO-timing via INTA hoort bij de functies. Zonder INTA faalt inpluggen.
// Test tot en met de meting in loop(). Compilatie en hardwarewerking zijn nog niet bewezen.

#include <GroeiAcademie.h>

#if SENSOR_HCSR04_AANTAL != 1
  #error Zet SENSOR_HCSR04_AANTAL op 1 voor deze test.
#endif
#if EXTENDER_MCP23017_AANTAL == 0
  #error Zet EXTENDER_MCP23017_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE voor deze test.
#endif

#if EXTENDER_MCP23017_AANTAL == 1
ExtenderMCP23017 extender(
  &Native, GedeeldeBusComponent::DIGITAL_PINS_MCP23017,
  I2C_ADDRESS_EXTENDER_MCP23017_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, EXTENDER_MCP23017_1_INTA, EXTENDER_MCP23017_1_INTB, EXTENDER_MCP23017_1_RESET
);
#else
ExtenderMCP23017 extender(
  &Native, GedeeldeBusComponent::DIGITAL_PINS_MCP23017,
  HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, 0
);
#endif

HSR0x* sensor = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_HSR0x_MCP23017"));
  const bool gelukt = extender.aanmelden() && extender.controleren() && extender.inpluggen() && extender.activeren();
  GA_SERIAL.println(gelukt ? F("EXTENDERS: OK") : F("EXTENDERS: FOUT"));
  if (!gelukt) return;
  sensor = GedeeldeBusNewComponent<HSR0x>(&extender, GedeeldeBusComponent::HCSR04, static_cast<HardwareResourcePin>(static_cast<uint8_t>(ExtenderMCP23017::ExtenderPins::EP_GPA0)), static_cast<HardwareResourcePin>(static_cast<uint8_t>(ExtenderMCP23017::ExtenderPins::EP_GPA1)));
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
