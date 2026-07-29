#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <Screen.h>

#if ((SCREEN_OUTPUT & (SCREEN_TYPE_CHARACTER | SCREEN_TYPE_PIXELS)) != (SCREEN_TYPE_CHARACTER | SCREEN_TYPE_PIXELS))
  #error Stel SCREEN_OUTPUT in SystemConfig.h in met SCREEN_TYPE_CHARACTER en SCREEN_TYPE_PIXELS.
#endif

#define LEESTIJD_VOORBEELD_MS       2000UL
#define WACHTTIJD_TUSSEN_PAGINAS_MS 1500UL

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
