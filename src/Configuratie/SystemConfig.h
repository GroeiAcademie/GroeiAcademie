#ifndef SYSTEM_CONFIG_H
#define SYSTEM_CONFIG_H

#include "../Systeem/Screen/ScreenTypes.h"

#define LANGUAGE_NL
// #define LANGUAGE_DE
// #define LANGUAGE_EN
// #define LANGUAGE_FR

#if (defined(LANGUAGE_NL) + defined(LANGUAGE_DE) + defined(LANGUAGE_EN) + defined(LANGUAGE_FR)) == 0
  #error Selecteer een taal in SystemConfig.h: LANGUAGE_NL, LANGUAGE_DE, LANGUAGE_EN of LANGUAGE_FR.
#elif (defined(LANGUAGE_NL) + defined(LANGUAGE_DE) + defined(LANGUAGE_EN) + defined(LANGUAGE_FR)) > 1
  #error Selecteer slechts een taal in SystemConfig.h.
#endif

// ============================================================================
// MAG JE AANPASSEN NAAR EIGEN SETUP
// ============================================================================
#define DEBUG // ENKEL wanneer DEBUG nodig, staan hier geen commentaar '//' tekens voor :)

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
// Kies hieronder welke schermuitvoer je wilt gebruiken; staat standaard op: CharacterScreen.
#ifndef SCREEN_OUTPUT_CONFIG
  #define SCREEN_OUTPUT_CONFIG SCREEN_TYPE_CHARACTER | SCREEN_TYPE_PIXELS | SCREEN_TYPE_SERIAL
#endif

// ============================================================================
// SCHERMEN
// ============================================================================
// Characterscherm
#define I2C_ADRES                     0x27
#define ACTIEF_CHARACTER_SCREEN       SCREEN_LCD1602
#define LCD_LEESTIJD_FOUTMELDING_MS   2000UL

// Pixelscherm
#define ACTIEF_PIXEL_SCREEN           SCREEN_240X320

#define PIXEL_SCREEN_CS               10
#define PIXEL_SCREEN_DC               9
#define PIXEL_SCREEN_RST              8

#define PIXEL_SCREEN_ROTATION         0
#define PIXEL_SCREEN_TEXT_SIZE        2
#define PIXEL_SCREEN_TEXT_COLOR       0xFFFF
#define PIXEL_SCREEN_BACKGROUND_COLOR 0x0000

// ============================================================================
// ADC BACKEND (naast UNO_VERSION)
// ============================================================================
// LET OP: Wanneer je ADC_BACKEND op ADC_BACKEND_ADS1115 zet, moet de Adafruit ADS1X15-library geïnstalleerd zijn via de Arduino Library Manager.
// ============================================================================
#define AANTAL_SENSOREN_AANWEZIG  2

#define ADC_BACKEND_NATIVE        0                   // ingebouwde ADC van de Arduino
#define ADC_BACKEND_ADS1115       1                   // externe 16-bit ADC via I2C

#define ADC_BACKEND               ADC_BACKEND_NATIVE  // wissel dit om van backend te wisselen
#define ADS1115_I2C_ADDRESS       0x48                // standaardadres via SW1: ADDR naar GND

// Instelbare vertraging in de busy-wait loop van WachtTotAlleSensorsLosgelatenVoorTest().
// Bij ADC_BACKEND_NATIVE blijft dit 0. Bij ADC_BACKEND_ADS1115 voorkomt dit dat de I2C-bus zonder onderbreking bevraagd wordt.
#if ADC_BACKEND == ADC_BACKEND_ADS1115
  #define WACHT_LOSLATEN_DELAY_MS 5UL
#else
  #define WACHT_LOSLATEN_DELAY_MS 0UL
#endif

// Definieer de pinnen voor de sensoren op jouw sensorboard
#if ADC_BACKEND == ADC_BACKEND_ADS1115
  #define PIN_SENSOR_1 0   // ADS1115-kanaal 0
  #define PIN_SENSOR_2 1   // ADS1115-kanaal 1
  #define PIN_SENSOR_3 2   // ADS1115-kanaal 2
  #define PIN_SENSOR_4 3   // ADS1115-kanaal 3
#else
  #define PIN_SENSOR_1 A0  // Analoge pin voor de 1ste test-sensor
  #define PIN_SENSOR_2 A1  // Analoge pin voor de  2de test-sensor
  #define PIN_SENSOR_3 A2  // Analoge pin voor de  3de test-sensor
  #define PIN_SENSOR_4 A3  // Analoge pin voor de  4de test-sensor
#endif

// Definieer type Arduino UNO R3 of R4
#define UNO_VERSION   3  // R3=3 & R4=4

#if ADC_BACKEND == ADC_BACKEND_ADS1115
  #define ADC_BITS 15    // ADS1115 single-ended: 0-32767 bruikbaar bereik
  #define DELAY_US 0     // niet van toepassing; I2C-conversietijd zit in de leesfunctie zelf
#elif UNO_VERSION == 3
  #define ADC_BITS 10    // R3 = 10
  #define DELAY_US 100   // Vertraging tussen samples in de meetlussen
#elif UNO_VERSION == 4
  #define ADC_BITS 14    // R4 = 10 of 14
  #define DELAY_US 200   // Vertraging tussen samples in de meetlussen
#else
  #error UNO_VERSION moet 3 of 4 zijn.
#endif

#define ADC_MAX ((1UL << ADC_BITS) - 1)

#if ADC_BITS == 10
  #define ADC(x) (x)
#elif ADC_BITS == 14 || ADC_BITS == 15
  #define ADC(x) (((x) * ADC_MAX) / 1023L)
#else
  #error ADC_BITS moet 10, 14 of 15 zijn.
#endif

// ============================================================================
// DEBUG INSTELLINGEN 
// ============================================================================
#if defined(DEBUG) && (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
  #define DEBUG_PRINT(x)          Serial.print(x)
  #define DEBUG_PRINTLN(x)        Serial.println(x)
  #define DEBUG_PRINTLN2(x, f)    Serial.println(x, f)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
  #define DEBUG_PRINTLN2(x, f)
#endif

#endif
