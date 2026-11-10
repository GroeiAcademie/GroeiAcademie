// ============================================================================
// Sturing: WS2812_HardwareTest
// ============================================================================
// Begeleide fysieke WS2812-test: pixels, hoeken, rijen, kolommen en diagonaal zichtbaar controleren.
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
// Na START: PX x y r g b, H, R, K, D en X via Serial invoeren.
// ============================================================================

#include <GroeiAcademie.h>
#if STURING_WS2812_AANTAL == 0
  #error Zet STURING_WS2812_AANTAL op minstens 1 voor deze test.
#endif

WS2812* ws2812 = nullptr;

bool testActief = false;
bool startGelukt = false;
bool testGestopt = false;
String invoer;

void toonFunctieResultaat(bool ok) {
  GA_SERIAL.println(ok ? F("OPDRACHT: OK") : F("OPDRACHT: FOUT"));
}

void toonStartInstructies() {
  GA_SERIAL.println(F("HARDWARETEST: Sturing_WS2812_HardwareTest: typ START na controle van bedrading en voeding."));
  GA_SERIAL.println(F("Na START: PX x y r g b voor een pixel; H=hoeken, R=rijen, K=kolommen, D=diagonaal, X=wissen."));
  GA_SERIAL.println(F("Controleer het LED-beeld zelf; er is geen automatische optische terugmelding."));
  GA_SERIAL.println(F("STOP stopt testuitvoer; herstart voor nieuwe test."));
}
void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  #if defined(DEBUG)
  while (!GA_SERIAL) { ; }
  #else
  unsigned long startTijd = millis();
  while (!GA_SERIAL && (millis() - startTijd) < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  #endif
  GA_SERIAL.println(F("TEST: Sturing_WS2812_HardwareTest"));
  ws2812 = GedeeldeBusNewComponent<WS2812>(&Native, HardwareResourcePin::NP_D6);
  startGelukt = ws2812 != nullptr;
  GA_SERIAL.println(startGelukt ? F("LEVENSCYCLUS: OK") : F("LEVENSCYCLUS: FOUT"));
  toonStartInstructies();
}
void loop() {
  if (!GA_SERIAL.available()) return;
  invoer = GA_SERIAL.readStringUntil('\n');
  invoer.trim();
  invoer.toUpperCase();

  if (invoer == "STOP") {
    if (startGelukt) ws2812->StopSturing();
    testActief = false;
    testGestopt = true;
    GA_SERIAL.println(F("STOP: uitvoer uit / opdracht gestopt. Herstart om opnieuw te testen."));
    return;
  }

  if (testGestopt) {
    GA_SERIAL.println(F("Test gestopt: reset Arduino."));
    return;
  }

  if (!startGelukt) {
    GA_SERIAL.println(F("FOUT: levenscyclus."));
    return;
  }

  if (invoer == "START") {
    if (!testActief) {
      testActief = true;
      GA_SERIAL.println(F("START: bevestigd."));
    }

    return;
  }

  if (!testActief) {
    GA_SERIAL.println(F("Eerst START invoeren."));
    return;
  }

  if (invoer == "X") {
    ws2812->StopSturing();
    GA_SERIAL.println(F("Matrix gewist."));
    return;
  }

  // PX x y r g b; H = hoeken; R = rijen; K = kolommen; D = diagonaal.
  if (invoer.startsWith("PX ")) {
    int x, y, r, g, b;

    if (sscanf(invoer.c_str(), "PX %d %d %d %d %d", &x, &y, &r, &g, &b) != 5 || x < 0 || x > 7 || y < 0 || y > 7 || r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255) {
      GA_SERIAL.println(F("Gebruik PX x y r g b (x/y 0..7, RGB 0..255)."));
      return;
    }

    toonFunctieResultaat(ws2812->setPixelColor((uint8_t)x, (uint8_t)y, ((uint32_t)r << 16) | ((uint32_t)g << 8) | b));
    return;
  }

  if (invoer != "H" && invoer != "R" && invoer != "K" && invoer != "D") {
    GA_SERIAL.println(F("Gebruik PX x y r g b, H, R, K, D, X."));
    return;
  }

  ws2812->StopSturing();
  bool ok = true;

  for (uint8_t y = 0; y < 8; y++)
    for (uint8_t x = 0; x < 8; x++) {
      uint32_t kleur = 0;

      if (invoer == "H") {
        if (x == 0 && y == 0) kleur = 0xFF0000;
        if (x == 7 && y == 0) kleur = 0x00FF00;
        if (x == 0 && y == 7) kleur = 0x0000FF;
        if (x == 7 && y == 7) kleur = 0xFFFF00;
      }

      if (invoer == "R") kleur = (y % 3 == 0) ? 0x400000 : ((y % 3 == 1) ? 0x004000 : 0x000040);
      if (invoer == "K") kleur = (x % 3 == 0) ? 0x400000 : ((x % 3 == 1) ? 0x004000 : 0x000040);
      if (invoer == "D" && x == y) kleur = 0x004000;
      if (kleur && !ws2812->setPixelColor(x, y, kleur)) ok = false;
    }

  toonFunctieResultaat(ok);
  GA_SERIAL.println(F("Controleer het patroon visueel."));
}
