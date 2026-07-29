#include <Wire.h>
#include <Screen.h>

#if !(SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER)
  #error Stel SCREEN_OUTPUT in SystemConfig.h in met SCREEN_TYPE_CHARACTER.
#endif

#define LEESTIJD_VOORBEELD_MS       2000UL
#define WACHTTIJD_TUSSEN_PAGINAS_MS 1500UL

void setup() {
#if defined(DEBUG) && (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
  Serial.begin(115200);
#endif

  lcd.init();
  lcd.backlight();
}

void loop() {
  while (true) {
    PrintToScreen("GROEI ACADEMIE", "TYPE_NONE", LEESTIJD_VOORBEELD_MS);
    PrintToScreen("EERSTE REGEL", "TWEEDE REGEL", LEESTIJD_VOORBEELD_MS, " NU", "DERDE REGEL", "VIERDE REGEL", WACHTTIJD_TUSSEN_PAGINAS_MS);
  }
}
