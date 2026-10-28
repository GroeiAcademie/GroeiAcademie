// GROEI ACADEMIE FrameWork v2.0.0 RC1
// Interactieve PCF8574-foutinjectie: referentie -> fout -> herstel.
// Basis: examples/Systeem/Input/InputkanalenHX1838/InputkanalenHX1838.ino
//        examples/Systeem/GedeeldeBus/Extenders/Extender_PCF8574/Extender_PCF8574.ino
// Runtime-toegangen worden via de bestaande FrameWork-foutladder verwerkt.
// GB400/GB407 verschijnen alleen wanneer de ingestelde fout-/hersteldrempels bereikt zijn.
// Een ontbrekende foutcode is GEEN geslaagde foutcodetest.
// LET OP: geen draden onder spanning lostrekken. Gebruik een vooraf veilige,
//        gecontroleerde testvoorziening die de HX1838 bereikbaar houdt.

#include <GroeiAcademie.h>
#include <Configuratie/Examples.h>
#include <Configuratie/ExamplesConfig.h>

#if INPUT_KANAAL_CONFIG != INPUT_TYPE_HX1838
  #error Deze interactieve test vereist INPUT_KANAAL_CONFIG == INPUT_TYPE_HX1838.
#endif
#if EXTENDER_PCF8574_AANTAL < 1
  #error Deze interactieve test vereist EXTENDER_PCF8574_AANTAL >= 1.
#endif
#if HX1838_TOETSENINDELING != HX1838_TOETSENINDELING_REMOTE_OK_BOVENAAN_17_TOETSEN && HX1838_TOETSENINDELING != HX1838_TOETSENINDELING_REMOTE_OK_ONDERAAN_17_TOETSEN
  #error Deze test vereist een HX1838-afstandsbediening met 17 toetsen en een OK-toets.
#endif

ExtenderPCF8574 extender(
  &Native, GedeeldeBusComponent::DIGITAL_PINS_PCF8574,
  I2C_ADDRESS_EXTENDER_PCF8574_1,
  HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL,
  EXTENDER_PCF8574_1_INT
);

uint8_t gekozenTest = 0;
uint8_t testFase = 0;
uint8_t testResultaat = 0;
bool gelukt = false;

void ToonTestMenu() {
  GA_SERIAL.println(F("\n=== INTERACTIEVE FOUTINJECTIE ==="));
  GA_SERIAL.println(F("1: PCF8574 normaal / storing / herstel"));
  GA_SERIAL.println(F("9: resultaat van laatste sessie"));
  GA_SERIAL.println(F("Gebruik HX1838: toets 1 of 9; bevestig elke stap met OK."));
  GA_SERIAL.println(F("Andere foutcodes: nog niet geimplementeerd of geverifieerd."));
}

bool ControleerTestResultaat() {
  if (!gelukt) {
    GA_SERIAL.println(F("PCF8574 lifecycle eerder mislukt: controle niet uitvoerbaar."));
    return false;
  }
  if (!extender.ComponentBeschikbaarTijdensRuntime()) {
    GA_SERIAL.println(F("PCF8574 runtime: niet beschikbaar; herstelpoging/pauze volgens FrameWork."));
    return false;
  }
  extender.read8();
  const bool bereikbaar = extender.lastError() == 0;
  GA_SERIAL.print(F("PCF8574 lastError: "));
  GA_SERIAL.println(extender.lastError());
  extender.ResultaatComponentToegangTijdensRuntimeVerwerken(bereikbaar);
  return bereikbaar;
}

void VoerGekozenTestUit() {
  if (gekozenTest != 1) return;

  if (testFase == 1) {
    if (ControleerTestResultaat()) {
      testResultaat |= 0x01;
      GA_SERIAL.println(F("REFERENTIE: OK, PCF8574 antwoordt."));
    } else {
      GA_SERIAL.println(F("REFERENTIE: FOUT, test afgebroken; verhelp eerst het probleem."));
      testFase = 0;
      ToonTestMenu();
      return;
    }
    GA_SERIAL.println(F("FASE 2: maak PCF8574 via een VEILIGE testvoorziening onbereikbaar."));
    GA_SERIAL.println(F("Laat Arduino en HX1838 in bedrijf. Druk OK als de fout actief is."));
    testFase = 2;
  } else if (testFase == 2) {
    if (!ControleerTestResultaat()) {
      testResultaat |= 0x02;
      GA_SERIAL.println(F("FOUTDETECTIE: OK, PCF8574 antwoordt niet."));
    } else {
      GA_SERIAL.println(F("FOUTDETECTIE: FOUT, PCF8574 antwoordt nog."));
    }
    GA_SERIAL.println(F("FASE 3: herstel de verbinding via dezelfde VEILIGE voorziening."));
    GA_SERIAL.println(F("Druk OK als de normale toestand hersteld is."));
    testFase = 3;
  } else if (testFase == 3) {
    if (ControleerTestResultaat()) {
      testResultaat |= 0x04;
      GA_SERIAL.println(F("HERSTEL: OK, PCF8574 antwoordt opnieuw."));
    } else {
      GA_SERIAL.println(F("HERSTEL: FOUT, PCF8574 antwoordt niet."));
    }
    GA_SERIAL.println(F("=== SAMENVATTING ==="));
    GA_SERIAL.println((testResultaat & 0x01) ? F("Normaal: OK") : F("Normaal: FOUT"));
    GA_SERIAL.println((testResultaat & 0x02) ? F("Foutdetectie: OK") : F("Foutdetectie: FOUT"));
    GA_SERIAL.println((testResultaat & 0x04) ? F("Herstel: OK") : F("Herstel: FOUT"));
    GA_SERIAL.println(F("GB400/GB407: controleer de werkelijke FrameWork-meldingen in Serial."));
    GA_SERIAL.println(F("Drie fasen OK betekent nog NIET dat beide foutcodes verschenen."));
    testFase = 0;
    gekozenTest = 0;
    ToonTestMenu();
  }
}

void TestFunctie1() {
  if (testFase != 0) return;
  gekozenTest = 1;
  testFase = 1;
  testResultaat = 0;
  GA_SERIAL.println(F("FASE 1: controleer of PCF8574 aangesloten en bereikbaar is."));
  GA_SERIAL.println(F("Druk OK voor de referentiemeting."));
}
void TestFunctie9() {
  GA_SERIAL.println(F("=== LAATSTE RESULTAAT ==="));
  GA_SERIAL.println((testResultaat & 0x01) ? F("Normaal: OK") : F("Normaal: NIET OK/beoordeeld"));
  GA_SERIAL.println((testResultaat & 0x02) ? F("Foutdetectie: OK") : F("Foutdetectie: NIET OK/beoordeeld"));
  GA_SERIAL.println((testResultaat & 0x04) ? F("Herstel: OK") : F("Herstel: NIET OK/beoordeeld"));
}
void TestFunctieOk() { VoerGekozenTestUit(); }

const MappingTussenToetsaanslagEnUitTeVoerenFunctie mappingTestMenu[] = {
  {_LABEL_OPSCHRIFT_1, TestFunctie1},
  {_LABEL_OPSCHRIFT_9, TestFunctie9},
  {_LABEL_OPSCHRIFT_OK, TestFunctieOk}
};

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }

  Screen = GedeeldeBusNewComponent<struct Screen>();
  if (Screen == nullptr) exit(0);
  Input = GedeeldeBusNewComponent<struct Input>(INPUT_TYPE_HX1838);
  if (Input == nullptr) exit(0);

  gelukt = extender.aanmelden();
  if (gelukt) gelukt = extender.controleren();
  if (gelukt) gelukt = extender.inpluggen();
  if (gelukt) gelukt = extender.activeren();
  GA_SERIAL.println(gelukt ? F("PCF8574 lifecycle: OK") : F("PCF8574 lifecycle: FOUT"));
  ToonTestMenu();
}

void loop() {
  Input->UitVoerenFunctieVolgensMappingMetToetsAanslag(true, mappingTestMenu);
}
