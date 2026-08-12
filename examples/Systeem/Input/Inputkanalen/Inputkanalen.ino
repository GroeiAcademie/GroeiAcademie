// Inputkanalen.ino
// Testtoepassing voor de Input-laag (InputTypes.h/Input.h/Input.cpp).

// INPUT TESTEN:
// Standaard wordt KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4 gebruikt.
//
// Kies in UserConfig.h het te testen invoerkanaal via INPUT_KANAAL_CONFIG:
// #define INPUT_KANAAL_CONFIG INPUT_TYPE_DIGITAL
// #define INPUT_KANAAL_CONFIG INPUT_TYPE_PCF8574
// #define INPUT_KANAAL_CONFIG INPUT_TYPE_HX1838
// #define INPUT_KANAAL_CONFIG (INPUT_TYPE_PCF8574 | INPUT_TYPE_HX1838)
//
// Alleen de toetsen met opschrift 1 t.e.m. 4 worden in deze test gebruikt.
// Dit geldt ook wanneer het gekozen keypad of de HX1838-remote meer toetsen heeft.

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

void TestFunctie1() {
  PrintToScreen("TestFunctie 1", "", INPUT_TEST_WEERGAVE_MS);
}

void TestFunctie2() {
  PrintToScreen("TestFunctie 2", "", INPUT_TEST_WEERGAVE_MS);
}

void TestFunctie3() {
  PrintToScreen("TestFunctie 3", "", INPUT_TEST_WEERGAVE_MS);
}

void TestFunctie4() {
  PrintToScreen("TestFunctie 4", "", INPUT_TEST_WEERGAVE_MS);
}

const MappingTussenToetsaanslagEnUitTeVoerenFunctie mappingTestMenu[] = {
  {_LABEL_OPSCHRIFT_1, TestFunctie1},
  {_LABEL_OPSCHRIFT_2, TestFunctie2},
  {_LABEL_OPSCHRIFT_3, TestFunctie3},
  {_LABEL_OPSCHRIFT_4, TestFunctie4}
};

void setup() {
  Serial.begin(115200);

#if (SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER)
  CharacterScreenConfigureren();
#endif
#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
  pixelScreen.init(ACTIEF_PIXEL_SCREEN_BREEDTE, ACTIEF_PIXEL_SCREEN_HOOGTE);
  pixelScreen.setRotation(PIXEL_SCREEN_ROTATION);
  PixelScreen = &pixelScreen;
  PixelScreenConfigureren();
#endif

  InputConfigureren();
}

void loop() {
  PrintToScreen(LCD_MENU_L1, LCD_MENU_L2);
  ToonMenuEnUitVoerenFunctieVolgensMappingMetToetsAanslag(true, mappingTestMenu);
}
