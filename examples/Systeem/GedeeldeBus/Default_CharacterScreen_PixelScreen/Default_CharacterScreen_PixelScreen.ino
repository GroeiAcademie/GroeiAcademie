// ============================================================================
// Default CharacterScreen en PixelScreen
// ============================================================================
// ============================================================================
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <Screen.h>
#include <Systeem/GedeeldeBus/GedeeldeBus.h>
#include <Configuratie/ExamplesConfig.h>

// Schakel GEDEELDE_BUS_PROTOTYPE om de nieuwe GedeeldeBus-functionaliteit te kunnen testen.
// Zonder deze regel wordt alleen de bestaande, stabiele GedeeldeBus-code gebruikt en krijgt deze .ino compileerfouten.
// Verwijder in 'SystemConfig.h' dus // voor '// #define GEDEELDE_BUS_PROTOTYPE' om deze prototypefuncties beschikbaar te maken.

// Dit example stelt SCREEN_OUTPUT_CONFIG NIET zelf in — dat kan een .ino structureel niet: Screen.cpp wordt als apart bestand gecompileerd en ziet een #define hier nooit. 
// Zet SCREEN_TYPE_CHARACTER én SCREEN_TYPE_PIXELS daarom in UserConfig.h (kopieer van UserConfig_template.h) of rechtstreeks in SystemConfig.h. 
// Onderstaande controle geeft een duidelijke foutmelding als dat nog niet gebeurd is, in plaats van de sketch stil te laten falen.
#if (((SCREEN_OUTPUT_CONFIG) & (SCREEN_TYPE_CHARACTER | SCREEN_TYPE_PIXELS)) != (SCREEN_TYPE_CHARACTER | SCREEN_TYPE_PIXELS))
  #error Stel SCREEN_OUTPUT_CONFIG in UserConfig.h in met SCREEN_TYPE_CHARACTER en SCREEN_TYPE_PIXELS.
#else

Adafruit_ST7789 pixelScreen(PIXEL_SCREEN_CS, PIXEL_SCREEN_DC, PIXEL_SCREEN_RST);

void setup() {
  // FASE 1: aanmelden.
  RegistratiesResettenOpGedeeldeBus();
  I2CAanmeldenOpGedeeldeBus(GedeeldeBusComponent::CHARACTER_SCREEN, I2C_ADDRESS_CHARACTER_SCREEN, HardwareResourcePin::SDA, HardwareResourcePin::SCL, SetupOfLoop::SETUP);
  SPIAanmeldenOpGedeeldeBus(GedeeldeBusComponent::PIXEL_SCREEN, HardwareResourcePin::MISO, HardwareResourcePin::MOSI, HardwareResourcePin::SCK, SetupOfLoop::SETUP);
  ResourcesAanmeldenOpGedeeldeBus(GedeeldeBusComponent::PIXEL_SCREEN, HardwareResourceType::GPIO, HardwareResourcePin::SS, HardwareResourceToegang::EXCLUSIEF, SetupOfLoop::SETUP);
  ResourcesAanmeldenOpGedeeldeBus(GedeeldeBusComponent::PIXEL_SCREEN, HardwareResourceType::GPIO, HardwareResourcePin::D9, HardwareResourceToegang::EXCLUSIEF, SetupOfLoop::SETUP);
  ResourcesAanmeldenOpGedeeldeBus(GedeeldeBusComponent::PIXEL_SCREEN, HardwareResourceType::GPIO, HardwareResourcePin::D7, HardwareResourceToegang::EXCLUSIEF, SetupOfLoop::SETUP);

#if defined(DEBUG) && (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; } // Wacht hier totdat er een seriële verbinding is
#endif

/*
#if BOARD_VERSION == BOARD_CYTRON_MAKER_UNO_RP2040
  // Dwing de RP2040-chip hardwarematig naar de juiste SPI-pinnen
  SPI.setRX(ARDUINO_UNO_SHIELD_PIN_D12);   // MISO hard op 12
  SPI.setTX(ARDUINO_UNO_SHIELD_PIN_D11);   // Zet SDA/MOSI hard op 11
  SPI.setSCK(ARDUINO_UNO_SHIELD_PIN_SCK);  // Zet SCL/SCK hard op 10
  SPI.begin();                             // Start de hardware SPI-bus
  
  // Vertraag de bus direct naar een stabiele snelheid voor het pixelscherm
  SPI.beginTransaction(SPISettings(16000000, MSBFIRST, SPI_MODE0)); 
#endif
*/

  // FASE 2 en 3.
  bool magInpluggen = AlleAangemeldeResourcesInpluggenOpGedeeldeBus();
  if (!magInpluggen) {
    PrintToScreen("GedeeldeBus", "RESOURCECONFLICT", LEESTIJD_VOORBEELD_MS);
    return;
  }

  pixelScreen.init(ACTIEF_PIXEL_SCREEN_BREEDTE, ACTIEF_PIXEL_SCREEN_HOOGTE);
  pixelScreen.setRotation(PIXEL_SCREEN_ROTATION);
  PixelScreen = &pixelScreen;

  // ScreensConfigureren() roept, aan de hand van SCREEN_OUTPUT, automatisch
  // CharacterScreenConfigureren() en/of PixelScreenConfigureren() aan — geen
  // deprecatie van de losse functies, dit is enkel een optionele gemakslaag.
  // Wil je granulaire controle (bv. één scherm pas later configureren), gebruik
  // dan de losse functies zoals in de andere Screen-voorbeelden.
  ScreensConfigureren();

  PrintToScreen("GedeeldeBus", "registratie OK", LEESTIJD_VOORBEELD_MS);

#if defined(DEBUG) && (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
  PrintActieveSystemConfiguratie();
#endif
}

void loop() {
  while (true) {
#if defined(DEBUG) && (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
  ST7789LeesID();  
#endif
    PrintToScreen("GROEI ACADEMIE", "TYPE_NONE", LEESTIJD_VOORBEELD_MS);
    PrintToScreen(ScreenData::TYPE_NONE, "EERSTE REGEL", "TWEEDE REGEL", LEESTIJD_VOORBEELD_MS, " NU", "DERDE REGEL", "VIERDE REGEL", WACHTTIJD_TUSSEN_PAGINAS_MS);
  }
}

#if defined(DEBUG) && (SCREEN_OUTPUT & SCREEN_TYPE_SERIAL)
void ST7789LeesID() {
  pinMode(PIXEL_SCREEN_CS, OUTPUT);
  pinMode(PIXEL_SCREEN_DC, OUTPUT);
  digitalWrite(PIXEL_SCREEN_CS, HIGH);
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE3));
  digitalWrite(PIXEL_SCREEN_CS, LOW);
  digitalWrite(PIXEL_SCREEN_DC, LOW);   // commando-modus
  SPI.transfer(0x04);                   // RDDID
  digitalWrite(PIXEL_SCREEN_DC, HIGH);  // datamodus
  byte dummy = SPI.transfer(0x00);      // eerste byte is doorgaans dummy
  byte id1 = SPI.transfer(0x00);
  byte id2 = SPI.transfer(0x00);
  byte id3 = SPI.transfer(0x00);
  digitalWrite(PIXEL_SCREEN_CS, HIGH);
  SPI.endTransaction();
  GA_SERIAL.print("RDDID: ");
  GA_SERIAL.print(id1, HEX); GA_SERIAL.print(" ");
  GA_SERIAL.print(id2, HEX); GA_SERIAL.print(" ");
  GA_SERIAL.println(id3, HEX);
}

void PrintSPICongruentie(const char* label, uint8_t waarde) {
  GA_SERIAL.print(label);
  GA_SERIAL.println(waarde);
}

void PrintActieveSystemConfiguratie() {
  GA_SERIAL.println(F("=== PrintActieveSystemConfiguratie() ==="));
  GA_SERIAL.println(F("BOARD: ---------------------------------"));
  GA_SERIAL.print(F("BOARD_VERSION=")); GA_SERIAL.println(BOARD_VERSION);
  GA_SERIAL.print(F("BOARD_ID="));      GA_SERIAL.println(BOARD_ID);

  GA_SERIAL.println(F("----------------------------------------"));
  GA_SERIAL.println(F("BOARD PINs: ----------------------------"));
  GA_SERIAL.println(F("----------------------------------------"));
  GA_SERIAL.println(F("ARDUINO_UNO_SHIELD_PIN_* (bibliotheek, board-specifiek)."));

  GA_SERIAL.println(F("----------------------------------------"));
  PrintSPICongruentie("ARDUINO_UNO_SHIELD_PIN_MOSI=", ARDUINO_UNO_SHIELD_PIN_MOSI);
  PrintSPICongruentie("ARDUINO_UNO_SHIELD_PIN_MISO=", ARDUINO_UNO_SHIELD_PIN_MISO);
  PrintSPICongruentie("ARDUINO_UNO_SHIELD_PIN_SCK=",  ARDUINO_UNO_SHIELD_PIN_SCK);
  PrintSPICongruentie("ARDUINO_UNO_SHIELD_PIN_SS=",   ARDUINO_UNO_SHIELD_PIN_SS);
  PrintSPICongruentie("ARDUINO_UNO_SHIELD_PIN_SDA=",  ARDUINO_UNO_SHIELD_PIN_SDA);
  PrintSPICongruentie("ARDUINO_UNO_SHIELD_PIN_SCL=",  ARDUINO_UNO_SHIELD_PIN_SCL);

  GA_SERIAL.println(F("----------------------------------------"));
  PrintSPICongruentie("ARDUINO_UNO_SHIELD_PIN_D0=",  ARDUINO_UNO_SHIELD_PIN_D0);
  PrintSPICongruentie("ARDUINO_UNO_SHIELD_PIN_D1=",  ARDUINO_UNO_SHIELD_PIN_D1);
  PrintSPICongruentie("ARDUINO_UNO_SHIELD_PIN_D2=",  ARDUINO_UNO_SHIELD_PIN_D2);
  PrintSPICongruentie("ARDUINO_UNO_SHIELD_PIN_D3=",  ARDUINO_UNO_SHIELD_PIN_D3);
  PrintSPICongruentie("ARDUINO_UNO_SHIELD_PIN_D4=",  ARDUINO_UNO_SHIELD_PIN_D4);
  PrintSPICongruentie("ARDUINO_UNO_SHIELD_PIN_D5=",  ARDUINO_UNO_SHIELD_PIN_D5);
  PrintSPICongruentie("ARDUINO_UNO_SHIELD_PIN_D6=",  ARDUINO_UNO_SHIELD_PIN_D6);
  PrintSPICongruentie("ARDUINO_UNO_SHIELD_PIN_D7=",  ARDUINO_UNO_SHIELD_PIN_D7);
  PrintSPICongruentie("ARDUINO_UNO_SHIELD_PIN_D8=",  ARDUINO_UNO_SHIELD_PIN_D8);
  PrintSPICongruentie("ARDUINO_UNO_SHIELD_PIN_D9=",  ARDUINO_UNO_SHIELD_PIN_D9);
  PrintSPICongruentie("ARDUINO_UNO_SHIELD_PIN_D10=", ARDUINO_UNO_SHIELD_PIN_D10);
  PrintSPICongruentie("ARDUINO_UNO_SHIELD_PIN_D11=", ARDUINO_UNO_SHIELD_PIN_D11);
  PrintSPICongruentie("ARDUINO_UNO_SHIELD_PIN_D12=", ARDUINO_UNO_SHIELD_PIN_D12);
  PrintSPICongruentie("ARDUINO_UNO_SHIELD_PIN_D13=", ARDUINO_UNO_SHIELD_PIN_D13);

  GA_SERIAL.println(F("----------------------------------------"));
  PrintSPICongruentie("ARDUINO_UNO_SHIELD_PIN_A0=", ARDUINO_UNO_SHIELD_PIN_A0);
  PrintSPICongruentie("ARDUINO_UNO_SHIELD_PIN_A1=", ARDUINO_UNO_SHIELD_PIN_A1);
  PrintSPICongruentie("ARDUINO_UNO_SHIELD_PIN_A2=", ARDUINO_UNO_SHIELD_PIN_A2);
  PrintSPICongruentie("ARDUINO_UNO_SHIELD_PIN_A3=", ARDUINO_UNO_SHIELD_PIN_A3);
#ifdef ARDUINO_UNO_SHIELD_PIN_A4
  PrintSPICongruentie("ARDUINO_UNO_SHIELD_PIN_A4=", ARDUINO_UNO_SHIELD_PIN_A4);
#endif
#ifdef ARDUINO_UNO_SHIELD_PIN_A5
  PrintSPICongruentie("ARDUINO_UNO_SHIELD_PIN_A5=", ARDUINO_UNO_SHIELD_PIN_A5);
#endif

  GA_SERIAL.println(F("----------------------------------------"));
  GA_SERIAL.println(F("ADC: -----------------------------------"));
  GA_SERIAL.println(F("----------------------------------------"));
  GA_SERIAL.print(F("ADC_BACKEND=")); GA_SERIAL.println(ADC_BACKEND);
  GA_SERIAL.print(F("ADC_BITS="));    GA_SERIAL.println(ADC_BITS);
#if ADC_BACKEND == ADC_BACKEND_ADS1115
  GA_SERIAL.print(F("I2C_ADDRESS_ADS1115=")); GA_SERIAL.println(I2C_ADDRESS_ADS1115);
#endif

  GA_SERIAL.println(F("----------------------------------------"));
  GA_SERIAL.println(F("SCREEN: --------------------------------"));
  GA_SERIAL.println(F("----------------------------------------"));
  GA_SERIAL.print(F("SCREEN_OUTPUT_CONFIG=")); GA_SERIAL.println(SCREEN_OUTPUT_CONFIG);

#if (SCREEN_OUTPUT_CONFIG & SCREEN_TYPE_CHARACTER)
  GA_SERIAL.println(F("----------------------------------------"));
  GA_SERIAL.print(F("ACTIEF_CHARACTER_SCREEN=")); GA_SERIAL.println(ACTIEF_CHARACTER_SCREEN);
  GA_SERIAL.print(F("I2C_ADDRESS_CHARACTER_SCREEN=")); GA_SERIAL.println(I2C_ADDRESS_CHARACTER_SCREEN);
#endif

#if (SCREEN_OUTPUT_CONFIG & SCREEN_TYPE_PIXELS)
  GA_SERIAL.println(F("----------------------------------------"));
  GA_SERIAL.print(F("ACTIEF_PIXEL_SCREEN=")); GA_SERIAL.println(ACTIEF_PIXEL_SCREEN);
  GA_SERIAL.print(F("PIXEL_SCREEN_CS="));  GA_SERIAL.println(PIXEL_SCREEN_CS);
  GA_SERIAL.print(F("PIXEL_SCREEN_DC="));  GA_SERIAL.println(PIXEL_SCREEN_DC);
  GA_SERIAL.print(F("PIXEL_SCREEN_RST=")); GA_SERIAL.println(PIXEL_SCREEN_RST);
#endif

  GA_SERIAL.println(F("----------------------------------------"));
  GA_SERIAL.println(F("INPUT: ---------------------------------"));
  GA_SERIAL.println(F("----------------------------------------"));
  GA_SERIAL.print(F("INPUT_KANAAL_CONFIG="));   GA_SERIAL.println(INPUT_KANAAL_CONFIG);

#if (INPUT_KANAAL_CONFIG & INPUT_TYPE_HX1838)
  GA_SERIAL.println(F("----------------------------------------"));
  GA_SERIAL.print(F("HX1838_ONTVANGER_PIN="));   GA_SERIAL.println(HX1838_ONTVANGER_PIN);
  GA_SERIAL.print(F("HX1838_TOETSENINDELING=")); GA_SERIAL.println(HX1838_TOETSENINDELING);
  GA_SERIAL.print(F("HX1838_BRON_CODES="));      GA_SERIAL.println(HX1838_BRON_CODES);
#endif

#if (INPUT_KANAAL_CONFIG & INPUT_TYPE_PCF8574)
  GA_SERIAL.println(F("----------------------------------------"));
  GA_SERIAL.print(F("I2C_ADDRESS_PCF8574=")); GA_SERIAL.println(I2C_ADDRESS_PCF8574);
  GA_SERIAL.print(F("KEYPAD_TYPE="));         GA_SERIAL.println(KEYPAD_TYPE);
#endif

#if (INPUT_KANAAL_CONFIG & INPUT_TYPE_DIGITAL)
  GA_SERIAL.println(F("----------------------------------------"));
  GA_SERIAL.print(F("PIN_TOETS_1=")); GA_SERIAL.println(PIN_TOETS_1);
  GA_SERIAL.print(F("PIN_TOETS_2=")); GA_SERIAL.println(PIN_TOETS_2);
  GA_SERIAL.print(F("PIN_TOETS_3=")); GA_SERIAL.println(PIN_TOETS_3);
  GA_SERIAL.print(F("PIN_TOETS_4=")); GA_SERIAL.println(PIN_TOETS_4);
#endif

  GA_SERIAL.println(F("========================================"));
}
#endif

#endif
