// ============================================================================
// Default CharacterScreen en PixelScreen
// ============================================================================
// ============================================================================
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <Screen.h>
#include <Configuratie/ExamplesConfig.h>

// Dit example stelt SCREEN_OUTPUT_CONFIG NIET zelf in — dat kan een .ino
// structureel niet: Screen.cpp wordt als apart bestand gecompileerd en ziet
// een #define hier nooit. Zet SCREEN_TYPE_CHARACTER én SCREEN_TYPE_PIXELS
// daarom in UserConfig.h (kopieer van UserConfig_template.h) of rechtstreeks
// in SystemConfig.h. Onderstaande controle geeft een duidelijke foutmelding
// als dat nog niet gebeurd is, in plaats van de sketch stil te laten falen.
#if ((SCREEN_OUTPUT_CONFIG & (SCREEN_TYPE_CHARACTER | SCREEN_TYPE_PIXELS)) != (SCREEN_TYPE_CHARACTER | SCREEN_TYPE_PIXELS))
  #error Stel SCREEN_OUTPUT_CONFIG in UserConfig.h in met SCREEN_TYPE_CHARACTER en SCREEN_TYPE_PIXELS.
#else

Adafruit_ST7789 pixelScreen(PIXEL_SCREEN_CS, PIXEL_SCREEN_DC, PIXEL_SCREEN_RST);

void setup() {
#if defined(DEBUG) && (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
  Serial.begin(115200);
#endif

  lcd.init();
  lcd.backlight();

  pixelScreen.init(ACTIEF_PIXEL_SCREEN_BREEDTE, ACTIEF_PIXEL_SCREEN_HOOGTE);
  pixelScreen.setRotation(PIXEL_SCREEN_ROTATION);
  PixelScreen = &pixelScreen;
  PixelScreenConfigureren();
}

void loop() {
  while (true) {
    PrintToScreen("GROEI ACADEMIE", "TYPE_NONE", LEESTIJD_VOORBEELD_MS);
    PrintToScreen(ScreenData::TYPE_NONE, "EERSTE REGEL", "TWEEDE REGEL", LEESTIJD_VOORBEELD_MS, " NU", "DERDE REGEL", "VIERDE REGEL", WACHTTIJD_TUSSEN_PAGINAS_MS);
  }
}
#endif
