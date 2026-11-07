#ifndef EXTENDERPCF8575_H
#define EXTENDERPCF8575_H

struct ExtenderPCF8575 : HardwareResourceTypeI2C, PCF8575 {
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

  HardwareResourcePin np_INT;
  uint8_t exclusiefMetInt_[2];

  // ============================================================================
  // DEFAULT — PCF8575 #1
  // ============================================================================
  #if EXTENDER_PCF8575_AANTAL == 1
  ExtenderPCF8575(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t adres, HardwareResourcePin np_SDA, HardwareResourcePin np_SCL, HardwareResourcePin np_INT, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD, uint8_t tcaChannel = 0xFF)
    : HardwareResourceTypeI2C(parent, component, adres, np_SDA, np_SCL, Extender, tcaChannel), PCF8575(adres, &Wire), np_INT(np_INT) {
    exclusiefMetInt_[0] = exclusief_[0];
    exclusiefMetInt_[1] = static_cast<uint8_t>(np_INT);
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

  ExtenderPCF8575(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin np_SDA, HardwareResourcePin np_SCL, uint8_t teller, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD, uint8_t tcaChannel = 0xFF)
    : HardwareResourceTypeI2C(parent, component, (teller < 2 ? ExtenderLijst[teller].adres : 0xFF), np_SDA, np_SCL, Extender, tcaChannel), PCF8575((teller < 2 ? ExtenderLijst[teller].adres : 0xFF), &Wire), np_INT((teller < 2 ? ExtenderLijst[teller].intPin : HardwareResourcePin::NONE)) {
    exclusiefMetInt_[0] = exclusief_[0];
    exclusiefMetInt_[1] = static_cast<uint8_t>(np_INT);
    aangemeldePinnen = { gedeeld_, 2, exclusiefMetInt_, 2 };
  }
  #endif

  bool Activeren() override {
    // Geen teruglezen: de PCF8575 heeft geen register, alleen quasi-bidirectionele pinnen waarvan de stand van de aansluiting afhangt.
    const bool gelukt = PCF8575::begin() && PCF8575::isConnected();
    return FoutDiagnoseI2C(gelukt, PCF8575::lastError(), gelukt);
  }

#if GEDEELDE_BUS_EXTENDER_GPIO_ACTIEF
  // Pin van deze Extender voor een component van een ander gebied (bv. CS van een SPI-chip).
  // SetExtenderPinMode: geen richtingsregister. INPUT schrijft HIGH; OUTPUT laat de laatst geschreven waarde staan.
  // Runtime-herstel: eerst ComponentBeschikbaarTijdensRuntime(), daarna het resultaat van de library doorgeven.
  bool SetExtenderPinMode(uint8_t pin, uint8_t modus) override {
    if (pin > static_cast<uint8_t>(ExtenderPins::EP_P17) || (modus != INPUT && modus != OUTPUT)) return false;
    if (!ComponentBeschikbaarTijdensRuntime()) return false;
    if (modus == OUTPUT) return true; // Geen richtingsregister; bestaande uitgangswaarde behouden.
    PCF8575::write(pin, HIGH);
    return ResultaatComponentToegangTijdensRuntimeVerwerken(PCF8575::lastError() == PCF8575_OK);
  }
  bool WriteExtenderPin(uint8_t pin, uint8_t niveau) override {
    if (pin > static_cast<uint8_t>(ExtenderPins::EP_P17)) return false;
    if (!ComponentBeschikbaarTijdensRuntime()) return false;
    PCF8575::write(pin, niveau);
    return ResultaatComponentToegangTijdensRuntimeVerwerken(PCF8575::lastError() == PCF8575_OK);
  }
  uint8_t ReadExtenderPin(uint8_t pin) override {
    if (pin > static_cast<uint8_t>(ExtenderPins::EP_P17)) return 0xFF;
    if (!ComponentBeschikbaarTijdensRuntime()) return 0xFF;
    const uint8_t waarde = PCF8575::read(pin);
    if (!ResultaatComponentToegangTijdensRuntimeVerwerken(PCF8575::lastError() == PCF8575_OK)) return 0xFF;
    return waarde;
  }
#endif
};

#endif
