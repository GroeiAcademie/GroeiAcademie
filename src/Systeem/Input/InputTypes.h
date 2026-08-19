#ifndef INPUT_TYPES_H
#define INPUT_TYPES_H

// INPUT TYPES
#define INPUT_TYPE_NONE       0x00 // geen input
#define INPUT_TYPE_DIGITAL    0x01 // rechtstreekse aansluiting op de digitale pinnen
#define INPUT_TYPE_PCF8574    0x02 // indirecte aansluiting via de PCF8574
#define INPUT_TYPE_HX1838     0x04 // afstanndbediening

// ============================================================================
// PCF8574 PINNEN: vaste hardware-identiteiten
// ============================================================================
#define PCF8574_PIN_P0 0
#define PCF8574_PIN_P1 1
#define PCF8574_PIN_P2 2
#define PCF8574_PIN_P3 3
#define PCF8574_PIN_P4 4
#define PCF8574_PIN_P5 5
#define PCF8574_PIN_P6 6
#define PCF8574_PIN_P7 7

// ============================================================================
// TTP229 OUTPUT LEVEL: vaste keuzewaarden
// ============================================================================
#define TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED_LOW   0
#define TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED_HIGH  1

// ============================================================================
// KEYPAD TYPES: één fysiek keypad per build
// ============================================================================
#define KEYPAD_TYPE_DRUKKNOP_DIRECT_1x4        1   // K1, K2, K3, K4, GND 
#define KEYPAD_TYPE_DRUKKNOP_DIRECT_2x4        2   // K1, K2, K3, K4, K5, K6, K7, K8, GND 

#define KEYPAD_TYPE_DRUKKNOP_MATRIX_2x2       10   // L1, L2, R1, R2 (OT3688)
// Betekenis van de pinnen (OT3688):
// * L1 (Line 1): Rij 1 (Bovenste twee knoppen)
// * L2 (Line 2): Rij 2 (Onderste twee knoppen)
// * R1 (Row/Rank 1 of Right 1): Kolom 1 (Linker twee knoppen)
// * R2 (Row/Rank 2 of Right 2): Kolom 2 (Rechter twee knoppen)

#define KEYPAD_TYPE_DRUKKNOP_MATRIX_4x4       11   // C4,C3,C2,C1,R1,R2,R3,R4 (OT3241)

#define KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4       20   // 1, 2, 3, 4, GND
#define KEYPAD_TYPE_MEMBRAAN_DIRECT_4x1       21   // 1, 2, 3, 4, GND

#define KEYPAD_TYPE_MEMBRAAN_MATRIX_1x4       30   // R1,C1,C2,C3,C4 
#define KEYPAD_TYPE_MEMBRAAN_MATRIX_2x4       31   // R1,R2,C1,C2,C3,C4 
#define KEYPAD_TYPE_MEMBRAAN_MATRIX_4x3       32   // R1,R2,R3,R4,C1,C2,C3
#define KEYPAD_TYPE_MEMBRAAN_MATRIX_4x4       33   // R1,R2,R3,R4,C1,C2,C3,C4
// #define KEYPAD_TYPE_MEMBRAAN_MATRIX_4x5       34   // R1,R2,R3,R4,C1,C2,C3,C4,C5      TODO: nog te implementeren

#define KEYPAD_TYPE_TOUCH_TTP224_DIRECT_1x4   40   // OUT1, OUT2, OUT3, OUT4, GND, VCC

#define KEYPAD_TYPE_TOUCH_TTP229_MATRIX_4x4   50   // VCC, GND, (SCL, SDO), OUT1, OUT2, OUT3, OUT4, OUT5, OUT6, OUT7, OUT8 (HW-136) - fysiek 4x4 touch keypad: via de 8 parallelle uitgangen op PCF8574 P0-P7 kunnen enkel de eerste 8 toetsen uitgelezen worden via OUTn

// #define KEYPAD_TYPE_TOUCH_TTP229_SERIAL_4x4   60   // via SCL/SDO; dit is geen I2C.   TODO: nog te implementeren

// Experimenteel: enkel bij INPUT_TYPE_PCF8574.
// Pinnen, KEYPAD_GENERIEK_OUTPUT_LEVEL_WHEN_KEY_PRESSED en de opschriftkoppeling worden volledig in UserConfig.h ingesteld, niet hier in de bibliotheek. 
// Zie docs/Systeem/INPUT.md.
#define KEYPAD_TYPE_USER_DEFINED_DIRECT       90   // gebruiker bepaalt zelf pinnen, output-niveau en opschrift via UserConfig.h
#define KEYPAD_TYPE_USER_DEFINED_MATRIX       91   // gebruiker bepaalt zelf rij-/kolompinnen en opschrift via UserConfig.h

#define KEYPAD_GENERIEK_OUTPUT_LEVEL_WHEN_KEY_PRESSED_LOW   0
#define KEYPAD_GENERIEK_OUTPUT_LEVEL_WHEN_KEY_PRESSED_HIGH  1

// ============================================================================
// HX1838 TOETSENINDELINGEN: enkel relevant bij INPUT_TYPE_HX1838
// ============================================================================
#define HX1838_TOETSENINDELING_REMOTE_OK_BOVENAAN_17_TOETSEN  1   // remote met 17 toetsen: (UP, DOWN, OK, LEFT, RIGHT, 1-9, *, 0, #) [UP, DOWN, OK, LEFT, RIGHT, 1, 2, 3, 4, 5, 6, 7, 8, 9, *, 0, #]
#define HX1838_TOETSENINDELING_REMOTE_OK_ONDERAAN_17_TOETSEN  2   // remote met 17 toetsen: (1-9, *, 0, #, UP, DOWN, OK, LEFT, RIGHT) [1, 2, 3, 4, 5, 6, 7, 8, 9, *, 0, #, UP, DOWN, OK, LEFT, RIGHT]
#define HX1838_TOETSENINDELING_REMOTE_21_TOETSEN_MP3          3   // remote met 21 toetsen: 0-9 + MP3-toetsen [CH-, CH, CH+, PREV, NEXT, PLAY, -, +, EQ, 0, 100+, 200+, 1, 2, 3, 4, 5, 6, 7, 8, 9]

// ============================================================================
// HX1838 BRONCODES: waar de afstandsbedieningscodes vandaan komen
// ============================================================================
#define HX1838_BRON_CODES_DEFINE                         0
#define HX1838_BRON_CODES_EEPROM_ALTIJD                  1
#define HX1838_BRON_CODES_EEPROM_WANNEER_GEEN_DEFINE     2

// ============================================================================
// TOETSLABELS: vaste, taalonafhankelijke technische sleutels 
// ============================================================================
// drukknoppen
#define _LABEL_OPSCHRIFT_S1         "S1"
#define _LABEL_OPSCHRIFT_S2         "S2"
#define _LABEL_OPSCHRIFT_S3         "S3"
#define _LABEL_OPSCHRIFT_S4         "S4"
#define _LABEL_OPSCHRIFT_S5         "S5"
#define _LABEL_OPSCHRIFT_S6         "S6"
#define _LABEL_OPSCHRIFT_S7         "S7"
#define _LABEL_OPSCHRIFT_S8         "S8"
#define _LABEL_OPSCHRIFT_S9         "S9"
#define _LABEL_OPSCHRIFT_S10        "S10"
#define _LABEL_OPSCHRIFT_S11        "S11"
#define _LABEL_OPSCHRIFT_S12        "S12"
#define _LABEL_OPSCHRIFT_S13        "S13"
#define _LABEL_OPSCHRIFT_S14        "S14"
#define _LABEL_OPSCHRIFT_S15        "S15"
#define _LABEL_OPSCHRIFT_S16        "S16"

// membranen & remote met 17 toetsen
#define _LABEL_OPSCHRIFT_0          "0"
#define _LABEL_OPSCHRIFT_1          "1"
#define _LABEL_OPSCHRIFT_2          "2"
#define _LABEL_OPSCHRIFT_3          "3"
#define _LABEL_OPSCHRIFT_4          "4"
#define _LABEL_OPSCHRIFT_5          "5"
#define _LABEL_OPSCHRIFT_6          "6"
#define _LABEL_OPSCHRIFT_7          "7"
#define _LABEL_OPSCHRIFT_8          "8"
#define _LABEL_OPSCHRIFT_9          "9"

// membranen 4x4
#define _LABEL_OPSCHRIFT_A          "A"
#define _LABEL_OPSCHRIFT_B          "B"
#define _LABEL_OPSCHRIFT_C          "C"
#define _LABEL_OPSCHRIFT_D          "D"

// remote met 17 toetsen
#define _LABEL_OPSCHRIFT_STER       "*"
#define _LABEL_OPSCHRIFT_HEKJE      "#"

// extended remote met 17 toetsen
#define _LABEL_OPSCHRIFT_UP         "UP"
#define _LABEL_OPSCHRIFT_DOWN       "DOWN"
#define _LABEL_OPSCHRIFT_OK         "OK"
#define _LABEL_OPSCHRIFT_LEFT       "LEFT"
#define _LABEL_OPSCHRIFT_RIGHT      "RIGHT"

// extra MP3 remote met 21 toetsen
#define _LABEL_OPSCHRIFT_CH_MINUS   "CH-"
#define _LABEL_OPSCHRIFT_CH         "CH"
#define _LABEL_OPSCHRIFT_CH_PLUS    "CH+"
#define _LABEL_OPSCHRIFT_PREV       "PREV"
#define _LABEL_OPSCHRIFT_NEXT       "NEXT"
#define _LABEL_OPSCHRIFT_PLAY       "PLAY"
#define _LABEL_OPSCHRIFT_MINUS      "-"
#define _LABEL_OPSCHRIFT_PLUS       "+"
#define _LABEL_OPSCHRIFT_EQ         "EQ"
#define _LABEL_OPSCHRIFT_100_PLUS   "100+"
#define _LABEL_OPSCHRIFT_200_PLUS   "200+"

#endif // INPUT_TYPES_H
