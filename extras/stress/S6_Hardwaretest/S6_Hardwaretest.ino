// ============================================================================
// Tool: I2C_MetenHerstelEnBlokkeertijd
// ============================================================================
// Meet hoe lang een toegang via het framework blokkeert, ook tijdens een storing en tijdens het runtime-herstel (D042).
// Hoort bij het testprotocol stabiliteit S4, S1 en S6 (kolom "Blokkeertijd").
//
// Werking:
// - elke I2C_HERSTELTEST_TUSSENTIJD_TOEGANGEN_MS één toegang per aanwezige bron:
//   RFP602 via de gekozen ADS1115-Extender en een keypadscan van Input PCF8574 (alleen met INPUT_TYPE_PCF8574);
// - rond elke toegang meet micros() de duur;
// - elke I2C_HERSTELTEST_RAPPORT_ELKE_MS op Serial per bron: langste duur, gemiddelde duur en aantal toegangen;
// - tegelijk een teller op het scherm via Screen->Print(): zo volgt na een hotplug altijd een "volgende Print()" (S6).
//
// Runtime-meldingen (GB400 tot GB410) staan tussen de rapportregels op Serial.
// Testconfiguratie: I2C-shield met CharacterScreen, PCF8574 en ADS1115, zoals in Tik_Enkele_Samen_Instortend_Cocktail.
// Voor de meldingen van het tweetrapsherstel (S4): bouwen met DEBUG (TRACE geeft te veel uitvoer om ze te zien).
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


#ifndef I2C_HERSTELTEST_TUSSENTIJD_TOEGANGEN_MS
  #define I2C_HERSTELTEST_TUSSENTIJD_TOEGANGEN_MS 200UL
#endif
#ifndef I2C_HERSTELTEST_RAPPORT_ELKE_MS
  #define I2C_HERSTELTEST_RAPPORT_ELKE_MS 1000UL
#endif

// Per bron: de langste duur, de som van alle duren en het aantal toegangen sinds het vorige rapport.
struct BlokkeertijdPerBron {
  unsigned long langsteDuurMicros = 0;
  unsigned long totaleDuurMicros = 0;
  uint16_t aantalToegangen = 0;
};

BlokkeertijdPerBron blokkeertijdMetingADS1115;
#if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_PCF8574)
BlokkeertijdPerBron blokkeertijdKeypadscanPCF8574;
#endif

unsigned long tijdstipVorigeToegang = 0;
unsigned long tijdstipVorigRapport = 0;
unsigned long aantalRapporten = 0;

void DuurVanToegangBijhouden(BlokkeertijdPerBron& bron, unsigned long startMicros) {
  const unsigned long duurMicros = micros() - startMicros;
  if (duurMicros > bron.langsteDuurMicros) bron.langsteDuurMicros = duurMicros;
  bron.totaleDuurMicros += duurMicros;
  bron.aantalToegangen++;
}

void RapportRegelTonen(const __FlashStringHelper* naamBron, BlokkeertijdPerBron& bron) {
  GA_SERIAL.print(naamBron);
  GA_SERIAL.print(F(": langste "));
  GA_SERIAL.print(bron.langsteDuurMicros / 1000.0, 2);
  GA_SERIAL.print(F(" ms, gemiddeld "));
  GA_SERIAL.print(bron.aantalToegangen > 0 ? (bron.totaleDuurMicros / bron.aantalToegangen) / 1000.0 : 0.0, 2);
  GA_SERIAL.print(F(" ms, toegangen "));
  GA_SERIAL.println(bron.aantalToegangen);
  bron = BlokkeertijdPerBron();
}

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && (millis() - startTijd) < SERIAL_CONNECT_TIMEOUT_MS) { ; }

  Screen = GedeeldeBusNewComponent<struct Screen>();
  if (Screen == nullptr) { GA_SERIAL.println(F("FOUT: Screen")); while (true) { ; } }

  sensorRFP602set1 = GedeeldeBusNewComponent<struct RFP602>(&extenderADS1115, GedeeldeBusComponent::RFP602);
  if (sensorRFP602set1 == nullptr) GA_SERIAL.println(F("FOUT: RFP602 (ADS1115 wordt niet gemeten)"));

#if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_PCF8574)
  Input = GedeeldeBusNewComponent<struct Input>();
  if (Input == nullptr) GA_SERIAL.println(F("FOUT: Input (keypadscan wordt niet gemeten)"));
#endif

  GA_SERIAL.println(F("I2C HERSTEL EN BLOKKEERTIJD"));
  GA_SERIAL.println(F("---------------------------"));
  GA_SERIAL.print(F("Toegang elke ")); GA_SERIAL.print(I2C_HERSTELTEST_TUSSENTIJD_TOEGANGEN_MS);
  GA_SERIAL.print(F(" ms, rapport elke ")); GA_SERIAL.print(I2C_HERSTELTEST_RAPPORT_ELKE_MS); GA_SERIAL.println(F(" ms"));
  Screen->Print("Hersteltest", "gestart");
}

void loop() {
  const unsigned long nu = millis();

  if (nu - tijdstipVorigeToegang >= I2C_HERSTELTEST_TUSSENTIJD_TOEGANGEN_MS) {
    tijdstipVorigeToegang = nu;

    if (sensorRFP602set1 != nullptr) {
      const unsigned long startMicros = micros();
      sensorRFP602set1->RawAnalogRead(sensorRFP602set1->sensorPin[0]);
      DuurVanToegangBijhouden(blokkeertijdMetingADS1115, startMicros);
    }

#if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_PCF8574)
    if (Input != nullptr) {
      const unsigned long startMicros = micros();
      Input->OpvragenHuidigeToetsAanslag(false);   // één scan, niet wachten
      DuurVanToegangBijhouden(blokkeertijdKeypadscanPCF8574, startMicros);
    }
#endif
  }

  if (nu - tijdstipVorigRapport >= I2C_HERSTELTEST_RAPPORT_ELKE_MS) {
    tijdstipVorigRapport = nu;
    aantalRapporten++;

    GA_SERIAL.print(F("Rapport ")); GA_SERIAL.println(aantalRapporten);
    RapportRegelTonen(F("  ADS1115 (RFP602-meting)"), blokkeertijdMetingADS1115);
#if ((INPUT_KANAAL_CONFIG) & INPUT_TYPE_PCF8574)
    RapportRegelTonen(F("  PCF8574 (keypadscan)"), blokkeertijdKeypadscanPCF8574);
#endif

    // Ook het scherm meten: Print() test eerst het LCD (runtime-herstel) en schrijft dan. Een volgende Print() na een hotplug toont S6.
    const unsigned long startMicros = micros();
    Screen->Print("Hersteltest", String(aantalRapporten));
    GA_SERIAL.print(F("  Screen->Print(): "));
    GA_SERIAL.print((micros() - startMicros) / 1000.0, 2);
    GA_SERIAL.println(F(" ms"));
  }
}
