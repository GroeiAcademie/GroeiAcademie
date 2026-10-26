// Compileer- en levenscyclustest voor WS2812. Geen automatische beweging of schakelactie.
#include <GroeiAcademie.h>
#if STURING_WS2812_AANTAL == 0
  #error Zet STURING_WS2812_AANTAL op minstens 1 voor deze test.
#endif


WS2812 sturing(&Native,HardwareResourcePin::NP_D6);
void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  bool gelukt = sturing.aanmelden() && sturing.controleren() && sturing.inpluggen() && sturing.activeren();
  GA_SERIAL.println(gelukt ? F("RESULTAAT: OK") : F("RESULTAAT: FOUT"));
}
void loop() {}
