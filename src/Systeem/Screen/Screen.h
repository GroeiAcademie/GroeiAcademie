#ifndef SCREEN_H
#define SCREEN_H

#include <Arduino.h>

#include "ScreenTypes.h"

#include "../../Configuratie/SystemConfig.h"

#ifndef SCREEN_OUTPUT_CONFIG
  #error SCREEN_OUTPUT_CONFIG moet in SystemConfig.h worden gedefinieerd.
#endif

#ifndef SCREEN_OUTPUT
  #ifdef DEBUG
    #define SCREEN_OUTPUT (SCREEN_OUTPUT_CONFIG | SCREEN_TYPE_SERIAL)
  #else
    #define SCREEN_OUTPUT SCREEN_OUTPUT_CONFIG
  #endif
#endif

#if ((SCREEN_OUTPUT) & ~(SCREEN_TYPE_SERIAL | SCREEN_TYPE_CHARACTER | SCREEN_TYPE_PIXELS))
  #error SCREEN_OUTPUT bevat een onbekende uitvoerbit. Gebruik alleen SCREEN_TYPE_NONE, SCREEN_TYPE_SERIAL, SCREEN_TYPE_CHARACTER en SCREEN_TYPE_PIXELS.
#endif

#if (SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER)
#include <LiquidCrystal_I2C.h>
extern LiquidCrystal_I2C lcd;

#if (ACTIEF_CHARACTER_SCREEN == SCREEN_LCD1604 || ACTIEF_CHARACTER_SCREEN == SCREEN_LCD2004)
  #define ACTIEF_CHARACTER_SCREEN_MET_VIER_REGELS true
#else
  #define ACTIEF_CHARACTER_SCREEN_MET_VIER_REGELS false
#endif
#endif // SCREEN_TYPE_CHARACTER

#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
#include <Adafruit_GFX.h>
extern Adafruit_GFX* PixelScreen;

#if (ACTIEF_PIXEL_SCREEN == SCREEN_128X32)
  #define ACTIEF_PIXEL_SCREEN_BREEDTE 128
  #define ACTIEF_PIXEL_SCREEN_HOOGTE 32
#elif (ACTIEF_PIXEL_SCREEN == SCREEN_128X64)
  #define ACTIEF_PIXEL_SCREEN_BREEDTE 128
  #define ACTIEF_PIXEL_SCREEN_HOOGTE 64
#elif (ACTIEF_PIXEL_SCREEN == SCREEN_128X160)
  #define ACTIEF_PIXEL_SCREEN_BREEDTE 128
  #define ACTIEF_PIXEL_SCREEN_HOOGTE 160
#elif (ACTIEF_PIXEL_SCREEN == SCREEN_240X240)
  #define ACTIEF_PIXEL_SCREEN_BREEDTE 240
  #define ACTIEF_PIXEL_SCREEN_HOOGTE 240
#elif (ACTIEF_PIXEL_SCREEN == SCREEN_240X320)
  #define ACTIEF_PIXEL_SCREEN_BREEDTE 240
  #define ACTIEF_PIXEL_SCREEN_HOOGTE 320
#elif (ACTIEF_PIXEL_SCREEN == SCREEN_320X480)
  #define ACTIEF_PIXEL_SCREEN_BREEDTE 320
  #define ACTIEF_PIXEL_SCREEN_HOOGTE 480
#elif (ACTIEF_PIXEL_SCREEN == SCREEN_480X320)
  #define ACTIEF_PIXEL_SCREEN_BREEDTE 480
  #define ACTIEF_PIXEL_SCREEN_HOOGTE 320
#else
  #error ACTIEF_PIXEL_SCREEN bevat geen ondersteunde pixelschermresolutie.
#endif

#if (PIXEL_SCREEN_ROTATION > 3)
  #error PIXEL_SCREEN_ROTATION moet 0, 1, 2 of 3 zijn.
#endif

extern bool ACTIEF_PIXEL_SCREEN_MET_VIER_REGELS;

enum class ScreenData : uint8_t {
    // Geen expliciet informatietype opgegeven
    TYPE_NONE,

    // Informatief
    TYPE_INFO,
    TYPE_MESSAGE,
    TYPE_NOTIFY,
    TYPE_SUCCESS,

    // Interactie
    TYPE_PROMPT,
    TYPE_CONFIRM,

    // Waarschuwingen en fouten
    TYPE_WARNING,
    TYPE_ALERT,
    TYPE_ERROR,
    TYPE_CRITICAL,
    TYPE_FATAL,
    TYPE_ABORT,
    TYPE_PANIC,

    // Diagnose
    TYPE_DEBUG,
    TYPE_TRACE,

    // Inhoud
    TYPE_TEXT,
    TYPE_GRAPHICS,
    TYPE_VIDEO
};
#endif // SCREEN_TYPE_PIXELS

#if (SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER)
typedef void (*CharacterScreenCallback)(const String& eersteRegel, const String& tweedeRegel, unsigned long delayTime, const String& action, const String& derdeRegel, const String& vierdeRegel, unsigned long delayTussenPaginas);
void RegistreerCallbackScreenTypeCharacter(CharacterScreenCallback callback);
extern CharacterScreenCallback CallbackScreenTypeCharacter;

// D022/D023: CHARACTERSCREEN_I2C_ADRES_MODUS bepaalt hoe CharacterScreenConfigureren()
// omgaat met het I2C-adres van het characterscherm (default in SystemConfig.h).
//   0 = geen scan, enkel de handdruk-check op het geconfigureerde I2C_ADRES (kleinste footprint).
//   1 = scan + rapporteren via de foutmelding, geen zelfherstel (standaard).
//   2 = scan + automatisch herbouwen op het gevonden adres (placement-new), nooit hercompileren.
#if (CHARACTERSCREEN_I2C_ADRES_MODUS < 0) || (CHARACTERSCREEN_I2C_ADRES_MODUS > 2)
  #error CHARACTERSCREEN_I2C_ADRES_MODUS moet 0, 1 of 2 zijn.
#endif

// O2/D024-vervolg: opnieuwProberen=true dwingt een nieuwe configuratiepoging af,
// ook na een eerdere mislukking — standaard false, dus bestaande aanroepen
// (zonder argument) blijven exact hetzelfde werken. Niet automatisch herhalen
// (bv. in loop()): elke poging doet een echte I2C-transactie.
bool CharacterScreenConfigureren(bool opnieuwProberen = false);
#endif // SCREEN_TYPE_CHARACTER

#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
typedef void (*PixelScreenCallback)(ScreenData screenData, const String& eersteRegel, const String& tweedeRegel, unsigned long delayTime, const String& action, const String& derdeRegel, const String& vierdeRegel, unsigned long delayTussenPaginas);
void RegistreerCallbackScreenTypePixel(PixelScreenCallback callback);
extern PixelScreenCallback CallbackScreenTypePixel;

bool PixelScreenConfigureren(bool opnieuwProberen = false);

// ============================================================================
// D020: PixelScreenClear(), PixelScreenSetCursor(), PixelScreenPrint() en
// PixelScreenFoutmeldingWeergeven() dienden uitsluitend als interne bouwsteen
// en zijn niet langer publiek. Hun prototypes staan nu als static forward
// declarations bovenaan Screen.cpp.
// ============================================================================
#endif // SCREEN_TYPE_PIXELS

#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
void PrintToScreen(ScreenData screenData, const String& eersteRegel, const String& tweedeRegel, unsigned long delayTime = 0, const String& action = "", const String& derdeRegel = "", const String& vierdeRegel = "", unsigned long delayTussenPaginas = 0);
#endif // SCREEN_TYPE_PIXELS

void PrintToScreen(const String& eersteRegel, const String& tweedeRegel, unsigned long delayTime = 0, const String& action = "", const String& derdeRegel = "", const String& vierdeRegel = "", unsigned long delayTussenPaginas = 0);

// D022: optionele gemakslaag, geen deprecatie van CharacterScreenConfigureren()/
// PixelScreenConfigureren() — die blijven de granulaire, expliciete route.
void ScreensConfigureren(bool opnieuwProberen = false);

#endif // SCREEN_H
