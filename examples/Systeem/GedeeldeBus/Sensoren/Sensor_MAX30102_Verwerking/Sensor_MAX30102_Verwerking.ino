// ============================================================================
// Sensor: MAX30102_Verwerking
// Verwerking: hartslag (BPM) en SpO2 (%).
// ============================================================================
// Vereist geïnstalleerde officiële library: MAX30105.h.
// Aansluiting gebruikte I2C-parent/Extender: SDA -> board-SDA, SCL -> board-SCL; GND -> board-GND.
// UNO R4 Minima/WiFi: fysieke SDA- en SCL-header (bij R4 Minima ook A4=SDA, A5=SCL); UNO Q: board-SDA/SCL.
// ESP32 D1 UNO R32: SDA=GPIO21, SCL=GPIO22 (bestaand FrameWork-boardprofiel).
// ESP32-S3 ARDI-32: SDA=GPIO38, SCL=GPIO39; DEV: SDA=GPIO8, SCL=GPIO9 (FrameWork-profiel).
// RP2040 Cytron Maker UNO: SDA=GPIO20, SCL=GPIO21 (ook Maker/Grove 6).
// STM32 F401RE Nucleo: SDA=PB9, SCL=PB8 (bestaand FrameWork-boardprofiel).
// Voeding VCC en GND volgens het concrete modulebord; controleer I2C-logicaniveau, pull-ups en eventuele levelshifting.
// ============================================================================

#include <GroeiAcademie.h>
#if SENSOR_MAX30102_AANTAL != 1
#error SENSOR_MAX30102_AANTAL moet 1 zijn voor deze hardwaretest.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE && DATA_LAYER_CONFIG != DATA_TYPE_DIRECT
#error DATA_LAYER_CONFIG moet DATA_TYPE_NONE of DATA_TYPE_DIRECT zijn voor deze hardwaretest.
#endif
HardwareResourceTypeI2C apparaat(&Native, GedeeldeBusComponent::MAX30102,
  I2C_ADDRESS_SENSOR_MAX30102_1, HardwareResourcePin::NP_SDA,
  HardwareResourcePin::NP_SCL, HardwareResourceToegang::GEDEELD);
MAX30102* sensor = nullptr;
void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  #if defined(DEBUG)
  while (!GA_SERIAL) { ; }
  #else
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && millis() - startTijd < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  #endif
  GA_SERIAL.println(F("TEST: Sensor_MAX30102_Verwerking"));
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  if (Data == nullptr) Data = GedeeldeBusNewComponent<struct Data>(DATA_LAYER_CONFIG);
  GA_SERIAL.println(Data ? F("DATA: OK") : F("DATA: FOUT"));
  if (Data == nullptr) return;
#else
  GA_SERIAL.println(F("LET OP: extra meetkanalen vereisen DATA_TYPE_DIRECT in UserConfig.h."));
#endif
  const bool parentOK=apparaat.aanmelden() && apparaat.controleren() && apparaat.inpluggen() && apparaat.activeren();
  GA_SERIAL.println(parentOK ? F("I2C RESOURCE: OK") : F("I2C RESOURCE: FOUT"));
  if(parentOK) sensor=GedeeldeBusNewComponent<MAX30102>(&apparaat,GedeeldeBusComponent::MAX30102);
  GA_SERIAL.println(sensor ? F("SENSOR LIBRARY INIT/LIFECYCLE: OK") : F("SENSOR LIBRARY INIT/LIFECYCLE: FOUT"));
}
void loop() {
  if (sensor == nullptr) return;
  float waarde = NAN;
  (void)sensor->LeesHoofdwaarde(waarde);   // haalt de samples op (25 per seconde); minstens 10 keer per seconde oproepen
  static unsigned long laatsteRapport = 0;
  if (millis() - laatsteRapport < 1000UL) return;
  laatsteRapport = millis();
  GA_SERIAL.print(F("Hartslag (BPM): "));
  if (sensor->ReadHeartRateBeatsPerMinute(waarde)) GA_SERIAL.println(waarde); else GA_SERIAL.println(F("nog geen geldige hartslag"));
  GA_SERIAL.print(F("SpO2 (%): "));
  if (sensor->ReadOxygenSaturationPercent(waarde)) GA_SERIAL.println(waarde); else GA_SERIAL.println(F("nog geen geldige waarde (eerste 4 s, vinger, beweging)"));
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  GA_SERIAL.print(F("Via Data, hartslag (BPM): "));
  GA_SERIAL.println(sensor->ReadQueueOrStackFloat(3));
  GA_SERIAL.print(F("Via Data, SpO2 (%): "));
  GA_SERIAL.println(sensor->ReadQueueOrStackFloat(4));
#endif
}
