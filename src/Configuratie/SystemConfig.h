#ifndef SYSTEM_CONFIG_H
#define SYSTEM_CONFIG_H

// ============================================================================
// MAG JE AANPASSEN NAAR EIGEN SETUP
// ============================================================================
//#define DEBUG // ENKEL wanneer DEBUG nodig, staan hier geen commentaar '//' tekens voor :)

// ============================================================================
// SCHERM ADRES & 
// ============================================================================
// Characterscherm: I2C adres
#define I2C_ADRES       0x27                   // Soms 0x32 of 0x3F
#define ACTIEF_CHARACTER_SCREEN SCREEN_LCD1602 // Kies jouw resolutie (zie mogelijke Characterschermen in screen.h)

// Hardwarematig aanwezig zijn 2 of 4 sensoren. Scenarios 1, 2 en 3 gebruiken 2 sensoren. Scenario 4 kan bij 4 aanwezige sensoren ook 3 sensoren gebruiken.
#define AANTAL_SENSOREN_AANWEZIG   2 // DIT MAG MEN WEIZIGEN IN FUNCTIE VAN DE BESCHIKBARE HARDWARE

// Definieer de pinnen voor de sensoren op jouw sensorboard
#define PIN_SENSOR_1 A0  // Analoge pin voor de 1ste test-sensor
#define PIN_SENSOR_2 A1  // Analoge pin voor de  2de test-sensor
#define PIN_SENSOR_3 A2  // Analoge pin voor de  3de test-sensor
#define PIN_SENSOR_4 A3  // Analoge pin voor de  4de test-sensor

// Definieer type Arduino UNO (of ESP32)
#define UNO_VERSION   3  // R3=3 & R4=4

#if UNO_VERSION == 3
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
#elif ADC_BITS == 14
  #define ADC(x) (((x) * ADC_MAX) / 1023L)
#else
  #error ADC_BITS moet 10 of 14 zijn.
#endif

// ============================================================================
// DEBUG INSTELLINGEN 
// ============================================================================
#ifdef DEBUG
  #define DEBUG_PRINT(x)          Serial.print(x)
  #define DEBUG_PRINTLN(x)        Serial.println(x)
  #define DEBUG_PRINTLN2(x, f)    Serial.println(x, f)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
  #define DEBUG_PRINTLN2(x, f) 
#endif

#endif

