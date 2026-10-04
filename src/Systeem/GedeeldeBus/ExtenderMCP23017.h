#ifndef EXTENDERMCP23017_H
#define EXTENDERMCP23017_H

struct ExtenderMCP23017 : HardwareResourceTypeI2C {
  enum class ExtenderPins : uint8_t {
    EP_GPA0 = 0x00, // Digitale I/O GPA0, bank A
    EP_GPA1 = 0x01, // Digitale I/O GPA1, bank A
    EP_GPA2 = 0x02, // Digitale I/O GPA2, bank A
    EP_GPA3 = 0x03, // Digitale I/O GPA3, bank A
    EP_GPA4 = 0x04, // Digitale I/O GPA4, bank A
    EP_GPA5 = 0x05, // Digitale I/O GPA5, bank A
    EP_GPA6 = 0x06, // Digitale I/O GPA6, bank A
    EP_GPA7 = 0x07, // Digitale I/O GPA7, bank A
    EP_GPB0 = 0x08, // Digitale I/O GPB0, bank B
    EP_GPB1 = 0x09, // Digitale I/O GPB1, bank B
    EP_GPB2 = 0x0A, // Digitale I/O GPB2, bank B
    EP_GPB3 = 0x0B, // Digitale I/O GPB3, bank B
    EP_GPB4 = 0x0C, // Digitale I/O GPB4, bank B
    EP_GPB5 = 0x0D, // Digitale I/O GPB5, bank B
    EP_GPB6 = 0x0E, // Digitale I/O GPB6, bank B
    EP_GPB7 = 0x0F  // Digitale I/O GPB7, bank B
  };

  HardwareResourcePin np_INTA, np_INTB, np_RESET;
  uint8_t exclusiefMetIntReset_[4];

  // ============================================================================
  // DEFAULT — MCP23017 #1
  // ============================================================================
  #if EXTENDER_MCP23017_AANTAL == 1
  ExtenderMCP23017(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t adres, HardwareResourcePin np_SDA, HardwareResourcePin np_SCL, HardwareResourcePin np_INTA, HardwareResourcePin np_INTB, HardwareResourcePin np_RESET, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : HardwareResourceTypeI2C(parent, component, adres, np_SDA, np_SCL, Extender), np_INTA(np_INTA), np_INTB(np_INTB), np_RESET(np_RESET) {
    exclusiefMetIntReset_[0] = exclusief_[0];
    exclusiefMetIntReset_[1] = static_cast<uint8_t>(np_INTA);
    exclusiefMetIntReset_[2] = static_cast<uint8_t>(np_INTB);
    exclusiefMetIntReset_[3] = static_cast<uint8_t>(np_RESET);
    aangemeldePinnen = { gedeeld_, 2, exclusiefMetIntReset_, 4 };
  }
  #endif

  // ============================================================================
  // EXPERIMENTEEL — MCP23017 — AANTAL >= 2
  // ============================================================================
  #if EXTENDER_MCP23017_AANTAL >= 2
  struct Mcp23017Pinnen { uint8_t adres; HardwareResourcePin inta; HardwareResourcePin intb; HardwareResourcePin reset; };

  #define BUNDEL_EXTENDER(N) { I2C_ADDRESS_EXTENDER_MCP23017_##N, EXTENDER_MCP23017_##N##_INTA, EXTENDER_MCP23017_##N##_INTB, EXTENDER_MCP23017_##N##_RESET }

  static constexpr Mcp23017Pinnen ExtenderLijst[] = {
    BUNDEL_EXTENDER(1),
    BUNDEL_EXTENDER(2)
  };

  #undef BUNDEL_EXTENDER

  ExtenderMCP23017(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin np_SDA, HardwareResourcePin np_SCL, uint8_t teller, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : HardwareResourceTypeI2C(parent, component, ExtenderLijst[teller].adres, np_SDA, np_SCL, Extender), np_INTA(ExtenderLijst[teller].inta), np_INTB(ExtenderLijst[teller].intb), np_RESET(ExtenderLijst[teller].reset) {
    exclusiefMetIntReset_[0] = exclusief_[0];
    exclusiefMetIntReset_[1] = static_cast<uint8_t>(np_INTA);
    exclusiefMetIntReset_[2] = static_cast<uint8_t>(np_INTB);
    exclusiefMetIntReset_[3] = static_cast<uint8_t>(np_RESET);
    aangemeldePinnen = { gedeeld_, 2, exclusiefMetIntReset_, 4 };
  }
  #endif
};

#endif
