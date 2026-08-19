#ifndef GROEIACADEMIE_USER_LIBRARY_EN_H
#define GROEIACADEMIE_USER_LIBRARY_EN_H

// This provisional version was automatically translated from the Dutch version.
// Pending review by someone proficient in this language.

// Kopieer dit bestand naar UserLibrary_EN.h in dezelfde map.
// Alle aanpasbare teksten uit het overeenkomstige officiële taalbestand staan hieronder als voorbeeld.
// Vaste systeemfoutcodes (_FATAL_CSxxx, _FATAL_PSxxx en _CRITICAL_SSxxx) zijn bewust niet aanpasbaar.
//=========================================================
// FATAL
//=========================================================
  #define FATAL_ZOEK_OP "LOOK THIS UP NOW"


// PixelScreen

// InputKanalen

//=========================================================
// PrintToScreen
//=========================================================
// ADS1115
  #define _LCD_ADS1115_FOUT                "ADS1115"
  #define _LCD_ADS1115_NIET_GEVONDEN       "NOT FOUND"

// Input Type: HX1838
  #define _INPUT_HX1838_CONTROLE                                   "IR CHECK:"
  #define _INPUT_HX1838_DRUK_OP_ELKE_TOETS_VOOR_CONTROLE           "PRESS EACH KEY"

  #define _INPUT_HX1838_CONTROLE_GESLAAGD                          "IR CHECK: OK"

  #define _INPUT_HX1838_DRUK_NU_OP                                 "PRESS NOW:"
// REGEL 2 IS DYNAMISCH: WEERGAVETEKST VAN DE TOETS.

// REGEL 1 IS DYNAMISCH: WEERGAVETEKST VAN DE HERKENDE TOETS.
  #define _INPUT_HX1838_TOETS_HERKEND                              "RECOGNIZED"

  #define _INPUT_HX1838_KALIBRATIE_UITVOEREN                       "CALIBRATION"
  #define _INPUT_HX1838_KALIBRATIE_OPGESLAGEN                      "SAVED"
  #define _INPUT_HX1838_KALIBRATIE_TIMEOUT                         "TIMEOUT"

  #define _INPUT_HX1838_MAPPING_EEPROM_GELADEN                     "MAPPING LOADED"

  #define _INPUT_HX1838_MAPPING_GELADEN                            "MAPPING LOADED"

  #define _INPUT_HX1838_INTERRUPT_KOPPELING_MISLUKT                "IR PIN ERROR"

// Input Type: PCF8574
  #define _INPUT_PCF8574_FOUT                                      "PCF8574 ERROR"
  #define _INPUT_PCF8574_CONTROLEER_I2C                            "CHECK I2C"

// Stimulus
  #define _LCD_KRACHT_TE_HARD              "TOO HARD"
  #define _LCD_KRACHT_TE_ZACHT             "TOO SOFT"

  #define _LCD_SCORE_TIKKRACHT             "TAP FORCE "
  #define _LCD_SCORE_TIKTIJD               "TAP TIME "

  #define _LCD_TIJD_METEN_STOPT            "WE STOP HERE"
  #define _LCD_TIJD_TEVEEL_FOUT            "BAD START :)"
  #define _LCD_TIJD_TE_KORT                "TOO SHORT"
  #define _LCD_TIJD_TE_LANG                "TOO LONG"

//=========================================================
// Serial
//=========================================================

// Input Type: HX1838
  #define _INPUT_HX1838_DRUK_NU_OP_SERIAL                          "Press now: "
  #define _INPUT_HX1838_GEEN_GELDIGE_KALIBRATIE_SERIAL             "No valid IR calibration found."
  #define _INPUT_HX1838_MAPPING_EEPROM_GELADEN_SERIAL              "IR mapping loaded from EEPROM:"
  #define _INPUT_HX1838_KALIBRATIE_GESTART_SERIAL                  "=== IR calibration started ==="
  #define _INPUT_HX1838_KALIBRATIE_TIMEOUT_SERIAL                  "IR calibration aborted: timeout."

  #define _INPUT_HX1838_INTERRUPT_KOPPELING_MISLUKT_SERIAL         "IR receiver: interrupt could not be attached to HX1838_ONTVANGER_PIN."
  #define _INPUT_HX1838_KALIBRATIE_KLAAR_CONTROLE_SERIAL           "Calibration complete. Press all keys again to verify."
  #define _INPUT_HX1838_KALIBRATIE_OPGESLAGEN_SERIAL               "Calibration saved."
  #define _INPUT_HX1838_MAPPING_CONFIG_GELADEN_SERIAL              "Default IR mapping loaded from configuration:"

  #define _INPUT_HX1838_MAPPING_CONFIG_ONVOLLEDIG                  "MAPPING INCOMPLETE"

  #define _INPUT_HX1838_MAPPING_CONFIG_ONVOLLEDIG_SERIAL           "Fixed IR mapping in UserConfig.h is incomplete, not all HX1838_CODE_x are filled in."
  #define _INPUT_HX1838_TOETS_HERKEND_SERIAL                       "Recognized: "

  #define _INPUT_HX1838_DEBUG_FAALDE                                      "failed"
  #define _INPUT_HX1838_DEBUG_MAGIC_MISMATCH                              "magic mismatch"
  #define _INPUT_HX1838_DEBUG_VERSIE_MISMATCH                             "version mismatch"
  #define _INPUT_HX1838_DEBUG_TOETSENINDELING_MISMATCH                    "key layout mismatch"
  #define _INPUT_HX1838_DEBUG_ALLE_TOETSEN_GEKALIBREER_START_VERIFICATIE  "all keys calibrated, start verification"
  #define _INPUT_HX1838_DEBUG_VOOR_INIT                                   "before init"
  #define _INPUT_HX1838_DEBUG_NA_INIT                                     "after init"

// Input Type: PCF8574
  #define _INPUT_PCF8574_NIET_BEREIKBAAR_SERIAL                    "PCF8574 not reachable at configured I2C address."

//=========================================================
// INPUT - TOETSLABELS
//=========================================================
  #define LABEL_TOETS_0 "Key 0"
  #define LABEL_TOETS_1 "Key 1"
  #define LABEL_TOETS_2 "Key 2"
  #define LABEL_TOETS_3 "Key 3"
  #define LABEL_TOETS_4 "Key 4"
  #define LABEL_TOETS_5 "Key 5"
  #define LABEL_TOETS_6 "Key 6"
  #define LABEL_TOETS_7 "Key 7"
  #define LABEL_TOETS_8 "Key 8"
  #define LABEL_TOETS_9 "Key 9"

  #define LABEL_TOETS_A "Key A"
  #define LABEL_TOETS_B "Key B"
  #define LABEL_TOETS_C "Key C"
  #define LABEL_TOETS_D "Key D"

  #define LABEL_TOETS_S1 "Key S1"
  #define LABEL_TOETS_S2 "Key S2"
  #define LABEL_TOETS_S3 "Key S3"
  #define LABEL_TOETS_S4 "Key S4"
  #define LABEL_TOETS_S5 "Key S5"
  #define LABEL_TOETS_S6 "Key S6"
  #define LABEL_TOETS_S7 "Key S7"
  #define LABEL_TOETS_S8 "Key S8"
  #define LABEL_TOETS_S9 "Key S9"
  #define LABEL_TOETS_S10 "Key S10"
  #define LABEL_TOETS_S11 "Key S11"
  #define LABEL_TOETS_S12 "Key S12"
  #define LABEL_TOETS_S13 "Key S13"
  #define LABEL_TOETS_S14 "Key S14"
  #define LABEL_TOETS_S15 "Key S15"
  #define LABEL_TOETS_S16 "Key S16"

  #define LABEL_TOETS_STER "Key *"
  #define LABEL_TOETS_HEKJE "Key #"

// extended 
  #define LABEL_TOETS_UP    "Navigation up"
  #define LABEL_TOETS_DOWN  "Navigation down"
  #define LABEL_TOETS_OK    "Navigation OK"
  #define LABEL_TOETS_LEFT  "Navigation left"
  #define LABEL_TOETS_RIGHT "Navigation right"

// MP3-afstandsbediening
  #define LABEL_TOETS_CH_MINUS "CH-"
  #define LABEL_TOETS_CH "CH"
  #define LABEL_TOETS_CH_PLUS "CH+"
  #define LABEL_TOETS_PREV "Previous"
  #define LABEL_TOETS_NEXT "Next"
  #define LABEL_TOETS_PLAY "Play/Pause"
  #define LABEL_TOETS_MINUS "-"
  #define LABEL_TOETS_PLUS "+"
  #define LABEL_TOETS_EQ "EQ"
  #define LABEL_TOETS_100_PLUS "100+"
  #define LABEL_TOETS_200_PLUS "200+"

#endif
