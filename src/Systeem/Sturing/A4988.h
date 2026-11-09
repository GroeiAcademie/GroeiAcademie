#ifndef A4988_H
#define A4988_H

#include <Arduino.h>
#include "../GedeeldeBus/GedeeldeBus.h"

// STEP en DIR native of op dezelfde GPIO-Extender. ENABLE is optioneel (active LOW).
// RESET, SLEEP, motorvoeding en de stroombegrenzing blijven externe hardwareaansluitingen.
struct A4988 : GedeeldeBusNode {
  uint8_t exclusief_[3],stepPin,dirPin,enablePin;
  A4988(GedeeldeBusNode* parent,HardwareResourcePin step,HardwareResourcePin dir,HardwareResourcePin enablePin=HardwareResourcePin::NONE)
    :GedeeldeBusNode(parent,{nullptr,0,exclusief_,3},GedeeldeBusComponent::A4988),
     exclusief_{static_cast<uint8_t>(step),static_cast<uint8_t>(dir),static_cast<uint8_t>(enablePin)},
     stepPin(NativeArduinoPinVan(step)),dirPin(NativeArduinoPinVan(dir)),enablePin(NativeArduinoPinVan(enablePin)){}

#if GEDEELDE_BUS_EXTENDER_GPIO_ACTIEF
  A4988(GedeeldeBusNode* parent,uint8_t stepPin,uint8_t dirPin,uint8_t enablePin=static_cast<uint8_t>(HardwareResourcePin::NONE))
    :GedeeldeBusNode(parent,{nullptr,0,exclusief_,3},GedeeldeBusComponent::A4988),
     exclusief_{stepPin,dirPin,enablePin},stepPin(stepPin),dirPin(dirPin),enablePin(enablePin){}
#endif

  bool Activeren() override {
    if (!parent || !parent->actief) return false;

    if (parent == &Native) {
      if (stepPin==static_cast<uint8_t>(HardwareResourcePin::NONE) || dirPin==static_cast<uint8_t>(HardwareResourcePin::NONE) || stepPin==dirPin ||
          (enablePin!=static_cast<uint8_t>(HardwareResourcePin::NONE) && (enablePin==stepPin || enablePin==dirPin))) return false;
      if (enablePin!=static_cast<uint8_t>(HardwareResourcePin::NONE)) { digitalWrite(enablePin,HIGH); pinMode(enablePin,OUTPUT); }
      digitalWrite(stepPin,LOW);
      pinMode(stepPin,OUTPUT);
      pinMode(dirPin,OUTPUT);
      return true;
    }

#if GEDEELDE_BUS_EXTENDER_GPIO_ACTIEF
    if (parent!=firstExtenderNode()) return false;
    switch (parent->component) {
      case GedeeldeBusComponent::DIGITAL_PINS_PCF8574:
        if (stepPin>7 || dirPin>7 || (enablePin!=static_cast<uint8_t>(HardwareResourcePin::NONE) && enablePin>7)) return false;
        break;
      case GedeeldeBusComponent::DIGITAL_PINS_PCF8575:
      case GedeeldeBusComponent::DIGITAL_PINS_MCP23017:
      case GedeeldeBusComponent::DIGITAL_PINS_MCP23S17:
        if (stepPin>15 || dirPin>15 || (enablePin!=static_cast<uint8_t>(HardwareResourcePin::NONE) && enablePin>15)) return false;
        break;
      default: return false;
    }
    if (stepPin==dirPin || (enablePin!=static_cast<uint8_t>(HardwareResourcePin::NONE) && (enablePin==stepPin || enablePin==dirPin))) return false;
    if (enablePin!=static_cast<uint8_t>(HardwareResourcePin::NONE) &&
        (!parent->WriteExtenderPin(enablePin,HIGH) || !parent->SetExtenderPinMode(enablePin,OUTPUT))) return false;
    return parent->WriteExtenderPin(stepPin,LOW) && parent->SetExtenderPinMode(stepPin,OUTPUT) && parent->SetExtenderPinMode(dirPin,OUTPUT);
#else
    return false;
#endif
  }

  bool VoerEenStapUit(bool forward,uint16_t pulseMicroseconds=2) {
    if (!actief || pulseMicroseconds<1) return false;
    if (parent==&Native) {
      if (enablePin!=static_cast<uint8_t>(HardwareResourcePin::NONE)) digitalWrite(enablePin,LOW);
      digitalWrite(dirPin,forward?HIGH:LOW);
      delayMicroseconds(1); // DIR setup >= 200 ns
      digitalWrite(stepPin,HIGH);
      delayMicroseconds(pulseMicroseconds);
      digitalWrite(stepPin,LOW);
      delayMicroseconds(1); // STEP LOW >= 1 us voor een eventuele volgende puls
      return true;
    }
#if GEDEELDE_BUS_EXTENDER_GPIO_ACTIEF
    // Elke I2C-/SPI-GPIO-transactie duurt langer dan de minimale STEP-puls van 1 us.
    // De werkelijke stapfrequentie wordt begrensd door de buslatentie.
    if (enablePin!=static_cast<uint8_t>(HardwareResourcePin::NONE) && !parent->WriteExtenderPin(enablePin,LOW)) return false;
    if (!parent->WriteExtenderPin(dirPin,forward?HIGH:LOW)) return false;
    delayMicroseconds(1);
    if (!parent->WriteExtenderPin(stepPin,HIGH)) return false;
    delayMicroseconds(pulseMicroseconds);
    if (!parent->WriteExtenderPin(stepPin,LOW)) return false;
    delayMicroseconds(1);
    return true;
#else
    return false;
#endif
  }

  void StopSturing() override {
    if (!parent) return;
    if (parent==&Native) {
      if (stepPin==static_cast<uint8_t>(HardwareResourcePin::NONE) || dirPin==static_cast<uint8_t>(HardwareResourcePin::NONE) || stepPin==dirPin ||
          (enablePin!=static_cast<uint8_t>(HardwareResourcePin::NONE) && (enablePin==stepPin || enablePin==dirPin))) return;
      digitalWrite(stepPin,LOW);
      if (enablePin!=static_cast<uint8_t>(HardwareResourcePin::NONE)) digitalWrite(enablePin,HIGH);
      return;
    }
#if GEDEELDE_BUS_EXTENDER_GPIO_ACTIEF
    if (!parent->actief || parent!=firstExtenderNode() || stepPin==dirPin ||
        (enablePin!=static_cast<uint8_t>(HardwareResourcePin::NONE) && (enablePin==stepPin || enablePin==dirPin))) return;
    switch (parent->component) {
      case GedeeldeBusComponent::DIGITAL_PINS_PCF8574:
        if (stepPin>7 || dirPin>7 || (enablePin!=static_cast<uint8_t>(HardwareResourcePin::NONE) && enablePin>7)) return;
        break;
      case GedeeldeBusComponent::DIGITAL_PINS_PCF8575:
      case GedeeldeBusComponent::DIGITAL_PINS_MCP23017:
      case GedeeldeBusComponent::DIGITAL_PINS_MCP23S17:
        if (stepPin>15 || dirPin>15 || (enablePin!=static_cast<uint8_t>(HardwareResourcePin::NONE) && enablePin>15)) return;
        break;
      default: return;
    }
    parent->WriteExtenderPin(stepPin,LOW);
    if (enablePin!=static_cast<uint8_t>(HardwareResourcePin::NONE)) parent->WriteExtenderPin(enablePin,HIGH);
#endif
  }
};

#endif
