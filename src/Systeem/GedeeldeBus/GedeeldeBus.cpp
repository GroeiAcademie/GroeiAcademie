#include "GedeeldeBus.h"
#include <Wire.h>
#include <SPI.h>
#include "../../Configuratie/SystemConfig.h"

static bool i2cGeinitialiseerd = false;
static bool spiGeinitialiseerd = false;

void InitialiserenGedeeldeBus(GedeeldeBusType busType) {
  if (busType == GedeeldeBusType::I2C) {
    if (!i2cGeinitialiseerd) {
      #if BOARD_VERSION == BOARD_ESP32S3_ARDI32 || BOARD_VERSION == BOARD_ESP32S3_DEV
        Wire.begin(ARDUINO_UNO_SHIELD_PIN_SDA, ARDUINO_UNO_SHIELD_PIN_SCL);
      #else
        Wire.begin();
      #endif

      i2cGeinitialiseerd = true;
    }
  } else if (busType == GedeeldeBusType::SPI) {
    if (!spiGeinitialiseerd) {
      SPI.begin();
      spiGeinitialiseerd = true;
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

#ifdef GEDEELDE_BUS_PROTOTYPE

static GedeeldeBusHardwareResourceAanmelding* OpvragenPointerAanmeldingenOpGedeeldeBus() {
  static GedeeldeBusHardwareResourceAanmelding registraties[GEDEELDE_BUS_MAX_AANMELDINGEN_HARDWARE_RESOURCES];
  return registraties;
}

static uint8_t& AantalInterneAanmeldingenOpGedeeldeBus() {
  static uint8_t aantalRegistraties = 0;
  return aantalRegistraties;
}

// Interne, niet-publieke as: welke van de twee tegengestelde bewerkingen wordt uitgevoerd.
enum class GedeeldeBusAanOfAfmelden : uint8_t { AANMELDEN = 0x00, AFMELDEN = 0x01 };

static bool BestaandeAanmeldingOfAfmeldenToggelenOpGedeeldeBus(GedeeldeBusAanOfAfmelden actie, GedeeldeBusComponent component, HardwareResourceType type, HardwareResourcePin resource, HardwareResourceToegang toegang, GedeeldeBusRol rol, uint8_t adres, uint8_t pinOverride) {
  uint8_t pin = GEDEELDE_BUS_GEEN_PIN;

  switch (resource) {
    case HardwareResourcePin::D0:     pin = ARDUINO_UNO_SHIELD_PIN_D0;   break;
    case HardwareResourcePin::D1:     pin = ARDUINO_UNO_SHIELD_PIN_D1;   break;
    case HardwareResourcePin::D2:     pin = ARDUINO_UNO_SHIELD_PIN_D2;   break;
    case HardwareResourcePin::D3:     pin = ARDUINO_UNO_SHIELD_PIN_D3;   break;
    case HardwareResourcePin::D4:     pin = ARDUINO_UNO_SHIELD_PIN_D4;   break;
    case HardwareResourcePin::D5:     pin = ARDUINO_UNO_SHIELD_PIN_D5;   break;
    case HardwareResourcePin::D6:     pin = ARDUINO_UNO_SHIELD_PIN_D6;   break;
    case HardwareResourcePin::D7:     pin = ARDUINO_UNO_SHIELD_PIN_D7;   break;
    case HardwareResourcePin::D8:     pin = ARDUINO_UNO_SHIELD_PIN_D8;   break;
    case HardwareResourcePin::D9:     pin = ARDUINO_UNO_SHIELD_PIN_D9;   break;
    case HardwareResourcePin::D10:    pin = ARDUINO_UNO_SHIELD_PIN_D10;  break;
    case HardwareResourcePin::D11:    pin = ARDUINO_UNO_SHIELD_PIN_D11;  break;
    case HardwareResourcePin::D12:    pin = ARDUINO_UNO_SHIELD_PIN_D12;  break;
    case HardwareResourcePin::D13:    pin = ARDUINO_UNO_SHIELD_PIN_D13;  break;
    case HardwareResourcePin::A0:     pin = ARDUINO_UNO_SHIELD_PIN_A0;   break;
    case HardwareResourcePin::A1:     pin = ARDUINO_UNO_SHIELD_PIN_A1;   break;
    case HardwareResourcePin::A2:     pin = ARDUINO_UNO_SHIELD_PIN_A2;   break;
    case HardwareResourcePin::A3:     pin = ARDUINO_UNO_SHIELD_PIN_A3;   break;
    case HardwareResourcePin::A4:     pin = ARDUINO_UNO_SHIELD_PIN_A4;   break;
    case HardwareResourcePin::A5:     pin = ARDUINO_UNO_SHIELD_PIN_A5;   break;
    case HardwareResourcePin::SDA:    pin = ARDUINO_UNO_SHIELD_PIN_SDA;  break;
    case HardwareResourcePin::SCL:    pin = ARDUINO_UNO_SHIELD_PIN_SCL;  break;
    case HardwareResourcePin::MISO:   pin = ARDUINO_UNO_SHIELD_PIN_MISO; break;
    case HardwareResourcePin::MOSI:   pin = ARDUINO_UNO_SHIELD_PIN_MOSI; break;
    case HardwareResourcePin::SCK:    pin = ARDUINO_UNO_SHIELD_PIN_SCK;  break;
    case HardwareResourcePin::SS:     pin = ARDUINO_UNO_SHIELD_PIN_SS;   break;
    case HardwareResourcePin::CUSTOM: pin = pinOverride; break;
    case HardwareResourcePin::NONE:   break;
  }

  GedeeldeBusHardwareResourceAanmelding* registraties = OpvragenPointerAanmeldingenOpGedeeldeBus();
  uint8_t& aantalRegistraties = AantalInterneAanmeldingenOpGedeeldeBus();

  if (actie == GedeeldeBusAanOfAfmelden::AANMELDEN) {
    for (uint8_t i = 0; i < aantalRegistraties; i++) {
      const GedeeldeBusHardwareResourceAanmelding& bestaand = registraties[i];
      if (bestaand.component == component && bestaand.type == type && bestaand.resource == resource && bestaand.pin == pin && bestaand.toegang == toegang && bestaand.rol == rol && bestaand.adres == adres) return true;
    }
    if (aantalRegistraties >= GEDEELDE_BUS_MAX_AANMELDINGEN_HARDWARE_RESOURCES) return false;
    registraties[aantalRegistraties++] = { component, type, resource, pin, toegang, rol, adres };
    return true;
  }

  for (uint8_t i = 0; i < aantalRegistraties; i++) {
    const GedeeldeBusHardwareResourceAanmelding& bestaand = registraties[i];
    if (bestaand.component == component && bestaand.type == type && bestaand.resource == resource && bestaand.pin == pin && bestaand.toegang == toegang && bestaand.rol == rol && bestaand.adres == adres) {
      registraties[i] = registraties[aantalRegistraties - 1];
      aantalRegistraties--;
      return true;
    }
  }
  return false;
}

bool AanmeldenHardwareResourcesOpGedeeldeBus(GedeeldeBusComponent component, HardwareResourceType type, HardwareResourcePin resource, HardwareResourceToegang toegang, bool gedeeldeBusAanmeldenOfAfmeldenInSetup, GedeeldeBusRol rol, uint8_t adres, uint8_t pinOverride) {
  bool aangemeld = BestaandeAanmeldingOfAfmeldenToggelenOpGedeeldeBus(GedeeldeBusAanOfAfmelden::AANMELDEN, component, type, resource, toegang, rol, adres, pinOverride);
  if (!aangemeld) return false;

  if (!gedeeldeBusAanmeldenOfAfmeldenInSetup) {
    bool magInpluggen = AanmeldingenInpluggenOpGedeeldeBus();
    if (!magInpluggen) {
      // CRITICAL: conflict tijdens LOOP. De zonet aangemelde resource wordt weer afgemeld,
      // zodat er geen conflicterende registratie blijft hangen. De aanroeper krijgt false
      // terug en is zelf verantwoordelijk voor de melding (GedeeldeBus.h kent Screen niet).
      BestaandeAanmeldingOfAfmeldenToggelenOpGedeeldeBus(GedeeldeBusAanOfAfmelden::AFMELDEN, component, type, resource, toegang, rol, adres, pinOverride);
      return false;
    }
  }

  return true;
}

bool AfmeldenHardwareResourcesOpGedeeldeBus(GedeeldeBusComponent component, HardwareResourceType type, HardwareResourcePin resource, HardwareResourceToegang toegang, bool gedeeldeBusAanmeldenOfAfmeldenInSetup, GedeeldeBusRol rol, uint8_t adres, uint8_t pinOverride) {
  bool afgemeld = BestaandeAanmeldingOfAfmeldenToggelenOpGedeeldeBus(GedeeldeBusAanOfAfmelden::AFMELDEN, component, type, resource, toegang, rol, adres, pinOverride);
  if (!afgemeld) return false;

  if (!gedeeldeBusAanmeldenOfAfmeldenInSetup) return AanmeldingenInpluggenOpGedeeldeBus();
  return true;
}

bool AanmeldenHardwareResourcesOpGedeeldeBus(GedeeldeBusComponent component, HardwareResourceType type, std::initializer_list<uint8_t> pinnen, HardwareResourceToegang toegang, bool gedeeldeBusAanmeldenOfAfmeldenInSetup, GedeeldeBusRol rol) {
  for (uint8_t pin : pinnen) {
    if (!AanmeldenHardwareResourcesOpGedeeldeBus(component, type, HardwareResourcePin::CUSTOM, toegang, gedeeldeBusAanmeldenOfAfmeldenInSetup, rol, GEDEELDE_BUS_GEEN_ADRES, pin)) return false;
  }
  return true;
}

bool I2CAanmeldenOpGedeeldeBus(GedeeldeBusComponent component, uint8_t adres, HardwareResourcePin sdaResource, HardwareResourcePin sclResource, bool gedeeldeBusAanmeldenOfAfmeldenInSetup) {
  InitialiserenGedeeldeBus(GedeeldeBusType::I2C);

  if (!AanmeldenHardwareResourcesOpGedeeldeBus(component, HardwareResourceType::I2C, sdaResource, HardwareResourceToegang::GEDEELD, gedeeldeBusAanmeldenOfAfmeldenInSetup, GedeeldeBusRol::MASTER)) return false;
  if (!AanmeldenHardwareResourcesOpGedeeldeBus(component, HardwareResourceType::I2C, sclResource, HardwareResourceToegang::GEDEELD, gedeeldeBusAanmeldenOfAfmeldenInSetup, GedeeldeBusRol::MASTER)) return false;

  return AanmeldenHardwareResourcesOpGedeeldeBus(component, HardwareResourceType::I2C, HardwareResourcePin::NONE, HardwareResourceToegang::EXCLUSIEF, gedeeldeBusAanmeldenOfAfmeldenInSetup, GedeeldeBusRol::SLAVE, adres);
}

bool SPIAanmeldenOpGedeeldeBus(GedeeldeBusComponent component, HardwareResourcePin misoResource, HardwareResourcePin mosiResource, HardwareResourcePin sckResource, bool gedeeldeBusAanmeldenOfAfmeldenInSetup) {
  InitialiserenGedeeldeBus(GedeeldeBusType::SPI);

  if (!AanmeldenHardwareResourcesOpGedeeldeBus(component, HardwareResourceType::SPI, misoResource, HardwareResourceToegang::GEDEELD, gedeeldeBusAanmeldenOfAfmeldenInSetup, GedeeldeBusRol::MASTER)) return false;
  if (!AanmeldenHardwareResourcesOpGedeeldeBus(component, HardwareResourceType::SPI, mosiResource, HardwareResourceToegang::GEDEELD, gedeeldeBusAanmeldenOfAfmeldenInSetup, GedeeldeBusRol::MASTER)) return false;
  return AanmeldenHardwareResourcesOpGedeeldeBus(component, HardwareResourceType::SPI, sckResource, HardwareResourceToegang::GEDEELD, gedeeldeBusAanmeldenOfAfmeldenInSetup, GedeeldeBusRol::MASTER);
}

bool EEPROMAanmeldenOpGedeeldeBus(GedeeldeBusComponent component, bool gedeeldeBusAanmeldenOfAfmeldenInSetup) {
  return AanmeldenHardwareResourcesOpGedeeldeBus(component, HardwareResourceType::EEPROM, HardwareResourcePin::NONE, HardwareResourceToegang::GEDEELD, gedeeldeBusAanmeldenOfAfmeldenInSetup);
}

bool UARTAanmeldenOpGedeeldeBus(GedeeldeBusComponent component, bool gedeeldeBusAanmeldenOfAfmeldenInSetup) {
  return AanmeldenHardwareResourcesOpGedeeldeBus(component, HardwareResourceType::UART, HardwareResourcePin::NONE, HardwareResourceToegang::GEDEELD, gedeeldeBusAanmeldenOfAfmeldenInSetup);
}

bool AanmeldingenOpConflictenControlerenOpGedeeldeBus(uint8_t* conflictIndexA, uint8_t* conflictIndexB) {
  GedeeldeBusHardwareResourceAanmelding* registraties = OpvragenPointerAanmeldingenOpGedeeldeBus();
  uint8_t aantalRegistraties = AantalInterneAanmeldingenOpGedeeldeBus();

  for (uint8_t i = 0; i < aantalRegistraties; i++) {
    for (uint8_t j = i + 1; j < aantalRegistraties; j++) {
      const GedeeldeBusHardwareResourceAanmelding& a = registraties[i];
      const GedeeldeBusHardwareResourceAanmelding& b = registraties[j];

      bool zelfdePin = a.pin != GEDEELDE_BUS_GEEN_PIN && b.pin != GEDEELDE_BUS_GEEN_PIN && a.pin == b.pin && (a.toegang == HardwareResourceToegang::EXCLUSIEF || b.toegang == HardwareResourceToegang::EXCLUSIEF);
      bool zelfdeI2CAdres = a.type == HardwareResourceType::I2C && b.type == HardwareResourceType::I2C && a.adres != GEDEELDE_BUS_GEEN_ADRES && a.adres == b.adres && a.component != b.component;

      if (zelfdePin || zelfdeI2CAdres) {
        if (conflictIndexA != nullptr) *conflictIndexA = i;
        if (conflictIndexB != nullptr) *conflictIndexB = j;
        return false;
      }
    }
  }
  return true;
}

bool AanmeldingenInpluggenOpGedeeldeBus() {
  return AanmeldingenOpConflictenControlerenOpGedeeldeBus();
}

void AantalAanmeldingenOpNulZettenOpGedeeldeBus() {
  AantalInterneAanmeldingenOpGedeeldeBus() = 0;
}

uint8_t OpvragenAantalAanmeldingenOpGedeeldeBus() {
  return AantalInterneAanmeldingenOpGedeeldeBus();
}

const GedeeldeBusHardwareResourceAanmelding* OpvragenPointerAanmeldingOpGedeeldeBus(uint8_t index) {
  if (index >= AantalInterneAanmeldingenOpGedeeldeBus()) return nullptr;
  return &OpvragenPointerAanmeldingenOpGedeeldeBus()[index];
}

#endif // GEDEELDE_BUS_PROTOTYPE

