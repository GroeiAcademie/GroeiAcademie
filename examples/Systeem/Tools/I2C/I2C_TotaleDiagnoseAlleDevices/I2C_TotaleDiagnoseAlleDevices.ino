// ============================================================================
// Tool: I2C_TotaleDiagnoseAlleDevices
// ============================================================================
// Combineert GedeeldeBus-status met een fysieke scan van de I2C-bus.
// Toont aangemelde, ontbrekende, niet aangemelde en conflicterende devices.
// Testconfiguratie: I2C-shield met CharacterScreen, PCF8574 en ADS1115.
// De shieldcomponenten worden voor de diagnose aangemeld via Screen, RFP602 en Input,
// op dezelfde manier als in Tik_Enkele_Samen_Instortend_Cocktail.
// ============================================================================

#include <GroeiAcademie.h>

#if EXTENDER_ADS1115_AANTAL == 0
  #error Zet EXTENDER_ADS1115_AANTAL in UserConfig.h op minstens 1: RFP602 meet via een ADS1115-Extender.
#endif

ExtenderADS1115 extenderADS1115(
  &Native, GedeeldeBusComponent::ADC_ADS1115,
  I2C_ADDRESS_EXTENDER_ADS1115_1,
  HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL,
  EXTENDER_ADS1115_1_ALERT_RDY,
  HardwareResourceToegang::GEDEELD
);

#include <I2C_SCANNER.h>

I2C_SCANNER i2cScanner;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }

  Screen = GedeeldeBusNewComponent<struct Screen>();
  if (Screen == nullptr) { GA_SERIAL.println(F("FOUT: Screen")); while (true) { ; } }

  sensorRFP602set1 = GedeeldeBusNewComponent<struct RFP602>(&extenderADS1115, GedeeldeBusComponent::RFP602);
  if (sensorRFP602set1 == nullptr) { GA_SERIAL.println(F("FOUT: RFP602")); while (true) { ; } }

  Input = GedeeldeBusNewComponent<struct Input>();
  if (Input == nullptr) { GA_SERIAL.println(F("FOUT: Input")); while (true) { ; } }

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
    if (
      node->gedeeldeIdentiteiten != nullptr &&
      node->aangemeldePinnen.aantalGedeeld == 2 &&
      node->gedeeldeIdentiteiten[0] == HardwareResourceIdentiteit::I2C_SDA &&
      node->gedeeldeIdentiteiten[1] == HardwareResourceIdentiteit::I2C_SCL
    ) {
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
      if (
        zoekNode->gedeeldeIdentiteiten != nullptr &&
        zoekNode->aangemeldePinnen.aantalGedeeld == 2 &&
        zoekNode->gedeeldeIdentiteiten[0] == HardwareResourceIdentiteit::I2C_SDA &&
        zoekNode->gedeeldeIdentiteiten[1] == HardwareResourceIdentiteit::I2C_SCL
      ) {
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
