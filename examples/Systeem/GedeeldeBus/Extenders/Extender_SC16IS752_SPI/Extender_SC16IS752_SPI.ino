// ============================================================================
// Extender: SC16IS752 SPI
// ============================================================================
// Basistest voor ExtenderSC16IS752_SPI.
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
// CS-keuze in deze sketch: EXTENDER_SC16IS752_SPI_1_CS_PIN.
// ============================================================================

#include <GroeiAcademie.h>

#if EXTENDER_SC16IS752_SPI_AANTAL == 0
  #error Zet EXTENDER_SC16IS752_SPI_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif

#if EXTENDER_SC16IS752_SPI_AANTAL == 1
ExtenderSC16IS752SPI extender(
  &Native,
  GedeeldeBusComponent::UART_SC16IS752,
  EXTENDER_SC16IS752_SPI_1_CS_PIN, HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, EXTENDER_SC16IS752_SPI_1_IRQ, EXTENDER_SC16IS752_SPI_1_RESET
);
#else
ExtenderSC16IS752SPI extender(
  &Native,
  GedeeldeBusComponent::UART_SC16IS752,
  HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, 0
);
#endif

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Extender_SC16IS752_SPI"));

  GA_SERIAL.println(F("=== ExtenderSC16IS752SPI aanmelden/controleren-test ==="));

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
