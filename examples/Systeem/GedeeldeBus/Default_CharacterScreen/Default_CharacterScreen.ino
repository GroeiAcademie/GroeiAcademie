// ============================================================================
// Default CharacterScreen
// ============================================================================
// ============================================================================
#include <Wire.h>
#include <Screen.h>
#include <Systeem/GedeeldeBus/GedeeldeBus.h>
#include <Configuratie/ExamplesConfig.h>

// Schakel GEDEELDE_BUS_PROTOTYPE om de nieuwe GedeeldeBus-functionaliteit te kunnen testen.
// Zonder deze regel wordt alleen de bestaande, stabiele GedeeldeBus-code gebruikt en krijgt deze .ino compileerfouten.
// Verwijder in 'SystemConfig.h' dus // voor '// #define GEDEELDE_BUS_PROTOTYPE' om deze prototypefuncties beschikbaar te maken.

// Dit example stelt SCREEN_OUTPUT_CONFIG NIET zelf in — dat kan een .ino structureel niet: Screen.cpp wordt als apart bestand gecompileerd en ziet een #define hier nooit. 
// Zet SCREEN_TYPE_CHARACTER én SCREEN_TYPE_PIXELS daarom in UserConfig.h (kopieer van UserConfig_template.h) of rechtstreeks in SystemConfig.h. 
// Onderstaande controle geeft een duidelijke foutmelding als dat nog niet gebeurd is, in plaats van de sketch stil te laten falen.
#if !((SCREEN_OUTPUT_CONFIG) & SCREEN_TYPE_CHARACTER)
  #error Stel SCREEN_OUTPUT_CONFIG in UserConfig.h of SystemConfig.h in op (minstens) SCREEN_TYPE_CHARACTER.
#else

void setup() {
  // FASE 1: aanmelden.
  RegistratiesResettenOpGedeeldeBus();
  I2CAanmeldenOpGedeeldeBus(GedeeldeBusComponent::CHARACTER_SCREEN, I2C_ADDRESS_CHARACTER_SCREEN, HardwareResourcePin::SDA, HardwareResourcePin::SCL, SetupOfLoop::SETUP);

  // FASE 2 en 3.
  bool magInpluggen = AlleAangemeldeResourcesInpluggenOpGedeeldeBus();

  if (!magInpluggen) {
    PrintToScreen("GedeeldeBus", "RESOURCECONFLICT", LEESTIJD_VOORBEELD_MS);
    return;
  }

  PrintToScreen("GedeeldeBus", "registratie OK", LEESTIJD_VOORBEELD_MS);
  CharacterScreenConfigureren();
}

void loop() {
  while (true) {
    PrintToScreen("GROEI ACADEMIE", "TYPE_NONE", LEESTIJD_VOORBEELD_MS);
    PrintToScreen("EERSTE REGEL", "TWEEDE REGEL", LEESTIJD_VOORBEELD_MS, " NU", "DERDE REGEL", "VIERDE REGEL", WACHTTIJD_TUSSEN_PAGINAS_MS);
  }
}
#endif
