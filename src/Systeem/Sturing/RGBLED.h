#ifndef RGBLED_H
#define RGBLED_H
#include <Arduino.h>
#include "../GedeeldeBus/GedeeldeBus.h"

// Drie PWM-kanalen van een PCA9685, gemeenschappelijke frequentie voor alle kanalen.
struct RGBLED : GedeeldeBusNode {
  uint8_t exclusief_[3],r,g,b;
  bool invertPWMvalueRGBLED;
  RGBLED(GedeeldeBusNode* parent,uint8_t red,uint8_t green,uint8_t blue,bool commonAnode=false)
    : GedeeldeBusNode(parent,{nullptr,0,exclusief_,3},GedeeldeBusComponent::RGBLED),
      exclusief_{red,green,blue},r(red),g(green),b(blue),invertPWMvalueRGBLED(commonAnode){}
  uint16_t switchPWMvalues(uint16_t v) const {return invertPWMvalueRGBLED?4095-v:v;}
  bool Activeren() override {return parent && r<16 && g<16 && b<16 && r!=g && r!=b && g!=b &&
    parent->WriteExtenderPWM(r,switchPWMvalues(0)) && parent->WriteExtenderPWM(g,switchPWMvalues(0)) && parent->WriteExtenderPWM(b,switchPWMvalues(0));}
  bool setRGBcolor(uint16_t red,uint16_t green,uint16_t blue){if(!actief||!parent||red>4095||green>4095||blue>4095)return false;
    return parent->WriteExtenderPWM(r,switchPWMvalues(red)) && parent->WriteExtenderPWM(g,switchPWMvalues(green)) && parent->WriteExtenderPWM(b,switchPWMvalues(blue));}
  void StopSturing() override {if(parent){parent->WriteExtenderPWM(r,switchPWMvalues(0));parent->WriteExtenderPWM(g,switchPWMvalues(0));parent->WriteExtenderPWM(b,switchPWMvalues(0));}}
};

#endif
