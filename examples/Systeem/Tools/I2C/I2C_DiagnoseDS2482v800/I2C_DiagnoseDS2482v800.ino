// ============================================================================
// Tool: I2C_DiagnoseDS2482v800
// ============================================================================
// Diagnoseert de DS2482v800 op de GedeeldeBus en test elk van de acht 1-Wire-kanalen.
// Toont I2C-diagnose van de bridge en zoekt per kanaal naar 1-Wire-devices.
// Configuratie gebeurt via UserConfig.h.
// ============================================================================

#include <GroeiAcademie.h>
#include <I2C_SCANNER.h>

#if EXTENDER_DS2482_800_AANTAL == 0
  #error Zet EXTENDER_DS2482_800_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif

#if EXTENDER_DS2482_800_AANTAL == 1
ExtenderDS2482v800 extender(
  &Native,
  GedeeldeBusComponent::ONE_WIRE_DS2482v800,
  I2C_ADDRESS_EXTENDER_DS2482_800_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL
);
#else
ExtenderDS2482v800 extender(
  &Native,
  GedeeldeBusComponent::ONE_WIRE_DS2482v800,
  HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, 0
);
#endif

I2C_SCANNER i2cScanner;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }

  GA_SERIAL.println(F("I2C DIAGNOSE DS2482v800"));
  GA_SERIAL.println(F("-----------------------"));

  bool gelukt = extender.aanmelden();
  GA_SERIAL.print(F("aanmelden: "));
  GA_SERIAL.println(gelukt ? F("OK") : F("FOUT"));

  if (gelukt) {
    gelukt = extender.controleren();
    GA_SERIAL.print(F("controleren: "));
    GA_SERIAL.println(gelukt ? F("OK") : F("FOUT"));
  }

  if (gelukt) {
    gelukt = extender.inpluggen();
    GA_SERIAL.print(F("inpluggen: "));
    GA_SERIAL.println(gelukt ? F("OK") : F("FOUT"));
  }

  if (gelukt) {
    gelukt = extender.activeren();
    GA_SERIAL.print(F("activeren: "));
    GA_SERIAL.println(gelukt ? F("OK") : F("FOUT"));
  }

  GedeeldeBusPrintEnVerwijderTijdelijkeConflicten(&extender);

  if (!gelukt) {
    GA_SERIAL.println(F("RESULTAAT: FOUT"));
    return;
  }

  i2cScanner.begin();

  GA_SERIAL.print(F("Adres DS2482v800: 0x"));
  if (extender.adres < 0x10) GA_SERIAL.print('0');
  GA_SERIAL.println(extender.adres, HEX);

  GA_SERIAL.print(F("Ping: "));
  GA_SERIAL.println(i2cScanner.ping(extender.adres));

  GA_SERIAL.print(F("Diag: "));
  GA_SERIAL.println(i2cScanner.diag(extender.adres));

  GA_SERIAL.print(F("PingTime: "));
  GA_SERIAL.print(i2cScanner.pingTime(extender.adres));
  GA_SERIAL.println(F(" us"));

  for (uint8_t teller = 0; teller < 8; teller++) {
    GA_SERIAL.println();
    GA_SERIAL.print(F("Kanaal: "));
    GA_SERIAL.println(teller);

    gelukt = extender.selectChannel(teller);
    GA_SERIAL.print(F("selectChannel: "));
    GA_SERIAL.println(gelukt ? F("OK") : F("FOUT"));

    if (!gelukt) continue;

    gelukt = extender.OneWireReset();
    GA_SERIAL.print(F("OneWireReset: "));
    GA_SERIAL.println(gelukt ? F("OK") : F("FOUT"));

    GA_SERIAL.print(F("presencePulseDetected: "));
    GA_SERIAL.println(extender.presencePulseDetected() ? F("JA") : F("NEE"));

    GA_SERIAL.print(F("shortDetected: "));
    GA_SERIAL.println(extender.shortDetected() ? F("JA") : F("NEE"));

    GA_SERIAL.print(F("logicLevel: "));
    GA_SERIAL.println(extender.logicLevel() ? F("HOOG") : F("LAAG"));

    GA_SERIAL.print(F("is1WBusy: "));
    GA_SERIAL.println(extender.is1WBusy() ? F("JA") : F("NEE"));

    extender.OneWireSearchReset();

    uint8_t adres[8];
    uint8_t aantal = 0;

    while (extender.OneWireSearch(adres)) {
      GA_SERIAL.print(F("1-Wire adres: "));

      for (uint8_t teller = 0; teller < 8; teller++) {
        if (adres[teller] < 0x10) GA_SERIAL.print('0');
        GA_SERIAL.print(adres[teller], HEX);
      }

      GA_SERIAL.println();
      aantal++;
    }

    GA_SERIAL.print(F("Aantal 1-Wire-devices: "));
    GA_SERIAL.println(aantal);
  }

  GA_SERIAL.println();
  GA_SERIAL.println(F("RESULTAAT: OK"));
}

void loop() {
}
