// ============================================================================
// Sturing: A4988
// ============================================================================
// Compileer- en levenscyclustest voor A4988. Geen automatische beweging of schakelactie.
// Pinnen zijn afgeleid uit de bestaande constructoren en de boardmapping in src/Configuratie/UserConfig.h en SystemConfig.h.
// A4988 STEP -> D6, DIR -> D7; ENABLE, RESET, SLEEP en motorvoeding volgens de concrete driver.
// Native pinlabels in de code: D6 en D7; boardmapping:
// UNO R4 Minima/WiFi en UNO Q: D6 en D7.
// ESP32 D1 UNO R32: D6=GPIO27, D7=GPIO14.
// ESP32-S3 ARDI-32: D6=GPIO9, D7=GPIO14; ESP32-S3 DEV: D6=GPIO3, D7=GPIO14.
// RP2040 Cytron Maker UNO: D6=GPIO6, D7=GPIO7.
// STM32 F401RE Nucleo: D6=PB10, D7=PA8.
// ============================================================================

#include <GroeiAcademie.h>
#if STURING_A4988_AANTAL == 0
  #error Zet STURING_A4988_AANTAL op minstens 1 voor deze test.
#endif


A4988* a4988 = nullptr;
void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sturing_A4988"));
  a4988 = GedeeldeBusNewComponent<A4988>(&Native, HardwareResourcePin::NP_D6, HardwareResourcePin::NP_D7);
  bool gelukt = a4988 != nullptr;
  GA_SERIAL.println(gelukt ? F("RESULTAAT: OK") : F("RESULTAAT: FOUT"));
}
void loop() {}
