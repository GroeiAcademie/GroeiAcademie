// MAX7219: 8x8 Sturing, native hardware-SPI; geen Screen of tekst.
// Test voor spiraalbeweging en impulsbediening.
// Gebruik voor de werkelijke MH-module eerst de officiële MD_MAX72xx_HW_Mapper.
// De hardwaretypekeuze FC16_HW hieronder is VOORLOPIG en moet visueel worden bevestigd.
// Bedrading: DIN -> MOSI, CLK -> SCK, CS -> D10, VCC -> modulegeschikte 5V, GND -> GND.
// Bij 3,3V-logica op een 5V-MAX7219 is mogelijk niveauomzetting nodig.
#include <GroeiAcademie.h>
#include <Systeem/Sturing/Functies/MatrixFuncties/SpiraalPatroon.h>
#if STURING_MAX7219_AANTAL == 0
  #error Zet STURING_MAX7219_AANTAL op minstens 1 voor deze test.
#endif

MAX7219 sturing(&Native,HardwareResourcePin::NP_D10,MD_MAX72XX::FC16_HW);
uint8_t beeld[8]={0,0,0,0,0,0,0,0};
int8_t huidigeStap=-1;
bool laatSpoorAchter=true;
uint8_t stapGrootte=1;
bool wrapAround=false;
bool startGelukt=false;
bool testActief=false;
bool testGestopt=false;
bool autoActief=false;
bool autoNaarBinnen=true;
uint16_t stapVertragingms=150;
uint32_t vorigeStapMs=0;

void Reset() {
  for (uint8_t i=0;i<8;i++) beeld[i]=0;
  sturing.ToonPatroon(8,beeld);
  huidigeStap=-1;
}

void VerwerkImpuls(bool isUp) {
  if (!BerekenSpiraalPatroon(8,huidigeStap,isUp,stapGrootte,wrapAround,laatSpoorAchter,beeld)) return;
  sturing.ToonPatroon(8,beeld);
  GA_SERIAL.print(F("SPIRAAL STAP: ")); GA_SERIAL.println(huidigeStap);
}

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  startGelukt=sturing.aanmelden() && sturing.controleren() && sturing.inpluggen() && sturing.activeren();
  GA_SERIAL.println(startGelukt?F("LEVENSCYCLUS: OK"):F("LEVENSCYCLUS: FOUT"));
  GA_SERIAL.println(F("Controleer bedrading en matrixmapping. Typ START voor de test."));
  GA_SERIAL.println(F("UP, DOWN, AUTO, AUTOTERUG, SPOOR 0/1, STAP 1..64, WRAP 0/1"));
  GA_SERIAL.println(F("SNELHEID 20..5000, HELDER 0..15, PIXEL x y 0/1, RESET, STOP"));
}
void loop() {
  if (GA_SERIAL.available()) {
    String invoer=GA_SERIAL.readStringUntil('\n');
    invoer.trim(); invoer.toUpperCase();
    if (invoer=="STOP") { sturing.StopSturing(); autoActief=false; testActief=false; testGestopt=true; GA_SERIAL.println(F("GESTOPT: herstart Arduino.")); return; }
    if (testGestopt) { GA_SERIAL.println(F("Test gestopt: herstart Arduino.")); return; }
    if (!startGelukt) { GA_SERIAL.println(F("FOUT: levenscyclus.")); return; }
    if (invoer=="START") { testActief=true; GA_SERIAL.println(F("START: bevestigd.")); return; }
    if (!testActief) { GA_SERIAL.println(F("Eerst START.")); return; }
    autoActief=false;
    if (invoer=="RESET") { Reset(); GA_SERIAL.println(F("RESET: OK")); }
    else if (invoer=="UP") VerwerkImpuls(true);
    else if (invoer=="DOWN") VerwerkImpuls(false);
    else if (invoer=="AUTO") { Reset(); autoNaarBinnen=true; autoActief=true; vorigeStapMs=millis(); }
    else if (invoer=="AUTOTERUG") { autoNaarBinnen=false; autoActief=true; vorigeStapMs=millis(); }
    else if (invoer.startsWith("SPOOR ")) {
      if (invoer=="SPOOR 0" || invoer=="SPOOR 1") { laatSpoorAchter=(invoer=="SPOOR 1"); Reset(); GA_SERIAL.println(F("SPOOR: OK")); }
      else GA_SERIAL.println(F("Gebruik SPOOR 0 of 1."));
    }
    else if (invoer.startsWith("WRAP ")) {
      if (invoer=="WRAP 0" || invoer=="WRAP 1") { wrapAround=(invoer=="WRAP 1"); GA_SERIAL.println(F("WRAP: OK")); }
      else GA_SERIAL.println(F("Gebruik WRAP 0 of 1."));
    }
    else if (invoer.startsWith("STAP ")) {
      String arg=invoer.substring(5); bool cijfers=arg.length()>0;
      for (unsigned int i=0;i<arg.length();i++) if (!isDigit(arg[i])) cijfers=false;
      long waarde=arg.toInt();
      if (cijfers && waarde>=1 && waarde<=64) { stapGrootte=static_cast<uint8_t>(waarde); GA_SERIAL.println(F("STAP: OK")); }
      else GA_SERIAL.println(F("Gebruik STAP 1..64."));
    }
    else if (invoer.startsWith("SNELHEID ")) {
      String arg=invoer.substring(9); bool cijfers=arg.length()>0;
      for (unsigned int i=0;i<arg.length();i++) if (!isDigit(arg[i])) cijfers=false;
      long waarde=arg.toInt();
      if (cijfers && waarde>=20 && waarde<=5000) { stapVertragingms=static_cast<uint16_t>(waarde); GA_SERIAL.println(F("SNELHEID: OK")); }
      else GA_SERIAL.println(F("Gebruik SNELHEID 20..5000 (ms)."));
    }
    else if (invoer.startsWith("HELDER ")) {
      String arg=invoer.substring(7); bool cijfers=arg.length()>0;
      for (unsigned int i=0;i<arg.length();i++) if (!isDigit(arg[i])) cijfers=false;
      long waarde=arg.toInt();
      GA_SERIAL.println(cijfers && waarde>=0 && waarde<=15 && sturing.setIntensity(static_cast<uint8_t>(waarde)) ? F("HELDER: OK") : F("HELDER: FOUT (0..15)"));
    }
    else if (invoer.startsWith("PIXEL ")) {
      // Exact één cijfer per coördinaat; geen niet-gevalideerde parsing of extra hardwareacties.
      if (invoer.length()==11 && invoer[6]>='0' && invoer[6]<='7' && invoer[7]==' ' &&
          invoer[8]>='0' && invoer[8]<='7' && invoer[9]==' ' && (invoer[10]=='0' || invoer[10]=='1'))
      {
        const uint8_t x=static_cast<uint8_t>(invoer[6]-'0'), y=static_cast<uint8_t>(invoer[8]-'0');
        const bool aan=invoer[10]=='1';
        const bool ok=sturing.setPixel(x,y,aan);
        if (ok) {
          if (aan) beeld[y] |= static_cast<uint8_t>(1u<<x);
          else beeld[y] &= static_cast<uint8_t>(~(1u<<x));
        }
        GA_SERIAL.println(ok?F("PIXEL: OK"):F("PIXEL: FOUT"));
      }
      else GA_SERIAL.println(F("Gebruik PIXEL x y 0/1, met x,y van 0..7."));
    }
    else GA_SERIAL.println(F("Onbekende opdracht."));
  }
  if (testActief && !testGestopt && autoActief && (uint32_t)(millis()-vorigeStapMs)>=stapVertragingms) {
    vorigeStapMs=millis();
    if ((autoNaarBinnen && huidigeStap==63) || (!autoNaarBinnen && huidigeStap==-1)) { autoActief=false; GA_SERIAL.println(F("AUTO: klaar.")); }
    else VerwerkImpuls(autoNaarBinnen);
  }
}
