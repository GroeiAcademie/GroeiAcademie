// Sensor: MF52x via ADS1158 + PCA9685: signaal -> ADS1158 AIN0, VOEDING (NTC-deler) -> PCA9685 PWM0 (MIXED).
// Stroom van de hulppin fysiek controleren tegen de uitgangsstroom van de Extender.
// Test tot en met de meting in loop(). Compilatie en hardwarewerking zijn nog niet bewezen.

#include <GroeiAcademie.h>

#if SENSOR_MF52D_AANTAL != 1
  #error Zet SENSOR_MF52D_AANTAL op 1 voor deze test.
#endif
#if EXTENDER_ADS1158_AANTAL == 0
  #error Zet EXTENDER_ADS1158_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif
#if EXTENDER_PCA9685_AANTAL == 0
  #error Zet EXTENDER_PCA9685_AANTAL in UserConfig.h op minstens 1 voor deze test.
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

#if EXTENDER_PCA9685_AANTAL == 1
ExtenderPCA9685 extenderPCA9685(&Native, GedeeldeBusComponent::PWM_PCA9685, I2C_ADDRESS_EXTENDER_PCA9685_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL);
#else
ExtenderPCA9685 extenderPCA9685(&Native, GedeeldeBusComponent::PWM_PCA9685, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, 0);
#endif

MF52x* sensor = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_MF52x_ADS1158_PCA9685_MIXED"));
  const bool gelukt = extender.aanmelden() && extender.controleren() && extender.inpluggen() && extender.activeren() &&
                      extenderPCA9685.aanmelden() && extenderPCA9685.controleren() && extenderPCA9685.inpluggen() && extenderPCA9685.activeren();
  GA_SERIAL.println(gelukt ? F("EXTENDERS: OK") : F("EXTENDERS: FOUT"));
  if (!gelukt) return;
  sensor = GedeeldeBusNewComponent<MF52x>(&extender, GedeeldeBusComponent::MF52D, static_cast<uint8_t>(ExtenderADS1158::ExtenderPins::EP_AIN0), &extenderPCA9685, static_cast<uint8_t>(ExtenderPCA9685::ExtenderPins::EP_PWM0), HardwareResourceToegang::GEDEELD);
  GA_SERIAL.println(sensor ? F("LIFECYCLE: OK") : F("LIFECYCLE: FOUT"));
}

void loop() {
  if (sensor == nullptr) return;
  int32_t waarde = 0;
  if (sensor->RawAnalogRead(waarde)) {
    GA_SERIAL.print(F("MF52x ruwe ADC: ")); GA_SERIAL.println(waarde);
  } else {
    GA_SERIAL.println(F("MF52x: ADC-UITLEESFOUT (controleer bereik/referentie)"));
  }
}
