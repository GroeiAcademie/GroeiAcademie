#ifndef EXTENDERPCF8574_H
#define EXTENDERPCF8574_H

struct ExtenderPCF8574 : HardwareResourceTypeI2C, PCF8574 {
  enum class ExtenderPins : uint8_t {
    EP_P0 = 0x00, // Quasi-bidirectionele digitale I/O P0
    EP_P1 = 0x01, // Quasi-bidirectionele digitale I/O P1
    EP_P2 = 0x02, // Quasi-bidirectionele digitale I/O P2
    EP_P3 = 0x03, // Quasi-bidirectionele digitale I/O P3
    EP_P4 = 0x04, // Quasi-bidirectionele digitale I/O P4
    EP_P5 = 0x05, // Quasi-bidirectionele digitale I/O P5
    EP_P6 = 0x06, // Quasi-bidirectionele digitale I/O P6
    EP_P7 = 0x07  // Quasi-bidirectionele digitale I/O P7
  };

  HardwareResourcePin np_INT;
  uint8_t exclusiefMetInt_[2];

  // ============================================================================
  // PCF8574-BASIS — gedeeld door INPUT_PCF8574 en EXTENDER_PCF8574
  // ============================================================================
  ExtenderPCF8574(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t adres, HardwareResourcePin np_SDA, HardwareResourcePin np_SCL, HardwareResourcePin np_INT, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : HardwareResourceTypeI2C(parent, component, adres, np_SDA, np_SCL, Extender), PCF8574(adres, &Wire), np_INT(np_INT) {
    exclusiefMetInt_[0] = exclusief_[0];
    exclusiefMetInt_[1] = static_cast<uint8_t>(np_INT);
    aangemeldePinnen = { gedeeld_, 2, exclusiefMetInt_, 2 };
  }

  // ============================================================================
  // EXPERIMENTEEL — PCF8574 — AANTAL >= 2
  // ============================================================================
  #if EXTENDER_PCF8574_AANTAL >= 2
  struct Pcf8574Pinnen { uint8_t adres; HardwareResourcePin intPin; };

  #define BUNDEL_EXTENDER(N) { I2C_ADDRESS_EXTENDER_PCF8574_##N, EXTENDER_PCF8574_##N##_INT }

  static constexpr Pcf8574Pinnen ExtenderLijst[] = {
    BUNDEL_EXTENDER(1),
    BUNDEL_EXTENDER(2)
  };

  #undef BUNDEL_EXTENDER

  ExtenderPCF8574(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin np_SDA, HardwareResourcePin np_SCL, uint8_t teller, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : HardwareResourceTypeI2C(parent, component, ExtenderLijst[teller].adres, np_SDA, np_SCL, Extender), PCF8574(ExtenderLijst[teller].adres, &Wire), np_INT(ExtenderLijst[teller].intPin) {
    exclusiefMetInt_[0] = exclusief_[0];
    exclusiefMetInt_[1] = static_cast<uint8_t>(np_INT);
    aangemeldePinnen = { gedeeld_, 2, exclusiefMetInt_, 2 };
  }
  #endif

  bool Activeren() override {
    InitialiserenGedeeldeBus(GedeeldeBusType::I2C);
    return PCF8574::begin() && PCF8574::isConnected();
  }

  // Bestaande FrameWork-extra: de PCF8574 heeft geen apart richtingsregister.
  // INPUT schrijft HIGH zodat de pin kan worden gelezen; OUTPUT schrijft LOW.
  void pinMode(ExtenderPins pin, uint8_t modus) {
    PCF8574::write(static_cast<uint8_t>(pin), (modus == INPUT) ? HIGH : LOW);
  }
};

#endif
