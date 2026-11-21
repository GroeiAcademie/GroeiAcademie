// Sensor: DS3231 via TCA9548A (I2C, kanaal 0) + MCP23S17 (INT/SQW): MIXED.
// Bestaande FrameWork-API; mixed-patroon zoals Sturing_MAX7219_PCF8574_MIXED (ownerClaim, viaMixed).
// Bedrading: SDA/SCL van de sensor op TCA9548A kanaal 0; INT/SQW -> MCP23S17 EP_GPA0.
// MCP23S17: SPI (SCK/MISO/MOSI, CS). Voeding, GND en logicaniveaus fysiek controleren.
// Test tot en met de meting in loop(). Compilatie en hardwarewerking zijn nog niet bewezen.

#include <GroeiAcademie.h>

#if SENSOR_DS3231_AANTAL != 1
  #error Zet SENSOR_DS3231_AANTAL op 1 voor deze test.
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

HardwareResourceTypeI2C apparaat(&extender, GedeeldeBusComponent::DS3231, I2C_ADDRESS_SENSOR_DS3231_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, HardwareResourceToegang::GEDEELD, 0);
DS3231* sensor = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_DS3231_TCA9548A_MCP23S17_MIXED"));
  const bool gelukt = extender.aanmelden() && extender.controleren() && extender.inpluggen() && extender.activeren() &&
                      extenderMCP23S17.aanmelden() && extenderMCP23S17.controleren() && extenderMCP23S17.inpluggen() && extenderMCP23S17.activeren();
  GA_SERIAL.println(gelukt ? F("EXTENDERS: OK") : F("EXTENDERS: FOUT"));
  if (!gelukt) return;
  const bool parentOK = apparaat.aanmelden() && apparaat.controleren() && apparaat.inpluggen() && apparaat.activeren();
  GA_SERIAL.println(parentOK ? F("I2C: OK") : F("I2C: FOUT"));
  if (!parentOK) return;
  sensor = GedeeldeBusNewComponent<DS3231>(&apparaat, GedeeldeBusComponent::DS3231, &extenderMCP23S17, static_cast<uint8_t>(ExtenderMCP23S17::ExtenderPins::EP_GPA0));
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
      if (sensor->ReadSQW(toestand)) {
        GA_SERIAL.print(F("SQW: "));
        GA_SERIAL.println(toestand);
      } else {
        GA_SERIAL.println(F("SQW: UITLEESFOUT"));
      }
    }
  }
  DateTime datumTijd;
  if (sensor->LeesDatumTijd(datumTijd)) {
    GA_SERIAL.print(F("Datum/tijd: "));
    GA_SERIAL.print(datumTijd.year()); GA_SERIAL.print('-');
    GA_SERIAL.print(datumTijd.month()); GA_SERIAL.print('-');
    GA_SERIAL.print(datumTijd.day()); GA_SERIAL.print(' ');
    GA_SERIAL.print(datumTijd.hour()); GA_SERIAL.print(':');
    GA_SERIAL.print(datumTijd.minute()); GA_SERIAL.print(':');
    GA_SERIAL.println(datumTijd.second());
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
    GA_SERIAL.print(F("Unix-tijd (s): "));
    GA_SERIAL.println(sensor->ReadQueueOrStackUInt32(1));
    GA_SERIAL.print(F("Chiptemperatuur (C): "));
    GA_SERIAL.println(sensor->ReadQueueOrStackFloat(2));
#endif
  } else {
    GA_SERIAL.println(F("Datum/tijd: GEEN GELDIGE METING (controleer voeding en klokinstelling)"));
  }
  delay(250);
}
