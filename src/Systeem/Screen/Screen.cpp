#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "Screen.h"

/* TODO
static bool screenGeinitialiseerd = false;

if (!screenGeinitialiseerd) {
  lcd.init();
  lcd.backlight();
  screenGeinitialiseerd = true;
}

if ((SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER) && !screenGeinitialiseerd) {
  lcd.init();
  lcd.backlight();
  screenGeinitialiseerd = true;
}
*/

// ============================================================================
// HARDWARE INSTELLINGEN (De pinnen van de Arduino UNO R3/R4)
// ============================================================================

// De compiler leest ACTIEF_CHARACTER_SCREEN en vult breedte en hoogte in.
LiquidCrystal_I2C lcd(I2C_ADRES, (ACTIEF_CHARACTER_SCREEN == SCREEN_LCD2002 || ACTIEF_CHARACTER_SCREEN == SCREEN_LCD2004) ? 20 : (ACTIEF_CHARACTER_SCREEN == SCREEN_LCD4002 ? 40 : 16), (ACTIEF_CHARACTER_SCREEN == SCREEN_LCD1604 || ACTIEF_CHARACTER_SCREEN == SCREEN_LCD2004) ? 4 : 2);

// ACTIEVE UITVOER kan maximaal omvatten: Serial + één characterscherm + één pixelscherm
#ifdef DEBUG
  uint8_t SCREEN_OUTPUT = SCREEN_TYPE_SERIAL | SCREEN_TYPE_CHARACTER | SCREEN_TYPE_PIXELS;
#else
  uint8_t SCREEN_OUTPUT = SCREEN_TYPE_CHARACTER | SCREEN_TYPE_PIXELS;
#endif

// ============================================================================
// GEREGISTREERDE CALLBACKS
// nullptr bij CHARACTER gebruikt de standaardafhandeling via LiquidCrystal_I2C.
// nullptr bij PIXELS geeft geen pixeluitvoer.
// ============================================================================
ScreenCallback CallbackScreenTypeCharacter = nullptr;
ScreenCallback CallbackScreenTypePixel = nullptr;

// CALLBACKS REGISTREREN
void RegistreerCallbackScreenTypeCharacter(ScreenCallback callback) { CallbackScreenTypeCharacter = callback; }
void RegistreerCallbackScreenTypePixel(ScreenCallback callback) { CallbackScreenTypePixel = callback; }

// ============================================================================
// CENTRALE PRINTFUNCTIE
//
// Deze functie:
// 1. print de twee tot 4 regels op alle actieve uitvoerdoelen en indien nodig voert het de delayTussenPaginas uit bij vier regels op een scherm van twee regels;
// 2. voert de delay één keer uit;
// 3. stuurt daarna uitsluitend de action naar alle actieve uitvoerdoelen.
// 
// De standaarduitvoer naar Serial en het standaard characterscherm behoudt één centrale tijdlijn. Een geregistreerde character- of pixelcallback ontvangt exact één keer de volledige opdracht en neemt voor dat schermtype alles over.
// ============================================================================
void PrintToScreen(const String& eersteRegel, const String& tweedeRegel, unsigned long delayTime, const String& action, const String& derdeRegel, const String& vierdeRegel, unsigned long delayTussenPaginas) {
  const bool serialActief = SCREEN_OUTPUT & SCREEN_TYPE_SERIAL;
  const bool characterScreenActief = (SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER) && !CallbackScreenTypeCharacter;

  // --------------------------------------------------------------------------
  // STANDAARDUITVOER: REGEL 1 EN REGEL 2
  // --------------------------------------------------------------------------
  if (eersteRegel != "" || tweedeRegel != "") {
    if (serialActief) {
      Serial.println(eersteRegel);
      Serial.println(tweedeRegel);
    }

    if (characterScreenActief) {
      lcd.clear();
      lcd.setCursor(0, 0); lcd.print(eersteRegel);
      lcd.setCursor(0, 1); lcd.print(tweedeRegel);
    }
  }

  // --------------------------------------------------------------------------
  // STANDAARDUITVOER: REGEL 3 EN REGEL 4
  // --------------------------------------------------------------------------
  if (derdeRegel != "" || vierdeRegel != "") {
    if (serialActief) {
      Serial.println(derdeRegel);
      Serial.println(vierdeRegel);
    }

    if (characterScreenActief) {
      if (!ACTIEF_CHARACTER_SCREEN_MET_VIER_REGELS) {
        if (delayTussenPaginas) delay(delayTussenPaginas);
        lcd.clear();
      }

      lcd.setCursor(0, ACTIEF_CHARACTER_SCREEN_MET_VIER_REGELS ? 2 : 0);
      lcd.print(derdeRegel);

      lcd.setCursor(0, ACTIEF_CHARACTER_SCREEN_MET_VIER_REGELS ? 3 : 1);
      lcd.print(vierdeRegel);
    }
  }

  // --------------------------------------------------------------------------
  // STANDAARDUITVOER: DELAY EN ACTION
  // --------------------------------------------------------------------------
  bool callbackZorgtVoorWachttijd = ((SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER) && CallbackScreenTypeCharacter) || ((SCREEN_OUTPUT & SCREEN_TYPE_PIXELS) && CallbackScreenTypePixel);

  if (serialActief || characterScreenActief) {
    if (delayTime && !callbackZorgtVoorWachttijd) delay(delayTime);

    if (action != "") {
      if (serialActief) Serial.println(action);
      if (characterScreenActief) lcd.print(action);
    }
  }

  // --------------------------------------------------------------------------
  // CALLBACK CHARACTER: ÉÉN AANROEP MET DE VOLLEDIGE OPDRACHT
  // --------------------------------------------------------------------------
  if ((SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER) && CallbackScreenTypeCharacter) CallbackScreenTypeCharacter(eersteRegel, tweedeRegel, delayTime, action, derdeRegel, vierdeRegel, delayTussenPaginas);

  // --------------------------------------------------------------------------
  // CALLBACK PIXELS: ÉÉN AANROEP MET DE VOLLEDIGE OPDRACHT
  // --------------------------------------------------------------------------
  if ((SCREEN_OUTPUT & SCREEN_TYPE_PIXELS) && CallbackScreenTypePixel) CallbackScreenTypePixel(eersteRegel, tweedeRegel, delayTime, action, derdeRegel, vierdeRegel, delayTussenPaginas);
}
