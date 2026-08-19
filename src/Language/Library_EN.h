#ifndef GROEIACADEMIE_LIBRARY_EN_H
#define GROEIACADEMIE_LIBRARY_EN_H

// This provisional version was automatically translated from the Dutch version.
// Pending review by someone proficient in this language.

//=========================================================
// FATAL
//=========================================================
#ifndef FATAL_ZOEK_OP
  #define FATAL_ZOEK_OP "LOOK THIS UP NOW"
#endif

// CharacterScreen
#define _FATAL_CS000 "FATAL: CS000" // CharacterScreenConfigureren() niet aangeroepen
#define _FATAL_CS001 "FATAL: CS001" // Geen I2C-antwoord op geen enkel geprobeerd adres
#define _FATAL_CS002 "FATAL: CS002" // Scherm gevonden op ander I2C-adres dan I2C_ADRES (modus 1)

// PixelScreen
#define _FATAL_PS000 "FATAL: PS000" // PixelScreenConfigureren() niet aangeroepen
#define _FATAL_PS001 "FATAL: PS001" // PixelScreen niet gekoppeld
#define _FATAL_PS002 "FATAL: PS002" // Omgewisselde breedte en hoogte komen niet overeen
#define _FATAL_PS003 "FATAL: PS003" // Niet-omgewisselde breedte en hoogte komen niet overeen
#define _FATAL_PS004 "FATAL: PS004" // Tekstgrid kleiner dan 16x2

// SerialScreen
#define _CRITICAL_SS001 "CRITICAL: SS001" // SerialScreen niet beschikbaar na SERIAL_CONNECT_TIMEOUT_MS

// InputKanalen
#define _FATAL_IN000 "FATAL: IN000" // PCF8574 niet bereikbaar op ingesteld I2C-adres

//=========================================================
// PrintToScreen
//=========================================================
// ADS1115
#ifndef _LCD_ADS1115_FOUT
  #define _LCD_ADS1115_FOUT                "ADS1115"
#endif
#ifndef _LCD_ADS1115_NIET_GEVONDEN
  #define _LCD_ADS1115_NIET_GEVONDEN       "NOT FOUND"
#endif

// Input Type: HX1838
#ifndef _INPUT_HX1838_CONTROLE
  #define _INPUT_HX1838_CONTROLE                                   "IR CHECK:"
#endif
#ifndef _INPUT_HX1838_DRUK_OP_ELKE_TOETS_VOOR_CONTROLE
  #define _INPUT_HX1838_DRUK_OP_ELKE_TOETS_VOOR_CONTROLE           "PRESS EACH KEY"
#endif

#ifndef _INPUT_HX1838_CONTROLE_GESLAAGD
  #define _INPUT_HX1838_CONTROLE_GESLAAGD                          "IR CHECK: OK"
#endif

#ifndef _INPUT_HX1838_DRUK_NU_OP
  #define _INPUT_HX1838_DRUK_NU_OP                                 "PRESS NOW:"
#endif
// REGEL 2 IS DYNAMISCH: WEERGAVETEKST VAN DE TOETS.

// REGEL 1 IS DYNAMISCH: WEERGAVETEKST VAN DE HERKENDE TOETS.
#ifndef _INPUT_HX1838_TOETS_HERKEND
  #define _INPUT_HX1838_TOETS_HERKEND                              "RECOGNIZED"
#endif

#ifndef _INPUT_HX1838_KALIBRATIE_UITVOEREN
  #define _INPUT_HX1838_KALIBRATIE_UITVOEREN                       "CALIBRATION"
#endif
#ifndef _INPUT_HX1838_KALIBRATIE_TIMEOUT
  #define _INPUT_HX1838_KALIBRATIE_TIMEOUT                          "TIMEOUT"
#endif

#ifndef _INPUT_HX1838_KALIBRATIE_OPGESLAGEN
  #define _INPUT_HX1838_KALIBRATIE_OPGESLAGEN                      "SAVED"
#endif

#ifndef _INPUT_HX1838_MAPPING_EEPROM_GELADEN
  #define _INPUT_HX1838_MAPPING_EEPROM_GELADEN                     "MAPPING LOADED"
#endif

#ifndef _INPUT_HX1838_MAPPING_GELADEN
  #define _INPUT_HX1838_MAPPING_GELADEN                            "MAPPING LOADED"
#endif

#ifndef _INPUT_HX1838_INTERRUPT_KOPPELING_MISLUKT
  #define _INPUT_HX1838_INTERRUPT_KOPPELING_MISLUKT                "IR PIN ERROR"
#endif

// Input Type: PCF8574
#ifndef _INPUT_PCF8574_FOUT
  #define _INPUT_PCF8574_FOUT                                      "PCF8574 ERROR"
#endif
#ifndef _INPUT_PCF8574_CONTROLEER_I2C
  #define _INPUT_PCF8574_CONTROLEER_I2C                            "CHECK I2C"
#endif

// Stimulus
#ifndef _LCD_KRACHT_TE_HARD
  #define _LCD_KRACHT_TE_HARD              "TOO HARD"
#endif
#ifndef _LCD_KRACHT_TE_ZACHT
  #define _LCD_KRACHT_TE_ZACHT             "TOO SOFT"
#endif

#ifndef _LCD_SCORE_TIKKRACHT
  #define _LCD_SCORE_TIKKRACHT             "TAP FORCE "
#endif
#ifndef _LCD_SCORE_TIKTIJD
  #define _LCD_SCORE_TIKTIJD               "TAP TIME "
#endif

#ifndef _LCD_TIJD_METEN_STOPT
  #define _LCD_TIJD_METEN_STOPT            "WE STOP HERE"
#endif
#ifndef _LCD_TIJD_TEVEEL_FOUT
  #define _LCD_TIJD_TEVEEL_FOUT            "BAD START :)"
#endif
#ifndef _LCD_TIJD_TE_KORT
  #define _LCD_TIJD_TE_KORT                "TOO SHORT"
#endif
#ifndef _LCD_TIJD_TE_LANG
  #define _LCD_TIJD_TE_LANG                "TOO LONG"
#endif  

//=========================================================
// Serial
//=========================================================

// Input Type: HX1838
#ifndef _INPUT_HX1838_DRUK_NU_OP_SERIAL
  #define _INPUT_HX1838_DRUK_NU_OP_SERIAL                          "Press now: "
#endif

#ifndef _INPUT_HX1838_GEEN_GELDIGE_KALIBRATIE_SERIAL
  #define _INPUT_HX1838_GEEN_GELDIGE_KALIBRATIE_SERIAL             "No valid IR calibration found."
#endif

#ifndef _INPUT_HX1838_TOETS_HERKEND_SERIAL
  #define _INPUT_HX1838_TOETS_HERKEND_SERIAL                       "Recognized: "
#endif

#ifndef _INPUT_HX1838_MAPPING_EEPROM_GELADEN_SERIAL
  #define _INPUT_HX1838_MAPPING_EEPROM_GELADEN_SERIAL              "IR mapping loaded from EEPROM:"
#endif

#ifndef _INPUT_HX1838_KALIBRATIE_GESTART_SERIAL
  #define _INPUT_HX1838_KALIBRATIE_GESTART_SERIAL                  "=== IR calibration started ==="
#endif

#ifndef _INPUT_HX1838_INTERRUPT_KOPPELING_MISLUKT_SERIAL
  #define _INPUT_HX1838_INTERRUPT_KOPPELING_MISLUKT_SERIAL         "IR receiver: interrupt could not be attached to HX1838_ONTVANGER_PIN."
#endif

#ifndef _INPUT_HX1838_KALIBRATIE_KLAAR_CONTROLE_SERIAL
  #define _INPUT_HX1838_KALIBRATIE_KLAAR_CONTROLE_SERIAL           "Calibration complete. Press all keys again to verify."
#endif

#ifndef _INPUT_HX1838_KALIBRATIE_TIMEOUT_SERIAL
  #define _INPUT_HX1838_KALIBRATIE_TIMEOUT_SERIAL                   "IR calibration aborted: timeout."
#endif

#ifndef _INPUT_HX1838_KALIBRATIE_OPGESLAGEN_SERIAL
  #define _INPUT_HX1838_KALIBRATIE_OPGESLAGEN_SERIAL               "Calibration saved."
#endif

#ifndef _INPUT_HX1838_MAPPING_CONFIG_GELADEN_SERIAL
  #define _INPUT_HX1838_MAPPING_CONFIG_GELADEN_SERIAL              "Default IR mapping loaded from configuration:"
#endif

#ifndef _INPUT_HX1838_MAPPING_CONFIG_ONVOLLEDIG
  #define _INPUT_HX1838_MAPPING_CONFIG_ONVOLLEDIG                  "MAPPING INCOMPLETE"
#endif

#ifndef _INPUT_HX1838_MAPPING_CONFIG_ONVOLLEDIG_SERIAL
  #define _INPUT_HX1838_MAPPING_CONFIG_ONVOLLEDIG_SERIAL           "Fixed IR mapping in UserConfig.h is incomplete, not all HX1838_CODE_x are filled in."
#endif

#ifndef _INPUT_HX1838_DEBUG_FAALDE
  #define _INPUT_HX1838_DEBUG_FAALDE                                      "failed"
#endif
#ifndef _INPUT_HX1838_DEBUG_MAGIC_MISMATCH
  #define _INPUT_HX1838_DEBUG_MAGIC_MISMATCH                              "magic mismatch"
#endif
#ifndef _INPUT_HX1838_DEBUG_VERSIE_MISMATCH
  #define _INPUT_HX1838_DEBUG_VERSIE_MISMATCH                             "version mismatch"
#endif
#ifndef _INPUT_HX1838_DEBUG_TOETSENINDELING_MISMATCH
  #define _INPUT_HX1838_DEBUG_TOETSENINDELING_MISMATCH                    "key layout mismatch"
#endif
#ifndef _INPUT_HX1838_DEBUG_ALLE_TOETSEN_GEKALIBREER_START_VERIFICATIE
  #define _INPUT_HX1838_DEBUG_ALLE_TOETSEN_GEKALIBREER_START_VERIFICATIE  "all keys calibrated, start verification"
#endif
#ifndef _INPUT_HX1838_DEBUG_VOOR_INIT
  #define _INPUT_HX1838_DEBUG_VOOR_INIT                                   "before init"
#endif
#ifndef _INPUT_HX1838_DEBUG_NA_INIT
  #define _INPUT_HX1838_DEBUG_NA_INIT                                     "after init"
#endif

// Input Type: PCF8574
#ifndef _INPUT_PCF8574_NIET_BEREIKBAAR_SERIAL
  #define _INPUT_PCF8574_NIET_BEREIKBAAR_SERIAL                    "PCF8574 not reachable at configured I2C address."
#endif

//=========================================================
// INPUT - TOETSLABELS
//=========================================================
// drukknoppen
#ifndef LABEL_TOETS_S1
  #define LABEL_TOETS_S1 "Key S1"
#endif
#ifndef LABEL_TOETS_S2
  #define LABEL_TOETS_S2 "Key S2"
#endif
#ifndef LABEL_TOETS_S3
  #define LABEL_TOETS_S3 "Key S3"
#endif
#ifndef LABEL_TOETS_S4
  #define LABEL_TOETS_S4 "Key S4"
#endif
#ifndef LABEL_TOETS_S5
  #define LABEL_TOETS_S5 "Key S5"
#endif
#ifndef LABEL_TOETS_S6
  #define LABEL_TOETS_S6 "Key S6"
#endif
#ifndef LABEL_TOETS_S7
  #define LABEL_TOETS_S7 "Key S7"
#endif
#ifndef LABEL_TOETS_S8
  #define LABEL_TOETS_S8 "Key S8"
#endif
#ifndef LABEL_TOETS_S9
  #define LABEL_TOETS_S9 "Key S9"
#endif
#ifndef LABEL_TOETS_S10
  #define LABEL_TOETS_S10 "Key S10"
#endif
#ifndef LABEL_TOETS_S11
  #define LABEL_TOETS_S11 "Key S11"
#endif
#ifndef LABEL_TOETS_S12
  #define LABEL_TOETS_S12 "Key S12"
#endif
#ifndef LABEL_TOETS_S13
  #define LABEL_TOETS_S13 "Key S13"
#endif
#ifndef LABEL_TOETS_S14
  #define LABEL_TOETS_S14 "Key S14"
#endif
#ifndef LABEL_TOETS_S15
  #define LABEL_TOETS_S15 "Key S15"
#endif
#ifndef LABEL_TOETS_S16
  #define LABEL_TOETS_S16 "Key S16"
#endif

// membranen & remote met 17 toetsen
#ifndef LABEL_TOETS_0
  #define LABEL_TOETS_0 "Key 0"
#endif
#ifndef LABEL_TOETS_1
  #define LABEL_TOETS_1 "Key 1"
#endif
#ifndef LABEL_TOETS_2
  #define LABEL_TOETS_2 "Key 2"
#endif
#ifndef LABEL_TOETS_3
  #define LABEL_TOETS_3 "Key 3"
#endif
#ifndef LABEL_TOETS_4
  #define LABEL_TOETS_4 "Key 4"
#endif
#ifndef LABEL_TOETS_5
  #define LABEL_TOETS_5 "Key 5"
#endif
#ifndef LABEL_TOETS_6
  #define LABEL_TOETS_6 "Key 6"
#endif
#ifndef LABEL_TOETS_7
  #define LABEL_TOETS_7 "Key 7"
#endif
#ifndef LABEL_TOETS_8
  #define LABEL_TOETS_8 "Key 8"
#endif
#ifndef LABEL_TOETS_9
  #define LABEL_TOETS_9 "Key 9"
#endif

// membranen 4x4
#ifndef LABEL_TOETS_A
  #define LABEL_TOETS_A "Key A"
#endif
#ifndef LABEL_TOETS_B
  #define LABEL_TOETS_B "Key B"
#endif
#ifndef LABEL_TOETS_C
  #define LABEL_TOETS_C "Key C"
#endif
#ifndef LABEL_TOETS_D
  #define LABEL_TOETS_D "Key D"
#endif

// remote met 17 toetsen
#ifndef LABEL_TOETS_STER
  #define LABEL_TOETS_STER "Key *"
#endif
#ifndef LABEL_TOETS_HEKJE
  #define LABEL_TOETS_HEKJE "Key #"
#endif

// extended remote met 17 toetsen
#ifndef LABEL_TOETS_UP
  #define LABEL_TOETS_UP "Navigation up"
#endif
#ifndef LABEL_TOETS_DOWN
  #define LABEL_TOETS_DOWN "Navigation down"
#endif
#ifndef LABEL_TOETS_OK
  #define LABEL_TOETS_OK "Navigation OK"
#endif
#ifndef LABEL_TOETS_LEFT
  #define LABEL_TOETS_LEFT "Navigation left"
#endif
#ifndef LABEL_TOETS_RIGHT
  #define LABEL_TOETS_RIGHT "Navigation right"
#endif

// extra MP3 remote met 21 toetsen
#ifndef LABEL_TOETS_CH_MINUS
  #define LABEL_TOETS_CH_MINUS "CH-"
#endif
#ifndef LABEL_TOETS_CH
  #define LABEL_TOETS_CH "CH"
#endif
#ifndef LABEL_TOETS_CH_PLUS
  #define LABEL_TOETS_CH_PLUS "CH+"
#endif
#ifndef LABEL_TOETS_PREV
  #define LABEL_TOETS_PREV "Previous"
#endif
#ifndef LABEL_TOETS_NEXT
  #define LABEL_TOETS_NEXT "Next"
#endif
#ifndef LABEL_TOETS_PLAY
  #define LABEL_TOETS_PLAY "Play/Pause"
#endif
#ifndef LABEL_TOETS_MINUS
  #define LABEL_TOETS_MINUS "-"
#endif
#ifndef LABEL_TOETS_PLUS
  #define LABEL_TOETS_PLUS "+"
#endif
#ifndef LABEL_TOETS_EQ
  #define LABEL_TOETS_EQ "EQ"
#endif
#ifndef LABEL_TOETS_100_PLUS
  #define LABEL_TOETS_100_PLUS "100+"
#endif
#ifndef LABEL_TOETS_200_PLUS
  #define LABEL_TOETS_200_PLUS "200+"
#endif

#endif
