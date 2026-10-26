#ifndef A4988_H
#define A4988_H
#include <Arduino.h>
#include "../GedeeldeBus/GedeeldeBus.h"

// STEP en DIR native: protocol vereist korte nauwkeurige pulsen. Parent is native/root.
// Driver ENABLE, RESET en SLEEP moeten extern correct en veilig bedraad worden.
struct A4988 : GedeeldeBusNode {
  uint8_t exclusief_[2],stepPin,dirPin;
  A4988(GedeeldeBusNode* parent,HardwareResourcePin step,HardwareResourcePin dir)
    :GedeeldeBusNode(parent,{nullptr,0,exclusief_,2},GedeeldeBusComponent::A4988),
     exclusief_{static_cast<uint8_t>(step),static_cast<uint8_t>(dir)},stepPin(NativeArduinoPinVan(step)),dirPin(NativeArduinoPinVan(dir)){}
  bool Activeren() override {if(stepPin==static_cast<uint8_t>(HardwareResourcePin::NONE)||dirPin==static_cast<uint8_t>(HardwareResourcePin::NONE)||stepPin==dirPin)return false;digitalWrite(stepPin,LOW);pinMode(stepPin,OUTPUT);pinMode(dirPin,OUTPUT);return true;}
  bool doOneStep(bool forward,uint16_t pulseMicroseconds=2){if(!actief||pulseMicroseconds<1)return false;
    digitalWrite(dirPin,forward?HIGH:LOW);digitalWrite(stepPin,HIGH);delayMicroseconds(pulseMicroseconds);digitalWrite(stepPin,LOW);return true;}
  void StopSturing() override {digitalWrite(stepPin,LOW);}
};

#endif
