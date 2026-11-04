// ============================================================================
// ExtenderADS1115: Test ExtenderPins Geweigerd
// ============================================================================
// Test één ExtenderPin direct buiten de momenteel aangeboden set.
// aanmelden() mag slagen; controleren() MOET weigeren met GB111.
// Er wordt geen hardware ingeplugd of geactiveerd.
// Configuratie #define EXTENDER_ADS1115_AANTAL 1 of 2 gebeurt via UserConfig.h.
// Aansluiting gebruikte I2C-parent/Extender: SDA -> board-SDA, SCL -> board-SCL; GND -> board-GND.
// UNO R4 Minima/WiFi: fysieke SDA- en SCL-header (bij R4 Minima ook A4=SDA, A5=SCL); UNO Q: board-SDA/SCL.
// ESP32 D1 UNO R32: SDA=GPIO21, SCL=GPIO22 (bestaand FrameWork-boardprofiel).
// ESP32-S3 ARDI-32: SDA=GPIO38, SCL=GPIO39; DEV: SDA=GPIO8, SCL=GPIO9 (FrameWork-profiel).
// RP2040 Cytron Maker UNO: SDA=GPIO20, SCL=GPIO21 (ook Maker/Grove 6).
// STM32 F401RE Nucleo: SDA=PB9, SCL=PB8 (bestaand FrameWork-boardprofiel).
// Voeding VCC en GND volgens het concrete modulebord; controleer I2C-logicaniveau, pull-ups en eventuele levelshifting.
// Bij een analoge sensor is dit de bus van de ADC-Extender, NIET een I2C-aansluiting op de sensor zelf.
// Sluit de analoge sensoruitgang op het in de code/configuratie gebruikte ADC-kanaal aan.
// ============================================================================

#include <GroeiAcademie.h>

#if EXTENDER_ADS1115_AANTAL == 0
  #error Zet EXTENDER_ADS1115_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif

#if EXTENDER_ADS1115_AANTAL == 1
ExtenderADS1115 extender(
  &Native,
  GedeeldeBusComponent::ADC_ADS1115,
  I2C_ADDRESS_EXTENDER_ADS1115_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, EXTENDER_ADS1115_1_ALERT_RDY
);
#else
ExtenderADS1115 extender(
  &Native,
  GedeeldeBusComponent::ADC_ADS1115,
  HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, 0
);
#endif

uint8_t ExtenderPins[] = { 0x04 };

GedeeldeBusNode SensorTest(
  &extender,
  { nullptr, 0, ExtenderPins, 1 },
  GedeeldeBusComponent::GC_SENSOR,
  HardwareResourceToegang::GEDEELD
);

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Extender_ADS1115_Test_ExtenderPins_Geweigerd"));

  GA_SERIAL.println(F("=== ExtenderADS1115 ExtenderPins Geweigerd ==="));

  bool gelukt = extender.aanmelden();
  if (gelukt) gelukt = extender.controleren();
  GA_SERIAL.print(F("Extender aanmelden/controleren: "));
  GA_SERIAL.println(gelukt ? F("OK") : F("FOUT"));

  if (!gelukt) {
    GedeeldeBusPrintEnVerwijderTijdelijkeConflicten(&extender);
    GA_SERIAL.println(F("FOUT: extender zelf faalt vóór ExtenderPins-test"));
    return;
  }

  const bool aangemeld = SensorTest.aanmelden();
  GA_SERIAL.print(F("Sensor aanmelden: "));
  GA_SERIAL.println(aangemeld ? F("OK") : F("FOUT"));

  bool geweigerd = false;
  if (aangemeld) {
    const bool gecontroleerd = SensorTest.controleren();
    geweigerd = !gecontroleerd;
    GA_SERIAL.print(F("Ongeldige ExtenderPin geweigerd: "));
    GA_SERIAL.println(geweigerd ? F("OK") : F("FOUT"));
  }

  GedeeldeBusPrintEnVerwijderTijdelijkeConflicten(&SensorTest);
  GA_SERIAL.println((aangemeld && geweigerd) ? F("RESULTAAT: OK") : F("RESULTAAT: FOUT"));
}

void loop() {
}
