// ============================================================================
// Extender: MAX14830_SPI
// ============================================================================
// Basistest voor ExtenderMAX14830SPI.
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
// CS-keuze in deze sketch: EXTENDER_MAX14830_SPI_1_CS_PIN.
// ============================================================================

#include <GroeiAcademie.h>

#if EXTENDER_MAX14830_BRONKLOK_HZ == 0
  #error Stel EXTENDER_MAX14830_BRONKLOK_HZ in op de werkelijke oscillatorfrequentie van jouw MAX14830-module voordat je deze test uitvoert.
#endif

#if EXTENDER_MAX14830_SPI_AANTAL == 0
  #error Zet EXTENDER_MAX14830_SPI_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif

#if EXTENDER_MAX14830_SPI_AANTAL == 1
ExtenderMAX14830SPI extender(
  &Native,
  GedeeldeBusComponent::UART_MAX14830,
  EXTENDER_MAX14830_SPI_1_CS_PIN, HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, EXTENDER_MAX14830_SPI_1_IRQ
);
#else
ExtenderMAX14830SPI extender(
  &Native,
  GedeeldeBusComponent::UART_MAX14830,
  HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, 0
);
#endif

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  #if defined(DEBUG)
  while (!GA_SERIAL) { ; }
  #else
  unsigned long startTijd = millis();
  while (!GA_SERIAL && (millis() - startTijd) < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  #endif
  GA_SERIAL.println(F("TEST: Extender_MAX14830_SPI"));

  GA_SERIAL.println(F("=== ExtenderMAX14830SPI aanmelden/controleren-test ==="));

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
