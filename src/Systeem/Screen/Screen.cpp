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
#define CHARACTERSCREEN_KOLOMMEN ((ACTIEF_CHARACTER_SCREEN == SCREEN_LCD2002 || ACTIEF_CHARACTER_SCREEN == SCREEN_LCD2004) ? 20 : (ACTIEF_CHARACTER_SCREEN == SCREEN_LCD4002 ? 40 : 16))
#define CHARACTERSCREEN_REGELS   ((ACTIEF_CHARACTER_SCREEN == SCREEN_LCD1604 || ACTIEF_CHARACTER_SCREEN == SCREEN_LCD2004) ? 4 : 2)
#endif

Screen::Screen()
  : Screen(SCREEN_OUTPUT)
{}

Screen::Screen(uint8_t typesActief)
  : GedeeldeBusNode(&Native, { nullptr, 0, nullptr, 0 }, GedeeldeBusComponent::SCREEN, HardwareResourceToegang::GEDEELD), typesActief(typesActief)
{}

#if (SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER)
Screen::Screen(CharacterScreenCallback callback)
  : Screen(SCREEN_OUTPUT, callback)
{}

Screen::Screen(uint8_t typesActief, CharacterScreenCallback callback)
  : Screen(typesActief) {
  this->Character.callback = callback;
}
#endif

#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
Screen::Screen(PixelScreenCallback callback)
  : Screen(SCREEN_OUTPUT, callback)
{}

Screen::Screen(uint8_t typesActief, PixelScreenCallback callback)
  : Screen(typesActief) {
  this->Pixel.callback = callback;
}
#endif

#if ((SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER) && (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS))
Screen::Screen(CharacterScreenCallback characterCallback, PixelScreenCallback pixelCallback)
  : Screen(SCREEN_OUTPUT, characterCallback, pixelCallback)
{}

Screen::Screen(uint8_t typesActief, CharacterScreenCallback characterCallback, PixelScreenCallback pixelCallback)
  : Screen(typesActief) {
  this->Character.callback = characterCallback;
  this->Pixel.callback = pixelCallback;
}
#endif

struct Screen* Screen = nullptr;

#if (SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER)
void Screen::Character::RegistreerCallback(CharacterScreenCallback callback) { this->callback = callback; }
#endif

#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
#define PIXELGRID_MIN_KOLOMMEN 16
#define PIXELGRID_MAX_KOLOMMEN 40

#define PIXELGRID_MIN_RIJEN     2
#define PIXELGRID_MAX_RIJEN     4

#define PIXEL_SCREEN_CHARACTER_WIDTH  6
#define PIXEL_SCREEN_CHARACTER_HEIGHT 8

int16_t Screen::Pixel::KarakterBreedte() { return PIXEL_SCREEN_CHARACTER_WIDTH * PIXEL_SCREEN_TEXT_SIZE; }
int16_t Screen::Pixel::KarakterStap()    { return KarakterBreedte() + PIXEL_SCREEN_CHARACTER_SPACING; }
int16_t Screen::Pixel::RegelHoogte()     { return PIXEL_SCREEN_CHARACTER_HEIGHT * PIXEL_SCREEN_TEXT_SIZE; }
int16_t Screen::Pixel::RegelStap()       { return RegelHoogte() + PIXEL_SCREEN_LINE_SPACING; }

uint8_t Screen::Pixel::GridClamp(int32_t waarde, uint8_t minimum, uint8_t maximum) {
  if (waarde < minimum) return minimum;
  if (waarde > maximum) return maximum;
  return (uint8_t)waarde;
}

void Screen::Pixel::RegistreerCallback(PixelScreenCallback callback) { this->callback = callback; }

void Screen::Pixel::Clear() {
  if (!this->actief || this->gfx == nullptr) return;
  this->gfx->fillScreen(PIXEL_SCREEN_BACKGROUND_COLOR);
  this->cursorKolom = 0;
  this->cursorRegel = 0;
}

void Screen::Pixel::SetCursor(uint8_t kolom, uint8_t regel) {
  if (!this->actief || this->gfx == nullptr) return;
  this->cursorKolom = kolom;
  this->cursorRegel = regel;
  this->gfx->setCursor(this->offsetX + kolom * KarakterStap(), this->offsetY + regel * RegelStap());
}

void Screen::Pixel::Print(const String& tekst) {
  if (!this->actief || this->gfx == nullptr) return;

  for (uint16_t index = 0; index < tekst.length(); index++) {
    // Enkel tekenen zolang de positie binnen het berekende grid valt, geen automatische regelsprong (dat blijft de verantwoordelijkheid van de aanroeper,
    // die met vaste regel-indelingen werkt); tekst die niet meer past, wordt afgekapt in plaats van buiten het grid getekend.
    if (this->cursorRegel >= this->aantalRegels) break;
    if (this->cursorKolom + index >= this->aantalKolommen) break;

    this->gfx->setCursor(this->offsetX + (this->cursorKolom + index) * KarakterStap(), this->offsetY + this->cursorRegel * RegelStap());
    this->gfx->print(tekst[index]);
  }

  this->cursorKolom += tekst.length();

  while (this->cursorKolom >= this->aantalKolommen) {
    this->cursorKolom -= this->aantalKolommen;
    this->cursorRegel++;
  }

  // Begrenzen zodat opeenvolgende te lange teksten nooit onder het grid belanden.
  if (this->cursorRegel >= this->aantalRegels) {
    this->cursorRegel = this->aantalRegels > 0 ? this->aantalRegels - 1 : 0;
  }
}

void Screen::Pixel::FoutmeldingWeergeven(const String& foutmelding) {
#if (SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER)
  if (::Screen->Character.actief) {
    if (::Screen->Character.callback) {
      ::Screen->Character.callback(foutmelding, FATAL_ZOEK_OP, FATAL_LEESTIJD_MS, "", "", "", 0);
    } else {
      ::Screen->Character.display.clear();
      ::Screen->Character.display.setCursor(0, 0); ::Screen->Character.display.print(foutmelding);
      ::Screen->Character.display.setCursor(0, 1); ::Screen->Character.display.print(FATAL_ZOEK_OP);
      delay(FATAL_LEESTIJD_MS);
    }

    return;
  }
#endif

  // Bewuste uitzondering, geen vergeten #if: dit is de laatste garantie dat een FATAL-fout nooit volledig onzichtbaar blijft. 
  // Daarom niet binnen SCREEN_TYPE_SERIAL; ook zonder geselecteerde SerialScreen blijft dit terugvalpad beschikbaar.
  ::GA_SERIAL.println(foutmelding);
  ::GA_SERIAL.println(FATAL_ZOEK_OP);
}
#endif

#if (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
void Screen::Serial::FoutmeldingWeergeven() {
#if (SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER)
  if (::Screen->Character.callback) {
    ::Screen->Character.callback(this->foutmelding != nullptr ? this->foutmelding : _CRITICAL_SS302, FATAL_ZOEK_OP, 0, "", "", "", 0);
  } else if (::Screen->Character.actief) {
    ::Screen->Character.display.clear();
    ::Screen->Character.display.setCursor(0, 0); ::Screen->Character.display.print(this->foutmelding != nullptr ? this->foutmelding : _CRITICAL_SS302);
    ::Screen->Character.display.setCursor(0, 1); ::Screen->Character.display.print(FATAL_ZOEK_OP);
  }
#endif

#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
  if (::Screen->Pixel.callback) {
    ::Screen->Pixel.callback(ScreenData::TYPE_CRITICAL, this->foutmelding != nullptr ? this->foutmelding : _CRITICAL_SS302, FATAL_ZOEK_OP, 0, "", "", "", 0);
  } else if (::Screen->Pixel.actief) {
    ::Screen->Pixel.Clear();
    ::Screen->Pixel.SetCursor(0, 0); ::Screen->Pixel.Print(this->foutmelding != nullptr ? this->foutmelding : _CRITICAL_SS302);
    ::Screen->Pixel.SetCursor(0, 1); ::Screen->Pixel.Print(FATAL_ZOEK_OP);
  }
#endif
}
#endif

// ============================================================================
// Character.FoutmeldingWeergeven: gebruikt enkel een reeds actief en werkend pixelscherm als terugvalpad; activeert zelf nooit hardware (voorkomt een oneindige lus). 
// Staat buiten het PIXELS-blok, want ze moet ook bestaan wanneer enkel CHARACTER actief is.
// ============================================================================
#if (SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER)
void Screen::Character::FoutmeldingWeergeven(const String& foutmelding) {
#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
  if (::Screen->Pixel.actief) {
    if (::Screen->Pixel.callback) {
      ::Screen->Pixel.callback(ScreenData::TYPE_FATAL, foutmelding, FATAL_ZOEK_OP, FATAL_LEESTIJD_MS, "", "", "", 0);
    } else {
      ::Screen->Pixel.Clear();
      ::Screen->Pixel.SetCursor(0, 0); ::Screen->Pixel.Print(foutmelding);
      ::Screen->Pixel.SetCursor(0, 1); ::Screen->Pixel.Print(FATAL_ZOEK_OP);
      delay(FATAL_LEESTIJD_MS);
    }
    return;
  }
#endif

  // Bewuste uitzondering, geen vergeten #if: dit is de laatste garantie dat een FATAL-fout nooit volledig onzichtbaar blijft. 
  // Daarom niet binnen SCREEN_TYPE_SERIAL; ook zonder geselecteerde SerialScreen blijft dit terugvalpad beschikbaar.
  ::GA_SERIAL.println(foutmelding);
  ::GA_SERIAL.println(FATAL_ZOEK_OP);
}
#endif

void Screen::PrintPrivate(
#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
ScreenData screenData,
#endif
const String& eersteRegel, const String& tweedeRegel, unsigned long delayTime, const String& action, const String& derdeRegel, const String& vierdeRegel, unsigned long delayTussenPaginas) {
#if !(SCREEN_OUTPUT & (SCREEN_TYPE_CHARACTER | SCREEN_TYPE_PIXELS))
  (void)delayTussenPaginas;
#endif

  // --------------------------------------------------------------------------
  // Verplichte, expliciete Screen-lifecycle — geen impliciete activatie.
  // Een vergeten of mislukte Inpluggen()-stap wordt hier gemeld (maximaal één keer) in plaats van stilzwijgend gecorrigeerd.
  // --------------------------------------------------------------------------
  bool lifecycleFoutWeergegeven = false;

#if (SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER)
  // Enkel afdwingen voor de ingebouwde hardware, een geregistreerde callback vervangt die volledig en is de eigen verantwoordelijkheid van de gebruiker.
  if (!this->actief && !this->Character.callback && !this->Character.actief && !this->Character.foutmeldingWeergegeven) {
    if (!this->Character.foutmeldingWeergegeven) {
      if (this->Character.foutmelding != nullptr) {
        this->Character.FoutmeldingWeergeven(this->Character.foutmelding);
      } else if (this->Character.gedeeldeBus == nullptr) {
        this->Character.FoutmeldingWeergeven(_FATAL_CS401);
      }

      this->Character.foutmeldingWeergegeven = true;
    }

    lifecycleFoutWeergegeven = true;
  }
#endif

#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
  // Zelfde redenering: enkel afdwingen wanneer er geen callback geregistreerd is.
  if (!this->actief && !this->Pixel.callback && !this->Pixel.actief && !this->Pixel.foutmeldingWeergegeven) {
    if (!this->Pixel.foutmeldingWeergegeven) {
      if (this->Pixel.foutmelding != nullptr) {
        this->Pixel.FoutmeldingWeergeven(this->Pixel.foutmelding);
      } else if (this->Pixel.gedeeldeBus == nullptr) {
        this->Pixel.FoutmeldingWeergeven(_FATAL_PS401);
      }

      this->Pixel.foutmeldingWeergegeven = true;
    }

    lifecycleFoutWeergegeven = true;
  }
#endif

#if (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
  if (!this->actief && (this->Serial.gedeeldeBus == nullptr || !this->Serial.actief) && !this->Serial.foutmeldingWeergegeven) {
    this->Serial.FoutmeldingWeergeven();
    lifecycleFoutWeergegeven = true;
  }
#endif

  if (lifecycleFoutWeergegeven) return;

#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
  const bool pixelScreenGeselecteerd = !this->Pixel.callback && screenData != ScreenData::TYPE_DEBUG;

  // Zelfde garantie als Character.FoutmeldingWeergeven/Pixel.FoutmeldingWeergeven (CS401/PS401): een TYPE_FATAL/TYPE_PANIC/TYPE_ABORT/TYPE_CRITICAL-melding mag nooit stil verdwijnen. 
  // Enkel van toepassing wanneer geen enkel scherm en geen enkele callback beschikbaar is; is er wel een scherm of callback, dan loopt dit gewoon via het normale, verdere pad hieronder.
  // Bewuste uitzondering: net als bij CS401/PS401 hierboven, niet binnen DEBUG of SCREEN_TYPE_SERIAL, want dit ís de garantie zelf.
  if (screenData == ScreenData::TYPE_FATAL || screenData == ScreenData::TYPE_PANIC || screenData == ScreenData::TYPE_ABORT || screenData == ScreenData::TYPE_CRITICAL) {
    bool geenEnkelScreenBeschikbaar = !this->Pixel.callback && !(pixelScreenGeselecteerd && this->Pixel.actief);
#if (SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER)
    geenEnkelScreenBeschikbaar = geenEnkelScreenBeschikbaar && !this->Character.callback && !(!this->Character.callback && this->Character.actief);
#endif

    if (geenEnkelScreenBeschikbaar) {
      if (eersteRegel != "") ::GA_SERIAL.println(eersteRegel);
      if (tweedeRegel != "") ::GA_SERIAL.println(tweedeRegel);
      if (derdeRegel != "") ::GA_SERIAL.println(derdeRegel);
      if (vierdeRegel != "") ::GA_SERIAL.println(vierdeRegel);
      return;
    }
  }
#endif

  // --------------------------------------------------------------------------
  // STANDAARDUITVOER: REGEL 1 EN REGEL 2
  // --------------------------------------------------------------------------
  if (eersteRegel != "" || tweedeRegel != "") {
#if (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
    if (this->Serial.actief) {
      ::GA_SERIAL.println(eersteRegel);
      ::GA_SERIAL.println(tweedeRegel);
    }
#endif

#if (SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER)
    if ((!this->Character.callback && this->Character.actief)) {
      this->Character.display.clear();
      this->Character.display.setCursor(0, 0); this->Character.display.print(eersteRegel);
      this->Character.display.setCursor(0, 1); this->Character.display.print(tweedeRegel);
    }
#endif

#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
    if ((pixelScreenGeselecteerd && this->Pixel.actief)) {
      this->Pixel.Clear();
      this->Pixel.SetCursor(0, 0); this->Pixel.Print(eersteRegel);
      this->Pixel.SetCursor(0, 1); this->Pixel.Print(tweedeRegel);
    }
#endif
  }

  // --------------------------------------------------------------------------
  // STANDAARDUITVOER: DELAY EN ACTION
  // --------------------------------------------------------------------------
  bool standaardScreenActief = false;

#if (SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER)
  standaardScreenActief = standaardScreenActief || (!this->Character.callback && this->Character.actief);
#endif
#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
  standaardScreenActief = standaardScreenActief || (pixelScreenGeselecteerd && this->Pixel.actief);
#endif
#if (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL) && !(SCREEN_OUTPUT & (SCREEN_TYPE_CHARACTER | SCREEN_TYPE_PIXELS))
  standaardScreenActief = this->Serial.actief;
#endif

  if (standaardScreenActief) {
    if (delayTime) delay(delayTime);

    if (action != "") {
#if (SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER)
      if ((!this->Character.callback && this->Character.actief)) this->Character.display.print(action);
#endif

#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
      if ((pixelScreenGeselecteerd && this->Pixel.actief)) this->Pixel.Print(action);
#endif
    }
  }

#if (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
  if (action != "" && this->Serial.actief) {
    ::GA_SERIAL.println(action);
  }
#endif

  // --------------------------------------------------------------------------
  // STANDAARDUITVOER: REGEL 3 EN REGEL 4
  // --------------------------------------------------------------------------
  if (derdeRegel != "" || vierdeRegel != "") {
#if (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
    if (this->Serial.actief) {
      ::GA_SERIAL.println(derdeRegel);
      ::GA_SERIAL.println(vierdeRegel);
    }
#endif

    bool tweedePaginaNodig = false;
#if (SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER)
    tweedePaginaNodig = tweedePaginaNodig || ((!this->Character.callback && this->Character.actief) && !ACTIEF_CHARACTER_SCREEN_MET_VIER_REGELS);
#endif
#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
    tweedePaginaNodig = tweedePaginaNodig || ((pixelScreenGeselecteerd && this->Pixel.actief) && !this->Pixel.emulateLCDxxx4);
#endif
    if (tweedePaginaNodig && delayTussenPaginas) delay(delayTussenPaginas);

#if (SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER)
    if ((!this->Character.callback && this->Character.actief)) {
      if (!ACTIEF_CHARACTER_SCREEN_MET_VIER_REGELS) this->Character.display.clear();
      this->Character.display.setCursor(0, ACTIEF_CHARACTER_SCREEN_MET_VIER_REGELS ? 2 : 0); this->Character.display.print(derdeRegel);
      this->Character.display.setCursor(0, ACTIEF_CHARACTER_SCREEN_MET_VIER_REGELS ? 3 : 1); this->Character.display.print(vierdeRegel);
    }
#endif

#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
    if ((pixelScreenGeselecteerd && this->Pixel.actief)) {
      if (!this->Pixel.emulateLCDxxx4) this->Pixel.Clear();
      this->Pixel.SetCursor(0, this->Pixel.emulateLCDxxx4 ? 2 : 0); this->Pixel.Print(derdeRegel);
      this->Pixel.SetCursor(0, this->Pixel.emulateLCDxxx4 ? 3 : 1); this->Pixel.Print(vierdeRegel);
    }
#endif
  }

  // --------------------------------------------------------------------------
  // CALLBACK CHARACTER: ÉÉN AANROEP MET DE VOLLEDIGE OPDRACHT
  // --------------------------------------------------------------------------
#if (SCREEN_OUTPUT & (SCREEN_TYPE_CHARACTER | SCREEN_TYPE_PIXELS))
  const unsigned long callbackDelayTime = standaardScreenActief ? 0 : delayTime;
  const unsigned long callbackDelayTussenPaginas = standaardScreenActief ? 0 : delayTussenPaginas;
#endif

#if (SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER)
  if (this->Character.callback) this->Character.callback(eersteRegel, tweedeRegel, callbackDelayTime, action, derdeRegel, vierdeRegel, callbackDelayTussenPaginas);
#endif

  // --------------------------------------------------------------------------
  // CALLBACK PIXELS: ÉÉN AANROEP MET DE VOLLEDIGE OPDRACHT
  // --------------------------------------------------------------------------
#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
  if (this->Pixel.callback) this->Pixel.callback(screenData, eersteRegel, tweedeRegel, callbackDelayTime, action, derdeRegel, vierdeRegel, callbackDelayTussenPaginas);
#endif
}

#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
void Screen::Print(ScreenData screenData, const String& eersteRegel, const String& tweedeRegel, unsigned long delayTime, const String& action, const String& derdeRegel, const String& vierdeRegel, unsigned long delayTussenPaginas) { PrintPrivate(screenData, eersteRegel, tweedeRegel, delayTime, action, derdeRegel, vierdeRegel, delayTussenPaginas); }
void Screen::Print(const String& eersteRegel, const String& tweedeRegel, unsigned long delayTime, const String& action, const String& derdeRegel, const String& vierdeRegel, unsigned long delayTussenPaginas) { PrintPrivate(ScreenData::TYPE_NONE, eersteRegel, tweedeRegel, delayTime, action, derdeRegel, vierdeRegel, delayTussenPaginas); }
#else
void Screen::Print(const String& eersteRegel, const String& tweedeRegel, unsigned long delayTime, const String& action, const String& derdeRegel, const String& vierdeRegel, unsigned long delayTussenPaginas) { PrintPrivate(eersteRegel, tweedeRegel, delayTime, action, derdeRegel, vierdeRegel, delayTussenPaginas); }
#endif

// ============================================================================
// v2.0.0: Screen-levenscyclus.
// ============================================================================
// Hulpfuncties voor de levenscyclus van Screen.
template<typename T>
static void ChildVerwijderen(T*& child) {
  if (child != nullptr) {
#ifdef TRACE
    GA_SERIAL.print("TRACE: ChildVerwijderen(): component=0x");
    GA_SERIAL.println(static_cast<uint8_t>(child->component), HEX);
#endif
    child->afmelden();   // verwijdert het child, want het werd met componentCreated aangemaakt
    child = nullptr;
  }
}

static int AantalUitvoeren(const struct Screen& scherm) {
  int aantal = 0;
#if (SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER)
  if (scherm.Character.gedeeldeBus != nullptr) aantal++;
#endif
#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
  if (scherm.Pixel.gedeeldeBus != nullptr) aantal++;
#endif
#if (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
  if (scherm.Serial.gedeeldeBus != nullptr) aantal++;
#endif
  return aantal;
}

// Elke stap gaat via Screen naar zijn uitvoeren. Een uitvoer die in een stap faalt, wordt gemeld en verwijderd.
// De andere uitvoeren blijven werken. Enkel wanneer van de gekozen uitvoeren geen enkele overblijft, faalt Screen zelf.
bool Screen::aanmelden() {
  if (aangemeld) return true;
  if ((this->typesActief & SCREEN_OUTPUT) != this->typesActief) return false;
  if (!GedeeldeBusNode::aanmelden()) return false;

#if (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
  if ((this->typesActief & SCREEN_TYPE_SERIAL) && this->Serial.gedeeldeBus == nullptr) {
    this->Serial.foutmelding = nullptr;
    this->Serial.foutmeldingWeergegeven = false;
    SerialOutput* child = new SerialOutput(this, GedeeldeBusComponent::SERIAL_OUTPUT, HardwareResourcePin::D1, HardwareResourcePin::D0);
    child->componentCreated = true;

    if (child->aanmelden()) {
      this->Serial.gedeeldeBus = child;
    } else {
      delete child;
      this->Serial.foutmelding = _ABORT_SS001;
    }
  }
#endif

#if (SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER)
  if ((this->typesActief & SCREEN_TYPE_CHARACTER) && this->Character.gedeeldeBus == nullptr) {
    this->Character.foutmelding = nullptr;
    this->Character.foutmeldingWeergegeven = false;
    ::CharacterScreen* child = new ::CharacterScreen(this, GedeeldeBusComponent::CHARACTER_SCREEN, I2C_ADDRESS_CHARACTER_SCREEN, HardwareResourcePin::SDA, HardwareResourcePin::SCL);
    child->componentCreated = true;

    if (child->aanmelden()) {
      this->Character.gedeeldeBus = child;
    } else {
      delete child;
      this->Character.foutmelding = _ABORT_CS001;
    }
  }
#endif

#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
  if ((this->typesActief & SCREEN_TYPE_PIXELS) && this->Pixel.gedeeldeBus == nullptr) {
    this->Pixel.foutmelding = nullptr;
    this->Pixel.foutmeldingWeergegeven = false;
    ::PixelScreen* child = new ::PixelScreen(this, GedeeldeBusComponent::PIXEL_SCREEN, PIXEL_SCREEN_CS, HardwareResourcePin::SCK, HardwareResourcePin::MISO, HardwareResourcePin::MOSI, PIXEL_SCREEN_DC, PIXEL_SCREEN_RST);
    child->componentCreated = true;

    if (child->aanmelden()) {
      this->Pixel.gedeeldeBus = child;
    } else {
      delete child;
      this->Pixel.foutmelding = _ABORT_PS001;
    }
  }
#endif

  if (this->typesActief != SCREEN_TYPE_NONE && AantalUitvoeren(*this) == 0) {
    this->ActivatiefoutWeergeven();
    return false;
  }
  return true;
}

bool Screen::controleren() {
  if (gecontroleerd) return true;
  if (!GedeeldeBusNode::controleren()) return false;

#if (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
  if (this->Serial.gedeeldeBus != nullptr && !this->Serial.gedeeldeBus->controleren()) {
    if (this->Serial.gedeeldeBus->conflictGevonden) this->Serial.foutmeldingWeergegeven = true;
    else this->Serial.foutmelding = _FATAL_SS101;
  }
#endif

#if (SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER)
  if (this->Character.gedeeldeBus != nullptr && !this->Character.gedeeldeBus->controleren()) {
    if (this->Character.gedeeldeBus->conflictGevonden) this->Character.foutmeldingWeergegeven = true;
    else this->Character.foutmelding = _FATAL_CS101;
  }
#endif

#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
  if (this->Pixel.gedeeldeBus != nullptr && !this->Pixel.gedeeldeBus->controleren()) {
    if (this->Pixel.gedeeldeBus->conflictGevonden) this->Pixel.foutmeldingWeergegeven = true;
    else this->Pixel.foutmelding = _FATAL_PS101;
  }
#endif

#if (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
  if (this->Serial.gedeeldeBus != nullptr && !this->Serial.gedeeldeBus->gecontroleerd) ChildVerwijderen(this->Serial.gedeeldeBus);
#endif
#if (SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER)
  if (this->Character.gedeeldeBus != nullptr && !this->Character.gedeeldeBus->gecontroleerd) ChildVerwijderen(this->Character.gedeeldeBus);
#endif
#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
  if (this->Pixel.gedeeldeBus != nullptr && !this->Pixel.gedeeldeBus->gecontroleerd) ChildVerwijderen(this->Pixel.gedeeldeBus);
#endif

  if (this->typesActief != SCREEN_TYPE_NONE && AantalUitvoeren(*this) == 0) {
    this->ActivatiefoutWeergeven();
    return false;
  }

  return true;
}

// Toont de bewaarde foutmeldingen van uitvoeren die niet werken, op de uitvoeren die wel werken, zoals in v1.1.2.
// Tijdens de aanmaak is ::Screen nog nullptr, terwijl FoutmeldingWeergeven() en de callbacks ::Screen-> gebruiken.
// Daarom wijst ::Screen hier tijdelijk naar dit object, daarna wordt de vorige waarde hersteld.
void Screen::ActivatiefoutWeergeven() {
  Screen* vorige = ::Screen;
  ::Screen = this;

#if (SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER)
  if (this->Character.foutmelding != nullptr && !this->Character.foutmeldingWeergegeven) {
    this->Character.FoutmeldingWeergeven(this->Character.foutmelding);
    this->Character.foutmeldingWeergegeven = true;
  }
#endif

#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
  if (this->Pixel.foutmelding != nullptr && !this->Pixel.foutmeldingWeergegeven) {
    this->Pixel.FoutmeldingWeergeven(this->Pixel.foutmelding);
    this->Pixel.foutmeldingWeergegeven = true;
  }
#endif

#if (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
  if ((this->typesActief & SCREEN_TYPE_SERIAL) && !this->Serial.actief && !this->Serial.foutmeldingWeergegeven) {
    this->Serial.FoutmeldingWeergeven();
    this->Serial.foutmeldingWeergegeven = true;
  }
#endif

  ::Screen = vorige;
}

bool Screen::inpluggen() {
  if (ingeplugd) return true;
  if (!GedeeldeBusNode::inpluggen()) return false;

#if (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
  if (this->Serial.gedeeldeBus != nullptr && !this->Serial.gedeeldeBus->inpluggen()) {
    this->Serial.foutmelding = _CRITICAL_SS201;
    ChildVerwijderen(this->Serial.gedeeldeBus);
  }
#endif

#if (SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER)
  if (this->Character.gedeeldeBus != nullptr && !this->Character.gedeeldeBus->inpluggen()) {
    this->Character.foutmelding = _CRITICAL_CS201;
    ChildVerwijderen(this->Character.gedeeldeBus);
  }
#endif

#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
  if (this->Pixel.gedeeldeBus != nullptr && !this->Pixel.gedeeldeBus->inpluggen()) {
    this->Pixel.foutmelding = _CRITICAL_PS201;
    ChildVerwijderen(this->Pixel.gedeeldeBus);
  }
#endif

  if (this->typesActief != SCREEN_TYPE_NONE && AantalUitvoeren(*this) == 0) {
    this->ActivatiefoutWeergeven();
    return false;
  }

  return true;
}

// Activeren: elke uitvoer activeren, en daarna het scherm zelf instellen (init, rotatie, raster).
bool Screen::Activeren() {
#if (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
  if (this->Serial.gedeeldeBus != nullptr) {
    if (this->Serial.gedeeldeBus->activeren()) {
      this->Serial.actief = true;
#ifdef TRACE
      ::GA_SERIAL.println("TRACE: Screen::Activeren(): Serial actief, TRACE start");
#endif
    } else {
      this->Serial.actief = false;
      this->Serial.foutmelding = _CRITICAL_SS301;
      ChildVerwijderen(this->Serial.gedeeldeBus);
    }
  }
#endif

#if (SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER)
#ifdef TRACE
  ::GA_SERIAL.println("TRACE: Screen::Activeren(): Character");
#endif
  if (this->Character.gedeeldeBus != nullptr) {
    if (this->Character.gedeeldeBus->activeren()) {
      this->Character.display.init();
      this->Character.display.backlight();
      this->Character.actief = true;
    } else {
      this->Character.foutmelding = _CRITICAL_CS301;
      ChildVerwijderen(this->Character.gedeeldeBus);
    }
  }
#endif

#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
#ifdef TRACE
  ::GA_SERIAL.println("TRACE: Screen::Activeren(): Pixel");
#endif
  if (this->Pixel.gedeeldeBus != nullptr) {
    bool pixelOk = this->Pixel.gedeeldeBus->activeren();
    if (!pixelOk) this->Pixel.foutmelding = _CRITICAL_PS301;

    if (pixelOk) {
      this->Pixel.display.init(ACTIEF_PIXEL_SCREEN_BREEDTE, ACTIEF_PIXEL_SCREEN_HOOGTE);
      this->Pixel.display.setRotation(PIXEL_SCREEN_ROTATION);
      this->Pixel.gfx = &this->Pixel.display;

      if (this->Pixel.gfx == nullptr) {
        this->Pixel.foutmelding = _CRITICAL_PS302;
        pixelOk = false;
      } else {
#if (PIXEL_SCREEN_ROTATION == 1 || PIXEL_SCREEN_ROTATION == 3)
        if (this->Pixel.gfx->width() != ACTIEF_PIXEL_SCREEN_HOOGTE || this->Pixel.gfx->height() != ACTIEF_PIXEL_SCREEN_BREEDTE) {
          this->Pixel.foutmelding = _CRITICAL_PS303;
          pixelOk = false;
        }
#else
        if (this->Pixel.gfx->width() != ACTIEF_PIXEL_SCREEN_BREEDTE || this->Pixel.gfx->height() != ACTIEF_PIXEL_SCREEN_HOOGTE) {
          this->Pixel.foutmelding = _CRITICAL_PS304;
          pixelOk = false;
        }
#endif
      }

      if (pixelOk) {
        const int16_t bruikbareBreedte = this->Pixel.gfx->width() - (2 * PIXEL_SCREEN_MARGIN);
        const int16_t bruikbareHoogte  = this->Pixel.gfx->height() - (2 * PIXEL_SCREEN_MARGIN);

        const int32_t ruweKolommen     = (bruikbareBreedte + PIXEL_SCREEN_CHARACTER_SPACING) / this->Pixel.KarakterStap();
        const int32_t ruweRegels       = (bruikbareHoogte + PIXEL_SCREEN_LINE_SPACING) / this->Pixel.RegelStap();

        if (ruweKolommen < PIXELGRID_MIN_KOLOMMEN || ruweRegels < PIXELGRID_MIN_RIJEN) {
          this->Pixel.foutmelding = _CRITICAL_PS305;
          pixelOk = false;
        } else {
          this->Pixel.aantalKolommen  = this->Pixel.GridClamp(ruweKolommen, PIXELGRID_MIN_KOLOMMEN, PIXELGRID_MAX_KOLOMMEN);
          this->Pixel.aantalRegels    = this->Pixel.GridClamp(ruweRegels, PIXELGRID_MIN_RIJEN, PIXELGRID_MAX_RIJEN);

          const int16_t gridBreedtePx = this->Pixel.aantalKolommen * this->Pixel.KarakterBreedte() + (this->Pixel.aantalKolommen - 1) * PIXEL_SCREEN_CHARACTER_SPACING;
          const int16_t gridHoogtePx  = this->Pixel.aantalRegels * this->Pixel.RegelHoogte() + (this->Pixel.aantalRegels - 1) * PIXEL_SCREEN_LINE_SPACING;

          this->Pixel.offsetX = PIXEL_SCREEN_MARGIN + (bruikbareBreedte - gridBreedtePx) / 2;
          this->Pixel.offsetY = PIXEL_SCREEN_MARGIN + (bruikbareHoogte - gridHoogtePx) / 2;

          if (this->Pixel.offsetX < PIXEL_SCREEN_MARGIN) this->Pixel.offsetX = PIXEL_SCREEN_MARGIN;
          if (this->Pixel.offsetY < PIXEL_SCREEN_MARGIN) this->Pixel.offsetY = PIXEL_SCREEN_MARGIN;

          this->Pixel.emulateLCDxxx4 = this->Pixel.aantalRegels >= 4;
          this->Pixel.gfx->setTextSize(PIXEL_SCREEN_TEXT_SIZE);
          this->Pixel.gfx->setTextColor(PIXEL_SCREEN_TEXT_COLOR, PIXEL_SCREEN_BACKGROUND_COLOR);
          this->Pixel.gfx->setTextWrap(false);
          this->Pixel.actief = true;
        }
      }
    }

    if (!pixelOk) {
      this->Pixel.gfx = nullptr;
      this->Pixel.actief = false;
      ChildVerwijderen(this->Pixel.gedeeldeBus);
    }
  }
#endif

  this->ActivatiefoutWeergeven();
  return this->typesActief == SCREEN_TYPE_NONE || AantalUitvoeren(*this) > 0;
}

bool Screen::afmelden() {
#if (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
  this->Serial.actief = false;

  if (this->Serial.gedeeldeBus != nullptr) {
    if (!this->Serial.gedeeldeBus->afmelden()) return false;
    this->Serial.gedeeldeBus = nullptr;
  }
#endif

#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
  this->Pixel.actief = false;
  this->Pixel.gfx = nullptr;

  if (this->Pixel.gedeeldeBus != nullptr) {
    if (!this->Pixel.gedeeldeBus->afmelden()) return false;
    this->Pixel.gedeeldeBus = nullptr;
  }
#endif

#if (SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER)
  this->Character.actief = false;

  if (this->Character.gedeeldeBus != nullptr) {
    if (!this->Character.gedeeldeBus->afmelden()) return false;
    this->Character.gedeeldeBus = nullptr;
  }
#endif

  if (::Screen == this) ::Screen = nullptr;
  return GedeeldeBusNode::afmelden();
}

