#include <Wire.h>
#include <Screen.h>

#if !(SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER)
  #error Stel SCREEN_OUTPUT in SystemConfig.h in met SCREEN_TYPE_CHARACTER.
#endif
#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
  #error Dit voorbeeld verwacht dat SCREEN_TYPE_PIXELS niet gezet is. Gebruik Callback_PixelScreen.ino voor een pixelscherm, of zet SCREEN_TYPE_PIXELS terug uit SCREEN_OUTPUT om dit voorbeeld te testen.
#endif

#define LEESTIJD_VOORBEELD_MS       2000UL
#define WACHTTIJD_TUSSEN_PAGINAS_MS 1500UL

// Deze callback neemt de volledige characterschermuitvoer over.
// PrintToScreen() roept haar exact één keer aan en voert voor dit schermtype zelf geen aanvullende schermlogica, wachttijd of action meer uit.
void MijnCharacterScreen(const String& eersteRegel, const String& tweedeRegel, unsigned long delayTime, const String& action, const String& derdeRegel, const String& vierdeRegel, unsigned long delayTussenPaginas) {
  if (eersteRegel != "" || tweedeRegel != "") {
    String eersteRegelLC = eersteRegel; eersteRegelLC.toLowerCase();
    String tweedeRegelLC = tweedeRegel; tweedeRegelLC.toLowerCase();

    lcd.clear();
    lcd.setCursor(0, 0); lcd.print(eersteRegelLC);
    lcd.setCursor(0, 1); lcd.print(tweedeRegelLC);
  }

  if (derdeRegel != "" || vierdeRegel != "") {
    String derdeRegelLC = derdeRegel; derdeRegelLC.toLowerCase();
    String vierdeRegelLC = vierdeRegel; vierdeRegelLC.toLowerCase();

    if (!ACTIEF_CHARACTER_SCREEN_MET_VIER_REGELS) {
      if (delayTussenPaginas) delay(delayTussenPaginas);
      lcd.clear();
    }

    lcd.setCursor(0, ACTIEF_CHARACTER_SCREEN_MET_VIER_REGELS ? 2 : 0); lcd.print(derdeRegelLC);
    lcd.setCursor(0, ACTIEF_CHARACTER_SCREEN_MET_VIER_REGELS ? 3 : 1); lcd.print(vierdeRegelLC);
  }

  if (delayTime) delay(delayTime);

  if (action != "") {
    String actionLC = action; actionLC.toLowerCase();
    lcd.print(actionLC);
  }
}

void setup() {
  lcd.init();
  lcd.backlight();
  RegistreerCallbackScreenTypeCharacter(MijnCharacterScreen);
}

void loop() {
  while (true) {
    PrintToScreen("GROEI ACADEMIE", "CHARACTER SCREEN", LEESTIJD_VOORBEELD_MS);
    PrintToScreen("EERSTE REGEL", "TWEEDE REGEL", LEESTIJD_VOORBEELD_MS, " NU", "DERDE REGEL", "VIERDE REGEL", WACHTTIJD_TUSSEN_PAGINAS_MS);
  }
}
