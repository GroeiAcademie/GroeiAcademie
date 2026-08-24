// InputkanalenPCF8574UserDefinedMatrix.ino
// Testtoepassing voor de Input-laag (InputTypes.h/Input.h/Input.cpp), specifiek voor
// KEYPAD_TYPE_USER_DEFINED_MATRIX. Enkel bij INPUT_TYPE_PCF8574 beschikbaar.

// INPUT TESTEN:
// Dit voorbeeld test uitsluitend KEYPAD_TYPE_USER_DEFINED_MATRIX. Zet in UserConfig.h:
// #define INPUT_KANAAL_CONFIG INPUT_TYPE_PCF8574
// #define KEYPAD_TYPE KEYPAD_TYPE_USER_DEFINED_MATRIX
//
// De KEYPAD_GENERIEK_...-instellingen moeten in UserConfig.h staan. Bij
// KEYPAD_TYPE_USER_DEFINED_MATRIX staat daar een uitgecommentarieerd voorbeeld: haal in
// UserConfig.h de // voor de benodigde #define-regels weg en pas de waarden aan je matrix aan.
// Het onderstaande 4x3-configuratiefragment is uitsluitend een referentie en wordt vanuit
// deze .ino niet meegecompileerd; het komt exact overeen met het voorbeeld in
// UserConfig_template.h en met de mappingTestMenu[] hieronder (12 toetsen). Generieke
// matrix-keypads zijn active-low.

/*
#if defined(INPUT_KANAAL_CONFIG) && ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_PCF8574)
  #if defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_USER_DEFINED_DIRECT
  // ...
  #elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_USER_DEFINED_MATRIX
    #define KEYPAD_GENERIEK_AANTAL_RIJEN     4
    #define KEYPAD_GENERIEK_AANTAL_KOLOMMEN  3
    #define KEYPAD_GENERIEK_RIJ_PINNEN       {PCF8574_PIN_P0, PCF8574_PIN_P1, PCF8574_PIN_P2, PCF8574_PIN_P3}
    #define KEYPAD_GENERIEK_KOLOM_PINNEN     {PCF8574_PIN_P4, PCF8574_PIN_P5, PCF8574_PIN_P6}

    #define KEYPAD_GENERIEK_KEY_LAYOUT { \
      {"1", "Toets 1"}, {"2", "Toets 2"}, {"3", "Toets 3"}, \
      {"4", "Toets 4"}, {"5", "Toets 5"}, {"6", "Toets 6"}, \
      {"7", "Toets 7"}, {"8", "Toets 8"}, {"9", "Toets 9"}, \
      {"*", "Toets *"}, {"0", "Toets 0"}, {"#", "Toets #"} \
    }
  #endif
#endif
*/

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

void TestFunctie1()     { PrintToScreen("Toets ingedrukt:", "1", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie2()     { PrintToScreen("Toets ingedrukt:", "2", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie3()     { PrintToScreen("Toets ingedrukt:", "3", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie4()     { PrintToScreen("Toets ingedrukt:", "4", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie5()     { PrintToScreen("Toets ingedrukt:", "5", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie6()     { PrintToScreen("Toets ingedrukt:", "6", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie7()     { PrintToScreen("Toets ingedrukt:", "7", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie8()     { PrintToScreen("Toets ingedrukt:", "8", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie9()     { PrintToScreen("Toets ingedrukt:", "9", INPUT_TEST_WEERGAVE_MS); }
void TestFunctieSter()  { PrintToScreen("Toets ingedrukt:", "*", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie0()     { PrintToScreen("Toets ingedrukt:", "0", INPUT_TEST_WEERGAVE_MS); }
void TestFunctieHekje() { PrintToScreen("Toets ingedrukt:", "#", INPUT_TEST_WEERGAVE_MS); }

const MappingTussenToetsaanslagEnUitTeVoerenFunctie mappingTestMenu[] = {
  {_LABEL_OPSCHRIFT_1,     TestFunctie1},
  {_LABEL_OPSCHRIFT_2,     TestFunctie2},
  {_LABEL_OPSCHRIFT_3,     TestFunctie3},
  {_LABEL_OPSCHRIFT_4,     TestFunctie4},
  {_LABEL_OPSCHRIFT_5,     TestFunctie5},
  {_LABEL_OPSCHRIFT_6,     TestFunctie6},
  {_LABEL_OPSCHRIFT_7,     TestFunctie7},
  {_LABEL_OPSCHRIFT_8,     TestFunctie8},
  {_LABEL_OPSCHRIFT_9,     TestFunctie9},
  {_LABEL_OPSCHRIFT_STER,  TestFunctieSter},
  {_LABEL_OPSCHRIFT_0,     TestFunctie0},
  {_LABEL_OPSCHRIFT_HEKJE, TestFunctieHekje}
};

void setup() {

#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
  pixelScreen.init(ACTIEF_PIXEL_SCREEN_BREEDTE, ACTIEF_PIXEL_SCREEN_HOOGTE);
  pixelScreen.setRotation(PIXEL_SCREEN_ROTATION);
  PixelScreen = &pixelScreen;
#endif
  ScreensConfigureren();

  InputConfigureren();

#ifdef INPUT_MAPPING_EXTRA_CONTROLES_INSCHAKELEN
  ControleerMappingVolledigheid(mappingTestMenu);
#endif
}

void loop() {
  PrintToScreen("Input-test PCF8574", "druk een toets");
  UitVoerenFunctieVolgensMappingMetToetsAanslag(true, mappingTestMenu);
}
