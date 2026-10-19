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

// Runtime-herstel (D042): true tijdens een herstelpoging, zodat FoutDiagnoseI2C() GB401 tot GB406 meldt in plaats van GB301 tot GB306.
static bool herstelTijdensRuntimeBezig = false;
// De oorzaak van de laatst mislukte herstelpoging (GB401, met DEBUG GB402 tot GB406), ingevuld door FoutDiagnoseI2C().
static const char* oorzaakHerstelTijdensRuntime = nullptr;
static void MeldingTijdensRuntimeTonen(uint8_t foutniveau, const char* code, HardwareResourceTypeI2C* component, uint8_t kanaal = 0);

void InitialiserenGedeeldeBus(GedeeldeBusType busType) {
#ifdef TRACE
  if (GA_SERIAL) {
    GA_SERIAL.print("TRACE: InitialiserenGedeeldeBus(): ");
    GA_SERIAL.println(busType == GedeeldeBusType::I2C ? "I2C" : "SPI");
  }
#endif

  if (busType == GedeeldeBusType::I2C) {
    if (!i2cGeinitialiseerd) {
      // Enkel op ESP32 geeft Wire.begin() een bool terug; op de andere cores is Wire.begin() void.
      #if BOARD_VERSION == BOARD_ESP32S3_ARDI32 || BOARD_VERSION == BOARD_ESP32S3_DEV
        i2cGeinitialiseerd = Wire.begin(ARDUINO_UNO_SHIELD_PIN_SDA, ARDUINO_UNO_SHIELD_PIN_SCL);
      #elif defined(ARDUINO_ARCH_ESP32)
        i2cGeinitialiseerd = Wire.begin();
      #else
        Wire.begin();
        i2cGeinitialiseerd = true;
      #endif
      // Runtime-herstel (D042): een transactie wacht nooit langer dan I2C_TIMEOUT_PER_TRANSACTIE_US; true = de I2C-hardware resetten na een timeout.
      // Nagekeken: AVR (Wire.h, setWireTimeout) en UNO R4 (Wire.h, setWireTimeout). Andere cores houden hun eigen standaard.
      #if defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_RENESAS)
        Wire.setWireTimeout(I2C_TIMEOUT_PER_TRANSACTIE_US, true);
      #endif

#ifdef DEBUG
      if (i2cGeinitialiseerd && GA_SERIAL) GA_DEBUG_PRINTLN("DEBUG: I2C geinitialiseerd");
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

#if GEDEELDE_BUS_MIXED_PINS
GedeeldeBusNode* GedeeldeBusFindI2CExtender(GedeeldeBusComponent extender, uint8_t extenderAdres) {
  for (const GedeeldeBusNode* n = &Native; n != nullptr; n = GedeeldeBusNode::VolgendeInBoom(n, &Native)) {
    // Alleen de Extenders met digitale pinnen voor dit gebruik: PCF8574, PCF8575 en MCP23017. Die zijn alle drie I2C,
    // dus de omzetting naar HardwareResourceTypeI2C hieronder is alleen voor deze drie geldig. Elk ander component: overslaan.
    const bool toegelaten = n->component == GedeeldeBusComponent::DIGITAL_PINS_PCF8574 ||
                            n->component == GedeeldeBusComponent::DIGITAL_PINS_PCF8575 ||
                            n->component == GedeeldeBusComponent::DIGITAL_PINS_MCP23017;
    if (n->component != extender || !toegelaten) continue;
    const HardwareResourceTypeI2C* i2c = static_cast<const HardwareResourceTypeI2C*>(n);
    if (i2c->adres == extenderAdres) return const_cast<GedeeldeBusNode*>(n);
  }
  return nullptr;
}

bool HardwareResourceTypeSPI::ConnectCStoExtender(GedeeldeBusComponent extender, uint8_t extenderAdres) {
  extenderCS[0] = GedeeldeBusFindI2CExtender(extender, extenderAdres);
  ownerClaim = extenderCS;
  if (extenderCS[0] == nullptr) np_CS = HardwareResourcePin::NONE;   // controleren() meldt dan GB108
  exclusief_[0] = static_cast<uint8_t>(np_CS);
  return extenderCS[0] != nullptr;
}
#endif

#if ADC_BACKEND == ADC_BACKEND_NATIVE
struct ADC_NATIVE ADC_NATIVE(
  &Native,
  GedeeldeBusComponent::ADC_NATIVE,
  ADC_PIN_SENSOR_1,
  ADC_PIN_SENSOR_2,
#if AANTAL_SENSOREN_AANWEZIG == 4
  ADC_PIN_SENSOR_3,
  ADC_PIN_SENSOR_4,
#else
  HardwareResourcePin::NONE,   // enkel de pinnen van de aanwezige sensoren claimen
  HardwareResourcePin::NONE,
#endif
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
#if !GEDEELDE_BUS_MIXED_PINS
  if (a->firstExtenderNode() != b->firstExtenderNode()) return;
  const bool native = (a->firstExtenderNode() == nullptr);
#endif
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
#if GEDEELDE_BUS_MIXED_PINS
      // Per claim het gebied: alleen claims in hetzelfde gebied (het board of dezelfde Extender) kunnen botsen.
      const GedeeldeBusNode* gebied = a->GetOwnerClaim(x);
      if (gebied != b->GetOwnerClaim(y)) continue;
      const bool native = (gebied == nullptr);
#endif

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
    gecontroleerd = false;
    conflictGevonden = true;
    return false;
  }

  return GedeeldeBusNode::controleren();
}

bool GedeeldeBusNode::activeren() {
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
#if EXTENDER_TCA9548A_AANTAL > 0
  // Achter een TCA9548A: eerst het kanaal van dit component openen, na Activeren() alle kanalen sluiten.
  if (parent != nullptr && parent->component == GedeeldeBusComponent::I2C_MULTIPLEXER_TCA9548A) static_cast<ExtenderTCA9548A*>(parent)->SelectChannel(this);
#endif
  const bool gelukt = Activeren();
#if EXTENDER_TCA9548A_AANTAL > 0
  if (parent != nullptr && parent->component == GedeeldeBusComponent::I2C_MULTIPLEXER_TCA9548A) static_cast<ExtenderTCA9548A*>(parent)->disableAllChannels();
#endif
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

bool HardwareResourceTypeI2C::inpluggen() {
  if (ingeplugd) return true;
  if (!GedeeldeBusNode::inpluggen()) return false;
  InitialiserenGedeeldeBus(GedeeldeBusType::I2C);

  // Kan enkel op ESP32 mislukken: daar geeft Wire.begin() een bool terug.
  if (!IsGedeeldeBusGeinitialiseerd(GedeeldeBusType::I2C)) {
#ifdef DEBUG
    GedeeldeBusAanmakenTijdelijkConflict(_CRITICAL_GB202, component, component, adres, true);
#else
    GedeeldeBusAanmakenTijdelijkConflict(_CRITICAL_GB201, component, component, adres, true);
#endif
    ingeplugd = false;
    return false;
  }

  return true;
}

bool HardwareResourceTypeI2C::FoutDiagnoseI2C(bool gelukt, int fout, bool terugGelezen) {
  if (gelukt && terugGelezen) return true;

  // Enkel bij een mislukte begin() het adres zelf testen, om de oorzaak te vinden.
  // endTransmission(): 0 = antwoord, 2 = geen antwoord op het adres, 4 = andere busfout, 5 = timeout.
#if defined(DEBUG) || defined(TRACE)
  uint8_t i2cStatus = 0;
  if (!gelukt) {
    Wire.beginTransmission(adres);
    i2cStatus = Wire.endTransmission();
  }
#endif

#ifdef TRACE
  if (GA_SERIAL) {
    GA_SERIAL.print("TRACE: FoutDiagnoseI2C(): component=0x");
    GA_SERIAL.print(static_cast<uint8_t>(component), HEX);
    GA_SERIAL.print(" adres=0x");
    GA_SERIAL.print(adres, HEX);
    GA_SERIAL.print(" begin=");
    GA_SERIAL.print(gelukt ? "OK" : "FOUT");
    GA_SERIAL.print(" teruglezen=");
    GA_SERIAL.print(terugGelezen ? "OK" : "FOUT");
    GA_SERIAL.print(" endTransmission=");
    GA_SERIAL.print(i2cStatus);
    GA_SERIAL.print(" library fout=");
    GA_SERIAL.println(fout);
  }
#endif

#ifdef DEBUG
  // Tijdens een herstelpoging (runtime): alleen de oorzaak onthouden (GB402 tot GB406); ComponentBeschikbaarTijdensRuntime() meldt.
  if (herstelTijdensRuntimeBezig) {
    oorzaakHerstelTijdensRuntime = gelukt ? _GB406 : (i2cStatus == 2 ? _GB402 : ((i2cStatus == 4 || i2cStatus == 5) ? _GB405 : _GB403));
    return false;
  }
  if (gelukt) {
    GedeeldeBusAanmakenTijdelijkConflict(_CRITICAL_GB306, component, component, adres, true);
  } else if (i2cStatus == 2) {
    GedeeldeBusAanmakenTijdelijkConflict(_CRITICAL_GB302, component, component, adres, true);
  } else if (i2cStatus == 4 || i2cStatus == 5) {
    GedeeldeBusAanmakenTijdelijkConflict(_CRITICAL_GB305, component, component, adres, true);
  } else {
    GedeeldeBusAanmakenTijdelijkConflict(_CRITICAL_GB303, component, component, static_cast<uint8_t>(fout), false);
  }
#else
  (void)fout;
  // Tijdens een herstelpoging (runtime): ComponentBeschikbaarTijdensRuntime() meldt GB401.
  if (herstelTijdensRuntimeBezig) return false;
  GedeeldeBusAanmakenTijdelijkConflict(_CRITICAL_GB301, component, component, adres, true);
#endif

  return false;
}

bool HardwareResourceTypeSPI::controleren() {
  if (gecontroleerd) return true;

  // Zonder CS kan een SPI-component nooit geselecteerd worden: verplichte resource ontbreekt.
  if (np_CS == HardwareResourcePin::NONE) {
    GedeeldeBusAanmakenTijdelijkConflict(_FATAL_GB108, component, component, static_cast<uint8_t>(np_CS), false, true);
    GedeeldeBusNode::controleren();
    gecontroleerd = false;
    conflictGevonden = true;
    return false;
  }

  return GedeeldeBusNode::controleren();
}

bool HardwareResourceTypeSPI::FoutDiagnoseSPI(bool gelukt, int fout, bool terugGelezen) {
  if (gelukt && terugGelezen) return true;

#if GEDEELDE_BUS_MIXED_PINS
  const bool native = (GetOwnerClaim(0) == nullptr);   // claim 0 = CS
#else
  const bool native = (firstExtenderNode() == nullptr);
#endif

#ifdef TRACE
  if (GA_SERIAL) {
    GA_SERIAL.print("TRACE: FoutDiagnoseSPI(): component=0x");
    GA_SERIAL.print(static_cast<uint8_t>(component), HEX);
    GA_SERIAL.print(" CS=");
    GA_SERIAL.print(static_cast<uint8_t>(np_CS));
    GA_SERIAL.print(" begin=");
    GA_SERIAL.print(gelukt ? "OK" : "FOUT");
    GA_SERIAL.print(" teruglezen=");
    GA_SERIAL.print(terugGelezen ? "OK" : "FOUT");
    GA_SERIAL.print(" library fout=");
    GA_SERIAL.println(fout);
  }
#else
  (void)fout;
#endif

#ifdef DEBUG
  GedeeldeBusAanmakenTijdelijkConflict(_CRITICAL_GB304, component, component, static_cast<uint8_t>(np_CS), false, native);
#else
  GedeeldeBusAanmakenTijdelijkConflict(_CRITICAL_GB301, component, component, static_cast<uint8_t>(np_CS), false, native);
#endif

  return false;
}

bool PixelScreen::controleren() {
  if (gecontroleerd) return true;

  // Zonder DC kan het scherm geen commando's van data onderscheiden: verplichte resource ontbreekt.
  if (np_DC == HardwareResourcePin::NONE) {
    GedeeldeBusAanmakenTijdelijkConflict(_FATAL_GB108, component, component, static_cast<uint8_t>(np_DC), false, true);
    HardwareResourceTypeSPI::controleren();
    gecontroleerd = false;
    conflictGevonden = true;
    return false;
  }

  return HardwareResourceTypeSPI::controleren();
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

// ============================================================================
// Runtime-herstel (D042)
// Een los contact maakt een component niet meer voor altijd onbruikbaar: fouten tellen, foutniveau WARNING tot FATAL,
// vanaf het ingestelde foutniveau uitschakelen, met pauze opnieuw activeren(), bij succes herstelmelding GB407.
// Twee assen: de code (GB400 tot GB410) zegt WAT er gebeurde, het foutniveau HOE ERNSTIG het nu is.
// ============================================================================
static_assert(RUNTIME_WARNING_OPEENVOLGENDE_FOUTEN_NA_AANTAL >= 1 && RUNTIME_WARNING_OPEENVOLGENDE_FOUTEN_NA_AANTAL <= 255, "RUNTIME_WARNING_OPEENVOLGENDE_FOUTEN_NA_AANTAL moet 1 tot 255 zijn.");
static_assert(RUNTIME_ALERT_OPEENVOLGENDE_FOUTEN_NA_AANTAL >= 1 && RUNTIME_ALERT_OPEENVOLGENDE_FOUTEN_NA_AANTAL <= 255, "RUNTIME_ALERT_OPEENVOLGENDE_FOUTEN_NA_AANTAL moet 1 tot 255 zijn.");
static_assert(RUNTIME_FAULT_OPEENVOLGENDE_FOUTEN_NA_AANTAL >= 1 && RUNTIME_FAULT_OPEENVOLGENDE_FOUTEN_NA_AANTAL <= 255, "RUNTIME_FAULT_OPEENVOLGENDE_FOUTEN_NA_AANTAL moet 1 tot 255 zijn.");
static_assert(RUNTIME_CRITICAL_OPEENVOLGENDE_FOUTEN_NA_AANTAL >= 1 && RUNTIME_CRITICAL_OPEENVOLGENDE_FOUTEN_NA_AANTAL <= 255, "RUNTIME_CRITICAL_OPEENVOLGENDE_FOUTEN_NA_AANTAL moet 1 tot 255 zijn.");
static_assert(RUNTIME_FATAL_OPEENVOLGENDE_FOUTEN_NA_AANTAL >= 1 && RUNTIME_FATAL_OPEENVOLGENDE_FOUTEN_NA_AANTAL <= 255, "RUNTIME_FATAL_OPEENVOLGENDE_FOUTEN_NA_AANTAL moet 1 tot 255 zijn.");

constexpr DrempelsFoutniveauTijdensRuntime drempelsFoutniveauAlgemeen = {
  { RUNTIME_WARNING_OPEENVOLGENDE_FOUTEN_NA_AANTAL, RUNTIME_ALERT_OPEENVOLGENDE_FOUTEN_NA_AANTAL, RUNTIME_FAULT_OPEENVOLGENDE_FOUTEN_NA_AANTAL,
    RUNTIME_CRITICAL_OPEENVOLGENDE_FOUTEN_NA_AANTAL, RUNTIME_FATAL_OPEENVOLGENDE_FOUTEN_NA_AANTAL },
  { RUNTIME_WARNING_SINDS_EERSTE_FOUT_NA_MS, RUNTIME_ALERT_SINDS_EERSTE_FOUT_NA_MS, RUNTIME_FAULT_SINDS_EERSTE_FOUT_NA_MS,
    RUNTIME_CRITICAL_SINDS_EERSTE_FOUT_NA_MS, RUNTIME_FATAL_SINDS_EERSTE_FOUT_NA_MS },
  RUNTIME_HOOGST_TOEGELATEN_FOUTNIVEAU,
  RUNTIME_COMPONENT_UITSCHAKELEN_VANAF_FOUTNIVEAU
};
static_assert(DrempelsFoutniveauGeldig(drempelsFoutniveauAlgemeen), "RUNTIME_..._OPEENVOLGENDE_FOUTEN_NA_AANTAL en RUNTIME_..._SINDS_EERSTE_FOUT_NA_MS moeten oplopen van WARNING naar FATAL; RUNTIME_HOOGST_TOEGELATEN_FOUTNIVEAU en RUNTIME_COMPONENT_UITSCHAKELEN_VANAF_FOUTNIVEAU moeten ScreenData::TYPE_WARNING tot ScreenData::TYPE_FATAL zijn.");

// Foutniveau 1 tot 5 uit een ScreenData-waarde (TYPE_WARNING = 1 ... TYPE_FATAL = 5).
static uint8_t FoutniveauVanScreenData(ScreenData screenData) {
  return static_cast<uint8_t>(static_cast<uint8_t>(screenData) - static_cast<uint8_t>(ScreenData::TYPE_WARNING) + 1);
}

// Overrides per type (alle exemplaren) of per exemplaar (op I2C-adres). Alleen gecompileerd als de gebruiker de define zet.
// Volgorde: exemplaar, dan type, dan algemeen.
const DrempelsFoutniveauTijdensRuntime& DrempelsFoutniveauVoorComponent(const HardwareResourceTypeI2C* c) {
  (void)c;
#if defined(EXTENDER_ADS1115_1_RUNTIME_DREMPELS_FOUTNIVEAU)
  static constexpr DrempelsFoutniveauTijdensRuntime ads1115nr1 = EXTENDER_ADS1115_1_RUNTIME_DREMPELS_FOUTNIVEAU;
  static_assert(DrempelsFoutniveauGeldig(ads1115nr1), "EXTENDER_ADS1115_1_RUNTIME_DREMPELS_FOUTNIVEAU: aantallen en tijden moeten oplopen, foutniveaus ScreenData::TYPE_WARNING tot ScreenData::TYPE_FATAL.");
  if (c->component == GedeeldeBusComponent::ADC_ADS1115 && c->adres == I2C_ADDRESS_EXTENDER_ADS1115_1) return ads1115nr1;
#endif
#if defined(EXTENDER_ADS1115_2_RUNTIME_DREMPELS_FOUTNIVEAU)
  static constexpr DrempelsFoutniveauTijdensRuntime ads1115nr2 = EXTENDER_ADS1115_2_RUNTIME_DREMPELS_FOUTNIVEAU;
  static_assert(DrempelsFoutniveauGeldig(ads1115nr2), "EXTENDER_ADS1115_2_RUNTIME_DREMPELS_FOUTNIVEAU: aantallen en tijden moeten oplopen, foutniveaus ScreenData::TYPE_WARNING tot ScreenData::TYPE_FATAL.");
  if (c->component == GedeeldeBusComponent::ADC_ADS1115 && c->adres == I2C_ADDRESS_EXTENDER_ADS1115_2) return ads1115nr2;
#endif
#if defined(EXTENDER_ADS1115_RUNTIME_DREMPELS_FOUTNIVEAU)
  static constexpr DrempelsFoutniveauTijdensRuntime ads1115 = EXTENDER_ADS1115_RUNTIME_DREMPELS_FOUTNIVEAU;
  static_assert(DrempelsFoutniveauGeldig(ads1115), "EXTENDER_ADS1115_RUNTIME_DREMPELS_FOUTNIVEAU: aantallen en tijden moeten oplopen, foutniveaus ScreenData::TYPE_WARNING tot ScreenData::TYPE_FATAL.");
  if (c->component == GedeeldeBusComponent::ADC_ADS1115) return ads1115;
#endif
#if defined(EXTENDER_MCP23017_1_RUNTIME_DREMPELS_FOUTNIVEAU)
  static constexpr DrempelsFoutniveauTijdensRuntime mcp23017nr1 = EXTENDER_MCP23017_1_RUNTIME_DREMPELS_FOUTNIVEAU;
  static_assert(DrempelsFoutniveauGeldig(mcp23017nr1), "EXTENDER_MCP23017_1_RUNTIME_DREMPELS_FOUTNIVEAU: aantallen en tijden moeten oplopen, foutniveaus ScreenData::TYPE_WARNING tot ScreenData::TYPE_FATAL.");
  if (c->component == GedeeldeBusComponent::DIGITAL_PINS_MCP23017 && c->adres == I2C_ADDRESS_EXTENDER_MCP23017_1) return mcp23017nr1;
#endif
#if defined(EXTENDER_MCP23017_2_RUNTIME_DREMPELS_FOUTNIVEAU)
  static constexpr DrempelsFoutniveauTijdensRuntime mcp23017nr2 = EXTENDER_MCP23017_2_RUNTIME_DREMPELS_FOUTNIVEAU;
  static_assert(DrempelsFoutniveauGeldig(mcp23017nr2), "EXTENDER_MCP23017_2_RUNTIME_DREMPELS_FOUTNIVEAU: aantallen en tijden moeten oplopen, foutniveaus ScreenData::TYPE_WARNING tot ScreenData::TYPE_FATAL.");
  if (c->component == GedeeldeBusComponent::DIGITAL_PINS_MCP23017 && c->adres == I2C_ADDRESS_EXTENDER_MCP23017_2) return mcp23017nr2;
#endif
#if defined(EXTENDER_MCP23017_RUNTIME_DREMPELS_FOUTNIVEAU)
  static constexpr DrempelsFoutniveauTijdensRuntime mcp23017 = EXTENDER_MCP23017_RUNTIME_DREMPELS_FOUTNIVEAU;
  static_assert(DrempelsFoutniveauGeldig(mcp23017), "EXTENDER_MCP23017_RUNTIME_DREMPELS_FOUTNIVEAU: aantallen en tijden moeten oplopen, foutniveaus ScreenData::TYPE_WARNING tot ScreenData::TYPE_FATAL.");
  if (c->component == GedeeldeBusComponent::DIGITAL_PINS_MCP23017) return mcp23017;
#endif
#if defined(EXTENDER_PCF8574_1_RUNTIME_DREMPELS_FOUTNIVEAU)
  static constexpr DrempelsFoutniveauTijdensRuntime pcf8574nr1 = EXTENDER_PCF8574_1_RUNTIME_DREMPELS_FOUTNIVEAU;
  static_assert(DrempelsFoutniveauGeldig(pcf8574nr1), "EXTENDER_PCF8574_1_RUNTIME_DREMPELS_FOUTNIVEAU: aantallen en tijden moeten oplopen, foutniveaus ScreenData::TYPE_WARNING tot ScreenData::TYPE_FATAL.");
  if (c->component == GedeeldeBusComponent::DIGITAL_PINS_PCF8574 && c->adres == I2C_ADDRESS_EXTENDER_PCF8574_1) return pcf8574nr1;
#endif
#if defined(EXTENDER_PCF8574_2_RUNTIME_DREMPELS_FOUTNIVEAU)
  static constexpr DrempelsFoutniveauTijdensRuntime pcf8574nr2 = EXTENDER_PCF8574_2_RUNTIME_DREMPELS_FOUTNIVEAU;
  static_assert(DrempelsFoutniveauGeldig(pcf8574nr2), "EXTENDER_PCF8574_2_RUNTIME_DREMPELS_FOUTNIVEAU: aantallen en tijden moeten oplopen, foutniveaus ScreenData::TYPE_WARNING tot ScreenData::TYPE_FATAL.");
  if (c->component == GedeeldeBusComponent::DIGITAL_PINS_PCF8574 && c->adres == I2C_ADDRESS_EXTENDER_PCF8574_2) return pcf8574nr2;
#endif
#if defined(EXTENDER_PCF8574_RUNTIME_DREMPELS_FOUTNIVEAU)
  // Ook de PCF8574 van Input (zelfde chip).
  static constexpr DrempelsFoutniveauTijdensRuntime pcf8574 = EXTENDER_PCF8574_RUNTIME_DREMPELS_FOUTNIVEAU;
  static_assert(DrempelsFoutniveauGeldig(pcf8574), "EXTENDER_PCF8574_RUNTIME_DREMPELS_FOUTNIVEAU: aantallen en tijden moeten oplopen, foutniveaus ScreenData::TYPE_WARNING tot ScreenData::TYPE_FATAL.");
  if (c->component == GedeeldeBusComponent::DIGITAL_PINS_PCF8574 || c->component == GedeeldeBusComponent::INPUT_PCF8574) return pcf8574;
#endif
#if defined(EXTENDER_PCF8575_1_RUNTIME_DREMPELS_FOUTNIVEAU)
  static constexpr DrempelsFoutniveauTijdensRuntime pcf8575nr1 = EXTENDER_PCF8575_1_RUNTIME_DREMPELS_FOUTNIVEAU;
  static_assert(DrempelsFoutniveauGeldig(pcf8575nr1), "EXTENDER_PCF8575_1_RUNTIME_DREMPELS_FOUTNIVEAU: aantallen en tijden moeten oplopen, foutniveaus ScreenData::TYPE_WARNING tot ScreenData::TYPE_FATAL.");
  if (c->component == GedeeldeBusComponent::DIGITAL_PINS_PCF8575 && c->adres == I2C_ADDRESS_EXTENDER_PCF8575_1) return pcf8575nr1;
#endif
#if defined(EXTENDER_PCF8575_2_RUNTIME_DREMPELS_FOUTNIVEAU)
  static constexpr DrempelsFoutniveauTijdensRuntime pcf8575nr2 = EXTENDER_PCF8575_2_RUNTIME_DREMPELS_FOUTNIVEAU;
  static_assert(DrempelsFoutniveauGeldig(pcf8575nr2), "EXTENDER_PCF8575_2_RUNTIME_DREMPELS_FOUTNIVEAU: aantallen en tijden moeten oplopen, foutniveaus ScreenData::TYPE_WARNING tot ScreenData::TYPE_FATAL.");
  if (c->component == GedeeldeBusComponent::DIGITAL_PINS_PCF8575 && c->adres == I2C_ADDRESS_EXTENDER_PCF8575_2) return pcf8575nr2;
#endif
#if defined(EXTENDER_PCF8575_RUNTIME_DREMPELS_FOUTNIVEAU)
  static constexpr DrempelsFoutniveauTijdensRuntime pcf8575 = EXTENDER_PCF8575_RUNTIME_DREMPELS_FOUTNIVEAU;
  static_assert(DrempelsFoutniveauGeldig(pcf8575), "EXTENDER_PCF8575_RUNTIME_DREMPELS_FOUTNIVEAU: aantallen en tijden moeten oplopen, foutniveaus ScreenData::TYPE_WARNING tot ScreenData::TYPE_FATAL.");
  if (c->component == GedeeldeBusComponent::DIGITAL_PINS_PCF8575) return pcf8575;
#endif
#if defined(EXTENDER_TCA9548A_RUNTIME_DREMPELS_FOUTNIVEAU)
  static constexpr DrempelsFoutniveauTijdensRuntime tca9548a = EXTENDER_TCA9548A_RUNTIME_DREMPELS_FOUTNIVEAU;
  static_assert(DrempelsFoutniveauGeldig(tca9548a), "EXTENDER_TCA9548A_RUNTIME_DREMPELS_FOUTNIVEAU: aantallen en tijden moeten oplopen, foutniveaus ScreenData::TYPE_WARNING tot ScreenData::TYPE_FATAL.");
  if (c->component == GedeeldeBusComponent::I2C_MULTIPLEXER_TCA9548A) return tca9548a;
#endif
  return drempelsFoutniveauAlgemeen;
}

// Eén ladderstap. Geeft terug: 0 = geen verandering, 1 tot 5 = nieuw (hoger) foutniveau, 255 = hersteld.
static uint8_t FoutniveauTijdensRuntimeBerekenen(ToestandTijdensRuntime& toestand, bool gelukt, const DrempelsFoutniveauTijdensRuntime& drempels) {
  if (gelukt) {
    if (toestand.aantalOpeenvolgendeFouten == 0 && toestand.huidigFoutniveau == 0) return 0;   // snelste pad: geen storing
    const bool wasInStoring = toestand.huidigFoutniveau != 0;
    toestand = ToestandTijdensRuntime();
    return wasInStoring ? 255 : 0;
  }

  const unsigned long nu = millis();
  if (toestand.aantalOpeenvolgendeFouten == 0) toestand.tijdstipEersteFout = nu;
  if (toestand.aantalOpeenvolgendeFouten < 255) toestand.aantalOpeenvolgendeFouten++;
  const unsigned long duur = nu - toestand.tijdstipEersteFout;

  uint8_t nieuwFoutniveau = 0;
  for (uint8_t foutniveau = 5; foutniveau > 0; foutniveau--) {
    if (toestand.aantalOpeenvolgendeFouten >= drempels.opeenvolgendeFoutenNaAantal[foutniveau - 1] ||
        (drempels.sindsEersteFoutNaMs[foutniveau - 1] > 0 && duur >= drempels.sindsEersteFoutNaMs[foutniveau - 1])) {
      nieuwFoutniveau = foutniveau;
      break;
    }
  }
  const uint8_t hoogste = FoutniveauVanScreenData(drempels.hoogstToegelatenFoutniveau);
  if (nieuwFoutniveau > hoogste) nieuwFoutniveau = hoogste;
  if (nieuwFoutniveau <= toestand.huidigFoutniveau) return 0;   // alleen melden als het foutniveau hoger wordt
  toestand.huidigFoutniveau = nieuwFoutniveau;
  return nieuwFoutniveau;
}

// Eén melding: regel 1 = foutniveau en code (bv. ALERT: GB400), regel 2 = I2C-adres, regel 3 = kanaal (achter een TCA9548A of per sensorkanaal).
// Zonder snprintf (D042: ±13 KB op UNO R4). Alleen naar Serial, nooit via Screen::Print(): dat zou de opdracht op het scherm
// overschrijven (niets tekent ze terug) en het LCD via I2C testen, midden in de afhandeling van een I2C-fout (cascade).
static void MeldingTijdensRuntimeTonen(uint8_t foutniveau, const char* code, HardwareResourceTypeI2C* component, uint8_t kanaal) {
  static const char* const foutniveauTekst[5] = { _FOUTNIVEAU_WARNING, _FOUTNIVEAU_ALERT, _FOUTNIVEAU_FAULT, _FOUTNIVEAU_CRITICAL, _FOUTNIVEAU_FATAL };
  static const char hex[] = "0123456789ABCDEF";

  // Zelf kopiëren in plaats van strcpy/strcat/strncat: ±340 bytes flash minder op UNO R4 (gemeten).
  char regel1[24];
  uint8_t positie = 0;
  if (foutniveau >= 1 && foutniveau <= 5) {
    for (const char* teken = foutniveauTekst[foutniveau - 1]; *teken != '\0' && positie < 12; teken++) regel1[positie++] = *teken;
    regel1[positie++] = ':';
    regel1[positie++] = ' ';
  }
  for (const char* teken = code; *teken != '\0' && positie < sizeof(regel1) - 1; teken++) regel1[positie++] = *teken;
  regel1[positie] = '\0';

  char regel2[] = "I2C-ADRES: 0x00";
  regel2[13] = hex[component->adres >> 4];
  regel2[14] = hex[component->adres & 0x0F];

#if EXTENDER_TCA9548A_AANTAL > 0
  // Achter een TCA9548A: het kanaal uit de claim EP_CH0 tot EP_CH7 (zoals SelectChannel()).
  if (kanaal == 0 && component->parent != nullptr && component->parent->component == GedeeldeBusComponent::I2C_MULTIPLEXER_TCA9548A) {
    for (uint8_t i = 0; i < component->AantalClaims(); i++) {
      bool exclusief = false; HardwareResourceIdentiteit identiteit = HardwareResourceIdentiteit::GEEN;
      const uint8_t waarde = component->ClaimWaarde(i, exclusief, identiteit);
      if (waarde <= static_cast<uint8_t>(ExtenderTCA9548A::ExtenderPins::EP_CH7)) { kanaal = waarde + 1; break; }
    }
  }
#endif
  char regel3[] = "KANAAL: 0";
  if (kanaal > 0 && kanaal <= 9) regel3[8] = static_cast<char>('0' + kanaal);

  ::GA_SERIAL.println(regel1);
  ::GA_SERIAL.println(regel2);
  if (kanaal > 0) ::GA_SERIAL.println(regel3);
}

bool VastgelopenGedeeldeBusVrijmaken(GedeeldeBusType busType) {
  if (busType != GedeeldeBusType::I2C) return true;
#if defined(WIRE_HAS_END)
  #if BOARD_VERSION == BOARD_ESP32S3_ARDI32 || BOARD_VERSION == BOARD_ESP32S3_DEV
    const uint8_t pinSDA = ARDUINO_UNO_SHIELD_PIN_SDA;
    const uint8_t pinSCL = ARDUINO_UNO_SHIELD_PIN_SCL;
  #else
    const uint8_t pinSDA = SDA;
    const uint8_t pinSCL = SCL;
  #endif
  // Eerst lezen met Wire actief: SDA hoog = de bus is vrij, dan niets aanraken (geen herstart van LCD, PCF8574, ...).
  // TRACE alleen in de tweede trap van ComponentBeschikbaarTijdensRuntime(): enkel daar is herstelTijdensRuntimeBezig hier true.
  if (digitalRead(pinSDA) == HIGH) {
#ifdef TRACE
    if (herstelTijdensRuntimeBezig && GA_SERIAL) GA_SERIAL.println("TRACE: herstel trap 2, SDA hoog: Wire opnieuw");
#endif
    return true;
  }
#ifdef TRACE
  if (herstelTijdensRuntimeBezig && GA_SERIAL) GA_SERIAL.println("TRACE: herstel trap 2, SDA laag: busherstel");
#endif
  Wire.end();
  pinMode(pinSDA, INPUT_PULLUP);
  pinMode(pinSCL, INPUT_PULLUP);
  // Open-drain nabootsen: LOW = uitgang laag, HIGH = loslaten (INPUT_PULLUP). Nooit actief hoog sturen.
  for (uint8_t puls = 0; puls < 9 && digitalRead(pinSDA) == LOW; puls++) {
    digitalWrite(pinSCL, LOW); pinMode(pinSCL, OUTPUT); delayMicroseconds(5);
    pinMode(pinSCL, INPUT_PULLUP); delayMicroseconds(5);
  }
  // STOP-conditie: SDA van laag naar hoog terwijl SCL hoog is.
  digitalWrite(pinSDA, LOW); pinMode(pinSDA, OUTPUT); delayMicroseconds(5);
  pinMode(pinSDA, INPUT_PULLUP); delayMicroseconds(5);
  const bool vrij = digitalRead(pinSDA) == HIGH;
  i2cGeinitialiseerd = false;
  InitialiserenGedeeldeBus(GedeeldeBusType::I2C);
  return vrij;
#else
  return true;
#endif
}

void GedeeldeBusNode::AlleOnderliggendeNodesUitschakelen() {
  for (const GedeeldeBusNode* n = VolgendeInBoom(this, this); n != nullptr; n = VolgendeInBoom(n, this)) {
    const_cast<GedeeldeBusNode*>(n)->actief = false;
  }
}

// Een I2C-node? Op de inhoud getest: GedeeldeBusIdentiteitI2C is static constexpr in de header, dus elk .cpp-bestand heeft zijn eigen kopie (ander adres).
static bool IsI2CNode(const GedeeldeBusNode* node) {
  return node->gedeeldeIdentiteiten != nullptr && node->gedeeldeIdentiteiten[0] == HardwareResourceIdentiteit::I2C_SDA;
}

void GedeeldeBusNode::AlleOnderliggendeNodesOpnieuwActiveren() {
  // Van boven naar onder: een parent is altijd eerst opnieuw actief.
  // Een node die niet herstelt, blijft uitgeschakeld. Een eigen herstelpad van die node (I2C: ComponentBeschikbaarTijdensRuntime(),
  // een sensor: zijn eigen pad, zoals RFP602::OpnieuwAanmeldenNaPauze()) probeert later opnieuw; een node zonder zo'n pad niet.
  for (const GedeeldeBusNode* n = VolgendeInBoom(this, this); n != nullptr; n = VolgendeInBoom(n, this)) {
    GedeeldeBusNode* node = const_cast<GedeeldeBusNode*>(n);
    // Parent niet actief: de fout komt van de parent, die zelf al meldde. Geen eigen melding.
    if (node->parent != nullptr && !node->parent->actief) continue;

    const bool i2c = IsI2CNode(node);
    oorzaakHerstelTijdensRuntime = _GB401;
    herstelTijdensRuntimeBezig = i2c;
    const bool gelukt = node->activeren();
    herstelTijdensRuntimeBezig = false;
    if (gelukt) continue;

    if (i2c) {
      // Eigen fout van een I2C-node: een stap op zijn eigen ladder, met zijn eigen drempels; zijn eigen pauze begint nu.
      HardwareResourceTypeI2C* component = static_cast<HardwareResourceTypeI2C*>(node);
      ToestandTijdensRuntime& toestand = component->toestandTijdensRuntime;
      FoutniveauTijdensRuntimeBerekenen(toestand, false, DrempelsFoutniveauVoorComponent(component));
      toestand.tijdstipLaatsteHerstelpoging = millis();
      toestand.oorzaakHerstelGemeld = true;
      if (toestand.huidigFoutniveau != 0) MeldingTijdensRuntimeTonen(toestand.huidigFoutniveau, oorzaakHerstelTijdensRuntime, component);
    } else {
      // Geen eigen toestand (bv. een sensor): de fout hoort bij de parent. Het foutniveau van de dichtstbijzijnde I2C-node erboven die
      // nog een foutniveau heeft, overnemen (al begrensd door diens maximum); dat is minstens de node die nu herstelt, want
      // ComponentBeschikbaarTijdensRuntime() reset zijn toestand pas na deze functie. Met het I2C-adres van die node.
      GedeeldeBusNode* boven = node->parent;
      while (boven != nullptr && !(IsI2CNode(boven) && static_cast<HardwareResourceTypeI2C*>(boven)->toestandTijdensRuntime.huidigFoutniveau != 0)) boven = boven->parent;
      if (boven != nullptr) MeldingTijdensRuntimeTonen(static_cast<HardwareResourceTypeI2C*>(boven)->toestandTijdensRuntime.huidigFoutniveau, _GB401, static_cast<HardwareResourceTypeI2C*>(boven));
    }
  }
}

bool HardwareResourceTypeI2C::ComponentBeschikbaarTijdensRuntime() {
  if (actief) return true;
  if (!ingeplugd) return false;

  const unsigned long nu = millis();
  if (nu - toestandTijdensRuntime.tijdstipLaatsteHerstelpoging < RUNTIME_PAUZE_TUSSEN_HERSTELPOGINGEN_MS) return false;
  toestandTijdensRuntime.tijdstipLaatsteHerstelpoging = nu;

  oorzaakHerstelTijdensRuntime = _GB401;   // FoutDiagnoseI2C() vult met DEBUG de echte oorzaak in
  bool gelukt = false;
  if (VastgelopenGedeeldeBusVrijmaken(GedeeldeBusType::I2C)) {
    herstelTijdensRuntimeBezig = true;
    gelukt = activeren();
#if defined(WIRE_HAS_END)
    // Tweede trap: na een timeout breekt de Wire van de UNO R4 de lopende transactie niet af (setWireTimeout() negeert de reset,
    // nagekeken in Wire.cpp), dan faalt elke volgende transactie naar elk adres, ook met SDA hoog.
    // Eerst SDA opnieuw beoordelen (de eerste activeren() kan de bus zelf laten vastlopen): blijft SDA laag, GB409 en geen
    // tweede activeren(). Anders Wire opnieuw initialiseren (na een busherstel dus dubbel, bewust) en nog één keer activeren().
    // Samen telt dit als één herstelpoging.
    if (!gelukt) {
      if (VastgelopenGedeeldeBusVrijmaken(GedeeldeBusType::I2C)) {
        Wire.end();
        i2cGeinitialiseerd = false;
        InitialiserenGedeeldeBus(GedeeldeBusType::I2C);
        gelukt = activeren();
      } else {
        oorzaakHerstelTijdensRuntime = _GB409;
      }
    }
#endif
    herstelTijdensRuntimeBezig = false;
  } else {
    oorzaakHerstelTijdensRuntime = _GB409;   // SDA blijft laag na het busherstel: geen activeren() in deze poging (spaart een timeout)
  }

  if (!gelukt) {
    // Een mislukte herstelpoging telt als fout: de ladder klimt verder op aantal en tijd (bv. CRITICAL naar FATAL).
    // Melden: de eerste keer met het huidige foutniveau, daarna alleen bij een hoger foutniveau. De code zegt de oorzaak.
    const uint8_t resultaat = FoutniveauTijdensRuntimeBerekenen(toestandTijdensRuntime, false, DrempelsFoutniveauVoorComponent(this));
    if (resultaat != 0 || !toestandTijdensRuntime.oorzaakHerstelGemeld) {
      MeldingTijdensRuntimeTonen(toestandTijdensRuntime.huidigFoutniveau, oorzaakHerstelTijdensRuntime, this);
      toestandTijdensRuntime.oorzaakHerstelGemeld = true;
    }
    return false;
  }

  // Volgorde: eerst melden en de onderliggende nodes heractiveren (die lezen nog het foutniveau van deze node),
  // daarna altijd resetten, ook als een onderliggende node faalt: deze node zelf is hersteld.
  MeldingTijdensRuntimeTonen(0, _GB407, this);
  AlleOnderliggendeNodesOpnieuwActiveren();
  toestandTijdensRuntime = ToestandTijdensRuntime();
  return true;
}

bool HardwareResourceTypeI2C::ResultaatComponentToegangTijdensRuntimeVerwerken(bool gelukt) {
  const DrempelsFoutniveauTijdensRuntime& drempels = DrempelsFoutniveauVoorComponent(this);
  const uint8_t resultaat = FoutniveauTijdensRuntimeBerekenen(toestandTijdensRuntime, gelukt, drempels);
  if (resultaat == 255) { MeldingTijdensRuntimeTonen(0, _GB407, this); return true; }
  if (gelukt) return true;
  if (resultaat == 0) return false;

  MeldingTijdensRuntimeTonen(resultaat, _GB400, this);
  if (actief && resultaat >= FoutniveauVanScreenData(drempels.uitschakelenVanafFoutniveau)) {
    actief = false;
    AlleOnderliggendeNodesUitschakelen();
    toestandTijdensRuntime.oorzaakHerstelGemeld = false;
    toestandTijdensRuntime.tijdstipLaatsteHerstelpoging = millis();   // de eerste herstelpoging na de pauze; aantal en tijd lopen door
  }
  return false;
}

uint8_t MetingSensorTijdensRuntimeVerwerken(ToestandTijdensRuntime& toestand, bool gelukt, const DrempelsFoutniveauTijdensRuntime& drempels, HardwareResourceTypeI2C* extender, uint8_t kanaal) {
  const uint8_t resultaat = FoutniveauTijdensRuntimeBerekenen(toestand, gelukt, drempels);
  // GB410 = de meting (Sensor- of kanaalladder) is hersteld; niet: de Extender opnieuw geactiveerd (dat is GB407).
  if (resultaat == 255) { MeldingTijdensRuntimeTonen(0, _GB410, extender, kanaal); return resultaat; }
  if (resultaat == 0) return resultaat;

  MeldingTijdensRuntimeTonen(resultaat, _GB408, extender, kanaal);
  // Eén effectieve ladder per meting: vanaf het uitschakelniveau gaat de Extender zelf uit, want herstel gebeurt op chipniveau.
  // Daarmee vallen alle kanalen en sensoren op die Extender mee uit. De ladder gaat over naar de Extender en loopt daar verder
  // in de herstelpogingen; deze ladder begint na het herstel opnieuw.
  if (extender->actief && resultaat >= FoutniveauVanScreenData(drempels.uitschakelenVanafFoutniveau)) {
    // Staat de Extender op "nooit automatisch uitschakelen" (zijn uitschakelniveau boven zijn maximum): ook deze ladder schakelt
    // hem niet uit en start dus ook zijn herstelpad niet. Alleen GB408 hierboven.
    const DrempelsFoutniveauTijdensRuntime& extenderDrempels = DrempelsFoutniveauVoorComponent(extender);
    const uint8_t extenderHoogste = FoutniveauVanScreenData(extenderDrempels.hoogstToegelatenFoutniveau);
    if (FoutniveauVanScreenData(extenderDrempels.uitschakelenVanafFoutniveau) > extenderHoogste) return resultaat;

    // Samenvoegen met wat de Extender zelf al opbouwde (bv. door een kanaal zonder override): hoogste foutniveau, hoogste aantal
    // (niet optellen: zelfde storing) en oudste eerste fout (als duur vergeleken, ook juist na de overloop van millis()).
    ToestandTijdensRuntime& extenderToestand = extender->toestandTijdensRuntime;
    if (extenderToestand.aantalOpeenvolgendeFouten == 0) {
      extenderToestand = toestand;
    } else {
      const unsigned long nu = millis();
      if (toestand.huidigFoutniveau > extenderToestand.huidigFoutniveau) extenderToestand.huidigFoutniveau = toestand.huidigFoutniveau;
      if (toestand.aantalOpeenvolgendeFouten > extenderToestand.aantalOpeenvolgendeFouten) extenderToestand.aantalOpeenvolgendeFouten = toestand.aantalOpeenvolgendeFouten;
      if (nu - toestand.tijdstipEersteFout > nu - extenderToestand.tijdstipEersteFout) extenderToestand.tijdstipEersteFout = toestand.tijdstipEersteFout;
    }
    // Pas na het samenvoegen: het foutniveau begrenzen op het maximum van de Extender (in beide takken).
    // Aantal en oudste eerste fout blijven: de Extender herstelt vanaf nu volgens zijn eigen drempels.
    if (extenderToestand.huidigFoutniveau > extenderHoogste) extenderToestand.huidigFoutniveau = extenderHoogste;
    toestand = ToestandTijdensRuntime();
    extender->actief = false;
    extender->AlleOnderliggendeNodesUitschakelen();
    extender->toestandTijdensRuntime.oorzaakHerstelGemeld = false;
    extender->toestandTijdensRuntime.tijdstipLaatsteHerstelpoging = millis();   // de eerste herstelpoging na de pauze
  }
  return resultaat;
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
#if !GEDEELDE_BUS_MIXED_PINS
  const bool native = (firstExtenderNode() == nullptr);
#endif

  for (uint8_t x = 0; x < AantalClaims(); x++) {
    bool exclusiefA = false; HardwareResourceIdentiteit idA = HardwareResourceIdentiteit::GEEN;
    const uint8_t waardeA = ClaimWaarde(x, exclusiefA, idA);
    if (waardeA == geen) continue;

    if (IsAdres(waardeA)) continue;

    for (uint8_t y = x + 1; y < AantalClaims(); y++) {
      bool exclusiefB = false; HardwareResourceIdentiteit idB = HardwareResourceIdentiteit::GEEN;
      const uint8_t waardeB = ClaimWaarde(y, exclusiefB, idB);
      if (waardeB == geen || IsAdres(waardeB)) continue;
#if GEDEELDE_BUS_MIXED_PINS
      const GedeeldeBusNode* gebied = GetOwnerClaim(x);
      if (gebied != GetOwnerClaim(y)) continue;
      const bool native = (gebied == nullptr);
#endif

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
    // Componentcodes 0x20 t/m 0x7F zijn Extenders.
    if (!IsExtenderComponent(component)) {
      for (uint8_t i = 0; i < AantalClaims(); i++) {
        bool exclusief = false; HardwareResourceIdentiteit identiteit = HardwareResourceIdentiteit::GEEN;
        const uint8_t waarde = ClaimWaarde(i, exclusief, identiteit);
        if (waarde == geen || IsAdres(waarde)) continue;
#if GEDEELDE_BUS_MIXED_PINS
        if (GetOwnerClaim(i) != parent) continue;   // claim met een eigen Extender: hieronder apart gecontroleerd
#endif

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
#if EXTENDER_MCP23S17_AANTAL > 0
          case GedeeldeBusComponent::DIGITAL_PINS_MCP23S17: geldig = waarde <= static_cast<uint8_t>(ExtenderMCP23S17::ExtenderPins::EP_GPB7); break;
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
#if EXTENDER_PCA9685_AANTAL > 0
          case GedeeldeBusComponent::PWM_PCA9685: geldig = waarde <= static_cast<uint8_t>(ExtenderPCA9685::ExtenderPins::EP_PWM15); break;
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

#if GEDEELDE_BUS_MIXED_PINS
  // Exclusieve claims met een eigen Extender: de pin moet op die Extender bestaan. Alleen Extenders met digitale pinnen (SetExtenderPinMode/WriteExtenderPin/ReadExtenderPin).
  if (ownerClaim != nullptr) {
    for (uint8_t i = 0; i < aangemeldePinnen.aantalExclusief; i++) {
      const GedeeldeBusNode* extender = ownerClaim[i];
      const uint8_t waarde = aangemeldePinnen.exclusievePinnen[i];
      if (extender == nullptr || waarde == geen) continue;

      bool geldig = false;
      switch (extender->component) {
  #if EXTENDER_PCF8574_AANTAL > 0 || ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_PCF8574)
        case GedeeldeBusComponent::DIGITAL_PINS_PCF8574: geldig = waarde <= static_cast<uint8_t>(ExtenderPCF8574::ExtenderPins::EP_P7); break;
  #endif
  #if EXTENDER_PCF8575_AANTAL > 0
        case GedeeldeBusComponent::DIGITAL_PINS_PCF8575: geldig = waarde <= static_cast<uint8_t>(ExtenderPCF8575::ExtenderPins::EP_P17); break;
  #endif
  #if EXTENDER_MCP23017_AANTAL > 0
        case GedeeldeBusComponent::DIGITAL_PINS_MCP23017: geldig = waarde <= static_cast<uint8_t>(ExtenderMCP23017::ExtenderPins::EP_GPB7); break;
  #endif
        default: break;
      }

      if (!geldig) {
        conflictGevonden = true;
        GedeeldeBusAanmakenTijdelijkConflict(_FATAL_GB111, component, extender->component, waarde, false);
      }
    }
  }
#endif

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

void GedeeldeBusVerwijderTijdelijkeConflicten() {
  while (GedeeldeBusHuidigeCyclusConflicten != nullptr) {
    GedeeldeBusTijdelijkConflict* r = GedeeldeBusHuidigeCyclusConflicten;
    GedeeldeBusHuidigeCyclusConflicten = r->volgende;
    delete r;
  }
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
      if (strcmp(r->code, _CRITICAL_GB303) == 0) {
        // GB303: de waarde is de foutcode van de library, geen pin of adres.
        String hexWaarde = String(r->waarde, HEX);
        if (hexWaarde.length() < 2) hexWaarde = "0" + hexWaarde;
        regel4 = String("FOUT: 0x") + hexWaarde;
      } else if (r->isAdres) {
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
      // Leestijd volgens het niveau van de code: CRITICAL apart, ABORT en FATAL samen (D042).
      const bool critical = strncmp(r->code, "CRITICAL", 8) == 0;
      const unsigned long leestijd = critical ? GENERAL_MELDING_CRITICAL_LEESTIJD_MS : GENERAL_MELDING_FATAL_ABORT_PANIC_LEESTIJD_MS;
#if (SCREEN_OUTPUT & SCREEN_TYPE_PIXELS)
      scherm->Print(critical ? ScreenData::TYPE_CRITICAL : ScreenData::TYPE_FATAL, r->code, GedeeldeBusComponentNaam(r->A), leestijd, "", GedeeldeBusComponentNaam(r->B), regel4, leestijd);
#else
      scherm->Print(r->code, GedeeldeBusComponentNaam(r->A), leestijd, "", GedeeldeBusComponentNaam(r->B), regel4, leestijd);
#endif
      // Print() wacht enkel TUSSEN regel 1-2 en regel 3-4, niet erna. Zonder deze wacht zou de volgende melding in deze lus pagina 2 van de huidige meteen overschrijven, onleesbaar.
      if (r->volgende != nullptr) delay(leestijd);
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

