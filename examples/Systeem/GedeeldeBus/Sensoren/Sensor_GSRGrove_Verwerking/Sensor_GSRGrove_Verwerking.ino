// ============================================================================
// Sensor: GSRGrove_Verwerking
// Verwerking: huidweerstand (Ohm) en huidgeleiding (microsiemens), Seeed-formule.
// ============================================================================
// Hardwaretest voor GSRGrove binnen de bestaande Sensor-architectuur.
// Configuratie: Zet SENSOR_GSRGROVE_AANTAL op 1 voor deze hardwaretest.
// Configuratie: Zet EXTENDER_ADS1115_AANTAL op minstens 1 voor deze hardwaretest.
// Configuratie: Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE of DATA_TYPE_DIRECT voor deze hardwaretest.
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
#if SENSOR_GSRGROVE_AANTAL != 1
  #error Zet SENSOR_GSRGROVE_AANTAL op 1 voor deze hardwaretest.
#endif
#if EXTENDER_ADS1115_AANTAL == 0
  #error Zet EXTENDER_ADS1115_AANTAL op minstens 1 voor deze hardwaretest.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE && DATA_LAYER_CONFIG != DATA_TYPE_DIRECT
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE of DATA_TYPE_DIRECT voor deze hardwaretest.
#endif

ExtenderADS1115 extenderADS1115(&Native, GedeeldeBusComponent::ADC_ADS1115,
  I2C_ADDRESS_EXTENDER_ADS1115_1, HardwareResourcePin::NP_SDA,
  HardwareResourcePin::NP_SCL, EXTENDER_ADS1115_1_ALERT_RDY,
  HardwareResourceToegang::GEDEELD);
GSRGrove* sensorGSRGrove = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  #if defined(DEBUG)
  while (!GA_SERIAL) { ; }
  #else
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && millis() - startTijd < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  #endif
  GA_SERIAL.println(F("TEST: Sensor_GSRGrove_Verwerking"));
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  if (Data == nullptr) Data = GedeeldeBusNewComponent<struct Data>(DATA_LAYER_CONFIG);
  GA_SERIAL.println(Data ? F("DATA: OK") : F("DATA: FOUT"));
  if (Data == nullptr) return;
#else
  GA_SERIAL.println(F("LET OP: extra meetkanalen vereisen DATA_TYPE_DIRECT in UserConfig.h."));
#endif
  sensorGSRGrove = GedeeldeBusNewComponent<GSRGrove>(
    &extenderADS1115, GedeeldeBusComponent::GSRGrove,
    static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN0),
    HardwareResourceToegang::GEDEELD);
  GA_SERIAL.println(sensorGSRGrove ? F("LIFECYCLE: OK") : F("LIFECYCLE: FOUT"));
}
void loop() {
  if (sensorGSRGrove == nullptr) return;
  static unsigned long laatsteRapport = 0;
  if (millis() - laatsteRapport < 500UL) return;
  laatsteRapport = millis();
  float waarde = NAN;
  // Eén ADC-meting: ReadSkinConductanceMicrosiemens() leest intern de huidweerstand, dus geleiding en weerstand komen van dezelfde meting.
  if (!sensorGSRGrove->ReadSkinConductanceMicrosiemens(waarde) || waarde <= 0) { GA_SERIAL.println(F("Geen huidcontact of SENSOR_GSRGROVE_CALIBRATION_VALUE niet juist ingesteld")); return; }
  GA_SERIAL.print(F("Huidweerstand (Ohm): "));
  GA_SERIAL.println(1000000.0f / waarde);
  GA_SERIAL.print(F("Huidgeleiding (microsiemens): "));
  GA_SERIAL.println(waarde);
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  GA_SERIAL.print(F("Via Data, huidweerstand (Ohm): "));
  GA_SERIAL.println(sensorGSRGrove->ReadQueueOrStackFloat(3));
  GA_SERIAL.print(F("Via Data, huidgeleiding (microsiemens): "));
  GA_SERIAL.println(sensorGSRGrove->ReadQueueOrStackFloat(2));
#endif
}
