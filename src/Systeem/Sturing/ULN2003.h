#ifndef ULN2003_H
#define ULN2003_H
#include <Arduino.h>
#include "../GedeeldeBus/GedeeldeBus.h"

#if GEDEELDE_BUS_MIXED_PINS
// Vier digitale uitgangen op DEZELFDE GPIO-Extender; lage stapfrequentie vanwege buslatentie.
struct ULN2003 : GedeeldeBusNode {
  uint8_t exclusief_[4],p[4],phase=0;
  ULN2003(GedeeldeBusNode* parent,uint8_t p1,uint8_t p2,uint8_t p3,uint8_t p4)
   : GedeeldeBusNode(parent,{nullptr,0,exclusief_,4},GedeeldeBusComponent::ULN2003),
     exclusief_{p1,p2,p3,p4},p{p1,p2,p3,p4}{}
  bool Activeren() override {if(!parent)return false;for(uint8_t i=0;i<4;i++){parent->WriteExtenderPin(p[i],LOW);parent->SetExtenderPinMode(p[i],OUTPUT);}return true;}
  bool doOneStep(bool forward){if(!actief||!parent)return false;phase=(phase+(forward?1:3))%4;
    const uint8_t patterns[4]={0x03,0x06,0x0C,0x09};
    for(uint8_t i=0;i<4;i++)parent->WriteExtenderPin(p[i],(patterns[phase]&(1<<i))?HIGH:LOW);return true;}
  void StopSturing() override {if(parent)for(uint8_t i=0;i<4;i++)parent->WriteExtenderPin(p[i],LOW);}
};
#else
#error ULN2003 vereist GEDEELDE_BUS_MIXED_PINS 1
#endif

#endif
