#ifndef GROEIACADEMIE_LIBRARY_NL_H
#define GROEIACADEMIE_LIBRARY_NL_H

//=========================================================
// FATAL
//=========================================================
#ifndef FATAL_ZOEK_OP
  #define FATAL_ZOEK_OP "ZOEK DIT NU OP"
#endif

// CharacterScreen
#ifndef _FATAL_CS000
  #define _FATAL_CS000 "FATAL: CS000" // CharacterScreenConfigureren() niet aangeroepen
#endif
#ifndef _FATAL_CS001
  #define _FATAL_CS001 "FATAL: CS001" // Geen I2C-antwoord op geen enkel geprobeerd adres
#endif
#ifndef _FATAL_CS002
  #define _FATAL_CS002 "FATAL: CS002" // Scherm gevonden op ander I2C-adres dan I2C_ADRES (modus 1)
#endif

// PixelScreen
#ifndef _FATAL_PS000
  #define _FATAL_PS000 "FATAL: PS000" // PixelScreenConfigureren() niet aangeroepen
#endif
#ifndef _FATAL_PS001
  #define _FATAL_PS001 "FATAL: PS001" // PixelScreen niet gekoppeld
#endif
#ifndef _FATAL_PS002
  #define _FATAL_PS002 "FATAL: PS002" // Omgewisselde breedte en hoogte komen niet overeen
#endif
#ifndef _FATAL_PS003
  #define _FATAL_PS003 "FATAL: PS003" // Niet-omgewisselde breedte en hoogte komen niet overeen
#endif
#ifndef _FATAL_PS004
  #define _FATAL_PS004 "FATAL: PS004" // Tekstgrid kleiner dan 16x2
#endif

//=========================================================
// LCD
//=========================================================
// ADS1115
#ifndef _LCD_ADS1115_FOUT
  #define _LCD_ADS1115_FOUT           "ADS1115"
#endif
#ifndef _LCD_ADS1115_NIET_GEVONDEN
  #define _LCD_ADS1115_NIET_GEVONDEN  "NIET GEVONDEN"
#endif

// Stimulus
#ifndef _LCD_KRACHT_TE_HARD
  #define _LCD_KRACHT_TE_HARD         "TE HARD"
#endif
#ifndef _LCD_KRACHT_TE_ZACHT
  #define _LCD_KRACHT_TE_ZACHT        "TE ZACHT"
#endif

#ifndef _LCD_SCORE_TIKKRACHT
  #define _LCD_SCORE_TIKKRACHT        "TIKKRACHT "
#endif
#ifndef _LCD_SCORE_TIKTIJD
  #define _LCD_SCORE_TIKTIJD          "TIKTIJD "
#endif

#ifndef _LCD_TIJD_METEN_STOPT
  #define _LCD_TIJD_METEN_STOPT       "WE STOPPEN ERMEE"
#endif
#ifndef _LCD_TIJD_TEVEEL_FOUT
  #define _LCD_TIJD_TEVEEL_FOUT       "SLECHTE START :)"
#endif
#ifndef _LCD_TIJD_TE_KORT
  #define _LCD_TIJD_TE_KORT           "TE KORT"
#endif
#ifndef _LCD_TIJD_TE_LANG
  #define _LCD_TIJD_TE_LANG           "TE LANG"
#endif

#endif
