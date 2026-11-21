// Sensor: MPU6500 via TCA9548A (I2C, kanaal 0) + PCF8575 (INT): MIXED.
// Bestaande FrameWork-API; mixed-patroon zoals Sturing_MAX7219_PCF8574_MIXED (ownerClaim, viaMixed).
// Bedrading: SDA/SCL van de sensor op TCA9548A kanaal 0; INT -> PCF8575 EP_P00.
// PCF8575: SDA/SCL op de hoofdbus, niet achter de TCA9548A. Voeding, GND en logicaniveaus fysiek controleren.
// Test tot en met de meting in loop(). Compilatie en hardwarewerking zijn nog niet bewezen.

#include <GroeiAcademie.h>

#if SENSOR_MPU6500_AANTAL != 1
  #error Zet SENSOR_MPU6500_AANTAL op 1 voor deze test.
#endif
#if EXTENDER_TCA9548A_AANTAL == 0
  #error Zet EXTENDER_TCA9548A_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif
#if EXTENDER_PCF8575_AANTAL == 0 || !GEDEELDE_BUS_MIXED_PINS
  #error Deze test vereist EXTENDER_PCF8575_AANTAL > 0 en GEDEELDE_BUS_MIXED_PINS.
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

#if EXTENDER_PCF8575_AANTAL == 1
ExtenderPCF8575 extenderPCF8575(
  &Native, GedeeldeBusComponent::DIGITAL_PINS_PCF8575,
  I2C_ADDRESS_EXTENDER_PCF8575_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, EXTENDER_PCF8575_1_INT
);
#else
ExtenderPCF8575 extenderPCF8575(
  &Native, GedeeldeBusComponent::DIGITAL_PINS_PCF8575,
  HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, 0
);
#endif

HardwareResourceTypeI2C apparaat(&extender, GedeeldeBusComponent::MPU6500, I2C_ADDRESS_SENSOR_MPU6500_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, HardwareResourceToegang::GEDEELD, 0);
MPU6500* sensor = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_MPU6500_TCA9548A_PCF8575_MIXED"));
  const bool gelukt = extender.aanmelden() && extender.controleren() && extender.inpluggen() && extender.activeren() &&
                      extenderPCF8575.aanmelden() && extenderPCF8575.controleren() && extenderPCF8575.inpluggen() && extenderPCF8575.activeren();
  GA_SERIAL.println(gelukt ? F("EXTENDERS: OK") : F("EXTENDERS: FOUT"));
  if (!gelukt) return;
  const bool parentOK = apparaat.aanmelden() && apparaat.controleren() && apparaat.inpluggen() && apparaat.activeren();
  GA_SERIAL.println(parentOK ? F("I2C: OK") : F("I2C: FOUT"));
  if (!parentOK) return;
  sensor = GedeeldeBusNewComponent<MPU6500>(&apparaat, GedeeldeBusComponent::MPU6500, &extenderPCF8575, static_cast<uint8_t>(ExtenderPCF8575::ExtenderPins::EP_P00));
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
