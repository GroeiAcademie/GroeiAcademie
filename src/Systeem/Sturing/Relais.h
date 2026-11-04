#ifndef RELAIS_H
#define RELAIS_H

#include <Arduino.h>
#include "../GedeeldeBus/GedeeldeBus.h"

// Actief niveau dient overeen te komen met de eigen relaismodule.
struct Relais : GedeeldeBusNode {
  uint8_t exclusief_[1], channel, relayValueOn;
  Relais(GedeeldeBusNode* parent,uint8_t pin,uint8_t relayValueOnAtCreation)
    :GedeeldeBusNode(parent,{nullptr,0,exclusief_,1},GedeeldeBusComponent::Relais),
     exclusief_{pin},channel(pin),relayValueOn(relayValueOnAtCreation){}
  uint8_t getRelayValueOn() const { return relayValueOn; }

  bool Activeren() override {
    if (!parent) return false;
    if (parent == &Native) {
      if (NativeArduinoPinVan(static_cast<HardwareResourcePin>(channel)) == static_cast<uint8_t>(HardwareResourcePin::NONE)) return false;
      digitalWrite(NativeArduinoPinVan(static_cast<HardwareResourcePin>(channel)), relayValueOn == LOW ? HIGH : LOW);
      pinMode(NativeArduinoPinVan(static_cast<HardwareResourcePin>(channel)), OUTPUT);
      return true;
    }
#if GEDEELDE_BUS_EXTENDER_GPIO_ACTIEF
    parent->WriteExtenderPin(channel, relayValueOn == LOW ? HIGH : LOW);
    parent->SetExtenderPinMode(channel, OUTPUT);
    return true;
#else
    return false;
#endif
  }

  bool setRelayStatusOnOff(bool on) {
    if (!actief || !parent) return false;
    if (parent == &Native) {
      digitalWrite(NativeArduinoPinVan(static_cast<HardwareResourcePin>(channel)), on ? relayValueOn : (relayValueOn == LOW ? HIGH : LOW));
      return true;
    }
#if GEDEELDE_BUS_EXTENDER_GPIO_ACTIEF
    parent->WriteExtenderPin(channel, on ? relayValueOn : (relayValueOn == LOW ? HIGH : LOW));
    return true;
#else
    return false;
#endif
  }

  void StopSturing() override {
    if (!parent) return;
    if (parent == &Native) {
      if (NativeArduinoPinVan(static_cast<HardwareResourcePin>(channel)) != static_cast<uint8_t>(HardwareResourcePin::NONE)) {
        digitalWrite(NativeArduinoPinVan(static_cast<HardwareResourcePin>(channel)), relayValueOn == LOW ? HIGH : LOW);
      }
      return;
    }
#if GEDEELDE_BUS_EXTENDER_GPIO_ACTIEF
    parent->WriteExtenderPin(channel, relayValueOn == LOW ? HIGH : LOW);
#endif
  }
};

#endif
