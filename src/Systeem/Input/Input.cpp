#include "Input.h"
#include "../Screen/Screen.h"   // PrintToScreen()
#include "../GedeeldeBus/GedeeldeBus.h"
#if defined(LANGUAGE_NL)
  #if defined(__has_include)
    #if __has_include("../../Language/UserLibrary_NL.h")
      #include "../../Language/UserLibrary_NL.h"
    #endif
  #endif
  #include "../../Language/Library_NL.h"
#elif defined(LANGUAGE_DE)
  #if defined(__has_include)
    #if __has_include("../../Language/UserLibrary_DE.h")
      #include "../../Language/UserLibrary_DE.h"
    #endif
  #endif
  #include "../../Language/Library_DE.h"
#elif defined(LANGUAGE_EN)
  #if defined(__has_include)
    #if __has_include("../../Language/UserLibrary_EN.h")
      #include "../../Language/UserLibrary_EN.h"
    #endif
  #endif
  #include "../../Language/Library_EN.h"
#elif defined(LANGUAGE_FR)
  #if defined(__has_include)
    #if __has_include("../../Language/UserLibrary_FR.h")
      #include "../../Language/UserLibrary_FR.h"
    #endif
  #endif
  #include "../../Language/Library_FR.h"
#endif

#include <string.h>

#if (INPUT_KANAAL_CONFIG & (INPUT_TYPE_DIGITAL | INPUT_TYPE_PCF8574))

  #if KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_DIRECT_1x4
    const MappingTussenOpschriftEnWeergavetekst KEY_LAYOUT[] = {
      {_LABEL_OPSCHRIFT_S1, LABEL_TOETS_S1}, {_LABEL_OPSCHRIFT_S2, LABEL_TOETS_S2}, {_LABEL_OPSCHRIFT_S3, LABEL_TOETS_S3}, {_LABEL_OPSCHRIFT_S4, LABEL_TOETS_S4}
    };

  #elif KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_MATRIX_2x2
    const MappingTussenOpschriftEnWeergavetekst KEY_LAYOUT[] = {
      {_LABEL_OPSCHRIFT_S1, LABEL_TOETS_S1}, {_LABEL_OPSCHRIFT_S2, LABEL_TOETS_S2}, {_LABEL_OPSCHRIFT_S3, LABEL_TOETS_S3}, {_LABEL_OPSCHRIFT_S4, LABEL_TOETS_S4}
    };

  #elif KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4
    const MappingTussenOpschriftEnWeergavetekst KEY_LAYOUT[] = {
      {_LABEL_OPSCHRIFT_1, LABEL_TOETS_1}, {_LABEL_OPSCHRIFT_2, LABEL_TOETS_2}, {_LABEL_OPSCHRIFT_3, LABEL_TOETS_3}, {_LABEL_OPSCHRIFT_4, LABEL_TOETS_4}
    };

  #elif KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_DIRECT_4x1
    const MappingTussenOpschriftEnWeergavetekst KEY_LAYOUT[] = {
      {_LABEL_OPSCHRIFT_1, LABEL_TOETS_1}, {_LABEL_OPSCHRIFT_2, LABEL_TOETS_2}, {_LABEL_OPSCHRIFT_3, LABEL_TOETS_3}, {_LABEL_OPSCHRIFT_4, LABEL_TOETS_4}
    };

  #elif KEYPAD_TYPE == KEYPAD_TYPE_TOUCH_TTP224_DIRECT_1x4
    const MappingTussenOpschriftEnWeergavetekst KEY_LAYOUT[] = {
      {_LABEL_OPSCHRIFT_1, LABEL_TOETS_1}, {_LABEL_OPSCHRIFT_2, LABEL_TOETS_2}, {_LABEL_OPSCHRIFT_3, LABEL_TOETS_3}, {_LABEL_OPSCHRIFT_4, LABEL_TOETS_4}
    };

  #elif (INPUT_KANAAL_CONFIG & INPUT_TYPE_PCF8574)
    #if KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_DIRECT_2x4
      const MappingTussenOpschriftEnWeergavetekst KEY_LAYOUT[] = {
        {_LABEL_OPSCHRIFT_S1, LABEL_TOETS_S1}, {_LABEL_OPSCHRIFT_S2, LABEL_TOETS_S2}, {_LABEL_OPSCHRIFT_S3, LABEL_TOETS_S3}, {_LABEL_OPSCHRIFT_S4, LABEL_TOETS_S4},
        {_LABEL_OPSCHRIFT_S5, LABEL_TOETS_S5}, {_LABEL_OPSCHRIFT_S6, LABEL_TOETS_S6}, {_LABEL_OPSCHRIFT_S7, LABEL_TOETS_S7}, {_LABEL_OPSCHRIFT_S8, LABEL_TOETS_S8}
      };

    #elif KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_MATRIX_4x4
      const MappingTussenOpschriftEnWeergavetekst KEY_LAYOUT[] = {
        {_LABEL_OPSCHRIFT_S1, LABEL_TOETS_S1}, {_LABEL_OPSCHRIFT_S2, LABEL_TOETS_S2}, {_LABEL_OPSCHRIFT_S3, LABEL_TOETS_S3}, {_LABEL_OPSCHRIFT_S4, LABEL_TOETS_S4},
        {_LABEL_OPSCHRIFT_S5, LABEL_TOETS_S5}, {_LABEL_OPSCHRIFT_S6, LABEL_TOETS_S6}, {_LABEL_OPSCHRIFT_S7, LABEL_TOETS_S7}, {_LABEL_OPSCHRIFT_S8, LABEL_TOETS_S8},
        {_LABEL_OPSCHRIFT_S9, LABEL_TOETS_S9}, {_LABEL_OPSCHRIFT_S10, LABEL_TOETS_S10}, {_LABEL_OPSCHRIFT_S11, LABEL_TOETS_S11}, {_LABEL_OPSCHRIFT_S12, LABEL_TOETS_S12},
        {_LABEL_OPSCHRIFT_S13, LABEL_TOETS_S13}, {_LABEL_OPSCHRIFT_S14, LABEL_TOETS_S14}, {_LABEL_OPSCHRIFT_S15, LABEL_TOETS_S15}, {_LABEL_OPSCHRIFT_S16, LABEL_TOETS_S16}
      };

    #elif KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_MATRIX_1x4
      // AANNAME, welk fysiek board hierbij hoort is nog niet bevestigd.
      const MappingTussenOpschriftEnWeergavetekst KEY_LAYOUT[] = {
        {_LABEL_OPSCHRIFT_1, LABEL_TOETS_1}, {_LABEL_OPSCHRIFT_2, LABEL_TOETS_2}, {_LABEL_OPSCHRIFT_3, LABEL_TOETS_3}, {_LABEL_OPSCHRIFT_4, LABEL_TOETS_4}
      };

    #elif KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_MATRIX_2x4
      const MappingTussenOpschriftEnWeergavetekst KEY_LAYOUT[] = {
        {_LABEL_OPSCHRIFT_1, LABEL_TOETS_1}, {_LABEL_OPSCHRIFT_2, LABEL_TOETS_2}, {_LABEL_OPSCHRIFT_3, LABEL_TOETS_3}, {_LABEL_OPSCHRIFT_4, LABEL_TOETS_4},
        {_LABEL_OPSCHRIFT_5, LABEL_TOETS_5}, {_LABEL_OPSCHRIFT_6, LABEL_TOETS_6}, {_LABEL_OPSCHRIFT_7, LABEL_TOETS_7}, {_LABEL_OPSCHRIFT_8, LABEL_TOETS_8}
      };

    #elif KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_MATRIX_4x3
      const MappingTussenOpschriftEnWeergavetekst KEY_LAYOUT[] = {
        {_LABEL_OPSCHRIFT_1, LABEL_TOETS_1}, {_LABEL_OPSCHRIFT_2, LABEL_TOETS_2}, {_LABEL_OPSCHRIFT_3, LABEL_TOETS_3},
        {_LABEL_OPSCHRIFT_4, LABEL_TOETS_4}, {_LABEL_OPSCHRIFT_5, LABEL_TOETS_5}, {_LABEL_OPSCHRIFT_6, LABEL_TOETS_6},
        {_LABEL_OPSCHRIFT_7, LABEL_TOETS_7}, {_LABEL_OPSCHRIFT_8, LABEL_TOETS_8}, {_LABEL_OPSCHRIFT_9, LABEL_TOETS_9},
        {_LABEL_OPSCHRIFT_STER, LABEL_TOETS_STER}, {_LABEL_OPSCHRIFT_0, LABEL_TOETS_0}, {_LABEL_OPSCHRIFT_HEKJE, LABEL_TOETS_HEKJE}
      };

    #elif KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_MATRIX_4x4
      const MappingTussenOpschriftEnWeergavetekst KEY_LAYOUT[] = {
        {_LABEL_OPSCHRIFT_1, LABEL_TOETS_1}, {_LABEL_OPSCHRIFT_2, LABEL_TOETS_2}, {_LABEL_OPSCHRIFT_3, LABEL_TOETS_3}, {_LABEL_OPSCHRIFT_A, LABEL_TOETS_A},
        {_LABEL_OPSCHRIFT_4, LABEL_TOETS_4}, {_LABEL_OPSCHRIFT_5, LABEL_TOETS_5}, {_LABEL_OPSCHRIFT_6, LABEL_TOETS_6}, {_LABEL_OPSCHRIFT_B, LABEL_TOETS_B},
        {_LABEL_OPSCHRIFT_7, LABEL_TOETS_7}, {_LABEL_OPSCHRIFT_8, LABEL_TOETS_8}, {_LABEL_OPSCHRIFT_9, LABEL_TOETS_9}, {_LABEL_OPSCHRIFT_C, LABEL_TOETS_C},
        {_LABEL_OPSCHRIFT_STER, LABEL_TOETS_STER}, {_LABEL_OPSCHRIFT_0, LABEL_TOETS_0}, {_LABEL_OPSCHRIFT_HEKJE, LABEL_TOETS_HEKJE}, {_LABEL_OPSCHRIFT_D, LABEL_TOETS_D}
      };

    #elif KEYPAD_TYPE == KEYPAD_TYPE_TOUCH_TTP229_MATRIX_4x4
      const MappingTussenOpschriftEnWeergavetekst KEY_LAYOUT[] = {
        {_LABEL_OPSCHRIFT_1, LABEL_TOETS_1}, {_LABEL_OPSCHRIFT_2, LABEL_TOETS_2}, {_LABEL_OPSCHRIFT_3, LABEL_TOETS_3}, {_LABEL_OPSCHRIFT_4, LABEL_TOETS_4},
        {_LABEL_OPSCHRIFT_5, LABEL_TOETS_5}, {_LABEL_OPSCHRIFT_6, LABEL_TOETS_6}, {_LABEL_OPSCHRIFT_7, LABEL_TOETS_7}, {_LABEL_OPSCHRIFT_8, LABEL_TOETS_8}
      };

    #elif KEYPAD_TYPE == KEYPAD_TYPE_USER_DEFINED_DIRECT || KEYPAD_TYPE == KEYPAD_TYPE_USER_DEFINED_MATRIX
      const MappingTussenOpschriftEnWeergavetekst KEY_LAYOUT[] = KEYPAD_GENERIEK_KEY_LAYOUT;
    #else
      #error Ongeldige KEYPAD_TYPE voor INPUT_TYPE_PCF8574.
    #endif
  #else
    #error Ongeldige KEYPAD_TYPE voor INPUT_TYPE_DIGITAL.
  #endif
#endif

#if (INPUT_KANAAL_CONFIG & INPUT_TYPE_HX1838)
  #if HX1838_TOETSENINDELING == HX1838_TOETSENINDELING_3x4
    const MappingTussenOpschriftEnWeergavetekst IR_KEY_LAYOUT[] = {
      {_LABEL_OPSCHRIFT_1, LABEL_TOETS_1}, {_LABEL_OPSCHRIFT_2, LABEL_TOETS_2}, {_LABEL_OPSCHRIFT_3, LABEL_TOETS_3},
      {_LABEL_OPSCHRIFT_4, LABEL_TOETS_4}, {_LABEL_OPSCHRIFT_5, LABEL_TOETS_5}, {_LABEL_OPSCHRIFT_6, LABEL_TOETS_6},
      {_LABEL_OPSCHRIFT_7, LABEL_TOETS_7}, {_LABEL_OPSCHRIFT_8, LABEL_TOETS_8}, {_LABEL_OPSCHRIFT_9, LABEL_TOETS_9},
      {_LABEL_OPSCHRIFT_STER, LABEL_TOETS_STER}, {_LABEL_OPSCHRIFT_0, LABEL_TOETS_0}, {_LABEL_OPSCHRIFT_HEKJE, LABEL_TOETS_HEKJE}
    };
  #elif HX1838_TOETSENINDELING == HX1838_TOETSENINDELING_REMOTE_17_TOETSEN
    const MappingTussenOpschriftEnWeergavetekst IR_KEY_LAYOUT[] = {
      {_LABEL_OPSCHRIFT_1, LABEL_TOETS_1}, {_LABEL_OPSCHRIFT_2, LABEL_TOETS_2}, {_LABEL_OPSCHRIFT_3, LABEL_TOETS_3},
      {_LABEL_OPSCHRIFT_4, LABEL_TOETS_4}, {_LABEL_OPSCHRIFT_5, LABEL_TOETS_5}, {_LABEL_OPSCHRIFT_6, LABEL_TOETS_6},
      {_LABEL_OPSCHRIFT_7, LABEL_TOETS_7}, {_LABEL_OPSCHRIFT_8, LABEL_TOETS_8}, {_LABEL_OPSCHRIFT_9, LABEL_TOETS_9},
      {_LABEL_OPSCHRIFT_STER, LABEL_TOETS_STER}, {_LABEL_OPSCHRIFT_0, LABEL_TOETS_0}, {_LABEL_OPSCHRIFT_HEKJE, LABEL_TOETS_HEKJE},
      {_LABEL_OPSCHRIFT_UP, LABEL_TOETS_UP}, {_LABEL_OPSCHRIFT_DOWN, LABEL_TOETS_DOWN},
      {_LABEL_OPSCHRIFT_LEFT, LABEL_TOETS_LEFT}, {_LABEL_OPSCHRIFT_RIGHT, LABEL_TOETS_RIGHT}, {_LABEL_OPSCHRIFT_OK, LABEL_TOETS_OK}
    };
  #elif HX1838_TOETSENINDELING == HX1838_TOETSENINDELING_REMOTE_21_TOETSEN_MP3
    const MappingTussenOpschriftEnWeergavetekst IR_KEY_LAYOUT[] = {
      {_LABEL_OPSCHRIFT_CH_MINUS, LABEL_TOETS_CH_MINUS}, {_LABEL_OPSCHRIFT_CH, LABEL_TOETS_CH}, {_LABEL_OPSCHRIFT_CH_PLUS, LABEL_TOETS_CH_PLUS},
      {_LABEL_OPSCHRIFT_PREV, LABEL_TOETS_PREV}, {_LABEL_OPSCHRIFT_NEXT, LABEL_TOETS_NEXT}, {_LABEL_OPSCHRIFT_PLAY, LABEL_TOETS_PLAY},
      {_LABEL_OPSCHRIFT_MINUS, LABEL_TOETS_MINUS}, {_LABEL_OPSCHRIFT_PLUS, LABEL_TOETS_PLUS}, {_LABEL_OPSCHRIFT_EQ, LABEL_TOETS_EQ},
      {_LABEL_OPSCHRIFT_0, LABEL_TOETS_0}, {_LABEL_OPSCHRIFT_100_PLUS, LABEL_TOETS_100_PLUS}, {_LABEL_OPSCHRIFT_200_PLUS, LABEL_TOETS_200_PLUS},
      {_LABEL_OPSCHRIFT_1, LABEL_TOETS_1}, {_LABEL_OPSCHRIFT_2, LABEL_TOETS_2}, {_LABEL_OPSCHRIFT_3, LABEL_TOETS_3},
      {_LABEL_OPSCHRIFT_4, LABEL_TOETS_4}, {_LABEL_OPSCHRIFT_5, LABEL_TOETS_5}, {_LABEL_OPSCHRIFT_6, LABEL_TOETS_6},
      {_LABEL_OPSCHRIFT_7, LABEL_TOETS_7}, {_LABEL_OPSCHRIFT_8, LABEL_TOETS_8}, {_LABEL_OPSCHRIFT_9, LABEL_TOETS_9}
    };
  #else
    #error Ongeldige HX1838_TOETSENINDELING.
  #endif
#endif

#if (INPUT_KANAAL_CONFIG & INPUT_TYPE_PCF8574)
  #include <Wire.h>
  #include <PCF8574.h>
#endif

#if (INPUT_KANAAL_CONFIG & INPUT_TYPE_HX1838)
  #include <IRremote.h>
  #include <EEPROM.h>
#endif

extern const MappingTussenToetsaanslagEnUitTeVoerenFunctie mappingTussenToetsaanslagEnUitTeVoerenFunctie[] __attribute__((weak)) = {
  {nullptr, nullptr}
};
extern const byte aantalToetsFuncties __attribute__((weak)) = 0;

// ============================================================================
// TOETS -> FUNCTIE OPZOEKEN (mappingTussenToetsaanslagEnUitTeVoerenFunctie wordt door de gebruiker gedefinieerd, in de sketch of UserConfig.h)
// ============================================================================
// OpzoekenUitTeVoerenFunctieViaOpschriftToetsAanslag() retourneert de volledige koppeling met opschrift en functiepointer.
const MappingTussenToetsaanslagEnUitTeVoerenFunctie* OpzoekenUitTeVoerenFunctieViaOpschriftToetsAanslag(const char* opschriftToetsAanslag) {
  if (opschriftToetsAanslag == nullptr) return nullptr;
  for (byte i = 0; i < aantalToetsFuncties; i++) {
    if (strcmp(mappingTussenToetsaanslagEnUitTeVoerenFunctie[i].opschriftToetsAanslag, opschriftToetsAanslag) == 0) {
      return &mappingTussenToetsaanslagEnUitTeVoerenFunctie[i];
    }
  }
  return nullptr;
}

#ifdef INPUT_MAPPING_EXTRA_CONTROLES_INSCHAKELEN
// ============================================================================
// MAPPING-VOLLEDIGHEIDSCONTROLE (enkel bedoeld om tijdens het testen op te roepen)
// ============================================================================
void ControleerMappingVolledigheidIntern(const char* const opschriftToetsAanslag[], byte aantalEntries) {
  (void)opschriftToetsAanslag;
  (void)aantalEntries;
  byte aantalOntbrekend = 0;
#if (INPUT_KANAAL_CONFIG & (INPUT_TYPE_DIGITAL | INPUT_TYPE_PCF8574))
  for (byte i = 0; i < (sizeof(KEY_LAYOUT) / sizeof(KEY_LAYOUT[0])); i++) {
    bool gevonden = false;
    for (byte j = 0; j < aantalEntries; j++) {
      if (strcmp(KEY_LAYOUT[i].opschrift, opschriftToetsAanslag[j]) == 0) { gevonden = true; break; }
    }
    if (!gevonden) {
#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
      PrintToScreen(ScreenData::TYPE_WARNING, F("WAARSCHUWING"), String(F("Ontbreekt: ")) + KEY_LAYOUT[i].opschrift);
#else
      PrintToScreen(F("WAARSCHUWING"), String(F("Ontbreekt: ")) + KEY_LAYOUT[i].opschrift);
#endif
      aantalOntbrekend++;
    }
  }
#endif
#if (INPUT_KANAAL_CONFIG & INPUT_TYPE_HX1838)
  for (byte i = 0; i < (sizeof(IR_KEY_LAYOUT) / sizeof(IR_KEY_LAYOUT[0])); i++) {
    bool gevonden = false;
    for (byte j = 0; j < aantalEntries; j++) {
      if (strcmp(IR_KEY_LAYOUT[i].opschrift, opschriftToetsAanslag[j]) == 0) { gevonden = true; break; }
    }
    if (!gevonden) {
#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
      PrintToScreen(ScreenData::TYPE_WARNING, F("WAARSCHUWING"), String(F("Ontbreekt: ")) + IR_KEY_LAYOUT[i].opschrift);
#else
      PrintToScreen(F("WAARSCHUWING"), String(F("Ontbreekt: ")) + IR_KEY_LAYOUT[i].opschrift);
#endif
      aantalOntbrekend++;
    }
  }
#endif
  if (aantalOntbrekend == 0) {
#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
    PrintToScreen(ScreenData::TYPE_INFO, F("Mapping-controle"), F("alle opschriften OK"));
#else
    PrintToScreen(F("Mapping-controle"), F("alle opschriften OK"));
#endif
  }
}
#endif // INPUT_MAPPING_EXTRA_CONTROLES_INSCHAKELEN

void UitVoerenFunctieVolgensMappingMetToetsAanslag(bool wachten) {
  InputResultaat invoer = OpvragenHuidigeToetsAanslag(wachten);
  if (invoer.inputKanaal == InputKanaal::NONE || invoer.opschriftToetsAanslag == nullptr) return;
  const MappingTussenToetsaanslagEnUitTeVoerenFunctie* mapping = OpzoekenUitTeVoerenFunctieViaOpschriftToetsAanslag(invoer.opschriftToetsAanslag);
  if (mapping == nullptr) return;

#ifdef INPUT_KANAAL_OPVRAGEN_HUIDIGE_TOETSAANSLAG_UITGEBREID
  if (invoer.gebeurtenis == InputGebeurtenis::LOSGELATEN) {
    if (mapping->functieBijLoslaten != nullptr) mapping->functieBijLoslaten();
    return;
  }

  if (invoer.gebeurtenis == InputGebeurtenis::LANG_INDRUKKEN) {
    if (mapping->functieBijLangIndrukken != nullptr) mapping->functieBijLangIndrukken();
    return;
  }
#endif
  if (mapping->functie != nullptr) mapping->functie();
}

// ============================================================================
// PCF8574 PINMAPPING EN LAAGSTE-NIVEAUFUNCTIES
// ============================================================================
#if (INPUT_KANAAL_CONFIG & INPUT_TYPE_PCF8574)
  #if KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_DIRECT_1x4
    #define KEYPAD_IS_DIRECT
    static const byte AANTAL_DIRECT_PINNEN = 4;
    static const byte TOETS_PINNEN[AANTAL_DIRECT_PINNEN] = {KEYPAD_PIN_K1, KEYPAD_PIN_K2, KEYPAD_PIN_K3, KEYPAD_PIN_K4};
  #elif KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_DIRECT_2x4
    #define KEYPAD_IS_DIRECT
    static const byte AANTAL_DIRECT_PINNEN = 8;
    static const byte TOETS_PINNEN[AANTAL_DIRECT_PINNEN] = {KEYPAD_PIN_K1, KEYPAD_PIN_K2, KEYPAD_PIN_K3, KEYPAD_PIN_K4, KEYPAD_PIN_K5, KEYPAD_PIN_K6, KEYPAD_PIN_K7, KEYPAD_PIN_K8};
  #elif KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_MATRIX_2x2
    // OT3688: vier knoppen, hergebruikt K1..K4 op de PCF8574
    #define KEYPAD_IS_MATRIX
    static const byte AANTAL_RIJEN = 2;
    static const byte AANTAL_KOLOMMEN = 2;
    static const byte RIJ_PINNEN[AANTAL_RIJEN] = {KEYPAD_PIN_L1, KEYPAD_PIN_L2};
    static const byte KOLOM_PINNEN[AANTAL_KOLOMMEN] = {KEYPAD_PIN_R1, KEYPAD_PIN_R2};
  #elif KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_MATRIX_4x4
    #define KEYPAD_IS_MATRIX
    static const byte AANTAL_RIJEN = 4;
    static const byte AANTAL_KOLOMMEN = 4;
    static const byte RIJ_PINNEN[AANTAL_RIJEN] = {KEYPAD_PIN_R1, KEYPAD_PIN_R2, KEYPAD_PIN_R3, KEYPAD_PIN_R4};
    static const byte KOLOM_PINNEN[AANTAL_KOLOMMEN] = {KEYPAD_PIN_C1, KEYPAD_PIN_C2, KEYPAD_PIN_C3, KEYPAD_PIN_C4};
  #elif KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4 || KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_DIRECT_4x1
    #define KEYPAD_IS_DIRECT
    static const byte AANTAL_DIRECT_PINNEN = 4;
    static const byte TOETS_PINNEN[AANTAL_DIRECT_PINNEN] = {KEYPAD_PIN_1, KEYPAD_PIN_2, KEYPAD_PIN_3, KEYPAD_PIN_4};
  #elif KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_MATRIX_1x4
    #define KEYPAD_IS_MATRIX
    static const byte AANTAL_RIJEN = 1;
    static const byte AANTAL_KOLOMMEN = 4;
    static const byte RIJ_PINNEN[AANTAL_RIJEN] = {KEYPAD_PIN_R1};
    static const byte KOLOM_PINNEN[AANTAL_KOLOMMEN] = {KEYPAD_PIN_C1, KEYPAD_PIN_C2, KEYPAD_PIN_C3, KEYPAD_PIN_C4};
  #elif KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_MATRIX_2x4
    #define KEYPAD_IS_MATRIX
    static const byte AANTAL_RIJEN = 2;
    static const byte AANTAL_KOLOMMEN = 4;
    static const byte RIJ_PINNEN[AANTAL_RIJEN] = {KEYPAD_PIN_R1, KEYPAD_PIN_R2};
    static const byte KOLOM_PINNEN[AANTAL_KOLOMMEN] = {KEYPAD_PIN_C1, KEYPAD_PIN_C2, KEYPAD_PIN_C3, KEYPAD_PIN_C4};
  #elif KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_MATRIX_4x3
    #define KEYPAD_IS_MATRIX
    static const byte AANTAL_RIJEN = 4;
    static const byte AANTAL_KOLOMMEN = 3;
    static const byte RIJ_PINNEN[AANTAL_RIJEN] = {KEYPAD_PIN_R1, KEYPAD_PIN_R2, KEYPAD_PIN_R3, KEYPAD_PIN_R4};
    static const byte KOLOM_PINNEN[AANTAL_KOLOMMEN] = {KEYPAD_PIN_C1, KEYPAD_PIN_C2, KEYPAD_PIN_C3};
  #elif KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_MATRIX_4x4
    #define KEYPAD_IS_MATRIX
    static const byte AANTAL_RIJEN = 4;
    static const byte AANTAL_KOLOMMEN = 4;
    static const byte RIJ_PINNEN[AANTAL_RIJEN] = {KEYPAD_PIN_R1, KEYPAD_PIN_R2, KEYPAD_PIN_R3, KEYPAD_PIN_R4};
    static const byte KOLOM_PINNEN[AANTAL_KOLOMMEN] = {KEYPAD_PIN_C1, KEYPAD_PIN_C2, KEYPAD_PIN_C3, KEYPAD_PIN_C4};
  #elif KEYPAD_TYPE == KEYPAD_TYPE_TOUCH_TTP224_DIRECT_1x4
    #define KEYPAD_IS_DIRECT
    static const byte AANTAL_DIRECT_PINNEN = 4;
    static const byte TOETS_PINNEN[AANTAL_DIRECT_PINNEN] = {KEYPAD_PIN_OUT1, KEYPAD_PIN_OUT2, KEYPAD_PIN_OUT3, KEYPAD_PIN_OUT4};
  #elif KEYPAD_TYPE == KEYPAD_TYPE_TOUCH_TTP229_MATRIX_4x4
    #define KEYPAD_IS_DIRECT
    static const byte AANTAL_DIRECT_PINNEN = 8;
    static const byte TOETS_PINNEN[AANTAL_DIRECT_PINNEN] = {KEYPAD_PIN_OUT1, KEYPAD_PIN_OUT2, KEYPAD_PIN_OUT3, KEYPAD_PIN_OUT4, KEYPAD_PIN_OUT5, KEYPAD_PIN_OUT6, KEYPAD_PIN_OUT7, KEYPAD_PIN_OUT8};
  #elif KEYPAD_TYPE == KEYPAD_TYPE_USER_DEFINED_DIRECT
    #define KEYPAD_IS_DIRECT
    static const byte AANTAL_DIRECT_PINNEN = KEYPAD_GENERIEK_AANTAL_PINNEN;
    static const byte TOETS_PINNEN[AANTAL_DIRECT_PINNEN] = KEYPAD_GENERIEK_PINNEN;
  #elif KEYPAD_TYPE == KEYPAD_TYPE_USER_DEFINED_MATRIX
    #define KEYPAD_IS_MATRIX
    static const byte AANTAL_RIJEN = KEYPAD_GENERIEK_AANTAL_RIJEN;
    static const byte AANTAL_KOLOMMEN = KEYPAD_GENERIEK_AANTAL_KOLOMMEN;
    static const byte RIJ_PINNEN[AANTAL_RIJEN] = KEYPAD_GENERIEK_RIJ_PINNEN;
    static const byte KOLOM_PINNEN[AANTAL_KOLOMMEN] = KEYPAD_GENERIEK_KOLOM_PINNEN;
  #endif

  #if defined(KEYPAD_IS_DIRECT)
    static_assert(AANTAL_DIRECT_PINNEN <= 8, "Het directe keypad gebruikt meer dan 8 PCF8574-pinnen.");
  #endif
  #if defined(KEYPAD_IS_MATRIX)
    static_assert((AANTAL_RIJEN + AANTAL_KOLOMMEN) <= 8, "De matrix gebruikt meer dan 8 PCF8574-pinnen.");
  #endif

  static PCF8574 pcf8574(I2C_ADDRESS_PCF8574);

  // Blijvende status: eenmaal onbereikbaar, blijft de melding staan en wordt geen
  // verdere I2C-communicatie meer geprobeerd, i.p.v. stil "geen toets" te blijven melden.
  // Zelfde patroon als characterScreenStatus.foutmeldingWeergegeven in Screen.cpp.
  static bool pcf8574Bereikbaar = true;
  static bool pcf8574FoutmeldingWeergegeven = false;

  static bool PCF8574poortPatroonMatrixUitlezenInstellen(byte waarde) {
    pcf8574.write8(waarde);
    return pcf8574.lastError() == PCF8574_OK;
  }

  static bool PCF8574poortPatroonUitlezen(byte& waarde) {
    waarde = pcf8574.read8();
    return pcf8574.lastError() == PCF8574_OK;
  }

  #if defined(KEYPAD_IS_DIRECT)
    static int PCF8574uitLezenPoortenP0totP7DirectAansluiting() {
      if (!pcf8574Bereikbaar) return 0;
      byte status = 0xFF;
      if (!PCF8574poortPatroonUitlezen(status)) return 0;
      for (byte i = 0; i < AANTAL_DIRECT_PINNEN; i++) {
        #if KEYPAD_TYPE == KEYPAD_TYPE_TOUCH_TTP224_DIRECT_1x4
          if (bitRead(status, TOETS_PINNEN[i])) return i + 1;
        #elif KEYPAD_TYPE == KEYPAD_TYPE_TOUCH_TTP229_MATRIX_4x4
          #if TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED == TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED_HIGH
            if (bitRead(status, TOETS_PINNEN[i])) return i + 1;
          #else
            if (!bitRead(status, TOETS_PINNEN[i])) return i + 1;
          #endif
        #elif KEYPAD_TYPE == KEYPAD_TYPE_USER_DEFINED_DIRECT
          if (KEYPAD_GENERIEK_OUTPUT_LEVEL_WHEN_KEY_PRESSED == KEYPAD_GENERIEK_OUTPUT_LEVEL_WHEN_KEY_PRESSED_HIGH) {
            if (bitRead(status, TOETS_PINNEN[i])) return i + 1;
          } else {
            if (!bitRead(status, TOETS_PINNEN[i])) return i + 1;
          }
        #else
          if (!bitRead(status, TOETS_PINNEN[i])) return i + 1;
        #endif
      }
      return 0;
    }
  #endif

  #if defined(KEYPAD_IS_MATRIX)
    static int PCF8574uitLezenPoortenP0totP7MatrixAansluiting() {
      if (!pcf8574Bereikbaar) return 0;
      for (byte rij = 0; rij < AANTAL_RIJEN; rij++) {
        byte uitgang = 0xFF;
        bitClear(uitgang, RIJ_PINNEN[rij]);
        if (!PCF8574poortPatroonMatrixUitlezenInstellen(uitgang)) return 0;
        delayMicroseconds(50);
        byte ingangen = 0xFF;

        if (!PCF8574poortPatroonUitlezen(ingangen)) {
          PCF8574poortPatroonMatrixUitlezenInstellen(0xFF);
          return 0;
        }

        for (byte kolom = 0; kolom < AANTAL_KOLOMMEN; kolom++) {
          if (!bitRead(ingangen, KOLOM_PINNEN[kolom])) {
            PCF8574poortPatroonMatrixUitlezenInstellen(0xFF);
            return (rij * AANTAL_KOLOMMEN) + kolom + 1;
          }
        }
      }

      PCF8574poortPatroonMatrixUitlezenInstellen(0xFF);
      return 0;
    }
  #endif
#endif

// ============================================================================
// DIGITALE DIRECTE UITLEZING (INPUT_TYPE_DIGITAL)
// ============================================================================
#if (INPUT_KANAAL_CONFIG & INPUT_TYPE_DIGITAL)
  static int DigitaalUitLezenRuweData() {
    #if KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_MATRIX_2x2
      const byte rijPinnen[2] = {PIN_TOETS_1, PIN_TOETS_2};
      const byte kolomPinnen[2] = {PIN_TOETS_3, PIN_TOETS_4};

      for (byte rij = 0; rij < 2; rij++) {
        digitalWrite(rijPinnen[rij], LOW);
        delayMicroseconds(50);
        for (byte kolom = 0; kolom < 2; kolom++) {
          if (digitalRead(kolomPinnen[kolom]) == LOW) {
            digitalWrite(rijPinnen[rij], HIGH);
            return (rij * 2) + kolom + 1;
          }
        }
        digitalWrite(rijPinnen[rij], HIGH);
      }

      return 0;
    #elif KEYPAD_TYPE == KEYPAD_TYPE_TOUCH_TTP224_DIRECT_1x4
      if (digitalRead(PIN_TOETS_1) == HIGH) return 1;
      if (digitalRead(PIN_TOETS_2) == HIGH) return 2;
      if (digitalRead(PIN_TOETS_3) == HIGH) return 3;
      if (digitalRead(PIN_TOETS_4) == HIGH) return 4;
      return 0;
    #else
      if (digitalRead(PIN_TOETS_1) == LOW) return 1;
      if (digitalRead(PIN_TOETS_2) == LOW) return 2;
      if (digitalRead(PIN_TOETS_3) == LOW) return 3;
      if (digitalRead(PIN_TOETS_4) == LOW) return 4;
      return 0;
    #endif
  }
#endif

// ============================================================================
// DEBOUNCE + NIEUWE-TOETSAANSLAGDETECTIE VOOR FYSIEKE KEYPADS
// ============================================================================
#if (INPUT_KANAAL_CONFIG & (INPUT_TYPE_DIGITAL | INPUT_TYPE_PCF8574))
  static int vorigeRauweKeypadPositie = 0;
  static int stabieleKeypadPositie = 0;
  static unsigned long keypadWijzigingSinds = 0;

  #ifdef INPUT_KANAAL_OPVRAGEN_HUIDIGE_TOETSAANSLAG_UITGEBREID
    static int laatstePositieVoorLoslatenDetectie = 0;
    static unsigned long keypadDrukBeginTijd = 0;
    static bool langIndrukkenAlGemeldVoorHuidigeDruk = false;
  #endif

  static int KeypadUitLezenRuweData() {
    #if (INPUT_KANAAL_CONFIG & INPUT_TYPE_DIGITAL)
      return DigitaalUitLezenRuweData();
    #elif (INPUT_KANAAL_CONFIG & INPUT_TYPE_PCF8574)
      #if defined(KEYPAD_IS_MATRIX)
        return PCF8574uitLezenPoortenP0totP7MatrixAansluiting();
      #else
        return PCF8574uitLezenPoortenP0totP7DirectAansluiting();
      #endif
    #endif
  }

  static int KeypadUitLezenToetsAanslag() {
    int rauw = KeypadUitLezenRuweData();
    unsigned long nu = millis();

    if (rauw != vorigeRauweKeypadPositie) {
      vorigeRauweKeypadPositie = rauw;
      keypadWijzigingSinds = nu;
      return 0;
    }

    if ((nu - keypadWijzigingSinds) < INPUT_DEBOUNCE_MS) return 0;

    if (rauw != stabieleKeypadPositie) {
      stabieleKeypadPositie = rauw;
      #ifdef INPUT_KANAAL_OPVRAGEN_HUIDIGE_TOETSAANSLAG_UITGEBREID
        if (stabieleKeypadPositie > 0) {
          keypadDrukBeginTijd = nu;
          langIndrukkenAlGemeldVoorHuidigeDruk = false;
        }
      #endif
      if (stabieleKeypadPositie > 0) return stabieleKeypadPositie;
    }

    return 0;
  }

  #ifdef INPUT_KANAAL_OPVRAGEN_HUIDIGE_TOETSAANSLAG_UITGEBREID
  // Geeft de positie terug die net losgelaten werd (0 als er niets net is losgelaten).
  // Roep dit pas op NA KeypadUitLezenToetsAanslag()/KeypadUitLezenRuweData() in dezelfde cyclus,
  // zodat stabieleKeypadPositie al up-to-date is.
  static int KeypadUitLezenLosgelatenPositie() {
    int resultaat = 0;
    if (laatstePositieVoorLoslatenDetectie > 0 && stabieleKeypadPositie == 0) {
      resultaat = laatstePositieVoorLoslatenDetectie;
    }
    laatstePositieVoorLoslatenDetectie = stabieleKeypadPositie;
    return resultaat;
  }

  // Geeft de huidige, nog ingedrukte positie terug zodra ze langer dan drempelMs ingedrukt is,
  // en meldt dat maar één keer per druk (niet herhaald bij elke aanroep).
  static int KeypadUitLezenLangIngedruktePositie(unsigned long drempelMs) {
    if (stabieleKeypadPositie == 0) return 0;
    if (langIndrukkenAlGemeldVoorHuidigeDruk) return 0;
    if ((millis() - keypadDrukBeginTijd) < drempelMs) return 0;
    langIndrukkenAlGemeldVoorHuidigeDruk = true;
    return stabieleKeypadPositie;
  }
  #endif
#endif

// ============================================================================
// HX1838 KALIBRATIE EN UITLEZING
// ============================================================================
#if (INPUT_KANAAL_CONFIG & INPUT_TYPE_HX1838)
  static uint8_t irCodes[AANTAL_IR_TOETSEN];

  static bool HX1838mappingUitUserConfigInladen() {
    const uint8_t standaardCodes[] = {HX1838_CODE_1, HX1838_CODE_2, HX1838_CODE_3, HX1838_CODE_4, HX1838_CODE_5, HX1838_CODE_6, HX1838_CODE_7, HX1838_CODE_8, HX1838_CODE_9, HX1838_CODE_10, HX1838_CODE_11, HX1838_CODE_12, HX1838_CODE_13, HX1838_CODE_14, HX1838_CODE_15, HX1838_CODE_16, HX1838_CODE_17, HX1838_CODE_18, HX1838_CODE_19, HX1838_CODE_20, HX1838_CODE_21};
    for (byte i = 0; i < AANTAL_IR_TOETSEN; i++) if (standaardCodes[i] == 0UL) return false;
    for (byte i = 0; i < AANTAL_IR_TOETSEN; i++) irCodes[i] = standaardCodes[i];
    return true;
  }

  #define EEPROM_ADRES_MAGIC    0
  #define EEPROM_MAGIC_WAARDE   0xAC
  #define EEPROM_ADRES_VERSIE   1
  #define EEPROM_INPUT_VERSIE   2
  #define EEPROM_ADRES_CODES    2
  #define EEPROM_BENODIGDE_GROOTTE (EEPROM_ADRES_CODES + (AANTAL_IR_TOETSEN * sizeof(uint8_t)))

  static bool EEPROMopslagBeginnen() {
    #if BOARD_VERSION == BOARD_ESP32_UNO || BOARD_VERSION == BOARD_ARDI32
      return EEPROM.begin(EEPROM_BENODIGDE_GROOTTE);
    #elif BOARD_VERSION == BOARD_CYTRON_MAKER_UNO_RP2040
      EEPROM.begin(EEPROM_BENODIGDE_GROOTTE);
      return true;
    #else
      return true;
    #endif
  }

  static void EEPROMopslagBevestigen() {
    #if BOARD_VERSION == BOARD_ESP32_UNO || BOARD_VERSION == BOARD_ARDI32 || BOARD_VERSION == BOARD_CYTRON_MAKER_UNO_RP2040
      EEPROM.commit();
    #endif
  }

  static int HX1838indexUitZoekenVoorSignaal(uint8_t signaalwaarde) {
    for (byte i = 0; i < AANTAL_IR_TOETSEN; i++) if (irCodes[i] == signaalwaarde) return i;
    return -1;
  }

  static void HX1838toetsKalibreren(byte index) {
#if (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
    Serial.print(_INPUT_HX1838_DRUK_NU_OP_SERIAL);
    Serial.println(IR_KEY_LAYOUT[index].weergavetekst);
#endif
    PrintToScreen(_INPUT_HX1838_DRUK_NU_OP, IR_KEY_LAYOUT[index].weergavetekst, 0);

    while (true) {
      if (IrReceiver.decode()) {
        if (!(IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT)) {
          irCodes[index] = IrReceiver.decodedIRData.command;
          IrReceiver.resume();
          break;
        }

        IrReceiver.resume();
      }
    }
    
    delay(HX1838_KALIBRATIE_TOETS_PAUZE_MS);
  }

  static void HX1838kalibratieOpslaan() {
    if (!EEPROMopslagBeginnen()) return;
    EEPROM.write(EEPROM_ADRES_MAGIC, EEPROM_MAGIC_WAARDE);
    EEPROM.write(EEPROM_ADRES_VERSIE, EEPROM_INPUT_VERSIE);
    for (byte i = 0; i < AANTAL_IR_TOETSEN; i++) EEPROM.put(EEPROM_ADRES_CODES + (i * sizeof(uint8_t)), irCodes[i]);
    EEPROMopslagBevestigen();
#if (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
    Serial.println(_INPUT_HX1838_KALIBRATIE_OPGESLAGEN_SERIAL);
#endif
    PrintToScreen(_INPUT_HX1838_KALIBRATIE_UITVOEREN, _INPUT_HX1838_KALIBRATIE_OPGESLAGEN, 2000);
  }

  static bool HX1838kalibratieLaden() {
    if (!EEPROMopslagBeginnen()) return false;
    if (EEPROM.read(EEPROM_ADRES_MAGIC) != EEPROM_MAGIC_WAARDE) return false;
    if (EEPROM.read(EEPROM_ADRES_VERSIE) != EEPROM_INPUT_VERSIE) return false;
    for (byte i = 0; i < AANTAL_IR_TOETSEN; i++) EEPROM.get(EEPROM_ADRES_CODES + (i * sizeof(uint8_t)), irCodes[i]);
    return true;
  }

  static void HX1838kalibratieVerifieren() {
#if (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
    Serial.println(_INPUT_HX1838_KALIBRATIE_KLAAR_CONTROLE_SERIAL);
#endif
    PrintToScreen(_INPUT_HX1838_CONTROLE, _INPUT_HX1838_DRUK_OP_ELKE_TOETS_VOOR_CONTROLE, 0);

    bool geverifieerd[AANTAL_IR_TOETSEN];
    for (byte i = 0; i < AANTAL_IR_TOETSEN; i++) geverifieerd[i] = false;
    byte aantalGeverifieerd = 0;

    while (aantalGeverifieerd < AANTAL_IR_TOETSEN) {
      if (IrReceiver.decode()) {
        if (!(IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT)) {
          int index = HX1838indexUitZoekenVoorSignaal(IrReceiver.decodedIRData.command);

          if (index >= 0 && !geverifieerd[index]) {
            geverifieerd[index] = true;
            aantalGeverifieerd++;
#if (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
            Serial.print(_INPUT_HX1838_TOETS_HERKEND_SERIAL);
            Serial.println(IR_KEY_LAYOUT[index].weergavetekst);
#endif
            PrintToScreen(IR_KEY_LAYOUT[index].weergavetekst, _INPUT_HX1838_TOETS_HERKEND, 500);
          }
        }

        IrReceiver.resume();
      }
    }

    PrintToScreen(_INPUT_HX1838_CONTROLE_GESLAAGD, "", 2000);
    HX1838kalibratieOpslaan();
  }

  static void HX1838kalibratieUitvoeren() {
#if (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
    Serial.println(_INPUT_HX1838_KALIBRATIE_GESTART_SERIAL);
#endif
    for (byte i = 0; i < AANTAL_IR_TOETSEN; i++) HX1838toetsKalibreren(i);
    HX1838kalibratieVerifieren();
  }

  static int HX1838uitLezenToetsAanslag() {
    if (!IrReceiver.decode()) return 0;
    int positie = 0;

    if (!(IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT)) {
      int index = HX1838indexUitZoekenVoorSignaal(IrReceiver.decodedIRData.command);
      if (index >= 0) positie = index + 1;
    }

    IrReceiver.resume();
    return positie;
  }
#endif

// ============================================================================
// PUBLIEKE API
// ============================================================================
void InputConfigureren() {
  #if (INPUT_KANAAL_CONFIG & INPUT_TYPE_DIGITAL)
    #if KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_MATRIX_2x2
      pinMode(PIN_TOETS_1, OUTPUT);
      pinMode(PIN_TOETS_2, OUTPUT);
      digitalWrite(PIN_TOETS_1, HIGH);
      digitalWrite(PIN_TOETS_2, HIGH);
      pinMode(PIN_TOETS_3, INPUT_PULLUP);
      pinMode(PIN_TOETS_4, INPUT_PULLUP);
    #elif KEYPAD_TYPE == KEYPAD_TYPE_TOUCH_TTP224_DIRECT_1x4
      pinMode(PIN_TOETS_1, INPUT);
      pinMode(PIN_TOETS_2, INPUT);
      pinMode(PIN_TOETS_3, INPUT);
      pinMode(PIN_TOETS_4, INPUT);
    #else
      pinMode(PIN_TOETS_1, INPUT_PULLUP);
      pinMode(PIN_TOETS_2, INPUT_PULLUP);
      pinMode(PIN_TOETS_3, INPUT_PULLUP);
      pinMode(PIN_TOETS_4, INPUT_PULLUP);
    #endif
  #endif

  #if (INPUT_KANAAL_CONFIG & INPUT_TYPE_PCF8574)
  // businitialisatie loopt nu via GedeeldeBus (Systeem/GedeeldeBus/), gedragsbehoudend t.o.v. de vorige, hier lokaal herhaalde ARDI32-logica.
  GedeeldeBusInitialiseren(GedeeldeBusType::I2C);
    if (!pcf8574.begin(0xFF)) {
      pcf8574Bereikbaar = false;
      
      if (!pcf8574FoutmeldingWeergegeven) {
        static char pcf8574AdresBuffer[17];
        snprintf(pcf8574AdresBuffer, sizeof(pcf8574AdresBuffer), "I2C-adres: 0x%02X", I2C_ADDRESS_PCF8574);
#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
        PrintToScreen(ScreenData::TYPE_FATAL, _FATAL_IN000, pcf8574AdresBuffer, FATAL_LEESTIJD_MS);
#else
        PrintToScreen(_FATAL_IN000, pcf8574AdresBuffer, FATAL_LEESTIJD_MS);
#endif
        pcf8574FoutmeldingWeergegeven = true;
      }
    } else {
      pcf8574Bereikbaar = true;
    }
  #endif

  #if (INPUT_KANAAL_CONFIG & INPUT_TYPE_HX1838)
    IrReceiver.begin(HX1838_ONTVANGER_PIN, ENABLE_LED_FEEDBACK);
    #if HX1838_BRON_CODES == HX1838_BRON_CODES_DEFINE
      HX1838mappingUitUserConfigInladen();
#if (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
      Serial.println(_INPUT_HX1838_MAPPING_CONFIG_GELADEN_SERIAL);
#endif
      PrintToScreen(_INPUT_HX1838_MAPPING_GELADEN, "", 2000);
    #elif HX1838_BRON_CODES == HX1838_BRON_CODES_EEPROM_ALTIJD
      if (HX1838kalibratieLaden()) {
#if (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
        Serial.println(_INPUT_HX1838_KALIBRATIE_GELADEN_SERIAL);
#endif
        PrintToScreen(_INPUT_HX1838_KALIBRATIE_GELADEN, "", 2000);
      } else {
#if (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
        Serial.println(_INPUT_HX1838_GEEN_GELDIGE_KALIBRATIE_SERIAL);
#endif
        HX1838kalibratieUitvoeren();
      }
    #else
      if (HX1838mappingUitUserConfigInladen()) {
#if (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
        Serial.println(_INPUT_HX1838_MAPPING_CONFIG_GELADEN_SERIAL);
#endif
        PrintToScreen(_INPUT_HX1838_MAPPING_GELADEN, "", 2000);
      } else if (HX1838kalibratieLaden()) {
#if (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
        Serial.println(_INPUT_HX1838_KALIBRATIE_GELADEN_SERIAL);
#endif
        PrintToScreen(_INPUT_HX1838_KALIBRATIE_GELADEN, "", 2000);
      } else {
#if (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
        Serial.println(_INPUT_HX1838_GEEN_GELDIGE_KALIBRATIE_SERIAL);
#endif
        HX1838kalibratieUitvoeren();
      }
    #endif
  #endif
}

#ifdef INPUT_KANAAL_OPVRAGEN_HUIDIGE_TOETSAANSLAG_UITGEBREID
static unsigned long laatsteInvoerTijdstipVoorTimeout = 0;
#endif

InputResultaat OpvragenHuidigeToetsAanslag(bool wachten) {
  InputResultaat resultaat = {InputKanaal::NONE, 0, nullptr};

  #if INPUT_KANAAL_CONFIG == INPUT_TYPE_NONE
    return resultaat;
  #endif

  #if (INPUT_KANAAL_CONFIG & (INPUT_TYPE_DIGITAL | INPUT_TYPE_PCF8574))
    if (wachten) {
      while (KeypadUitLezenRuweData() != 0) delay(1);
      vorigeRauweKeypadPositie = 0;
      stabieleKeypadPositie = 0;
      keypadWijzigingSinds = millis();
    }
  #endif

  while (true) {
    #if (INPUT_KANAAL_CONFIG & INPUT_TYPE_DIGITAL)
      int positieKeypad = wachten ? KeypadUitLezenToetsAanslag() : KeypadUitLezenRuweData();

      if (positieKeypad > 0 && positieKeypad <= AANTAL_KEYPAD_TOETSEN) {
        #ifdef INPUT_KANAAL_OPVRAGEN_HUIDIGE_TOETSAANSLAG_UITGEBREID
          laatsteInvoerTijdstipVoorTimeout = millis();
        #endif
        if (wachten) {
          while (KeypadUitLezenRuweData() != 0) delay(1);
          vorigeRauweKeypadPositie = 0;
          stabieleKeypadPositie = 0;
          keypadWijzigingSinds = millis();
        }

        return {InputKanaal::DIGITAL, positieKeypad, KEY_LAYOUT[positieKeypad - 1].opschrift};
      }

      #ifdef INPUT_KANAAL_OPVRAGEN_HUIDIGE_TOETSAANSLAG_UITGEBREID
      if (!wachten) {
        int losgelatenPositie = KeypadUitLezenLosgelatenPositie();
        if (losgelatenPositie > 0 && losgelatenPositie <= AANTAL_KEYPAD_TOETSEN) {
          laatsteInvoerTijdstipVoorTimeout = millis();
          return {InputKanaal::DIGITAL, losgelatenPositie, KEY_LAYOUT[losgelatenPositie - 1].opschrift, InputGebeurtenis::LOSGELATEN};
        }

        const char* opschriftVoorDrempel = (stabieleKeypadPositie > 0 && stabieleKeypadPositie <= AANTAL_KEYPAD_TOETSEN) ? KEY_LAYOUT[stabieleKeypadPositie - 1].opschrift : nullptr;
        if (opschriftVoorDrempel != nullptr) {
          const MappingTussenToetsaanslagEnUitTeVoerenFunctie* regel = OpzoekenUitTeVoerenFunctieViaOpschriftToetsAanslag(opschriftVoorDrempel);
          if (regel != nullptr && regel->functieBijLangIndrukken != nullptr) {
            int langIndrukkenPositie = KeypadUitLezenLangIngedruktePositie(regel->langIndrukkenDrempelMs);
            if (langIndrukkenPositie > 0) {
              laatsteInvoerTijdstipVoorTimeout = millis();
              return {InputKanaal::DIGITAL, langIndrukkenPositie, KEY_LAYOUT[langIndrukkenPositie - 1].opschrift, InputGebeurtenis::LANG_INDRUKKEN};
            }
          }
        }
      }
      #endif
    #elif (INPUT_KANAAL_CONFIG & INPUT_TYPE_PCF8574)
      int positieKeypad = wachten ? KeypadUitLezenToetsAanslag() : KeypadUitLezenRuweData();

      if (positieKeypad > 0 && positieKeypad <= AANTAL_KEYPAD_TOETSEN) {
        #ifdef INPUT_KANAAL_OPVRAGEN_HUIDIGE_TOETSAANSLAG_UITGEBREID
          laatsteInvoerTijdstipVoorTimeout = millis();
        #endif
        if (wachten) {
          while (KeypadUitLezenRuweData() != 0) delay(1);
          vorigeRauweKeypadPositie = 0;
          stabieleKeypadPositie = 0;
          keypadWijzigingSinds = millis();
        }

        return {InputKanaal::PCF8574, positieKeypad, KEY_LAYOUT[positieKeypad - 1].opschrift};
      }

      #ifdef INPUT_KANAAL_OPVRAGEN_HUIDIGE_TOETSAANSLAG_UITGEBREID
      if (!wachten) {
        int losgelatenPositie = KeypadUitLezenLosgelatenPositie();
        if (losgelatenPositie > 0 && losgelatenPositie <= AANTAL_KEYPAD_TOETSEN) {
          laatsteInvoerTijdstipVoorTimeout = millis();
          return {InputKanaal::PCF8574, losgelatenPositie, KEY_LAYOUT[losgelatenPositie - 1].opschrift, InputGebeurtenis::LOSGELATEN};
        }

        const char* opschriftVoorDrempel = (stabieleKeypadPositie > 0 && stabieleKeypadPositie <= AANTAL_KEYPAD_TOETSEN) ? KEY_LAYOUT[stabieleKeypadPositie - 1].opschrift : nullptr;
        if (opschriftVoorDrempel != nullptr) {
          const MappingTussenToetsaanslagEnUitTeVoerenFunctie* regel = OpzoekenUitTeVoerenFunctieViaOpschriftToetsAanslag(opschriftVoorDrempel);
          if (regel != nullptr && regel->functieBijLangIndrukken != nullptr) {
            int langIndrukkenPositie = KeypadUitLezenLangIngedruktePositie(regel->langIndrukkenDrempelMs);
            if (langIndrukkenPositie > 0) {
              laatsteInvoerTijdstipVoorTimeout = millis();
              return {InputKanaal::PCF8574, langIndrukkenPositie, KEY_LAYOUT[langIndrukkenPositie - 1].opschrift, InputGebeurtenis::LANG_INDRUKKEN};
            }
          }
        }
      }
      #endif
    #endif

    #if (INPUT_KANAAL_CONFIG & INPUT_TYPE_HX1838)
      int positieIR = HX1838uitLezenToetsAanslag();
      if (positieIR > 0 && positieIR <= AANTAL_IR_TOETSEN) {
        #ifdef INPUT_KANAAL_OPVRAGEN_HUIDIGE_TOETSAANSLAG_UITGEBREID
          laatsteInvoerTijdstipVoorTimeout = millis();
        #endif
        return {InputKanaal::HX1838, positieIR, IR_KEY_LAYOUT[positieIR - 1].opschrift};
      }
    #endif

    #ifdef INPUT_KANAAL_OPVRAGEN_HUIDIGE_TOETSAANSLAG_UITGEBREID
      if (!wachten && INPUT_KANAAL_TIMEOUT_BIJ_GEEN_TOETSAANSLAG_BINNEN_MS > 0 && (millis() - laatsteInvoerTijdstipVoorTimeout) >= INPUT_KANAAL_TIMEOUT_BIJ_GEEN_TOETSAANSLAG_BINNEN_MS) {
        laatsteInvoerTijdstipVoorTimeout = millis();
        return {InputKanaal::NONE, 0, nullptr, InputGebeurtenis::TIMEOUT_GEEN_INVOER};
      }
    #endif

    if (!wachten) return resultaat;
    delay(1);
  }
}

InputResultaten OpvragenHuidigeToetsAanslagen(bool wachten, byte aantalSimultaan) {
  InputResultaten resultaten = { StatusOpvragenToetsAanslagen::GEEN, 0, {{InputKanaal::NONE, 0, nullptr}} };

  if (aantalSimultaan != 1) {
    resultaten.status = StatusOpvragenToetsAanslagen::NIET_GEIMPLEMENTEERD;
    return resultaten;
  }

  InputResultaat resultaat = OpvragenHuidigeToetsAanslag(wachten);
  if (resultaat.inputKanaal == InputKanaal::NONE) return resultaten;

  resultaten.status = StatusOpvragenToetsAanslagen::GELDIG;
  resultaten.aantalToetsAanslagen = 1;
  resultaten.toetsAanslagen[0] = resultaat;
  return resultaten;
}

