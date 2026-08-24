#ifndef GROEIACADEMIE_USER_LIBRARY_FR_H
#define GROEIACADEMIE_USER_LIBRARY_FR_H

// Cette version provisoire a été traduite automatiquement à partir de la version néerlandaise.
// Dans l'attente d'une vérification par une personne maîtrisant cette langue.

// Kopieer dit bestand naar UserLibrary_FR.h in dezelfde map.
// Alle aanpasbare teksten uit het overeenkomstige officiële taalbestand staan hieronder als voorbeeld.
// Vaste systeemfoutcodes (_FATAL_CSxxx, _FATAL_PSxxx en _CRITICAL_SSxxx) zijn bewust niet aanpasbaar.
//=========================================================
// FATAL
//=========================================================
  #define FATAL_ZOEK_OP "CHERCHEZ CECI"


// PixelScreen

// InputKanalen

//=========================================================
// PrintToScreen
//=========================================================
// ADS1115
  #define _LCD_ADS1115_FOUT                "ADS1115"
  #define _LCD_ADS1115_NIET_GEVONDEN       "NON TROUVE"

// Input Type: HX1838
  #define _INPUT_HX1838_CONTROLE                                   "CONTROLE IR"
  #define _INPUT_HX1838_DRUK_OP_ELKE_TOETS_VOOR_CONTROLE           "CHAQUE TOUCHE"

  #define _INPUT_HX1838_CONTROLE_GESLAAGD                          "CONTROLE IR: OK"

  #define _INPUT_HX1838_DRUK_NU_OP                                 "APPUYEZ:"
// REGEL 2 IS DYNAMISCH: WEERGAVETEKST VAN DE TOETS.

// REGEL 1 IS DYNAMISCH: WEERGAVETEKST VAN DE HERKENDE TOETS.
  #define _INPUT_HX1838_TOETS_HERKEND                              "RECONNUE"

  #define _INPUT_HX1838_KALIBRATIE_UITVOEREN                       "CALIBRAGE"
  #define _INPUT_HX1838_KALIBRATIE_OPGESLAGEN                      "ENREGISTRE"
  #define _INPUT_HX1838_KALIBRATIE_TIMEOUT                         "DELAI DEPASSE"

  #define _INPUT_HX1838_MAPPING_EEPROM_GELADEN                     "MAPPING CHARGE"

  #define _INPUT_HX1838_MAPPING_GELADEN                            "MAPPING CHARGE"

  #define _INPUT_HX1838_INTERRUPT_KOPPELING_MISLUKT                "ERREUR PIN IR"

  #define _INPUT_MAPPINGCONTROLE_TITEL                             "Controle du mapping"
  #define _INPUT_MAPPINGCONTROLE_OPSCHRIFTEN_OK                    "ETIQUETTES OK"

  #define _INPUT_MAPPINGCONTROLE_WAARSCHUWING                      "AVERTISSEMENT"
  #define _INPUT_MAPPINGCONTROLE_ONTBREEKT                         "Manquant"

  #define _INPUT_HX1838_GENERIEK_CODES_GEKALIBREERD                "CALIBRE"
  #define _INPUT_HX1838_GENERIEK_CODES_ZIE_SERIEEL                 "VOIR SERIE"

// Input Type: PCF8574
  #define _INPUT_PCF8574_FOUT                                      "ERREUR PCF8574"
  #define _INPUT_PCF8574_CONTROLEER_I2C                            "VERIFIEZ I2C"
  #define _INPUT_PCF8574_ADRES_LABEL                                "Adresse I2C"

// Stimulus
  #define _LCD_KRACHT_TE_HARD              "TROP FORT"
  #define _LCD_KRACHT_TE_ZACHT             "TROP DOUX"

  #define _LCD_SCORE_TIKKRACHT             "FORCE TAP "
  #define _LCD_SCORE_TIKTIJD               "TEMPS TAP "

  #define _LCD_TIJD_METEN_STOPT            "ON ARRETE"
  #define _LCD_TIJD_TEVEEL_FOUT            "MAUVAIS DEPART"
  #define _LCD_TIJD_TE_KORT                "TROP COURT"
  #define _LCD_TIJD_TE_LANG                "TROP LONG"

//=========================================================
// Serial
//=========================================================

// Input Type: HX1838
  #define _INPUT_HX1838_DRUK_NU_OP_SERIAL                          "Appuyez maintenant sur : "
  #define _INPUT_HX1838_GEEN_GELDIGE_KALIBRATIE_SERIAL             "Aucun calibrage IR valide trouvé."
  #define _INPUT_HX1838_MAPPING_EEPROM_GELADEN_SERIAL              "Mapping IR chargé depuis l'EEPROM :"
  #define _INPUT_HX1838_KALIBRATIE_GESTART_SERIAL                  "=== Calibrage IR démarré ==="
  #define _INPUT_HX1838_KALIBRATIE_TIMEOUT_SERIAL                  "Calibrage IR interrompu : délai dépassé."

  #define _INPUT_HX1838_INTERRUPT_KOPPELING_MISLUKT_SERIAL         "Récepteur IR : l'interruption n'a pas pu être associée à HX1838_ONTVANGER_PIN."
  #define _INPUT_HX1838_KALIBRATIE_KLAAR_CONTROLE_SERIAL           "Calibrage terminé. Appuyez de nouveau sur toutes les touches pour contrôle."
  #define _INPUT_HX1838_KALIBRATIE_OPGESLAGEN_SERIAL               "Calibrage enregistré."
  #define _INPUT_HX1838_MAPPING_CONFIG_GELADEN_SERIAL              "Mapping IR standard chargé depuis la configuration :"

  #define _INPUT_HX1838_MAPPING_CONFIG_ONVOLLEDIG                  "MAPPING INCOMPLET"

  #define _INPUT_HX1838_MAPPING_CONFIG_ONVOLLEDIG_SERIAL           "Le mapping IR fixe dans UserConfig.h est incomplet, tous les HX1838_CODE_x ne sont pas renseignés."
  #define _INPUT_HX1838_TOETS_HERKEND_SERIAL                       "Reconnue : "

  #define _INPUT_HX1838_DEBUG_FAALDE                                      "a échoué"
  #define _INPUT_HX1838_DEBUG_MAGIC_MISMATCH                              "magic incorrect"
  #define _INPUT_HX1838_DEBUG_VERSIE_MISMATCH                             "version incompatible"
  #define _INPUT_HX1838_DEBUG_TOETSENINDELING_MISMATCH                    "disposition des touches incompatible"
  #define _INPUT_HX1838_DEBUG_ALLE_TOETSEN_GEKALIBREER_START_VERIFICATIE  "toutes les touches calibrées, démarrage de la vérification"
  #define _INPUT_HX1838_DEBUG_VOOR_INIT                                   "avant initialisation"
  #define _INPUT_HX1838_DEBUG_NA_INIT                                     "après initialisation"

  #define _INPUT_HX1838_DEBUG_GEEN_MATCH                                  "AUCUNE CORRESPONDANCE"
  #define _INPUT_HX1838_DEBUG_IR_ONTVANGEN                                "IR recu"
  #define _INPUT_HX1838_DEBUG_MATCH                                       "MATCH"
  #define _INPUT_HX1838_DEBUG_REGEL                                       "ligne"
  #define _INPUT_HX1838_DEBUG_TOETS                                       "touche"
  #define _INPUT_HX1838_DEBUG_VERGELEKEN_MET                              "compare a"

  #define _INPUT_HX1838_GENERIEK_CODES_NIET_GEDEFINIEERD_KALIBRATIE_GESTART   "non defini, calibration demarree."
  #define _INPUT_HX1838_GENERIEK_CODES_GEKALIBREERD_KOPIEER_REGEL             "Codes calibres, copiez la ligne ci-dessus dans"

// Input Type: PCF8574
  #define _INPUT_PCF8574_NIET_BEREIKBAAR_SERIAL                    "PCF8574 inaccessible à l'adresse I2C configurée."

//=========================================================
// Static Assert
//=========================================================
// Input Type: HX1838
  #define _INPUT_HX1838_STATIC_ASSERT_AANTAL_TOETSEN               "ne contient pas le nombre attendu de touches HX1838."
  #define _INPUT_HX1838_STATIC_ASSERT_CODES_BEVATTEN               "codes."
  #define _INPUT_HX1838_STATIC_ASSERT_GEEN_CODE_WAARDE_NUL         "contenir de code avec la valeur 0."
  #define _INPUT_HX1838_STATIC_ASSERT_MAG_BIJ                      "ne peut, avec"
  #define _INPUT_HX1838_STATIC_ASSERT_MOET_EXACT                   "doit contenir exactement"

// Input Type: PCF8574
  #define _INPUT_PCF8574_STATIC_ASSERT_DIRECT_MAX_ACHT_PINNEN      "Le clavier generique direct utilise plus de 8 broches PCF8574."
  #define _INPUT_PCF8574_STATIC_ASSERT_DIRECT_MIN_EEN_PIN          "Le clavier generique direct doit utiliser au moins une broche PCF8574."
  #define _INPUT_PCF8574_STATIC_ASSERT_DIRECT_PINNEN_MAX_ACHT      "Le clavier direct utilise plus de 8 broches PCF8574."
  #define _INPUT_PCF8574_STATIC_ASSERT_KEY_LAYOUT_DIRECT_AANTAL    "doit contenir exactement une entree par broche de clavier direct."
  #define _INPUT_PCF8574_STATIC_ASSERT_KEY_LAYOUT_MATRIX_AANTAL    "doit contenir exactement une entree par position de matrice."
  #define _INPUT_PCF8574_STATIC_ASSERT_MATRIX_DUBBELE_PINNEN       "La matrice generique contient des broches de ligne ou de colonne en double."
  #define _INPUT_PCF8574_STATIC_ASSERT_MATRIX_MAX_ACHT_PINNEN      "Le clavier matriciel generique utilise plus de 8 broches PCF8574."
  #define _INPUT_PCF8574_STATIC_ASSERT_MATRIX_MIN_EEN_KOLOM        "Le clavier matriciel generique doit avoir au moins une colonne."
  #define _INPUT_PCF8574_STATIC_ASSERT_MATRIX_MIN_EEN_RIJ          "Le clavier matriciel generique doit avoir au moins une ligne."
  #define _INPUT_PCF8574_STATIC_ASSERT_MATRIX_PINNEN_MAX_ACHT      "La matrice utilise plus de 8 broches PCF8574."
  #define _INPUT_PCF8574_STATIC_ASSERT_PINNEN_BEREIK               "ne peut utiliser que les positions de bits PCF8574 0 a 7."
  #define _INPUT_PCF8574_STATIC_ASSERT_PINNEN_UNIEK                "contient des broches PCF8574 en double."
  #define _INPUT_PCF8574_STATIC_ASSERT_RIJ_KOLOM_BEREIK            "ne peuvent utiliser que les positions de bits PCF8574 0 a 7."
  #define _INPUT_PCF8574_STATIC_ASSERT_RIJ_KOLOM_EN                "et"
  #define _INPUT_PCF8574_STATIC_ASSERT_RIJ_KOLOM_OVERLAP           "La meme broche PCF8574 ne peut pas etre a la fois une broche de ligne et de colonne."

//=========================================================
// INPUT - TOETSLABELS
//=========================================================
  #define LABEL_TOETS_0 "Touche 0"
  #define LABEL_TOETS_1 "Touche 1"
  #define LABEL_TOETS_2 "Touche 2"
  #define LABEL_TOETS_3 "Touche 3"
  #define LABEL_TOETS_4 "Touche 4"
  #define LABEL_TOETS_5 "Touche 5"
  #define LABEL_TOETS_6 "Touche 6"
  #define LABEL_TOETS_7 "Touche 7"
  #define LABEL_TOETS_8 "Touche 8"
  #define LABEL_TOETS_9 "Touche 9"

  #define LABEL_TOETS_A "Touche A"
  #define LABEL_TOETS_B "Touche B"
  #define LABEL_TOETS_C "Touche C"
  #define LABEL_TOETS_D "Touche D"

  #define LABEL_TOETS_S1 "Touche S1"
  #define LABEL_TOETS_S2 "Touche S2"
  #define LABEL_TOETS_S3 "Touche S3"
  #define LABEL_TOETS_S4 "Touche S4"
  #define LABEL_TOETS_S5 "Touche S5"
  #define LABEL_TOETS_S6 "Touche S6"
  #define LABEL_TOETS_S7 "Touche S7"
  #define LABEL_TOETS_S8 "Touche S8"
  #define LABEL_TOETS_S9 "Touche S9"
  #define LABEL_TOETS_S10 "Touche S10"
  #define LABEL_TOETS_S11 "Touche S11"
  #define LABEL_TOETS_S12 "Touche S12"
  #define LABEL_TOETS_S13 "Touche S13"
  #define LABEL_TOETS_S14 "Touche S14"
  #define LABEL_TOETS_S15 "Touche S15"
  #define LABEL_TOETS_S16 "Touche S16"

  #define LABEL_TOETS_STER "Touche *"
  #define LABEL_TOETS_HEKJE "Touche #"

// extended 
  #define LABEL_TOETS_UP    "Navigation haut"
  #define LABEL_TOETS_DOWN  "Navigation bas"
  #define LABEL_TOETS_OK    "Navigation OK"
  #define LABEL_TOETS_LEFT  "Navigation gauche"
  #define LABEL_TOETS_RIGHT "Navigation droite"

// MP3-afstandsbediening
  #define LABEL_TOETS_CH_MINUS "CH-"
  #define LABEL_TOETS_CH "CH"
  #define LABEL_TOETS_CH_PLUS "CH+"
  #define LABEL_TOETS_PREV "Précédent"
  #define LABEL_TOETS_NEXT "Suivant"
  #define LABEL_TOETS_PLAY "Play/Pause"
  #define LABEL_TOETS_MINUS "-"
  #define LABEL_TOETS_PLUS "+"
  #define LABEL_TOETS_EQ "EQ"
  #define LABEL_TOETS_100_PLUS "100+"
  #define LABEL_TOETS_200_PLUS "200+"

#endif
