#ifndef DCMOTORL298N_H
#define DCMOTORL298N_H

#include <Arduino.h>
#include "../GedeeldeBus/GedeeldeBus.h"

// Bij PCA9685: drie PWM-kanalen van DEZELFDE Extender (IN1, IN2, EN), of gemengd met digitale GPIO-richting.
// Bij Native: twee digitale richtingspinnen en een native PWM-pin voor EN.
// De motor krijgt voeding via de externe brugdriver, nooit via de stuurpinnen.
struct L298N : GedeeldeBusNode {
  uint8_t exclusief_[3];
  uint8_t in1,in2,en;
#if GEDEELDE_BUS_MIXED_PINS && GEDEELDE_BUS_EXTENDER_GPIO_ACTIEF && EXTENDER_PCA9685_AANTAL > 0
  GedeeldeBusNode* extenderPWM_[3] = {nullptr,nullptr,nullptr};
#endif
  L298N(GedeeldeBusNode* parent,uint8_t pin1,uint8_t pin2,uint8_t pinEnable=static_cast<uint8_t>(HardwareResourcePin::NONE))
    :GedeeldeBusNode(parent,{nullptr,0,exclusief_,3},GedeeldeBusComponent::L298N),
     exclusief_{pin1,pin2,pinEnable},in1(pin1),in2(pin2),en(pinEnable){}
#if GEDEELDE_BUS_MIXED_PINS && GEDEELDE_BUS_EXTENDER_GPIO_ACTIEF && EXTENDER_PCA9685_AANTAL > 0
  // IN1/IN2 via GPIO-parent, EN via PCA9685-parent.
  L298N(GedeeldeBusNode* parent,GedeeldeBusNode* extenderPWM_,uint8_t pin1,uint8_t pin2,uint8_t pinEnable)
    :GedeeldeBusNode(parent,{nullptr,0,exclusief_,3},GedeeldeBusComponent::L298N),
     exclusief_{pin1,pin2,pinEnable},in1(pin1),in2(pin2),en(pinEnable),extenderPWM_{nullptr,nullptr,extenderPWM_} {
    ownerClaim=this->extenderPWM_;
  }
#endif

  bool Activeren() override {
    if (!parent || !parent->actief) return false;

    if (parent == &Native) {
      if (in1>13 || in2>13 || in1==in2 || (en!=static_cast<uint8_t>(HardwareResourcePin::NONE) && (en>13 || in1==en || in2==en))) return false;

      if (en!=static_cast<uint8_t>(HardwareResourcePin::NONE)) {

        digitalWrite(NativeArduinoPinVan(static_cast<HardwareResourcePin>(en)),LOW);

        pinMode(NativeArduinoPinVan(static_cast<HardwareResourcePin>(en)),OUTPUT);

      }
      digitalWrite(NativeArduinoPinVan(static_cast<HardwareResourcePin>(in1)), LOW);
      pinMode(NativeArduinoPinVan(static_cast<HardwareResourcePin>(in1)), OUTPUT);
      digitalWrite(NativeArduinoPinVan(static_cast<HardwareResourcePin>(in2)), LOW);
      pinMode(NativeArduinoPinVan(static_cast<HardwareResourcePin>(in2)), OUTPUT);
      return true;
    }

#if GEDEELDE_BUS_MIXED_PINS && GEDEELDE_BUS_EXTENDER_GPIO_ACTIEF && EXTENDER_PCA9685_AANTAL > 0
    if (ownerClaim!=nullptr) {
      if (!parent || !parent->actief || parent!=firstExtenderNode() || !extenderPWM_[2] || !extenderPWM_[2]->actief ||
          extenderPWM_[2]->component!=GedeeldeBusComponent::PWM_PCA9685 || en>15 || in1==in2) return false;
      switch (parent->component) {
        case GedeeldeBusComponent::DIGITAL_PINS_PCF8574:
          if (in1>7 || in2>7) return false;
          break;
        case GedeeldeBusComponent::DIGITAL_PINS_PCF8575:
        case GedeeldeBusComponent::DIGITAL_PINS_MCP23017:
        case GedeeldeBusComponent::DIGITAL_PINS_MCP23S17:
          if (in1>15 || in2>15) return false;
          break;
        default: return false;
      }
      return extenderPWM_[2]->WriteExtenderPWM(en,0) && parent->WriteExtenderPin(in1,LOW) &&
             parent->SetExtenderPinMode(in1,OUTPUT) && parent->WriteExtenderPin(in2,LOW) && parent->SetExtenderPinMode(in2,OUTPUT);
    }
#endif
    // Twee ingangspinnen, zonder apart bereikbare ENABLE. Deze moet op het bord actief zijn.
    if (en==static_cast<uint8_t>(HardwareResourcePin::NONE)) {
      if (in1==in2) return false;
#if GEDEELDE_BUS_EXTENDER_GPIO_ACTIEF
      if (parent==firstExtenderNode()) {
        switch (parent->component) {
          case GedeeldeBusComponent::DIGITAL_PINS_PCF8574:
            if (in1>7 || in2>7) return false;
            break;
          case GedeeldeBusComponent::DIGITAL_PINS_PCF8575:
          case GedeeldeBusComponent::DIGITAL_PINS_MCP23017:
          case GedeeldeBusComponent::DIGITAL_PINS_MCP23S17:
            if (in1>15 || in2>15) return false;
            break;
          default: break;
        }
        switch (parent->component) {
          case GedeeldeBusComponent::DIGITAL_PINS_PCF8574:
          case GedeeldeBusComponent::DIGITAL_PINS_PCF8575:
          case GedeeldeBusComponent::DIGITAL_PINS_MCP23017:
          case GedeeldeBusComponent::DIGITAL_PINS_MCP23S17:
            return parent->WriteExtenderPin(in1,LOW) && parent->SetExtenderPinMode(in1,OUTPUT) &&
                   parent->WriteExtenderPin(in2,LOW) && parent->SetExtenderPinMode(in2,OUTPUT);
          default: break;
        }
      }
#endif
#if EXTENDER_PCA9685_AANTAL > 0
      if (parent->component==GedeeldeBusComponent::PWM_PCA9685 && in1<16 && in2<16)
        return parent->WriteExtenderPWM(in1,0) && parent->WriteExtenderPWM(in2,0);
#endif
      return false;
    }
    return parent && in1<16 && in2<16 && en<16 && in1!=in2 && in1!=en && in2!=en &&
      parent->WriteExtenderPWM(en,0) && parent->WriteExtenderPWM(in1,0) && parent->WriteExtenderPWM(in2,0);
  }

  bool StelDraairichtingEnSnelheidIn(bool forward,uint16_t pwm) {
    if(!actief || !parent || pwm>4095) return false;

    if (parent == &Native) {
      if (en==static_cast<uint8_t>(HardwareResourcePin::NONE)) {
#if defined(ARDUINO_ARCH_ESP32)
        // Na analogWrite() moet ESP32 de pinnen terug als GPIO-uitgangen instellen.
        pinMode(NativeArduinoPinVan(static_cast<HardwareResourcePin>(in1)),OUTPUT);
        pinMode(NativeArduinoPinVan(static_cast<HardwareResourcePin>(in2)),OUTPUT);
#endif
        digitalWrite(NativeArduinoPinVan(static_cast<HardwareResourcePin>(in1)),LOW);
        digitalWrite(NativeArduinoPinVan(static_cast<HardwareResourcePin>(in2)),LOW);
        if (pwm==0) return true;
        if (pwm==4095) {
          digitalWrite(NativeArduinoPinVan(static_cast<HardwareResourcePin>(forward?in1:in2)),HIGH);
        } else {
          // Alleen op Native PWM-pinnen gebruiken; beide ingangspinnen moeten PWM kunnen leveren.
          analogWrite(NativeArduinoPinVan(static_cast<HardwareResourcePin>(forward?in1:in2)),(uint32_t(pwm)*255UL+2047UL)/4095UL);
        }
        return true;
      }
      analogWrite(NativeArduinoPinVan(static_cast<HardwareResourcePin>(en)),0);
      digitalWrite(NativeArduinoPinVan(static_cast<HardwareResourcePin>(in1)), forward ? HIGH : LOW);
      digitalWrite(NativeArduinoPinVan(static_cast<HardwareResourcePin>(in2)), forward ? LOW : HIGH);
      analogWrite(NativeArduinoPinVan(static_cast<HardwareResourcePin>(en)), (uint32_t(pwm)*255UL+2047UL)/4095UL);
      return true;
    }

#if GEDEELDE_BUS_MIXED_PINS && GEDEELDE_BUS_EXTENDER_GPIO_ACTIEF && EXTENDER_PCA9685_AANTAL > 0
    if (ownerClaim!=nullptr) {
      if (!extenderPWM_[2]->WriteExtenderPWM(en,0) || !parent->WriteExtenderPin(in1,forward?HIGH:LOW) ||
          !parent->WriteExtenderPin(in2,forward?LOW:HIGH) || !extenderPWM_[2]->WriteExtenderPWM(en,pwm)) {
        StopSturing();
        return false;
      }
      return true;
    }
#endif
    if (en==static_cast<uint8_t>(HardwareResourcePin::NONE)) {
#if GEDEELDE_BUS_EXTENDER_GPIO_ACTIEF
      switch (parent->component) {
        case GedeeldeBusComponent::DIGITAL_PINS_PCF8574:
        case GedeeldeBusComponent::DIGITAL_PINS_PCF8575:
        case GedeeldeBusComponent::DIGITAL_PINS_MCP23017:
        case GedeeldeBusComponent::DIGITAL_PINS_MCP23S17:
          // GPIO-Extenders kennen hier alleen 0 of volle aansturing, geen PWM.
          if (pwm!=0 && pwm!=4095) return false;
          if (!parent->WriteExtenderPin(in1,LOW) || !parent->WriteExtenderPin(in2,LOW) ||
              (pwm!=0 && !parent->WriteExtenderPin(forward?in1:in2,HIGH))) {
            StopSturing();
            return false;
          }
          return true;
        default: break;
      }
#endif
#if EXTENDER_PCA9685_AANTAL > 0
      if (parent->component==GedeeldeBusComponent::PWM_PCA9685) {
        if (!parent->WriteExtenderPWM(in1,0) || !parent->WriteExtenderPWM(in2,0) ||
            !parent->WriteExtenderPWM(forward?in1:in2,pwm)) {
          StopSturing();
          return false;
        }
        return true;
      }
#endif
      return false;
    }
    if (!parent->WriteExtenderPWM(en,0) || !parent->WriteExtenderPWM(in1,forward?4095:0) ||
        !parent->WriteExtenderPWM(in2,forward?0:4095) || !parent->WriteExtenderPWM(en,pwm)) {
      StopSturing();
      return false;
    }
    return true;
  }

  void StopSturing() override {
    if (parent == &Native) {
      if (in1>13 || in2>13 || (en!=static_cast<uint8_t>(HardwareResourcePin::NONE) && en>13)) return;

      if (en!=static_cast<uint8_t>(HardwareResourcePin::NONE)) analogWrite(NativeArduinoPinVan(static_cast<HardwareResourcePin>(en)),0);
#if defined(ARDUINO_ARCH_ESP32)
      if (en==static_cast<uint8_t>(HardwareResourcePin::NONE)) {
        // Eventuele LEDC-PWM vrijgeven voordat de pinnen digitaal LOW worden.
        pinMode(NativeArduinoPinVan(static_cast<HardwareResourcePin>(in1)),OUTPUT);
        pinMode(NativeArduinoPinVan(static_cast<HardwareResourcePin>(in2)),OUTPUT);
      }
#endif
      digitalWrite(NativeArduinoPinVan(static_cast<HardwareResourcePin>(in1)), LOW);
      digitalWrite(NativeArduinoPinVan(static_cast<HardwareResourcePin>(in2)), LOW);
      return;
    }

#if GEDEELDE_BUS_MIXED_PINS && GEDEELDE_BUS_EXTENDER_GPIO_ACTIEF && EXTENDER_PCA9685_AANTAL > 0
    if (ownerClaim!=nullptr) {
      if (extenderPWM_[2]) extenderPWM_[2]->WriteExtenderPWM(en,0);
      if (parent) { parent->WriteExtenderPin(in1,LOW); parent->WriteExtenderPin(in2,LOW); }
      return;
    }
#endif
    if (!parent) return;
    if (en==static_cast<uint8_t>(HardwareResourcePin::NONE)) {
#if GEDEELDE_BUS_EXTENDER_GPIO_ACTIEF
      switch (parent->component) {
        case GedeeldeBusComponent::DIGITAL_PINS_PCF8574:
        case GedeeldeBusComponent::DIGITAL_PINS_PCF8575:
        case GedeeldeBusComponent::DIGITAL_PINS_MCP23017:
        case GedeeldeBusComponent::DIGITAL_PINS_MCP23S17:
          parent->WriteExtenderPin(in1,LOW);
          parent->WriteExtenderPin(in2,LOW);
          return;
        default: break;
      }
#endif
#if EXTENDER_PCA9685_AANTAL > 0
      if (parent->component==GedeeldeBusComponent::PWM_PCA9685) {
        parent->WriteExtenderPWM(in1,0);
        parent->WriteExtenderPWM(in2,0);
      }
#endif
      return;
    }
    parent->WriteExtenderPWM(en,0);
    parent->WriteExtenderPWM(in1,0);
    parent->WriteExtenderPWM(in2,0);
  }
};

#endif
