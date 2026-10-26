// Compileer- en levenscyclustest voor SG90. Geen automatische beweging of schakelactie.
#include <GroeiAcademie.h>
#if STURING_SG90_AANTAL == 0
  #error Zet STURING_SG90_AANTAL op minstens 1 voor deze test.
#endif
#if EXTENDER_PCA9685_AANTAL == 0
  #error Schakel de vereiste Extender in voor deze Sturing-test.
#endif

ExtenderPCA9685 extender(&Native, GedeeldeBusComponent::PWM_PCA9685, I2C_ADDRESS_EXTENDER_PCA9685_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL);
SG90 sturing(&extender,0,205,410);
void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  bool gelukt = extender.aanmelden() && extender.controleren() && extender.inpluggen() && extender.activeren() && sturing.aanmelden() && sturing.controleren() && sturing.inpluggen() && sturing.activeren();
  GA_SERIAL.println(gelukt ? F("RESULTAAT: OK") : F("RESULTAAT: FOUT"));
}
void loop() {}
