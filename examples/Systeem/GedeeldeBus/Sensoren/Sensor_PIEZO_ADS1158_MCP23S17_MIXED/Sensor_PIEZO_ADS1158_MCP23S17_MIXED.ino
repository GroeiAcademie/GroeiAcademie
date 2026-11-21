// Sensor: PIEZO via ADS1158 + MCP23S17: signaal -> ADS1158 AIN0, DO (comparator) -> MCP23S17 GPA0 (MIXED).
// Stroom van de hulppin fysiek controleren tegen de uitgangsstroom van de Extender.
// Test tot en met de meting in loop(). Compilatie en hardwarewerking zijn nog niet bewezen.

#include <GroeiAcademie.h>

#if SENSOR_PIEZO_AANTAL != 1
  #error Zet SENSOR_PIEZO_AANTAL op 1 voor deze test.
#endif
#if EXTENDER_ADS1158_AANTAL == 0
  #error Zet EXTENDER_ADS1158_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif
#if EXTENDER_MCP23S17_AANTAL == 0
  #error Zet EXTENDER_MCP23S17_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif
#if !GEDEELDE_BUS_MIXED_PINS
  #error Zet GEDEELDE_BUS_MIXED_PINS op 1 in UserConfig.h voor deze test.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE voor deze test.
#endif

#if EXTENDER_ADS1158_AANTAL == 1
ExtenderADS1158 extender(
  &Native,
  GedeeldeBusComponent::ADC_ADS1158,
  EXTENDER_ADS1158_1_CS_PIN, HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, EXTENDER_ADS1158_1_START, EXTENDER_ADS1158_1_RESET, EXTENDER_ADS1158_1_PWDN
);
#else
ExtenderADS1158 extender(
  &Native,
  GedeeldeBusComponent::ADC_ADS1158,
  HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, 0
);
#endif

#if EXTENDER_MCP23S17_AANTAL == 1
ExtenderMCP23S17 extenderMCP23S17(
  &Native, GedeeldeBusComponent::DIGITAL_PINS_MCP23S17,
  EXTENDER_MCP23S17_1_CS_PIN, HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI
);
#else
ExtenderMCP23S17 extenderMCP23S17(
  &Native, GedeeldeBusComponent::DIGITAL_PINS_MCP23S17,
  HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, 0
);
#endif

PIEZO* sensor = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_PIEZO_ADS1158_MCP23S17_MIXED"));
  const bool gelukt = extender.aanmelden() && extender.controleren() && extender.inpluggen() && extender.activeren() &&
                      extenderMCP23S17.aanmelden() && extenderMCP23S17.controleren() && extenderMCP23S17.inpluggen() && extenderMCP23S17.activeren();
  GA_SERIAL.println(gelukt ? F("EXTENDERS: OK") : F("EXTENDERS: FOUT"));
  if (!gelukt) return;
  sensor = GedeeldeBusNewComponent<PIEZO>(&extender, GedeeldeBusComponent::PIEZO, static_cast<uint8_t>(ExtenderADS1158::ExtenderPins::EP_AIN0), &extenderMCP23S17, static_cast<uint8_t>(ExtenderMCP23S17::ExtenderPins::EP_GPA0), HardwareResourceToegang::GEDEELD);
  GA_SERIAL.println(sensor ? F("LIFECYCLE: OK") : F("LIFECYCLE: FOUT"));
}

void loop() {
  if (sensor == nullptr) return;
  // Hulppin(nen) via de tweede Extender.
  {
    static unsigned long laatsteRapport = 0;
    if (millis() - laatsteRapport >= 500UL) {
      laatsteRapport = millis();
      bool toestand = false;
      if (sensor->ReadDO(toestand)) {
        GA_SERIAL.print(F("DO: "));
        GA_SERIAL.println(toestand);
      } else {
        GA_SERIAL.println(F("DO: UITLEESFOUT"));
      }
    }
  }
  int32_t waarde = 0;
  if (sensor->RawAnalogRead(waarde)) {
    GA_SERIAL.print(F("PIEZO ruwe ADC-sample (geen fysieke eenheid): "));
    GA_SERIAL.println(waarde);
  } else {
    GA_SERIAL.println(F("ADC-UITLEESFOUT: geen geldige nieuwe meetwaarde (bereik, referentie en verbinding controleren)."));
  }
  // Een geslaagde ruwe uitlezing bewijst nog geen kalibratie of correcte fysieke meetfrequentie.
}
