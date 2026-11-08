#include "ServomotorBesturing.h"

bool ServomotorBesturing::PositioneerAbsoluutNaarHoek(uint8_t hoek) {
  if (hoek > 180 || gepauzeerd) {
    GA_DEBUG_PRINTLN(F("DEBUG: ServomotorBesturing hoekopdracht geweigerd (hoek buiten bereik of gepauzeerd)."));
    return false;
  }
  bool gelukt = false;
#if STURING_SG90_AANTAL > 0
  if (sg90) gelukt = sg90->PositioneerAbsoluutNaarHoek(hoek);
#endif
#if STURING_MG996R_AANTAL > 0
  if (mg996r) gelukt = mg996r->PositioneerAbsoluutNaarHoek(hoek);
#endif
  if (gelukt) { huidigePositie = hoek; positieBekend = true; pulsOpdracht = false; gestart = true; }
  return gelukt;
}

bool ServomotorBesturing::DraaiRelatiefOverHoek(int16_t aantalGraden) {
  if (!positieBekend || gepauzeerd) {
    GA_DEBUG_PRINTLN(F("DEBUG: ServomotorBesturing relatieve hoek geweigerd (doelhoek onbekend of gepauzeerd)."));
    return false;
  }
  const int32_t hoek = static_cast<int32_t>(huidigePositie) + aantalGraden;
  if (hoek < 0 || hoek > 180) {
    GA_DEBUG_PRINTLN(F("DEBUG: ServomotorBesturing relatieve doelhoek buiten 0..180."));
    return false;
  }
  return PositioneerAbsoluutNaarHoek(static_cast<uint8_t>(hoek));
}

bool ServomotorBesturing::PositioneerNaarMiddenpositie() { return PositioneerAbsoluutNaarHoek(90); }

bool ServomotorBesturing::SetPulsLimitsMS(uint16_t minimumUs, uint16_t maximumUs) {
  bool gelukt = false;
#if STURING_SG90_AANTAL > 0
  if (sg90) gelukt = sg90->SetPulsLimitsMS(minimumUs, maximumUs);
#endif
#if STURING_MG996R_AANTAL > 0
  if (mg996r) gelukt = mg996r->SetPulsLimitsMS(minimumUs, maximumUs);
#endif
  return gelukt;
}

bool ServomotorBesturing::SetAbsolutePulseWidthUS(uint16_t microseconden) {
  if (gepauzeerd) {
    GA_DEBUG_PRINTLN(F("DEBUG: ServomotorBesturing pulsbreedteopdracht geweigerd tijdens pauze."));
    return false;
  }
  bool gelukt = false;
#if STURING_SG90_AANTAL > 0
  if (sg90) gelukt = sg90->SetAbsolutePulseWidthUS(microseconden);
#endif
#if STURING_MG996R_AANTAL > 0
  if (mg996r) gelukt = mg996r->SetAbsolutePulseWidthUS(microseconden);
#endif
  if (gelukt) {
    laatstePulsbreedteUS = microseconden;
    pulsOpdracht = true;
    positieBekend = false;
    gestart = true;
  }
  return gelukt;
}

bool ServomotorBesturing::CalibrationServomotor(uint16_t microseconden) {
  if (gepauzeerd) {
    GA_DEBUG_PRINTLN(F("DEBUG: ServomotorBesturing kalibratie geweigerd tijdens pauze."));
    return false;
  }

  bool gelukt = false;
#if STURING_SG90_AANTAL > 0
  if (sg90) gelukt = sg90->CalibrationServomotor(microseconden);
#endif
#if STURING_MG996R_AANTAL > 0
  if (mg996r) gelukt = mg996r->CalibrationServomotor(microseconden);
#endif

  if (gelukt) {
    // Geen gewone doelopdracht bewaren: kalibratie mag niet automatisch herstarten of hervatten.
    positieBekend = false;
    pulsOpdracht = false;
    gestart = true;
  }

  return gelukt;
}

bool ServomotorBesturing::ReadTargetAngle(uint8_t& hoek) const {
  if (!positieBekend) return false;
  hoek = huidigePositie;
  return true;
}

bool ServomotorBesturing::ReadLastPulseWidthUS(uint16_t& microseconden) {
#if STURING_SG90_AANTAL > 0
  if (sg90) return sg90->ReadLastPulseWidthUS(microseconden);
#endif
#if STURING_MG996R_AANTAL > 0
  if (mg996r) return mg996r->ReadLastPulseWidthUS(microseconden);
#endif
  return false;
}

bool ServomotorBesturing::StartMotor() {
  if (gepauzeerd) {
    GA_DEBUG_PRINTLN(F("DEBUG: ServomotorBesturing kan niet starten tijdens pauze."));
    return false;
  }
  if (pulsOpdracht) return SetAbsolutePulseWidthUS(laatstePulsbreedteUS);
  if (!positieBekend) {
    GA_DEBUG_PRINTLN(F("DEBUG: ServomotorBesturing geen bewaarde doelopdracht om te starten."));
    return false;
  }
  return PositioneerAbsoluutNaarHoek(huidigePositie);
}

bool ServomotorBesturing::StopMotor() {
  bool actief = false;
#if STURING_SG90_AANTAL > 0
  if (sg90 && sg90->actief) { sg90->StopSturing(); actief = true; }
#endif
#if STURING_MG996R_AANTAL > 0
  if (mg996r && mg996r->actief) { mg996r->StopSturing(); actief = true; }
#endif
  if (!actief) return false;
  gestart = false;
  gepauzeerd = false;
  return true;
}

bool ServomotorBesturing::PauzeerMotor() {
  if (!gestart || gepauzeerd || (!positieBekend && !pulsOpdracht)) {
    GA_DEBUG_PRINTLN(F("DEBUG: ServomotorBesturing pauzeren geweigerd (geen actieve doelopdracht)."));
    return false;
  }
  bool actief = false;
#if STURING_SG90_AANTAL > 0
  if (sg90 && sg90->actief) { sg90->StopSturing(); actief = true; }
#endif
#if STURING_MG996R_AANTAL > 0
  if (mg996r && mg996r->actief) { mg996r->StopSturing(); actief = true; }
#endif
  if (!actief) return false;
  gestart = false;
  gepauzeerd = true;
  return true;
}

bool ServomotorBesturing::HervatMotor() {
  if (!gepauzeerd || (!positieBekend && !pulsOpdracht)) {
    GA_DEBUG_PRINTLN(F("DEBUG: ServomotorBesturing hervatten geweigerd (geen gepauzeerde doelopdracht)."));
    return false;
  }
  gepauzeerd = false;
  if (StartMotor()) return true;
  gepauzeerd = true;
  return false;
}

bool ServomotorBesturing::SchakelMotorIn() { return StartMotor(); }
bool ServomotorBesturing::SchakelMotorUit() { return StopMotor(); }
