// InputkanalenUserDefinedDirect.ino
// Testtoepassing voor de Input-laag (InputTypes.h/Input.h/Input.cpp), specifiek voor
// KEYPAD_TYPE_USER_DEFINED_DIRECT. Enkel bij INPUT_TYPE_PCF8574 beschikbaar.

// INPUT TESTEN:
// Dit voorbeeld test uitsluitend KEYPAD_TYPE_USER_DEFINED_DIRECT. Zet in UserConfig.h:
// #define INPUT_KANAAL_CONFIG INPUT_TYPE_PCF8574
// #define KEYPAD_TYPE KEYPAD_TYPE_USER_DEFINED_DIRECT
//
// Dit voorbeeld werkt ook zonder eigen KEYPAD_GENERIEK_...-instellingen in UserConfig.h: hieronder
// staan standaardwaarden (#ifndef), vier PCF8574-bitposities P0-P3, laagactief, opschrift 1-4.
// Sluit je een ander, eigen keypad aan, geef dan de eigen instellingen in UserConfig.h op, die
// krijgen dan voorrang op de standaardwaarden hier.

#ifndef KEYPAD_GENERIEK_AANTAL_PINNEN
  #define KEYPAD_GENERIEK_AANTAL_PINNEN 4
#endif
#ifndef KEYPAD_GENERIEK_PINNEN
  #define KEYPAD_GENERIEK_PINNEN {0, 1, 2, 3}
#endif
#ifndef KEYPAD_GENERIEK_OUTPUT_LEVEL_WHEN_KEY_PRESSED
  #define KEYPAD_GENERIEK_OUTPUT_LEVEL_WHEN_KEY_PRESSED KEYPAD_GENERIEK_OUTPUT_LEVEL_WHEN_KEY_PRESSED_LOW
#endif
#ifndef KEYPAD_GENERIEK_KEY_LAYOUT
  #define KEYPAD_GENERIEK_KEY_LAYOUT { \
    {"1", "Toets 1"}, {"2", "Toets 2"}, {"3", "Toets 3"}, {"4", "Toets 4"} \
  }
#endif

#include <GroeiAcademie.h>
#include <Configuratie/Examples.h>
#include <Configuratie/ExamplesConfig.h>

#ifndef INPUT_TEST_WEERGAVE_MS
  #define INPUT_TEST_WEERGAVE_MS 5000UL
#endif

#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
  #include <Adafruit_ST7789.h>
  Adafruit_ST7789 pixelScreen(PIXEL_SCREEN_CS, PIXEL_SCREEN_DC, PIXEL_SCREEN_RST);
#endif

void TestFunctie1() { PrintToScreen("Toets ingedrukt:", "1", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie2() { PrintToScreen("Toets ingedrukt:", "2", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie3() { PrintToScreen("Toets ingedrukt:", "3", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie4() { PrintToScreen("Toets ingedrukt:", "4", INPUT_TEST_WEERGAVE_MS); }

const MappingTussenToetsaanslagEnUitTeVoerenFunctie mappingTestMenu[] = {
  {_LABEL_OPSCHRIFT_1, TestFunctie1},
  {_LABEL_OPSCHRIFT_2, TestFunctie2},
  {_LABEL_OPSCHRIFT_3, TestFunctie3},
  {_LABEL_OPSCHRIFT_4, TestFunctie4}
};

void setup() {
  InputConfigureren();
  #ifdef INPUT_MAPPING_EXTRA_CONTROLES_INSCHAKELEN
    ControleerMappingVolledigheid(mappingTestMenu);
  #endif
}

void loop() {
  UitVoerenFunctieVolgensMappingMetToetsAanslag(mappingTestMenu);
}
