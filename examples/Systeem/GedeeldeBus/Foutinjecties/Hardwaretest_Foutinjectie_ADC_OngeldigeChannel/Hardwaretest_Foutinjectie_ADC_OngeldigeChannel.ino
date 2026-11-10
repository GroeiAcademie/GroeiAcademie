// ============================================================================
// Foutinjectie: ADC_OngeldigeChannel
// ============================================================================
// Negatieve ADC-contracttest. Vereist ADS7828 op de normale I2C-bus.
// Geen hardwareactie: channel 8 is buiten het geldige bereik 0..7.
// Verwacht: StartExtenderReadAnalog(8, 1500) == -2, geen runtime-foutladder.
// ============================================================================

#include <GroeiAcademie.h>
#if EXTENDER_ADS7828_AANTAL != 1
#error Stel EXTENDER_ADS7828_AANTAL op 1 voor deze negatieve contracttest.
#endif
ExtenderADS7828 extender(
  &Native, GedeeldeBusComponent::ADC_ADS7828,
  I2C_ADDRESS_EXTENDER_ADS7828_1, HardwareResourcePin::NP_SDA,
  HardwareResourcePin::NP_SCL);
void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  #if defined(DEBUG)
  while (!GA_SERIAL) { ; }
  #else
  unsigned long startTijd = millis();
  while (!GA_SERIAL && (millis() - startTijd) < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  #endif
  GA_SERIAL.println(F("TEST: Hardwaretest_Foutinjectie_ADC_OngeldigeChannel"));
  const bool foutcodeJuist = extender.StartExtenderReadAnalog(8, 1500) == -2;
  GA_SERIAL.println(foutcodeJuist ? F("RESULTAAT: OK") : F("RESULTAAT: FOUT"));
}
void loop() {}
