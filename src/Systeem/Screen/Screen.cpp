#include "Screen.h"
#if defined(LANGUAGE_NL)
  #if defined(__has_include)
    #if __has_include("../../Language/UserLibrary_NL.h")
      #include "../../Language/UserLibrary_NL.h"
    #endif
  #endif
  #include "../../Language/Library_NL.h"
#elif defined(LANGUAGE_DE)
  #if defined(__has_include)
    #if __has_include("../../Language/UserLibrary_DE.h")
      #include "../../Language/UserLibrary_DE.h"
    #endif
  #endif
  #include "../../Language/Library_DE.h"
#elif defined(LANGUAGE_EN)
  #if defined(__has_include)
    #if __has_include("../../Language/UserLibrary_EN.h")
      #include "../../Language/UserLibrary_EN.h"
    #endif
  #endif
  #include "../../Language/Library_EN.h"
#elif defined(LANGUAGE_FR)
  #if defined(__has_include)
    #if __has_include("../../Language/UserLibrary_FR.h")
      #include "../../Language/UserLibrary_FR.h"
    #endif
  #endif
  #include "../../Language/Library_FR.h"
#endif

#if (SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER)
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(I2C_ADRES, (ACTIEF_CHARACTER_SCREEN == SCREEN_LCD2002 || ACTIEF_CHARACTER_SCREEN == SCREEN_LCD2004) ? 20 : (ACTIEF_CHARACTER_SCREEN == SCREEN_LCD4002 ? 40 : 16), (ACTIEF_CHARACTER_SCREEN == SCREEN_LCD1604 || ACTIEF_CHARACTER_SCREEN == SCREEN_LCD2004) ? 4 : 2);
CharacterScreenCallback CallbackScreenTypeCharacter = nullptr;

void RegistreerCallbackScreenTypeCharacter(CharacterScreenCallback callback) { CallbackScreenTypeCharacter = callback; }
#endif

#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
Adafruit_GFX* PixelScreen = nullptr;
PixelScreenCallback CallbackScreenTypePixel = nullptr;
bool ACTIEF_PIXEL_SCREEN_MET_VIER_REGELS = false;

struct PixelScreenStatus {
  bool gecontroleerd;
  bool pixelScreenActief;
  uint8_t aantalKolommen;
  uint8_t aantalRegels;
  int16_t offsetX;
  int16_t offsetY;
  uint8_t cursorKolom;
  uint8_t cursorRegel;
};

static PixelScreenStatus pixelScreenStatus = { false, true, 0, 0, 0, 0, 0, 0 };

#define PIXELGRID_MIN_KOLOMMEN 16
#define PIXELGRID_MAX_KOLOMMEN 40

#define PIXELGRID_MIN_RIJEN     2
#define PIXELGRID_MAX_RIJEN     4

#define PIXEL_SCREEN_CHARACTER_WIDTH  6
#define PIXEL_SCREEN_CHARACTER_HEIGHT 8

static int16_t PixelScreenKarakterBreedte() { return PIXEL_SCREEN_CHARACTER_WIDTH * PIXEL_SCREEN_TEXT_SIZE; }
static int16_t PixelScreenKarakterStap() { return PixelScreenKarakterBreedte() + PIXEL_SCREEN_CHARACTER_SPACING; }
static int16_t PixelScreenRegelHoogte() { return PIXEL_SCREEN_CHARACTER_HEIGHT * PIXEL_SCREEN_TEXT_SIZE; }
static int16_t PixelScreenRegelStap() { return PixelScreenRegelHoogte() + PIXEL_SCREEN_LINE_SPACING; }

static uint8_t PixelGridClamp(int32_t waarde, uint8_t minimum, uint8_t maximum) {
  if (waarde < minimum) return minimum;
  if (waarde > maximum) return maximum;
  return (uint8_t)waarde;
}

void RegistreerCallbackScreenTypePixel(PixelScreenCallback callback) { CallbackScreenTypePixel = callback; }

// ============================================================================
// Interne hulpfuncties (D020): dienen uitsluitend als bouwsteen binnen dit
// bestand, worden door geen enkel voorbeeld of ander bronbestand aangeroepen,
// en zijn daarom niet langer publiek gedeclareerd in Screen.h.
// ============================================================================
static void PixelScreenClear();
static void PixelScreenSetCursor(uint8_t kolom, uint8_t regel);
static void PixelScreenPrint(const String& tekst);
static void PixelScreenFoutmeldingWeergeven(const String& foutmelding);

bool PixelScreenConfigureren() {
  if (!PixelScreen) {
    pixelScreenStatus.gecontroleerd = true;
    pixelScreenStatus.pixelScreenActief = false;
    PixelScreenFoutmeldingWeergeven (_FATAL_PS001);
    return false;
  }

  if (pixelScreenStatus.gecontroleerd) return pixelScreenStatus.pixelScreenActief;

  pixelScreenStatus.gecontroleerd = true;
#if (PIXEL_SCREEN_ROTATION == 1 || PIXEL_SCREEN_ROTATION == 3)
  if (PixelScreen->width() != ACTIEF_PIXEL_SCREEN_HOOGTE || PixelScreen->height() != ACTIEF_PIXEL_SCREEN_BREEDTE) { pixelScreenStatus.pixelScreenActief = false; PixelScreenFoutmeldingWeergeven (_FATAL_PS002); return false; }
#else
  if (PixelScreen->width() != ACTIEF_PIXEL_SCREEN_BREEDTE || PixelScreen->height() != ACTIEF_PIXEL_SCREEN_HOOGTE) { pixelScreenStatus.pixelScreenActief = false; PixelScreenFoutmeldingWeergeven (_FATAL_PS003); return false; }
#endif

  int16_t bruikbareBreedte = PixelScreen->width() - (2 * PIXEL_SCREEN_MARGIN);
  int16_t bruikbareHoogte = PixelScreen->height() - (2 * PIXEL_SCREEN_MARGIN);
  
  int32_t ruweKolommen = (bruikbareBreedte + PIXEL_SCREEN_CHARACTER_SPACING) / PixelScreenKarakterStap();
  int32_t ruweRegels = (bruikbareHoogte + PIXEL_SCREEN_LINE_SPACING) / PixelScreenRegelStap();

  // Minimumvereiste (16x2, dezelfde ondergrens als de kleinste ondersteunde characterscherm-resolutie) niet gehaald: geen pixeluitvoer voor TYPE_NONE.
  if (ruweKolommen < PIXELGRID_MIN_KOLOMMEN || ruweRegels < PIXELGRID_MIN_RIJEN) {
    pixelScreenStatus.pixelScreenActief = false;
    PixelScreenFoutmeldingWeergeven (_FATAL_PS004);
    return false;
  }

  pixelScreenStatus.aantalKolommen = PixelGridClamp(ruweKolommen, PIXELGRID_MIN_KOLOMMEN, PIXELGRID_MAX_KOLOMMEN);
  pixelScreenStatus.aantalRegels   = PixelGridClamp(ruweRegels,   PIXELGRID_MIN_RIJEN,     PIXELGRID_MAX_RIJEN);

  // Centreren: restruimte gelijk verdelen links/rechts en boven/onder.
  int16_t gridBreedtePx = pixelScreenStatus.aantalKolommen * PixelScreenKarakterBreedte() + (pixelScreenStatus.aantalKolommen - 1) * PIXEL_SCREEN_CHARACTER_SPACING;
  int16_t gridHoogtePx = pixelScreenStatus.aantalRegels * PixelScreenRegelHoogte() + (pixelScreenStatus.aantalRegels - 1) * PIXEL_SCREEN_LINE_SPACING;
  
  pixelScreenStatus.offsetX = PIXEL_SCREEN_MARGIN + (bruikbareBreedte - gridBreedtePx) / 2;
  pixelScreenStatus.offsetY = PIXEL_SCREEN_MARGIN + (bruikbareHoogte - gridHoogtePx) / 2;
  
  if (pixelScreenStatus.offsetX < PIXEL_SCREEN_MARGIN) pixelScreenStatus.offsetX = PIXEL_SCREEN_MARGIN;
  if (pixelScreenStatus.offsetY < PIXEL_SCREEN_MARGIN) pixelScreenStatus.offsetY = PIXEL_SCREEN_MARGIN;

  ACTIEF_PIXEL_SCREEN_MET_VIER_REGELS = pixelScreenStatus.aantalRegels >= 4;
  PixelScreen->setTextSize(PIXEL_SCREEN_TEXT_SIZE);
  PixelScreen->setTextColor(PIXEL_SCREEN_TEXT_COLOR, PIXEL_SCREEN_BACKGROUND_COLOR);
  PixelScreen->setTextWrap(false);
  return true;
}

static void PixelScreenClear() {
  if (!PixelScreenConfigureren()) return;
  PixelScreen->fillScreen(PIXEL_SCREEN_BACKGROUND_COLOR);
  pixelScreenStatus.cursorKolom = 0;
  pixelScreenStatus.cursorRegel = 0;
}

static void PixelScreenSetCursor(uint8_t kolom, uint8_t regel) {
  if (!PixelScreenConfigureren()) return;
  pixelScreenStatus.cursorKolom = kolom;
  pixelScreenStatus.cursorRegel = regel;
  PixelScreen->setCursor(pixelScreenStatus.offsetX + kolom * PixelScreenKarakterStap(), pixelScreenStatus.offsetY + regel * PixelScreenRegelStap());
}

static void PixelScreenPrint(const String& tekst) {
  if (!PixelScreenConfigureren()) return;
  
  for (uint16_t index = 0; index < tekst.length(); index++) {
    PixelScreen->setCursor(pixelScreenStatus.offsetX + (pixelScreenStatus.cursorKolom + index) * PixelScreenKarakterStap(), pixelScreenStatus.offsetY + pixelScreenStatus.cursorRegel * PixelScreenRegelStap());
    PixelScreen->print(tekst[index]);
  }

  pixelScreenStatus.cursorKolom += tekst.length();
  
  while (pixelScreenStatus.cursorKolom >= pixelScreenStatus.aantalKolommen) {
    pixelScreenStatus.cursorKolom -= pixelScreenStatus.aantalKolommen;
    pixelScreenStatus.cursorRegel++;
  }
}

static void PixelScreenFoutmeldingWeergeven(const String& foutmelding) {
#if (SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER)
  if (CallbackScreenTypeCharacter) {
    CallbackScreenTypeCharacter(foutmelding, FATAL_ZOEK_OP, LCD_LEESTIJD_FOUTMELDING_MS, "", "", "", 0);
  } else {
    lcd.clear();
    lcd.setCursor(0, 0); lcd.print(foutmelding);
    lcd.setCursor(0, 1); lcd.print(FATAL_ZOEK_OP);
    delay(LCD_LEESTIJD_FOUTMELDING_MS);
  }
#endif

#if !(SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER)
  Serial.begin(115200);
  Serial.println(foutmelding);
  Serial.println(FATAL_ZOEK_OP);
#endif
}
#endif

static void PrintToScreenIntern(
#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
ScreenData screenData,
#endif
const String& eersteRegel, const String& tweedeRegel, unsigned long delayTime, const String& action, const String& derdeRegel, const String& vierdeRegel, unsigned long delayTussenPaginas) {
#if !(SCREEN_OUTPUT & (SCREEN_TYPE_CHARACTER | SCREEN_TYPE_PIXELS))
  (void)delayTussenPaginas;
#endif

#if (SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER)
  const bool characterScreenActief = !CallbackScreenTypeCharacter;
#endif

#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
  const bool pixelScreenGeselecteerd = !CallbackScreenTypePixel && screenData == ScreenData::TYPE_NONE;
  if (pixelScreenGeselecteerd && pixelScreenStatus.pixelScreenActief) PixelScreenConfigureren();
  const bool pixelScreenActief = pixelScreenGeselecteerd && pixelScreenStatus.pixelScreenActief;
#endif

  // --------------------------------------------------------------------------
  // STANDAARDUITVOER: REGEL 1 EN REGEL 2
  // --------------------------------------------------------------------------
  if (eersteRegel != "" || tweedeRegel != "") {
#if (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
#ifdef DEBUG
    DEBUG_PRINTLN(eersteRegel);
    DEBUG_PRINTLN(tweedeRegel);
#endif
#endif

#if (SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER)
    if (characterScreenActief) {
      lcd.clear();
      lcd.setCursor(0, 0); lcd.print(eersteRegel);
      lcd.setCursor(0, 1); lcd.print(tweedeRegel);
    }
#endif

#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
    if (pixelScreenActief) {
      PixelScreenClear();
      PixelScreenSetCursor(0, 0); PixelScreenPrint(eersteRegel);
      PixelScreenSetCursor(0, 1); PixelScreenPrint(tweedeRegel);
    }
#endif
  }

  // --------------------------------------------------------------------------
  // STANDAARDUITVOER: REGEL 3 EN REGEL 4
  // --------------------------------------------------------------------------
  if (derdeRegel != "" || vierdeRegel != "") {
#if (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
#ifdef DEBUG
    DEBUG_PRINTLN(derdeRegel);
    DEBUG_PRINTLN(vierdeRegel);
#endif
#endif

    bool tweedePaginaNodig = false;
#if (SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER)
    tweedePaginaNodig = tweedePaginaNodig || (characterScreenActief && !ACTIEF_CHARACTER_SCREEN_MET_VIER_REGELS);
#endif
#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
    tweedePaginaNodig = tweedePaginaNodig || (pixelScreenActief && !ACTIEF_PIXEL_SCREEN_MET_VIER_REGELS);
#endif
    if (tweedePaginaNodig && delayTussenPaginas) delay(delayTussenPaginas);

#if (SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER)
    if (characterScreenActief) {
      if (!ACTIEF_CHARACTER_SCREEN_MET_VIER_REGELS) lcd.clear();
      lcd.setCursor(0, ACTIEF_CHARACTER_SCREEN_MET_VIER_REGELS ? 2 : 0); lcd.print(derdeRegel);
      lcd.setCursor(0, ACTIEF_CHARACTER_SCREEN_MET_VIER_REGELS ? 3 : 1); lcd.print(vierdeRegel);
    }
#endif

#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
    if (pixelScreenActief) {
      if (!ACTIEF_PIXEL_SCREEN_MET_VIER_REGELS) PixelScreenClear();
      PixelScreenSetCursor(0, ACTIEF_PIXEL_SCREEN_MET_VIER_REGELS ? 2 : 0); PixelScreenPrint(derdeRegel);
      PixelScreenSetCursor(0, ACTIEF_PIXEL_SCREEN_MET_VIER_REGELS ? 3 : 1); PixelScreenPrint(vierdeRegel);
    }
#endif
  }

  // --------------------------------------------------------------------------
  // STANDAARDUITVOER: DELAY EN ACTION
  // --------------------------------------------------------------------------
  bool standaardScreenActief = false;

#if (SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER)
  standaardScreenActief = standaardScreenActief || characterScreenActief;
#endif
#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
  standaardScreenActief = standaardScreenActief || pixelScreenActief;
#endif

  if (standaardScreenActief) {
    if (delayTime) delay(delayTime);

    if (action != "") {
#if (SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER)
      if (characterScreenActief) lcd.print(action);
#endif

#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
      if (pixelScreenActief) PixelScreenPrint(action);
#endif
    }
  }

#if (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
#ifdef DEBUG
  if (action != "") DEBUG_PRINTLN(action);
#endif
#endif

  // --------------------------------------------------------------------------
  // CALLBACK CHARACTER: ÉÉN AANROEP MET DE VOLLEDIGE OPDRACHT
  // --------------------------------------------------------------------------
#if (SCREEN_OUTPUT & (SCREEN_TYPE_CHARACTER | SCREEN_TYPE_PIXELS))
  const unsigned long callbackDelayTime = standaardScreenActief ? 0 : delayTime;
  const unsigned long callbackDelayTussenPaginas = standaardScreenActief ? 0 : delayTussenPaginas;
#endif

#if (SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER)
  if (CallbackScreenTypeCharacter) CallbackScreenTypeCharacter(eersteRegel, tweedeRegel, callbackDelayTime, action, derdeRegel, vierdeRegel, callbackDelayTussenPaginas);
#endif

  // --------------------------------------------------------------------------
  // CALLBACK PIXELS: ÉÉN AANROEP MET DE VOLLEDIGE OPDRACHT
  // --------------------------------------------------------------------------
#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
  if (CallbackScreenTypePixel) CallbackScreenTypePixel(screenData, eersteRegel, tweedeRegel, callbackDelayTime, action, derdeRegel, vierdeRegel, callbackDelayTussenPaginas);
#endif
}

#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
void PrintToScreen(ScreenData screenData, const String& eersteRegel, const String& tweedeRegel, unsigned long delayTime, const String& action, const String& derdeRegel, const String& vierdeRegel, unsigned long delayTussenPaginas) { PrintToScreenIntern(screenData, eersteRegel, tweedeRegel, delayTime, action, derdeRegel, vierdeRegel, delayTussenPaginas); }
void PrintToScreen(const String& eersteRegel, const String& tweedeRegel, unsigned long delayTime, const String& action, const String& derdeRegel, const String& vierdeRegel, unsigned long delayTussenPaginas) { PrintToScreenIntern(ScreenData::TYPE_NONE, eersteRegel, tweedeRegel, delayTime, action, derdeRegel, vierdeRegel, delayTussenPaginas); }
#else
void PrintToScreen(const String& eersteRegel, const String& tweedeRegel, unsigned long delayTime, const String& action, const String& derdeRegel, const String& vierdeRegel, unsigned long delayTussenPaginas) { PrintToScreenIntern(eersteRegel, tweedeRegel, delayTime, action, derdeRegel, vierdeRegel, delayTussenPaginas); }
#endif
