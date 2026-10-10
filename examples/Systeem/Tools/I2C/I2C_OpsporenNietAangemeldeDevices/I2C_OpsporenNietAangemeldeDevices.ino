#include <GroeiAcademie.h>
#include <I2C_SCANNER.h>

I2C_SCANNER i2cScanner;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }

  InitialiserenGedeeldeBus(GedeeldeBusType::I2C);
  i2cScanner.begin();

  GA_SERIAL.println(F("I2C ONBEKENDE ADRESSEN"));
  GA_SERIAL.println(F("----------------------"));

  uint8_t aantalGevonden = 0;
  uint8_t aantalOnbekend = 0;

  for (uint8_t adres = 0x08; adres <= 0x77; adres++) {
    if (i2cScanner.ping(adres) == 0) continue;

    aantalGevonden++;
    bool aangemeld = false;

    const GedeeldeBusNode* node = &Native;
    while (node != nullptr) {
      if (node->gedeeldeIdentiteiten == GedeeldeBusIdentiteitI2C) {
        const HardwareResourceTypeI2C* i2c = static_cast<const HardwareResourceTypeI2C*>(node);
        if (i2c->adres == adres) {
          aangemeld = true;
          break;
        }
      }

      node = GedeeldeBusNode::VolgendeInBoom(node, &Native);
    }

    GA_SERIAL.print(F("0x"));
    if (adres < 0x10) GA_SERIAL.print('0');
    GA_SERIAL.print(adres, HEX);
    GA_SERIAL.print(F("  "));
    GA_SERIAL.println(aangemeld ? F("AANGEMELD") : F("NIET AANGEMELD"));

    if (!aangemeld) aantalOnbekend++;
  }

  GA_SERIAL.print(F("Fysiek gevonden: "));
  GA_SERIAL.println(aantalGevonden);

  GA_SERIAL.print(F("Niet aangemeld: "));
  GA_SERIAL.println(aantalOnbekend);
}

void loop() {
}
