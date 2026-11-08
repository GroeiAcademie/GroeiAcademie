#ifndef SG90_H
#define SG90_H

#include <Arduino.h>
#include "../../Configuratie/SystemConfig.h"
#include "../GedeeldeBus/GedeeldeBus.h"

// PWM-channel van PCA9685. Minimum en maximum worden in ticks gekalibreerd bij de frequentie van de parent.
struct SG90 : GedeeldeBusNode {
  uint8_t exclusief_[1];
  uint8_t channel;
  uint16_t minimumPWM, maximumPWM;
  uint16_t lastPWM = 0;
  bool hasPWM = false;
  SG90(GedeeldeBusNode* parent, uint8_t pin, uint16_t minPWM, uint16_t maxPWM)
    : GedeeldeBusNode(parent, {nullptr,0,exclusief_,1}, GedeeldeBusComponent::SG90),
      exclusief_{pin}, channel(pin), minimumPWM(minPWM), maximumPWM(maxPWM) {}
#if defined(STURING_SG90_MIN_PWM) && defined(STURING_SG90_MAX_PWM)
  SG90(GedeeldeBusNode* parent,uint8_t pin) : SG90(parent,pin,STURING_SG90_MIN_PWM,STURING_SG90_MAX_PWM) {}
#endif
  bool Activeren() override {
    return parent && channel < 16 && minimumPWM > 0 && minimumPWM < maximumPWM && maximumPWM < 4096
      && parent->WriteExtenderPWM(channel,0);
  }

  bool PositioneerAbsoluutNaarHoek(uint8_t angle) {
    if (!actief || !parent || angle > 180) {
      GA_DEBUG_PRINTLN(F("DEBUG: SG90 hoekopdracht geweigerd (inactief of hoek buiten 0..180)."));
      return false;
    }

    uint16_t ticks = minimumPWM + (uint32_t(maximumPWM-minimumPWM)*angle)/180;
    const bool gelukt = parent->WriteExtenderPWM(channel,ticks);

    if (gelukt) {
      lastPWM = ticks;
      hasPWM = true;
    } else {
      GA_DEBUG_PRINTLN(F("DEBUG: SG90 WriteExtenderPWM mislukt."));
    }

#ifdef TRACE
    if (GA_SERIAL) {
      GA_SERIAL.print(F("TRACE: SG90 kanaal=")); GA_SERIAL.print(channel);
      GA_SERIAL.print(F(" hoek=")); GA_SERIAL.print(angle);
      GA_SERIAL.print(F(" ticks=")); GA_SERIAL.print(ticks);
      uint16_t frequentie = 0;
      if (parent->ReadExtenderPWMFrequency(frequentie) && frequentie != 0) {
        GA_SERIAL.print(F(" frequentie=")); GA_SERIAL.print(frequentie);
        GA_SERIAL.print(F(" Hz puls="));
        GA_SERIAL.print((uint32_t(ticks) * 1000000UL + uint32_t(frequentie) * 2048UL) / (uint32_t(frequentie) * 4096UL));
        GA_SERIAL.print(F(" us"));
      }
      GA_SERIAL.println(gelukt ? F(" OK") : F(" FOUT"));
    }
#endif

    return gelukt;
  }

  // SetPulsLimitsMS: naam volgens afspraak; beide argumenten zijn microseconden (us), niet milliseconden.
  bool SetPulsLimitsMS(uint16_t minimumUs, uint16_t maximumUs) {
    if (!actief || !parent || minimumUs == 0 || minimumUs >= maximumUs) {
      GA_DEBUG_PRINTLN(F("DEBUG: SG90 ongeldige pulsgrenzen."));
      return false;
    }

    uint16_t frequentie = 0;
    if (!parent->ReadExtenderPWMFrequency(frequentie) || frequentie == 0) {
      GA_DEBUG_PRINTLN(F("DEBUG: SG90 PWM-frequentie niet beschikbaar."));
      return false;
    }

    const uint64_t minimumTicks = (uint64_t(minimumUs) * frequentie * 4096ULL + 500000ULL) / 1000000ULL;
    const uint64_t maximumTicks = (uint64_t(maximumUs) * frequentie * 4096ULL + 500000ULL) / 1000000ULL;
    if (minimumTicks == 0 || minimumTicks >= maximumTicks || maximumTicks >= 4096) {
      GA_DEBUG_PRINTLN(F("DEBUG: SG90 pulsgrenzen buiten PWM-bereik."));
      return false;
    }

    minimumPWM = static_cast<uint16_t>(minimumTicks);
    maximumPWM = static_cast<uint16_t>(maximumTicks);

#ifdef TRACE
    if (GA_SERIAL) {
      GA_SERIAL.print(F("TRACE: SG90 pulsgrenzen us=")); GA_SERIAL.print(minimumUs);
      GA_SERIAL.print(F("..")); GA_SERIAL.print(maximumUs);
      GA_SERIAL.print(F(" ticks=")); GA_SERIAL.print(minimumPWM);
      GA_SERIAL.print(F("..")); GA_SERIAL.println(maximumPWM);
    }
#endif

    return true;
  }

  bool SetAbsolutePulseWidthUS(uint16_t microseconden) {
    if (!actief || !parent || microseconden == 0) {
      GA_DEBUG_PRINTLN(F("DEBUG: SG90 pulsbreedteopdracht geweigerd."));
      return false;
    }

    uint16_t frequentie = 0;
    if (!parent->ReadExtenderPWMFrequency(frequentie) || frequentie == 0) {
      GA_DEBUG_PRINTLN(F("DEBUG: SG90 PWM-frequentie niet beschikbaar."));
      return false;
    }

    const uint64_t berekend = (uint64_t(microseconden) * frequentie * 4096ULL + 500000ULL) / 1000000ULL;
    if (berekend < minimumPWM || berekend > maximumPWM) {
      GA_DEBUG_PRINTLN(F("DEBUG: SG90 pulsbreedte buiten gekalibreerde grenzen."));
      return false;
    }

    const uint16_t ticks = static_cast<uint16_t>(berekend);
    const bool gelukt = parent->WriteExtenderPWM(channel, ticks);

    if (gelukt) {
      lastPWM = ticks;
      hasPWM = true;
    } else {
      GA_DEBUG_PRINTLN(F("DEBUG: SG90 WriteExtenderPWM mislukt."));
    }

#ifdef TRACE
    if (GA_SERIAL) {
      GA_SERIAL.print(F("TRACE: SG90 kanaal=")); GA_SERIAL.print(channel);
      GA_SERIAL.print(F(" puls=")); GA_SERIAL.print(microseconden);
      GA_SERIAL.print(F(" us frequentie=")); GA_SERIAL.print(frequentie);
      GA_SERIAL.print(F(" Hz ticks=")); GA_SERIAL.print(ticks);
      GA_SERIAL.println(gelukt ? F(" OK") : F(" FOUT"));
    }
#endif

    return gelukt;
  }

  // Alleen voor begeleide kalibratie: beperkte pulsen buiten de ingestelde servohoek-grenzen.
  // 750..2250 us is een softwaregrens, GEEN garantie dat de fysieke servo die waarden verdraagt.
  bool CalibrationServomotor(uint16_t microseconden) {
    if (!actief || !parent || microseconden < 750 || microseconden > 2250) {
      GA_DEBUG_PRINTLN(F("DEBUG: SG90 kalibratiepuls geweigerd (buiten 750..2250 us of inactief)."));
      return false;
    }

    uint16_t frequentie = 0;
    if (!parent->ReadExtenderPWMFrequency(frequentie) || frequentie == 0) {
      GA_DEBUG_PRINTLN(F("DEBUG: SG90 PWM-frequentie niet beschikbaar voor kalibratie."));
      return false;
    }

    const uint64_t berekend = (uint64_t(microseconden) * frequentie * 4096ULL + 500000ULL) / 1000000ULL;
    if (berekend == 0 || berekend >= 4096) {
      GA_DEBUG_PRINTLN(F("DEBUG: SG90 kalibratiepuls buiten 12-bit PWM-bereik."));
      return false;
    }

    const uint16_t ticks = static_cast<uint16_t>(berekend);
    const bool gelukt = parent->WriteExtenderPWM(channel, ticks);

    if (gelukt) {
      lastPWM = ticks;
      hasPWM = true;
    } else {
      GA_DEBUG_PRINTLN(F("DEBUG: SG90 WriteExtenderPWM bij kalibratie mislukt."));
    }

#ifdef TRACE
    if (GA_SERIAL) {
      GA_SERIAL.print(F("TRACE: SG90 kalibratie kanaal=")); GA_SERIAL.print(channel);
      GA_SERIAL.print(F(" puls=")); GA_SERIAL.print(microseconden);
      GA_SERIAL.print(F(" us frequentie=")); GA_SERIAL.print(frequentie);
      GA_SERIAL.print(F(" Hz ticks=")); GA_SERIAL.print(ticks);
      GA_SERIAL.println(gelukt ? F(" OK") : F(" FOUT"));
    }
#endif

    return gelukt;
  }

  bool ReadLastPulseWidthUS(uint16_t& microseconden) {
    if (!hasPWM || !parent) return false;

    uint16_t frequentie = 0;
    if (!parent->ReadExtenderPWMFrequency(frequentie) || frequentie == 0) return false;

    const uint32_t deler = uint32_t(frequentie) * 4096UL;
    microseconden = static_cast<uint16_t>((uint64_t(lastPWM) * 1000000ULL + deler / 2) / deler);
    return true;
  }

  void StopSturing() override { if(parent) parent->WriteExtenderPWM(channel,0); }
};

#endif
