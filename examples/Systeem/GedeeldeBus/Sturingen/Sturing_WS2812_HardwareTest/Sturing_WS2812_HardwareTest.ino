// Compileer- en levenscyclustest voor WS2812. Geen automatische beweging of schakelactie.
#include <GroeiAcademie.h>
#if STURING_WS2812_AANTAL == 0
  #error Zet STURING_WS2812_AANTAL op minstens 1 voor deze test.
#endif


WS2812 sturing(&Native,HardwareResourcePin::NP_D6);


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
  startGelukt = sturing.aanmelden() && sturing.controleren() && sturing.inpluggen() && sturing.activeren();
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
  if (invoer == "X") { sturing.StopSturing(); GA_SERIAL.println(F("Matrix gewist.")); return; }
  // PX x y r g b; H = hoeken; R = rijen; K = kolommen; D = diagonaal.
  if (invoer.startsWith("PX ")) {
    int x,y,r,g,b;
    if (sscanf(invoer.c_str(),"PX %d %d %d %d %d",&x,&y,&r,&g,&b)!=5 || x<0 || x>7 || y<0 || y>7 || r<0 || r>255 || g<0 || g>255 || b<0 || b>255) { GA_SERIAL.println(F("Gebruik PX x y r g b (x/y 0..7, RGB 0..255).")); return; }
    antwoord(sturing.setPixelColor((uint8_t)x,(uint8_t)y,((uint32_t)r<<16)|((uint32_t)g<<8)|b)); return;
  }
  if (invoer!="H" && invoer!="R" && invoer!="K" && invoer!="D") { GA_SERIAL.println(F("Gebruik PX x y r g b, H, R, K, D, X.")); return; }
  sturing.StopSturing();
  bool ok = true;
  for (uint8_t y=0;y<8;y++) for(uint8_t x=0;x<8;x++) {
    uint32_t kleur=0;
    if(invoer=="H") { if(x==0&&y==0)kleur=0xFF0000; if(x==7&&y==0)kleur=0x00FF00; if(x==0&&y==7)kleur=0x0000FF; if(x==7&&y==7)kleur=0xFFFF00; }
    if(invoer=="R") kleur=(y%3==0)?0x400000:((y%3==1)?0x004000:0x000040);
    if(invoer=="K") kleur=(x%3==0)?0x400000:((x%3==1)?0x004000:0x000040);
    if(invoer=="D" && x==y) kleur=0x004000;
    if(kleur && !sturing.setPixelColor(x,y,kleur)) ok=false;
  }
  antwoord(ok); GA_SERIAL.println(F("Controleer het patroon visueel."));
}
