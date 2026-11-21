// Sensor: HW201 via MCP23017 + ADS1158: OUT (DO) -> MCP23017 GPA0, AO -> ADS1158 AIN0 (MIXED).
// Test tot en met de meting in loop(). Compilatie en hardwarewerking zijn nog niet bewezen.

#include <GroeiAcademie.h>

#if SENSOR_HW201_AANTAL != 1
  #error Zet SENSOR_HW201_AANTAL op 1 voor deze test.
#endif
#if EXTENDER_MCP23017_AANTAL == 0
  #error Zet EXTENDER_MCP23017_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif
#if EXTENDER_ADS1158_AANTAL == 0
  #error Zet EXTENDER_ADS1158_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif
#if !GEDEELDE_BUS_MIXED_PINS
  #error Zet GEDEELDE_BUS_MIXED_PINS op 1 in UserConfig.h voor deze test.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE voor deze test.
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

#if EXTENDER_ADS1158_AANTAL == 1
ExtenderADS1158 extenderADS1158(
  &Native,
  GedeeldeBusComponent::ADC_ADS1158,
  EXTENDER_ADS1158_1_CS_PIN, HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, EXTENDER_ADS1158_1_START, EXTENDER_ADS1158_1_RESET, EXTENDER_ADS1158_1_PWDN
);
#else
ExtenderADS1158 extenderADS1158(
  &Native,
  GedeeldeBusComponent::ADC_ADS1158,
  HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, 0
);
#endif

HW201* sensor = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_HW201_MCP23017_ADS1158_MIXED"));
  const bool gelukt = extenderMCP23017.aanmelden() && extenderMCP23017.controleren() && extenderMCP23017.inpluggen() && extenderMCP23017.activeren() &&
                      extenderADS1158.aanmelden() && extenderADS1158.controleren() && extenderADS1158.inpluggen() && extenderADS1158.activeren();
  GA_SERIAL.println(gelukt ? F("EXTENDERS: OK") : F("EXTENDERS: FOUT"));
  if (!gelukt) return;
  sensor = GedeeldeBusNewComponent<HW201>(&extenderMCP23017, GedeeldeBusComponent::HW201, static_cast<HardwareResourcePin>(static_cast<uint8_t>(ExtenderMCP23017::ExtenderPins::EP_GPA0)), &extenderADS1158, static_cast<uint8_t>(ExtenderADS1158::ExtenderPins::EP_AIN0));
  GA_SERIAL.println(sensor ? F("LIFECYCLE: OK") : F("LIFECYCLE: FOUT"));
}

void loop() {
  if (sensor == nullptr) return;
  static unsigned long laatsteRapport = 0;
  if (millis() - laatsteRapport < 200UL) return;
  laatsteRapport = millis();
  int32_t analogOutputAO = 0;
  if (sensor->RawAnalogReadAnalogOutputAO(analogOutputAO)) {
    GA_SERIAL.print(F("AO ruwe ADC: "));
    GA_SERIAL.println(analogOutputAO);
  } else {
    GA_SERIAL.println(F("AO: UITLEESFOUT"));
  }
  float waarde = NAN;
  if (sensor->LeesHoofdwaarde(waarde)) {
    GA_SERIAL.print(F("Detectie OUT (0/1): "));
    GA_SERIAL.println(waarde);
  } else {
    GA_SERIAL.println(F("Detectie OUT: UITLEESFOUT"));
  }
}
