// ============================================================================
// Sensor: TestOpDubbeleComponentcodes
// ============================================================================
// Compileertest: een dubbele X-macro-componentcode moet een compileerfout veroorzaken.
// De sketch controleert dit via de bestaande componentcodes en een switch-statement.
// ============================================================================

#include <GroeiAcademie.h>
// Compileertest: een dubbele X-macro-componentcode veroorzaakt een duplicate case value.
static void ControleerOpDubbeleComponentcodes(uint8_t code) {
  switch (code) {
#define X(naam, waarde) case waarde: break;
    GEDEELDE_BUS_ELKE_COMPONENT(X)
#undef X
    default: break;
  }
}
void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);

#if defined(DEBUG)
  while (!GA_SERIAL) { ; }
#else
  unsigned long startTijd = millis();
  while (!GA_SERIAL && (millis() - startTijd) < SERIAL_CONNECT_TIMEOUT_MS) { ; }
#endif

  ControleerOpDubbeleComponentcodes(0);
}
void loop() {}
