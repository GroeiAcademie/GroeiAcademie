// ============================================================================
// Extender — MAX14830_I2C
// ============================================================================
// Basistest voor ExtenderMAX14830I2C.
// Deze test valideert alleen de GedeeldeBus-stappen:
// aanmelden() -> controleren().
// inpluggen() en activeren() worden getest via een echte client/sensor.
// Configuratie gebeurt via UserConfig.h.
// ============================================================================

#include <Systeem/GedeeldeBus/GedeeldeBus.h>
#include <Configuratie/SystemConfig.h>

#if EXTENDER_MAX14830_I2C_AANTAL == 0
  #error Zet EXTENDER_MAX14830_I2C_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif

#if EXTENDER_MAX14830_I2C_AANTAL == 1
ExtenderMAX14830I2C extender(
  &Native,
  GedeeldeBusComponent::UART_MAX14830,
  I2C_ADDRESS_EXTENDER_MAX14830_I2C_1, HardwareResourcePin::SDA, HardwareResourcePin::SCL, EXTENDER_MAX14830_I2C_1_IRQ
);
#else
ExtenderMAX14830I2C extender(
  &Native,
  GedeeldeBusComponent::UART_MAX14830,
  HardwareResourcePin::SDA, HardwareResourcePin::SCL, 0
);
#endif

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }

  GA_SERIAL.println(F("=== ExtenderMAX14830I2C aanmelden/controleren-test ==="));

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
