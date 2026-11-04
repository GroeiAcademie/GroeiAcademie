#ifndef SYSTEM_CONFIG_H
#define SYSTEM_CONFIG_H

#include "../Systeem/Input/InputTypes.h"
#include "../Systeem/Screen/ScreenTypes.h"

// BOARD_VERSIONs: ondersteunde Arduino Uno R3-vormfactorborden die vóór UserConfig.h beschikbaar moeten zijn.
#define BOARD_UNO_R4_MINIMA                1
#define BOARD_UNO_R4_WIFI                  2
#define BOARD_UNO_Q                        7
#define BOARD_ESP32_D1_UNO_R32             5
#define BOARD_ESP32S3_ARDI32               3
#define BOARD_ESP32S3_DEV                  8
#define BOARD_RP2040_CYTRON_MAKER_UNO      4
#define BOARD_STM32F4_NUCLEO64_F401RE      6

// DATA: vaste keuzewaarden die vóór UserConfig.h beschikbaar moeten zijn.
// Per medium een vaste volgorde: NATIVE, I2C, SPI. Een combinatie die niet bestaat, krijgt geen naam: het nummer blijft vrij.
#define DATA_TYPE_NONE                     0x0001
#define DATA_TYPE_DIRECT                   0x0002
#define DATA_TYPE_EEPROM_NATIVE            0x0004
#define DATA_TYPE_EEPROM_I2C               0x0008
#define DATA_TYPE_EEPROM_SPI               0x0010   // nog niet ondersteund in v2.0.0
// 0x0020 vrij: FLASH via I2C bestaat niet als gangbare chip
#define DATA_TYPE_FLASH_SPI                0x0040
#define DATA_TYPE_FRAM_I2C                 0x0080
#define DATA_TYPE_FRAM_SPI                 0x0100   // nog niet ondersteund in v2.0.0
// 0x0200 vrij: microSD via I2C bestaat niet (de SD-specificatie kent geen I2C)
#define DATA_TYPE_MICROSD_SPI              0x0400
// 0x0800 vrij: NRF24L01 native bestaat niet
// 0x1000 vrij: NRF24L01 via I2C bestaat niet
#define DATA_TYPE_NRF24L01_SPI             0x2000   // radiomodule: elk record ook draadloos doorsturen

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

// I2C
#define I2C_ADDRESS_NONE                   0xFF

// SERIAL REDIRECT: vaste keuzewaarden die vóór UserConfig.h beschikbaar moeten zijn.
#define REDIRECT_SERIAL_NONE               0
#define REDIRECT_SERIAL_TO_MICROSD         1

// opladen van UserConfig.h
#ifndef GROEIACADEMIE_IGNORE_USER_CONFIG
  #if defined(__has_include)
    #if __has_include("UserConfig.h")
      #include "UserConfig.h"
      #define GROEIACADEMIE_USER_CONFIG_GELADEN
    #endif

    // Extenders: Persoonlijke configuratie (optioneel), na UserConfig.h en vóór de standaardwaarden hieronder.
    #if __has_include("Extenders/ConfigADS1115.h")
      #include "Extenders/ConfigADS1115.h"
    #endif
    #if __has_include("Extenders/ConfigADS1158.h")
      #include "Extenders/ConfigADS1158.h"
    #endif
    #if __has_include("Extenders/ConfigADS7828.h")
      #include "Extenders/ConfigADS7828.h"
    #endif
    #if __has_include("Extenders/ConfigADS7953.h")
      #include "Extenders/ConfigADS7953.h"
    #endif
    #if __has_include("Extenders/ConfigCD74HC4067.h")
      #include "Extenders/ConfigCD74HC4067.h"
    #endif
    #if __has_include("Extenders/ConfigDS2482v800.h")
      #include "Extenders/ConfigDS2482v800.h"
    #endif
    #if __has_include("Extenders/ConfigMAX14830I2C.h")
      #include "Extenders/ConfigMAX14830I2C.h"
    #endif
    #if __has_include("Extenders/ConfigMAX14830SPI.h")
      #include "Extenders/ConfigMAX14830SPI.h"
    #endif
    #if __has_include("Extenders/ConfigMCP23017.h")
      #include "Extenders/ConfigMCP23017.h"
    #endif
    #if __has_include("Extenders/ConfigMCP23S17.h")
      #include "Extenders/ConfigMCP23S17.h"
    #endif
    #if __has_include("Extenders/ConfigPCA9685.h")
      #include "Extenders/ConfigPCA9685.h"
    #endif
    #if __has_include("Extenders/ConfigPCF8574.h")
      #include "Extenders/ConfigPCF8574.h"
    #endif
    #if __has_include("Extenders/ConfigPCF8575.h")
      #include "Extenders/ConfigPCF8575.h"
    #endif
    #if __has_include("Extenders/ConfigSC16IS752I2C.h")
      #include "Extenders/ConfigSC16IS752I2C.h"
    #endif
    #if __has_include("Extenders/ConfigSC16IS752SPI.h")
      #include "Extenders/ConfigSC16IS752SPI.h"
    #endif
    #if __has_include("Extenders/ConfigTCA9548A.h")
      #include "Extenders/ConfigTCA9548A.h"
    #endif

    // Sensoren: Persoonlijke configuratie (optioneel)  
    #if __has_include("Sensoren/ConfigAD8232.h")
      #include "Sensoren/ConfigAD8232.h"
    #endif
    #if __has_include("Sensoren/ConfigADXL345.h")
      #include "Sensoren/ConfigADXL345.h"
    #endif
    #if __has_include("Sensoren/ConfigAHT10.h")
      #include "Sensoren/ConfigAHT10.h"
    #endif
    #if __has_include("Sensoren/ConfigAS608.h")
      #include "Sensoren/ConfigAS608.h"
    #endif
    #if __has_include("Sensoren/ConfigBioAmpEXGPill.h")
      #include "Sensoren/ConfigBioAmpEXGPill.h"
    #endif
    #if __has_include("Sensoren/ConfigBME680.h")
      #include "Sensoren/ConfigBME680.h"
    #endif
    #if __has_include("Sensoren/ConfigBME688.h")
      #include "Sensoren/ConfigBME688.h"
    #endif
    #if __has_include("Sensoren/ConfigBMP280.h")
      #include "Sensoren/ConfigBMP280.h"
    #endif
    #if __has_include("Sensoren/ConfigDHTxx.h")
      #include "Sensoren/ConfigDHTxx.h"
    #endif
    #if __has_include("Sensoren/ConfigDS18B20.h")
      #include "Sensoren/ConfigDS18B20.h"
    #endif
    #if __has_include("Sensoren/ConfigDS3231.h")
      #include "Sensoren/ConfigDS3231.h"
    #endif
    #if __has_include("Sensoren/ConfigEGBOPulseSensor.h")
      #include "Sensoren/ConfigEGBOPulseSensor.h"
    #endif
    #if __has_include("Sensoren/ConfigFsrRfp.h")
      #include "Sensoren/ConfigFsrRfp.h"
    #endif
    #if __has_include("Sensoren/ConfigGSRGrove.h")
      #include "Sensoren/ConfigGSRGrove.h"
    #endif
    #if __has_include("Sensoren/ConfigHCSR04.h")
      #include "Sensoren/ConfigHCSR04.h"
    #endif
    #if __has_include("Sensoren/ConfigHCSR04P.h")
      #include "Sensoren/ConfigHCSR04P.h"
    #endif
    #if __has_include("Sensoren/ConfigHCSR501.h")
      #include "Sensoren/ConfigHCSR501.h"
    #endif
    #if __has_include("Sensoren/ConfigHW201.h")
      #include "Sensoren/ConfigHW201.h"
    #endif
    #if __has_include("Sensoren/ConfigHX711.h")
      #include "Sensoren/ConfigHX711.h"
    #endif
    #if __has_include("Sensoren/ConfigHYSRF05.h")
      #include "Sensoren/ConfigHYSRF05.h"
    #endif
    #if __has_include("Sensoren/ConfigINMP441.h")
      #include "Sensoren/ConfigINMP441.h"
    #endif
    #if __has_include("Sensoren/ConfigLD2410D.h")
      #include "Sensoren/ConfigLD2410D.h"
    #endif
    #if __has_include("Sensoren/ConfigLD2450.h")
      #include "Sensoren/ConfigLD2450.h"
    #endif
    #if __has_include("Sensoren/ConfigLM35DZ.h")
      #include "Sensoren/ConfigLM35DZ.h"
    #endif
    #if __has_include("Sensoren/ConfigLM75A.h")
      #include "Sensoren/ConfigLM75A.h"
    #endif
    #if __has_include("Sensoren/ConfigMAX30102.h")
      #include "Sensoren/ConfigMAX30102.h"
    #endif
    #if __has_include("Sensoren/ConfigMAX4466.h")
      #include "Sensoren/ConfigMAX4466.h"
    #endif
    #if __has_include("Sensoren/ConfigMAX9814.h")
      #include "Sensoren/ConfigMAX9814.h"
    #endif
    #if __has_include("Sensoren/ConfigMCU6701.h")
      #include "Sensoren/ConfigMCU6701.h"
    #endif
    #if __has_include("Sensoren/ConfigMF52A.h")
      #include "Sensoren/ConfigMF52A.h"
    #endif
    #if __has_include("Sensoren/ConfigMF52D.h")
      #include "Sensoren/ConfigMF52D.h"
    #endif
    #if __has_include("Sensoren/ConfigMPU6050.h")
      #include "Sensoren/ConfigMPU6050.h"
    #endif
    #if __has_include("Sensoren/ConfigMPU6500.h")
      #include "Sensoren/ConfigMPU6500.h"
    #endif
    #if __has_include("Sensoren/ConfigMR60BHA2.h")
      #include "Sensoren/ConfigMR60BHA2.h"
    #endif
    #if __has_include("Sensoren/ConfigNeuroSkyTGAM.h")
      #include "Sensoren/ConfigNeuroSkyTGAM.h"
    #endif
    #if __has_include("Sensoren/ConfigOT3686.h")
      #include "Sensoren/ConfigOT3686.h"
    #endif
    #if __has_include("Sensoren/ConfigOV7670.h")
      #include "Sensoren/ConfigOV7670.h"
    #endif
    #if __has_include("Sensoren/ConfigPIEZO.h")
      #include "Sensoren/ConfigPIEZO.h"
    #endif
    #if __has_include("Sensoren/ConfigSCD41.h")
      #include "Sensoren/ConfigSCD41.h"
    #endif
    #if __has_include("Sensoren/ConfigSDP810125PA.h")
      #include "Sensoren/ConfigSDP810125PA.h"
    #endif
    #if __has_include("Sensoren/ConfigTTP223.h")
      #include "Sensoren/ConfigTTP223.h"
    #endif
    #if __has_include("Sensoren/ConfigVL53L0X.h")
      #include "Sensoren/ConfigVL53L0X.h"
    #endif
    #if __has_include("Sensoren/ConfigVL6180X.h")
      #include "Sensoren/ConfigVL6180X.h"
    #endif
    #if __has_include("Sensoren/ConfigWATER.h")
      #include "Sensoren/ConfigWATER.h"
    #endif
   
    // Sturingen: Persoonlijke configuratie (optioneel)  
    #if __has_include("Sturingen/ConfigSG90.h")
      #include "Sturingen/ConfigSG90.h"
    #endif
    #if __has_include("Sturingen/ConfigMG996R.h")
      #include "Sturingen/ConfigMG996R.h"
    #endif
    #if __has_include("Sturingen/ConfigL293D.h")
      #include "Sturingen/ConfigL293D.h"
    #endif
    #if __has_include("Sturingen/ConfigL298N.h")
      #include "Sturingen/ConfigL298N.h"
    #endif
    #if __has_include("Sturingen/ConfigMAX7219.h")
      #include "Sturingen/ConfigMAX7219.h"
    #endif
    #if __has_include("Sturingen/ConfigRelais.h")
      #include "Sturingen/ConfigRelais.h"
    #endif
    #if __has_include("Sturingen/ConfigRGBLED.h")
      #include "Sturingen/ConfigRGBLED.h"
    #endif
    #if __has_include("Sturingen/ConfigWS2812.h")
      #include "Sturingen/ConfigWS2812.h"
    #endif
    #if __has_include("Sturingen/ConfigULN2003.h")
      #include "Sturingen/ConfigULN2003.h"
    #endif
    #if __has_include("Sturingen/ConfigA4988.h")
      #include "Sturingen/ConfigA4988.h"
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
// TAAL: standaard keuze ======================================================
// ============================================================================
#if !defined(LANGUAGE_NL) && !defined(LANGUAGE_DE) && !defined(LANGUAGE_EN) && !defined(LANGUAGE_FR)
  #define LANGUAGE_NL // #define LANGUAGE_DE | #define LANGUAGE_EN | #define LANGUAGE_FR
#endif


// ============================================================================
// BOARD_VERSION ==============================================================
// ============================================================================
#ifndef BOARD_VERSION
  // Automatische detectie gebruikt de compiler-/boardmacro van de geselecteerde Arduino-boardcore.
  // Voor SB Components Ardi32 blijft expliciete BOARD_VERSION-keuze nodig wanneer de generieke ESP32S3 Dev Module wordt gebruikt; zie docs/HARDWARE_SUPPORT.md.
  #if defined(ARDUINO_UNOR4_MINIMA)
    #define BOARD_VERSION BOARD_UNO_R4_MINIMA
  #elif defined(ARDUINO_UNOR4_WIFI)
    #define BOARD_VERSION BOARD_UNO_R4_WIFI
  #elif defined(ARDUINO_UNO_Q)              // MOGELIJKE ALTERNATIEVE NAMEN: ARDUINO_ZEPHIR | ARDUINO_ESP32S3_WROOM_1
    #define BOARD_VERSION BOARD_UNO_Q
  #elif defined(ARDUINO_D1_UNO32)
    #define BOARD_VERSION BOARD_ESP32_D1_UNO_R32
  #elif defined(ARDUINO_ARDI32)
    #define BOARD_VERSION BOARD_ESP32S3_ARDI32
  #elif defined(ARDUINO_ESP32S3_DEV)
    #define BOARD_VERSION BOARD_ESP32S3_DEV // MOGELIJKE ALTERNATIEVE NAMEN: ARDUINO_ESP32S3_UNO | ARDUINO_ESP32S3_WROOM_1
  #elif defined(ARDUINO_CYTRON_MAKER_UNO_RP2040)
    #define BOARD_VERSION BOARD_RP2040_CYTRON_MAKER_UNO
  #elif defined(ARDUINO_NUCLEO_F401RE)
    #define BOARD_VERSION BOARD_STM32F4_NUCLEO64_F401RE
  #else
    #error Dit board wordt momenteel (nog) niet ondersteund.
  #endif
#endif

// Native analogRead()-resolutie per bestaand board (standaardwaarde van de boardcore).
// Als de sketch analogReadResolution() zelf wijzigt, moet deze instelling overeenkomen.
#ifndef NATIVE_ADC_RESOLUTIE_BITS
  #if BOARD_VERSION == BOARD_ESP32_D1_UNO_R32 || BOARD_VERSION == BOARD_ESP32S3_ARDI32 || BOARD_VERSION == BOARD_ESP32S3_DEV
    #define NATIVE_ADC_RESOLUTIE_BITS 12
  #else
    #define NATIVE_ADC_RESOLUTIE_BITS 10
  #endif
#endif
#if NATIVE_ADC_RESOLUTIE_BITS < 1 || NATIVE_ADC_RESOLUTIE_BITS > 16
  #error NATIVE_ADC_RESOLUTIE_BITS moet tussen 1 en 16 liggen.
#endif

// UserConfig.h heeft voorrang op automatische boarddetectie.
// Wanneer de gebruiker in UserConfig.h zelf BOARD_VERSION definieert (bijvoorbeeld #define BOARD_VERSION BOARD_ESP32S3_ARDI32),
// gebruikt de library die keuze en wordt bovenstaande automatische detectie niet uitgevoerd (de #ifndef hierboven slaat dan over).
// Enkel wanneer BOARD_VERSION nergens vooraf gedefinieerd is, bepaalt dit bestand het board automatisch op basis van de compiler-/boardmacro's van de Arduino IDE.

#ifndef REDIRECT_SERIAL
  #define REDIRECT_SERIAL REDIRECT_SERIAL_NONE
#endif

#ifndef REDIRECT_SERIAL_FILE
  #define REDIRECT_SERIAL_FILE "serial.log"
#endif

#if REDIRECT_SERIAL != REDIRECT_SERIAL_NONE && REDIRECT_SERIAL != REDIRECT_SERIAL_TO_MICROSD
  #error REDIRECT_SERIAL bevat een onbekende redirect.
#endif

#if REDIRECT_SERIAL == REDIRECT_SERIAL_TO_MICROSD && !defined(REDIRECT_SERIAL_CONNECT_TIMEOUT_MS)
  #error REDIRECT_SERIAL_TO_MICROSD vereist REDIRECT_SERIAL_CONNECT_TIMEOUT_MS.
#endif

#if REDIRECT_SERIAL == REDIRECT_SERIAL_TO_MICROSD
  #include "../Systeem/Data/RedirectSerial.h"
  #define GA_SERIAL RedirectSerial()
#elif BOARD_VERSION == BOARD_ESP32S3_ARDI32 || BOARD_VERSION == BOARD_ESP32S3_DEV
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
// DATA =======================================================================
// ============================================================================
#ifndef DATA_LAYER_CONFIG
  #define DATA_LAYER_CONFIG DATA_TYPE_NONE
#endif


// ============================================================================
// INPUT ======================================================================
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
// Standaard UIT om geheugen te sparen.
// Enkel van toepassing bij OpvragenHuidigeToetsAanslag(false), niet bij wachten=true.
// #define INPUT_KANAAL_OPVRAGEN_HUIDIGE_TOETSAANSLAG_UITGEBREID

// INPUT_KANAAL_CONFIG standaard INPUT_TYPE_PCF8574
#ifndef INPUT_KANAAL_CONFIG
  #define INPUT_KANAAL_CONFIG INPUT_TYPE_PCF8574
#endif

#if ((INPUT_KANAAL_CONFIG) & (INPUT_TYPE_PCF8574))
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
// OUTPUT =====================================================================
// ============================================================================

// MELDINGEN: LEESTIJD
// GENERAL: meldingen bij de opstart (aanmelden, controleren, inpluggen, activeren).
// Runtime-meldingen (GB400 tot GB410) gaan alleen naar Serial en hebben geen leestijd.
#ifndef GENERAL_MELDING_CRITICAL_LEESTIJD_MS
  #define GENERAL_MELDING_CRITICAL_LEESTIJD_MS   30000UL
#endif
#ifndef GENERAL_MELDING_FATAL_ABORT_PANIC_LEESTIJD_MS
  #define GENERAL_MELDING_FATAL_ABORT_PANIC_LEESTIJD_MS   30000UL
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
  #define PIXEL_SCREEN_CS              HardwareResourcePin::NP_SS
#endif
#ifndef PIXEL_SCREEN_DC
  #define PIXEL_SCREEN_DC              HardwareResourcePin::NP_D9
#endif
#ifndef PIXEL_SCREEN_RST
  #define PIXEL_SCREEN_RST             HardwareResourcePin::NP_D7
#endif

#ifndef ACTIEF_PIXEL_SCREEN
  #define ACTIEF_PIXEL_SCREEN            SCREEN_240X320
#endif

#ifndef ACTIEF_PIXEL_SCREEN_CONTROLLER
  #define ACTIEF_PIXEL_SCREEN_CONTROLLER SCREEN_ST7789
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


// ============================================================================
// SENSOREN ===================================================================
// ============================================================================

#ifndef SENSOR_HERAANMELDEN_NA_PAUZE_MS
  #define SENSOR_HERAANMELDEN_NA_PAUZE_MS 100UL // minimale wachttijd tussen twee pogingen om een mislukte sensor opnieuw aan te melden
#endif

// ============================================================================
// Meetbereik afhankelijk van sensorvoeding en aangesloten ADC; geen bewezen modulegrens.
#ifndef SENSOR_AD8232_ADC_BEREIK_MILLIVOLT
#define SENSOR_AD8232_ADC_BEREIK_MILLIVOLT 3300
#endif

#ifndef SENSOR_AD8232_AANTAL
#define SENSOR_AD8232_AANTAL 0
#endif
#if SENSOR_AD8232_AANTAL != 0 && SENSOR_AD8232_AANTAL != 1
#error SENSOR_AD8232_AANTAL moet 0 of 1 zijn.
#endif

// I2C-sensor-identiteitstest. Standaard uitgeschakeld.
#ifndef I2C_ADDRESS_SENSOR_ADXL345_1
#define I2C_ADDRESS_SENSOR_ADXL345_1 I2C_ADDRESS_NONE
#endif
#ifndef SENSOR_ADXL345_AANTAL
#define SENSOR_ADXL345_AANTAL 0
#endif
#if SENSOR_ADXL345_AANTAL != 0 && SENSOR_ADXL345_AANTAL != 1
#error SENSOR_ADXL345_AANTAL moet 0 of 1 zijn.
#endif

#ifndef I2C_ADDRESS_SENSOR_AHT10_1
#define I2C_ADDRESS_SENSOR_AHT10_1 I2C_ADDRESS_NONE
#endif
#ifndef SENSOR_AHT10_AANTAL
#define SENSOR_AHT10_AANTAL 0
#endif
#if SENSOR_AHT10_AANTAL != 0 && SENSOR_AHT10_AANTAL != 1
#error SENSOR_AHT10_AANTAL moet 0 of 1 zijn.
#endif

#ifndef SENSOR_AS608_AANTAL
#define SENSOR_AS608_AANTAL 0
#endif
#if SENSOR_AS608_AANTAL != 0 && SENSOR_AS608_AANTAL != 1
#error SENSOR_AS608_AANTAL moet 0 of 1 zijn.
#endif

// Meetbereik afhankelijk van sensorvoeding en aangesloten ADC; geen bewezen modulegrens.
// BioAmp EXG Pill, officiële Upside Down Labs voorbeelden.
// De vijfde keuze RAW is FixedSampling, geen vijfde biopotentiaalsoort.
#define BIOAMPEXGPILL_MEETMETHODE_RAW 0
#define BIOAMPEXGPILL_MEETMETHODE_ECG 1
#define BIOAMPEXGPILL_MEETMETHODE_EMG 2
#define BIOAMPEXGPILL_MEETMETHODE_EOG 3
#define BIOAMPEXGPILL_MEETMETHODE_EEG 4
#ifndef SENSOR_BIOAMPEXGPILL_MEETMETHODE
#define SENSOR_BIOAMPEXGPILL_MEETMETHODE BIOAMPEXGPILL_MEETMETHODE_RAW
#endif
#ifndef SENSOR_BIOAMPEXGPILL_ENVELOP_VENSTER
#define SENSOR_BIOAMPEXGPILL_ENVELOP_VENSTER 128   // aantal samples voor de EMG-envelop (alleen bij EMG)
#endif
#if SENSOR_BIOAMPEXGPILL_ENVELOP_VENSTER < 1 || SENSOR_BIOAMPEXGPILL_ENVELOP_VENSTER > 1024
#error SENSOR_BIOAMPEXGPILL_ENVELOP_VENSTER moet tussen 1 en 1024 liggen.
#endif
#ifndef SENSOR_BIOAMPEXGPILL_ADC_BEREIK_MILLIVOLT
#define SENSOR_BIOAMPEXGPILL_ADC_BEREIK_MILLIVOLT 5000
#endif

#ifndef SENSOR_BIOAMPEXGPILL_AANTAL
#define SENSOR_BIOAMPEXGPILL_AANTAL 0
#endif
#if SENSOR_BIOAMPEXGPILL_AANTAL != 0 && SENSOR_BIOAMPEXGPILL_AANTAL != 1
#error SENSOR_BIOAMPEXGPILL_AANTAL moet 0 of 1 zijn.
#endif

#ifndef I2C_ADDRESS_SENSOR_BME680_1
#define I2C_ADDRESS_SENSOR_BME680_1 I2C_ADDRESS_NONE
#endif
#ifndef SENSOR_BME680_AANTAL
#define SENSOR_BME680_AANTAL 0
#endif
#if SENSOR_BME680_AANTAL != 0 && SENSOR_BME680_AANTAL != 1
#error SENSOR_BME680_AANTAL moet 0 of 1 zijn.
#endif

#ifndef I2C_ADDRESS_SENSOR_BME688_1
#define I2C_ADDRESS_SENSOR_BME688_1 I2C_ADDRESS_NONE
#endif
#ifndef SENSOR_BME688_AANTAL
#define SENSOR_BME688_AANTAL 0
#endif
#if SENSOR_BME688_AANTAL != 0 && SENSOR_BME688_AANTAL != 1
#error SENSOR_BME688_AANTAL moet 0 of 1 zijn.
#endif

#ifndef I2C_ADDRESS_SENSOR_BMP280_1
#define I2C_ADDRESS_SENSOR_BMP280_1 I2C_ADDRESS_NONE
#endif
#ifndef SENSOR_BMP280_AANTAL
#define SENSOR_BMP280_AANTAL 0
#endif
#if SENSOR_BMP280_AANTAL != 0 && SENSOR_BMP280_AANTAL != 1
#error SENSOR_BMP280_AANTAL moet 0 of 1 zijn.
#endif

#ifndef SENSOR_DS18B20_AANTAL
#define SENSOR_DS18B20_AANTAL 0
#endif
// Meerdere DS18B20-objecten per DS2482 channel zijn toegestaan met expliciete ROM.
#if SENSOR_DS18B20_AANTAL < 0
#error SENSOR_DS18B20_AANTAL mag niet negatief zijn.
#endif

#ifndef I2C_ADDRESS_SENSOR_DS3231_1
#define I2C_ADDRESS_SENSOR_DS3231_1 0x68
#endif
#ifndef SENSOR_DS3231_AANTAL
#define SENSOR_DS3231_AANTAL 0
#endif
#if SENSOR_DS3231_AANTAL != 0 && SENSOR_DS3231_AANTAL != 1
#error SENSOR_DS3231_AANTAL moet 0 of 1 zijn.
#endif
#if SENSOR_DS3231_AANTAL > 0 && I2C_ADDRESS_SENSOR_DS3231_1 != 0x68
#error I2C_ADDRESS_SENSOR_DS3231_1 moet 0x68 zijn: het adres van de DS3231 ligt vast in de chip.
#endif

// Meetbereik afhankelijk van sensorvoeding en aangesloten ADC; geen bewezen modulegrens.
#ifndef SENSOR_EGBOPULSESENSOR_ADC_BEREIK_MILLIVOLT
#define SENSOR_EGBOPULSESENSOR_ADC_BEREIK_MILLIVOLT 5000
#endif

#ifndef SENSOR_EGBOPULSESENSOR_AANTAL
#define SENSOR_EGBOPULSESENSOR_AANTAL 0
#endif
#if SENSOR_EGBOPULSESENSOR_AANTAL != 0 && SENSOR_EGBOPULSESENSOR_AANTAL != 1
#error SENSOR_EGBOPULSESENSOR_AANTAL moet 0 of 1 zijn.
#endif

#ifndef FSR_RFP_ADC_BEREIK_MILLIVOLT
#define FSR_RFP_ADC_BEREIK_MILLIVOLT 5000
#endif
#if FSR_RFP_ADC_BEREIK_MILLIVOLT < 1
#error FSR_RFP_ADC_BEREIK_MILLIVOLT moet groter dan 0 zijn (de maximale spanning van de sensoruitgang in mV).
#endif

// FsrRfp =====================================================================
// ----------------------------------------------------------------------------
// LET OP: FsrRfp meet via een ADC-Extender (nu: ADS1115). Daarvoor moet de ADS1X15-library geïnstalleerd zijn via de Arduino Library Manager.
// ============================================================================

#ifndef FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_1
  #define FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_1  2
#endif

#if FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_1 != 2 && FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_1 != 4
  #error FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_1 moet 2 of 4 zijn.
#endif

// FsrRfp: het type probe per set. RFP602, FSR400 en FSR402 hebben dezelfde aansluiting (een weerstand in een spanningsdeler)
// en dezelfde meetcode; de instelling bewaart welk type aangesloten is.
#define FSR_RFP_PROBE_RFP602 1
#define FSR_RFP_PROBE_FSR400 2
#define FSR_RFP_PROBE_FSR402 3
#ifndef FSR_RFP_PROBE_SET_1
  #define FSR_RFP_PROBE_SET_1 FSR_RFP_PROBE_RFP602
#endif
#if FSR_RFP_PROBE_SET_1 != FSR_RFP_PROBE_RFP602 && FSR_RFP_PROBE_SET_1 != FSR_RFP_PROBE_FSR400 && FSR_RFP_PROBE_SET_1 != FSR_RFP_PROBE_FSR402
  #error FSR_RFP_PROBE_SET_1 moet FSR_RFP_PROBE_RFP602, FSR_RFP_PROBE_FSR400 of FSR_RFP_PROBE_FSR402 zijn.
#endif
#ifndef FSR_RFP_PROBE_SET_2
  #define FSR_RFP_PROBE_SET_2 FSR_RFP_PROBE_RFP602
#endif
#if FSR_RFP_PROBE_SET_2 != FSR_RFP_PROBE_RFP602 && FSR_RFP_PROBE_SET_2 != FSR_RFP_PROBE_FSR400 && FSR_RFP_PROBE_SET_2 != FSR_RFP_PROBE_FSR402
  #error FSR_RFP_PROBE_SET_2 moet FSR_RFP_PROBE_RFP602, FSR_RFP_PROBE_FSR400 of FSR_RFP_PROBE_FSR402 zijn.
#endif

// Set 2 (1b): bestaat alleen als FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_2 gedefinieerd is. Geen standaardwaarde: zonder set 2 geen extra geheugen of code.
#if defined(FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_2)
  #if FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_2 != 2 && FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_2 != 4
    #error FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_2 moet 2 of 4 zijn.
  #endif
#endif

// Instelbare vertraging in de busy-wait loop van WachtTotAlleSensorsLosgelatenVoorTest().
// Voorkomt dat de I2C-bus via de ADC-Extender zonder onderbreking bevraagd wordt.
#ifndef WACHT_LOSLATEN_DELAY_MS
  #define WACHT_LOSLATEN_DELAY_MS 5UL
#endif

// ============================================================================
// FsrRfp DATA
// ============================================================================
#ifndef FSR_RFP_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1
  #define FSR_RFP_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 0
#endif

#ifndef FSR_RFP_CHANNEL_1_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1
  #define FSR_RFP_CHANNEL_1_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 FSR_RFP_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1
#endif
#ifndef FSR_RFP_CHANNEL_2_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1
  #define FSR_RFP_CHANNEL_2_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 FSR_RFP_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1
#endif
#ifndef FSR_RFP_CHANNEL_3_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1
  #define FSR_RFP_CHANNEL_3_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 FSR_RFP_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1
#endif
#ifndef FSR_RFP_CHANNEL_4_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1
  #define FSR_RFP_CHANNEL_4_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1 FSR_RFP_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1
#endif

// Set 2 (1b): alleen als set 2 bestaat.
#if defined(FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_2)
#ifndef FSR_RFP_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2
  #define FSR_RFP_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2 0
#endif
#ifndef FSR_RFP_CHANNEL_1_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2
  #define FSR_RFP_CHANNEL_1_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2 FSR_RFP_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2
#endif
#ifndef FSR_RFP_CHANNEL_2_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2
  #define FSR_RFP_CHANNEL_2_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2 FSR_RFP_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2
#endif
#ifndef FSR_RFP_CHANNEL_3_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2
  #define FSR_RFP_CHANNEL_3_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2 FSR_RFP_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2
#endif
#ifndef FSR_RFP_CHANNEL_4_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2
  #define FSR_RFP_CHANNEL_4_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2 FSR_RFP_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_2
#endif
#endif

#ifndef SENSOR_GSRGROVE_AANTAL
  #define SENSOR_GSRGROVE_AANTAL 0
#endif
#if SENSOR_GSRGROVE_AANTAL != 0 && SENSOR_GSRGROVE_AANTAL != 1
  #error SENSOR_GSRGROVE_AANTAL moet 0 of 1 zijn.
#endif
#ifndef SENSOR_GSRGROVE_ADC_BEREIK_MILLIVOLT
#define SENSOR_GSRGROVE_ADC_BEREIK_MILLIVOLT 5000
#endif
#if SENSOR_GSRGROVE_ADC_BEREIK_MILLIVOLT < 1
#error SENSOR_GSRGROVE_ADC_BEREIK_MILLIVOLT moet groter dan 0 zijn (de maximale spanning van de sensoruitgang in mV).
#endif

#ifndef SENSOR_HCSR04_AANTAL
#define SENSOR_HCSR04_AANTAL 0
#endif
#if SENSOR_HCSR04_AANTAL != 0 && SENSOR_HCSR04_AANTAL != 1
#error SENSOR_HCSR04_AANTAL moet 0 of 1 zijn.
#endif

#ifndef SENSOR_HCSR04P_AANTAL
#define SENSOR_HCSR04P_AANTAL 0
#endif
#if SENSOR_HCSR04P_AANTAL != 0 && SENSOR_HCSR04P_AANTAL != 1
#error SENSOR_HCSR04P_AANTAL moet 0 of 1 zijn.
#endif

#ifndef SENSOR_HCSR501_AANTAL
#define SENSOR_HCSR501_AANTAL 0
#endif
#if SENSOR_HCSR501_AANTAL != 0 && SENSOR_HCSR501_AANTAL != 1
#error SENSOR_HCSR501_AANTAL moet 0 of 1 zijn.
#endif

#ifndef SENSOR_HW201_AANTAL
#define SENSOR_HW201_AANTAL 0
#endif
#if SENSOR_HW201_AANTAL != 0 && SENSOR_HW201_AANTAL != 1
#error SENSOR_HW201_AANTAL moet 0 of 1 zijn.
#endif

#ifndef SENSOR_HX711_AANTAL
#define SENSOR_HX711_AANTAL 0
#endif
#if SENSOR_HX711_AANTAL != 0 && SENSOR_HX711_AANTAL != 1
#error SENSOR_HX711_AANTAL moet 0 of 1 zijn.
#endif

#ifndef SENSOR_HYSRF05_AANTAL
#define SENSOR_HYSRF05_AANTAL 0
#endif
#if SENSOR_HYSRF05_AANTAL != 0 && SENSOR_HYSRF05_AANTAL != 1
#error SENSOR_HYSRF05_AANTAL moet 0 of 1 zijn.
#endif

// ESP32-S3 native interfaces: standaard uit, geen boardpinnen aangenomen.
#ifndef SENSOR_INMP441_AANTAL
#define SENSOR_INMP441_AANTAL 0
#endif
#if SENSOR_INMP441_AANTAL != 0 && SENSOR_INMP441_AANTAL != 1
#error SENSOR_INMP441_AANTAL moet 0 of 1 zijn.
#endif
#ifndef SENSOR_INMP441_SAMPLEFREQUENTIE_HZ
#define SENSOR_INMP441_SAMPLEFREQUENTIE_HZ 16000
#endif
#if SENSOR_INMP441_SAMPLEFREQUENTIE_HZ < 8000 || SENSOR_INMP441_SAMPLEFREQUENTIE_HZ > 48000
#error SENSOR_INMP441_SAMPLEFREQUENTIE_HZ moet tussen 8000 en 48000 Hz liggen.
#endif

#ifndef SENSOR_DHT11_AANTAL
#define SENSOR_DHT11_AANTAL 0
#endif
#if SENSOR_DHT11_AANTAL != 0 && SENSOR_DHT11_AANTAL != 1
#error SENSOR_DHT11_AANTAL moet 0 of 1 zijn.
#endif

#ifndef SENSOR_DHT22_AANTAL
#define SENSOR_DHT22_AANTAL 0
#endif
#if SENSOR_DHT22_AANTAL != 0 && SENSOR_DHT22_AANTAL != 1
#error SENSOR_DHT22_AANTAL moet 0 of 1 zijn.
#endif

#ifndef SENSOR_LD2410D_AANTAL
#define SENSOR_LD2410D_AANTAL 0
#endif
#if SENSOR_LD2410D_AANTAL != 0 && SENSOR_LD2410D_AANTAL != 1
#error SENSOR_LD2410D_AANTAL moet 0 of 1 zijn.
#endif

#ifndef SENSOR_LD2450_AANTAL
#define SENSOR_LD2450_AANTAL 0
#endif
#if SENSOR_LD2450_AANTAL != 0 && SENSOR_LD2450_AANTAL != 1
#error SENSOR_LD2450_AANTAL moet 0 of 1 zijn.
#endif

#ifndef SENSOR_LM35DZ_ADC_BEREIK_MILLIVOLT
#define SENSOR_LM35DZ_ADC_BEREIK_MILLIVOLT 1500
#endif
#if SENSOR_LM35DZ_ADC_BEREIK_MILLIVOLT < 1
#error SENSOR_LM35DZ_ADC_BEREIK_MILLIVOLT moet groter dan 0 zijn (de maximale spanning van de sensoruitgang in mV).
#endif
#ifndef SENSOR_LM35DZ_AANTAL
  #define SENSOR_LM35DZ_AANTAL 0
#endif
#if SENSOR_LM35DZ_AANTAL != 0 && SENSOR_LM35DZ_AANTAL != 1
  #error SENSOR_LM35DZ_AANTAL moet 0 of 1 zijn.
#endif

#ifndef I2C_ADDRESS_SENSOR_LM75A_1
#define I2C_ADDRESS_SENSOR_LM75A_1 I2C_ADDRESS_NONE
#endif
#ifndef SENSOR_LM75A_AANTAL
#define SENSOR_LM75A_AANTAL 0
#endif
#if SENSOR_LM75A_AANTAL != 0 && SENSOR_LM75A_AANTAL != 1
#error SENSOR_LM75A_AANTAL moet 0 of 1 zijn.
#endif

#ifndef I2C_ADDRESS_SENSOR_MAX30102_1
#define I2C_ADDRESS_SENSOR_MAX30102_1 I2C_ADDRESS_NONE
#endif
#ifndef SENSOR_MAX30102_AANTAL
#define SENSOR_MAX30102_AANTAL 0
#endif
#if SENSOR_MAX30102_AANTAL != 0 && SENSOR_MAX30102_AANTAL != 1
#error SENSOR_MAX30102_AANTAL moet 0 of 1 zijn.
#endif

// Meetbereik afhankelijk van sensorvoeding en aangesloten ADC; geen bewezen modulegrens.
#ifndef SENSOR_MAX4466_ADC_BEREIK_MILLIVOLT
#define SENSOR_MAX4466_ADC_BEREIK_MILLIVOLT 5000
#endif

#ifndef SENSOR_MAX4466_AANTAL
#define SENSOR_MAX4466_AANTAL 0
#endif
#if SENSOR_MAX4466_AANTAL != 0 && SENSOR_MAX4466_AANTAL != 1
#error SENSOR_MAX4466_AANTAL moet 0 of 1 zijn.
#endif

// Meetbereik afhankelijk van sensorvoeding en aangesloten ADC; geen bewezen modulegrens.
#ifndef SENSOR_MAX9814_ADC_BEREIK_MILLIVOLT
#define SENSOR_MAX9814_ADC_BEREIK_MILLIVOLT 3300
#endif

#ifndef SENSOR_MAX9814_AANTAL
#define SENSOR_MAX9814_AANTAL 0
#endif
#if SENSOR_MAX9814_AANTAL != 0 && SENSOR_MAX9814_AANTAL != 1
#error SENSOR_MAX9814_AANTAL moet 0 of 1 zijn.
#endif

#ifndef SENSOR_MCU6701_AANTAL
#define SENSOR_MCU6701_AANTAL 0
#endif
#if SENSOR_MCU6701_AANTAL != 0 && SENSOR_MCU6701_AANTAL != 1
#error SENSOR_MCU6701_AANTAL moet 0 of 1 zijn.
#endif
#ifndef SENSOR_MCU6701_ADC_BEREIK_MILLIVOLT
#define SENSOR_MCU6701_ADC_BEREIK_MILLIVOLT 5000
#endif

#ifndef SENSOR_MF52A_AANTAL
  #define SENSOR_MF52A_AANTAL 0
#endif
#if SENSOR_MF52A_AANTAL != 0 && SENSOR_MF52A_AANTAL != 1
  #error SENSOR_MF52A_AANTAL moet 0 of 1 zijn.
#endif

#ifndef SENSOR_MF52A_ADC_BEREIK_MILLIVOLT
#define SENSOR_MF52A_ADC_BEREIK_MILLIVOLT 5000
#endif
#if SENSOR_MF52A_ADC_BEREIK_MILLIVOLT < 1
#error SENSOR_MF52A_ADC_BEREIK_MILLIVOLT moet groter dan 0 zijn (de maximale spanning van de sensoruitgang in mV).
#endif

#ifndef SENSOR_MF52D_AANTAL
  #define SENSOR_MF52D_AANTAL 0
#endif
#if SENSOR_MF52D_AANTAL != 0 && SENSOR_MF52D_AANTAL != 1
  #error SENSOR_MF52D_AANTAL moet 0 of 1 zijn.
#endif
#ifndef SENSOR_MF52D_ADC_BEREIK_MILLIVOLT
#define SENSOR_MF52D_ADC_BEREIK_MILLIVOLT 5000
#endif
#if SENSOR_MF52D_ADC_BEREIK_MILLIVOLT < 1
#error SENSOR_MF52D_ADC_BEREIK_MILLIVOLT moet groter dan 0 zijn (de maximale spanning van de sensoruitgang in mV).
#endif

#ifndef I2C_ADDRESS_SENSOR_MPU6050_1
#define I2C_ADDRESS_SENSOR_MPU6050_1 I2C_ADDRESS_NONE
#endif
#ifndef SENSOR_MPU6050_AANTAL
#define SENSOR_MPU6050_AANTAL 0
#endif
#if SENSOR_MPU6050_AANTAL != 0 && SENSOR_MPU6050_AANTAL != 1
#error SENSOR_MPU6050_AANTAL moet 0 of 1 zijn.
#endif

#ifndef I2C_ADDRESS_SENSOR_MPU6500_1
#define I2C_ADDRESS_SENSOR_MPU6500_1 I2C_ADDRESS_NONE
#endif
#ifndef SENSOR_MPU6500_AANTAL
#define SENSOR_MPU6500_AANTAL 0
#endif
#if SENSOR_MPU6500_AANTAL != 0 && SENSOR_MPU6500_AANTAL != 1
#error SENSOR_MPU6500_AANTAL moet 0 of 1 zijn.
#endif

#ifndef SENSOR_MR60BHA2_AANTAL
#define SENSOR_MR60BHA2_AANTAL 0
#endif
#if SENSOR_MR60BHA2_AANTAL != 0 && SENSOR_MR60BHA2_AANTAL != 1
#error SENSOR_MR60BHA2_AANTAL moet 0 of 1 zijn.
#endif

#ifndef SENSOR_NEUROSKYTGAM_AANTAL
#define SENSOR_NEUROSKYTGAM_AANTAL 0
#endif
#if SENSOR_NEUROSKYTGAM_AANTAL != 0 && SENSOR_NEUROSKYTGAM_AANTAL != 1
#error SENSOR_NEUROSKYTGAM_AANTAL moet 0 of 1 zijn.
#endif

#ifndef SENSOR_OT3686_AANTAL
#define SENSOR_OT3686_AANTAL 0
#endif
#if SENSOR_OT3686_AANTAL != 0 && SENSOR_OT3686_AANTAL != 1
#error SENSOR_OT3686_AANTAL moet 0 of 1 zijn (één native interrupt per Sensor).
#endif
#ifndef SENSOR_OT3686_AANTAL_SLEUVEN
#define SENSOR_OT3686_AANTAL_SLEUVEN 20
#endif
#if SENSOR_OT3686_AANTAL_SLEUVEN < 1
#error SENSOR_OT3686_AANTAL_SLEUVEN moet minstens 1 zijn.
#endif
#ifndef SENSOR_OT3686_MEETINTERVAL_MS
#define SENSOR_OT3686_MEETINTERVAL_MS 100
#endif
#if SENSOR_OT3686_MEETINTERVAL_MS < 10
#error SENSOR_OT3686_MEETINTERVAL_MS moet minstens 10 zijn.
#endif
#ifndef SENSOR_OT3686_STILSTAND_TIMEOUT_MS
#define SENSOR_OT3686_STILSTAND_TIMEOUT_MS 1000
#endif
#ifndef SENSOR_OT3686_MINIMALE_PULSPERIODE_US
#define SENSOR_OT3686_MINIMALE_PULSPERIODE_US 100
#endif

#ifndef SENSOR_OV7670_AANTAL
#define SENSOR_OV7670_AANTAL 0
#endif
#if SENSOR_OV7670_AANTAL != 0 && SENSOR_OV7670_AANTAL != 1
#error SENSOR_OV7670_AANTAL moet 0 of 1 zijn.
#endif

#ifndef SENSOR_PIEZO_AANTAL
  #define SENSOR_PIEZO_AANTAL 0
#endif
#if SENSOR_PIEZO_AANTAL != 0 && SENSOR_PIEZO_AANTAL != 1
  #error SENSOR_PIEZO_AANTAL moet 0 of 1 zijn.
#endif
#ifndef SENSOR_PIEZO_ADC_BEREIK_MILLIVOLT
#define SENSOR_PIEZO_ADC_BEREIK_MILLIVOLT 5000
#endif
#if SENSOR_PIEZO_ADC_BEREIK_MILLIVOLT < 1
#error SENSOR_PIEZO_ADC_BEREIK_MILLIVOLT moet groter dan 0 zijn (de maximale spanning van de sensoruitgang in mV).
#endif

#ifndef I2C_ADDRESS_SENSOR_SCD41_1
#define I2C_ADDRESS_SENSOR_SCD41_1 I2C_ADDRESS_NONE
#endif
#ifndef SENSOR_SCD41_AANTAL
#define SENSOR_SCD41_AANTAL 0
#endif
#if SENSOR_SCD41_AANTAL != 0 && SENSOR_SCD41_AANTAL != 1
#error SENSOR_SCD41_AANTAL moet 0 of 1 zijn.
#endif

#ifndef I2C_ADDRESS_SENSOR_SDP810125PA_1
#define I2C_ADDRESS_SENSOR_SDP810125PA_1 I2C_ADDRESS_NONE
#endif
#ifndef SENSOR_SDP810125PA_AANTAL
#define SENSOR_SDP810125PA_AANTAL 0
#endif
#if SENSOR_SDP810125PA_AANTAL != 0 && SENSOR_SDP810125PA_AANTAL != 1
#error SENSOR_SDP810125PA_AANTAL moet 0 of 1 zijn.
#endif

#ifndef SENSOR_TTP223_AANTAL
#define SENSOR_TTP223_AANTAL 0
#endif
#if SENSOR_TTP223_AANTAL != 0 && SENSOR_TTP223_AANTAL != 1
#error SENSOR_TTP223_AANTAL moet 0 of 1 zijn.
#endif
#ifndef SENSOR_TTP223_DIRECTE_MODUS
#define SENSOR_TTP223_DIRECTE_MODUS 1
#endif
#if SENSOR_TTP223_DIRECTE_MODUS != 0 && SENSOR_TTP223_DIRECTE_MODUS != 1
#error SENSOR_TTP223_DIRECTE_MODUS moet 0 of 1 zijn.
#endif
#ifndef SENSOR_TTP223_LANGE_AANRAKING_MS
#define SENSOR_TTP223_LANGE_AANRAKING_MS 500
#endif
#ifndef SENSOR_TTP223_DUBBELE_AANRAKING_MS
#define SENSOR_TTP223_DUBBELE_AANRAKING_MS 350
#endif

#ifndef I2C_ADDRESS_SENSOR_VL53L0X_1
#define I2C_ADDRESS_SENSOR_VL53L0X_1 I2C_ADDRESS_NONE
#endif
#ifndef SENSOR_VL53L0X_AANTAL
#define SENSOR_VL53L0X_AANTAL 0
#endif
#if SENSOR_VL53L0X_AANTAL != 0 && SENSOR_VL53L0X_AANTAL != 1
#error SENSOR_VL53L0X_AANTAL moet 0 of 1 zijn.
#endif

#ifndef I2C_ADDRESS_SENSOR_VL6180X_1
#define I2C_ADDRESS_SENSOR_VL6180X_1 I2C_ADDRESS_NONE
#endif
#ifndef SENSOR_VL6180X_AANTAL
#define SENSOR_VL6180X_AANTAL 0
#endif
#if SENSOR_VL6180X_AANTAL != 0 && SENSOR_VL6180X_AANTAL != 1
#error SENSOR_VL6180X_AANTAL moet 0 of 1 zijn.
#endif

#ifndef SENSOR_WATER_AANTAL
  #define SENSOR_WATER_AANTAL 0
#endif
#if SENSOR_WATER_AANTAL != 0 && SENSOR_WATER_AANTAL != 1
  #error SENSOR_WATER_AANTAL moet 0 of 1 zijn.
#endif
#ifndef SENSOR_WATER_ADC_BEREIK_MILLIVOLT
#define SENSOR_WATER_ADC_BEREIK_MILLIVOLT 5000
#endif
#if SENSOR_WATER_ADC_BEREIK_MILLIVOLT < 1
#error SENSOR_WATER_ADC_BEREIK_MILLIVOLT moet groter dan 0 zijn (de maximale spanning van de sensoruitgang in mV).
#endif

// ****************************************************************************
// VALIDATIES
// ****************************************************************************

// BOARD
#if BOARD_VERSION != BOARD_UNO_R4_MINIMA && BOARD_VERSION != BOARD_UNO_R4_WIFI && BOARD_VERSION != BOARD_UNO_Q && BOARD_VERSION != BOARD_ESP32S3_ARDI32 && BOARD_VERSION != BOARD_ESP32S3_DEV && BOARD_VERSION != BOARD_ESP32_D1_UNO_R32 && BOARD_VERSION != BOARD_RP2040_CYTRON_MAKER_UNO && BOARD_VERSION != BOARD_STM32F4_NUCLEO64_F401RE
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
// BOARD_VERSION VERSUS ARDUINO UNO-SHIELDPINNEN ==============================
// ============================================================================

// Gebruik deze GPIOxx-labels in plaats van kale cijfers, zodat de mapping leesbaar en controleerbaar blijft.
#if BOARD_VERSION == BOARD_UNO_R4_MINIMA || BOARD_VERSION == BOARD_UNO_R4_WIFI || BOARD_VERSION == BOARD_UNO_Q
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
  #else // Standaard voor Uno R4 Minima en R4 WiFi
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

  /* MOGELIJKE UITBREIDING NA v2.0.0?
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
// DATA =======================================================================
// ============================================================================

// ============================================================================
// DATA: EEPROM NATIVE
// ============================================================================
// Ruimte in bytes die Data krijgt in de native EEPROM van het board. Ze begint na het blok van HX1838 (HX1838_EEPROM_NATIVE_SIZE).
#ifndef DATA_LAYER_EEPROM_NATIVE_SIZE
  #define DATA_LAYER_EEPROM_NATIVE_SIZE 0
#endif

// ============================================================================
// DATA: EEPROM
// ============================================================================
#ifndef DATA_LAYER_EEPROM_I2C_AANTAL
  #if ((DATA_LAYER_CONFIG) & DATA_TYPE_EEPROM_I2C)
    #define DATA_LAYER_EEPROM_I2C_AANTAL 1
  #else
    #define DATA_LAYER_EEPROM_I2C_AANTAL 0
  #endif
#endif

// ============================================================================
// DEFAULT: EEPROM #1
// ============================================================================
#if DATA_LAYER_EEPROM_I2C_AANTAL >= 1
  #ifndef DATA_LAYER_EEPROM_I2C_1_I2C_ADDRESS
    #define DATA_LAYER_EEPROM_I2C_1_I2C_ADDRESS I2C_ADDRESS_NONE
  #endif
#endif

// ============================================================================
// EXPERIMENTEEL: EEPROM #2
// ============================================================================
#if DATA_LAYER_EEPROM_I2C_AANTAL >= 2
  #ifndef DATA_LAYER_EEPROM_I2C_2_I2C_ADDRESS
    #define DATA_LAYER_EEPROM_I2C_2_I2C_ADDRESS I2C_ADDRESS_NONE
  #endif
#endif


// ============================================================================
// DATA: FLASH
// ============================================================================
#ifndef DATA_LAYER_FLASH_SPI_AANTAL
  #if ((DATA_LAYER_CONFIG) & DATA_TYPE_FLASH_SPI)
    #define DATA_LAYER_FLASH_SPI_AANTAL 1
  #else
    #define DATA_LAYER_FLASH_SPI_AANTAL 0
  #endif
#endif

// ============================================================================
// DEFAULT: FLASH #1
// ============================================================================
#if DATA_LAYER_FLASH_SPI_AANTAL >= 1
  #ifndef DATA_LAYER_FLASH_SPI_1_CS_PIN
    #define DATA_LAYER_FLASH_SPI_1_CS_PIN HardwareResourcePin::NONE
  #endif
  #ifndef DATA_LAYER_FLASH_SPI_1_BUFFER_SIZE
    #define DATA_LAYER_FLASH_SPI_1_BUFFER_SIZE 0
  #endif
#endif

// ============================================================================
// EXPERIMENTEEL: FLASH #2
// ============================================================================
#if DATA_LAYER_FLASH_SPI_AANTAL >= 2
  #ifndef DATA_LAYER_FLASH_SPI_2_CS_PIN
    #define DATA_LAYER_FLASH_SPI_2_CS_PIN HardwareResourcePin::NONE
  #endif
  #ifndef DATA_LAYER_FLASH_SPI_2_BUFFER_SIZE
    #define DATA_LAYER_FLASH_SPI_2_BUFFER_SIZE 0
  #endif
#endif


// ============================================================================
// DATA: FRAM
// ============================================================================
#ifndef DATA_LAYER_FRAM_I2C_AANTAL
  #if ((DATA_LAYER_CONFIG) & DATA_TYPE_FRAM_I2C)
    #define DATA_LAYER_FRAM_I2C_AANTAL 1
  #else
    #define DATA_LAYER_FRAM_I2C_AANTAL 0
  #endif
#endif

// ============================================================================
// DEFAULT: FRAM #1
// ============================================================================
#if DATA_LAYER_FRAM_I2C_AANTAL >= 1
  #ifndef DATA_LAYER_FRAM_I2C_1_I2C_ADDRESS
    #define DATA_LAYER_FRAM_I2C_1_I2C_ADDRESS I2C_ADDRESS_NONE
  #endif
#endif

// ============================================================================
// EXPERIMENTEEL: FRAM #2
// ============================================================================
#if DATA_LAYER_FRAM_I2C_AANTAL >= 2
  #ifndef DATA_LAYER_FRAM_I2C_2_I2C_ADDRESS
    #define DATA_LAYER_FRAM_I2C_2_I2C_ADDRESS I2C_ADDRESS_NONE
  #endif
#endif


// ============================================================================
// DATA: microSD
// ============================================================================
#ifndef DATA_LAYER_MICROSD_SPI_FLUSH_NA_AANTAL_REGEL
  #define DATA_LAYER_MICROSD_SPI_FLUSH_NA_AANTAL_REGEL 50
#endif
#if DATA_LAYER_MICROSD_SPI_FLUSH_NA_AANTAL_REGEL < 1 || DATA_LAYER_MICROSD_SPI_FLUSH_NA_AANTAL_REGEL > 255
  #error DATA_LAYER_MICROSD_SPI_FLUSH_NA_AANTAL_REGEL moet tussen 1 en 255 liggen.
#endif
#ifndef DATA_LAYER_MICROSD_SPI_FLUSH_NA_MS
  #define DATA_LAYER_MICROSD_SPI_FLUSH_NA_MS 5000UL
#endif

#ifndef DATA_LAYER_MICROSD_SPI_AANTAL
  #if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD_SPI) || REDIRECT_SERIAL == REDIRECT_SERIAL_TO_MICROSD
    #define DATA_LAYER_MICROSD_SPI_AANTAL 1
  #else
    #define DATA_LAYER_MICROSD_SPI_AANTAL 0
  #endif
#endif

// ============================================================================
// DEFAULT: microSD #1
// ============================================================================
#if DATA_LAYER_MICROSD_SPI_AANTAL >= 1
  #ifndef DATA_LAYER_MICROSD_SPI_1_CS_PIN
    #define DATA_LAYER_MICROSD_SPI_1_CS_PIN HardwareResourcePin::NONE
  #endif
#endif

// ============================================================================
// EXPERIMENTEEL: microSD #2
// ============================================================================
#if DATA_LAYER_MICROSD_SPI_AANTAL >= 2
  #ifndef DATA_LAYER_MICROSD_SPI_2_CS_PIN
    #define DATA_LAYER_MICROSD_SPI_2_CS_PIN HardwareResourcePin::NONE
  #endif
#endif


// ============================================================================
// DATA: NRF24L01 (radiomodule)
// ============================================================================
#ifndef DATA_LAYER_NRF24L01_SPI_AANTAL
  #if ((DATA_LAYER_CONFIG) & DATA_TYPE_NRF24L01_SPI)
    #define DATA_LAYER_NRF24L01_SPI_AANTAL 1
  #else
    #define DATA_LAYER_NRF24L01_SPI_AANTAL 0
  #endif
#endif

// ============================================================================
// DEFAULT: NRF24L01 #1
// ============================================================================
#if DATA_LAYER_NRF24L01_SPI_AANTAL >= 1
  #ifndef DATA_LAYER_NRF24L01_SPI_1_CS_PIN
    #define DATA_LAYER_NRF24L01_SPI_1_CS_PIN HardwareResourcePin::NONE
  #endif
  #ifndef DATA_LAYER_NRF24L01_SPI_1_CE_PIN
    #define DATA_LAYER_NRF24L01_SPI_1_CE_PIN HardwareResourcePin::NONE
  #endif
  #ifndef DATA_LAYER_NRF24L01_SPI_1_RF_CHANNEL
    #define DATA_LAYER_NRF24L01_SPI_1_RF_CHANNEL 76     // radiokanaal 0..125 (2400 MHz + kanaal)
  #endif
  #ifndef DATA_LAYER_NRF24L01_SPI_1_ADRES
    #define DATA_LAYER_NRF24L01_SPI_1_ADRES "GROEI"     // precies 5 tekens; zender en ontvanger gebruiken hetzelfde adres
  #endif
#endif


// ============================================================================
// DATA: Validatie
// ============================================================================
#ifdef DATA_QUEUE_OR_STACK_ACTIEF
  #undef DATA_QUEUE_OR_STACK_ACTIEF
#endif

#if (DATA_LAYER_CONFIG) != DATA_TYPE_NONE
  #define DATA_QUEUE_OR_STACK_ACTIEF 1
#else
  #define DATA_QUEUE_OR_STACK_ACTIEF 0
#endif

// AFGELEID (niet zelf instellen): exclusieve runtimeclaim op een FLASH- of microSD-object.
// Alleen nodig wanneer minstens twee schrijvers naar opslag samen gebruikt worden:
// Data (FLASH/microSD), REDIRECT_SERIAL naar microSD en OV7670 (beeld naar microSD).
#if ((((DATA_LAYER_CONFIG) & (DATA_TYPE_FLASH_SPI | DATA_TYPE_MICROSD_SPI)) != 0) + (REDIRECT_SERIAL == REDIRECT_SERIAL_TO_MICROSD) + (SENSOR_OV7670_AANTAL > 0)) >= 2
  #define DATA_LAYER_EXCLUSIEVE_OPSLAG_ACTIEF 1
#else
  #define DATA_LAYER_EXCLUSIEVE_OPSLAG_ACTIEF 0
#endif


// S-09: capaciteit voor automatische Sensor-QueueOrStack, exclusief de bestaande FsrRfp-tabellen.
// Met DATA_LAYER_SENSOR_QUEUE_OR_STACK_SIZE_FOR(component, channel) kan per sensor en kanaal
// een andere capaciteit worden gekozen. 0 = alleen lastValue, geen FIFO/LIFO.
#ifndef DATA_LAYER_SENSOR_QUEUE_OR_STACK_SIZE
  #if BOARD_VERSION == BOARD_ESP32S3_ARDI32 || BOARD_VERSION == BOARD_ESP32S3_DEV
    #define DATA_LAYER_SENSOR_QUEUE_OR_STACK_SIZE 32
  #else
    #define DATA_LAYER_SENSOR_QUEUE_OR_STACK_SIZE 16
  #endif
#endif
#if DATA_LAYER_SENSOR_QUEUE_OR_STACK_SIZE < 0 || DATA_LAYER_SENSOR_QUEUE_OR_STACK_SIZE > 65535
  #error DATA_LAYER_SENSOR_QUEUE_OR_STACK_SIZE moet tussen 0 en 65535 liggen.
#endif
#ifndef DATA_LAYER_SENSOR_QUEUE_OR_STACK_SIZE_FOR
  #define DATA_LAYER_SENSOR_QUEUE_OR_STACK_SIZE_FOR(component, channel) DATA_LAYER_SENSOR_QUEUE_OR_STACK_SIZE
#endif

// ============================================================================
// GEDEELDEBUS: MIXED PINS
// ============================================================================
// STURINGEN: Defaults per component; persoonlijke Config*.h is eerder ingelezen.
#ifndef STURING_A4988_AANTAL
  #define STURING_A4988_AANTAL 0
#endif
#ifndef STURING_L293D_AANTAL
  #define STURING_L293D_AANTAL 0
#endif
#ifndef STURING_L298N_AANTAL
  #define STURING_L298N_AANTAL 0
#endif
#ifndef STURING_MAX7219_AANTAL
  #define STURING_MAX7219_AANTAL 0
#endif
#ifndef STURING_MG996R_AANTAL
  #define STURING_MG996R_AANTAL 0
#endif
#ifndef STURING_RELAIS_AANTAL
  #define STURING_RELAIS_AANTAL 0
#endif
#ifndef STURING_RGBLED_AANTAL
  #define STURING_RGBLED_AANTAL 0
#endif
#ifndef STURING_SG90_AANTAL
  #define STURING_SG90_AANTAL 0
#endif
#ifndef STURING_ULN2003_AANTAL
  #define STURING_ULN2003_AANTAL 0
#endif
#ifndef STURING_WS2812_AANTAL
  #define STURING_WS2812_AANTAL 0
#endif
#ifndef STURING_SG90_MIN_PWM
  #define STURING_SG90_MIN_PWM 205
#endif
#ifndef STURING_SG90_MAX_PWM
  #define STURING_SG90_MAX_PWM 410
#endif
#ifndef STURING_MG996R_MIN_PWM
  #define STURING_MG996R_MIN_PWM 205
#endif
#ifndef STURING_MG996R_MAX_PWM
  #define STURING_MG996R_MAX_PWM 410
#endif
#ifndef STURING_WS2812_WIDTH
  #define STURING_WS2812_WIDTH 8
#endif
#ifndef STURING_WS2812_HEIGHT
  #define STURING_WS2812_HEIGHT 8
#endif

// GEDEELDE_BUS_MIXED_PINS: 1 = een component mag per exclusieve pin een eigen gebied hebben (het board of een Extender),
// bv. een SPI-chip met SCK/MISO/MOSI op het board en CS op een PCF8574. 0 = elk component heeft één gebied (zoals voorheen), zonder extra geheugen.
// Wordt automatisch 1 wanneer een Data-chip CS via een Extender gebruikt.
#if defined(DATA_LAYER_FLASH_SPI_1_CS_EXTENDER) || defined(DATA_LAYER_FLASH_SPI_2_CS_EXTENDER) || \
    defined(DATA_LAYER_MICROSD_SPI_1_CS_EXTENDER) || defined(DATA_LAYER_MICROSD_SPI_2_CS_EXTENDER)
  #undef GEDEELDE_BUS_MIXED_PINS
  #define GEDEELDE_BUS_MIXED_PINS 1
#endif
#ifndef GEDEELDE_BUS_MIXED_PINS
  #define GEDEELDE_BUS_MIXED_PINS 0
#endif


// ============================================================================
// GEDEELDEBUS: RUNTIME-HERSTEL
// ============================================================================
// Elk I2C-component (Extenders, Input PCF8574, LCD) telt zijn opeenvolgende fouten tijdens het draaien en klimt de foutladder op:
// WARNING, ALERT, FAULT, CRITICAL, FATAL. Een niveau komt er na het aantal fouten OF na de tijd sinds de eerste fout, wat eerst bereikt is.
// Vanaf RUNTIME_COMPONENT_UITSCHAKELEN_VANAF_FOUTNIVEAU: uitschakelen, elke RUNTIME_PAUZE_TUSSEN_HERSTELPOGINGEN_MS een herstelpoging.
// Overrides (alleen wie ze zet, betaalt ze), waarde { {5 aantallen}, {5 tijden in ms}, hoogst toegelaten foutniveau, uitschakelen vanaf foutniveau }:
//   EXTENDER_<TYPE>_RUNTIME_DREMPELS_FOUTNIVEAU, EXTENDER_<TYPE>_<N>_RUNTIME_DREMPELS_FOUTNIVEAU,
//   <SENSOR>_RUNTIME_DREMPELS_FOUTNIVEAU, <SENSOR>_CHANNEL_<N>_RUNTIME_DREMPELS_FOUTNIVEAU.
#ifndef RUNTIME_WARNING_OPEENVOLGENDE_FOUTEN_NA_AANTAL
  #define RUNTIME_WARNING_OPEENVOLGENDE_FOUTEN_NA_AANTAL 1
#endif
#ifndef RUNTIME_ALERT_OPEENVOLGENDE_FOUTEN_NA_AANTAL
  #define RUNTIME_ALERT_OPEENVOLGENDE_FOUTEN_NA_AANTAL 3
#endif
#ifndef RUNTIME_FAULT_OPEENVOLGENDE_FOUTEN_NA_AANTAL
  #define RUNTIME_FAULT_OPEENVOLGENDE_FOUTEN_NA_AANTAL 5
#endif
#ifndef RUNTIME_CRITICAL_OPEENVOLGENDE_FOUTEN_NA_AANTAL
  #define RUNTIME_CRITICAL_OPEENVOLGENDE_FOUTEN_NA_AANTAL 10
#endif
#ifndef RUNTIME_FATAL_OPEENVOLGENDE_FOUTEN_NA_AANTAL
  #define RUNTIME_FATAL_OPEENVOLGENDE_FOUTEN_NA_AANTAL 20
#endif

#ifndef RUNTIME_WARNING_SINDS_EERSTE_FOUT_NA_MS
  #define RUNTIME_WARNING_SINDS_EERSTE_FOUT_NA_MS 0UL   // 0 = dit foutniveau niet op tijd
#endif
#ifndef RUNTIME_ALERT_SINDS_EERSTE_FOUT_NA_MS
  #define RUNTIME_ALERT_SINDS_EERSTE_FOUT_NA_MS 100UL
#endif
#ifndef RUNTIME_FAULT_SINDS_EERSTE_FOUT_NA_MS
  #define RUNTIME_FAULT_SINDS_EERSTE_FOUT_NA_MS 500UL
#endif
#ifndef RUNTIME_CRITICAL_SINDS_EERSTE_FOUT_NA_MS
  #define RUNTIME_CRITICAL_SINDS_EERSTE_FOUT_NA_MS 2000UL
#endif
#ifndef RUNTIME_FATAL_SINDS_EERSTE_FOUT_NA_MS
  #define RUNTIME_FATAL_SINDS_EERSTE_FOUT_NA_MS 10000UL
#endif

#ifndef RUNTIME_HOOGST_TOEGELATEN_FOUTNIVEAU
  #define RUNTIME_HOOGST_TOEGELATEN_FOUTNIVEAU ScreenData::TYPE_FATAL   // ScreenData::TYPE_WARNING tot ScreenData::TYPE_FATAL
#endif
#ifndef RUNTIME_COMPONENT_UITSCHAKELEN_VANAF_FOUTNIVEAU
  #define RUNTIME_COMPONENT_UITSCHAKELEN_VANAF_FOUTNIVEAU ScreenData::TYPE_CRITICAL   // ScreenData::TYPE_WARNING tot ScreenData::TYPE_FATAL
#endif
#ifndef RUNTIME_PAUZE_TUSSEN_HERSTELPOGINGEN_MS
  #define RUNTIME_PAUZE_TUSSEN_HERSTELPOGINGEN_MS 100UL
#endif
// Maximale duur van één I2C-transactie (setWireTimeout op ondersteunde cores).
#ifndef I2C_TIMEOUT_PER_TRANSACTIE_US
  #define I2C_TIMEOUT_PER_TRANSACTIE_US 25000UL
#endif

#if defined(DATA_LAYER_FLASH_SPI_1_CS_EXTENDER) && !defined(DATA_LAYER_FLASH_SPI_1_CS_EXTENDER_I2C_ADDRESS)
  #error DATA_LAYER_FLASH_SPI_1_CS_EXTENDER vereist DATA_LAYER_FLASH_SPI_1_CS_EXTENDER_I2C_ADDRESS.
#endif
#if defined(DATA_LAYER_FLASH_SPI_2_CS_EXTENDER) && !defined(DATA_LAYER_FLASH_SPI_2_CS_EXTENDER_I2C_ADDRESS)
  #error DATA_LAYER_FLASH_SPI_2_CS_EXTENDER vereist DATA_LAYER_FLASH_SPI_2_CS_EXTENDER_I2C_ADDRESS.
#endif
#if defined(DATA_LAYER_MICROSD_SPI_1_CS_EXTENDER) && !defined(DATA_LAYER_MICROSD_SPI_1_CS_EXTENDER_I2C_ADDRESS)
  #error DATA_LAYER_MICROSD_SPI_1_CS_EXTENDER vereist DATA_LAYER_MICROSD_SPI_1_CS_EXTENDER_I2C_ADDRESS.
#endif
#if defined(DATA_LAYER_MICROSD_SPI_2_CS_EXTENDER) && !defined(DATA_LAYER_MICROSD_SPI_2_CS_EXTENDER_I2C_ADDRESS)
  #error DATA_LAYER_MICROSD_SPI_2_CS_EXTENDER vereist DATA_LAYER_MICROSD_SPI_2_CS_EXTENDER_I2C_ADDRESS.
#endif


#if ((DATA_LAYER_CONFIG) & (DATA_TYPE_EEPROM_SPI | DATA_TYPE_FRAM_SPI))
  #error DATA_TYPE_EEPROM_SPI en DATA_TYPE_FRAM_SPI worden nog niet ondersteund in v2.0.0.
#endif

#if ((DATA_LAYER_CONFIG) & ~(DATA_TYPE_NONE | DATA_TYPE_DIRECT | DATA_TYPE_EEPROM_NATIVE | DATA_TYPE_EEPROM_I2C | DATA_TYPE_FLASH_SPI | DATA_TYPE_FRAM_I2C | DATA_TYPE_MICROSD_SPI | DATA_TYPE_NRF24L01_SPI))
  #error DATA_LAYER_CONFIG bevat een onbekend datatype.
#endif

#if (DATA_LAYER_CONFIG) == 0
  #error DATA_LAYER_CONFIG moet minstens één datatype bevatten.
#endif

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_NONE) && (DATA_LAYER_CONFIG) != DATA_TYPE_NONE
  #error DATA_TYPE_NONE kan niet met een ander datatype worden gecombineerd.
#endif


#if ((DATA_LAYER_CONFIG) & DATA_TYPE_EEPROM_NATIVE) && DATA_LAYER_EEPROM_NATIVE_SIZE == 0
  #error DATA_TYPE_EEPROM_NATIVE vereist DATA_LAYER_EEPROM_NATIVE_SIZE > 0.
#endif


#if ((DATA_LAYER_CONFIG) & DATA_TYPE_EEPROM_I2C) && DATA_LAYER_EEPROM_I2C_AANTAL < 1
  #error DATA_TYPE_EEPROM_I2C vereist DATA_LAYER_EEPROM_I2C_AANTAL >= 1.
#endif

#if !((DATA_LAYER_CONFIG) & DATA_TYPE_EEPROM_I2C) && DATA_LAYER_EEPROM_I2C_AANTAL > 0
  #error DATA_LAYER_EEPROM_I2C_AANTAL > 0 vereist DATA_TYPE_EEPROM_I2C in DATA_LAYER_CONFIG.
#endif

#if DATA_LAYER_EEPROM_I2C_AANTAL > 2
  #error DATA_LAYER_EEPROM_I2C_AANTAL ondersteunt momenteel maximaal 2 instanties.
#endif


#if ((DATA_LAYER_CONFIG) & DATA_TYPE_FLASH_SPI) && DATA_LAYER_FLASH_SPI_AANTAL < 1
  #error DATA_TYPE_FLASH_SPI vereist DATA_LAYER_FLASH_SPI_AANTAL >= 1.
#endif

#if !((DATA_LAYER_CONFIG) & DATA_TYPE_FLASH_SPI) && DATA_LAYER_FLASH_SPI_AANTAL > 0
  #error DATA_LAYER_FLASH_SPI_AANTAL > 0 vereist DATA_TYPE_FLASH_SPI in DATA_LAYER_CONFIG.
#endif

#if DATA_LAYER_FLASH_SPI_AANTAL >= 1 && (DATA_LAYER_FLASH_SPI_1_BUFFER_SIZE < 0 || DATA_LAYER_FLASH_SPI_1_BUFFER_SIZE > 256)
  #error DATA_LAYER_FLASH_SPI_1_BUFFER_SIZE mag maximaal 256 zijn (één pagina).
#endif
#if DATA_LAYER_FLASH_SPI_AANTAL >= 2 && (DATA_LAYER_FLASH_SPI_2_BUFFER_SIZE < 0 || DATA_LAYER_FLASH_SPI_2_BUFFER_SIZE > 256)
  #error DATA_LAYER_FLASH_SPI_2_BUFFER_SIZE mag maximaal 256 zijn (één pagina).
#endif

#if DATA_LAYER_FLASH_SPI_AANTAL > 2
  #error DATA_LAYER_FLASH_SPI_AANTAL ondersteunt momenteel maximaal 2 instanties.
#endif


#if ((DATA_LAYER_CONFIG) & DATA_TYPE_FRAM_I2C) && DATA_LAYER_FRAM_I2C_AANTAL < 1
  #error DATA_TYPE_FRAM_I2C vereist DATA_LAYER_FRAM_I2C_AANTAL >= 1.
#endif

#if !((DATA_LAYER_CONFIG) & DATA_TYPE_FRAM_I2C) && DATA_LAYER_FRAM_I2C_AANTAL > 0
  #error DATA_LAYER_FRAM_I2C_AANTAL > 0 vereist DATA_TYPE_FRAM_I2C in DATA_LAYER_CONFIG.
#endif

#if DATA_LAYER_FRAM_I2C_AANTAL > 2
  #error DATA_LAYER_FRAM_I2C_AANTAL ondersteunt momenteel maximaal 2 instanties.
#endif


#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD_SPI) && DATA_LAYER_MICROSD_SPI_AANTAL < 1
  #error DATA_TYPE_MICROSD_SPI vereist DATA_LAYER_MICROSD_SPI_AANTAL >= 1.
#endif

#if !((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD_SPI) && REDIRECT_SERIAL != REDIRECT_SERIAL_TO_MICROSD && DATA_LAYER_MICROSD_SPI_AANTAL > 0
  #error DATA_LAYER_MICROSD_SPI_AANTAL > 0 vereist DATA_TYPE_MICROSD_SPI in DATA_LAYER_CONFIG of REDIRECT_SERIAL_TO_MICROSD.
#endif

#if DATA_LAYER_MICROSD_SPI_AANTAL > 2
  #error DATA_LAYER_MICROSD_SPI_AANTAL ondersteunt momenteel maximaal 2 instanties.
#endif


#if ((DATA_LAYER_CONFIG) & DATA_TYPE_NRF24L01_SPI) && DATA_LAYER_NRF24L01_SPI_AANTAL < 1
  #error DATA_TYPE_NRF24L01_SPI vereist DATA_LAYER_NRF24L01_SPI_AANTAL >= 1.
#endif

#if !((DATA_LAYER_CONFIG) & DATA_TYPE_NRF24L01_SPI) && DATA_LAYER_NRF24L01_SPI_AANTAL > 0
  #error DATA_LAYER_NRF24L01_SPI_AANTAL > 0 vereist DATA_TYPE_NRF24L01_SPI in DATA_LAYER_CONFIG.
#endif

#if DATA_LAYER_NRF24L01_SPI_AANTAL > 1
  #error DATA_LAYER_NRF24L01_SPI_AANTAL ondersteunt momenteel maximaal 1 instantie.
#endif

#if DATA_LAYER_NRF24L01_SPI_AANTAL >= 1 && (DATA_LAYER_NRF24L01_SPI_1_RF_CHANNEL < 0 || DATA_LAYER_NRF24L01_SPI_1_RF_CHANNEL > 125)
  #error DATA_LAYER_NRF24L01_SPI_1_RF_CHANNEL moet tussen 0 en 125 liggen.
#endif


// ============================================================================
// EXTENDERS ==================================================================
// ============================================================================
// Elke EXTENDER_<NAAM>_AANTAL bepaalt hoeveel instanties er zijn; elke adres-/CS-macro staat achter zijn eigen if op dat aantal,
// zodat er nooit een macro bestaat voor een instantie die niet actief is.
// DEFAULT: EXTENDER_<NAAM>_AANTAL = 0 of 1.
// EXPERIMENTEEL: EXTENDER_<NAAM>_AANTAL >= 2.
// ============================================================================

// ============================================================================
// EXTENDER: ADS1115
// ============================================================================
#ifndef EXTENDER_ADS1115_AANTAL
  #define EXTENDER_ADS1115_AANTAL 1
#endif
// ADS1115 data rate: 0 = 8, 1 = 16, 2 = 32, 3 = 64, 4 = 128, 5 = 250, 6 = 475, 7 = 860 samples per seconde.
// Snellere biosignalen (bv. BioAmp EEG 256 Hz of EMG 500 Hz) vragen 7.
#ifndef EXTENDER_ADS1115_DATARATE
  #define EXTENDER_ADS1115_DATARATE 4
#endif
#if EXTENDER_ADS1115_DATARATE < 0 || EXTENDER_ADS1115_DATARATE > 7
  #error EXTENDER_ADS1115_DATARATE moet tussen 0 en 7 liggen.
#endif

#if EXTENDER_ADS1115_AANTAL > 2
  #error EXTENDER_ADS1115_AANTAL ondersteunt momenteel maximaal 2 instanties.
#endif

// ============================================================================
// DEFAULT: ADS1115 #1
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
// EXPERIMENTEEL: ADS1115 #2
// ============================================================================
#if EXTENDER_ADS1115_AANTAL >= 2
  // EXCLUSIEF: I2C-adres van deze extender-instance op dezelfde I2C-bus.
  #ifndef I2C_ADDRESS_EXTENDER_ADS1115_2
    #define I2C_ADDRESS_EXTENDER_ADS1115_2 I2C_ADDRESS_NONE
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_ADS1115_2_ALERT_RDY
    #define EXTENDER_ADS1115_2_ALERT_RDY HardwareResourcePin::NONE
  #endif
#endif


// ============================================================================
// Externe VREFP - VREFN van de ADS1158, in mV.
#ifndef EXTENDER_ADS1158_REFERENTIE_MILLIVOLT
  #define EXTENDER_ADS1158_REFERENTIE_MILLIVOLT 5000
#endif

// EXTENDER: ADS1158
// ============================================================================
#ifndef EXTENDER_ADS1158_AANTAL
  #define EXTENDER_ADS1158_AANTAL 0
#endif

#if EXTENDER_ADS1158_AANTAL > 2
  #error EXTENDER_ADS1158_AANTAL ondersteunt momenteel maximaal 2 instanties.
#endif

// ============================================================================
// DEFAULT: ADS1158 #1
// ============================================================================
#if EXTENDER_ADS1158_AANTAL >= 1
  // EXCLUSIEF: CS van deze extender-instance; kies een vrije HardwareResourcePin.
  #ifndef EXTENDER_ADS1158_1_CS_PIN
    #define EXTENDER_ADS1158_1_CS_PIN HardwareResourcePin::NONE
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
// EXPERIMENTEEL: ADS1158 #2
// ============================================================================
#if EXTENDER_ADS1158_AANTAL >= 2
  // EXCLUSIEF: CS van deze extender-instance; kies een vrije HardwareResourcePin.
  #ifndef EXTENDER_ADS1158_2_CS_PIN
    #define EXTENDER_ADS1158_2_CS_PIN HardwareResourcePin::NONE
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


// ============================================================================
// EXTENDER: ADS7828
// ============================================================================
#ifndef EXTENDER_ADS7828_AANTAL
  #define EXTENDER_ADS7828_AANTAL 0
#endif

#ifndef EXTENDER_ADS7828_EXTERNE_REFERENTIE
#define EXTENDER_ADS7828_EXTERNE_REFERENTIE 0
#endif
#if EXTENDER_ADS7828_EXTERNE_REFERENTIE != 0 && EXTENDER_ADS7828_EXTERNE_REFERENTIE != 1
#error EXTENDER_ADS7828_EXTERNE_REFERENTIE moet 0 (intern) of 1 (extern) zijn.
#endif
#ifndef EXTENDER_ADS7828_REFERENTIE_MILLIVOLT
#define EXTENDER_ADS7828_REFERENTIE_MILLIVOLT 2500
#endif
#if EXTENDER_ADS7828_REFERENTIE_MILLIVOLT < 1 || EXTENDER_ADS7828_REFERENTIE_MILLIVOLT > 5250
#error EXTENDER_ADS7828_REFERENTIE_MILLIVOLT moet binnen 1..5250 mV liggen.
#endif

#if EXTENDER_ADS7828_AANTAL > 2
  #error EXTENDER_ADS7828_AANTAL ondersteunt momenteel maximaal 2 instanties.
#endif

// ============================================================================
// DEFAULT: ADS7828 #1
// ============================================================================
#if EXTENDER_ADS7828_AANTAL >= 1
  // EXCLUSIEF: I2C-adres van deze extender-instance op dezelfde I2C-bus.
  #ifndef I2C_ADDRESS_EXTENDER_ADS7828_1
    #define I2C_ADDRESS_EXTENDER_ADS7828_1 I2C_ADDRESS_NONE
  #endif
#endif

// ============================================================================
// EXPERIMENTEEL: ADS7828 #2
// ============================================================================
#if EXTENDER_ADS7828_AANTAL >= 2
  // EXCLUSIEF: I2C-adres van deze extender-instance op dezelfde I2C-bus.
  #ifndef I2C_ADDRESS_EXTENDER_ADS7828_2
    #define I2C_ADDRESS_EXTENDER_ADS7828_2 I2C_ADDRESS_NONE
  #endif
#endif


// ============================================================================
// De ADS7953 heeft een EXTERNE analoge referentie op REF (typisch 2500 mV).
#ifndef EXTENDER_ADS7953_REFERENTIE_MILLIVOLT
  #define EXTENDER_ADS7953_REFERENTIE_MILLIVOLT 2500
#endif
// 0 = onbekend: 2xVREF-bereik wordt veilig geweigerd tot de echte +VA bekend is.
#ifndef EXTENDER_ADS7953_VOEDING_MILLIVOLT
  #define EXTENDER_ADS7953_VOEDING_MILLIVOLT 0
#endif
#if EXTENDER_ADS7953_VOEDING_MILLIVOLT != 0 && (EXTENDER_ADS7953_VOEDING_MILLIVOLT < 2700 || EXTENDER_ADS7953_VOEDING_MILLIVOLT > 5250)
  #error EXTENDER_ADS7953_VOEDING_MILLIVOLT moet 0 of 2700..5250 mV zijn.
#endif

// EXTENDER: ADS7953
// ============================================================================
#ifndef EXTENDER_ADS7953_AANTAL
  #define EXTENDER_ADS7953_AANTAL 0
#endif

#if EXTENDER_ADS7953_AANTAL > 2
  #error EXTENDER_ADS7953_AANTAL ondersteunt momenteel maximaal 2 instanties.
#endif

// ============================================================================
// DEFAULT: ADS7953 #1
// ============================================================================
#if EXTENDER_ADS7953_AANTAL >= 1
  // EXCLUSIEF: CS van deze extender-instance; kies een vrije HardwareResourcePin.
  #ifndef EXTENDER_ADS7953_1_CS_PIN
    #define EXTENDER_ADS7953_1_CS_PIN HardwareResourcePin::NONE
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_ADS7953_1_GPIO0_TO_UNO
    #define EXTENDER_ADS7953_1_GPIO0_TO_UNO HardwareResourcePin::NONE
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_ADS7953_1_GPIO1_TO_UNO
    #define EXTENDER_ADS7953_1_GPIO1_TO_UNO HardwareResourcePin::NONE
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_ADS7953_1_GPIO2_TO_UNO
    #define EXTENDER_ADS7953_1_GPIO2_TO_UNO HardwareResourcePin::NONE
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_ADS7953_1_GPIO3_TO_UNO
    #define EXTENDER_ADS7953_1_GPIO3_TO_UNO HardwareResourcePin::NONE
  #endif
#endif

// ============================================================================
// EXPERIMENTEEL: ADS7953 #2
// ============================================================================
#if EXTENDER_ADS7953_AANTAL >= 2
  // EXCLUSIEF: CS van deze extender-instance; kies een vrije HardwareResourcePin.
  #ifndef EXTENDER_ADS7953_2_CS_PIN
    #define EXTENDER_ADS7953_2_CS_PIN HardwareResourcePin::NONE
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_ADS7953_2_GPIO0_TO_UNO
    #define EXTENDER_ADS7953_2_GPIO0_TO_UNO HardwareResourcePin::NONE
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_ADS7953_2_GPIO1_TO_UNO
    #define EXTENDER_ADS7953_2_GPIO1_TO_UNO HardwareResourcePin::NONE
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_ADS7953_2_GPIO2_TO_UNO
    #define EXTENDER_ADS7953_2_GPIO2_TO_UNO HardwareResourcePin::NONE
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_ADS7953_2_GPIO3_TO_UNO
    #define EXTENDER_ADS7953_2_GPIO3_TO_UNO HardwareResourcePin::NONE
  #endif
#endif


// ============================================================================
// EXTENDER: CD74HC4067
// ============================================================================
// Referentiespanning van de native ADC als CD74HC4067 direct op Native staat.
// 0 = onbekend: native metingen worden afgewezen in plaats van verkeerd omgerekend.
#ifndef EXTENDER_CD74HC4067_NATIVE_REFERENTIE_MILLIVOLT
  #define EXTENDER_CD74HC4067_NATIVE_REFERENTIE_MILLIVOLT 0
#endif
#if EXTENDER_CD74HC4067_NATIVE_REFERENTIE_MILLIVOLT > 5500
  #error EXTENDER_CD74HC4067_NATIVE_REFERENTIE_MILLIVOLT moet 0 of maximaal 5500 mV zijn.
#endif

#ifndef EXTENDER_CD74HC4067_AANTAL
  #define EXTENDER_CD74HC4067_AANTAL 0
#endif

#if EXTENDER_CD74HC4067_AANTAL > 2
  #error EXTENDER_CD74HC4067_AANTAL ondersteunt momenteel maximaal 2 instanties.
#endif

// ============================================================================
// DEFAULT: CD74HC4067 #1
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
// EXPERIMENTEEL: CD74HC4067 #2
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


// ============================================================================
// EXTENDER: DS2482-800
// ============================================================================
#ifndef EXTENDER_DS2482_800_AANTAL
  #define EXTENDER_DS2482_800_AANTAL 0
#endif

#if EXTENDER_DS2482_800_AANTAL > 2
  #error EXTENDER_DS2482_800_AANTAL ondersteunt momenteel maximaal 2 instanties.
#endif

// ============================================================================
// DEFAULT: DS2482-800 #1
// ============================================================================
#if EXTENDER_DS2482_800_AANTAL >= 1
  // EXCLUSIEF: I2C-adres van deze extender-instance op dezelfde I2C-bus.
  #ifndef I2C_ADDRESS_EXTENDER_DS2482_800_1
    #define I2C_ADDRESS_EXTENDER_DS2482_800_1 I2C_ADDRESS_NONE
  #endif
#endif

// ============================================================================
// EXPERIMENTEEL: DS2482-800 #2
// ============================================================================
#if EXTENDER_DS2482_800_AANTAL >= 2
  // EXCLUSIEF: I2C-adres van deze extender-instance op dezelfde I2C-bus.
  #ifndef I2C_ADDRESS_EXTENDER_DS2482_800_2
    #define I2C_ADDRESS_EXTENDER_DS2482_800_2 I2C_ADDRESS_NONE
  #endif
#endif


// ============================================================================
// MAX14830: werkelijke klok aan CLK/XIN, in Hz. 0 = onbekend: start van UART weigeren.
#ifndef EXTENDER_MAX14830_BRONKLOK_HZ
  #define EXTENDER_MAX14830_BRONKLOK_HZ 0
#endif
// MAX14830: 1 = kristal aan XIN/XOUT, 0 = externe klok aan XIN.
#ifndef EXTENDER_MAX14830_BRONKLOK_KRISTAL
  #define EXTENDER_MAX14830_BRONKLOK_KRISTAL 1
#endif
#if EXTENDER_MAX14830_BRONKLOK_KRISTAL != 0 && EXTENDER_MAX14830_BRONKLOK_KRISTAL != 1
  #error EXTENDER_MAX14830_BRONKLOK_KRISTAL moet 0 (externe klok) of 1 (kristal) zijn.
#endif
// MAX14830 PLL: beide 0 = PLL uit. Anders voordeler 1 t/m 63 en vermenigvuldiger 6, 48, 96 of 144.
#ifndef EXTENDER_MAX14830_PLL_VOORDELER
  #define EXTENDER_MAX14830_PLL_VOORDELER 0
#endif
#ifndef EXTENDER_MAX14830_PLL_VERMENIGVULDIGER
  #define EXTENDER_MAX14830_PLL_VERMENIGVULDIGER 0
#endif
#if (EXTENDER_MAX14830_PLL_VOORDELER == 0) != (EXTENDER_MAX14830_PLL_VERMENIGVULDIGER == 0)
  #error EXTENDER_MAX14830_PLL_VOORDELER en EXTENDER_MAX14830_PLL_VERMENIGVULDIGER zijn samen 0 (PLL uit) of samen ingesteld.
#endif
#if EXTENDER_MAX14830_PLL_VOORDELER < 0 || EXTENDER_MAX14830_PLL_VOORDELER > 63
  #error EXTENDER_MAX14830_PLL_VOORDELER moet tussen 0 en 63 liggen.
#endif
#if EXTENDER_MAX14830_PLL_VERMENIGVULDIGER != 0 && EXTENDER_MAX14830_PLL_VERMENIGVULDIGER != 6 && EXTENDER_MAX14830_PLL_VERMENIGVULDIGER != 48 && \
    EXTENDER_MAX14830_PLL_VERMENIGVULDIGER != 96 && EXTENDER_MAX14830_PLL_VERMENIGVULDIGER != 144
  #error EXTENDER_MAX14830_PLL_VERMENIGVULDIGER moet 0, 6, 48, 96 of 144 zijn.
#endif
#if EXTENDER_MAX14830_PLL_VOORDELER && EXTENDER_MAX14830_BRONKLOK_HZ && \
    (EXTENDER_MAX14830_BRONKLOK_HZ / EXTENDER_MAX14830_PLL_VOORDELER) * EXTENDER_MAX14830_PLL_VERMENIGVULDIGER > 96000000UL
  #error MAX14830: de klok na de PLL (EXTENDER_MAX14830_BRONKLOK_HZ / VOORDELER x VERMENIGVULDIGER) mag niet hoger zijn dan 96 MHz.
#endif
// PLL-ingang (EXTENDER_MAX14830_BRONKLOK_HZ / VOORDELER) per vermenigvuldiger; grenzen volgens de Linux-driver max310x.c
// (x6: 500-800 kHz, x48: 850-1200 kHz, x96: 425-1000 kHz, x144: 390-667 kHz).
#if EXTENDER_MAX14830_PLL_VOORDELER && EXTENDER_MAX14830_BRONKLOK_HZ && ( \
    (EXTENDER_MAX14830_PLL_VERMENIGVULDIGER == 6   && (EXTENDER_MAX14830_BRONKLOK_HZ / EXTENDER_MAX14830_PLL_VOORDELER < 500000UL || EXTENDER_MAX14830_BRONKLOK_HZ / EXTENDER_MAX14830_PLL_VOORDELER > 800000UL)) || \
    (EXTENDER_MAX14830_PLL_VERMENIGVULDIGER == 48  && (EXTENDER_MAX14830_BRONKLOK_HZ / EXTENDER_MAX14830_PLL_VOORDELER < 850000UL || EXTENDER_MAX14830_BRONKLOK_HZ / EXTENDER_MAX14830_PLL_VOORDELER > 1200000UL)) || \
    (EXTENDER_MAX14830_PLL_VERMENIGVULDIGER == 96  && (EXTENDER_MAX14830_BRONKLOK_HZ / EXTENDER_MAX14830_PLL_VOORDELER < 425000UL || EXTENDER_MAX14830_BRONKLOK_HZ / EXTENDER_MAX14830_PLL_VOORDELER > 1000000UL)) || \
    (EXTENDER_MAX14830_PLL_VERMENIGVULDIGER == 144 && (EXTENDER_MAX14830_BRONKLOK_HZ / EXTENDER_MAX14830_PLL_VOORDELER < 390000UL || EXTENDER_MAX14830_BRONKLOK_HZ / EXTENDER_MAX14830_PLL_VOORDELER > 667000UL)))
  #error MAX14830: de PLL-ingang (EXTENDER_MAX14830_BRONKLOK_HZ / EXTENDER_MAX14830_PLL_VOORDELER) ligt buiten het bereik van de gekozen vermenigvuldiger.
#endif

// EXTENDER: MAX14830 I2C
// ============================================================================
#ifndef EXTENDER_MAX14830_I2C_AANTAL
  #define EXTENDER_MAX14830_I2C_AANTAL 0
#endif

#if EXTENDER_MAX14830_I2C_AANTAL > 2
  #error EXTENDER_MAX14830_I2C_AANTAL ondersteunt momenteel maximaal 2 instanties.
#endif

// ============================================================================
// DEFAULT: MAX14830 I2C #1
// ============================================================================
#if EXTENDER_MAX14830_I2C_AANTAL >= 1
  // EXCLUSIEF: I2C-adres van deze extender-instance op dezelfde I2C-bus.
  #ifndef I2C_ADDRESS_EXTENDER_MAX14830_I2C_1
    #define I2C_ADDRESS_EXTENDER_MAX14830_I2C_1 I2C_ADDRESS_NONE
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_MAX14830_I2C_1_IRQ
    #define EXTENDER_MAX14830_I2C_1_IRQ HardwareResourcePin::NONE
  #endif
#endif

// ============================================================================
// EXPERIMENTEEL: MAX14830 I2C #2
// ============================================================================
#if EXTENDER_MAX14830_I2C_AANTAL >= 2
  // EXCLUSIEF: I2C-adres van deze extender-instance op dezelfde I2C-bus.
  #ifndef I2C_ADDRESS_EXTENDER_MAX14830_I2C_2
    #define I2C_ADDRESS_EXTENDER_MAX14830_I2C_2 I2C_ADDRESS_NONE
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_MAX14830_I2C_2_IRQ
    #define EXTENDER_MAX14830_I2C_2_IRQ HardwareResourcePin::NONE
  #endif
#endif


// ============================================================================
// EXTENDER: MAX14830 SPI
// ============================================================================
#ifndef EXTENDER_MAX14830_SPI_AANTAL
  #define EXTENDER_MAX14830_SPI_AANTAL 0
#endif

#if EXTENDER_MAX14830_SPI_AANTAL > 2
  #error EXTENDER_MAX14830_SPI_AANTAL ondersteunt momenteel maximaal 2 instanties.
#endif

// ============================================================================
// DEFAULT: MAX14830 SPI #1
// ============================================================================
#if EXTENDER_MAX14830_SPI_AANTAL >= 1
  // EXCLUSIEF: CS van deze extender-instance; kies een vrije HardwareResourcePin.
  #ifndef EXTENDER_MAX14830_SPI_1_CS_PIN
    #define EXTENDER_MAX14830_SPI_1_CS_PIN HardwareResourcePin::NONE
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_MAX14830_SPI_1_IRQ
    #define EXTENDER_MAX14830_SPI_1_IRQ HardwareResourcePin::NONE
  #endif
#endif

// ============================================================================
// EXPERIMENTEEL: MAX14830 SPI #2
// ============================================================================
#if EXTENDER_MAX14830_SPI_AANTAL >= 2
  // EXCLUSIEF: CS van deze extender-instance; kies een vrije HardwareResourcePin.
  #ifndef EXTENDER_MAX14830_SPI_2_CS_PIN
    #define EXTENDER_MAX14830_SPI_2_CS_PIN HardwareResourcePin::NONE
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_MAX14830_SPI_2_IRQ
    #define EXTENDER_MAX14830_SPI_2_IRQ HardwareResourcePin::NONE
  #endif
#endif


// ============================================================================
// EXTENDER: MCP23017
// ============================================================================
#ifndef EXTENDER_MCP23017_AANTAL
  #define EXTENDER_MCP23017_AANTAL 0
#endif

#if EXTENDER_MCP23017_AANTAL > 2
  #error EXTENDER_MCP23017_AANTAL ondersteunt momenteel maximaal 2 instanties.
#endif

// ============================================================================
// DEFAULT: MCP23017 #1
// ============================================================================
#if EXTENDER_MCP23017_AANTAL >= 1
  // EXCLUSIEF: I2C-adres van deze extender-instance op dezelfde I2C-bus.
  #ifndef I2C_ADDRESS_EXTENDER_MCP23017_1
    #define I2C_ADDRESS_EXTENDER_MCP23017_1 I2C_ADDRESS_NONE
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
// EXPERIMENTEEL: MCP23017 #2
// ============================================================================
#if EXTENDER_MCP23017_AANTAL >= 2
  // EXCLUSIEF: I2C-adres van deze extender-instance op dezelfde I2C-bus.
  #ifndef I2C_ADDRESS_EXTENDER_MCP23017_2
    #define I2C_ADDRESS_EXTENDER_MCP23017_2 I2C_ADDRESS_NONE
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


// ============================================================================
// EXTENDER: MCP23S17
// ============================================================================
#ifndef EXTENDER_MCP23S17_AANTAL
  #define EXTENDER_MCP23S17_AANTAL 0
#endif

#if EXTENDER_MCP23S17_AANTAL > 2
  #error EXTENDER_MCP23S17_AANTAL ondersteunt momenteel maximaal 2 instanties.
#endif

// ============================================================================
// DEFAULT: MCP23S17 #1
// ============================================================================
#if EXTENDER_MCP23S17_AANTAL >= 1
  #ifndef EXTENDER_MCP23S17_1_CS_PIN
    #define EXTENDER_MCP23S17_1_CS_PIN HardwareResourcePin::NONE
  #endif
#endif

// ============================================================================
// EXPERIMENTEEL: MCP23S17 #2
// ============================================================================
#if EXTENDER_MCP23S17_AANTAL >= 2
  #ifndef EXTENDER_MCP23S17_2_CS_PIN
    #define EXTENDER_MCP23S17_2_CS_PIN HardwareResourcePin::NONE
  #endif
#endif


// ============================================================================
// EXTENDER: PCA9685
// ============================================================================
#ifndef EXTENDER_PCA9685_AANTAL
  #define EXTENDER_PCA9685_AANTAL 0
#endif

#if EXTENDER_PCA9685_AANTAL > 2
  #error EXTENDER_PCA9685_AANTAL ondersteunt momenteel maximaal 2 instanties.
#endif
// EXTENDER_PCA9685_1_PWM_FREQUENTIE en EXTENDER_PCA9685_2_PWM_FREQUENTIE (Hz): optioneel, één frequentie per chip voor alle 16 kanalen.
// Niet ingesteld: de frequentie van de chip blijft. Servo's en LED's daarom op aparte modules.
// EXTENDER_PCA9685_1_OE_PIN en EXTENDER_PCA9685_2_OE_PIN: optioneel, een pin van het board naar de OE-pin van de chip.
// Bij uitval gaan via die pin alle 16 uitgangen in één keer uit, ook als de I2C-bus hangt. Niet ingesteld: er verandert niets.
// De OE-pin wordt niet gereserveerd in de GedeeldeBus: kies een pin die geen ander component gebruikt.

// ============================================================================
// DEFAULT: PCA9685 #1
// ============================================================================
#if EXTENDER_PCA9685_AANTAL >= 1
  #ifndef I2C_ADDRESS_EXTENDER_PCA9685_1
    #define I2C_ADDRESS_EXTENDER_PCA9685_1 I2C_ADDRESS_NONE
  #endif
#endif

// ============================================================================
// EXPERIMENTEEL: PCA9685 #2
// ============================================================================
#if EXTENDER_PCA9685_AANTAL >= 2
  #ifndef I2C_ADDRESS_EXTENDER_PCA9685_2
    #define I2C_ADDRESS_EXTENDER_PCA9685_2 I2C_ADDRESS_NONE
  #endif
#endif


// ============================================================================
// EXTENDER: PCF8574
// ============================================================================
#ifndef EXTENDER_PCF8574_AANTAL
  #define EXTENDER_PCF8574_AANTAL 0
#endif

#if EXTENDER_PCF8574_AANTAL > 2
  #error EXTENDER_PCF8574_AANTAL ondersteunt momenteel maximaal 2 instanties.
#endif

// ============================================================================
// DEFAULT: PCF8574 #1
// ============================================================================
#if EXTENDER_PCF8574_AANTAL >= 1
  // EXCLUSIEF: I2C-adres van deze extender-instance op dezelfde I2C-bus.
  #ifndef I2C_ADDRESS_EXTENDER_PCF8574_1
    #define I2C_ADDRESS_EXTENDER_PCF8574_1 I2C_ADDRESS_NONE
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_PCF8574_1_INT
    #define EXTENDER_PCF8574_1_INT HardwareResourcePin::NONE
  #endif
#endif

// ============================================================================
// EXPERIMENTEEL: PCF8574 #2
// ============================================================================
#if EXTENDER_PCF8574_AANTAL >= 2
  // EXCLUSIEF: I2C-adres van deze extender-instance op dezelfde I2C-bus.
  #ifndef I2C_ADDRESS_EXTENDER_PCF8574_2
    #define I2C_ADDRESS_EXTENDER_PCF8574_2 I2C_ADDRESS_NONE
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_PCF8574_2_INT
    #define EXTENDER_PCF8574_2_INT HardwareResourcePin::NONE
  #endif
#endif


// ============================================================================
// EXTENDER: PCF8575
// ============================================================================
#ifndef EXTENDER_PCF8575_AANTAL
  #define EXTENDER_PCF8575_AANTAL 0
#endif

#if EXTENDER_PCF8575_AANTAL > 2
  #error EXTENDER_PCF8575_AANTAL ondersteunt momenteel maximaal 2 instanties.
#endif

// ============================================================================
// DEFAULT: PCF8575 #1
// ============================================================================
#if EXTENDER_PCF8575_AANTAL >= 1
  // EXCLUSIEF: I2C-adres van deze extender-instance op dezelfde I2C-bus.
  #ifndef I2C_ADDRESS_EXTENDER_PCF8575_1
    #define I2C_ADDRESS_EXTENDER_PCF8575_1 I2C_ADDRESS_NONE
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_PCF8575_1_INT
    #define EXTENDER_PCF8575_1_INT HardwareResourcePin::NONE
  #endif
#endif

// ============================================================================
// EXPERIMENTEEL: PCF8575 #2
// ============================================================================
#if EXTENDER_PCF8575_AANTAL >= 2
  // EXCLUSIEF: I2C-adres van deze extender-instance op dezelfde I2C-bus.
  #ifndef I2C_ADDRESS_EXTENDER_PCF8575_2
    #define I2C_ADDRESS_EXTENDER_PCF8575_2 I2C_ADDRESS_NONE
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_PCF8575_2_INT
    #define EXTENDER_PCF8575_2_INT HardwareResourcePin::NONE
  #endif
#endif


// ============================================================================
// AFGELEID: GPIO via een Extender (niet zelf instellen)
// ============================================================================
// 1 = SetExtenderPinMode(), WriteExtenderPin() en ReadExtenderPin() worden meegecompileerd:
// bij gemengde pinnen, of wanneer HCSR501, HW201, MAX7219, Relais of ULN2003 samen met een GPIO-Extender gebruikt wordt.
#if (GEDEELDE_BUS_MIXED_PINS || ((SENSOR_HCSR501_AANTAL > 0 || SENSOR_HW201_AANTAL > 0 || STURING_MAX7219_AANTAL > 0 || STURING_RELAIS_AANTAL > 0 || STURING_ULN2003_AANTAL > 0) && (EXTENDER_PCF8574_AANTAL > 0 || EXTENDER_PCF8575_AANTAL > 0 || EXTENDER_MCP23017_AANTAL > 0 || EXTENDER_MCP23S17_AANTAL > 0)))
  #define GEDEELDE_BUS_EXTENDER_GPIO_ACTIEF 1
#else
  #define GEDEELDE_BUS_EXTENDER_GPIO_ACTIEF 0
#endif

// ============================================================================
// EXTENDER: SC16IS752 I2C
// ============================================================================
#ifndef EXTENDER_SC16IS752_I2C_AANTAL
  #define EXTENDER_SC16IS752_I2C_AANTAL 0
#endif

#if EXTENDER_SC16IS752_I2C_AANTAL > 2
  #error EXTENDER_SC16IS752_I2C_AANTAL ondersteunt momenteel maximaal 2 instanties.
#endif

// ============================================================================
// DEFAULT: SC16IS752 I2C #1
// ============================================================================
#if EXTENDER_SC16IS752_I2C_AANTAL >= 1
  // EXCLUSIEF: I2C-adres van deze extender-instance op dezelfde I2C-bus.
  #ifndef I2C_ADDRESS_EXTENDER_SC16IS752_I2C_1
    #define I2C_ADDRESS_EXTENDER_SC16IS752_I2C_1 I2C_ADDRESS_NONE
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
// EXPERIMENTEEL: SC16IS752 I2C #2
// ============================================================================
#if EXTENDER_SC16IS752_I2C_AANTAL >= 2
  // EXCLUSIEF: I2C-adres van deze extender-instance op dezelfde I2C-bus.
  #ifndef I2C_ADDRESS_EXTENDER_SC16IS752_I2C_2
    #define I2C_ADDRESS_EXTENDER_SC16IS752_I2C_2 I2C_ADDRESS_NONE
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


// ============================================================================
// EXTENDER: SC16IS752 SPI
// ============================================================================
#ifndef EXTENDER_SC16IS752_SPI_AANTAL
  #define EXTENDER_SC16IS752_SPI_AANTAL 0
#endif

#if EXTENDER_SC16IS752_SPI_AANTAL > 2
  #error EXTENDER_SC16IS752_SPI_AANTAL ondersteunt momenteel maximaal 2 instanties.
#endif

// ============================================================================
// DEFAULT: SC16IS752 SPI #1
// ============================================================================
#if EXTENDER_SC16IS752_SPI_AANTAL >= 1
  // EXCLUSIEF: CS van deze extender-instance; kies een vrije HardwareResourcePin.
  #ifndef EXTENDER_SC16IS752_SPI_1_CS_PIN
    #define EXTENDER_SC16IS752_SPI_1_CS_PIN HardwareResourcePin::NONE
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
// EXPERIMENTEEL: SC16IS752 SPI #2
// ============================================================================
#if EXTENDER_SC16IS752_SPI_AANTAL >= 2
  // EXCLUSIEF: CS van deze extender-instance; kies een vrije HardwareResourcePin.
  #ifndef EXTENDER_SC16IS752_SPI_2_CS_PIN
    #define EXTENDER_SC16IS752_SPI_2_CS_PIN HardwareResourcePin::NONE
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
// EXTENDER: TCA9548A
// ============================================================================
#ifndef EXTENDER_TCA9548A_AANTAL
  #define EXTENDER_TCA9548A_AANTAL 0
#endif

#if EXTENDER_TCA9548A_AANTAL > 2
  #error EXTENDER_TCA9548A_AANTAL ondersteunt momenteel maximaal 2 instanties.
#endif

// ============================================================================
// DEFAULT: TCA9548A #1
// ============================================================================
#if EXTENDER_TCA9548A_AANTAL >= 1
  // EXCLUSIEF: I2C-adres van deze extender-instance op dezelfde I2C-bus.
  #ifndef I2C_ADDRESS_EXTENDER_TCA9548A_1
    #define I2C_ADDRESS_EXTENDER_TCA9548A_1 I2C_ADDRESS_NONE
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_TCA9548A_1_RESET
    #define EXTENDER_TCA9548A_1_RESET HardwareResourcePin::NONE
  #endif
#endif

// ============================================================================
// EXPERIMENTEEL: TCA9548A #2
// ============================================================================
#if EXTENDER_TCA9548A_AANTAL >= 2
  // EXCLUSIEF: I2C-adres van deze extender-instance op dezelfde I2C-bus.
  #ifndef I2C_ADDRESS_EXTENDER_TCA9548A_2
    #define I2C_ADDRESS_EXTENDER_TCA9548A_2 I2C_ADDRESS_NONE
  #endif
  // EXCLUSIEF: native aansluiting van deze extender-instance; NONE = niet gebruikt/aangesloten.
  #ifndef EXTENDER_TCA9548A_2_RESET
    #define EXTENDER_TCA9548A_2_RESET HardwareResourcePin::NONE
  #endif
#endif


// ============================================================================
// FsrRfp: standaard ADC-kanalen per sensorset ================================
// ============================================================================

// Een .ino kan per FsrRfp-instantie een eigen ADC-Extender (parent) en vier eigen kanalen opgeven.
// Zonder opgave gelden deze standaardkanalen. ADS1115 is momenteel de enige ondersteunde ADC-Extender voor FsrRfp.
// G2: elke set heeft een eigen ADS1115. Set 2 vraagt dus minstens 2 ADS1115's.
#if defined(FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_2) && EXTENDER_ADS1115_AANTAL < 2
  #error FsrRfp set 2 vraagt een eigen ADS1115: zet EXTENDER_ADS1115_AANTAL op 2.
#endif
#if EXTENDER_ADS1115_AANTAL > 0
  #ifndef FSR_RFP_ADC_PIN_SENSOR_1_SET_1
    #define FSR_RFP_ADC_PIN_SENSOR_1_SET_1 static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN0)
  #endif
  #ifndef FSR_RFP_ADC_PIN_SENSOR_2_SET_1
    #define FSR_RFP_ADC_PIN_SENSOR_2_SET_1 static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN1)
  #endif
  #ifndef FSR_RFP_ADC_PIN_SENSOR_3_SET_1
    #define FSR_RFP_ADC_PIN_SENSOR_3_SET_1 static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN2)
  #endif
  #ifndef FSR_RFP_ADC_PIN_SENSOR_4_SET_1
    #define FSR_RFP_ADC_PIN_SENSOR_4_SET_1 static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN3)
  #endif
  // Set 2 (1b): standaard dezelfde ingangen, op de ADS1115 die de .ino als parent van set 2 opgeeft.
  #if defined(FSR_RFP_AANTAL_SENSOREN_BESCHIKBAAR_SET_2)
    #ifndef FSR_RFP_ADC_PIN_SENSOR_1_SET_2
      #define FSR_RFP_ADC_PIN_SENSOR_1_SET_2 static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN0)
    #endif
    #ifndef FSR_RFP_ADC_PIN_SENSOR_2_SET_2
      #define FSR_RFP_ADC_PIN_SENSOR_2_SET_2 static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN1)
    #endif
    #ifndef FSR_RFP_ADC_PIN_SENSOR_3_SET_2
      #define FSR_RFP_ADC_PIN_SENSOR_3_SET_2 static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN2)
    #endif
    #ifndef FSR_RFP_ADC_PIN_SENSOR_4_SET_2
      #define FSR_RFP_ADC_PIN_SENSOR_4_SET_2 static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN3)
    #endif
  #endif
#endif

#ifndef ADC_BITS
  #define ADC_BITS 15      // ADS1115 single-ended: 0-32767 bruikbaar bereik
#endif

#ifndef DELAY_US
  #define DELAY_US 0       // niet van toepassing; de ADS1115 wacht zelf op de conversie
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
// INPUT ======================================================================
// ============================================================================

// INPUT_TYPE_PCF8574 ---------------------------------------------------------
#if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_PCF8574)
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

// HX1838_EEPROM_NATIVE_SIZE: het blok vooraan in de native EEPROM dat HX1838 gebruikt voor zijn IR-codes.
// Vaste maximale grootte, zodat Data daarna altijd op hetzelfde adres begint, welke toetsenindeling ook gekozen is:
// 3 bytes (magic, versie, toetsenindeling) + 21 codes (de grootste indeling die met EEPROM mag: 21 toetsen).
// 0 wanneer HX1838 de native EEPROM niet gebruikt.
#if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_HX1838) && (HX1838_BRON_CODES == HX1838_BRON_CODES_EEPROM_ALTIJD)
  #define HX1838_EEPROM_NATIVE_SIZE (3 + 21)
#else
  #define HX1838_EEPROM_NATIVE_SIZE 0
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
// DEBUG INSTELLINGEN =========================================================
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

// GEDEELDE_BUS_FOUTINJECTIE: 1 = testingang voor de foutinjectietests (examples/Systeem/GedeeldeBus/Foutinjecties).
// Een sketch kiest via foutinjectieComponent en foutinjectieStap welk kind in welke stap faalt. 0 = niets meegecompileerd.
// Standaard aan bij DEBUG (TRACE zet hierboven al DEBUG aan).
#ifndef GEDEELDE_BUS_FOUTINJECTIE
  #if defined(DEBUG)
    #define GEDEELDE_BUS_FOUTINJECTIE 1
  #else
    #define GEDEELDE_BUS_FOUTINJECTIE 0
  #endif
#endif

#endif
