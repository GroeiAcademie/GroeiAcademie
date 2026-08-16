// InputkanalenUserDefinedMatrix.ino
// Testtoepassing voor de Input-laag (InputTypes.h/Input.h/Input.cpp), specifiek voor
// KEYPAD_TYPE_USER_DEFINED_MATRIX. Enkel bij INPUT_TYPE_PCF8574 beschikbaar.

// INPUT TESTEN:
// Dit voorbeeld test uitsluitend KEYPAD_TYPE_USER_DEFINED_MATRIX. Zet in UserConfig.h:
// #define INPUT_KANAAL_CONFIG INPUT_TYPE_PCF8574
// #define KEYPAD_TYPE KEYPAD_TYPE_USER_DEFINED_MATRIX
//
// Dit voorbeeld werkt ook zonder eigen KEYPAD_GENERIEK_...-instellingen in UserConfig.h: hieronder
// staan standaardwaarden (#ifndef), een 3x3-matrix op PCF8574-bitposities P0-P2 (rijen) en
// P3-P5 (kolommen), opschrift 1-9. Sluit je een andere, eigen matrix aan, geef dan de eigen
// instellingen in UserConfig.h op, die krijgen dan voorrang op de standaardwaarden hier.

#ifndef KEYPAD_GENERIEK_RIJ_PINNEN
  #define KEYPAD_GENERIEK_RIJ_PINNEN {0, 1, 2}
#endif
#ifndef KEYPAD_GENERIEK_AANTAL_RIJEN
  #define KEYPAD_GENERIEK_AANTAL_RIJEN 3
#endif
#ifndef KEYPAD_GENERIEK_KOLOM_PINNEN
  #define KEYPAD_GENERIEK_KOLOM_PINNEN {3, 4, 5}
#endif
#ifndef KEYPAD_GENERIEK_AANTAL_KOLOMMEN
  #define KEYPAD_GENERIEK_AANTAL_KOLOMMEN 3
#endif
#ifndef KEYPAD_GENERIEK_KEY_LAYOUT
  #define KEYPAD_GENERIEK_KEY_LAYOUT { \
    {"1", "Toets 1"}, {"2", "Toets 2"}, {"3", "Toets 3"}, \
    {"4", "Toets 4"}, {"5", "Toets 5"}, {"6", "Toets 6"}, \
    {"7", "Toets 7"}, {"8", "Toets 8"}, {"9", "Toets 9"} \
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
void TestFunctie5() { PrintToScreen("Toets ingedrukt:", "5", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie6() { PrintToScreen("Toets ingedrukt:", "6", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie7() { PrintToScreen("Toets ingedrukt:", "7", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie8() { PrintToScreen("Toets ingedrukt:", "8", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie9() { PrintToScreen("Toets ingedrukt:", "9", INPUT_TEST_WEERGAVE_MS); }

const MappingTussenToetsaanslagEnUitTeVoerenFunctie mappingTestMenu[] = {
  {_LABEL_OPSCHRIFT_1, TestFunctie1},
  {_LABEL_OPSCHRIFT_2, TestFunctie2},
  {_LABEL_OPSCHRIFT_3, TestFunctie3},
  {_LABEL_OPSCHRIFT_4, TestFunctie4},
  {_LABEL_OPSCHRIFT_5, TestFunctie5},
  {_LABEL_OPSCHRIFT_6, TestFunctie6},
  {_LABEL_OPSCHRIFT_7, TestFunctie7},
  {_LABEL_OPSCHRIFT_8, TestFunctie8},
  {_LABEL_OPSCHRIFT_9, TestFunctie9}
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
