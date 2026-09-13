// ============================================================================
// Callback PixelScreen
// ============================================================================
// ============================================================================
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <Screen.h>
#include <Configuratie/ExamplesConfig.h>

// Dit example stelt SCREEN_OUTPUT_CONFIG NIET zelf in — dat kan een .ino
// structureel niet: Screen.cpp wordt als apart bestand gecompileerd en ziet
// een #define hier nooit. Zet SCREEN_TYPE_PIXELS daarom in UserConfig.h
// (kopieer van UserConfig_template.h) of rechtstreeks in SystemConfig.h.
// Onderstaande controle geeft een duidelijke foutmelding als dat nog niet
// gebeurd is, in plaats van de sketch stil te laten falen.
#if !((SCREEN_OUTPUT_CONFIG) & SCREEN_TYPE_PIXELS)
  #error Stel SCREEN_OUTPUT_CONFIG in UserConfig.h of SystemConfig.h in op (minstens) SCREEN_TYPE_PIXELS.
#else

Adafruit_ST7789 pixelScreen(PIXEL_SCREEN_CS, PIXEL_SCREEN_DC, PIXEL_SCREEN_RST);

void MijnPixelScreen(ScreenData screenData, const String& eersteRegel, const String& tweedeRegel, unsigned long delayTime, const String& action, const String& derdeRegel, const String& vierdeRegel, unsigned long delayTussenPaginas) {
  if (screenData == ScreenData::TYPE_NONE) return;

  uint16_t kleur;
  switch (screenData) {
    case ScreenData::TYPE_FATAL:    kleur = PIXEL_SCREEN_KLEUR_FATAL;    break;
    case ScreenData::TYPE_FAULT:    kleur = PIXEL_SCREEN_KLEUR_FAULT;    break; 
    case ScreenData::TYPE_WARNING:  kleur = PIXEL_SCREEN_KLEUR_WARNING;  break;
    case ScreenData::TYPE_INFO:     kleur = PIXEL_SCREEN_KLEUR_INFO;     break;
    case ScreenData::TYPE_CRITICAL: kleur = PIXEL_SCREEN_KLEUR_CRITICAL; break;
    case ScreenData::TYPE_ABORT:    kleur = PIXEL_SCREEN_KLEUR_ABORT;    break;
    case ScreenData::TYPE_PANIC:    kleur = PIXEL_SCREEN_KLEUR_PANIC;    break;
    default:                        kleur = PIXEL_SCREEN_TEXT_COLOR;     break;
  }

  if (eersteRegel != "" || tweedeRegel != "") {
    String eersteRegelLC = eersteRegel; eersteRegelLC.toLowerCase();
    String tweedeRegelLC = tweedeRegel; tweedeRegelLC.toLowerCase();

    pixelScreen.fillScreen(PIXEL_SCREEN_BACKGROUND_COLOR);
    pixelScreen.setTextSize(PIXEL_SCREEN_TEXT_SIZE);
    pixelScreen.setTextColor(kleur, PIXEL_SCREEN_BACKGROUND_COLOR);
    pixelScreen.setCursor(0, 0); pixelScreen.print(eersteRegelLC);
    pixelScreen.setCursor(0, 8 * PIXEL_SCREEN_TEXT_SIZE); pixelScreen.print(tweedeRegelLC);
  }

  if (derdeRegel != "" || vierdeRegel != "") {
    String derdeRegelLC = derdeRegel; derdeRegelLC.toLowerCase();
    String vierdeRegelLC = vierdeRegel; vierdeRegelLC.toLowerCase();

    if (delayTussenPaginas) delay(delayTussenPaginas);
    pixelScreen.fillScreen(PIXEL_SCREEN_BACKGROUND_COLOR);
    pixelScreen.setCursor(0, 0); pixelScreen.print(derdeRegelLC);
    pixelScreen.setCursor(0, 8 * PIXEL_SCREEN_TEXT_SIZE); pixelScreen.print(vierdeRegelLC);
  }

  if (delayTime) delay(delayTime);

  if (action != "") {
    String actionLC = action; actionLC.toLowerCase();
    pixelScreen.print(actionLC);
  }
}

void setup() {
  pixelScreen.init(ACTIEF_PIXEL_SCREEN_BREEDTE, ACTIEF_PIXEL_SCREEN_HOOGTE);
  pixelScreen.setRotation(PIXEL_SCREEN_ROTATION);
  PixelScreen = &pixelScreen;
  PixelScreenConfigureren();
  RegistreerCallbackScreenTypePixel(MijnPixelScreen);
}

void loop() {
  while (true) PrintToScreen(ScreenData::TYPE_INFO, "EERSTE REGEL", "TWEEDE REGEL", LEESTIJD_VOORBEELD_MS, " NU", "DERDE REGEL", "VIERDE REGEL", WACHTTIJD_TUSSEN_PAGINAS_MS);
}
#endif
