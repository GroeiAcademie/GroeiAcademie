// InputkanalenHX1838UserDefined.ino
// Testtoepassing voor de Input-laag, specifiek voor HX1838_TOETSENINDELING_REMOTE_USER_DEFINED.
//
// Deze UserDefined-route is in v1.1.1 uitsluitend beschikbaar met HX1838_BRON_CODES_DEFINE.
// De configuratie moet op buildniveau in UserConfig.h staan; defines in deze .ino bereiken
// de apart gecompileerde Input.cpp niet.
//
// Voorbeeld voor UserConfig.h:
// #define INPUT_KANAAL_CONFIG INPUT_TYPE_HX1838
// #define HX1838_BRON_CODES HX1838_BRON_CODES_DEFINE
// #define HX1838_TOETSENINDELING HX1838_TOETSENINDELING_REMOTE_USER_DEFINED
// #define HX1838_GENERIEK_AANTAL_TOETSEN 4
// #define HX1838_GENERIEK_CODES {0x45UL, 0x46UL, 0x47UL, 0x44UL}
// #define HX1838_GENERIEK_KEY_LAYOUT { {"1", "1"}, {"2", "2"}, {"3", "3"}, {"4", "4"} }
//
// De huidige HX1838-laag vergelijkt 8-bit commandwaarden (uint8_t).
//
// Lokale keuze voor dit voorbeeld:
// - gedefinieerd: test via UitVoerenFunctieVolgensMappingMetToetsAanslag().
// - niet gedefinieerd: test rechtstreeks via OpvragenHuidigeToetsAanslag().
#define USER_DEFINED_VIA_MAPPING

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

#ifdef USER_DEFINED_VIA_MAPPING
void TestFunctie1() { PrintToScreen("Toets ingedrukt:", "1", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie2() { PrintToScreen("Toets ingedrukt:", "2", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie3() { PrintToScreen("Toets ingedrukt:", "3", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie4() { PrintToScreen("Toets ingedrukt:", "4", INPUT_TEST_WEERGAVE_MS); }

const MappingTussenToetsaanslagEnUitTeVoerenFunctie mappingTestMenu[] = {
  {_LABEL_OPSCHRIFT_1, TestFunctie1},
  {_LABEL_OPSCHRIFT_2, TestFunctie2},
  {_LABEL_OPSCHRIFT_3, TestFunctie3},
  {_LABEL_OPSCHRIFT_4, TestFunctie4},
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

#if defined(USER_DEFINED_VIA_MAPPING) && defined(INPUT_MAPPING_EXTRA_CONTROLES_INSCHAKELEN)
  // Optioneel, enkel voor testdoeleinden: meldt via Serial welke opschriften van de gecompileerde UserDefined-indeling ontbreken in mappingTestMenu[].
  // Weglaten in productiecode. Enkel actief wanneer INPUT_MAPPING_EXTRA_CONTROLES_INSCHAKELEN in UserConfig.h staat.
  ControleerMappingVolledigheid(mappingTestMenu);
#endif
}

void loop() {
  PrintToScreen("HX1838 UserDefined", "druk een toets");

#ifdef USER_DEFINED_VIA_MAPPING
  UitVoerenFunctieVolgensMappingMetToetsAanslag(true, mappingTestMenu);
#else
  InputResultaat resultaat = OpvragenHuidigeToetsAanslag(true);
  if (resultaat.inputKanaal == InputKanaal::HX1838 && resultaat.opschriftToetsAanslag != nullptr) {
    PrintToScreen("Toets ingedrukt:", resultaat.opschriftToetsAanslag, INPUT_TEST_WEERGAVE_MS);
  }
#endif
}
