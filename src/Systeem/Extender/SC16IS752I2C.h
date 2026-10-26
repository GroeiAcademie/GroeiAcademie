#ifndef EXTENDERSC16IS752I2C_H
#define EXTENDERSC16IS752I2C_H

struct ExtenderSC16IS752I2C : HardwareResourceTypeI2C {
  enum class ExtenderPins : uint8_t {
    EP_CHANNEL_A = 0x00, // UART kanaal A: TXA/RXA/RTSA/CTSA
    EP_CHANNEL_B = 0x01, // UART kanaal B: TXB/RXB/RTSB/CTSB

    EP_GPIO0     = 0x02, // GPIO0 / DSRB
    EP_GPIO1     = 0x03, // GPIO1 / DTRB
    EP_GPIO2     = 0x04, // GPIO2 / CDB
    EP_GPIO3     = 0x05, // GPIO3 / RIB
    EP_GPIO4     = 0x06, // GPIO4 / DSRA
    EP_GPIO5     = 0x07, // GPIO5 / DTRA
    EP_GPIO6     = 0x08, // GPIO6 / CDA
    EP_GPIO7     = 0x09  // GPIO7 / RIA
  };

  HardwareResourcePin np_IRQ, np_RESET;
  uint8_t exclusiefMetIrqReset_[3];

  // ============================================================================
  // DEFAULT — SC16IS752 I2C #1
  // ============================================================================
  #if EXTENDER_SC16IS752_I2C_AANTAL == 1
  ExtenderSC16IS752I2C(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t adres, HardwareResourcePin np_SDA, HardwareResourcePin np_SCL, HardwareResourcePin np_IRQ, HardwareResourcePin np_RESET, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : HardwareResourceTypeI2C(parent, component, adres, np_SDA, np_SCL, Extender), np_IRQ(np_IRQ), np_RESET(np_RESET) {
    exclusiefMetIrqReset_[0] = exclusief_[0];
    exclusiefMetIrqReset_[1] = static_cast<uint8_t>(np_IRQ);
    exclusiefMetIrqReset_[2] = static_cast<uint8_t>(np_RESET);
    aangemeldePinnen = { gedeeld_, 2, exclusiefMetIrqReset_, 3 };
  }
  #endif

  // ============================================================================
  // EXPERIMENTEEL — SC16IS752 I2C — AANTAL >= 2
  // ============================================================================
  #if EXTENDER_SC16IS752_I2C_AANTAL >= 2
  struct Sc16is752I2cPinnen { uint8_t adres; HardwareResourcePin irq; HardwareResourcePin reset; };

  #define BUNDEL_EXTENDER(N) { I2C_ADDRESS_EXTENDER_SC16IS752_I2C_##N, EXTENDER_SC16IS752_I2C_##N##_IRQ, EXTENDER_SC16IS752_I2C_##N##_RESET }

  static constexpr Sc16is752I2cPinnen ExtenderLijst[] = {
    BUNDEL_EXTENDER(1),
    BUNDEL_EXTENDER(2)
  };

  #undef BUNDEL_EXTENDER

  ExtenderSC16IS752I2C(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin np_SDA, HardwareResourcePin np_SCL, uint8_t teller, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : HardwareResourceTypeI2C(parent, component, ExtenderLijst[teller].adres, np_SDA, np_SCL, Extender), np_IRQ(ExtenderLijst[teller].irq), np_RESET(ExtenderLijst[teller].reset) {
    exclusiefMetIrqReset_[0] = exclusief_[0];
    exclusiefMetIrqReset_[1] = static_cast<uint8_t>(np_IRQ);
    exclusiefMetIrqReset_[2] = static_cast<uint8_t>(np_RESET);
    aangemeldePinnen = { gedeeld_, 2, exclusiefMetIrqReset_, 3 };
  }
  #endif
};

#endif
