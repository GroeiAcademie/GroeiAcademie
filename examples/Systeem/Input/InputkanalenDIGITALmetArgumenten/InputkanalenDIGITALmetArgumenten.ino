// InputkanalenDIGITALmetArgumenten.ino
// Testtoepassing voor de Input-laag, specifiek voor INPUT_TYPE_DIGITAL, die de
// void*-argumentenaanpak voor InputFunctie demonstreert.

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
// In tegenstelling tot InputkanalenDIGITAL.ino (één functie per opschrift) toont dit bestand
// hoe één enkele, gedeelde functie per KEYPAD_TYPE-tak kan volstaan, door via het nieuwe
// argumenten-veld (void*) door te geven welke concrete gegevens bij die ene toets horen.
//
// mappingTestMenu[] hieronder bevat, per KEYPAD_TYPE, de volledige set opschriften die dat
// type kan opleveren, elk gekoppeld aan een functie met het argumenten-veld. Zo toont het
// display precies welke fysieke toets herkend werd, niet enkel dat er "een" toets herkend werd.
// Voor INPUT_TYPE_PCF8574, zie InputkanalenPCF8574metArgumenten.ino.
// Voor INPUT_TYPE_HX1838, zie InputkanalenHX1838metArgumenten.ino.

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
#elif KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4 || KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_DIRECT_4x1 || KEYPAD_TYPE == KEYPAD_TYPE_TOUCH_TTP224_DIRECT_1x4
// Eén struct, telkens gevuld met wat er bij dat specifieke opschrift hoort.
// Ter vergelijking met de rechtstreekse (void*)-cast in de eerste tak hierboven:
// twee verschillende technieken, naast elkaar, bewust zo gehouden.
struct ToetsData {
  const char* opschrift;
};

void TestFunctie(void* argumenten) {
  ToetsData* data = (ToetsData*)argumenten;
  PrintToScreen("Toets ingedrukt:", data->opschrift, INPUT_TEST_WEERGAVE_MS);
}

ToetsData data1 = {"1"};
ToetsData data2 = {"2"};
ToetsData data3 = {"3"};
ToetsData data4 = {"4"};

const MappingTussenToetsaanslagEnUitTeVoerenFunctieMetArgumenten mappingTestMenu[] = {
  {_LABEL_OPSCHRIFT_1, TestFunctie, &data1},
  {_LABEL_OPSCHRIFT_2, TestFunctie, &data2},
  {_LABEL_OPSCHRIFT_3, TestFunctie, &data3},
  {_LABEL_OPSCHRIFT_4, TestFunctie, &data4}
};
#endif

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
  PrintToScreen("Input-test DIGITAL (met argumenten)", "druk een toets");
  UitVoerenFunctieVolgensMappingMetToetsAanslag(true, mappingTestMenu);
}
