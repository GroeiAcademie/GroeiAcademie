// Sensor: MPU6500 via TCA9548A (I2C, kanaal 0) + SC16IS752I2C (INT -> GPIO0 van de SC16IS752I2C): MIXED.
// Mixed-patroon zoals Sturing_MAX7219_PCF8574_MIXED (ownerClaim, viaMixed). GPIO van de UART-chip via eigen registers.
// Logicaniveaus en voeding fysiek controleren.
// Test tot en met de meting in loop(). Compilatie en hardwarewerking zijn nog niet bewezen.

#include <GroeiAcademie.h>

#if SENSOR_MPU6500_AANTAL != 1
  #error Zet SENSOR_MPU6500_AANTAL op 1 voor deze test.
#endif
#if EXTENDER_TCA9548A_AANTAL == 0
  #error Zet EXTENDER_TCA9548A_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif
#if EXTENDER_SC16IS752_I2C_AANTAL == 0
  #error Zet EXTENDER_SC16IS752_I2C_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif
#if !GEDEELDE_BUS_MIXED_PINS
  #error Zet GEDEELDE_BUS_MIXED_PINS op 1 in UserConfig.h voor deze test.
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

#if EXTENDER_SC16IS752_I2C_AANTAL == 1
ExtenderSC16IS752I2C extenderSC16IS752I2C(
  &Native,
  GedeeldeBusComponent::UART_SC16IS752,
  I2C_ADDRESS_EXTENDER_SC16IS752_I2C_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, EXTENDER_SC16IS752_I2C_1_IRQ, EXTENDER_SC16IS752_I2C_1_RESET
);
#else
ExtenderSC16IS752I2C extenderSC16IS752I2C(
  &Native,
  GedeeldeBusComponent::UART_SC16IS752,
  HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, 0
);
#endif

HardwareResourceTypeI2C apparaat(&extender, GedeeldeBusComponent::MPU6500, I2C_ADDRESS_SENSOR_MPU6500_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, HardwareResourceToegang::GEDEELD, 0);
MPU6500* sensor = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_MPU6500_TCA9548A_SC16IS752I2C_MIXED"));
  const bool gelukt = extender.aanmelden() && extender.controleren() && extender.inpluggen() && extender.activeren() &&
                      extenderSC16IS752I2C.aanmelden() && extenderSC16IS752I2C.controleren() && extenderSC16IS752I2C.inpluggen() && extenderSC16IS752I2C.activeren();
  GA_SERIAL.println(gelukt ? F("EXTENDERS: OK") : F("EXTENDERS: FOUT"));
  if (!gelukt) return;
  const bool parentOK = apparaat.aanmelden() && apparaat.controleren() && apparaat.inpluggen() && apparaat.activeren();
  GA_SERIAL.println(parentOK ? F("I2C: OK") : F("I2C: FOUT"));
  if (!parentOK) return;
  sensor = GedeeldeBusNewComponent<MPU6500>(&apparaat, GedeeldeBusComponent::MPU6500, &extenderSC16IS752I2C, static_cast<uint8_t>(ExtenderSC16IS752I2C::ExtenderPins::EP_GPIO0));
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
    GA_SERIAL.print(F("Versnelling X (m/s2): "));
    GA_SERIAL.println(waarde);
    GA_SERIAL.println(F("Bewegingsassen niet via huidige publieke Sensor-API beschikbaar."));
  }
  // Een false-resultaat kan ook betekenen: conversie nog bezig of geen verse sample.
  delay(250);
}
