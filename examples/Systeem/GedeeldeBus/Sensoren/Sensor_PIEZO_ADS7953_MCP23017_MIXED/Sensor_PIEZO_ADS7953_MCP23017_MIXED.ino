// Sensor: PIEZO via ADS7953 + MCP23017: signaal -> ADS7953 CH0, DO (comparator) -> MCP23017 GPA0 (MIXED).
// Stroom van de hulppin fysiek controleren tegen de uitgangsstroom van de Extender.
// Test tot en met de meting in loop(). Compilatie en hardwarewerking zijn nog niet bewezen.

#include <GroeiAcademie.h>

#if SENSOR_PIEZO_AANTAL != 1
  #error Zet SENSOR_PIEZO_AANTAL op 1 voor deze test.
#endif
#if EXTENDER_ADS7953_AANTAL == 0
  #error Zet EXTENDER_ADS7953_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif
#if EXTENDER_MCP23017_AANTAL == 0
  #error Zet EXTENDER_MCP23017_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif
#if !GEDEELDE_BUS_MIXED_PINS
  #error Zet GEDEELDE_BUS_MIXED_PINS op 1 in UserConfig.h voor deze test.
#endif
#if EXTENDER_ADS7953_VOEDING_MILLIVOLT == 0
  #error Zet EXTENDER_ADS7953_VOEDING_MILLIVOLT op de werkelijke +VA (2700..5250 mV): het bereik van deze sensor vraagt 2xVREF.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE voor deze test.
#endif

#if EXTENDER_ADS7953_AANTAL == 1
ExtenderADS7953 extender(
  &Native,
  GedeeldeBusComponent::ADC_ADS7953,
  EXTENDER_ADS7953_1_CS_PIN, HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI
);
#else
ExtenderADS7953 extender(
  &Native,
  GedeeldeBusComponent::ADC_ADS7953,
  HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, 0
);
#endif

#if EXTENDER_MCP23017_AANTAL == 1
ExtenderMCP23017 extenderMCP23017(
  &Native, GedeeldeBusComponent::DIGITAL_PINS_MCP23017,
  I2C_ADDRESS_EXTENDER_MCP23017_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, EXTENDER_MCP23017_1_INTA, EXTENDER_MCP23017_1_INTB, EXTENDER_MCP23017_1_RESET
);
#else
ExtenderMCP23017 extenderMCP23017(
  &Native, GedeeldeBusComponent::DIGITAL_PINS_MCP23017,
  HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, 0
);
#endif

PIEZO* sensor = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_PIEZO_ADS7953_MCP23017_MIXED"));
  const bool gelukt = extender.aanmelden() && extender.controleren() && extender.inpluggen() && extender.activeren() &&
                      extenderMCP23017.aanmelden() && extenderMCP23017.controleren() && extenderMCP23017.inpluggen() && extenderMCP23017.activeren();
  GA_SERIAL.println(gelukt ? F("EXTENDERS: OK") : F("EXTENDERS: FOUT"));
  if (!gelukt) return;
  sensor = GedeeldeBusNewComponent<PIEZO>(&extender, GedeeldeBusComponent::PIEZO, static_cast<uint8_t>(ExtenderADS7953::ExtenderPins::EP_CH0), &extenderMCP23017, static_cast<uint8_t>(ExtenderMCP23017::ExtenderPins::EP_GPA0), HardwareResourceToegang::GEDEELD);
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
