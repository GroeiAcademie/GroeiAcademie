// Sensor: MAX30102 via TCA9548A (I2C, kanaal 0) + MCP23S17 (INT): MIXED.
// Bestaande FrameWork-API; mixed-patroon zoals Sturing_MAX7219_PCF8574_MIXED (ownerClaim, viaMixed).
// Bedrading: SDA/SCL van de sensor op TCA9548A kanaal 0; INT -> MCP23S17 EP_GPA0.
// MCP23S17: SPI (SCK/MISO/MOSI, CS). Voeding, GND en logicaniveaus fysiek controleren.
// Test tot en met de meting in loop(). Compilatie en hardwarewerking zijn nog niet bewezen.

#include <GroeiAcademie.h>

#if SENSOR_MAX30102_AANTAL != 1
  #error Zet SENSOR_MAX30102_AANTAL op 1 voor deze test.
#endif
#if EXTENDER_TCA9548A_AANTAL == 0
  #error Zet EXTENDER_TCA9548A_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif
#if EXTENDER_MCP23S17_AANTAL == 0 || !GEDEELDE_BUS_MIXED_PINS
  #error Deze test vereist EXTENDER_MCP23S17_AANTAL > 0 en GEDEELDE_BUS_MIXED_PINS.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE voor deze test.
#endif

#if EXTENDER_TCA9548A_AANTAL == 1
ExtenderTCA9548A extender(
  &Native,
  GedeeldeBusComponent::I2C_MULTIPLEXER_TCA9548A,
  I2C_ADDRESS_EXTENDER_TCA9548A_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, EXTENDER_TCA9548A_1_RESET
);
#else
ExtenderTCA9548A extender(
  &Native,
  GedeeldeBusComponent::I2C_MULTIPLEXER_TCA9548A,
  HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, 0
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

HardwareResourceTypeI2C apparaat(&extender, GedeeldeBusComponent::MAX30102, I2C_ADDRESS_SENSOR_MAX30102_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, HardwareResourceToegang::GEDEELD, 0);
MAX30102* sensor = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_MAX30102_TCA9548A_MCP23S17_MIXED"));
  const bool gelukt = extender.aanmelden() && extender.controleren() && extender.inpluggen() && extender.activeren() &&
                      extenderMCP23S17.aanmelden() && extenderMCP23S17.controleren() && extenderMCP23S17.inpluggen() && extenderMCP23S17.activeren();
  GA_SERIAL.println(gelukt ? F("EXTENDERS: OK") : F("EXTENDERS: FOUT"));
  if (!gelukt) return;
  const bool parentOK = apparaat.aanmelden() && apparaat.controleren() && apparaat.inpluggen() && apparaat.activeren();
  GA_SERIAL.println(parentOK ? F("I2C: OK") : F("I2C: FOUT"));
  if (!parentOK) return;
  sensor = GedeeldeBusNewComponent<MAX30102>(&apparaat, GedeeldeBusComponent::MAX30102, &extenderMCP23S17, static_cast<uint8_t>(ExtenderMCP23S17::ExtenderPins::EP_GPA0));
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
      if (sensor->ReadINT(toestand)) {
        GA_SERIAL.print(F("INT: "));
        GA_SERIAL.println(toestand);
      } else {
        GA_SERIAL.println(F("INT: UITLEESFOUT"));
      }
    }
  }
  float waarde = NAN;
  if (sensor->LeesHoofdwaarde(waarde)) {
    // Sensor blijft uitlezen; alleen de Serial-uitvoer wordt beperkt.
    static unsigned long laatsteRapport = 0;
    if (millis() - laatsteRapport < 200UL) return;
    laatsteRapport = millis();
    GA_SERIAL.print(F("IR (ruwe telling, geen SpO2): "));
    GA_SERIAL.println(waarde);
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
    GA_SERIAL.print(F("IR (ruwe telling): "));
    GA_SERIAL.println(sensor->ReadQueueOrStackUInt32(1));
    GA_SERIAL.print(F("Rood (ruwe telling): "));
    GA_SERIAL.println(sensor->ReadQueueOrStackUInt32(2));
#endif
    GA_SERIAL.print(F("Verloren FIFO-samples (driver): "));
    GA_SERIAL.println(sensor->aantalVerlorenSamples);
  }
  // Een false-resultaat kan ook betekenen: conversie nog bezig of geen verse sample.
  // Geen delay: de bestaande FIFO bevat slechts een beperkt aantal samples.
}
