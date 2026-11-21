// ============================================================================
// Sensor: LD2410D
// ============================================================================
// Fabrikantdocument: standaard 115200 baud (NIET 256000 van andere LD2410-varianten).
// Fysieke UART: sensor TX -> GA_SENSOR_SERIAL_PIN_RX; sensor RX -> GA_SENSOR_SERIAL_PIN_TX.
// GND gemeenschappelijk; voeding en UART-logicaniveau volgens de concrete sensormodule.
// De pinnen per board staan bij GA_SENSOR_SERIAL in SystemConfig.h.
// ============================================================================

#include <GroeiAcademie.h>
#if SENSOR_LD2410D_AANTAL != 1
#error SENSOR_LD2410D_AANTAL moet 1 zijn.
#endif
#if BOARD_VERSION == BOARD_STM32F4_NUCLEO64_F401RE
// De STM32-core maakt standaard alleen Serial aan.
// Daarom maakt dit sketch Serial1 aan op de pinnen van GA_SENSOR_SERIAL.
// Sinds STM32duino 3.0.0 heet de seriële klasse Uart; dit sketch vraagt dus STM32duino 3.0.0 of nieuwer.
Uart Serial1(NativeArduinoPinVan(GA_SENSOR_SERIAL_PIN_RX), NativeArduinoPinVan(GA_SENSOR_SERIAL_PIN_TX));
#endif
LD2410D* sensor = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  #if defined(DEBUG)
  while (!GA_SERIAL) { ; }
  #else
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && millis() - startTijd < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  #endif
  GA_SERIAL.println(F("TEST: Sensor_LD2410D"));
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  if (Data == nullptr) Data = GedeeldeBusNewComponent<struct Data>(DATA_LAYER_CONFIG);
  GA_SERIAL.println(Data ? F("DATA: OK") : F("DATA: FOUT"));
  if (Data == nullptr) return;
#else
  GA_SERIAL.println(F("LET OP: extra meetkanalen vereisen DATA_TYPE_DIRECT in UserConfig.h."));
#endif
  #if defined(ARDUINO_ARCH_ESP32)
  GA_SENSOR_SERIAL.begin(115200, SERIAL_8N1, NativeArduinoPinVan(GA_SENSOR_SERIAL_PIN_RX), NativeArduinoPinVan(GA_SENSOR_SERIAL_PIN_TX));
  #else
  GA_SENSOR_SERIAL.begin(115200);
  #endif
  bool basis = Native.aanmelden() && Native.controleren() && Native.inpluggen() && Native.activeren();
  if (!basis) { GA_SERIAL.println(F("PARENT: FOUT")); return; }
  sensor = GedeeldeBusNewComponent<LD2410D>(&Native, GedeeldeBusComponent::LD2410D, GA_SENSOR_SERIAL, GA_SENSOR_SERIAL_PIN_TX, GA_SENSOR_SERIAL_PIN_RX);
  GA_SERIAL.println(sensor ? F("COMPONENT: AANGEMAAKT; verifieer Activeren()") : F("COMPONENT: FOUT"));
}
void loop() {
  if (sensor == nullptr) return;
  float waarde = NAN;
  if (!sensor->LeesHoofdwaarde(waarde)) return;
  static unsigned long laatsteRapport = 0;
  if (millis() - laatsteRapport < 250UL) return;
  laatsteRapport = millis();
  GA_SERIAL.print(F("Aanwezigheidsstatus (0-3): "));
  GA_SERIAL.println(waarde);
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  GA_SERIAL.print(F("Via Data, aanwezigheidsstatus (0-3): "));
  GA_SERIAL.println(sensor->ReadQueueOrStackFloat(1));
#endif
}
