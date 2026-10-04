#include "GedeeldeBus.h"
#include <Wire.h>
#include <SPI.h>
#include <string.h>
#include "../../Configuratie/SystemConfig.h"
#include "../Screen/Screen.h"   // enkel deze .cpp mag dit includeren - Screen.h zelf includeert GedeeldeBus.h

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

static bool i2cGeinitialiseerd = false;
static bool spiGeinitialiseerd = false;

void InitialiserenGedeeldeBus(GedeeldeBusType busType) {
#ifdef TRACE
  if (GA_SERIAL) {
    GA_SERIAL.print("TRACE: InitialiserenGedeeldeBus(): ");
    GA_SERIAL.println(busType == GedeeldeBusType::I2C ? "I2C" : "SPI");
  }
#endif

  if (busType == GedeeldeBusType::I2C) {
    if (!i2cGeinitialiseerd) {
      #if BOARD_VERSION == BOARD_ESP32S3_ARDI32 || BOARD_VERSION == BOARD_ESP32S3_DEV
        Wire.begin(ARDUINO_UNO_SHIELD_PIN_SDA, ARDUINO_UNO_SHIELD_PIN_SCL);
      #else
        Wire.begin();
      #endif

      i2cGeinitialiseerd = true;
#ifdef DEBUG
      if (GA_SERIAL) GA_DEBUG_PRINTLN("DEBUG: I2C geinitialiseerd");
#endif
#ifdef TRACE
    } else {
      if (GA_SERIAL) GA_SERIAL.println("TRACE: I2C reeds geinitialiseerd");
#endif
    }
  } else if (busType == GedeeldeBusType::SPI) {
    if (!spiGeinitialiseerd) {
      SPI.begin();
      spiGeinitialiseerd = true;
#ifdef DEBUG
      if (GA_SERIAL) GA_DEBUG_PRINTLN("DEBUG: SPI geinitialiseerd");
#endif
#ifdef TRACE
    } else {
      if (GA_SERIAL) GA_SERIAL.println("TRACE: SPI reeds geinitialiseerd");
#endif
    }
  }
}

/*
void InitialiserenGedeeldeBus(GedeeldeBusType busType) {
  if (busType == GedeeldeBusType::I2C) {
    if (!i2cGeinitialiseerd) {
      #if BOARD_VERSION == BOARD_ESP32S3_ARDI32
        Wire.begin(ARDUINO_UNO_SHIELD_PIN_SDA, ARDUINO_UNO_SHIELD_PIN_SCL);
      #else
        Wire.begin();
      #endif
      
      #if defined(BOARD_UNO_Q) // Extra hersteltijd voor het Zephyr RTOS om threads en interrupts te ordenen
        delay(150); 
      #endif
      
      i2cGeinitialiseerd = true;
    }
  } else if (busType == GedeeldeBusType::SPI) {
    if (!spiGeinitialiseerd) {
      #if defined(BOARD_UNO_Q) // Zet de CS (SS) van het pixelscreen HARD HOOG vóór SPI.begin(). Dit voorkomt dat het pixelscreen de gedeelde I2C-klok/data stoort tijdens de opstartpuls.
      pinMode(ARDUINO_UNO_SHIELD_PIN_SS, OUTPUT);
      digitalWrite(ARDUINO_UNO_SHIELD_PIN_SS, HIGH);
      delay(50);
      #endif

      SPI.begin();    

      #if defined(BOARD_UNO_Q)
        delay(100); 
      #endif

      spiGeinitialiseerd = true;
    }
  }
}
*/

bool IsGedeeldeBusGeinitialiseerd(GedeeldeBusType busType) {
  if (busType == GedeeldeBusType::I2C) return i2cGeinitialiseerd;
  if (busType == GedeeldeBusType::SPI) return spiGeinitialiseerd;
  return false;
}

void ResettenGedeeldeBus() {
  i2cGeinitialiseerd = false;
  spiGeinitialiseerd = false;
}

uint8_t volgendeGedeeldeBusId = 0;
GedeeldeBusNode Native(nullptr, { nullptr, 0, nullptr, 0 }, GedeeldeBusComponent::ROOT, HardwareResourceToegang::GEDEELD);

#if ADC_BACKEND == ADC_BACKEND_NATIVE
struct ADC_NATIVE ADC_NATIVE(
  &Native,
  GedeeldeBusComponent::ADC_NATIVE,
  ADC_PIN_SENSOR_1,
  ADC_PIN_SENSOR_2,
  ADC_PIN_SENSOR_3,
  ADC_PIN_SENSOR_4,
  HardwareResourceToegang::GEDEELD
);
#elif ADC_BACKEND == ADC_BACKEND_ADS1115
struct ADC_ADS1115 ADC_ADS1115(
  &Native,
  GedeeldeBusComponent::ADC_ADS1115,
  I2C_ADDRESS_ADS1115,
  HardwareResourcePin::NP_SDA,
  HardwareResourcePin::NP_SCL,
  HardwareResourcePin::NONE,
  HardwareResourceToegang::GEDEELD
);
#endif

// Vertaalt een native HardwareResourcePin naar het echte, board-specifieke Arduino-pinnummer. Enkel geldig voor native boardresources (D0-D13, A0-A5,
// SDA, SCL, MISO, MOSI, SCK, SS). Extenderpins (ExtenderPins per Extender) gaan hier NOOIT doorheen - die blijven resources van hun eigen Extender.
uint8_t NativeArduinoPinVan(HardwareResourcePin resource, uint8_t pinOverride) {
  uint8_t pin = static_cast<uint8_t>(HardwareResourcePin::NONE);

  switch (resource) {
    case HardwareResourcePin::NP_D0:     pin = ARDUINO_UNO_SHIELD_PIN_D0;   break;
    case HardwareResourcePin::NP_D1:     pin = ARDUINO_UNO_SHIELD_PIN_D1;   break;
    case HardwareResourcePin::NP_D2:     pin = ARDUINO_UNO_SHIELD_PIN_D2;   break;
    case HardwareResourcePin::NP_D3:     pin = ARDUINO_UNO_SHIELD_PIN_D3;   break;
    case HardwareResourcePin::NP_D4:     pin = ARDUINO_UNO_SHIELD_PIN_D4;   break;
    case HardwareResourcePin::NP_D5:     pin = ARDUINO_UNO_SHIELD_PIN_D5;   break;
    case HardwareResourcePin::NP_D6:     pin = ARDUINO_UNO_SHIELD_PIN_D6;   break;
    case HardwareResourcePin::NP_D7:     pin = ARDUINO_UNO_SHIELD_PIN_D7;   break;
    case HardwareResourcePin::NP_D8:     pin = ARDUINO_UNO_SHIELD_PIN_D8;   break;
    case HardwareResourcePin::NP_D9:     pin = ARDUINO_UNO_SHIELD_PIN_D9;   break;
    case HardwareResourcePin::NP_D10:    pin = ARDUINO_UNO_SHIELD_PIN_D10;  break;
    case HardwareResourcePin::NP_D11:    pin = ARDUINO_UNO_SHIELD_PIN_D11;  break;
    case HardwareResourcePin::NP_D12:    pin = ARDUINO_UNO_SHIELD_PIN_D12;  break;
    case HardwareResourcePin::NP_D13:    pin = ARDUINO_UNO_SHIELD_PIN_D13;  break;

    case HardwareResourcePin::NP_A0:     pin = ARDUINO_UNO_SHIELD_PIN_A0;   break;
    case HardwareResourcePin::NP_A1:     pin = ARDUINO_UNO_SHIELD_PIN_A1;   break;
    case HardwareResourcePin::NP_A2:     pin = ARDUINO_UNO_SHIELD_PIN_A2;   break;
    case HardwareResourcePin::NP_A3:     pin = ARDUINO_UNO_SHIELD_PIN_A3;   break;
#ifdef ARDUINO_UNO_SHIELD_PIN_A4
    case HardwareResourcePin::NP_A4:     pin = ARDUINO_UNO_SHIELD_PIN_A4;   break;
#endif
#ifdef ARDUINO_UNO_SHIELD_PIN_A5
    case HardwareResourcePin::NP_A5:     pin = ARDUINO_UNO_SHIELD_PIN_A5;   break;
#endif

    case HardwareResourcePin::NP_SDA:    pin = ARDUINO_UNO_SHIELD_PIN_SDA;  break;
    case HardwareResourcePin::NP_SCL:    pin = ARDUINO_UNO_SHIELD_PIN_SCL;  break;
    case HardwareResourcePin::NP_MISO:   pin = ARDUINO_UNO_SHIELD_PIN_MISO; break;
    case HardwareResourcePin::NP_MOSI:   pin = ARDUINO_UNO_SHIELD_PIN_MOSI; break;
    case HardwareResourcePin::NP_SCK:    pin = ARDUINO_UNO_SHIELD_PIN_SCK;  break;
    case HardwareResourcePin::NP_SS:     pin = ARDUINO_UNO_SHIELD_PIN_SS;   break;

    case HardwareResourcePin::CUSTOM:    pin = pinOverride;                 break;
    case HardwareResourcePin::NONE:      break;
  }

  return pin;
}

HardwareResourcePin ArduinoUnoShieldPinOmzettenNaarHardwareResourcePin(uint8_t pin) {
  if (pin == ARDUINO_UNO_SHIELD_PIN_D0)  return HardwareResourcePin::NP_D0;
  if (pin == ARDUINO_UNO_SHIELD_PIN_D1)  return HardwareResourcePin::NP_D1;
  if (pin == ARDUINO_UNO_SHIELD_PIN_D2)  return HardwareResourcePin::NP_D2;
  if (pin == ARDUINO_UNO_SHIELD_PIN_D3)  return HardwareResourcePin::NP_D3;
  if (pin == ARDUINO_UNO_SHIELD_PIN_D4)  return HardwareResourcePin::NP_D4;
  if (pin == ARDUINO_UNO_SHIELD_PIN_D5)  return HardwareResourcePin::NP_D5;
  if (pin == ARDUINO_UNO_SHIELD_PIN_D6)  return HardwareResourcePin::NP_D6;
  if (pin == ARDUINO_UNO_SHIELD_PIN_D7)  return HardwareResourcePin::NP_D7;
  if (pin == ARDUINO_UNO_SHIELD_PIN_D8)  return HardwareResourcePin::NP_D8;
  if (pin == ARDUINO_UNO_SHIELD_PIN_D9)  return HardwareResourcePin::NP_D9;
  if (pin == ARDUINO_UNO_SHIELD_PIN_D10) return HardwareResourcePin::NP_D10;
  if (pin == ARDUINO_UNO_SHIELD_PIN_D11) return HardwareResourcePin::NP_D11;
  if (pin == ARDUINO_UNO_SHIELD_PIN_D12) return HardwareResourcePin::NP_D12;
  if (pin == ARDUINO_UNO_SHIELD_PIN_D13) return HardwareResourcePin::NP_D13;

  if (pin == ARDUINO_UNO_SHIELD_PIN_A0)  return HardwareResourcePin::NP_A0;
  if (pin == ARDUINO_UNO_SHIELD_PIN_A1)  return HardwareResourcePin::NP_A1;
  if (pin == ARDUINO_UNO_SHIELD_PIN_A2)  return HardwareResourcePin::NP_A2;
  if (pin == ARDUINO_UNO_SHIELD_PIN_A3)  return HardwareResourcePin::NP_A3;
#ifdef ARDUINO_UNO_SHIELD_PIN_A4
  if (pin == ARDUINO_UNO_SHIELD_PIN_A4)  return HardwareResourcePin::NP_A4;
#endif
#ifdef ARDUINO_UNO_SHIELD_PIN_A5
  if (pin == ARDUINO_UNO_SHIELD_PIN_A5)  return HardwareResourcePin::NP_A5;
#endif
  return HardwareResourcePin::NONE;
}

// ============================================================================
// Naam per component. Regel: INPUT/INPUT_*, SCREEN/*_SCREEN, SERIAL_OUTPUT -> volledig.
// Al de rest -> het stuk na de laatste underscore (Extenders, Sensor).
// ============================================================================
static const char* GedeeldeBusComponentRuweNaam(GedeeldeBusComponent c) {
  switch (c) {
    #define X(naam, waarde) case GedeeldeBusComponent::naam: return #naam;
    GEDEELDE_BUS_ELKE_COMPONENT(X)
    #undef X
    default: return "?";
  }
}

static const char* GedeeldeBusComponentNaam(GedeeldeBusComponent c) {
  const char* n = GedeeldeBusComponentRuweNaam(c);
  size_t len = strlen(n);
  bool isInput  = (strncmp(n, "INPUT", 5) == 0) && (len == 5 || n[5] == '_');
  bool isScreen = (len >= 6 && strcmp(n + len - 6, "SCREEN") == 0);
  bool isSerial = (strcmp(n, "SERIAL_OUTPUT") == 0);
  if (isInput || isScreen || isSerial) return n;
  const char* laatste = strrchr(n, '_');
  return laatste ? laatste + 1 : n;
}

// ============================================================================
// Cyclus-gebonden, gekoppelde lijst van gevonden GB-conflicten, tijdelijk werkgeheugen van precies één GedeeldeBusNewComponent(...)-cyclus. Kop-pointer hier,
// niet in de header, want dat geeft per vertaaleenheid een aparte kopie. Geen array, geen vast maximum.
// ============================================================================
struct GedeeldeBusTijdelijkConflict {
  const char* code;
  GedeeldeBusComponent A;
  GedeeldeBusComponent B;
  uint8_t waarde;
  bool isAdres;
  bool native;
  GedeeldeBusTijdelijkConflict* volgende;
};

static GedeeldeBusTijdelijkConflict* GedeeldeBusHuidigeCyclusConflicten = nullptr;

static void GedeeldeBusAanmakenTijdelijkConflict(const char* code, GedeeldeBusComponent A, GedeeldeBusComponent B, uint8_t waarde, bool isAdres, bool native = false) {
  GedeeldeBusHuidigeCyclusConflicten = new GedeeldeBusTijdelijkConflict{ code, A, B, waarde, isAdres, native, GedeeldeBusHuidigeCyclusConflicten };
}

void GedeeldeBusNode::HardwareResourceClaimsBetweenTwoNodes(GedeeldeBusNode* a, const GedeeldeBusNode* b) {
  if (a->firstExtenderNode() != b->firstExtenderNode()) return;
  const bool native = (a->firstExtenderNode() == nullptr);
  const uint8_t geen = static_cast<uint8_t>(HardwareResourcePin::NONE);

  for (uint8_t x = 0; x < a->AantalClaims(); x++) {
    bool exclusiefA = false; HardwareResourceIdentiteit idA = HardwareResourceIdentiteit::GEEN;
    const uint8_t waardeA = a->ClaimWaarde(x, exclusiefA, idA);
    if (waardeA == geen) continue;

    for (uint8_t y = 0; y < b->AantalClaims(); y++) {
      bool exclusiefB = false; HardwareResourceIdentiteit idB = HardwareResourceIdentiteit::GEEN;
      const uint8_t waardeB = b->ClaimWaarde(y, exclusiefB, idB);
      if (waardeB == geen) continue;
      if (IsAdres(waardeA) != IsAdres(waardeB)) continue;

      if (IsAdres(waardeA)) {
        if (exclusiefA && exclusiefB && waardeA == waardeB) {
          GedeeldeBusAanmakenTijdelijkConflict(_FATAL_GB102, a->component, b->component, waardeA, true);
          a->conflictGevonden = true;
        }
        
        continue;
      }

      const bool zelfdePin = native ? NativeArduinoPinVan(static_cast<HardwareResourcePin>(waardeA)) == NativeArduinoPinVan(static_cast<HardwareResourcePin>(waardeB)) : waardeA == waardeB;
      if (!zelfdePin) continue;

      const uint8_t pinVoorMelding = waardeA;

      if (exclusiefA || exclusiefB) {
        GedeeldeBusAanmakenTijdelijkConflict(_FATAL_GB101, a->component, b->component, pinVoorMelding, false, native);
        a->conflictGevonden = true;
      } else if (idA != idB) {
        GedeeldeBusAanmakenTijdelijkConflict(_FATAL_GB103, a->component, b->component, pinVoorMelding, false, native);
        a->conflictGevonden = true;
      }
    }
  }
}

bool HardwareResourceTypeI2C::controleren() {
  if (gecontroleerd) return true;

  // Moet vóór de algemene vergelijking gebeuren, op het rauwe adres: eenmaal opgeslagen als adres | 0x80, 
  // zijn een ongeldig adres (bv. 0x89) en een geldig adres (bv. 0x09) niet meer te onderscheiden (0x89 | 0x80 == 0x09 | 0x80 == 0x89). 
  // Een ongeldig adres mag daarom nooit meedoen aan de algemene botsingsvergelijking; dat zou een vals GB102 tegen een volledig ongerelateerd, geldig adres kunnen opleveren.
  // Geldig normaal 7-bit I2C-device-adresbereik: 0x08 t/m 0x77. 0x00 t/m 0x07 en 0x78 t/m 0xFF zijn niet geldig als normaal 7-bit device-adres.
  if (adres < 0x08 || adres > 0x77) {
    GedeeldeBusAanmakenTijdelijkConflict(_FATAL_GB110, component, component, adres, true);
    exclusief_[0] = static_cast<uint8_t>(HardwareResourcePin::NONE);
    GedeeldeBusNode::controleren();
    exclusief_[0] = static_cast<uint8_t>(adres | 0x80);
    gecontroleerd = false;
    conflictGevonden = true;
    return false;
  }

  return GedeeldeBusNode::controleren();
}

// Zoekt Screen in de boom, nodig omdat de globale ::Screen tijdens GedeeldeBusNewComponent<Screen>() zelf nog nullptr is, want de toewijzing gebeurt pas na de return, bij de aanroeper.
static struct Screen* GedeeldeBusVindHardwareResourceComponentScreen(const GedeeldeBusNode* vanaf) {
  const GedeeldeBusNode* wortel = vanaf;
  while (wortel->parent != nullptr) wortel = wortel->parent;

  for (const GedeeldeBusNode* n = wortel; n != nullptr; n = GedeeldeBusNode::VolgendeInBoom(n, wortel)) {
    if (n->component == GedeeldeBusComponent::GC_SCREEN) return (struct Screen*)n;
  }

  return nullptr;
}

bool GedeeldeBusNode::aanmelden() {
#ifdef TRACE
  if (GA_SERIAL) {
    GA_SERIAL.print("TRACE: GedeeldeBusNode::aanmelden(): component=0x");
    GA_SERIAL.println(static_cast<uint8_t>(component), HEX);
  }
#endif
  if (aangemeld) return true;

  // Structurele wacht: een GEDEELD-claim zonder geldige identiteit is een fout in het component zelf.
  // Blijft stil, geen melding, geen record, want het component wordt sowieso nooit gekoppeld.
  if (aangemeldePinnen.aantalGedeeld > 0) {
    bool geldig = (gedeeldeIdentiteiten != nullptr);
    if (geldig) {
      for (uint8_t i = 0; i < aangemeldePinnen.aantalGedeeld; i++) {
        if (gedeeldeIdentiteiten[i] == HardwareResourceIdentiteit::GEEN) { geldig = false; break; }
      }
    }

    if (!geldig) return false;
  }

  if (parent != nullptr && !parent->aanmelden()) return false;

  if (parent != nullptr) {
    nextChild = parent->firstChild;
    if (nextChild != nullptr) nextChild->prevChild = this;
    parent->firstChild = this;
  }

  aangemeld = true;
#ifdef DEBUG
  if (GA_SERIAL) {
    GA_DEBUG_PRINT("DEBUG: aanmelden OK component=0x");
    GA_DEBUG_PRINTLN2(static_cast<uint8_t>(component), HEX);
  }
#endif
  return true;
}

bool GedeeldeBusNode::controleren() {
#ifdef TRACE
  if (GA_SERIAL) {
    GA_SERIAL.print("TRACE: GedeeldeBusNode::controleren(): component=0x");
    GA_SERIAL.println(static_cast<uint8_t>(component), HEX);
  }
#endif
  if (gecontroleerd) return true;
  if (!aangemeld) return false;
  if (parent != nullptr && !parent->controleren()) return false;

  conflictGevonden = false;
  const uint8_t geen = static_cast<uint8_t>(HardwareResourcePin::NONE);
  const bool native = (firstExtenderNode() == nullptr);

  for (uint8_t x = 0; x < AantalClaims(); x++) {
    bool exclusiefA = false; HardwareResourceIdentiteit idA = HardwareResourceIdentiteit::GEEN;
    const uint8_t waardeA = ClaimWaarde(x, exclusiefA, idA);
    if (waardeA == geen) continue;

    if (IsAdres(waardeA)) continue;

    for (uint8_t y = x + 1; y < AantalClaims(); y++) {
      bool exclusiefB = false; HardwareResourceIdentiteit idB = HardwareResourceIdentiteit::GEEN;
      const uint8_t waardeB = ClaimWaarde(y, exclusiefB, idB);
      if (waardeB == geen || IsAdres(waardeB)) continue;

      const bool zelfdePin = native ? NativeArduinoPinVan(static_cast<HardwareResourcePin>(waardeA)) == NativeArduinoPinVan(static_cast<HardwareResourcePin>(waardeB)) : waardeA == waardeB;
      if (!zelfdePin) continue;

      if (exclusiefA && exclusiefB) {
        conflictGevonden = true;
        GedeeldeBusAanmakenTijdelijkConflict(_FATAL_GB105, component, component, waardeA, false, native);
      } else if (exclusiefA || exclusiefB) {
        conflictGevonden = true;
        GedeeldeBusAanmakenTijdelijkConflict(_FATAL_GB106, component, component, waardeA, false, native);
      } else if (idA != idB) {
        conflictGevonden = true;
        GedeeldeBusAanmakenTijdelijkConflict(_FATAL_GB107, component, component, waardeA, false, native);
      }
    }
  }

  if (parent != nullptr && parent == firstExtenderNode()) {
    const uint8_t code = static_cast<uint8_t>(component);

    if (!(code >= 0x20 && code <= 0x7F)) {
      for (uint8_t i = 0; i < AantalClaims(); i++) {
        bool exclusief = false; HardwareResourceIdentiteit identiteit = HardwareResourceIdentiteit::GEEN;
        const uint8_t waarde = ClaimWaarde(i, exclusief, identiteit);
        if (waarde == geen || IsAdres(waarde)) continue;

        bool geldig = true;

        switch (parent->component) {
#if EXTENDER_DS2482_800_AANTAL > 0
          case GedeeldeBusComponent::ONE_WIRE_DS2482v800: geldig = waarde <= static_cast<uint8_t>(ExtenderDS2482v800::ExtenderPins::EP_IO7); break;
#endif
#if EXTENDER_ADS1115_AANTAL > 0 || ADC_BACKEND == ADC_BACKEND_ADS1115
          case GedeeldeBusComponent::ADC_ADS1115: geldig = waarde <= static_cast<uint8_t>(ExtenderADS1115::ExtenderPins::EP_AIN3); break;
#endif
#if EXTENDER_ADS1158_AANTAL > 0
          case GedeeldeBusComponent::ADC_ADS1158: geldig = waarde <= static_cast<uint8_t>(ExtenderADS1158::ExtenderPins::EP_GPIO7); break;
#endif
#if EXTENDER_ADS7828_AANTAL > 0
          case GedeeldeBusComponent::ADC_ADS7828: geldig = waarde <= static_cast<uint8_t>(ExtenderADS7828::ExtenderPins::EP_CH7); break;
#endif
#if EXTENDER_ADS7953_AANTAL > 0
          case GedeeldeBusComponent::ADC_ADS7953: geldig = waarde <= static_cast<uint8_t>(ExtenderADS7953::ExtenderPins::EP_GPIO3); break;
#endif
#if EXTENDER_CD74HC4067_AANTAL > 0
          case GedeeldeBusComponent::ADS_MULTIPLEXER_CD74HC4067: geldig = waarde <= static_cast<uint8_t>(ExtenderCD74HC4067::ExtenderPins::EP_Y15); break;
#endif
#if EXTENDER_MCP23017_AANTAL > 0
          case GedeeldeBusComponent::DIGITAL_PINS_MCP23017: geldig = waarde <= static_cast<uint8_t>(ExtenderMCP23017::ExtenderPins::EP_GPB7); break;
#endif
#if EXTENDER_PCF8574_AANTAL > 0 || ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_PCF8574)
          case GedeeldeBusComponent::DIGITAL_PINS_PCF8574: geldig = waarde <= static_cast<uint8_t>(ExtenderPCF8574::ExtenderPins::EP_P7); break;
#endif
#if EXTENDER_PCF8575_AANTAL > 0
          case GedeeldeBusComponent::DIGITAL_PINS_PCF8575: geldig = waarde <= static_cast<uint8_t>(ExtenderPCF8575::ExtenderPins::EP_P17); break;
#endif
#if EXTENDER_TCA9548A_AANTAL > 0
          case GedeeldeBusComponent::I2C_MULTIPLEXER_TCA9548A: geldig = waarde <= static_cast<uint8_t>(ExtenderTCA9548A::ExtenderPins::EP_CH7); break;
#endif
#if EXTENDER_MAX14830_I2C_AANTAL > 0 || EXTENDER_MAX14830_SPI_AANTAL > 0
          case GedeeldeBusComponent::UART_MAX14830:
#if EXTENDER_MAX14830_I2C_AANTAL > 0
            geldig = waarde <= static_cast<uint8_t>(ExtenderMAX14830I2C::ExtenderPins::EP_GPIO15);
#else
            geldig = waarde <= static_cast<uint8_t>(ExtenderMAX14830SPI::ExtenderPins::EP_GPIO15);
#endif
            break;
#endif
#if EXTENDER_SC16IS752_I2C_AANTAL > 0 || EXTENDER_SC16IS752_SPI_AANTAL > 0
          case GedeeldeBusComponent::UART_SC16IS752:
#if EXTENDER_SC16IS752_I2C_AANTAL > 0
            geldig = waarde <= static_cast<uint8_t>(ExtenderSC16IS752I2C::ExtenderPins::EP_GPIO7);
#else
            geldig = waarde <= static_cast<uint8_t>(ExtenderSC16IS752SPI::ExtenderPins::EP_GPIO7);
#endif
            break;
#endif
          default: break;
        }

        if (!geldig) {
          conflictGevonden = true;
          GedeeldeBusAanmakenTijdelijkConflict(_FATAL_GB111, component, parent->component, waarde, false);
        }
      }
    }
  }

  if (parent != nullptr && parent->Extender == HardwareResourceToegang::EXCLUSIEF) {
    for (GedeeldeBusNode* sameParent = parent->firstChild; sameParent != nullptr; sameParent = sameParent->nextChild) {
      if (sameParent != this && sameParent->gecontroleerd) {
        conflictGevonden = true;
        GedeeldeBusAanmakenTijdelijkConflict(_FATAL_GB104, sameParent->component, this->component, 0, false);
      }
    }
  }

  const GedeeldeBusNode* wortel = this;
  while (wortel->parent != nullptr) wortel = wortel->parent;

  // Vergelijkt enkel met wat al eerder, in dezelfde vaste volgorde, succesvol gecontroleerd is.
  // Een zelf al gefaalde node telt nooit mee als bezetter en wordt hier overgeslagen.
  // Meldt elk gevonden conflict apart (geen stop bij de eerste), dus meerdere onafhankelijke botsingen van dezelfde node worden allemaal gemeld.
  for (const GedeeldeBusNode* n = wortel; n != nullptr; n = VolgendeInBoom(n, wortel)) {
    if (n == this || !n->gecontroleerd) continue;
    if (IsVoorouderVan(n) || n->IsVoorouderVan(this)) continue;

    HardwareResourceClaimsBetweenTwoNodes(this, n);
  }

  if (conflictGevonden) {
#ifdef DEBUG
    if (GA_SERIAL) {
      GA_DEBUG_PRINT("DEBUG: controleren conflict component=0x");
      GA_DEBUG_PRINTLN2(static_cast<uint8_t>(component), HEX);
    }
#endif
    return false;
  }

  gecontroleerd = true;
#ifdef DEBUG
  if (GA_SERIAL) {
    GA_DEBUG_PRINT("DEBUG: controleren OK component=0x");
    GA_DEBUG_PRINTLN2(static_cast<uint8_t>(component), HEX);
  }
#endif
  return true;
}

void GedeeldeBusPrintEnVerwijderTijdelijkeConflicten(GedeeldeBusNode* vanaf) {
  if (GedeeldeBusHuidigeCyclusConflicten == nullptr) return;

  struct Screen* scherm = (::Screen != nullptr) ? ::Screen : GedeeldeBusVindHardwareResourceComponentScreen(vanaf);
  struct Screen* vorige = ::Screen;

  if (scherm != nullptr) ::Screen = scherm;

  while (GedeeldeBusHuidigeCyclusConflicten != nullptr) {
    GedeeldeBusTijdelijkConflict* r = GedeeldeBusHuidigeCyclusConflicten;
    const bool heeftResource = strcmp(r->code, _FATAL_GB104) != 0;
    String regel4 = "";

    if (heeftResource) {
      if (r->isAdres) {
        String hexWaarde = String(strcmp(r->code, _FATAL_GB110) == 0 ? r->waarde : (r->waarde & 0x7F), HEX);
        if (hexWaarde.length() < 2) hexWaarde = "0" + hexWaarde;
        regel4 = String("I2C-ADRES: 0x") + hexWaarde;
      } else if (r->native) {
        regel4 = "PIN: ";

        switch (static_cast<HardwareResourcePin>(r->waarde)) {
          case HardwareResourcePin::NP_D0:   regel4 += "D0";   break;
          case HardwareResourcePin::NP_D1:   regel4 += "D1";   break;
          case HardwareResourcePin::NP_D2:   regel4 += "D2";   break;
          case HardwareResourcePin::NP_D3:   regel4 += "D3";   break;
          case HardwareResourcePin::NP_D4:   regel4 += "D4";   break;
          case HardwareResourcePin::NP_D5:   regel4 += "D5";   break;
          case HardwareResourcePin::NP_D6:   regel4 += "D6";   break;
          case HardwareResourcePin::NP_D7:   regel4 += "D7";   break;
          case HardwareResourcePin::NP_D8:   regel4 += "D8";   break;
          case HardwareResourcePin::NP_D9:   regel4 += "D9";   break;
          case HardwareResourcePin::NP_D10:  regel4 += "D10";  break;
          case HardwareResourcePin::NP_D11:  regel4 += "D11";  break;
          case HardwareResourcePin::NP_D12:  regel4 += "D12";  break;
          case HardwareResourcePin::NP_D13:  regel4 += "D13";  break;

          case HardwareResourcePin::NP_A0:   regel4 += "A0";   break;
          case HardwareResourcePin::NP_A1:   regel4 += "A1";   break;
          case HardwareResourcePin::NP_A2:   regel4 += "A2";   break;
          case HardwareResourcePin::NP_A3:   regel4 += "A3";   break;
          case HardwareResourcePin::NP_A4:   regel4 += "A4";   break;
          case HardwareResourcePin::NP_A5:   regel4 += "A5";   break;

          case HardwareResourcePin::NP_SDA:  regel4 += "SDA";  break;
          case HardwareResourcePin::NP_SCL:  regel4 += "SCL";  break;

          case HardwareResourcePin::NP_MISO: regel4 += "MISO"; break;
          case HardwareResourcePin::NP_MOSI: regel4 += "MOSI"; break;
          case HardwareResourcePin::NP_SCK:  regel4 += "SCK";  break;
          case HardwareResourcePin::NP_SS:   regel4 += "SS";   break;

          case HardwareResourcePin::CUSTOM: regel4 += "CUSTOM"; break;
          case HardwareResourcePin::NONE:   regel4 += "NONE";   break;
        }
        
        regel4 += " ("; regel4 += String(NativeArduinoPinVan(static_cast<HardwareResourcePin>(r->waarde))); regel4 += ")";
      } else regel4 = String("PIN: ") + String(r->waarde);
    }

    if (scherm != nullptr) {
#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
      scherm->Print(ScreenData::TYPE_FATAL, r->code, GedeeldeBusComponentNaam(r->A), FATAL_LEESTIJD_MS, "", GedeeldeBusComponentNaam(r->B), regel4, FATAL_LEESTIJD_MS);
#else
      scherm->Print(r->code, GedeeldeBusComponentNaam(r->A), FATAL_LEESTIJD_MS, "", GedeeldeBusComponentNaam(r->B), regel4, FATAL_LEESTIJD_MS);
#endif
      // Print() wacht enkel TUSSEN regel 1-2 en regel 3-4, niet erna. Zonder deze wacht zou de volgende melding in deze lus pagina 2 van de huidige meteen overschrijven, onleesbaar.
      delay(FATAL_LEESTIJD_MS);
    } else {
      ::GA_SERIAL.println(r->code);
      ::GA_SERIAL.println(GedeeldeBusComponentNaam(r->A));
      ::GA_SERIAL.println(GedeeldeBusComponentNaam(r->B));
      if (regel4 != "") ::GA_SERIAL.println(regel4);
    }

    GedeeldeBusHuidigeCyclusConflicten = r->volgende;
    delete r;
  }

  ::Screen = vorige;
}

