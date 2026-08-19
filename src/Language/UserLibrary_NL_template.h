#ifndef GROEIACADEMIE_USER_LIBRARY_NL_H
#define GROEIACADEMIE_USER_LIBRARY_NL_H

// Kopieer dit bestand naar UserLibrary_NL.h in dezelfde map.
// Alle aanpasbare teksten uit het overeenkomstige officiële taalbestand staan hieronder als voorbeeld.
// Vaste systeemfoutcodes (_FATAL_CSxxx, _FATAL_PSxxx en _CRITICAL_SSxxx) zijn bewust niet aanpasbaar.

//=========================================================
// FATAL
//=========================================================
#define FATAL_ZOEK_OP "ZOEK DIT NU OP"

//=========================================================
// PrintToScreen
//=========================================================
// ADS1115
#define _LCD_ADS1115_FOUT           "ADS1115"
#define _LCD_ADS1115_NIET_GEVONDEN  "NIET GEVONDEN"

// Input Type: HX1838
#define _INPUT_HX1838_CONTROLE                                   "CONTROLE IR:"
#define _INPUT_HX1838_DRUK_OP_ELKE_TOETS_VOOR_CONTROLE           "DRUK ELKE TOETS"

#define _INPUT_HX1838_CONTROLE_GESLAAGD                          "CONTROLE IR: OK"

#define _INPUT_HX1838_DRUK_NU_OP                                 "DRUK NU OP:"
// REGEL 2 IS DYNAMISCH: WEERGAVETEKST VAN DE TOETS.

// REGEL 1 IS DYNAMISCH: WEERGAVETEKST VAN DE HERKENDE TOETS.
#define _INPUT_HX1838_TOETS_HERKEND                              "HERKEND"

#define _INPUT_HX1838_KALIBRATIE_UITVOEREN                       "KALIBRATIE"
#define _INPUT_HX1838_KALIBRATIE_OPGESLAGEN                      "OPGESLAGEN"
#define _INPUT_HX1838_KALIBRATIE_TIMEOUT                         "TIME-OUT"

#define _INPUT_HX1838_MAPPING_EEPROM_GELADEN                     "MAPPING GELADEN"

#define _INPUT_HX1838_MAPPING_GELADEN                            "MAPPING GELADEN"

#define _INPUT_HX1838_INTERRUPT_KOPPELING_MISLUKT                "IR PIN FOUT"

// Input Type: PCF8574
#define _INPUT_PCF8574_FOUT                                      "PCF8574 FOUT"
#define _INPUT_PCF8574_CONTROLEER_I2C                            "CONTROLEER I2C"

// Stimulus
#define _LCD_KRACHT_TE_HARD              "TE HARD"
#define _LCD_KRACHT_TE_ZACHT             "TE ZACHT"

#define _LCD_SCORE_TIKKRACHT             "TIKKRACHT "
#define _LCD_SCORE_TIKTIJD               "TIKTIJD "

#define _LCD_TIJD_METEN_STOPT            "WE STOPPEN ERMEE"
#define _LCD_TIJD_TEVEEL_FOUT            "SLECHTE START :)"
#define _LCD_TIJD_TE_KORT                "TE KORT"
#define _LCD_TIJD_TE_LANG                "TE LANG"

//=========================================================
// Serial
//=========================================================

// Input Type: HX1838
#define _INPUT_HX1838_DRUK_NU_OP_SERIAL                         "Druk nu op: "
#define _INPUT_HX1838_GEEN_GELDIGE_KALIBRATIE_SERIAL            "Geen geldige IR-kalibratie gevonden."
#define _INPUT_HX1838_MAPPING_EEPROM_GELADEN_SERIAL              "IR-mapping uit EEPROM geladen:"
#define _INPUT_HX1838_KALIBRATIE_GESTART_SERIAL                 "=== IR-kalibratie gestart ==="
#define _INPUT_HX1838_KALIBRATIE_TIMEOUT_SERIAL                 "IR-kalibratie afgebroken: timeout."

#define _INPUT_HX1838_INTERRUPT_KOPPELING_MISLUKT_SERIAL        "IR-ontvanger: interrupt kon niet gekoppeld worden aan HX1838_ONTVANGER_PIN."
#define _INPUT_HX1838_KALIBRATIE_KLAAR_CONTROLE_SERIAL          "Kalibratie klaar. Druk alle toetsen opnieuw in ter controle."
#define _INPUT_HX1838_KALIBRATIE_OPGESLAGEN_SERIAL              "Kalibratie opgeslagen."
#define _INPUT_HX1838_MAPPING_CONFIG_GELADEN_SERIAL             "Standaard IR-mapping uit configuratie geladen:"

#define _INPUT_HX1838_MAPPING_CONFIG_ONVOLLEDIG                 "MAPPING ONVOLLEDIG"

#define _INPUT_HX1838_MAPPING_CONFIG_ONVOLLEDIG_SERIAL          "Vaste IR-mapping in UserConfig.h is onvolledig, niet alle HX1838_CODE_x zijn ingevuld."
#define _INPUT_HX1838_TOETS_HERKEND_SERIAL                      "Herkend: "

  #define _INPUT_HX1838_DEBUG_FAALDE                                      "faalde"
  #define _INPUT_HX1838_DEBUG_MAGIC_MISMATCH                              "magic mismatch"
  #define _INPUT_HX1838_DEBUG_VERSIE_MISMATCH                             "versie mismatch"
  #define _INPUT_HX1838_DEBUG_TOETSENINDELING_MISMATCH                    "toetsenindeling mismatch"
  #define _INPUT_HX1838_DEBUG_ALLE_TOETSEN_GEKALIBREER_START_VERIFICATIE  "alle toetsen gekalibreerd, start verificatie"
  #define _INPUT_HX1838_DEBUG_VOOR_INIT                                   "voor init"
  #define _INPUT_HX1838_DEBUG_NA_INIT                                     "na init"

// Input Type: PCF8574
#define _INPUT_PCF8574_NIET_BEREIKBAAR_SERIAL                   "PCF8574 niet bereikbaar op ingesteld I2C-adres."

//=========================================================
// INPUT - TOETSLABELS
//=========================================================
// drukknoppen
#define LABEL_TOETS_S1               "Toets S1"
#define LABEL_TOETS_S2               "Toets S2"
#define LABEL_TOETS_S3               "Toets S3"
#define LABEL_TOETS_S4               "Toets S4"
#define LABEL_TOETS_S5               "Toets S5"
#define LABEL_TOETS_S6               "Toets S6"
#define LABEL_TOETS_S7               "Toets S7"
#define LABEL_TOETS_S8               "Toets S8"
#define LABEL_TOETS_S9               "Toets S9"
#define LABEL_TOETS_S10              "Toets S10"
#define LABEL_TOETS_S11              "Toets S11"
#define LABEL_TOETS_S12              "Toets S12"
#define LABEL_TOETS_S13              "Toets S13"
#define LABEL_TOETS_S14              "Toets S14"
#define LABEL_TOETS_S15              "Toets S15"
#define LABEL_TOETS_S16              "Toets S16"

// membranen & remote met 17 toetsen
#define LABEL_TOETS_0                "Toets 0"
#define LABEL_TOETS_1                "Toets 1"
#define LABEL_TOETS_2                "Toets 2"
#define LABEL_TOETS_3                "Toets 3"
#define LABEL_TOETS_4                "Toets 4"
#define LABEL_TOETS_5                "Toets 5"
#define LABEL_TOETS_6                "Toets 6"
#define LABEL_TOETS_7                "Toets 7"
#define LABEL_TOETS_8                "Toets 8"
#define LABEL_TOETS_9                "Toets 9"

// membranen 4x4
#define LABEL_TOETS_A                "Toets A"
#define LABEL_TOETS_B                "Toets B"
#define LABEL_TOETS_C                "Toets C"
#define LABEL_TOETS_D                "Toets D"

// remote met 17 toetsen
#define LABEL_TOETS_STER             "Toets *"
#define LABEL_TOETS_HEKJE            "Toets #"

// extended remote met 17 toetsen
#define LABEL_TOETS_UP               "Navigatie omhoog"
#define LABEL_TOETS_DOWN             "Navigatie omlaag"
#define LABEL_TOETS_OK               "Navigatie OK"
#define LABEL_TOETS_LEFT             "Navigatie links"
#define LABEL_TOETS_RIGHT            "Navigatie rechts"

// extra MP3 remote met 21 toetsen
#define LABEL_TOETS_CH_MINUS         "CH-"
#define LABEL_TOETS_CH               "CH"
#define LABEL_TOETS_CH_PLUS          "CH+"
#define LABEL_TOETS_PREV             "Vorige"
#define LABEL_TOETS_NEXT             "Volgende"
#define LABEL_TOETS_PLAY             "Play/Pause"
#define LABEL_TOETS_MINUS            "-"
#define LABEL_TOETS_PLUS             "+"
#define LABEL_TOETS_EQ               "EQ"
#define LABEL_TOETS_100_PLUS         "100+"
#define LABEL_TOETS_200_PLUS         "200+"

#endif
