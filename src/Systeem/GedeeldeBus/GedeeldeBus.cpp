#include "GedeeldeBus.h"
#include <Wire.h>
#include <SPI.h>
#include "../../Configuratie/SystemConfig.h"

static bool i2cGeinitialiseerd = false;
static bool spiGeinitialiseerd = false;


#ifdef GEDEELDE_BUS_PROTOTYPE

static GedeeldeBusResourceRegistratie* RegistratiesInternOpGedeeldeBus() {
  static GedeeldeBusResourceRegistratie registraties[MAX_GEDEELDE_BUS_RESOURCE_REGISTRATIES];
  return registraties;
}

static uint8_t& AantalRegistratiesInternOpGedeeldeBus() {
  static uint8_t aantalRegistraties = 0;
  return aantalRegistraties;
}

// Interne, niet-publieke as: welke van de twee tegengestelde bewerkingen wordt uitgevoerd.
enum class AanOfAfmeldenOpGedeeldeBus : uint8_t { AANMELDEN = 0x00, AFMELDEN = 0x01 };

static bool ResourceWijzigenOpGedeeldeBus(AanOfAfmeldenOpGedeeldeBus actie, GedeeldeBusComponent component, HardwareResourceType type, HardwareResourcePin resource, HardwareResourceToegang toegang, GedeeldeBusRol rol, uint8_t adres) {
  uint8_t pin = GEDEELDE_BUS_GEEN_PIN;

  switch (resource) {
#ifdef ARDUINO_UNO_SHIELD_PIN_D0
    case HardwareResourcePin::D0:   pin = ARDUINO_UNO_SHIELD_PIN_D0;   break;
#endif
#ifdef ARDUINO_UNO_SHIELD_PIN_D1
    case HardwareResourcePin::D1:   pin = ARDUINO_UNO_SHIELD_PIN_D1;   break;
#endif
#ifdef ARDUINO_UNO_SHIELD_PIN_D2
    case HardwareResourcePin::D2:   pin = ARDUINO_UNO_SHIELD_PIN_D2;   break;
#endif
#ifdef ARDUINO_UNO_SHIELD_PIN_D3
    case HardwareResourcePin::D3:   pin = ARDUINO_UNO_SHIELD_PIN_D3;   break;
#endif
#ifdef ARDUINO_UNO_SHIELD_PIN_D4
    case HardwareResourcePin::D4:   pin = ARDUINO_UNO_SHIELD_PIN_D4;   break;
#endif
#ifdef ARDUINO_UNO_SHIELD_PIN_D5
    case HardwareResourcePin::D5:   pin = ARDUINO_UNO_SHIELD_PIN_D5;   break;
#endif
#ifdef ARDUINO_UNO_SHIELD_PIN_D6
    case HardwareResourcePin::D6:   pin = ARDUINO_UNO_SHIELD_PIN_D6;   break;
#endif
#ifdef ARDUINO_UNO_SHIELD_PIN_D7
    case HardwareResourcePin::D7:   pin = ARDUINO_UNO_SHIELD_PIN_D7;   break;
#endif
#ifdef ARDUINO_UNO_SHIELD_PIN_D8
    case HardwareResourcePin::D8:   pin = ARDUINO_UNO_SHIELD_PIN_D8;   break;
#endif
#ifdef ARDUINO_UNO_SHIELD_PIN_D9
    case HardwareResourcePin::D9:   pin = ARDUINO_UNO_SHIELD_PIN_D9;   break;
#endif
#ifdef ARDUINO_UNO_SHIELD_PIN_D10
    case HardwareResourcePin::D10:  pin = ARDUINO_UNO_SHIELD_PIN_D10;  break;
#endif
#ifdef ARDUINO_UNO_SHIELD_PIN_D11
    case HardwareResourcePin::D11:  pin = ARDUINO_UNO_SHIELD_PIN_D11;  break;
#endif
#ifdef ARDUINO_UNO_SHIELD_PIN_D12
    case HardwareResourcePin::D12:  pin = ARDUINO_UNO_SHIELD_PIN_D12;  break;
#endif
#ifdef ARDUINO_UNO_SHIELD_PIN_D13
    case HardwareResourcePin::D13:  pin = ARDUINO_UNO_SHIELD_PIN_D13;  break;
#endif
#ifdef ARDUINO_UNO_SHIELD_PIN_A0
    case HardwareResourcePin::A0:   pin = ARDUINO_UNO_SHIELD_PIN_A0;   break;
#endif
#ifdef ARDUINO_UNO_SHIELD_PIN_A1
    case HardwareResourcePin::A1:   pin = ARDUINO_UNO_SHIELD_PIN_A1;   break;
#endif
#ifdef ARDUINO_UNO_SHIELD_PIN_A2
    case HardwareResourcePin::A2:   pin = ARDUINO_UNO_SHIELD_PIN_A2;   break;
#endif
#ifdef ARDUINO_UNO_SHIELD_PIN_A3
    case HardwareResourcePin::A3:   pin = ARDUINO_UNO_SHIELD_PIN_A3;   break;
#endif
#ifdef ARDUINO_UNO_SHIELD_PIN_A4
    case HardwareResourcePin::A4:   pin = ARDUINO_UNO_SHIELD_PIN_A4;   break;
#endif
#ifdef ARDUINO_UNO_SHIELD_PIN_A5
    case HardwareResourcePin::A5:   pin = ARDUINO_UNO_SHIELD_PIN_A5;   break;
#endif
#ifdef ARDUINO_UNO_SHIELD_PIN_SDA
    case HardwareResourcePin::SDA:  pin = ARDUINO_UNO_SHIELD_PIN_SDA;  break;
#endif
#ifdef ARDUINO_UNO_SHIELD_PIN_SCL
    case HardwareResourcePin::SCL:  pin = ARDUINO_UNO_SHIELD_PIN_SCL;  break;
#endif
#ifdef ARDUINO_UNO_SHIELD_PIN_MISO
    case HardwareResourcePin::MISO: pin = ARDUINO_UNO_SHIELD_PIN_MISO; break;
#endif
#ifdef ARDUINO_UNO_SHIELD_PIN_MOSI
    case HardwareResourcePin::MOSI: pin = ARDUINO_UNO_SHIELD_PIN_MOSI; break;
#endif
#ifdef ARDUINO_UNO_SHIELD_PIN_SCK
    case HardwareResourcePin::SCK:  pin = ARDUINO_UNO_SHIELD_PIN_SCK;  break;
#endif
#ifdef ARDUINO_UNO_SHIELD_PIN_SS
    case HardwareResourcePin::SS:   pin = ARDUINO_UNO_SHIELD_PIN_SS;   break;
#endif
    case HardwareResourcePin::GEEN: break;
  }

  GedeeldeBusResourceRegistratie* registraties = RegistratiesInternOpGedeeldeBus();
  uint8_t& aantalRegistraties = AantalRegistratiesInternOpGedeeldeBus();

  if (actie == AanOfAfmeldenOpGedeeldeBus::AANMELDEN) {
    for (uint8_t i = 0; i < aantalRegistraties; i++) {
      const GedeeldeBusResourceRegistratie& bestaand = registraties[i];
      if (bestaand.component == component && bestaand.type == type && bestaand.resource == resource && bestaand.pin == pin && bestaand.toegang == toegang && bestaand.rol == rol && bestaand.adres == adres) return true;
    }
    if (aantalRegistraties >= MAX_GEDEELDE_BUS_RESOURCE_REGISTRATIES) return false;
    registraties[aantalRegistraties++] = { component, type, resource, pin, toegang, rol, adres };
    return true;
  }

  for (uint8_t i = 0; i < aantalRegistraties; i++) {
    const GedeeldeBusResourceRegistratie& bestaand = registraties[i];
    if (bestaand.component == component && bestaand.type == type && bestaand.resource == resource && bestaand.pin == pin && bestaand.toegang == toegang && bestaand.rol == rol && bestaand.adres == adres) {
      registraties[i] = registraties[aantalRegistraties - 1];
      aantalRegistraties--;
      return true;
    }
  }
  return false;
}

bool ResourcesAanmeldenOpGedeeldeBus(GedeeldeBusComponent component, HardwareResourceType type, HardwareResourcePin resource, HardwareResourceToegang toegang, SetupOfLoop setupOfLoop, GedeeldeBusRol rol, uint8_t adres) {
  bool aangemeld = ResourceWijzigenOpGedeeldeBus(AanOfAfmeldenOpGedeeldeBus::AANMELDEN, component, type, resource, toegang, rol, adres);
  if (!aangemeld) return false;

  if (setupOfLoop == SetupOfLoop::LOOP) {
    bool magInpluggen = AlleAangemeldeResourcesInpluggenOpGedeeldeBus();
    if (!magInpluggen) {
      // CRITICAL: conflict tijdens LOOP. De zonet aangemelde resource wordt weer afgemeld,
      // zodat er geen conflicterende registratie blijft hangen. De aanroeper krijgt false
      // terug en is zelf verantwoordelijk voor de melding (GedeeldeBus.h kent Screen niet).
      ResourceWijzigenOpGedeeldeBus(AanOfAfmeldenOpGedeeldeBus::AFMELDEN, component, type, resource, toegang, rol, adres);
      return false;
    }
  }

  return true;
}

bool ResourcesAfmeldenOpGedeeldeBus(GedeeldeBusComponent component, HardwareResourceType type, HardwareResourcePin resource, HardwareResourceToegang toegang, SetupOfLoop setupOfLoop, GedeeldeBusRol rol, uint8_t adres) {
  bool afgemeld = ResourceWijzigenOpGedeeldeBus(AanOfAfmeldenOpGedeeldeBus::AFMELDEN, component, type, resource, toegang, rol, adres);
  if (!afgemeld) return false;

  if (setupOfLoop == SetupOfLoop::LOOP) return AlleAangemeldeResourcesInpluggenOpGedeeldeBus();
  return true;
}

bool I2CAanmeldenOpGedeeldeBus(GedeeldeBusComponent component, uint8_t adres, HardwareResourcePin sdaResource, HardwareResourcePin sclResource, SetupOfLoop setupOfLoop) {
  GedeeldeBusInitialiseren(GedeeldeBusType::I2C);

  if (!ResourcesAanmeldenOpGedeeldeBus(component, HardwareResourceType::I2C, sdaResource, HardwareResourceToegang::GEDEELD, setupOfLoop, GedeeldeBusRol::MASTER)) return false;
  if (!ResourcesAanmeldenOpGedeeldeBus(component, HardwareResourceType::I2C, sclResource, HardwareResourceToegang::GEDEELD, setupOfLoop, GedeeldeBusRol::MASTER)) return false;

  return ResourcesAanmeldenOpGedeeldeBus(component, HardwareResourceType::I2C, HardwareResourcePin::GEEN, HardwareResourceToegang::EXCLUSIEF, setupOfLoop, GedeeldeBusRol::SLAVE, adres);
}

bool SPIAanmeldenOpGedeeldeBus(GedeeldeBusComponent component, HardwareResourcePin misoResource, HardwareResourcePin mosiResource, HardwareResourcePin sckResource, SetupOfLoop setupOfLoop) {
  GedeeldeBusInitialiseren(GedeeldeBusType::SPI);

  if (!ResourcesAanmeldenOpGedeeldeBus(component, HardwareResourceType::SPI, misoResource, HardwareResourceToegang::GEDEELD, setupOfLoop, GedeeldeBusRol::MASTER)) return false;
  if (!ResourcesAanmeldenOpGedeeldeBus(component, HardwareResourceType::SPI, mosiResource, HardwareResourceToegang::GEDEELD, setupOfLoop, GedeeldeBusRol::MASTER)) return false;
  return ResourcesAanmeldenOpGedeeldeBus(component, HardwareResourceType::SPI, sckResource, HardwareResourceToegang::GEDEELD, setupOfLoop, GedeeldeBusRol::MASTER);
}

bool EEPROMAanmeldenOpGedeeldeBus(GedeeldeBusComponent component, SetupOfLoop setupOfLoop) {
  return ResourcesAanmeldenOpGedeeldeBus(component, HardwareResourceType::EEPROM, HardwareResourcePin::GEEN, HardwareResourceToegang::GEDEELD, setupOfLoop);
}

bool UARTAanmeldenOpGedeeldeBus(GedeeldeBusComponent component, SetupOfLoop setupOfLoop) {
  return ResourcesAanmeldenOpGedeeldeBus(component, HardwareResourceType::UART, HardwareResourcePin::GEEN, HardwareResourceToegang::GEDEELD, setupOfLoop);
}

bool AlleAangemeldeResourcesControlerenOpGedeeldeBus(uint8_t* conflictIndexA, uint8_t* conflictIndexB) {
  GedeeldeBusResourceRegistratie* registraties = RegistratiesInternOpGedeeldeBus();
  uint8_t aantalRegistraties = AantalRegistratiesInternOpGedeeldeBus();

  for (uint8_t i = 0; i < aantalRegistraties; i++) {
    for (uint8_t j = i + 1; j < aantalRegistraties; j++) {
      const GedeeldeBusResourceRegistratie& a = registraties[i];
      const GedeeldeBusResourceRegistratie& b = registraties[j];

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

bool AlleAangemeldeResourcesInpluggenOpGedeeldeBus() {
  return AlleAangemeldeResourcesControlerenOpGedeeldeBus();
}

void RegistratiesResettenOpGedeeldeBus() {
  AantalRegistratiesInternOpGedeeldeBus() = 0;
}

uint8_t AantalRegistratiesOpGedeeldeBus() {
  return AantalRegistratiesInternOpGedeeldeBus();
}

const GedeeldeBusResourceRegistratie* RegistratieOpIndexOpGedeeldeBus(uint8_t index) {
  if (index >= AantalRegistratiesInternOpGedeeldeBus()) return nullptr;
  return &RegistratiesInternOpGedeeldeBus()[index];
}

#endif // GEDEELDE_BUS_PROTOTYPE

void GedeeldeBusInitialiseren(GedeeldeBusType busType) {
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
void GedeeldeBusInitialiseren(GedeeldeBusType busType) {
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

bool GedeeldeBusIsGeinitialiseerd(GedeeldeBusType busType) {
  if (busType == GedeeldeBusType::I2C) return i2cGeinitialiseerd;
  if (busType == GedeeldeBusType::SPI) return spiGeinitialiseerd;
  return false;
}

void GedeeldeBusResetten() {
  i2cGeinitialiseerd = false;
  spiGeinitialiseerd = false;
}
