#ifndef EXTENDERMCP23S17_H
#define EXTENDERMCP23S17_H

struct ExtenderMCP23S17 : HardwareResourceTypeSPI, MCP23S17 {
  enum class ExtenderPins : uint8_t {
    EP_GPA0 = 0x00,
    EP_GPA1 = 0x01,
    EP_GPA2 = 0x02,
    EP_GPA3 = 0x03,
    EP_GPA4 = 0x04,
    EP_GPA5 = 0x05,
    EP_GPA6 = 0x06,
    EP_GPA7 = 0x07,
    EP_GPB0 = 0x08,
    EP_GPB1 = 0x09,
    EP_GPB2 = 0x0A,
    EP_GPB3 = 0x0B,
    EP_GPB4 = 0x0C,
    EP_GPB5 = 0x0D,
    EP_GPB6 = 0x0E,
    EP_GPB7 = 0x0F
  };

  // ============================================================================
  // DEFAULT: MCP23S17 #1
  // ============================================================================
  #if EXTENDER_MCP23S17_AANTAL == 1
  ExtenderMCP23S17(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin np_CS, HardwareResourcePin np_SCK, HardwareResourcePin np_MISO, HardwareResourcePin np_MOSI, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : HardwareResourceTypeSPI(parent, component, np_CS, np_SCK, np_MISO, np_MOSI, Extender), MCP23S17(NativeArduinoPinVan(np_CS), 0x00, &SPI) {}
  #endif

  // ============================================================================
  // EXPERIMENTEEL: MCP23S17 #2 en hoger
  // ============================================================================
  #if EXTENDER_MCP23S17_AANTAL >= 2
  struct Mcp23s17Pinnen { HardwareResourcePin csPin; };

  #define BUNDEL_EXTENDER(N) { CS_PIN_EXTENDER_MCP23S17_##N }

  static constexpr Mcp23s17Pinnen ExtenderLijst[] = {
    BUNDEL_EXTENDER(1),
    BUNDEL_EXTENDER(2)
  };

  #undef BUNDEL_EXTENDER

  ExtenderMCP23S17(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin np_SCK, HardwareResourcePin np_MISO, HardwareResourcePin np_MOSI, uint8_t teller, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : HardwareResourceTypeSPI(parent, component, ExtenderLijst[teller].csPin, np_SCK, np_MISO, np_MOSI, Extender), MCP23S17(NativeArduinoPinVan(ExtenderLijst[teller].csPin), 0x00, &SPI) {}
  #endif

  bool Activeren() override {
    return MCP23S17::begin() && MCP23S17::isConnected();
  }
};

#endif
