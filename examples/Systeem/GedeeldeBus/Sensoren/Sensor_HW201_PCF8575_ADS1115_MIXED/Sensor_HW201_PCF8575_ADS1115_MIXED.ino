// Sensor: HW201 via PCF8575 + ADS1115: OUT (DO) -> PCF8575 P00, AO -> ADS1115 AIN0 (MIXED).
// Test tot en met de meting in loop(). Compilatie en hardwarewerking zijn nog niet bewezen.

#include <GroeiAcademie.h>

#if SENSOR_HW201_AANTAL != 1
  #error Zet SENSOR_HW201_AANTAL op 1 voor deze test.
#endif
#if EXTENDER_PCF8575_AANTAL == 0
  #error Zet EXTENDER_PCF8575_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif
#if EXTENDER_ADS1115_AANTAL == 0
  #error Zet EXTENDER_ADS1115_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif
#if !GEDEELDE_BUS_MIXED_PINS
  #error Zet GEDEELDE_BUS_MIXED_PINS op 1 in UserConfig.h voor deze test.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE voor deze test.
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

#if EXTENDER_ADS1115_AANTAL == 1
ExtenderADS1115 extenderADS1115(
  &Native,
  GedeeldeBusComponent::ADC_ADS1115,
  I2C_ADDRESS_EXTENDER_ADS1115_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, EXTENDER_ADS1115_1_ALERT_RDY
);
#else
ExtenderADS1115 extenderADS1115(
  &Native,
  GedeeldeBusComponent::ADC_ADS1115,
  HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, 0
);
#endif

HW201* sensor = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_HW201_PCF8575_ADS1115_MIXED"));
  const bool gelukt = extenderPCF8575.aanmelden() && extenderPCF8575.controleren() && extenderPCF8575.inpluggen() && extenderPCF8575.activeren() &&
                      extenderADS1115.aanmelden() && extenderADS1115.controleren() && extenderADS1115.inpluggen() && extenderADS1115.activeren();
  GA_SERIAL.println(gelukt ? F("EXTENDERS: OK") : F("EXTENDERS: FOUT"));
  if (!gelukt) return;
  sensor = GedeeldeBusNewComponent<HW201>(&extenderPCF8575, GedeeldeBusComponent::HW201, static_cast<HardwareResourcePin>(static_cast<uint8_t>(ExtenderPCF8575::ExtenderPins::EP_P00)), &extenderADS1115, static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN0));
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
