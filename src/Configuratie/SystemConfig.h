#ifndef SYSTEM_CONFIG_H
#define SYSTEM_CONFIG_H

#include "../Systeem/Screen/ScreenTypes.h"
#include "../Systeem/Input/InputTypes.h"

// Vaste keuzewaarden die vóór UserConfig.h beschikbaar moeten zijn.
#ifndef ADC_BACKEND_NATIVE
  #define ADC_BACKEND_NATIVE      0                   // ingebouwde ADC van de Arduino
#endif
#ifndef ADC_BACKEND_ADS1115
  #define ADC_BACKEND_ADS1115     1                   // externe 16-bit ADC via I2C
#endif

#define BOARD_UNO_R3                     0
#define BOARD_UNO_R4_MINIMA              1
#define BOARD_UNO_R4_WIFI                2
#define BOARD_ARDI32                     3
#define BOARD_CYTRON_MAKER_UNO_RP2040    4
#define BOARD_ESP32_UNO                  5
#define BOARD_NUCLEO_F401RE              6

#ifndef GROEIACADEMIE_IGNORE_USER_CONFIG
  #if defined(__has_include)
    #if __has_include("UserConfig.h")
      #include "UserConfig.h"
      #define GROEIACADEMIE_USER_CONFIG_GELADEN
    #endif
  #endif
#endif

// ============================================================================
// BOARD_VERSION EN ARDUINO UNO-SHIELDPINNEN
// ============================================================================
#ifndef BOARD_VERSION
  // NIET BEVESTIGD (zie docs/HARDWARE_SUPPORT.md): de macronamen hieronder voor ARDI32, Cytron Maker Uno RP2040 en Nucleo F401RE zijn niet rechtstreeks in een boards.txt/pins_arduino.h geverifieerd. 
  // Bij twijfel: BOARD_VERSION expliciet instellen in UserConfig.h
  #if defined(ARDUINO_AVR_UNO)
    #define BOARD_VERSION BOARD_UNO_R3
  #elif defined(ARDUINO_UNOR4_MINIMA)
    #define BOARD_VERSION BOARD_UNO_R4_MINIMA
  #elif defined(ARDUINO_UNOR4_WIFI)
    #define BOARD_VERSION BOARD_UNO_R4_WIFI
  #elif defined(ARDUINO_ARDI32)
    #define BOARD_VERSION BOARD_ARDI32
  #elif defined(ARDUINO_D1_UNO32)
    #define BOARD_VERSION BOARD_ESP32_UNO
  #elif defined(ARDUINO_CYTRON_MAKER_UNO_RP2040)
    #define BOARD_VERSION BOARD_CYTRON_MAKER_UNO_RP2040
  #elif defined(ARDUINO_NUCLEO_F401RE)
    #define BOARD_VERSION BOARD_NUCLEO_F401RE
  #else
    #error Dit board wordt momenteel niet ondersteund.
  #endif
#endif
// UserConfig.h heeft voorrang op automatische boarddetectie. 
// Wanneer de gebruiker in UserConfig.h zelf BOARD_VERSION definieert (bijvoorbeeld #define BOARD_VERSION BOARD_ARDI32), 
// gebruikt de library die keuze en wordt bovenstaande automatische detectie niet uitgevoerd (de #ifndef hierboven slaat dan over). 
// Enkel wanneer BOARD_VERSION nergens vooraf gedefinieerd is, bepaalt dit bestand het board automatisch op basis van de compiler-/boardmacro's van de Arduino IDE.

#if BOARD_VERSION != BOARD_UNO_R3 && BOARD_VERSION != BOARD_UNO_R4_MINIMA && BOARD_VERSION != BOARD_UNO_R4_WIFI && BOARD_VERSION != BOARD_ARDI32 && BOARD_VERSION != BOARD_ESP32_UNO && BOARD_VERSION != BOARD_CYTRON_MAKER_UNO_RP2040 && BOARD_VERSION != BOARD_NUCLEO_F401RE
  #error Selecteer een geldige BOARD_VERSION.
#endif

#if BOARD_VERSION == BOARD_ARDI32
  #ifndef ARDUINO_UNO_SHIELD_PIN_D0
    //-- #define ARDUINO_UNO_SHIELD_PIN_D0 44  // TODO: Ardi-32 mapping rechtstreeks verifiëren aan officiële SB Components-hardwarebron.
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D1
    //-- #define ARDUINO_UNO_SHIELD_PIN_D1 43  // TODO: Ardi-32 mapping rechtstreeks verifiëren aan officiële SB Components-hardwarebron.
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D2
    //-- #define ARDUINO_UNO_SHIELD_PIN_D2 5  // TODO: Ardi-32 mapping rechtstreeks verifiëren aan officiële SB Components-hardwarebron.
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D3
    //-- #define ARDUINO_UNO_SHIELD_PIN_D3 6  // TODO: Ardi-32 mapping rechtstreeks verifiëren aan officiële SB Components-hardwarebron.
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D4
    //-- #define ARDUINO_UNO_SHIELD_PIN_D4 7  // TODO: Ardi-32 mapping rechtstreeks verifiëren aan officiële SB Components-hardwarebron.
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D5
    //-- #define ARDUINO_UNO_SHIELD_PIN_D5 8  // TODO: Ardi-32 mapping rechtstreeks verifiëren aan officiële SB Components-hardwarebron.
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D6
    //-- #define ARDUINO_UNO_SHIELD_PIN_D6 9  // TODO: Ardi-32 mapping rechtstreeks verifiëren aan officiële SB Components-hardwarebron.
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D7
    //-- #define ARDUINO_UNO_SHIELD_PIN_D7 10  // TODO: Ardi-32 mapping rechtstreeks verifiëren aan officiële SB Components-hardwarebron.
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D8
    //-- #define ARDUINO_UNO_SHIELD_PIN_D8 11  // TODO: Ardi-32 mapping rechtstreeks verifiëren aan officiële SB Components-hardwarebron.
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D9
    //-- #define ARDUINO_UNO_SHIELD_PIN_D9 12  // TODO: Ardi-32 mapping rechtstreeks verifiëren aan officiële SB Components-hardwarebron.
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D10
    //-- #define ARDUINO_UNO_SHIELD_PIN_D10 13  // TODO: Ardi-32 mapping rechtstreeks verifiëren aan officiële SB Components-hardwarebron.
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D11
    //-- #define ARDUINO_UNO_SHIELD_PIN_D11 14  // TODO: Ardi-32 mapping rechtstreeks verifiëren aan officiële SB Components-hardwarebron.
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D12
    //-- #define ARDUINO_UNO_SHIELD_PIN_D12 21  // TODO: Ardi-32 mapping rechtstreeks verifiëren aan officiële SB Components-hardwarebron.
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D13
    //-- #define ARDUINO_UNO_SHIELD_PIN_D13 47  // TODO: Ardi-32 mapping rechtstreeks verifiëren aan officiële SB Components-hardwarebron.
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_A0
    //-- #define ARDUINO_UNO_SHIELD_PIN_A0 1  // TODO: Ardi-32 mapping rechtstreeks verifiëren aan officiële SB Components-hardwarebron.
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_A1
    //-- #define ARDUINO_UNO_SHIELD_PIN_A1 2  // TODO: Ardi-32 mapping rechtstreeks verifiëren aan officiële SB Components-hardwarebron.
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_A2
    //-- #define ARDUINO_UNO_SHIELD_PIN_A2 3  // TODO: Ardi-32 mapping rechtstreeks verifiëren aan officiële SB Components-hardwarebron.
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_A3
    //-- #define ARDUINO_UNO_SHIELD_PIN_A3 4  // TODO: Ardi-32 mapping rechtstreeks verifiëren aan officiële SB Components-hardwarebron.
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_A4
    //-- #define ARDUINO_UNO_SHIELD_PIN_A4 17  // TODO: Ardi-32 mapping rechtstreeks verifiëren aan officiële SB Components-hardwarebron.
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_A5
    //-- #define ARDUINO_UNO_SHIELD_PIN_A5 18  // TODO: Ardi-32 mapping rechtstreeks verifiëren aan officiële SB Components-hardwarebron.
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_SDA
    //-- #define ARDUINO_UNO_SHIELD_PIN_SDA 17  // TODO: Ardi-32 mapping rechtstreeks verifiëren aan officiële SB Components-hardwarebron.
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_SCL
    //-- #define ARDUINO_UNO_SHIELD_PIN_SCL 18  // TODO: Ardi-32 mapping rechtstreeks verifiëren aan officiële SB Components-hardwarebron.
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_SS
    //-- #define ARDUINO_UNO_SHIELD_PIN_SS 13  // TODO: Ardi-32 mapping rechtstreeks verifiëren aan officiële SB Components-hardwarebron.
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_MOSI
    //-- #define ARDUINO_UNO_SHIELD_PIN_MOSI 14  // TODO: Ardi-32 mapping rechtstreeks verifiëren aan officiële SB Components-hardwarebron.
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_MISO
    //-- #define ARDUINO_UNO_SHIELD_PIN_MISO 21  // TODO: Ardi-32 mapping rechtstreeks verifiëren aan officiële SB Components-hardwarebron.
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_SCK
    //-- #define ARDUINO_UNO_SHIELD_PIN_SCK 47  // TODO: Ardi-32 mapping rechtstreeks verifiëren aan officiële SB Components-hardwarebron.
  #endif
#elif BOARD_VERSION == BOARD_ESP32_UNO
  // WEMOS D1 R32 / TTGO D1 R32: pinmapping bevestigd tegen effectieve pins_arduino.h.
  #ifndef ARDUINO_UNO_SHIELD_PIN_D0
    #define ARDUINO_UNO_SHIELD_PIN_D0 D0
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D1
    #define ARDUINO_UNO_SHIELD_PIN_D1 D1
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D2
    #define ARDUINO_UNO_SHIELD_PIN_D2 D2
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D3
    #define ARDUINO_UNO_SHIELD_PIN_D3 D3
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D4
    #define ARDUINO_UNO_SHIELD_PIN_D4 D4
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D5
    #define ARDUINO_UNO_SHIELD_PIN_D5 D5
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D6
    #define ARDUINO_UNO_SHIELD_PIN_D6 D6
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D7
    #define ARDUINO_UNO_SHIELD_PIN_D7 D7
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D8
    #define ARDUINO_UNO_SHIELD_PIN_D8 D8
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D9
    #define ARDUINO_UNO_SHIELD_PIN_D9 D9
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D10
    #define ARDUINO_UNO_SHIELD_PIN_D10 D10
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D11
    #define ARDUINO_UNO_SHIELD_PIN_D11 D11
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D12
    #define ARDUINO_UNO_SHIELD_PIN_D12 D12
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D13
    #define ARDUINO_UNO_SHIELD_PIN_D13 D13
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_A0
    #define ARDUINO_UNO_SHIELD_PIN_A0 A0
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_A1
    #define ARDUINO_UNO_SHIELD_PIN_A1 A1
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_A2
    #define ARDUINO_UNO_SHIELD_PIN_A2 A2
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_A3
    #define ARDUINO_UNO_SHIELD_PIN_A3 A3
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_A4
    #define ARDUINO_UNO_SHIELD_PIN_A4 A4
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_A5
    #define ARDUINO_UNO_SHIELD_PIN_A5 A5
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_SDA
    #define ARDUINO_UNO_SHIELD_PIN_SDA SDA
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_SCL
    #define ARDUINO_UNO_SHIELD_PIN_SCL SCL
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_SS
    #define ARDUINO_UNO_SHIELD_PIN_SS SS
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_MOSI
    #define ARDUINO_UNO_SHIELD_PIN_MOSI MOSI
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_MISO
    #define ARDUINO_UNO_SHIELD_PIN_MISO MISO
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_SCK
    #define ARDUINO_UNO_SHIELD_PIN_SCK SCK
  #endif
#else
  #ifndef ARDUINO_UNO_SHIELD_PIN_D0
    #define ARDUINO_UNO_SHIELD_PIN_D0 0
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D1
    #define ARDUINO_UNO_SHIELD_PIN_D1 1
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D2
    #define ARDUINO_UNO_SHIELD_PIN_D2 2
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D3
    #define ARDUINO_UNO_SHIELD_PIN_D3 3
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D4
    #define ARDUINO_UNO_SHIELD_PIN_D4 4
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D5
    #define ARDUINO_UNO_SHIELD_PIN_D5 5
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D6
    #define ARDUINO_UNO_SHIELD_PIN_D6 6
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D7
    #define ARDUINO_UNO_SHIELD_PIN_D7 7
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D8
    #define ARDUINO_UNO_SHIELD_PIN_D8 8
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D9
    #define ARDUINO_UNO_SHIELD_PIN_D9 9
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D10
    #define ARDUINO_UNO_SHIELD_PIN_D10 10
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D11
    #define ARDUINO_UNO_SHIELD_PIN_D11 11
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D12
    #define ARDUINO_UNO_SHIELD_PIN_D12 12
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D13
    #define ARDUINO_UNO_SHIELD_PIN_D13 13
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_A0
    #define ARDUINO_UNO_SHIELD_PIN_A0 A0
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_A1
    #define ARDUINO_UNO_SHIELD_PIN_A1 A1
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_A2
    #define ARDUINO_UNO_SHIELD_PIN_A2 A2
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_A3
    #define ARDUINO_UNO_SHIELD_PIN_A3 A3
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_A4
    #define ARDUINO_UNO_SHIELD_PIN_A4 A4
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_A5
    #define ARDUINO_UNO_SHIELD_PIN_A5 A5
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_SDA
    #define ARDUINO_UNO_SHIELD_PIN_SDA SDA
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_SCL
    #define ARDUINO_UNO_SHIELD_PIN_SCL SCL
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_SS
    #define ARDUINO_UNO_SHIELD_PIN_SS SS
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_MOSI
    #define ARDUINO_UNO_SHIELD_PIN_MOSI MOSI
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_MISO
    #define ARDUINO_UNO_SHIELD_PIN_MISO MISO
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_SCK
    #define ARDUINO_UNO_SHIELD_PIN_SCK SCK
  #endif
#endif

// ============================================================================
// INPUT
// ============================================================================
#ifndef INPUT_KANAAL_CONFIG
  #define INPUT_KANAAL_CONFIG INPUT_TYPE_DIGITAL
#endif

#if (INPUT_KANAAL_CONFIG & (INPUT_TYPE_DIGITAL | INPUT_TYPE_PCF8574))
  #ifndef KEYPAD_TYPE
    #define KEYPAD_TYPE KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4
  #endif
#endif

#ifndef INPUT_DEBOUNCE_MS
  #define INPUT_DEBOUNCE_MS 25UL
#endif

#ifndef INPUT_KANAAL_TIMEOUT_BIJ_GEEN_TOETSAANSLAG_BINNEN_MS
  #define INPUT_KANAAL_TIMEOUT_BIJ_GEEN_TOETSAANSLAG_BINNEN_MS 0UL   // 0 = geen timeout, blijven wachten (huidig gedrag)
#endif

#ifndef MAX_AANTAL_SIMULTANE_TOETSAANSLAGEN
  #define MAX_AANTAL_SIMULTANE_TOETSAANSLAGEN 1
#endif

#ifndef I2C_ADDRESS_PCF8574
  #define I2C_ADDRESS_PCF8574 0x20
#endif

#if defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_DIRECT_1x4
  #ifndef KEYPAD_PIN_K1
    #define KEYPAD_PIN_K1 PCF8574_PIN_P0
  #endif
  #ifndef KEYPAD_PIN_K2
    #define KEYPAD_PIN_K2 PCF8574_PIN_P1
  #endif
  #ifndef KEYPAD_PIN_K3
    #define KEYPAD_PIN_K3 PCF8574_PIN_P2
  #endif
  #ifndef KEYPAD_PIN_K4
    #define KEYPAD_PIN_K4 PCF8574_PIN_P3
  #endif
#elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_DIRECT_2x4
  #ifndef KEYPAD_PIN_K1
    #define KEYPAD_PIN_K1 PCF8574_PIN_P0
  #endif
  #ifndef KEYPAD_PIN_K2
    #define KEYPAD_PIN_K2 PCF8574_PIN_P1
  #endif
  #ifndef KEYPAD_PIN_K3
    #define KEYPAD_PIN_K3 PCF8574_PIN_P2
  #endif
  #ifndef KEYPAD_PIN_K4
    #define KEYPAD_PIN_K4 PCF8574_PIN_P3
  #endif
  #ifndef KEYPAD_PIN_K5
    #define KEYPAD_PIN_K5 PCF8574_PIN_P4
  #endif
  #ifndef KEYPAD_PIN_K6
    #define KEYPAD_PIN_K6 PCF8574_PIN_P5
  #endif
  #ifndef KEYPAD_PIN_K7
    #define KEYPAD_PIN_K7 PCF8574_PIN_P6
  #endif
  #ifndef KEYPAD_PIN_K8
    #define KEYPAD_PIN_K8 PCF8574_PIN_P7
  #endif
#elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_MATRIX_2x2
  #ifndef KEYPAD_PIN_L1
    #define KEYPAD_PIN_L1 PCF8574_PIN_P0
  #endif
  #ifndef KEYPAD_PIN_L2
    #define KEYPAD_PIN_L2 PCF8574_PIN_P1
  #endif
  #ifndef KEYPAD_PIN_R1
    #define KEYPAD_PIN_R1 PCF8574_PIN_P2
  #endif
  #ifndef KEYPAD_PIN_R2
    #define KEYPAD_PIN_R2 PCF8574_PIN_P3
  #endif
#elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_MATRIX_4x4
  #ifndef KEYPAD_PIN_C4
    #define KEYPAD_PIN_C4 PCF8574_PIN_P0
  #endif
  #ifndef KEYPAD_PIN_C3
    #define KEYPAD_PIN_C3 PCF8574_PIN_P1
  #endif
  #ifndef KEYPAD_PIN_C2
    #define KEYPAD_PIN_C2 PCF8574_PIN_P2
  #endif
  #ifndef KEYPAD_PIN_C1
    #define KEYPAD_PIN_C1 PCF8574_PIN_P3
  #endif
  #ifndef KEYPAD_PIN_R1
    #define KEYPAD_PIN_R1 PCF8574_PIN_P4
  #endif
  #ifndef KEYPAD_PIN_R2
    #define KEYPAD_PIN_R2 PCF8574_PIN_P5
  #endif
  #ifndef KEYPAD_PIN_R3
    #define KEYPAD_PIN_R3 PCF8574_PIN_P6
  #endif
  #ifndef KEYPAD_PIN_R4
    #define KEYPAD_PIN_R4 PCF8574_PIN_P7
  #endif
#elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4
  #ifndef KEYPAD_PIN_K1
    #define KEYPAD_PIN_K1 PCF8574_PIN_P1
  #endif
  #ifndef KEYPAD_PIN_K2
    #define KEYPAD_PIN_K2 PCF8574_PIN_P0
  #endif
  #ifndef KEYPAD_PIN_K3
    #define KEYPAD_PIN_K3 PCF8574_PIN_P3
  #endif
  #ifndef KEYPAD_PIN_K4
    #define KEYPAD_PIN_K4 PCF8574_PIN_P2
  #endif
#elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_DIRECT_4x1
  #ifndef KEYPAD_PIN_K1
    #define KEYPAD_PIN_K1 PCF8574_PIN_P0
  #endif
  #ifndef KEYPAD_PIN_K2
    #define KEYPAD_PIN_K2 PCF8574_PIN_P1
  #endif
  #ifndef KEYPAD_PIN_K3
    #define KEYPAD_PIN_K3 PCF8574_PIN_P2
  #endif
  #ifndef KEYPAD_PIN_K4
    #define KEYPAD_PIN_K4 PCF8574_PIN_P3
  #endif
#elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_MATRIX_1x4
  #ifndef KEYPAD_PIN_R1
    #define KEYPAD_PIN_R1 PCF8574_PIN_P0
  #endif
  #ifndef KEYPAD_PIN_C1
    #define KEYPAD_PIN_C1 PCF8574_PIN_P1
  #endif
  #ifndef KEYPAD_PIN_C2
    #define KEYPAD_PIN_C2 PCF8574_PIN_P2
  #endif
  #ifndef KEYPAD_PIN_C3
    #define KEYPAD_PIN_C3 PCF8574_PIN_P3
  #endif
  #ifndef KEYPAD_PIN_C4
    #define KEYPAD_PIN_C4 PCF8574_PIN_P4
  #endif
#elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_MATRIX_2x4
  #ifndef KEYPAD_PIN_R1
    #define KEYPAD_PIN_R1 PCF8574_PIN_P0
  #endif
  #ifndef KEYPAD_PIN_R2
    #define KEYPAD_PIN_R2 PCF8574_PIN_P1
  #endif
  #ifndef KEYPAD_PIN_C1
    #define KEYPAD_PIN_C1 PCF8574_PIN_P2
  #endif
  #ifndef KEYPAD_PIN_C2
    #define KEYPAD_PIN_C2 PCF8574_PIN_P3
  #endif
  #ifndef KEYPAD_PIN_C3
    #define KEYPAD_PIN_C3 PCF8574_PIN_P4
  #endif
  #ifndef KEYPAD_PIN_C4
    #define KEYPAD_PIN_C4 PCF8574_PIN_P5
  #endif
#elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_MATRIX_4x3
  #ifndef KEYPAD_PIN_R1
    #define KEYPAD_PIN_R1 PCF8574_PIN_P0
  #endif
  #ifndef KEYPAD_PIN_R2
    #define KEYPAD_PIN_R2 PCF8574_PIN_P1
  #endif
  #ifndef KEYPAD_PIN_R3
    #define KEYPAD_PIN_R3 PCF8574_PIN_P2
  #endif
  #ifndef KEYPAD_PIN_R4
    #define KEYPAD_PIN_R4 PCF8574_PIN_P3
  #endif
  #ifndef KEYPAD_PIN_C1
    #define KEYPAD_PIN_C1 PCF8574_PIN_P4
  #endif
  #ifndef KEYPAD_PIN_C2
    #define KEYPAD_PIN_C2 PCF8574_PIN_P5
  #endif
  #ifndef KEYPAD_PIN_C3
    #define KEYPAD_PIN_C3 PCF8574_PIN_P6
  #endif
#elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_MATRIX_4x4
  #ifndef KEYPAD_PIN_R1
    #define KEYPAD_PIN_R1 PCF8574_PIN_P0
  #endif
  #ifndef KEYPAD_PIN_R2
    #define KEYPAD_PIN_R2 PCF8574_PIN_P1
  #endif
  #ifndef KEYPAD_PIN_R3
    #define KEYPAD_PIN_R3 PCF8574_PIN_P2
  #endif
  #ifndef KEYPAD_PIN_R4
    #define KEYPAD_PIN_R4 PCF8574_PIN_P3
  #endif
  #ifndef KEYPAD_PIN_C1
    #define KEYPAD_PIN_C1 PCF8574_PIN_P4
  #endif
  #ifndef KEYPAD_PIN_C2
    #define KEYPAD_PIN_C2 PCF8574_PIN_P5
  #endif
  #ifndef KEYPAD_PIN_C3
    #define KEYPAD_PIN_C3 PCF8574_PIN_P6
  #endif
  #ifndef KEYPAD_PIN_C4
    #define KEYPAD_PIN_C4 PCF8574_PIN_P7
  #endif
#elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_TOUCH_TTP224_DIRECT_1x4
  #ifndef KEYPAD_PIN_K1
    #define KEYPAD_PIN_K1 PCF8574_PIN_P0
  #endif
  #ifndef KEYPAD_PIN_K2
    #define KEYPAD_PIN_K2 PCF8574_PIN_P1
  #endif
  #ifndef KEYPAD_PIN_K3
    #define KEYPAD_PIN_K3 PCF8574_PIN_P2
  #endif
  #ifndef KEYPAD_PIN_K4
    #define KEYPAD_PIN_K4 PCF8574_PIN_P3
  #endif
#elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_TOUCH_TTP229_MATRIX_4x4
  #ifndef KEYPAD_PIN_K1
    #define KEYPAD_PIN_K1 PCF8574_PIN_P0
  #endif
  #ifndef KEYPAD_PIN_K2
    #define KEYPAD_PIN_K2 PCF8574_PIN_P1
  #endif
  #ifndef KEYPAD_PIN_K3
    #define KEYPAD_PIN_K3 PCF8574_PIN_P2
  #endif
  #ifndef KEYPAD_PIN_K4
    #define KEYPAD_PIN_K4 PCF8574_PIN_P3
  #endif
  #ifndef KEYPAD_PIN_K5
    #define KEYPAD_PIN_K5 PCF8574_PIN_P4
  #endif
  #ifndef KEYPAD_PIN_K6
    #define KEYPAD_PIN_K6 PCF8574_PIN_P5
  #endif
  #ifndef KEYPAD_PIN_K7
    #define KEYPAD_PIN_K7 PCF8574_PIN_P6
  #endif
  #ifndef KEYPAD_PIN_K8
    #define KEYPAD_PIN_K8 PCF8574_PIN_P7
  #endif
#endif

#ifndef TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED
  #define TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED_HIGH
#endif

#ifndef HX1838_BRON_CODES
  #define HX1838_BRON_CODES HX1838_BRON_CODES_EEPROM_WANNEER_GEEN_DEFINE
#endif

#ifndef HX1838_KALIBRATIE_TOETS_PAUZE_MS
  #define HX1838_KALIBRATIE_TOETS_PAUZE_MS 300UL
#endif

#ifndef HX1838_TOETSENINDELING
  #define HX1838_TOETSENINDELING HX1838_TOETSENINDELING_3x4
#endif

// GPIO2 is op ESP32-boards een strapping pin en vaak gekoppeld aan de ingebouwde LED, wat opstart-/flashproblemen kan geven als een extern onderdeel zoals de HX1838 daarop actief signaal geeft.
// D6 heeft die rol niet en is daarom de veiligere keuze op BOARD_ESP32_UNO. Bij AVR/Renesas speelt dit niet.
#ifndef HX1838_ONTVANGER_PIN
  #if BOARD_VERSION == BOARD_ESP32_UNO
    #define HX1838_ONTVANGER_PIN ARDUINO_UNO_SHIELD_PIN_D6
  #else
    #define HX1838_ONTVANGER_PIN ARDUINO_UNO_SHIELD_PIN_D2
  #endif
#endif

#ifndef HX1838_CODE_1
  #define HX1838_CODE_1 0UL
#endif
#ifndef HX1838_CODE_2
  #define HX1838_CODE_2 0UL
#endif
#ifndef HX1838_CODE_3
  #define HX1838_CODE_3 0UL
#endif
#ifndef HX1838_CODE_4
  #define HX1838_CODE_4 0UL
#endif
#ifndef HX1838_CODE_5
  #define HX1838_CODE_5 0UL
#endif
#ifndef HX1838_CODE_6
  #define HX1838_CODE_6 0UL
#endif
#ifndef HX1838_CODE_7
  #define HX1838_CODE_7 0UL
#endif
#ifndef HX1838_CODE_8
  #define HX1838_CODE_8 0UL
#endif
#ifndef HX1838_CODE_9
  #define HX1838_CODE_9 0UL
#endif
#ifndef HX1838_CODE_10
  #define HX1838_CODE_10 0UL
#endif
#ifndef HX1838_CODE_11
  #define HX1838_CODE_11 0UL
#endif
#ifndef HX1838_CODE_12
  #define HX1838_CODE_12 0UL
#endif
#ifndef HX1838_CODE_13
  #define HX1838_CODE_13 0UL
#endif
#ifndef HX1838_CODE_14
  #define HX1838_CODE_14 0UL
#endif
#ifndef HX1838_CODE_15
  #define HX1838_CODE_15 0UL
#endif
#ifndef HX1838_CODE_16
  #define HX1838_CODE_16 0UL
#endif
#ifndef HX1838_CODE_17
  #define HX1838_CODE_17 0UL
#endif
#ifndef HX1838_CODE_18
  #define HX1838_CODE_18 0UL
#endif
#ifndef HX1838_CODE_19
  #define HX1838_CODE_19 0UL
#endif
#ifndef HX1838_CODE_20
  #define HX1838_CODE_20 0UL
#endif
#ifndef HX1838_CODE_21
  #define HX1838_CODE_21 0UL
#endif

#ifndef PIN_TOETS_1
  #define PIN_TOETS_1 ARDUINO_UNO_SHIELD_PIN_D3
#endif
#ifndef PIN_TOETS_2
  #define PIN_TOETS_2 ARDUINO_UNO_SHIELD_PIN_D2
#endif
#ifndef PIN_TOETS_3
  #define PIN_TOETS_3 ARDUINO_UNO_SHIELD_PIN_D5
#endif
#ifndef PIN_TOETS_4
  #define PIN_TOETS_4 ARDUINO_UNO_SHIELD_PIN_D4
#endif

#if !defined(LANGUAGE_NL) && !defined(LANGUAGE_DE) && !defined(LANGUAGE_EN) && !defined(LANGUAGE_FR)
  #define LANGUAGE_NL
  // #define LANGUAGE_DE
  // #define LANGUAGE_EN
  // #define LANGUAGE_FR
#endif

#if (defined(LANGUAGE_NL) + defined(LANGUAGE_DE) + defined(LANGUAGE_EN) + defined(LANGUAGE_FR)) == 0
  #error Selecteer een taal in UserConfig.h of SystemConfig.h: LANGUAGE_NL, LANGUAGE_DE, LANGUAGE_EN of LANGUAGE_FR.
#elif (defined(LANGUAGE_NL) + defined(LANGUAGE_DE) + defined(LANGUAGE_EN) + defined(LANGUAGE_FR)) > 1
  #error Selecteer slechts een taal in UserConfig.h of SystemConfig.h.
#endif

// ============================================================================
// AANPASSEN NAAR EIGEN SETUP DOE JE BEST VIA: 'UserConfig.h' OMDAT NIEUWE RELEASE DIT BESTANND OVERSCHRIJVEN!
// ============================================================================
#ifdef TRACE  // ENKEL wanneer TRACE nodig, staan hier geen commentaar '//' tekens voor :)
  #ifndef DEBUG
    #define DEBUG
  #endif
#endif

// SCREEN_OUTPUT_CONFIG bepaalt welke schermuitvoertypes in deze build aanwezig zijn.
// Combineer meerdere uitvoertypes met de bitwise OR-operator |.
//
// Mogelijke waarden:
// 0 = geen uitvoer                                 (SCREEN_TYPE_NONE)
// 1 = Serial                                       (SCREEN_TYPE_SERIAL)
// 2 = CharacterScreen                              (SCREEN_TYPE_CHARACTER)
// 3 = Serial + CharacterScreen
// 4 = PixelScreen                                  (SCREEN_TYPE_PIXELS)
// 5 = Serial + PixelScreen
// 6 = CharacterScreen + PixelScreen
// 7 = Serial + CharacterScreen + PixelScreen
//
// Wat hier niet geselecteerd wordt, wordt niet gecompileerd. Meerdere selecteren doe je door | tussen jouw keuzes te plaatsen.
//
// Opmerking:
// Wanneer DEBUG actief is, voegt de library automatisch SCREEN_TYPE_SERIAL toe. Je hoeft SCREEN_TYPE_SERIAL dus niet zelf te selecteren voor debug-uitvoer.
//
// Kies hieronder welke schermuitvoer je wilt gebruiken; staat standaard op: geen uitvoer.
#ifndef SCREEN_OUTPUT_CONFIG
  #define SCREEN_OUTPUT_CONFIG SCREEN_TYPE_NONE
#endif

// ============================================================================
// SCHERMEN
// ============================================================================
// Characterscherm
#ifndef I2C_ADDRESS_CHARACTER_SCREEN
  #ifdef I2C_ADRES
    #define I2C_ADDRESS_CHARACTER_SCREEN I2C_ADRES
  #else
    #define I2C_ADDRESS_CHARACTER_SCREEN 0x27
  #endif
#endif

#ifndef I2C_ADRES
  #define I2C_ADRES I2C_ADDRESS_CHARACTER_SCREEN
#endif

// CHARACTERSCREEN_I2C_ADRES_MODUS: 
// 0 = geen scan (kleinste footprint), 
// 1 = scan + rapporteren (standaard),
// 2 = scan + automatisch herbouwen op het gevonden adres.
#ifndef CHARACTERSCREEN_I2C_ADRES_MODUS
  #define CHARACTERSCREEN_I2C_ADRES_MODUS 1
#endif
#ifndef ACTIEF_CHARACTER_SCREEN
  #define ACTIEF_CHARACTER_SCREEN       SCREEN_LCD1602
#endif

#ifndef DEFAULT_TOEGESTANE_MARGE_SIMULTANE_STARTTIJD_MS
  #define DEFAULT_TOEGESTANE_MARGE_SIMULTANE_STARTTIJD_MS 100UL
#endif

// Pixelscherm
#ifndef ACTIEF_PIXEL_SCREEN
  #define ACTIEF_PIXEL_SCREEN           SCREEN_240X320
#endif

#ifndef PIXEL_SCREEN_CS
  #define PIXEL_SCREEN_CS               ARDUINO_UNO_SHIELD_PIN_SS
#endif
#ifndef PIXEL_SCREEN_DC
 #define PIXEL_SCREEN_DC                ARDUINO_UNO_SHIELD_PIN_D9
#endif
#ifndef PIXEL_SCREEN_RST
  #define PIXEL_SCREEN_RST              ARDUINO_UNO_SHIELD_PIN_D8
#endif

#ifndef PIXEL_SCREEN_ROTATION
  #define PIXEL_SCREEN_ROTATION         0
#endif
#ifndef PIXEL_SCREEN_TEXT_SIZE
  #define PIXEL_SCREEN_TEXT_SIZE        2
#endif
#ifndef PIXEL_SCREEN_MARGIN
  #define PIXEL_SCREEN_MARGIN           5
#endif
#ifndef PIXEL_SCREEN_CHARACTER_SPACING
  #define PIXEL_SCREEN_CHARACTER_SPACING 1
#endif
#ifndef PIXEL_SCREEN_LINE_SPACING
  #define PIXEL_SCREEN_LINE_SPACING     2
#endif
#ifndef PIXEL_SCREEN_TEXT_COLOR
  #define PIXEL_SCREEN_TEXT_COLOR       0xFFFF
#endif
#ifndef PIXEL_SCREEN_BACKGROUND_COLOR
  #define PIXEL_SCREEN_BACKGROUND_COLOR 0x0000
#endif

// FATAL
#ifndef FATAL_LEESTIJD_MS
  #define FATAL_LEESTIJD_MS   30000UL
#endif

// ============================================================================
// ADC BACKEND (naast BOARD_VERSION)
// ============================================================================
// LET OP: Wanneer je ADC_BACKEND op ADC_BACKEND_ADS1115 zet, moet de Adafruit ADS1X15-library geïnstalleerd zijn via de Arduino Library Manager.
// ============================================================================
#ifndef AANTAL_SENSOREN_AANWEZIG
  #define AANTAL_SENSOREN_AANWEZIG  2
#endif

#if AANTAL_SENSOREN_AANWEZIG != 2 && AANTAL_SENSOREN_AANWEZIG != 4
  #error AANTAL_SENSOREN_AANWEZIG moet 2 of 4 zijn.
#endif

#ifndef ADC_BACKEND
  #define ADC_BACKEND               ADC_BACKEND_NATIVE  // wissel dit om van backend te wisselen
#endif
#if ADC_BACKEND != ADC_BACKEND_NATIVE && ADC_BACKEND != ADC_BACKEND_ADS1115
  #error Selecteer een geldige ADC_BACKEND.
#endif

#ifndef I2C_ADDRESS_ADS1115
  #ifdef ADS1115_I2C_ADDRESS
    #define I2C_ADDRESS_ADS1115 ADS1115_I2C_ADDRESS
  #else
    #define I2C_ADDRESS_ADS1115 0x48
  #endif
#endif

#ifndef ADS1115_I2C_ADDRESS
  #define ADS1115_I2C_ADDRESS I2C_ADDRESS_ADS1115
#endif

// Instelbare vertraging in de busy-wait loop van WachtTotAlleSensorsLosgelatenVoorTest().
// Bij ADC_BACKEND_NATIVE blijft dit 0. Bij ADC_BACKEND_ADS1115 voorkomt dit dat de I2C-bus zonder onderbreking bevraagd wordt.
#ifndef WACHT_LOSLATEN_DELAY_MS
  #if ADC_BACKEND == ADC_BACKEND_ADS1115
    #define WACHT_LOSLATEN_DELAY_MS 5UL
  #else
    #define WACHT_LOSLATEN_DELAY_MS 0UL
  #endif
#endif

// Definieer de pinnen voor de sensoren op jouw sensorboard
#if ADC_BACKEND == ADC_BACKEND_ADS1115
  #ifndef PIN_SENSOR_1
    #define PIN_SENSOR_1 0   // ADS1115-kanaal 0
  #endif
  #ifndef PIN_SENSOR_2
    #define PIN_SENSOR_2 1   // ADS1115-kanaal 1
  #endif
  #ifndef PIN_SENSOR_3
    #define PIN_SENSOR_3 2   // ADS1115-kanaal 2
  #endif
  #ifndef PIN_SENSOR_4
    #define PIN_SENSOR_4 3   // ADS1115-kanaal 3
  #endif
#else
  #ifndef PIN_SENSOR_1
    #define PIN_SENSOR_1 ARDUINO_UNO_SHIELD_PIN_A0  // Analoge pin voor de 1ste test-sensor
  #endif
  #ifndef PIN_SENSOR_2
    #define PIN_SENSOR_2 ARDUINO_UNO_SHIELD_PIN_A1  // Analoge pin voor de  2de test-sensor
  #endif
  #ifndef PIN_SENSOR_3
    #define PIN_SENSOR_3 ARDUINO_UNO_SHIELD_PIN_A2  // Analoge pin voor de  3de test-sensor
  #endif
  #ifndef PIN_SENSOR_4
    #define PIN_SENSOR_4 ARDUINO_UNO_SHIELD_PIN_A3  // Analoge pin voor de  4de test-sensor
  #endif
#endif

#ifndef ADC_BITS
  #if ADC_BACKEND == ADC_BACKEND_ADS1115
    #define ADC_BITS 15      // ADS1115 single-ended: 0-32767 bruikbaar bereik
  #elif BOARD_VERSION == BOARD_UNO_R3
    #define ADC_BITS 10      // R3 = 10
  #elif (BOARD_VERSION == BOARD_UNO_R4_MINIMA || BOARD_VERSION == BOARD_UNO_R4_WIFI)
    #define ADC_BITS 14      // R4 = 10 of 14
  #elif BOARD_VERSION == BOARD_ARDI32
    #define ADC_BITS 12      // ESP32-S3 Arduino core: analogRead() standaard 12-bit
  #elif BOARD_VERSION == BOARD_ESP32_UNO
    #define ADC_BITS 12      // zie kanttekening in docs/Configuratie/SystemConfig.md
  #elif BOARD_VERSION == BOARD_CYTRON_MAKER_UNO_RP2040
    #define ADC_BITS 10      // Earle Philhower RP2040 core: analogReadResolution() standaard 10-bit
  #elif BOARD_VERSION == BOARD_NUCLEO_F401RE
    #define ADC_BITS 10      // STM32duino: analogRead() standaard 10-bit voor Arduino-compatibiliteit
  #endif
#endif

#ifndef DELAY_US
  #if ADC_BACKEND == ADC_BACKEND_ADS1115
    #define DELAY_US 0     // niet van toepassing; I2C-conversietijd zit in de leesfunctie zelf
  #elif BOARD_VERSION == BOARD_UNO_R3
    #define DELAY_US 100   // Vertraging tussen samples in de meetlussen
  #elif (BOARD_VERSION == BOARD_UNO_R4_MINIMA || BOARD_VERSION == BOARD_UNO_R4_WIFI)
    #define DELAY_US 200   // Vertraging tussen samples in de meetlussen
  #elif BOARD_VERSION == BOARD_ARDI32
    #define DELAY_US 0
  #elif BOARD_VERSION == BOARD_ESP32_UNO
    #define DELAY_US 0     // zie kanttekening in docs/Configuratie/SystemConfig.md
  #elif BOARD_VERSION == BOARD_CYTRON_MAKER_UNO_RP2040
    #define DELAY_US 0
  #elif BOARD_VERSION == BOARD_NUCLEO_F401RE
    #define DELAY_US 0
  #endif
#endif

#define ADC_MAX ((1UL << ADC_BITS) - 1UL)

#if ADC_BITS == 10
  #define ADC(x) (x)
#elif ADC_BITS == 12 || ADC_BITS == 14 || ADC_BITS == 15
  #define ADC(x) (((x) * ADC_MAX) / 1023UL)
#else
  #error ADC_BITS moet 10, 12, 14 of 15 zijn.
#endif

// ============================================================================
// DEBUG INSTELLINGEN 
// ============================================================================
#if defined(DEBUG)
  #define DEBUG_PRINT(x)          Serial.print(x)
  #define DEBUG_PRINTLN(x)        Serial.println(x)
  #define DEBUG_PRINTLN2(x, f)    Serial.println(x, f)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
  #define DEBUG_PRINTLN2(x, f)
#endif

#endif
