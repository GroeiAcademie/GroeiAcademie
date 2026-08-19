// InputkanalenPCF8574.ino
// Testtoepassing voor de Input-laag (InputTypes.h/Input.h/Input.cpp), specifiek voor INPUT_TYPE_PCF8574.

// INPUT TESTEN:
// Dit voorbeeld test uitsluitend INPUT_TYPE_PCF8574. Zet in UserConfig.h:
// #define INPUT_KANAAL_CONFIG INPUT_TYPE_PCF8574
//
// Ondersteunde KEYPAD_TYPE-waarden bij INPUT_TYPE_PCF8574, elk met hun volledige opschrift:
// * KEYPAD_TYPE_DRUKKNOP_DIRECT_1x4      K1, K2, K3, K4, GND [S1, S2, S3, S4]
// * KEYPAD_TYPE_DRUKKNOP_DIRECT_2x4      K1, K2, K3, K4, K5, K6, K7, K8, GND [S1, S2, S3, S4, S5, S6, S7, S8]
//
// - KEYPAD_TYPE_DRUKKNOP_MATRIX_2x2      L1, L2, R1, R2 (OT3688) [S1, S2, S3, S4]
//
// * KEYPAD_TYPE_DRUKKNOP_MATRIX_4x4      C4, C3, C2, C1, R1, R2, R3, R4 (OT3241) [S1, S2, S3, S4, S5, S6, S7, S8, S9, S10, S11, S12, S13, S14, S15, S16]
//
// * KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4      1, 2, 3, 4, GND [1, 2, 3, 4]
// * KEYPAD_TYPE_MEMBRAAN_DIRECT_4x1      1, 2, 3, 4, GND [1, 2, 3, 4]
//
// - KEYPAD_TYPE_MEMBRAAN_MATRIX_1x4      R1, C1, C2, C3, C4 [1, 2, 3, 4]
// - KEYPAD_TYPE_MEMBRAAN_MATRIX_2x4      R1, R2, C1, C2, C3, C4 [1, 2, 3, 4, 5, 6, 7, 8]
// * KEYPAD_TYPE_MEMBRAAN_MATRIX_4x3      R1, R2, R3, R4, C1, C2, C3 [1, 2, 3, 4, 5, 6, 7, 8, 9, *, 0, #]
// - KEYPAD_TYPE_MEMBRAAN_MATRIX_4x4      R1, R2, R3, R4, C1, C2, C3, C4 [1, 2, 3, A, 4, 5, 6, B, 7, 8, 9, C, *, 0, #, D]
//
// * KEYPAD_TYPE_TOUCH_TTP224_DIRECT_1x4  OUT1, OUT2, OUT3, OUT4, GND, VCC [1, 2, 3, 4]
//
// * KEYPAD_TYPE_TOUCH_TTP229_MATRIX_4x4  VCC, GND, (SCL, SDO), OUT1, OUT2, OUT3, OUT4, OUT5, OUT6, OUT7, OUT8 (HW-136) [1, 2, 3, 4, 5, 6, 7, 8] - fysiek 4x4 touch keypad: via de 8 parallelle uitgangen op PCF8574 P0-P7 kunnen enkel de eerste 8 toetsen uitgelezen worden via OUTn
//
// mappingTestMenu[] hieronder bevat, per KEYPAD_TYPE, de volledige set opschriften die dat
// type kan opleveren, elk gekoppeld aan zijn eigen, herkenbare functie. Zo toont het display
// precies welke fysieke toets herkend werd, niet enkel dat er "een" toets herkend werd.
// Voor INPUT_TYPE_DIGITAL, zie InputkanalenDIGITAL.ino.
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
#elif KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4 || KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_DIRECT_4x1 || KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_MATRIX_1x4 || KEYPAD_TYPE == KEYPAD_TYPE_TOUCH_TTP224_DIRECT_1x4
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
#elif KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_DIRECT_2x4
void TestFunctieS1() { PrintToScreen("Toets ingedrukt:", "S1", INPUT_TEST_WEERGAVE_MS); }
void TestFunctieS2() { PrintToScreen("Toets ingedrukt:", "S2", INPUT_TEST_WEERGAVE_MS); }
void TestFunctieS3() { PrintToScreen("Toets ingedrukt:", "S3", INPUT_TEST_WEERGAVE_MS); }
void TestFunctieS4() { PrintToScreen("Toets ingedrukt:", "S4", INPUT_TEST_WEERGAVE_MS); }
void TestFunctieS5() { PrintToScreen("Toets ingedrukt:", "S5", INPUT_TEST_WEERGAVE_MS); }
void TestFunctieS6() { PrintToScreen("Toets ingedrukt:", "S6", INPUT_TEST_WEERGAVE_MS); }
void TestFunctieS7() { PrintToScreen("Toets ingedrukt:", "S7", INPUT_TEST_WEERGAVE_MS); }
void TestFunctieS8() { PrintToScreen("Toets ingedrukt:", "S8", INPUT_TEST_WEERGAVE_MS); }

const MappingTussenToetsaanslagEnUitTeVoerenFunctie mappingTestMenu[] = {
  {_LABEL_OPSCHRIFT_S1, TestFunctieS1},
  {_LABEL_OPSCHRIFT_S2, TestFunctieS2},
  {_LABEL_OPSCHRIFT_S3, TestFunctieS3},
  {_LABEL_OPSCHRIFT_S4, TestFunctieS4},
  {_LABEL_OPSCHRIFT_S5, TestFunctieS5},
  {_LABEL_OPSCHRIFT_S6, TestFunctieS6},
  {_LABEL_OPSCHRIFT_S7, TestFunctieS7},
  {_LABEL_OPSCHRIFT_S8, TestFunctieS8}
};
#elif KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_MATRIX_2x4
void TestFunctie1() { PrintToScreen("Toets ingedrukt:", "1", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie2() { PrintToScreen("Toets ingedrukt:", "2", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie3() { PrintToScreen("Toets ingedrukt:", "3", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie4() { PrintToScreen("Toets ingedrukt:", "4", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie5() { PrintToScreen("Toets ingedrukt:", "5", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie6() { PrintToScreen("Toets ingedrukt:", "6", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie7() { PrintToScreen("Toets ingedrukt:", "7", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie8() { PrintToScreen("Toets ingedrukt:", "8", INPUT_TEST_WEERGAVE_MS); }

const MappingTussenToetsaanslagEnUitTeVoerenFunctie mappingTestMenu[] = {
  {_LABEL_OPSCHRIFT_1, TestFunctie1},
  {_LABEL_OPSCHRIFT_2, TestFunctie2},
  {_LABEL_OPSCHRIFT_3, TestFunctie3},
  {_LABEL_OPSCHRIFT_4, TestFunctie4},
  {_LABEL_OPSCHRIFT_5, TestFunctie5},
  {_LABEL_OPSCHRIFT_6, TestFunctie6},
  {_LABEL_OPSCHRIFT_7, TestFunctie7},
  {_LABEL_OPSCHRIFT_8, TestFunctie8}
};
#elif KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_MATRIX_4x3
void TestFunctie1() { PrintToScreen("Toets ingedrukt:", "1", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie2() { PrintToScreen("Toets ingedrukt:", "2", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie3() { PrintToScreen("Toets ingedrukt:", "3", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie4() { PrintToScreen("Toets ingedrukt:", "4", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie5() { PrintToScreen("Toets ingedrukt:", "5", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie6() { PrintToScreen("Toets ingedrukt:", "6", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie7() { PrintToScreen("Toets ingedrukt:", "7", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie8() { PrintToScreen("Toets ingedrukt:", "8", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie9() { PrintToScreen("Toets ingedrukt:", "9", INPUT_TEST_WEERGAVE_MS); }
void TestFunctieSter() { PrintToScreen("Toets ingedrukt:", "*", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie0() { PrintToScreen("Toets ingedrukt:", "0", INPUT_TEST_WEERGAVE_MS); }
void TestFunctieHekje() { PrintToScreen("Toets ingedrukt:", "#", INPUT_TEST_WEERGAVE_MS); }

const MappingTussenToetsaanslagEnUitTeVoerenFunctie mappingTestMenu[] = {
  {_LABEL_OPSCHRIFT_1, TestFunctie1},
  {_LABEL_OPSCHRIFT_2, TestFunctie2},
  {_LABEL_OPSCHRIFT_3, TestFunctie3},
  {_LABEL_OPSCHRIFT_4, TestFunctie4},
  {_LABEL_OPSCHRIFT_5, TestFunctie5},
  {_LABEL_OPSCHRIFT_6, TestFunctie6},
  {_LABEL_OPSCHRIFT_7, TestFunctie7},
  {_LABEL_OPSCHRIFT_8, TestFunctie8},
  {_LABEL_OPSCHRIFT_9, TestFunctie9},
  {_LABEL_OPSCHRIFT_STER, TestFunctieSter},
  {_LABEL_OPSCHRIFT_0, TestFunctie0},
  {_LABEL_OPSCHRIFT_HEKJE, TestFunctieHekje}
};
#elif KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_MATRIX_4x4
void TestFunctie1() { PrintToScreen("Toets ingedrukt:", "1", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie2() { PrintToScreen("Toets ingedrukt:", "2", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie3() { PrintToScreen("Toets ingedrukt:", "3", INPUT_TEST_WEERGAVE_MS); }
void TestFunctieA() { PrintToScreen("Toets ingedrukt:", "A", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie4() { PrintToScreen("Toets ingedrukt:", "4", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie5() { PrintToScreen("Toets ingedrukt:", "5", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie6() { PrintToScreen("Toets ingedrukt:", "6", INPUT_TEST_WEERGAVE_MS); }
void TestFunctieB() { PrintToScreen("Toets ingedrukt:", "B", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie7() { PrintToScreen("Toets ingedrukt:", "7", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie8() { PrintToScreen("Toets ingedrukt:", "8", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie9() { PrintToScreen("Toets ingedrukt:", "9", INPUT_TEST_WEERGAVE_MS); }
void TestFunctieC() { PrintToScreen("Toets ingedrukt:", "C", INPUT_TEST_WEERGAVE_MS); }
void TestFunctieSter() { PrintToScreen("Toets ingedrukt:", "*", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie0() { PrintToScreen("Toets ingedrukt:", "0", INPUT_TEST_WEERGAVE_MS); }
void TestFunctieHekje() { PrintToScreen("Toets ingedrukt:", "#", INPUT_TEST_WEERGAVE_MS); }
void TestFunctieD() { PrintToScreen("Toets ingedrukt:", "D", INPUT_TEST_WEERGAVE_MS); }

const MappingTussenToetsaanslagEnUitTeVoerenFunctie mappingTestMenu[] = {
  {_LABEL_OPSCHRIFT_1, TestFunctie1},
  {_LABEL_OPSCHRIFT_2, TestFunctie2},
  {_LABEL_OPSCHRIFT_3, TestFunctie3},
  {_LABEL_OPSCHRIFT_A, TestFunctieA},
  {_LABEL_OPSCHRIFT_4, TestFunctie4},
  {_LABEL_OPSCHRIFT_5, TestFunctie5},
  {_LABEL_OPSCHRIFT_6, TestFunctie6},
  {_LABEL_OPSCHRIFT_B, TestFunctieB},
  {_LABEL_OPSCHRIFT_7, TestFunctie7},
  {_LABEL_OPSCHRIFT_8, TestFunctie8},
  {_LABEL_OPSCHRIFT_9, TestFunctie9},
  {_LABEL_OPSCHRIFT_C, TestFunctieC},
  {_LABEL_OPSCHRIFT_STER, TestFunctieSter},
  {_LABEL_OPSCHRIFT_0, TestFunctie0},
  {_LABEL_OPSCHRIFT_HEKJE, TestFunctieHekje},
  {_LABEL_OPSCHRIFT_D, TestFunctieD}
};
#elif KEYPAD_TYPE == KEYPAD_TYPE_TOUCH_TTP229_MATRIX_4x4
void TestFunctie1() { PrintToScreen("Toets ingedrukt:", "1", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie2() { PrintToScreen("Toets ingedrukt:", "2", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie3() { PrintToScreen("Toets ingedrukt:", "3", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie4() { PrintToScreen("Toets ingedrukt:", "4", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie5() { PrintToScreen("Toets ingedrukt:", "5", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie6() { PrintToScreen("Toets ingedrukt:", "6", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie7() { PrintToScreen("Toets ingedrukt:", "7", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie8() { PrintToScreen("Toets ingedrukt:", "8", INPUT_TEST_WEERGAVE_MS); }

const MappingTussenToetsaanslagEnUitTeVoerenFunctie mappingTestMenu[] = {
  {_LABEL_OPSCHRIFT_1, TestFunctie1},
  {_LABEL_OPSCHRIFT_2, TestFunctie2},
  {_LABEL_OPSCHRIFT_3, TestFunctie3},
  {_LABEL_OPSCHRIFT_4, TestFunctie4},
  {_LABEL_OPSCHRIFT_5, TestFunctie5},
  {_LABEL_OPSCHRIFT_6, TestFunctie6},
  {_LABEL_OPSCHRIFT_7, TestFunctie7},
  {_LABEL_OPSCHRIFT_8, TestFunctie8}
};
#elif KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_MATRIX_4x4
void TestFunctieS1() { PrintToScreen("Toets ingedrukt:", "S1", INPUT_TEST_WEERGAVE_MS); }
void TestFunctieS2() { PrintToScreen("Toets ingedrukt:", "S2", INPUT_TEST_WEERGAVE_MS); }
void TestFunctieS3() { PrintToScreen("Toets ingedrukt:", "S3", INPUT_TEST_WEERGAVE_MS); }
void TestFunctieS4() { PrintToScreen("Toets ingedrukt:", "S4", INPUT_TEST_WEERGAVE_MS); }
void TestFunctieS5() { PrintToScreen("Toets ingedrukt:", "S5", INPUT_TEST_WEERGAVE_MS); }
void TestFunctieS6() { PrintToScreen("Toets ingedrukt:", "S6", INPUT_TEST_WEERGAVE_MS); }
void TestFunctieS7() { PrintToScreen("Toets ingedrukt:", "S7", INPUT_TEST_WEERGAVE_MS); }
void TestFunctieS8() { PrintToScreen("Toets ingedrukt:", "S8", INPUT_TEST_WEERGAVE_MS); }
void TestFunctieS9() { PrintToScreen("Toets ingedrukt:", "S9", INPUT_TEST_WEERGAVE_MS); }
void TestFunctieS10() { PrintToScreen("Toets ingedrukt:", "S10", INPUT_TEST_WEERGAVE_MS); }
void TestFunctieS11() { PrintToScreen("Toets ingedrukt:", "S11", INPUT_TEST_WEERGAVE_MS); }
void TestFunctieS12() { PrintToScreen("Toets ingedrukt:", "S12", INPUT_TEST_WEERGAVE_MS); }
void TestFunctieS13() { PrintToScreen("Toets ingedrukt:", "S13", INPUT_TEST_WEERGAVE_MS); }
void TestFunctieS14() { PrintToScreen("Toets ingedrukt:", "S14", INPUT_TEST_WEERGAVE_MS); }
void TestFunctieS15() { PrintToScreen("Toets ingedrukt:", "S15", INPUT_TEST_WEERGAVE_MS); }
void TestFunctieS16() { PrintToScreen("Toets ingedrukt:", "S16", INPUT_TEST_WEERGAVE_MS); }

const MappingTussenToetsaanslagEnUitTeVoerenFunctie mappingTestMenu[] = {
  {_LABEL_OPSCHRIFT_S1, TestFunctieS1},
  {_LABEL_OPSCHRIFT_S2, TestFunctieS2},
  {_LABEL_OPSCHRIFT_S3, TestFunctieS3},
  {_LABEL_OPSCHRIFT_S4, TestFunctieS4},
  {_LABEL_OPSCHRIFT_S5, TestFunctieS5},
  {_LABEL_OPSCHRIFT_S6, TestFunctieS6},
  {_LABEL_OPSCHRIFT_S7, TestFunctieS7},
  {_LABEL_OPSCHRIFT_S8, TestFunctieS8},
  {_LABEL_OPSCHRIFT_S9, TestFunctieS9},
  {_LABEL_OPSCHRIFT_S10, TestFunctieS10},
  {_LABEL_OPSCHRIFT_S11, TestFunctieS11},
  {_LABEL_OPSCHRIFT_S12, TestFunctieS12},
  {_LABEL_OPSCHRIFT_S13, TestFunctieS13},
  {_LABEL_OPSCHRIFT_S14, TestFunctieS14},
  {_LABEL_OPSCHRIFT_S15, TestFunctieS15},
  {_LABEL_OPSCHRIFT_S16, TestFunctieS16}
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
  PrintToScreen("Input-test PCF8574", "druk een toets");
  UitVoerenFunctieVolgensMappingMetToetsAanslag(true, mappingTestMenu);
}
