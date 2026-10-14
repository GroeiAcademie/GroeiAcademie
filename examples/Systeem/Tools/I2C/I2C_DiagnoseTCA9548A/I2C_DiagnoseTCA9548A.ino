// ============================================================================
// Tool: I2C_DiagnoseTCA9548A
// ============================================================================
// Diagnoseert de TCA9548A op de GedeeldeBus en scant elk van de acht I2C-kanalen.
// Toont per kanaal de selectie, kanaalmasker, kanaalstatus en gevonden I2C-adressen.
// Configuratie gebeurt via UserConfig.h.
// ============================================================================

#include <GroeiAcademie.h>
#include <I2C_SCANNER.h>

#if EXTENDER_TCA9548A_AANTAL == 0
  #error Zet EXTENDER_TCA9548A_AANTAL in UserConfig.h op minstens 1 voor deze test.
#endif

#if EXTENDER_TCA9548A_AANTAL == 1
ExtenderTCA9548A extender(
  &Native,
  GedeeldeBusComponent::I2C_MULTIPLEXER_TCA9548A,
  I2C_ADDRESS_EXTENDER_TCA9548A_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, EXTENDER_TCA9548A_1_RESET
);
#else
ExtenderTCA9548A extender(
  &Native,
  GedeeldeBusComponent::I2C_MULTIPLEXER_TCA9548A,
  HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, 0
);
#endif

I2C_SCANNER i2cScanner;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }

  GA_SERIAL.println(F("I2C DIAGNOSE TCA9548A"));
  GA_SERIAL.println(F("---------------------"));

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

  GA_SERIAL.print(F("Adres TCA9548A: 0x"));
  if (extender.adres < 0x10) GA_SERIAL.print('0');
  GA_SERIAL.println(extender.adres, HEX);

  GA_SERIAL.print(F("Ping: "));
  GA_SERIAL.println(i2cScanner.ping(extender.adres));

  GA_SERIAL.print(F("Diag: "));
  GA_SERIAL.println(i2cScanner.diag(extender.adres));

  GA_SERIAL.print(F("PingTime: "));
  GA_SERIAL.print(i2cScanner.pingTime(extender.adres));
  GA_SERIAL.println(F(" us"));

  // Eerst scannen met alle kanalen dicht. Wat dan antwoordt, hangt op Native (niet achter deze TCA9548A)
  // en wordt per kanaal overgeslagen. 1 bit per adres 0x00 tot 0x7F.
  extender.disableAllChannels();
  uint8_t adresOpNativeI2C[16] = {};
  for (uint8_t adres = 0x08; adres <= 0x77; adres++) {
    if (i2cScanner.ping(adres) > 0) adresOpNativeI2C[adres / 8] |= (1 << (adres % 8));
  }

  for (uint8_t teller = 0; teller < 8; teller++) {
    GA_SERIAL.println();
    GA_SERIAL.print(F("Kanaal: "));
    GA_SERIAL.println(teller);

    gelukt = extender.selectChannel(teller);
    GA_SERIAL.print(F("selectChannel: "));
    GA_SERIAL.println(gelukt ? F("OK") : F("FOUT"));

    if (!gelukt) continue;

    GA_SERIAL.print(F("getChannelMask: 0x"));
    GA_SERIAL.println(extender.getChannelMask(), HEX);

    GA_SERIAL.print(F("isEnabled: "));
    GA_SERIAL.println(extender.isEnabled(teller) ? F("JA") : F("NEE"));

    GA_SERIAL.print(F("getError: "));
    GA_SERIAL.println(extender.getError());

    uint8_t aantal = 0;

    for (uint8_t adres = 0x08; adres <= 0x77; adres++) {
      if (adres == extender.adres) continue;
      if (adresOpNativeI2C[adres / 8] & (1 << (adres % 8))) continue;
      if (i2cScanner.ping(adres) == 0) continue;

      GA_SERIAL.print(F("0x"));
      if (adres < 0x10) GA_SERIAL.print('0');
      GA_SERIAL.println(adres, HEX);
      aantal++;
    }

    GA_SERIAL.print(F("Aantal gevonden op kanaal: "));
    GA_SERIAL.println(aantal);
  }

  gelukt = extender.disableAllChannels();
  GA_SERIAL.println();
  GA_SERIAL.print(F("disableAllChannels: "));
  GA_SERIAL.println(gelukt ? F("OK") : F("FOUT"));

  GA_SERIAL.println(gelukt ? F("RESULTAAT: OK") : F("RESULTAAT: FOUT"));
}

void loop() {
}
