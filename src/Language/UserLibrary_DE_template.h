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
  #define _INPUT_HX1838_CONTROLE                                   "IR-KONTROLLE"
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

  #define _INPUT_MAPPINGCONTROLE_TITEL                             "Mapping-Kontrolle"
  #define _INPUT_MAPPINGCONTROLE_OPSCHRIFTEN_OK                    "BESCHRIFTUNG OK"

  #define _INPUT_MAPPINGCONTROLE_WAARSCHUWING                      "WARNUNG"
  #define _INPUT_MAPPINGCONTROLE_ONTBREEKT                         "Fehlt"

  #define _INPUT_HX1838_GENERIEK_CODES_GEKALIBREERD                "KALIBRIERT"
  #define _INPUT_HX1838_GENERIEK_CODES_ZIE_SERIEEL                 "SIEHE SERIELL"

// Input Type: PCF8574
  #define _INPUT_PCF8574_FOUT                                      "PCF8574 FEHLER"
  #define _INPUT_PCF8574_CONTROLEER_I2C                            "I2C PRUEFEN"
  #define _INPUT_PCF8574_ADRES_LABEL                                "I2C-Adresse"

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

  #define _INPUT_HX1838_DEBUG_GEEN_MATCH                                  "KEIN MATCH"
  #define _INPUT_HX1838_DEBUG_IR_ONTVANGEN                                "IR empfangen"
  #define _INPUT_HX1838_DEBUG_MATCH                                       "MATCH"
  #define _INPUT_HX1838_DEBUG_REGEL                                       "Zeile"
  #define _INPUT_HX1838_DEBUG_TOETS                                       "Taste"
  #define _INPUT_HX1838_DEBUG_VERGELEKEN_MET                              "verglichen mit"

  #define _INPUT_HX1838_GENERIEK_CODES_NIET_GEDEFINIEERD_KALIBRATIE_GESTART   "nicht definiert, Kalibrierung gestartet."
  #define _INPUT_HX1838_GENERIEK_CODES_GEKALIBREERD_KOPIEER_REGEL             "Codes kalibriert, kopieren Sie die obige Zeile in"

// Input Type: PCF8574
  #define _INPUT_PCF8574_NIET_BEREIKBAAR_SERIAL                    "PCF8574 unter der eingestellten I2C-Adresse nicht erreichbar."

//=========================================================
// Static Assert
//=========================================================
// Input Type: HX1838
  #define _INPUT_HX1838_STATIC_ASSERT_AANTAL_TOETSEN               "enthält nicht die erwartete Anzahl HX1838-Tasten."
  #define _INPUT_HX1838_STATIC_ASSERT_CODES_BEVATTEN               "Codes enthalten."
  #define _INPUT_HX1838_STATIC_ASSERT_GEEN_CODE_WAARDE_NUL         "keinen Code mit Wert 0 enthalten."
  #define _INPUT_HX1838_STATIC_ASSERT_MAG_BIJ                      "darf bei"
  #define _INPUT_HX1838_STATIC_ASSERT_MOET_EXACT                   "muss genau"

// Input Type: PCF8574
  #define _INPUT_PCF8574_STATIC_ASSERT_DIRECT_MAX_ACHT_PINNEN      "Das direkte generische Keypad verwendet mehr als 8 PCF8574-Pins."
  #define _INPUT_PCF8574_STATIC_ASSERT_DIRECT_MIN_EEN_PIN          "Das direkte generische Keypad muss mindestens einen PCF8574-Pin verwenden."
  #define _INPUT_PCF8574_STATIC_ASSERT_DIRECT_PINNEN_MAX_ACHT      "Das direkte Keypad verwendet mehr als 8 PCF8574-Pins."
  #define _INPUT_PCF8574_STATIC_ASSERT_KEY_LAYOUT_DIRECT_AANTAL    "muss genau einen Eintrag pro direktem Keypad-Pin enthalten."
  #define _INPUT_PCF8574_STATIC_ASSERT_KEY_LAYOUT_MATRIX_AANTAL    "muss genau einen Eintrag pro Matrixposition enthalten."
  #define _INPUT_PCF8574_STATIC_ASSERT_MATRIX_DUBBELE_PINNEN       "Die generische Matrix enthält doppelte Zeilen- oder Spaltenpins."
  #define _INPUT_PCF8574_STATIC_ASSERT_MATRIX_MAX_ACHT_PINNEN      "Das generische Matrix-Keypad verwendet mehr als 8 PCF8574-Pins."
  #define _INPUT_PCF8574_STATIC_ASSERT_MATRIX_MIN_EEN_KOLOM        "Das generische Matrix-Keypad muss mindestens eine Spalte haben."
  #define _INPUT_PCF8574_STATIC_ASSERT_MATRIX_MIN_EEN_RIJ          "Das generische Matrix-Keypad muss mindestens eine Zeile haben."
  #define _INPUT_PCF8574_STATIC_ASSERT_MATRIX_PINNEN_MAX_ACHT      "Die Matrix verwendet mehr als 8 PCF8574-Pins."
  #define _INPUT_PCF8574_STATIC_ASSERT_PINNEN_BEREIK               "darf nur PCF8574-Bitpositionen 0 bis 7 verwenden."
  #define _INPUT_PCF8574_STATIC_ASSERT_PINNEN_UNIEK                "enthält doppelte PCF8574-Pins."
  #define _INPUT_PCF8574_STATIC_ASSERT_RIJ_KOLOM_BEREIK            "dürfen nur PCF8574-Bitpositionen 0 bis 7 verwenden."
  #define _INPUT_PCF8574_STATIC_ASSERT_RIJ_KOLOM_EN                "und"
  #define _INPUT_PCF8574_STATIC_ASSERT_RIJ_KOLOM_OVERLAP           "Derselbe PCF8574-Pin darf nicht gleichzeitig Zeilen- und Spaltenpin sein."

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
