#ifndef MG996R_H
#define MG996R_H
#include <Arduino.h>
#include "../../Configuratie/SystemConfig.h"
#include "../GedeeldeBus/GedeeldeBus.h"

// PWM-channel van PCA9685. Minimum en maximum worden in ticks gekalibreerd bij de frequentie van de parent.
struct MG996R : GedeeldeBusNode {
  uint8_t exclusief_[1];
  uint8_t channel;
  uint16_t minimumPWM, maximumPWM;
  MG996R(GedeeldeBusNode* parent, uint8_t pin, uint16_t minPWM, uint16_t maxPWM)
    : GedeeldeBusNode(parent, {nullptr,0,exclusief_,1}, GedeeldeBusComponent::MG996R),
      exclusief_{pin}, channel(pin), minimumPWM(minPWM), maximumPWM(maxPWM) {}
#if defined(STURING_MG996R_MIN_PWM) && defined(STURING_MG996R_MAX_PWM)
  MG996R(GedeeldeBusNode* parent,uint8_t pin) : MG996R(parent,pin,STURING_MG996R_MIN_PWM,STURING_MG996R_MAX_PWM) {}
#endif
  bool Activeren() override {
    return parent && channel < 16 && minimumPWM > 0 && minimumPWM < maximumPWM && maximumPWM < 4096
      && parent->WriteExtenderPWM(channel,0);
  }
  bool setServoAngle(uint8_t angle) {
    if (!actief || !parent || angle > 180) return false;
    uint16_t ticks = minimumPWM + (uint32_t(maximumPWM-minimumPWM)*angle)/180;
    return parent->WriteExtenderPWM(channel,ticks);
  }
  void StopSturing() override { if(parent) parent->WriteExtenderPWM(channel,0); }
};

#endif
