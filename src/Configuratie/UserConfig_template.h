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
// Screen.cpp en Stimulus.cpp worden door de Arduino-toolchain als losse,
// onafhankelijke bestanden gecompileerd, niet samen met je .ino. 
// Instellingen die bepalen wat in deze librarybestanden wordt
// gecompileerd horen daarom in UserConfig.h of, als officiële fallback, in SystemConfig.h.
//
// Gewone examples stellen deze libraryconfiguratie niet rechtstreeks in de .ino in.
// Instellingen die uitsluitend het voorbeeldprogramma zelf gebruikt, 
// horen in ExamplesConfig.h of lokaal in dat voorbeeld.
// ============================================================================

// #define LANGUAGE_NL
// #define LANGUAGE_DE
// #define LANGUAGE_EN
// #define LANGUAGE_FR

// ============================================================================
// MAG JE AANPASSEN NAAR EIGEN SETUP
// ============================================================================
// #define DEBUG // ENKEL wanneer DEBUG nodig, staan hier geen commentaar '//' tekens voor :)

// #define TRACE // ENKEL wanneer TRACE nodig, staan hier geen commentaar '//' tekens voor :)

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
// #define SCREEN_OUTPUT_CONFIG SCREEN_TYPE_NONE
#endif

// ============================================================================
// INPUT
// ============================================================================
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
// #define INPUT_KANAAL_CONFIG INPUT_TYPE_DIGITAL
#endif

#ifndef KEYPAD_TYPE
// #define KEYPAD_TYPE KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4
#endif

// PCF8574 (OT8980): 0x20 t.e.m. 0x27 via A0/A1/A2; elk apparaat op dezelfde I2C-bus moet een uniek adres hebben.
#ifndef I2C_ADDRESS_PCF8574
// #define I2C_ADDRESS_PCF8574 0x20
#endif

// PCF8574-logische aansluitingen. Pas alleen het blok aan dat bij het gekozen KEYPAD_TYPE hoort.
// PCF8574_PIN_P0 t.e.m. PCF8574_PIN_P7 zijn vaste hardware-identiteiten en worden niet aangepast.

#if defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_DIRECT_1x4
// #define KEYPAD_PIN_K1 PCF8574_PIN_P1
// #define KEYPAD_PIN_K2 PCF8574_PIN_P0
// #define KEYPAD_PIN_K3 PCF8574_PIN_P3
// #define KEYPAD_PIN_K4 PCF8574_PIN_P2
#elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_DIRECT_2x4
// #define KEYPAD_PIN_K1 PCF8574_PIN_P0
// #define KEYPAD_PIN_K2 PCF8574_PIN_P1
// #define KEYPAD_PIN_K3 PCF8574_PIN_P2
// #define KEYPAD_PIN_K4 PCF8574_PIN_P3
// #define KEYPAD_PIN_K5 PCF8574_PIN_P4
// #define KEYPAD_PIN_K6 PCF8574_PIN_P5
// #define KEYPAD_PIN_K7 PCF8574_PIN_P6
// #define KEYPAD_PIN_K8 PCF8574_PIN_P7
#elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_MATRIX_2x2
// #define KEYPAD_PIN_L1 PCF8574_PIN_P0
// #define KEYPAD_PIN_L2 PCF8574_PIN_P1
// #define KEYPAD_PIN_R1 PCF8574_PIN_P2
// #define KEYPAD_PIN_R2 PCF8574_PIN_P3
#elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_DRUKKNOP_MATRIX_4x4
// #define KEYPAD_PIN_C4 PCF8574_PIN_P0
// #define KEYPAD_PIN_C3 PCF8574_PIN_P1
// #define KEYPAD_PIN_C2 PCF8574_PIN_P2
// #define KEYPAD_PIN_C1 PCF8574_PIN_P3
// #define KEYPAD_PIN_R1 PCF8574_PIN_P4
// #define KEYPAD_PIN_R2 PCF8574_PIN_P5
// #define KEYPAD_PIN_R3 PCF8574_PIN_P6
// #define KEYPAD_PIN_R4 PCF8574_PIN_P7
#elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_DIRECT_1x4
// #define KEYPAD_PIN_K1 PCF8574_PIN_P0
// #define KEYPAD_PIN_K2 PCF8574_PIN_P1
// #define KEYPAD_PIN_K3 PCF8574_PIN_P2
// #define KEYPAD_PIN_K4 PCF8574_PIN_P3
#elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_DIRECT_4x1
// #define KEYPAD_PIN_K1 PCF8574_PIN_P0
// #define KEYPAD_PIN_K2 PCF8574_PIN_P1
// #define KEYPAD_PIN_K3 PCF8574_PIN_P2
// #define KEYPAD_PIN_K4 PCF8574_PIN_P3
#elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_MATRIX_1x4
// #define KEYPAD_PIN_R1 PCF8574_PIN_P0
// #define KEYPAD_PIN_C1 PCF8574_PIN_P1
// #define KEYPAD_PIN_C2 PCF8574_PIN_P2
// #define KEYPAD_PIN_C3 PCF8574_PIN_P3
// #define KEYPAD_PIN_C4 PCF8574_PIN_P4
#elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_MATRIX_2x4
// #define KEYPAD_PIN_R1 PCF8574_PIN_P0
// #define KEYPAD_PIN_R2 PCF8574_PIN_P1
// #define KEYPAD_PIN_C1 PCF8574_PIN_P2
// #define KEYPAD_PIN_C2 PCF8574_PIN_P3
// #define KEYPAD_PIN_C3 PCF8574_PIN_P4
// #define KEYPAD_PIN_C4 PCF8574_PIN_P5
#elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_MATRIX_4x3
// #define KEYPAD_PIN_R1 PCF8574_PIN_P0
// #define KEYPAD_PIN_R2 PCF8574_PIN_P1
// #define KEYPAD_PIN_R3 PCF8574_PIN_P2
// #define KEYPAD_PIN_R4 PCF8574_PIN_P3
// #define KEYPAD_PIN_C1 PCF8574_PIN_P4
// #define KEYPAD_PIN_C2 PCF8574_PIN_P5
// #define KEYPAD_PIN_C3 PCF8574_PIN_P6
#elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_MEMBRAAN_MATRIX_4x4
// #define KEYPAD_PIN_R1 PCF8574_PIN_P0
// #define KEYPAD_PIN_R2 PCF8574_PIN_P1
// #define KEYPAD_PIN_R3 PCF8574_PIN_P2
// #define KEYPAD_PIN_R4 PCF8574_PIN_P3
// #define KEYPAD_PIN_C1 PCF8574_PIN_P4
// #define KEYPAD_PIN_C2 PCF8574_PIN_P5
// #define KEYPAD_PIN_C3 PCF8574_PIN_P6
// #define KEYPAD_PIN_C4 PCF8574_PIN_P7
#elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_TOUCH_TTP224_DIRECT_1x4
// #define KEYPAD_PIN_K1 PCF8574_PIN_P0
// #define KEYPAD_PIN_K2 PCF8574_PIN_P1
// #define KEYPAD_PIN_K3 PCF8574_PIN_P2
// #define KEYPAD_PIN_K4 PCF8574_PIN_P3
#elif defined(KEYPAD_TYPE) && KEYPAD_TYPE == KEYPAD_TYPE_TOUCH_TTP229_MATRIX_4x4
// Via de 8 parallelle uitgangen op PCF8574 P0-P7 kunnen enkel de eerste 8 toetsen uitgelezen worden.
// #define KEYPAD_PIN_K1 PCF8574_PIN_P0
// #define KEYPAD_PIN_K2 PCF8574_PIN_P1
// #define KEYPAD_PIN_K3 PCF8574_PIN_P2
// #define KEYPAD_PIN_K4 PCF8574_PIN_P3
// #define KEYPAD_PIN_K5 PCF8574_PIN_P4
// #define KEYPAD_PIN_K6 PCF8574_PIN_P5
// #define KEYPAD_PIN_K7 PCF8574_PIN_P6
// #define KEYPAD_PIN_K8 PCF8574_PIN_P7
#endif

#ifndef TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED
// #define TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED_HIGH // ofwel TTP229_OUTPUT_LEVEL_WHEN_KEY_PRESSED_LOW
#endif

#ifndef HX1838_ONTVANGER_PIN
// #define HX1838_ONTVANGER_PIN ARDUINO_UNO_SHIELD_PIN_D2
#endif

#ifndef HX1838_TOETSENINDELING
// #define HX1838_TOETSENINDELING HX1838_TOETSENINDELING_3x4
// #define HX1838_TOETSENINDELING HX1838_TOETSENINDELING_REMOTE_17_TOETSEN
// #define HX1838_TOETSENINDELING HX1838_TOETSENINDELING_REMOTE_21_TOETSEN_MP3
#endif

#ifndef HX1838_BRON_CODES
// #define HX1838_BRON_CODES HX1838_BRON_CODES_DEFINE
// #define HX1838_BRON_CODES HX1838_BRON_CODES_EEPROM_ALTIJD
// #define HX1838_BRON_CODES HX1838_BRON_CODES_EEPROM_WANNEER_GEEN_DEFINE
#endif

// Optionele vaste HX1838-mapping. Laat een code op 0 staan wanneer geen vaste code gekend is; dan gebruikt Input EEPROM of de kalibratieprocedure.
// Wanneer alle HX1838_CODE_x waarden voor het gekozen remote-layout ingevuld zijn, heeft deze mapping in UserConfig.h voorrang op een eerder opgeslagen EEPROM-kalibratie.
// Laat de waarden op 0 wanneer je geen vaste mapping wilt gebruiken; dan probeert Input eerst EEPROM en start daarna indien nodig de kalibratieprocedure.
// #define HX1838_CODE_1  0x00000000UL
// #define HX1838_CODE_2  0x00000000UL
// #define HX1838_CODE_3  0x00000000UL
// #define HX1838_CODE_4  0x00000000UL
// #define HX1838_CODE_5  0x00000000UL
// #define HX1838_CODE_6  0x00000000UL
// #define HX1838_CODE_7  0x00000000UL
// #define HX1838_CODE_8  0x00000000UL
// #define HX1838_CODE_9  0x00000000UL
// #define HX1838_CODE_10 0x00000000UL
// #define HX1838_CODE_11 0x00000000UL
// #define HX1838_CODE_12 0x00000000UL
// #define HX1838_CODE_13 0x00000000UL
// #define HX1838_CODE_14 0x00000000UL
// #define HX1838_CODE_15 0x00000000UL
// #define HX1838_CODE_16 0x00000000UL
// #define HX1838_CODE_17 0x00000000UL
// #define HX1838_CODE_18 0x00000000UL
// #define HX1838_CODE_19 0x00000000UL
// #define HX1838_CODE_20 0x00000000UL
// #define HX1838_CODE_21 0x00000000UL

// Bekende NEC-commandmapping voor de Remote 17-toets; voor de 3x4-layout worden de eerste 12 waarden gebruikt.
// HX1838_CODE_1=0x16, 2=0x19, 3=0x0D, 4=0x0C, 5=0x18, 6=0x5E, 7=0x08, 8=0x1C, 9=0x5A, *=0x42, 0=0x52, #=0x4A, UP=0x46, DOWN=0x15, LEFT=0x44, RIGHT=0x43, OK=0x40.

// Bekende NEC-commandmapping voor de Remote 21-toets MP3, in de volgorde van de bijbehorende HX1838-toetsenindeling:
// CH-=0x45, CH=0x46, CH+=0x47, PREV=0x44, NEXT=0x40, PLAY=0x43, -=0x07, +=0x15, EQ=0x09, 0=0x16, 100+=0x19, 200+=0x0D, 1=0x0C, 2=0x18, 3=0x5E, 4=0x08, 5=0x1C, 6=0x5A, 7=0x42, 8=0x52, 9=0x4A.

// Arduino Uno-shieldheaderpinnen. Alleen activeren wanneer de geselecteerde boardcore een afwijkende mapping nodig heeft.
// #define ARDUINO_UNO_SHIELD_PIN_D0  0
// #define ARDUINO_UNO_SHIELD_PIN_D1  1
// #define ARDUINO_UNO_SHIELD_PIN_D2  2
// #define ARDUINO_UNO_SHIELD_PIN_D3  3
// #define ARDUINO_UNO_SHIELD_PIN_D4  4
// #define ARDUINO_UNO_SHIELD_PIN_D5  5
// #define ARDUINO_UNO_SHIELD_PIN_D6  6
// #define ARDUINO_UNO_SHIELD_PIN_D7  7
// #define ARDUINO_UNO_SHIELD_PIN_D8  8
// #define ARDUINO_UNO_SHIELD_PIN_D9  9
// #define ARDUINO_UNO_SHIELD_PIN_D10 10
// #define ARDUINO_UNO_SHIELD_PIN_D11 11
// #define ARDUINO_UNO_SHIELD_PIN_D12 12
// #define ARDUINO_UNO_SHIELD_PIN_D13 13
// #define ARDUINO_UNO_SHIELD_PIN_A0  A0
// #define ARDUINO_UNO_SHIELD_PIN_A1  A1
// #define ARDUINO_UNO_SHIELD_PIN_A2  A2
// #define ARDUINO_UNO_SHIELD_PIN_A3  A3
// #define ARDUINO_UNO_SHIELD_PIN_A4  A4
// #define ARDUINO_UNO_SHIELD_PIN_A5  A5
// #define ARDUINO_UNO_SHIELD_PIN_SDA SDA
// #define ARDUINO_UNO_SHIELD_PIN_SCL SCL
// #define ARDUINO_UNO_SHIELD_PIN_SS   SS
// #define ARDUINO_UNO_SHIELD_PIN_MOSI MOSI
// #define ARDUINO_UNO_SHIELD_PIN_MISO MISO
// #define ARDUINO_UNO_SHIELD_PIN_SCK  SCK

// Definieer de pinnen voor het 1x4 keypad
// #define PIN_TOETS_1 ARDUINO_UNO_SHIELD_PIN_D3
// #define PIN_TOETS_2 ARDUINO_UNO_SHIELD_PIN_D2
// #define PIN_TOETS_3 ARDUINO_UNO_SHIELD_PIN_D5
// #define PIN_TOETS_4 ARDUINO_UNO_SHIELD_PIN_D4

// Uitgebreide gebeurtenissen (loslaten, lang indrukken, timeout bij geen invoer).
// Standaard UIT, om geheugen te sparen op geheugenarme boards (bv. AVR Uno R3).
// Enkel van toepassing bij OpvragenHuidigeToetsAanslag(false), niet bij wachten=true.
// #define INPUT_KANAAL_OPVRAGEN_HUIDIGE_TOETSAANSLAG_UITGEBREID

// Enkel relevant wanneer INPUT_KANAAL_OPVRAGEN_HUIDIGE_TOETSAANSLAG_UITGEBREID aan staat.
// 0 = geen timeout, blijven wachten op een toetsaanslag (huidig gedrag).
#ifndef INPUT_KANAAL_TIMEOUT_BIJ_GEEN_TOETSAANSLAG_BINNEN_MS
// #define INPUT_KANAAL_TIMEOUT_BIJ_GEEN_TOETSAANSLAG_BINNEN_MS 5000UL
#endif

#ifndef MAX_AANTAL_SIMULTANE_TOETSAANSLAGEN
// #define MAX_AANTAL_SIMULTANE_TOETSAANSLAGEN 1
#endif

#ifndef INPUT_DEBOUNCE_MS
// #define INPUT_DEBOUNCE_MS 25UL
#endif

#ifndef HX1838_KALIBRATIE_TOETS_PAUZE_MS
// #define HX1838_KALIBRATIE_TOETS_PAUZE_MS 300UL
#endif

// ============================================================================
// SCHERMEN
// ============================================================================
// Characterscherm
#ifndef I2C_ADDRESS_CHARACTER_SCREEN
// #define I2C_ADDRESS_CHARACTER_SCREEN 0x27
#endif

#ifndef I2C_ADRES
// #define I2C_ADRES                     0x27
#endif

// CHARACTERSCREEN_I2C_ADRES_MODUS — impact per waarde (zie ook docs/DECISION_LOG.md, D023):
//   0 = geen scan, enkel de handdruk-check op I2C_ADRES. Kleinste footprint.
//       Gebruik dit wanneer het board tegen zijn geheugengrens zit (bv. UNO R3-grensgevallen).
//   1 = scan (I2C_ADRES, daarna 0x27, 0x3F) + rapporteren via de foutmelding, 
//       geen zelfherstel. Bij een ander gevonden adres: melding tonen, 
//       één keer hercompileren met het juiste I2C_ADRES. 
//       STANDAARD: gebruik dit in elke opstelling waar reproduceerbare, 
//       voorspelbare resultaten tellen (onderzoek, metingen, meerdere I2C-apparaten op dezelfde bus).
//   2 = scan + automatisch herbouwen (placement-new) op het gevonden adres. 
//       Nooit meer hercompileren bij een ander scherm/adres. Gebruik dit enkel tijdens actief
//       ontwikkelen met wisselende schermen, wanneer er geen ander I2C-apparaat op de bus
//       hangt en er geen metingen worden vastgelegd, de scan controleert enkel of een
//       apparaat op dat adres reageert (ACK), niet of het effectief een LCD-scherm is.
#ifndef CHARACTERSCREEN_I2C_ADRES_MODUS
// #define CHARACTERSCREEN_I2C_ADRES_MODUS 1
#endif

#ifndef ACTIEF_CHARACTER_SCREEN
// #define ACTIEF_CHARACTER_SCREEN       SCREEN_LCD1602
#endif

#ifndef FATAL_LEESTIJD_MS
// #define FATAL_LEESTIJD_MS   30000UL
#endif

#ifndef DEFAULT_TOEGESTANE_MARGE_SIMULTANE_STARTTIJD_MS
// #define DEFAULT_TOEGESTANE_MARGE_SIMULTANE_STARTTIJD_MS 100UL
#endif

// Pixelscherm
#ifndef ACTIEF_PIXEL_SCREEN
// #define ACTIEF_PIXEL_SCREEN           SCREEN_240X320
#endif

#ifndef PIXEL_SCREEN_CS
// #define PIXEL_SCREEN_CS               SS
#endif
#ifndef PIXEL_SCREEN_DC
// #define PIXEL_SCREEN_DC               ARDUINO_UNO_SHIELD_PIN_D9
#endif
#ifndef PIXEL_SCREEN_RST
// #define PIXEL_SCREEN_RST              ARDUINO_UNO_SHIELD_PIN_D8
#endif

#ifndef PIXEL_SCREEN_ROTATION
// #define PIXEL_SCREEN_ROTATION         0
#endif
#ifndef PIXEL_SCREEN_TEXT_SIZE
// #define PIXEL_SCREEN_TEXT_SIZE        2
#endif
#ifndef PIXEL_SCREEN_MARGIN
// #define PIXEL_SCREEN_MARGIN           5
#endif
#ifndef PIXEL_SCREEN_CHARACTER_SPACING
// #define PIXEL_SCREEN_CHARACTER_SPACING 1
#endif
#ifndef PIXEL_SCREEN_LINE_SPACING
// #define PIXEL_SCREEN_LINE_SPACING     2
#endif
#ifndef PIXEL_SCREEN_TEXT_COLOR
// #define PIXEL_SCREEN_TEXT_COLOR       0xFFFF
#endif
#ifndef PIXEL_SCREEN_BACKGROUND_COLOR
// #define PIXEL_SCREEN_BACKGROUND_COLOR 0x0000
#endif

// ============================================================================
// BOARD_VERSION
// ============================================================================
// Mogelijke waarden: BOARD_UNO_R3, BOARD_UNO_R4_MINIMA, BOARD_UNO_R4_WIFI, BOARD_ESP32_UNO, BOARD_ARDI32, BOARD_CYTRON_MAKER_UNO_RP2040, BOARD_NUCLEO_F401RE.
// Hardwarevalidatiestatus (zie docs/HARDWARE_SUPPORT.md voor details):
// - Getest en goedgekeurd: BOARD_UNO_R3, BOARD_UNO_R4_MINIMA, BOARD_UNO_R4_WIFI.
// - Compileert, hardwarematig nog niet bevestigd: BOARD_ESP32_UNO (WEMOS D1 R32) sinds v1.0.0.
// - Aanvullend ondersteund (experimenteel), geen aparte hardwarevalidatiestatus vastgelegd: BOARD_ARDI32, BOARD_CYTRON_MAKER_UNO_RP2040, BOARD_NUCLEO_F401RE.
#ifndef BOARD_VERSION
// #define BOARD_VERSION BOARD_UNO_R3   // wissel dit om van bord te wisselen
#endif

// ============================================================================
// ADC BACKEND (naast BOARD_VERSION)
// ============================================================================
// LET OP: Wanneer je ADC_BACKEND op ADC_BACKEND_ADS1115 zet, moet de Adafruit ADS1X15-library geïnstalleerd zijn via de Arduino Library Manager.
// ============================================================================
#ifndef AANTAL_SENSOREN_AANWEZIG
// #define AANTAL_SENSOREN_AANWEZIG  2
#endif

// Mogelijke waarden: ADC_BACKEND_NATIVE of ADC_BACKEND_ADS1115
#ifndef ADC_BACKEND
// #define ADC_BACKEND               ADC_BACKEND_NATIVE  // wissel dit om van backend te wisselen
#endif

#ifndef I2C_ADDRESS_ADS1115
// #define I2C_ADDRESS_ADS1115       0x48
#endif

#ifndef ADS1115_I2C_ADDRESS
// #define ADS1115_I2C_ADDRESS       0x48                // standaardadres via SW1: ADDR naar GND
#endif

// Instelbare vertraging in de busy-wait loop van WachtTotAlleSensorsLosgelatenVoorTest().
// Bij ADC_BACKEND_NATIVE blijft dit 0. Bij ADC_BACKEND_ADS1115 voorkomt dit dat de I2C-bus zonder onderbreking bevraagd wordt.
#ifndef WACHT_LOSLATEN_DELAY_MS
// #define WACHT_LOSLATEN_DELAY_MS 0UL
#endif

// Definieer de pinnen voor de sensoren op jouw sensorboard
// Bij ADC_BACKEND_ADS1115 zijn dit kanaalnummers 0 tot 3; bij ADC_BACKEND_NATIVE zijn dit Arduino-pinnen.
#ifndef PIN_SENSOR_1
// #define PIN_SENSOR_1 A0  // Analoge pin voor de 1ste test-sensor
#endif
#ifndef PIN_SENSOR_2
// #define PIN_SENSOR_2 A1  // Analoge pin voor de  2de test-sensor
#endif
#ifndef PIN_SENSOR_3
// #define PIN_SENSOR_3 A2  // Analoge pin voor de  3de test-sensor
#endif
#ifndef PIN_SENSOR_4
// #define PIN_SENSOR_4 A3  // Analoge pin voor de  4de test-sensor
#endif

#ifndef DELAY_US
// #define DELAY_US 100   // Vertraging tussen samples in de meetlussen
#endif

// ============================================================================
// DEBUG INSTELLINGEN 
// ============================================================================
// DEBUG_PRINT, DEBUG_PRINTLN en DEBUG_PRINTLN2 worden door SystemConfig.h afgeleid uit DEBUG.

#endif
