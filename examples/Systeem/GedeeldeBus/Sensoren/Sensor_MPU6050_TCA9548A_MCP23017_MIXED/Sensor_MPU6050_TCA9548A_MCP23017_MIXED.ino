// Sensor: MPU6050 via TCA9548A (I2C, kanaal 0) + MCP23017 (INT): MIXED.
// Bestaande FrameWork-API; mixed-patroon zoals Sturing_MAX7219_PCF8574_MIXED (ownerClaim, viaMixed).
// Bedrading: SDA/SCL van de sensor op TCA9548A kanaal 0; INT -> MCP23017 EP_GPA0.
// MCP23017: SDA/SCL op de hoofdbus, niet achter de TCA9548A. Voeding, GND en logicaniveaus fysiek controleren.
// Test tot en met de meting in loop(). Compilatie en hardwarewerking zijn nog niet bewezen.

#include <GroeiAcademie.h>

#if SENSOR_MPU6050_AANTAL != 1
  #error Zet SENSOR_MPU6050_AANTAL op 1 voor deze test.
#endif
#if EXTENDER_TCA9548A_AANTAL == 0
  #error Zet EXTENDER_TCA9548A_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif
#if EXTENDER_MCP23017_AANTAL == 0 || !GEDEELDE_BUS_MIXED_PINS
  #error Deze test vereist EXTENDER_MCP23017_AANTAL > 0 en GEDEELDE_BUS_MIXED_PINS.
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

HardwareResourceTypeI2C apparaat(&extender, GedeeldeBusComponent::MPU6050, I2C_ADDRESS_SENSOR_MPU6050_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, HardwareResourceToegang::GEDEELD, 0);
SensorMPU6050* sensor = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_MPU6050_TCA9548A_MCP23017_MIXED"));
  const bool gelukt = extender.aanmelden() && extender.controleren() && extender.inpluggen() && extender.activeren() &&
                      extenderMCP23017.aanmelden() && extenderMCP23017.controleren() && extenderMCP23017.inpluggen() && extenderMCP23017.activeren();
  GA_SERIAL.println(gelukt ? F("EXTENDERS: OK") : F("EXTENDERS: FOUT"));
  if (!gelukt) return;
  const bool parentOK = apparaat.aanmelden() && apparaat.controleren() && apparaat.inpluggen() && apparaat.activeren();
  GA_SERIAL.println(parentOK ? F("I2C: OK") : F("I2C: FOUT"));
  if (!parentOK) return;
  sensor = GedeeldeBusNewComponent<SensorMPU6050>(&apparaat, GedeeldeBusComponent::MPU6050, &extenderMCP23017, static_cast<uint8_t>(ExtenderMCP23017::ExtenderPins::EP_GPA0));
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
    GA_SERIAL.print(F("Versnelling X (m/s2): "));
    GA_SERIAL.println(waarde);
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
    GA_SERIAL.print(F("Versnelling X (m/s2): "));
    GA_SERIAL.println(sensor->ReadQueueOrStackFloat(1));
    GA_SERIAL.print(F("Versnelling Y (m/s2): "));
    GA_SERIAL.println(sensor->ReadQueueOrStackFloat(2));
    GA_SERIAL.print(F("Versnelling Z (m/s2): "));
    GA_SERIAL.println(sensor->ReadQueueOrStackFloat(3));
    GA_SERIAL.print(F("Gyro X (rad/s): "));
    GA_SERIAL.println(sensor->ReadQueueOrStackFloat(4));
    GA_SERIAL.print(F("Gyro Y (rad/s): "));
    GA_SERIAL.println(sensor->ReadQueueOrStackFloat(5));
    GA_SERIAL.print(F("Gyro Z (rad/s): "));
    GA_SERIAL.println(sensor->ReadQueueOrStackFloat(6));
    GA_SERIAL.print(F("Temperatuur (C): "));
    GA_SERIAL.println(sensor->ReadQueueOrStackFloat(7));
#endif
  }
  // Een false-resultaat kan ook betekenen: conversie nog bezig of geen verse sample.
}
