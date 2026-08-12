#ifndef INPUT_H
#define INPUT_H

#include <Arduino.h>

#include "InputTypes.h"

#include "../../Configuratie/SystemConfig.h"

#if (INPUT_KANAAL_CONFIG & ~(INPUT_TYPE_DIGITAL | INPUT_TYPE_PCF8574 | INPUT_TYPE_HX1838))
  #error INPUT_KANAAL_CONFIG bevat een onbekend invoertype.
#endif

#if INPUT_KANAAL_CONFIG != INPUT_TYPE_NONE && INPUT_KANAAL_CONFIG != INPUT_TYPE_DIGITAL && INPUT_KANAAL_CONFIG != INPUT_TYPE_PCF8574 && INPUT_KANAAL_CONFIG != INPUT_TYPE_HX1838 && INPUT_KANAAL_CONFIG != (INPUT_TYPE_PCF8574 | INPUT_TYPE_HX1838)
  #error Geldige INPUT_KANAAL_CONFIG: INPUT_TYPE_NONE, INPUT_TYPE_DIGITAL, INPUT_TYPE_PCF8574, INPUT_TYPE_HX1838 of INPUT_TYPE_PCF8574 | INPUT_TYPE_HX1838.
#endif

#if defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_MATRIX_4x5
  #error KEYPAD_TYPE_MEMBRAAN_MATRIX_4x5 gebruikt negen signaallijnen en past niet op INPUT_TYPE_DIGITAL of INPUT_TYPE_PCF8574.
#endif

#if (INPUT_KANAAL_CONFIG & INPUT_TYPE_DIGITAL)
  #if KEYPAD_TYPE != KEYPAD_TYPE_DRUKKNOP_DIRECT_1x4 && KEYPAD_TYPE != KEYPAD_TYPE_DRUKKNOP_MATRIX_2x2 && KEYPAD_TYPE != KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4 && KEYPAD_TYPE != KEYPAD_TYPE_MEMBRAAN_DIRECT_4x1 && KEYPAD_TYPE != KEYPAD_TYPE_TOUCH_TTP224_DIRECT_1x4
    #error INPUT_TYPE_DIGITAL ondersteunt alleen KEYPAD_TYPE_DRUKKNOP_DIRECT_1x4, KEYPAD_TYPE_DRUKKNOP_MATRIX_2x2, KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4, KEYPAD_TYPE_MEMBRAAN_DIRECT_4x1 en KEYPAD_TYPE_TOUCH_TTP224_DIRECT_1x4.
  #endif
#endif

#if (INPUT_KANAAL_CONFIG & INPUT_TYPE_PCF8574)
  #if (SCREEN_OUTPUT_CONFIG & SCREEN_TYPE_CHARACTER) && (I2C_ADDRESS_PCF8574 == I2C_ADDRESS_CHARACTER_SCREEN)
    #error I2C_ADDRESS_PCF8574 mag niet gelijk zijn aan I2C_ADDRESS_CHARACTER_SCREEN.
  #endif

  #if KEYPAD_TYPE == KEYPAD_TYPE_TOUCH_TTP229_MATRIX_4x4 && TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED != TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED_LOW && TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED != TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED_HIGH
    #error TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED moet TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED_LOW of TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED_HIGH zijn.
  #endif

  #if KEYPAD_TYPE != KEYPAD_TYPE_DRUKKNOP_DIRECT_1x4 && KEYPAD_TYPE != KEYPAD_TYPE_DRUKKNOP_DIRECT_2x4 && KEYPAD_TYPE != KEYPAD_TYPE_DRUKKNOP_MATRIX_2x2 && KEYPAD_TYPE != KEYPAD_TYPE_DRUKKNOP_MATRIX_4x4 && KEYPAD_TYPE != KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4 && KEYPAD_TYPE != KEYPAD_TYPE_MEMBRAAN_DIRECT_4x1 && KEYPAD_TYPE != KEYPAD_TYPE_MEMBRAAN_MATRIX_1x4 && KEYPAD_TYPE != KEYPAD_TYPE_MEMBRAAN_MATRIX_2x4 && KEYPAD_TYPE != KEYPAD_TYPE_MEMBRAAN_MATRIX_4x3 && KEYPAD_TYPE != KEYPAD_TYPE_MEMBRAAN_MATRIX_4x4 && KEYPAD_TYPE != KEYPAD_TYPE_TOUCH_TTP224_DIRECT_1x4 && KEYPAD_TYPE != KEYPAD_TYPE_TOUCH_TTP229_MATRIX_4x4
    #error INPUT_TYPE_PCF8574 ondersteunt alleen de gedefinieerde keypads die maximaal acht signaallijnen nodig hebben.
  #endif
#endif

#if (INPUT_KANAAL_CONFIG & INPUT_TYPE_HX1838)
  #if HX1838_BRON_CODES != HX1838_BRON_CODES_DEFINE && HX1838_BRON_CODES != HX1838_BRON_CODES_EEPROM_ALTIJD && HX1838_BRON_CODES != HX1838_BRON_CODES_EEPROM_WANNEER_GEEN_DEFINE
    #error Ongeldige HX1838_BRON_CODES.
  #endif

  #if HX1838_BRON_CODES == HX1838_BRON_CODES_DEFINE
    #if HX1838_TOETSENINDELING == HX1838_TOETSENINDELING_3x4
      #if HX1838_CODE_1 == 0 || HX1838_CODE_2 == 0 || HX1838_CODE_3 == 0 || HX1838_CODE_4 == 0 || HX1838_CODE_5 == 0 || HX1838_CODE_6 == 0 || HX1838_CODE_7 == 0 || HX1838_CODE_8 == 0 || HX1838_CODE_9 == 0 || HX1838_CODE_10 == 0 || HX1838_CODE_11 == 0 || HX1838_CODE_12 == 0
        #error HX1838_BRON_CODES_DEFINE vereist een volledige 12-toetsenmapping.
      #endif
    #elif HX1838_TOETSENINDELING == HX1838_TOETSENINDELING_REMOTE_17_TOETSEN
      #if HX1838_CODE_1 == 0 || HX1838_CODE_2 == 0 || HX1838_CODE_3 == 0 || HX1838_CODE_4 == 0 || HX1838_CODE_5 == 0 || HX1838_CODE_6 == 0 || HX1838_CODE_7 == 0 || HX1838_CODE_8 == 0 || HX1838_CODE_9 == 0 || HX1838_CODE_10 == 0 || HX1838_CODE_11 == 0 || HX1838_CODE_12 == 0 || HX1838_CODE_13 == 0 || HX1838_CODE_14 == 0 || HX1838_CODE_15 == 0 || HX1838_CODE_16 == 0 || HX1838_CODE_17 == 0
        #error HX1838_BRON_CODES_DEFINE vereist een volledige 17-toetsenmapping.
      #endif
    #elif HX1838_TOETSENINDELING == HX1838_TOETSENINDELING_REMOTE_21_TOETSEN_MP3
      #if HX1838_CODE_1 == 0 || HX1838_CODE_2 == 0 || HX1838_CODE_3 == 0 || HX1838_CODE_4 == 0 || HX1838_CODE_5 == 0 || HX1838_CODE_6 == 0 || HX1838_CODE_7 == 0 || HX1838_CODE_8 == 0 || HX1838_CODE_9 == 0 || HX1838_CODE_10 == 0 || HX1838_CODE_11 == 0 || HX1838_CODE_12 == 0 || HX1838_CODE_13 == 0 || HX1838_CODE_14 == 0 || HX1838_CODE_15 == 0 || HX1838_CODE_16 == 0 || HX1838_CODE_17 == 0 || HX1838_CODE_18 == 0 || HX1838_CODE_19 == 0 || HX1838_CODE_20 == 0 || HX1838_CODE_21 == 0
        #error HX1838_BRON_CODES_DEFINE vereist een volledige 21-toetsenmapping.
      #endif
    #endif
  #endif
#endif

// ============================================================================
// TOETSLABEL EN RESULTAAT VAN EEN INVOERLEZING
// ============================================================================
struct MappingTussenOpschriftEnWeergavetekst {
  const char* opschrift;
  const char* weergavetekst;
};

// ============================================================================
// TOETSENINDELING: door de gebruiker zelf ingevuld in de .ino, afhankelijk van het gekozen KEYPAD_TYPE/HX1838_TOETSENINDELING. 
// Input.cpp bevat de scanlogica en de vaste indeling waarmee een positie aan opschrift/weergavetekst wordt gekoppeld.
// ============================================================================
#if (INPUT_KANAAL_CONFIG & (INPUT_TYPE_DIGITAL | INPUT_TYPE_PCF8574))
  #if KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_DIRECT_1x4 || KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_MATRIX_2x2 || KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4 || KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_DIRECT_4x1 || KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_MATRIX_1x4 || KEYPAD_TYPE == KEYPAD_TYPE_TOUCH_TTP224_DIRECT_1x4
    #define AANTAL_KEYPAD_TOETSEN 4
  #elif KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_DIRECT_2x4 || KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_MATRIX_2x4 || KEYPAD_TYPE == KEYPAD_TYPE_TOUCH_TTP229_MATRIX_4x4
    #define AANTAL_KEYPAD_TOETSEN 8
  #elif KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_MATRIX_4x3
    #define AANTAL_KEYPAD_TOETSEN 12
  #elif KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_MATRIX_4x4 || KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_MATRIX_4x4
    #define AANTAL_KEYPAD_TOETSEN 16
  #elif KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_MATRIX_4x5
    #define AANTAL_KEYPAD_TOETSEN 20
  #else
    #error Ongeldige KEYPAD_TYPE voor AANTAL_KEYPAD_TOETSEN.
  #endif
  extern const MappingTussenOpschriftEnWeergavetekst KEY_LAYOUT[];
#endif

#if (INPUT_KANAAL_CONFIG & INPUT_TYPE_HX1838)
  #if HX1838_TOETSENINDELING == HX1838_TOETSENINDELING_3x4
    #define AANTAL_IR_TOETSEN 12
  #elif HX1838_TOETSENINDELING == HX1838_TOETSENINDELING_REMOTE_17_TOETSEN
    #define AANTAL_IR_TOETSEN 17
  #elif HX1838_TOETSENINDELING == HX1838_TOETSENINDELING_REMOTE_21_TOETSEN_MP3
    #define AANTAL_IR_TOETSEN 21
  #else
    #error Ongeldige HX1838_TOETSENINDELING.
  #endif
  extern const MappingTussenOpschriftEnWeergavetekst IR_KEY_LAYOUT[];
#endif

enum class InputKanaal : byte {
  NONE    = 0,
  DIGITAL = 1,
  PCF8574 = 2,
  HX1838  = 3
};

enum class InputGebeurtenis : byte {
  TOETSAANSLAG        = 0,  // gewone, nieuwe druk (bestaand gedrag)
  LOSGELATEN          = 1,  // de toets die net actief was, is nu losgelaten
  LANG_INDRUKKEN      = 2,  // de toets blijft ononderbroken ingedrukt, langer dan de ingestelde drempel (langIndrukkenDrempelMs)
  TIMEOUT_GEEN_INVOER = 3   // gedurende INPUT_KANAAL_TIMEOUT_BIJ_GEEN_TOETSAANSLAG_BINNEN_MS is er op geen enkel kanaal een toetsaanslag geweest
};

struct InputResultaat {
  InputKanaal inputKanaal;
  int positieToetsAanslag;
  const char* opschriftToetsAanslag;
#ifdef INPUT_KANAAL_OPVRAGEN_HUIDIGE_TOETSAANSLAG_UITGEBREID
  InputGebeurtenis gebeurtenis;
#endif
};

enum class StatusOpvragenToetsAanslagen : byte {
  GEEN      = 0,
  GELDIG    = 1,
  TE_WEINIG = 2,
  TE_VEEL   = 3,
  NIET_GEIMPLEMENTEERD = 4
};

struct InputResultaten {
  StatusOpvragenToetsAanslagen status;
  byte aantalToetsAanslagen;
  InputResultaat toetsAanslagen[MAX_AANTAL_SIMULTANE_TOETSAANSLAGEN];
};

// ============================================================================
// KOPPELING TUSSEN TOETSAANSLAG EN UIT TE VOEREN FUNCTIE
// De gebruiker definieert mappingTussenToetsaanslagEnUitTeVoerenFunctie zelf (in de sketch of UserConfig.h), enkel voor de toetsen die effectief een functie hebben.
// ============================================================================
typedef void (*InputFunctie)();

struct MappingTussenToetsaanslagEnUitTeVoerenFunctie {
  const char* opschriftToetsAanslag;
  InputFunctie functie;
#ifdef INPUT_KANAAL_OPVRAGEN_HUIDIGE_TOETSAANSLAG_UITGEBREID
  InputFunctie functieBijLoslaten;
  InputFunctie functieBijLangIndrukken;
  unsigned long langIndrukkenDrempelMs; // enkel relevant als functieBijLangIndrukken niet nullptr is
#endif
};

extern const MappingTussenToetsaanslagEnUitTeVoerenFunctie mappingTussenToetsaanslagEnUitTeVoerenFunctie[];

extern const byte aantalToetsFuncties;

const MappingTussenToetsaanslagEnUitTeVoerenFunctie* OpzoekenUitTeVoerenFunctieViaOpschriftToetsAanslag(const char* opschriftToetsAanslag);

template <size_t N>
const MappingTussenToetsaanslagEnUitTeVoerenFunctie* OpzoekenUitTeVoerenFunctieViaOpschriftToetsAanslag(const char* opschriftToetsAanslag, const MappingTussenToetsaanslagEnUitTeVoerenFunctie (&mapping)[N]) {
  if (opschriftToetsAanslag == nullptr) return nullptr;
  for (size_t i = 0; i < N; i++) { if (strcmp(mapping[i].opschriftToetsAanslag, opschriftToetsAanslag) == 0) return &mapping[i]; }
  return nullptr;
}

// ============================================================================
// PUBLIEKE API
// ============================================================================

// Initialiseert de geconfigureerde invoerkanalen (pinMode/Wire.begin/IrReceiver.begin),
// en start bij HX1838 automatisch de kalibratieprocedure als er nog geen geldige kalibratie in EEPROM staat. 
// Wanneer een volledige vaste HX1838-mapping in de configuratie staat, wordt die gebruikt vóór de kalibratieprocedure.
void InputConfigureren();

// Geeft één toetsaanslag terug via de gecompileerde invoerkanalen. 
// Bij wachten=true wordt blokkerend gewacht volgens de bestaande Tik-werking; 
// bij wachten=false wordt alleen de huidige beschikbare status opgevraagd.
// Bij PCF8574 + HX1838 samen krijgt het fysieke keypad voorrang wanneer beide op hetzelfde moment invoer leveren.
// Deze functie hoort bij Input: zij vraagt één huidige toetsaanslag op via de geconfigureerde invoerkanalen.
// Enkel actief wanneer INPUT_KANAAL_OPVRAGEN_HUIDIGE_TOETSAANSLAG_UITGEBREID is ingeschakeld: het veld "gebeurtenis" in het
// teruggegeven InputResultaat kan dan ook InputGebeurtenis::LOSGELATEN, ::LANG_INDRUKKEN of ::TIMEOUT_GEEN_INVOER zijn niet enkel ::TOETSAANSLAG. 
// Zonder die schakelaar blijft het gedrag exact zoals voorheen.
InputResultaat OpvragenHuidigeToetsAanslag(bool wachten = true);
InputResultaten OpvragenHuidigeToetsAanslagen(bool wachten = true, byte aantalSimultaan = 1);

void ToonMenuEnUitVoerenFunctieVolgensMappingMetToetsAanslag(bool wachten = true);

template <size_t N>
void ToonMenuEnUitVoerenFunctieVolgensMappingMetToetsAanslag(bool wachten, const MappingTussenToetsaanslagEnUitTeVoerenFunctie (&mapping)[N]) {
  InputResultaat invoer = OpvragenHuidigeToetsAanslag(wachten);
  if (invoer.inputKanaal == InputKanaal::NONE || invoer.opschriftToetsAanslag == nullptr) return;
  const MappingTussenToetsaanslagEnUitTeVoerenFunctie* gevondenMapping = OpzoekenUitTeVoerenFunctieViaOpschriftToetsAanslag(invoer.opschriftToetsAanslag, mapping);
  if (gevondenMapping == nullptr) return;

#ifdef INPUT_KANAAL_OPVRAGEN_HUIDIGE_TOETSAANSLAG_UITGEBREID
  if (invoer.gebeurtenis == InputGebeurtenis::LOSGELATEN) {
    if (gevondenMapping->functieBijLoslaten != nullptr) gevondenMapping->functieBijLoslaten();
    return;
  }
  if (invoer.gebeurtenis == InputGebeurtenis::LANG_INDRUKKEN) {
    if (gevondenMapping->functieBijLangIndrukken != nullptr) gevondenMapping->functieBijLangIndrukken();
    return;
  }
#endif

  if (gevondenMapping->functie != nullptr) gevondenMapping->functie();
}

#endif // INPUT_H
