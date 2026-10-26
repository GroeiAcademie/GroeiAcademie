// ============================================================================
// Tool: I2C_OpsporenNietAangemeldeDevices
// ============================================================================
// Scant de fysieke I2C-bus en vergelijkt de gevonden adressen aangemeld op de GedeeldeBus.
// Toont fysieke I2C-devices die niet in GedeeldeBus zijn aangemeld.
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
      if (
      node->gedeeldeIdentiteiten != nullptr &&
      node->aangemeldePinnen.aantalGedeeld == 2 &&
      node->gedeeldeIdentiteiten[0] == HardwareResourceIdentiteit::I2C_SDA &&
      node->gedeeldeIdentiteiten[1] == HardwareResourceIdentiteit::I2C_SCL
    ) {
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
