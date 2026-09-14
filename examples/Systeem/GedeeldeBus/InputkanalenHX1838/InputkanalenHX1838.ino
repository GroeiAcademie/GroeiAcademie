// InputkanalenHX1838.ino
// Testtoepassing voor de Input-laag (InputTypes.h/Input.h/Input.cpp), specifiek voor INPUT_TYPE_HX1838.

// Schakel GEDEELDE_BUS_PROTOTYPE om de nieuwe GedeeldeBus-functionaliteit te kunnen testen.
// Zonder deze regel wordt alleen de bestaande, stabiele GedeeldeBus-code gebruikt en krijgt deze .ino compileerfouten.
// Verwijder in 'SystemConfig.h' dus // voor '// #define GEDEELDE_BUS_PROTOTYPE' om deze prototypefuncties beschikbaar te maken.

// INPUT TESTEN:
// Dit voorbeeld test uitsluitend INPUT_TYPE_HX1838. Zet in UserConfig.h: #define INPUT_KANAAL_CONFIG INPUT_TYPE_HX1838
//
// Dit voorbeeld behandelt de drie vaste HX1838_TOETSENINDELING-waarden, elk met hun volledige opschrift:
// - HX1838_TOETSENINDELING_REMOTE_OK_BOVENAAN_17_TOETSEN     remote met 17 toetsen: (UP, DOWN, OK, LEFT, RIGHT, 1-9, *, 0, #) [UP, DOWN, OK, LEFT, RIGHT, 1, 2, 3, 4, 5, 6, 7, 8, 9, *, 0, #]
// - HX1838_TOETSENINDELING_REMOTE_OK_ONDERAAN_17_TOETSEN     remote met 17 toetsen: (1-9, *, 0, #, UP, DOWN, OK, LEFT, RIGHT) [1, 2, 3, 4, 5, 6, 7, 8, 9, *, 0, #, UP, DOWN, OK, LEFT, RIGHT]
// - HX1838_TOETSENINDELING_REMOTE_21_TOETSEN_MP3             remote met 21 toetsen: 0-9 + MP3-toetsen [CH-, CH, CH+, PREV, NEXT, PLAY, -, +, EQ, 0, 100+, 200+, 1, 2, 3, 4, 5, 6, 7, 8, 9]
// Voor HX1838_TOETSENINDELING_REMOTE_USER_DEFINED, zie InputkanalenHX1838UserDefined.ino.
//
// mappingTestMenu[] hieronder bevat, per HX1838_TOETSENINDELING, 
// de volledige set opschriften die die indeling kan opleveren, elk gekoppeld aan zijn eigen, herkenbare functie. 
// Zo toont het display precies welke fysieke toets herkend werd, niet enkel dat er "een" toets herkend werd.
// Voor INPUT_TYPE_DIGITAL, zie InputkanalenDIGITAL.ino.
// Voor INPUT_TYPE_PCF8574, zie InputkanalenPCF8574.ino.

#include <GroeiAcademie.h>
#include <Configuratie/Examples.h>
#include <Configuratie/ExamplesConfig.h>

#include <Systeem/GedeeldeBus/GedeeldeBus.h>

bool ScreenAanmeldenHardwareResourcesOpGedeeldeBus() {
#if (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
  if (!UARTAanmeldenOpGedeeldeBus(GedeeldeBusComponent::SERIAL, true)) return false;
#endif

#if (SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER)
  if (!I2CAanmeldenOpGedeeldeBus(GedeeldeBusComponent::CHARACTER_SCREEN, I2C_ADDRESS_CHARACTER_SCREEN, HardwareResourcePin::SDA, HardwareResourcePin::SCL, true)) return false;
#endif

#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
  if (!SPIAanmeldenOpGedeeldeBus(GedeeldeBusComponent::PIXEL_SCREEN, HardwareResourcePin::MISO, HardwareResourcePin::MOSI, HardwareResourcePin::SCK, true)) return false;
  if (!AanmeldenHardwareResourcesOpGedeeldeBus(GedeeldeBusComponent::PIXEL_SCREEN, HardwareResourceType::GPIO, {PIXEL_SCREEN_CS, PIXEL_SCREEN_DC, PIXEL_SCREEN_RST}, HardwareResourceToegang::EXCLUSIEF, true)) return false;
#endif

  return true;
}

bool InputAanmeldenHardwareResourcesOpGedeeldeBus() {
  if (!AanmeldenHardwareResourcesOpGedeeldeBus(GedeeldeBusComponent::INPUT_HX1838, HardwareResourceType::IR, HardwareResourcePin::CUSTOM, HardwareResourceToegang::EXCLUSIEF, true, GedeeldeBusRol::AUTONOOM, GEDEELDE_BUS_GEEN_ADRES, HX1838_ONTVANGER_PIN)) return false;
#if HX1838_USE_TINYIRRECEIVER_INSTEAD_OF_IRREMOTE == 1
  if (!AanmeldenHardwareResourcesOpGedeeldeBus(GedeeldeBusComponent::INPUT_HX1838, HardwareResourceType::INTERRUPT, HardwareResourcePin::CUSTOM, HardwareResourceToegang::EXCLUSIEF, true, GedeeldeBusRol::AUTONOOM, GEDEELDE_BUS_GEEN_ADRES, HX1838_ONTVANGER_PIN)) return false;
#endif
#if HX1838_BRON_CODES == HX1838_BRON_CODES_EEPROM_ALTIJD || HX1838_BRON_CODES == HX1838_BRON_CODES_EEPROM_WANNEER_GEEN_DEFINE
  if (!EEPROMAanmeldenOpGedeeldeBus(GedeeldeBusComponent::INPUT_HX1838, true)) return false;
#endif
  return true;
}


#ifndef INPUT_TEST_WEERGAVE_MS
  #define INPUT_TEST_WEERGAVE_MS 2000UL
#endif

#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
  #include <Adafruit_ST7789.h>
  Adafruit_ST7789 pixelScreen(PIXEL_SCREEN_CS, PIXEL_SCREEN_DC, PIXEL_SCREEN_RST);
#endif

#if HX1838_TOETSENINDELING == HX1838_TOETSENINDELING_REMOTE_OK_BOVENAAN_17_TOETSEN
void TestFunctieUp()    { PrintToScreen("Toets ingedrukt:", "UP",    INPUT_TEST_WEERGAVE_MS); }
void TestFunctieDown()  { PrintToScreen("Toets ingedrukt:", "DOWN",  INPUT_TEST_WEERGAVE_MS); }
void TestFunctieOk()    { PrintToScreen("Toets ingedrukt:", "OK",    INPUT_TEST_WEERGAVE_MS); }
void TestFunctieLeft()  { PrintToScreen("Toets ingedrukt:", "LEFT",  INPUT_TEST_WEERGAVE_MS); }
void TestFunctieRight() { PrintToScreen("Toets ingedrukt:", "RIGHT", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie1()     { PrintToScreen("Toets ingedrukt:", "1",     INPUT_TEST_WEERGAVE_MS); }
void TestFunctie2()     { PrintToScreen("Toets ingedrukt:", "2",     INPUT_TEST_WEERGAVE_MS); }
void TestFunctie3()     { PrintToScreen("Toets ingedrukt:", "3",     INPUT_TEST_WEERGAVE_MS); }
void TestFunctie4()     { PrintToScreen("Toets ingedrukt:", "4",     INPUT_TEST_WEERGAVE_MS); }
void TestFunctie5()     { PrintToScreen("Toets ingedrukt:", "5",     INPUT_TEST_WEERGAVE_MS); }
void TestFunctie6()     { PrintToScreen("Toets ingedrukt:", "6",     INPUT_TEST_WEERGAVE_MS); }
void TestFunctie7()     { PrintToScreen("Toets ingedrukt:", "7",     INPUT_TEST_WEERGAVE_MS); }
void TestFunctie8()     { PrintToScreen("Toets ingedrukt:", "8",     INPUT_TEST_WEERGAVE_MS); }
void TestFunctie9()     { PrintToScreen("Toets ingedrukt:", "9",     INPUT_TEST_WEERGAVE_MS); }
void TestFunctieSter()  { PrintToScreen("Toets ingedrukt:", "*",     INPUT_TEST_WEERGAVE_MS); }
void TestFunctie0()     { PrintToScreen("Toets ingedrukt:", "0",     INPUT_TEST_WEERGAVE_MS); }
void TestFunctieHekje() { PrintToScreen("Toets ingedrukt:", "#",     INPUT_TEST_WEERGAVE_MS); }

const MappingTussenToetsaanslagEnUitTeVoerenFunctie mappingTestMenu[] = {
  {_LABEL_OPSCHRIFT_UP,    TestFunctieUp},
  {_LABEL_OPSCHRIFT_DOWN,  TestFunctieDown},
  {_LABEL_OPSCHRIFT_OK,    TestFunctieOk},
  {_LABEL_OPSCHRIFT_LEFT,  TestFunctieLeft},
  {_LABEL_OPSCHRIFT_RIGHT, TestFunctieRight},
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
  {_LABEL_OPSCHRIFT_HEKJE, TestFunctieHekje},
};
#elif HX1838_TOETSENINDELING == HX1838_TOETSENINDELING_REMOTE_OK_ONDERAAN_17_TOETSEN
void TestFunctie1()     { PrintToScreen("Toets ingedrukt:", "1",     INPUT_TEST_WEERGAVE_MS); }
void TestFunctie2()     { PrintToScreen("Toets ingedrukt:", "2",     INPUT_TEST_WEERGAVE_MS); }
void TestFunctie3()     { PrintToScreen("Toets ingedrukt:", "3",     INPUT_TEST_WEERGAVE_MS); }
void TestFunctie4()     { PrintToScreen("Toets ingedrukt:", "4",     INPUT_TEST_WEERGAVE_MS); }
void TestFunctie5()     { PrintToScreen("Toets ingedrukt:", "5",     INPUT_TEST_WEERGAVE_MS); }
void TestFunctie6()     { PrintToScreen("Toets ingedrukt:", "6",     INPUT_TEST_WEERGAVE_MS); }
void TestFunctie7()     { PrintToScreen("Toets ingedrukt:", "7",     INPUT_TEST_WEERGAVE_MS); }
void TestFunctie8()     { PrintToScreen("Toets ingedrukt:", "8",     INPUT_TEST_WEERGAVE_MS); }
void TestFunctie9()     { PrintToScreen("Toets ingedrukt:", "9",     INPUT_TEST_WEERGAVE_MS); }
void TestFunctieSter()  { PrintToScreen("Toets ingedrukt:", "*",     INPUT_TEST_WEERGAVE_MS); }
void TestFunctie0()     { PrintToScreen("Toets ingedrukt:", "0",     INPUT_TEST_WEERGAVE_MS); }
void TestFunctieHekje() { PrintToScreen("Toets ingedrukt:", "#",     INPUT_TEST_WEERGAVE_MS); }
void TestFunctieUp()    { PrintToScreen("Toets ingedrukt:", "UP",    INPUT_TEST_WEERGAVE_MS); }
void TestFunctieDown()  { PrintToScreen("Toets ingedrukt:", "DOWN",  INPUT_TEST_WEERGAVE_MS); }
void TestFunctieOk()    { PrintToScreen("Toets ingedrukt:", "OK",    INPUT_TEST_WEERGAVE_MS); }
void TestFunctieLeft()  { PrintToScreen("Toets ingedrukt:", "LEFT",  INPUT_TEST_WEERGAVE_MS); }
void TestFunctieRight() { PrintToScreen("Toets ingedrukt:", "RIGHT", INPUT_TEST_WEERGAVE_MS); }

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
  {_LABEL_OPSCHRIFT_HEKJE, TestFunctieHekje},
  {_LABEL_OPSCHRIFT_UP,    TestFunctieUp},
  {_LABEL_OPSCHRIFT_DOWN,  TestFunctieDown},
  {_LABEL_OPSCHRIFT_OK,    TestFunctieOk},
  {_LABEL_OPSCHRIFT_LEFT,  TestFunctieLeft},
  {_LABEL_OPSCHRIFT_RIGHT, TestFunctieRight},
};
#elif HX1838_TOETSENINDELING == HX1838_TOETSENINDELING_REMOTE_21_TOETSEN_MP3
void TestFunctieChMinus() { PrintToScreen("Toets ingedrukt:", "CH-",  INPUT_TEST_WEERGAVE_MS); }
void TestFunctieCh()      { PrintToScreen("Toets ingedrukt:", "CH",   INPUT_TEST_WEERGAVE_MS); }
void TestFunctieChPlus()  { PrintToScreen("Toets ingedrukt:", "CH+",  INPUT_TEST_WEERGAVE_MS); }
void TestFunctiePrev()    { PrintToScreen("Toets ingedrukt:", "PREV", INPUT_TEST_WEERGAVE_MS); }
void TestFunctieNext()    { PrintToScreen("Toets ingedrukt:", "NEXT", INPUT_TEST_WEERGAVE_MS); }
void TestFunctiePlay()    { PrintToScreen("Toets ingedrukt:", "PLAY", INPUT_TEST_WEERGAVE_MS); }
void TestFunctieMinus()   { PrintToScreen("Toets ingedrukt:", "-",    INPUT_TEST_WEERGAVE_MS); }
void TestFunctiePlus()    { PrintToScreen("Toets ingedrukt:", "+",    INPUT_TEST_WEERGAVE_MS); }
void TestFunctieEq()      { PrintToScreen("Toets ingedrukt:", "EQ",   INPUT_TEST_WEERGAVE_MS); }
void TestFunctie0()       { PrintToScreen("Toets ingedrukt:", "0",    INPUT_TEST_WEERGAVE_MS); }
void TestFunctie100Plus() { PrintToScreen("Toets ingedrukt:", "100+", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie200Plus() { PrintToScreen("Toets ingedrukt:", "200+", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie1()       { PrintToScreen("Toets ingedrukt:", "1",    INPUT_TEST_WEERGAVE_MS); }
void TestFunctie2()       { PrintToScreen("Toets ingedrukt:", "2",    INPUT_TEST_WEERGAVE_MS); }
void TestFunctie3()       { PrintToScreen("Toets ingedrukt:", "3",    INPUT_TEST_WEERGAVE_MS); }
void TestFunctie4()       { PrintToScreen("Toets ingedrukt:", "4",    INPUT_TEST_WEERGAVE_MS); }
void TestFunctie5()       { PrintToScreen("Toets ingedrukt:", "5",    INPUT_TEST_WEERGAVE_MS); }
void TestFunctie6()       { PrintToScreen("Toets ingedrukt:", "6",    INPUT_TEST_WEERGAVE_MS); }
void TestFunctie7()       { PrintToScreen("Toets ingedrukt:", "7",    INPUT_TEST_WEERGAVE_MS); }
void TestFunctie8()       { PrintToScreen("Toets ingedrukt:", "8",    INPUT_TEST_WEERGAVE_MS); }
void TestFunctie9()       { PrintToScreen("Toets ingedrukt:", "9",    INPUT_TEST_WEERGAVE_MS); }

const MappingTussenToetsaanslagEnUitTeVoerenFunctie mappingTestMenu[] = {
  {_LABEL_OPSCHRIFT_CH_MINUS, TestFunctieChMinus},
  {_LABEL_OPSCHRIFT_CH,       TestFunctieCh},
  {_LABEL_OPSCHRIFT_CH_PLUS,  TestFunctieChPlus},
  {_LABEL_OPSCHRIFT_PREV,     TestFunctiePrev},
  {_LABEL_OPSCHRIFT_NEXT,     TestFunctieNext},
  {_LABEL_OPSCHRIFT_PLAY,     TestFunctiePlay},
  {_LABEL_OPSCHRIFT_MINUS,    TestFunctieMinus},
  {_LABEL_OPSCHRIFT_PLUS,     TestFunctiePlus},
  {_LABEL_OPSCHRIFT_EQ,       TestFunctieEq},
  {_LABEL_OPSCHRIFT_0,        TestFunctie0},
  {_LABEL_OPSCHRIFT_100_PLUS, TestFunctie100Plus},
  {_LABEL_OPSCHRIFT_200_PLUS, TestFunctie200Plus},
  {_LABEL_OPSCHRIFT_1,        TestFunctie1},
  {_LABEL_OPSCHRIFT_2,        TestFunctie2},
  {_LABEL_OPSCHRIFT_3,        TestFunctie3},
  {_LABEL_OPSCHRIFT_4,        TestFunctie4},
  {_LABEL_OPSCHRIFT_5,        TestFunctie5},
  {_LABEL_OPSCHRIFT_6,        TestFunctie6},
  {_LABEL_OPSCHRIFT_7,        TestFunctie7},
  {_LABEL_OPSCHRIFT_8,        TestFunctie8},
  {_LABEL_OPSCHRIFT_9,        TestFunctie9}
};
#endif

void setup() {
  // FASE 1: aanmelden.
  AantalAanmeldingenOpNulZettenOpGedeeldeBus();
  ScreenAanmeldenHardwareResourcesOpGedeeldeBus();
  InputAanmeldenHardwareResourcesOpGedeeldeBus();

  // FASE 2 en 3.
  bool magInpluggen = AanmeldingenInpluggenOpGedeeldeBus();
  if (!magInpluggen) {
    PrintToScreen("GedeeldeBus", "RESOURCECONFLICT", INPUT_TEST_WEERGAVE_MS);
    return;
  }

  PrintToScreen("GedeeldeBus", "registratie OK", INPUT_TEST_WEERGAVE_MS);

#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
  pixelScreen.init(ACTIEF_PIXEL_SCREEN_BREEDTE, ACTIEF_PIXEL_SCREEN_HOOGTE);
  pixelScreen.setRotation(PIXEL_SCREEN_ROTATION);
  PixelScreen = &pixelScreen;
#endif
  ScreensConfigureren();

  InputConfigureren();

#ifdef INPUT_MAPPING_EXTRA_CONTROLES_INSCHAKELEN
  // Optioneel, enkel voor testdoeleinden: meldt via Serial welke opschriften van de gecompileerde HX1838_TOETSENINDELING ontbreken in mappingTestMenu[].
  // Weglaten in productiecode. Enkel actief wanneer INPUT_MAPPING_EXTRA_CONTROLES_INSCHAKELEN in UserConfig.h staat.
  ControleerMappingVolledigheid(mappingTestMenu);
#endif
}

void loop() {
  PrintToScreen("Input-test HX1838", "druk een toets");
  UitVoerenFunctieVolgensMappingMetToetsAanslag(true, mappingTestMenu);
}
