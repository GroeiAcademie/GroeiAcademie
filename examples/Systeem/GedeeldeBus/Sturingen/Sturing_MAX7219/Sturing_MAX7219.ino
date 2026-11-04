// ============================================================================
// Sturing_MAX7219
// ============================================================================
// MAX7219: 8x8 Sturing, native hardware-SPI; geen Screen of tekst.
// Test voor spiraalbeweging en impulsbediening.
// Gebruik voor de werkelijke MH-module eerst de officiële MD_MAX72xx_HW_Mapper.
// De hardwaretypekeuze FC16_HW hieronder is VOORLOPIG en moet visueel worden bevestigd.
// Bedrading: DIN -> MOSI, CLK -> SCK, CS -> D10, VCC -> modulegeschikte 5V, GND -> GND.
// Bij 3,3V-logica op een 5V-MAX7219 is mogelijk niveauomzetting nodig.
// Hardware-SPI-aansluitingen volgens src/Configuratie/UserConfig.h: DIN=MOSI, CLK=SCK, CS=D10.
// UNO R4 Minima/WiFi: DIN=D11 (MOSI), CLK=D13 (SCK), CS=D10.
// ESP32 D1 UNO R32: DIN=GPIO23 (D11), CLK=GPIO18 (D13), CS=GPIO5 (D10).
// ESP32-S3 ARDI-32/DEV: DIN=GPIO11 (D11), CLK=GPIO12 (D13), CS=GPIO10 (D10).
// RP2040 Cytron Maker UNO: DIN=GPIO11 (D11), CLK=GPIO10 (D13), CS=GPIO13 (D10).
// UNO Q: DIN=D11, CLK=D13, CS=D10; STM32 F401RE Nucleo: DIN=PA7 (D11), CLK=PA5 (D13), CS=PB6 (D10, standaard).
// Selecteer de bij jouw MAX7219-module passende MD_MAX72XX-hardwaremapping; FC16_HW is hier nog niet bevestigd.
// ============================================================================

// De #error hieronder stopt compilatie als STURING_MAX7219_AANTAL niet actief is.
// In dat geval verschijnt er ook GEEN tekst op Serial.
// ============================================================================

#include <GroeiAcademie.h>
#include <Systeem/Sturing/Functies/MatrixFuncties/SpiraalPatroon.h>
#if STURING_MAX7219_AANTAL == 0
  #error Zet STURING_MAX7219_AANTAL in ConfigMAX7219.h of UserConfig.h op minstens 1 voor deze test.
#endif

MAX7219* sturing = nullptr;
uint8_t beeld[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int8_t huidigeStap = -1;
bool laatSpoorAchter = true;
uint8_t stapGrootte = 1;
bool wrapAround = false;
bool startGelukt = false;
bool testActief = false;
bool testGestopt = false;
bool autoActief = false;
bool autoNaarBinnen = true;
uint16_t stapVertragingms = 150;
uint32_t vorigeStapMs = 0;
uint8_t testStap = 0;

bool Reset() {
  for (uint8_t i = 0; i < 8; i++) beeld[i] = 0;
  huidigeStap = -1;
  return sturing->ToonPatroon(8, beeld);
}

bool VerwerkImpuls(bool isUp) {
  if (!BerekenSpiraalPatroon(8, huidigeStap, isUp, stapGrootte, wrapAround, laatSpoorAchter, beeld)) return false;
  if (!sturing->ToonPatroon(8, beeld)) return false;
  GA_SERIAL.print(F("SPIRAAL STAP: ")); GA_SERIAL.println(huidigeStap);
  return true;
}

void toonTest() {
  GA_SERIAL.println();
  switch (testStap) {
    case 0: GA_SERIAL.println(F("TEST 1: PIXEL AAN. Verwacht: alleen pixel (0,0) licht op.")); break;
    case 1: GA_SERIAL.println(F("TEST 2: PIXELS OP DE HOEKEN. Verwacht: vier hoekpixels branden; controleer orientatie.")); break;
    case 2: GA_SERIAL.println(F("TEST 3: PIXEL UIT. Verwacht: drie hoekpixels branden, (0,0) is uit.")); break;
    case 3: GA_SERIAL.println(F("TEST 4: UP. Verwacht: een pixel op de eerste positie van de spiraal.")); break;
    case 4: GA_SERIAL.println(F("TEST 5: DOWN. Verwacht: de vorige pixel wordt weer uitgezet; matrix leeg.")); break;
    case 5: GA_SERIAL.println(F("TEST 6: AUTO + SPOOR 1 + STAP 1. Verwacht: een spiraal van buiten naar binnen die blijft staan.")); break;
    case 6: GA_SERIAL.println(F("TEST 7: AUTOTERUG + SPOOR 1. Verwacht: de spiraal wordt van binnen naar buiten leeggemaakt.")); break;
    case 7: GA_SERIAL.println(F("TEST 8: AUTO + SPOOR 0. Verwacht: slechts een bewegende pixel, geen blijvend spoor.")); break;
    case 8: GA_SERIAL.println(F("TEST 9: STAP 4 + WRAP 0. Verwacht: spiraal springt telkens vier posities vooruit en stopt aan het einde.")); break;
    case 9: GA_SERIAL.println(F("TEST 10: WRAP 0 aan de grens. Verwacht: de volle matrix blijft vol, geen omslag naar het begin.")); break;
    case 10: GA_SERIAL.println(F("TEST 11: WRAP 1 aan de grens. Verwacht: de volle matrix wordt gewist en de eerste pixel gaat aan.")); break;
    case 11: GA_SERIAL.println(F("TEST 12: STAP 64. Verwacht: de volledige spiraal verschijnt in een stap.")); break;
    case 12: GA_SERIAL.println(F("TEST 13: SNELHEID 20 ms. Verwacht: snel bewegende pixel van buiten naar binnen.")); break;
    case 13: GA_SERIAL.println(F("TEST 14: SNELHEID 5000 ms. Verwacht: na circa vijf seconden de eerste pixel.")); break;
    case 14: GA_SERIAL.println(F("TEST 15: HELDER 0. Verwacht: een volle matrix op de laagste helderheid.")); break;
    case 15: GA_SERIAL.println(F("TEST 16: HELDER 15. Verwacht: een volle matrix op de hoogste helderheid.")); break;
    case 16: GA_SERIAL.println(F("TEST 17: RESET. Verwacht: alle leds uit.")); break;
    case 17: GA_SERIAL.println(F("TEST 18: STOP. Verwacht: de matrix wordt gestopt; herstart de Arduino voor een nieuwe reeks.")); break;
    default: GA_SERIAL.println(F("ALLE BESCHIKBARE SPIRAALFUNCTIES AANGEROEPEN.")); return;
  }
  GA_SERIAL.println(F("Controleer wat je moet zien. Typ 1 + Enter om deze test uit te voeren, of STOP + Enter."));
}

void rondTestAf(bool ok) {
  autoActief = false;
  testActief = false;
  GA_SERIAL.println(ok ? F("FUNCTIEAANROEP: OK. Controleer ZELF het fysieke beeld.") : F("FUNCTIEAANROEP: FOUT. De test stopt."));
  if (!ok) { testGestopt = true; if (sturing != nullptr) sturing->StopSturing(); return; }
  ++testStap;
  toonTest();
}

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sturing_MAX7219"));

  sturing = GedeeldeBusNewComponent<MAX7219>(&Native, HardwareResourcePin::NP_D10, MD_MAX72XX::FC16_HW);
  GA_SERIAL.println(F("Het FrameWork verzorgt aanmelden, controleren, inpluggen en activeren."));
  GA_SERIAL.println(F("De matrix blijft na iedere test zichtbaar tot je op 1 drukt."));
  GA_SERIAL.println(F("STOP stopt de test. Herstart de Arduino om opnieuw te beginnen."));
  GA_SERIAL.println(F("Voor de MAX7219: controleer het gekozen hardwaretype; FC16_HW is niet bevestigd."));
  if (startGelukt) {
    if (!Reset()) { GA_SERIAL.println(F("FOUT: matrix wissen mislukt.")); testGestopt = true; return; }
    toonTest();
  } else GA_SERIAL.println(F("Controleer de actieve configuratie, bedrading en hardware. Geen test mogelijk."));
}

void loop() {
  if (GA_SERIAL.available()) {
    String invoer = GA_SERIAL.readStringUntil('\n');
    invoer.trim();
    invoer.toUpperCase();
    if (invoer == "STOP") {
      autoActief = false;
      testActief = false;
      testGestopt = true;
      if (sturing != nullptr) sturing->StopSturing();
      GA_SERIAL.println(F("STOP: matrix gestopt. Herstart de Arduino voor een nieuwe reeks."));
      return;
    }
    if (testGestopt) { GA_SERIAL.println(F("Test gestopt: herstart Arduino.")); return; }
    if (!startGelukt) { GA_SERIAL.println(F("FOUT: automatische levenscyclus niet geslaagd.")); return; }
    if (invoer != "1") { GA_SERIAL.println(F("Typ 1 + Enter voor de aangekondigde test, of STOP + Enter.")); return; }
    if (testActief) { GA_SERIAL.println(F("Test loopt. Gebruik STOP om te onderbreken.")); return; }
    if (testStap >= 18) { toonTest(); return; }
    GA_SERIAL.print(F("UITVOEREN TEST ")); GA_SERIAL.println(testStap + 1);
    bool ok = Reset();
    laatSpoorAchter = true;
    stapGrootte = 1;
    wrapAround = false;
    stapVertragingms = 150;
    autoNaarBinnen = true;
    switch (testStap) {
    case 0:
      if (ok) ok = sturing->setPixel(0, 0, true);
      rondTestAf(ok);
      return;
    case 1:
      if (ok) ok = sturing->setPixel(0, 0, true) && sturing->setPixel(7, 0, true) && sturing->setPixel(0, 7, true) && sturing->setPixel(7, 7, true);
      rondTestAf(ok);
      return;
    case 2:
      if (ok) ok = sturing->setPixel(0, 0, true) && sturing->setPixel(7, 0, true) && sturing->setPixel(0, 7, true) && sturing->setPixel(7, 7, true) && sturing->setPixel(0, 0, false);
      rondTestAf(ok);
      return;
    case 3:
      if (ok) ok = VerwerkImpuls(true);
      rondTestAf(ok);
      return;
    case 4:
      if (ok) ok = VerwerkImpuls(true) && VerwerkImpuls(false);
      rondTestAf(ok);
      return;
    case 5:
      laatSpoorAchter = true;
      break;
    case 6:
      if (ok) {
        while (huidigeStap < 63 && ok) ok = BerekenSpiraalPatroon(8, huidigeStap, true, 1, false, true, beeld);
        if (ok) ok = sturing->ToonPatroon(8, beeld);
      }
      autoNaarBinnen = false;
      break;
    case 7:
      laatSpoorAchter = false;
      break;
    case 8:
      stapGrootte = 4;
      break;
    case 9:
    case 10:
      if (ok) {
        while (huidigeStap < 63 && ok) ok = BerekenSpiraalPatroon(8, huidigeStap, true, 1, false, true, beeld);
        if (ok) ok = sturing->ToonPatroon(8, beeld);
      }
      wrapAround = (testStap == 10);
      if (ok && testStap == 9) ok = !BerekenSpiraalPatroon(8, huidigeStap, true, 1, false, true, beeld) && huidigeStap == 63;
      if (ok && testStap == 10) ok = VerwerkImpuls(true) && huidigeStap == 0;
      rondTestAf(ok);
      return;
    case 11:
      stapGrootte = 64;
      if (ok) ok = VerwerkImpuls(true) && huidigeStap == 63;
      rondTestAf(ok);
      return;
    case 12:
      laatSpoorAchter = false;
      stapVertragingms = 20;
      break;
    case 13:
      stapVertragingms = 5000;
      break;
    case 14:
    case 15:
      if (ok) { stapGrootte = 64; ok = VerwerkImpuls(true); }
      if (ok) ok = sturing->setIntensity(testStap == 14 ? 0 : 15);
      rondTestAf(ok);
      return;
    case 16:
      rondTestAf(ok);
      return;
    case 17:
      if (sturing != nullptr) sturing->StopSturing();
      testGestopt = true;
      GA_SERIAL.println(F("STOP UITGEVOERD. Testreeks klaar. Herstart Arduino om opnieuw te beginnen."));
      return;
    default:
      GA_SERIAL.println(F("FOUT: onbekend testnummer."));
      return;
    }
    if (!ok) { rondTestAf(false); return; }
    autoActief = true;
    testActief = true;
    vorigeStapMs = millis();
    GA_SERIAL.println(F("SPIRAAL LOOPT. Bekijk de leds; de test stopt automatisch."));
  }
  if (!autoActief || !testActief || testGestopt) return;
  if (static_cast<uint32_t>(millis() - vorigeStapMs) < stapVertragingms) return;
  vorigeStapMs = millis();
  if (!VerwerkImpuls(autoNaarBinnen)) { rondTestAf(false); return; }
  if ((autoNaarBinnen && huidigeStap == 63) || (!autoNaarBinnen && huidigeStap == -1) || (testStap == 13 && huidigeStap == 0)) rondTestAf(true);
}
