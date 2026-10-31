#include <GroeiAcademie.h>
#if SENSOR_MCU6701_AANTAL == 0
#error Zet SENSOR_MCU6701_AANTAL op 1 voor deze test.
#endif
MCU6701* sensorGSR = nullptr;
void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
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
