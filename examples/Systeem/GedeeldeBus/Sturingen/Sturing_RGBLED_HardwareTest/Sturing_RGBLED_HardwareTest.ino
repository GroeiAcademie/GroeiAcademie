// ============================================================================
// Sturing: RGBLED_HardwareTest
// ============================================================================
// Begeleide fysieke RGBLED-test via PCA9685: de commando's R/G/B wijzigen de LED-uitgangen.
// Pinnen zijn afgeleid uit de bestaande constructoren en de boardmapping in src/Configuratie/UserConfig.h en SystemConfig.h.
// PCA9685 kanaal 0/1/2 -> RGBLED R/G/B; stroombegrenzing en gemeenschappelijke anode/kathode volgens module.
// I2C-aansluiting: SDA/SCL en GND op dezelfde bus; modulevoeding volgens het werkelijke Extender-bord.
// UNO R4 Minima/WiFi en UNO Q: SDA/SCL op de gelijknamige boardaansluitingen.
// ESP32 D1 UNO R32: SDA=GPIO21, SCL=GPIO22.
// ESP32-S3 ARDI-32: SDA=GPIO38, SCL=GPIO39; ESP32-S3 DEV: SDA=GPIO8, SCL=GPIO9.
// RP2040 Cytron Maker UNO: SDA=GPIO20, SCL=GPIO21.
// STM32 F401RE Nucleo: SDA=PB9, SCL=PB8.
// ============================================================================

// ============================================================================
// CONCREET AANSLUITEN EN TESTEN
// ============================================================================
// Arduino SDA -> SDA van de I2C-Extender; Arduino SCL -> SCL; GND -> GND.
// Voed de Extender-logica op het voor board en I2C-niveau geschikte VCC/VDD.
// Verifieer fysiek I2C-adres en optionele INT/RESET-pinnen met de actieve Config*.h.
// Bij 3,3V-logica op een 5V-bus: controleer een passende I2C-niveauomzetter.
// PCA9685 VCC is de logica-/I2C-voeding; V+ is een APARTE servo-/lastvoeding.
// PCA9685 OE moet een gedefinieerd niveau hebben; controleer de concrete breakout.
// Actieve Extenderconfig: src/Configuratie/Extenders/ConfigPCA9685.h
//   EXTENDER_PCA9685_AANTAL >= 1, I2C_ADDRESS_EXTENDER_PCA9685_1 = fysiek adres.
// PCA9685 kanaal 0 -> R, kanaal 1 -> G, kanaal 2 -> B.
// Per afzonderlijke LED-kleur een passende SERIEWEERSTAND plaatsen.
// De bestaande RGBLED-constructor gebruikt commonAnode=false (standaard):
//   gemeenschappelijke kathode -> GND; geen gemeenschappelijke anode aansluiten
//   zonder de polariteit en de bestaande constructor te herzien.
// Controleer met R/G/B-commando of de overeenkomstige LED-kleur oplicht.
// Actieve configuratie: src/Configuratie/Sturingen/ConfigRGBLED.h,
//   STURING_RGBLED_AANTAL >= 1.
// Na START R/G/B met waarden 0..4095 instellen; controleer kleur en UIT.
// ============================================================================

#include <GroeiAcademie.h>
#if STURING_RGBLED_AANTAL == 0
  #error Zet STURING_RGBLED_AANTAL op minstens 1 voor deze test.
#endif
#if EXTENDER_PCA9685_AANTAL == 0
  #error Schakel de vereiste Extender in voor deze Sturing-test.
#endif

ExtenderPCA9685* extenderPCA9685 = nullptr;
RGBLED* rgbled = nullptr;

bool testActief = false;
bool startGelukt = false;
bool testGestopt = false;
String invoer;

void toonFunctieResultaat(bool ok) {
  GA_SERIAL.println(ok ? F("OPDRACHT: OK") : F("OPDRACHT: FOUT"));
}

void toonStartInstructies() {
  GA_SERIAL.println(F("HARDWARETEST: Sturing_RGBLED_HardwareTest: typ START na controle van bedrading en voeding."));
  GA_SERIAL.println(F("Na START: R0..4095, G0..4095 en B0..4095 stellen de kanalen in; 0 schakelt alles UIT."));
  GA_SERIAL.println(F("Controleer de LED-kleur en het lichtniveau zelf; er is geen fysieke sensor-terugmelding."));
  GA_SERIAL.println(F("STOP stopt testuitvoer; herstart voor nieuwe test."));
}
uint16_t rgb[3] = {0, 0, 0};

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  #if defined(DEBUG)
  while (!GA_SERIAL) { ; }
  #else
  unsigned long startTijd = millis();
  while (!GA_SERIAL && (millis() - startTijd) < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  #endif
  GA_SERIAL.println(F("TEST: Sturing_RGBLED_HardwareTest"));
  extenderPCA9685 = GedeeldeBusNewComponent<ExtenderPCA9685>(&Native, GedeeldeBusComponent::PWM_PCA9685, I2C_ADDRESS_EXTENDER_PCA9685_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL);
  if (extenderPCA9685 != nullptr) rgbled = GedeeldeBusNewComponent<RGBLED>(extenderPCA9685, 0, 1, 2);
  startGelukt = rgbled != nullptr;
  GA_SERIAL.println(startGelukt ? F("LEVENSCYCLUS: OK") : F("LEVENSCYCLUS: FOUT"));
  toonStartInstructies();
}
void loop() {
  if (!GA_SERIAL.available()) return;
  invoer = GA_SERIAL.readStringUntil('\n');
  invoer.trim();
  invoer.toUpperCase();

  if (invoer == "STOP") {
    if (startGelukt) rgbled->StopSturing();
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

  if (invoer == "0") {
    rgb[0] = rgb[1] = rgb[2] = 0;
  } else {
    char kleur = invoer.charAt(0);

    if (kleur != 'R' && kleur != 'G' && kleur != 'B') {
      GA_SERIAL.println(F("Gebruik R0..4095, G0..4095, B0..4095 of 0."));
      return;
    }

    int waarde = invoer.substring(1).toInt();

    if (invoer.length() < 2 || waarde < 0 || waarde > 4095) {
      GA_SERIAL.println(F("FOUT: 0..4095"));
      return;
    }

    rgb[kleur == 'R' ? 0 : (kleur == 'G' ? 1 : 2)] = (uint16_t)waarde;
  }

  toonFunctieResultaat(rgbled->setRGBcolor(rgb[0], rgb[1], rgb[2]));
  GA_SERIAL.print(F("RGB: "));
  GA_SERIAL.print(rgb[0]);
  GA_SERIAL.print(',');
  GA_SERIAL.print(rgb[1]);
  GA_SERIAL.print(',');
  GA_SERIAL.println(rgb[2]);
}
