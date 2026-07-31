#ifndef GROEIACADEMIE_USER_CONFIG_H
#define GROEIACADEMIE_USER_CONFIG_H

// Kopieer dit bestand vóór gebruik naar UserConfig.h in dezelfde map.
// UserConfig.h is optioneel: zonder dit bestand gebruikt de library uitsluitend de standaardwaarden uit SystemConfig.h.
// Een aanwezig UserConfig.h wordt vóór de standaardwaarden uit SystemConfig.h ingelezen.
// De volledige oorspronkelijke structuur, headers, commentaren, volgorde en witregels uit SystemConfig.h blijven hieronder behouden.
//
// ============================================================================
// BELANGRIJK — CENTRALE GEBRUIKERSCONFIGURATIE
// ============================================================================
// Screen.cpp en Stimulus.cpp worden door de Arduino-toolchain als losse,
// onafhankelijke bestanden gecompileerd, niet samen met je .ino. 
// Instellingen die bepalen wat in deze librarybestanden wordt
// gecompileerd horen daarom in UserConfig.h of, als officiële fallback, in SystemConfig.h.
//
// Gewone examples stellen deze libraryconfiguratie niet rechtstreeks in de .ino in.
// Instellingen die uitsluitend het voorbeeldprogramma zelf gebruikt, 
// horen in ExamplesConfig.h of lokaal in dat voorbeeld.
// ============================================================================

// #define LANGUAGE_NL
// #define LANGUAGE_DE
// #define LANGUAGE_EN
// #define LANGUAGE_FR

// ============================================================================
// MAG JE AANPASSEN NAAR EIGEN SETUP
// ============================================================================
// #define DEBUG // ENKEL wanneer DEBUG nodig, staan hier geen commentaar '//' tekens voor :)

// #define TRACE // ENKEL wanneer TRACE nodig, staan hier geen commentaar '//' tekens voor :)

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
// #define SCREEN_OUTPUT_CONFIG SCREEN_TYPE_NONE
#endif

// Arduino Uno-shieldheaderpinnen. Alleen activeren wanneer de geselecteerde boardcore een afwijkende mapping nodig heeft.
// #define ARDUINO_UNO_SHIELD_PIN_D0  0
// #define ARDUINO_UNO_SHIELD_PIN_D1  1
// #define ARDUINO_UNO_SHIELD_PIN_D2  2
// #define ARDUINO_UNO_SHIELD_PIN_D3  3
// #define ARDUINO_UNO_SHIELD_PIN_D4  4
// #define ARDUINO_UNO_SHIELD_PIN_D5  5
// #define ARDUINO_UNO_SHIELD_PIN_D6  6
// #define ARDUINO_UNO_SHIELD_PIN_D7  7
// #define ARDUINO_UNO_SHIELD_PIN_D8  8
// #define ARDUINO_UNO_SHIELD_PIN_D9  9
// #define ARDUINO_UNO_SHIELD_PIN_D10 10
// #define ARDUINO_UNO_SHIELD_PIN_D11 11
// #define ARDUINO_UNO_SHIELD_PIN_D12 12
// #define ARDUINO_UNO_SHIELD_PIN_D13 13

// Definieer de pinnen voor het 1x4 keypad
// #define PIN_TOETS_1 ARDUINO_UNO_SHIELD_PIN_D3
// #define PIN_TOETS_2 ARDUINO_UNO_SHIELD_PIN_D2
// #define PIN_TOETS_3 ARDUINO_UNO_SHIELD_PIN_D5
// #define PIN_TOETS_4 ARDUINO_UNO_SHIELD_PIN_D4

// ============================================================================
// SCHERMEN
// ============================================================================
// Characterscherm
#ifndef I2C_ADRES
// #define I2C_ADRES                     0x27
#endif
#ifndef ACTIEF_CHARACTER_SCREEN
// #define ACTIEF_CHARACTER_SCREEN       SCREEN_LCD1602
#endif
#ifndef LCD_LEESTIJD_FOUTMELDING_MS
// #define LCD_LEESTIJD_FOUTMELDING_MS   2000UL
#endif
#ifndef DEFAULT_TOEGESTANE_MARGE_SIMULTANE_STARTTIJD_MS
// #define DEFAULT_TOEGESTANE_MARGE_SIMULTANE_STARTTIJD_MS 100UL
#endif

// Pixelscherm
#ifndef ACTIEF_PIXEL_SCREEN
// #define ACTIEF_PIXEL_SCREEN           SCREEN_240X320
#endif

#ifndef PIXEL_SCREEN_CS
// #define PIXEL_SCREEN_CS               SS
#endif
#ifndef PIXEL_SCREEN_DC
// #define PIXEL_SCREEN_DC               ARDUINO_UNO_SHIELD_PIN_D9
#endif
#ifndef PIXEL_SCREEN_RST
// #define PIXEL_SCREEN_RST              ARDUINO_UNO_SHIELD_PIN_D8
#endif

#ifndef PIXEL_SCREEN_ROTATION
// #define PIXEL_SCREEN_ROTATION         0
#endif
#ifndef PIXEL_SCREEN_TEXT_SIZE
// #define PIXEL_SCREEN_TEXT_SIZE        2
#endif
#ifndef PIXEL_SCREEN_MARGIN
// #define PIXEL_SCREEN_MARGIN           5
#endif
#ifndef PIXEL_SCREEN_CHARACTER_SPACING
// #define PIXEL_SCREEN_CHARACTER_SPACING 1
#endif
#ifndef PIXEL_SCREEN_LINE_SPACING
// #define PIXEL_SCREEN_LINE_SPACING     2
#endif
#ifndef PIXEL_SCREEN_TEXT_COLOR
// #define PIXEL_SCREEN_TEXT_COLOR       0xFFFF
#endif
#ifndef PIXEL_SCREEN_BACKGROUND_COLOR
// #define PIXEL_SCREEN_BACKGROUND_COLOR 0x0000
#endif

// ============================================================================
// BOARD_VERSION
// ============================================================================
// Mogelijke waarden: BOARD_UNO_R3, BOARD_UNO_R4_MINIMA, BOARD_UNO_R4_WIFI of BOARD_ESP32_UNO
#ifndef BOARD_VERSION
// #define BOARD_VERSION BOARD_UNO_R3   // wissel dit om van bord te wisselen
#endif

// ============================================================================
// ADC BACKEND (naast BOARD_VERSION)
// ============================================================================
// LET OP: Wanneer je ADC_BACKEND op ADC_BACKEND_ADS1115 zet, moet de Adafruit ADS1X15-library geïnstalleerd zijn via de Arduino Library Manager.
// ============================================================================
#ifndef AANTAL_SENSOREN_AANWEZIG
// #define AANTAL_SENSOREN_AANWEZIG  2
#endif

// Mogelijke waarden: ADC_BACKEND_NATIVE of ADC_BACKEND_ADS1115
#ifndef ADC_BACKEND
// #define ADC_BACKEND               ADC_BACKEND_NATIVE  // wissel dit om van backend te wisselen
#endif
#ifndef ADS1115_I2C_ADDRESS
// #define ADS1115_I2C_ADDRESS       0x48                // standaardadres via SW1: ADDR naar GND
#endif

// Instelbare vertraging in de busy-wait loop van WachtTotAlleSensorsLosgelatenVoorTest().
// Bij ADC_BACKEND_NATIVE blijft dit 0. Bij ADC_BACKEND_ADS1115 voorkomt dit dat de I2C-bus zonder onderbreking bevraagd wordt.
#ifndef WACHT_LOSLATEN_DELAY_MS
// #define WACHT_LOSLATEN_DELAY_MS 0UL
#endif

// Definieer de pinnen voor de sensoren op jouw sensorboard
// Bij ADC_BACKEND_ADS1115 zijn dit kanaalnummers 0 tot 3; bij ADC_BACKEND_NATIVE zijn dit Arduino-pinnen.
#ifndef PIN_SENSOR_1
// #define PIN_SENSOR_1 A0  // Analoge pin voor de 1ste test-sensor
#endif
#ifndef PIN_SENSOR_2
// #define PIN_SENSOR_2 A1  // Analoge pin voor de  2de test-sensor
#endif
#ifndef PIN_SENSOR_3
// #define PIN_SENSOR_3 A2  // Analoge pin voor de  3de test-sensor
#endif
#ifndef PIN_SENSOR_4
// #define PIN_SENSOR_4 A3  // Analoge pin voor de  4de test-sensor
#endif

#ifndef DELAY_US
// #define DELAY_US 100   // Vertraging tussen samples in de meetlussen
#endif

// ============================================================================
// DEBUG INSTELLINGEN 
// ============================================================================
// DEBUG_PRINT, DEBUG_PRINTLN en DEBUG_PRINTLN2 worden door SystemConfig.h afgeleid uit DEBUG.

#endif
