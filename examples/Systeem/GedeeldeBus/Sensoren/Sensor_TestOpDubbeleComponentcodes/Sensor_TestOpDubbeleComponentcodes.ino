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
void setup() { ControleerOpDubbeleComponentcodes(0); }
void loop() {}
