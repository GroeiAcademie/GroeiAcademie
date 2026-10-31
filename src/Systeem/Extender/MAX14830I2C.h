#ifndef EXTENDERMAX14830I2C_H
#define EXTENDERMAX14830I2C_H

struct ExtenderMAX14830I2C : HardwareResourceTypeI2C {
  enum class ExtenderPins : uint8_t {
    EP_UART0  = 0x00, // UART0: RX0/TX0 en bijhorende flow-controlfuncties
    EP_UART1  = 0x01, // UART1: RX1/TX1 en bijhorende flow-controlfuncties
    EP_UART2  = 0x02, // UART2: RX2/TX2 en bijhorende flow-controlfuncties
    EP_UART3  = 0x03, // UART3: RX3/TX3 en bijhorende flow-controlfuncties
    EP_GPIO0  = 0x04, // GPIO0, gekoppeld aan UART0
    EP_GPIO1  = 0x05, // GPIO1, gekoppeld aan UART0
    EP_GPIO2  = 0x06, // GPIO2, gekoppeld aan UART0
    EP_GPIO3  = 0x07, // GPIO3, gekoppeld aan UART0
    EP_GPIO4  = 0x08, // GPIO4, gekoppeld aan UART1
    EP_GPIO5  = 0x09, // GPIO5, gekoppeld aan UART1
    EP_GPIO6  = 0x0A, // GPIO6, gekoppeld aan UART1
    EP_GPIO7  = 0x0B, // GPIO7, gekoppeld aan UART1
    EP_GPIO8  = 0x0C, // GPIO8, gekoppeld aan UART2
    EP_GPIO9  = 0x0D, // GPIO9, gekoppeld aan UART2
    EP_GPIO10 = 0x0E, // GPIO10, gekoppeld aan UART2
    EP_GPIO11 = 0x0F, // GPIO11, gekoppeld aan UART2
    EP_GPIO12 = 0x10, // GPIO12, gekoppeld aan UART3
    EP_GPIO13 = 0x11, // GPIO13, gekoppeld aan UART3
    EP_GPIO14 = 0x12, // GPIO14, gekoppeld aan UART3
    EP_GPIO15 = 0x13  // GPIO15, gekoppeld aan UART3
  };

  HardwareResourcePin np_IRQ;
  uint8_t exclusiefMetIrq_[2];

  // Volgens MAX14830-datasheet: RHR/THR=0x00, TxFIFOLvl=0x11,
  // RxFIFOLvl=0x12, LCR=0x0B, BRG-divisor registers 0x1C/0x1D.
  bool MAX14830LeesRegister(uint8_t channel, uint8_t registerAdres, uint8_t& waarde) {

    if (channel > 3 || registerAdres > 0x25) return false;
    const uint8_t uartAdres = static_cast<uint8_t>((channel == 0 ? 0x60 : channel == 1 ? 0x50 : channel == 2 ? 0x20 : 0x10) | (adres & 0x0F));
    if (actief && !ComponentBeschikbaarTijdensRuntime()) return false;
    Wire.beginTransmission(uartAdres);
    Wire.write(registerAdres);
    if (Wire.endTransmission(false) != 0 || Wire.requestFrom(uartAdres, static_cast<uint8_t>(1)) != 1) return false;
    waarde = Wire.read();
    return true;
  }
  bool MAX14830SchrijfRegister(uint8_t channel, uint8_t registerAdres, uint8_t waarde) {

    if (channel > 3 || registerAdres > 0x1F) return false;
    const uint8_t uartAdres = static_cast<uint8_t>((channel == 0 ? 0x60 : channel == 1 ? 0x50 : channel == 2 ? 0x20 : 0x10) | (adres & 0x0F));
    if (actief && !ComponentBeschikbaarTijdensRuntime()) return false;
    Wire.beginTransmission(uartAdres);
    Wire.write(registerAdres);
    Wire.write(waarde);
    return Wire.endTransmission() == 0;
  }
  // De werkelijke kristal/clockfrequentie moet door de gebruiker zijn ingesteld.
  // Geen stilzwijgende aanname over de fysieke klok of PLL-configuratie.
  bool MAX14830StartUART(uint8_t channel, uint32_t baud) {
    if (channel > 3 || !baud || !EXTENDER_MAX14830_BRONKLOK_HZ) return false;
    // Referentieklok van de baudrategenerator: de bronklok, of na de PLL (bronklok / voordeler x vermenigvuldiger).
    const uint32_t referentieklok = EXTENDER_MAX14830_PLL_VOORDELER
      ? (EXTENDER_MAX14830_BRONKLOK_HZ / EXTENDER_MAX14830_PLL_VOORDELER) * EXTENDER_MAX14830_PLL_VERMENIGVULDIGER
      : EXTENDER_MAX14830_BRONKLOK_HZ;
    // Datasheet: D = fREF / (factor x baud), DIV = TRUNC(D) in DIVLSB/DIVMSB, FRACT = ROUND(16 x (D - DIV)) in BRGConfig[3:0].
    // Te hoge baud voor factor 16: 2x-modus (factor 8, BRGConfig bit 4) of 4x-modus (factor 4, BRGConfig bit 5).
    uint32_t factor = 16;
    uint8_t modus = 0x00;
    if (referentieklok / baud < 8) { factor = 4; modus = 0x20; }
    else if (referentieklok / baud < 16) { factor = 8; modus = 0x10; }
    const uint32_t noemer = factor * baud;
    uint32_t divisor = referentieklok / noemer;
    uint32_t breuk = ((referentieklok % noemer) * 16UL + noemer / 2UL) / noemer;
    if (breuk == 16) { ++divisor; breuk = 0; }
    if (divisor < 1 || divisor > 65535UL) return false;   // baud niet haalbaar met deze klok: weigeren, geen verkeerde baud
    if (!MAX14830SchrijfRegister(channel, 0x1B, static_cast<uint8_t>(modus | breuk))) return false;
    if (!MAX14830SchrijfRegister(channel, 0x1C, static_cast<uint8_t>(divisor))) return false;
    if (!MAX14830SchrijfRegister(channel, 0x1D, static_cast<uint8_t>(divisor >> 8))) return false;
    // LCR = 0x03: 8 databits, geen parity, één stopbit.
    if (!MAX14830SchrijfRegister(channel, 0x0B, 0x03)) return false;
    uartChannels[channel].baud = baud;   // voor de byte-tijd in flush()
    return true;
  }
  // Op een concrete MAX14830 zijn de vier kanaal-Streams onafhankelijke poorten.
  // Verbindingsstatus en UART-baud worden door de Extender beheerd.
  struct MAX14830ChannelStream : Stream {
    ExtenderMAX14830I2C* eigenaar = nullptr;
    uint8_t channel = 0;
    int peekWaarde = -1;
    uint32_t baud = 0;   // ingesteld door MAX14830StartUART(); 0 = onbekend
    void verbinden(ExtenderMAX14830I2C* node, uint8_t channelNummer) { eigenaar = node; channel = channelNummer; }
    int available() override {
      // Een al via peek() gelezen byte blijft beschikbaar, ook als het FIFO-niveau niet gelezen kan worden.
      const int bewaard = peekWaarde >= 0 ? 1 : 0;
      if (!eigenaar || !eigenaar->actief) return bewaard;
      uint8_t aantal = 0;
      if (!eigenaar->MAX14830LeesRegister(channel, 0x12, aantal)) return bewaard;
      return aantal + bewaard;
    }
    int read() override {
      if (peekWaarde >= 0) { const int v = peekWaarde; peekWaarde = -1; return v; }
      if (!eigenaar || !eigenaar->actief) return -1;
      uint8_t aantal = 0, resultaat = 0;
      if (!eigenaar->MAX14830LeesRegister(channel, 0x12, aantal) || !aantal ||
          !eigenaar->MAX14830LeesRegister(channel, 0x00, resultaat)) return -1;
      return resultaat;
    }
    int peek() override {
      if (peekWaarde < 0) peekWaarde = read();
      return peekWaarde;
    }
    void flush() override {
      // Wachten tot de TxFIFO leeg is (TxFIFOLvl 0x11 = 0), begrensd op 50 ms: geen oneindige blokkerende wachtlus.
      // Een lege FIFO betekent dat de laatste byte nog in het schuifregister kan zitten (één byte lang).
      if (!eigenaar || !eigenaar->actief) return;
      const unsigned long start = millis();
      uint8_t aantal = 0;
      while (millis() - start < 50UL) {
        if (!eigenaar->MAX14830LeesRegister(channel, 0x11, aantal)) return;
        if (aantal == 0) {
          // FIFO leeg: de laatste byte zit mogelijk nog in het schuifregister; één byte-tijd (10 bits) wachten.
          if (baud) delayMicroseconds(10000000UL / baud + 1);
          return;
        }
      }
    }
    size_t write(uint8_t waarde) override {
      if (!eigenaar || !eigenaar->actief) return 0;
      uint8_t aantal = 0;
      if (!eigenaar->MAX14830LeesRegister(channel, 0x11, aantal) || aantal >= 128 ||
          !eigenaar->MAX14830SchrijfRegister(channel, 0x00, waarde)) return 0;
      return 1;
    }
    using Print::write;
  };
  MAX14830ChannelStream uartChannels[4];
  // UART-contract: channel 0 tot en met 3.
  Stream* GetExtenderStream(uint8_t channel) override { return MAX14830GetStream(channel); }
  bool StartExtenderUART(uint8_t channel, uint32_t baud) override { return actief && MAX14830StartUART(channel, baud); }
  // Ook vóór de activering: een Sensor vraagt zijn Stream in zijn constructor op. De Stream controleert zelf bij elke oproep of de Extender actief is.
  Stream* MAX14830GetStream(uint8_t channel) {
    return channel <= 3 ? &uartChannels[channel] : nullptr;
  }

  bool Activeren() override {
    if (!ingeplugd || parent == nullptr || !parent->actief) return false;
    uint8_t revisie = 0;
    if (!(MAX14830LeesRegister(0, 0x25, revisie) && revisie != 0x00 && revisie != 0xFF)) return false;
    // Klokbron (globaal, via UART0): CLKSource 0x1E, met de PLL ook PLLConfig 0x1A.
    // CLKSource: bit 4 externe klok aan, bit 1 kristal, bit 2 PLL aan, bit 3 PLL omzeilen.
    uint8_t klokbron = static_cast<uint8_t>(0x10 | (EXTENDER_MAX14830_BRONKLOK_KRISTAL ? 0x02 : 0x00));
#if EXTENDER_MAX14830_PLL_VOORDELER
    // PLLConfig: bits 7:6 vermenigvuldiger (0 = 6, 1 = 48, 2 = 96, 3 = 144), bits 5:0 voordeler.
    const uint8_t pllFactor = EXTENDER_MAX14830_PLL_VERMENIGVULDIGER == 6 ? 0 : EXTENDER_MAX14830_PLL_VERMENIGVULDIGER == 48 ? 1 :
                              EXTENDER_MAX14830_PLL_VERMENIGVULDIGER == 96 ? 2 : 3;
    if (!MAX14830SchrijfRegister(0, 0x1A, static_cast<uint8_t>((pllFactor << 6) | EXTENDER_MAX14830_PLL_VOORDELER))) return false;
    klokbron |= 0x04;
#else
    klokbron |= 0x08;
#endif
    if (!MAX14830SchrijfRegister(0, 0x1E, klokbron)) return false;
#if EXTENDER_MAX14830_BRONKLOK_KRISTAL
    // Wachten tot het kristal stabiel is: STSInt 0x08, bit 5 = CLKREADY; maximaal 20 ms.
    for (uint8_t poging = 0; poging < 20; ++poging) {
      uint8_t status = 0;
      if (MAX14830LeesRegister(0, 0x08, status) && (status & 0x20)) return true;
      delay(1);
    }
    return false;
#else
    return true;
#endif
  }


  // ============================================================================
  // DEFAULT — MAX14830 I2C #1
  // ============================================================================
  #if EXTENDER_MAX14830_I2C_AANTAL == 1
  ExtenderMAX14830I2C(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t adres, HardwareResourcePin np_SDA, HardwareResourcePin np_SCL, HardwareResourcePin np_IRQ, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD, uint8_t tcaChannel = 0xFF)
    : HardwareResourceTypeI2C(parent, component, adres, np_SDA, np_SCL, Extender, tcaChannel), np_IRQ(np_IRQ) {
    exclusiefMetIrq_[0] = exclusief_[0];
    exclusiefMetIrq_[1] = static_cast<uint8_t>(np_IRQ);
    aangemeldePinnen = { gedeeld_, 2, exclusiefMetIrq_, 2 };
    for (uint8_t i = 0; i < 4; ++i) uartChannels[i].verbinden(this, i);
  }
  #endif

  // ============================================================================
  // EXPERIMENTEEL — MAX14830 I2C — AANTAL >= 2
  // ============================================================================
  #if EXTENDER_MAX14830_I2C_AANTAL >= 2
  struct Max14830I2cPinnen { uint8_t adres; HardwareResourcePin irq; };

  #define BUNDEL_EXTENDER(N) { I2C_ADDRESS_EXTENDER_MAX14830_I2C_##N, EXTENDER_MAX14830_I2C_##N##_IRQ }

  static constexpr Max14830I2cPinnen ExtenderLijst[] = {
    BUNDEL_EXTENDER(1),
    BUNDEL_EXTENDER(2)
  };

  #undef BUNDEL_EXTENDER

  ExtenderMAX14830I2C(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin np_SDA, HardwareResourcePin np_SCL, uint8_t teller, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD, uint8_t tcaChannel = 0xFF)
    : HardwareResourceTypeI2C(parent, component, ExtenderLijst[teller].adres, np_SDA, np_SCL, Extender, tcaChannel), np_IRQ(ExtenderLijst[teller].irq) {
    exclusiefMetIrq_[0] = exclusief_[0];
    exclusiefMetIrq_[1] = static_cast<uint8_t>(np_IRQ);
    aangemeldePinnen = { gedeeld_, 2, exclusiefMetIrq_, 2 };
    for (uint8_t i = 0; i < 4; ++i) uartChannels[i].verbinden(this, i);
  }
  #endif
};

#endif
