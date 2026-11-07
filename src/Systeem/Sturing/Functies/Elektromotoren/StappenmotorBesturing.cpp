#include "StappenmotorBesturing.h"

bool StappenmotorBesturing::StartMotor() {
  bool actief = false;
#if STURING_A4988_AANTAL > 0
  if (a4988) actief = a4988->actief;
#endif
#if STURING_ULN2003_AANTAL > 0
  if (uln2003) actief = uln2003->actief;
#endif
  if (!actief) return false;
  gestart = true;
  gepauzeerd = false;
  return true;
}

bool StappenmotorBesturing::StopMotor() {
  bool actief = false;
#if STURING_A4988_AANTAL > 0
  if (a4988 && a4988->actief) { a4988->StopSturing(); actief = true; }
#endif
#if STURING_ULN2003_AANTAL > 0
  if (uln2003 && uln2003->actief) { uln2003->StopSturing(); actief = true; }
#endif
  if (!actief) return false;
  gestart = false;
  gepauzeerd = false;
  return true;
}

bool StappenmotorBesturing::PauzeerMotor() {
  if (!gestart || gepauzeerd) return false;
#if STURING_A4988_AANTAL > 0
  if (a4988 && !a4988->actief) return false;
  if (a4988) a4988->StopSturing();
#endif
#if STURING_ULN2003_AANTAL > 0
  if (uln2003 && !uln2003->actief) return false;
  if (uln2003) uln2003->StopSturing();
#endif
  gepauzeerd = true;
  gestart = false;
  return true;
}

bool StappenmotorBesturing::HervatMotor() {
  if (!gepauzeerd) return false;
  return StartMotor();
}

bool StappenmotorBesturing::DraairichtingOmkeren() {
  richtingVooruit = !richtingVooruit;
  return true;
}

bool StappenmotorBesturing::VoerEenStapUit() {
  if (!gestart || gepauzeerd) return false;
#if STURING_A4988_AANTAL > 0
  if (a4988) return a4988->VoerEenStapUit(richtingVooruit);
#endif
#if STURING_ULN2003_AANTAL > 0
  if (uln2003) return uln2003->VoerEenStapUit(richtingVooruit);
#endif
  return false;
}

bool StappenmotorBesturing::VoerMeerdereStappenUit(uint32_t aantalStappen) {
  if (!gestart || gepauzeerd || !aantalStappen) return false;
  // Synchroon: tijdens deze aanroep kan geen nieuw pauzecommando worden verwerkt.
  for (uint32_t stap = 0; stap < aantalStappen; ++stap) {
    const unsigned long start = micros();
    if (!VoerEenStapUit()) return false;
    if (stap + 1 < aantalStappen) {
      const unsigned long verstreken = micros() - start;
      const unsigned long periode = 1000000UL / stappenPerSeconde;
      if (verstreken < periode) delayMicroseconds(periode - verstreken);
    }
  }
  return true;
}

bool StappenmotorBesturing::StelSnelheidIn(uint16_t stappenPerSeconde) {
  if (stappenPerSeconde < 1 || stappenPerSeconde > 1000 || gestart) return false;
  this->stappenPerSeconde = stappenPerSeconde;
  return true;
}
