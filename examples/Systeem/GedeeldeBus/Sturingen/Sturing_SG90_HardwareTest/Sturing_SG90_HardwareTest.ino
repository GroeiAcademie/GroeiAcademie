// Compileer- en levenscyclustest voor SG90. Geen automatische beweging of schakelactie.
#include <GroeiAcademie.h>
#if STURING_SG90_AANTAL == 0
  #error Zet STURING_SG90_AANTAL op minstens 1 voor deze test.
#endif
#if EXTENDER_PCA9685_AANTAL == 0
  #error Schakel de vereiste Extender in voor deze Sturing-test.
#endif

ExtenderPCA9685 extender(&Native, GedeeldeBusComponent::PWM_PCA9685, I2C_ADDRESS_EXTENDER_PCA9685_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL);
SG90 sturing(&extender,0,205,410);


bool testActief = false;
bool startGelukt = false;
bool testGestopt = false;
String invoer;
void antwoord(bool ok) { GA_SERIAL.println(ok ? F("OPDRACHT: OK") : F("OPDRACHT: FOUT")); }
void toonStart() {
  GA_SERIAL.println(F("RUN42 HARDWARETEST: typ START na controle van bedrading en voeding."));
  GA_SERIAL.println(F("STOP stopt testuitvoer; herstart voor nieuwe test."));
}
int huidigePositie = 90;
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
  if (invoer.startsWith("A")) {
    int positie = invoer.substring(1).toInt();
    if (positie < 0 || positie > 180 || invoer.length() < 2) { GA_SERIAL.println(F("FOUT: A0..A180")); return; }
    antwoord(sturing.setServoAngle((uint8_t)positie)); huidigePositie = positie;
  } else if (invoer.startsWith("+") || invoer.startsWith("-")) {
    int stap = invoer.toInt(); int positie = huidigePositie + stap;
    if (positie < 0 || positie > 180) { GA_SERIAL.println(F("FOUT: hoek buiten 0..180")); return; }
    antwoord(sturing.setServoAngle((uint8_t)positie)); huidigePositie = positie;
  } else { GA_SERIAL.println(F("Gebruik A90, +15 of -15.")); return; }
  GA_SERIAL.print(F("Gevraagde hoek: ")); GA_SERIAL.println(huidigePositie);
}
