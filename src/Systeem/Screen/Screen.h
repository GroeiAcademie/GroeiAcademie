#ifndef SCREEN_H
#define SCREEN_H

#include <Arduino.h>

#include "ScreenTypes.h"
#include "../../Configuratie/SystemConfig.h"
#include "../GedeeldeBus/GedeeldeBus.h"

#ifndef SCREEN_OUTPUT_CONFIG
  #error SCREEN_OUTPUT_CONFIG moet in SystemConfig.h worden gedefinieerd.
#endif

#ifndef SCREEN_OUTPUT
  #ifdef DEBUG
    #define SCREEN_OUTPUT (SCREEN_OUTPUT_CONFIG | SCREEN_TYPE_SERIAL)
  #else
    #define SCREEN_OUTPUT (SCREEN_OUTPUT_CONFIG)
  #endif
#endif

#if ((SCREEN_OUTPUT) & ~(SCREEN_TYPE_SERIAL | SCREEN_TYPE_CHARACTER | SCREEN_TYPE_PIXELS))
  #error SCREEN_OUTPUT_CONFIG bevat een onbekende uitvoerbit. Gebruik alleen SCREEN_TYPE_NONE, SCREEN_TYPE_SERIAL, SCREEN_TYPE_CHARACTER en SCREEN_TYPE_PIXELS.
#endif

#if (SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER)
#include <LiquidCrystal_I2C.h>

#if (ACTIEF_CHARACTER_SCREEN == SCREEN_LCD1604 || ACTIEF_CHARACTER_SCREEN == SCREEN_LCD2004)
  #define ACTIEF_CHARACTER_SCREEN_MET_VIER_REGELS true
#else
  #define ACTIEF_CHARACTER_SCREEN_MET_VIER_REGELS false
#endif

#endif // SCREEN_TYPE_CHARACTER

#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

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

enum class ScreenData : uint8_t {
    TYPE_NONE,
    TYPE_INFO,
    TYPE_MESSAGE,
    TYPE_NOTIFY,
    TYPE_SUCCESS,
    TYPE_PROMPT,
    TYPE_CONFIRM,
    TYPE_WARNING,
    TYPE_ALERT,
    TYPE_FAULT,
    TYPE_CRITICAL,
    TYPE_FATAL,
    TYPE_ABORT,
    TYPE_PANIC,
    TYPE_DEBUG,
    TYPE_TRACE,
    TYPE_TEXT,
    TYPE_GRAPHICS,
    TYPE_VIDEO
};
#endif // SCREEN_TYPE_PIXELS

#if (SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER)
typedef void (*CharacterScreenCallback)(const String& eersteRegel, const String& tweedeRegel, unsigned long delayTime, const String& action, const String& derdeRegel, const String& vierdeRegel, unsigned long delayTussenPaginas);
#endif

#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
typedef void (*PixelScreenCallback)(ScreenData screenData, const String& eersteRegel, const String& tweedeRegel, unsigned long delayTime, const String& action, const String& derdeRegel, const String& vierdeRegel, unsigned long delayTussenPaginas);
#endif

struct Screen : GedeeldeBusNode {
  Screen();
  Screen(uint8_t SCREEN_TYPES_ACTIEF);
#if (SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER)
  Screen(CharacterScreenCallback callback);
  Screen(uint8_t SCREEN_TYPES_ACTIEF, CharacterScreenCallback callback);
#endif
#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
  Screen(PixelScreenCallback callback);
  Screen(uint8_t SCREEN_TYPES_ACTIEF, PixelScreenCallback callback);
#endif
#if ((SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER) && (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS))
  Screen(CharacterScreenCallback characterCallback, PixelScreenCallback pixelCallback);
  Screen(uint8_t SCREEN_TYPES_ACTIEF, CharacterScreenCallback characterCallback, PixelScreenCallback pixelCallback);
#endif

  void ActivatiefoutWeergeven();

#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
  void Print(ScreenData screenData, const String& eersteRegel, const String& tweedeRegel, unsigned long delayTime = 0, const String& action = "", const String& derdeRegel = "", const String& vierdeRegel = "", unsigned long delayTussenPaginas = 0);
#endif
  void Print(const String& eersteRegel, const String& tweedeRegel, unsigned long delayTime = 0, const String& action = "", const String& derdeRegel = "", const String& vierdeRegel = "", unsigned long delayTussenPaginas = 0);

#if (SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER)
  struct Character {
    bool actief = false;

    CharacterScreenCallback callback = nullptr;
    ::CharacterScreen* gedeeldeBus   = nullptr;

    LiquidCrystal_I2C display{I2C_ADDRESS_CHARACTER_SCREEN, (ACTIEF_CHARACTER_SCREEN == SCREEN_LCD2002 || ACTIEF_CHARACTER_SCREEN == SCREEN_LCD2004) ? 20 : (ACTIEF_CHARACTER_SCREEN == SCREEN_LCD4002 ? 40 : 16), (ACTIEF_CHARACTER_SCREEN == SCREEN_LCD1604 || ACTIEF_CHARACTER_SCREEN == SCREEN_LCD2004) ? 4 : 2};

    const char* foutmelding     = nullptr;
    char foutmeldingBuffer[24]  = {};
    bool foutmeldingWeergegeven = false;

    void FoutmeldingWeergeven(const String& foutmelding);
    void RegistreerCallback(CharacterScreenCallback callback);
  } Character;
#endif

#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
  struct Pixel {
    bool actief = false;

    PixelScreenCallback callback = nullptr;
    ::PixelScreen* gedeeldeBus   = nullptr;

    uint8_t aantalKolommen = 0;
    uint8_t aantalRegels   = 0;
    uint8_t cursorKolom    = 0;
    uint8_t cursorRegel    = 0;
    int16_t offsetX        = 0;
    int16_t offsetY        = 0;

    Adafruit_ST7789 display{ static_cast<int8_t>(NativeArduinoPinVan(PIXEL_SCREEN_CS)), static_cast<int8_t>(NativeArduinoPinVan(PIXEL_SCREEN_DC)), static_cast<int8_t>(NativeArduinoPinVan(PIXEL_SCREEN_RST))};
    Adafruit_GFX* gfx = nullptr;

    bool emulateLCDxxx4         = false;
    const char* foutmelding     = nullptr;
    bool foutmeldingWeergegeven = false;

    void FoutmeldingWeergeven(const String& foutmelding);
    void RegistreerCallback(PixelScreenCallback callback);

    void    Clear();
    uint8_t GridClamp(int32_t waarde, uint8_t minimumWaarde, uint8_t maximumWaarde);
    int16_t KarakterBreedte();
    int16_t KarakterStap();
    void    Print(const String& tekst);
    int16_t RegelHoogte();
    int16_t RegelStap();
    void    SetCursor(uint8_t kolom, uint8_t regel);
  } Pixel;
#endif

#if (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
  struct Serial {
    bool actief = false;

    SerialOutput* gedeeldeBus = nullptr;

    const char* foutmelding     = nullptr;
    bool foutmeldingWeergegeven = false;

    void FoutmeldingWeergeven();
  } Serial;
#endif

private:
  uint8_t SCREEN_TYPES_ACTIEF = SCREEN_OUTPUT;

  // Screen stuurt elke stap intern door naar zijn uitvoeren (CharacterScreen, PixelScreen, SerialOutput).
  bool aanmelden() override;
  bool controleren() override;
  bool inpluggen() override;
  bool Activeren() override;
  bool afmelden() override;

  void PrintPrivate(
#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
    ScreenData screenData,
#endif
    const String& eersteRegel, const String& tweedeRegel, unsigned long delayTime, const String& action, const String& derdeRegel, const String& vierdeRegel, unsigned long delayTussenPaginas);
};

extern struct Screen* Screen;

#endif // SCREEN_H
