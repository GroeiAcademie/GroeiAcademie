// Compileer- en levenscyclustest voor DCmotorL293D. Geen automatische beweging of schakelactie.
#include <GroeiAcademie.h>
#if STURING_DCMOTORL293D_AANTAL == 0
  #error Zet STURING_DCMOTORL293D_AANTAL op minstens 1 voor deze test.
#endif
#if EXTENDER_PCA9685_AANTAL == 0
  #error Schakel de vereiste Extender in voor deze Sturing-test.
#endif

ExtenderPCA9685 extender(&Native, GedeeldeBusComponent::PWM_PCA9685, I2C_ADDRESS_EXTENDER_PCA9685_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL);
DCmotorL293D sturing(&extender,0,1,2);


bool testActief = false;
bool startGelukt = false;
bool testGestopt = false;
String invoer;
void antwoord(bool ok) { GA_SERIAL.println(ok ? F("OPDRACHT: OK") : F("OPDRACHT: FOUT")); }
void toonStart() {
  GA_SERIAL.println(F("RUN42 HARDWARETEST: typ START na controle van bedrading en voeding."));
  GA_SERIAL.println(F("STOP stopt testuitvoer; herstart voor nieuwe test."));
}
void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  startGelukt = extender.aanmelden() && extender.controleren() && extender.inpluggen() && extender.activeren() && sturing.aanmelden() && sturing.controleren() && sturing.inpluggen() && sturing.activeren();
  GA_SERIAL.println(startGelukt ? F("LEVENSCYCLUS: OK") : F("LEVENSCYCLUS: FOUT"));
  toonStart();
}
void loop() {
  if (!GA_SERIAL.available()) return;
  invoer = GA_SERIAL.readStringUntil('\n');
  invoer.trim();
  invoer.toUpperCase();
  if (invoer == "STOP") { if (startGelukt) sturing.StopSturing(); testActief=false; testGestopt=true; GA_SERIAL.println(F("STOP: uitvoer uit / opdracht gestopt. Herstart om opnieuw te testen.")); return; }
  if (testGestopt) { GA_SERIAL.println(F("Test gestopt: reset Arduino.")); return; }
  if (!startGelukt) { GA_SERIAL.println(F("FOUT: levenscyclus.")); return; }
  if (invoer == "START") { if (!testActief) { testActief=true; GA_SERIAL.println(F("START: bevestigd.")); } return; }
  if (!testActief) { GA_SERIAL.println(F("Eerst START invoeren.")); return; }
  if (invoer == "0") { sturing.StopSturing(); GA_SERIAL.println(F("Motor gestopt (EN=0).")); return; }
  if (!(invoer.startsWith("+") || invoer.startsWith("-"))) { GA_SERIAL.println(F("Gebruik +1..+100, -1..-100 of 0.")); return; }
  int snelheid = invoer.toInt();
  if (snelheid == 0 || snelheid < -100 || snelheid > 100) { GA_SERIAL.println(F("FOUT: bereik -100..100")); return; }
  uint16_t pwm = (uint32_t)abs(snelheid) * 4095 / 100;
  antwoord(sturing.setDirectionAndSpeed(snelheid > 0, pwm));
  GA_SERIAL.print(F("Gevraagd percentage: ")); GA_SERIAL.println(snelheid);
}
