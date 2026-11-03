#ifndef STAPPENMOTORBESTURING_H
#define STAPPENMOTORBESTURING_H
#include <Arduino.h>
#include "../../../../Configuratie/SystemConfig.h"
#if STURING_A4988_AANTAL > 0
#include "../../A4988.h"
#endif
#if STURING_ULN2003_AANTAL > 0
#include "../../ULN2003.h"
#endif

// Sturing blijft verantwoordelijk voor de hardware. Een uitgevoerde stap is niet fysiek bevestigd.
struct StappenmotorBesturing {
#if STURING_A4988_AANTAL > 0
  explicit StappenmotorBesturing(A4988& sturing) : a4988(&sturing) {}
#endif
#if STURING_ULN2003_AANTAL > 0
  explicit StappenmotorBesturing(ULN2003& sturing) : uln2003(&sturing) {}
#endif
  bool StartMotor();
  bool StopMotor();
  bool PauzeerMotor();
  bool HervatMotor();
  bool DraairichtingOmkeren();
  bool VoerEenStapUit();
  bool VoerMeerdereStappenUit(uint32_t aantalStappen);
private:
#if STURING_A4988_AANTAL > 0
  A4988* a4988 = nullptr;
#endif
#if STURING_ULN2003_AANTAL > 0
  ULN2003* uln2003 = nullptr;
#endif
  bool gestart = false;
  bool gepauzeerd = false;
  bool richtingVooruit = true;
};
#endif
