#ifndef ULN2003_H
#define ULN2003_H

#include <Arduino.h>
#include "../GedeeldeBus/GedeeldeBus.h"

// Vier digitale uitgangen van dezelfde GPIO-Extender of vier Native-pinnen.
struct ULN2003 : GedeeldeBusNode {
  uint8_t exclusief_[4],p[4],phase=0;
  ULN2003(GedeeldeBusNode* parent,uint8_t p1,uint8_t p2,uint8_t p3,uint8_t p4)
   : GedeeldeBusNode(parent,{nullptr,0,exclusief_,4},GedeeldeBusComponent::ULN2003),
     exclusief_{p1,p2,p3,p4},p{p1,p2,p3,p4}{}

  bool Activeren() override {
    if (!parent || !parent->actief) return false;
    for (uint8_t i=0;i<4;i++) if (p[i]==static_cast<uint8_t>(HardwareResourcePin::NONE)) return false;
    if (p[0]==p[1] || p[0]==p[2] || p[0]==p[3] || p[1]==p[2] || p[1]==p[3] || p[2]==p[3]) return false;
    if (parent==&Native) {
      for (uint8_t i=0;i<4;i++) {
        if (p[i]>13) return false;
        digitalWrite(NativeArduinoPinVan(static_cast<HardwareResourcePin>(p[i])),LOW);
        pinMode(NativeArduinoPinVan(static_cast<HardwareResourcePin>(p[i])),OUTPUT);
      }
      return true;
    }
#if GEDEELDE_BUS_EXTENDER_GPIO_ACTIEF
    if (parent!=firstExtenderNode()) return false;
    switch (parent->component) {
      case GedeeldeBusComponent::DIGITAL_PINS_PCF8574:
        for (uint8_t i=0;i<4;i++) if (p[i]>7) return false;
        break;
      case GedeeldeBusComponent::DIGITAL_PINS_PCF8575:
      case GedeeldeBusComponent::DIGITAL_PINS_MCP23017:
      case GedeeldeBusComponent::DIGITAL_PINS_MCP23S17:
        for (uint8_t i=0;i<4;i++) if (p[i]>15) return false;
        break;
      default: return false;
    }
    for (uint8_t i=0;i<4;i++) {
      if (!parent->WriteExtenderPin(p[i],LOW) || !parent->SetExtenderPinMode(p[i],OUTPUT)) return false;
    }
    return true;
#else
    return false;
#endif
  }

  bool VoerEenStapUit(bool forward) {
    if (!actief || !parent) return false;
    phase=(phase+(forward?1:3))%4;
    const uint8_t patterns[4]={0x03,0x06,0x0C,0x09};
    for (uint8_t i=0;i<4;i++) {
      if (parent==&Native) {
        digitalWrite(NativeArduinoPinVan(static_cast<HardwareResourcePin>(p[i])),(patterns[phase]&(1<<i))?HIGH:LOW);
      } else {
#if GEDEELDE_BUS_EXTENDER_GPIO_ACTIEF
        if (!parent->WriteExtenderPin(p[i],(patterns[phase]&(1<<i))?HIGH:LOW)) {
          StopSturing();
          return false;
        }
#else
        return false;
#endif
      }
    }
    return true;
  }

  void StopSturing() override {
    if (!parent) return;
    for (uint8_t i=0;i<4;i++) {
      if (parent==&Native) {
        digitalWrite(NativeArduinoPinVan(static_cast<HardwareResourcePin>(p[i])),LOW);
      } else {
#if GEDEELDE_BUS_EXTENDER_GPIO_ACTIEF
        parent->WriteExtenderPin(p[i],LOW);
#endif
      }
    }
  }
};

#endif
