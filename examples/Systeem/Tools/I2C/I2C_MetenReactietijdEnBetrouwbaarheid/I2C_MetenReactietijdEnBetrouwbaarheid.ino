// ============================================================================
// Tool: I2C_MetenReactietijdEnBetrouwbaarheid
// ============================================================================
// Meet de reactietijd en betrouwbaarheid van op de GedeeldeBus aangemelde I2C-devices.
// Voert herhaalde pings uit en rapporteert geslaagde en mislukte metingen.
// Testconfiguratie: I2C-shield met CharacterScreen, PCF8574 en ADS1115.
// De shieldcomponenten worden voor de diagnose aangemeld via Screen, RFP602 en Input,
// op dezelfde manier als in Tik_Enkele_Samen_Instortend_Cocktail.
// ============================================================================

#include <GroeiAcademie.h>
#include <I2C_SCANNER.h>

I2C_SCANNER i2cScanner;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }

  Screen = GedeeldeBusNewComponent<struct Screen>();
  if (Screen == nullptr) exit(0);

  sensorRFP602 = GedeeldeBusNewComponent<struct RFP602>();
  if (sensorRFP602 == nullptr) exit(0);

  Input = GedeeldeBusNewComponent<struct Input>();
  if (Input == nullptr) exit(0);

  InitialiserenGedeeldeBus(GedeeldeBusType::I2C);
  i2cScanner.begin();

  GA_SERIAL.println(F("I2C REACTIETIJD EN BETROUWBAARHEID"));
  GA_SERIAL.println(F("----------------------------------"));

  const GedeeldeBusNode* node = &Native;

  while (node != nullptr) {
    if (
      node->gedeeldeIdentiteiten != nullptr &&
      node->aangemeldePinnen.aantalGedeeld == 2 &&
      node->gedeeldeIdentiteiten[0] == HardwareResourceIdentiteit::I2C_SDA &&
      node->gedeeldeIdentiteiten[1] == HardwareResourceIdentiteit::I2C_SCL
    ) {
      const HardwareResourceTypeI2C* i2c = static_cast<const HardwareResourceTypeI2C*>(node);
      const uint8_t adres = i2c->adres;

      const uint16_t pogingen = 100;
      const uint16_t gelukt = i2cScanner.ping(adres, pogingen);

      int32_t minimum = 2147483647L;
      int32_t maximum = 0;
      int64_t totaal = 0;
      uint16_t tijdGelukt = 0;
      uint16_t tijdMislukt = 0;

      for (uint8_t teller = 0; teller < 20; teller++) {
        const int32_t tijd = i2cScanner.pingTime(adres);

        if (tijd >= 0) {
          if (tijd < minimum) minimum = tijd;
          if (tijd > maximum) maximum = tijd;
          totaal += tijd;
          tijdGelukt++;
        } else {
          tijdMislukt++;
        }
      }

      GA_SERIAL.print(F("Adres: 0x"));
      if (adres < 0x10) GA_SERIAL.print('0');
      GA_SERIAL.println(adres, HEX);

      GA_SERIAL.print(F("Pogingen: "));
      GA_SERIAL.println(pogingen);

      GA_SERIAL.print(F("Geslaagd: "));
      GA_SERIAL.println(gelukt);

      GA_SERIAL.print(F("Mislukt: "));
      GA_SERIAL.println(pogingen - gelukt);

      if (tijdGelukt > 0) {
        GA_SERIAL.print(F("Minimum: "));
        GA_SERIAL.print(minimum);
        GA_SERIAL.println(F(" us"));

        GA_SERIAL.print(F("Maximum: "));
        GA_SERIAL.print(maximum);
        GA_SERIAL.println(F(" us"));

        GA_SERIAL.print(F("Gemiddeld: "));
        GA_SERIAL.print(static_cast<int32_t>(totaal / tijdGelukt));
        GA_SERIAL.println(F(" us"));
      }

      GA_SERIAL.print(F("PingTime mislukt: "));
      GA_SERIAL.println(tijdMislukt);

      GA_SERIAL.println();
    }

    node = GedeeldeBusNode::VolgendeInBoom(node, &Native);
  }
}

void loop() {
}
