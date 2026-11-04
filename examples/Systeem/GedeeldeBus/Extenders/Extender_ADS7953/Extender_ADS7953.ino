// ============================================================================
// Extender: ADS7953
// ============================================================================
// Basistest voor ExtenderADS7953.
// Deze test valideert alleen de GedeeldeBus-stappen: aanmelden() -> controleren().
// inpluggen() en activeren() worden getest via een echte client/sensor.
// Configuratie gebeurt via UserConfig.h.
// SPI-parent/Extender: DIN/SDI -> MOSI, DOUT/SDO -> MISO, CLK/SCK -> SCK; CS volgens de bestaande constructor/configuratie.
// UNO R4 Minima/WiFi: MOSI=D11 (COPI), MISO=D12 (CIPO), SCK=D13; CS via de geconfigureerde pin.
// ESP32 D1 UNO R32: MOSI=GPIO23, MISO=GPIO19, SCK=GPIO18 (bestaand FrameWork-profiel).
// ESP32-S3 ARDI-32/DEV: MOSI=GPIO11, MISO=GPIO13, SCK=GPIO12 (FrameWork-profiel).
// RP2040 Cytron Maker UNO: MOSI=GPIO11, MISO=GPIO12, SCK=GPIO10.
// STM32 F401RE Nucleo: MOSI=PA7, MISO=PA6, SCK=PA5 (FrameWork-profiel).
// Module-VCC en GND volgens het gebruikte bord; SPI-niveaus en eventuele levelshifting controleren.
// CS-keuze in deze sketch: EXTENDER_ADS7953_1_CS_PIN.
// ============================================================================

#include <GroeiAcademie.h>

#if EXTENDER_ADS7953_AANTAL == 0
  #error Zet EXTENDER_ADS7953_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif

#if EXTENDER_ADS7953_AANTAL == 1
ExtenderADS7953 extender(
  &Native,
  GedeeldeBusComponent::ADC_ADS7953,
  EXTENDER_ADS7953_1_CS_PIN, HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI
);
#else
ExtenderADS7953 extender(
  &Native,
  GedeeldeBusComponent::ADC_ADS7953,
  HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, 0
);
#endif

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Extender_ADS7953"));

  GA_SERIAL.println(F("=== ExtenderADS7953 aanmelden/controleren-test ==="));

  bool gelukt = extender.aanmelden();
  GA_SERIAL.print(F("aanmelden: "));
  GA_SERIAL.println(gelukt ? F("OK") : F("FOUT"));

  if (gelukt) {
    gelukt = extender.controleren();
    GA_SERIAL.print(F("controleren: "));
    GA_SERIAL.println(gelukt ? F("OK") : F("FOUT"));
  }

  GedeeldeBusPrintEnVerwijderTijdelijkeConflicten(&extender);
  GA_SERIAL.println(gelukt ? F("RESULTAAT: OK") : F("RESULTAAT: FOUT"));
}

void loop() {
}
