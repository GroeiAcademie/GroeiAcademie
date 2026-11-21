// Sensor: MAX30102 via TCA9548A (I2C, kanaal 0) + MAX14830SPI (INT -> GPIO0 van de MAX14830SPI): MIXED.
// Mixed-patroon zoals Sturing_MAX7219_PCF8574_MIXED (ownerClaim, viaMixed). GPIO van de UART-chip via eigen registers.
// Logicaniveaus en voeding fysiek controleren.
// Test tot en met de meting in loop(). Compilatie en hardwarewerking zijn nog niet bewezen.

#include <GroeiAcademie.h>

#if SENSOR_MAX30102_AANTAL != 1
  #error Zet SENSOR_MAX30102_AANTAL op 1 voor deze test.
#endif
#if EXTENDER_TCA9548A_AANTAL == 0
  #error Zet EXTENDER_TCA9548A_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif
#if EXTENDER_MAX14830_SPI_AANTAL == 0
  #error Zet EXTENDER_MAX14830_SPI_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif
#if !GEDEELDE_BUS_MIXED_PINS
  #error Zet GEDEELDE_BUS_MIXED_PINS op 1 in UserConfig.h voor deze test.
#endif
#if EXTENDER_MAX14830_BRONKLOK_HZ == 0
  #error Stel EXTENDER_MAX14830_BRONKLOK_HZ in op de werkelijke oscillatorfrequentie van jouw MAX14830-module voordat je deze test uitvoert.
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

#if EXTENDER_MAX14830_SPI_AANTAL == 1
ExtenderMAX14830SPI extenderMAX14830SPI(
  &Native,
  GedeeldeBusComponent::UART_MAX14830,
  EXTENDER_MAX14830_SPI_1_CS_PIN, HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, EXTENDER_MAX14830_SPI_1_IRQ
);
#else
ExtenderMAX14830SPI extenderMAX14830SPI(
  &Native,
  GedeeldeBusComponent::UART_MAX14830,
  HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, 0
);
#endif

HardwareResourceTypeI2C apparaat(&extender, GedeeldeBusComponent::MAX30102, I2C_ADDRESS_SENSOR_MAX30102_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, HardwareResourceToegang::GEDEELD, 0);
MAX30102* sensor = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_MAX30102_TCA9548A_MAX14830SPI_MIXED"));
  const bool gelukt = extender.aanmelden() && extender.controleren() && extender.inpluggen() && extender.activeren() &&
                      extenderMAX14830SPI.aanmelden() && extenderMAX14830SPI.controleren() && extenderMAX14830SPI.inpluggen() && extenderMAX14830SPI.activeren();
  GA_SERIAL.println(gelukt ? F("EXTENDERS: OK") : F("EXTENDERS: FOUT"));
  if (!gelukt) return;
  const bool parentOK = apparaat.aanmelden() && apparaat.controleren() && apparaat.inpluggen() && apparaat.activeren();
  GA_SERIAL.println(parentOK ? F("I2C: OK") : F("I2C: FOUT"));
  if (!parentOK) return;
  sensor = GedeeldeBusNewComponent<MAX30102>(&apparaat, GedeeldeBusComponent::MAX30102, &extenderMAX14830SPI, static_cast<uint8_t>(ExtenderMAX14830SPI::ExtenderPins::EP_GPIO0));
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
