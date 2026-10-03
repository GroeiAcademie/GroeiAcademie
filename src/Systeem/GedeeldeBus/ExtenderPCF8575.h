#ifndef EXTENDERPCF8575_H
#define EXTENDERPCF8575_H

struct ExtenderPCF8575 : HardwareResourceTypeI2C {
  enum class ExtenderPins : uint8_t {
    EP_P00 = 0x00, // Quasi-bidirectionele digitale I/O P00, eerste byte
    EP_P01 = 0x01, // Quasi-bidirectionele digitale I/O P01, eerste byte
    EP_P02 = 0x02, // Quasi-bidirectionele digitale I/O P02, eerste byte
    EP_P03 = 0x03, // Quasi-bidirectionele digitale I/O P03, eerste byte
    EP_P04 = 0x04, // Quasi-bidirectionele digitale I/O P04, eerste byte
    EP_P05 = 0x05, // Quasi-bidirectionele digitale I/O P05, eerste byte
    EP_P06 = 0x06, // Quasi-bidirectionele digitale I/O P06, eerste byte
    EP_P07 = 0x07, // Quasi-bidirectionele digitale I/O P07, eerste byte
    EP_P10 = 0x08, // Quasi-bidirectionele digitale I/O P10, tweede byte
    EP_P11 = 0x09, // Quasi-bidirectionele digitale I/O P11, tweede byte
    EP_P12 = 0x0A, // Quasi-bidirectionele digitale I/O P12, tweede byte
    EP_P13 = 0x0B, // Quasi-bidirectionele digitale I/O P13, tweede byte
    EP_P14 = 0x0C, // Quasi-bidirectionele digitale I/O P14, tweede byte
    EP_P15 = 0x0D, // Quasi-bidirectionele digitale I/O P15, tweede byte
    EP_P16 = 0x0E, // Quasi-bidirectionele digitale I/O P16, tweede byte
    EP_P17 = 0x0F  // Quasi-bidirectionele digitale I/O P17, tweede byte
  };

  HardwareResourcePin INT;
  uint8_t exclusiefMetInt_[2];

  // ============================================================================
  // DEFAULT — PCF8575 #1
  // ============================================================================
  #if EXTENDER_PCF8575_AANTAL == 1
  ExtenderPCF8575(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t adres, HardwareResourcePin SDA, HardwareResourcePin SCL, HardwareResourcePin INT, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : HardwareResourceTypeI2C(parent, component, adres, SDA, SCL, Extender), INT(INT) {
    exclusiefMetInt_[0] = exclusief_[0];
    exclusiefMetInt_[1] = static_cast<uint8_t>(INT);
    aangemeldePinnen = { gedeeld_, 2, exclusiefMetInt_, 2 };
  }
  #endif

  // ============================================================================
  // EXPERIMENTEEL — PCF8575 — AANTAL >= 2
  // ============================================================================
  #if EXTENDER_PCF8575_AANTAL >= 2
  struct Pcf8575Pinnen { uint8_t adres; HardwareResourcePin intPin; };

  #define BUNDEL_EXTENDER(N) { I2C_ADDRESS_EXTENDER_PCF8575_##N, EXTENDER_PCF8575_##N##_INT }

  static constexpr Pcf8575Pinnen ExtenderLijst[] = {
    BUNDEL_EXTENDER(1),
    BUNDEL_EXTENDER(2)
  };

  #undef BUNDEL_EXTENDER

  ExtenderPCF8575(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin SDA, HardwareResourcePin SCL, uint8_t teller, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : HardwareResourceTypeI2C(parent, component, ExtenderLijst[teller].adres, SDA, SCL, Extender), INT(ExtenderLijst[teller].intPin) {
    exclusiefMetInt_[0] = exclusief_[0];
    exclusiefMetInt_[1] = static_cast<uint8_t>(INT);
    aangemeldePinnen = { gedeeld_, 2, exclusiefMetInt_, 2 };
  }
  #endif
};

#endif
