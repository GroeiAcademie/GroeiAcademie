#ifndef GROEIACADEMIE_LIBRARY_DE_H
#define GROEIACADEMIE_LIBRARY_DE_H

// Diese vorläufige Version wurde automatisch aus der niederländischen Version übersetzt.
// Bis zur Überprüfung durch eine Person, die diese Sprache beherrscht.

//=========================================================
// FATAL
//=========================================================
#ifndef FATAL_ZOEK_OP
  #define FATAL_ZOEK_OP "JETZT NACHSEHEN"
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
  #define _LCD_ADS1115_NIET_GEVONDEN       "NICHT GEFUNDEN"
#endif

// Input Type: HX1838
#ifndef _INPUT_HX1838_CONTROLE
  #define _INPUT_HX1838_CONTROLE                                   "IR-KONTROLLE"
#endif
#ifndef _INPUT_HX1838_DRUK_OP_ELKE_TOETS_VOOR_CONTROLE
  #define _INPUT_HX1838_DRUK_OP_ELKE_TOETS_VOOR_CONTROLE           "JEDE TASTE DRÜCK"
#endif

#ifndef _INPUT_HX1838_CONTROLE_GESLAAGD
  #define _INPUT_HX1838_CONTROLE_GESLAAGD                          "IR-KONTROLLE OK"
#endif

#ifndef _INPUT_HX1838_DRUK_NU_OP
  #define _INPUT_HX1838_DRUK_NU_OP                                 "JETZT DRUECKEN:"
#endif
// REGEL 2 IS DYNAMISCH: WEERGAVETEKST VAN DE TOETS.

// REGEL 1 IS DYNAMISCH: WEERGAVETEKST VAN DE HERKENDE TOETS.
#ifndef _INPUT_HX1838_TOETS_HERKEND
  #define _INPUT_HX1838_TOETS_HERKEND                              "ERKANNT"
#endif

#ifndef _INPUT_HX1838_KALIBRATIE_UITVOEREN
  #define _INPUT_HX1838_KALIBRATIE_UITVOEREN                       "KALIBRIERUNG"
#endif
#ifndef _INPUT_HX1838_KALIBRATIE_TIMEOUT
  #define _INPUT_HX1838_KALIBRATIE_TIMEOUT                          "ZEITLIMIT"
#endif

#ifndef _INPUT_HX1838_KALIBRATIE_OPGESLAGEN
  #define _INPUT_HX1838_KALIBRATIE_OPGESLAGEN                      "GESPEICHERT"
#endif

#ifndef _INPUT_HX1838_MAPPING_EEPROM_GELADEN
  #define _INPUT_HX1838_MAPPING_EEPROM_GELADEN                     "MAPPING GELADEN"
#endif

#ifndef _INPUT_HX1838_MAPPING_GELADEN
  #define _INPUT_HX1838_MAPPING_GELADEN                            "MAPPING GELADEN"
#endif

#ifndef _INPUT_HX1838_INTERRUPT_KOPPELING_MISLUKT
  #define _INPUT_HX1838_INTERRUPT_KOPPELING_MISLUKT                "IR PIN FEHLER"
#endif

#ifndef _INPUT_MAPPINGCONTROLE_TITEL
  #define _INPUT_MAPPINGCONTROLE_TITEL                             "Mapping-Kontrolle"
#endif
#ifndef _INPUT_MAPPINGCONTROLE_OPSCHRIFTEN_OK
  #define _INPUT_MAPPINGCONTROLE_OPSCHRIFTEN_OK                    "BESCHRIFTUNG OK"
#endif

#ifndef _INPUT_MAPPINGCONTROLE_WAARSCHUWING
  #define _INPUT_MAPPINGCONTROLE_WAARSCHUWING                      "WARNUNG"
#endif
#ifndef _INPUT_MAPPINGCONTROLE_ONTBREEKT
  #define _INPUT_MAPPINGCONTROLE_ONTBREEKT                         "Fehlt"
#endif

#ifndef _INPUT_HX1838_GENERIEK_CODES_GEKALIBREERD
  #define _INPUT_HX1838_GENERIEK_CODES_GEKALIBREERD                "KALIBRIERT"
#endif
#ifndef _INPUT_HX1838_GENERIEK_CODES_ZIE_SERIEEL
  #define _INPUT_HX1838_GENERIEK_CODES_ZIE_SERIEEL                 "SIEHE SERIELL"
#endif

// Input Type: PCF8574
#ifndef _INPUT_PCF8574_FOUT
  #define _INPUT_PCF8574_FOUT                                      "PCF8574 FEHLER"
#endif
#ifndef _INPUT_PCF8574_CONTROLEER_I2C
  #define _INPUT_PCF8574_CONTROLEER_I2C                            "I2C PRUEFEN"
#endif
#ifndef _INPUT_PCF8574_ADRES_LABEL
  #define _INPUT_PCF8574_ADRES_LABEL                                "I2C-Adresse"
#endif

// Stimulus
#ifndef _LCD_KRACHT_TE_HARD
  #define _LCD_KRACHT_TE_HARD              "ZU HART"
#endif
#ifndef _LCD_KRACHT_TE_ZACHT
  #define _LCD_KRACHT_TE_ZACHT             "ZU WEICH"
#endif

#ifndef _LCD_SCORE_TIKKRACHT
  #define _LCD_SCORE_TIKKRACHT             "KLOPFKRAFT "
#endif
#ifndef _LCD_SCORE_TIKTIJD
  #define _LCD_SCORE_TIKTIJD               "KLOPFZEIT "
#endif

#ifndef _LCD_TIJD_METEN_STOPT
  #define _LCD_TIJD_METEN_STOPT            "WIR STOPPEN"
#endif
#ifndef _LCD_TIJD_TEVEEL_FOUT
  #define _LCD_TIJD_TEVEEL_FOUT            "SCHLECHTER START"
#endif
#ifndef _LCD_TIJD_TE_KORT
  #define _LCD_TIJD_TE_KORT                "ZU KURZ"
#endif
#ifndef _LCD_TIJD_TE_LANG
  #define _LCD_TIJD_TE_LANG                "ZU LANG"
#endif  

//=========================================================
// Serial
//=========================================================

// Input Type: HX1838
#ifndef _INPUT_HX1838_DRUK_NU_OP_SERIAL
  #define _INPUT_HX1838_DRUK_NU_OP_SERIAL                          "Jetzt drücken: "
#endif

#ifndef _INPUT_HX1838_GEEN_GELDIGE_KALIBRATIE_SERIAL
  #define _INPUT_HX1838_GEEN_GELDIGE_KALIBRATIE_SERIAL             "Keine gültige IR-Kalibrierung gefunden."
#endif

#ifndef _INPUT_HX1838_TOETS_HERKEND_SERIAL
  #define _INPUT_HX1838_TOETS_HERKEND_SERIAL                       "Erkannt: "
#endif

#ifndef _INPUT_HX1838_MAPPING_EEPROM_GELADEN_SERIAL
  #define _INPUT_HX1838_MAPPING_EEPROM_GELADEN_SERIAL              "IR-Mapping aus EEPROM geladen:"
#endif

#ifndef _INPUT_HX1838_KALIBRATIE_GESTART_SERIAL
  #define _INPUT_HX1838_KALIBRATIE_GESTART_SERIAL                  "=== IR-Kalibrierung gestartet ==="
#endif

#ifndef _INPUT_HX1838_INTERRUPT_KOPPELING_MISLUKT_SERIAL
  #define _INPUT_HX1838_INTERRUPT_KOPPELING_MISLUKT_SERIAL         "IR-Empfänger: Interrupt konnte nicht an HX1838_ONTVANGER_PIN gekoppelt werden."
#endif

#ifndef _INPUT_HX1838_KALIBRATIE_KLAAR_CONTROLE_SERIAL
  #define _INPUT_HX1838_KALIBRATIE_KLAAR_CONTROLE_SERIAL           "Kalibrierung abgeschlossen. Alle Tasten zur Kontrolle erneut drücken."
#endif

#ifndef _INPUT_HX1838_KALIBRATIE_TIMEOUT_SERIAL
  #define _INPUT_HX1838_KALIBRATIE_TIMEOUT_SERIAL                   "IR-Kalibrierung abgebrochen: Zeitlimit."
#endif

#ifndef _INPUT_HX1838_KALIBRATIE_OPGESLAGEN_SERIAL
  #define _INPUT_HX1838_KALIBRATIE_OPGESLAGEN_SERIAL               "Kalibrierung gespeichert."
#endif

#ifndef _INPUT_HX1838_MAPPING_CONFIG_GELADEN_SERIAL
  #define _INPUT_HX1838_MAPPING_CONFIG_GELADEN_SERIAL              "Standard-IR-Mapping aus der Konfiguration geladen:"
#endif

#ifndef _INPUT_HX1838_MAPPING_CONFIG_ONVOLLEDIG
  #define _INPUT_HX1838_MAPPING_CONFIG_ONVOLLEDIG                  "MAPPING UNVOLLST."
#endif

#ifndef _INPUT_HX1838_MAPPING_CONFIG_ONVOLLEDIG_SERIAL
  #define _INPUT_HX1838_MAPPING_CONFIG_ONVOLLEDIG_SERIAL           "Feste IR-Mapping in UserConfig.h ist unvollständig, nicht alle HX1838_CODE_x sind ausgefüllt."
#endif

#ifndef _INPUT_HX1838_DEBUG_FAALDE
  #define _INPUT_HX1838_DEBUG_FAALDE                                      "fehlgeschlagen"
#endif
#ifndef _INPUT_HX1838_DEBUG_MAGIC_MISMATCH
  #define _INPUT_HX1838_DEBUG_MAGIC_MISMATCH                              "Magic stimmt nicht überein"
#endif
#ifndef _INPUT_HX1838_DEBUG_VERSIE_MISMATCH
  #define _INPUT_HX1838_DEBUG_VERSIE_MISMATCH                             "Version stimmt nicht überein"
#endif
#ifndef _INPUT_HX1838_DEBUG_TOETSENINDELING_MISMATCH
  #define _INPUT_HX1838_DEBUG_TOETSENINDELING_MISMATCH                    "Tastenbelegung stimmt nicht überein"
#endif
#ifndef _INPUT_HX1838_DEBUG_ALLE_TOETSEN_GEKALIBREER_START_VERIFICATIE
  #define _INPUT_HX1838_DEBUG_ALLE_TOETSEN_GEKALIBREER_START_VERIFICATIE  "alle Tasten kalibriert, Verifikation startet"
#endif
#ifndef _INPUT_HX1838_DEBUG_VOOR_INIT
  #define _INPUT_HX1838_DEBUG_VOOR_INIT                                   "vor Initialisierung"
#endif
#ifndef _INPUT_HX1838_DEBUG_NA_INIT
  #define _INPUT_HX1838_DEBUG_NA_INIT                                     "nach Initialisierung"
#endif

#ifndef _INPUT_HX1838_DEBUG_GEEN_MATCH
  #define _INPUT_HX1838_DEBUG_GEEN_MATCH                                  "KEIN MATCH"
#endif
#ifndef _INPUT_HX1838_DEBUG_IR_ONTVANGEN
  #define _INPUT_HX1838_DEBUG_IR_ONTVANGEN                                "IR empfangen"
#endif
#ifndef _INPUT_HX1838_DEBUG_MATCH
  #define _INPUT_HX1838_DEBUG_MATCH                                       "MATCH"
#endif
#ifndef _INPUT_HX1838_DEBUG_REGEL
  #define _INPUT_HX1838_DEBUG_REGEL                                       "Zeile"
#endif
#ifndef _INPUT_HX1838_DEBUG_TOETS
  #define _INPUT_HX1838_DEBUG_TOETS                                       "Taste"
#endif
#ifndef _INPUT_HX1838_DEBUG_VERGELEKEN_MET
  #define _INPUT_HX1838_DEBUG_VERGELEKEN_MET                              "verglichen mit"
#endif

#ifndef _INPUT_HX1838_GENERIEK_CODES_NIET_GEDEFINIEERD_KALIBRATIE_GESTART
  #define _INPUT_HX1838_GENERIEK_CODES_NIET_GEDEFINIEERD_KALIBRATIE_GESTART   "nicht definiert, Kalibrierung gestartet."
#endif
#ifndef _INPUT_HX1838_GENERIEK_CODES_GEKALIBREERD_KOPIEER_REGEL
  #define _INPUT_HX1838_GENERIEK_CODES_GEKALIBREERD_KOPIEER_REGEL             "Codes kalibriert, kopieren Sie die obige Zeile in"
#endif

// Input Type: PCF8574
#ifndef _INPUT_PCF8574_NIET_BEREIKBAAR_SERIAL
  #define _INPUT_PCF8574_NIET_BEREIKBAAR_SERIAL                    "PCF8574 unter der eingestellten I2C-Adresse nicht erreichbar."
#endif

//=========================================================
// Static Assert
//=========================================================
// Input Type: HX1838
#ifndef _INPUT_HX1838_STATIC_ASSERT_AANTAL_TOETSEN
  #define _INPUT_HX1838_STATIC_ASSERT_AANTAL_TOETSEN               "enthält nicht die erwartete Anzahl HX1838-Tasten."
#endif
#ifndef _INPUT_HX1838_STATIC_ASSERT_CODES_BEVATTEN
  #define _INPUT_HX1838_STATIC_ASSERT_CODES_BEVATTEN               "Codes enthalten."
#endif
#ifndef _INPUT_HX1838_STATIC_ASSERT_GEEN_CODE_WAARDE_NUL
  #define _INPUT_HX1838_STATIC_ASSERT_GEEN_CODE_WAARDE_NUL         "keinen Code mit Wert 0 enthalten."
#endif
#ifndef _INPUT_HX1838_STATIC_ASSERT_MAG_BIJ
  #define _INPUT_HX1838_STATIC_ASSERT_MAG_BIJ                      "darf bei"
#endif
#ifndef _INPUT_HX1838_STATIC_ASSERT_MOET_EXACT
  #define _INPUT_HX1838_STATIC_ASSERT_MOET_EXACT                   "muss genau"
#endif

// Input Type: PCF8574
#ifndef _INPUT_PCF8574_STATIC_ASSERT_DIRECT_MAX_ACHT_PINNEN
  #define _INPUT_PCF8574_STATIC_ASSERT_DIRECT_MAX_ACHT_PINNEN      "Das direkte generische Keypad verwendet mehr als 8 PCF8574-Pins."
#endif
#ifndef _INPUT_PCF8574_STATIC_ASSERT_DIRECT_MIN_EEN_PIN
  #define _INPUT_PCF8574_STATIC_ASSERT_DIRECT_MIN_EEN_PIN          "Das direkte generische Keypad muss mindestens einen PCF8574-Pin verwenden."
#endif
#ifndef _INPUT_PCF8574_STATIC_ASSERT_DIRECT_PINNEN_MAX_ACHT
  #define _INPUT_PCF8574_STATIC_ASSERT_DIRECT_PINNEN_MAX_ACHT      "Das direkte Keypad verwendet mehr als 8 PCF8574-Pins."
#endif
#ifndef _INPUT_PCF8574_STATIC_ASSERT_KEY_LAYOUT_DIRECT_AANTAL
  #define _INPUT_PCF8574_STATIC_ASSERT_KEY_LAYOUT_DIRECT_AANTAL    "muss genau einen Eintrag pro direktem Keypad-Pin enthalten."
#endif
#ifndef _INPUT_PCF8574_STATIC_ASSERT_KEY_LAYOUT_MATRIX_AANTAL
  #define _INPUT_PCF8574_STATIC_ASSERT_KEY_LAYOUT_MATRIX_AANTAL    "muss genau einen Eintrag pro Matrixposition enthalten."
#endif
#ifndef _INPUT_PCF8574_STATIC_ASSERT_MATRIX_DUBBELE_PINNEN
  #define _INPUT_PCF8574_STATIC_ASSERT_MATRIX_DUBBELE_PINNEN       "Die generische Matrix enthält doppelte Zeilen- oder Spaltenpins."
#endif
#ifndef _INPUT_PCF8574_STATIC_ASSERT_MATRIX_MAX_ACHT_PINNEN
  #define _INPUT_PCF8574_STATIC_ASSERT_MATRIX_MAX_ACHT_PINNEN      "Das generische Matrix-Keypad verwendet mehr als 8 PCF8574-Pins."
#endif
#ifndef _INPUT_PCF8574_STATIC_ASSERT_MATRIX_MIN_EEN_KOLOM
  #define _INPUT_PCF8574_STATIC_ASSERT_MATRIX_MIN_EEN_KOLOM        "Das generische Matrix-Keypad muss mindestens eine Spalte haben."
#endif
#ifndef _INPUT_PCF8574_STATIC_ASSERT_MATRIX_MIN_EEN_RIJ
  #define _INPUT_PCF8574_STATIC_ASSERT_MATRIX_MIN_EEN_RIJ          "Das generische Matrix-Keypad muss mindestens eine Zeile haben."
#endif
#ifndef _INPUT_PCF8574_STATIC_ASSERT_MATRIX_PINNEN_MAX_ACHT
  #define _INPUT_PCF8574_STATIC_ASSERT_MATRIX_PINNEN_MAX_ACHT      "Die Matrix verwendet mehr als 8 PCF8574-Pins."
#endif
#ifndef _INPUT_PCF8574_STATIC_ASSERT_PINNEN_BEREIK
  #define _INPUT_PCF8574_STATIC_ASSERT_PINNEN_BEREIK               "darf nur PCF8574-Bitpositionen 0 bis 7 verwenden."
#endif
#ifndef _INPUT_PCF8574_STATIC_ASSERT_PINNEN_UNIEK
  #define _INPUT_PCF8574_STATIC_ASSERT_PINNEN_UNIEK                "enthält doppelte PCF8574-Pins."
#endif
#ifndef _INPUT_PCF8574_STATIC_ASSERT_RIJ_KOLOM_BEREIK
  #define _INPUT_PCF8574_STATIC_ASSERT_RIJ_KOLOM_BEREIK            "dürfen nur PCF8574-Bitpositionen 0 bis 7 verwenden."
#endif
#ifndef _INPUT_PCF8574_STATIC_ASSERT_RIJ_KOLOM_EN
  #define _INPUT_PCF8574_STATIC_ASSERT_RIJ_KOLOM_EN                "und"
#endif
#ifndef _INPUT_PCF8574_STATIC_ASSERT_RIJ_KOLOM_OVERLAP
  #define _INPUT_PCF8574_STATIC_ASSERT_RIJ_KOLOM_OVERLAP           "Derselbe PCF8574-Pin darf nicht gleichzeitig Zeilen- und Spaltenpin sein."
#endif

//=========================================================
// INPUT - TOETSLABELS
//=========================================================
// drukknoppen
#ifndef LABEL_TOETS_S1
  #define LABEL_TOETS_S1 "Taste S1"
#endif
#ifndef LABEL_TOETS_S2
  #define LABEL_TOETS_S2 "Taste S2"
#endif
#ifndef LABEL_TOETS_S3
  #define LABEL_TOETS_S3 "Taste S3"
#endif
#ifndef LABEL_TOETS_S4
  #define LABEL_TOETS_S4 "Taste S4"
#endif
#ifndef LABEL_TOETS_S5
  #define LABEL_TOETS_S5 "Taste S5"
#endif
#ifndef LABEL_TOETS_S6
  #define LABEL_TOETS_S6 "Taste S6"
#endif
#ifndef LABEL_TOETS_S7
  #define LABEL_TOETS_S7 "Taste S7"
#endif
#ifndef LABEL_TOETS_S8
  #define LABEL_TOETS_S8 "Taste S8"
#endif
#ifndef LABEL_TOETS_S9
  #define LABEL_TOETS_S9 "Taste S9"
#endif
#ifndef LABEL_TOETS_S10
  #define LABEL_TOETS_S10 "Taste S10"
#endif
#ifndef LABEL_TOETS_S11
  #define LABEL_TOETS_S11 "Taste S11"
#endif
#ifndef LABEL_TOETS_S12
  #define LABEL_TOETS_S12 "Taste S12"
#endif
#ifndef LABEL_TOETS_S13
  #define LABEL_TOETS_S13 "Taste S13"
#endif
#ifndef LABEL_TOETS_S14
  #define LABEL_TOETS_S14 "Taste S14"
#endif
#ifndef LABEL_TOETS_S15
  #define LABEL_TOETS_S15 "Taste S15"
#endif
#ifndef LABEL_TOETS_S16
  #define LABEL_TOETS_S16 "Taste S16"
#endif

// membranen & remote met 17 toetsen
#ifndef LABEL_TOETS_0
  #define LABEL_TOETS_0 "Taste 0"
#endif
#ifndef LABEL_TOETS_1
  #define LABEL_TOETS_1 "Taste 1"
#endif
#ifndef LABEL_TOETS_2
  #define LABEL_TOETS_2 "Taste 2"
#endif
#ifndef LABEL_TOETS_3
  #define LABEL_TOETS_3 "Taste 3"
#endif
#ifndef LABEL_TOETS_4
  #define LABEL_TOETS_4 "Taste 4"
#endif
#ifndef LABEL_TOETS_5
  #define LABEL_TOETS_5 "Taste 5"
#endif
#ifndef LABEL_TOETS_6
  #define LABEL_TOETS_6 "Taste 6"
#endif
#ifndef LABEL_TOETS_7
  #define LABEL_TOETS_7 "Taste 7"
#endif
#ifndef LABEL_TOETS_8
  #define LABEL_TOETS_8 "Taste 8"
#endif
#ifndef LABEL_TOETS_9
  #define LABEL_TOETS_9 "Taste 9"
#endif

// membranen 4x4
#ifndef LABEL_TOETS_A
  #define LABEL_TOETS_A "Taste A"
#endif
#ifndef LABEL_TOETS_B
  #define LABEL_TOETS_B "Taste B"
#endif
#ifndef LABEL_TOETS_C
  #define LABEL_TOETS_C "Taste C"
#endif
#ifndef LABEL_TOETS_D
  #define LABEL_TOETS_D "Taste D"
#endif

// remote met 17 toetsen
#ifndef LABEL_TOETS_STER
  #define LABEL_TOETS_STER "Taste *"
#endif
#ifndef LABEL_TOETS_HEKJE
  #define LABEL_TOETS_HEKJE "Taste #"
#endif

// extended remote met 17 toetsen
#ifndef LABEL_TOETS_UP
  #define LABEL_TOETS_UP "Navigation hoch"
#endif
#ifndef LABEL_TOETS_DOWN
  #define LABEL_TOETS_DOWN "Navi runter"
#endif
#ifndef LABEL_TOETS_OK
  #define LABEL_TOETS_OK "Navigation OK"
#endif
#ifndef LABEL_TOETS_LEFT
  #define LABEL_TOETS_LEFT "Navigation links"
#endif
#ifndef LABEL_TOETS_RIGHT
  #define LABEL_TOETS_RIGHT "Navi rechts"
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
  #define LABEL_TOETS_PREV "Zurück"
#endif
#ifndef LABEL_TOETS_NEXT
  #define LABEL_TOETS_NEXT "Weiter"
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
