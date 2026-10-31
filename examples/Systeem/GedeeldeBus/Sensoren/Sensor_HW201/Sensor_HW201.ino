// VCC/GND volgens concrete module; controleer OUT-niveau voor 3,3V-board.
// De sensor heeft geen bus-ID. Dit voorbeeld bevestigt GEEN echte detectie.
#include <GroeiAcademie.h>
#if SENSOR_HW201_AANTAL != 1
#error Zet SENSOR_HW201_AANTAL op 1 voor deze test.
#endif
HW201* sensorHW201 = nullptr;
void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && millis() - startTijd < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  sensorHW201 = GedeeldeBusNewComponent<HW201>(&Native, GedeeldeBusComponent::HW201, HardwareResourcePin::NP_D2);
  GA_SERIAL.println(sensorHW201 ? F("HW201: component aangemaakt; controleer lifecycle en OUT tijdens hardwaretest") : F("HW201: FOUT"));
}
void loop() {}
