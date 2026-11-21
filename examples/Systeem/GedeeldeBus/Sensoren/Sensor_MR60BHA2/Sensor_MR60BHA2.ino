// ============================================================================
// Sensor: MR60BHA2
// ============================================================================
// MR60BHA2 gebruikt de FrameWork-Stream-driver, ook voor niet-ESP32-borden.
// Aansluiting: radar TX -> GA_SENSOR_SERIAL_PIN_RX; radar RX -> GA_SENSOR_SERIAL_PIN_TX; GND gemeenschappelijk.
// De pinnen per board staan bij GA_SENSOR_SERIAL in SystemConfig.h.
// Voeding volgens de officiele datasheet van de concrete module.
// ============================================================================

#include <GroeiAcademie.h>
#if SENSOR_MR60BHA2_AANTAL != 1
#error Zet SENSOR_MR60BHA2_AANTAL op 1 voor deze hardwaretest.
#endif
#if BOARD_VERSION == BOARD_STM32F4_NUCLEO64_F401RE
// De STM32-core maakt standaard alleen Serial aan.
// Daarom maakt dit sketch Serial1 aan op de pinnen van GA_SENSOR_SERIAL.
// Sinds STM32duino 3.0.0 heet de seriële klasse Uart; dit sketch vraagt dus STM32duino 3.0.0 of nieuwer.
Uart Serial1(NativeArduinoPinVan(GA_SENSOR_SERIAL_PIN_RX), NativeArduinoPinVan(GA_SENSOR_SERIAL_PIN_TX));
#endif
MR60BHA2* sensorMR60BHA2 = nullptr;
void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  #if defined(DEBUG)
  while (!GA_SERIAL) { ; }
  #else
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && millis() - startTijd < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  #endif
  GA_SERIAL.println(F("TEST: Sensor_MR60BHA2"));
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  if (Data == nullptr) Data = GedeeldeBusNewComponent<struct Data>(DATA_LAYER_CONFIG);
  GA_SERIAL.println(Data ? F("DATA: OK") : F("DATA: FOUT"));
  if (Data == nullptr) return;
#else
  GA_SERIAL.println(F("LET OP: extra meetkanalen vereisen DATA_TYPE_DIRECT in UserConfig.h."));
#endif
  // Gebruik GA_SENSOR_SERIAL; FrameWork initialiseert deze op 115200 baud.
  // Op een ESP32 krijgt de poort eerst zijn pinnen; begin() in de driver behoudt die pinnen.
  #if defined(ARDUINO_ARCH_ESP32)
  if (!GA_SENSOR_SERIAL.setPins(NativeArduinoPinVan(GA_SENSOR_SERIAL_PIN_RX), NativeArduinoPinVan(GA_SENSOR_SERIAL_PIN_TX))) { GA_SERIAL.println(F("UART: FOUT")); return; }
  #endif
  sensorMR60BHA2 = GedeeldeBusNewComponent<MR60BHA2>(
      &Native, GedeeldeBusComponent::MR60BHA2, &GA_SENSOR_SERIAL, GA_SENSOR_SERIAL_PIN_TX, GA_SENSOR_SERIAL_PIN_RX);
  GA_SERIAL.println(sensorMR60BHA2 ? F("INITIALISATIE: component aangemaakt; SENSORRESPONS: niet bewezen") : F("LIFECYCLE: FOUT"));
}
void loop() {
  if (sensorMR60BHA2 == nullptr) return;
  float waarde = NAN;
  if (!sensorMR60BHA2->LeesHoofdwaarde(waarde)) return;
  static unsigned long laatsteRapport = 0;
  if (millis() - laatsteRapport < 250UL) return;
  laatsteRapport = millis();
  GA_SERIAL.print(F("Radarwaarde (eenheid volgens firmware): "));
  GA_SERIAL.println(waarde);
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  GA_SERIAL.print(F("Via Data, radarwaarde: "));
  GA_SERIAL.println(sensorMR60BHA2->ReadQueueOrStackFloat(1));
#endif
}
