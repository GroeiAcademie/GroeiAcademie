#include "../Screen/Screen.h"
#include "Input.h"
#include "../GedeeldeBus/GedeeldeBus.h"

Input::Input()
  : Input(INPUT_KANAAL_CONFIG)
{}

Input::Input(uint8_t typesActief)
  : GedeeldeBusNode(&Native, { nullptr, 0, nullptr, 0 }, GedeeldeBusComponent::GC_INPUT, HardwareResourceToegang::GEDEELD), typesActief(typesActief)
{}

struct Input* Input = nullptr;
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

#if ((INPUT_KANAAL_CONFIG) & (INPUT_TYPE_DIGITAL | INPUT_TYPE_PCF8574))

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

  #elif ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_PCF8574)
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

#if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_HX1838)
  #if HX1838_TOETSENINDELING == HX1838_TOETSENINDELING_REMOTE_OK_BOVENAAN_17_TOETSEN
    const MappingTussenOpschriftEnWeergavetekst IR_KEY_LAYOUT[] = {
      {_LABEL_OPSCHRIFT_UP, LABEL_TOETS_UP}, {_LABEL_OPSCHRIFT_DOWN, LABEL_TOETS_DOWN}, {_LABEL_OPSCHRIFT_OK, LABEL_TOETS_OK},
      {_LABEL_OPSCHRIFT_LEFT, LABEL_TOETS_LEFT}, {_LABEL_OPSCHRIFT_RIGHT, LABEL_TOETS_RIGHT},
      {_LABEL_OPSCHRIFT_1, LABEL_TOETS_1}, {_LABEL_OPSCHRIFT_2, LABEL_TOETS_2}, {_LABEL_OPSCHRIFT_3, LABEL_TOETS_3},
      {_LABEL_OPSCHRIFT_4, LABEL_TOETS_4}, {_LABEL_OPSCHRIFT_5, LABEL_TOETS_5}, {_LABEL_OPSCHRIFT_6, LABEL_TOETS_6},
      {_LABEL_OPSCHRIFT_7, LABEL_TOETS_7}, {_LABEL_OPSCHRIFT_8, LABEL_TOETS_8}, {_LABEL_OPSCHRIFT_9, LABEL_TOETS_9},
      {_LABEL_OPSCHRIFT_STER, LABEL_TOETS_STER}, {_LABEL_OPSCHRIFT_0, LABEL_TOETS_0}, {_LABEL_OPSCHRIFT_HEKJE, LABEL_TOETS_HEKJE}
    };
  #elif HX1838_TOETSENINDELING == HX1838_TOETSENINDELING_REMOTE_OK_ONDERAAN_17_TOETSEN
    const MappingTussenOpschriftEnWeergavetekst IR_KEY_LAYOUT[] = {
      {_LABEL_OPSCHRIFT_1, LABEL_TOETS_1}, {_LABEL_OPSCHRIFT_2, LABEL_TOETS_2}, {_LABEL_OPSCHRIFT_3, LABEL_TOETS_3},
      {_LABEL_OPSCHRIFT_4, LABEL_TOETS_4}, {_LABEL_OPSCHRIFT_5, LABEL_TOETS_5}, {_LABEL_OPSCHRIFT_6, LABEL_TOETS_6},
      {_LABEL_OPSCHRIFT_7, LABEL_TOETS_7}, {_LABEL_OPSCHRIFT_8, LABEL_TOETS_8}, {_LABEL_OPSCHRIFT_9, LABEL_TOETS_9},
      {_LABEL_OPSCHRIFT_STER, LABEL_TOETS_STER}, {_LABEL_OPSCHRIFT_0, LABEL_TOETS_0}, {_LABEL_OPSCHRIFT_HEKJE, LABEL_TOETS_HEKJE},
      {_LABEL_OPSCHRIFT_UP, LABEL_TOETS_UP}, {_LABEL_OPSCHRIFT_DOWN, LABEL_TOETS_DOWN}, {_LABEL_OPSCHRIFT_OK, LABEL_TOETS_OK},
      {_LABEL_OPSCHRIFT_LEFT, LABEL_TOETS_LEFT}, {_LABEL_OPSCHRIFT_RIGHT, LABEL_TOETS_RIGHT}
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
  #elif HX1838_TOETSENINDELING == HX1838_TOETSENINDELING_REMOTE_USER_DEFINED
    const MappingTussenOpschriftEnWeergavetekst IR_KEY_LAYOUT[] = HX1838_GENERIEK_KEY_LAYOUT;
  #else
    #error Ongeldige HX1838_TOETSENINDELING.
  #endif

  static_assert(
    (sizeof(IR_KEY_LAYOUT) / sizeof(IR_KEY_LAYOUT[0])) == AANTAL_IR_TOETSEN,
    "IR_KEY_LAYOUT " _INPUT_HX1838_STATIC_ASSERT_AANTAL_TOETSEN
  );
#endif

#if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_PCF8574)
#endif

#if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_HX1838)
  #define NO_LED_RECEIVE_FEEDBACK_CODE

  #if HX1838_USE_TINYIRRECEIVER_INSTEAD_OF_IRREMOTE
    #define IR_RECEIVE_PIN HX1838_ONTVANGER_PIN
    #include <TinyIRReceiver.hpp>
  #else
    #include <IRremote.hpp>
  #endif

  #if HX1838_BRON_CODES == HX1838_BRON_CODES_EEPROM_ALTIJD || HX1838_BRON_CODES == HX1838_BRON_CODES_EEPROM_WANNEER_GEEN_DEFINE
    #include <EEPROM.h>
  #endif
#endif

extern const MappingTussenToetsaanslagEnUitTeVoerenFunctie mappingTussenToetsaanslagEnUitTeVoerenFunctie[] __attribute__((weak)) = {
  {nullptr, nullptr}
};
extern const byte aantalToetsFuncties __attribute__((weak)) = 0;

// ============================================================================
// TOETS -> FUNCTIE OPZOEKEN (mappingTussenToetsaanslagEnUitTeVoerenFunctie wordt door de gebruiker gedefinieerd, in de sketch of UserConfig.h)
// ============================================================================
// OpzoekenUitTeVoerenFunctieViaOpschriftToetsAanslag() retourneert de volledige koppeling met opschrift en functiepointer.
const MappingTussenToetsaanslagEnUitTeVoerenFunctie* Input::OpzoekenUitTeVoerenFunctieViaOpschriftToetsAanslag(const char* opschriftToetsAanslag) {
  if (opschriftToetsAanslag == nullptr) return nullptr;

  for (byte i = 0; i < aantalToetsFuncties; i++) {
    if (strcmp(mappingTussenToetsaanslagEnUitTeVoerenFunctie[i].opschriftToetsAanslag, opschriftToetsAanslag) == 0) {
      return &mappingTussenToetsaanslagEnUitTeVoerenFunctie[i];
    }
  }

  return nullptr;
}

#ifdef INPUT_KANAAL_OPVRAGEN_HUIDIGE_TOETSAANSLAG_UITGEBREID
unsigned long Input::_StandaardLangIndrukkenDrempelOpzoeker(const char* opschriftToetsAanslag) {
  const MappingTussenToetsaanslagEnUitTeVoerenFunctie* regel = ::Input->OpzoekenUitTeVoerenFunctieViaOpschriftToetsAanslag(opschriftToetsAanslag);
  if (regel == nullptr || regel->functieBijLangIndrukken == nullptr) return 0;
  return regel->langIndrukkenDrempelMs;
}
#else
unsigned long Input::_StandaardLangIndrukkenDrempelOpzoeker(const char* opschriftToetsAanslag) {
  (void)opschriftToetsAanslag;
  return 0;
}
#endif

#ifdef INPUT_KANAAL_OPVRAGEN_HUIDIGE_TOETSAANSLAG_UITGEBREID
unsigned long Input::_LangIndrukkenDrempelOpzoekerViaActieveMapping(const char* opschriftToetsAanslag) {
  if (::Input->_actieveMappingVoorLangIndrukken == nullptr || opschriftToetsAanslag == nullptr) return 0;

  for (byte i = 0; i < ::Input->_actieveMappingAantalVoorLangIndrukken; i++) {
    if (strcmp(::Input->_actieveMappingVoorLangIndrukken[i].opschriftToetsAanslag, opschriftToetsAanslag) == 0) {
      if (::Input->_actieveMappingVoorLangIndrukken[i].functieBijLangIndrukken == nullptr) return 0;
      return ::Input->_actieveMappingVoorLangIndrukken[i].langIndrukkenDrempelMs;
    }
  }

  return 0;
}

unsigned long Input::_LangIndrukkenDrempelOpzoekerViaActieveMappingMetArgumenten(const char* opschriftToetsAanslag) {
  if (::Input->_actieveMappingMetArgumentenVoorLangIndrukken == nullptr || opschriftToetsAanslag == nullptr) return 0;

  for (byte i = 0; i < ::Input->_actieveMappingMetArgumentenAantalVoorLangIndrukken; i++) {
    if (strcmp(::Input->_actieveMappingMetArgumentenVoorLangIndrukken[i].opschriftToetsAanslag, opschriftToetsAanslag) == 0) {
      if (::Input->_actieveMappingMetArgumentenVoorLangIndrukken[i].functieBijLangIndrukken == nullptr) return 0;
      return ::Input->_actieveMappingMetArgumentenVoorLangIndrukken[i].langIndrukkenDrempelMs;
    }
  }

  return 0;
}
#endif

#ifdef INPUT_MAPPING_EXTRA_CONTROLES_INSCHAKELEN
// ============================================================================
// MAPPING-VOLLEDIGHEIDSCONTROLE (enkel bedoeld om tijdens het testen op te roepen)
// ============================================================================
void Input::ControleerMappingVolledigheidIntern(const char* const opschriftToetsAanslag[], byte aantalEntries) {
  (void)opschriftToetsAanslag;
  (void)aantalEntries;
  byte aantalOntbrekend = 0;
#if ((INPUT_KANAAL_CONFIG) & (INPUT_TYPE_DIGITAL | INPUT_TYPE_PCF8574))
  for (byte i = 0; i < (sizeof(KEY_LAYOUT) / sizeof(KEY_LAYOUT[0])); i++) {
    bool gevonden = false;

    for (byte j = 0; j < aantalEntries; j++) {
      if (strcmp(KEY_LAYOUT[i].opschrift, opschriftToetsAanslag[j]) == 0) { gevonden = true; break; }
    }

    if (!gevonden) {
#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
      Screen->Print(ScreenData::TYPE_WARNING, _INPUT_MAPPINGCONTROLE_WAARSCHUWING, String(_INPUT_MAPPINGCONTROLE_ONTBREEKT) + ": " + KEY_LAYOUT[i].opschrift);
#else
      Screen->Print(_INPUT_MAPPINGCONTROLE_WAARSCHUWING, String(_INPUT_MAPPINGCONTROLE_ONTBREEKT) + ": " + KEY_LAYOUT[i].opschrift);
#endif
      aantalOntbrekend++;
    }
  }
#endif
#if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_HX1838)
  for (byte i = 0; i < (sizeof(IR_KEY_LAYOUT) / sizeof(IR_KEY_LAYOUT[0])); i++) {
    bool gevonden = false;

    for (byte j = 0; j < aantalEntries; j++) {
      if (strcmp(IR_KEY_LAYOUT[i].opschrift, opschriftToetsAanslag[j]) == 0) { gevonden = true; break; }
    }

    if (!gevonden) {
#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
      Screen->Print(ScreenData::TYPE_WARNING, _INPUT_MAPPINGCONTROLE_WAARSCHUWING, String(_INPUT_MAPPINGCONTROLE_ONTBREEKT) + ": " + IR_KEY_LAYOUT[i].opschrift);
#else
      Screen->Print(_INPUT_MAPPINGCONTROLE_WAARSCHUWING, String(_INPUT_MAPPINGCONTROLE_ONTBREEKT) + ": " + IR_KEY_LAYOUT[i].opschrift);
#endif
      aantalOntbrekend++;
    }
  }
#endif
  if (aantalOntbrekend == 0) {
#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
    Screen->Print(ScreenData::TYPE_INFO, _INPUT_MAPPINGCONTROLE_TITEL, _INPUT_MAPPINGCONTROLE_OPSCHRIFTEN_OK);
#else
    Screen->Print(_INPUT_MAPPINGCONTROLE_TITEL, _INPUT_MAPPINGCONTROLE_OPSCHRIFTEN_OK);
#endif
  }
}
#endif // INPUT_MAPPING_EXTRA_CONTROLES_INSCHAKELEN

void Input::UitVoerenFunctieVolgensMappingMetToetsAanslag(bool wachten) {
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
#if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_PCF8574)
  template <size_t N>
  constexpr bool PCF8574PinnenBinnenBereik(const byte (&pinnen)[N], size_t index = 0) { return index >= N ? true : (pinnen[index] <= 7 && PCF8574PinnenBinnenBereik(pinnen, index + 1)); }

  template <size_t N>
  constexpr bool PCF8574PinnenUniek(const byte (&pinnen)[N], size_t eerste = 0, size_t tweede = 1) { return eerste >= N ? true : (tweede >= N ? PCF8574PinnenUniek(pinnen, eerste + 1, eerste + 2) : (pinnen[eerste] != pinnen[tweede] && PCF8574PinnenUniek(pinnen, eerste, tweede + 1))); }

  template <size_t N, size_t M>
  constexpr bool PCF8574PinnenNietOverlappend(const byte (&eerste)[N], const byte (&tweede)[M], size_t eersteIndex = 0, size_t tweedeIndex = 0) { return eersteIndex >= N ? true : (tweedeIndex >= M ? PCF8574PinnenNietOverlappend(eerste, tweede, eersteIndex + 1, 0) : (eerste[eersteIndex] != tweede[tweedeIndex] && PCF8574PinnenNietOverlappend(eerste, tweede, eersteIndex, tweedeIndex + 1))); }

  #if KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_DIRECT_1x4
    #define KEYPAD_IS_DIRECT
    static const byte AANTAL_DIRECT_PINNEN = 4;
    static const byte TOETS_PINNEN[AANTAL_DIRECT_PINNEN] = {INPUT_KEYPAD_PIN_K1, INPUT_KEYPAD_PIN_K2, INPUT_KEYPAD_PIN_K3, INPUT_KEYPAD_PIN_K4};
  #elif KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_DIRECT_2x4
    #define KEYPAD_IS_DIRECT
    static const byte AANTAL_DIRECT_PINNEN = 8;
    static const byte TOETS_PINNEN[AANTAL_DIRECT_PINNEN] = {INPUT_KEYPAD_PIN_K1, INPUT_KEYPAD_PIN_K2, INPUT_KEYPAD_PIN_K3, INPUT_KEYPAD_PIN_K4, INPUT_KEYPAD_PIN_K5, INPUT_KEYPAD_PIN_K6, INPUT_KEYPAD_PIN_K7, INPUT_KEYPAD_PIN_K8};
  #elif KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_MATRIX_2x2
    // OT3688: vier knoppen, hergebruikt K1..K4 op de PCF8574
    #define KEYPAD_IS_MATRIX
    static const byte AANTAL_RIJEN = 2;
    static const byte AANTAL_KOLOMMEN = 2;
    static const byte RIJ_PINNEN[AANTAL_RIJEN] = {INPUT_KEYPAD_PIN_L1, INPUT_KEYPAD_PIN_L2};
    static const byte KOLOM_PINNEN[AANTAL_KOLOMMEN] = {INPUT_KEYPAD_PIN_R1, INPUT_KEYPAD_PIN_R2};
  #elif KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_MATRIX_4x4
    #define KEYPAD_IS_MATRIX
    static const byte AANTAL_RIJEN = 4;
    static const byte AANTAL_KOLOMMEN = 4;
    static const byte RIJ_PINNEN[AANTAL_RIJEN] = {INPUT_KEYPAD_PIN_R1, INPUT_KEYPAD_PIN_R2, INPUT_KEYPAD_PIN_R3, INPUT_KEYPAD_PIN_R4};
    static const byte KOLOM_PINNEN[AANTAL_KOLOMMEN] = {INPUT_KEYPAD_PIN_C1, INPUT_KEYPAD_PIN_C2, INPUT_KEYPAD_PIN_C3, INPUT_KEYPAD_PIN_C4};
  #elif KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4 || KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_DIRECT_4x1
    #define KEYPAD_IS_DIRECT
    static const byte AANTAL_DIRECT_PINNEN = 4;
    static const byte TOETS_PINNEN[AANTAL_DIRECT_PINNEN] = {INPUT_KEYPAD_PIN_1, INPUT_KEYPAD_PIN_2, INPUT_KEYPAD_PIN_3, INPUT_KEYPAD_PIN_4};
  #elif KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_MATRIX_1x4
    #define KEYPAD_IS_MATRIX
    static const byte AANTAL_RIJEN = 1;
    static const byte AANTAL_KOLOMMEN = 4;
    static const byte RIJ_PINNEN[AANTAL_RIJEN] = {INPUT_KEYPAD_PIN_R1};
    static const byte KOLOM_PINNEN[AANTAL_KOLOMMEN] = {INPUT_KEYPAD_PIN_C1, INPUT_KEYPAD_PIN_C2, INPUT_KEYPAD_PIN_C3, INPUT_KEYPAD_PIN_C4};
  #elif KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_MATRIX_2x4
    #define KEYPAD_IS_MATRIX
    static const byte AANTAL_RIJEN = 2;
    static const byte AANTAL_KOLOMMEN = 4;
    static const byte RIJ_PINNEN[AANTAL_RIJEN] = {INPUT_KEYPAD_PIN_R1, INPUT_KEYPAD_PIN_R2};
    static const byte KOLOM_PINNEN[AANTAL_KOLOMMEN] = {INPUT_KEYPAD_PIN_C1, INPUT_KEYPAD_PIN_C2, INPUT_KEYPAD_PIN_C3, INPUT_KEYPAD_PIN_C4};
  #elif KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_MATRIX_4x3
    #define KEYPAD_IS_MATRIX
    static const byte AANTAL_RIJEN = 4;
    static const byte AANTAL_KOLOMMEN = 3;
    static const byte RIJ_PINNEN[AANTAL_RIJEN] = {INPUT_KEYPAD_PIN_R1, INPUT_KEYPAD_PIN_R2, INPUT_KEYPAD_PIN_R3, INPUT_KEYPAD_PIN_R4};
    static const byte KOLOM_PINNEN[AANTAL_KOLOMMEN] = {INPUT_KEYPAD_PIN_C1, INPUT_KEYPAD_PIN_C2, INPUT_KEYPAD_PIN_C3};
  #elif KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_MATRIX_4x4
    #define KEYPAD_IS_MATRIX
    static const byte AANTAL_RIJEN = 4;
    static const byte AANTAL_KOLOMMEN = 4;
    static const byte RIJ_PINNEN[AANTAL_RIJEN] = {INPUT_KEYPAD_PIN_R1, INPUT_KEYPAD_PIN_R2, INPUT_KEYPAD_PIN_R3, INPUT_KEYPAD_PIN_R4};
    static const byte KOLOM_PINNEN[AANTAL_KOLOMMEN] = {INPUT_KEYPAD_PIN_C1, INPUT_KEYPAD_PIN_C2, INPUT_KEYPAD_PIN_C3, INPUT_KEYPAD_PIN_C4};
  #elif KEYPAD_TYPE == KEYPAD_TYPE_TOUCH_TTP224_DIRECT_1x4
    #define KEYPAD_IS_DIRECT
    static const byte AANTAL_DIRECT_PINNEN = 4;
    static const byte TOETS_PINNEN[AANTAL_DIRECT_PINNEN] = {INPUT_KEYPAD_PIN_OUT1, INPUT_KEYPAD_PIN_OUT2, INPUT_KEYPAD_PIN_OUT3, INPUT_KEYPAD_PIN_OUT4};
  #elif KEYPAD_TYPE == KEYPAD_TYPE_TOUCH_TTP229_MATRIX_4x4
    #define KEYPAD_IS_DIRECT
    static const byte AANTAL_DIRECT_PINNEN = 8;
    static const byte TOETS_PINNEN[AANTAL_DIRECT_PINNEN] = {INPUT_KEYPAD_PIN_OUT1, INPUT_KEYPAD_PIN_OUT2, INPUT_KEYPAD_PIN_OUT3, INPUT_KEYPAD_PIN_OUT4, INPUT_KEYPAD_PIN_OUT5, INPUT_KEYPAD_PIN_OUT6, INPUT_KEYPAD_PIN_OUT7, INPUT_KEYPAD_PIN_OUT8};
  #elif KEYPAD_TYPE == KEYPAD_TYPE_USER_DEFINED_DIRECT
    #define KEYPAD_IS_DIRECT
    static_assert(KEYPAD_GENERIEK_AANTAL_PINNEN > 0, _INPUT_PCF8574_STATIC_ASSERT_DIRECT_MIN_EEN_PIN);
    static_assert(KEYPAD_GENERIEK_AANTAL_PINNEN <= 8, _INPUT_PCF8574_STATIC_ASSERT_DIRECT_MAX_ACHT_PINNEN);
    static constexpr byte AANTAL_DIRECT_PINNEN = KEYPAD_GENERIEK_AANTAL_PINNEN;
    static constexpr byte TOETS_PINNEN[AANTAL_DIRECT_PINNEN] = KEYPAD_GENERIEK_PINNEN;
  #elif KEYPAD_TYPE == KEYPAD_TYPE_USER_DEFINED_MATRIX
    #define KEYPAD_IS_MATRIX
    static_assert(KEYPAD_GENERIEK_AANTAL_RIJEN > 0, _INPUT_PCF8574_STATIC_ASSERT_MATRIX_MIN_EEN_RIJ);
    static_assert(KEYPAD_GENERIEK_AANTAL_KOLOMMEN > 0, _INPUT_PCF8574_STATIC_ASSERT_MATRIX_MIN_EEN_KOLOM);
    static_assert((KEYPAD_GENERIEK_AANTAL_RIJEN + KEYPAD_GENERIEK_AANTAL_KOLOMMEN) <= 8, _INPUT_PCF8574_STATIC_ASSERT_MATRIX_MAX_ACHT_PINNEN);
    static constexpr byte AANTAL_RIJEN = KEYPAD_GENERIEK_AANTAL_RIJEN;
    static constexpr byte AANTAL_KOLOMMEN = KEYPAD_GENERIEK_AANTAL_KOLOMMEN;
    static constexpr byte RIJ_PINNEN[AANTAL_RIJEN] = KEYPAD_GENERIEK_RIJ_PINNEN;
    static constexpr byte KOLOM_PINNEN[AANTAL_KOLOMMEN] = KEYPAD_GENERIEK_KOLOM_PINNEN;
  #endif

  #if defined(KEYPAD_IS_DIRECT)
    static_assert(AANTAL_DIRECT_PINNEN <= 8, _INPUT_PCF8574_STATIC_ASSERT_DIRECT_PINNEN_MAX_ACHT);
  #endif
  #if defined(KEYPAD_IS_MATRIX)
    static_assert((AANTAL_RIJEN + AANTAL_KOLOMMEN) <= 8, _INPUT_PCF8574_STATIC_ASSERT_MATRIX_PINNEN_MAX_ACHT);
  #endif

  #if KEYPAD_TYPE == KEYPAD_TYPE_USER_DEFINED_DIRECT
    static_assert((sizeof(KEY_LAYOUT) / sizeof(KEY_LAYOUT[0])) == AANTAL_DIRECT_PINNEN, "KEYPAD_GENERIEK_KEY_LAYOUT " _INPUT_PCF8574_STATIC_ASSERT_KEY_LAYOUT_DIRECT_AANTAL);
    static_assert(PCF8574PinnenBinnenBereik(TOETS_PINNEN), "KEYPAD_GENERIEK_PINNEN " _INPUT_PCF8574_STATIC_ASSERT_PINNEN_BEREIK);
    static_assert(PCF8574PinnenUniek(TOETS_PINNEN), "KEYPAD_GENERIEK_PINNEN " _INPUT_PCF8574_STATIC_ASSERT_PINNEN_UNIEK);
  #elif KEYPAD_TYPE == KEYPAD_TYPE_USER_DEFINED_MATRIX
    static_assert((sizeof(KEY_LAYOUT) / sizeof(KEY_LAYOUT[0])) == (AANTAL_RIJEN * AANTAL_KOLOMMEN), "KEYPAD_GENERIEK_KEY_LAYOUT " _INPUT_PCF8574_STATIC_ASSERT_KEY_LAYOUT_MATRIX_AANTAL);
    static_assert(PCF8574PinnenBinnenBereik(RIJ_PINNEN) && PCF8574PinnenBinnenBereik(KOLOM_PINNEN), "KEYPAD_GENERIEK_RIJ_PINNEN " _INPUT_PCF8574_STATIC_ASSERT_RIJ_KOLOM_EN " KEYPAD_GENERIEK_KOLOM_PINNEN " _INPUT_PCF8574_STATIC_ASSERT_RIJ_KOLOM_BEREIK);
    static_assert(PCF8574PinnenUniek(RIJ_PINNEN) && PCF8574PinnenUniek(KOLOM_PINNEN), _INPUT_PCF8574_STATIC_ASSERT_MATRIX_DUBBELE_PINNEN);
    static_assert(PCF8574PinnenNietOverlappend(RIJ_PINNEN, KOLOM_PINNEN), _INPUT_PCF8574_STATIC_ASSERT_RIJ_KOLOM_OVERLAP);
  #endif


  // Blijvende status: eenmaal onbereikbaar, blijft de melding staan en wordt geen
  // verdere I2C-communicatie meer geprobeerd, i.p.v. stil "geen toets" te blijven melden.
  // Zelfde patroon als characterScreenStatus.foutmeldingWeergegeven in Screen.cpp.

  void Input::PCF8574OnbereikbaarMelden() {
#ifdef DEBUG
    GA_DEBUG_PRINT("DEBUG: IN402 PCF8574 adres=0x");
    GA_DEBUG_PRINTLN2(I2C_ADDRESS_PCF8574, HEX);
#endif
    pcf8574Bereikbaar = false;
    if (pcf8574FoutmeldingWeergegeven) return;
    char pcf8574AdresBuffer[17];
    snprintf(pcf8574AdresBuffer, sizeof(pcf8574AdresBuffer), _INPUT_PCF8574_ADRES_LABEL ": 0x%02X", I2C_ADDRESS_PCF8574);
#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
    Screen->Print(ScreenData::TYPE_FATAL, _FATAL_IN402, pcf8574AdresBuffer, FATAL_LEESTIJD_MS);
#else
    Screen->Print(_FATAL_IN402, pcf8574AdresBuffer, FATAL_LEESTIJD_MS);
#endif
    pcf8574FoutmeldingWeergegeven = true;
  }

  bool Input::PCF8574poortPatroonMatrixUitlezenInstellen(byte waarde) {
#ifdef TRACE
    GA_SERIAL.println("TRACE: Input::PCF8574poortPatroonMatrixUitlezenInstellen()");
#endif
    if (!pcf8574Bereikbaar) return false;
    gedeeldeBusInputPCF8574->WriteByte(waarde);
    if (gedeeldeBusInputPCF8574->lastError() == 0) return true;
    PCF8574OnbereikbaarMelden();
    return false;
  }

  bool Input::PCF8574poortPatroonUitlezen(byte& waarde) {
#ifdef TRACE
    GA_SERIAL.println("TRACE: Input::PCF8574poortPatroonUitlezen()");
#endif
    if (!pcf8574Bereikbaar) return false;
    waarde = gedeeldeBusInputPCF8574->ReadByte();
    if (gedeeldeBusInputPCF8574->lastError() == 0) return true;
    PCF8574OnbereikbaarMelden();
    return false;
  }

  #if defined(KEYPAD_IS_DIRECT)
    int Input::PCF8574uitLezenPoortenP0totP7DirectAansluiting() {
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
    int Input::PCF8574uitLezenPoortenP0totP7MatrixAansluiting() {
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
#if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_DIGITAL)
  int Input::DigitaalUitLezenRuweData() {
    #if KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_MATRIX_2x2
      const byte rijPinnen[2] = {NativeArduinoPinVan(INPUT_KEYPAD_PIN_L1), NativeArduinoPinVan(INPUT_KEYPAD_PIN_L2)};
      const byte kolomPinnen[2] = {NativeArduinoPinVan(INPUT_KEYPAD_PIN_R1), NativeArduinoPinVan(INPUT_KEYPAD_PIN_R2)};

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
      if (digitalRead(NativeArduinoPinVan(INPUT_KEYPAD_PIN_OUT1)) == HIGH) return 1;
      if (digitalRead(NativeArduinoPinVan(INPUT_KEYPAD_PIN_OUT2)) == HIGH) return 2;
      if (digitalRead(NativeArduinoPinVan(INPUT_KEYPAD_PIN_OUT3)) == HIGH) return 3;
      if (digitalRead(NativeArduinoPinVan(INPUT_KEYPAD_PIN_OUT4)) == HIGH) return 4;
      return 0;
    #elif KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_DIRECT_1x4
      if (digitalRead(NativeArduinoPinVan(INPUT_KEYPAD_PIN_K1)) == LOW) return 1;
      if (digitalRead(NativeArduinoPinVan(INPUT_KEYPAD_PIN_K2)) == LOW) return 2;
      if (digitalRead(NativeArduinoPinVan(INPUT_KEYPAD_PIN_K3)) == LOW) return 3;
      if (digitalRead(NativeArduinoPinVan(INPUT_KEYPAD_PIN_K4)) == LOW) return 4;
      return 0;
    #else
      if (digitalRead(NativeArduinoPinVan(INPUT_KEYPAD_PIN_1)) == LOW) return 1;
      if (digitalRead(NativeArduinoPinVan(INPUT_KEYPAD_PIN_2)) == LOW) return 2;
      if (digitalRead(NativeArduinoPinVan(INPUT_KEYPAD_PIN_3)) == LOW) return 3;
      if (digitalRead(NativeArduinoPinVan(INPUT_KEYPAD_PIN_4)) == LOW) return 4;
      return 0;
    #endif
  }
#endif

#ifdef INPUT_KANAAL_OPVRAGEN_HUIDIGE_TOETSAANSLAG_UITGEBREID
#endif

// ============================================================================
// DEBOUNCE + NIEUWE-TOETSAANSLAGDETECTIE VOOR FYSIEKE KEYPADS
// ============================================================================
#if ((INPUT_KANAAL_CONFIG) & (INPUT_TYPE_DIGITAL | INPUT_TYPE_PCF8574))

  #ifdef INPUT_KANAAL_OPVRAGEN_HUIDIGE_TOETSAANSLAG_UITGEBREID
  #endif

  int Input::KeypadUitLezenRuweData() {
    #if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_DIGITAL)
      return DigitaalUitLezenRuweData();
    #elif ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_PCF8574)
      #if defined(KEYPAD_IS_MATRIX)
        return PCF8574uitLezenPoortenP0totP7MatrixAansluiting();
      #else
        return PCF8574uitLezenPoortenP0totP7DirectAansluiting();
      #endif
    #endif
  }

  int Input::KeypadUitLezenToetsAanslag() {
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
  int Input::KeypadUitLezenLosgelatenPositie() {
    int resultaat = 0;

    if (laatstePositieVoorLoslatenDetectie > 0 && stabieleKeypadPositie == 0) {
      resultaat = laatstePositieVoorLoslatenDetectie;
    }

    laatstePositieVoorLoslatenDetectie = stabieleKeypadPositie;
    return resultaat;
  }

  // Geeft de huidige, nog ingedrukte positie terug zodra ze langer dan drempelMs ingedrukt is,
  // en meldt dat maar één keer per druk (niet herhaald bij elke aanroep).
  int Input::KeypadUitLezenLangIngedruktePositie(unsigned long drempelMs) {
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
#if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_HX1838)

  template <size_t N>
  constexpr bool HX1838CodesNietNul(const uint8_t (&codes)[N], size_t index = 0) { return index >= N ? true : (codes[index] != 0 && HX1838CodesNietNul(codes, index + 1)); }

#if (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
  void Input::HX1838toonTabelMetCodes() {
#if HX1838_TOETSENINDELING == HX1838_TOETSENINDELING_REMOTE_USER_DEFINED
    GA_SERIAL.print(F("// #define HX1838_GENERIEK_CODES {"));
    for (byte i = 0; i < AANTAL_IR_TOETSEN; i++) {
      if (i > 0) GA_SERIAL.print(F(", "));
      GA_SERIAL.print(F("0x"));
      if (irCodes[i] < 0x10) GA_SERIAL.print('0');
      GA_SERIAL.print(irCodes[i], HEX);
      GA_SERIAL.print(F("UL"));
    }
    GA_SERIAL.println(F("}"));
#else
    for (byte i = 0; i < AANTAL_IR_TOETSEN; i++) {
#ifdef TRACE
      GA_DEBUG_PRINT("HX1838toonTabelMetCodes(): ");
      GA_DEBUG_PRINT(_INPUT_HX1838_DEBUG_REGEL);
      GA_DEBUG_PRINT(" ");
      GA_DEBUG_PRINT(i + 1);
      GA_DEBUG_PRINT("/");
      GA_DEBUG_PRINTLN(AANTAL_IR_TOETSEN);
#endif
      GA_SERIAL.print(F("// #define HX1838_CODE_"));
      GA_SERIAL.print(i + 1);
      GA_SERIAL.print((i + 1) < 10 ? F("  0x") : F(" 0x"));
      if (irCodes[i] < 0x10) GA_SERIAL.print('0');
      GA_SERIAL.print(irCodes[i], HEX);
      GA_SERIAL.println(F("UL"));
    }
#endif
  }
#endif

#if HX1838_TOETSENINDELING == HX1838_TOETSENINDELING_REMOTE_USER_DEFINED && defined(HX1838_GENERIEK_CODES_KALIBREREN)

  void Input::HX1838GeneriekCodesKalibreren() {
#if (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
    GA_SERIAL.println("HX1838_GENERIEK_CODES " _INPUT_HX1838_GENERIEK_CODES_NIET_GEDEFINIEERD_KALIBRATIE_GESTART);
#endif
    for (byte i = 0; i < AANTAL_IR_TOETSEN; i++) {
#ifdef TRACE
      GA_DEBUG_PRINT("HX1838GeneriekCodesKalibreren(): ");
      GA_DEBUG_PRINT(_INPUT_HX1838_DEBUG_TOETS);
      GA_DEBUG_PRINT(" ");
      GA_DEBUG_PRINT(i + 1);
      GA_DEBUG_PRINT("/");
      GA_DEBUG_PRINTLN(AANTAL_IR_TOETSEN);
#endif
      if (!HX1838toetsKalibreren(i)) {
#if (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
        GA_SERIAL.println(_INPUT_HX1838_KALIBRATIE_TIMEOUT_SERIAL);
#endif
        Screen->Print(_INPUT_HX1838_KALIBRATIE_UITVOEREN, _INPUT_HX1838_KALIBRATIE_TIMEOUT, 2000);
        return;
      }
    }
#if (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
    GA_SERIAL.println(_INPUT_HX1838_GENERIEK_CODES_GEKALIBREERD_KOPIEER_REGEL " UserConfig.h:");
    HX1838toonTabelMetCodes();
#endif
    Screen->Print(_INPUT_HX1838_GENERIEK_CODES_GEKALIBREERD, _INPUT_HX1838_GENERIEK_CODES_ZIE_SERIEEL, 0);
    while (true) { }
  }
#endif

#if !(HX1838_TOETSENINDELING == HX1838_TOETSENINDELING_REMOTE_USER_DEFINED && defined(HX1838_GENERIEK_CODES_KALIBREREN))
  bool Input::HX1838mappingUitUserConfigInladen() {
#if HX1838_TOETSENINDELING == HX1838_TOETSENINDELING_REMOTE_OK_BOVENAAN_17_TOETSEN
    const uint8_t standaardCodes[] = {HX1838_CODE_1, HX1838_CODE_2, HX1838_CODE_3, HX1838_CODE_4, HX1838_CODE_5, HX1838_CODE_6, HX1838_CODE_7, HX1838_CODE_8, HX1838_CODE_9, HX1838_CODE_10, HX1838_CODE_11, HX1838_CODE_12, HX1838_CODE_13, HX1838_CODE_14, HX1838_CODE_15, HX1838_CODE_16, HX1838_CODE_17};
#elif HX1838_TOETSENINDELING == HX1838_TOETSENINDELING_REMOTE_OK_ONDERAAN_17_TOETSEN
    const uint8_t standaardCodes[] = {HX1838_CODE_1, HX1838_CODE_2, HX1838_CODE_3, HX1838_CODE_4, HX1838_CODE_5, HX1838_CODE_6, HX1838_CODE_7, HX1838_CODE_8, HX1838_CODE_9, HX1838_CODE_10, HX1838_CODE_11, HX1838_CODE_12, HX1838_CODE_13, HX1838_CODE_14, HX1838_CODE_15, HX1838_CODE_16, HX1838_CODE_17};
#elif HX1838_TOETSENINDELING == HX1838_TOETSENINDELING_REMOTE_21_TOETSEN_MP3
    const uint8_t standaardCodes[] = {HX1838_CODE_1, HX1838_CODE_2, HX1838_CODE_3, HX1838_CODE_4, HX1838_CODE_5, HX1838_CODE_6, HX1838_CODE_7, HX1838_CODE_8, HX1838_CODE_9, HX1838_CODE_10, HX1838_CODE_11, HX1838_CODE_12, HX1838_CODE_13, HX1838_CODE_14, HX1838_CODE_15, HX1838_CODE_16, HX1838_CODE_17, HX1838_CODE_18, HX1838_CODE_19, HX1838_CODE_20, HX1838_CODE_21};
#elif HX1838_TOETSENINDELING == HX1838_TOETSENINDELING_REMOTE_USER_DEFINED
    static constexpr uint8_t standaardCodes[] = HX1838_GENERIEK_CODES;
    static_assert((sizeof(standaardCodes) / sizeof(standaardCodes[0])) == AANTAL_IR_TOETSEN, "HX1838_GENERIEK_CODES " _INPUT_HX1838_STATIC_ASSERT_MOET_EXACT " HX1838_GENERIEK_AANTAL_TOETSEN " _INPUT_HX1838_STATIC_ASSERT_CODES_BEVATTEN);
    static_assert(HX1838CodesNietNul(standaardCodes), "HX1838_GENERIEK_CODES " _INPUT_HX1838_STATIC_ASSERT_MAG_BIJ " HX1838_BRON_CODES_DEFINE " _INPUT_HX1838_STATIC_ASSERT_GEEN_CODE_WAARDE_NUL);
#endif
    for (byte i = 0; i < AANTAL_IR_TOETSEN; i++) if (standaardCodes[i] == 0UL) return false;
    for (byte i = 0; i < AANTAL_IR_TOETSEN; i++) irCodes[i] = standaardCodes[i];
#if (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
    GA_SERIAL.println(_INPUT_HX1838_MAPPING_CONFIG_GELADEN_SERIAL);
    HX1838toonTabelMetCodes();
#endif
    return true;
  }
#endif

  int Input::HX1838indexUitZoekenVoorSignaal(uint8_t signaalwaarde) {
    for (byte i = 0; i < AANTAL_IR_TOETSEN; i++) if (irCodes[i] == signaalwaarde) return i;
    return -1;
  }

  bool Input::HX1838toetsKalibreren(byte index) {
#if (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
    GA_SERIAL.print(_INPUT_HX1838_DRUK_NU_OP_SERIAL);
    GA_SERIAL.println(IR_KEY_LAYOUT[index].weergavetekst);
#endif
    Screen->Print(_INPUT_HX1838_DRUK_NU_OP, IR_KEY_LAYOUT[index].weergavetekst, 0);

    unsigned long startWachten = millis();

    while ((millis() - startWachten) < HX1838_KALIBRATIE_TIMEOUT_MS) {
#if HX1838_USE_TINYIRRECEIVER_INSTEAD_OF_IRREMOTE
      if (TinyIRReceiverData.justWritten) {
        TinyIRReceiverData.justWritten = false;

        if (!(TinyIRReceiverData.Flags & IRDATA_FLAGS_IS_REPEAT)) {
          irCodes[index] = TinyIRReceiverData.Command;
          delay(HX1838_KALIBRATIE_TOETS_PAUZE_MS);
          return true;
        }
      }
#else
      if (IrReceiver.decode()) {
        if (!(IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT)) {
          irCodes[index] = IrReceiver.decodedIRData.command;
          IrReceiver.resume();
          delay(HX1838_KALIBRATIE_TOETS_PAUZE_MS);
          return true;
        }

        IrReceiver.resume();
      }
#endif
    }

    return false;
  }

#if HX1838_BRON_CODES == HX1838_BRON_CODES_EEPROM_ALTIJD || HX1838_BRON_CODES == HX1838_BRON_CODES_EEPROM_WANNEER_GEEN_DEFINE
  #define EEPROM_ADRES_MAGIC    0
  #define EEPROM_MAGIC_WAARDE   0xAC
  #define EEPROM_ADRES_VERSIE   1
  #define EEPROM_INPUT_VERSIE   3
  #define EEPROM_ADRES_TOETSENINDELING 2
  #define EEPROM_ADRES_CODES    3
  #define EEPROM_BENODIGDE_GROOTTE (EEPROM_ADRES_CODES + (AANTAL_IR_TOETSEN * sizeof(uint8_t)))

  bool Input::EEPROMopslagBeginnen() {
    #if BOARD_VERSION == BOARD_ESP32_D1_UNO_R32 || BOARD_VERSION == BOARD_ESP32S3_ARDI32
      return EEPROM.begin(EEPROM_BENODIGDE_GROOTTE);
    #elif BOARD_VERSION == BOARD_RP2040_CYTRON_MAKER_UNO
      EEPROM.begin(EEPROM_BENODIGDE_GROOTTE);
      return true;
    #else
      return true;
    #endif
  }

  void Input::EEPROMopslagBevestigen() {
    #if BOARD_VERSION == BOARD_ESP32_D1_UNO_R32 || BOARD_VERSION == BOARD_ESP32S3_ARDI32 || BOARD_VERSION == BOARD_RP2040_CYTRON_MAKER_UNO
      EEPROM.commit();
    #endif
  }

  void Input::HX1838kalibratieOpslaan() {
    if (!EEPROMopslagBeginnen()) return;
    EEPROM.write(EEPROM_ADRES_MAGIC, EEPROM_MAGIC_WAARDE);
    EEPROM.write(EEPROM_ADRES_VERSIE, EEPROM_INPUT_VERSIE);
    EEPROM.write(EEPROM_ADRES_TOETSENINDELING, HX1838_TOETSENINDELING);
    for (byte i = 0; i < AANTAL_IR_TOETSEN; i++) EEPROM.put(EEPROM_ADRES_CODES + (i * sizeof(uint8_t)), irCodes[i]);
    EEPROMopslagBevestigen();
#if (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
    GA_SERIAL.println(_INPUT_HX1838_KALIBRATIE_OPGESLAGEN_SERIAL);
    HX1838toonTabelMetCodes();
#endif
    Screen->Print(_INPUT_HX1838_KALIBRATIE_UITVOEREN, _INPUT_HX1838_KALIBRATIE_OPGESLAGEN, 2000);
  }

  bool Input::HX1838kalibratieLaden() {
    if (!EEPROMopslagBeginnen()) {
#ifdef DEBUG
      GA_DEBUG_PRINT("HX1838kalibratieLaden(): EEPROMopslagBeginnen(): ");
      GA_DEBUG_PRINTLN(_INPUT_HX1838_DEBUG_FAALDE);
#endif
      return false;
    }
    if (EEPROM.read(EEPROM_ADRES_MAGIC) != EEPROM_MAGIC_WAARDE) {
#ifdef DEBUG
      GA_DEBUG_PRINT("HX1838kalibratieLaden(): ");
      GA_DEBUG_PRINTLN(_INPUT_HX1838_DEBUG_MAGIC_MISMATCH);
#endif
      return false;
    }
    if (EEPROM.read(EEPROM_ADRES_VERSIE) != EEPROM_INPUT_VERSIE) {
#ifdef DEBUG
      GA_DEBUG_PRINT("HX1838kalibratieLaden(): ");
      GA_DEBUG_PRINTLN(_INPUT_HX1838_DEBUG_VERSIE_MISMATCH);
#endif
      return false;
    }
    if (EEPROM.read(EEPROM_ADRES_TOETSENINDELING) != HX1838_TOETSENINDELING) {
#ifdef DEBUG
      GA_DEBUG_PRINT("HX1838kalibratieLaden(): ");
      GA_DEBUG_PRINTLN(_INPUT_HX1838_DEBUG_TOETSENINDELING_MISMATCH);
#endif
      return false;
    }
    for (byte i = 0; i < AANTAL_IR_TOETSEN; i++) EEPROM.get(EEPROM_ADRES_CODES + (i * sizeof(uint8_t)), irCodes[i]);
#if (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
    GA_SERIAL.println(_INPUT_HX1838_MAPPING_EEPROM_GELADEN_SERIAL);
    HX1838toonTabelMetCodes();
#endif
    return true;
  }

  bool Input::HX1838kalibratieVerifieren() {
#if (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
    GA_SERIAL.println(_INPUT_HX1838_KALIBRATIE_KLAAR_CONTROLE_SERIAL);
#endif
    Screen->Print(_INPUT_HX1838_CONTROLE, _INPUT_HX1838_DRUK_OP_ELKE_TOETS_VOOR_CONTROLE, 0);

    bool geverifieerd[AANTAL_IR_TOETSEN];
    for (byte i = 0; i < AANTAL_IR_TOETSEN; i++) geverifieerd[i] = false;
    byte aantalGeverifieerd = 0;
    unsigned long laatsteHerkenning = millis();

    while (aantalGeverifieerd < AANTAL_IR_TOETSEN) {
      if ((millis() - laatsteHerkenning) >= HX1838_KALIBRATIE_TIMEOUT_MS) return false;
#if HX1838_USE_TINYIRRECEIVER_INSTEAD_OF_IRREMOTE
      if (TinyIRReceiverData.justWritten) {
        TinyIRReceiverData.justWritten = false;
        if (!(TinyIRReceiverData.Flags & IRDATA_FLAGS_IS_REPEAT)) {
          int index = HX1838indexUitZoekenVoorSignaal(TinyIRReceiverData.Command);
#else
      if (IrReceiver.decode()) {
        if (!(IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT)) {
          int index = HX1838indexUitZoekenVoorSignaal(IrReceiver.decodedIRData.command);
#endif
          if (index >= 0 && !geverifieerd[index]) {
            geverifieerd[index] = true;
            aantalGeverifieerd++;
            laatsteHerkenning = millis();
#if (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
            GA_SERIAL.print(_INPUT_HX1838_TOETS_HERKEND_SERIAL);
            GA_SERIAL.println(IR_KEY_LAYOUT[index].weergavetekst);
#endif
            Screen->Print(IR_KEY_LAYOUT[index].weergavetekst, _INPUT_HX1838_TOETS_HERKEND, 500);
          }
        }

#if HX1838_USE_TINYIRRECEIVER_INSTEAD_OF_IRREMOTE == 0
        IrReceiver.resume();
#endif
      }
    }

    Screen->Print(_INPUT_HX1838_CONTROLE_GESLAAGD, "", 2000);
    HX1838kalibratieOpslaan();
    return true;
  }

  void Input::HX1838kalibratieUitvoeren() {
#if (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
    GA_SERIAL.println(_INPUT_HX1838_KALIBRATIE_GESTART_SERIAL);
#endif
    for (byte i = 0; i < AANTAL_IR_TOETSEN; i++) {
#ifdef TRACE
      GA_DEBUG_PRINT("HX1838kalibratieUitvoeren(): ");
      GA_DEBUG_PRINT(_INPUT_HX1838_DEBUG_TOETS);
      GA_DEBUG_PRINT(" ");
      GA_DEBUG_PRINT(i + 1);
      GA_DEBUG_PRINT("/");
      GA_DEBUG_PRINTLN(AANTAL_IR_TOETSEN);
#endif
      if (!HX1838toetsKalibreren(i)) {
#if (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
        GA_SERIAL.println(_INPUT_HX1838_KALIBRATIE_TIMEOUT_SERIAL);
#endif
        Screen->Print(_INPUT_HX1838_KALIBRATIE_UITVOEREN, _INPUT_HX1838_KALIBRATIE_TIMEOUT, 2000);
        return;
      }
    }
#ifdef DEBUG
    GA_DEBUG_PRINT("HX1838kalibratieUitvoeren(): ");
    GA_DEBUG_PRINTLN(_INPUT_HX1838_DEBUG_ALLE_TOETSEN_GEKALIBREER_START_VERIFICATIE);
#endif
    if (!HX1838kalibratieVerifieren()) {
#if (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
      GA_SERIAL.println(_INPUT_HX1838_KALIBRATIE_TIMEOUT_SERIAL);
#endif
      Screen->Print(_INPUT_HX1838_CONTROLE, _INPUT_HX1838_KALIBRATIE_TIMEOUT, 2000);
    }
  }
#endif

  int Input::HX1838uitLezenToetsAanslag() {
#if HX1838_USE_TINYIRRECEIVER_INSTEAD_OF_IRREMOTE
    if (!TinyIRReceiverData.justWritten) return 0;
    TinyIRReceiverData.justWritten = false;
#else
    if (!IrReceiver.decode()) return 0;
#endif
#ifdef TRACE
    hx1838DecodeTeller++;
#endif
    int positie = 0;
#if HX1838_USE_TINYIRRECEIVER_INSTEAD_OF_IRREMOTE
    if (!(TinyIRReceiverData.Flags & IRDATA_FLAGS_IS_REPEAT)) {
      int index = HX1838indexUitZoekenVoorSignaal(TinyIRReceiverData.Command);
#ifdef TRACE
      GA_DEBUG_PRINT(_INPUT_HX1838_DEBUG_IR_ONTVANGEN);
      GA_DEBUG_PRINT("=0x");
      GA_DEBUG_PRINT(String(TinyIRReceiverData.Command, HEX));

      if (index >= 0) {
        GA_DEBUG_PRINT(" ");
        GA_DEBUG_PRINT(_INPUT_HX1838_DEBUG_VERGELEKEN_MET);
        GA_DEBUG_PRINT(" HX1838_CODE_");
        GA_DEBUG_PRINT(index + 1);
        GA_DEBUG_PRINT("=0x");
        GA_DEBUG_PRINT(String(irCodes[index], HEX));
        GA_DEBUG_PRINT(" (");
        GA_DEBUG_PRINT(_INPUT_HX1838_DEBUG_MATCH);
        GA_DEBUG_PRINT(") #");
      } else {
        GA_DEBUG_PRINT(" (");
        GA_DEBUG_PRINT(_INPUT_HX1838_DEBUG_GEEN_MATCH);
        GA_DEBUG_PRINT(") #");
      }

      GA_DEBUG_PRINTLN(hx1838DecodeTeller);
#endif
      if (index >= 0) positie = index + 1;
    }
#else
    if (!(IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT)) {
      int index = HX1838indexUitZoekenVoorSignaal(IrReceiver.decodedIRData.command);
#ifdef TRACE
      GA_DEBUG_PRINT(_INPUT_HX1838_DEBUG_IR_ONTVANGEN);
      GA_DEBUG_PRINT("=0x");
      GA_DEBUG_PRINT(String(IrReceiver.decodedIRData.command, HEX));

      if (index >= 0) {
        GA_DEBUG_PRINT(" ");
        GA_DEBUG_PRINT(_INPUT_HX1838_DEBUG_VERGELEKEN_MET);
        GA_DEBUG_PRINT(" HX1838_CODE_");
        GA_DEBUG_PRINT(index + 1);
        GA_DEBUG_PRINT("=0x");
        GA_DEBUG_PRINT(String(irCodes[index], HEX));
        GA_DEBUG_PRINT(" (");
        GA_DEBUG_PRINT(_INPUT_HX1838_DEBUG_MATCH);
        GA_DEBUG_PRINT(") #");
      } else {
        GA_DEBUG_PRINT(" (");
        GA_DEBUG_PRINT(_INPUT_HX1838_DEBUG_GEEN_MATCH);
        GA_DEBUG_PRINT(") #");
      }

      GA_DEBUG_PRINTLN(hx1838DecodeTeller);
#endif
      if (index >= 0) positie = index + 1;
    }
    IrReceiver.resume();
#endif
    return positie;
  }
#endif

// ============================================================================
// PUBLIEKE API
// ============================================================================
// Elke stap gaat via Input naar zijn kanalen, zelfde werkwijze als Screen: faalt één kanaal, dan valt enkel dat kanaal weg (afmelden() + nullptr); de overige, geselecteerde kanalen worden gewoon geactiveerd.
// Input faalt pas volledig wanneer geen enkel geselecteerd kanaal overleeft.
bool Input::aanmelden() {
#ifdef TRACE
  GA_SERIAL.println("TRACE: Input::aanmelden()");
#endif
  if (aangemeld) return true;
  if ((this->typesActief & (INPUT_KANAAL_CONFIG)) != this->typesActief) return false;
  if (!GedeeldeBusNode::aanmelden()) return false;

#if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_DIGITAL)
  if ((this->typesActief & INPUT_TYPE_DIGITAL) && gedeeldeBusInputDigital == nullptr) {
  #if KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_DIRECT_1x4
    gedeeldeBusInputDigitalPinnen[0] = static_cast<uint8_t>(INPUT_KEYPAD_PIN_K1);
    gedeeldeBusInputDigitalPinnen[1] = static_cast<uint8_t>(INPUT_KEYPAD_PIN_K2);
    gedeeldeBusInputDigitalPinnen[2] = static_cast<uint8_t>(INPUT_KEYPAD_PIN_K3);
    gedeeldeBusInputDigitalPinnen[3] = static_cast<uint8_t>(INPUT_KEYPAD_PIN_K4);
  #elif KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_MATRIX_2x2
    gedeeldeBusInputDigitalPinnen[0] = static_cast<uint8_t>(INPUT_KEYPAD_PIN_L1);
    gedeeldeBusInputDigitalPinnen[1] = static_cast<uint8_t>(INPUT_KEYPAD_PIN_L2);
    gedeeldeBusInputDigitalPinnen[2] = static_cast<uint8_t>(INPUT_KEYPAD_PIN_R1);
    gedeeldeBusInputDigitalPinnen[3] = static_cast<uint8_t>(INPUT_KEYPAD_PIN_R2);
  #elif KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4 || KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_DIRECT_4x1
    gedeeldeBusInputDigitalPinnen[0] = static_cast<uint8_t>(INPUT_KEYPAD_PIN_1);
    gedeeldeBusInputDigitalPinnen[1] = static_cast<uint8_t>(INPUT_KEYPAD_PIN_2);
    gedeeldeBusInputDigitalPinnen[2] = static_cast<uint8_t>(INPUT_KEYPAD_PIN_3);
    gedeeldeBusInputDigitalPinnen[3] = static_cast<uint8_t>(INPUT_KEYPAD_PIN_4);
  #elif KEYPAD_TYPE == KEYPAD_TYPE_TOUCH_TTP224_DIRECT_1x4
    gedeeldeBusInputDigitalPinnen[0] = static_cast<uint8_t>(INPUT_KEYPAD_PIN_OUT1);
    gedeeldeBusInputDigitalPinnen[1] = static_cast<uint8_t>(INPUT_KEYPAD_PIN_OUT2);
    gedeeldeBusInputDigitalPinnen[2] = static_cast<uint8_t>(INPUT_KEYPAD_PIN_OUT3);
    gedeeldeBusInputDigitalPinnen[3] = static_cast<uint8_t>(INPUT_KEYPAD_PIN_OUT4);
  #endif

    InputDigital* kanaal = new InputDigital(this, BezettingPinnen{ nullptr, 0, gedeeldeBusInputDigitalPinnen, 4 }, GedeeldeBusComponent::INPUT_DIGITAL, HardwareResourceToegang::GEDEELD);
    kanaal->componentCreated = true;

    if (kanaal->aanmelden()) {
      gedeeldeBusInputDigital = kanaal;
    } else {
      delete kanaal;
    }
  }
#endif

#if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_PCF8574)
  if ((this->typesActief & INPUT_TYPE_PCF8574) && gedeeldeBusInputPCF8574 == nullptr) {
    InputPCF8574* kanaal = new InputPCF8574(this, GedeeldeBusComponent::INPUT_PCF8574, I2C_ADDRESS_PCF8574, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, HardwareResourceToegang::GEDEELD);
    kanaal->componentCreated = true;

    if (kanaal->aanmelden()) {
      gedeeldeBusInputPCF8574 = kanaal;
    } else {
      delete kanaal;
    }
  }
#endif

#if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_HX1838)
  if ((this->typesActief & INPUT_TYPE_HX1838) && gedeeldeBusInputHX1838 == nullptr) {
    gedeeldeBusInputHX1838Pinnen[0] = static_cast<uint8_t>(ArduinoUnoShieldPinOmzettenNaarHardwareResourcePin(HX1838_ONTVANGER_PIN));

    InputHX1838* kanaal = new InputHX1838(this, BezettingPinnen{ nullptr, 0, gedeeldeBusInputHX1838Pinnen, 1 }, GedeeldeBusComponent::INPUT_HX1838, HardwareResourceToegang::GEDEELD);
    kanaal->componentCreated = true;

    if (kanaal->aanmelden()) {
      gedeeldeBusInputHX1838 = kanaal;
    } else {
      delete kanaal;
    }
  }
#endif

  if (this->typesActief != INPUT_TYPE_NONE
#if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_DIGITAL)
      && gedeeldeBusInputDigital == nullptr
#endif
#if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_PCF8574)
      && gedeeldeBusInputPCF8574 == nullptr
#endif
#if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_HX1838)
      && gedeeldeBusInputHX1838 == nullptr
#endif
  ) return false;

  return true;
}

bool Input::controleren() {
#ifdef TRACE
  GA_SERIAL.println("TRACE: Input::controleren()");
#endif
  if (gecontroleerd) return true;
  if (!GedeeldeBusNode::controleren()) return false;

#if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_DIGITAL)
#ifdef TRACE
  GA_SERIAL.println("TRACE: Input::controleren(): DIGITAL");
#endif
  if (gedeeldeBusInputDigital != nullptr && !gedeeldeBusInputDigital->controleren()) {
    gedeeldeBusInputDigital->afmelden();
    gedeeldeBusInputDigital = nullptr;
  }
#endif
#if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_PCF8574)
#ifdef TRACE
  GA_SERIAL.println("TRACE: Input::controleren(): PCF8574 I2C");
#endif
  if (gedeeldeBusInputPCF8574 != nullptr && !gedeeldeBusInputPCF8574->controleren()) {
    gedeeldeBusInputPCF8574->afmelden();
    gedeeldeBusInputPCF8574 = nullptr;
  }
#endif
#if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_HX1838)
#ifdef TRACE
  GA_SERIAL.println("TRACE: Input::controleren(): HX1838");
#endif
  if (gedeeldeBusInputHX1838 != nullptr && !gedeeldeBusInputHX1838->controleren()) {
    gedeeldeBusInputHX1838->afmelden();
    gedeeldeBusInputHX1838 = nullptr;
  }
#endif

  if (this->typesActief != INPUT_TYPE_NONE
#if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_DIGITAL)
      && gedeeldeBusInputDigital == nullptr
#endif
#if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_PCF8574)
      && gedeeldeBusInputPCF8574 == nullptr
#endif
#if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_HX1838)
      && gedeeldeBusInputHX1838 == nullptr
#endif
  ) return false;

  return true;
}

bool Input::inpluggen() {
#ifdef TRACE
  GA_SERIAL.println("TRACE: Input::inpluggen()");
#endif
  if (ingeplugd) return true;
  if (!GedeeldeBusNode::inpluggen()) return false;

#if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_DIGITAL)
#ifdef TRACE
  GA_SERIAL.println("TRACE: Input::inpluggen(): DIGITAL");
#endif
  if (gedeeldeBusInputDigital != nullptr && !gedeeldeBusInputDigital->inpluggen()) {
    gedeeldeBusInputDigital->afmelden();
    gedeeldeBusInputDigital = nullptr;
  }
#endif
#if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_PCF8574)
#ifdef TRACE
  GA_SERIAL.println("TRACE: Input::inpluggen(): PCF8574 I2C");
#endif
  if (gedeeldeBusInputPCF8574 != nullptr && !gedeeldeBusInputPCF8574->inpluggen()) {
    gedeeldeBusInputPCF8574->afmelden();
    gedeeldeBusInputPCF8574 = nullptr;
  }

  if (gedeeldeBusInputPCF8574 != nullptr) {
    if (!gedeeldeBusInputPCF8574->begin(0xFF)) {
      PCF8574OnbereikbaarMelden();
      gedeeldeBusInputPCF8574->afmelden();
      gedeeldeBusInputPCF8574 = nullptr;
    } else {
      pcf8574Bereikbaar = true;
      pcf8574FoutmeldingWeergegeven = false;
    }
  }
#endif
#if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_HX1838)
  if (gedeeldeBusInputHX1838 != nullptr && (static_cast<HardwareResourcePin>(gedeeldeBusInputHX1838Pinnen[0]) == HardwareResourcePin::NONE
    || NativeArduinoPinVan(static_cast<HardwareResourcePin>(gedeeldeBusInputHX1838Pinnen[0])) == static_cast<uint8_t>(HardwareResourcePin::NONE))) {
    Screen->Print(_CRITICAL_IN202, "", 2000);
    gedeeldeBusInputHX1838->afmelden();
    gedeeldeBusInputHX1838 = nullptr;
  } else if (gedeeldeBusInputHX1838 != nullptr && !gedeeldeBusInputHX1838->inpluggen()) {
    gedeeldeBusInputHX1838->afmelden();
    gedeeldeBusInputHX1838 = nullptr;
  }

  if (gedeeldeBusInputHX1838 != nullptr) {
#ifdef DEBUG
    GA_DEBUG_PRINT("Input::inpluggen(): ");
    GA_DEBUG_PRINTLN(_INPUT_HX1838_DEBUG_VOOR_INIT);
#endif
#if HX1838_USE_TINYIRRECEIVER_INSTEAD_OF_IRREMOTE
    if (!initPCIInterruptForTinyReceiver()) {
#if (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
      GA_SERIAL.println(_INPUT_HX1838_INTERRUPT_KOPPELING_MISLUKT_SERIAL);
#endif
      Screen->Print(_INPUT_HX1838_INTERRUPT_KOPPELING_MISLUKT, "", 2000);
      gedeeldeBusInputHX1838->afmelden();
      gedeeldeBusInputHX1838 = nullptr;
    }
#else
    IrReceiver.begin(HX1838_ONTVANGER_PIN, DISABLE_LED_FEEDBACK);

    if (!IrReceiver.isIdle()) {
      delay(5);

      if (!IrReceiver.isIdle()) {
        Screen->Print(_CRITICAL_IN305, "", 2000);
        gedeeldeBusInputHX1838->afmelden();
        gedeeldeBusInputHX1838 = nullptr;
      }
    }
#endif
#ifdef DEBUG
    if (gedeeldeBusInputHX1838 != nullptr) {
      GA_DEBUG_PRINT("Input::inpluggen(): ");
      GA_DEBUG_PRINTLN(_INPUT_HX1838_DEBUG_NA_INIT);
    }
#endif
  }
#endif

  if (this->typesActief != INPUT_TYPE_NONE
#if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_DIGITAL)
      && gedeeldeBusInputDigital == nullptr
#endif
#if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_PCF8574)
      && gedeeldeBusInputPCF8574 == nullptr
#endif
#if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_HX1838)
      && gedeeldeBusInputHX1838 == nullptr
#endif
  ) return false;

  return true;
}

bool Input::Activeren() {
#ifdef TRACE
  GA_SERIAL.println("TRACE: Input::Activeren()");
#endif
#if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_DIGITAL)
#ifdef TRACE
  GA_SERIAL.println("TRACE: Input::Activeren(): DIGITAL");
#endif
  if (gedeeldeBusInputDigital != nullptr && !gedeeldeBusInputDigital->activeren()) {
    gedeeldeBusInputDigital->afmelden();
    gedeeldeBusInputDigital = nullptr;
  }
#endif
#if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_PCF8574)
#ifdef TRACE
  GA_SERIAL.println("TRACE: Input::Activeren(): PCF8574 I2C");
#endif
  if (gedeeldeBusInputPCF8574 != nullptr && !gedeeldeBusInputPCF8574->activeren()) {
    gedeeldeBusInputPCF8574->afmelden();
    gedeeldeBusInputPCF8574 = nullptr;
  }
#endif
#if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_HX1838)
#ifdef TRACE
  GA_SERIAL.println("TRACE: Input::Activeren(): HX1838");
#endif
/*
  if (gedeeldeBusInputHX1838 != nullptr && digitalRead(HX1838_ONTVANGER_PIN) == LOW) {
    delay(5);

    if (digitalRead(HX1838_ONTVANGER_PIN) == LOW) {
      Screen->Print(_CRITICAL_IN302, "", 2000);
      gedeeldeBusInputHX1838->afmelden();
      gedeeldeBusInputHX1838 = nullptr;
    }
  }

#ifdef DEBUG
  if (gedeeldeBusInputHX1838 != nullptr) { GA_DEBUG_PRINT("Input::Activeren(): "); GA_DEBUG_PRINTLN(_INPUT_HX1838_DEBUG_VOOR_INIT); }
#endif

  #if HX1838_USE_TINYIRRECEIVER_INSTEAD_OF_IRREMOTE
    #if defined(USE_ATTACH_INTERRUPT) && defined(NOT_AN_INTERRUPT)
      if (gedeeldeBusInputHX1838 != nullptr && digitalPinToInterrupt(HX1838_ONTVANGER_PIN) == NOT_AN_INTERRUPT) {
        Screen->Print(_CRITICAL_IN304, "", 2000);
        gedeeldeBusInputHX1838->afmelden();
        gedeeldeBusInputHX1838 = nullptr;
      }
    #endif
    if (gedeeldeBusInputHX1838 != nullptr && !initPCIInterruptForTinyReceiver()) {
      Screen->Print(_CRITICAL_IN303, "", 2000);
      gedeeldeBusInputHX1838->afmelden();
      gedeeldeBusInputHX1838 = nullptr;
    }
  #else
    if (gedeeldeBusInputHX1838 != nullptr) {
      IrReceiver.begin(HX1838_ONTVANGER_PIN, DISABLE_LED_FEEDBACK);

      if (!IrReceiver.isIdle()) {
        delay(5);

        if (!IrReceiver.isIdle()) {
          Screen->Print(_CRITICAL_IN305, "", 2000);
          gedeeldeBusInputHX1838->afmelden();
          gedeeldeBusInputHX1838 = nullptr;
        }
      }
    }
  #endif

#ifdef DEBUG
  if (gedeeldeBusInputHX1838 != nullptr) { GA_DEBUG_PRINT("Input::Activeren(): "); GA_DEBUG_PRINTLN(_INPUT_HX1838_DEBUG_NA_INIT); }
#endif
*/

  if (gedeeldeBusInputHX1838 != nullptr && !gedeeldeBusInputHX1838->activeren()) {
    gedeeldeBusInputHX1838->afmelden();
    gedeeldeBusInputHX1838 = nullptr;
  }
#endif

  if (this->typesActief != INPUT_TYPE_NONE
#if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_DIGITAL)
      && gedeeldeBusInputDigital == nullptr
#endif
#if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_PCF8574)
      && gedeeldeBusInputPCF8574 == nullptr
#endif
#if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_HX1838)
      && gedeeldeBusInputHX1838 == nullptr
#endif
  ) return false;

#if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_DIGITAL)
  if (gedeeldeBusInputDigital != nullptr) {
    #if KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_MATRIX_2x2
      pinMode(NativeArduinoPinVan(INPUT_KEYPAD_PIN_L1), OUTPUT);
      pinMode(NativeArduinoPinVan(INPUT_KEYPAD_PIN_L2), OUTPUT);
      digitalWrite(NativeArduinoPinVan(INPUT_KEYPAD_PIN_L1), HIGH);
      digitalWrite(NativeArduinoPinVan(INPUT_KEYPAD_PIN_L2), HIGH);
      pinMode(NativeArduinoPinVan(INPUT_KEYPAD_PIN_R1), INPUT_PULLUP);
      pinMode(NativeArduinoPinVan(INPUT_KEYPAD_PIN_R2), INPUT_PULLUP);
    #elif KEYPAD_TYPE == KEYPAD_TYPE_TOUCH_TTP224_DIRECT_1x4
      pinMode(NativeArduinoPinVan(INPUT_KEYPAD_PIN_OUT1), INPUT);
      pinMode(NativeArduinoPinVan(INPUT_KEYPAD_PIN_OUT2), INPUT);
      pinMode(NativeArduinoPinVan(INPUT_KEYPAD_PIN_OUT3), INPUT);
      pinMode(NativeArduinoPinVan(INPUT_KEYPAD_PIN_OUT4), INPUT);
    #elif KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_DIRECT_1x4
      pinMode(NativeArduinoPinVan(INPUT_KEYPAD_PIN_K1), INPUT_PULLUP);
      pinMode(NativeArduinoPinVan(INPUT_KEYPAD_PIN_K2), INPUT_PULLUP);
      pinMode(NativeArduinoPinVan(INPUT_KEYPAD_PIN_K3), INPUT_PULLUP);
      pinMode(NativeArduinoPinVan(INPUT_KEYPAD_PIN_K4), INPUT_PULLUP);
    #else
      pinMode(NativeArduinoPinVan(INPUT_KEYPAD_PIN_1), INPUT_PULLUP);
      pinMode(NativeArduinoPinVan(INPUT_KEYPAD_PIN_2), INPUT_PULLUP);
      pinMode(NativeArduinoPinVan(INPUT_KEYPAD_PIN_3), INPUT_PULLUP);
      pinMode(NativeArduinoPinVan(INPUT_KEYPAD_PIN_4), INPUT_PULLUP);
    #endif
  }
#endif

#if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_HX1838)
  if (gedeeldeBusInputHX1838 != nullptr) {
    #if HX1838_BRON_CODES == HX1838_BRON_CODES_DEFINE
      #if HX1838_TOETSENINDELING == HX1838_TOETSENINDELING_REMOTE_USER_DEFINED && defined(HX1838_GENERIEK_CODES_KALIBREREN)
        HX1838GeneriekCodesKalibreren();
      #else
        if (HX1838mappingUitUserConfigInladen()) {
          Screen->Print(_INPUT_HX1838_MAPPING_GELADEN, "", 2000);
        } else {
#if (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
          GA_SERIAL.println(_INPUT_HX1838_MAPPING_CONFIG_ONVOLLEDIG_SERIAL);
#endif
          Screen->Print(_INPUT_HX1838_MAPPING_CONFIG_ONVOLLEDIG, "", 2000);
        }
      #endif
    #elif HX1838_BRON_CODES == HX1838_BRON_CODES_EEPROM_ALTIJD
      if (HX1838kalibratieLaden()) {
        Screen->Print(_INPUT_HX1838_MAPPING_EEPROM_GELADEN, "", 2000);
      } else {
#if (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
        GA_SERIAL.println(_INPUT_HX1838_GEEN_GELDIGE_KALIBRATIE_SERIAL);
#endif
        HX1838kalibratieUitvoeren();
      }
    #endif
  }
#endif

#ifdef INPUT_KANAAL_OPVRAGEN_HUIDIGE_TOETSAANSLAG_UITGEBREID
  laatsteInvoerTijdstipVoorTimeout = millis();
  #if ((INPUT_KANAAL_CONFIG) & (INPUT_TYPE_DIGITAL | INPUT_TYPE_PCF8574))
    vorigeRauweKeypadPositie = 0;
    stabieleKeypadPositie = 0;
    keypadWijzigingSinds = millis();
    laatstePositieVoorLoslatenDetectie = 0;
    keypadDrukBeginTijd = 0;
    langIndrukkenAlGemeldVoorHuidigeDruk = false;
  #endif
#endif

  return true;
}

bool Input::afmelden() {
#ifdef TRACE
  GA_SERIAL.println("TRACE: Input::afmelden()");
#endif
#if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_HX1838)
  if (gedeeldeBusInputHX1838 != nullptr) {
    if (!gedeeldeBusInputHX1838->afmelden()) return false;
    gedeeldeBusInputHX1838 = nullptr;
  }
#endif
#if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_PCF8574)
  if (gedeeldeBusInputPCF8574 != nullptr) {
    if (!gedeeldeBusInputPCF8574->afmelden()) return false;
    gedeeldeBusInputPCF8574 = nullptr;
  }
#endif
#if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_DIGITAL)
  if (gedeeldeBusInputDigital != nullptr) {
    if (!gedeeldeBusInputDigital->afmelden()) return false;
    gedeeldeBusInputDigital = nullptr;
  }
#endif
  if (::Input == this) ::Input = nullptr;
  return GedeeldeBusNode::afmelden();
}


InputResultaat Input::OpvragenHuidigeToetsAanslagIntern(bool wachten, LangIndrukkenDrempelOpzoekerFunctie drempelOpzoeker) {
#ifdef TRACE
  GA_SERIAL.println("TRACE: Input::OpvragenHuidigeToetsAanslagIntern()");
#endif
  InputResultaat resultaat = {InputKanaal::NONE, 0, nullptr};

#if (INPUT_KANAAL_CONFIG) == INPUT_TYPE_NONE
  return resultaat;
#endif

#if ((INPUT_KANAAL_CONFIG) & (INPUT_TYPE_DIGITAL | INPUT_TYPE_PCF8574))
  #if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_DIGITAL)
  if (gedeeldeBusInputDigital != nullptr && gedeeldeBusInputDigital->actief && wachten) {
  #elif ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_PCF8574)
  if (gedeeldeBusInputPCF8574 != nullptr && gedeeldeBusInputPCF8574->actief && wachten) {
  #endif
    while (KeypadUitLezenRuweData() != 0) delay(1);
    vorigeRauweKeypadPositie = 0;
    stabieleKeypadPositie = 0;
    keypadWijzigingSinds = millis();
  }
#endif

  while (true) {
    #if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_DIGITAL)
      if (gedeeldeBusInputDigital != nullptr && gedeeldeBusInputDigital->actief) {
      #ifdef INPUT_KANAAL_OPVRAGEN_HUIDIGE_TOETSAANSLAG_UITGEBREID
        int positieKeypad = KeypadUitLezenToetsAanslag();
      #else
        int positieKeypad = wachten ? KeypadUitLezenToetsAanslag() : KeypadUitLezenRuweData();
      #endif

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
          unsigned long langIndrukkenDrempelMs = drempelOpzoeker(opschriftVoorDrempel);

          if (langIndrukkenDrempelMs > 0) {
            int langIndrukkenPositie = KeypadUitLezenLangIngedruktePositie(langIndrukkenDrempelMs);

            if (langIndrukkenPositie > 0) {
              laatsteInvoerTijdstipVoorTimeout = millis();
              return {InputKanaal::DIGITAL, langIndrukkenPositie, KEY_LAYOUT[langIndrukkenPositie - 1].opschrift, InputGebeurtenis::LANG_INDRUKKEN};
            }
          }
        }
      }
      #endif
      }
      
    #elif ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_PCF8574)
      if (gedeeldeBusInputPCF8574 != nullptr && gedeeldeBusInputPCF8574->actief) {
      #ifdef INPUT_KANAAL_OPVRAGEN_HUIDIGE_TOETSAANSLAG_UITGEBREID
        int positieKeypad = KeypadUitLezenToetsAanslag();
      #else
        int positieKeypad = wachten ? KeypadUitLezenToetsAanslag() : KeypadUitLezenRuweData();
      #endif

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

#ifdef TRACE
        GA_SERIAL.println("TRACE: Input kanaal PCF8574 toets gevonden");
#endif
#ifdef DEBUG
        GA_DEBUG_PRINT("DEBUG: PCF8574 positie=");
        GA_DEBUG_PRINTLN(positieKeypad);
#endif
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
          unsigned long langIndrukkenDrempelMs = drempelOpzoeker(opschriftVoorDrempel);

          if (langIndrukkenDrempelMs > 0) {
            int langIndrukkenPositie = KeypadUitLezenLangIngedruktePositie(langIndrukkenDrempelMs);

            if (langIndrukkenPositie > 0) {
              laatsteInvoerTijdstipVoorTimeout = millis();
              return {InputKanaal::PCF8574, langIndrukkenPositie, KEY_LAYOUT[langIndrukkenPositie - 1].opschrift, InputGebeurtenis::LANG_INDRUKKEN};
            }
          }
        }
      }
      #endif
    }
    #endif

    #if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_HX1838)
    if (gedeeldeBusInputHX1838 != nullptr && gedeeldeBusInputHX1838->actief) {
      int positieIR = HX1838uitLezenToetsAanslag();
      if (positieIR > 0 && positieIR <= AANTAL_IR_TOETSEN) {
        #ifdef INPUT_KANAAL_OPVRAGEN_HUIDIGE_TOETSAANSLAG_UITGEBREID
          laatsteInvoerTijdstipVoorTimeout = millis();
        #endif
#ifdef TRACE
        GA_SERIAL.println("TRACE: Input kanaal HX1838 toets gevonden");
#endif
#ifdef DEBUG
        GA_DEBUG_PRINT("DEBUG: HX1838 positie=");
        GA_DEBUG_PRINTLN(positieIR);
#endif
        return {InputKanaal::HX1838, positieIR, IR_KEY_LAYOUT[positieIR - 1].opschrift};
      }
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

InputResultaat Input::OpvragenHuidigeToetsAanslag(bool wachten) {
  return OpvragenHuidigeToetsAanslagIntern(wachten, _StandaardLangIndrukkenDrempelOpzoeker);
}

InputResultaat Input::OpvragenHuidigeToetsAanslag(bool wachten, LangIndrukkenDrempelOpzoekerFunctie drempelOpzoeker) {
  return OpvragenHuidigeToetsAanslagIntern(wachten, drempelOpzoeker);
}

InputResultaten Input::OpvragenHuidigeToetsAanslagen(bool wachten, byte aantalSimultaan) {
  InputResultaten resultaten = { StatusOpvragenToetsAanslagen::GEEN, 0, {{InputKanaal::NONE, 0, nullptr}} };

  if (aantalSimultaan != 1) {
    resultaten.status = StatusOpvragenToetsAanslagen::NIET_GEIMPLEMENTEERD;
    return resultaten;
  }

  InputResultaat resultaat = OpvragenHuidigeToetsAanslag(wachten);
#ifdef INPUT_KANAAL_OPVRAGEN_HUIDIGE_TOETSAANSLAG_UITGEBREID
  if (resultaat.inputKanaal == InputKanaal::NONE && resultaat.gebeurtenis == InputGebeurtenis::TIMEOUT_GEEN_INVOER) {
    resultaten.status = StatusOpvragenToetsAanslagen::GELDIG;
    resultaten.aantalToetsAanslagen = 1;
    resultaten.toetsAanslagen[0] = resultaat;
    return resultaten;
  }
#endif
  if (resultaat.inputKanaal == InputKanaal::NONE) return resultaten;

  resultaten.status = StatusOpvragenToetsAanslagen::GELDIG;
  resultaten.aantalToetsAanslagen = 1;
  resultaten.toetsAanslagen[0] = resultaat;
  return resultaten;
}

