// ============================================================================
// Sturing_WS2812
// ============================================================================
// WS2812: 8x8 Sturing met native datapin; dezelfde gedeelde spiraalberekening.
// Test voor spiraalbeweging en impulsbediening.
// Bedrading: DIN -> D6, geschikte eigen voeding, GND gemeenschappelijk.
// Signaalniveau en voeding controleren voor de concrete WS2812-module.
// Pinnen zijn afgeleid uit de bestaande constructoren en de boardmapping in src/Configuratie/UserConfig.h en SystemConfig.h.
// WS2812 DIN -> D6; voeding volgens het LED-moduleverbruik, gemeenschappelijke GND en correct dataniveau.
// Native pinlabel in de code: D6; boardmapping:
// UNO R4 Minima/WiFi en UNO Q: D6.
// ESP32 D1 UNO R32: D6=GPIO27.
// ESP32-S3 ARDI-32: D6=GPIO9; ESP32-S3 DEV: D6=GPIO3.
// RP2040 Cytron Maker UNO: D6=GPIO6.
// STM32 F401RE Nucleo: D6=PB10.
// ============================================================================

// ============================================================================
// CONCREET AANSLUITEN EN TESTEN
// ============================================================================
// Native Arduino D6 -> DI/DIN van de EERSTE WS2812-pixel, NIET DOUT.
// Matrix +5V -> voor de volledige matrix geschikte externe voeding (+).
// Matrix GND -> voeding (-) en Arduino GND (gemeenschappelijke referentie).
// Een datalijnserieweerstand van 300..500 ohm en buffercondensator
//   bij de matrixvoeding worden door Adafruit aanbevolen; module kan die al hebben.
// Controleer bij 3,3V-datapin of een geschikt niveau naar de 5V-matrix nodig is.
// Fysiek: pixels, hoeken, rijen, kolommen, diagonaal zichtbaar volgen.
// De Arduino kan niet meten of de LED werkelijk licht geeft.
// Actieve configuratie: src/Configuratie/Sturingen/ConfigWS2812.h,
//   STURING_WS2812_AANTAL >= 1.
// Gebruik de bestaande interactieve sketch voor spiraalcommando's.
// ============================================================================

#include <GroeiAcademie.h>
#include <Systeem/Sturing/Functies/MatrixFuncties/SpiraalPatroon.h>
#if STURING_WS2812_AANTAL == 0
  #error Zet STURING_WS2812_AANTAL in de actieve configuratie op minstens 1 voor deze test.
#endif

WS2812* ws2812 = nullptr;
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
  return ws2812->ToonPatroon(8, beeld, 0x0000FF);
}

bool VerwerkImpuls(bool isUp) {
  if (!BerekenSpiraalPatroon(8, huidigeStap, isUp, stapGrootte, wrapAround, laatSpoorAchter, beeld)) return false;
  if (!ws2812->ToonPatroon(8, beeld, 0x0000FF)) return false;
  GA_SERIAL.print(F("SPIRAAL STAP: ")); GA_SERIAL.println(huidigeStap);
  return true;
}

void toonTestInstructies() {
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
    case 14: GA_SERIAL.println(F("TEST 15: RESET. Verwacht: alle leds uit.")); break;
    case 15: GA_SERIAL.println(F("TEST 16: STOP. Verwacht: de matrix wordt gestopt; herstart de Arduino voor een nieuwe reeks.")); break;
    default: GA_SERIAL.println(F("ALLE BESCHIKBARE SPIRAALFUNCTIES AANGEROEPEN.")); return;
  }
  GA_SERIAL.println(F("Controleer wat je moet zien. Typ 1 + Enter om deze test uit te voeren, of STOP + Enter."));
}

void rondTestAf(bool ok) {
  autoActief = false;
  testActief = false;
  GA_SERIAL.println(ok ? F("FUNCTIEAANROEP: OK. Controleer ZELF het fysieke beeld.") : F("FUNCTIEAANROEP: FOUT. De test stopt."));

  if (!ok) {
    testGestopt = true;
    if (ws2812 != nullptr) ws2812->StopSturing();
    return;
  }
  ++testStap;
  toonTestInstructies();
}

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sturing_WS2812"));
  ws2812 = GedeeldeBusNewComponent<WS2812>(&Native, HardwareResourcePin::NP_D6);
  startGelukt = ws2812 != nullptr;
  GA_SERIAL.println(startGelukt ? F("AUTOMATISCHE LEVENSCYCLUS: OK") : F("AUTOMATISCHE LEVENSCYCLUS: FOUT"));
  GA_SERIAL.println(F("Het FrameWork verzorgt aanmelden, controleren, inpluggen en activeren."));
  GA_SERIAL.println(F("De matrix blijft na iedere test zichtbaar tot je op 1 drukt."));
  GA_SERIAL.println(F("STOP stopt de test. Herstart de Arduino om opnieuw te beginnen."));
  if (startGelukt) {

    if (!Reset()) {
      GA_SERIAL.println(F("FOUT: matrix wissen mislukt."));
      testGestopt = true;
      return;
    }
    toonTestInstructies();
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
      if (ws2812 != nullptr) ws2812->StopSturing();
      GA_SERIAL.println(F("STOP: matrix gestopt. Herstart de Arduino voor een nieuwe reeks."));
      return;
    }

    if (testGestopt) {
      GA_SERIAL.println(F("Test gestopt: herstart Arduino."));
      return;
    }

    if (!startGelukt) {
      GA_SERIAL.println(F("FOUT: automatische levenscyclus niet geslaagd."));
      return;
    }

    if (invoer != "1") {
      GA_SERIAL.println(F("Typ 1 + Enter voor de aangekondigde test, of STOP + Enter."));
      return;
    }

    if (testActief) {
      GA_SERIAL.println(F("Test loopt. Gebruik STOP om te onderbreken."));
      return;
    }

    if (testStap >= 16) {
      toonTestInstructies();
      return;
    }
    GA_SERIAL.print(F("UITVOEREN TEST ")); GA_SERIAL.println(testStap + 1);
    bool ok = Reset();
    laatSpoorAchter = true;
    stapGrootte = 1;
    wrapAround = false;
    stapVertragingms = 150;
    autoNaarBinnen = true;
    switch (testStap) {
    case 0:
      if (ok) ok = ws2812->setPixelColor(0, 0, 0x0000FF);
      rondTestAf(ok);
      return;
    case 1:
      if (ok) ok = ws2812->setPixelColor(0, 0, 0x0000FF) && ws2812->setPixelColor(7, 0, 0x0000FF) && ws2812->setPixelColor(0, 7, 0x0000FF) && ws2812->setPixelColor(7, 7, 0x0000FF);
      rondTestAf(ok);
      return;
    case 2:
      if (ok) ok = ws2812->setPixelColor(0, 0, 0x0000FF) && ws2812->setPixelColor(7, 0, 0x0000FF) && ws2812->setPixelColor(0, 7, 0x0000FF) && ws2812->setPixelColor(7, 7, 0x0000FF) && ws2812->setPixelColor(0, 0, 0);
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
        if (ok) ok = ws2812->ToonPatroon(8, beeld, 0x0000FF);
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
        if (ok) ok = ws2812->ToonPatroon(8, beeld, 0x0000FF);
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
      rondTestAf(ok);
      return;
    case 15:
      if (ws2812 != nullptr) ws2812->StopSturing();
      testGestopt = true;
      GA_SERIAL.println(F("STOP UITGEVOERD. Testreeks klaar. Herstart Arduino om opnieuw te beginnen."));
      return;
    default:
      GA_SERIAL.println(F("FOUT: onbekend testnummer."));
      return;
    }

    if (!ok) {
      rondTestAf(false);
      return;
    }
    autoActief = true;
    testActief = true;
    vorigeStapMs = millis();
    GA_SERIAL.println(F("SPIRAAL LOOPT. Bekijk de leds; de test stopt automatisch."));
  }
  if (!autoActief || !testActief || testGestopt) return;
  if (static_cast<uint32_t>(millis() - vorigeStapMs) < stapVertragingms) return;
  vorigeStapMs = millis();

  if (!VerwerkImpuls(autoNaarBinnen)) {
    rondTestAf(false);
    return;
  }
  if ((autoNaarBinnen && huidigeStap == 63) || (!autoNaarBinnen && huidigeStap == -1) || (testStap == 13 && huidigeStap == 0)) rondTestAf(true);
}
