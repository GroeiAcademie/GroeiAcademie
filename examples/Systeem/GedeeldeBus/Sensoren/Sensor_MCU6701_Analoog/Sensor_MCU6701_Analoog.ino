// ============================================================================
// Sensor: MCU6701_Analoog
// ============================================================================
// Hardwaretest voor MCU6701_Analoog binnen de bestaande Sensor-architectuur.
// Configuratie: Zet SENSOR_MCU6701_AANTAL op 1 voor deze test.
// Configuratie: Deze analoge voorbeeldsketch gebruikt een ADS1115. Activeer die Extender in de bestaande configuratie.
// Serial toont de uitlezing van het in deze sketch gebruikte ADC-kanaal.
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
#if SENSOR_MCU6701_AANTAL == 0
#error Zet SENSOR_MCU6701_AANTAL op 1 voor deze test.
#endif
#if EXTENDER_ADS1115_AANTAL == 0
#error Deze analoge voorbeeldsketch gebruikt een ADS1115. Activeer die Extender in de bestaande configuratie.
#endif
ExtenderADS1115 extenderVoorGSR(&Native, GedeeldeBusComponent::ADC_ADS1115,
    I2C_ADDRESS_EXTENDER_ADS1115_1, HardwareResourcePin::NP_SDA,
    HardwareResourcePin::NP_SCL, EXTENDER_ADS1115_1_ALERT_RDY,
    HardwareResourceToegang::GEDEELD);
MCU6701* analogeGSR = nullptr;
void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_MCU6701_Analoog"));
  // OUT -> AIN0 van ADS1115; CS, MIS en SCK hier niet aansluiten.
  analogeGSR = GedeeldeBusNewComponent<MCU6701>(&extenderVoorGSR,
      GedeeldeBusComponent::MCU6701, static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN0));
  GA_SERIAL.println(analogeGSR ? F("MCU6701 OUT: OK") : F("MCU6701 OUT: FOUT"));
}
void loop() {
  if (!analogeGSR) return;
  float ruw = NAN;
  if (analogeGSR->LeesHoofdwaarde(ruw)) {
    GA_SERIAL.print(F("GSR OUT ADC geschaald: ")); GA_SERIAL.println(ruw);
  }
  delay(200);
}
