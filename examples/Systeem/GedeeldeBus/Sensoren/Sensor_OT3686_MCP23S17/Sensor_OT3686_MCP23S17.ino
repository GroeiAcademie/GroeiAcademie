// Sensor: OT3686 via MCP23S17: DO -> GPA0 (ingang). Geen interrupt via deze Extender: pulsen tellen hoort bij de functies.
// Test tot en met de meting in loop(). Compilatie en hardwarewerking zijn nog niet bewezen.

#include <GroeiAcademie.h>

#if SENSOR_OT3686_AANTAL != 1
  #error Zet SENSOR_OT3686_AANTAL op 1 voor deze test.
#endif
#if EXTENDER_MCP23S17_AANTAL == 0
  #error Zet EXTENDER_MCP23S17_AANTAL in UserConfig.h op minstens 1 voor deze test.
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

OT3686* sensor = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_OT3686_MCP23S17"));
  const bool gelukt = extender.aanmelden() && extender.controleren() && extender.inpluggen() && extender.activeren();
  GA_SERIAL.println(gelukt ? F("EXTENDERS: OK") : F("EXTENDERS: FOUT"));
  if (!gelukt) return;
  sensor = GedeeldeBusNewComponent<OT3686>(&extender, GedeeldeBusComponent::OT3686, static_cast<HardwareResourcePin>(static_cast<uint8_t>(ExtenderMCP23S17::ExtenderPins::EP_GPA0)));
  GA_SERIAL.println(sensor ? F("LIFECYCLE: OK") : F("LIFECYCLE: FOUT"));
}

void loop() {
  if (!sensor) return;
  sensor->PollPulseCount();   // geen interrupt via deze Extender: zo vaak mogelijk de ingang lezen
  float venster = NAN, periode = NAN;
  uint32_t aantal = 0;
  bool toestand = false;
  sensor->LeesRPMViaTijdvenster(venster);
  static unsigned long laatsteRapport = 0;
  if (millis() - laatsteRapport < 250) return;
  laatsteRapport = millis();
  sensor->LeesRPMViaPeriode(periode);
  sensor->LeesAantalPulsen(aantal);
  sensor->LeesDigitaleToestand(toestand);
  GA_SERIAL.print(F("DO=")); GA_SERIAL.print(toestand);
  GA_SERIAL.print(F(" pulsen=")); GA_SERIAL.print(aantal);
  GA_SERIAL.print(F(" RPM_venster=")); GA_SERIAL.print(venster);
  GA_SERIAL.print(F(" RPM_periode=")); GA_SERIAL.println(periode);
}
