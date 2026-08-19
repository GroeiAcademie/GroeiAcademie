#ifndef GROEIACADEMIE_LIBRARY_NL_H
#define GROEIACADEMIE_LIBRARY_NL_H

//=========================================================
// FATAL
//=========================================================
#ifndef FATAL_ZOEK_OP
  #define FATAL_ZOEK_OP "ZOEK DIT NU OP"
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

//=========================================================
// CRITICAL
//=========================================================
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
  #define _LCD_ADS1115_NIET_GEVONDEN       "NIET GEVONDEN"
#endif

// Input Type: HX1838
#ifndef _INPUT_HX1838_CONTROLE
  #define _INPUT_HX1838_CONTROLE                                   "CONTROLE IR:"
#endif
#ifndef _INPUT_HX1838_DRUK_OP_ELKE_TOETS_VOOR_CONTROLE
  #define _INPUT_HX1838_DRUK_OP_ELKE_TOETS_VOOR_CONTROLE           "DRUK ELKE TOETS"
#endif

#ifndef _INPUT_HX1838_CONTROLE_GESLAAGD
  #define _INPUT_HX1838_CONTROLE_GESLAAGD                          "CONTROLE IR: OK"
#endif

#ifndef _INPUT_HX1838_DRUK_NU_OP
  #define _INPUT_HX1838_DRUK_NU_OP                                 "DRUK NU OP:"
#endif
// REGEL 2 IS DYNAMISCH: WEERGAVETEKST VAN DE TOETS.

// REGEL 1 IS DYNAMISCH: WEERGAVETEKST VAN DE HERKENDE TOETS.
#ifndef _INPUT_HX1838_TOETS_HERKEND
  #define _INPUT_HX1838_TOETS_HERKEND                              "HERKEND"
#endif

#ifndef _INPUT_HX1838_KALIBRATIE_UITVOEREN
  #define _INPUT_HX1838_KALIBRATIE_UITVOEREN                       "KALIBRATIE"
#endif
#ifndef _INPUT_HX1838_KALIBRATIE_TIMEOUT
  #define _INPUT_HX1838_KALIBRATIE_TIMEOUT                          "TIME-OUT"
#endif

#ifndef _INPUT_HX1838_KALIBRATIE_OPGESLAGEN
  #define _INPUT_HX1838_KALIBRATIE_OPGESLAGEN                      "OPGESLAGEN"
#endif

#ifndef _INPUT_HX1838_MAPPING_EEPROM_GELADEN
  #define _INPUT_HX1838_MAPPING_EEPROM_GELADEN                     "MAPPING GELADEN"
#endif

#ifndef _INPUT_HX1838_MAPPING_GELADEN
  #define _INPUT_HX1838_MAPPING_GELADEN                            "MAPPING GELADEN"
#endif

#ifndef _INPUT_HX1838_INTERRUPT_KOPPELING_MISLUKT
  #define _INPUT_HX1838_INTERRUPT_KOPPELING_MISLUKT                "IR PIN FOUT"
#endif

// Input Type: PCF8574
#ifndef _INPUT_PCF8574_FOUT
  #define _INPUT_PCF8574_FOUT                                      "PCF8574 FOUT"
#endif
#ifndef _INPUT_PCF8574_CONTROLEER_I2C
  #define _INPUT_PCF8574_CONTROLEER_I2C                            "CONTROLEER I2C"
#endif

// Stimulus
#ifndef _LCD_KRACHT_TE_HARD
  #define _LCD_KRACHT_TE_HARD              "TE HARD"
#endif
#ifndef _LCD_KRACHT_TE_ZACHT
  #define _LCD_KRACHT_TE_ZACHT             "TE ZACHT"
#endif

#ifndef _LCD_SCORE_TIKKRACHT
  #define _LCD_SCORE_TIKKRACHT             "TIKKRACHT "
#endif
#ifndef _LCD_SCORE_TIKTIJD
  #define _LCD_SCORE_TIKTIJD               "TIKTIJD "
#endif

#ifndef _LCD_TIJD_METEN_STOPT
  #define _LCD_TIJD_METEN_STOPT            "WE STOPPEN ERMEE"
#endif
#ifndef _LCD_TIJD_TEVEEL_FOUT
  #define _LCD_TIJD_TEVEEL_FOUT            "SLECHTE START :)"
#endif
#ifndef _LCD_TIJD_TE_KORT
  #define _LCD_TIJD_TE_KORT                "TE KORT"
#endif
#ifndef _LCD_TIJD_TE_LANG
  #define _LCD_TIJD_TE_LANG                "TE LANG"
#endif  

//=========================================================
// Serial
//=========================================================

// Input Type: HX1838
#ifndef _INPUT_HX1838_DRUK_NU_OP_SERIAL
  #define _INPUT_HX1838_DRUK_NU_OP_SERIAL                          "Druk nu op: "
#endif

#ifndef _INPUT_HX1838_GEEN_GELDIGE_KALIBRATIE_SERIAL
  #define _INPUT_HX1838_GEEN_GELDIGE_KALIBRATIE_SERIAL             "Geen geldige IR-kalibratie gevonden."
#endif

#ifndef _INPUT_HX1838_TOETS_HERKEND_SERIAL
  #define _INPUT_HX1838_TOETS_HERKEND_SERIAL                       "Herkend: "
#endif

#ifndef _INPUT_HX1838_MAPPING_EEPROM_GELADEN_SERIAL
  #define _INPUT_HX1838_MAPPING_EEPROM_GELADEN_SERIAL              "IR-mapping uit EEPROM geladen:"
#endif

#ifndef _INPUT_HX1838_KALIBRATIE_GESTART_SERIAL
  #define _INPUT_HX1838_KALIBRATIE_GESTART_SERIAL                  "=== IR-kalibratie gestart ==="
#endif

#ifndef _INPUT_HX1838_INTERRUPT_KOPPELING_MISLUKT_SERIAL
  #define _INPUT_HX1838_INTERRUPT_KOPPELING_MISLUKT_SERIAL         "IR-ontvanger: interrupt kon niet gekoppeld worden aan HX1838_ONTVANGER_PIN."
#endif

#ifndef _INPUT_HX1838_KALIBRATIE_KLAAR_CONTROLE_SERIAL
  #define _INPUT_HX1838_KALIBRATIE_KLAAR_CONTROLE_SERIAL           "Kalibratie klaar. Druk alle toetsen opnieuw in ter controle."
#endif

#ifndef _INPUT_HX1838_KALIBRATIE_TIMEOUT_SERIAL
  #define _INPUT_HX1838_KALIBRATIE_TIMEOUT_SERIAL                   "IR-kalibratie afgebroken: timeout."
#endif

#ifndef _INPUT_HX1838_KALIBRATIE_OPGESLAGEN_SERIAL
  #define _INPUT_HX1838_KALIBRATIE_OPGESLAGEN_SERIAL               "Kalibratie opgeslagen."
#endif

#ifndef _INPUT_HX1838_MAPPING_CONFIG_GELADEN_SERIAL
  #define _INPUT_HX1838_MAPPING_CONFIG_GELADEN_SERIAL              "Standaard IR-mapping uit configuratie geladen:"
#endif

#ifndef _INPUT_HX1838_MAPPING_CONFIG_ONVOLLEDIG
  #define _INPUT_HX1838_MAPPING_CONFIG_ONVOLLEDIG                  "MAPPING ONVOLLEDIG"
#endif

#ifndef _INPUT_HX1838_MAPPING_CONFIG_ONVOLLEDIG_SERIAL
  #define _INPUT_HX1838_MAPPING_CONFIG_ONVOLLEDIG_SERIAL           "Vaste IR-mapping in UserConfig.h is onvolledig, niet alle HX1838_CODE_x zijn ingevuld."
#endif

#ifndef _INPUT_HX1838_DEBUG_FAALDE
  #define _INPUT_HX1838_DEBUG_FAALDE                                      "faalde"
#endif
#ifndef _INPUT_HX1838_DEBUG_MAGIC_MISMATCH
  #define _INPUT_HX1838_DEBUG_MAGIC_MISMATCH                              "magic mismatch"
#endif
#ifndef _INPUT_HX1838_DEBUG_VERSIE_MISMATCH
  #define _INPUT_HX1838_DEBUG_VERSIE_MISMATCH                             "versie mismatch"
#endif
#ifndef _INPUT_HX1838_DEBUG_TOETSENINDELING_MISMATCH
  #define _INPUT_HX1838_DEBUG_TOETSENINDELING_MISMATCH                    "toetsenindeling mismatch"
#endif
#ifndef _INPUT_HX1838_DEBUG_ALLE_TOETSEN_GEKALIBREER_START_VERIFICATIE
  #define _INPUT_HX1838_DEBUG_ALLE_TOETSEN_GEKALIBREER_START_VERIFICATIE  "alle toetsen gekalibreerd, start verificatie"
#endif
#ifndef _INPUT_HX1838_DEBUG_VOOR_INIT
  #define _INPUT_HX1838_DEBUG_VOOR_INIT                                   "voor init"
#endif
#ifndef _INPUT_HX1838_DEBUG_NA_INIT
  #define _INPUT_HX1838_DEBUG_NA_INIT                                     "na init"
#endif

// Input Type: PCF8574
#ifndef _INPUT_PCF8574_NIET_BEREIKBAAR_SERIAL
  #define _INPUT_PCF8574_NIET_BEREIKBAAR_SERIAL                    "PCF8574 niet bereikbaar op ingesteld I2C-adres."
#endif

//=========================================================
// INPUT - TOETSLABELS
//=========================================================
// drukknoppen
#ifndef LABEL_TOETS_S1
  #define LABEL_TOETS_S1 "Toets S1"
#endif
#ifndef LABEL_TOETS_S2
  #define LABEL_TOETS_S2 "Toets S2"
#endif
#ifndef LABEL_TOETS_S3
  #define LABEL_TOETS_S3 "Toets S3"
#endif
#ifndef LABEL_TOETS_S4
  #define LABEL_TOETS_S4 "Toets S4"
#endif
#ifndef LABEL_TOETS_S5
  #define LABEL_TOETS_S5 "Toets S5"
#endif
#ifndef LABEL_TOETS_S6
  #define LABEL_TOETS_S6 "Toets S6"
#endif
#ifndef LABEL_TOETS_S7
  #define LABEL_TOETS_S7 "Toets S7"
#endif
#ifndef LABEL_TOETS_S8
  #define LABEL_TOETS_S8 "Toets S8"
#endif
#ifndef LABEL_TOETS_S9
  #define LABEL_TOETS_S9 "Toets S9"
#endif
#ifndef LABEL_TOETS_S10
  #define LABEL_TOETS_S10 "Toets S10"
#endif
#ifndef LABEL_TOETS_S11
  #define LABEL_TOETS_S11 "Toets S11"
#endif
#ifndef LABEL_TOETS_S12
  #define LABEL_TOETS_S12 "Toets S12"
#endif
#ifndef LABEL_TOETS_S13
  #define LABEL_TOETS_S13 "Toets S13"
#endif
#ifndef LABEL_TOETS_S14
  #define LABEL_TOETS_S14 "Toets S14"
#endif
#ifndef LABEL_TOETS_S15
  #define LABEL_TOETS_S15 "Toets S15"
#endif
#ifndef LABEL_TOETS_S16
  #define LABEL_TOETS_S16 "Toets S16"
#endif

// membranen & remote met 17 toetsen
#ifndef LABEL_TOETS_0
  #define LABEL_TOETS_0 "Toets 0"
#endif
#ifndef LABEL_TOETS_1
  #define LABEL_TOETS_1 "Toets 1"
#endif
#ifndef LABEL_TOETS_2
  #define LABEL_TOETS_2 "Toets 2"
#endif
#ifndef LABEL_TOETS_3
  #define LABEL_TOETS_3 "Toets 3"
#endif
#ifndef LABEL_TOETS_4
  #define LABEL_TOETS_4 "Toets 4"
#endif
#ifndef LABEL_TOETS_5
  #define LABEL_TOETS_5 "Toets 5"
#endif
#ifndef LABEL_TOETS_6
  #define LABEL_TOETS_6 "Toets 6"
#endif
#ifndef LABEL_TOETS_7
  #define LABEL_TOETS_7 "Toets 7"
#endif
#ifndef LABEL_TOETS_8
  #define LABEL_TOETS_8 "Toets 8"
#endif
#ifndef LABEL_TOETS_9
  #define LABEL_TOETS_9 "Toets 9"
#endif

// membranen 4x4
#ifndef LABEL_TOETS_A
  #define LABEL_TOETS_A "Toets A"
#endif
#ifndef LABEL_TOETS_B
  #define LABEL_TOETS_B "Toets B"
#endif
#ifndef LABEL_TOETS_C
  #define LABEL_TOETS_C "Toets C"
#endif
#ifndef LABEL_TOETS_D
  #define LABEL_TOETS_D "Toets D"
#endif

// remote met 17 toetsen
#ifndef LABEL_TOETS_STER
  #define LABEL_TOETS_STER "Toets *"
#endif
#ifndef LABEL_TOETS_HEKJE
  #define LABEL_TOETS_HEKJE "Toets #"
#endif

// extended remote met 17 toetsen
#ifndef LABEL_TOETS_UP
  #define LABEL_TOETS_UP "Navigatie omhoog"
#endif
#ifndef LABEL_TOETS_DOWN
  #define LABEL_TOETS_DOWN "Navigatie omlaag"
#endif
#ifndef LABEL_TOETS_OK
  #define LABEL_TOETS_OK "Navigatie OK"
#endif
#ifndef LABEL_TOETS_LEFT
  #define LABEL_TOETS_LEFT "Navigatie links"
#endif
#ifndef LABEL_TOETS_RIGHT
  #define LABEL_TOETS_RIGHT "Navigatie rechts"
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
  #define LABEL_TOETS_PREV "Vorige"
#endif
#ifndef LABEL_TOETS_NEXT
  #define LABEL_TOETS_NEXT "Volgende"
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
