// InputkanalenPCF8574UserDefinedDirect.ino
// Testtoepassing voor de Input-laag (InputTypes.h/Input.h/Input.cpp), specifiek voor
// KEYPAD_TYPE_USER_DEFINED_DIRECT. Enkel bij INPUT_TYPE_PCF8574 beschikbaar.

// INPUT TESTEN:
// Dit voorbeeld test uitsluitend KEYPAD_TYPE_USER_DEFINED_DIRECT. Zet in UserConfig.h:
// #define INPUT_KANAAL_CONFIG INPUT_TYPE_PCF8574
// #define KEYPAD_TYPE KEYPAD_TYPE_USER_DEFINED_DIRECT
//
// De KEYPAD_GENERIEK_...-instellingen moeten in UserConfig.h staan. Bij
// KEYPAD_TYPE_USER_DEFINED_DIRECT staat daar een uitgecommentarieerd voorbeeld: haal in
// UserConfig.h de // voor de benodigde #define-regels weg en pas de waarden aan je keypad aan.
// Het onderstaande oude configuratiefragment is uitsluitend een referentie en wordt vanuit
// deze .ino niet meegecompileerd.

/*
#if defined(INPUT_KANAAL_CONFIG) && ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_PCF8574)
  #if defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_USER_DEFINED_DIRECT
  // Experimenteel: eigen directe PCF8574-configuratie. Zie docs/Systeem/INPUT.md.
  // Let op: dit zijn PCF8574-pinnen P0 t.e.m. P7, geen Arduino-pinnummers.
  #define KEYPAD_GENERIEK_AANTAL_PINNEN 4
  #define KEYPAD_GENERIEK_PINNEN {PCF8574_PIN_P0, PCF8574_PIN_P1, PCF8574_PIN_P2, PCF8574_PIN_P3}
  #define KEYPAD_GENERIEK_KEY_LAYOUT { {"1", "Toets 1"}, {"2", "Toets 2"}, {"3", "Toets 3"}, {"4", "Toets 4"} }
  #define KEYPAD_GENERIEK_OUTPUT_LEVEL_WHEN_KEY_PRESSED KEYPAD_GENERIEK_OUTPUT_LEVEL_WHEN_KEY_PRESSED_LOW
  #elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_USER_DEFINED_MATRIX
*/

#include <GroeiAcademie.h>
#include <Configuratie/Examples.h>
#include <Configuratie/ExamplesConfig.h>

#ifndef INPUT_TEST_WEERGAVE_MS
  #define INPUT_TEST_WEERGAVE_MS 5000UL
#endif

void TestFunctie1() { Screen->Print("Toets ingedrukt:", "1", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie2() { Screen->Print("Toets ingedrukt:", "2", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie3() { Screen->Print("Toets ingedrukt:", "3", INPUT_TEST_WEERGAVE_MS); }
void TestFunctie4() { Screen->Print("Toets ingedrukt:", "4", INPUT_TEST_WEERGAVE_MS); }

const MappingTussenToetsaanslagEnUitTeVoerenFunctie mappingTestMenu[] = {
  {_LABEL_OPSCHRIFT_1, TestFunctie1},
  {_LABEL_OPSCHRIFT_2, TestFunctie2},
  {_LABEL_OPSCHRIFT_3, TestFunctie3},
  {_LABEL_OPSCHRIFT_4, TestFunctie4}
};

void setup() {

  Screen = GedeeldeBusNewComponent<struct Screen>();
  if (Screen == nullptr) exit(0);

  Input = GedeeldeBusNewComponent<struct Input>();
  if (Input == nullptr) exit(0);
  Input->InputConfigureren();

#ifdef INPUT_MAPPING_EXTRA_CONTROLES_INSCHAKELEN
  Input->ControleerMappingVolledigheid(mappingTestMenu);
#endif
}

void loop() {
  Screen->Print("Input-test PCF8574", "druk een toets");
  Input->UitVoerenFunctieVolgensMappingMetToetsAanslag(true, mappingTestMenu);
}
