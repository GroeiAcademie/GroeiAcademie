#ifndef SYSTEM_CONFIG_H
#define SYSTEM_CONFIG_H

#include "../Systeem/Input/InputTypes.h"
#include "../Systeem/Screen/ScreenTypes.h"

// BOARD_VERSIONs: ondersteunde ARDUINO UNO-vormfactor boards die vóór UserConfig.h beschikbaar moeten zijn.
#define BOARD_UNO_R3                       0
#define BOARD_UNO_R4_MINIMA                1
#define BOARD_UNO_R4_WIFI                  2
#define BOARD_UNO_Q                        7
#define BOARD_ESP32_D1_UNO_R32             5
#define BOARD_ESP32S3_ARDI32               3
#define BOARD_ESP32S3_DEV                  8
#define BOARD_RP2040_CYTRON_MAKER_UNO      4
#define BOARD_STM32F4_NUCLEO64_F401RE      6

// ADC BACKEND: vaste keuzewaarden die vóór UserConfig.h beschikbaar moeten zijn.
#ifndef ADC_BACKEND_NATIVE
  #define ADC_BACKEND_NATIVE               0  // ingebouwde ADC van de Arduino
#endif
#ifndef ADC_BACKEND_ADS1115
  #define ADC_BACKEND_ADS1115              1  // externe 16-bit ADC via I2C
#endif

// GPIO: General Purpose Input/Output, programmeerbare pin op een computerchip of printplaat waarvan de functie via software kan worden ingesteld.
#define GPIO0    0
#define GPIO1    1
#define GPIO2    2
#define GPIO3    3
#define GPIO4    4
#define GPIO5    5
#define GPIO6    6
#define GPIO7    7
#define GPIO8    8
#define GPIO9    9

#define GPIO10  10
#define GPIO11  11
#define GPIO12  12
#define GPIO13  13
#define GPIO14  14
#define GPIO15  15
#define GPIO16  16
#define GPIO17  17
#define GPIO18  18
#define GPIO19  19

#define GPIO20  20
#define GPIO21  21
#define GPIO22  22
#define GPIO23  23
#define GPIO24  24
#define GPIO25  25
#define GPIO26  26
#define GPIO27  27
#define GPIO28  28
#define GPIO29  29

#define GPIO30  30
#define GPIO31  31
#define GPIO32  32
#define GPIO33  33
#define GPIO34  34
#define GPIO35  35
#define GPIO36  36
#define GPIO37  37
#define GPIO38  38
#define GPIO39  39

#define GPIO40  40
#define GPIO41  41
#define GPIO42  42
#define GPIO43  43
#define GPIO44  44
#define GPIO45  45
#define GPIO46  46
#define GPIO47  47
#define GPIO48  48
#define GPIO49  49

// opladen van UserConfig.h
#ifndef GROEIACADEMIE_IGNORE_USER_CONFIG
  #if defined(__has_include)
    #if __has_include("UserConfig.h")
      #include "UserConfig.h"
      #define GROEIACADEMIE_USER_CONFIG_GELADEN
    #endif
  #endif
#endif

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// AANPASSEN NAAR EIGEN BEHOEFTE DOE JE VIA: 'UserConfig.h', NIEUWE RELEASE OVERSCHRIJVEN DIT BESTAND!
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// ****************************************************************************
// BASIS INSTELLINGEN
// ****************************************************************************

// ============================================================================
// TAAL: standaard keuze
// ============================================================================
#if !defined(LANGUAGE_NL) && !defined(LANGUAGE_DE) && !defined(LANGUAGE_EN) && !defined(LANGUAGE_FR)
  #define LANGUAGE_NL // #define LANGUAGE_DE | #define LANGUAGE_EN | #define LANGUAGE_FR
#endif

// ============================================================================
// BOARD_VERSION 
// ============================================================================
#ifndef BOARD_VERSION
  // Automatische detectie gebruikt de compiler-/boardmacro van de geselecteerde Arduino-boardcore.
  // Voor SB Components Ardi32 blijft expliciete BOARD_VERSION-keuze nodig wanneer de generieke ESP32S3 Dev Module wordt gebruikt; zie docs/HARDWARE_SUPPORT.md.
  #if defined(ARDUINO_AVR_UNO)
    #define BOARD_VERSION BOARD_UNO_R3
  #elif defined(ARDUINO_UNOR4_MINIMA)
    #define BOARD_VERSION BOARD_UNO_R4_MINIMA
  #elif defined(ARDUINO_UNOR4_WIFI)
    #define BOARD_VERSION BOARD_UNO_R4_WIFI
  #elif defined(ARDUINO_UNO_Q)              // TODO: ARDUINO_ZEPHIR | ARDUINO_ESP32S3_WROOM_1
    #define BOARD_VERSION BOARD_UNO_Q
  #elif defined(ARDUINO_D1_UNO32)
    #define BOARD_VERSION BOARD_ESP32_D1_UNO_R32
  #elif defined(ARDUINO_ARDI32)
    #define BOARD_VERSION BOARD_ESP32S3_ARDI32 
  #elif defined(ARDUINO_ESP32S3_DEV)
    #define BOARD_VERSION BOARD_ESP32S3_DEV // TODO: ARDUINO_ESP32S3_UNO | ARDUINO_ESP32S3_WROOM_1
  #elif defined(ARDUINO_CYTRON_MAKER_UNO_RP2040)
    #define BOARD_VERSION BOARD_RP2040_CYTRON_MAKER_UNO
  #elif defined(ARDUINO_NUCLEO_F401RE)
    #define BOARD_VERSION BOARD_STM32F4_NUCLEO64_F401RE
  #else
    #error Dit board wordt momenteel (nog) niet ondersteund.
  #endif
#endif
// UserConfig.h heeft voorrang op automatische boarddetectie. 
// Wanneer de gebruiker in UserConfig.h zelf BOARD_VERSION definieert (bijvoorbeeld #define BOARD_VERSION BOARD_ESP32S3_ARDI32), 
// gebruikt de library die keuze en wordt bovenstaande automatische detectie niet uitgevoerd (de #ifndef hierboven slaat dan over). 
// Enkel wanneer BOARD_VERSION nergens vooraf gedefinieerd is, bepaalt dit bestand het board automatisch op basis van de compiler-/boardmacro's van de Arduino IDE.

#if BOARD_VERSION == BOARD_ESP32S3_ARDI32 || BOARD_VERSION == BOARD_ESP32S3_DEV 
  #define GA_SERIAL Serial0
#else
  #define GA_SERIAL Serial
#endif

// ============================================================================
// BOARD_ID: generieke, per-fysiek-bord-identiteit, bewust hier in SystemConfig.h en niet in een subsysteem-specifiek configuratiebestand, want "welk fysiek bord is dit" is geen eigenschap van één subsysteem. 
// (FURURE) Heeft nog geen functioneel effect; dient als voorbereiding op scenario's waarbij meerdere borden met elkaar communiceren en elkaar moeten kunnen onderscheiden. 
// Wijzig per fysiek bord via UserConfig.h.
// ============================================================================
#ifndef BOARD_ID
  #define BOARD_ID  1
#endif

// ============================================================================
// ADC BACKEND
// ----------------------------------------------------------------------------
// LET OP: Wanneer je ADC_BACKEND op ADC_BACKEND_ADS1115 zet, moet de Adafruit ADS1X15-library geïnstalleerd zijn via de Arduino Library Manager.
// ============================================================================
#ifndef AANTAL_SENSOREN_AANWEZIG
  #define AANTAL_SENSOREN_AANWEZIG  2
#endif

#if AANTAL_SENSOREN_AANWEZIG != 2 && AANTAL_SENSOREN_AANWEZIG != 4
  #error AANTAL_SENSOREN_AANWEZIG moet 2 of 4 zijn.
#endif

#ifndef ADC_BACKEND
  #define ADC_BACKEND ADC_BACKEND_NATIVE  // wissel dit om van backend te wisselen
#endif

#if ADC_BACKEND != ADC_BACKEND_NATIVE && ADC_BACKEND != ADC_BACKEND_ADS1115
  #error Selecteer een geldige ADC_BACKEND.
#endif

#ifndef I2C_ADDRESS_ADS1115
  #define I2C_ADDRESS_ADS1115 0x48
#endif

// Instelbare vertraging in de busy-wait loop van WachtTotAlleSensorsLosgelatenVoorTest().
// Bij ADC_BACKEND_NATIVE blijft dit 0. Bij ADC_BACKEND_ADS1115 voorkomt dit dat de I2C-bus zonder onderbreking bevraagd wordt.
#ifndef WACHT_LOSLATEN_DELAY_MS
  #if ADC_BACKEND == ADC_BACKEND_ADS1115
    #define WACHT_LOSLATEN_DELAY_MS 5UL
  #else
    #define WACHT_LOSLATEN_DELAY_MS 0UL // standaard waarde
  #endif
#endif

// ============================================================================
// INPUT
// ============================================================================

#ifndef INPUT_DEBOUNCE_MS
  #define INPUT_DEBOUNCE_MS 25UL
#endif

#ifndef INPUT_KANAAL_TIMEOUT_BIJ_GEEN_TOETSAANSLAG_BINNEN_MS
  #define INPUT_KANAAL_TIMEOUT_BIJ_GEEN_TOETSAANSLAG_BINNEN_MS 0UL   // 0UL = geen timeout, blijven wachten (huidig gedrag)
#endif

#ifndef MAX_AANTAL_SIMULTANE_TOETSAANSLAGEN
  #define MAX_AANTAL_SIMULTANE_TOETSAANSLAGEN 1
#endif

#ifndef DEFAULT_TOEGESTANE_MARGE_SIMULTANE_STARTTIJD_MS
  #define DEFAULT_TOEGESTANE_MARGE_SIMULTANE_STARTTIJD_MS 100UL
#endif

// Uitgebreide gebeurtenissen (loslaten, lang indrukken, timeout bij geen invoer).
// Standaard UIT, om geheugen te sparen op geheugenarme boards (bv. AVR Uno R3).
// Enkel van toepassing bij OpvragenHuidigeToetsAanslag(false), niet bij wachten=true.
// #define INPUT_KANAAL_OPVRAGEN_HUIDIGE_TOETSAANSLAG_UITGEBREID

// INPUT_KANAAL_CONFIG standaard INPUT_TYPE_DIGITAL
#ifndef INPUT_KANAAL_CONFIG
  #define INPUT_KANAAL_CONFIG INPUT_TYPE_DIGITAL
#endif

#if ((INPUT_KANAAL_CONFIG) & (INPUT_TYPE_DIGITAL | INPUT_TYPE_PCF8574))
  #ifndef KEYPAD_TYPE
    #define KEYPAD_TYPE KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4 // standaard waarde
  #endif
#endif

// INPUT_TYPE_PCF8574 ---------------------------------------------------------
#ifndef I2C_ADDRESS_PCF8574
  #define I2C_ADDRESS_PCF8574 0x20
#endif

#ifndef TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED
  #define TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED_HIGH
#endif

// INPUT_TYPE_HX1838 ----------------------------------------------------------
// D8 is de standaard HX1838-ontvangerpin; getest met zowel TinyIRReceiver als IRremote terwijl het PixelScreen aangesloten was. 
#ifndef HX1838_ONTVANGER_PIN
  #define HX1838_ONTVANGER_PIN ARDUINO_UNO_SHIELD_PIN_D8
#endif

// HX1838_USE_TINYIRRECEIVER_INSTEAD_OF_IRREMOTE: Standaard 1.
// 1 = TinyIRReceiver.hpp (pin-change-interrupt, geen timerkanaal nodig, kleinere flash/RAM-voetafdruk), 
// 0 = klassieke IRremote.hpp/IrReceiver (polling, timer-gebaseerd). 
#ifndef HX1838_USE_TINYIRRECEIVER_INSTEAD_OF_IRREMOTE
  #define HX1838_USE_TINYIRRECEIVER_INSTEAD_OF_IRREMOTE 1
#endif

#if HX1838_USE_TINYIRRECEIVER_INSTEAD_OF_IRREMOTE != 0 && HX1838_USE_TINYIRRECEIVER_INSTEAD_OF_IRREMOTE != 1
  #error HX1838_USE_TINYIRRECEIVER_INSTEAD_OF_IRREMOTE moet 0 of 1 zijn.
#endif

#ifndef HX1838_TOETSENINDELING
  #define HX1838_TOETSENINDELING HX1838_TOETSENINDELING_REMOTE_OK_BOVENAAN_17_TOETSEN
#endif

#ifndef HX1838_BRON_CODES
  #define HX1838_BRON_CODES HX1838_BRON_CODES_DEFINE
#endif

#ifndef HX1838_KALIBRATIE_TOETS_PAUZE_MS
  #define HX1838_KALIBRATIE_TOETS_PAUZE_MS 300UL
#endif

#ifndef HX1838_KALIBRATIE_TIMEOUT_MS
  #define HX1838_KALIBRATIE_TIMEOUT_MS 30000UL
#endif

// Schakelt ControleerMappingVolledigheid() in.
// Enkel bedoeld voor tijdens het testen: meldt via Serial welke opschriften van het gecompileerde KEYPAD_TYPE/HX1838_TOETSENINDELING ontbreken in een mapping die je zelf als argument meegeeft.
// Kost extra flashgeheugen en Serial-afhankelijkheid, standaard uit. Niet aanraden om aan te laten in productiecode.
// #define INPUT_MAPPING_EXTRA_CONTROLES_INSCHAKELEN

// ============================================================================
// OUTPUT
// ============================================================================

// FATAL
#ifndef FATAL_LEESTIJD_MS
  #define FATAL_LEESTIJD_MS   30000UL
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
// Opmerking:
// Wanneer DEBUG actief is, voegt de library automatisch SCREEN_TYPE_SERIAL toe. Je hoeft SCREEN_TYPE_SERIAL dus niet zelf te selecteren voor debug-uitvoer.
//
// Kies hieronder welke schermuitvoer je wilt gebruiken; staat standaard op: geen uitvoer.
#ifndef SCREEN_OUTPUT_CONFIG
  #define SCREEN_OUTPUT_CONFIG SCREEN_TYPE_NONE
#endif

// Characterscherm ----------------------------------------------------------------
#ifndef I2C_ADDRESS_CHARACTER_SCREEN
  #define I2C_ADDRESS_CHARACTER_SCREEN 0x27
#endif

#ifndef ACTIEF_CHARACTER_SCREEN
  #define ACTIEF_CHARACTER_SCREEN        SCREEN_LCD1602
#endif

// Pixelscherm --------------------------------------------------------------------
#ifndef PIXEL_SCREEN_CS
  #define PIXEL_SCREEN_CS              HardwareResourcePin::SS
#endif
#ifndef PIXEL_SCREEN_DC
  #define PIXEL_SCREEN_DC              HardwareResourcePin::D9
#endif
#ifndef PIXEL_SCREEN_RST
  #define PIXEL_SCREEN_RST             HardwareResourcePin::D7
#endif

#ifndef ACTIEF_PIXEL_SCREEN
  #define ACTIEF_PIXEL_SCREEN            SCREEN_240X320
#endif

// Standaard rotatiewaarden (met de klok mee):
// 0:   0 graden: Portretmodus (standaard, met de kabels vaak aan de onder- of bovenkant).
// 1:  90 graden: Landschapsmodus (horizontaal).
// 2: 180 graden: Portretmodus op zijn kop.
// 3: 270 graden: Landschapsmodus op zijn kop.
// Tip: Bij sommige displays (zoals de ST7789 zonder CS-pin) zorgen de waarden 4, 5, 6 of 7 voor een gespiegeld beeld, wat handig kan zijn voor specifieke projecten.
#ifndef PIXEL_SCREEN_ROTATION
  #define PIXEL_SCREEN_ROTATION          3
#endif
#ifndef PIXEL_SCREEN_TEXT_SIZE
  #define PIXEL_SCREEN_TEXT_SIZE         3
#endif
#ifndef PIXEL_SCREEN_MARGIN
  #define PIXEL_SCREEN_MARGIN            5
#endif
#ifndef PIXEL_SCREEN_CHARACTER_SPACING
  #define PIXEL_SCREEN_CHARACTER_SPACING 1
#endif
#ifndef PIXEL_SCREEN_LINE_SPACING
  #define PIXEL_SCREEN_LINE_SPACING      5
#endif
#ifndef PIXEL_SCREEN_TEXT_COLOR
  #define PIXEL_SCREEN_TEXT_COLOR        0xFFFF
#endif
#ifndef PIXEL_SCREEN_BACKGROUND_COLOR
  #define PIXEL_SCREEN_BACKGROUND_COLOR  0x0000
#endif

// Kleuren per ScreenData-type, enkel gebruikt door een zelfgeschreven PixelScreen-callback (zie examples/Systeem/Screen/Callback_PixelScreen.ino). RGB565, driveronafhankelijk.
#ifndef PIXEL_SCREEN_KLEUR_FATAL
  #define PIXEL_SCREEN_KLEUR_FATAL       0xF800   // rood
#endif
#ifndef PIXEL_SCREEN_KLEUR_FAULT
  #define PIXEL_SCREEN_KLEUR_FAULT       0xFC00   // oranje
#endif
#ifndef PIXEL_SCREEN_KLEUR_WARNING
  #define PIXEL_SCREEN_KLEUR_WARNING     0xFFE0   // geel
#endif
#ifndef PIXEL_SCREEN_KLEUR_INFO
  #define PIXEL_SCREEN_KLEUR_INFO        0x07FF   // cyaan
#endif
#ifndef PIXEL_SCREEN_KLEUR_CRITICAL
  #define PIXEL_SCREEN_KLEUR_CRITICAL    0xF81F   // magenta, nog ernstiger dan ERROR, onderscheiden van FATAL-rood
#endif
#ifndef PIXEL_SCREEN_KLEUR_ABORT
  #define PIXEL_SCREEN_KLEUR_ABORT       0x780F   // paars
#endif
#ifndef PIXEL_SCREEN_KLEUR_PANIC
  #define PIXEL_SCREEN_KLEUR_PANIC       0xFFFF   // wit, springt er maximaal uit op een donkere achtergrond
#endif

// SerialScreen -------------------------------------------------------------------
#ifndef SERIAL_BAUDRATE
  #define SERIAL_BAUDRATE 115200UL
#endif

#ifndef SERIAL_CONNECT_TIMEOUT_MS
  #define SERIAL_CONNECT_TIMEOUT_MS 2000UL
#endif

// ****************************************************************************
// VALIDATIES
// ****************************************************************************

// BOARD
#if BOARD_VERSION != BOARD_UNO_R3 && BOARD_VERSION != BOARD_UNO_R4_MINIMA && BOARD_VERSION != BOARD_UNO_R4_WIFI && BOARD_VERSION != BOARD_UNO_Q && BOARD_VERSION != BOARD_ESP32S3_ARDI32 && BOARD_VERSION != BOARD_ESP32S3_DEV && BOARD_VERSION != BOARD_ESP32_D1_UNO_R32 && BOARD_VERSION != BOARD_RP2040_CYTRON_MAKER_UNO && BOARD_VERSION != BOARD_STM32F4_NUCLEO64_F401RE
  #error Selecteer een geldige BOARD_VERSION.
#endif

// TAAL
#if (defined(LANGUAGE_NL) + defined(LANGUAGE_DE) + defined(LANGUAGE_EN) + defined(LANGUAGE_FR)) == 0
  #error Selecteer een taal in UserConfig.h of SystemConfig.h: LANGUAGE_NL, LANGUAGE_DE, LANGUAGE_EN of LANGUAGE_FR.
#elif (defined(LANGUAGE_NL) + defined(LANGUAGE_DE) + defined(LANGUAGE_EN) + defined(LANGUAGE_FR)) > 1
  #error Selecteer slechts een taal in UserConfig.h of SystemConfig.h.
#endif

// ****************************************************************************
// UITGBREIDE INSTELLINGEN
// ****************************************************************************

// ============================================================================
// BOARD_VERSION VERSUS ARDUINO UNO-SHIELDPINNEN
// ============================================================================

// Gebruik deze GPIOxx-labels in plaats van kale cijfers, zodat de mapping leesbaar en controleerbaar blijft.
#if BOARD_VERSION == BOARD_UNO_R3 || BOARD_VERSION == BOARD_UNO_R4_MINIMA || BOARD_VERSION == BOARD_UNO_R4_WIFI || BOARD_VERSION == BOARD_UNO_Q
  #if BOARD_VERSION == BOARD_UNO_R3
    #ifndef ARDUINO_UNO_SHIELD_PIN_D0
      #define ARDUINO_UNO_SHIELD_PIN_D0 0
    #endif
    #ifndef ARDUINO_UNO_SHIELD_PIN_D1
      #define ARDUINO_UNO_SHIELD_PIN_D1 1
    #endif
    #ifndef ARDUINO_UNO_SHIELD_PIN_D2
      #define ARDUINO_UNO_SHIELD_PIN_D2 2
    #endif
    #ifndef ARDUINO_UNO_SHIELD_PIN_D3
      #define ARDUINO_UNO_SHIELD_PIN_D3 3
    #endif
    #ifndef ARDUINO_UNO_SHIELD_PIN_D4
      #define ARDUINO_UNO_SHIELD_PIN_D4 4
    #endif
    #ifndef ARDUINO_UNO_SHIELD_PIN_D5
      #define ARDUINO_UNO_SHIELD_PIN_D5 5
    #endif
    #ifndef ARDUINO_UNO_SHIELD_PIN_D6
      #define ARDUINO_UNO_SHIELD_PIN_D6 6
    #endif
    #ifndef ARDUINO_UNO_SHIELD_PIN_D7
      #define ARDUINO_UNO_SHIELD_PIN_D7 7
    #endif
    #ifndef ARDUINO_UNO_SHIELD_PIN_D8
      #define ARDUINO_UNO_SHIELD_PIN_D8 8
    #endif
    #ifndef ARDUINO_UNO_SHIELD_PIN_D9
      #define ARDUINO_UNO_SHIELD_PIN_D9 9
    #endif
    #ifndef ARDUINO_UNO_SHIELD_PIN_D10
      #define ARDUINO_UNO_SHIELD_PIN_D10 10
    #endif
    #ifndef ARDUINO_UNO_SHIELD_PIN_D11
      #define ARDUINO_UNO_SHIELD_PIN_D11 11
    #endif
    #ifndef ARDUINO_UNO_SHIELD_PIN_D12
      #define ARDUINO_UNO_SHIELD_PIN_D12 12
    #endif
    #ifndef ARDUINO_UNO_SHIELD_PIN_D13
      #define ARDUINO_UNO_SHIELD_PIN_D13 13
    #endif

  #elif BOARD_VERSION == BOARD_UNO_R4_MINIMA || BOARD_VERSION == BOARD_UNO_R4_WIFI || BOARD_VERSION == BOARD_UNO_Q
    #ifndef ARDUINO_UNO_SHIELD_PIN_D0
      #define ARDUINO_UNO_SHIELD_PIN_D0 D0
    #endif
    #ifndef ARDUINO_UNO_SHIELD_PIN_D1
      #define ARDUINO_UNO_SHIELD_PIN_D1 D1
    #endif
    #ifndef ARDUINO_UNO_SHIELD_PIN_D2
      #define ARDUINO_UNO_SHIELD_PIN_D2 D2
    #endif
    #ifndef ARDUINO_UNO_SHIELD_PIN_D3
      #define ARDUINO_UNO_SHIELD_PIN_D3 D3
    #endif
    #ifndef ARDUINO_UNO_SHIELD_PIN_D4
      #define ARDUINO_UNO_SHIELD_PIN_D4 D4
    #endif
    #ifndef ARDUINO_UNO_SHIELD_PIN_D5
      #define ARDUINO_UNO_SHIELD_PIN_D5 D5
    #endif
    #ifndef ARDUINO_UNO_SHIELD_PIN_D6
      #define ARDUINO_UNO_SHIELD_PIN_D6 D6
    #endif
    #ifndef ARDUINO_UNO_SHIELD_PIN_D7
      #define ARDUINO_UNO_SHIELD_PIN_D7 D7
    #endif
    #ifndef ARDUINO_UNO_SHIELD_PIN_D8
      #define ARDUINO_UNO_SHIELD_PIN_D8 D8
    #endif
    #ifndef ARDUINO_UNO_SHIELD_PIN_D9
      #define ARDUINO_UNO_SHIELD_PIN_D9 D9
    #endif
    #ifndef ARDUINO_UNO_SHIELD_PIN_D10
      #define ARDUINO_UNO_SHIELD_PIN_D10 D10
    #endif
    #ifndef ARDUINO_UNO_SHIELD_PIN_D11
      #define ARDUINO_UNO_SHIELD_PIN_D11 D11
    #endif
    #ifndef ARDUINO_UNO_SHIELD_PIN_D12
      #define ARDUINO_UNO_SHIELD_PIN_D12 D12
    #endif
    #ifndef ARDUINO_UNO_SHIELD_PIN_D13
      #define ARDUINO_UNO_SHIELD_PIN_D13 D13
    #endif
  #endif

  #ifndef ARDUINO_UNO_SHIELD_PIN_A0
    #define ARDUINO_UNO_SHIELD_PIN_A0 A0
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_A1
    #define ARDUINO_UNO_SHIELD_PIN_A1 A1
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_A2
    #define ARDUINO_UNO_SHIELD_PIN_A2 A2
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_A3
    #define ARDUINO_UNO_SHIELD_PIN_A3 A3
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_A4
    #define ARDUINO_UNO_SHIELD_PIN_A4 A4
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_A5
    #define ARDUINO_UNO_SHIELD_PIN_A5 A5
  #endif

  #ifndef ARDUINO_UNO_SHIELD_PIN_SDA
    #define ARDUINO_UNO_SHIELD_PIN_SDA SDA
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_SCL
    #define ARDUINO_UNO_SHIELD_PIN_SCL SCL
  #endif

  // Specifieke afhandeling voor de hardware SPI-pinnen om macro-vervuiling te voorkomen
  #if BOARD_VERSION == BOARD_UNO_Q // Voor de Uno Q (Zephyr) gebruiken we de veilige Dxx pinlabels om de kale cijfers te omzeilen.
    #ifndef ARDUINO_UNO_SHIELD_PIN_SS
      #define ARDUINO_UNO_SHIELD_PIN_SS   D10
    #endif
    #ifndef ARDUINO_UNO_SHIELD_PIN_MOSI
      #define ARDUINO_UNO_SHIELD_PIN_MOSI D11
    #endif
    #ifndef ARDUINO_UNO_SHIELD_PIN_MISO
      #define ARDUINO_UNO_SHIELD_PIN_MISO D12
    #endif
    #ifndef ARDUINO_UNO_SHIELD_PIN_SCK
      #define ARDUINO_UNO_SHIELD_PIN_SCK  D13
    #endif
  #else // Standaard fallback voor Uno R3, R4 Minima en R4 WiFi
    #ifndef ARDUINO_UNO_SHIELD_PIN_SS
      #define ARDUINO_UNO_SHIELD_PIN_SS   SS
    #endif
    #ifndef ARDUINO_UNO_SHIELD_PIN_MOSI
      #define ARDUINO_UNO_SHIELD_PIN_MOSI MOSI
    #endif
    #ifndef ARDUINO_UNO_SHIELD_PIN_MISO
      #define ARDUINO_UNO_SHIELD_PIN_MISO MISO
    #endif
    #ifndef ARDUINO_UNO_SHIELD_PIN_SCK
      #define ARDUINO_UNO_SHIELD_PIN_SCK  SCK
    #endif
  #endif

#elif BOARD_VERSION == BOARD_ESP32_D1_UNO_R32 // WEMOS D1 R32 & TTGO D1 R32: pinmapping bevestigd tegen espboards.dev/esp32/d1-uno32/
  #ifndef ARDUINO_UNO_SHIELD_PIN_D0
    #define ARDUINO_UNO_SHIELD_PIN_D0 GPIO3    // D0  /RX   /PWM / GPIO3  (UART RX)
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D1
    #define ARDUINO_UNO_SHIELD_PIN_D1 GPIO1    // D1  /TX   /PWM / GPIO1  (UART TX)
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D2
    #define ARDUINO_UNO_SHIELD_PIN_D2 GPIO26   // D2  /     /PWM / GPIO26 (vrij, geen bijzondere rol)
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D3
    #define ARDUINO_UNO_SHIELD_PIN_D3 GPIO25   // D3  /     /PWM / GPIO25 (vrij, geen bijzondere rol)
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D4
    #define ARDUINO_UNO_SHIELD_PIN_D4 GPIO17   // D4  /     /PWM / GPIO17 (gedeeld met PSRAM op PSRAM-modules)
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D5
    #define ARDUINO_UNO_SHIELD_PIN_D5 GPIO16   // D5  /     /PWM / GPIO16 (gedeeld met PSRAM op PSRAM-modules)
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D6
    #define ARDUINO_UNO_SHIELD_PIN_D6 GPIO27   // D6  /     /PWM / GPIO27 (vrij, geen bijzondere rol)
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D7
    #define ARDUINO_UNO_SHIELD_PIN_D7 GPIO14   // D7  /     /PWM / GPIO14 (JTAG TMS)
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D8
    #define ARDUINO_UNO_SHIELD_PIN_D8 GPIO12   // D8  /     /PWM / GPIO12 (boot-strapping, MTDI)
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D9
    #define ARDUINO_UNO_SHIELD_PIN_D9 GPIO13   // D9  /     /PWM / GPIO13 (JTAG TCK)
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D10
    #define ARDUINO_UNO_SHIELD_PIN_D10 GPIO5   // D10 /SS   /PWM / GPIO5  (SPI, boot-strapping)
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D11
    #define ARDUINO_UNO_SHIELD_PIN_D11 GPIO23  // D11 /MOSI /PWM / GPIO23 (SPI, vrij)
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D12
    #define ARDUINO_UNO_SHIELD_PIN_D12 GPIO19  // D12 /MISO /PWM / GPIO19 (SPI, vrij)
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D13
    #define ARDUINO_UNO_SHIELD_PIN_D13 GPIO18  // D13 /SCK  /PWM / GPIO18 (SPI, vrij, ook onboard-LED)
  #endif

  #ifndef ARDUINO_UNO_SHIELD_PIN_A0
    #define ARDUINO_UNO_SHIELD_PIN_A0  GPIO2   // A0  /LED  /PWM / GPIO2  (ADC, boot-strapping, ook LED_BUILTIN)
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_A1
    #define ARDUINO_UNO_SHIELD_PIN_A1  GPIO4   // A1  /     /PWM / GPIO4  (ADC, boot-strapping)
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_A2
    #define ARDUINO_UNO_SHIELD_PIN_A2  GPIO35  // A2  /     /    / GPIO35 (ADC, enkel input)
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_A3
    #define ARDUINO_UNO_SHIELD_PIN_A3  GPIO34  // A3  /     /    / GPIO34 (ADC, enkel input)
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_A4
    #define ARDUINO_UNO_SHIELD_PIN_A4  GPIO36  // A4  /     /    / GPIO36 (ADC, enkel input)
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_A5
    #define ARDUINO_UNO_SHIELD_PIN_A5  GPIO39  // A5  /     /    / GPIO39 (ADC, enkel input)
  #endif

  #ifndef ARDUINO_UNO_SHIELD_PIN_SDA
    #define ARDUINO_UNO_SHIELD_PIN_SDA GPIO21  // SDA /     /PWM / GPIO21 (I2C, vrij)
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_SCL
    #define ARDUINO_UNO_SHIELD_PIN_SCL GPIO22  // SCL /     /PWM / GPIO22 (I2C, vrij)
  #endif

  #ifndef ARDUINO_UNO_SHIELD_PIN_SS
    #define ARDUINO_UNO_SHIELD_PIN_SS   GPIO5  // ARDUINO_UNO_SHIELD_PIN_D10
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_MOSI
    #define ARDUINO_UNO_SHIELD_PIN_MOSI GPIO23 // ARDUINO_UNO_SHIELD_PIN_D11
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_MISO
    #define ARDUINO_UNO_SHIELD_PIN_MISO GPIO19 // ARDUINO_UNO_SHIELD_PIN_D12
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_SCK
    #define ARDUINO_UNO_SHIELD_PIN_SCK  GPIO18 // ARDUINO_UNO_SHIELD_PIN_D13
  #endif

#elif BOARD_VERSION == BOARD_ESP32S3_ARDI32 // ESP32-S3-WROOM-1 - ARDI-32: pinmapping bevestigd tegen https://github.com/sbcshop/Ardi-32_Software
  #ifndef ARDUINO_UNO_SHIELD_PIN_D0
    #define ARDUINO_UNO_SHIELD_PIN_D0   GPIO18  // D0  / RX      / GPIO18
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D1
    #define ARDUINO_UNO_SHIELD_PIN_D1   GPIO17  // D1  / TX      / GPIO17
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D2
    #define ARDUINO_UNO_SHIELD_PIN_D2   GPIO1   // D2  / CS_2    / GPIO1
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D3
    #define ARDUINO_UNO_SHIELD_PIN_D3   GPIO42  // D3  / CLK_2   / GPIO42
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D4
    #define ARDUINO_UNO_SHIELD_PIN_D4   GPIO41  // D4  / MISO_2  / GPIO41
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D5
    #define ARDUINO_UNO_SHIELD_PIN_D5   GPIO2   // D5  / MOSI_2  / GPIO2
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D6
    #define ARDUINO_UNO_SHIELD_PIN_D6   GPIO9   // D6  / GPIO9   / GPIO9
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D7
    #define ARDUINO_UNO_SHIELD_PIN_D7   GPIO14  // D7  / GPIO14  / GPIO14
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D8
    #define ARDUINO_UNO_SHIELD_PIN_D8   GPIO47  // D8  / GPIO47  / GPIO47
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D9
    #define ARDUINO_UNO_SHIELD_PIN_D9   GPIO21  // D9  / GPIO21  / GPIO21
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D10
    #define ARDUINO_UNO_SHIELD_PIN_D10  GPIO10  // D10 / CS_1    / GPIO10
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D11
    #define ARDUINO_UNO_SHIELD_PIN_D11  GPIO11  // D11 / MOSI_1  / GPIO11
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D12
    #define ARDUINO_UNO_SHIELD_PIN_D12  GPIO13  // D12 / MISO_1  / GPIO13
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D13
    #define ARDUINO_UNO_SHIELD_PIN_D13  GPIO12  // D13 / CLK_1   / GPIO12
  #endif

  #ifndef ARDUINO_UNO_SHIELD_PIN_A0
    #define ARDUINO_UNO_SHIELD_PIN_A0   GPIO4   // A0 / A0 / GPIO4
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_A1
    #define ARDUINO_UNO_SHIELD_PIN_A1   GPIO5   // A1 / A3 / GPIO5
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_A2
    #define ARDUINO_UNO_SHIELD_PIN_A2   GPIO6   // A2 / A6 / GPIO6
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_A3
    #define ARDUINO_UNO_SHIELD_PIN_A3   GPIO7   // A3 / A7 / GPIO7
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_A4
    #define ARDUINO_UNO_SHIELD_PIN_A4   GPIO15  // A4 / A4 / GPIO15
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_A5
    #define ARDUINO_UNO_SHIELD_PIN_A5   GPIO16  // A5 / A5 / GPIO16
  #endif

  #ifndef ARDUINO_UNO_SHIELD_PIN_SDA
    #define ARDUINO_UNO_SHIELD_PIN_SDA  GPIO38  // SDA / SDA / GPIO38
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_SCL
    #define ARDUINO_UNO_SHIELD_PIN_SCL  GPIO39  // SCL / SCL / GPIO39
  #endif

  #ifndef ARDUINO_UNO_SHIELD_PIN_SS
    #define ARDUINO_UNO_SHIELD_PIN_SS   GPIO10  // ARDUINO_UNO_SHIELD_PIN_D10
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_MOSI
    #define ARDUINO_UNO_SHIELD_PIN_MOSI GPIO11  // ARDUINO_UNO_SHIELD_PIN_D11
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_MISO
    #define ARDUINO_UNO_SHIELD_PIN_MISO GPIO13  // ARDUINO_UNO_SHIELD_PIN_D12
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_SCK
    #define ARDUINO_UNO_SHIELD_PIN_SCK  GPIO12  // ARDUINO_UNO_SHIELD_PIN_D13
  #endif  

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
  #ifndef ARDUINO_UNO_SHIELD_PIN_D0
    #define ARDUINO_UNO_SHIELD_PIN_D0   GPIO44  // D0  / RX      / GPIO44
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D1
    #define ARDUINO_UNO_SHIELD_PIN_D1   GPIO43  // D1  / TX      / GPIO43
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D2
    #define ARDUINO_UNO_SHIELD_PIN_D2   GPIO18  // D2  /         / GPIO18
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D3
    #define ARDUINO_UNO_SHIELD_PIN_D3   GPIO17  // D3  / PWM     / GPIO17
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D4
    #define ARDUINO_UNO_SHIELD_PIN_D4   GPIO19  // D4  /         / GPIO19
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D5
    #define ARDUINO_UNO_SHIELD_PIN_D5   GPIO20  // D5  / PWM     / GPIO20
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D6
    #define ARDUINO_UNO_SHIELD_PIN_D6   GPIO3   // D6  / PWM     / GPIO3
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D7
    #define ARDUINO_UNO_SHIELD_PIN_D7   GPIO14  // D7  /         / GPIO14
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D8
    #define ARDUINO_UNO_SHIELD_PIN_D8   GPIO21  // D8  /         / GPIO21
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D9
    #define ARDUINO_UNO_SHIELD_PIN_D9   GPIO46  // D9  /         / GPIO46
  #endif

  #ifndef ARDUINO_UNO_SHIELD_PIN_D10
    #define ARDUINO_UNO_SHIELD_PIN_D10  GPIO10  // D10 / CS      / GPIO10
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D11
    #define ARDUINO_UNO_SHIELD_PIN_D11  GPIO11  // D11 / MOSI    / GPIO11
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D12
    #define ARDUINO_UNO_SHIELD_PIN_D12  GPIO13  // D12 / MISO    / GPIO13
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D13
    #define ARDUINO_UNO_SHIELD_PIN_D13  GPIO12  // D13 / CLK     / GPIO12
  #endif

  #ifndef ARDUINO_UNO_SHIELD_PIN_A0
    #define ARDUINO_UNO_SHIELD_PIN_A0   GPIO2   // A0  / A0      / GPIO2
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_A1
    #define ARDUINO_UNO_SHIELD_PIN_A1   GPIO1   // A1  / A1      / GPIO1
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_A2
    #define ARDUINO_UNO_SHIELD_PIN_A2   GPIO7   // A2  / A2      / GPIO7
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_A3
    #define ARDUINO_UNO_SHIELD_PIN_A3   GPIO6   // A3  / A3      / GPIO6
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_A4
    #define ARDUINO_UNO_SHIELD_PIN_A4   GPIO5   // A4  / A4      / GPIO5
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_A5
    #define ARDUINO_UNO_SHIELD_PIN_A5   GPIO4   // A5  / A5      / GPIO4
  #endif

  #ifndef ARDUINO_UNO_SHIELD_PIN_SDA
    #define ARDUINO_UNO_SHIELD_PIN_SDA  GPIO8   // SDA / SDA     / GPIO8
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_SCL
    #define ARDUINO_UNO_SHIELD_PIN_SCL  GPIO9   // SCL / SCL     / GPIO9
  #endif

  #ifndef ARDUINO_UNO_SHIELD_PIN_SS
    #define ARDUINO_UNO_SHIELD_PIN_SS   GPIO10  // ARDUINO_UNO_SHIELD_PIN_D10
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_MOSI
    #define ARDUINO_UNO_SHIELD_PIN_MOSI GPIO11  // ARDUINO_UNO_SHIELD_PIN_D11
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_MISO
    #define ARDUINO_UNO_SHIELD_PIN_MISO GPIO13  // ARDUINO_UNO_SHIELD_PIN_D12 
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_SCK
    #define ARDUINO_UNO_SHIELD_PIN_SCK  GPIO12  // ARDUINO_UNO_SHIELD_PIN_D13 
  #endif

#elif BOARD_VERSION == BOARD_RP2040_CYTRON_MAKER_UNO // CYTRON MAKER UNO RP2040: pinmapping bevestigd tegen officiële Zephyr-boarddocumentatie (https://docs.zephyrproject.org/latest/boards/cytron/maker_uno_rp2040/doc/index.html), bevestigd tegen eigen meting op het board
  #ifndef ARDUINO_UNO_SHIELD_PIN_D0
    #define ARDUINO_UNO_SHIELD_PIN_D0   GPIO1   // D0  / GP1  / GPIO1  (UART0 RX)
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D1
    #define ARDUINO_UNO_SHIELD_PIN_D1   GPIO0   // D1  / GP0  / GPIO0  (UART0 TX)
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D2
    #define ARDUINO_UNO_SHIELD_PIN_D2   GPIO2   // D2  / GP2  / GPIO2  (User button, pull-up)
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D3
    #define ARDUINO_UNO_SHIELD_PIN_D3   GPIO3   // D3  / GP3  / GPIO3  (Alias led0)
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D4
    #define ARDUINO_UNO_SHIELD_PIN_D4   GPIO4   // D4  / GP4  / GPIO4  (ook Grove 2)
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D5
    #define ARDUINO_UNO_SHIELD_PIN_D5   GPIO5   // D5  / GP5  / GPIO5  (ook Grove 2)
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D6
    #define ARDUINO_UNO_SHIELD_PIN_D6   GPIO6   // D6  / GP6  / GPIO6  (ook Grove 3)
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D7
    #define ARDUINO_UNO_SHIELD_PIN_D7   GPIO7   // D7  / GP7  / GPIO7  (ook Grove 4)
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D8
    #define ARDUINO_UNO_SHIELD_PIN_D8   GPIO8   // D8  / GP8  / GPIO8  (Buzzer)
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D9
    #define ARDUINO_UNO_SHIELD_PIN_D9   GPIO9   // D9  / GP9  / GPIO9  (geen andere toewijzing)
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D10
    #define ARDUINO_UNO_SHIELD_PIN_D10  GPIO13  // D10 / GP13 / GPIO13 (SPI1 CS,  SPI-header)
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D11
    #define ARDUINO_UNO_SHIELD_PIN_D11  GPIO11  // D11 / GP11 / GPIO11 (SPI1 MOSI, SPI-header)
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D12
    #define ARDUINO_UNO_SHIELD_PIN_D12  GPIO12  // D12 / GP12 / GPIO12 (SPI1 MISO, SPI-header)
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D13
    #define ARDUINO_UNO_SHIELD_PIN_D13  GPIO10  // D13 / GP10 / GPIO10 (SPI1 SCK,  SPI-header)
  #endif

  #ifndef ARDUINO_UNO_SHIELD_PIN_A0
    #define ARDUINO_UNO_SHIELD_PIN_A0   GPIO26  // A0  / GP26 / GPIO26 (ADC0, ook Grove 3)
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_A1
    #define ARDUINO_UNO_SHIELD_PIN_A1   GPIO27  // A1  / GP27 / GPIO27 (ADC1, ook Grove 4)
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_A2
    #define ARDUINO_UNO_SHIELD_PIN_A2   GPIO28  // A2  / GP28 / GPIO28 (ADC2, ook Grove 5)
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_A3
    #define ARDUINO_UNO_SHIELD_PIN_A3   GPIO29  // A3  / GP29 / GPIO29 (ADC3, ook Grove 5)
  #endif

  #ifndef ARDUINO_UNO_SHIELD_PIN_SDA
    #define ARDUINO_UNO_SHIELD_PIN_SDA  GPIO20  // SDA / GP20 / GPIO20 (I2C0 SDA, ook Grove 6/Maker)
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_SCL
    #define ARDUINO_UNO_SHIELD_PIN_SCL  GPIO21  // SCL / GP21 / GPIO21 (I2C0 SCL, ook Grove 6/Maker)
  #endif

  #ifndef ARDUINO_UNO_SHIELD_PIN_SS
    #define ARDUINO_UNO_SHIELD_PIN_SS   GPIO13  // ARDUINO_UNO_SHIELD_PIN_D10
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_MOSI
    #define ARDUINO_UNO_SHIELD_PIN_MOSI GPIO11  // ARDUINO_UNO_SHIELD_PIN_D11
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_MISO
    #define ARDUINO_UNO_SHIELD_PIN_MISO GPIO12  // ARDUINO_UNO_SHIELD_PIN_D12
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_SCK
    #define ARDUINO_UNO_SHIELD_PIN_SCK  GPIO10  // ARDUINO_UNO_SHIELD_PIN_D13
  #endif

#elif BOARD_VERSION == BOARD_STM32F4_NUCLEO64_F401RE // STMicroelectronics STM32F4 NUCLEO64 F401RE
  #ifndef ARDUINO_UNO_SHIELD_PIN_D0
    #define ARDUINO_UNO_SHIELD_PIN_D0   D0   // D0  / RX   / PA3
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D1
    #define ARDUINO_UNO_SHIELD_PIN_D1   D1   // D1  / TX   / PA2
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D2
    #define ARDUINO_UNO_SHIELD_PIN_D2   D2   // D2  /      / PA10
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D3
    #define ARDUINO_UNO_SHIELD_PIN_D3   D3   // D3  / PWM  / PB3
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D4
    #define ARDUINO_UNO_SHIELD_PIN_D4   D4   // D4  /      / PB5
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D5
    #define ARDUINO_UNO_SHIELD_PIN_D5   D5   // D5  / PWM  / PB4
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D6
    #define ARDUINO_UNO_SHIELD_PIN_D6   D6   // D6  / PWM  / PB10
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D7
    #define ARDUINO_UNO_SHIELD_PIN_D7   D7   // D7  /      / PA8
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D8
    #define ARDUINO_UNO_SHIELD_PIN_D8   D8   // D8  /      / PA9
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D9
    #define ARDUINO_UNO_SHIELD_PIN_D9   D9   // D9  / PWM  / PC7
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D10
    #define ARDUINO_UNO_SHIELD_PIN_D10  D10  // D10 / CS   / PB6 of PA6 (depending on config: PB6 default)
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D11
    #define ARDUINO_UNO_SHIELD_PIN_D11  D11  // D11 / MOSI / PA7
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D12
    #define ARDUINO_UNO_SHIELD_PIN_D12  D12  // D12 / MISO / PA6
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_D13
    #define ARDUINO_UNO_SHIELD_PIN_D13  D13  // D13 / SCK  / PA5
  #endif

  #ifndef ARDUINO_UNO_SHIELD_PIN_A0
    #define ARDUINO_UNO_SHIELD_PIN_A0   A0   // A0  /      / PA0
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_A1
    #define ARDUINO_UNO_SHIELD_PIN_A1   A1   // A1  /      / PA1
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_A2
    #define ARDUINO_UNO_SHIELD_PIN_A2   A2   // A2  /      / PA4
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_A3
    #define ARDUINO_UNO_SHIELD_PIN_A3   A3   // A3  /      / PB0
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_A4
    #define ARDUINO_UNO_SHIELD_PIN_A4   A4   // A4  /      / PC1
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_A5
    #define ARDUINO_UNO_SHIELD_PIN_A5   A5   // A5  /      / PC0
  #endif

  #ifndef ARDUINO_UNO_SHIELD_PIN_SDA
    #define ARDUINO_UNO_SHIELD_PIN_SDA  SDA  // D14 / SDA  / PB9
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_SCL
    #define ARDUINO_UNO_SHIELD_PIN_SCL  SCL  // D15 / SCL  / PB8
  #endif

  #ifndef ARDUINO_UNO_SHIELD_PIN_SS
    #define ARDUINO_UNO_SHIELD_PIN_SS   PIN_SPI_SS    // ARDUINO_UNO_SHIELD_PIN_D10
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_MOSI
    #define ARDUINO_UNO_SHIELD_PIN_MOSI PIN_SPI_MOSI  // ARDUINO_UNO_SHIELD_PIN_D11
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_MISO
    #define ARDUINO_UNO_SHIELD_PIN_MISO PIN_SPI_MISO  // ARDUINO_UNO_SHIELD_PIN_D12
  #endif
  #ifndef ARDUINO_UNO_SHIELD_PIN_SCK
    #define ARDUINO_UNO_SHIELD_PIN_SCK  PIN_SPI_SCK   // ARDUINO_UNO_SHIELD_PIN_D13
  #endif

#endif

// ============================================================================
// EXTENDERS
// ============================================================================
// Elke EXTENDER_<NAAM>_AANTAL bepaalt hoeveel instanties er zijn; elke adres-/CS-macro staat achter zijn eigen if op dat aantal, 
// zodat er nooit een macro bestaat voor een instantie die niet actief is.
// DEFAULT: EXTENDER_<NAAM>_AANTAL = 0 of 1.
// EXPERIMENTEEL: EXTENDER_<NAAM>_AANTAL >= 2.
// ============================================================================

// --- I2C-Extenders ---
// SDA en SCL zijn GEDEELDE native resources en komen uit de boardconfiguratie.
// Elk I2C-adres hieronder is EXCLUSIEF op dezelfde I2C-bus.
// Extra native aansluitingen (INT, IRQ, RESET, ALERT_RDY, ...) zijn EXCLUSIEF
// wanneer ze hieronder per extender-instance als HardwareResourcePin worden ingesteld.
#ifndef EXTENDER_ADS1115_AANTAL
  #define EXTENDER_ADS1115_AANTAL 0
#endif
// ============================================================================
// DEFAULT — ADS1115 #1
// ============================================================================
#if EXTENDER_ADS1115_AANTAL >= 1
  // EXCLUSIEF: I2C-adres van deze extender-instance op dezelfde I2C-bus.
  #ifndef I2C_ADDRESS_EXTENDER_ADS1115_1
    #define I2C_ADDRESS_EXTENDER_ADS1115_1 0x48
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_ADS1115_1_ALERT_RDY
    #define EXTENDER_ADS1115_1_ALERT_RDY HardwareResourcePin::NONE
  #endif
#endif
// ============================================================================
// EXPERIMENTEEL — ADS1115 #2
// ============================================================================
#if EXTENDER_ADS1115_AANTAL >= 2
  // EXCLUSIEF: I2C-adres van deze extender-instance op dezelfde I2C-bus.
  #ifndef I2C_ADDRESS_EXTENDER_ADS1115_2
    #define I2C_ADDRESS_EXTENDER_ADS1115_2 0x49
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_ADS1115_2_ALERT_RDY
    #define EXTENDER_ADS1115_2_ALERT_RDY HardwareResourcePin::NONE
  #endif
#endif

#ifndef EXTENDER_MCP23017_AANTAL
  #define EXTENDER_MCP23017_AANTAL 0
#endif
// ============================================================================
// DEFAULT — MCP23017 #1
// ============================================================================
#if EXTENDER_MCP23017_AANTAL >= 1
  // EXCLUSIEF: I2C-adres van deze extender-instance op dezelfde I2C-bus.
  #ifndef I2C_ADDRESS_EXTENDER_MCP23017_1
    #define I2C_ADDRESS_EXTENDER_MCP23017_1 0x20
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_MCP23017_1_INTA
    #define EXTENDER_MCP23017_1_INTA HardwareResourcePin::NONE
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_MCP23017_1_INTB
    #define EXTENDER_MCP23017_1_INTB HardwareResourcePin::NONE
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_MCP23017_1_RESET
    #define EXTENDER_MCP23017_1_RESET HardwareResourcePin::NONE
  #endif
#endif
// ============================================================================
// EXPERIMENTEEL — MCP23017 #2
// ============================================================================
#if EXTENDER_MCP23017_AANTAL >= 2
  // EXCLUSIEF: I2C-adres van deze extender-instance op dezelfde I2C-bus.
  #ifndef I2C_ADDRESS_EXTENDER_MCP23017_2
    #define I2C_ADDRESS_EXTENDER_MCP23017_2 0x21
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_MCP23017_2_INTA
    #define EXTENDER_MCP23017_2_INTA HardwareResourcePin::NONE
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_MCP23017_2_INTB
    #define EXTENDER_MCP23017_2_INTB HardwareResourcePin::NONE
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_MCP23017_2_RESET
    #define EXTENDER_MCP23017_2_RESET HardwareResourcePin::NONE
  #endif
#endif

#ifndef EXTENDER_PCF8574_AANTAL
  #define EXTENDER_PCF8574_AANTAL 0
#endif
// ============================================================================
// DEFAULT — PCF8574 #1
// ============================================================================
#if EXTENDER_PCF8574_AANTAL >= 1
  // EXCLUSIEF: I2C-adres van deze extender-instance op dezelfde I2C-bus.
  #ifndef I2C_ADDRESS_EXTENDER_PCF8574_1
    #define I2C_ADDRESS_EXTENDER_PCF8574_1 0x20
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_PCF8574_1_INT
    #define EXTENDER_PCF8574_1_INT HardwareResourcePin::NONE
  #endif
#endif
// ============================================================================
// EXPERIMENTEEL — PCF8574 #2
// ============================================================================
#if EXTENDER_PCF8574_AANTAL >= 2
  // EXCLUSIEF: I2C-adres van deze extender-instance op dezelfde I2C-bus.
  #ifndef I2C_ADDRESS_EXTENDER_PCF8574_2
    #define I2C_ADDRESS_EXTENDER_PCF8574_2 0x21
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_PCF8574_2_INT
    #define EXTENDER_PCF8574_2_INT HardwareResourcePin::NONE
  #endif
#endif

#ifndef EXTENDER_PCF8575_AANTAL
  #define EXTENDER_PCF8575_AANTAL 0
#endif
// ============================================================================
// DEFAULT — PCF8575 #1
// ============================================================================
#if EXTENDER_PCF8575_AANTAL >= 1
  // EXCLUSIEF: I2C-adres van deze extender-instance op dezelfde I2C-bus.
  #ifndef I2C_ADDRESS_EXTENDER_PCF8575_1
    #define I2C_ADDRESS_EXTENDER_PCF8575_1 0x20
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_PCF8575_1_INT
    #define EXTENDER_PCF8575_1_INT HardwareResourcePin::NONE
  #endif
#endif
// ============================================================================
// EXPERIMENTEEL — PCF8575 #2
// ============================================================================
#if EXTENDER_PCF8575_AANTAL >= 2
  // EXCLUSIEF: I2C-adres van deze extender-instance op dezelfde I2C-bus.
  #ifndef I2C_ADDRESS_EXTENDER_PCF8575_2
    #define I2C_ADDRESS_EXTENDER_PCF8575_2 0x21
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_PCF8575_2_INT
    #define EXTENDER_PCF8575_2_INT HardwareResourcePin::NONE
  #endif
#endif

#ifndef EXTENDER_TCA9548A_AANTAL
  #define EXTENDER_TCA9548A_AANTAL 0
#endif
// ============================================================================
// DEFAULT — TCA9548A #1
// ============================================================================
#if EXTENDER_TCA9548A_AANTAL >= 1
  // EXCLUSIEF: I2C-adres van deze extender-instance op dezelfde I2C-bus.
  #ifndef I2C_ADDRESS_EXTENDER_TCA9548A_1
    #define I2C_ADDRESS_EXTENDER_TCA9548A_1 0x70
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_TCA9548A_1_RESET
    #define EXTENDER_TCA9548A_1_RESET HardwareResourcePin::NONE
  #endif
#endif
// ============================================================================
// EXPERIMENTEEL — TCA9548A #2
// ============================================================================
#if EXTENDER_TCA9548A_AANTAL >= 2
  // EXCLUSIEF: I2C-adres van deze extender-instance op dezelfde I2C-bus.
  #ifndef I2C_ADDRESS_EXTENDER_TCA9548A_2
    #define I2C_ADDRESS_EXTENDER_TCA9548A_2 0x71
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_TCA9548A_2_RESET
    #define EXTENDER_TCA9548A_2_RESET HardwareResourcePin::NONE
  #endif
#endif

#ifndef EXTENDER_ADS7828_AANTAL
  #define EXTENDER_ADS7828_AANTAL 0
#endif
// ============================================================================
// DEFAULT — ADS7828 #1
// ============================================================================
#if EXTENDER_ADS7828_AANTAL >= 1
  // EXCLUSIEF: I2C-adres van deze extender-instance op dezelfde I2C-bus.
  #ifndef I2C_ADDRESS_EXTENDER_ADS7828_1
    #define I2C_ADDRESS_EXTENDER_ADS7828_1 0x48
  #endif
#endif
// ============================================================================
// EXPERIMENTEEL — ADS7828 #2
// ============================================================================
#if EXTENDER_ADS7828_AANTAL >= 2
  // EXCLUSIEF: I2C-adres van deze extender-instance op dezelfde I2C-bus.
  #ifndef I2C_ADDRESS_EXTENDER_ADS7828_2
    #define I2C_ADDRESS_EXTENDER_ADS7828_2 0x49
  #endif
#endif

#ifndef EXTENDER_DS2482_800_AANTAL
  #define EXTENDER_DS2482_800_AANTAL 0
#endif
// ============================================================================
// DEFAULT — DS2482-800 #1
// ============================================================================
#if EXTENDER_DS2482_800_AANTAL >= 1
  // EXCLUSIEF: I2C-adres van deze extender-instance op dezelfde I2C-bus.
  #ifndef I2C_ADDRESS_EXTENDER_DS2482_800_1
    #define I2C_ADDRESS_EXTENDER_DS2482_800_1 0x18
  #endif
#endif
// ============================================================================
// EXPERIMENTEEL — DS2482-800 #2
// ============================================================================
#if EXTENDER_DS2482_800_AANTAL >= 2
  // EXCLUSIEF: I2C-adres van deze extender-instance op dezelfde I2C-bus.
  #ifndef I2C_ADDRESS_EXTENDER_DS2482_800_2
    #define I2C_ADDRESS_EXTENDER_DS2482_800_2 0x19
  #endif
#endif

// --- SPI-Extenders (CS-pin i.p.v. adres) ---
// SCK, MISO en MOSI zijn GEDEELDE native resources en komen uit de boardconfiguratie.
// CS is per extender-instance EXCLUSIEF en moet expliciet op een vrije HardwareResourcePin worden ingesteld.
// Extra native aansluitingen (IRQ, RESET, START, PWDN, GPIO..., ...) zijn EXCLUSIEF
// wanneer ze hieronder per extender-instance als HardwareResourcePin worden ingesteld.
#ifndef EXTENDER_ADS1158_AANTAL
  #define EXTENDER_ADS1158_AANTAL 0
#endif
// ============================================================================
// DEFAULT — ADS1158 #1
// ============================================================================
#if EXTENDER_ADS1158_AANTAL >= 1
  // EXCLUSIEF: CS van deze extender-instance; kies een vrije HardwareResourcePin.
  #ifndef CS_PIN_EXTENDER_ADS1158_1
    #define CS_PIN_EXTENDER_ADS1158_1 HardwareResourcePin::NONE
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_ADS1158_1_PWDN
    #define EXTENDER_ADS1158_1_PWDN HardwareResourcePin::NONE
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_ADS1158_1_RESET
    #define EXTENDER_ADS1158_1_RESET HardwareResourcePin::NONE
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_ADS1158_1_START
    #define EXTENDER_ADS1158_1_START HardwareResourcePin::NONE
  #endif
#endif
// ============================================================================
// EXPERIMENTEEL — ADS1158 #2
// ============================================================================
#if EXTENDER_ADS1158_AANTAL >= 2
  // EXCLUSIEF: CS van deze extender-instance; kies een vrije HardwareResourcePin.
  #ifndef CS_PIN_EXTENDER_ADS1158_2
    #define CS_PIN_EXTENDER_ADS1158_2 HardwareResourcePin::NONE
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_ADS1158_2_PWDN
    #define EXTENDER_ADS1158_2_PWDN HardwareResourcePin::NONE
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_ADS1158_2_RESET
    #define EXTENDER_ADS1158_2_RESET HardwareResourcePin::NONE
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_ADS1158_2_START
    #define EXTENDER_ADS1158_2_START HardwareResourcePin::NONE
  #endif
#endif

#ifndef EXTENDER_ADS7953_AANTAL
  #define EXTENDER_ADS7953_AANTAL 0
#endif
// ============================================================================
// DEFAULT — ADS7953 #1
// ============================================================================
#if EXTENDER_ADS7953_AANTAL >= 1
  // EXCLUSIEF: CS van deze extender-instance; kies een vrije HardwareResourcePin.
  #ifndef CS_PIN_EXTENDER_ADS7953_1
    #define CS_PIN_EXTENDER_ADS7953_1 HardwareResourcePin::NONE
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_ADS7953_GPIO0_TO_UNO_1
    #define EXTENDER_ADS7953_GPIO0_TO_UNO_1 HardwareResourcePin::D5
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_ADS7953_GPIO1_TO_UNO_1
    #define EXTENDER_ADS7953_GPIO1_TO_UNO_1 HardwareResourcePin::D6
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_ADS7953_GPIO2_TO_UNO_1
    #define EXTENDER_ADS7953_GPIO2_TO_UNO_1 HardwareResourcePin::NONE
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_ADS7953_GPIO3_TO_UNO_1
    #define EXTENDER_ADS7953_GPIO3_TO_UNO_1 HardwareResourcePin::NONE
  #endif
#endif
// ============================================================================
// EXPERIMENTEEL — ADS7953 #2
// ============================================================================
#if EXTENDER_ADS7953_AANTAL >= 2
  // EXCLUSIEF: CS van deze extender-instance; kies een vrije HardwareResourcePin.
  #ifndef CS_PIN_EXTENDER_ADS7953_2
    #define CS_PIN_EXTENDER_ADS7953_2 HardwareResourcePin::NONE
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_ADS7953_GPIO0_TO_UNO_2
    #define EXTENDER_ADS7953_GPIO0_TO_UNO_2 HardwareResourcePin::D7
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_ADS7953_GPIO1_TO_UNO_2
    #define EXTENDER_ADS7953_GPIO1_TO_UNO_2 HardwareResourcePin::D8
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_ADS7953_GPIO2_TO_UNO_2
    #define EXTENDER_ADS7953_GPIO2_TO_UNO_2 HardwareResourcePin::NONE
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_ADS7953_GPIO3_TO_UNO_2
    #define EXTENDER_ADS7953_GPIO3_TO_UNO_2 HardwareResourcePin::NONE
  #endif
#endif

// --- Geen protocol (native select-pinnen, geen adres/CS) ---
// EN, S0, S1, S2, S3 en Z zijn per extender-instance EXCLUSIEVE native resources.
#ifndef EXTENDER_CD74HC4067_AANTAL
  #define EXTENDER_CD74HC4067_AANTAL 0
#endif
// ============================================================================
// DEFAULT — CD74HC4067 #1
// ============================================================================
#if EXTENDER_CD74HC4067_AANTAL >= 1
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_CD74HC4067_1_EN
    #define EXTENDER_CD74HC4067_1_EN HardwareResourcePin::NONE
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_CD74HC4067_1_S0
    #define EXTENDER_CD74HC4067_1_S0 HardwareResourcePin::NONE
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_CD74HC4067_1_S1
    #define EXTENDER_CD74HC4067_1_S1 HardwareResourcePin::NONE
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_CD74HC4067_1_S2
    #define EXTENDER_CD74HC4067_1_S2 HardwareResourcePin::NONE
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_CD74HC4067_1_S3
    #define EXTENDER_CD74HC4067_1_S3 HardwareResourcePin::NONE
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_CD74HC4067_1_Z
    #define EXTENDER_CD74HC4067_1_Z HardwareResourcePin::NONE
  #endif
#endif
// ============================================================================
// EXPERIMENTEEL — CD74HC4067 #2
// ============================================================================
#if EXTENDER_CD74HC4067_AANTAL >= 2
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_CD74HC4067_2_EN
    #define EXTENDER_CD74HC4067_2_EN HardwareResourcePin::NONE
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_CD74HC4067_2_S0
    #define EXTENDER_CD74HC4067_2_S0 HardwareResourcePin::NONE
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_CD74HC4067_2_S1
    #define EXTENDER_CD74HC4067_2_S1 HardwareResourcePin::NONE
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_CD74HC4067_2_S2
    #define EXTENDER_CD74HC4067_2_S2 HardwareResourcePin::NONE
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_CD74HC4067_2_S3
    #define EXTENDER_CD74HC4067_2_S3 HardwareResourcePin::NONE
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_CD74HC4067_2_Z
    #define EXTENDER_CD74HC4067_2_Z HardwareResourcePin::NONE
  #endif
#endif

// --- I2C-of-SPI-keuze door de aanmelder (twee tellingen mogelijk) ---
// I2C: SDA/SCL zijn GEDEELD; het I2C-adres en de extra native aansluitingen zijn EXCLUSIEF.
// SPI: SCK/MISO/MOSI zijn GEDEELD; CS en de extra native aansluitingen zijn EXCLUSIEF.
#ifndef EXTENDER_MAX14830_I2C_AANTAL
  #define EXTENDER_MAX14830_I2C_AANTAL 0
#endif
#ifndef EXTENDER_MAX14830_SPI_AANTAL
  #define EXTENDER_MAX14830_SPI_AANTAL 0
#endif
// ============================================================================
// DEFAULT — MAX14830 SPI #1
// ============================================================================
#if EXTENDER_MAX14830_SPI_AANTAL >= 1
  // EXCLUSIEF: CS van deze extender-instance; kies een vrije HardwareResourcePin.
  #ifndef CS_PIN_EXTENDER_MAX14830_SPI_1
    #define CS_PIN_EXTENDER_MAX14830_SPI_1 HardwareResourcePin::NONE
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_MAX14830_SPI_1_IRQ
    #define EXTENDER_MAX14830_SPI_1_IRQ HardwareResourcePin::NONE
  #endif
#endif
// ============================================================================
// EXPERIMENTEEL — MAX14830 SPI #2
// ============================================================================
#if EXTENDER_MAX14830_SPI_AANTAL >= 2
  // EXCLUSIEF: CS van deze extender-instance; kies een vrije HardwareResourcePin.
  #ifndef CS_PIN_EXTENDER_MAX14830_SPI_2
    #define CS_PIN_EXTENDER_MAX14830_SPI_2 HardwareResourcePin::NONE
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_MAX14830_SPI_2_IRQ
    #define EXTENDER_MAX14830_SPI_2_IRQ HardwareResourcePin::NONE
  #endif
#endif
// ============================================================================
// DEFAULT — MAX14830 I2C #1
// ============================================================================
#if EXTENDER_MAX14830_I2C_AANTAL >= 1
  // EXCLUSIEF: I2C-adres van deze extender-instance op dezelfde I2C-bus.
  #ifndef I2C_ADDRESS_EXTENDER_MAX14830_I2C_1
    #define I2C_ADDRESS_EXTENDER_MAX14830_I2C_1 0x60
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_MAX14830_I2C_1_IRQ
    #define EXTENDER_MAX14830_I2C_1_IRQ HardwareResourcePin::NONE
  #endif
#endif
// ============================================================================
// EXPERIMENTEEL — MAX14830 I2C #2
// ============================================================================
#if EXTENDER_MAX14830_I2C_AANTAL >= 2
  // EXCLUSIEF: I2C-adres van deze extender-instance op dezelfde I2C-bus.
  #ifndef I2C_ADDRESS_EXTENDER_MAX14830_I2C_2
    #define I2C_ADDRESS_EXTENDER_MAX14830_I2C_2 0x61
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_MAX14830_I2C_2_IRQ
    #define EXTENDER_MAX14830_I2C_2_IRQ HardwareResourcePin::NONE
  #endif
#endif

#ifndef EXTENDER_SC16IS752_I2C_AANTAL
  #define EXTENDER_SC16IS752_I2C_AANTAL 0
#endif
// ============================================================================
// DEFAULT — SC16IS752 I2C #1
// ============================================================================
#if EXTENDER_SC16IS752_I2C_AANTAL >= 1
  // EXCLUSIEF: I2C-adres van deze extender-instance op dezelfde I2C-bus.
  #ifndef I2C_ADDRESS_EXTENDER_SC16IS752_I2C_1
    #define I2C_ADDRESS_EXTENDER_SC16IS752_I2C_1 0x48
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_SC16IS752_I2C_1_IRQ
    #define EXTENDER_SC16IS752_I2C_1_IRQ HardwareResourcePin::NONE
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_SC16IS752_I2C_1_RESET
    #define EXTENDER_SC16IS752_I2C_1_RESET HardwareResourcePin::NONE
  #endif
#endif
// ============================================================================
// EXPERIMENTEEL — SC16IS752 I2C #2
// ============================================================================
#if EXTENDER_SC16IS752_I2C_AANTAL >= 2
  // EXCLUSIEF: I2C-adres van deze extender-instance op dezelfde I2C-bus.
  #ifndef I2C_ADDRESS_EXTENDER_SC16IS752_I2C_2
    #define I2C_ADDRESS_EXTENDER_SC16IS752_I2C_2 0x49
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_SC16IS752_I2C_2_IRQ
    #define EXTENDER_SC16IS752_I2C_2_IRQ HardwareResourcePin::NONE
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_SC16IS752_I2C_2_RESET
    #define EXTENDER_SC16IS752_I2C_2_RESET HardwareResourcePin::NONE
  #endif
#endif
#ifndef EXTENDER_SC16IS752_SPI_AANTAL
  #define EXTENDER_SC16IS752_SPI_AANTAL 0
#endif
// ============================================================================
// DEFAULT — SC16IS752 SPI #1
// ============================================================================
#if EXTENDER_SC16IS752_SPI_AANTAL >= 1
  // EXCLUSIEF: CS van deze extender-instance; kies een vrije HardwareResourcePin.
  #ifndef CS_PIN_EXTENDER_SC16IS752_SPI_1
    #define CS_PIN_EXTENDER_SC16IS752_SPI_1 HardwareResourcePin::NONE
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_SC16IS752_SPI_1_IRQ
    #define EXTENDER_SC16IS752_SPI_1_IRQ HardwareResourcePin::NONE
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_SC16IS752_SPI_1_RESET
    #define EXTENDER_SC16IS752_SPI_1_RESET HardwareResourcePin::NONE
  #endif
#endif
// ============================================================================
// EXPERIMENTEEL — SC16IS752 SPI #2
// ============================================================================
#if EXTENDER_SC16IS752_SPI_AANTAL >= 2
  // EXCLUSIEF: CS van deze extender-instance; kies een vrije HardwareResourcePin.
  #ifndef CS_PIN_EXTENDER_SC16IS752_SPI_2
    #define CS_PIN_EXTENDER_SC16IS752_SPI_2 HardwareResourcePin::NONE
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_SC16IS752_SPI_2_IRQ
    #define EXTENDER_SC16IS752_SPI_2_IRQ HardwareResourcePin::NONE
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_SC16IS752_SPI_2_RESET
    #define EXTENDER_SC16IS752_SPI_2_RESET HardwareResourcePin::NONE
  #endif
#endif

// ============================================================================
// ============================================================================
// ============================================================================

// ============================================================================
// ADC BACKEND
// ============================================================================

// Definieer de pinnen voor de sensoren op jouw sensorboard
#if ADC_BACKEND == ADC_BACKEND_ADS1115
  #ifndef ADC_PIN_SENSOR_1
    #define ADC_PIN_SENSOR_1 0   // ADS1115-kanaal 0
  #endif
  #ifndef ADC_PIN_SENSOR_2
    #define ADC_PIN_SENSOR_2 1   // ADS1115-kanaal 1
  #endif
  #ifndef ADC_PIN_SENSOR_3
    #define ADC_PIN_SENSOR_3 2   // ADS1115-kanaal 2
  #endif
  #ifndef ADC_PIN_SENSOR_4
    #define ADC_PIN_SENSOR_4 3   // ADS1115-kanaal 3
  #endif
#else
  #ifndef ADC_PIN_SENSOR_1
    #define ADC_PIN_SENSOR_1 HardwareResourcePin::A0  // standaard waarde: HardwareResourcePin::A0, Analoge pin voor de 1ste test-sensor
  #endif
  #ifndef ADC_PIN_SENSOR_2
    #define ADC_PIN_SENSOR_2 HardwareResourcePin::A1  // standaard waarde: HardwareResourcePin::A1, Analoge pin voor de 2de test-sensor
  #endif
  #ifndef ADC_PIN_SENSOR_3
    #define ADC_PIN_SENSOR_3 HardwareResourcePin::A2  // standaard waarde: HardwareResourcePin::A2, Analoge pin voor de 3de test-sensor
  #endif
  #ifndef ADC_PIN_SENSOR_4
    #define ADC_PIN_SENSOR_4 HardwareResourcePin::A3  // standaard waarde: HardwareResourcePin::A3, Analoge pin voor de 4de test-sensor
  #endif
#endif

#ifndef ADC_BITS
  #if ADC_BACKEND == ADC_BACKEND_ADS1115
    #define ADC_BITS 15      // ADS1115 single-ended: 0-32767 bruikbaar bereik
  #elif BOARD_VERSION == BOARD_UNO_R3
    #define ADC_BITS 10      // R3 = 10
  #elif (BOARD_VERSION == BOARD_UNO_R4_MINIMA || BOARD_VERSION == BOARD_UNO_R4_WIFI)
    #define ADC_BITS 14      // R4 = 10 of 14
  #elif BOARD_VERSION == BOARD_UNO_Q
    #define ADC_BITS 10      // Arduino Zephyr core: analogReadResolution() standaard 10-bit
  #elif BOARD_VERSION == BOARD_ESP32S3_ARDI32
    #define ADC_BITS 12      // ESP32-S3 Arduino core: analogRead() standaard 12-bit
  #elif BOARD_VERSION == BOARD_ESP32S3_DEV
    #define ADC_BITS 12      // ESP32-S3 Arduino core: analogRead() standaard 12-bit
  #elif BOARD_VERSION == BOARD_ESP32_D1_UNO_R32
    #define ADC_BITS 12      // zie kanttekening in docs/Configuratie/SystemConfig.md
  #elif BOARD_VERSION == BOARD_RP2040_CYTRON_MAKER_UNO
    #define ADC_BITS 10      // Earle Philhower RP2040 core: analogReadResolution() standaard 10-bit
  #elif BOARD_VERSION == BOARD_STM32F4_NUCLEO64_F401RE
    #define ADC_BITS 10      // STM32duino: analogRead() standaard 10-bit voor Arduino-compatibiliteit
  #endif
#endif

#ifndef DELAY_US
  #if ADC_BACKEND == ADC_BACKEND_ADS1115
    #define DELAY_US 0     // niet van toepassing; I2C-conversietijd zit in de leesfunctie zelf
  #elif BOARD_VERSION == BOARD_UNO_R3
    #define DELAY_US 100   // Vertraging tussen samples in de meetlussen
  #elif (BOARD_VERSION == BOARD_UNO_R4_MINIMA || BOARD_VERSION == BOARD_UNO_R4_WIFI)
    #define DELAY_US 200   // Vertraging tussen samples in de meetlussen
  #elif BOARD_VERSION == BOARD_UNO_Q
    #define DELAY_US 0     // Arduino Zephyr core: analogRead() voert de ADC-conversie synchroon uit
  #elif BOARD_VERSION == BOARD_ESP32S3_ARDI32
    #define DELAY_US 0
  #elif BOARD_VERSION == BOARD_ESP32S3_DEV
    #define DELAY_US 0
  #elif BOARD_VERSION == BOARD_ESP32_D1_UNO_R32
    #define DELAY_US 0     // zie kanttekening in docs/Configuratie/SystemConfig.md
  #elif BOARD_VERSION == BOARD_RP2040_CYTRON_MAKER_UNO
    #define DELAY_US 0
  #elif BOARD_VERSION == BOARD_STM32F4_NUCLEO64_F401RE
    #define DELAY_US 0
  #endif
#endif

#define ADC_MAX ((1UL << ADC_BITS) - 1UL)

#if ADC_BITS == 10
  #define ADC(x) (x)
#elif ADC_BITS == 12 || ADC_BITS == 14 || ADC_BITS == 15
  #define ADC(x) (((x) * ADC_MAX) / 1023UL)
#else
  #error ADC_BITS moet 10, 12, 14 of 15 zijn.
#endif

// ============================================================================
// INPUT
// ============================================================================

// INPUT_TYPE_DIGITAL ---------------------------------------------------------
#if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_DIGITAL)
  // DIGITAL: mapping van keypadlabels naar Arduino Uno-shieldheaderpinnen.
  // De Arduino Uno-shieldpinnen blijven ARDUINO_UNO_SHIELD_PIN_D2..D5.
  // De INPUT_KEYPAD_PIN_*-namen volgen het gekozen keypadtype:
  // - drukknoppen direct: K1..K4
  // - drukknoppen matrix: L/R- of R/C-lijnen
  // - membraan direct: 1..4
  // - touch direct: OUT1..OUT4
  // Wanneer de gebruiker één INPUT_KEYPAD_PIN_* definieert, moeten alle vier pinnen van dat keypadtype gedefinieerd zijn.
  #if defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_DIRECT_1x4
    #if defined(INPUT_KEYPAD_PIN_K1) || defined(INPUT_KEYPAD_PIN_K2) || defined(INPUT_KEYPAD_PIN_K3) || defined(INPUT_KEYPAD_PIN_K4)
      #if !defined(INPUT_KEYPAD_PIN_K1) || !defined(INPUT_KEYPAD_PIN_K2) || !defined(INPUT_KEYPAD_PIN_K3) || !defined(INPUT_KEYPAD_PIN_K4)
        #error "INPUT_KEYPAD_PIN_K1 t/m INPUT_KEYPAD_PIN_K4 moeten samen gedefinieerd worden."
      #endif
    #else
      #define INPUT_KEYPAD_PIN_K1 HardwareResourcePin::D2
      #define INPUT_KEYPAD_PIN_K2 HardwareResourcePin::D3
      #define INPUT_KEYPAD_PIN_K3 HardwareResourcePin::D4
      #define INPUT_KEYPAD_PIN_K4 HardwareResourcePin::D5
    #endif
  #elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_MATRIX_2x2
    #if defined(INPUT_KEYPAD_PIN_L1) || defined(INPUT_KEYPAD_PIN_L2) || defined(INPUT_KEYPAD_PIN_R1) || defined(INPUT_KEYPAD_PIN_R2)
      #if !defined(INPUT_KEYPAD_PIN_L1) || !defined(INPUT_KEYPAD_PIN_L2) || !defined(INPUT_KEYPAD_PIN_R1) || !defined(INPUT_KEYPAD_PIN_R2)
        #error "INPUT_KEYPAD_PIN_L1, INPUT_KEYPAD_PIN_L2, INPUT_KEYPAD_PIN_R1 en INPUT_KEYPAD_PIN_R2 moeten samen gedefinieerd worden."
      #endif
    #else
      #define INPUT_KEYPAD_PIN_L1 HardwareResourcePin::D2
      #define INPUT_KEYPAD_PIN_L2 HardwareResourcePin::D3
      #define INPUT_KEYPAD_PIN_R1 HardwareResourcePin::D4
      #define INPUT_KEYPAD_PIN_R2 HardwareResourcePin::D5
    #endif
  #elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4
    // De gedraaide D3/D2/D5/D4-volgorde hoort bij het membraan-directtype dat ik ter beschikking had om te testen.
    // Deze softwaremapping volgt de vaste flatkabelvolgorde, zodat er geen draden fysiek gewisseld moeten worden.
    #if defined(INPUT_KEYPAD_PIN_1) || defined(INPUT_KEYPAD_PIN_2) || defined(INPUT_KEYPAD_PIN_3) || defined(INPUT_KEYPAD_PIN_4)
      #if !defined(INPUT_KEYPAD_PIN_1) || !defined(INPUT_KEYPAD_PIN_2) || !defined(INPUT_KEYPAD_PIN_3) || !defined(INPUT_KEYPAD_PIN_4)
        #error "INPUT_KEYPAD_PIN_1 t/m INPUT_KEYPAD_PIN_4 moeten samen gedefinieerd worden."
      #endif
    #else
      #define INPUT_KEYPAD_PIN_1 HardwareResourcePin::D3
      #define INPUT_KEYPAD_PIN_2 HardwareResourcePin::D2
      #define INPUT_KEYPAD_PIN_3 HardwareResourcePin::D5
      #define INPUT_KEYPAD_PIN_4 HardwareResourcePin::D4
    #endif
  #elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_DIRECT_4x1
    // De gedraaide D3/D2/D5/D4-volgorde hoort bij het membraan-directtype dat ik ter beschikking had om te testen.
    // Deze softwaremapping volgt de vaste flatkabelvolgorde, zodat er geen draden fysiek gewisseld moeten worden.
    #if defined(INPUT_KEYPAD_PIN_1) || defined(INPUT_KEYPAD_PIN_2) || defined(INPUT_KEYPAD_PIN_3) || defined(INPUT_KEYPAD_PIN_4)
      #if !defined(INPUT_KEYPAD_PIN_1) || !defined(INPUT_KEYPAD_PIN_2) || !defined(INPUT_KEYPAD_PIN_3) || !defined(INPUT_KEYPAD_PIN_4)
        #error "INPUT_KEYPAD_PIN_1 t/m INPUT_KEYPAD_PIN_4 moeten samen gedefinieerd worden."
      #endif
    #else    
      #define INPUT_KEYPAD_PIN_1 HardwareResourcePin::D3
      #define INPUT_KEYPAD_PIN_2 HardwareResourcePin::D2
      #define INPUT_KEYPAD_PIN_3 HardwareResourcePin::D5
      #define INPUT_KEYPAD_PIN_4 HardwareResourcePin::D4
    #endif
  #elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_TOUCH_TTP224_DIRECT_1x4
    #if defined(INPUT_KEYPAD_PIN_OUT1) || defined(INPUT_KEYPAD_PIN_OUT2) || defined(INPUT_KEYPAD_PIN_OUT3) || defined(INPUT_KEYPAD_PIN_OUT4)
      #if !defined(INPUT_KEYPAD_PIN_OUT1) || !defined(INPUT_KEYPAD_PIN_OUT2) || !defined(INPUT_KEYPAD_PIN_OUT3) || !defined(INPUT_KEYPAD_PIN_OUT4)
        #error "INPUT_KEYPAD_PIN_OUT1 t/m INPUT_KEYPAD_PIN_OUT4 moeten samen gedefinieerd worden."
      #endif
    #else
      #define INPUT_KEYPAD_PIN_OUT1 HardwareResourcePin::D2
      #define INPUT_KEYPAD_PIN_OUT2 HardwareResourcePin::D3
      #define INPUT_KEYPAD_PIN_OUT3 HardwareResourcePin::D4
      #define INPUT_KEYPAD_PIN_OUT4 HardwareResourcePin::D5
    #endif
  #else
    #error "Deze KEYPAD_TYPE wordt niet ondersteund met INPUT_TYPE_DIGITAL."
  #endif

// INPUT_TYPE_PCF8574 ---------------------------------------------------------
#elif ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_PCF8574)
#if defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_USER_DEFINED_DIRECT
  #ifndef KEYPAD_GENERIEK_AANTAL_PINNEN
    #error "KEYPAD_GENERIEK_AANTAL_PINNEN moet ingesteld worden in UserConfig.h bij KEYPAD_TYPE_USER_DEFINED_DIRECT."
  #endif
  #ifndef KEYPAD_GENERIEK_PINNEN
    #error "KEYPAD_GENERIEK_PINNEN moet ingesteld worden in UserConfig.h bij KEYPAD_TYPE_USER_DEFINED_DIRECT."
  #endif
  #ifndef KEYPAD_GENERIEK_OUTPUT_LEVEL_WHEN_KEY_PRESSED
    #error "KEYPAD_GENERIEK_OUTPUT_LEVEL_WHEN_KEY_PRESSED moet ingesteld worden in UserConfig.h bij KEYPAD_TYPE_USER_DEFINED_DIRECT."
  #endif
  #ifndef KEYPAD_GENERIEK_KEY_LAYOUT
    #error "KEYPAD_GENERIEK_KEY_LAYOUT moet ingesteld worden in UserConfig.h bij KEYPAD_TYPE_USER_DEFINED_DIRECT."
  #endif

#elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_USER_DEFINED_MATRIX
  #ifndef KEYPAD_GENERIEK_RIJ_PINNEN
    #error "KEYPAD_GENERIEK_RIJ_PINNEN moet ingesteld worden in UserConfig.h bij KEYPAD_TYPE_USER_DEFINED_MATRIX."
  #endif
  #ifndef KEYPAD_GENERIEK_AANTAL_RIJEN
    #error "KEYPAD_GENERIEK_AANTAL_RIJEN moet ingesteld worden in UserConfig.h bij KEYPAD_TYPE_USER_DEFINED_MATRIX."
  #endif
  #ifndef KEYPAD_GENERIEK_KOLOM_PINNEN
    #error "KEYPAD_GENERIEK_KOLOM_PINNEN moet ingesteld worden in UserConfig.h bij KEYPAD_TYPE_USER_DEFINED_MATRIX."
  #endif
  #ifndef KEYPAD_GENERIEK_AANTAL_KOLOMMEN
    #error "KEYPAD_GENERIEK_AANTAL_KOLOMMEN moet ingesteld worden in UserConfig.h bij KEYPAD_TYPE_USER_DEFINED_MATRIX."
  #endif
  #ifndef KEYPAD_GENERIEK_KEY_LAYOUT
    #error "KEYPAD_GENERIEK_KEY_LAYOUT moet ingesteld worden in UserConfig.h bij KEYPAD_TYPE_USER_DEFINED_MATRIX."
  #endif

#elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_DIRECT_1x4
  #ifndef INPUT_KEYPAD_PIN_K1
    #define INPUT_KEYPAD_PIN_K1 PCF8574_PIN_P0
  #endif
  #ifndef INPUT_KEYPAD_PIN_K2
    #define INPUT_KEYPAD_PIN_K2 PCF8574_PIN_P1
  #endif
  #ifndef INPUT_KEYPAD_PIN_K3
    #define INPUT_KEYPAD_PIN_K3 PCF8574_PIN_P2
  #endif
  #ifndef INPUT_KEYPAD_PIN_K4
    #define INPUT_KEYPAD_PIN_K4 PCF8574_PIN_P3
  #endif

#elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_DIRECT_2x4
  #ifndef INPUT_KEYPAD_PIN_K1
    #define INPUT_KEYPAD_PIN_K1 PCF8574_PIN_P0
  #endif
  #ifndef INPUT_KEYPAD_PIN_K2
    #define INPUT_KEYPAD_PIN_K2 PCF8574_PIN_P1
  #endif
  #ifndef INPUT_KEYPAD_PIN_K3
    #define INPUT_KEYPAD_PIN_K3 PCF8574_PIN_P2
  #endif
  #ifndef INPUT_KEYPAD_PIN_K4
    #define INPUT_KEYPAD_PIN_K4 PCF8574_PIN_P3
  #endif
  #ifndef INPUT_KEYPAD_PIN_K5
    #define INPUT_KEYPAD_PIN_K5 PCF8574_PIN_P4
  #endif
  #ifndef INPUT_KEYPAD_PIN_K6
    #define INPUT_KEYPAD_PIN_K6 PCF8574_PIN_P5
  #endif
  #ifndef INPUT_KEYPAD_PIN_K7
    #define INPUT_KEYPAD_PIN_K7 PCF8574_PIN_P6
  #endif
  #ifndef INPUT_KEYPAD_PIN_K8
    #define INPUT_KEYPAD_PIN_K8 PCF8574_PIN_P7
  #endif

#elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_MATRIX_2x2
  #ifndef INPUT_KEYPAD_PIN_L1
    #define INPUT_KEYPAD_PIN_L1 PCF8574_PIN_P0
  #endif
  #ifndef INPUT_KEYPAD_PIN_L2
    #define INPUT_KEYPAD_PIN_L2 PCF8574_PIN_P1
  #endif
  #ifndef INPUT_KEYPAD_PIN_R1
    #define INPUT_KEYPAD_PIN_R1 PCF8574_PIN_P2
  #endif
  #ifndef INPUT_KEYPAD_PIN_R2
    #define INPUT_KEYPAD_PIN_R2 PCF8574_PIN_P3
  #endif

#elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_MATRIX_4x4
  #ifndef INPUT_KEYPAD_PIN_C4
    #define INPUT_KEYPAD_PIN_C4 PCF8574_PIN_P0
  #endif
  #ifndef INPUT_KEYPAD_PIN_C3
    #define INPUT_KEYPAD_PIN_C3 PCF8574_PIN_P1
  #endif
  #ifndef INPUT_KEYPAD_PIN_C2
    #define INPUT_KEYPAD_PIN_C2 PCF8574_PIN_P2
  #endif
  #ifndef INPUT_KEYPAD_PIN_C1
    #define INPUT_KEYPAD_PIN_C1 PCF8574_PIN_P3
  #endif
  #ifndef INPUT_KEYPAD_PIN_R1
    #define INPUT_KEYPAD_PIN_R1 PCF8574_PIN_P4
  #endif
  #ifndef INPUT_KEYPAD_PIN_R2
    #define INPUT_KEYPAD_PIN_R2 PCF8574_PIN_P5
  #endif
  #ifndef INPUT_KEYPAD_PIN_R3
    #define INPUT_KEYPAD_PIN_R3 PCF8574_PIN_P6
  #endif
  #ifndef INPUT_KEYPAD_PIN_R4
    #define INPUT_KEYPAD_PIN_R4 PCF8574_PIN_P7
  #endif

#elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4
  #ifndef INPUT_KEYPAD_PIN_1
    #define INPUT_KEYPAD_PIN_1 PCF8574_PIN_P1
  #endif
  #ifndef INPUT_KEYPAD_PIN_2
    #define INPUT_KEYPAD_PIN_2 PCF8574_PIN_P0
  #endif
  #ifndef INPUT_KEYPAD_PIN_3
    #define INPUT_KEYPAD_PIN_3 PCF8574_PIN_P3
  #endif
  #ifndef INPUT_KEYPAD_PIN_4
    #define INPUT_KEYPAD_PIN_4 PCF8574_PIN_P2
  #endif

#elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_DIRECT_4x1
  #ifndef INPUT_KEYPAD_PIN_1
    #define INPUT_KEYPAD_PIN_1 PCF8574_PIN_P1
  #endif
  #ifndef INPUT_KEYPAD_PIN_2
    #define INPUT_KEYPAD_PIN_2 PCF8574_PIN_P0
  #endif
  #ifndef INPUT_KEYPAD_PIN_3
    #define INPUT_KEYPAD_PIN_3 PCF8574_PIN_P3
  #endif
  #ifndef INPUT_KEYPAD_PIN_4
    #define INPUT_KEYPAD_PIN_4 PCF8574_PIN_P2
  #endif

#elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_MATRIX_1x4
  #ifndef INPUT_KEYPAD_PIN_R1
    #define INPUT_KEYPAD_PIN_R1 PCF8574_PIN_P0
  #endif
  #ifndef INPUT_KEYPAD_PIN_C1
    #define INPUT_KEYPAD_PIN_C1 PCF8574_PIN_P1
  #endif
  #ifndef INPUT_KEYPAD_PIN_C2
    #define INPUT_KEYPAD_PIN_C2 PCF8574_PIN_P2
  #endif
  #ifndef INPUT_KEYPAD_PIN_C3
    #define INPUT_KEYPAD_PIN_C3 PCF8574_PIN_P3
  #endif
  #ifndef INPUT_KEYPAD_PIN_C4
    #define INPUT_KEYPAD_PIN_C4 PCF8574_PIN_P4
  #endif

#elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_MATRIX_2x4
  #ifndef INPUT_KEYPAD_PIN_R1
    #define INPUT_KEYPAD_PIN_R1 PCF8574_PIN_P0
  #endif
  #ifndef INPUT_KEYPAD_PIN_R2
    #define INPUT_KEYPAD_PIN_R2 PCF8574_PIN_P1
  #endif
  #ifndef INPUT_KEYPAD_PIN_C1
    #define INPUT_KEYPAD_PIN_C1 PCF8574_PIN_P2
  #endif
  #ifndef INPUT_KEYPAD_PIN_C2
    #define INPUT_KEYPAD_PIN_C2 PCF8574_PIN_P3
  #endif
  #ifndef INPUT_KEYPAD_PIN_C3
    #define INPUT_KEYPAD_PIN_C3 PCF8574_PIN_P4
  #endif
  #ifndef INPUT_KEYPAD_PIN_C4
    #define INPUT_KEYPAD_PIN_C4 PCF8574_PIN_P5
  #endif

#elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_MATRIX_4x3
  #ifndef INPUT_KEYPAD_PIN_R1
    #define INPUT_KEYPAD_PIN_R1 PCF8574_PIN_P0
  #endif
  #ifndef INPUT_KEYPAD_PIN_R2
    #define INPUT_KEYPAD_PIN_R2 PCF8574_PIN_P1
  #endif
  #ifndef INPUT_KEYPAD_PIN_R3
    #define INPUT_KEYPAD_PIN_R3 PCF8574_PIN_P2
  #endif
  #ifndef INPUT_KEYPAD_PIN_R4
    #define INPUT_KEYPAD_PIN_R4 PCF8574_PIN_P3
  #endif
  #ifndef INPUT_KEYPAD_PIN_C1
    #define INPUT_KEYPAD_PIN_C1 PCF8574_PIN_P4
  #endif
  #ifndef INPUT_KEYPAD_PIN_C2
    #define INPUT_KEYPAD_PIN_C2 PCF8574_PIN_P5
  #endif
  #ifndef INPUT_KEYPAD_PIN_C3
    #define INPUT_KEYPAD_PIN_C3 PCF8574_PIN_P6
  #endif

#elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_MATRIX_4x4
  #ifndef INPUT_KEYPAD_PIN_R1
    #define INPUT_KEYPAD_PIN_R1 PCF8574_PIN_P0
  #endif
  #ifndef INPUT_KEYPAD_PIN_R2
    #define INPUT_KEYPAD_PIN_R2 PCF8574_PIN_P1
  #endif
  #ifndef INPUT_KEYPAD_PIN_R3
    #define INPUT_KEYPAD_PIN_R3 PCF8574_PIN_P2
  #endif
  #ifndef INPUT_KEYPAD_PIN_R4
    #define INPUT_KEYPAD_PIN_R4 PCF8574_PIN_P3
  #endif
  #ifndef INPUT_KEYPAD_PIN_C1
    #define INPUT_KEYPAD_PIN_C1 PCF8574_PIN_P4
  #endif
  #ifndef INPUT_KEYPAD_PIN_C2
    #define INPUT_KEYPAD_PIN_C2 PCF8574_PIN_P5
  #endif
  #ifndef INPUT_KEYPAD_PIN_C3
    #define INPUT_KEYPAD_PIN_C3 PCF8574_PIN_P6
  #endif
  #ifndef INPUT_KEYPAD_PIN_C4
    #define INPUT_KEYPAD_PIN_C4 PCF8574_PIN_P7
  #endif

#elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_TOUCH_TTP224_DIRECT_1x4
  #ifndef INPUT_KEYPAD_PIN_OUT1
    #define INPUT_KEYPAD_PIN_OUT1 PCF8574_PIN_P0
  #endif
  #ifndef INPUT_KEYPAD_PIN_OUT2
    #define INPUT_KEYPAD_PIN_OUT2 PCF8574_PIN_P1
  #endif
  #ifndef INPUT_KEYPAD_PIN_OUT3
    #define INPUT_KEYPAD_PIN_OUT3 PCF8574_PIN_P2
  #endif
  #ifndef INPUT_KEYPAD_PIN_OUT4
    #define INPUT_KEYPAD_PIN_OUT4 PCF8574_PIN_P3
  #endif

#elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_TOUCH_TTP229_MATRIX_4x4
  #ifndef INPUT_KEYPAD_PIN_OUT1
    #define INPUT_KEYPAD_PIN_OUT1 PCF8574_PIN_P0
  #endif
  #ifndef INPUT_KEYPAD_PIN_OUT2
    #define INPUT_KEYPAD_PIN_OUT2 PCF8574_PIN_P1
  #endif
  #ifndef INPUT_KEYPAD_PIN_OUT3
    #define INPUT_KEYPAD_PIN_OUT3 PCF8574_PIN_P2
  #endif
  #ifndef INPUT_KEYPAD_PIN_OUT4
    #define INPUT_KEYPAD_PIN_OUT4 PCF8574_PIN_P3
  #endif
  #ifndef INPUT_KEYPAD_PIN_OUT5
    #define INPUT_KEYPAD_PIN_OUT5 PCF8574_PIN_P4
  #endif
  #ifndef INPUT_KEYPAD_PIN_OUT6
    #define INPUT_KEYPAD_PIN_OUT6 PCF8574_PIN_P5
  #endif
  #ifndef INPUT_KEYPAD_PIN_OUT7
    #define INPUT_KEYPAD_PIN_OUT7 PCF8574_PIN_P6
  #endif
  #ifndef INPUT_KEYPAD_PIN_OUT8
    #define INPUT_KEYPAD_PIN_OUT8 PCF8574_PIN_P7
  #endif
#endif

#endif

// INPUT_TYPE_HX1838 ----------------------------------------------------------
#if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_HX1838) && HX1838_TOETSENINDELING == HX1838_TOETSENINDELING_REMOTE_USER_DEFINED
  #if HX1838_BRON_CODES != HX1838_BRON_CODES_DEFINE
    #error HX1838_TOETSENINDELING_REMOTE_USER_DEFINED wordt alleen ondersteund met HX1838_BRON_CODES_DEFINE.
  #endif
#endif

#if HX1838_BRON_CODES == HX1838_BRON_CODES_EEPROM_WANNEER_GEEN_DEFINE
  #if defined(HX1838_CODE_1) || defined(HX1838_CODE_2) || defined(HX1838_CODE_3) || defined(HX1838_CODE_4) || defined(HX1838_CODE_5) || defined(HX1838_CODE_6) || defined(HX1838_CODE_7) || defined(HX1838_CODE_8) || defined(HX1838_CODE_9) || defined(HX1838_CODE_10) || defined(HX1838_CODE_11) || defined(HX1838_CODE_12) || defined(HX1838_CODE_13) || defined(HX1838_CODE_14) || defined(HX1838_CODE_15) || defined(HX1838_CODE_16) || defined(HX1838_CODE_17) || defined(HX1838_CODE_18) || defined(HX1838_CODE_19) || defined(HX1838_CODE_20) || defined(HX1838_CODE_21)
    #undef HX1838_BRON_CODES
    #define HX1838_BRON_CODES HX1838_BRON_CODES_DEFINE
  #else
    #undef HX1838_BRON_CODES
    #define HX1838_BRON_CODES HX1838_BRON_CODES_EEPROM_ALTIJD
  #endif
#endif

#if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_HX1838) && HX1838_TOETSENINDELING == HX1838_TOETSENINDELING_REMOTE_USER_DEFINED
  #ifndef HX1838_GENERIEK_AANTAL_TOETSEN
    #error HX1838_GENERIEK_AANTAL_TOETSEN moet ingesteld worden in UserConfig.h bij HX1838_TOETSENINDELING_REMOTE_USER_DEFINED.
  #else
    #if HX1838_GENERIEK_AANTAL_TOETSEN < 1 || HX1838_GENERIEK_AANTAL_TOETSEN > 255
      #error HX1838_GENERIEK_AANTAL_TOETSEN moet tussen 1 en 255 liggen.
    #endif
  #endif
  #ifndef HX1838_GENERIEK_CODES
    #define HX1838_GENERIEK_CODES_KALIBREREN
  #endif
  #ifndef HX1838_GENERIEK_KEY_LAYOUT
    #error HX1838_GENERIEK_KEY_LAYOUT moet ingesteld worden in UserConfig.h bij HX1838_TOETSENINDELING_REMOTE_USER_DEFINED.
  #endif
#endif

#if HX1838_TOETSENINDELING == HX1838_TOETSENINDELING_REMOTE_OK_BOVENAAN_17_TOETSEN
  #ifndef HX1838_CODE_1
    #define HX1838_CODE_1 0x46UL // = UP
  #endif
  #ifndef HX1838_CODE_2
    #define HX1838_CODE_2 0x15UL // = DOWN
  #endif
  #ifndef HX1838_CODE_3
    #define HX1838_CODE_3 0x40UL // = OK
  #endif
  #ifndef HX1838_CODE_4
    #define HX1838_CODE_4 0x44UL // = LEFT
  #endif
  #ifndef HX1838_CODE_5
    #define HX1838_CODE_5 0x43UL // = RIGHT
  #endif
  #ifndef HX1838_CODE_6
    #define HX1838_CODE_6 0x16UL // = 1
  #endif
  #ifndef HX1838_CODE_7
    #define HX1838_CODE_7 0x19UL // = 2
  #endif
  #ifndef HX1838_CODE_8
    #define HX1838_CODE_8 0x0DUL // = 3
  #endif
  #ifndef HX1838_CODE_9
    #define HX1838_CODE_9 0x0CUL // = 4
  #endif
  #ifndef HX1838_CODE_10
    #define HX1838_CODE_10 0x18UL // = 5
  #endif
  #ifndef HX1838_CODE_11
    #define HX1838_CODE_11 0x5EUL // = 6
  #endif
  #ifndef HX1838_CODE_12
    #define HX1838_CODE_12 0x08UL // = 7
  #endif
  #ifndef HX1838_CODE_13
    #define HX1838_CODE_13 0x1CUL // = 8
  #endif
  #ifndef HX1838_CODE_14
    #define HX1838_CODE_14 0x5AUL // = 9
  #endif
  #ifndef HX1838_CODE_15
    #define HX1838_CODE_15 0x42UL // = *
  #endif
  #ifndef HX1838_CODE_16
    #define HX1838_CODE_16 0x52UL // = 0
  #endif
  #ifndef HX1838_CODE_17
    #define HX1838_CODE_17 0x4AUL // = #
  #endif

#elif HX1838_TOETSENINDELING == HX1838_TOETSENINDELING_REMOTE_OK_ONDERAAN_17_TOETSEN
  #ifndef HX1838_CODE_1
    #define HX1838_CODE_1 0x45UL // = 1
  #endif
  #ifndef HX1838_CODE_2
    #define HX1838_CODE_2 0x46UL // = 2
  #endif
  #ifndef HX1838_CODE_3
    #define HX1838_CODE_3 0x47UL // = 3
  #endif
  #ifndef HX1838_CODE_4
    #define HX1838_CODE_4 0x44UL // = 4
  #endif
  #ifndef HX1838_CODE_5
    #define HX1838_CODE_5 0x40UL // = 5
  #endif
  #ifndef HX1838_CODE_6
    #define HX1838_CODE_6 0x43UL // = 6
  #endif
  #ifndef HX1838_CODE_7
    #define HX1838_CODE_7 0x07UL // = 7
  #endif
  #ifndef HX1838_CODE_8
    #define HX1838_CODE_8 0x15UL // = 8
  #endif
  #ifndef HX1838_CODE_9
    #define HX1838_CODE_9 0x09UL // = 9
  #endif
  #ifndef HX1838_CODE_10
    #define HX1838_CODE_10 0x16UL // = *
  #endif
  #ifndef HX1838_CODE_11
    #define HX1838_CODE_11 0x19UL // = 0
  #endif
  #ifndef HX1838_CODE_12
    #define HX1838_CODE_12 0x0DUL // = #
  #endif
  #ifndef HX1838_CODE_13
    #define HX1838_CODE_13 0x18UL // = UP
  #endif
  #ifndef HX1838_CODE_14
    #define HX1838_CODE_14 0x52UL // = DOWN
  #endif
  #ifndef HX1838_CODE_15
    #define HX1838_CODE_15 0x1CUL // = OK
  #endif
  #ifndef HX1838_CODE_16
    #define HX1838_CODE_16 0x08UL // = LEFT
  #endif
  #ifndef HX1838_CODE_17
    #define HX1838_CODE_17 0x5AUL // = RIGHT
  #endif  

#elif HX1838_TOETSENINDELING == HX1838_TOETSENINDELING_REMOTE_21_TOETSEN_MP3
  #ifndef HX1838_CODE_1
    #define HX1838_CODE_1 0x45UL // = CH-
  #endif
  #ifndef HX1838_CODE_2
    #define HX1838_CODE_2 0x46UL // = CH
  #endif
  #ifndef HX1838_CODE_3
    #define HX1838_CODE_3 0x47UL // = CH+
  #endif
  #ifndef HX1838_CODE_4
    #define HX1838_CODE_4 0x44UL // = PREV
  #endif
  #ifndef HX1838_CODE_5
    #define HX1838_CODE_5 0x40UL // = NEXT
  #endif
  #ifndef HX1838_CODE_6
    #define HX1838_CODE_6 0x43UL // = PLAY
  #endif
  #ifndef HX1838_CODE_7
    #define HX1838_CODE_7 0x07UL // = -
  #endif
  #ifndef HX1838_CODE_8
    #define HX1838_CODE_8 0x15UL // = +
  #endif
  #ifndef HX1838_CODE_9
    #define HX1838_CODE_9 0x09UL // = EQ
  #endif
  #ifndef HX1838_CODE_10
    #define HX1838_CODE_10 0x16UL // = 0
  #endif
  #ifndef HX1838_CODE_11
    #define HX1838_CODE_11 0x19UL // = 100+
  #endif
  #ifndef HX1838_CODE_12
    #define HX1838_CODE_12 0x0DUL // = 200+
  #endif
  #ifndef HX1838_CODE_13
    #define HX1838_CODE_13 0x0CUL // = 1
  #endif
  #ifndef HX1838_CODE_14
    #define HX1838_CODE_14 0x18UL // = 2
  #endif
  #ifndef HX1838_CODE_15
    #define HX1838_CODE_15 0x5EUL // = 3
  #endif
  #ifndef HX1838_CODE_16
    #define HX1838_CODE_16 0x08UL // = 4
  #endif
  #ifndef HX1838_CODE_17
    #define HX1838_CODE_17 0x1CUL // = 5
  #endif
  #ifndef HX1838_CODE_18
    #define HX1838_CODE_18 0x5AUL // = 6
  #endif
  #ifndef HX1838_CODE_19
    #define HX1838_CODE_19 0x42UL // = 7
  #endif
  #ifndef HX1838_CODE_20
    #define HX1838_CODE_20 0x52UL // = 8
  #endif
  #ifndef HX1838_CODE_21
    #define HX1838_CODE_21 0x4AUL // = 9
  #endif
#endif

// ============================================================================
// DEBUG INSTELLINGEN 
// ============================================================================
#ifdef TRACE 
  #ifndef DEBUG
    #define DEBUG
  #endif
#endif

#if defined(DEBUG)
  #define GA_DEBUG_PRINT(x)          GA_SERIAL.print(x)
  #define GA_DEBUG_PRINTLN(x)        GA_SERIAL.println(x)
  #define GA_DEBUG_PRINTLN2(x, f)    GA_SERIAL.println(x, f)
#else
  #define GA_DEBUG_PRINT(x)
  #define GA_DEBUG_PRINTLN(x)
  #define GA_DEBUG_PRINTLN2(x, f)
#endif

#endif
