// ============================================================================
// Sensor: AD8232
// ============================================================================
// SparkFun AD8232, OUTPUT via ADC; LO+/LO- later afzonderlijk
// Bron: https://learn.sparkfun.com/tutorials/ad8232-heart-rate-monitor-hookup-guide/
// Biosignalen: nooit op een persoon testen zonder geschikte elektrische isolatie/veiligheidscontrole.
// Aansluiting gebruikte I2C-parent/Extender: SDA -> board-SDA, SCL -> board-SCL; GND -> board-GND.
// UNO R4 Minima/WiFi: fysieke SDA- en SCL-header (bij R4 Minima ook A4=SDA, A5=SCL); UNO Q: board-SDA/SCL.
// ESP32 D1 UNO R32: SDA=GPIO21, SCL=GPIO22 (bestaand FrameWork-boardprofiel).
// ESP32-S3 ARDI-32: SDA=GPIO38, SCL=GPIO39; DEV: SDA=GPIO8, SCL=GPIO9 (FrameWork-profiel).
// RP2040 Cytron Maker UNO: SDA=GPIO20, SCL=GPIO21 (ook Maker/Grove 6).
// STM32 F401RE Nucleo: SDA=PB9, SCL=PB8 (bestaand FrameWork-boardprofiel).
// Voeding VCC en GND volgens het concrete modulebord; controleer I2C-logicaniveau, pull-ups en eventuele levelshifting.
// Bij een analoge sensor is dit de bus van de ADC-Extender, NIET een I2C-aansluiting op de sensor zelf.
// Sluit de analoge sensoruitgang op het in de code/configuratie gebruikte ADC-kanaal aan.
// De analoge sensoruitgang gaat in deze sketch naar ADS1115 AIN0.
// ============================================================================

#include <GroeiAcademie.h>
#if SENSOR_AD8232_AANTAL != 1
#error Zet SENSOR_AD8232_AANTAL op 1 voor deze hardwaretest.
#endif
#if EXTENDER_ADS1115_AANTAL < 1
#error Zet EXTENDER_ADS1115_AANTAL op minstens 1.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
#error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE.
#endif
ExtenderADS1115 extenderADS1115(&Native, GedeeldeBusComponent::ADC_ADS1115,
  I2C_ADDRESS_EXTENDER_ADS1115_1, HardwareResourcePin::NP_SDA,
  HardwareResourcePin::NP_SCL, EXTENDER_ADS1115_1_ALERT_RDY,
  HardwareResourceToegang::GEDEELD);
AD8232* sensorAD8232 = nullptr;
void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_AD8232"));
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && millis() - startTijd < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  sensorAD8232 = GedeeldeBusNewComponent<AD8232>(&extenderADS1115,
    GedeeldeBusComponent::AD8232,
    static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN0),
    HardwareResourceToegang::GEDEELD);
  GA_SERIAL.println(sensorAD8232 ? F("AD8232 lifecycle: component aangemaakt (fysieke werking nog niet bewezen)") : F("AD8232 lifecycle: FOUT"));
}
void loop() {}
