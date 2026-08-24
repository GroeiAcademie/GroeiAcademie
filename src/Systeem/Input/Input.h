#ifndef INPUT_H
#define INPUT_H

#include <Arduino.h>

#include "InputTypes.h"

#include "../../Configuratie/SystemConfig.h"

#if ((INPUT_KANAAL_CONFIG) & ~(INPUT_TYPE_DIGITAL | INPUT_TYPE_PCF8574 | INPUT_TYPE_HX1838))
  #error INPUT_KANAAL_CONFIG bevat een onbekend invoertype.
#endif

#if (INPUT_KANAAL_CONFIG) != INPUT_TYPE_NONE && (INPUT_KANAAL_CONFIG) != INPUT_TYPE_DIGITAL && (INPUT_KANAAL_CONFIG) != INPUT_TYPE_PCF8574 && (INPUT_KANAAL_CONFIG) != INPUT_TYPE_HX1838 && (INPUT_KANAAL_CONFIG) != (INPUT_TYPE_PCF8574 | INPUT_TYPE_HX1838)
  #error Geldige INPUT_KANAAL_CONFIG: INPUT_TYPE_NONE, INPUT_TYPE_DIGITAL, INPUT_TYPE_PCF8574, INPUT_TYPE_HX1838 of INPUT_TYPE_PCF8574 | INPUT_TYPE_HX1838.
#endif

#if defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_MATRIX_4x5
  #error KEYPAD_TYPE_MEMBRAAN_MATRIX_4x5 gebruikt negen signaallijnen en past niet op INPUT_TYPE_DIGITAL of INPUT_TYPE_PCF8574.
#endif

#if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_HX1838) && (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
  #if (HX1838_ONTVANGER_PIN == ARDUINO_UNO_SHIELD_PIN_MOSI) || (HX1838_ONTVANGER_PIN == ARDUINO_UNO_SHIELD_PIN_MISO) || (HX1838_ONTVANGER_PIN == ARDUINO_UNO_SHIELD_PIN_SCK) || (HX1838_ONTVANGER_PIN == PIXEL_SCREEN_CS) || (HX1838_ONTVANGER_PIN == PIXEL_SCREEN_DC) || (HX1838_ONTVANGER_PIN == PIXEL_SCREEN_RST)
    #error HX1838_ONTVANGER_PIN conflicteert met een pin die het pixelscherm gebruikt (SPI MOSI/MISO/SCK of PIXEL_SCREEN_CS/DC/RST). Kies een andere HX1838_ONTVANGER_PIN in UserConfig.h.
  #endif
#endif

#if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_DIGITAL)
  #if KEYPAD_TYPE != KEYPAD_TYPE_DRUKKNOP_DIRECT_1x4 && KEYPAD_TYPE != KEYPAD_TYPE_DRUKKNOP_MATRIX_2x2 && KEYPAD_TYPE != KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4 && KEYPAD_TYPE != KEYPAD_TYPE_MEMBRAAN_DIRECT_4x1 && KEYPAD_TYPE != KEYPAD_TYPE_TOUCH_TTP224_DIRECT_1x4
    #error INPUT_TYPE_DIGITAL ondersteunt alleen KEYPAD_TYPE_DRUKKNOP_DIRECT_1x4, KEYPAD_TYPE_DRUKKNOP_MATRIX_2x2, KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4, KEYPAD_TYPE_MEMBRAAN_DIRECT_4x1 en KEYPAD_TYPE_TOUCH_TTP224_DIRECT_1x4.
  #endif
#endif

#if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_PCF8574)
  #if ((SCREEN_OUTPUT_CONFIG) & SCREEN_TYPE_CHARACTER) && (I2C_ADDRESS_PCF8574 == I2C_ADDRESS_CHARACTER_SCREEN)
    #error I2C_ADDRESS_PCF8574 mag niet gelijk zijn aan I2C_ADDRESS_CHARACTER_SCREEN.
  #endif

  #if KEYPAD_TYPE == KEYPAD_TYPE_TOUCH_TTP229_MATRIX_4x4 && TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED != TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED_LOW && TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED != TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED_HIGH
    #error TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED moet TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED_LOW of TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED_HIGH zijn.
  #endif

  #if KEYPAD_TYPE != KEYPAD_TYPE_DRUKKNOP_DIRECT_1x4 && KEYPAD_TYPE != KEYPAD_TYPE_DRUKKNOP_DIRECT_2x4 && KEYPAD_TYPE != KEYPAD_TYPE_DRUKKNOP_MATRIX_2x2 && KEYPAD_TYPE != KEYPAD_TYPE_DRUKKNOP_MATRIX_4x4 && KEYPAD_TYPE != KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4 && KEYPAD_TYPE != KEYPAD_TYPE_MEMBRAAN_DIRECT_4x1 && KEYPAD_TYPE != KEYPAD_TYPE_MEMBRAAN_MATRIX_1x4 && KEYPAD_TYPE != KEYPAD_TYPE_MEMBRAAN_MATRIX_2x4 && KEYPAD_TYPE != KEYPAD_TYPE_MEMBRAAN_MATRIX_4x3 && KEYPAD_TYPE != KEYPAD_TYPE_MEMBRAAN_MATRIX_4x4 && KEYPAD_TYPE != KEYPAD_TYPE_TOUCH_TTP224_DIRECT_1x4 && KEYPAD_TYPE != KEYPAD_TYPE_TOUCH_TTP229_MATRIX_4x4 && KEYPAD_TYPE != KEYPAD_TYPE_USER_DEFINED_DIRECT && KEYPAD_TYPE != KEYPAD_TYPE_USER_DEFINED_MATRIX
    #error INPUT_TYPE_PCF8574 ondersteunt alleen de gedefinieerde keypads die maximaal acht signaallijnen nodig hebben.
  #endif
#endif

#if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_HX1838)
  #if HX1838_BRON_CODES != HX1838_BRON_CODES_DEFINE && HX1838_BRON_CODES != HX1838_BRON_CODES_EEPROM_ALTIJD && HX1838_BRON_CODES != HX1838_BRON_CODES_EEPROM_WANNEER_GEEN_DEFINE
    #error Ongeldige HX1838_BRON_CODES.
  #endif

  #if HX1838_BRON_CODES == HX1838_BRON_CODES_DEFINE
    #if HX1838_TOETSENINDELING == HX1838_TOETSENINDELING_REMOTE_OK_BOVENAAN_17_TOETSEN || HX1838_TOETSENINDELING == HX1838_TOETSENINDELING_REMOTE_OK_ONDERAAN_17_TOETSEN
      #if HX1838_CODE_1 == 0 || HX1838_CODE_2 == 0 || HX1838_CODE_3 == 0 || HX1838_CODE_4 == 0 || HX1838_CODE_5 == 0 || HX1838_CODE_6 == 0 || HX1838_CODE_7 == 0 || HX1838_CODE_8 == 0 || HX1838_CODE_9 == 0 || HX1838_CODE_10 == 0 || HX1838_CODE_11 == 0 || HX1838_CODE_12 == 0 || HX1838_CODE_13 == 0 || HX1838_CODE_14 == 0 || HX1838_CODE_15 == 0 || HX1838_CODE_16 == 0 || HX1838_CODE_17 == 0
        #error HX1838_CODE_1 t.e.m. HX1838_CODE_17 mogen bij HX1838_BRON_CODES_DEFINE niet 0 zijn.
      #endif
    #elif HX1838_TOETSENINDELING == HX1838_TOETSENINDELING_REMOTE_21_TOETSEN_MP3
      #if HX1838_CODE_1 == 0 || HX1838_CODE_2 == 0 || HX1838_CODE_3 == 0 || HX1838_CODE_4 == 0 || HX1838_CODE_5 == 0 || HX1838_CODE_6 == 0 || HX1838_CODE_7 == 0 || HX1838_CODE_8 == 0 || HX1838_CODE_9 == 0 || HX1838_CODE_10 == 0 || HX1838_CODE_11 == 0 || HX1838_CODE_12 == 0 || HX1838_CODE_13 == 0 || HX1838_CODE_14 == 0 || HX1838_CODE_15 == 0 || HX1838_CODE_16 == 0 || HX1838_CODE_17 == 0 || HX1838_CODE_18 == 0 || HX1838_CODE_19 == 0 || HX1838_CODE_20 == 0 || HX1838_CODE_21 == 0
        #error HX1838_CODE_1 t.e.m. HX1838_CODE_21 mogen bij HX1838_BRON_CODES_DEFINE niet 0 zijn.
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
// TOETSENINDELING: Input.cpp bevat de scanlogica en de vaste indelingen waarmee een positie aan opschrift/weergavetekst wordt gekoppeld. 
// Bij de twee KEYPAD_TYPE_USER_DEFINED_...-types komt KEY_LAYOUT uit de KEYPAD_GENERIEK_... configuratie in UserConfig.h; een .ino kan de apart gecompileerde Input.cpp niet configureren.
// ============================================================================
#if ((INPUT_KANAAL_CONFIG) & (INPUT_TYPE_DIGITAL | INPUT_TYPE_PCF8574))
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
  #elif KEYPAD_TYPE == KEYPAD_TYPE_USER_DEFINED_DIRECT
    #define AANTAL_KEYPAD_TOETSEN KEYPAD_GENERIEK_AANTAL_PINNEN
  #elif KEYPAD_TYPE == KEYPAD_TYPE_USER_DEFINED_MATRIX
    #define AANTAL_KEYPAD_TOETSEN (KEYPAD_GENERIEK_AANTAL_RIJEN * KEYPAD_GENERIEK_AANTAL_KOLOMMEN)
  #else
    #error Ongeldige KEYPAD_TYPE voor AANTAL_KEYPAD_TOETSEN.
  #endif
  extern const MappingTussenOpschriftEnWeergavetekst KEY_LAYOUT[];
#endif

#if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_HX1838)
  #if HX1838_TOETSENINDELING   == HX1838_TOETSENINDELING_REMOTE_OK_BOVENAAN_17_TOETSEN
    #define AANTAL_IR_TOETSEN 17
  #elif HX1838_TOETSENINDELING == HX1838_TOETSENINDELING_REMOTE_OK_ONDERAAN_17_TOETSEN
    #define AANTAL_IR_TOETSEN 17
  #elif HX1838_TOETSENINDELING == HX1838_TOETSENINDELING_REMOTE_21_TOETSEN_MP3
    #define AANTAL_IR_TOETSEN 21
  #elif HX1838_TOETSENINDELING == HX1838_TOETSENINDELING_REMOTE_USER_DEFINED
    #define AANTAL_IR_TOETSEN HX1838_GENERIEK_AANTAL_TOETSEN
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

// Apart type, enkel voor mappings die het argumenten-veld (void*) willen gebruiken.
// Raakt InputFunctie/MappingTussenToetsaanslagEnUitTeVoerenFunctie hierboven niet aan.
typedef void (*InputFunctieMetArgumenten)(void* argumenten);

struct MappingTussenToetsaanslagEnUitTeVoerenFunctieMetArgumenten {
  const char* opschriftToetsAanslag;
  InputFunctieMetArgumenten functie;
  void* argumenten;
#ifdef INPUT_KANAAL_OPVRAGEN_HUIDIGE_TOETSAANSLAG_UITGEBREID
  InputFunctieMetArgumenten functieBijLoslaten;
  void* argumentenBijLoslaten;
  InputFunctieMetArgumenten functieBijLangIndrukken;
  void* argumentenBijLangIndrukken;
  unsigned long langIndrukkenDrempelMs;
#endif
};

extern const MappingTussenToetsaanslagEnUitTeVoerenFunctie mappingTussenToetsaanslagEnUitTeVoerenFunctie[];

extern const byte aantalToetsFuncties;

const MappingTussenToetsaanslagEnUitTeVoerenFunctie* OpzoekenUitTeVoerenFunctieViaOpschriftToetsAanslag(const char* opschriftToetsAanslag);

// --- Interne koppeling voor mapping-bewuste LANG_INDRUKKEN-detectie (niet rechtstreeks gebruiken) ---
// OpvragenHuidigeToetsAanslag() moet, om te weten of/wanneer een LANG_INDRUKKEN-gebeurtenis
// gegenereerd wordt, ergens een lang-indrukken-drempel (in ms) opvragen voor de toets die nu
// stabiel ingedrukt is. Standaard gebeurt dat via het globale mappingTussenToetsaanslagEnUitTeVoerenFunctie[]
// hierboven. De UitVoerenFunctieVolgensMappingMetToetsAanslag-templates verderop in dit bestand
// zetten deze koppeling vlak vóór de aanroep om naar hún eigen, expliciet meegegeven mapping, zodat
// ook toepassingen met meerdere menu's (elk met een eigen mapping-array) LANG_INDRUKKEN correct krijgen.
typedef unsigned long (*LangIndrukkenDrempelOpzoekerFunctie)(const char* opschriftToetsAanslag);

#ifdef INPUT_KANAAL_OPVRAGEN_HUIDIGE_TOETSAANSLAG_UITGEBREID
extern const MappingTussenToetsaanslagEnUitTeVoerenFunctie* _actieveMappingVoorLangIndrukken;
extern byte _actieveMappingAantalVoorLangIndrukken;
unsigned long _LangIndrukkenDrempelOpzoekerViaActieveMapping(const char* opschriftToetsAanslag);

extern const MappingTussenToetsaanslagEnUitTeVoerenFunctieMetArgumenten* _actieveMappingMetArgumentenVoorLangIndrukken;
extern byte _actieveMappingMetArgumentenAantalVoorLangIndrukken;
unsigned long _LangIndrukkenDrempelOpzoekerViaActieveMappingMetArgumenten(const char* opschriftToetsAanslag);
#endif

template <size_t N>
const MappingTussenToetsaanslagEnUitTeVoerenFunctie* OpzoekenUitTeVoerenFunctieViaOpschriftToetsAanslag(const char* opschriftToetsAanslag, const MappingTussenToetsaanslagEnUitTeVoerenFunctie (&mapping)[N]) {
  if (opschriftToetsAanslag == nullptr) return nullptr;
  for (size_t i = 0; i < N; i++) { if (strcmp(mapping[i].opschriftToetsAanslag, opschriftToetsAanslag) == 0) return &mapping[i]; }
  return nullptr;
}

template <size_t N>
const MappingTussenToetsaanslagEnUitTeVoerenFunctieMetArgumenten* OpzoekenUitTeVoerenFunctieViaOpschriftToetsAanslag(const char* opschriftToetsAanslag, const MappingTussenToetsaanslagEnUitTeVoerenFunctieMetArgumenten (&mapping)[N]) {
  if (opschriftToetsAanslag == nullptr) return nullptr;
  for (size_t i = 0; i < N; i++) { if (strcmp(mapping[i].opschriftToetsAanslag, opschriftToetsAanslag) == 0) return &mapping[i]; }
  return nullptr;
}

#ifdef INPUT_MAPPING_EXTRA_CONTROLES_INSCHAKELEN
// Interne werkfunctie, geïmplementeerd in Input.cpp waar KEY_LAYOUT/IR_KEY_LAYOUT voor het actief gecompileerde KEYPAD_TYPE/HX1838_TOETSENINDELING gekend zijn.
// Niet rechtstreeks door de gebruiker aan te roepen, gebruik ControleerMappingVolledigheid().
void ControleerMappingVolledigheidIntern(const char* const opschriftToetsAanslag[], byte aantalEntries);

// Controleert of mapping elk opschrift bevat dat het actief gecompileerde KEYPAD_TYPE (of HX1838_TOETSENINDELING) kan opleveren, en meldt via Serial welke ontbreken.
// Enkel bedoeld om tijdens het testen op te roepen, bijvoorbeeld eenmalig in setup(), niet in productiecode: dit voegt Serial-afhankelijkheid en extra flashgebruik toe.
// Enkel beschikbaar wanneer INPUT_MAPPING_EXTRA_CONTROLES_INSCHAKELEN in UserConfig.h staat.
template <size_t N>
void ControleerMappingVolledigheid(const MappingTussenToetsaanslagEnUitTeVoerenFunctie (&mapping)[N]) {
  const char* opschriftToetsAanslag[N];
  for (size_t i = 0; i < N; i++) opschriftToetsAanslag[i] = mapping[i].opschriftToetsAanslag;
  ControleerMappingVolledigheidIntern(opschriftToetsAanslag, N);
}
#endif

// ============================================================================
// PUBLIEKE API
// ============================================================================

// Initialiseert de geconfigureerde invoerkanalen (pinMode/Wire.begin/IrReceiver.begin),
// en start bij HX1838 automatisch de kalibratieprocedure als er nog geen geldige kalibratie in EEPROM staat. 
// Wanneer de geconfigureerde bron de vaste HX1838-mapping gebruikt, worden ontbrekende codes vanuit SystemConfig.h aangevuld.
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

// Zelfde werking als hierboven, maar de LANG_INDRUKKEN-drempel wordt via drempelOpzoeker() opgevraagd
// in plaats van via het globale mappingTussenToetsaanslagEnUitTeVoerenFunctie[]-array. Wordt gebruikt
// door de UitVoerenFunctieVolgensMappingMetToetsAanslag-templates hieronder; niet bedoeld voor rechtstreeks
// gebruik in een sketch (gebruik daarvoor gewoon de mapping-templates).
InputResultaat OpvragenHuidigeToetsAanslag(bool wachten, LangIndrukkenDrempelOpzoekerFunctie drempelOpzoeker);

InputResultaten OpvragenHuidigeToetsAanslagen(bool wachten = true, byte aantalSimultaan = 1);

void UitVoerenFunctieVolgensMappingMetToetsAanslag(bool wachten = true);

template <size_t N>
void UitVoerenFunctieVolgensMappingMetToetsAanslag(bool wachten, const MappingTussenToetsaanslagEnUitTeVoerenFunctie (&mapping)[N]) {
#ifdef INPUT_KANAAL_OPVRAGEN_HUIDIGE_TOETSAANSLAG_UITGEBREID
  _actieveMappingVoorLangIndrukken = mapping;
  _actieveMappingAantalVoorLangIndrukken = (byte)N;
  InputResultaat invoer = OpvragenHuidigeToetsAanslag(wachten, _LangIndrukkenDrempelOpzoekerViaActieveMapping);
#else
  InputResultaat invoer = OpvragenHuidigeToetsAanslag(wachten);
#endif
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

// Variant voor mappings die het argumenten-veld (void*) gebruiken. Zelfde werking, andere types.
template <size_t N>
void UitVoerenFunctieVolgensMappingMetToetsAanslag(bool wachten, const MappingTussenToetsaanslagEnUitTeVoerenFunctieMetArgumenten (&mapping)[N]) {
#ifdef INPUT_KANAAL_OPVRAGEN_HUIDIGE_TOETSAANSLAG_UITGEBREID
  _actieveMappingMetArgumentenVoorLangIndrukken = mapping;
  _actieveMappingMetArgumentenAantalVoorLangIndrukken = (byte)N;
  InputResultaat invoer = OpvragenHuidigeToetsAanslag(wachten, _LangIndrukkenDrempelOpzoekerViaActieveMappingMetArgumenten);
#else
  InputResultaat invoer = OpvragenHuidigeToetsAanslag(wachten);
#endif
  if (invoer.inputKanaal == InputKanaal::NONE || invoer.opschriftToetsAanslag == nullptr) return;
  const MappingTussenToetsaanslagEnUitTeVoerenFunctieMetArgumenten* gevondenMapping = OpzoekenUitTeVoerenFunctieViaOpschriftToetsAanslag(invoer.opschriftToetsAanslag, mapping);
  if (gevondenMapping == nullptr) return;

#ifdef INPUT_KANAAL_OPVRAGEN_HUIDIGE_TOETSAANSLAG_UITGEBREID
  if (invoer.gebeurtenis == InputGebeurtenis::LOSGELATEN) {
    if (gevondenMapping->functieBijLoslaten != nullptr) gevondenMapping->functieBijLoslaten(gevondenMapping->argumentenBijLoslaten);
    return;
  }
  if (invoer.gebeurtenis == InputGebeurtenis::LANG_INDRUKKEN) {
    if (gevondenMapping->functieBijLangIndrukken != nullptr) gevondenMapping->functieBijLangIndrukken(gevondenMapping->argumentenBijLangIndrukken);
    return;
  }
#endif

  if (gevondenMapping->functie != nullptr) gevondenMapping->functie(gevondenMapping->argumenten);
}

#endif // INPUT_H
