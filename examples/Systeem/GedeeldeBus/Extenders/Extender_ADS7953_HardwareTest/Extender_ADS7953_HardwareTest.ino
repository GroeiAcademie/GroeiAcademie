// ============================================================================
// Extender: ADS7953, begeleide hardwaretest (SPI)
// ============================================================================
// Doel: echte activering (CH0/CH1-kanaaltags) en analoge metingen controleren.
// Configuratie: EXTENDER_ADS7953_AANTAL=1 en EXTENDER_ADS7953_1_CS_PIN
// in de bestaande actieve configuratie. DATA_LAYER_CONFIG=DATA_TYPE_NONE.
// SPI: DIN/SDI -> MOSI; DOUT/SDO -> MISO; SCLK -> SCK; CS volgens ConfigADS7953.h.
// Verbind de correcte +VA, +VBD, REF en GND volgens het werkelijke modulebord.
// Controleer elektrische niveaus en voeding vóór inschakelen.
// Testopstelling: CH0 naar analoge GND, CH1 naar een bekende veilige spanning
// binnen het ingestelde bereik (bijvoorbeeld een gemeten deel van VREF).
// Vergelijk CH0/CH1 met de echte spanning en ADC_BITS; beoordeel zelf OK/FOUT.
// Negatieve proef: voeding UIT, MISO losmaken, inschakelen en START herhalen.
// Verwacht: activeren FOUT. Ook CS-verkeerd kan afzonderlijk worden beproefd.
// Nooit bedrading veranderen terwijl de voeding aan staat.
// ============================================================================

#include <GroeiAcademie.h>

#if EXTENDER_ADS7953_AANTAL == 0
  #error Zet EXTENDER_ADS7953_AANTAL in de actieve configuratie op minstens 1.
#endif

#if DATA_LAYER_CONFIG != DATA_TYPE_NONE
  #error Zet DATA_LAYER_CONFIG op DATA_TYPE_NONE voor deze hardwaretest.
#endif

ExtenderADS7953* extender = nullptr;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Extender_ADS7953_HardwareTest"));
  GA_SERIAL.println(F("=== ADS7953 hardwaretest ==="));
  GA_SERIAL.println(F("Voeding UIT voor aansluiten of wijzigen van de bedrading."));
  GA_SERIAL.println(F("CH0 = GND; CH1 = bekende veilige analoge spanning binnen VREF."));
  GA_SERIAL.println(F("START = aanmelden t/m activeren; CH0 / CH1 = meten; STOP = afmelden."));
  GA_SERIAL.println(F("OK / FOUT = eigen fysieke beoordeling noteren."));
}

void loop() {
  if (!GA_SERIAL.available()) return;
  String commando = GA_SERIAL.readStringUntil('\n');
  commando.trim();
  commando.toUpperCase();

  if (commando == "START") {
    if (extender != nullptr) {
      GA_SERIAL.println(F("ADS7953 is al actief; gebruik STOP voor een nieuwe proef."));
      return;
    }
#if EXTENDER_ADS7953_AANTAL == 1
    extender = GedeeldeBusNewComponent<ExtenderADS7953>(
      &Native, GedeeldeBusComponent::ADC_ADS7953,
      EXTENDER_ADS7953_1_CS_PIN, HardwareResourcePin::NP_SCK,
      HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI);
#else
    extender = GedeeldeBusNewComponent<ExtenderADS7953>(
      &Native, GedeeldeBusComponent::ADC_ADS7953,
      HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO,
      HardwareResourcePin::NP_MOSI, 0);
#endif
    GA_SERIAL.println(extender != nullptr ? F("Activeren: OK (kanaaltags CH0/CH1 gecontroleerd)") : F("Activeren: FOUT"));
    GA_SERIAL.println(F("Bij afwezige chip of onderbroken MISO/CS is FOUT het verwachte resultaat."));
    return;
  }

  if (commando == "STOP") {
    if (extender != nullptr) {
      extender->afmelden();
      extender = nullptr;
    }
    GA_SERIAL.println(F("ADS7953 afgemeld. Voeding UIT voor wijzigen van de bedrading."));
    return;
  }

  if (commando == "CH0" || commando == "CH1") {
    if (extender == nullptr || !extender->actief) {
      GA_SERIAL.println(F("FOUT: voer eerst START uit."));
      return;
    }
    const uint8_t pin = commando == "CH0" ? 0 : 1;
    const int8_t gestart = extender->StartExtenderReadAnalog(pin, EXTENDER_ADS7953_REFERENTIE_MILLIVOLT);
    if (gestart != 1) {
      GA_SERIAL.print(F("FOUT: ADC-startresultaat = "));
      GA_SERIAL.println(static_cast<int>(gestart));
      return;
    }
    int32_t waarde = 0;
    if (!extender->IsExtenderReadAnalogReady() || !extender->GetExtenderReadAnalogValue(waarde)) {
      GA_SERIAL.println(F("FOUT: SPI-overdracht, kanaaltag of ADC-lezing mislukt."));
      return;
    }
    GA_SERIAL.print(F("Kanaal "));
    GA_SERIAL.print(pin);
    GA_SERIAL.print(F("; ruwe waarde op ADC_BITS-schaal: "));
    GA_SERIAL.println(waarde);
    GA_SERIAL.println(F("Vergelijk met de werkelijk aangeboden spanning; geen automatische fysieke OK."));
    return;
  }

  if (commando == "OK" || commando == "FOUT") {
    GA_SERIAL.print(F("MANUELE BEOORDELING: "));
    GA_SERIAL.println(commando);
    return;
  }

  GA_SERIAL.println(F("Commando's: START, CH0, CH1, STOP, OK, FOUT."));
}
