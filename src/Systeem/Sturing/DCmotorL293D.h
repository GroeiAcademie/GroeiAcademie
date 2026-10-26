#ifndef DCMOTORL293D_H
#define DCMOTORL293D_H
#include <Arduino.h>
#include "../GedeeldeBus/GedeeldeBus.h"

// Drie PWM-kanalen van DEZELFDE PCA9685: IN1, IN2, EN.
// De motor krijgt voeding via de externe brugdriver, nooit via de PCA9685.
struct DCmotorL293D : GedeeldeBusNode {
  uint8_t exclusief_[3];
  uint8_t in1,in2,en;
  DCmotorL293D(GedeeldeBusNode* parent,uint8_t pin1,uint8_t pin2,uint8_t pinEnable)
    :GedeeldeBusNode(parent,{nullptr,0,exclusief_,3},GedeeldeBusComponent::DCmotorL293D),
     exclusief_{pin1,pin2,pinEnable},in1(pin1),in2(pin2),en(pinEnable){}
  bool Activeren() override {
    return parent && in1<16 && in2<16 && en<16 && in1!=in2 && in1!=en && in2!=en &&
      parent->WriteExtenderPWM(en,0) && parent->WriteExtenderPWM(in1,0) && parent->WriteExtenderPWM(in2,0);
  }
  bool setDirectionAndSpeed(bool forward,uint16_t pwm) {
    if(!actief || !parent || pwm>4095) return false;
    if(!parent->WriteExtenderPWM(en,0)) return false;
    if(!parent->WriteExtenderPWM(in1,forward?4095:0)) return false;
    if(!parent->WriteExtenderPWM(in2,forward?0:4095)) return false;
    return parent->WriteExtenderPWM(en,pwm);
  }
  void StopSturing() override {
    if(parent) { parent->WriteExtenderPWM(en,0); parent->WriteExtenderPWM(in1,0); parent->WriteExtenderPWM(in2,0); }
  }
};

#endif
