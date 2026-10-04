#ifndef EXTENDERSC16IS752SPI_H
#define EXTENDERSC16IS752SPI_H

struct ExtenderSC16IS752SPI : HardwareResourceTypeSPI {
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
  // DEFAULT — SC16IS752 SPI #1
  // ============================================================================
  #if EXTENDER_SC16IS752_SPI_AANTAL == 1
  ExtenderSC16IS752SPI(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin np_CS, HardwareResourcePin np_SCK, HardwareResourcePin np_MISO, HardwareResourcePin np_MOSI, HardwareResourcePin np_IRQ, HardwareResourcePin np_RESET, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : HardwareResourceTypeSPI(parent, component, np_CS, np_SCK, np_MISO, np_MOSI, Extender), np_IRQ(np_IRQ), np_RESET(np_RESET) {
    exclusiefMetIrqReset_[0] = static_cast<uint8_t>(np_CS);
    exclusiefMetIrqReset_[1] = static_cast<uint8_t>(np_IRQ);
    exclusiefMetIrqReset_[2] = static_cast<uint8_t>(np_RESET);
    aangemeldePinnen = { gedeeld_, 3, exclusiefMetIrqReset_, 3 };
  }
  #endif

  // ============================================================================
  // EXPERIMENTEEL — SC16IS752 SPI — AANTAL >= 2
  // ============================================================================
  #if EXTENDER_SC16IS752_SPI_AANTAL >= 2
  struct Sc16is752SpiPinnen { HardwareResourcePin csPin; HardwareResourcePin irq; HardwareResourcePin reset; };

  #define BUNDEL_EXTENDER(N) { CS_PIN_EXTENDER_SC16IS752_SPI_##N, EXTENDER_SC16IS752_SPI_##N##_IRQ, EXTENDER_SC16IS752_SPI_##N##_RESET }

  static constexpr Sc16is752SpiPinnen ExtenderLijst[] = {
    BUNDEL_EXTENDER(1),
    BUNDEL_EXTENDER(2)
  };

  #undef BUNDEL_EXTENDER

  ExtenderSC16IS752SPI(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin np_SCK, HardwareResourcePin np_MISO, HardwareResourcePin np_MOSI, uint8_t teller, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : HardwareResourceTypeSPI(parent, component, ExtenderLijst[teller].csPin, np_SCK, np_MISO, np_MOSI, Extender), np_IRQ(ExtenderLijst[teller].irq), np_RESET(ExtenderLijst[teller].reset) {
    exclusiefMetIrqReset_[0] = static_cast<uint8_t>(ExtenderLijst[teller].csPin);
    exclusiefMetIrqReset_[1] = static_cast<uint8_t>(np_IRQ);
    exclusiefMetIrqReset_[2] = static_cast<uint8_t>(np_RESET);
    aangemeldePinnen = { gedeeld_, 3, exclusiefMetIrqReset_, 3 };
  }
  #endif
};

#endif
