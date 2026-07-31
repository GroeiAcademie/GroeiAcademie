// ============================================================================
// Default CharacterScreen
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
#endif
