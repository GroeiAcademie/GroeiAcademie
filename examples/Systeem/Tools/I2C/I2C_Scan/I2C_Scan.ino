#include <GroeiAcademie.h>
#include <I2C_SCANNER.h>

I2C_SCANNER i2cScanner;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }

  InitialiserenGedeeldeBus(GedeeldeBusType::I2C);
  i2cScanner.begin();

  uint8_t aantal = 0;

  GA_SERIAL.println(F("I2C SCAN"));
  GA_SERIAL.println(F("---------"));

  for (uint8_t adres = 0x08; adres <= 0x77; adres++) {
    if (i2cScanner.ping(adres) > 0) {
      GA_SERIAL.print(F("0x"));
      if (adres < 0x10) GA_SERIAL.print('0');
      GA_SERIAL.println(adres, HEX);
      aantal++;
    }
  }

  GA_SERIAL.print(F("Aantal gevonden: "));
  GA_SERIAL.println(aantal);
}

void loop() {
}
