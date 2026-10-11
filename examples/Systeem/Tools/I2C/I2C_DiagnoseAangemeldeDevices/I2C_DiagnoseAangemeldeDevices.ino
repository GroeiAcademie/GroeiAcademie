// ============================================================================
// Tool: I2C_DiagnoseAangemeldeDevices
// ============================================================================
// Diagnoseert alle I2C-devices die op de GedeeldeBus zijn aangemeld.
// Toont GedeeldeBus-status, ping, diagnosecode en reactietijd per device.
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

  GA_SERIAL.println(F("I2C DIAGNOSE AANGEMELDE DEVICES"));
  GA_SERIAL.println(F("--------------------------------"));

  const GedeeldeBusNode* node = &Native;
  uint8_t aantal = 0;

  while (node != nullptr) {
    if (
      node->gedeeldeIdentiteiten != nullptr &&
      node->aangemeldePinnen.aantalGedeeld == 2 &&
      node->gedeeldeIdentiteiten[0] == HardwareResourceIdentiteit::I2C_SDA &&
      node->gedeeldeIdentiteiten[1] == HardwareResourceIdentiteit::I2C_SCL
    ) {
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
