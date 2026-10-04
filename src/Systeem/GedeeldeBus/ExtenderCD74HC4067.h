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
    if (np_EN != HardwareResourcePin::NONE) pinMode(NativeArduinoPinVan(np_EN), OUTPUT);
    return true;
  }
};

#endif
