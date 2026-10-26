#ifndef EXTENDERADS1115_H
#define EXTENDERADS1115_H

struct ExtenderADS1115 : HardwareResourceTypeI2C, ADS1115 {
  enum class ExtenderPins : uint8_t {
    EP_AIN0 = 0x00, // Analoge ingang AIN0
    EP_AIN1 = 0x01, // Analoge ingang AIN1
    EP_AIN2 = 0x02, // Analoge ingang AIN2
    EP_AIN3 = 0x03  // Analoge ingang AIN3
  };

  HardwareResourcePin np_ALERT_RDY;
  uint8_t exclusiefMetAlert_[2];

  // ============================================================================
  // ADS1115-BASIS: één ADS1115 als Extender (componentcode ADC_ADS1115); zijn sensoren, bv. RFP602, worden erop ingeplugd
  // ============================================================================
  ExtenderADS1115(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t adres, HardwareResourcePin np_SDA, HardwareResourcePin np_SCL, HardwareResourcePin np_ALERT_RDY, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : HardwareResourceTypeI2C(parent, component, adres, np_SDA, np_SCL, Extender), ADS1115(adres, &Wire), np_ALERT_RDY(np_ALERT_RDY) {
    exclusiefMetAlert_[0] = exclusief_[0];
    exclusiefMetAlert_[1] = static_cast<uint8_t>(np_ALERT_RDY);
    aangemeldePinnen = { gedeeld_, 2, exclusiefMetAlert_, 2 };
  }


  bool Activeren() override {
    // R2: ook na runtime-herstel geen oude meetstatus laten hangen.
    metingBezig = false;
    metingMislukt = false;
    InitialiserenGedeeldeBus(GedeeldeBusType::I2C);
    // Geen teruglezen: het configregister behoudt na een reset van het board de waarde van de vorige schets, dus er is geen vaste waarde om mee te vergelijken.
    const bool gelukt = ADS1115::begin() && ADS1115::isConnected();
    return FoutDiagnoseI2C(gelukt, ADS1115::getError(), gelukt);
  }

  // V8: analoge meting voor een child (bv. RFP602), zonder dat die de ADS1115 kent.
  // De gain wordt per meting gekozen uit bereikMillivolt; ADS1X15 schrijft hem bij elke meting toch mee naar het configregister.
  // Runtime-herstel (D042) blijft bij de child: die kiest de ladder (kanaal, sensor of deze ADS1115) en meldt het ene resultaat.
  bool metingMislukt = false;
  bool metingBezig = false;   // R2: een gestarte meting is nog niet opgehaald; een andere child krijgt 0 (bezet)

  int8_t StartExtenderReadAnalog(uint8_t pin, uint16_t bereikMillivolt) override {
    if (pin > static_cast<uint8_t>(ExtenderPins::EP_AIN3)) return -2;
    if (bereikMillivolt == 0) return 1;   // alleen controle: geen I2C, niets gewijzigd
    if (metingBezig) return 0;
    // Kleinste bereik dat bereikMillivolt nog omvat: 256, 512, 1024, 2048, 4096 of 6144 mV.
    uint8_t gain = 0;   // 6144 mV
    if      (bereikMillivolt <= 256)  gain = 16;
    else if (bereikMillivolt <= 512)  gain = 8;
    else if (bereikMillivolt <= 1024) gain = 4;
    else if (bereikMillivolt <= 2048) gain = 2;
    else if (bereikMillivolt <= 4096) gain = 1;
    ADS1115::setGain(gain);
    ADS1115::requestADC(pin);
    if (ADS1115::getError() != ADS1X15_OK) return -1;   // niet bezet: er loopt geen meting
    metingMislukt = false;
    metingBezig = true;
    return 1;
  }

  bool IsExtenderReadAnalogReady() override {
    if (!metingBezig || metingMislukt) return true;   // geen meting of een fout: niet blijven wachten
    const bool klaar = ADS1115::isReady();
    if (ADS1115::getError() != ADS1X15_OK) { metingMislukt = true; return true; }
    return klaar;
  }

  bool GetExtenderReadAnalogValue(int32_t& waarde) override {
    waarde = 0;
    if (!metingBezig) return false;
    metingBezig = false;   // R2: Get geeft altijd vrij, ook vóór de meting klaar is (getValue() leest alleen het register)
    if (metingMislukt) { metingMislukt = false; return false; }
    // Nooit een oude registerwaarde als nieuwe meting: is de conversie nog niet klaar, dan vrijgegeven en false.
    const bool klaar = ADS1115::isReady();
    if (ADS1115::getError() != ADS1X15_OK || !klaar) return false;
    const int16_t ruw = ADS1115::getValue();
    if (ADS1115::getError() != ADS1X15_OK) return false;
    // Single-ended: 0 tot 32767 (15 bit). Geschaald naar ADC_BITS (SystemConfig.h).
    waarde = static_cast<int32_t>(ruw) >> (15 - ADC_BITS);
    return true;
  }

  // ============================================================================
  // EXPERIMENTEEL — ADS1115 — AANTAL >= 2
  // ============================================================================
  #if EXTENDER_ADS1115_AANTAL >= 2
  struct Ads1115Pinnen { uint8_t adres; HardwareResourcePin alertRdy; };

  #define BUNDEL_EXTENDER(N) { I2C_ADDRESS_EXTENDER_ADS1115_##N, EXTENDER_ADS1115_##N##_ALERT_RDY }

  static constexpr Ads1115Pinnen ExtenderLijst[] = {
    BUNDEL_EXTENDER(1),
    BUNDEL_EXTENDER(2)
  };

  #undef BUNDEL_EXTENDER

  ExtenderADS1115(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin np_SDA, HardwareResourcePin np_SCL, uint8_t teller, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : HardwareResourceTypeI2C(parent, component, ExtenderLijst[teller].adres, np_SDA, np_SCL, Extender), ADS1115(ExtenderLijst[teller].adres, &Wire), np_ALERT_RDY(ExtenderLijst[teller].alertRdy) {
    exclusiefMetAlert_[0] = exclusief_[0];
    exclusiefMetAlert_[1] = static_cast<uint8_t>(np_ALERT_RDY);
    aangemeldePinnen = { gedeeld_, 2, exclusiefMetAlert_, 2 };
  }
  #endif
};

#endif
