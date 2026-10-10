#include <GroeiAcademie.h>
#include <I2C_SCANNER.h>

I2C_SCANNER i2cScanner;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }

  InitialiserenGedeeldeBus(GedeeldeBusType::I2C);
  i2cScanner.begin();

  GA_SERIAL.println(F("I2C TOTAALDIAGNOSE"));
  GA_SERIAL.println(F("=================="));

  uint8_t aantalAangemeld = 0;
  uint8_t aantalAangemeldGevonden = 0;
  uint8_t aantalAangemeldOntbreekt = 0;
  uint8_t aantalOnbekend = 0;
  uint8_t aantalConflicten = 0;

  GA_SERIAL.println();
  GA_SERIAL.println(F("AANGEMELDE DEVICES"));
  GA_SERIAL.println(F("-------------------"));

  const GedeeldeBusNode* node = &Native;

  while (node != nullptr) {
    if (node->gedeeldeIdentiteiten == GedeeldeBusIdentiteitI2C) {
      const HardwareResourceTypeI2C* i2c = static_cast<const HardwareResourceTypeI2C*>(node);
      const uint8_t adres = i2c->adres;

      aantalAangemeld++;

      const uint16_t ping = i2cScanner.ping(adres);
      const int diagnose = i2cScanner.diag(adres);
      const int32_t tijd = i2cScanner.pingTime(adres);

      if (ping > 0) aantalAangemeldGevonden++;
      else aantalAangemeldOntbreekt++;

      if (node->conflictGevonden) aantalConflicten++;

      GA_SERIAL.print(F("0x"));
      if (adres < 0x10) GA_SERIAL.print('0');
      GA_SERIAL.print(adres, HEX);

      GA_SERIAL.print(F(" component=0x"));
      GA_SERIAL.print(static_cast<uint8_t>(node->component), HEX);

      GA_SERIAL.print(F(" aangemeld="));
      GA_SERIAL.print(node->aangemeld ? F("JA") : F("NEE"));

      GA_SERIAL.print(F(" gecontroleerd="));
      GA_SERIAL.print(node->gecontroleerd ? F("JA") : F("NEE"));

      GA_SERIAL.print(F(" ingeplugd="));
      GA_SERIAL.print(node->ingeplugd ? F("JA") : F("NEE"));

      GA_SERIAL.print(F(" actief="));
      GA_SERIAL.print(node->actief ? F("JA") : F("NEE"));

      GA_SERIAL.print(F(" conflict="));
      GA_SERIAL.print(node->conflictGevonden ? F("JA") : F("NEE"));

      GA_SERIAL.print(F(" ping="));
      GA_SERIAL.print(ping);

      GA_SERIAL.print(F(" diag="));
      GA_SERIAL.print(diagnose);

      GA_SERIAL.print(F(" tijd="));
      GA_SERIAL.print(tijd);
      GA_SERIAL.println(F(" us"));
    }

    node = GedeeldeBusNode::VolgendeInBoom(node, &Native);
  }

  GA_SERIAL.println();
  GA_SERIAL.println(F("FYSIEK GEVONDEN MAAR NIET AANGEMELD"));
  GA_SERIAL.println(F("------------------------------------"));

  for (uint8_t adres = 0x08; adres <= 0x77; adres++) {
    if (i2cScanner.ping(adres) == 0) continue;

    bool aangemeld = false;
    const GedeeldeBusNode* zoekNode = &Native;

    while (zoekNode != nullptr) {
      if (zoekNode->gedeeldeIdentiteiten == GedeeldeBusIdentiteitI2C) {
        const HardwareResourceTypeI2C* i2c = static_cast<const HardwareResourceTypeI2C*>(zoekNode);
        if (i2c->adres == adres) {
          aangemeld = true;
          break;
        }
      }

      zoekNode = GedeeldeBusNode::VolgendeInBoom(zoekNode, &Native);
    }

    if (!aangemeld) {
      aantalOnbekend++;

      GA_SERIAL.print(F("0x"));
      if (adres < 0x10) GA_SERIAL.print('0');
      GA_SERIAL.println(adres, HEX);
    }
  }

  GA_SERIAL.println();
  GA_SERIAL.println(F("SAMENVATTING"));
  GA_SERIAL.println(F("------------"));

  GA_SERIAL.print(F("Aangemeld: "));
  GA_SERIAL.println(aantalAangemeld);

  GA_SERIAL.print(F("Aangemeld + gevonden: "));
  GA_SERIAL.println(aantalAangemeldGevonden);

  GA_SERIAL.print(F("Aangemeld + ontbreekt: "));
  GA_SERIAL.println(aantalAangemeldOntbreekt);

  GA_SERIAL.print(F("Niet aangemeld + gevonden: "));
  GA_SERIAL.println(aantalOnbekend);

  GA_SERIAL.print(F("GedeeldeBus-conflicten: "));
  GA_SERIAL.println(aantalConflicten);

  GA_SERIAL.print(F("RESULTAAT: "));
  GA_SERIAL.println(
    (aantalAangemeldOntbreekt == 0 && aantalOnbekend == 0 && aantalConflicten == 0)
      ? F("OK")
      : F("PROBLEMEN GEVONDEN")
  );
}

void loop() {
}
