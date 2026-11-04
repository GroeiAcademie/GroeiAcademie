// ============================================================================
// Sensor: LM35DZ
// ============================================================================
// Hardwaretest voor LM35DZ binnen de bestaande Sensor-architectuur.
// Configuratie: Zet SENSOR_LM35DZ_AANTAL op 1 voor deze hardwaretest.
// Configuratie: Zet EXTENDER_ADS1115_AANTAL op minstens 1 voor deze hardwaretest.
// Configuratie: Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE voor deze hardwaretest.
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
#if SENSOR_LM35DZ_AANTAL != 1
  #error Zet SENSOR_LM35DZ_AANTAL op 1 voor deze hardwaretest.
#endif
#if EXTENDER_ADS1115_AANTAL == 0
  #error Zet EXTENDER_ADS1115_AANTAL op minstens 1 voor deze hardwaretest.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE voor deze hardwaretest.
#endif

ExtenderADS1115 extenderADS1115(&Native, GedeeldeBusComponent::ADC_ADS1115,
  I2C_ADDRESS_EXTENDER_ADS1115_1, HardwareResourcePin::NP_SDA,
  HardwareResourcePin::NP_SCL, EXTENDER_ADS1115_1_ALERT_RDY,
  HardwareResourceToegang::GEDEELD);
LM35DZ* sensorLM35DZ = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sensor_LM35DZ"));
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && millis() - startTijd < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  sensorLM35DZ = GedeeldeBusNewComponent<LM35DZ>(
    &extenderADS1115, GedeeldeBusComponent::LM35DZ,
    static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN0),
    HardwareResourceToegang::GEDEELD);
  GA_SERIAL.println(sensorLM35DZ ? F("LIFECYCLE: OK") : F("LIFECYCLE: FOUT"));
}
void loop() {
  if (sensorLM35DZ == nullptr) return;
  // Niet-blokkerend: elke aanroep van loop() bedient hoogstens een ADC-fase.
  static bool bezig = false;
  if (!bezig) bezig = sensorLM35DZ->StartAnalogRead();
  if (bezig && sensorLM35DZ->IsAnalogReadKlaar()) {
    const int waarde = sensorLM35DZ->GetAnalogRead();
    bezig = false;
    GA_SERIAL.print(F("LM35DZ ADC: "));
    GA_SERIAL.println(waarde);
  }
  // Expliciet blokkerend alternatief: RawAnalogRead(waarde).
}
