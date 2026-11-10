// ============================================================================
// Extender: TCA9548A_Test_GedeeldChannelAdresconflict
// ============================================================================
// Test zonder hardwaretransacties: twee gelijke I2C-adressen op verschillende
// TCA9548A-channels toegelaten, hetzelfde adres op hetzelfde channel geweigerd.
// Vereist bestaande configuratie: EXTENDER_TCA9548A_AANTAL >= 1.
// De resultaten zijn pas geldig nadat deze sketch echt is gecompileerd en uitgevoerd.
// Aansluiting gebruikte I2C-parent/Extender: SDA -> board-SDA, SCL -> board-SCL; GND -> board-GND.
// UNO R4 Minima/WiFi: fysieke SDA- en SCL-header (bij R4 Minima ook A4=SDA, A5=SCL); UNO Q: board-SDA/SCL.
// ESP32 D1 UNO R32: SDA=GPIO21, SCL=GPIO22 (bestaand FrameWork-boardprofiel).
// ESP32-S3 ARDI-32: SDA=GPIO38, SCL=GPIO39; DEV: SDA=GPIO8, SCL=GPIO9 (FrameWork-profiel).
// RP2040 Cytron Maker UNO: SDA=GPIO20, SCL=GPIO21 (ook Maker/Grove 6).
// STM32 F401RE Nucleo: SDA=PB9, SCL=PB8 (bestaand FrameWork-boardprofiel).
// Voeding VCC en GND volgens het concrete modulebord; controleer I2C-logicaniveau, pull-ups en eventuele levelshifting.
// ============================================================================

#include <GroeiAcademie.h>

#if EXTENDER_TCA9548A_AANTAL == 0
  #error Stel EXTENDER_TCA9548A_AANTAL in op minstens 1 voor deze test.
#endif

#if EXTENDER_TCA9548A_AANTAL == 1
ExtenderTCA9548A multiplexer(
  &Native, GedeeldeBusComponent::I2C_MULTIPLEXER_TCA9548A,
  I2C_ADDRESS_EXTENDER_TCA9548A_1, HardwareResourcePin::NP_SDA,
  HardwareResourcePin::NP_SCL, EXTENDER_TCA9548A_1_RESET
);
#else
ExtenderTCA9548A multiplexer(
  &Native, GedeeldeBusComponent::I2C_MULTIPLEXER_TCA9548A,
  HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, 0
);
#endif

HardwareResourceTypeI2C sensorChannel0(
  &multiplexer, GedeeldeBusComponent::GC_SENSOR, 0x48,
  HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL,
  HardwareResourceToegang::GEDEELD, 0
);
HardwareResourceTypeI2C sensorChannel1(
  &multiplexer, GedeeldeBusComponent::GC_SENSOR, 0x48,
  HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL,
  HardwareResourceToegang::GEDEELD, 1
);
HardwareResourceTypeI2C sensorDubbelChannel0(
  &multiplexer, GedeeldeBusComponent::GC_SENSOR, 0x48,
  HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL,
  HardwareResourceToegang::GEDEELD, 0
);

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  #if defined(DEBUG)
  while (!GA_SERIAL) { ; }
  #else
  unsigned long startTijd = millis();
  while (!GA_SERIAL && (millis() - startTijd) < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  #endif
  GA_SERIAL.println(F("TEST: Extender_TCA9548A_Test_GedeeldChannelAdresconflict"));
  bool basis = multiplexer.aanmelden();
  if (basis) basis = multiplexer.controleren();
  bool eerste = basis && sensorChannel0.aanmelden();
  if (eerste) eerste = sensorChannel0.controleren();
  bool tweede = eerste && sensorChannel1.aanmelden();
  if (tweede) tweede = sensorChannel1.controleren();
  bool derde = tweede && sensorDubbelChannel0.aanmelden();
  if (derde) derde = sensorDubbelChannel0.controleren();

  GA_SERIAL.print(F("TCA channels 0 en 1, zelfde adres: "));
  GA_SERIAL.println(tweede ? F("OK") : F("FOUT"));
  GA_SERIAL.print(F("TCA channel 0 dubbel adres, verwacht GB102: "));
  GA_SERIAL.println(!derde && tweede ? F("GEWEIGERD (code controleren)") : F("ONVERWACHT"));
  GedeeldeBusPrintEnVerwijderTijdelijkeConflicten(&sensorDubbelChannel0);
  GA_SERIAL.println((tweede && !derde) ? F("RESULTAAT: OK") : F("RESULTAAT: FOUT"));
}
void loop() {}
