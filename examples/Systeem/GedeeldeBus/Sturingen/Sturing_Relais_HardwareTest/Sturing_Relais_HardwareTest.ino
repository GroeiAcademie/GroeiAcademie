// Compileer- en levenscyclustest voor Relais. Geen automatische beweging of schakelactie.
#include <GroeiAcademie.h>
#if STURING_RELAIS_AANTAL == 0
  #error Zet STURING_RELAIS_AANTAL op minstens 1 voor deze test.
#endif
#if EXTENDER_PCF8574_AANTAL == 0
  #error Schakel de vereiste Extender in voor deze Sturing-test.
#endif
#if !GEDEELDE_BUS_MIXED_PINS
  #error GEDEELDE_BUS_MIXED_PINS is vereist voor deze Sturing-test.
#endif

ExtenderPCF8574 extender(&Native, GedeeldeBusComponent::DIGITAL_PINS_PCF8574, I2C_ADDRESS_EXTENDER_PCF8574_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, EXTENDER_PCF8574_1_INT);
Relais sturing(&extender,0,LOW);


bool testActief = false;
bool startGelukt = false;
bool testGestopt = false;
String invoer;
void antwoord(bool ok) { GA_SERIAL.println(ok ? F("OPDRACHT: OK") : F("OPDRACHT: FOUT")); }
void toonStart() {
  GA_SERIAL.println(F("RUN42 HARDWARETEST: typ START na controle van bedrading en voeding."));
  GA_SERIAL.println(F("STOP stopt testuitvoer; herstart voor nieuwe test."));
}
// Terugmelding via NO -> digitale pin D2, COM -> GND.
// INPUT_PULLUP: NO open is HIGH, NO dicht is LOW.
// LED op NC vraagt een aparte, stroombegrensde en elektrisch veilige kring.
const uint8_t meetPin = 2;
void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  startGelukt = extender.aanmelden() && extender.controleren() && extender.inpluggen() && extender.activeren() && sturing.aanmelden() && sturing.controleren() && sturing.inpluggen() && sturing.activeren();
  GA_SERIAL.println(startGelukt ? F("LEVENSCYCLUS: OK") : F("LEVENSCYCLUS: FOUT"));
  pinMode(meetPin,INPUT_PULLUP);
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
  if (invoer != "1" && invoer != "0") { GA_SERIAL.println(F("Gebruik 1 (AAN) of 0 (UIT).")); return; }
  bool aan = invoer == "1";
  bool gelukt = sturing.setRelayStatusOnOff(aan);
  if (!gelukt) { antwoord(false); return; }
  delay(80);
  bool gesloten = digitalRead(meetPin) == LOW;
  GA_SERIAL.print(F("NO-contact: ")); GA_SERIAL.println(gesloten ? F("GESLOTEN") : F("OPEN"));
  GA_SERIAL.println(gesloten == aan ? F("TERUGMELDING: PASS") : F("TERUGMELDING: FAIL (controleer polariteit/bedrading)"));
  GA_SERIAL.println(F("LED visueel controleren (geen automatische LED-feedback)."));
}
