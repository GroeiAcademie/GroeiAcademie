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
  // Eén kanaal tegelijk: een digitale child houdt Z doorlopend open, een analoge meting schakelt de chip daarna uit.
  // Daarom nooit een analoge en een digitale child op dezelfde CD74HC4067: wie eerst aanmeldt of activeert, krijgt de chip.
  // Geen bewaarde pointer naar een child (die kan met afmelden() verdwijnen): de CD74HC4067 zoekt telkens in de boom,
  // zoals afmelden() dat doet, welke node een kanaal van hem claimt (bij gemengde pinnen: GetOwnerClaim() == deze node).
  // Een digitale child houdt het kanaal alleen zolang hij actief is of aan het activeren is; een analoge child zolang hij aangemeld is.
  bool Activeren() override {
    metingBezig = false;
    return ingeplugd && parent != nullptr && parent->actief;
  }
  int8_t StartExtenderReadAnalog(uint8_t pin, uint16_t bereikMillivolt) override {
    if (pin > static_cast<uint8_t>(ExtenderPins::EP_Y15) || parent == nullptr || np_Z == HardwareResourcePin::NONE || np_Z == HardwareResourcePin::CUSTOM) return -2;
    // Een analoge child (ook bij de aanmeldproef op bereik 0) wordt geweigerd zolang een digitale child Z gebruikt.
    bool exclusief = false; HardwareResourceIdentiteit identiteit = HardwareResourceIdentiteit::GEEN;
    GedeeldeBusNode* wortel = this;
    while (wortel->parent != nullptr) wortel = wortel->parent;
    for (const GedeeldeBusNode* node = wortel; node != nullptr; node = VolgendeInBoom(node, wortel)) {
      if (node->IsAnalogChild() || !(node->actief || node->activerenBezig)) continue;
      for (uint8_t index = 0; index < node->AantalClaims(); index++) {
#if GEDEELDE_BUS_MIXED_PINS
        if (node->ClaimWaarde(index, exclusief, identiteit) > static_cast<uint8_t>(ExtenderPins::EP_Y15) || !exclusief || node->GetOwnerClaim(index) != this) continue;
#else
        if (node->ClaimWaarde(index, exclusief, identiteit) > static_cast<uint8_t>(ExtenderPins::EP_Y15) || !exclusief || node->firstExtenderNode() != this) continue;
#endif
        return -2;
      }
    }
    // Native is geen ADC-Extender: Z moet fysiek op een echte analoge pin zitten.
    if (parent == &Native) {
      const uint8_t z = static_cast<uint8_t>(np_Z);
      if (z < static_cast<uint8_t>(HardwareResourcePin::NP_A0) ||
          z > static_cast<uint8_t>(HardwareResourcePin::NP_A5) ||
          NativeArduinoPinVan(np_Z) == static_cast<uint8_t>(HardwareResourcePin::NONE)) return -2;
      if (bereikMillivolt == 0) return 1;   // aanmeldproef van een analoge child
      if (EXTENDER_CD74HC4067_NATIVE_REFERENTIE_MILLIVOLT == 0 ||
          bereikMillivolt > EXTENDER_CD74HC4067_NATIVE_REFERENTIE_MILLIVOLT) return -2;
    } else {
      // Proef op bereik 0 verandert geen selectielijnen of parent-meetstatus.
      if (bereikMillivolt == 0) return parent->StartExtenderReadAnalog(static_cast<uint8_t>(np_Z), 0);   // aanmeldproef van een analoge child
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
  // Toegang voor een child: het kanaal EP_Yn kiezen dat de child bij deze CD74HC4067 claimt, en de chip inschakelen,
  // zoals de TCA9548A zijn kanaal kiest. Een lopende analoge meting wordt niet verstoord.
  // Het kanaal is een exclusieve claim in het gebied van deze CD74HC4067 (bij gemengde pinnen: GetOwnerClaim() == deze node).
  // Gedeelde buspinnen (I2C SDA/SCL, SPI SCK/MISO/MOSI) gaan nooit via een CD74HC4067.
  // Er is geen ReleaseAccessForChild(): het gekozen kanaal blijft open na Activeren().
  // Een digitale child (DHTxx, OT3686, HSR0x, HX711, INMP441_MS3625, MR60BHA2) gebruikt Z doorlopend: één digitale child per CD74HC4067.
  // Geen child, of een child zonder kanaal op deze CD74HC4067 (bij gemengde pinnen op een andere Extender): niets te kiezen, true.
  bool SelectAccessForChild(GedeeldeBusNode* child) override {
    if (child == nullptr) return true;
    for (uint8_t i = 0; i < child->AantalClaims(); i++) {
      bool exclusief = false; HardwareResourceIdentiteit identiteit = HardwareResourceIdentiteit::GEEN;
      const uint8_t waarde = child->ClaimWaarde(i, exclusief, identiteit);
      if (waarde > static_cast<uint8_t>(ExtenderPins::EP_Y15)) continue;
      bool claimIsChannelOfThisCD74HC4067 = exclusief;
#if GEDEELDE_BUS_MIXED_PINS
      if (claimIsChannelOfThisCD74HC4067 && child->GetOwnerClaim(i) != this) claimIsChannelOfThisCD74HC4067 = false;
#endif
      if (claimIsChannelOfThisCD74HC4067) {
        // Andere nodes met een kanaal op deze CD74HC4067 (exclusief en identiteit van deze claim zijn niet meer nodig).
        GedeeldeBusNode* wortel = this;
        while (wortel->parent != nullptr) wortel = wortel->parent;
        for (const GedeeldeBusNode* node = wortel; node != nullptr; node = VolgendeInBoom(node, wortel)) {
          if (node == child) continue;
          for (uint8_t index = 0; index < node->AantalClaims(); index++) {
#if GEDEELDE_BUS_MIXED_PINS
            if (node->ClaimWaarde(index, exclusief, identiteit) > static_cast<uint8_t>(ExtenderPins::EP_Y15) || !exclusief || node->GetOwnerClaim(index) != this) continue;
#else
            if (node->ClaimWaarde(index, exclusief, identiteit) > static_cast<uint8_t>(ExtenderPins::EP_Y15) || !exclusief || node->firstExtenderNode() != this) continue;
#endif
            // Een analoge child naast een digitale child die het kanaal gebruikt: weigeren, anders schakelt zijn meting het kanaal uit.
            if (child->IsAnalogChild() && !node->IsAnalogChild() && (node->actief || node->activerenBezig)) return false;
            // Een digitale child naast een aangemelde analoge child, of naast een andere digitale child die het kanaal gebruikt: weigeren.
            if (!child->IsAnalogChild() && (node->IsAnalogChild() || node->actief || node->activerenBezig)) return false;
          }
        }
        if (child->IsAnalogChild()) {
          // Een lopende analoge meting niet verstoren; de analoge child kiest zijn kanaal zelf bij StartExtenderReadAnalog().
          if (metingBezig) return true;
          selectChannel(static_cast<ExtenderPins>(waarde));
          enable();
          return true;
        }
        selectChannel(static_cast<ExtenderPins>(waarde));
        enable();
        return true;
      }
    }
    return true;
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
    : GedeeldeBusNode(parent, { nullptr, 0, exclusief_, 6 }, component, Extender), np_S0((teller < 2 ? ExtenderLijst[teller].s0 : HardwareResourcePin::NONE)), np_S1((teller < 2 ? ExtenderLijst[teller].s1 : HardwareResourcePin::NONE)), np_S2((teller < 2 ? ExtenderLijst[teller].s2 : HardwareResourcePin::NONE)), np_S3((teller < 2 ? ExtenderLijst[teller].s3 : HardwareResourcePin::NONE)), np_EN((teller < 2 ? ExtenderLijst[teller].en : HardwareResourcePin::NONE)), np_Z((teller < 2 ? ExtenderLijst[teller].z : HardwareResourcePin::NONE)) {
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
