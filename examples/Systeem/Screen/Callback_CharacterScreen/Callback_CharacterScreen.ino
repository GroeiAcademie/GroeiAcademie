// ============================================================================
// Callback CharacterScreen
// ============================================================================
// ============================================================================
#include <Wire.h>
#include <Screen.h>
#include <Configuratie/ExamplesConfig.h>

// Dit example stelt SCREEN_OUTPUT_CONFIG NIET zelf in — dat kan een .ino
// structureel niet: Screen.cpp wordt als apart bestand gecompileerd en ziet
// een #define hier nooit. Zet SCREEN_TYPE_CHARACTER daarom in UserConfig.h
// (kopieer van UserConfig_template.h) of rechtstreeks in SystemConfig.h.
// Onderstaande controle geeft een duidelijke foutmelding als dat nog niet
// gebeurd is, in plaats van de sketch stil te laten falen.
#if !(SCREEN_OUTPUT_CONFIG & SCREEN_TYPE_CHARACTER)
  #error Stel SCREEN_OUTPUT_CONFIG in UserConfig.h of SystemConfig.h in op (minstens) SCREEN_TYPE_CHARACTER.
#else

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
  CharacterScreenConfigureren();
  RegistreerCallbackScreenTypeCharacter(MijnCharacterScreen);
}

void loop() {
  while (true) {
    PrintToScreen("GROEI ACADEMIE", "CHARACTER SCREEN", LEESTIJD_VOORBEELD_MS);
    PrintToScreen("EERSTE REGEL", "TWEEDE REGEL", LEESTIJD_VOORBEELD_MS, " NU", "DERDE REGEL", "VIERDE REGEL", WACHTTIJD_TUSSEN_PAGINAS_MS);
  }
}
#endif
