#ifndef EXTENDERSC16IS752I2C_H
#define EXTENDERSC16IS752I2C_H

struct ExtenderSC16IS752I2C : HardwareResourceTypeI2C {
  Appnostic_SC16IS752 channelA{SC16IS752_CHANNEL_A};
  Appnostic_SC16IS752 channelB{SC16IS752_CHANNEL_B};
  Stream& StreamChannelA() { return channelA; }
  Stream& StreamChannelB() { return channelB; }
  // UART-contract: channel 0 = A, 1 = B.
  Stream* GetExtenderStream(uint8_t channel) override { return channel == 0 ? static_cast<Stream*>(&channelA) : channel == 1 ? static_cast<Stream*>(&channelB) : nullptr; }
  bool StartExtenderUART(uint8_t channel, uint32_t baud) override {
    if (!actief || channel > 1 || baud == 0) return false;
    // setBaudrate() geeft geen resultaat (Appnostic_SC16IS752 1.0.0); het scratchpad van het gekozen channel bevestigt daarna de registertoegang.
    // ping() is hiervoor niet bruikbaar, want ping() test altijd channel A.
    (channel == 0 ? channelA : channelB).setBaudrate(baud);
    channelA.writeRegister(channel, SC16IS7XX_REG_SPR, 0x55);
    return channelA.readRegister(channel, SC16IS7XX_REG_SPR) == 0x55;
  }
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
  ExtenderSC16IS752I2C(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t adres, HardwareResourcePin np_SDA, HardwareResourcePin np_SCL, HardwareResourcePin np_IRQ, HardwareResourcePin np_RESET, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD, uint8_t tcaChannel = 0xFF)
    : HardwareResourceTypeI2C(parent, component, adres, np_SDA, np_SCL, Extender, tcaChannel), np_IRQ(np_IRQ), np_RESET(np_RESET) {
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

  ExtenderSC16IS752I2C(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin np_SDA, HardwareResourcePin np_SCL, uint8_t teller, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD, uint8_t tcaChannel = 0xFF)
    : HardwareResourceTypeI2C(parent, component, (teller < 2 ? ExtenderLijst[teller].adres : 0xFF), np_SDA, np_SCL, Extender, tcaChannel), np_IRQ((teller < 2 ? ExtenderLijst[teller].irq : HardwareResourcePin::NONE)), np_RESET((teller < 2 ? ExtenderLijst[teller].reset : HardwareResourcePin::NONE)) {
    exclusiefMetIrqReset_[0] = exclusief_[0];
    exclusiefMetIrqReset_[1] = static_cast<uint8_t>(np_IRQ);
    exclusiefMetIrqReset_[2] = static_cast<uint8_t>(np_RESET);
    aangemeldePinnen = { gedeeld_, 2, exclusiefMetIrqReset_, 3 };
  }
  #endif

  bool Activeren() override {
    // RESET actief laag: hoog zetten, zoals de TCA9548A, zodat een zwevende pin de chip niet in reset houdt.
    if (np_RESET != HardwareResourcePin::NONE) {
      digitalWrite(NativeArduinoPinVan(np_RESET), HIGH);
      pinMode(NativeArduinoPinVan(np_RESET), OUTPUT);
    }
    // De library rekent de baud met de kristalfrequentie; standaard 14745600 Hz, instelbaar in de config.
    channelA.setCrystalFrequency(EXTENDER_SC16IS752_BRONKLOK_HZ);
    channelB.setCrystalFrequency(EXTENDER_SC16IS752_BRONKLOK_HZ);
    // Appnostic begin_i2c configureert de globale Wire-klok op 400 kHz;
    // herstel de huidige FrameWork-standaard van 100 kHz.
    const bool gestartA = channelA.begin_i2c(adres);
    const bool gestartB = channelB.begin_i2c(adres);
    Wire.setClock(100000UL);
    const bool gelukt = gestartA && gestartB && channelA.ping();
    return FoutDiagnoseI2C(gelukt, 0, gelukt);
  }

  // Eigen register lezen/schrijven, buiten de library (die heeft geen GPIO-functies in de gebruikte API).
  // NXP SC16IS752: I2C-subadres = register << 3 (channel-bits 00 voor de gedeelde GPIO-registers).
  // Tijdens runtime (actief): eerst de I2C-toegang herstellen (bv. het TCA9548A-kanaal), daarna het resultaat aan de runtime-foutladder melden.
  // Tijdens de eigen activering (nog niet actief) meldt FoutDiagnoseI2C() de fout.
  bool SC16IS752LeesRegister(uint8_t registerAdres, uint8_t& waarde) {
    waarde = 0;
    if (actief && !ComponentBeschikbaarTijdensRuntime()) return false;
    Wire.beginTransmission(adres);
    Wire.write(static_cast<uint8_t>(registerAdres << 3));
    const bool gelukt = Wire.endTransmission(false) == 0 && Wire.requestFrom(adres, static_cast<uint8_t>(1)) == 1;
    if (gelukt) waarde = Wire.read();
    return actief ? ResultaatComponentToegangTijdensRuntimeVerwerken(gelukt) : gelukt;
  }
  bool SC16IS752SchrijfRegister(uint8_t registerAdres, uint8_t waarde) {
    if (actief && !ComponentBeschikbaarTijdensRuntime()) return false;
    Wire.beginTransmission(adres);
    Wire.write(static_cast<uint8_t>(registerAdres << 3));
    Wire.write(waarde);
    const bool gelukt = Wire.endTransmission() == 0;
    return actief ? ResultaatComponentToegangTijdensRuntimeVerwerken(gelukt) : gelukt;
  }
#if GEDEELDE_BUS_EXTENDER_GPIO_ACTIEF
  // GPIO0..GPIO7 (EP_GPIO0..EP_GPIO7). NXP SC16IS752: IODir 0x0A (1 = uitgang), IOState 0x0B.
  // IOControl blijft op de resetwaarde: GPIO-functie, niet de modempinnen. Geen interne pull-up: INPUT_PULLUP wordt geweigerd.
  bool SetExtenderPinMode(uint8_t pin, uint8_t modus) override {
    if (!actief || pin < static_cast<uint8_t>(ExtenderPins::EP_GPIO0) || pin > static_cast<uint8_t>(ExtenderPins::EP_GPIO7)) return false;
    if (modus != INPUT && modus != OUTPUT) return false;
    uint8_t richting = 0;
    if (!SC16IS752LeesRegister(0x0A, richting)) return false;
    const uint8_t bit = static_cast<uint8_t>(1U << (pin - static_cast<uint8_t>(ExtenderPins::EP_GPIO0)));
    richting = modus == OUTPUT ? static_cast<uint8_t>(richting | bit) : static_cast<uint8_t>(richting & ~bit);
    return SC16IS752SchrijfRegister(0x0A, richting);
  }
  bool WriteExtenderPin(uint8_t pin, uint8_t niveau) override {
    if (!actief || pin < static_cast<uint8_t>(ExtenderPins::EP_GPIO0) || pin > static_cast<uint8_t>(ExtenderPins::EP_GPIO7)) return false;
    uint8_t data = 0;
    if (!SC16IS752LeesRegister(0x0B, data)) return false;
    const uint8_t bit = static_cast<uint8_t>(1U << (pin - static_cast<uint8_t>(ExtenderPins::EP_GPIO0)));
    data = niveau == HIGH ? static_cast<uint8_t>(data | bit) : static_cast<uint8_t>(data & ~bit);
    return SC16IS752SchrijfRegister(0x0B, data);
  }
  uint8_t ReadExtenderPin(uint8_t pin) override {
    if (!actief || pin < static_cast<uint8_t>(ExtenderPins::EP_GPIO0) || pin > static_cast<uint8_t>(ExtenderPins::EP_GPIO7)) return 0xFF;
    uint8_t data = 0;
    if (!SC16IS752LeesRegister(0x0B, data)) return 0xFF;
    return (data >> (pin - static_cast<uint8_t>(ExtenderPins::EP_GPIO0))) & 0x01 ? HIGH : LOW;
  }
#endif

  // 1-Wire-contract via een UART-channel (Maxim AN214): reset op 9600 baud, bits op 115200 baud.
  // Hardware: TX en RX van het channel samen op DQ via een open-drain schakeling (bv. diode of transistor) met pull-up.
  // Eén 1-Wire-apparaat per channel: zoeken gebeurt met READ ROM (0x33), niet met het volledige zoekalgoritme.
  uint8_t oneWireChannel = 0xFF;
  bool oneWireReadROMsearchDone = false;
  // aantal UART-bytes versturen en hun echo terug lezen (één byte = één 1-Wire-tijdslot).
  bool OneWireUARTbits(const uint8_t* uit, uint8_t* in, uint8_t aantal) {
    Stream* poort = GetExtenderStream(oneWireChannel);
    if (poort == nullptr) return false;
    // Hoogstens 128 oude bytes weggooien; raakt de buffer niet leeg of mislukt read(), dan is de UART niet bruikbaar.
    for (uint8_t i = 0; poort->available() > 0; ++i) {
      if (i >= 128 || poort->read() < 0) return false;
    }
    for (uint8_t i = 0; i < aantal; ++i) poort->write(uit[i]);
    const unsigned long start = millis();
    for (uint8_t i = 0; i < aantal; ++i) {
      while (poort->available() <= 0) {
        if (millis() - start > 20UL) return false;   // geen echo: TX/RX niet verbonden of busfout
      }
      const int waarde = poort->read();
      if (waarde < 0) return false;   // read() gaf geen geldige byte: geen echo, dus ook geen aanwezigheidspuls
      in[i] = static_cast<uint8_t>(waarde);
    }
    return true;
  }
  bool ExtenderOneWireSelectChannel(uint8_t channel) override {
    if (!actief || GetExtenderStream(channel) == nullptr) return false;
    oneWireChannel = channel;
    return true;
  }
  bool ExtenderOneWireSearchReset() override { oneWireReadROMsearchDone = false; return oneWireChannel != 0xFF; }
  bool ExtenderOneWireReset() override {
    if (!StartExtenderUART(oneWireChannel, 9600UL)) return false;
    const uint8_t puls = 0xF0;
    uint8_t antwoord = 0xF0;
    const bool gelukt = OneWireUARTbits(&puls, &antwoord, 1);
    if (!StartExtenderUART(oneWireChannel, 115200UL)) return false;
    return gelukt && antwoord != 0xF0;   // een gewijzigde echo is de aanwezigheidspuls
  }
  bool ExtenderOneWireWriteByte(uint8_t waarde) override {
    uint8_t uit[8], in[8];
    for (uint8_t i = 0; i < 8; ++i) uit[i] = ((waarde >> i) & 0x01) ? 0xFF : 0x00;   // LSB eerst
    return OneWireUARTbits(uit, in, 8);
  }
  bool ExtenderOneWireReadByte(uint8_t* waarde) override {
    uint8_t uit[8], in[8];
    for (uint8_t i = 0; i < 8; ++i) uit[i] = 0xFF;
    if (waarde == nullptr || !OneWireUARTbits(uit, in, 8)) return false;
    *waarde = 0;
    for (uint8_t i = 0; i < 8; ++i) if (in[i] == 0xFF) *waarde |= static_cast<uint8_t>(1U << i);
    return true;
  }
  bool ExtenderOneWireSearch(uint8_t* adres) override {
    if (adres == nullptr || oneWireReadROMsearchDone) return false;
    oneWireReadROMsearchDone = true;
    if (!ExtenderOneWireReset() || !ExtenderOneWireWriteByte(0x33)) return false;   // READ ROM
    for (uint8_t i = 0; i < 8; ++i) if (!ExtenderOneWireReadByte(&adres[i])) return false;
    // Dallas CRC8 over de eerste 7 bytes; meerdere apparaten op het channel geven een CRC-fout.
    uint8_t crc = 0;
    for (uint8_t i = 0; i < 7; ++i) {
      uint8_t b = adres[i];
      for (uint8_t j = 0; j < 8; ++j) {
        const uint8_t mix = static_cast<uint8_t>((crc ^ b) & 0x01);
        crc >>= 1;
        if (mix) crc ^= 0x8C;
        b >>= 1;
      }
    }
    return crc == adres[7];
  }
};

#endif
