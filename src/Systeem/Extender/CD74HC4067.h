#ifndef EXTENDERCD74HC4067_H
#define EXTENDERCD74HC4067_H

struct ExtenderCD74HC4067 : GedeeldeBusNode {
  enum class ExtenderPins : uint8_t {
    EP_Y0  = 0x00, // Bidirectioneel analoog kanaal Y0, via gemeenschappelijke pin Z
    EP_Y1  = 0x01, // Bidirectioneel analoog kanaal Y1, via gemeenschappelijke pin Z
    EP_Y2  = 0x02, // Bidirectioneel analoog kanaal Y2, via gemeenschappelijke pin Z
    EP_Y3  = 0x03, // Bidirectioneel analoog kanaal Y3, via gemeenschappelijke pin Z
    EP_Y4  = 0x04, // Bidirectioneel analoog kanaal Y4, via gemeenschappelijke pin Z
    EP_Y5  = 0x05, // Bidirectioneel analoog kanaal Y5, via gemeenschappelijke pin Z
    EP_Y6  = 0x06, // Bidirectioneel analoog kanaal Y6, via gemeenschappelijke pin Z
    EP_Y7  = 0x07, // Bidirectioneel analoog kanaal Y7, via gemeenschappelijke pin Z
    EP_Y8  = 0x08, // Bidirectioneel analoog kanaal Y8, via gemeenschappelijke pin Z
    EP_Y9  = 0x09, // Bidirectioneel analoog kanaal Y9, via gemeenschappelijke pin Z
    EP_Y10 = 0x0A, // Bidirectioneel analoog kanaal Y10, via gemeenschappelijke pin Z
    EP_Y11 = 0x0B, // Bidirectioneel analoog kanaal Y11, via gemeenschappelijke pin Z
    EP_Y12 = 0x0C, // Bidirectioneel analoog kanaal Y12, via gemeenschappelijke pin Z
    EP_Y13 = 0x0D, // Bidirectioneel analoog kanaal Y13, via gemeenschappelijke pin Z
    EP_Y14 = 0x0E, // Bidirectioneel analoog kanaal Y14, via gemeenschappelijke pin Z
    EP_Y15 = 0x0F  // Bidirectioneel analoog kanaal Y15, via gemeenschappelijke pin Z
  };

  HardwareResourcePin np_S0, np_S1, np_S2, np_S3, np_EN, np_Z;
  uint8_t exclusief_[6];
  // CD74HC4067 is geen ADC: channel kiezen en het bestaande contract doorgeven
  // aan de eigen ADC-parent, via Z = de ADC-ingang op die parent.
  bool metingBezig = false;
  uint8_t metingChannel = 0;
  bool Activeren() override {
    metingBezig = false;
    return ingeplugd && parent != nullptr && parent->actief;
  }
  int8_t StartExtenderReadAnalog(uint8_t pin, uint16_t bereikMillivolt) override {
    if (pin > static_cast<uint8_t>(ExtenderPins::EP_Y15) || parent == nullptr || np_Z == HardwareResourcePin::NONE || np_Z == HardwareResourcePin::CUSTOM) return -2;
    // Native is geen ADC-Extender: Z moet fysiek op een echte analoge pin zitten.
    if (parent == &Native) {
      const uint8_t z = static_cast<uint8_t>(np_Z);
      if (z < static_cast<uint8_t>(HardwareResourcePin::NP_A0) ||
          z > static_cast<uint8_t>(HardwareResourcePin::NP_A5) ||
          NativeArduinoPinVan(np_Z) == static_cast<uint8_t>(HardwareResourcePin::NONE)) return -2;
      if (bereikMillivolt == 0) return 1;
      if (EXTENDER_CD74HC4067_NATIVE_REFERENTIE_MILLIVOLT == 0 ||
          bereikMillivolt > EXTENDER_CD74HC4067_NATIVE_REFERENTIE_MILLIVOLT) return -2;
    } else {
      // Proef op bereik 0 verandert geen selectielijnen of parent-meetstatus.
      if (bereikMillivolt == 0) return parent->StartExtenderReadAnalog(static_cast<uint8_t>(np_Z), 0);
    }
    if (metingBezig) return 0;
    if (!actief) return -1;
    selectChannel(static_cast<ExtenderPins>(pin));
    enable();
    // Voor de eerste conversie na een channelwisseling moet de analoge ingang
    // fysiek voldoende settelen. De ADC-parent bepaalt zijn acquisitietijd.
    const int8_t resultaat = parent == &Native ? 1 : parent->StartExtenderReadAnalog(static_cast<uint8_t>(np_Z), bereikMillivolt);
    if (resultaat == 1) { metingBezig = true; metingChannel = pin; }
    else disable();
    return resultaat;
  }
  bool IsExtenderReadAnalogReady() override { return !metingBezig || parent == nullptr || parent == &Native || parent->IsExtenderReadAnalogReady(); }
  bool GetExtenderReadAnalogValue(int32_t& waarde) override {
    waarde = 0;
    if (!metingBezig || parent == nullptr) return false;
    // Bij concurrentie blijft de eerder gekozen channel op dezelfde Extender actief.
    bool gelukt;
    if (parent == &Native) {
      // Native analogRead() heeft de board-afhankelijke resolutie uit SystemConfig.h.
      // Omrekenen naar ADC_BITS zonder extra precisie aan de hardware toe te schrijven.
      const int ruw = analogRead(NativeArduinoPinVan(np_Z));
      gelukt = ruw >= 0 && static_cast<uint32_t>(ruw) <= ((1UL << NATIVE_ADC_RESOLUTIE_BITS) - 1UL);
      if (gelukt) waarde = static_cast<int32_t>((static_cast<uint32_t>(ruw) * ADC_MAX) / ((1UL << NATIVE_ADC_RESOLUTIE_BITS) - 1UL));
    } else {
      gelukt = parent->GetExtenderReadAnalogValue(waarde);
    }
    metingBezig = false;
    disable();
    return gelukt;
  }
  uint16_t GetExtenderReadAnalogEffectiefBereikMillivolt() const override {
    return parent == nullptr ? 0 :
      (parent == &Native ? EXTENDER_CD74HC4067_NATIVE_REFERENTIE_MILLIVOLT :
        parent->GetExtenderReadAnalogEffectiefBereikMillivolt());
  }


  // ============================================================================
  // DEFAULT — CD74HC4067 #1
  // ============================================================================
  #if EXTENDER_CD74HC4067_AANTAL == 1
  ExtenderCD74HC4067(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin np_S0, HardwareResourcePin np_S1, HardwareResourcePin np_S2, HardwareResourcePin np_S3, HardwareResourcePin np_EN, HardwareResourcePin np_Z, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : GedeeldeBusNode(parent, { nullptr, 0, exclusief_, 6 }, component, Extender), np_S0(np_S0), np_S1(np_S1), np_S2(np_S2), np_S3(np_S3), np_EN(np_EN), np_Z(np_Z) {
    exclusief_[0] = static_cast<uint8_t>(np_S0);
    exclusief_[1] = static_cast<uint8_t>(np_S1);
    exclusief_[2] = static_cast<uint8_t>(np_S2);
    exclusief_[3] = static_cast<uint8_t>(np_S3);
    exclusief_[4] = static_cast<uint8_t>(np_EN);
    exclusief_[5] = static_cast<uint8_t>(np_Z);
  }
  #endif

  // ============================================================================
  // EXPERIMENTEEL — CD74HC4067 — AANTAL >= 2
  // ============================================================================
  #if EXTENDER_CD74HC4067_AANTAL >= 2
  struct Cd74hc4067Pinnen { HardwareResourcePin en; HardwareResourcePin s0; HardwareResourcePin s1; HardwareResourcePin s2; HardwareResourcePin s3; HardwareResourcePin z; };

  #define BUNDEL_EXTENDER(N) { EXTENDER_CD74HC4067_##N##_EN, EXTENDER_CD74HC4067_##N##_S0, EXTENDER_CD74HC4067_##N##_S1, EXTENDER_CD74HC4067_##N##_S2, EXTENDER_CD74HC4067_##N##_S3, EXTENDER_CD74HC4067_##N##_Z }

  static constexpr Cd74hc4067Pinnen ExtenderLijst[] = {
    BUNDEL_EXTENDER(1),
    BUNDEL_EXTENDER(2)
  };

  #undef BUNDEL_EXTENDER

  ExtenderCD74HC4067(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t teller, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : GedeeldeBusNode(parent, { nullptr, 0, exclusief_, 6 }, component, Extender), np_S0(ExtenderLijst[teller].s0), np_S1(ExtenderLijst[teller].s1), np_S2(ExtenderLijst[teller].s2), np_S3(ExtenderLijst[teller].s3), np_EN(ExtenderLijst[teller].en), np_Z(ExtenderLijst[teller].z) {
    exclusief_[0] = static_cast<uint8_t>(np_S0);
    exclusief_[1] = static_cast<uint8_t>(np_S1);
    exclusief_[2] = static_cast<uint8_t>(np_S2);
    exclusief_[3] = static_cast<uint8_t>(np_S3);
    exclusief_[4] = static_cast<uint8_t>(np_EN);
    exclusief_[5] = static_cast<uint8_t>(np_Z);
  }
  #endif

  bool inpluggen() override {
    if (ingeplugd) return true;
    if (np_S0 == HardwareResourcePin::NONE || np_S1 == HardwareResourcePin::NONE || np_S2 == HardwareResourcePin::NONE || np_S3 == HardwareResourcePin::NONE) return false;
    if (!GedeeldeBusNode::inpluggen()) return false;
    pinMode(NativeArduinoPinVan(np_S0), OUTPUT);
    pinMode(NativeArduinoPinVan(np_S1), OUTPUT);
    pinMode(NativeArduinoPinVan(np_S2), OUTPUT);
    pinMode(NativeArduinoPinVan(np_S3), OUTPUT);
    if (np_EN != HardwareResourcePin::NONE) {
      digitalWrite(NativeArduinoPinVan(np_EN), HIGH);
      pinMode(NativeArduinoPinVan(np_EN), OUTPUT);
    }
    return true;
  }

  void selectChannel(ExtenderPins kanaal) {
    const uint8_t waarde = static_cast<uint8_t>(kanaal);
    digitalWrite(NativeArduinoPinVan(np_S0), (waarde & 0x01) ? HIGH : LOW);
    digitalWrite(NativeArduinoPinVan(np_S1), (waarde & 0x02) ? HIGH : LOW);
    digitalWrite(NativeArduinoPinVan(np_S2), (waarde & 0x04) ? HIGH : LOW);
    digitalWrite(NativeArduinoPinVan(np_S3), (waarde & 0x08) ? HIGH : LOW);
  }

  void enable() {
    if (np_EN != HardwareResourcePin::NONE) digitalWrite(NativeArduinoPinVan(np_EN), LOW);
  }

  void disable() {
    if (np_EN != HardwareResourcePin::NONE) digitalWrite(NativeArduinoPinVan(np_EN), HIGH);
  }

};

#endif
