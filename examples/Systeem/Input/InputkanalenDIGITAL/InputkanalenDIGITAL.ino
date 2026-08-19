// InputkanalenDIGITAL.ino
// Testtoepassing voor de Input-laag (InputTypes.h/Input.h/Input.cpp), specifiek voor INPUT_TYPE_DIGITAL.

// INPUT TESTEN:
// Dit voorbeeld test uitsluitend INPUT_TYPE_DIGITAL. Zet in UserConfig.h:
// #define INPUT_KANAAL_CONFIG INPUT_TYPE_DIGITAL
//
// Ondersteunde KEYPAD_TYPE-waarden bij INPUT_TYPE_DIGITAL, elk met hun volledige opschrift:
// * KEYPAD_TYPE_DRUKKNOP_DIRECT_1x4      K1, K2, K3, K4, GND [S1, S2, S3, S4]
//
// - KEYPAD_TYPE_DRUKKNOP_MATRIX_2x2      L1, L2, R1, R2 (OT3688) [S1, S2, S3, S4]
//
// * KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4      1, 2, 3, 4, GND [1, 2, 3, 4]
// * KEYPAD_TYPE_MEMBRAAN_DIRECT_4x1      1, 2, 3, 4, GND [1, 2, 3, 4]
//
// * KEYPAD_TYPE_TOUCH_TTP224_DIRECT_1x4  OUT1, OUT2, OUT3, OUT4, GND, VCC [1, 2, 3, 4]
//
// mappingTestMenu[] hieronder bevat, per KEYPAD_TYPE, de volledige set opschriften die dat
// type kan opleveren, elk gekoppeld aan zijn eigen, herkenbare functie. Zo toont het display
// precies welke fysieke toets herkend werd, niet enkel dat er "een" toets herkend werd.
// Voor INPUT_TYPE_PCF8574, zie InputkanalenPCF8574.ino.
// Voor INPUT_TYPE_HX1838, zie InputkanalenHX1838.ino.

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

#if KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_DIRECT_1x4 || KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_MATRIX_2x2
void TestFunctieS1() { PrintToScreen("Toets ingedrukt:", "S1", INPUT_TEST_WEERGAVE_MS); }
void TestFunctieS2() { PrintToScreen("Toets ingedrukt:", "S2", INPUT_TEST_WEERGAVE_MS); }
void TestFunctieS3() { PrintToScreen("Toets ingedrukt:", "S3", INPUT_TEST_WEERGAVE_MS); }
void TestFunctieS4() { PrintToScreen("Toets ingedrukt:", "S4", INPUT_TEST_WEERGAVE_MS); }

const MappingTussenToetsaanslagEnUitTeVoerenFunctie mappingTestMenu[] = {
  {_LABEL_OPSCHRIFT_S1, TestFunctieS1},
  {_LABEL_OPSCHRIFT_S2, TestFunctieS2},
  {_LABEL_OPSCHRIFT_S3, TestFunctieS3},
  {_LABEL_OPSCHRIFT_S4, TestFunctieS4}
};
#elif KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4 || KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_DIRECT_4x1 || KEYPAD_TYPE == KEYPAD_TYPE_TOUCH_TTP224_DIRECT_1x4
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
#endif

void setup() {

#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
  pixelScreen.init(ACTIEF_PIXEL_SCREEN_BREEDTE, ACTIEF_PIXEL_SCREEN_HOOGTE);
  pixelScreen.setRotation(PIXEL_SCREEN_ROTATION);
  PixelScreen = &pixelScreen;
#endif
  ScreensConfigureren();

  InputConfigureren();

#ifdef INPUT_MAPPING_EXTRA_CONTROLES_INSCHAKELEN
  // Optioneel, enkel voor testdoeleinden: meldt via Serial welke opschriften van het gecompileerde KEYPAD_TYPE ontbreken in mappingTestMenu[]. 
  // Weglaten in productiecode. Enkel actief wanneer INPUT_MAPPING_EXTRA_CONTROLES_INSCHAKELEN in UserConfig.h staat.
  ControleerMappingVolledigheid(mappingTestMenu);
#endif
}

void loop() {
  PrintToScreen("Input-test DIGITAL", "druk een toets");
  UitVoerenFunctieVolgensMappingMetToetsAanslag(true, mappingTestMenu);
}
