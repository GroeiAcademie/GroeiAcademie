#ifndef GROEIACADEMIE_LIBRARY_FR_H
#define GROEIACADEMIE_LIBRARY_FR_H

// Cette version provisoire a été traduite automatiquement à partir de la version néerlandaise.
// Dans l'attente d'une vérification par une personne maîtrisant cette langue.

//=========================================================
// FATAL
//=========================================================
#ifndef FATAL_ZOEK_OP
  #define FATAL_ZOEK_OP "CHERCHEZ CECI"
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
  #define _LCD_ADS1115_NIET_GEVONDEN       "NON TROUVE"
#endif

// Input Type: HX1838
#ifndef _INPUT_HX1838_CONTROLE
  #define _INPUT_HX1838_CONTROLE                                   "CONTROLE IR:"
#endif
#ifndef _INPUT_HX1838_DRUK_OP_ELKE_TOETS_VOOR_CONTROLE
  #define _INPUT_HX1838_DRUK_OP_ELKE_TOETS_VOOR_CONTROLE           "CHAQUE TOUCHE"
#endif

#ifndef _INPUT_HX1838_CONTROLE_GESLAAGD
  #define _INPUT_HX1838_CONTROLE_GESLAAGD                          "CONTROLE IR: OK"
#endif

#ifndef _INPUT_HX1838_DRUK_NU_OP
  #define _INPUT_HX1838_DRUK_NU_OP                                 "APPUYEZ:"
#endif
// REGEL 2 IS DYNAMISCH: WEERGAVETEKST VAN DE TOETS.

// REGEL 1 IS DYNAMISCH: WEERGAVETEKST VAN DE HERKENDE TOETS.
#ifndef _INPUT_HX1838_TOETS_HERKEND
  #define _INPUT_HX1838_TOETS_HERKEND                              "RECONNUE"
#endif

#ifndef _INPUT_HX1838_KALIBRATIE_UITVOEREN
  #define _INPUT_HX1838_KALIBRATIE_UITVOEREN                       "CALIBRAGE"
#endif
#ifndef _INPUT_HX1838_KALIBRATIE_TIMEOUT
  #define _INPUT_HX1838_KALIBRATIE_TIMEOUT                          "DELAI DEPASSE"
#endif

#ifndef _INPUT_HX1838_KALIBRATIE_OPGESLAGEN
  #define _INPUT_HX1838_KALIBRATIE_OPGESLAGEN                      "ENREGISTRE"
#endif

#ifndef _INPUT_HX1838_MAPPING_EEPROM_GELADEN
  #define _INPUT_HX1838_MAPPING_EEPROM_GELADEN                     "MAPPING CHARGE"
#endif

#ifndef _INPUT_HX1838_MAPPING_GELADEN
  #define _INPUT_HX1838_MAPPING_GELADEN                            "MAPPING CHARGE"
#endif

#ifndef _INPUT_HX1838_INTERRUPT_KOPPELING_MISLUKT
  #define _INPUT_HX1838_INTERRUPT_KOPPELING_MISLUKT                "ERREUR PIN IR"
#endif

// Input Type: PCF8574
#ifndef _INPUT_PCF8574_FOUT
  #define _INPUT_PCF8574_FOUT                                      "ERREUR PCF8574"
#endif
#ifndef _INPUT_PCF8574_CONTROLEER_I2C
  #define _INPUT_PCF8574_CONTROLEER_I2C                            "VERIFIEZ I2C"
#endif

// Stimulus
#ifndef _LCD_KRACHT_TE_HARD
  #define _LCD_KRACHT_TE_HARD              "TROP FORT"
#endif
#ifndef _LCD_KRACHT_TE_ZACHT
  #define _LCD_KRACHT_TE_ZACHT             "TROP DOUX"
#endif

#ifndef _LCD_SCORE_TIKKRACHT
  #define _LCD_SCORE_TIKKRACHT             "FORCE TAP "
#endif
#ifndef _LCD_SCORE_TIKTIJD
  #define _LCD_SCORE_TIKTIJD               "TEMPS TAP "
#endif

#ifndef _LCD_TIJD_METEN_STOPT
  #define _LCD_TIJD_METEN_STOPT            "ON ARRETE"
#endif
#ifndef _LCD_TIJD_TEVEEL_FOUT
  #define _LCD_TIJD_TEVEEL_FOUT            "MAUVAIS DEPART"
#endif
#ifndef _LCD_TIJD_TE_KORT
  #define _LCD_TIJD_TE_KORT                "TROP COURT"
#endif
#ifndef _LCD_TIJD_TE_LANG
  #define _LCD_TIJD_TE_LANG                "TROP LONG"
#endif  

//=========================================================
// Serial
//=========================================================

// Input Type: HX1838
#ifndef _INPUT_HX1838_DRUK_NU_OP_SERIAL
  #define _INPUT_HX1838_DRUK_NU_OP_SERIAL                          "Appuyez maintenant sur : "
#endif

#ifndef _INPUT_HX1838_GEEN_GELDIGE_KALIBRATIE_SERIAL
  #define _INPUT_HX1838_GEEN_GELDIGE_KALIBRATIE_SERIAL             "Aucun calibrage IR valide trouvé."
#endif

#ifndef _INPUT_HX1838_TOETS_HERKEND_SERIAL
  #define _INPUT_HX1838_TOETS_HERKEND_SERIAL                       "Reconnue : "
#endif

#ifndef _INPUT_HX1838_MAPPING_EEPROM_GELADEN_SERIAL
  #define _INPUT_HX1838_MAPPING_EEPROM_GELADEN_SERIAL              "Mapping IR chargé depuis l'EEPROM :"
#endif

#ifndef _INPUT_HX1838_KALIBRATIE_GESTART_SERIAL
  #define _INPUT_HX1838_KALIBRATIE_GESTART_SERIAL                  "=== Calibrage IR démarré ==="
#endif

#ifndef _INPUT_HX1838_INTERRUPT_KOPPELING_MISLUKT_SERIAL
  #define _INPUT_HX1838_INTERRUPT_KOPPELING_MISLUKT_SERIAL         "Récepteur IR : l'interruption n'a pas pu être associée à HX1838_ONTVANGER_PIN."
#endif

#ifndef _INPUT_HX1838_KALIBRATIE_KLAAR_CONTROLE_SERIAL
  #define _INPUT_HX1838_KALIBRATIE_KLAAR_CONTROLE_SERIAL           "Calibrage terminé. Appuyez de nouveau sur toutes les touches pour contrôle."
#endif

#ifndef _INPUT_HX1838_KALIBRATIE_TIMEOUT_SERIAL
  #define _INPUT_HX1838_KALIBRATIE_TIMEOUT_SERIAL                   "Calibrage IR interrompu : délai dépassé."
#endif

#ifndef _INPUT_HX1838_KALIBRATIE_OPGESLAGEN_SERIAL
  #define _INPUT_HX1838_KALIBRATIE_OPGESLAGEN_SERIAL               "Calibrage enregistré."
#endif

#ifndef _INPUT_HX1838_MAPPING_CONFIG_GELADEN_SERIAL
  #define _INPUT_HX1838_MAPPING_CONFIG_GELADEN_SERIAL              "Mapping IR standard chargé depuis la configuration :"
#endif

#ifndef _INPUT_HX1838_MAPPING_CONFIG_ONVOLLEDIG
  #define _INPUT_HX1838_MAPPING_CONFIG_ONVOLLEDIG                  "MAPPING INCOMPLET"
#endif

#ifndef _INPUT_HX1838_MAPPING_CONFIG_ONVOLLEDIG_SERIAL
  #define _INPUT_HX1838_MAPPING_CONFIG_ONVOLLEDIG_SERIAL           "Le mapping IR fixe dans UserConfig.h est incomplet, tous les HX1838_CODE_x ne sont pas renseignés."
#endif

#ifndef _INPUT_HX1838_DEBUG_FAALDE
  #define _INPUT_HX1838_DEBUG_FAALDE                                      "a échoué"
#endif
#ifndef _INPUT_HX1838_DEBUG_MAGIC_MISMATCH
  #define _INPUT_HX1838_DEBUG_MAGIC_MISMATCH                              "magic incorrect"
#endif
#ifndef _INPUT_HX1838_DEBUG_VERSIE_MISMATCH
  #define _INPUT_HX1838_DEBUG_VERSIE_MISMATCH                             "version incompatible"
#endif
#ifndef _INPUT_HX1838_DEBUG_TOETSENINDELING_MISMATCH
  #define _INPUT_HX1838_DEBUG_TOETSENINDELING_MISMATCH                    "disposition des touches incompatible"
#endif
#ifndef _INPUT_HX1838_DEBUG_ALLE_TOETSEN_GEKALIBREER_START_VERIFICATIE
  #define _INPUT_HX1838_DEBUG_ALLE_TOETSEN_GEKALIBREER_START_VERIFICATIE  "toutes les touches calibrées, démarrage de la vérification"
#endif
#ifndef _INPUT_HX1838_DEBUG_VOOR_INIT
  #define _INPUT_HX1838_DEBUG_VOOR_INIT                                   "avant initialisation"
#endif
#ifndef _INPUT_HX1838_DEBUG_NA_INIT
  #define _INPUT_HX1838_DEBUG_NA_INIT                                     "après initialisation"
#endif

// Input Type: PCF8574
#ifndef _INPUT_PCF8574_NIET_BEREIKBAAR_SERIAL
  #define _INPUT_PCF8574_NIET_BEREIKBAAR_SERIAL                    "PCF8574 inaccessible à l'adresse I2C configurée."
#endif

//=========================================================
// INPUT - TOETSLABELS
//=========================================================
// drukknoppen
#ifndef LABEL_TOETS_S1
  #define LABEL_TOETS_S1 "Touche S1"
#endif
#ifndef LABEL_TOETS_S2
  #define LABEL_TOETS_S2 "Touche S2"
#endif
#ifndef LABEL_TOETS_S3
  #define LABEL_TOETS_S3 "Touche S3"
#endif
#ifndef LABEL_TOETS_S4
  #define LABEL_TOETS_S4 "Touche S4"
#endif
#ifndef LABEL_TOETS_S5
  #define LABEL_TOETS_S5 "Touche S5"
#endif
#ifndef LABEL_TOETS_S6
  #define LABEL_TOETS_S6 "Touche S6"
#endif
#ifndef LABEL_TOETS_S7
  #define LABEL_TOETS_S7 "Touche S7"
#endif
#ifndef LABEL_TOETS_S8
  #define LABEL_TOETS_S8 "Touche S8"
#endif
#ifndef LABEL_TOETS_S9
  #define LABEL_TOETS_S9 "Touche S9"
#endif
#ifndef LABEL_TOETS_S10
  #define LABEL_TOETS_S10 "Touche S10"
#endif
#ifndef LABEL_TOETS_S11
  #define LABEL_TOETS_S11 "Touche S11"
#endif
#ifndef LABEL_TOETS_S12
  #define LABEL_TOETS_S12 "Touche S12"
#endif
#ifndef LABEL_TOETS_S13
  #define LABEL_TOETS_S13 "Touche S13"
#endif
#ifndef LABEL_TOETS_S14
  #define LABEL_TOETS_S14 "Touche S14"
#endif
#ifndef LABEL_TOETS_S15
  #define LABEL_TOETS_S15 "Touche S15"
#endif
#ifndef LABEL_TOETS_S16
  #define LABEL_TOETS_S16 "Touche S16"
#endif

// membranen & remote met 17 toetsen
#ifndef LABEL_TOETS_0
  #define LABEL_TOETS_0 "Touche 0"
#endif
#ifndef LABEL_TOETS_1
  #define LABEL_TOETS_1 "Touche 1"
#endif
#ifndef LABEL_TOETS_2
  #define LABEL_TOETS_2 "Touche 2"
#endif
#ifndef LABEL_TOETS_3
  #define LABEL_TOETS_3 "Touche 3"
#endif
#ifndef LABEL_TOETS_4
  #define LABEL_TOETS_4 "Touche 4"
#endif
#ifndef LABEL_TOETS_5
  #define LABEL_TOETS_5 "Touche 5"
#endif
#ifndef LABEL_TOETS_6
  #define LABEL_TOETS_6 "Touche 6"
#endif
#ifndef LABEL_TOETS_7
  #define LABEL_TOETS_7 "Touche 7"
#endif
#ifndef LABEL_TOETS_8
  #define LABEL_TOETS_8 "Touche 8"
#endif
#ifndef LABEL_TOETS_9
  #define LABEL_TOETS_9 "Touche 9"
#endif

// membranen 4x4
#ifndef LABEL_TOETS_A
  #define LABEL_TOETS_A "Touche A"
#endif
#ifndef LABEL_TOETS_B
  #define LABEL_TOETS_B "Touche B"
#endif
#ifndef LABEL_TOETS_C
  #define LABEL_TOETS_C "Touche C"
#endif
#ifndef LABEL_TOETS_D
  #define LABEL_TOETS_D "Touche D"
#endif

// remote met 17 toetsen
#ifndef LABEL_TOETS_STER
  #define LABEL_TOETS_STER "Touche *"
#endif
#ifndef LABEL_TOETS_HEKJE
  #define LABEL_TOETS_HEKJE "Touche #"
#endif

// extended remote met 17 toetsen
#ifndef LABEL_TOETS_UP
  #define LABEL_TOETS_UP "Navigat. Haut"
#endif
#ifndef LABEL_TOETS_DOWN
  #define LABEL_TOETS_DOWN "Navigat. Bas"
#endif
#ifndef LABEL_TOETS_OK
  #define LABEL_TOETS_OK "Navigation OK"
#endif
#ifndef LABEL_TOETS_LEFT
  #define LABEL_TOETS_LEFT "Navigat. Gauche"
#endif
#ifndef LABEL_TOETS_RIGHT
  #define LABEL_TOETS_RIGHT "Navigat. Droite"
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
  #define LABEL_TOETS_PREV "Précédent"
#endif
#ifndef LABEL_TOETS_NEXT
  #define LABEL_TOETS_NEXT "Suivant"
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
