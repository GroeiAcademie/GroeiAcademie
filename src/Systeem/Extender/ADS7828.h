#ifndef EXTENDERADS7828_H
#define EXTENDERADS7828_H

struct ExtenderADS7828 : HardwareResourceTypeI2C {
  enum class ExtenderPins : uint8_t {
    EP_CH0 = 0x00, // Analoge ingang kanaal 0
    EP_CH1 = 0x01, // Analoge ingang kanaal 1
    EP_CH2 = 0x02, // Analoge ingang kanaal 2
    EP_CH3 = 0x03, // Analoge ingang kanaal 3
    EP_CH4 = 0x04, // Analoge ingang kanaal 4
    EP_CH5 = 0x05, // Analoge ingang kanaal 5
    EP_CH6 = 0x06, // Analoge ingang kanaal 6
    EP_CH7 = 0x07  // Analoge ingang kanaal 7
  };

  // Bestaand GedeeldeBus Start/IsReady/Get-contract, met interne 2,5 V referentie.
  // TI ADS7828 datasheet: kanaalcode 0,4,1,5,2,6,3,7 voor CH0..CH7.
  // Geen bereikautomatiek: dit type heeft geen programmeerbare gain.
  bool metingBezig = false;
  unsigned long tijdstipReferentieInschakelen = 0;
  uint16_t GetExtenderReadAnalogEffectiefBereikMillivolt() const override {
    return EXTENDER_ADS7828_EXTERNE_REFERENTIE ? EXTENDER_ADS7828_REFERENTIE_MILLIVOLT : 2500;
  }

  bool Activeren() override {
    metingBezig = false;
    // TI ADS7828: de interne referentie is na inschakelen uit.
    // PD1=1 schakelt ze pas in na een STOP of repeated START.
    // De eerste meting mag pas gebeuren nadat de referentie gestabiliseerd is.
    Wire.beginTransmission(adres);
    Wire.write(static_cast<uint8_t>(EXTENDER_ADS7828_EXTERNE_REFERENTIE ? 0x84 : 0x8C)); // CH0; extern PD1=0, intern PD1=1
    if (Wire.endTransmission() != 0) return false;
    tijdstipReferentieInschakelen = micros();
    return true; // Geen verborgen wachttijd, ook niet bij runtime-herstel.
  }

  int8_t StartExtenderReadAnalog(uint8_t pin, uint16_t bereikMillivolt) override {
    if (pin > static_cast<uint8_t>(ExtenderPins::EP_CH7)) return -2;
    if (bereikMillivolt > (EXTENDER_ADS7828_EXTERNE_REFERENTIE ? EXTENDER_ADS7828_REFERENTIE_MILLIVOLT : 2500)) return -2;  // maximale referentiespanning; geen overspanning verbergen
    if (bereikMillivolt == 0) return 1;      // alleen kanaalondersteuning, geen busactie
    if (metingBezig) return 0;
    // TI: na inschakeling van de interne referentie circa 1,5 ms stabilisatietijd.
    if (!EXTENDER_ADS7828_EXTERNE_REFERENTIE && micros() - tijdstipReferentieInschakelen < 1500UL) return 0;
    if (!ComponentBeschikbaarTijdensRuntime()) return -1;
    const uint8_t kanaalcode = static_cast<uint8_t>((pin >> 1) | ((pin & 1) << 2));
    // SD=1 (single-ended), C2..C0 volgens TI, PD1=PD0=1 (interne ref aan).
    const uint8_t commando = static_cast<uint8_t>(0x80 | (kanaalcode << 4) | (EXTENDER_ADS7828_EXTERNE_REFERENTIE ? 0x04 : 0x0C));
    Wire.beginTransmission(adres);
    Wire.write(commando);
    if (Wire.endTransmission() != 0) return -1;
    // Volgens de ADS7828-datasheet volgt de conversie bij de I2C-uitleessequentie.
    // Een wachttijd vóór requestFrom() garandeert daarom geen voltooide conversie.
    metingBezig = true;
    return 1;
  }

  bool IsExtenderReadAnalogReady() override {
    // Het commando is verzonden; de eigenlijke conversie volgt bij het uitlezen.
    return true; // ook na herstel of annulering: niet blijven wachten
  }

  bool GetExtenderReadAnalogValue(int32_t& waarde) override {
    waarde = 0;
    if (!metingBezig) return false;
    if (!ComponentBeschikbaarTijdensRuntime() || !metingBezig) { metingBezig = false; return false; }
    metingBezig = false;
    if (Wire.requestFrom(adres, static_cast<uint8_t>(2)) != 2) return false;
    const uint16_t ruw = static_cast<uint16_t>((static_cast<uint16_t>(Wire.read()) << 8) | Wire.read()) & 0x0FFF;
    // Compatibel met het reeds bestaande ADC_BITS-contract, zonder nieuw API.
    #if ADC_BITS >= 12
    waarde = static_cast<int32_t>(ruw) << (ADC_BITS - 12);
    #else
    waarde = static_cast<int32_t>(ruw) >> (12 - ADC_BITS);
    #endif
    return true;
  }

  // ============================================================================
  // DEFAULT — ADS7828 #1
  // ============================================================================
  #if EXTENDER_ADS7828_AANTAL == 1
  using HardwareResourceTypeI2C::HardwareResourceTypeI2C;
  #endif

  // ============================================================================
  // EXPERIMENTEEL — ADS7828 — AANTAL >= 2
  // ============================================================================
  #if EXTENDER_ADS7828_AANTAL >= 2
  struct Ads7828Pinnen { uint8_t adres; };

  #define BUNDEL_EXTENDER(N) { I2C_ADDRESS_EXTENDER_ADS7828_##N }

  static constexpr Ads7828Pinnen ExtenderLijst[] = {
    BUNDEL_EXTENDER(1),
    BUNDEL_EXTENDER(2)
  };

  #undef BUNDEL_EXTENDER

  ExtenderADS7828(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin np_SDA, HardwareResourcePin np_SCL, uint8_t teller, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD, uint8_t tcaChannel = 0xFF)
    : HardwareResourceTypeI2C(parent, component, ExtenderLijst[teller].adres, np_SDA, np_SCL, Extender, tcaChannel) {}
  #endif
};

#endif
