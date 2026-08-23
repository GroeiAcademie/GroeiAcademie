// InputkanalenPCF8574metArgumenten.ino
// ILLUSTRATIEF VOORBEELD van de void*-argumentenaanpak voor InputFunctie, toegepast op
// INPUT_TYPE_PCF8574. Zet in UserConfig.h:
// #define INPUT_KANAAL_CONFIG INPUT_TYPE_PCF8574
//
// In tegenstelling tot InputkanalenPCF8574.ino (één functie per opschrift, geen gedeelde
// toestand) demonstreert dit bestand twee technieken:
// - KEYPAD_TYPE_MEMBRAAN_MATRIX_4x3 en KEYPAD_TYPE_MEMBRAAN_MATRIX_4x4: een PIN-code wordt
//   opgebouwd via de cijfer-/lettertoetsen (gedeelde, globale buffer), * bevestigt, # wist.
// - Alle overige KEYPAD_TYPE-waarden: de eenvoudige, rechtstreekse (void*)-cast, zonder
//   gedeelde toestand, zoals bij InputkanalenDIGITALmetArgumenten.ino.
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
// type kan opleveren, elk gekoppeld aan een functie met het argumenten-veld. Zo toont het
// display precies welke fysieke toets herkend werd, niet enkel dat er "een" toets herkend werd.
// Voor INPUT_TYPE_DIGITAL, zie InputkanalenDIGITALmetArgumenten.ino.
// Voor INPUT_TYPE_HX1838, zie InputkanalenHX1838metArgumenten.ino.

#include <GroeiAcademie.h>
#include <Configuratie/Examples.h>
#include <Configuratie/ExamplesConfig.h>

#ifndef INPUT_TEST_WEERGAVE_MS
  #define INPUT_TEST_WEERGAVE_MS 2000UL
#endif

#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
  #include <Adafruit_ST7789.h>
  Adafruit_ST7789 pixelScreen(PIXEL_SCREEN_CS, PIXEL_SCREEN_DC, PIXEL_SCREEN_RST);
#endif

#if KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_DIRECT_1x4 || KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_MATRIX_2x2
void TestFunctie(void* argumenten) {
  const char* opschrift = (const char*)argumenten;
  PrintToScreen("Toets ingedrukt:", opschrift, INPUT_TEST_WEERGAVE_MS);
}

const MappingTussenToetsaanslagEnUitTeVoerenFunctieMetArgumenten mappingTestMenu[] = {
  {_LABEL_OPSCHRIFT_S1, TestFunctie, (void*)"S1"},
  {_LABEL_OPSCHRIFT_S2, TestFunctie, (void*)"S2"},
  {_LABEL_OPSCHRIFT_S3, TestFunctie, (void*)"S3"},
  {_LABEL_OPSCHRIFT_S4, TestFunctie, (void*)"S4"}
};
#elif KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4 || KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_DIRECT_4x1 || KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_MATRIX_1x4 || KEYPAD_TYPE == KEYPAD_TYPE_TOUCH_TTP224_DIRECT_1x4
void TestFunctie(void* argumenten) {
  const char* opschrift = (const char*)argumenten;
  PrintToScreen("Toets ingedrukt:", opschrift, INPUT_TEST_WEERGAVE_MS);
}

const MappingTussenToetsaanslagEnUitTeVoerenFunctieMetArgumenten mappingTestMenu[] = {
  {_LABEL_OPSCHRIFT_1, TestFunctie, (void*)"1"},
  {_LABEL_OPSCHRIFT_2, TestFunctie, (void*)"2"},
  {_LABEL_OPSCHRIFT_3, TestFunctie, (void*)"3"},
  {_LABEL_OPSCHRIFT_4, TestFunctie, (void*)"4"}
};
#elif KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_DIRECT_2x4
void TestFunctie(void* argumenten) {
  const char* opschrift = (const char*)argumenten;
  PrintToScreen("Toets ingedrukt:", opschrift, INPUT_TEST_WEERGAVE_MS);
}

const MappingTussenToetsaanslagEnUitTeVoerenFunctieMetArgumenten mappingTestMenu[] = {
  {_LABEL_OPSCHRIFT_S1, TestFunctie, (void*)"S1"},
  {_LABEL_OPSCHRIFT_S2, TestFunctie, (void*)"S2"},
  {_LABEL_OPSCHRIFT_S3, TestFunctie, (void*)"S3"},
  {_LABEL_OPSCHRIFT_S4, TestFunctie, (void*)"S4"},
  {_LABEL_OPSCHRIFT_S5, TestFunctie, (void*)"S5"},
  {_LABEL_OPSCHRIFT_S6, TestFunctie, (void*)"S6"},
  {_LABEL_OPSCHRIFT_S7, TestFunctie, (void*)"S7"},
  {_LABEL_OPSCHRIFT_S8, TestFunctie, (void*)"S8"}
};
#elif KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_MATRIX_2x4
void TestFunctie(void* argumenten) {
  const char* opschrift = (const char*)argumenten;
  PrintToScreen("Toets ingedrukt:", opschrift, INPUT_TEST_WEERGAVE_MS);
}

const MappingTussenToetsaanslagEnUitTeVoerenFunctieMetArgumenten mappingTestMenu[] = {
  {_LABEL_OPSCHRIFT_1, TestFunctie, (void*)"1"},
  {_LABEL_OPSCHRIFT_2, TestFunctie, (void*)"2"},
  {_LABEL_OPSCHRIFT_3, TestFunctie, (void*)"3"},
  {_LABEL_OPSCHRIFT_4, TestFunctie, (void*)"4"},
  {_LABEL_OPSCHRIFT_5, TestFunctie, (void*)"5"},
  {_LABEL_OPSCHRIFT_6, TestFunctie, (void*)"6"},
  {_LABEL_OPSCHRIFT_7, TestFunctie, (void*)"7"},
  {_LABEL_OPSCHRIFT_8, TestFunctie, (void*)"8"}
};
#elif KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_MATRIX_4x3
struct InvoerBuffer {
  char tekst[9];
  byte lengte;
};

InvoerBuffer buffer = {"", 0};

void FunctieToevoegen(void* argumenten) {
  const char* opschrift = (const char*)argumenten;
  if (buffer.lengte < 8) {
    buffer.tekst[buffer.lengte] = opschrift[0];
    buffer.lengte++;
    buffer.tekst[buffer.lengte] = '\0';
  }
  PrintToScreen("Invoer:", buffer.tekst);
}

void FunctieBevestigen(void* argumenten) {
  (void)argumenten;
  PrintToScreen("Bevestigd:", buffer.tekst, INPUT_TEST_WEERGAVE_MS);
  buffer.lengte = 0;
  buffer.tekst[0] = '\0';
}

void FunctieWissen(void* argumenten) {
  (void)argumenten;
  buffer.lengte = 0;
  buffer.tekst[0] = '\0';
  PrintToScreen("Gewist", "");
}

const MappingTussenToetsaanslagEnUitTeVoerenFunctieMetArgumenten mappingTestMenu[] = {
  {_LABEL_OPSCHRIFT_1, FunctieToevoegen, (void*)"1"},
  {_LABEL_OPSCHRIFT_2, FunctieToevoegen, (void*)"2"},
  {_LABEL_OPSCHRIFT_3, FunctieToevoegen, (void*)"3"},
  {_LABEL_OPSCHRIFT_4, FunctieToevoegen, (void*)"4"},
  {_LABEL_OPSCHRIFT_5, FunctieToevoegen, (void*)"5"},
  {_LABEL_OPSCHRIFT_6, FunctieToevoegen, (void*)"6"},
  {_LABEL_OPSCHRIFT_7, FunctieToevoegen, (void*)"7"},
  {_LABEL_OPSCHRIFT_8, FunctieToevoegen, (void*)"8"},
  {_LABEL_OPSCHRIFT_9, FunctieToevoegen, (void*)"9"},
  {_LABEL_OPSCHRIFT_STER, FunctieBevestigen, nullptr},
  {_LABEL_OPSCHRIFT_0, FunctieToevoegen, (void*)"0"},
  {_LABEL_OPSCHRIFT_HEKJE, FunctieWissen, nullptr}
};
#elif KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_MATRIX_4x4
struct InvoerBuffer {
  char tekst[9];
  byte lengte;
};

InvoerBuffer buffer = {"", 0};

void FunctieToevoegen(void* argumenten) {
  const char* opschrift = (const char*)argumenten;
  if (buffer.lengte < 8) {
    buffer.tekst[buffer.lengte] = opschrift[0];
    buffer.lengte++;
    buffer.tekst[buffer.lengte] = '\0';
  }
  PrintToScreen("Invoer:", buffer.tekst);
}

void FunctieBevestigen(void* argumenten) {
  (void)argumenten;
  PrintToScreen("Bevestigd:", buffer.tekst, INPUT_TEST_WEERGAVE_MS);
  buffer.lengte = 0;
  buffer.tekst[0] = '\0';
}

void FunctieWissen(void* argumenten) {
  (void)argumenten;
  buffer.lengte = 0;
  buffer.tekst[0] = '\0';
  PrintToScreen("Gewist", "");
}

const MappingTussenToetsaanslagEnUitTeVoerenFunctieMetArgumenten mappingTestMenu[] = {
  {_LABEL_OPSCHRIFT_1, FunctieToevoegen, (void*)"1"},
  {_LABEL_OPSCHRIFT_2, FunctieToevoegen, (void*)"2"},
  {_LABEL_OPSCHRIFT_3, FunctieToevoegen, (void*)"3"},
  {_LABEL_OPSCHRIFT_A, FunctieToevoegen, (void*)"A"},
  {_LABEL_OPSCHRIFT_4, FunctieToevoegen, (void*)"4"},
  {_LABEL_OPSCHRIFT_5, FunctieToevoegen, (void*)"5"},
  {_LABEL_OPSCHRIFT_6, FunctieToevoegen, (void*)"6"},
  {_LABEL_OPSCHRIFT_B, FunctieToevoegen, (void*)"B"},
  {_LABEL_OPSCHRIFT_7, FunctieToevoegen, (void*)"7"},
  {_LABEL_OPSCHRIFT_8, FunctieToevoegen, (void*)"8"},
  {_LABEL_OPSCHRIFT_9, FunctieToevoegen, (void*)"9"},
  {_LABEL_OPSCHRIFT_C, FunctieToevoegen, (void*)"C"},
  {_LABEL_OPSCHRIFT_STER, FunctieBevestigen, nullptr},
  {_LABEL_OPSCHRIFT_0, FunctieToevoegen, (void*)"0"},
  {_LABEL_OPSCHRIFT_HEKJE, FunctieWissen, nullptr},
  {_LABEL_OPSCHRIFT_D, FunctieToevoegen, (void*)"D"}
};
#elif KEYPAD_TYPE == KEYPAD_TYPE_TOUCH_TTP229_MATRIX_4x4
void TestFunctie(void* argumenten) {
  const char* opschrift = (const char*)argumenten;
  PrintToScreen("Toets ingedrukt:", opschrift, INPUT_TEST_WEERGAVE_MS);
}

const MappingTussenToetsaanslagEnUitTeVoerenFunctieMetArgumenten mappingTestMenu[] = {
  {_LABEL_OPSCHRIFT_1, TestFunctie, (void*)"1"},
  {_LABEL_OPSCHRIFT_2, TestFunctie, (void*)"2"},
  {_LABEL_OPSCHRIFT_3, TestFunctie, (void*)"3"},
  {_LABEL_OPSCHRIFT_4, TestFunctie, (void*)"4"},
  {_LABEL_OPSCHRIFT_5, TestFunctie, (void*)"5"},
  {_LABEL_OPSCHRIFT_6, TestFunctie, (void*)"6"},
  {_LABEL_OPSCHRIFT_7, TestFunctie, (void*)"7"},
  {_LABEL_OPSCHRIFT_8, TestFunctie, (void*)"8"}
};
#elif KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_MATRIX_4x4
void TestFunctie(void* argumenten) {
  const char* opschrift = (const char*)argumenten;
  PrintToScreen("Toets ingedrukt:", opschrift, INPUT_TEST_WEERGAVE_MS);
}

const MappingTussenToetsaanslagEnUitTeVoerenFunctieMetArgumenten mappingTestMenu[] = {
  {_LABEL_OPSCHRIFT_S1, TestFunctie, (void*)"S1"},
  {_LABEL_OPSCHRIFT_S2, TestFunctie, (void*)"S2"},
  {_LABEL_OPSCHRIFT_S3, TestFunctie, (void*)"S3"},
  {_LABEL_OPSCHRIFT_S4, TestFunctie, (void*)"S4"},
  {_LABEL_OPSCHRIFT_S5, TestFunctie, (void*)"S5"},
  {_LABEL_OPSCHRIFT_S6, TestFunctie, (void*)"S6"},
  {_LABEL_OPSCHRIFT_S7, TestFunctie, (void*)"S7"},
  {_LABEL_OPSCHRIFT_S8, TestFunctie, (void*)"S8"},
  {_LABEL_OPSCHRIFT_S9, TestFunctie, (void*)"S9"},
  {_LABEL_OPSCHRIFT_S10, TestFunctie, (void*)"S10"},
  {_LABEL_OPSCHRIFT_S11, TestFunctie, (void*)"S11"},
  {_LABEL_OPSCHRIFT_S12, TestFunctie, (void*)"S12"},
  {_LABEL_OPSCHRIFT_S13, TestFunctie, (void*)"S13"},
  {_LABEL_OPSCHRIFT_S14, TestFunctie, (void*)"S14"},
  {_LABEL_OPSCHRIFT_S15, TestFunctie, (void*)"S15"},
  {_LABEL_OPSCHRIFT_S16, TestFunctie, (void*)"S16"}
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
}

void loop() {
  PrintToScreen("Input-test PCF8574 (met argumenten)", "druk een toets");
  UitVoerenFunctieVolgensMappingMetToetsAanslag(true, mappingTestMenu);
}
