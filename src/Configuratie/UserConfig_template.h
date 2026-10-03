#ifndef GROEIACADEMIE_USER_CONFIG_H
#define GROEIACADEMIE_USER_CONFIG_H

// Kopieer dit bestand vóór gebruik naar UserConfig.h in dezelfde map.
// UserConfig.h is optioneel: zonder dit bestand gebruikt de library uitsluitend de standaardwaarden uit SystemConfig.h.
// Een aanwezig UserConfig.h wordt vóór de standaardwaarden uit SystemConfig.h ingelezen.
// De volledige oorspronkelijke structuur, headers, commentaren, volgorde en witregels uit SystemConfig.h blijven hieronder behouden.
//
// ============================================================================
// BELANGRIJK: CENTRALE GEBRUIKERSCONFIGURATIE
// ============================================================================
// Screen.cpp en Stimulus.cpp worden door de Arduino-toolchain als losse, onafhankelijke bestanden gecompileerd, niet samen met je .ino. 
// Instellingen die bepalen wat in deze librarybestanden wordt gecompileerd horen daarom in UserConfig.h of, als officiële fallback, in SystemConfig.h.
//
// Gewone examples stellen deze libraryconfiguratie niet rechtstreeks in de .ino in.
// Instellingen die uitsluitend het voorbeeldprogramma zelf gebruikt, horen in ExamplesConfig.h of lokaal in dat voorbeeld.
// ============================================================================

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// HIERONDER AANPASSEN NAAR EIGEN BEHOEFTE
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// ****************************************************************************
// BASIS INSTELLINGEN
// ****************************************************************************

// ============================================================================
// TAAL: jouw keuze
// ============================================================================
// #define LANGUAGE_NL // standaard waarde
// #define LANGUAGE_DE
// #define LANGUAGE_EN
// #define LANGUAGE_FR

// ============================================================================
// DEBUG & TRACE: jouw keuze
// ============================================================================
// #define DEBUG // ENKEL wanneer DEBUG nodig, staan hier geen commentaar '//' tekens voor :)
// #define TRACE // ENKEL wanneer TRACE nodig, staan hier geen commentaar '//' tekens voor :)
// GA_DEBUG_PRINT, GA_DEBUG_PRINTLN en GA_DEBUG_PRINTLN2 worden door SystemConfig.h afgeleid uit DEBUG.

// ============================================================================
// BOARD_VERSION: jouw keuze
// ============================================================================
// De automatische detectie in SystemConfig.h gebruikt de compiler-/boardmacro van de geselecteerde Arduino-boardcore.
//
// Voor SB Components Ardi32 blijft expliciete BOARD_VERSION-keuze nodig wanneer de generieke ESP32S3 Dev Module wordt gebruikt; zie docs/HARDWARE_SUPPORT.md.
//
// Hardwarevalidatiestatus (zie docs/HARDWARE_SUPPORT.md voor details):
// - Getest en goedgekeurd: BOARD_UNO_R3, BOARD_UNO_R4_MINIMA, BOARD_UNO_R4_WIFI, BOARD_ESP32_D1_UNO_R32 (WEMOS D1 R32), BOARD_RP2040_CYTRON_MAKER_UNO & BOARD_STM32F4_NUCLEO64_F401RE.
// - Nog hardwarematig te testen: BOARD_UNO_Q, BOARD_ESP32S3_ARDI32 & BOARD_ESP32S3_DEV
//
// Mogelijke waarden: BOARD_UNO_R3, BOARD_UNO_R4_MINIMA, BOARD_UNO_R4_WIFI, BOARD_UNO_Q, BOARD_ESP32_D1_UNO_R32, BOARD_ESP32S3_ARDI32, BOARD_ESP32S3_DEV, BOARD_RP2040_CYTRON_MAKER_UNO & BOARD_STM32F4_NUCLEO64_F401RE
#ifndef BOARD_VERSION
// #define BOARD_VERSION BOARD_UNO_R4_MINIMA // geen standaard waarde, wordt automatisch bepaald in SystemConfig.h
#endif

// ============================================================================
// BOARD_ID: jouw instellingen
// ============================================================================
// BOARD_ID: generieke, per-fysiek-bord-identiteit. 
// (FUTURE) Wijzig per fysiek bord, bijvoorbeeld wanneer meerdere borden met elkaar moeten communiceren en elkaar moeten kunnen onderscheiden. Nog zonder functioneel effect zolang er geen bord-overschrijdende communicatie gebruikt wordt.
#ifndef BOARD_ID
// #define BOARD_ID 1   // standaard waarde: 1, wijzig dit per fysiek bord
#endif

// ============================================================================
// ADC BACKEND: jouw instellingen
// ============================================================================
// LET OP: Wanneer je ADC_BACKEND op ADC_BACKEND_ADS1115 zet, moet de Adafruit ADS1X15-library geïnstalleerd zijn via de Arduino Library Manager.
// ============================================================================
#ifndef AANTAL_SENSOREN_AANWEZIG
// #define AANTAL_SENSOREN_AANWEZIG  2                   // standaard waarde: 2
#endif

// Mogelijke waarden: ADC_BACKEND_NATIVE of ADC_BACKEND_ADS1115
#ifndef ADC_BACKEND
// #define ADC_BACKEND               ADC_BACKEND_NATIVE  // standaard waarde: ADC_BACKEND_NATIVE, pas aan om van backend te wisselen
#endif

#ifndef I2C_ADDRESS_ADS1115
// #define I2C_ADDRESS_ADS1115       0x48                // standaard waarde: 0x48
#endif

// Instelbare vertraging in de busy-wait loop van WachtTotAlleSensorsLosgelatenVoorTest().
// Bij ADC_BACKEND_NATIVE blijft dit 0. Bij ADC_BACKEND_ADS1115 voorkomt dit dat de I2C-bus zonder onderbreking bevraagd wordt.
#ifndef WACHT_LOSLATEN_DELAY_MS
// #define WACHT_LOSLATEN_DELAY_MS 0UL // standaard waarde: 0UL
#endif

// ============================================================================
// INPUT: jouw instellingen
// ============================================================================

#ifndef INPUT_DEBOUNCE_MS
// #define INPUT_DEBOUNCE_MS 25UL                                   // standaard waarde: 25UL
#endif

// Enkel relevant wanneer INPUT_KANAAL_OPVRAGEN_HUIDIGE_TOETSAANSLAG_UITGEBREID aan staat.
#ifndef INPUT_KANAAL_TIMEOUT_BIJ_GEEN_TOETSAANSLAG_BINNEN_MS
// #define INPUT_KANAAL_TIMEOUT_BIJ_GEEN_TOETSAANSLAG_BINNEN_MS 0UL // standaard waarde: 0UL = geen timeout, blijven wachten (huidig gedrag) 
#endif

#ifndef MAX_AANTAL_SIMULTANE_TOETSAANSLAGEN
// #define MAX_AANTAL_SIMULTANE_TOETSAANSLAGEN 1                    // standaard waarde: 1
#endif

#ifndef DEFAULT_TOEGESTANE_MARGE_SIMULTANE_STARTTIJD_MS
// #define DEFAULT_TOEGESTANE_MARGE_SIMULTANE_STARTTIJD_MS    100UL // standaard waarde: 100UL
#endif

// INPUT_KANAAL_CONFIG bepaalt welke invoerkanalen in deze build aanwezig zijn.
// Combineer meerdere kanalen met de bitwise OR-operator |.
//
// Mogelijke waarden:
// 0 = geen invoerkanaal                             (INPUT_TYPE_NONE)
// 1 = rechtstreekse digitale pinnen                 (INPUT_TYPE_DIGITAL)
// 2 = PCF8574 (I2C-poortuitbreiding)                (INPUT_TYPE_PCF8574)
// 4 = HX1838 (IR-afstandsbediening)                 (INPUT_TYPE_HX1838)
// 6 = PCF8574 + HX1838 samen
//
// Wat hier niet geselecteerd wordt, wordt niet gecompileerd. Meerdere selecteren doe je door | tussen jouw keuzes te plaatsen.
// Niet elke combinatie is toegelaten: INPUT_TYPE_DIGITAL kan niet samen met PCF8574 of HX1838 gecombineerd worden.
//
// Standaard gebruikt het elektronische schema INPUT_TYPE_DIGITAL.
#ifndef INPUT_KANAAL_CONFIG
// #define INPUT_KANAAL_CONFIG INPUT_TYPE_DIGITAL // standaard waarde: INPUT_TYPE_DIGITAL
#endif

#ifndef KEYPAD_TYPE
// #define KEYPAD_TYPE KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4 // standaard waarde: KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4 
#endif

// INPUT_TYPE_PCF8574 ---------------------------------------------------------

// PCF8574 (OT8980): 0x20 t.e.m. 0x27 via A0/A1/A2; elk apparaat op dezelfde I2C-bus moet een uniek adres hebben.
#ifndef I2C_ADDRESS_PCF8574
// #define I2C_ADDRESS_PCF8574 0x20 // standaard waarde: 0x20
#endif

#ifndef TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED
// #define TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED_HIGH // standaard waarde: TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED_HIGH (ofwel TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED_LOW)
#endif

// INPUT_TYPE_HX1838 ----------------------------------------------------------
// D8 is de standaard HX1838-ontvangerpin; getest met zowel TinyIRReceiver als IRremote terwijl het PixelScreen aangesloten was.
#ifndef HX1838_ONTVANGER_PIN
// #define HX1838_ONTVANGER_PIN ARDUINO_UNO_SHIELD_PIN_D8 // standaard waarde: ARDUINO_UNO_SHIELD_PIN_D8
#endif

// HX1838_USE_TINYIRRECEIVER_INSTEAD_OF_IRREMOTE: 
// 1 = TinyIRReceiver.hpp (pin-change-interrupt, geen timerkanaal nodig, kleinere flash/RAM-voetafdruk), 
// 0 = klassieke IRremote.hpp/IrReceiver (polling, timer-gebaseerd). 
#ifndef HX1838_USE_TINYIRRECEIVER_INSTEAD_OF_IRREMOTE
//  #define HX1838_USE_TINYIRRECEIVER_INSTEAD_OF_IRREMOTE 1 // standaard waarde: 1
#endif

#ifndef HX1838_TOETSENINDELING
// #define HX1838_TOETSENINDELING HX1838_TOETSENINDELING_REMOTE_OK_BOVENAAN_17_TOETSEN // standaard waarde
// #define HX1838_TOETSENINDELING HX1838_TOETSENINDELING_REMOTE_OK_ONDERAAN_17_TOETSEN
// #define HX1838_TOETSENINDELING HX1838_TOETSENINDELING_REMOTE_21_TOETSEN_MP3
// #define HX1838_TOETSENINDELING HX1838_TOETSENINDELING_REMOTE_USER_DEFINED
#endif

#ifndef HX1838_BRON_CODES
// #define HX1838_BRON_CODES HX1838_BRON_CODES_DEFINE // standaard waarde
// #define HX1838_BRON_CODES HX1838_BRON_CODES_EEPROM_ALTIJD
// #define HX1838_BRON_CODES HX1838_BRON_CODES_EEPROM_WANNEER_GEEN_DEFINE
#endif

#ifndef HX1838_KALIBRATIE_TOETS_PAUZE_MS
// #define HX1838_KALIBRATIE_TOETS_PAUZE_MS 300UL // standaard waarde: 300UL
#endif

#ifndef HX1838_KALIBRATIE_TIMEOUT_MS
// #define HX1838_KALIBRATIE_TIMEOUT_MS   30000UL // standaard waarde: 30000UL
#endif

// Uitgebreide gebeurtenissen (loslaten, lang indrukken, timeout bij geen invoer).
// Standaard UIT, om geheugen te sparen op geheugenarme boards (bv. AVR Uno R3).
// Enkel van toepassing bij OpvragenHuidigeToetsAanslag(false), niet bij wachten=true.
// #define INPUT_KANAAL_OPVRAGEN_HUIDIGE_TOETSAANSLAG_UITGEBREID

// Schakelt ControleerMappingVolledigheid() in. 
// Enkel bedoeld voor tijdens het testen: meldt via Serial welke opschriften van het gecompileerde KEYPAD_TYPE/HX1838_TOETSENINDELING ontbreken in een mapping die je zelf als argument meegeeft. 
// Kost extra flashgeheugen en Serial-afhankelijkheid, standaard uit. Niet aanraden om aan te laten in productiecode.
// #define INPUT_MAPPING_EXTRA_CONTROLES_INSCHAKELEN

// ============================================================================
// OUTPUT: jouw instellingen
// ============================================================================

// FATAL
#ifndef FATAL_LEESTIJD_MS
// #define FATAL_LEESTIJD_MS   30000UL // standaard waarde: 30000UL
#endif

// SCREEN_OUTPUT_CONFIG bepaalt welke schermuitvoertypes in deze build aanwezig zijn.
// Combineer meerdere uitvoertypes met de bitwise OR-operator |.
//
// Mogelijke waarden:
// 0 = geen uitvoer                                 (SCREEN_TYPE_NONE)
// 1 = Serial                                       (SCREEN_TYPE_SERIAL)
// 2 = CharacterScreen                              (SCREEN_TYPE_CHARACTER)
// 3 = Serial + CharacterScreen
// 4 = PixelScreen                                  (SCREEN_TYPE_PIXELS)
// 5 = Serial + PixelScreen
// 6 = CharacterScreen + PixelScreen
// 7 = Serial + CharacterScreen + PixelScreen
//
// Wat hier niet geselecteerd wordt, wordt niet gecompileerd. Meerdere selecteren doe je door | tussen jouw keuzes te plaatsen.
//
// Opmerking: Wanneer DEBUG actief is, voegt de library automatisch SCREEN_TYPE_SERIAL toe. Je hoeft SCREEN_TYPE_SERIAL dus niet zelf te selecteren voor debug-uitvoer.
//
// Kies hieronder welke schermuitvoer je wilt gebruiken; staat standaard op: geen uitvoer.
#ifndef SCREEN_OUTPUT_CONFIG
// #define SCREEN_OUTPUT_CONFIG SCREEN_TYPE_NONE // standaard waarde: SCREEN_TYPE_NONE 
#endif

// Characterscherm ----------------------------------------------------------------
#ifndef I2C_ADDRESS_CHARACTER_SCREEN
// #define I2C_ADDRESS_CHARACTER_SCREEN  0x27 // standaard waarde: 0x27
#endif


#ifndef ACTIEF_CHARACTER_SCREEN
// #define ACTIEF_CHARACTER_SCREEN        SCREEN_LCD1602 // standaard waarde: SCREEN_LCD1602
#endif

// Pixelscherm ----------------------------------------------------------------
#ifndef PIXEL_SCREEN_CS
// #define PIXEL_SCREEN_CS                HardwareResourcePin::SS // standaard waarde: HardwareResourcePin::SS
#endif
#ifndef PIXEL_SCREEN_DC
// #define PIXEL_SCREEN_DC                HardwareResourcePin::D9 // standaard waarde: HardwareResourcePin::D9
#endif
#ifndef PIXEL_SCREEN_RST
// #define PIXEL_SCREEN_RST               HardwareResourcePin::D7 // standaard waarde: HardwareResourcePin::D7
#endif

#ifndef ACTIEF_PIXEL_SCREEN
// #define ACTIEF_PIXEL_SCREEN            SCREEN_240X320           // standaard waarde: SCREEN_240X320
#endif

// Standaard rotatiewaarden (met de klok mee):
// 0:   0 graden: Portretmodus (standaard, met de kabels vaak aan de onder- of bovenkant).
// 1:  90 graden: Landschapsmodus (horizontaal).
// 2: 180 graden: Portretmodus op zijn kop.
// 3: 270 graden: Landschapsmodus op zijn kop.
// Tip: Bij sommige displays (zoals de ST7789 zonder CS-pin) zorgen de waarden 4, 5, 6 of 7 voor een gespiegeld beeld, wat handig kan zijn voor specifieke projecten.
#ifndef PIXEL_SCREEN_ROTATION
// #define PIXEL_SCREEN_ROTATION          3       // standaard waarde: 3
#endif
#ifndef PIXEL_SCREEN_TEXT_SIZE
// #define PIXEL_SCREEN_TEXT_SIZE         3       // standaard waarde: 3
#endif
#ifndef PIXEL_SCREEN_MARGIN
// #define PIXEL_SCREEN_MARGIN            5       // standaard waarde: 5
#endif
#ifndef PIXEL_SCREEN_CHARACTER_SPACING
// #define PIXEL_SCREEN_CHARACTER_SPACING 1       // standaard waarde: 1
#endif
#ifndef PIXEL_SCREEN_LINE_SPACING
// #define PIXEL_SCREEN_LINE_SPACING      5       // standaard waarde: 5
#endif

#ifndef PIXEL_SCREEN_TEXT_COLOR
// #define PIXEL_SCREEN_TEXT_COLOR        0xFFFF  // standaard waarde: 0xFFFF
#endif
#ifndef PIXEL_SCREEN_BACKGROUND_COLOR
// #define PIXEL_SCREEN_BACKGROUND_COLOR  0x0000  // standaard waarde: 0x0000
#endif

// Kleuren per ScreenData-type, enkel gebruikt door een zelfgeschreven PixelScreen-callback (zie examples/Systeem/Screen/Callback_PixelScreen.ino). RGB565, driveronafhankelijk.
#ifndef PIXEL_SCREEN_KLEUR_FATAL
// #define PIXEL_SCREEN_KLEUR_FATAL       0xF800   // standaard waarde: 0xF800, rood
#endif
#ifndef PIXEL_SCREEN_KLEUR_FAULT
// #define PIXEL_SCREEN_KLEUR_FAULT       0xFC00   // standaard waarde: 0xFC00, oranje
#endif
#ifndef PIXEL_SCREEN_KLEUR_WARNING
// #define PIXEL_SCREEN_KLEUR_WARNING     0xFFE0   // standaard waarde: 0xFFE0, geel
#endif
#ifndef PIXEL_SCREEN_KLEUR_INFO
// #define PIXEL_SCREEN_KLEUR_INFO        0x07FF   // standaard waarde: 0x07FF, cyaan
#endif
#ifndef PIXEL_SCREEN_KLEUR_CRITICAL
// #define PIXEL_SCREEN_KLEUR_CRITICAL    0xF81F   // standaard waarde: 0xF81F, magenta, nog ernstiger dan ERROR, onderscheiden van FATAL-rood
#endif
#ifndef PIXEL_SCREEN_KLEUR_ABORT
// #define PIXEL_SCREEN_KLEUR_ABORT       0x780F   // standaard waarde: 0x780F, paars
#endif
#ifndef PIXEL_SCREEN_KLEUR_PANIC
// #define PIXEL_SCREEN_KLEUR_PANIC       0xFFFF   // standaard waarde: 0xFFFF, wit, springt er maximaal uit op een donkere achtergrond
#endif

// SerialScreen ---------------------------------------------------------------
#ifndef SERIAL_BAUDRATE
// #define SERIAL_BAUDRATE              115200UL   // standaard waarde: 115200UL
#endif

#ifndef SERIAL_CONNECT_TIMEOUT_MS
// #define SERIAL_CONNECT_TIMEOUT_MS      2000UL   // standaard waarde: 2000UL
#endif

// #define USE_SERIAL_CONNECT_TIMEOUT_MS // ENKEL wanneer we SERIAL_CONNECT_TIMEOUT_MS wensen te gebruiken in plaats van oneindig wachten!

// ****************************************************************************
// UITGEBREIDE INSTELLINGEN
// ****************************************************************************

// ============================================================================
// BOARD_VERSION EN ARDUINO UNO-SHIELDPINNEN
// ============================================================================

// Gebruik GPIOxx-labels in plaats van kale cijfers, zodat de mapping controleerbaar blijft.
// Laat deze blokken normaal uitgeschakeld: SystemConfig.h bevat de standaardmappingen.
#if BOARD_VERSION == BOARD_UNO_R3 || BOARD_VERSION == BOARD_UNO_R4_MINIMA || BOARD_VERSION == BOARD_UNO_R4_WIFI || BOARD_VERSION == BOARD_UNO_Q
  #if BOARD_VERSION == BOARD_UNO_R3
    // #define ARDUINO_UNO_SHIELD_PIN_D0   0    // 0  / RX   / PD0 /
    // #define ARDUINO_UNO_SHIELD_PIN_D1   1    // 1  / TX   / PD1 /
    // #define ARDUINO_UNO_SHIELD_PIN_D2   2    // 2  / INT0 / PD2 /
    // #define ARDUINO_UNO_SHIELD_PIN_D3   3    // 3  / INT1 / PD3 / PWM
    // #define ARDUINO_UNO_SHIELD_PIN_D4   4    // 4  /      / PD4 /
    // #define ARDUINO_UNO_SHIELD_PIN_D5   5    // 5  /      / PD5 / PWM
    // #define ARDUINO_UNO_SHIELD_PIN_D6   6    // 6  /      / PD6 / PWM
    // #define ARDUINO_UNO_SHIELD_PIN_D7   7    // 7  /      / PD7 /
    // #define ARDUINO_UNO_SHIELD_PIN_D8   8    // 8  /      / PB0 /
    // #define ARDUINO_UNO_SHIELD_PIN_D9   9    // 9  /      / PB1 / PWM
    // #define ARDUINO_UNO_SHIELD_PIN_D10  10   // 10 / SS   / PB2 /
    // #define ARDUINO_UNO_SHIELD_PIN_D11  11   // 11 / MOSI / PB3 / PWM
    // #define ARDUINO_UNO_SHIELD_PIN_D12  12   // 12 / MISO / PB4 /
    // #define ARDUINO_UNO_SHIELD_PIN_D13  13   // 13 / SCK  / PB5 /

  #elif BOARD_VERSION == BOARD_UNO_R4_MINIMA || BOARD_VERSION == BOARD_UNO_R4_WIFI || BOARD_VERSION == BOARD_UNO_Q
    // #define ARDUINO_UNO_SHIELD_PIN_D0   D0   // D0  / RX   / PD0 /
    // #define ARDUINO_UNO_SHIELD_PIN_D1   D1   // D1  / TX   / PD1 /
    // #define ARDUINO_UNO_SHIELD_PIN_D2   D2   // D2  / INT0 / PD2 /
    // #define ARDUINO_UNO_SHIELD_PIN_D3   D3   // D3  / INT1 / PD3 / PWM
    // #define ARDUINO_UNO_SHIELD_PIN_D4   D4   // D4  /      / PD4 /
    // #define ARDUINO_UNO_SHIELD_PIN_D5   D5   // D5  /      / PD5 / PWM
    // #define ARDUINO_UNO_SHIELD_PIN_D6   D6   // D6  /      / PD6 / PWM
    // #define ARDUINO_UNO_SHIELD_PIN_D7   D7   // D7  /      / PD7 /
    // #define ARDUINO_UNO_SHIELD_PIN_D8   D8   // D8  /      / PB0 /
    // #define ARDUINO_UNO_SHIELD_PIN_D9   D9   // D9  /      / PB1 / PWM
    // #define ARDUINO_UNO_SHIELD_PIN_D10  D10  // D10 / SS   / PB2 / PWM
    // #define ARDUINO_UNO_SHIELD_PIN_D11  D11  // D11 / MOSI / PB3 / PWM
    // #define ARDUINO_UNO_SHIELD_PIN_D12  D12  // D12 / MISO / PB4 /
    // #define ARDUINO_UNO_SHIELD_PIN_D13  D13  // D13 / SCK  / PB5 /
  #endif

  // #define ARDUINO_UNO_SHIELD_PIN_A0   A0   // A0  /      / PC0
  // #define ARDUINO_UNO_SHIELD_PIN_A1   A1   // A1  /      / PC1
  // #define ARDUINO_UNO_SHIELD_PIN_A2   A2   // A2  /      / PC2
  // #define ARDUINO_UNO_SHIELD_PIN_A3   A3   // A3  /      / PC3
  // #define ARDUINO_UNO_SHIELD_PIN_A4   A4   // A4  / SDA  / PC4 
  // #define ARDUINO_UNO_SHIELD_PIN_A5   A5   // A5  / SCL  / PC5

  // #define ARDUINO_UNO_SHIELD_PIN_SDA  SDA  // SDA / A4   / PC4 
  // #define ARDUINO_UNO_SHIELD_PIN_SCL  SCL  // SCL / A5   / PC5

  #if BOARD_VERSION == BOARD_UNO_Q // Voor de Uno Q (Zephyr) gebruiken we de veilige Dxx pinlabels om de kale cijfers te omzeilen.
    // #define ARDUINO_UNO_SHIELD_PIN_SS   D10
    // #define ARDUINO_UNO_SHIELD_PIN_MOSI D11
    // #define ARDUINO_UNO_SHIELD_PIN_MISO D12
    // #define ARDUINO_UNO_SHIELD_PIN_SCK  D13
  #else // Standaard fallback voor Uno R3, R4 Minima en R4 WiFi
    // #define ARDUINO_UNO_SHIELD_PIN_SS   SS
    // #define ARDUINO_UNO_SHIELD_PIN_MOSI MOSI
    // #define ARDUINO_UNO_SHIELD_PIN_MISO MISO
    // #define ARDUINO_UNO_SHIELD_PIN_SCK  SCK
  #endif

#elif BOARD_VERSION == BOARD_ESP32_D1_UNO_R32 // WEMOS D1 R32 & TTGO D1 R32: pinmapping bevestigd tegen espboards.dev/esp32/d1-uno32/
  // #define ARDUINO_UNO_SHIELD_PIN_D0   GPIO3   // D0  /RX   /PWM / GPIO3  (UART RX)
  // #define ARDUINO_UNO_SHIELD_PIN_D1   GPIO1   // D1  /TX   /PWM / GPIO1  (UART TX)
  // #define ARDUINO_UNO_SHIELD_PIN_D2   GPIO26  // D2  /     /PWM / GPIO26 (vrij, geen bijzondere rol)
  // #define ARDUINO_UNO_SHIELD_PIN_D3   GPIO25  // D3  /     /PWM / GPIO25 (vrij, geen bijzondere rol)
  // #define ARDUINO_UNO_SHIELD_PIN_D4   GPIO17  // D4  /     /PWM / GPIO17 (gedeeld met PSRAM op PSRAM-modules)
  // #define ARDUINO_UNO_SHIELD_PIN_D5   GPIO16  // D5  /     /PWM / GPIO16 (gedeeld met PSRAM op PSRAM-modules)
  // #define ARDUINO_UNO_SHIELD_PIN_D6   GPIO27  // D6  /     /PWM / GPIO27 (vrij, geen bijzondere rol)
  // #define ARDUINO_UNO_SHIELD_PIN_D7   GPIO14  // D7  /     /PWM / GPIO14 (JTAG TMS)
  // #define ARDUINO_UNO_SHIELD_PIN_D8   GPIO12  // D8  /     /PWM / GPIO12 (boot-strapping, MTDI)
  // #define ARDUINO_UNO_SHIELD_PIN_D9   GPIO13  // D9  /     /PWM / GPIO13 (JTAG TCK)

  // #define ARDUINO_UNO_SHIELD_PIN_D10  GPIO5   // D10 /SS   /PWM / GPIO5  (SPI, boot-strapping)
  // #define ARDUINO_UNO_SHIELD_PIN_D11  GPIO23  // D11 /MOSI /PWM / GPIO23 (SPI, vrij)
  // #define ARDUINO_UNO_SHIELD_PIN_D12  GPIO19  // D12 /MISO /PWM / GPIO19 (SPI, vrij)
  // #define ARDUINO_UNO_SHIELD_PIN_D13  GPIO18  // D13 /SCK  /PWM / GPIO18 (SPI, vrij, ook onboard-LED)

  // #define ARDUINO_UNO_SHIELD_PIN_A0   GPIO2   // A0  /LED  /PWM / GPIO2  (ADC, boot-strapping, ook LED_BUILTIN)
  // #define ARDUINO_UNO_SHIELD_PIN_A1   GPIO4   // A1  /     /PWM / GPIO4  (ADC, boot-strapping)
  // #define ARDUINO_UNO_SHIELD_PIN_A2   GPIO35  // A2  /          / GPIO35 (ADC, enkel input)
  // #define ARDUINO_UNO_SHIELD_PIN_A3   GPIO34  // A3  /          / GPIO34 (ADC, enkel input)
  // #define ARDUINO_UNO_SHIELD_PIN_A4   GPIO36  // A4  /          / GPIO36 (ADC, enkel input)
  // #define ARDUINO_UNO_SHIELD_PIN_A5   GPIO39  // A5  /          / GPIO39 (ADC, enkel input)

  // #define ARDUINO_UNO_SHIELD_PIN_SDA  GPIO21  // SDA /     /PWM / GPIO21 (I2C, vrij)
  // #define ARDUINO_UNO_SHIELD_PIN_SCL  GPIO22  // SCL /     /PWM / GPIO22 (I2C, vrij)

  // #define ARDUINO_UNO_SHIELD_PIN_SS   GPIO5   // ARDUINO_UNO_SHIELD_PIN_D10
  // #define ARDUINO_UNO_SHIELD_PIN_MOSI GPIO23  // ARDUINO_UNO_SHIELD_PIN_D11
  // #define ARDUINO_UNO_SHIELD_PIN_MISO GPIO19  // ARDUINO_UNO_SHIELD_PIN_D12
  // #define ARDUINO_UNO_SHIELD_PIN_SCK  GPIO18  // ARDUINO_UNO_SHIELD_PIN_D13

#elif BOARD_VERSION == BOARD_ESP32S3_ARDI32 // ESP32-S3-WROOM-1 - ARDI-32: pinmapping bevestigd tegen https://github.com/sbcshop/Ardi-32_Software
  // #define ARDUINO_UNO_SHIELD_PIN_D0   GPIO18  // D0  / RX      / GPIO18
  // #define ARDUINO_UNO_SHIELD_PIN_D1   GPIO17  // D1  / TX      / GPIO17
  // #define ARDUINO_UNO_SHIELD_PIN_D2   GPIO1   // D2  / CS_2    / GPIO1
  // #define ARDUINO_UNO_SHIELD_PIN_D3   GPIO42  // D3  / CLK_2   / GPIO42
  // #define ARDUINO_UNO_SHIELD_PIN_D4   GPIO41  // D4  / MISO_2  / GPIO41
  // #define ARDUINO_UNO_SHIELD_PIN_D5   GPIO2   // D5  / MOSI_2  / GPIO2
  // #define ARDUINO_UNO_SHIELD_PIN_D6   GPIO9   // D6  / GPIO9   / GPIO9
  // #define ARDUINO_UNO_SHIELD_PIN_D7   GPIO14  // D7  / GPIO14  / GPIO14
  // #define ARDUINO_UNO_SHIELD_PIN_D8   GPIO47  // D8  / GPIO47  / GPIO47
  // #define ARDUINO_UNO_SHIELD_PIN_D9   GPIO21  // D9  / GPIO21  / GPIO21

  // #define ARDUINO_UNO_SHIELD_PIN_D10  GPIO10  // D10 / CS_1    / GPIO10
  // #define ARDUINO_UNO_SHIELD_PIN_D11  GPIO11  // D11 / MOSI_1  / GPIO11
  // #define ARDUINO_UNO_SHIELD_PIN_D12  GPIO13  // D12 / MISO_1  / GPIO13
  // #define ARDUINO_UNO_SHIELD_PIN_D13  GPIO12  // D13 / CLK_1   / GPIO12

  // #define ARDUINO_UNO_SHIELD_PIN_A0   GPIO4   // A0  / A0      / GPIO4
  // #define ARDUINO_UNO_SHIELD_PIN_A1   GPIO5   // A1  / A3      / GPIO5
  // #define ARDUINO_UNO_SHIELD_PIN_A2   GPIO6   // A2  / A6      / GPIO6
  // #define ARDUINO_UNO_SHIELD_PIN_A3   GPIO7   // A3  / A7      / GPIO7
  // #define ARDUINO_UNO_SHIELD_PIN_A4   GPIO15  // A4  / A4      / GPIO15
  // #define ARDUINO_UNO_SHIELD_PIN_A5   GPIO16  // A5  / A5      / GPIO16

  // #define ARDUINO_UNO_SHIELD_PIN_SDA  GPIO38  // SDA / SDA     / GPIO38
  // #define ARDUINO_UNO_SHIELD_PIN_SCL  GPIO39  // SCL / SCL     / GPIO39

  // #define ARDUINO_UNO_SHIELD_PIN_SS   GPIO10  // ARDUINO_UNO_SHIELD_PIN_D10 (CS_1)
  // #define ARDUINO_UNO_SHIELD_PIN_MOSI GPIO11  // ARDUINO_UNO_SHIELD_PIN_D11 (MOSI_1)
  // #define ARDUINO_UNO_SHIELD_PIN_MISO GPIO13  // ARDUINO_UNO_SHIELD_PIN_D12 (MISO_1)
  // #define ARDUINO_UNO_SHIELD_PIN_SCK  GPIO12  // ARDUINO_UNO_SHIELD_PIN_D13 (CLK_1)

  /* TODO: UIT TE WERKEN OPTIE?
  #if BOARD_VERSION == BOARD_ESP32S3_ARDI32
  // #define ARDI32_ONBOARD_SD_CS        GPIO1   // SD Card Chip Select
  // #define ARDI32_ONBOARD_SD_MOSI      GPIO2   // SD Card MOSI
  // #define ARDI32_ONBOARD_SD_MISO      GPIO41  // SD Card MISO
  // #define ARDI32_ONBOARD_SD_SCK       GPIO42  // SD Card Clock
  // #define ARDI32_ONBOARD_BUZZER       GPIO40  // Onboard PWM Buzzer
  // #define ARDI32_ONBOARD_LED          GPIO8   // Onboard Status LED
  endif
  */

#elif BOARD_VERSION == BOARD_ESP32S3_DEV // ESP32-S3-WROOM-1-S3-N16R8 (https://en.paradisetronic.com/)
  // #define ARDUINO_UNO_SHIELD_PIN_D0   GPIO44  // D0  / RX      / GPIO44
  // #define ARDUINO_UNO_SHIELD_PIN_D1   GPIO43  // D1  / TX      / GPIO43
  // #define ARDUINO_UNO_SHIELD_PIN_D2   GPIO18  // D2  /         / GPIO18
  // #define ARDUINO_UNO_SHIELD_PIN_D3   GPIO17  // D3  / PWM     / GPIO17
  // #define ARDUINO_UNO_SHIELD_PIN_D4   GPIO19  // D4  /         / GPIO19
  // #define ARDUINO_UNO_SHIELD_PIN_D5   GPIO20  // D5  / PWM     / GPIO20
  // #define ARDUINO_UNO_SHIELD_PIN_D6   GPIO3   // D6  / PWM     / GPIO3
  // #define ARDUINO_UNO_SHIELD_PIN_D7   GPIO14  // D7  /         / GPIO14
  // #define ARDUINO_UNO_SHIELD_PIN_D8   GPIO21  // D8  /         / GPIO21
  // #define ARDUINO_UNO_SHIELD_PIN_D9   GPIO46  // D9  /         / GPIO46

  // #define ARDUINO_UNO_SHIELD_PIN_D10  GPIO10  // D10 / CS      / GPIO10
  // #define ARDUINO_UNO_SHIELD_PIN_D11  GPIO11  // D11 / MOSI    / GPIO11
  // #define ARDUINO_UNO_SHIELD_PIN_D12  GPIO13  // D12 / MISO    / GPIO13
  // #define ARDUINO_UNO_SHIELD_PIN_D13  GPIO12  // D13 / CLK     / GPIO12

  // #define ARDUINO_UNO_SHIELD_PIN_A0   GPIO2   // A0  / A0      / GPIO2
  // #define ARDUINO_UNO_SHIELD_PIN_A1   GPIO1   // A1  / A1      / GPIO1
  // #define ARDUINO_UNO_SHIELD_PIN_A2   GPIO7   // A2  / A2      / GPIO7
  // #define ARDUINO_UNO_SHIELD_PIN_A3   GPIO6   // A3  / A3      / GPIO6
  // #define ARDUINO_UNO_SHIELD_PIN_A4   GPIO5   // A4  / A4      / GPIO5
  // #define ARDUINO_UNO_SHIELD_PIN_A5   GPIO4   // A5  / A5      / GPIO4

  // #define ARDUINO_UNO_SHIELD_PIN_SDA  GPIO8   // SDA / SDA     / GPIO8
  // #define ARDUINO_UNO_SHIELD_PIN_SCL  GPIO9   // SCL / SCL     / GPIO9

  // #define ARDUINO_UNO_SHIELD_PIN_SS   GPIO10  // ARDUINO_UNO_SHIELD_PIN_D10
  // #define ARDUINO_UNO_SHIELD_PIN_MOSI GPIO11  // ARDUINO_UNO_SHIELD_PIN_D11
  // #define ARDUINO_UNO_SHIELD_PIN_MISO GPIO13  // ARDUINO_UNO_SHIELD_PIN_D12 
  // #define ARDUINO_UNO_SHIELD_PIN_SCK  GPIO12  // ARDUINO_UNO_SHIELD_PIN_D13 

#elif BOARD_VERSION == BOARD_RP2040_CYTRON_MAKER_UNO // CYTRON MAKER UNO RP2040: pinmapping bevestigd tegen officiële Zephyr-boarddocumentatie (https://docs.zephyrproject.org/latest/boards/cytron/maker_uno_rp2040/doc/index.html), bevestigd tegen eigen meting op het board
  // #define ARDUINO_UNO_SHIELD_PIN_D0   GPIO1   // D0  / GP1  / GPIO1  (UART0 RX)
  // #define ARDUINO_UNO_SHIELD_PIN_D1   GPIO0   // D1  / GP0  / GPIO0  (UART0 TX)
  // #define ARDUINO_UNO_SHIELD_PIN_D2   GPIO2   // D2  / GP2  / GPIO2  (User button, pull-up)
  // #define ARDUINO_UNO_SHIELD_PIN_D3   GPIO3   // D3  / GP3  / GPIO3  (Alias led0)
  // #define ARDUINO_UNO_SHIELD_PIN_D4   GPIO4   // D4  / GP4  / GPIO4  (ook Grove 2)
  // #define ARDUINO_UNO_SHIELD_PIN_D5   GPIO5   // D5  / GP5  / GPIO5  (ook Grove 2)
  // #define ARDUINO_UNO_SHIELD_PIN_D6   GPIO6   // D6  / GP6  / GPIO6  (ook Grove 3)
  // #define ARDUINO_UNO_SHIELD_PIN_D7   GPIO7   // D7  / GP7  / GPIO7  (ook Grove 4)
  // #define ARDUINO_UNO_SHIELD_PIN_D8   GPIO8   // D8  / GP8  / GPIO8  (Buzzer)
  // #define ARDUINO_UNO_SHIELD_PIN_D9   GPIO9   // D9  / GP9  / GPIO9  (geen andere toewijzing)

  // #define ARDUINO_UNO_SHIELD_PIN_D10  GPIO13  // D10 / GP13 / GPIO13 (SPI1 CS,   SPI-header)
  // #define ARDUINO_UNO_SHIELD_PIN_D11  GPIO11  // D11 / GP11 / GPIO11 (SPI1 MOSI, SPI-header)
  // #define ARDUINO_UNO_SHIELD_PIN_D12  GPIO12  // D12 / GP12 / GPIO12 (SPI1 MISO, SPI-header)
  // #define ARDUINO_UNO_SHIELD_PIN_D13  GPIO10  // D13 / GP10 / GPIO13 (SPI1 SCK,  SPI-header)

  // #define ARDUINO_UNO_SHIELD_PIN_A0   GPIO26  // A0  / GP26 / GPIO26 (ADC0, ook Grove 3)
  // #define ARDUINO_UNO_SHIELD_PIN_A1   GPIO27  // A1  / GP27 / GPIO27 (ADC1, ook Grove 4)
  // #define ARDUINO_UNO_SHIELD_PIN_A2   GPIO28  // A2  / GP28 / GPIO28 (ADC2, ook Grove 5)
  // #define ARDUINO_UNO_SHIELD_PIN_A3   GPIO29  // A3  / GP29 / GPIO29 (ADC3, ook Grove 5)

  // #define ARDUINO_UNO_SHIELD_PIN_SDA  GPIO20  // SDA / GP20 / GPIO20 (I2C0 SDA, ook Grove 6/Maker)
  // #define ARDUINO_UNO_SHIELD_PIN_SCL  GPIO21  // SCL / GP21 / GPIO21 (I2C0 SCL, ook Grove 6/Maker)

  // #define ARDUINO_UNO_SHIELD_PIN_SS   GPIO13  // ARDUINO_UNO_SHIELD_PIN_D10
  // #define ARDUINO_UNO_SHIELD_PIN_MOSI GPIO11  // ARDUINO_UNO_SHIELD_PIN_D11
  // #define ARDUINO_UNO_SHIELD_PIN_MISO GPIO12  // ARDUINO_UNO_SHIELD_PIN_D12
  // #define ARDUINO_UNO_SHIELD_PIN_SCK  GPIO10  // ARDUINO_UNO_SHIELD_PIN_D13

#elif BOARD_VERSION == BOARD_STM32F4_NUCLEO64_F401RE // STMicroelectronics STM32F4 NUCLEO64 F401RE
  // #define ARDUINO_UNO_SHIELD_PIN_D0   D0   // D0  / RX   / PA3
  // #define ARDUINO_UNO_SHIELD_PIN_D1   D1   // D1  / TX   / PA2
  // #define ARDUINO_UNO_SHIELD_PIN_D2   D2   // D2  /      / PA10
  // #define ARDUINO_UNO_SHIELD_PIN_D3   D3   // D3  / PWM  / PB3
  // #define ARDUINO_UNO_SHIELD_PIN_D4   D4   // D4  /      / PB5
  // #define ARDUINO_UNO_SHIELD_PIN_D5   D5   // D5  / PWM  / PB4
  // #define ARDUINO_UNO_SHIELD_PIN_D6   D6   // D6  / PWM  / PB10
  // #define ARDUINO_UNO_SHIELD_PIN_D7   D7   // D7  /      / PA8
  // #define ARDUINO_UNO_SHIELD_PIN_D8   D8   // D8  /      / PA9
  // #define ARDUINO_UNO_SHIELD_PIN_D9   D9   // D9  / PWM  / PC7

  // #define ARDUINO_UNO_SHIELD_PIN_D10  D10  // D10 / CS   / PB6 of PA6 (depending on config: PB6 default)
  // #define ARDUINO_UNO_SHIELD_PIN_D11  D11  // D11 / MOSI / PA7
  // #define ARDUINO_UNO_SHIELD_PIN_D12  D12  // D12 / MISO / PA6
  // #define ARDUINO_UNO_SHIELD_PIN_D13  D13  // D13 / SCK  / PA5

  // #define ARDUINO_UNO_SHIELD_PIN_A0   A0   // A0  /      / PA0
  // #define ARDUINO_UNO_SHIELD_PIN_A1   A1   // A1  /      / PA1
  // #define ARDUINO_UNO_SHIELD_PIN_A2   A2   // A2  /      / PA4
  // #define ARDUINO_UNO_SHIELD_PIN_A3   A3   // A3  /      / PB0
  // #define ARDUINO_UNO_SHIELD_PIN_A4   A4   // A4  /      / PC1
  // #define ARDUINO_UNO_SHIELD_PIN_A5   A5   // A5  /      / PC0

  // #define ARDUINO_UNO_SHIELD_PIN_SDA  SDA  // D14 / SDA  / PB9
  // #define ARDUINO_UNO_SHIELD_PIN_SCL  SCL  // D15 / SCL  / PB8

  // #define ARDUINO_UNO_SHIELD_PIN_SS   PIN_SPI_SS   // ARDUINO_UNO_SHIELD_PIN_D10
  // #define ARDUINO_UNO_SHIELD_PIN_MOSI PIN_SPI_MOSI // ARDUINO_UNO_SHIELD_PIN_D11
  // #define ARDUINO_UNO_SHIELD_PIN_MISO PIN_SPI_MISO // ARDUINO_UNO_SHIELD_PIN_D12
  // #define ARDUINO_UNO_SHIELD_PIN_SCK  PIN_SPI_SCK  // ARDUINO_UNO_SHIELD_PIN_D13
#endif

// ============================================================================
// EXTENDERs
// ============================================================================
// Extender-resources:
// - DEFAULT: EXTENDER_<NAAM>_AANTAL = 0 of 1.
// - EXPERIMENTEEL: EXTENDER_<NAAM>_AANTAL >= 2.
// - I2C SDA/SCL en SPI SCK/MISO/MOSI zijn GEDEELD en komen uit de boardconfiguratie.
// - I2C-adressen, SPI-CS en de extra native extenderaansluitingen hieronder zijn EXCLUSIEF.
// - HardwareResourcePin::NONE betekent dat die extra aansluiting niet gebruikt/aangesloten is.
// - Kies voor een EXCLUSIEVE HardwareResourcePin nooit een pin die al exclusief geclaimd is.
#ifndef EXTENDER_ADS1115_AANTAL
// #define EXTENDER_ADS1115_AANTAL 0  // standaard waarde: 0
#endif
// ============================================================================
// DEFAULT — ADS1115 #1
// ============================================================================
#ifndef I2C_ADDRESS_EXTENDER_ADS1115_1
// #define I2C_ADDRESS_EXTENDER_ADS1115_1 0x48  // standaard: 0x48 bij ADC_BACKEND_NATIVE; bij ADC_BACKEND_ADS1115 volgt deze na I2C_ADDRESS_ADS1115. // EXCLUSIEF
#endif
#ifndef EXTENDER_ADS1115_1_ALERT_RDY
// #define EXTENDER_ADS1115_1_ALERT_RDY HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
// ============================================================================
// EXPERIMENTEEL — ADS1115 #2
// ============================================================================
#ifndef I2C_ADDRESS_EXTENDER_ADS1115_2
// #define I2C_ADDRESS_EXTENDER_ADS1115_2 0x49  // standaard: 0x49 bij ADC_BACKEND_NATIVE; bij ADC_BACKEND_ADS1115 volgt deze als tweede extender na I2C_ADDRESS_ADS1115. // EXCLUSIEF
#endif
#ifndef EXTENDER_ADS1115_2_ALERT_RDY
// #define EXTENDER_ADS1115_2_ALERT_RDY HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif

#ifndef EXTENDER_MCP23017_AANTAL
// #define EXTENDER_MCP23017_AANTAL 0  // standaard waarde: 0
#endif
// ============================================================================
// DEFAULT — MCP23017 #1
// ============================================================================
#ifndef I2C_ADDRESS_EXTENDER_MCP23017_1
// #define I2C_ADDRESS_EXTENDER_MCP23017_1 0x20  // standaard waarde: 0x20  // EXCLUSIEF
#endif
#ifndef EXTENDER_MCP23017_1_INTA
// #define EXTENDER_MCP23017_1_INTA HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
#ifndef EXTENDER_MCP23017_1_INTB
// #define EXTENDER_MCP23017_1_INTB HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
#ifndef EXTENDER_MCP23017_1_RESET
// #define EXTENDER_MCP23017_1_RESET HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
// ============================================================================
// EXPERIMENTEEL — MCP23017 #2
// ============================================================================
#ifndef I2C_ADDRESS_EXTENDER_MCP23017_2
// #define I2C_ADDRESS_EXTENDER_MCP23017_2 0x21  // standaard waarde: 0x21  // EXCLUSIEF
#endif
#ifndef EXTENDER_MCP23017_2_INTA
// #define EXTENDER_MCP23017_2_INTA HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
#ifndef EXTENDER_MCP23017_2_INTB
// #define EXTENDER_MCP23017_2_INTB HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
#ifndef EXTENDER_MCP23017_2_RESET
// #define EXTENDER_MCP23017_2_RESET HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif

#ifndef EXTENDER_PCF8574_AANTAL
// #define EXTENDER_PCF8574_AANTAL 0  // standaard waarde: 0
#endif
// ============================================================================
// DEFAULT — PCF8574 #1
// ============================================================================
#ifndef I2C_ADDRESS_EXTENDER_PCF8574_1
// #define I2C_ADDRESS_EXTENDER_PCF8574_1 0x20  // standaard: 0x20; bij INPUT_TYPE_PCF8574 het volgende vrije adres na I2C_ADDRESS_PCF8574  // EXCLUSIEF
#endif
#ifndef EXTENDER_PCF8574_1_INT
// #define EXTENDER_PCF8574_1_INT HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
// ============================================================================
// EXPERIMENTEEL — PCF8574 #2
// ============================================================================
#ifndef I2C_ADDRESS_EXTENDER_PCF8574_2
// #define I2C_ADDRESS_EXTENDER_PCF8574_2 0x21  // standaard: 0x21; bij INPUT_TYPE_PCF8574 het tweede vrije adres na I2C_ADDRESS_PCF8574  // EXCLUSIEF
#endif
#ifndef EXTENDER_PCF8574_2_INT
// #define EXTENDER_PCF8574_2_INT HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif

#ifndef EXTENDER_PCF8575_AANTAL
// #define EXTENDER_PCF8575_AANTAL 0  // standaard waarde: 0
#endif
// ============================================================================
// DEFAULT — PCF8575 #1
// ============================================================================
#ifndef I2C_ADDRESS_EXTENDER_PCF8575_1
// #define I2C_ADDRESS_EXTENDER_PCF8575_1 0x20  // standaard waarde: 0x20  // EXCLUSIEF
#endif
#ifndef EXTENDER_PCF8575_1_INT
// #define EXTENDER_PCF8575_1_INT HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
// ============================================================================
// EXPERIMENTEEL — PCF8575 #2
// ============================================================================
#ifndef I2C_ADDRESS_EXTENDER_PCF8575_2
// #define I2C_ADDRESS_EXTENDER_PCF8575_2 0x21  // standaard waarde: 0x21  // EXCLUSIEF
#endif
#ifndef EXTENDER_PCF8575_2_INT
// #define EXTENDER_PCF8575_2_INT HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif

#ifndef EXTENDER_TCA9548A_AANTAL
// #define EXTENDER_TCA9548A_AANTAL 0  // standaard waarde: 0
#endif
// ============================================================================
// DEFAULT — TCA9548A #1
// ============================================================================
#ifndef I2C_ADDRESS_EXTENDER_TCA9548A_1
// #define I2C_ADDRESS_EXTENDER_TCA9548A_1 0x70  // standaard waarde: 0x70  // EXCLUSIEF
#endif
#ifndef EXTENDER_TCA9548A_1_RESET
// #define EXTENDER_TCA9548A_1_RESET HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
// ============================================================================
// EXPERIMENTEEL — TCA9548A #2
// ============================================================================
#ifndef I2C_ADDRESS_EXTENDER_TCA9548A_2
// #define I2C_ADDRESS_EXTENDER_TCA9548A_2 0x71  // standaard waarde: 0x71  // EXCLUSIEF
#endif
#ifndef EXTENDER_TCA9548A_2_RESET
// #define EXTENDER_TCA9548A_2_RESET HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif

#ifndef EXTENDER_ADS7828_AANTAL
// #define EXTENDER_ADS7828_AANTAL 0  // standaard waarde: 0
#endif
// ============================================================================
// DEFAULT — ADS7828 #1
// ============================================================================
#ifndef I2C_ADDRESS_EXTENDER_ADS7828_1
// #define I2C_ADDRESS_EXTENDER_ADS7828_1 0x48  // standaard waarde: 0x48  // EXCLUSIEF
#endif
// ============================================================================
// EXPERIMENTEEL — ADS7828 #2
// ============================================================================
#ifndef I2C_ADDRESS_EXTENDER_ADS7828_2
// #define I2C_ADDRESS_EXTENDER_ADS7828_2 0x49  // standaard waarde: 0x49  // EXCLUSIEF
#endif

#ifndef EXTENDER_DS2482_800_AANTAL
// #define EXTENDER_DS2482_800_AANTAL 0  // standaard waarde: 0
#endif
// ============================================================================
// DEFAULT — DS2482-800 #1
// ============================================================================
#ifndef I2C_ADDRESS_EXTENDER_DS2482_800_1
// #define I2C_ADDRESS_EXTENDER_DS2482_800_1 0x18  // standaard waarde: 0x18  // EXCLUSIEF
#endif
// ============================================================================
// EXPERIMENTEEL — DS2482-800 #2
// ============================================================================
#ifndef I2C_ADDRESS_EXTENDER_DS2482_800_2
// #define I2C_ADDRESS_EXTENDER_DS2482_800_2 0x19  // standaard waarde: 0x19  // EXCLUSIEF
#endif

#ifndef EXTENDER_ADS1158_AANTAL
// #define EXTENDER_ADS1158_AANTAL 0  // standaard waarde: 0
#endif
// ============================================================================
// DEFAULT — ADS1158 #1
// ============================================================================
#ifndef CS_PIN_EXTENDER_ADS1158_1
// #define CS_PIN_EXTENDER_ADS1158_1 HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
#ifndef EXTENDER_ADS1158_1_PWDN
// #define EXTENDER_ADS1158_1_PWDN HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
#ifndef EXTENDER_ADS1158_1_RESET
// #define EXTENDER_ADS1158_1_RESET HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
#ifndef EXTENDER_ADS1158_1_START
// #define EXTENDER_ADS1158_1_START HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
// ============================================================================
// EXPERIMENTEEL — ADS1158 #2
// ============================================================================
#ifndef CS_PIN_EXTENDER_ADS1158_2
// #define CS_PIN_EXTENDER_ADS1158_2 HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
#ifndef EXTENDER_ADS1158_2_PWDN
// #define EXTENDER_ADS1158_2_PWDN HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
#ifndef EXTENDER_ADS1158_2_RESET
// #define EXTENDER_ADS1158_2_RESET HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
#ifndef EXTENDER_ADS1158_2_START
// #define EXTENDER_ADS1158_2_START HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif

#ifndef EXTENDER_ADS7953_AANTAL
// #define EXTENDER_ADS7953_AANTAL 0  // standaard waarde: 0
#endif
// ============================================================================
// DEFAULT — ADS7953 #1
// ============================================================================
#ifndef CS_PIN_EXTENDER_ADS7953_1
// #define CS_PIN_EXTENDER_ADS7953_1 HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
#ifndef EXTENDER_ADS7953_GPIO0_TO_UNO_1
// #define EXTENDER_ADS7953_GPIO0_TO_UNO_1 HardwareResourcePin::D5  // standaard waarde: HardwareResourcePin::D5  // EXCLUSIEF
#endif
#ifndef EXTENDER_ADS7953_GPIO1_TO_UNO_1
// #define EXTENDER_ADS7953_GPIO1_TO_UNO_1 HardwareResourcePin::D6  // standaard waarde: HardwareResourcePin::D6  // EXCLUSIEF
#endif
#ifndef EXTENDER_ADS7953_GPIO2_TO_UNO_1
// #define EXTENDER_ADS7953_GPIO2_TO_UNO_1 HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
#ifndef EXTENDER_ADS7953_GPIO3_TO_UNO_1
// #define EXTENDER_ADS7953_GPIO3_TO_UNO_1 HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
// ============================================================================
// EXPERIMENTEEL — ADS7953 #2
// ============================================================================
#ifndef CS_PIN_EXTENDER_ADS7953_2
// #define CS_PIN_EXTENDER_ADS7953_2 HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
#ifndef EXTENDER_ADS7953_GPIO0_TO_UNO_2
// #define EXTENDER_ADS7953_GPIO0_TO_UNO_2 HardwareResourcePin::D7  // standaard waarde: HardwareResourcePin::D7  // EXCLUSIEF
#endif
#ifndef EXTENDER_ADS7953_GPIO1_TO_UNO_2
// #define EXTENDER_ADS7953_GPIO1_TO_UNO_2 HardwareResourcePin::D8  // standaard waarde: HardwareResourcePin::D8  // EXCLUSIEF
#endif
#ifndef EXTENDER_ADS7953_GPIO2_TO_UNO_2
// #define EXTENDER_ADS7953_GPIO2_TO_UNO_2 HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
#ifndef EXTENDER_ADS7953_GPIO3_TO_UNO_2
// #define EXTENDER_ADS7953_GPIO3_TO_UNO_2 HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif

#ifndef EXTENDER_CD74HC4067_AANTAL
// #define EXTENDER_CD74HC4067_AANTAL 0  // standaard waarde: 0
#endif
// ============================================================================
// DEFAULT — CD74HC4067 #1
// ============================================================================
#ifndef EXTENDER_CD74HC4067_1_EN
// #define EXTENDER_CD74HC4067_1_EN HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
#ifndef EXTENDER_CD74HC4067_1_S0
// #define EXTENDER_CD74HC4067_1_S0 HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
#ifndef EXTENDER_CD74HC4067_1_S1
// #define EXTENDER_CD74HC4067_1_S1 HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
#ifndef EXTENDER_CD74HC4067_1_S2
// #define EXTENDER_CD74HC4067_1_S2 HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
#ifndef EXTENDER_CD74HC4067_1_S3
// #define EXTENDER_CD74HC4067_1_S3 HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
#ifndef EXTENDER_CD74HC4067_1_Z
// #define EXTENDER_CD74HC4067_1_Z HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
#ifndef EXTENDER_CD74HC4067_2_EN
// #define EXTENDER_CD74HC4067_2_EN HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
#ifndef EXTENDER_CD74HC4067_2_S0
// #define EXTENDER_CD74HC4067_2_S0 HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
#ifndef EXTENDER_CD74HC4067_2_S1
// #define EXTENDER_CD74HC4067_2_S1 HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
#ifndef EXTENDER_CD74HC4067_2_S2
// #define EXTENDER_CD74HC4067_2_S2 HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
#ifndef EXTENDER_CD74HC4067_2_S3
// #define EXTENDER_CD74HC4067_2_S3 HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
#ifndef EXTENDER_CD74HC4067_2_Z
// #define EXTENDER_CD74HC4067_2_Z HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif

#ifndef EXTENDER_MAX14830_I2C_AANTAL
// #define EXTENDER_MAX14830_I2C_AANTAL 0  // standaard waarde: 0
#endif
#ifndef EXTENDER_MAX14830_SPI_AANTAL
// #define EXTENDER_MAX14830_SPI_AANTAL 0  // standaard waarde: 0
#endif
// ============================================================================
// DEFAULT — MAX14830 SPI #1
// ============================================================================
#ifndef CS_PIN_EXTENDER_MAX14830_SPI_1
// #define CS_PIN_EXTENDER_MAX14830_SPI_1 HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
#ifndef EXTENDER_MAX14830_SPI_1_IRQ
// #define EXTENDER_MAX14830_SPI_1_IRQ HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
// ============================================================================
// EXPERIMENTEEL — MAX14830 SPI #2
// ============================================================================
#ifndef CS_PIN_EXTENDER_MAX14830_SPI_2
// #define CS_PIN_EXTENDER_MAX14830_SPI_2 HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
#ifndef EXTENDER_MAX14830_SPI_2_IRQ
// #define EXTENDER_MAX14830_SPI_2_IRQ HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
// ============================================================================
// DEFAULT — MAX14830 I2C #1
// ============================================================================
#ifndef I2C_ADDRESS_EXTENDER_MAX14830_I2C_1
// #define I2C_ADDRESS_EXTENDER_MAX14830_I2C_1 0x60  // standaard waarde: 0x60  // EXCLUSIEF
#endif
#ifndef EXTENDER_MAX14830_I2C_1_IRQ
// #define EXTENDER_MAX14830_I2C_1_IRQ HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
// ============================================================================
// EXPERIMENTEEL — MAX14830 I2C #2
// ============================================================================
#ifndef I2C_ADDRESS_EXTENDER_MAX14830_I2C_2
// #define I2C_ADDRESS_EXTENDER_MAX14830_I2C_2 0x61  // standaard waarde: 0x61  // EXCLUSIEF
#endif
#ifndef EXTENDER_MAX14830_I2C_2_IRQ
// #define EXTENDER_MAX14830_I2C_2_IRQ HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif

#ifndef EXTENDER_SC16IS752_I2C_AANTAL
// #define EXTENDER_SC16IS752_I2C_AANTAL 0  // standaard waarde: 0
#endif
// ============================================================================
// DEFAULT — SC16IS752 I2C #1
// ============================================================================
#ifndef I2C_ADDRESS_EXTENDER_SC16IS752_I2C_1
// #define I2C_ADDRESS_EXTENDER_SC16IS752_I2C_1 0x48  // standaard waarde: 0x48  // EXCLUSIEF
#endif
#ifndef EXTENDER_SC16IS752_I2C_1_IRQ
// #define EXTENDER_SC16IS752_I2C_1_IRQ HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
#ifndef EXTENDER_SC16IS752_I2C_1_RESET
// #define EXTENDER_SC16IS752_I2C_1_RESET HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
// ============================================================================
// EXPERIMENTEEL — SC16IS752 I2C #2
// ============================================================================
#ifndef I2C_ADDRESS_EXTENDER_SC16IS752_I2C_2
// #define I2C_ADDRESS_EXTENDER_SC16IS752_I2C_2 0x49  // standaard waarde: 0x49  // EXCLUSIEF
#endif
#ifndef EXTENDER_SC16IS752_I2C_2_IRQ
// #define EXTENDER_SC16IS752_I2C_2_IRQ HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
#ifndef EXTENDER_SC16IS752_I2C_2_RESET
// #define EXTENDER_SC16IS752_I2C_2_RESET HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
#ifndef EXTENDER_SC16IS752_SPI_AANTAL
// #define EXTENDER_SC16IS752_SPI_AANTAL 0  // standaard waarde: 0
#endif
// ============================================================================
// DEFAULT — SC16IS752 SPI #1
// ============================================================================
#ifndef CS_PIN_EXTENDER_SC16IS752_SPI_1
// #define CS_PIN_EXTENDER_SC16IS752_SPI_1 HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
#ifndef EXTENDER_SC16IS752_SPI_1_IRQ
// #define EXTENDER_SC16IS752_SPI_1_IRQ HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
#ifndef EXTENDER_SC16IS752_SPI_1_RESET
// #define EXTENDER_SC16IS752_SPI_1_RESET HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
// ============================================================================
// EXPERIMENTEEL — SC16IS752 SPI #2
// ============================================================================
#ifndef CS_PIN_EXTENDER_SC16IS752_SPI_2
// #define CS_PIN_EXTENDER_SC16IS752_SPI_2 HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
#ifndef EXTENDER_SC16IS752_SPI_2_IRQ
// #define EXTENDER_SC16IS752_SPI_2_IRQ HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif
#ifndef EXTENDER_SC16IS752_SPI_2_RESET
// #define EXTENDER_SC16IS752_SPI_2_RESET HardwareResourcePin::NONE  // standaard waarde: HardwareResourcePin::NONE  // EXCLUSIEF
#endif

// ============================================================================
// ADC BACKEND
// ============================================================================

// Definieer de pinnen voor de sensoren op jouw sensorboard.
// Bij ADC_BACKEND_ADS1115 zijn ADC_PIN_SENSOR_1..4 kanaalnummers 0 tot 3.
// Bij ADC_BACKEND_NATIVE zijn ADC_PIN_SENSOR_1..4 HardwareResourcePin-waarden.
#ifndef ADC_PIN_SENSOR_1
// #define ADC_PIN_SENSOR_1 HardwareResourcePin::A0  // standaard waarde: HardwareResourcePin::A0; bij ADC_BACKEND_ADS1115: 0
#endif
#ifndef ADC_PIN_SENSOR_2
// #define ADC_PIN_SENSOR_2 HardwareResourcePin::A1  // standaard waarde: HardwareResourcePin::A1; bij ADC_BACKEND_ADS1115: 1
#endif
#ifndef ADC_PIN_SENSOR_3
// #define ADC_PIN_SENSOR_3 HardwareResourcePin::A2  // standaard waarde: HardwareResourcePin::A2; bij ADC_BACKEND_ADS1115: 2
#endif
#ifndef ADC_PIN_SENSOR_4
// #define ADC_PIN_SENSOR_4 HardwareResourcePin::A3  // standaard waarde: HardwareResourcePin::A3; bij ADC_BACKEND_ADS1115: 3
#endif

// Vertraging tussen samples in de meetlussen 
#ifndef DELAY_US
// #define DELAY_US 100     // standaard waarde berekend aan de hand van automatisch gedetecteerde BOARD_VERSION
#endif

// ============================================================================
// INPUT
// ============================================================================

// INPUT_TYPE_DIGITAL gebruikt HardwareResourcePin-waarden voor de vier fysieke keypad-aansluitingen.
#if defined(INPUT_KANAAL_CONFIG) && ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_DIGITAL)
  #if defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_DIRECT_1x4
// #define INPUT_KEYPAD_PIN_K1 HardwareResourcePin::D2
// #define INPUT_KEYPAD_PIN_K2 HardwareResourcePin::D3
// #define INPUT_KEYPAD_PIN_K3 HardwareResourcePin::D4
// #define INPUT_KEYPAD_PIN_K4 HardwareResourcePin::D5

  #elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_MATRIX_2x2
// #define INPUT_KEYPAD_PIN_L1 HardwareResourcePin::D2
// #define INPUT_KEYPAD_PIN_L2 HardwareResourcePin::D3
// #define INPUT_KEYPAD_PIN_R1 HardwareResourcePin::D4
// #define INPUT_KEYPAD_PIN_R2 HardwareResourcePin::D5

  #elif defined(KEYPAD_TYPE) && (KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4 || KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_DIRECT_4x1)
// #define INPUT_KEYPAD_PIN_1 HardwareResourcePin::D3
// #define INPUT_KEYPAD_PIN_2 HardwareResourcePin::D2
// #define INPUT_KEYPAD_PIN_3 HardwareResourcePin::D5
// #define INPUT_KEYPAD_PIN_4 HardwareResourcePin::D4

  #elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_TOUCH_TTP224_DIRECT_1x4
// #define INPUT_KEYPAD_PIN_OUT1 HardwareResourcePin::D2
// #define INPUT_KEYPAD_PIN_OUT2 HardwareResourcePin::D3
// #define INPUT_KEYPAD_PIN_OUT3 HardwareResourcePin::D4
// #define INPUT_KEYPAD_PIN_OUT4 HardwareResourcePin::D5
  #endif
#endif

// INPUT_TYPE_PCF8574: logische aansluitingen op PCF8574 P0-P7 ----------------
// Pas alleen het blok aan dat bij het gekozen KEYPAD_TYPE hoort.
// PCF8574_PIN_P0 t.e.m. PCF8574_PIN_P7 zijn vaste hardware-identiteiten en worden niet aangepast.
#if defined(INPUT_KANAAL_CONFIG) && ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_PCF8574)
  #if defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_USER_DEFINED_DIRECT
  // Experimenteel: eigen directe PCF8574-configuratie. Zie docs/Systeem/INPUT.md.
  // Verwijder de // voor de benodigde #define-regels en pas de waarden aan je eigen keypad aan.
  // Let op: dit zijn PCF8574-pinnen P0 t.e.m. P7, geen Arduino-pinnummers.
  // #define KEYPAD_GENERIEK_AANTAL_PINNEN 4
  // #define KEYPAD_GENERIEK_PINNEN {PCF8574_PIN_P0, PCF8574_PIN_P1, PCF8574_PIN_P2, PCF8574_PIN_P3}
  // #define KEYPAD_GENERIEK_KEY_LAYOUT { {"1", "Toets 1"}, {"2", "Toets 2"}, {"3", "Toets 3"}, {"4", "Toets 4"} }
  // #define KEYPAD_GENERIEK_OUTPUT_LEVEL_WHEN_KEY_PRESSED KEYPAD_GENERIEK_OUTPUT_LEVEL_WHEN_KEY_PRESSED_LOW

  #elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_USER_DEFINED_MATRIX
  // Experimenteel: eigen matrix-PCF8574-configuratie. Zie docs/Systeem/INPUT.md.
  // Verwijder de // voor de benodigde #define-regels en pas de waarden aan je eigen matrix aan.
  // Let op: rijen + kolommen samen maximaal 8 PCF8574-pinnen; generieke matrices zijn active-low.
  // Onderstaand 4x3-profiel komt overeen met examples/Systeem/Input/InputkanalenPCF8574UserDefinedMatrix.
  // #define KEYPAD_GENERIEK_AANTAL_RIJEN     4
  // #define KEYPAD_GENERIEK_AANTAL_KOLOMMEN  3
  // #define KEYPAD_GENERIEK_RIJ_PINNEN       {PCF8574_PIN_P0, PCF8574_PIN_P1, PCF8574_PIN_P2, PCF8574_PIN_P3}
  // #define KEYPAD_GENERIEK_KOLOM_PINNEN     {PCF8574_PIN_P4, PCF8574_PIN_P5, PCF8574_PIN_P6}
  // #define KEYPAD_GENERIEK_KEY_LAYOUT { \
  //   {"1", "Toets 1"}, {"2", "Toets 2"}, {"3", "Toets 3"}, \
  //   {"4", "Toets 4"}, {"5", "Toets 5"}, {"6", "Toets 6"}, \
  //   {"7", "Toets 7"}, {"8", "Toets 8"}, {"9", "Toets 9"}, \
  //   {"*", "Toets *"}, {"0", "Toets 0"}, {"#", "Toets #"} \
  // }

  #elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_DIRECT_1x4
  // #define INPUT_KEYPAD_PIN_K1 PCF8574_PIN_P0 // standaard waarde: PCF8574_PIN_P0
  // #define INPUT_KEYPAD_PIN_K2 PCF8574_PIN_P1 // standaard waarde: PCF8574_PIN_P1
  // #define INPUT_KEYPAD_PIN_K3 PCF8574_PIN_P2 // standaard waarde: PCF8574_PIN_P2
  // #define INPUT_KEYPAD_PIN_K4 PCF8574_PIN_P3 // standaard waarde: PCF8574_PIN_P3

  #elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_DIRECT_2x4
  // #define INPUT_KEYPAD_PIN_K1 PCF8574_PIN_P0 // standaard waarde: PCF8574_PIN_P0
  // #define INPUT_KEYPAD_PIN_K2 PCF8574_PIN_P1 // standaard waarde: PCF8574_PIN_P1
  // #define INPUT_KEYPAD_PIN_K3 PCF8574_PIN_P2 // standaard waarde: PCF8574_PIN_P2
  // #define INPUT_KEYPAD_PIN_K4 PCF8574_PIN_P3 // standaard waarde: PCF8574_PIN_P3
  // #define INPUT_KEYPAD_PIN_K5 PCF8574_PIN_P4 // standaard waarde: PCF8574_PIN_P4
  // #define INPUT_KEYPAD_PIN_K6 PCF8574_PIN_P5 // standaard waarde: PCF8574_PIN_P5
  // #define INPUT_KEYPAD_PIN_K7 PCF8574_PIN_P6 // standaard waarde: PCF8574_PIN_P6
  // #define INPUT_KEYPAD_PIN_K8 PCF8574_PIN_P7 // standaard waarde: PCF8574_PIN_P7

  #elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_MATRIX_2x2
  // #define INPUT_KEYPAD_PIN_L1 PCF8574_PIN_P0 // standaard waarde: PCF8574_PIN_P0
  // #define INPUT_KEYPAD_PIN_L2 PCF8574_PIN_P1 // standaard waarde: PCF8574_PIN_P1
  // #define INPUT_KEYPAD_PIN_R1 PCF8574_PIN_P2 // standaard waarde: PCF8574_PIN_P2
  // #define INPUT_KEYPAD_PIN_R2 PCF8574_PIN_P3 // standaard waarde: PCF8574_PIN_P3

  #elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_MATRIX_4x4
  // #define INPUT_KEYPAD_PIN_C4 PCF8574_PIN_P0 // standaard waarde: PCF8574_PIN_P0
  // #define INPUT_KEYPAD_PIN_C3 PCF8574_PIN_P1 // standaard waarde: PCF8574_PIN_P1
  // #define INPUT_KEYPAD_PIN_C2 PCF8574_PIN_P2 // standaard waarde: PCF8574_PIN_P2
  // #define INPUT_KEYPAD_PIN_C1 PCF8574_PIN_P3 // standaard waarde: PCF8574_PIN_P3
  // #define INPUT_KEYPAD_PIN_R1 PCF8574_PIN_P4 // standaard waarde: PCF8574_PIN_P4
  // #define INPUT_KEYPAD_PIN_R2 PCF8574_PIN_P5 // standaard waarde: PCF8574_PIN_P5
  // #define INPUT_KEYPAD_PIN_R3 PCF8574_PIN_P6 // standaard waarde: PCF8574_PIN_P6
  // #define INPUT_KEYPAD_PIN_R4 PCF8574_PIN_P7 // standaard waarde: PCF8574_PIN_P7

  #elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4
  // #define INPUT_KEYPAD_PIN_1  PCF8574_PIN_P1 // standaard waarde: PCF8574_PIN_P1
  // #define INPUT_KEYPAD_PIN_2  PCF8574_PIN_P0 // standaard waarde: PCF8574_PIN_P0
  // #define INPUT_KEYPAD_PIN_3  PCF8574_PIN_P3 // standaard waarde: PCF8574_PIN_P3
  // #define INPUT_KEYPAD_PIN_4  PCF8574_PIN_P2 // standaard waarde: PCF8574_PIN_P2

  #elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_DIRECT_4x1
  // #define INPUT_KEYPAD_PIN_1  PCF8574_PIN_P1 // standaard waarde: PCF8574_PIN_P1
  // #define INPUT_KEYPAD_PIN_2  PCF8574_PIN_P0 // standaard waarde: PCF8574_PIN_P0
  // #define INPUT_KEYPAD_PIN_3  PCF8574_PIN_P3 // standaard waarde: PCF8574_PIN_P3
  // #define INPUT_KEYPAD_PIN_4  PCF8574_PIN_P2 // standaard waarde: PCF8574_PIN_P2

  #elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_MATRIX_1x4
  // #define INPUT_KEYPAD_PIN_R1 PCF8574_PIN_P0 // standaard waarde: PCF8574_PIN_P0
  // #define INPUT_KEYPAD_PIN_C1 PCF8574_PIN_P1 // standaard waarde: PCF8574_PIN_P1
  // #define INPUT_KEYPAD_PIN_C2 PCF8574_PIN_P2 // standaard waarde: PCF8574_PIN_P2
  // #define INPUT_KEYPAD_PIN_C3 PCF8574_PIN_P3 // standaard waarde: PCF8574_PIN_P3
  // #define INPUT_KEYPAD_PIN_C4 PCF8574_PIN_P4 // standaard waarde: PCF8574_PIN_P4

  #elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_MATRIX_2x4
  // #define INPUT_KEYPAD_PIN_R1 PCF8574_PIN_P0 // standaard waarde: PCF8574_PIN_P0
  // #define INPUT_KEYPAD_PIN_R2 PCF8574_PIN_P1 // standaard waarde: PCF8574_PIN_P1
  // #define INPUT_KEYPAD_PIN_C1 PCF8574_PIN_P2 // standaard waarde: PCF8574_PIN_P2
  // #define INPUT_KEYPAD_PIN_C2 PCF8574_PIN_P3 // standaard waarde: PCF8574_PIN_P3
  // #define INPUT_KEYPAD_PIN_C3 PCF8574_PIN_P4 // standaard waarde: PCF8574_PIN_P4
  // #define INPUT_KEYPAD_PIN_C4 PCF8574_PIN_P5 // standaard waarde: PCF8574_PIN_P5

  #elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_MATRIX_4x3
  // #define INPUT_KEYPAD_PIN_R1 PCF8574_PIN_P0 // standaard waarde: PCF8574_PIN_P0
  // #define INPUT_KEYPAD_PIN_R2 PCF8574_PIN_P1 // standaard waarde: PCF8574_PIN_P1
  // #define INPUT_KEYPAD_PIN_R3 PCF8574_PIN_P2 // standaard waarde: PCF8574_PIN_P2
  // #define INPUT_KEYPAD_PIN_R4 PCF8574_PIN_P3 // standaard waarde: PCF8574_PIN_P3
  // #define INPUT_KEYPAD_PIN_C1 PCF8574_PIN_P4 // standaard waarde: PCF8574_PIN_P4
  // #define INPUT_KEYPAD_PIN_C2 PCF8574_PIN_P5 // standaard waarde: PCF8574_PIN_P5
  // #define INPUT_KEYPAD_PIN_C3 PCF8574_PIN_P6 // standaard waarde: PCF8574_PIN_P6

  #elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_MATRIX_4x4
  // #define INPUT_KEYPAD_PIN_R1 PCF8574_PIN_P0 // standaard waarde: PCF8574_PIN_P0
  // #define INPUT_KEYPAD_PIN_R2 PCF8574_PIN_P1 // standaard waarde: PCF8574_PIN_P1
  // #define INPUT_KEYPAD_PIN_R3 PCF8574_PIN_P2 // standaard waarde: PCF8574_PIN_P2
  // #define INPUT_KEYPAD_PIN_R4 PCF8574_PIN_P3 // standaard waarde: PCF8574_PIN_P3
  // #define INPUT_KEYPAD_PIN_C1 PCF8574_PIN_P4 // standaard waarde: PCF8574_PIN_P4
  // #define INPUT_KEYPAD_PIN_C2 PCF8574_PIN_P5 // standaard waarde: PCF8574_PIN_P5
  // #define INPUT_KEYPAD_PIN_C3 PCF8574_PIN_P6 // standaard waarde: PCF8574_PIN_P6
  // #define INPUT_KEYPAD_PIN_C4 PCF8574_PIN_P7 // standaard waarde: PCF8574_PIN_P7

  #elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_TOUCH_TTP224_DIRECT_1x4
  // #define INPUT_KEYPAD_PIN_OUT1 PCF8574_PIN_P0 // standaard waarde: PCF8574_PIN_P0
  // #define INPUT_KEYPAD_PIN_OUT2 PCF8574_PIN_P1 // standaard waarde: PCF8574_PIN_P1
  // #define INPUT_KEYPAD_PIN_OUT3 PCF8574_PIN_P2 // standaard waarde: PCF8574_PIN_P2
  // #define INPUT_KEYPAD_PIN_OUT4 PCF8574_PIN_P3 // standaard waarde: PCF8574_PIN_P3

  #elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_TOUCH_TTP229_MATRIX_4x4
  // Via de 8 parallelle uitgangen op PCF8574 P0-P7 kunnen enkel de eerste 8 toetsen uitgelezen worden.
  // #define INPUT_KEYPAD_PIN_OUT1 PCF8574_PIN_P0 // standaard waarde: PCF8574_PIN_P0
  // #define INPUT_KEYPAD_PIN_OUT2 PCF8574_PIN_P1 // standaard waarde: PCF8574_PIN_P1
  // #define INPUT_KEYPAD_PIN_OUT3 PCF8574_PIN_P2 // standaard waarde: PCF8574_PIN_P2
  // #define INPUT_KEYPAD_PIN_OUT4 PCF8574_PIN_P3 // standaard waarde: PCF8574_PIN_P3
  // #define INPUT_KEYPAD_PIN_OUT5 PCF8574_PIN_P4 // standaard waarde: PCF8574_PIN_P4
  // #define INPUT_KEYPAD_PIN_OUT6 PCF8574_PIN_P5 // standaard waarde: PCF8574_PIN_P5
  // #define INPUT_KEYPAD_PIN_OUT7 PCF8574_PIN_P6 // standaard waarde: PCF8574_PIN_P6
  // #define INPUT_KEYPAD_PIN_OUT8 PCF8574_PIN_P7 // standaard waarde: PCF8574_PIN_P7
  #endif
#endif

// INPUT_TYPE_HX1838 ----------------------------------------------------------
#if defined(HX1838_TOETSENINDELING) && HX1838_TOETSENINDELING == HX1838_TOETSENINDELING_REMOTE_USER_DEFINED
// HX1838_TOETSENINDELING_REMOTE_USER_DEFINED is uitsluitend beschikbaar met HX1838_BRON_CODES_DEFINE.
// Geef het aantal toetsen en de opschrift/weergavetekstkoppeling volledig in UserConfig.h op.
// HX1838_GENERIEK_CODES is optioneel: aanwezig = opgegeven codes gebruiken; afwezig = HX1838GeneriekCodesKalibreren() starten om de codes te bepalen.
// De huidige HX1838-laag vergelijkt 8-bit commandwaarden (uint8_t).
// #define HX1838_GENERIEK_AANTAL_TOETSEN 4
// #define HX1838_GENERIEK_CODES {0x45UL, 0x46UL, 0x47UL, 0x44UL}
// #define HX1838_GENERIEK_KEY_LAYOUT { {"1", "1"}, {"2", "2"}, {"3", "3"}, {"4", "4"} }

// Optionele vaste HX1838-mapping voor de drie ingebouwde toetsenindelingen. Niet zelf gedefinieerde HX1838_CODE_x waarden worden door SystemConfig.h aangevuld met de standaardcodes.
// Bij HX1838_BRON_CODES_EEPROM_WANNEER_GEEN_DEFINE wordt de vaste mapping gebruikt zodra minstens één HX1838_CODE_x in UserConfig.h is gedefinieerd.
// Wil je in die modus EEPROM gebruiken, laat dan alle HX1838_CODE_x regels uitgeschakeld. Een expliciet gedefinieerde code met waarde 0 is ongeldig wanneer de vaste mapping wordt gebruikt.
// Onderstaande waarden zijn de gekende NEC-codes van de standaard meegeleverde afstandsbediening; verwijder de "//" om ze te activeren, of vervang door je eigen gekalibreerde waarden.
#elif defined(HX1838_TOETSENINDELING) && HX1838_TOETSENINDELING == HX1838_TOETSENINDELING_REMOTE_OK_BOVENAAN_17_TOETSEN
// #define HX1838_CODE_1  0x46UL // = UP    standaard waarde: 0x46UL
// #define HX1838_CODE_2  0x15UL // = DOWN  standaard waarde: 0x15UL
// #define HX1838_CODE_3  0x40UL // = OK    standaard waarde: 0x40UL
// #define HX1838_CODE_4  0x44UL // = LEFT  standaard waarde: 0x44UL
// #define HX1838_CODE_5  0x43UL // = RIGHT standaard waarde: 0x43UL
// #define HX1838_CODE_6  0x16UL // = 1     standaard waarde: 0x16UL
// #define HX1838_CODE_7  0x19UL // = 2     standaard waarde: 0x19UL
// #define HX1838_CODE_8  0x0DUL // = 3     standaard waarde: 0x0DUL
// #define HX1838_CODE_9  0x0CUL // = 4     standaard waarde: 0x0CUL
// #define HX1838_CODE_10 0x18UL // = 5     standaard waarde: 0x18UL
// #define HX1838_CODE_11 0x5EUL // = 6     standaard waarde: 0x5EUL
// #define HX1838_CODE_12 0x08UL // = 7     standaard waarde: 0x08UL
// #define HX1838_CODE_13 0x1CUL // = 8     standaard waarde: 0x1CUL
// #define HX1838_CODE_14 0x5AUL // = 9     standaard waarde: 0x5AUL
// #define HX1838_CODE_15 0x42UL // = *     standaard waarde: 0x42UL
// #define HX1838_CODE_16 0x52UL // = 0     standaard waarde: 0x52UL
// #define HX1838_CODE_17 0x4AUL // = #     standaard waarde: 0x4AUL

#elif defined(HX1838_TOETSENINDELING) && HX1838_TOETSENINDELING == HX1838_TOETSENINDELING_REMOTE_OK_ONDERAAN_17_TOETSEN
// #define HX1838_CODE_1  0x45UL // = 1     standaard waarde: 0x45UL
// #define HX1838_CODE_2  0x46UL // = 2     standaard waarde: 0x46UL
// #define HX1838_CODE_3  0x47UL // = 3     standaard waarde: 0x47UL
// #define HX1838_CODE_4  0x44UL // = 4     standaard waarde: 0x44UL
// #define HX1838_CODE_5  0x40UL // = 5     standaard waarde: 0x40UL
// #define HX1838_CODE_6  0x43UL // = 6     standaard waarde: 0x43UL
// #define HX1838_CODE_7  0x07UL // = 7     standaard waarde: 0x07UL
// #define HX1838_CODE_8  0x15UL // = 8     standaard waarde: 0x15UL
// #define HX1838_CODE_9  0x09UL // = 9     standaard waarde: 0x09UL
// #define HX1838_CODE_10 0x16UL // = *     standaard waarde: 0x16UL
// #define HX1838_CODE_11 0x19UL // = 0     standaard waarde: 0x19UL
// #define HX1838_CODE_12 0x0DUL // = #     standaard waarde: 0x0DUL
// #define HX1838_CODE_13 0x18UL // = UP    standaard waarde: 0x18UL
// #define HX1838_CODE_14 0x52UL // = DOWN  standaard waarde: 0x52UL
// #define HX1838_CODE_15 0x1CUL // = OK    standaard waarde: 0x1CUL
// #define HX1838_CODE_16 0x08UL // = LEFT  standaard waarde: 0x08UL
// #define HX1838_CODE_17 0x5AUL // = RIGHT standaard waarde: 0x5AUL

#elif defined(HX1838_TOETSENINDELING) && HX1838_TOETSENINDELING == HX1838_TOETSENINDELING_REMOTE_21_TOETSEN_MP3
// #define HX1838_CODE_1  0x45UL // = CH-   standaard waarde: 0x45UL
// #define HX1838_CODE_2  0x46UL // = CH    standaard waarde: 0x46UL
// #define HX1838_CODE_3  0x47UL // = CH+   standaard waarde: 0x47UL
// #define HX1838_CODE_4  0x44UL // = PREV  standaard waarde: 0x44UL
// #define HX1838_CODE_5  0x40UL // = NEXT  standaard waarde: 0x40UL
// #define HX1838_CODE_6  0x43UL // = PLAY  standaard waarde: 0x43UL
// #define HX1838_CODE_7  0x07UL // = -     standaard waarde: 0x07UL
// #define HX1838_CODE_8  0x15UL // = +     standaard waarde: 0x15UL
// #define HX1838_CODE_9  0x09UL // = EQ    standaard waarde: 0x09UL
// #define HX1838_CODE_10 0x16UL // = 0     standaard waarde: 0x16UL
// #define HX1838_CODE_11 0x19UL // = 100+  standaard waarde: 0x19UL
// #define HX1838_CODE_12 0x0DUL // = 200+  standaard waarde: 0x0DUL
// #define HX1838_CODE_13 0x0CUL // = 1     standaard waarde: 0x0CUL
// #define HX1838_CODE_14 0x18UL // = 2     standaard waarde: 0x18UL
// #define HX1838_CODE_15 0x5EUL // = 3     standaard waarde: 0x5EUL
// #define HX1838_CODE_16 0x08UL // = 4     standaard waarde: 0x08UL
// #define HX1838_CODE_17 0x1CUL // = 5     standaard waarde: 0x1CUL
// #define HX1838_CODE_18 0x5AUL // = 6     standaard waarde: 0x5AUL
// #define HX1838_CODE_19 0x42UL // = 7     standaard waarde: 0x42UL
// #define HX1838_CODE_20 0x52UL // = 8     standaard waarde: 0x52UL
// #define HX1838_CODE_21 0x4AUL // = 9     standaard waarde: 0x4AUL
#endif

#endif
