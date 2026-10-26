#ifndef RELAIS_H
#define RELAIS_H
#include <Arduino.h>
#include "../GedeeldeBus/GedeeldeBus.h"

#if GEDEELDE_BUS_MIXED_PINS
// Actief niveau dient overeen te komen met de eigen relaismodule.
struct Relais : GedeeldeBusNode {
  uint8_t exclusief_[1], channel, relayValueOn;
  Relais(GedeeldeBusNode* parent,uint8_t pin,uint8_t relayValueOnAtCreation)
    :GedeeldeBusNode(parent,{nullptr,0,exclusief_,1},GedeeldeBusComponent::Relais),
     exclusief_{pin},channel(pin),relayValueOn(relayValueOnAtCreation){}
  uint8_t getRelayValueOn() const { return relayValueOn; }
  bool Activeren() override { if(!parent)return false; parent->WriteExtenderPin(channel,relayValueOn==LOW?HIGH:LOW); parent->SetExtenderPinMode(channel,OUTPUT); return true; }
  bool setRelayStatusOnOff(bool on) {if(!actief||!parent)return false;parent->WriteExtenderPin(channel,on?relayValueOn:(relayValueOn==LOW?HIGH:LOW));return true;}
  void StopSturing() override {if(parent)parent->WriteExtenderPin(channel,relayValueOn==LOW?HIGH:LOW);}
};
#else
#error Relais vereist GEDEELDE_BUS_MIXED_PINS 1
#endif

#endif
