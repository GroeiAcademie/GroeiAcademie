#ifndef EXTENDERADS7953_H
#define EXTENDERADS7953_H

struct ExtenderADS7953 : HardwareResourceTypeSPI {
  enum class NativeClientPin : uint8_t { EXTENDER_CLIENT_PIN_GPIO0, EXTENDER_CLIENT_PIN_GPIO1, EXTENDER_CLIENT_PIN_GPIO2, EXTENDER_CLIENT_PIN_GPIO3 };

  enum class ExtenderPins : uint8_t {
    EP_CH0   = 0x00, // Analoge ingang kanaal 0
    EP_CH1   = 0x01, // Analoge ingang kanaal 1
    EP_CH2   = 0x02, // Analoge ingang kanaal 2
    EP_CH3   = 0x03, // Analoge ingang kanaal 3
    EP_CH4   = 0x04, // Analoge ingang kanaal 4
    EP_CH5   = 0x05, // Analoge ingang kanaal 5
    EP_CH6   = 0x06, // Analoge ingang kanaal 6
    EP_CH7   = 0x07, // Analoge ingang kanaal 7
    EP_CH8   = 0x08, // Analoge ingang kanaal 8
    EP_CH9   = 0x09, // Analoge ingang kanaal 9
    EP_CH10  = 0x0A, // Analoge ingang kanaal 10
    EP_CH11  = 0x0B, // Analoge ingang kanaal 11
    EP_CH12  = 0x0C, // Analoge ingang kanaal 12
    EP_CH13  = 0x0D, // Analoge ingang kanaal 13
    EP_CH14  = 0x0E, // Analoge ingang kanaal 14
    EP_CH15  = 0x0F, // Analoge ingang kanaal 15

    EP_GPIO0 = 0x10, // General-purpose digitale I/O GPIO0
    EP_GPIO1 = 0x11, // General-purpose digitale I/O GPIO1 (TSSOP)
    EP_GPIO2 = 0x12, // General-purpose digitale I/O GPIO2 (TSSOP)
    EP_GPIO3 = 0x13  // General-purpose digitale I/O GPIO3 (TSSOP)
  };

  uint8_t exclusiefMetGpio_[5];

  // ============================================================================
  // DEFAULT — ADS7953 #1
  // ============================================================================
  #if EXTENDER_ADS7953_AANTAL == 1
  ExtenderADS7953(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin np_CS, HardwareResourcePin np_SCK, HardwareResourcePin np_MISO, HardwareResourcePin np_MOSI, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : HardwareResourceTypeSPI(parent, component, np_CS, np_SCK, np_MISO, np_MOSI, Extender) {
    exclusiefMetGpio_[0] = static_cast<uint8_t>(np_CS);
    exclusiefMetGpio_[1 + static_cast<uint8_t>(NativeClientPin::EXTENDER_CLIENT_PIN_GPIO0)] = static_cast<uint8_t>(EXTENDER_ADS7953_GPIO0_TO_UNO_1);
    exclusiefMetGpio_[1 + static_cast<uint8_t>(NativeClientPin::EXTENDER_CLIENT_PIN_GPIO1)] = static_cast<uint8_t>(EXTENDER_ADS7953_GPIO1_TO_UNO_1);
    exclusiefMetGpio_[1 + static_cast<uint8_t>(NativeClientPin::EXTENDER_CLIENT_PIN_GPIO2)] = static_cast<uint8_t>(EXTENDER_ADS7953_GPIO2_TO_UNO_1);
    exclusiefMetGpio_[1 + static_cast<uint8_t>(NativeClientPin::EXTENDER_CLIENT_PIN_GPIO3)] = static_cast<uint8_t>(EXTENDER_ADS7953_GPIO3_TO_UNO_1);
    aangemeldePinnen = { gedeeld_, 3, exclusiefMetGpio_, 5 };
  }
  #endif

  // ============================================================================
  // EXPERIMENTEEL — ADS7953 — AANTAL >= 2
  // ============================================================================
  #if EXTENDER_ADS7953_AANTAL >= 2
  struct Ads7953Pinnen { HardwareResourcePin csPin; HardwareResourcePin gpio0; HardwareResourcePin gpio1; HardwareResourcePin gpio2; HardwareResourcePin gpio3; };

  #define BUNDEL_EXTENDER(N) { \
    CS_PIN_EXTENDER_ADS7953_##N, \
    EXTENDER_ADS7953_GPIO0_TO_UNO_##N, \
    EXTENDER_ADS7953_GPIO1_TO_UNO_##N, \
    EXTENDER_ADS7953_GPIO2_TO_UNO_##N, \
    EXTENDER_ADS7953_GPIO3_TO_UNO_##N \
  }

  static constexpr Ads7953Pinnen ExtenderLijst[] = {
    BUNDEL_EXTENDER(1),
    BUNDEL_EXTENDER(2)
  };

  #undef BUNDEL_EXTENDER

  ExtenderADS7953(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin np_SCK, HardwareResourcePin np_MISO, HardwareResourcePin np_MOSI, uint8_t teller, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : HardwareResourceTypeSPI(parent, component, ExtenderLijst[teller].csPin, np_SCK, np_MISO, np_MOSI, Extender) {
    exclusiefMetGpio_[0] = static_cast<uint8_t>(ExtenderLijst[teller].csPin);
    exclusiefMetGpio_[1 + static_cast<uint8_t>(NativeClientPin::EXTENDER_CLIENT_PIN_GPIO0)] = static_cast<uint8_t>(ExtenderLijst[teller].gpio0);
    exclusiefMetGpio_[1 + static_cast<uint8_t>(NativeClientPin::EXTENDER_CLIENT_PIN_GPIO1)] = static_cast<uint8_t>(ExtenderLijst[teller].gpio1);
    exclusiefMetGpio_[1 + static_cast<uint8_t>(NativeClientPin::EXTENDER_CLIENT_PIN_GPIO2)] = static_cast<uint8_t>(ExtenderLijst[teller].gpio2);
    exclusiefMetGpio_[1 + static_cast<uint8_t>(NativeClientPin::EXTENDER_CLIENT_PIN_GPIO3)] = static_cast<uint8_t>(ExtenderLijst[teller].gpio3);
    aangemeldePinnen = { gedeeld_, 3, exclusiefMetGpio_, 5 };
  }
  #endif
};

#endif
