// ============================================================================
// Sensor: SDP810125PA
// ============================================================================
// Hardwaretest voor SDP810125PA binnen de bestaande Sensor-architectuur.
// Configuratie: SENSOR_SDP810125PA_AANTAL moet 1 zijn voor deze hardwaretest.
// Configuratie: DATA_LAYER_CONFIG moet DATA_TYPE_NONE zijn voor deze test.
// De sketch toont de bestaande parent- en Sensor-controles via Serial.
// Aansluiting gebruikte I2C-parent/Extender: SDA -> board-SDA, SCL -> board-SCL; GND -> board-GND.
// UNO R4 Minima/WiFi: fysieke SDA- en SCL-header (bij R4 Minima ook A4=SDA, A5=SCL); UNO Q: board-SDA/SCL.
// ESP32 D1 UNO R32: SDA=GPIO21, SCL=GPIO22 (bestaand FrameWork-boardprofiel).
// ESP32-S3 ARDI-32: SDA=GPIO38, SCL=GPIO39; DEV: SDA=GPIO8, SCL=GPIO9 (FrameWork-profiel).
// RP2040 Cytron Maker UNO: SDA=GPIO20, SCL=GPIO21 (ook Maker/Grove 6).
// STM32 F401RE Nucleo: SDA=PB9, SCL=PB8 (bestaand FrameWork-boardprofiel).
// Voeding VCC en GND volgens het concrete modulebord; controleer I2C-logicaniveau, pull-ups en eventuele levelshifting.
// ============================================================================

#include <GroeiAcademie.h>
#if SENSOR_SDP810125PA_AANTAL != 1
#error SENSOR_SDP810125PA_AANTAL moet 1 zijn voor deze hardwaretest.
#endif
#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
#error DATA_LAYER_CONFIG moet DATA_TYPE_NONE zijn voor deze test.
#endif
HardwareResourceTypeI2C apparaat(&Native, GedeeldeBusComponent::SDP810125PA, 0x25,
 HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, HardwareResourceToegang::GEDEELD);
void setup() {
 GA_SERIAL.begin(SERIAL_BAUDRATE);
 #if defined(DEBUG)
 while (!GA_SERIAL) { ; }
 #else
 const unsigned long startTijd = millis();
 while (!GA_SERIAL && millis()-startTijd<SERIAL_CONNECT_TIMEOUT_MS) { ; }
 #endif
 GA_SERIAL.println(F("TEST: Sensor_SDP810125PA"));
 bool parentOK = apparaat.aanmelden() && apparaat.controleren() && apparaat.inpluggen() && apparaat.activeren();
 GA_SERIAL.println(parentOK ? F("PARENT: OK") : F("PARENT: FOUT"));
 if (!parentOK) return;
 SDP810125PA* sensor = GedeeldeBusNewComponent<SDP810125PA>(&apparaat,GedeeldeBusComponent::SDP810125PA);
 GA_SERIAL.println(sensor ? F("LIFECYCLE: OK") : F("LIFECYCLE: FOUT"));
 
}
void loop() {}
