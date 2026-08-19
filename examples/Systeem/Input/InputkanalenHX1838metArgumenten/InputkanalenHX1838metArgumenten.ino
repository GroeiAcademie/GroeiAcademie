// InputkanalenHX1838metArgumenten.ino
// ILLUSTRATIEF VOORBEELD van de void*-argumentenaanpak voor InputFunctie, toegepast op INPUT_TYPE_HX1838. Zet in UserConfig.h:
// #define INPUT_KANAAL_CONFIG INPUT_TYPE_HX1838
//
// In tegenstelling tot InputkanalenHX1838.ino (één functie per opschrift, 
// geen gedeelde toestand) demonstreert dit bestand twee technieken:
// - HX1838_TOETSENINDELING_REMOTE_OK_BOVENAAN_17_TOETSEN en HX1838_TOETSENINDELING_REMOTE_OK_ONDERAAN_17_TOETSEN: 
//   een PIN-code wordt opgebouwd via de cijfertoetsen (gedeelde, globale buffer), * bevestigt, # wist.
// - HX1838_TOETSENINDELING_REMOTE_21_TOETSEN_MP3: een gedeelde struct (kanaal + volume)
//   wordt door meerdere functies gelezen en geschreven, via &status als argument.
// Alle overige toetsen van elke indeling gebruiken de eenvoudige, rechtstreekse (void*)-cast.
//
// Ondersteunde HX1838_TOETSENINDELING-waarden, elk met hun volledige opschrift:
// - HX1838_TOETSENINDELING_REMOTE_OK_BOVENAAN_17_TOETSEN     remote met 17 toetsen: (UP, DOWN, OK, LEFT, RIGHT, 1-9, *, 0, #) [UP, DOWN, OK, LEFT, RIGHT, 1, 2, 3, 4, 5, 6, 7, 8, 9, *, 0, #]
// - HX1838_TOETSENINDELING_REMOTE_OK_ONDERAAN_17_TOETSEN     remote met 17 toetsen: (1-9, *, 0, #, UP, DOWN, OK, LEFT, RIGHT) [1, 2, 3, 4, 5, 6, 7, 8, 9, *, 0, #, UP, DOWN, OK, LEFT, RIGHT]
// - HX1838_TOETSENINDELING_REMOTE_21_TOETSEN_MP3             remote met 21 toetsen: 0-9 + MP3-toetsen [CH-, CH, CH+, PREV, NEXT, PLAY, -, +, EQ, 0, 100+, 200+, 1, 2, 3, 4, 5, 6, 7, 8, 9]
//
// mappingTestMenu[] hieronder bevat, per HX1838_TOETSENINDELING, de volledige set opschriften die die indeling kan opleveren, 
// elk gekoppeld aan een functie met het argumenten-veld. 
// Zo toont het display precies welke fysieke toets herkend werd, niet enkel dat er "een" toets herkend werd.
// Voor INPUT_TYPE_DIGITAL, zie InputkanalenDIGITALmetArgumenten.ino.
// Voor INPUT_TYPE_PCF8574, zie InputkanalenPCF8574metArgumenten.ino.

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

#if HX1838_TOETSENINDELING == HX1838_TOETSENINDELING_REMOTE_OK_BOVENAAN_17_TOETSEN || HX1838_TOETSENINDELING == HX1838_TOETSENINDELING_REMOTE_OK_ONDERAAN_17_TOETSEN
struct InvoerBuffer {
  char tekst[9];
  byte lengte;
};

InvoerBuffer buffer = {"", 0};
int navigatieTeller = 0;

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

void FunctieNavigatieOmhoog(void* argumenten) {
  (void)argumenten;
  navigatieTeller++;
  PrintToScreen("Navigatie:", String(navigatieTeller));
}

void FunctieNavigatieOmlaag(void* argumenten) {
  (void)argumenten;
  navigatieTeller--;
  PrintToScreen("Navigatie:", String(navigatieTeller));
}

void TestFunctie(void* argumenten) {
  const char* opschrift = (const char*)argumenten;
  PrintToScreen("Toets ingedrukt:", opschrift, INPUT_TEST_WEERGAVE_MS);
}

  #if HX1838_TOETSENINDELING == HX1838_TOETSENINDELING_REMOTE_OK_BOVENAAN_17_TOETSEN
const MappingTussenToetsaanslagEnUitTeVoerenFunctieMetArgumenten mappingTestMenu[] = {
  {_LABEL_OPSCHRIFT_UP,    FunctieNavigatieOmhoog, nullptr},
  {_LABEL_OPSCHRIFT_DOWN,  FunctieNavigatieOmlaag, nullptr},
  {_LABEL_OPSCHRIFT_OK,    TestFunctie,            (void*)"OK"},
  {_LABEL_OPSCHRIFT_LEFT,  TestFunctie,            (void*)"LEFT"},
  {_LABEL_OPSCHRIFT_RIGHT, TestFunctie,            (void*)"RIGHT"},
  {_LABEL_OPSCHRIFT_1,     FunctieToevoegen,       (void*)"1"},
  {_LABEL_OPSCHRIFT_2,     FunctieToevoegen,       (void*)"2"},
  {_LABEL_OPSCHRIFT_3,     FunctieToevoegen,       (void*)"3"},
  {_LABEL_OPSCHRIFT_4,     FunctieToevoegen,       (void*)"4"},
  {_LABEL_OPSCHRIFT_5,     FunctieToevoegen,       (void*)"5"},
  {_LABEL_OPSCHRIFT_6,     FunctieToevoegen,       (void*)"6"},
  {_LABEL_OPSCHRIFT_7,     FunctieToevoegen,       (void*)"7"},
  {_LABEL_OPSCHRIFT_8,     FunctieToevoegen,       (void*)"8"},
  {_LABEL_OPSCHRIFT_9,     FunctieToevoegen,       (void*)"9"},
  {_LABEL_OPSCHRIFT_STER,  FunctieBevestigen,      nullptr},
  {_LABEL_OPSCHRIFT_0,     FunctieToevoegen,       (void*)"0"},
  {_LABEL_OPSCHRIFT_HEKJE, FunctieWissen,          nullptr},
};
  #elif HX1838_TOETSENINDELING == HX1838_TOETSENINDELING_REMOTE_OK_ONDERAAN_17_TOETSEN
const MappingTussenToetsaanslagEnUitTeVoerenFunctieMetArgumenten mappingTestMenu[] = {
  {_LABEL_OPSCHRIFT_1,     FunctieToevoegen,       (void*)"1"},
  {_LABEL_OPSCHRIFT_2,     FunctieToevoegen,       (void*)"2"},
  {_LABEL_OPSCHRIFT_3,     FunctieToevoegen,       (void*)"3"},
  {_LABEL_OPSCHRIFT_4,     FunctieToevoegen,       (void*)"4"},
  {_LABEL_OPSCHRIFT_5,     FunctieToevoegen,       (void*)"5"},
  {_LABEL_OPSCHRIFT_6,     FunctieToevoegen,       (void*)"6"},
  {_LABEL_OPSCHRIFT_7,     FunctieToevoegen,       (void*)"7"},
  {_LABEL_OPSCHRIFT_8,     FunctieToevoegen,       (void*)"8"},
  {_LABEL_OPSCHRIFT_9,     FunctieToevoegen,       (void*)"9"},
  {_LABEL_OPSCHRIFT_STER,  FunctieBevestigen,      nullptr},
  {_LABEL_OPSCHRIFT_0,     FunctieToevoegen,       (void*)"0"},
  {_LABEL_OPSCHRIFT_HEKJE, FunctieWissen,          nullptr},
  {_LABEL_OPSCHRIFT_UP,    FunctieNavigatieOmhoog, nullptr},
  {_LABEL_OPSCHRIFT_DOWN,  FunctieNavigatieOmlaag, nullptr},
  {_LABEL_OPSCHRIFT_OK,    TestFunctie,            (void*)"OK"},
  {_LABEL_OPSCHRIFT_LEFT,  TestFunctie,            (void*)"LEFT"},
  {_LABEL_OPSCHRIFT_RIGHT, TestFunctie,            (void*)"RIGHT"},
};
  #endif
#elif HX1838_TOETSENINDELING == HX1838_TOETSENINDELING_REMOTE_21_TOETSEN_MP3
struct AfstandsbedieningStatus {
  int kanaal;
  int volume;
};

AfstandsbedieningStatus status = {1, 50};

void FunctieKanaalOmhoog(void* argumenten) {
  AfstandsbedieningStatus* s = (AfstandsbedieningStatus*)argumenten;
  s->kanaal++;
}

void FunctieKanaalOmlaag(void* argumenten) {
  AfstandsbedieningStatus* s = (AfstandsbedieningStatus*)argumenten;
  s->kanaal--;
}

void FunctieVolumeOmhoog(void* argumenten) {
  AfstandsbedieningStatus* s = (AfstandsbedieningStatus*)argumenten;
  s->volume += 5;
}

void FunctieVolumeOmlaag(void* argumenten) {
  AfstandsbedieningStatus* s = (AfstandsbedieningStatus*)argumenten;
  s->volume -= 5;
}

void FunctieToonStatus(void* argumenten) {
  AfstandsbedieningStatus* s = (AfstandsbedieningStatus*)argumenten;
  PrintToScreen("Kanaal: " + String(s->kanaal), "Volume: " + String(s->volume));
}

void TestFunctie(void* argumenten) {
  const char* opschrift = (const char*)argumenten;
  PrintToScreen("Toets ingedrukt:", opschrift, INPUT_TEST_WEERGAVE_MS);
}

const MappingTussenToetsaanslagEnUitTeVoerenFunctieMetArgumenten mappingTestMenu[] = {
  {_LABEL_OPSCHRIFT_CH_MINUS, FunctieKanaalOmlaag, &status},
  {_LABEL_OPSCHRIFT_CH,       TestFunctie,         (void*)"CH"},
  {_LABEL_OPSCHRIFT_CH_PLUS,  FunctieKanaalOmhoog, &status},
  {_LABEL_OPSCHRIFT_PREV,     TestFunctie,         (void*)"PREV"},
  {_LABEL_OPSCHRIFT_NEXT,     TestFunctie,         (void*)"NEXT"},
  {_LABEL_OPSCHRIFT_PLAY,     FunctieToonStatus,   &status},
  {_LABEL_OPSCHRIFT_MINUS,    FunctieVolumeOmlaag, &status},
  {_LABEL_OPSCHRIFT_PLUS,     FunctieVolumeOmhoog, &status},
  {_LABEL_OPSCHRIFT_EQ,       TestFunctie,         (void*)"EQ"},
  {_LABEL_OPSCHRIFT_0,        TestFunctie,         (void*)"0"},
  {_LABEL_OPSCHRIFT_100_PLUS, TestFunctie,         (void*)"100+"},
  {_LABEL_OPSCHRIFT_200_PLUS, TestFunctie,         (void*)"200+"},
  {_LABEL_OPSCHRIFT_1,        TestFunctie,         (void*)"1"},
  {_LABEL_OPSCHRIFT_2,        TestFunctie,         (void*)"2"},
  {_LABEL_OPSCHRIFT_3,        TestFunctie,         (void*)"3"},
  {_LABEL_OPSCHRIFT_4,        TestFunctie,         (void*)"4"},
  {_LABEL_OPSCHRIFT_5,        TestFunctie,         (void*)"5"},
  {_LABEL_OPSCHRIFT_6,        TestFunctie,         (void*)"6"},
  {_LABEL_OPSCHRIFT_7,        TestFunctie,         (void*)"7"},
  {_LABEL_OPSCHRIFT_8,        TestFunctie,         (void*)"8"},
  {_LABEL_OPSCHRIFT_9,        TestFunctie,         (void*)"9"}
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
  PrintToScreen("Input-test HX1838 (met argumenten)", "druk een toets");
  UitVoerenFunctieVolgensMappingMetToetsAanslag(true, mappingTestMenu);
}
