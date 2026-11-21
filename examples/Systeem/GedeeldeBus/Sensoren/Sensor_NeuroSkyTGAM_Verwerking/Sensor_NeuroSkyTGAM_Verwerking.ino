// ============================================================================
// Sensor: NeuroSkyTGAM_Verwerking
// Verwerking: ruwe EEG-golf en vermogen van de EEG-banden (EEGfrequencyBands, 512 Hz).
// ============================================================================
// Hardwaretest voor NeuroSkyTGAM binnen de bestaande Sensor-architectuur.
// Configuratie: SENSOR_NEUROSKYTGAM_AANTAL moet 1 zijn.
// Serial meldt initialisatie; een fysieke sensorrespons is niet automatisch bewezen.
// Fysieke UART: sensor TX -> GA_SENSOR_SERIAL_PIN_RX; sensor RX -> GA_SENSOR_SERIAL_PIN_TX.
// GND gemeenschappelijk; voeding en UART-logicaniveau volgens de concrete sensormodule.
// De pinnen per board staan bij GA_SENSOR_SERIAL in SystemConfig.h.
// ============================================================================

#include <GroeiAcademie.h>
#if SENSOR_NEUROSKYTGAM_AANTAL != 1
#error SENSOR_NEUROSKYTGAM_AANTAL moet 1 zijn.
#endif
#if BOARD_VERSION == BOARD_STM32F4_NUCLEO64_F401RE
// De STM32-core maakt standaard alleen Serial aan.
// Daarom maakt dit sketch Serial1 aan op de pinnen van GA_SENSOR_SERIAL.
// Sinds STM32duino 3.0.0 heet de seriële klasse Uart; dit sketch vraagt dus STM32duino 3.0.0 of nieuwer.
Uart Serial1(NativeArduinoPinVan(GA_SENSOR_SERIAL_PIN_RX), NativeArduinoPinVan(GA_SENSOR_SERIAL_PIN_TX));
#endif
NeuroSkyTGAM* sensor = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  #if defined(DEBUG)
  while (!GA_SERIAL) { ; }
  #else
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && millis() - startTijd < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  #endif
  GA_SERIAL.println(F("TEST: Sensor_NeuroSkyTGAM_Verwerking"));
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  if (Data == nullptr) Data = GedeeldeBusNewComponent<struct Data>(DATA_LAYER_CONFIG);
  GA_SERIAL.println(Data ? F("DATA: OK") : F("DATA: FOUT"));
  if (Data == nullptr) return;
#else
  GA_SERIAL.println(F("LET OP: extra meetkanalen vereisen DATA_TYPE_DIRECT in UserConfig.h."));
#endif
  #if defined(ARDUINO_ARCH_ESP32)
  GA_SENSOR_SERIAL.begin(57600, SERIAL_8N1, NativeArduinoPinVan(GA_SENSOR_SERIAL_PIN_RX), NativeArduinoPinVan(GA_SENSOR_SERIAL_PIN_TX));
  #else
  GA_SENSOR_SERIAL.begin(57600);
  #endif
  bool basis = Native.aanmelden() && Native.controleren() && Native.inpluggen() && Native.activeren();
  if (!basis) { GA_SERIAL.println(F("PARENT: FOUT")); return; }
  sensor = GedeeldeBusNewComponent<NeuroSkyTGAM>(&Native, GedeeldeBusComponent::NeuroSkyTGAM, GA_SENSOR_SERIAL, GA_SENSOR_SERIAL_PIN_TX, GA_SENSOR_SERIAL_PIN_RX);
  GA_SERIAL.println(sensor ? F("INITIALISATIE: OK; SENSORRESPONS: niet bewezen") : F("LIFECYCLE: FOUT"));
}
void loop() {
  if (sensor == nullptr) return;
  float waarde = NAN;
  (void)sensor->LeesHoofdwaarde(waarde);   // verwerkt alle pakketten, ook de ruwe golf; zo vaak mogelijk oproepen
  static unsigned long laatsteRapport = 0;
  if (millis() - laatsteRapport < 1000UL) return;
  laatsteRapport = millis();
  {
    int16_t waarde = 0;
    if (sensor->ReadRawEEGsample(waarde)) { GA_SERIAL.print(F("Ruwe EEG: ")); GA_SERIAL.println(waarde); }
  }
  float delta = NAN, theta = NAN, alpha = NAN, beta = NAN, gamma = NAN;
  if (!sensor->ReadEEGbandPowersFromRawWave(delta, theta, alpha, beta, gamma)) return;
  GA_SERIAL.print(F("Delta: ")); GA_SERIAL.print(delta);
  GA_SERIAL.print(F("  Theta: ")); GA_SERIAL.print(theta);
  GA_SERIAL.print(F("  Alfa: ")); GA_SERIAL.print(alpha);
  GA_SERIAL.print(F("  Beta: ")); GA_SERIAL.print(beta);
  GA_SERIAL.print(F("  Gamma: ")); GA_SERIAL.println(gamma);
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  GA_SERIAL.print(F("Via Data, delta: "));
  GA_SERIAL.println(sensor->ReadQueueOrStackFloat(11));
  GA_SERIAL.print(F("Via Data, theta: "));
  GA_SERIAL.println(sensor->ReadQueueOrStackFloat(12));
  GA_SERIAL.print(F("Via Data, alfa: "));
  GA_SERIAL.println(sensor->ReadQueueOrStackFloat(13));
  GA_SERIAL.print(F("Via Data, beta: "));
  GA_SERIAL.println(sensor->ReadQueueOrStackFloat(14));
  GA_SERIAL.print(F("Via Data, gamma: "));
  GA_SERIAL.println(sensor->ReadQueueOrStackFloat(15));
#endif
}
