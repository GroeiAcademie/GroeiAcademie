#include "DCmotorBesturing.h"

bool DCmotorBesturing::StartMotor() {
  if (gepauzeerd || snelheidPWM == 0) return false;
  bool gelukt = false;
#if STURING_L293D_AANTAL > 0
  if (l293d) gelukt = l293d->StelDraairichtingEnSnelheidIn(richtingVooruit, snelheidPWM);
#endif
#if STURING_L298N_AANTAL > 0
  if (l298n) gelukt = l298n->StelDraairichtingEnSnelheidIn(richtingVooruit, snelheidPWM);
#endif
  if (gelukt) gestart = true;
  return gelukt;
}

bool DCmotorBesturing::StopMotor() {
  bool actief = false;
#if STURING_L293D_AANTAL > 0
  if (l293d && l293d->actief) { l293d->StopSturing(); actief = true; }
#endif
#if STURING_L298N_AANTAL > 0
  if (l298n && l298n->actief) { l298n->StopSturing(); actief = true; }
#endif
  if (!actief) return false;
  gestart = false;
  gepauzeerd = false;
  snelheidPWM = 0;
  return true;
}

bool DCmotorBesturing::PauzeerMotor() {
  if (!gestart || gepauzeerd) return false;
  bool actief = false;
#if STURING_L293D_AANTAL > 0
  if (l293d && l293d->actief) { l293d->StopSturing(); actief = true; }
#endif
#if STURING_L298N_AANTAL > 0
  if (l298n && l298n->actief) { l298n->StopSturing(); actief = true; }
#endif
  if (!actief) return false;
  gestart = false;
  gepauzeerd = true;
  return true;
}

bool DCmotorBesturing::HervatMotor() {
  if (!gepauzeerd) return false;
  gepauzeerd = false;
  if (StartMotor()) return true;
  gepauzeerd = true;
  return false;
}

bool DCmotorBesturing::StelSnelheidIn(uint16_t snelheid) {
  if (snelheid > 4095 || gepauzeerd) return false;
  if (gestart && snelheid) {
    bool gelukt = false;
#if STURING_L293D_AANTAL > 0
    if (l293d) gelukt = l293d->StelDraairichtingEnSnelheidIn(richtingVooruit, snelheid);
#endif
#if STURING_L298N_AANTAL > 0
    if (l298n) gelukt = l298n->StelDraairichtingEnSnelheidIn(richtingVooruit, snelheid);
#endif
    if (!gelukt) return false;
  }
  if (gestart && !snelheid) {
#if STURING_L293D_AANTAL > 0
    if (l293d) l293d->StopSturing();
#endif
#if STURING_L298N_AANTAL > 0
    if (l298n) l298n->StopSturing();
#endif
    gestart = false;
  }
  snelheidPWM = snelheid;
  return true;
}

bool DCmotorBesturing::StelDraairichtingEnSnelheidIn(bool vooruit, uint16_t snelheid) {
  if (snelheid > 4095 || gepauzeerd) return false;
  // De driver zet ENABLE op nul voordat hij de draairichting wijzigt.
  if (gestart) {
    bool gelukt = false;
#if STURING_L293D_AANTAL > 0
    if (l293d) gelukt = l293d->StelDraairichtingEnSnelheidIn(vooruit, snelheid);
#endif
#if STURING_L298N_AANTAL > 0
    if (l298n) gelukt = l298n->StelDraairichtingEnSnelheidIn(vooruit, snelheid);
#endif
    if (!gelukt) return false;
  }
  richtingVooruit = vooruit;
  snelheidPWM = snelheid;
  if (!snelheid) gestart = false;
  return true;
}

bool DCmotorBesturing::BeweegVooruit() {
  if (gepauzeerd || !snelheidPWM) return false;
  richtingVooruit = true;
  return StartMotor();
}

bool DCmotorBesturing::BeweegAchteruit() {
  if (gepauzeerd || !snelheidPWM) return false;
  richtingVooruit = false;
  return StartMotor();
}

bool DCmotorBesturing::DraairichtingOmkeren() {
  if (gepauzeerd) return false;
  if (gestart) {
#if STURING_L293D_AANTAL > 0 || STURING_L298N_AANTAL > 0
    const bool omgekeerd = !richtingVooruit;
#endif
    bool gelukt = false;
#if STURING_L293D_AANTAL > 0
    if (l293d) gelukt = l293d->StelDraairichtingEnSnelheidIn(omgekeerd, snelheidPWM);
#endif
#if STURING_L298N_AANTAL > 0
    if (l298n) gelukt = l298n->StelDraairichtingEnSnelheidIn(omgekeerd, snelheidPWM);
#endif
    if (!gelukt) return false;
  }
  richtingVooruit = !richtingVooruit;
  return true;
}
