// Sensor: TTP223 via MCP23S17.
// OUT -> MCP23S17 EP_GPA0; voeding, gemeenschappelijke massa en logicaniveaus controleren.
// inpluggen() zet OUT als ingang via SetExtenderPinMode(), zoals SensorDigitaal (HW201, HCSR501).
// Geen fysiek ID: een geslaagde lifecycle bewijst niet dat de Sensor aangesloten is.
// Test tot en met de meting in loop(). Compilatie en hardwarewerking zijn nog niet bewezen.

#include <GroeiAcademie.h>

#if SENSOR_TTP223_AANTAL != 1
  #error Zet SENSOR_TTP223_AANTAL op 1 voor deze test.
#endif
#if EXTENDER_MCP23S17_AANTAL == 0
  #error Zet EXTENDER_MCP23S17_AANTAL op minstens 1 voor deze test.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE voor deze test.
#endif

#if EXTENDER_MCP23S17_AANTAL == 1
ExtenderMCP23S17 extender(
  &Native, GedeeldeBusComponent::DIGITAL_PINS_MCP23S17,
  EXTENDER_MCP23S17_1_CS_PIN, HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI
);
#else
ExtenderMCP23S17 extender(
  &Native, GedeeldeBusComponent::DIGITAL_PINS_MCP23S17,
  HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, 0
);
#endif

TTP223* sensor = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_TTP223_MCP23S17"));
  const bool parentOK = extender.aanmelden() && extender.controleren() && extender.inpluggen() && extender.activeren();
  GA_SERIAL.println(parentOK ? F("EXTENDER: OK") : F("EXTENDER: FOUT"));
  if (!parentOK) return;
  sensor = GedeeldeBusNewComponent<TTP223>(&extender, GedeeldeBusComponent::TTP223, static_cast<HardwareResourcePin>(static_cast<uint8_t>(ExtenderMCP23S17::ExtenderPins::EP_GPA0)));
  GA_SERIAL.println(sensor ? F("SENSOR: OK") : F("SENSOR: FOUT"));
}

void loop() {
  if (!sensor) return;
  float status = NAN;
  uint32_t aantal = 0;
  unsigned long duur = 0;
  if (sensor->LeesHoofdwaarde(status)) {
    sensor->LeesAantalAanrakingen(aantal);
    sensor->LeesLaatsteAanraakduur(duur);
    static unsigned long laatsteRapport = 0;
    if (millis() - laatsteRapport >= 200) {
      laatsteRapport = millis();
      GA_SERIAL.print(F("aanraakstatus=")); GA_SERIAL.print(status);
      GA_SERIAL.print(F(" aantal=")); GA_SERIAL.print(aantal);
      GA_SERIAL.print(F(" duur_ms=")); GA_SERIAL.println(duur);
    }
  }
}
