#ifndef SCREEN_H
#define SCREEN_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

extern LiquidCrystal_I2C lcd;

// SCHERM TYPES
#define SCREEN_TYPE_NONE       0x00 // geen uitvoer
#define SCREEN_TYPE_SERIAL     0x01 // gebruik seriële console
#define SCREEN_TYPE_CHARACTER  0x02 // characterscherm
#define SCREEN_TYPE_PIXELS     0x04 // pixelscherm

// ============================================================================
// SCHERMRESOLUTIES: enkel voor intern gebruik binnen de library
// ============================================================================

// Characterschermen: maximaal één keuze
#define SCREEN_LCD1602  0x0010 // 2 regels van 16 karakters
#define SCREEN_LCD1604  0x0020 // 4 regels van 16 karakters
#define SCREEN_LCD2002  0x0030 // 2 regels van 20 karakters
#define SCREEN_LCD2004  0x0040 // 4 regels van 20 karakters
#define SCREEN_LCD4002  0x0050 // 2 regels van 40 karakters

// Pixelschermen: maximaal één keuze
#define SCREEN_128X32   0x0100 // 128x32 pixels
#define SCREEN_128X64   0x0200 // 128x64 pixels
#define SCREEN_128X160  0x0300 // 128x160 pixels
#define SCREEN_240X240  0x0400 // 240x240 pixels
#define SCREEN_240X320  0x0500 // 240x320 pixels
#define SCREEN_320X480  0x0600 // 320x480 pixels
#define SCREEN_480X320  0x0700 // 480x320 pixels

#include "../../Configuratie/SystemConfig.h"

#if (ACTIEF_CHARACTER_SCREEN == SCREEN_LCD1604 || ACTIEF_CHARACTER_SCREEN == SCREEN_LCD2004)
  #define ACTIEF_CHARACTER_SCREEN_MET_VIER_REGELS true
#else
  #define ACTIEF_CHARACTER_SCREEN_MET_VIER_REGELS false
#endif

// ============================================================================
// SCREEN CALLBACK
//
// Een geregistreerde callback neemt de volledige uitvoer voor het betreffende
// schermtype over. De callback wordt exact één keer per PrintToScreen()-aanroep
// uitgevoerd en ontvangt de volledige schermopdracht.
//
// De callback is zelf verantwoordelijk voor:
// 1. initialisatie van het scherm wanneer die niet elders gebeurt;
// 2. wissen of behouden van de bestaande scherminhoud;
// 3. tonen van eersteRegel en tweedeRegel;
// 4. tonen van derdeRegel en vierdeRegel;
// 5. paginering wanneer niet alle regels gelijktijdig passen;
// 6. uitvoeren van delayTussenPaginas tussen beide pagina's;
// 7. uitvoeren van delayTime nadat alle tekst werd getoond;
// 8. daarna tonen of verwerken van action;
// 9. cursorplaatsing, regelafbreking, afkapping en scrolling.
//
// PrintToScreen() voert voor dat schermtype geen aanvullende schermlogica,
// wachttijd of action uit wanneer een callback geregistreerd is.
// ============================================================================
typedef void (*ScreenCallback)(const String& eersteRegel, const String& tweedeRegel, unsigned long delayTime, const String& action, const String& derdeRegel, const String& vierdeRegel, unsigned long delayTussenPaginas);

// CALLBACKS REGISTREREN
// nullptr bij CHARACTER gebruikt de standaardafhandeling via LiquidCrystal_I2C.
// nullptr bij PIXELS geeft geen pixeluitvoer.
void RegistreerCallbackScreenTypeCharacter(ScreenCallback callback);
void RegistreerCallbackScreenTypePixel(ScreenCallback callback);

// CENTRALE PRINTFUNCTIE
void PrintToScreen(const String& eersteRegel, const String& tweedeRegel, unsigned long delayTime = 0, const String& action = "", const String& derdeRegel = "", const String& vierdeRegel = "", unsigned long delayTussenPaginas = 0);

extern uint8_t SCREEN_OUTPUT;
extern ScreenCallback CallbackScreenTypeCharacter;
extern ScreenCallback CallbackScreenTypePixel;

#endif
