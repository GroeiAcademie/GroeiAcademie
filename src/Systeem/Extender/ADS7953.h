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

  // TI ADS7953, manual-mode SPI: de geselecteerde CH verschijnt pas twee frames later.
  bool metingBezig = false;
  uint8_t metingChannel = 0;
  uint16_t laatsteCommando = 0;
  uint16_t effectiefBereikMillivolt = 0;
  uint16_t GetExtenderReadAnalogEffectiefBereikMillivolt() const override { return effectiefBereikMillivolt; }

  bool Activeren() override {
    metingBezig = false;
    effectiefBereikMillivolt = 0;
    if (!ingeplugd || !parent || !parent->actief) return false;
    // ADS7953 manual mode: de gevraagde kanaaltag komt na de pijplijnvertraging terug.
    // Controleer twee verschillende kanalen zodat een vast laag/hoog MISO-signaal niet als chiprespons telt.
    uint16_t antwoord = 0;
    if (!ADCTransferFrame(0x1800, antwoord) || !ADCTransferFrame(0x1800, antwoord)) return false;
    if (!ADCTransferFrame(0x1800, antwoord) || (antwoord >> 12) != 0) return false;
    if (!ADCTransferFrame(0x1880, antwoord) || !ADCTransferFrame(0x1880, antwoord)) return false;
    return ADCTransferFrame(0x1880, antwoord) && (antwoord >> 12) == 1;
  }

  // één SPI-frame: eigen CS gedurende de 16 klokslagen actief.
  bool ADCTransferFrame(uint16_t commando, uint16_t& antwoord) {
    antwoord = 0;
    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
    if (!WriteCS(LOW)) { SPI.endTransaction(); return false; }
    antwoord = (static_cast<uint16_t>(SPI.transfer(static_cast<uint8_t>(commando >> 8))) << 8) |
               SPI.transfer(static_cast<uint8_t>(commando));
    const bool gelukt = WriteCS(HIGH);
    SPI.endTransaction();
    return gelukt;
  }

  int8_t StartExtenderReadAnalog(uint8_t pin, uint16_t bereikMillivolt) override {
    if (pin > static_cast<uint8_t>(ExtenderPins::EP_CH15)) return -2;
    // De chip heeft een externe REF. Bereik 0 blijft een zuivere capability-check.
    if (bereikMillivolt == 0) return 1;
    if (bereikMillivolt > EXTENDER_ADS7953_REFERENTIE_MILLIVOLT * 2UL) return -2;
    // TI: 2xVREF is alleen geldig als de analoge voeding minstens 2xVREF is.
    // Zonder expliciete voedingsspanning kan dit bereik niet veilig beloofd worden.
    if (bereikMillivolt > EXTENDER_ADS7953_REFERENTIE_MILLIVOLT &&
        (EXTENDER_ADS7953_VOEDING_MILLIVOLT == 0 ||
         EXTENDER_ADS7953_VOEDING_MILLIVOLT < EXTENDER_ADS7953_REFERENTIE_MILLIVOLT * 2UL)) return -2;
    if (EXTENDER_ADS7953_VOEDING_MILLIVOLT != 0 &&
        EXTENDER_ADS7953_VOEDING_MILLIVOLT < EXTENDER_ADS7953_REFERENTIE_MILLIVOLT) return -2;
    if (metingBezig) return 0;
    if (!actief) return -1;
    metingChannel = pin;
    // DI15..12=0001: manual mode; DI11=1: range/programming geldig;
    // DI10..07: channel; DI06: 0..REF of 0..2*REF.
    const bool dubbelBereik = bereikMillivolt > EXTENDER_ADS7953_REFERENTIE_MILLIVOLT;
    const uint16_t commando = static_cast<uint16_t>(0x1800 | (pin << 7) | (dubbelBereik ? 0x40 : 0));
    laatsteCommando = commando;
    uint16_t antwoord = 0;
    if (!ADCTransferFrame(commando, antwoord)) return -1;
    effectiefBereikMillivolt = static_cast<uint16_t>(EXTENDER_ADS7953_REFERENTIE_MILLIVOLT * (dubbelBereik ? 2UL : 1UL));
    metingBezig = true;
    return 1;
  }

  bool IsExtenderReadAnalogReady() override { return true; }
  bool GetExtenderReadAnalogValue(int32_t& waarde) override {
    waarde = 0;
    if (!metingBezig) return false;
    metingBezig = false;
    if (!actief) return false;
    // Frame N: channel aanvragen. Frame N+1: selectie; frame N+2: sample CHn.
    // Handhaaf de manual-mode en channelkeuze tijdens beide vervolgframes.
    uint16_t antwoord = 0;
    if (!ADCTransferFrame(laatsteCommando, antwoord) || !ADCTransferFrame(laatsteCommando, antwoord)) return false;
    if ((antwoord >> 12) != metingChannel) return false;
    const uint16_t ruw = antwoord & 0x0FFF;
#if ADC_BITS >= 12
    waarde = static_cast<int32_t>(ruw) << (ADC_BITS - 12);
#else
    waarde = static_cast<int32_t>(ruw) >> (12 - ADC_BITS);
#endif
    return true;
  }


  // ============================================================================
  // DEFAULT: ADS7953 #1
  // ============================================================================
  #if EXTENDER_ADS7953_AANTAL == 1
  ExtenderADS7953(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin np_CS, HardwareResourcePin np_SCK, HardwareResourcePin np_MISO, HardwareResourcePin np_MOSI, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : HardwareResourceTypeSPI(parent, component, np_CS, np_SCK, np_MISO, np_MOSI, Extender) {
    exclusiefMetGpio_[0] = static_cast<uint8_t>(np_CS);
    exclusiefMetGpio_[1 + static_cast<uint8_t>(NativeClientPin::EXTENDER_CLIENT_PIN_GPIO0)] = static_cast<uint8_t>(EXTENDER_ADS7953_1_GPIO0_TO_UNO);
    exclusiefMetGpio_[1 + static_cast<uint8_t>(NativeClientPin::EXTENDER_CLIENT_PIN_GPIO1)] = static_cast<uint8_t>(EXTENDER_ADS7953_1_GPIO1_TO_UNO);
    exclusiefMetGpio_[1 + static_cast<uint8_t>(NativeClientPin::EXTENDER_CLIENT_PIN_GPIO2)] = static_cast<uint8_t>(EXTENDER_ADS7953_1_GPIO2_TO_UNO);
    exclusiefMetGpio_[1 + static_cast<uint8_t>(NativeClientPin::EXTENDER_CLIENT_PIN_GPIO3)] = static_cast<uint8_t>(EXTENDER_ADS7953_1_GPIO3_TO_UNO);
    aangemeldePinnen = { gedeeld_, 3, exclusiefMetGpio_, 5 };
  }
  #endif

  // ============================================================================
  // EXPERIMENTEEL: ADS7953 #2
  // ============================================================================
  #if EXTENDER_ADS7953_AANTAL >= 2
  struct Ads7953Pinnen { HardwareResourcePin csPin; HardwareResourcePin gpio0; HardwareResourcePin gpio1; HardwareResourcePin gpio2; HardwareResourcePin gpio3; };

  #define BUNDEL_EXTENDER(N) { \
    EXTENDER_ADS7953_##N##_CS_PIN, \
    EXTENDER_ADS7953_##N##_GPIO0_TO_UNO, \
    EXTENDER_ADS7953_##N##_GPIO1_TO_UNO, \
    EXTENDER_ADS7953_##N##_GPIO2_TO_UNO, \
    EXTENDER_ADS7953_##N##_GPIO3_TO_UNO \
  }

  static constexpr Ads7953Pinnen ExtenderLijst[] = {
    BUNDEL_EXTENDER(1),
    BUNDEL_EXTENDER(2)
  };

  #undef BUNDEL_EXTENDER

  ExtenderADS7953(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin np_SCK, HardwareResourcePin np_MISO, HardwareResourcePin np_MOSI, uint8_t teller, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : HardwareResourceTypeSPI(parent, component, (teller < 2 ? ExtenderLijst[teller].csPin : HardwareResourcePin::NONE), np_SCK, np_MISO, np_MOSI, Extender) {
    exclusiefMetGpio_[0] = static_cast<uint8_t>((teller < 2 ? ExtenderLijst[teller].csPin : HardwareResourcePin::NONE));
    exclusiefMetGpio_[1 + static_cast<uint8_t>(NativeClientPin::EXTENDER_CLIENT_PIN_GPIO0)] = static_cast<uint8_t>((teller < 2 ? ExtenderLijst[teller].gpio0 : HardwareResourcePin::NONE));
    exclusiefMetGpio_[1 + static_cast<uint8_t>(NativeClientPin::EXTENDER_CLIENT_PIN_GPIO1)] = static_cast<uint8_t>((teller < 2 ? ExtenderLijst[teller].gpio1 : HardwareResourcePin::NONE));
    exclusiefMetGpio_[1 + static_cast<uint8_t>(NativeClientPin::EXTENDER_CLIENT_PIN_GPIO2)] = static_cast<uint8_t>((teller < 2 ? ExtenderLijst[teller].gpio2 : HardwareResourcePin::NONE));
    exclusiefMetGpio_[1 + static_cast<uint8_t>(NativeClientPin::EXTENDER_CLIENT_PIN_GPIO3)] = static_cast<uint8_t>((teller < 2 ? ExtenderLijst[teller].gpio3 : HardwareResourcePin::NONE));
    aangemeldePinnen = { gedeeld_, 3, exclusiefMetGpio_, 5 };
  }
  #endif
};

#endif
