#include "ServomotorBesturing.h"

bool ServomotorBesturing::PositioneerAbsoluutNaarHoek(uint8_t hoek) {
  if (hoek > 180 || gepauzeerd) return false;
  bool gelukt = false;
#if STURING_SG90_AANTAL > 0
  if (sg90) gelukt = sg90->PositioneerAbsoluutNaarHoek(hoek);
#endif
#if STURING_MG996R_AANTAL > 0
  if (mg996r) gelukt = mg996r->PositioneerAbsoluutNaarHoek(hoek);
#endif
  if (gelukt) { huidigePositie = hoek; positieBekend = true; gestart = true; }
  return gelukt;
}

bool ServomotorBesturing::DraaiRelatiefOverHoek(int16_t aantalGraden) {
  if (!positieBekend || gepauzeerd) return false;
  const int32_t hoek = static_cast<int32_t>(huidigePositie) + aantalGraden;
  if (hoek < 0 || hoek > 180) return false;
  return PositioneerAbsoluutNaarHoek(static_cast<uint8_t>(hoek));
}

bool ServomotorBesturing::PositioneerNaarMiddenpositie() { return PositioneerAbsoluutNaarHoek(90); }

bool ServomotorBesturing::StartMotor() {
  if (!positieBekend || gepauzeerd) return false;
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
  if (!gestart || gepauzeerd || !positieBekend) return false;
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
  if (!gepauzeerd || !positieBekend) return false;
  gepauzeerd = false;
  if (StartMotor()) return true;
  gepauzeerd = true;
  return false;
}

bool ServomotorBesturing::SchakelMotorIn() { return StartMotor(); }
bool ServomotorBesturing::SchakelMotorUit() { return StopMotor(); }
