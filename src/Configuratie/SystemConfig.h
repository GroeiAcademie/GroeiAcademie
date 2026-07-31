#ifndef SYSTEM_CONFIG_H
#define SYSTEM_CONFIG_H

#include "../Systeem/Screen/ScreenTypes.h"

// Vaste keuzewaarden die vóór UserConfig.h beschikbaar moeten zijn.
#ifndef ADC_BACKEND_NATIVE
  #define ADC_BACKEND_NATIVE      0                   // ingebouwde ADC van de Arduino
#endif
#ifndef ADC_BACKEND_ADS1115
  #define ADC_BACKEND_ADS1115     1                   // externe 16-bit ADC via I2C
#endif

#define BOARD_UNO_R3              0
#define BOARD_UNO_R4_MINIMA       1
#define BOARD_UNO_R4_WIFI         2
#define BOARD_ESP32_UNO           3

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
  #define BOARD_VERSION BOARD_UNO_R3   // wissel dit om van bord te wisselen
#endif

#if BOARD_VERSION != BOARD_UNO_R3 && BOARD_VERSION != BOARD_UNO_R4_MINIMA && BOARD_VERSION != BOARD_UNO_R4_WIFI && BOARD_VERSION != BOARD_ESP32_UNO
  #error Selecteer een geldige BOARD_VERSION.
#endif

#if BOARD_VERSION == BOARD_ESP32_UNO
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
#ifndef I2C_ADRES
  #define I2C_ADRES                     0x27
#endif
#ifndef ACTIEF_CHARACTER_SCREEN
  #define ACTIEF_CHARACTER_SCREEN       SCREEN_LCD1602
#endif
#ifndef LCD_LEESTIJD_FOUTMELDING_MS
  #define LCD_LEESTIJD_FOUTMELDING_MS   2000UL
#endif
#ifndef DEFAULT_TOEGESTANE_MARGE_SIMULTANE_STARTTIJD_MS
  #define DEFAULT_TOEGESTANE_MARGE_SIMULTANE_STARTTIJD_MS 100UL
#endif

// Pixelscherm
#ifndef ACTIEF_PIXEL_SCREEN
  #define ACTIEF_PIXEL_SCREEN           SCREEN_240X320
#endif

#ifndef PIXEL_SCREEN_CS
  #define PIXEL_SCREEN_CS               SS
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
#ifndef ADS1115_I2C_ADDRESS
  #define ADS1115_I2C_ADDRESS       0x48                // standaardadres via SW1: ADDR naar GND
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
    #define PIN_SENSOR_1 A0  // Analoge pin voor de 1ste test-sensor
  #endif
  #ifndef PIN_SENSOR_2
    #define PIN_SENSOR_2 A1  // Analoge pin voor de  2de test-sensor
  #endif
  #ifndef PIN_SENSOR_3
    #define PIN_SENSOR_3 A2  // Analoge pin voor de  3de test-sensor
  #endif
  #ifndef PIN_SENSOR_4
    #define PIN_SENSOR_4 A3  // Analoge pin voor de  4de test-sensor
  #endif
#endif

#if ADC_BACKEND == ADC_BACKEND_ADS1115
  #define ADC_BITS 15      // ADS1115 single-ended: 0-32767 bruikbaar bereik
  #ifndef DELAY_US
    #define DELAY_US 0     // niet van toepassing; I2C-conversietijd zit in de leesfunctie zelf
  #endif
#elif BOARD_VERSION == BOARD_UNO_R3
  #define ADC_BITS 10      // R3 = 10
  #ifndef DELAY_US
    #define DELAY_US 100   // Vertraging tussen samples in de meetlussen
  #endif
#elif (BOARD_VERSION == BOARD_UNO_R4_MINIMA || BOARD_VERSION == BOARD_UNO_R4_WIFI)
  #define ADC_BITS 14      // R4 = 10 of 14
  #ifndef DELAY_US
    #define DELAY_US 200   // Vertraging tussen samples in de meetlussen
  #endif
#elif BOARD_VERSION == BOARD_ESP32_UNO
  #define ADC_BITS 12      // zie kanttekening in docs/Configuratie/SystemConfig.md
  #ifndef DELAY_US
    #define DELAY_US 0     // zie kanttekening in docs/Configuratie/SystemConfig.md
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
