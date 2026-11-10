// ============================================================================
// Sensor: MCU6701_SPI
// ============================================================================
// Hardwaretest voor MCU6701_SPI binnen de bestaande Sensor-architectuur.
// Configuratie: Zet SENSOR_MCU6701_AANTAL op 1 voor deze test.
// De sketch toont de bestaande parent- en Sensor-controles via Serial.
// CJMCU-6701 via MCP3201: chip-CS -> D10, chip-SCK -> board-SCK, chip-MISO -> board-MISO.
// GEEN fysieke MOSI naar deze ADC; NP_MOSI in de constructor reserveert de gedeelde SPI-bus.
// UNO R4 Minima: CS=D10, SCK=D13, MISO=D12; GND gemeenschappelijk.
// Voeding VCC en signaalniveaus conform de concrete CJMCU-6701-print.
// ============================================================================

#include <GroeiAcademie.h>
#if SENSOR_MCU6701_AANTAL == 0
#error Zet SENSOR_MCU6701_AANTAL op 1 voor deze test.
#endif
MCU6701* sensorGSR = nullptr;
void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  #if defined(DEBUG)
  while (!GA_SERIAL) { ; }
  #else
  unsigned long startTijd = millis();
  while (!GA_SERIAL && (millis() - startTijd) < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  #endif
  GA_SERIAL.println(F("TEST: Sensor_MCU6701_SPI"));
  // CJMCU-6701: CS -> D10, MIS -> MISO, SCK -> SCK; geen MOSI-verbinding naar de chip.
  // NP_MOSI claimt de gedeelde SPI-buslijn, niet een fysieke sensor-MOSI.
  sensorGSR = GedeeldeBusNewComponent<MCU6701>(&Native, GedeeldeBusComponent::MCU6701,
      HardwareResourcePin::NP_D10, HardwareResourcePin::NP_SCK,
      HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI);
  GA_SERIAL.println(sensorGSR ? F("MCU6701 SPI: OK") : F("MCU6701 SPI: FOUT"));
}
void loop() {
  if (!sensorGSR) return;
  float ruw = NAN;
  if (sensorGSR->LeesHoofdwaarde(ruw)) {
    GA_SERIAL.print(F("GSR ADC 12-bit geschaald: ")); GA_SERIAL.println(ruw);
  }
  delay(200);
}
