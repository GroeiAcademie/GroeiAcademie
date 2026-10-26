// Compileer- en levenscyclustest voor A4988. Geen automatische beweging of schakelactie.
#include <GroeiAcademie.h>
#if STURING_A4988_AANTAL == 0
  #error Zet STURING_A4988_AANTAL op minstens 1 voor deze test.
#endif


A4988 sturing(&Native,HardwareResourcePin::NP_D6,HardwareResourcePin::NP_D7);
void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  bool gelukt = sturing.aanmelden() && sturing.controleren() && sturing.inpluggen() && sturing.activeren();
  GA_SERIAL.println(gelukt ? F("RESULTAAT: OK") : F("RESULTAAT: FOUT"));
}
void loop() {}
