#include <GroeiAcademie.h>
#include <I2C_SCANNER.h>

I2C_SCANNER i2cScanner;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }

  InitialiserenGedeeldeBus(GedeeldeBusType::I2C);
  i2cScanner.begin();

  GA_SERIAL.println(F("I2C DIAGNOSE AANGEMELDE DEVICES"));
  GA_SERIAL.println(F("--------------------------------"));

  const GedeeldeBusNode* node = &Native;
  uint8_t aantal = 0;

  while (node != nullptr) {
    if (node->gedeeldeIdentiteiten == GedeeldeBusIdentiteitI2C) {
      const HardwareResourceTypeI2C* i2c = static_cast<const HardwareResourceTypeI2C*>(node);
      const uint8_t adres = i2c->adres;

      GA_SERIAL.print(F("Adres: 0x"));
      if (adres < 0x10) GA_SERIAL.print('0');
      GA_SERIAL.println(adres, HEX);

      GA_SERIAL.print(F("Component: 0x"));
      GA_SERIAL.println(static_cast<uint8_t>(node->component), HEX);

      GA_SERIAL.print(F("Aangemeld: "));
      GA_SERIAL.println(node->aangemeld ? F("JA") : F("NEE"));

      GA_SERIAL.print(F("Gecontroleerd: "));
      GA_SERIAL.println(node->gecontroleerd ? F("JA") : F("NEE"));

      GA_SERIAL.print(F("Ingeplugd: "));
      GA_SERIAL.println(node->ingeplugd ? F("JA") : F("NEE"));

      GA_SERIAL.print(F("Actief: "));
      GA_SERIAL.println(node->actief ? F("JA") : F("NEE"));

      GA_SERIAL.print(F("Conflict: "));
      GA_SERIAL.println(node->conflictGevonden ? F("JA") : F("NEE"));

      const uint16_t ping = i2cScanner.ping(adres);
      const int diagnose = i2cScanner.diag(adres);
      const int32_t tijd = i2cScanner.pingTime(adres);

      GA_SERIAL.print(F("Ping: "));
      GA_SERIAL.println(ping);

      GA_SERIAL.print(F("Diag: "));
      GA_SERIAL.println(diagnose);

      GA_SERIAL.print(F("PingTime: "));
      GA_SERIAL.print(tijd);
      GA_SERIAL.println(F(" us"));

      GA_SERIAL.println();
      aantal++;
    }

    node = GedeeldeBusNode::VolgendeInBoom(node, &Native);
  }

  GA_SERIAL.print(F("Aantal aangemelde I2C-devices: "));
  GA_SERIAL.println(aantal);
}

void loop() {
}
