#include <GroeiAcademie.h>
#include <I2C_SCANNER.h>

I2C_SCANNER i2cScanner;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }

  InitialiserenGedeeldeBus(GedeeldeBusType::I2C);
  i2cScanner.begin();

  GA_SERIAL.println(F("I2C BUSSNELHEDEN"));
  GA_SERIAL.println(F("----------------"));

  const uint32_t snelheden[] = {
    100000UL,
    200000UL,
    400000UL
  };

  for (uint8_t snelheidIndex = 0; snelheidIndex < 3; snelheidIndex++) {
    const uint32_t snelheid = snelheden[snelheidIndex];

    i2cScanner.setClock(snelheid);

    GA_SERIAL.print(F("Snelheid: "));
    GA_SERIAL.print(snelheid);
    GA_SERIAL.println(F(" Hz"));

    const GedeeldeBusNode* node = &Native;

    while (node != nullptr) {
      if (node->gedeeldeIdentiteiten == GedeeldeBusIdentiteitI2C) {
        const HardwareResourceTypeI2C* i2c = static_cast<const HardwareResourceTypeI2C*>(node);
        const uint8_t adres = i2c->adres;

        const uint16_t ping = i2cScanner.ping(adres);
        const int diagnose = i2cScanner.diag(adres);
        const int32_t tijd = i2cScanner.pingTime(adres);

        GA_SERIAL.print(F("0x"));
        if (adres < 0x10) GA_SERIAL.print('0');
        GA_SERIAL.print(adres, HEX);

        GA_SERIAL.print(F("  ping="));
        GA_SERIAL.print(ping);

        GA_SERIAL.print(F("  diag="));
        GA_SERIAL.print(diagnose);

        GA_SERIAL.print(F("  tijd="));
        GA_SERIAL.print(tijd);
        GA_SERIAL.println(F(" us"));
      }

      node = GedeeldeBusNode::VolgendeInBoom(node, &Native);
    }

    GA_SERIAL.println();
  }

  i2cScanner.setClock(100000UL);
}

void loop() {
}
