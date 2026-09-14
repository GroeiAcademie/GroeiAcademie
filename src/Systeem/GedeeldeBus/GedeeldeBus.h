#ifndef GEDEELDE_BUS_H
#define GEDEELDE_BUS_H

#include <Arduino.h>
#include "../../Configuratie/SystemConfig.h"

// ============================================================================
// GedeeldeBus: neutrale, gedeelde Wire(I2C)/SPI-initialisatie.
//
// AANLEIDING: Screen.cpp, Input.cpp (PCF8574-tak) en Stimulus.cpp (InitialiseerADS1115()) riepen elk onafhankelijk Wire.begin() aan, met identieke, maar apart onderhouden board-specifieke logica (ARDI32 gebruikt aparte SDA/SCL-pinnen, andere boards de standaard Wire.begin()). 
// Bij gelijktijdig gebruik van bijvoorbeeld Input met PCF8574 én een I2C-scherm was er geen garantie dat dit conflictvrij bleef.
//
// GEEN OVERKOEPELENDE KERNEL: Input, Screen en Stimulus blijven bewust onafhankelijke, los te testen subsystemen.
// GedeeldeBus kent geen van drie; ze regelt uitsluitend businitialisatie, verder niets.
//
// GEDRAGSBEHOUDEND: deze wijziging wijzigt de publieke API niet.
// De board-specifieke ARDI32-logica die voorheen apart in Screen.cpp en Input.cpp stond, is hier samengevoegd, met exact hetzelfde gedrag.
//
// GEBRUIK: elk subsysteem dat I2C nodig heeft, roept dit aan in plaats van zelf Wire.begin() aan te roepen. 
// Meerdere aanroepen zijn veilig: enkel de eerste aanroep initialiseert de bus effectief.
// ============================================================================

// SPI-ondersteuning is voorbereid voor toekomstig gebruik. Momenteel heeft GedeeldeBusType::SPI nog geen actieve aanroeper in Input, Screen of Stimulus.
// GedeeldeBusType: geeft aan welke gedeelde bus geïnitialiseerd moet worden.
enum class GedeeldeBusType : uint8_t {
  I2C                = 0x00,
  SPI                = 0x01
};

// Initialiseert de opgegeven bus, enkel bij de eerste aanroep voor dat type.
// Veilig om meermaals aan te roepen vanuit verschillende subsystemen.
void InitialiserenGedeeldeBus(GedeeldeBusType busType);

// Enkel voor testdoeleinden/diagnose: geeft terug of een bus al geïnitialiseerd is.
bool IsGedeeldeBusGeinitialiseerd(GedeeldeBusType busType);

// Reset de interne "al geïnitialiseerd"-vlaggen. Beïnvloedt de fysieke bus zelf niet; enkel bedoeld voor testscenario's.
void ResettenGedeeldeBus();

#ifdef GEDEELDE_BUS_PROTOTYPE
  #include <initializer_list> // ARDUINO UNO R3 ondersteunt dit niet — tijdelijk, want UNO R3 wordt overwogen te schrappen in v1.1.4

// ============================================================================
// PROTOTYPE: centrale registratie van hardware-resources.
// Deze volledige uitbreiding bestaat alleen wanneer GEDEELDE_BUS_PROTOTYPE vóór het includen van dit bestand gedefinieerd is.
// ============================================================================

// GedeeldeBusComponent: geeft aan welk onderdeel van het GROEI ACADEMIE FrameWork een hardware-resource gebruikt.
enum class GedeeldeBusComponent : uint8_t {
  // ADC
  ADC_ADS1115        = 0x00,
  ADC_NATIVE         = 0x01,

  // Input
  INPUT_DIGITAL      = 0x10,
  INPUT_HX1838       = 0x11,
  INPUT_PCF8574      = 0x12,

  // Output
  CHARACTER_SCREEN   = 0x20,
  PIXEL_SCREEN       = 0x21,
  SERIAL_OUTPUT      = 0x22
};

// HardwareResourceType: geeft aan om welk type hardware-resource het gaat.
enum class HardwareResourceType : uint8_t {
  ADC                = 0x00, // Analog Digitaal Converter (analoog-digitaalomzetter)
  CAN                = 0x01, // Controller Area Network
  DAC                = 0x02, // Digital-to-Analog Converter 
  DMA                = 0x03, // Direct Memory Access
  EEPROM             = 0x04, // Electrically Erasable Programmable Read-Only Memory
  GPIO               = 0x05, // General-Purpose Input/Output
  I2C                = 0x06, // Inter-Integrated Circuit (SDS: Serial data & SCL: Serial clock)
  I2S                = 0x07, // Inter-IC Sound (SCK/BCLK: Continuoous Serial Clock/Bit Clock, WS/LRCLK:Word Select/Left-Richt Clock & SD/SDATA: Serial Data)
  INTERRUPT          = 0x08, // Pin interrupts
  IR                 = 0x09, // Infrarood hardware
  JTAG               = 0x0A, // Joint Test Action Group: voor het testen, debuggen en programmeren van microchips direct op de hardware
  MATRIX             = 0x0B, // On-board LED-matrix (Uno R4 WiFi, Uno Q)
  PWM                = 0x0C, // Pulse Width Modulation: Pulsbreedtemodulatie, een techniek waarmee een digitale outputpin een analoog signaal simuleert
  QSPI               = 0x0D, // Quad Serial Peripheral Interface: voor extern flash/PSRAMC(S3, RP2040, Uno Q)
  RMII               = 0x0E, // Reduced Media Independent Interface: netwerkinterface die de verbinding vormt tussen de MAC-laag en de PHY-laag
  RNG                = 0x0F, // Hardware Random Number Generator (Beveiliging)
  RTC                = 0x10, // Hardware Real-Time Clock (STM32, ESP32, Uno Q)
  SDIO               = 0x11, // Serial Digital Input Output: snelle SD-kaart bus
  SPI                = 0x12, // Serial Peripheral Interface: synchroon serieel communicatieprotocol
  SWD                = 0x13, // Serial Wire Debug: debuggen en programmeren van microcontrollers, met name chips gebaseerd op de ARM Cortex-architectuur
  TIMER              = 0x14, // Hardware timers
  TOUCH              = 0x15, // Hardware Capacitive Touch (ESP32-reeks)
  UART               = 0x16, // Universal Asynchronous Receiver-Transmitter: seriële communicatie tussen exact twee apparaten
  USB                = 0x17  // Universal Serial Bus
};

// Alleen fysieke resources van de UNO-vormfactor.
// Sensor- of module-interne resources horen bij die sensor/module en worden niet in deze enum verzameld.
// HardwareResourcePin: geeft aan welke fysieke resource van de UNO-vormfactor gebruikt wordt.
enum class HardwareResourcePin : uint8_t {
  // Digitale pinnen 
  D0                 = 0x00,
  D1                 = 0x01,
  D2                 = 0x02,
  D3                 = 0x03,
  D4                 = 0x04,
  D5                 = 0x05,
  D6                 = 0x06,
  D7                 = 0x07,
  D8                 = 0x08,
  D9                 = 0x09,
  D10                = 0x0A,
  D11                = 0x0B,
  D12                = 0x0C,
  D13                = 0x0D,

  // Analoge pinnen 
  A0                 = 0x10,
  A1                 = 0x11,
  A2                 = 0x12,
  A3                 = 0x13,
  A4                 = 0x14,
  A5                 = 0x15,

  // I2C 
  SDA                = 0x20,
  SCL                = 0x21,

  // SPI 
  MISO               = 0x30,
  MOSI               = 0x31,
  SCK                = 0x32,
  SS                 = 0x33,

  CUSTOM             = 0xFE, // geen vast shieldlabel; de effectieve pin wordt apart meegegeven (bv. instelbare pinnen zoals PIXEL_SCREEN_DC)
  NONE               = 0xFF  // geen pin van toepassing (bv. bij UART, EEPROM, of het I2C-adres zelf)
};

// HardwareResourceToegang: bepaalt of een hardware-resource door meerdere componenten gedeeld mag worden of exclusief gebruikt wordt.
enum class HardwareResourceToegang : uint8_t {
  GEDEELD            = 0x00,
  EXCLUSIEF          = 0x01
};

// GedeeldeBusRol: geeft aan welke rol een component op een gedeelde bus heeft.
enum class GedeeldeBusRol : uint8_t {
  AUTONOOM           = 0x00,
  MASTER             = 0x01,
  SLAVE              = 0x02
};

// GedeeldeBusHardwareResourceAanmelding: bevat één volledige registratie van een component en de hardware-resource die het gebruikt.
struct GedeeldeBusHardwareResourceAanmelding {
  GedeeldeBusComponent component;
  HardwareResourceType type;
  HardwareResourcePin resource;
  uint8_t pin;
  HardwareResourceToegang toegang;
  GedeeldeBusRol rol;
  uint8_t adres;
};

#ifndef GEDEELDE_BUS_MAX_AANMELDINGEN_HARDWARE_RESOURCES
  #define GEDEELDE_BUS_MAX_AANMELDINGEN_HARDWARE_RESOURCES 24
#endif

#define GEDEELDE_BUS_GEEN_PIN   0xFF
#define GEDEELDE_BUS_GEEN_ADRES 0xFF

// gedeeldeBusAanmeldenOfAfmeldenInSetup: true betekent enkel opslaan (in setup()), geen controle.
// false betekent meteen ook controleren en inpluggen (in loop()). Bij false en een conflict wordt
// de zonet aangemelde resource automatisch weer afgemeld.
bool AanmeldenHardwareResourcesOpGedeeldeBus(
  GedeeldeBusComponent component,
  HardwareResourceType type,
  HardwareResourcePin resource,
  HardwareResourceToegang toegang,
  bool gedeeldeBusAanmeldenOfAfmeldenInSetup,
  GedeeldeBusRol rol = GedeeldeBusRol::AUTONOOM,
  uint8_t adres = GEDEELDE_BUS_GEEN_ADRES,
  uint8_t pinOverride = GEDEELDE_BUS_GEEN_PIN
);

bool AfmeldenHardwareResourcesOpGedeeldeBus(
  GedeeldeBusComponent component,
  HardwareResourceType type,
  HardwareResourcePin resource,
  HardwareResourceToegang toegang,
  bool gedeeldeBusAanmeldenOfAfmeldenInSetup,
  GedeeldeBusRol rol = GedeeldeBusRol::AUTONOOM,
  uint8_t adres = GEDEELDE_BUS_GEEN_ADRES,
  uint8_t pinOverride = GEDEELDE_BUS_GEEN_PIN
);

// Meervoudsversie: meldt meerdere CUSTOM-pinnen van hetzelfde component in één aanroep aan,
// bv. AanmeldenHardwareResourcesOpGedeeldeBus(GedeeldeBusComponent::PIXEL_SCREEN, HardwareResourceType::GPIO, {PIXEL_SCREEN_CS, PIXEL_SCREEN_DC, PIXEL_SCREEN_RST}, HardwareResourceToegang::EXCLUSIEF, true);
// Roept intern, per pin, de enkelvoudsversie aan met HardwareResourcePin::CUSTOM.
bool AanmeldenHardwareResourcesOpGedeeldeBus(
  GedeeldeBusComponent component,
  HardwareResourceType type,
  std::initializer_list<uint8_t> pinnen,
  HardwareResourceToegang toegang,
  bool gedeeldeBusAanmeldenOfAfmeldenInSetup,
  GedeeldeBusRol rol = GedeeldeBusRol::AUTONOOM
);

bool I2CAanmeldenOpGedeeldeBus(
  GedeeldeBusComponent component,
  uint8_t adres,
  HardwareResourcePin sdaResource,
  HardwareResourcePin sclResource,
  bool gedeeldeBusAanmeldenOfAfmeldenInSetup
);

bool SPIAanmeldenOpGedeeldeBus(
  GedeeldeBusComponent component,
  HardwareResourcePin misoResource,
  HardwareResourcePin mosiResource,
  HardwareResourcePin sckResource,
  bool gedeeldeBusAanmeldenOfAfmeldenInSetup
);

bool EEPROMAanmeldenOpGedeeldeBus(GedeeldeBusComponent component, bool gedeeldeBusAanmeldenOfAfmeldenInSetup);
bool UARTAanmeldenOpGedeeldeBus(GedeeldeBusComponent component, bool gedeeldeBusAanmeldenOfAfmeldenInSetup);

// Fase 2: vergelijkt, na alle aanmeldingen, elk paar registraties onderling, symmetrisch.
// Bij een conflict wijzen de twee out-parameters (indien opgegeven) beide betrokken registraties aan.
bool AanmeldingenOpConflictenControlerenOpGedeeldeBus(uint8_t* conflictIndexA = nullptr, uint8_t* conflictIndexB = nullptr);

// Fase 3: veilige poort. Roept fase 2 aan; enkel bij true mag de hardware-init in Input.cpp/Screen.cpp doorgaan.
bool AanmeldingenInpluggenOpGedeeldeBus();

void AantalAanmeldingenOpNulZettenOpGedeeldeBus();
uint8_t OpvragenAantalAanmeldingenOpGedeeldeBus();
const GedeeldeBusHardwareResourceAanmelding* OpvragenPointerAanmeldingOpGedeeldeBus(uint8_t index);

#endif // GEDEELDE_BUS_PROTOTYPE

#endif // GEDEELDE_BUS_H
