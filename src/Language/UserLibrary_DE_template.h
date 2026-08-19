#ifndef GROEIACADEMIE_USER_LIBRARY_DE_H
#define GROEIACADEMIE_USER_LIBRARY_DE_H

// Diese vorläufige Version wurde automatisch aus der niederländischen Version übersetzt.
// Bis zur Überprüfung durch eine Person, die diese Sprache beherrscht.

// Kopieer dit bestand naar UserLibrary_DE.h in dezelfde map.
// Alle aanpasbare teksten uit het overeenkomstige officiële taalbestand staan hieronder als voorbeeld.
// Vaste systeemfoutcodes (_FATAL_CSxxx, _FATAL_PSxxx en _CRITICAL_SSxxx) zijn bewust niet aanpasbaar.
//=========================================================
// FATAL
//=========================================================
  #define FATAL_ZOEK_OP "JETZT NACHSCHLAGEN"


// PixelScreen

// InputKanalen

//=========================================================
// PrintToScreen
//=========================================================
// ADS1115
  #define _LCD_ADS1115_FOUT                "ADS1115"
  #define _LCD_ADS1115_NIET_GEVONDEN       "NICHT GEFUNDEN"

// Input Type: HX1838
  #define _INPUT_HX1838_CONTROLE                                   "IR-KONTROLLE:"
  #define _INPUT_HX1838_DRUK_OP_ELKE_TOETS_VOOR_CONTROLE           "JEDE TASTE DRUECK"

  #define _INPUT_HX1838_CONTROLE_GESLAAGD                          "IR-KONTROLLE OK"

  #define _INPUT_HX1838_DRUK_NU_OP                                 "JETZT DRUECKEN:"
// REGEL 2 IS DYNAMISCH: WEERGAVETEKST VAN DE TOETS.

// REGEL 1 IS DYNAMISCH: WEERGAVETEKST VAN DE HERKENDE TOETS.
  #define _INPUT_HX1838_TOETS_HERKEND                              "ERKANNT"

  #define _INPUT_HX1838_KALIBRATIE_UITVOEREN                       "KALIBRIERUNG"
  #define _INPUT_HX1838_KALIBRATIE_OPGESLAGEN                      "GESPEICHERT"
  #define _INPUT_HX1838_KALIBRATIE_TIMEOUT                         "ZEITLIMIT"

  #define _INPUT_HX1838_MAPPING_EEPROM_GELADEN                     "MAPPING GELADEN"

  #define _INPUT_HX1838_MAPPING_GELADEN                            "MAPPING GELADEN"

  #define _INPUT_HX1838_INTERRUPT_KOPPELING_MISLUKT                "IR PIN FEHLER"

// Input Type: PCF8574
  #define _INPUT_PCF8574_FOUT                                      "PCF8574 FEHLER"
  #define _INPUT_PCF8574_CONTROLEER_I2C                            "I2C PRUEFEN"

// Stimulus
  #define _LCD_KRACHT_TE_HARD              "ZU HART"
  #define _LCD_KRACHT_TE_ZACHT             "ZU WEICH"

  #define _LCD_SCORE_TIKKRACHT             "KLOPFKRAFT "
  #define _LCD_SCORE_TIKTIJD               "KLOPFZEIT "

  #define _LCD_TIJD_METEN_STOPT            "WIR STOPPEN"
  #define _LCD_TIJD_TEVEEL_FOUT            "SCHLECHTER START"
  #define _LCD_TIJD_TE_KORT                "ZU KURZ"
  #define _LCD_TIJD_TE_LANG                "ZU LANG"

//=========================================================
// Serial
//=========================================================

// Input Type: HX1838
  #define _INPUT_HX1838_DRUK_NU_OP_SERIAL                          "Jetzt drücken: "
  #define _INPUT_HX1838_GEEN_GELDIGE_KALIBRATIE_SERIAL             "Keine gültige IR-Kalibrierung gefunden."
  #define _INPUT_HX1838_MAPPING_EEPROM_GELADEN_SERIAL              "IR-Mapping aus EEPROM geladen:"
  #define _INPUT_HX1838_KALIBRATIE_GESTART_SERIAL                  "=== IR-Kalibrierung gestartet ==="
  #define _INPUT_HX1838_KALIBRATIE_TIMEOUT_SERIAL                  "IR-Kalibrierung abgebrochen: Zeitlimit."

  #define _INPUT_HX1838_INTERRUPT_KOPPELING_MISLUKT_SERIAL         "IR-Empfänger: Interrupt konnte nicht an HX1838_ONTVANGER_PIN gekoppelt werden."
  #define _INPUT_HX1838_KALIBRATIE_KLAAR_CONTROLE_SERIAL           "Kalibrierung abgeschlossen. Alle Tasten zur Kontrolle erneut drücken."
  #define _INPUT_HX1838_KALIBRATIE_OPGESLAGEN_SERIAL               "Kalibrierung gespeichert."
  #define _INPUT_HX1838_MAPPING_CONFIG_GELADEN_SERIAL              "Standard-IR-Mapping aus der Konfiguration geladen:"

  #define _INPUT_HX1838_MAPPING_CONFIG_ONVOLLEDIG                  "MAPPING UNVOLLST."

  #define _INPUT_HX1838_MAPPING_CONFIG_ONVOLLEDIG_SERIAL           "Feste IR-Mapping in UserConfig.h ist unvollständig, nicht alle HX1838_CODE_x sind ausgefüllt."
  #define _INPUT_HX1838_TOETS_HERKEND_SERIAL                       "Erkannt: "

  #define _INPUT_HX1838_DEBUG_FAALDE                                      "fehlgeschlagen"
  #define _INPUT_HX1838_DEBUG_MAGIC_MISMATCH                              "Magic stimmt nicht überein"
  #define _INPUT_HX1838_DEBUG_VERSIE_MISMATCH                             "Version stimmt nicht überein"
  #define _INPUT_HX1838_DEBUG_TOETSENINDELING_MISMATCH                    "Tastenbelegung stimmt nicht überein"
  #define _INPUT_HX1838_DEBUG_ALLE_TOETSEN_GEKALIBREER_START_VERIFICATIE  "alle Tasten kalibriert, Verifikation startet"
  #define _INPUT_HX1838_DEBUG_VOOR_INIT                                   "vor Initialisierung"
  #define _INPUT_HX1838_DEBUG_NA_INIT                                     "nach Initialisierung"

// Input Type: PCF8574
  #define _INPUT_PCF8574_NIET_BEREIKBAAR_SERIAL                    "PCF8574 unter der eingestellten I2C-Adresse nicht erreichbar."

//=========================================================
// INPUT - TOETSLABELS
//=========================================================
  #define LABEL_TOETS_0 "Taste 0"
  #define LABEL_TOETS_1 "Taste 1"
  #define LABEL_TOETS_2 "Taste 2"
  #define LABEL_TOETS_3 "Taste 3"
  #define LABEL_TOETS_4 "Taste 4"
  #define LABEL_TOETS_5 "Taste 5"
  #define LABEL_TOETS_6 "Taste 6"
  #define LABEL_TOETS_7 "Taste 7"
  #define LABEL_TOETS_8 "Taste 8"
  #define LABEL_TOETS_9 "Taste 9"

  #define LABEL_TOETS_A "Taste A"
  #define LABEL_TOETS_B "Taste B"
  #define LABEL_TOETS_C "Taste C"
  #define LABEL_TOETS_D "Taste D"

  #define LABEL_TOETS_S1 "Taste S1"
  #define LABEL_TOETS_S2 "Taste S2"
  #define LABEL_TOETS_S3 "Taste S3"
  #define LABEL_TOETS_S4 "Taste S4"
  #define LABEL_TOETS_S5 "Taste S5"
  #define LABEL_TOETS_S6 "Taste S6"
  #define LABEL_TOETS_S7 "Taste S7"
  #define LABEL_TOETS_S8 "Taste S8"
  #define LABEL_TOETS_S9 "Taste S9"
  #define LABEL_TOETS_S10 "Taste S10"
  #define LABEL_TOETS_S11 "Taste S11"
  #define LABEL_TOETS_S12 "Taste S12"
  #define LABEL_TOETS_S13 "Taste S13"
  #define LABEL_TOETS_S14 "Taste S14"
  #define LABEL_TOETS_S15 "Taste S15"
  #define LABEL_TOETS_S16 "Taste S16"

  #define LABEL_TOETS_STER "Taste *"
  #define LABEL_TOETS_HEKJE "Taste #"

// extended 
  #define LABEL_TOETS_UP    "Navigation hoch"
  #define LABEL_TOETS_DOWN  "Navigation runter"
  #define LABEL_TOETS_OK    "Navigation OK"
  #define LABEL_TOETS_LEFT  "Navigation links"
  #define LABEL_TOETS_RIGHT "Navigation rechts"

// MP3-afstandsbediening
  #define LABEL_TOETS_CH_MINUS "CH-"
  #define LABEL_TOETS_CH "CH"
  #define LABEL_TOETS_CH_PLUS "CH+"
  #define LABEL_TOETS_PREV "Zurück"
  #define LABEL_TOETS_NEXT "Weiter"
  #define LABEL_TOETS_PLAY "Play/Pause"
  #define LABEL_TOETS_MINUS "-"
  #define LABEL_TOETS_PLUS "+"
  #define LABEL_TOETS_EQ "EQ"
  #define LABEL_TOETS_100_PLUS "100+"
  #define LABEL_TOETS_200_PLUS "200+"

#endif
