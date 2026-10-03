#ifndef GEDEELDE_BUS_H
#define GEDEELDE_BUS_H

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_ADS1X15.h>
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

// ============================================================================
// PROTOTYPE: centrale registratie van hardware-resources.
// Deze volledige uitbreiding bestaat alleen wanneer GEDEELDE_BUS_PROTOTYPE vóór het includen van dit bestand gedefinieerd is.
// ============================================================================

// GedeeldeBusComponent: geeft aan welk onderdeel van het GROEI ACADEMIE FrameWork een hardware-resource gebruikt.
// OPGEPAST: de numerieke waarden hieronder zijn functioneel en mogen niet vrij worden hernummerd.
// GedeeldeBus gebruikt de nummerbereiken om componentgroepen te herkennen.
// Bij wijziging van deze nummering moet alle code die deze bereiken controleert mee worden aangepast.
#define GEDEELDE_BUS_ELKE_COMPONENT(X) \
  /* INPUT ------------------------ */ \
  X(INPUT,                        0x00) /* GedeeldeBusComponent::INPUT */ \
  X(INPUT_DIGITAL,                0x01) /* GedeeldeBusComponent::INPUT_DIGITAL */ \
  X(INPUT_HX1838,                 0x02) /* GedeeldeBusComponent::INPUT_HX1838 */ \
  X(INPUT_PCF8574,                0x03) /* GedeeldeBusComponent::INPUT_PCF8574 */ \
  /*                                */ \
  /* OUTPUT ----------------------- */ \
  X(SCREEN,                       0x10) /* GedeeldeBusComponent::SCREEN */ \
  X(CHARACTER_SCREEN,             0x11) /* GedeeldeBusComponent::CHARACTER_SCREEN */ \
  X(PIXEL_SCREEN,                 0x12) /* GedeeldeBusComponent::PIXEL_SCREEN */ \
  X(SERIAL_OUTPUT,                0x13) /* GedeeldeBusComponent::SERIAL_OUTPUT */ \
  /*                                */ \
  /* EXTENDER --------------------- */ \
  X(EXTENDER,                     0x20) /* GedeeldeBusComponent::EXTENDER */ \
  /* ADC */ \
  X(ADC_ADS1115,                  0x21) /* GedeeldeBusComponent::ADC_ADS1115 */ \
  X(ADC_ADS1158,                  0x22) /* GedeeldeBusComponent::ADC_ADS1158 */ \
  X(ADC_ADS7828,                  0x23) /* GedeeldeBusComponent::ADC_ADS7828 */ \
  X(ADC_ADS7953,                  0x24) /* GedeeldeBusComponent::ADC_ADS7953 */ \
  X(ADC_NATIVE,                   0x25) /* GedeeldeBusComponent::ADC_NATIVE */ \
  /*                                */ \
  /* ADS-multiplexer */ \
  X(ADS_MULTIPLEXER_CD74HC4067,   0x30) /* GedeeldeBusComponent::ADS_MULTIPLEXER_CD74HC4067 */ \
  /*                                */ \
  /* DigitalPins */ \
  X(DIGITAL_PINS_MCP23017,        0x35) /* GedeeldeBusComponent::DIGITAL_PINS_MCP23017 */ \
  X(DIGITAL_PINS_PCF8574,         0x36) /* GedeeldeBusComponent::DIGITAL_PINS_PCF8574 */ \
  X(DIGITAL_PINS_PCF8575,         0x37) /* GedeeldeBusComponent::DIGITAL_PINS_PCF8575 */ \
  /*                                */ \
  /* I2C-multiplexer */ \
  X(I2C_MULTIPLEXER_TCA9548A,     0x40) /* GedeeldeBusComponent::I2C_MULTIPLEXER_TCA9548A */ \
  /*                                */ \
  /* One-Wire */ \
  X(ONE_WIRE_DS2482v800,          0x45) /* GedeeldeBusComponent::ONE_WIRE_DS2482v800 */ \
  /*                                */ \
  /* UART */ \
  X(UART_MAX14830,                0x50) /* GedeeldeBusComponent::UART_MAX14830 */ \
  X(UART_SC16IS752,               0x51) /* GedeeldeBusComponent::UART_SC16IS752 */ \
  /*                                */ \
  /* SENSOR ----------------------- */ \
  X(SENSOR,                       0xA0) /* GedeeldeBusComponent::SENSOR */ \
  X(RFP602,                       0xA1) /* GedeeldeBusComponent::RFP602 */ \
  /*                                */ \
  /* ROOT-node -------------------- */ \
  X(ROOT,                         0xFF) /* GedeeldeBusComponent::ROOT */

enum class GedeeldeBusComponent : uint8_t {
  #define X(naam, waarde) naam = waarde,
  GEDEELDE_BUS_ELKE_COMPONENT(X)
  #undef X
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

// Identiteit van een GEDEELDE pin: welke rol, binnen welk protocol. EXCLUSIEF-claims hebben geen identiteit nodig (GEEN).
// Twee GEDEELD-claims op dezelfde pin zijn enkel compatibel bij exact dezelfde identiteit.
enum class HardwareResourceIdentiteit : uint8_t { GEEN, I2C_SDA, I2C_SCL, SPI_SCK, SPI_MISO, SPI_MOSI };

// Alleen fysieke resources van de UNO-vormfactor.
// Sensor- of module-interne resources horen bij die sensor/module en worden niet in deze enum verzameld.
// HardwareResourcePin: geeft aan welke fysieke resource van de UNO-vormfactor gebruikt wordt.
enum class HardwareResourcePin : uint8_t {
  // Digitale pinnen 
  D0                 = 0x00, // Native Arduino Pin 0
  D1                 = 0x01, // Native Arduino Pin 1
  D2                 = 0x02, // Native Arduino Pin 2
  D3                 = 0x03, // Native Arduino Pin 3
  D4                 = 0x04, // Native Arduino Pin 4
  D5                 = 0x05, // Native Arduino Pin 5
  D6                 = 0x06, // Native Arduino Pin 6
  D7                 = 0x07, // Native Arduino Pin 7
  D8                 = 0x08, // Native Arduino Pin 8
  D9                 = 0x09, // Native Arduino Pin 9
  D10                = 0x0A, // Native Arduino Pin 10
  D11                = 0x0B, // Native Arduino Pin 11
  D12                = 0x0C, // Native Arduino Pin 12
  D13                = 0x0D, // Native Arduino Pin 13

  // Analoge pinnen 
  A0                 = 0x10, // Native Arduino Pin 14
  A1                 = 0x11, // Native Arduino Pin 15
  A2                 = 0x12, // Native Arduino Pin 16
  A3                 = 0x13, // Native Arduino Pin 17
  A4                 = 0x14, // Native Arduino Pin 18
  A5                 = 0x15, // Native Arduino Pin 19

  // I2C 
  SDA                = 0x20, // Native Arduino Pin 18
  SCL                = 0x21, // Native Arduino Pin 19

  // SPI 
  MISO               = 0x30, // Native Arduino Pin 12
  MOSI               = 0x31, // Native Arduino Pin 11
  SCK                = 0x32, // Native Arduino Pin 13
  SS                 = 0x33, // Native Arduino Pin 10

  CUSTOM             = 0xFE, // geen vast shieldlabel; de effectieve pin wordt apart meegegeven (bv. instelbare pinnen zoals PIXEL_SCREEN_DC)
  NONE               = 0xFF  // geen pin van toepassing (bv. bij UART, EEPROM, of het I2C-adres zelf)
};

uint8_t NativeArduinoPinVan(HardwareResourcePin resource, uint8_t pinOverride = static_cast<uint8_t>(HardwareResourcePin::NONE));
HardwareResourcePin ArduinoUnoShieldPinOmzettenNaarHardwareResourcePin(uint8_t pin);
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

// ============================================================================
// Boomgebaseerde resourceregistratie (vervangt de vorige, vlakke array).
// Elke node houdt zelf zijn plaats in de boom bij ("firstChild + nextChild/prevChild"-patroon) - geen centrale array en geen vaste maximumgrootte.
// ============================================================================

// BezettingPinnen: welke pinnen een node claimt, in twee groepen. 
// Gedeelde pinnen mogen door meerdere children van dezelfde parent tegelijk gebruikt worden (bv. SDA/SCL); 
// exclusieve pinnen mogen dat niet (bv. CS, of een I2C-adres, dat hier ook als "gewoon nóg een exclusieve pin" wordt behandeld).
struct BezettingPinnen {
  const uint8_t* gedeeldePinnen;
  uint8_t aantalGedeeld;
  const uint8_t* exclusievePinnen;
  uint8_t aantalExclusief;
};

// Vaste identiteit per rol, voor de twee bestaande gedeelde bussen. Een Extender die aangemeldePinnen herschrijft,
// behoudt dezelfde volgorde (SDA,SCL of SCK,MISO,MOSI) en dus automatisch dezelfde identiteit.
static constexpr HardwareResourceIdentiteit GedeeldeBusIdentiteitI2C[2] = { HardwareResourceIdentiteit::I2C_SDA, HardwareResourceIdentiteit::I2C_SCL };
static constexpr HardwareResourceIdentiteit GedeeldeBusIdentiteitSPI[3] = { HardwareResourceIdentiteit::SPI_SCK, HardwareResourceIdentiteit::SPI_MISO, HardwareResourceIdentiteit::SPI_MOSI };

// HardwareResourceClaimsBetweenTwoNodes meldt zelf elk gevonden conflict (geen return bij de eerste treffer),
// zodat twee knopen die op meerdere pinnen tegelijk botsen ook elk van die botsingen te zien krijgen.
struct GedeeldeBusConflictInfo {
  GedeeldeBusComponent A;
  GedeeldeBusComponent B;
  uint8_t waarde;
  bool isAdres;
  bool botsteOpExclusief;
};

// GedeeldeBusNode: één punt in de boom - root (Native), internal (Extender) of leaf (Sensor/Input/Output). 
// Zelfde struct voor alle drie rollen; het onderscheid zit enkel in welke velden effectief gebruikt worden.
// Gedeelde, statische teller voor het ID-mechanisme (Deel IV, optie 3, definitief gekozen): 
// elk object kent zichzelf een uniek ID toe op het moment van aanmaken, via deze ene teller - geldig voor alle vier de rollen (Extender/Sensor/Input/Output) tegelijk. 
// Geen pool en geen vooraf-maximum. Objecten kunnen statisch bestaan of via GedeeldeBusNewComponent() met new worden aangemaakt.
// Statisch aangemaakte objecten worden door afmelden() niet verwijderd.
extern uint8_t volgendeGedeeldeBusId;

struct GedeeldeBusNode {
  uint8_t id;
  GedeeldeBusNode* parent;
  GedeeldeBusNode* firstChild = nullptr;
  GedeeldeBusNode* nextChild  = nullptr;
  GedeeldeBusNode* prevChild  = nullptr;
  BezettingPinnen aangemeldePinnen;

  // Enkel relevant wanneer deze node zelf een parent is: bepaalt of zijn children deze node exclusief (chip volledig bezet door één child, 
  // ongeacht hoeveel van zijn pinnen die child zelf gebruikt) of gedeeld (meerdere children, elk op hun eigen pin) mogen gebruiken. 
  // Statisch in de config vastgelegd, nooit dynamisch bepaald door wie het eerst aanmeldt.
  HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD;

  // Voor foutmeldingen: welk GedeeldeBusComponent deze node voorstelt.
  GedeeldeBusComponent component;

  bool aangemeld     = false;
  bool gecontroleerd = false;
  bool ingeplugd     = false;
  bool actief        = false;   // true na een geslaagde activeren()
  bool activerenBezig = false;  // true zolang de hook Activeren() van deze node loopt
  bool conflictGevonden = false;
  const HardwareResourceIdentiteit* gedeeldeIdentiteiten = nullptr;

  // true wanneer dit object met new werd aangemaakt door GedeeldeBusNewComponent().
  // Bepaalt of afmelden() dit object na ontkoppelen ook met delete this verwijdert.
  // Standaard false: statisch aangemaakte objecten worden nooit door afmelden() verwijderd.
  bool componentCreated = false;

  GedeeldeBusNode(GedeeldeBusNode* parent, BezettingPinnen aangemeldePinnen, GedeeldeBusComponent component, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : id(volgendeGedeeldeBusId++), parent(parent), aangemeldePinnen(aangemeldePinnen), Extender(Extender), component(component) {}

  virtual ~GedeeldeBusNode() = default;

  // Hook van de vierde stap: nagaan of het ingeplugde component effectief bruikbaar is, en het dan operationeel maken.
  // Standaard niets te doen. Dit is de opvolger van EffectiefInpluggen().
  virtual bool Activeren() { return true; }

#ifdef DEBUG
  virtual void Diagnose() {}
#endif

  // De vijf stappen zijn virtueel, zodat een parent (Screen, Input) zijn children in dezelfde stap kan meenemen.
  // Een override begint met een controle op de eigen toestand, bijvoorbeeld if (ingeplugd) return true;.
  // Een child roept via zijn parent dezelfde stap opnieuw aan, en zonder die controle loopt dat eindeloos door.
  // Lichaam in GedeeldeBus.cpp: legt bij een conflict een record aan in de cyclus-gebonden lijst.
  virtual bool aanmelden();

  virtual bool controleren();

  // Inpluggen: de eigen bus- en pinconfiguratie. Een override roept eerst de basisversie aan, die ook de parent inplugt.
  virtual bool inpluggen() {
#ifdef TRACE
    if (GA_SERIAL) {
      GA_SERIAL.print("TRACE: GedeeldeBusNode::inpluggen(): component=0x");
      GA_SERIAL.println(static_cast<uint8_t>(component), HEX);
    }
#endif
    if (ingeplugd) return true;
    if (!gecontroleerd) return false;
    if (parent != nullptr && !parent->inpluggen()) return false;

    ingeplugd = true;
#ifdef DEBUG
    if (GA_SERIAL) {
      GA_DEBUG_PRINT("DEBUG: inpluggen OK component=0x");
      GA_DEBUG_PRINTLN2(static_cast<uint8_t>(component), HEX);
    }
#endif
    return true;
  }

  // Activeren: de hook Activeren() uitvoeren, nadat de parent actief is.
  // Wanneer de parent net zijn children activeert (activerenBezig), wordt die niet opnieuw geactiveerd.
  virtual bool activeren() {
#ifdef TRACE
    if (GA_SERIAL) {
      GA_SERIAL.print("TRACE: GedeeldeBusNode::activeren(): component=0x");
      GA_SERIAL.println(static_cast<uint8_t>(component), HEX);
    }
#endif
    if (actief) return true;
    if (!ingeplugd) return false;
    if (parent != nullptr && !parent->actief && !parent->activerenBezig && !parent->activeren()) return false;

    activerenBezig = true;
    const bool gelukt = Activeren();
    activerenBezig = false;
    if (!gelukt) return false;

    actief = true;
#ifdef DEBUG
    if (GA_SERIAL) {
      GA_DEBUG_PRINT("DEBUG: activeren OK component=0x");
      GA_DEBUG_PRINTLN2(static_cast<uint8_t>(component), HEX);
    }
#endif
    return true;
  }

  virtual bool afmelden() {
#ifdef TRACE
    if (GA_SERIAL) {
      GA_SERIAL.print("TRACE: GedeeldeBusNode::afmelden(): component=0x");
      GA_SERIAL.println(static_cast<uint8_t>(component), HEX);
    }
#endif
    if (!aangemeld) return true;
    if (firstChild != nullptr) return false;  // heeft nog eigen kinderen: eerst die afmelden
    if (prevChild != nullptr) prevChild->nextChild = nextChild;
    else if (parent != nullptr) parent->firstChild = nextChild;
    if (nextChild != nullptr) nextChild->prevChild = prevChild;
    aangemeld = false;
    gecontroleerd = false;
    ingeplugd = false;
    actief = false;
    activerenBezig = false;
    conflictGevonden = false;

    if (componentCreated) {
      delete this;   // enkel wanneer componentCreated == true
    }

    return true;
  }

  // --------------------------------------------------------------------------
  // Hulpfuncties voor de conflictcontrole.
  // --------------------------------------------------------------------------
  // Een I2C-adres is opgeslagen als adres | 0x80 en valt daardoor buiten het bereik van de pinwaarden.
  static bool IsAdres(uint8_t waarde) { return waarde >= 0x80 && waarde != static_cast<uint8_t>(HardwareResourcePin::NONE); }

  // firstExtenderNode: nullptr betekent de pinnen van het board zelf (Native).
  // Een Extender (componentcode 0x20 t/m 0x7F, behalve ADC_NATIVE) biedt zijn children een eigen pinruimte.
  const GedeeldeBusNode* firstExtenderNode() const {
    for (const GedeeldeBusNode* voorouder = parent; voorouder != nullptr; voorouder = voorouder->parent) {
      const uint8_t code = static_cast<uint8_t>(voorouder->component);
      if (code >= 0x20 && code <= 0x7F && voorouder->component != GedeeldeBusComponent::ADC_NATIVE) return voorouder;
    }

    return nullptr;
  }

  bool IsVoorouderVan(const GedeeldeBusNode* ander) const {
    for (const GedeeldeBusNode* n = ander->parent; n != nullptr; n = n->parent) {
      if (n == this) return true;
    }

    return false;
  }

  static const GedeeldeBusNode* VolgendeInBoom(const GedeeldeBusNode* n, const GedeeldeBusNode* wortel) {
    if (n->firstChild != nullptr) return n->firstChild;

    while (n != wortel) {
      if (n->nextChild != nullptr) return n->nextChild;
      n = n->parent;
    }

    return nullptr;
  }

  uint8_t AantalClaims() const { return aangemeldePinnen.aantalExclusief + aangemeldePinnen.aantalGedeeld; }

  uint8_t ClaimWaarde(uint8_t index, bool& exclusief, HardwareResourceIdentiteit& identiteit) const {
    if (index < aangemeldePinnen.aantalExclusief) {
      exclusief = true;
      identiteit = HardwareResourceIdentiteit::GEEN;
      return aangemeldePinnen.exclusievePinnen[index];
    }

    exclusief = false;
    const uint8_t gedeeldIndex = index - aangemeldePinnen.aantalExclusief;
    identiteit = (gedeeldeIdentiteiten != nullptr) ? gedeeldeIdentiteiten[gedeeldIndex] : HardwareResourceIdentiteit::GEEN;
    return aangemeldePinnen.gedeeldePinnen[gedeeldIndex];
  }

  // Zuiver mechanisch: geen enkele verwijzing naar I2C, SPI of enig ander protocol.
  // Meldt zelf elk gevonden conflict via GedeeldeBusAanmakenTijdelijkConflict (lichaam in GedeeldeBus.cpp,
  // moet Screen.h kunnen includeren) en zet conflictGevonden op de knoop die faalt - geen return bij de
  // eerste treffer, dus twee knopen die op meerdere pinnen tegelijk botsen krijgen elk van die botsingen te zien.
  static void HardwareResourceClaimsBetweenTwoNodes(GedeeldeBusNode* a, const GedeeldeBusNode* b);
};

// ============================================================================
// Sluit de huidige registratiecyclus af: toont elk GB-conflict dat sinds de start van deze cyclus werd geregistreerd,
// op wat op dat moment al werkt, en geeft de tijdelijke lijst volledig vrij. Lichaam in GedeeldeBus.cpp, want deze functie moet Screen.h kunnen includeren.
void GedeeldeBusPrintEnVerwijderTijdelijkeConflicten(GedeeldeBusNode* vanaf);

// GedeeldeBusNewComponent: maakt met new één GedeeldeBus-object aan en voert aanmelden(), controleren(), inpluggen() en activeren() in de juiste volgorde uit.
// Geeft nullptr terug wanneer een stap faalt.
// ============================================================================
template<typename T, typename... Args>
T* GedeeldeBusNewComponent(Args&&... args) {
  T* object = new T(args...);
  object->componentCreated = true;

  const bool geslaagd = static_cast<GedeeldeBusNode*>(object)->aanmelden() && static_cast<GedeeldeBusNode*>(object)->controleren() && static_cast<GedeeldeBusNode*>(object)->inpluggen() && static_cast<GedeeldeBusNode*>(object)->activeren();
  GedeeldeBusPrintEnVerwijderTijdelijkeConflicten(static_cast<GedeeldeBusNode*>(object));

  if (!geslaagd) {
    if (object->aangemeld) static_cast<GedeeldeBusNode*>(object)->afmelden();   // verwijdert het object, want componentCreated is true
    else delete object;
    return nullptr;
  }

  return object;
}

// ============================================================================
// Twee tussenlagen: vangen wat alle I2C- resp. SPI-Extenders (en Screens die via I2C/SPI werken) delen. 
// Vernoemd naar de HardwareResourceType::I2C/SPI- waarden die ze vertegenwoordigen - niet "Extender", want ook CharacterScreen/ PixelScreen erven hiervan.
// ============================================================================
struct HardwareResourceTypeI2C : GedeeldeBusNode {
  uint8_t adres;
  HardwareResourcePin SDA;
  HardwareResourcePin SCL;
  uint8_t gedeeld_[2];
  uint8_t exclusief_[1];

  HardwareResourceTypeI2C(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t adres, HardwareResourcePin SDA, HardwareResourcePin SCL, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : GedeeldeBusNode(parent, { gedeeld_, 2, exclusief_, 1 }, component, Extender), adres(adres), SDA(SDA), SCL(SCL) {
    this->gedeeldeIdentiteiten = GedeeldeBusIdentiteitI2C;
    gedeeld_[0] = static_cast<uint8_t>(SDA);
    gedeeld_[1] = static_cast<uint8_t>(SCL);
    // Een ongeldig adres (buiten 0x08-0x77) wordt hier als NONE opgeslagen, niet als adres | 0x80:
    // dat laatste zou een ongeldig adres zoals 0x89 onomkeerbaar laten samenvallen met het geldige 0x09 (beide worden dan 0x89), 
    // wat een vals GB102 tegen een volledig ongerelateerd adres kan opleveren. 
    // Als NONE wordt de claim overal in de code al genegeerd (if (waarde == geen) continue;), terwijl GB110 los daarvan gewoon het rauwe adres-veld (adres) blijft controleren.
    exclusief_[0] = (adres >= 0x08 && adres <= 0x77) ? static_cast<uint8_t>(adres | 0x80) : static_cast<uint8_t>(HardwareResourcePin::NONE);
  }

  bool controleren() override;

  bool inpluggen() override {
    if (ingeplugd) return true;
    if (!GedeeldeBusNode::inpluggen()) return false;
    InitialiserenGedeeldeBus(GedeeldeBusType::I2C);
    return true;
  }

  // Over te erven basisfuncties - kale Wire-wrappers, generiek voor elk I2C-Extender.
  void WriteByte(uint8_t data) {
    Wire.beginTransmission(adres);
    Wire.write(data);
    Wire.endTransmission();
  }

  void WriteByteRegister(uint8_t reg, uint8_t waarde) {
    Wire.beginTransmission(adres);
    Wire.write(reg);
    Wire.write(waarde);
    Wire.endTransmission();
  }

  uint8_t ReadByte() {
    Wire.requestFrom(adres, (uint8_t)1);
    return Wire.available() ? Wire.read() : 0;
  }

  uint8_t ReadByteRegister(uint8_t reg) {
    Wire.beginTransmission(adres);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom(adres, (uint8_t)1);
    return Wire.available() ? Wire.read() : 0;
  }
};

struct HardwareResourceTypeSPI : GedeeldeBusNode {
  HardwareResourcePin CS;
  HardwareResourcePin SCK;
  HardwareResourcePin MISO;
  HardwareResourcePin MOSI;
  uint8_t gedeeld_[3];
  uint8_t exclusief_[1];

  HardwareResourceTypeSPI(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin CS, HardwareResourcePin SCK, HardwareResourcePin MISO, HardwareResourcePin MOSI, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : GedeeldeBusNode(parent, { gedeeld_, 3, exclusief_, 1 }, component, Extender), CS(CS), SCK(SCK), MISO(MISO), MOSI(MOSI) {
    this->gedeeldeIdentiteiten = GedeeldeBusIdentiteitSPI;
    gedeeld_[0] = static_cast<uint8_t>(SCK);
    gedeeld_[1] = static_cast<uint8_t>(MISO);
    gedeeld_[2] = static_cast<uint8_t>(MOSI);
    exclusief_[0] = static_cast<uint8_t>(CS);
  }

  bool inpluggen() override {
    if (ingeplugd) return true;
    if (CS == HardwareResourcePin::NONE) return false;
    if (!GedeeldeBusNode::inpluggen()) return false;
    InitialiserenGedeeldeBus(GedeeldeBusType::SPI);
    return true;
  }

  // Over te erven basisfunctie - kale SPI-wrapper, generiek voor elk SPI-Extender.
  uint8_t Transfer(uint8_t data) {
    digitalWrite(NativeArduinoPinVan(CS), LOW);
    uint8_t resultaat = SPI.transfer(data);
    digitalWrite(NativeArduinoPinVan(CS), HIGH);
    return resultaat;
  }
};

// ============================================================================
// CONCRETE EXTENDERS — blijven architecturaal onderdeel van GedeeldeBus,
// maar staan voor onderhoud en leesbaarheid elk in een eigen bestand.
// ============================================================================
#include "ExtenderDS2482v800.h"
#include "ExtenderADS1115.h"
#include "ExtenderADS1158.h"
#include "ExtenderADS7828.h"
#include "ExtenderADS7953.h"
#include "ExtenderCD74HC4067.h"
#include "ExtenderMCP23017.h"
#include "ExtenderPCF8574.h"
#include "ExtenderPCF8575.h"
#include "ExtenderTCA9548A.h"
#include "ExtenderMAX14830I2C.h"
#include "ExtenderMAX14830SPI.h"
#include "ExtenderSC16IS752I2C.h"
#include "ExtenderSC16IS752SPI.h"

// ============================================================================
// ADC-BASIS — ADC_NATIVE en ADC_ADS1115 worden als ADC-extender ingeplugd.
// De sensorlaag wordt pas daarna op de gekozen ADC-extender ingeplugd.
// ============================================================================
struct ADC_NATIVE : GedeeldeBusNode {
  uint8_t exclusief_[4];

  ADC_NATIVE(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin A0, HardwareResourcePin A1, HardwareResourcePin A2, HardwareResourcePin A3, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : GedeeldeBusNode(parent, { nullptr, 0, exclusief_, 4 }, component, Extender) {
    exclusief_[0] = static_cast<uint8_t>(A0);
    exclusief_[1] = static_cast<uint8_t>(A1);
    exclusief_[2] = static_cast<uint8_t>(A2);
    exclusief_[3] = static_cast<uint8_t>(A3);
  }
};

struct ADC_ADS1115 : ExtenderADS1115 {
  using ExtenderADS1115::ExtenderADS1115;
};

// ============================================================================
// OUTPUT
// ============================================================================
struct CharacterScreen : HardwareResourceTypeI2C {
  using HardwareResourceTypeI2C::HardwareResourceTypeI2C;

  // De bus is al ingeplugd. Activeren controleert alleen of het scherm antwoordt.
  bool Activeren() override {
#ifdef TRACE
    GA_SERIAL.print("TRACE: CharacterScreen::Activeren(): I2C adres=0x");
    GA_SERIAL.println(adres, HEX);
#endif
    Wire.beginTransmission(adres);
    if (Wire.endTransmission() == 0) {
#ifdef DEBUG
      GA_DEBUG_PRINTLN("DEBUG: CharacterScreen I2C OK");
#endif
      return true;
    }
#ifdef DEBUG
    GA_DEBUG_PRINTLN("DEBUG: CharacterScreen I2C fout");
#endif
    return false;
  }
};

// PixelScreen voegt wél eigen velden toe (DC, RST): eigen constructor, en herbouwt aangemeldePinnen om CS+DC+RST samen als exclusief te bevatten.
struct PixelScreen : HardwareResourceTypeSPI {
  HardwareResourcePin DC;
  HardwareResourcePin RST;
  uint8_t exclusiefMetDcRst_[3];

  PixelScreen(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin CS, HardwareResourcePin SCK, HardwareResourcePin MISO, HardwareResourcePin MOSI, HardwareResourcePin DC, HardwareResourcePin RST, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : HardwareResourceTypeSPI(parent, component, CS, SCK, MISO, MOSI, Extender), DC(DC), RST(RST) {
    exclusiefMetDcRst_[0] = static_cast<uint8_t>(CS);
    exclusiefMetDcRst_[1] = static_cast<uint8_t>(DC);
    exclusiefMetDcRst_[2] = static_cast<uint8_t>(RST);
    aangemeldePinnen = { gedeeld_, 3, exclusiefMetDcRst_, 3 };
  }

  // Inpluggen: de pinnen instellen en in rusttoestand zetten.
  bool inpluggen() override {
    if (ingeplugd) return true;
    if (DC == HardwareResourcePin::NONE) return false;
    if (!HardwareResourceTypeSPI::inpluggen()) return false;

    const uint8_t csPin = NativeArduinoPinVan(CS);
    const uint8_t dcPin = NativeArduinoPinVan(DC);

    pinMode(csPin, OUTPUT);
    pinMode(dcPin, OUTPUT);
    digitalWrite(csPin, HIGH);
    digitalWrite(dcPin, HIGH);

    if (RST != HardwareResourcePin::NONE) {
      const uint8_t rstPin = NativeArduinoPinVan(RST);
      pinMode(rstPin, OUTPUT);
      digitalWrite(rstPin, HIGH);
    }

    return true;
  }

  // Activeren: de verdere scherminitialisatie gebeurt in Screen::Activeren().
  bool Activeren() override {
  /*
    if (MISO == HardwareResourcePin::NONE) return false;

    const uint8_t csPin = NativeArduinoPinVan(CS);
    const uint8_t dcPin = NativeArduinoPinVan(DC);

    // ST7789 RDDID (0x04): commando met DC LOW, daarna dummy + drie ID-bytes lezen met DC HIGH.
    digitalWrite(csPin, LOW);
    digitalWrite(dcPin, LOW);
    SPI.transfer(0x04);
    digitalWrite(dcPin, HIGH);

    SPI.transfer(0x00);
    const uint8_t id1 = SPI.transfer(0x00);
    const uint8_t id2 = SPI.transfer(0x00);
    const uint8_t id3 = SPI.transfer(0x00);

    digitalWrite(csPin, HIGH);

    const bool geenAntwoordLaag = id1 == 0x00 && id2 == 0x00 && id3 == 0x00;
    const bool geenAntwoordHoog = id1 == 0xFF && id2 == 0xFF && id3 == 0xFF;
    return !geenAntwoordLaag && !geenAntwoordHoog;  	
    */

    return true;
  }
};

// SerialOutput voegt wél eigen velden toe (TX, RX): eigen constructor.
struct SerialOutput : GedeeldeBusNode {
  HardwareResourcePin TX;
  HardwareResourcePin RX;
  uint8_t exclusief_[2];

  SerialOutput(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin TX, HardwareResourcePin RX, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : GedeeldeBusNode(parent, { nullptr, 0, exclusief_, 2 }, component, Extender), TX(TX), RX(RX) {
    exclusief_[0] = static_cast<uint8_t>(TX);
    exclusief_[1] = static_cast<uint8_t>(RX);
  }

  // Activeren: Serial starten en wachten op een verbinding.
  bool Activeren() override {
    GA_SERIAL.begin(SERIAL_BAUDRATE);

#ifdef USE_SERIAL_CONNECT_TIMEOUT_MS
    const unsigned long startTijd = millis();
    while (!GA_SERIAL && (millis() - startTijd) < SERIAL_CONNECT_TIMEOUT_MS) { ; }
#ifdef TRACE
    GA_SERIAL.println("TRACE: SerialOutput::Activeren(): wachten klaar");
#endif
#ifdef DEBUG
    GA_DEBUG_PRINT("DEBUG: Serial actief=");
    GA_DEBUG_PRINTLN((bool)GA_SERIAL);
#endif
    return (bool)GA_SERIAL;
#else
    while (!GA_SERIAL) { ; }
#ifdef TRACE
    GA_SERIAL.println("TRACE: SerialOutput::Activeren(): Serial actief");
#endif
#ifdef DEBUG
    GA_DEBUG_PRINTLN("DEBUG: Serial actief");
#endif
    return true;
#endif
  }
};

// ============================================================================
// INPUT - geen eigen velden: erven de constructor van GedeeldeBusNode rechtstreeks.
// ============================================================================
struct InputDigital : GedeeldeBusNode {
  using GedeeldeBusNode::GedeeldeBusNode;
};

struct InputHX1838 : GedeeldeBusNode {
  using GedeeldeBusNode::GedeeldeBusNode;
};

struct InputPCF8574 : ExtenderPCF8574 {
  InputPCF8574(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t adres, HardwareResourcePin SDA, HardwareResourcePin SCL, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : ExtenderPCF8574(parent, component, adres, SDA, SCL, HardwareResourcePin::NONE, Extender) {}
};

// ============================================================================
// Tussenstruct voor alle sensoren. Leeg vandaag; hier komen later de sensor-specifieke functies bij (ReadyForData, DataOpslaan, ...) die niet in
// GedeeldeBusNode zelf horen, want dat zou ook de Extenders raken.
// ============================================================================
struct Sensor : GedeeldeBusNode {
  using GedeeldeBusNode::GedeeldeBusNode;
};

// ============================================================================
// Native-singleton (Deel V-fix): de echte, bestaande wortel van de boom.
// Alle top-level nodes (Extenders, Screens, native Input/Output) wijzen hiernaar als parent, 
// in plaats van nullptr - pas dan werkt de sibling-conflictcontrole in controleren() ook op dit niveau (bv. twee I2C-apparaten op hetzelfde adres, 
// of RFP602 en SerialOutput die toevallig dezelfde D-pin claimen).
// ============================================================================
extern GedeeldeBusNode Native;

#if ADC_BACKEND == ADC_BACKEND_NATIVE
extern struct ADC_NATIVE ADC_NATIVE;
#elif ADC_BACKEND == ADC_BACKEND_ADS1115
extern struct ADC_ADS1115 ADC_ADS1115;
#endif


#endif // GEDEELDE_BUS_H
