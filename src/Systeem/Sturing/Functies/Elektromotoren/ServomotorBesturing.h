#ifndef SERVOMOTORBESTURING_H
#define SERVOMOTORBESTURING_H

#include <Arduino.h>
#include "../../../../Configuratie/SystemConfig.h"

#if STURING_SG90_AANTAL > 0
#include "../../SG90.h"
#endif

#if STURING_MG996R_AANTAL > 0
#include "../../MG996R.h"
#endif

// huidigePositie is de laatst opgedragen hoek, geen gemeten motorpositie.
struct ServomotorBesturing {
#if STURING_SG90_AANTAL > 0
  explicit ServomotorBesturing(SG90& sturing) : sg90(&sturing) {}
#endif

#if STURING_MG996R_AANTAL > 0
  explicit ServomotorBesturing(MG996R& sturing) : mg996r(&sturing) {}
#endif
  bool StartMotor();
  bool StopMotor();
  bool PauzeerMotor();
  bool HervatMotor();
  bool SchakelMotorIn();
  bool SchakelMotorUit();
  bool PositioneerAbsoluutNaarHoek(uint8_t hoek);
  bool DraaiRelatiefOverHoek(int16_t aantalGraden);
  bool PositioneerNaarMiddenpositie();
  bool SetPulsLimitsMS(uint16_t minimumUs, uint16_t maximumUs);
  bool SetAbsolutePulseWidthUS(uint16_t microseconden);
  bool CalibrationServomotor(uint16_t microseconden);
  bool ReadTargetAngle(uint8_t& hoek) const;
  bool ReadLastPulseWidthUS(uint16_t& microseconden);
private:
#if STURING_SG90_AANTAL > 0
  SG90* sg90 = nullptr;
#endif

#if STURING_MG996R_AANTAL > 0
  MG996R* mg996r = nullptr;
#endif
  uint8_t huidigePositie = 0;
  bool positieBekend = false;
  uint16_t laatstePulsbreedteUS = 0;
  bool pulsOpdracht = false;
  bool gestart = false;
  bool gepauzeerd = false;
};

#endif
