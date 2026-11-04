#ifndef DCMOTORBESTURING_H
#define DCMOTORBESTURING_H

#include <Arduino.h>
#include "../../../../Configuratie/SystemConfig.h"

#if STURING_L293D_AANTAL > 0
#include "../../L293D.h"
#endif

#if STURING_L298N_AANTAL > 0
#include "../../L298N.h"
#endif

// snelheid = PCA9685 PWM-waarde 0..4095, geen gemeten toerental.
struct DCmotorBesturing {
#if STURING_L293D_AANTAL > 0
  explicit DCmotorBesturing(L293D& sturing) : l293d(&sturing) {}
#endif

#if STURING_L298N_AANTAL > 0
  explicit DCmotorBesturing(L298N& sturing) : l298n(&sturing) {}
#endif
  bool StartMotor();
  bool StopMotor();
  bool PauzeerMotor();
  bool HervatMotor();
  bool BeweegVooruit();
  bool BeweegAchteruit();
  bool DraairichtingOmkeren();
  bool StelSnelheidIn(uint16_t snelheid);
  bool StelDraairichtingEnSnelheidIn(bool vooruit, uint16_t snelheid);
private:
#if STURING_L293D_AANTAL > 0
  L293D* l293d = nullptr;
#endif

#if STURING_L298N_AANTAL > 0
  L298N* l298n = nullptr;
#endif
  uint16_t snelheidPWM = 0;
  bool richtingVooruit = true;
  bool gestart = false;
  bool gepauzeerd = false;
};

#endif
