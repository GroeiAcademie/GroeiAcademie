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

#if (CHARACTERSCREEN_I2C_ADRES_MODUS == 2)
#include <new>
#endif

#define CHARACTERSCREEN_KOLOMMEN ((ACTIEF_CHARACTER_SCREEN == SCREEN_LCD2002 || ACTIEF_CHARACTER_SCREEN == SCREEN_LCD2004) ? 20 : (ACTIEF_CHARACTER_SCREEN == SCREEN_LCD4002 ? 40 : 16))
#define CHARACTERSCREEN_REGELS   ((ACTIEF_CHARACTER_SCREEN == SCREEN_LCD1604 || ACTIEF_CHARACTER_SCREEN == SCREEN_LCD2004) ? 4 : 2)

LiquidCrystal_I2C lcd(I2C_ADRES, CHARACTERSCREEN_KOLOMMEN, CHARACTERSCREEN_REGELS);
CharacterScreenCallback CallbackScreenTypeCharacter = nullptr;

void RegistreerCallbackScreenTypeCharacter(CharacterScreenCallback callback) { CallbackScreenTypeCharacter = callback; }

struct CharacterScreenStatus {
  bool gecontroleerd;
  bool characterScreenActief;
  const char* foutmelding;
  bool foutmeldingWeergegeven;
};
static CharacterScreenStatus characterScreenStatus = { false, false, nullptr, false };

// ============================================================================
// Interne hulpfunctie (D022): dient uitsluitend als bouwsteen binnen dit
// bestand, wordt door geen enkel voorbeeld of ander bronbestand aangeroepen.
// ============================================================================
static void CharacterScreenFoutmeldingWeergeven(const String& foutmelding);

bool CharacterScreenConfigureren(bool opnieuwProberen) {
  if (characterScreenStatus.gecontroleerd && !opnieuwProberen) return characterScreenStatus.characterScreenActief;
  characterScreenStatus.gecontroleerd = true;
  characterScreenStatus.foutmeldingWeergegeven = false;

  Wire.begin();

#if (CHARACTERSCREEN_I2C_ADRES_MODUS == 0)
  // Geen scan: enkel de handdruk-check op het geconfigureerde I2C_ADRES.
  Wire.beginTransmission(I2C_ADRES);
  if (Wire.endTransmission() != 0) {
    characterScreenStatus.characterScreenActief = false;
    characterScreenStatus.foutmelding = _FATAL_CS001;
    return false;
  }
#else
  // Modus 1 en 2: kandidatenlijst aftasten, I2C_ADRES eerst geprobeerd.
  const uint8_t characterScreenI2CKandidaten[] = { I2C_ADRES, 0x27, 0x3F };
  const uint8_t characterScreenI2CAantalKandidaten = sizeof(characterScreenI2CKandidaten) / sizeof(characterScreenI2CKandidaten[0]);
  uint8_t characterScreenI2CGevonden = 0;
  bool characterScreenI2CGevondenOk = false;

  for (uint8_t i = 0; i < characterScreenI2CAantalKandidaten && !characterScreenI2CGevondenOk; i++) {
    bool reedsGeprobeerd = false;
    for (uint8_t j = 0; j < i; j++) {
      if (characterScreenI2CKandidaten[j] == characterScreenI2CKandidaten[i]) { reedsGeprobeerd = true; break; }
    }
    if (reedsGeprobeerd) continue;

    Wire.beginTransmission(characterScreenI2CKandidaten[i]);
    if (Wire.endTransmission() == 0) {
      characterScreenI2CGevonden = characterScreenI2CKandidaten[i];
      characterScreenI2CGevondenOk = true;
    }
  }

  if (!characterScreenI2CGevondenOk) {
    characterScreenStatus.characterScreenActief = false;
    characterScreenStatus.foutmelding = _FATAL_CS001;
    return false;
  }

  if (characterScreenI2CGevonden != I2C_ADRES) {
#if (CHARACTERSCREEN_I2C_ADRES_MODUS == 1)
    // Enkel rapporteren, niet zelf herstellen: de gevonden waarde in de melding opnemen.
    static char characterScreenFoutmeldingBuffer[24];
    snprintf(characterScreenFoutmeldingBuffer, sizeof(characterScreenFoutmeldingBuffer), "%s 0x%02X", _FATAL_CS002, characterScreenI2CGevonden);
    characterScreenStatus.characterScreenActief = false;
    characterScreenStatus.foutmelding = characterScreenFoutmeldingBuffer;
    return false;
#elif (CHARACTERSCREEN_I2C_ADRES_MODUS == 2)
    // Zelfherstellend: lcd op exact dezelfde geheugenplek herbouwen met het gevonden adres.
    lcd.~LiquidCrystal_I2C();
    new (&lcd) LiquidCrystal_I2C(characterScreenI2CGevonden, CHARACTERSCREEN_KOLOMMEN, CHARACTERSCREEN_REGELS);
#endif
  }
#endif // CHARACTERSCREEN_I2C_ADRES_MODUS

  lcd.init();
  lcd.backlight();
  characterScreenStatus.characterScreenActief = true;
  return true;
}
#endif

#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
Adafruit_GFX* PixelScreen = nullptr;
PixelScreenCallback CallbackScreenTypePixel = nullptr;
bool ACTIEF_PIXEL_SCREEN_MET_VIER_REGELS = false;

struct PixelScreenStatus {
  bool gecontroleerd;
  bool pixelScreenActief;
  const char* foutmelding;
  bool foutmeldingWeergegeven;
  uint8_t aantalKolommen;
  uint8_t aantalRegels;
  int16_t offsetX;
  int16_t offsetY;
  uint8_t cursorKolom;
  uint8_t cursorRegel;
};

static PixelScreenStatus pixelScreenStatus = { false, false, nullptr, false, 0, 0, 0, 0, 0, 0 };

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

bool PixelScreenConfigureren(bool opnieuwProberen) {
  if (pixelScreenStatus.gecontroleerd && !opnieuwProberen) return pixelScreenStatus.pixelScreenActief;
  pixelScreenStatus.gecontroleerd = true;
  pixelScreenStatus.foutmeldingWeergegeven = false;

  if (!PixelScreen) {
    pixelScreenStatus.pixelScreenActief = false;
    pixelScreenStatus.foutmelding = _FATAL_PS001;
    return false;
  }

#if (PIXEL_SCREEN_ROTATION == 1 || PIXEL_SCREEN_ROTATION == 3)
  if (PixelScreen->width() != ACTIEF_PIXEL_SCREEN_HOOGTE || PixelScreen->height() != ACTIEF_PIXEL_SCREEN_BREEDTE) { pixelScreenStatus.pixelScreenActief = false; pixelScreenStatus.foutmelding = _FATAL_PS002; return false; }
#else
  if (PixelScreen->width() != ACTIEF_PIXEL_SCREEN_BREEDTE || PixelScreen->height() != ACTIEF_PIXEL_SCREEN_HOOGTE) { pixelScreenStatus.pixelScreenActief = false; pixelScreenStatus.foutmelding = _FATAL_PS003; return false; }
#endif

  int16_t bruikbareBreedte = PixelScreen->width() - (2 * PIXEL_SCREEN_MARGIN);
  int16_t bruikbareHoogte = PixelScreen->height() - (2 * PIXEL_SCREEN_MARGIN);
  
  int32_t ruweKolommen = (bruikbareBreedte + PIXEL_SCREEN_CHARACTER_SPACING) / PixelScreenKarakterStap();
  int32_t ruweRegels = (bruikbareHoogte + PIXEL_SCREEN_LINE_SPACING) / PixelScreenRegelStap();

  // Minimumvereiste (16x2, dezelfde ondergrens als de kleinste ondersteunde characterscherm-resolutie) niet gehaald: geen pixeluitvoer voor TYPE_NONE.
  if (ruweKolommen < PIXELGRID_MIN_KOLOMMEN || ruweRegels < PIXELGRID_MIN_RIJEN) {
    pixelScreenStatus.pixelScreenActief = false;
    pixelScreenStatus.foutmelding = _FATAL_PS004;
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
  pixelScreenStatus.pixelScreenActief = true;
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
    // Enkel tekenen zolang de positie binnen het berekende grid valt,
    // geen automatische regelsprong (dat blijft de verantwoordelijkheid van de aanroeper,
    // die met vaste regel-indelingen werkt); tekst die niet meer past,
    // wordt afgekapt in plaats van buiten het grid getekend.
    if (pixelScreenStatus.cursorRegel >= pixelScreenStatus.aantalRegels) break;
    if (pixelScreenStatus.cursorKolom + index >= pixelScreenStatus.aantalKolommen) break;

    PixelScreen->setCursor(pixelScreenStatus.offsetX + (pixelScreenStatus.cursorKolom + index) * PixelScreenKarakterStap(), pixelScreenStatus.offsetY + pixelScreenStatus.cursorRegel * PixelScreenRegelStap());
    PixelScreen->print(tekst[index]);
  }

  pixelScreenStatus.cursorKolom += tekst.length();

  while (pixelScreenStatus.cursorKolom >= pixelScreenStatus.aantalKolommen) {
    pixelScreenStatus.cursorKolom -= pixelScreenStatus.aantalKolommen;
    pixelScreenStatus.cursorRegel++;
  }

  // Begrenzen zodat opeenvolgende te lange teksten nooit onder het grid belanden.
  if (pixelScreenStatus.cursorRegel >= pixelScreenStatus.aantalRegels) {
    pixelScreenStatus.cursorRegel = pixelScreenStatus.aantalRegels > 0 ? pixelScreenStatus.aantalRegels - 1 : 0;
  }
}

static void PixelScreenFoutmeldingWeergeven(const String& foutmelding) {
#if (SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER)
  if (characterScreenStatus.gecontroleerd && characterScreenStatus.characterScreenActief) {
    if (CallbackScreenTypeCharacter) {
      CallbackScreenTypeCharacter(foutmelding, FATAL_ZOEK_OP, LCD_LEESTIJD_FOUTMELDING_MS, "", "", "", 0);
    } else {
      lcd.clear();
      lcd.setCursor(0, 0); lcd.print(foutmelding);
      lcd.setCursor(0, 1); lcd.print(FATAL_ZOEK_OP);
      delay(LCD_LEESTIJD_FOUTMELDING_MS);
    }
    return;
  }
#endif

  Serial.begin(115200);
  Serial.println(foutmelding);
  Serial.println(FATAL_ZOEK_OP);
}
#endif

// ============================================================================
// CharacterScreenFoutmeldingWeergeven: gebruikt enkel een reeds geconfigureerd
// en werkend pixelscherm als terugvalpad; roept zelf nooit een configuratie-
// functie aan (voorkomt een oneindige lus). Staat buiten het PIXELS-blok,
// want ze moet ook bestaan wanneer enkel CHARACTER actief is.
// ============================================================================
#if (SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER)
static void CharacterScreenFoutmeldingWeergeven(const String& foutmelding) {
#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
  if (pixelScreenStatus.gecontroleerd && pixelScreenStatus.pixelScreenActief) {
    if (CallbackScreenTypePixel) {
      CallbackScreenTypePixel(ScreenData::TYPE_FATAL, foutmelding, FATAL_ZOEK_OP, LCD_LEESTIJD_FOUTMELDING_MS, "", "", "", 0);
    } else {
      PixelScreenClear();
      PixelScreenSetCursor(0, 0); PixelScreenPrint(foutmelding);
      PixelScreenSetCursor(0, 1); PixelScreenPrint(FATAL_ZOEK_OP);
      delay(LCD_LEESTIJD_FOUTMELDING_MS);
    }
    return;
  }
#endif

  Serial.begin(115200);
  Serial.println(foutmelding);
  Serial.println(FATAL_ZOEK_OP);
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

  // --------------------------------------------------------------------------
  // D022: verplichte, expliciete configuratie — geen impliciete auto-
  // configuratie meer. Een vergeten of mislukte configuratie wordt hier
  // gemeld (maximaal één keer) in plaats van stilzwijgend gecorrigeerd.
  // --------------------------------------------------------------------------
  bool configuratiefoutWeergegeven = false;

#if (SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER)
  // Enkel afdwingen voor de ingebouwde hardware, een geregistreerde callback
  // vervangt die volledig en is de eigen verantwoordelijkheid van de gebruiker.
  if (!CallbackScreenTypeCharacter) {
    if (!characterScreenStatus.gecontroleerd) {
      CharacterScreenFoutmeldingWeergeven(_FATAL_CS000);
      configuratiefoutWeergegeven = true;
    } else if (!characterScreenStatus.characterScreenActief && !characterScreenStatus.foutmeldingWeergegeven) {
      CharacterScreenFoutmeldingWeergeven(characterScreenStatus.foutmelding);
      characterScreenStatus.foutmeldingWeergegeven = true;
      configuratiefoutWeergegeven = true;
    }
  }
#endif

#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
  // Zelfde redenering: enkel afdwingen wanneer er geen callback geregistreerd is.
  if (!CallbackScreenTypePixel) {
    if (!pixelScreenStatus.gecontroleerd) {
      PixelScreenFoutmeldingWeergeven(_FATAL_PS000);
      configuratiefoutWeergegeven = true;
    } else if (!pixelScreenStatus.pixelScreenActief && !pixelScreenStatus.foutmeldingWeergegeven) {
      PixelScreenFoutmeldingWeergeven(pixelScreenStatus.foutmelding);
      pixelScreenStatus.foutmeldingWeergegeven = true;
      configuratiefoutWeergegeven = true;
    }
  }
#endif

  if (configuratiefoutWeergegeven) return;

#if (SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER)
  const bool characterScreenActief = !CallbackScreenTypeCharacter && characterScreenStatus.characterScreenActief;
#endif

#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
  const bool pixelScreenGeselecteerd = !CallbackScreenTypePixel && screenData == ScreenData::TYPE_NONE;
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

// ============================================================================
// D022: optionele gemakslaag. Roept enkel de configuratiefunctie(s) aan die
// volgens SCREEN_OUTPUT nodig zijn — geen deprecatie van de losse,
// granulaire CharacterScreenConfigureren()/PixelScreenConfigureren().
// ============================================================================
void ScreensConfigureren(bool opnieuwProberen) {
#if (SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER)
  CharacterScreenConfigureren(opnieuwProberen);
#endif
#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
  PixelScreenConfigureren(opnieuwProberen);
#endif
}
