// ============================================================================
// Sturing: Sturing_ISD1820_HardwareTest
// ============================================================================
// Begeleide fysieke test van opname en afspelen via de bestaande Relais-klasse
// op MCP23017 GPIO. GEEN fysieke relais aansluiten.
// GPA0 -> REC, GPA1 -> P-E/PLAYE, GPA2 -> P-L/PLAYL, GPA3 -> FT, GND -> GND.
// Speaker -> SP+ en SP-; gebruik voeding/logicaniveaus passend bij de echte module.
// MCP23017 bij voorkeur op 3,3 V; bij 5V-I2C alleen met passend busniveau.
// De ISD1820-chip is opgegeven voor 2,7..4,5 V. Verifieer het echte modulebord.
// STOP kan de synchrone opname van 5 s niet tussentijds onderbreken.
// PLAYL en FT blijven HIGH tot PLAYLSTOP, FTSTOP of STOP.
// Verwijder eventuele P-E/FT-jumpers op het modulebord voor externe GPIO-bediening.
// Hoorbare opname en afspelen moeten door de tester beoordeeld worden.
// Levenscyclus OK bewijst geen hoorbaar resultaat.
// Bestaande configuratie: ConfigMCP23017.h en ConfigRelais.h.
// ============================================================================

#include <GroeiAcademie.h>

#if STURING_RELAIS_AANTAL < 4
  #error Zet STURING_RELAIS_AANTAL op minstens 4 voor REC, PLAYE, PLAYL en FT.
#endif

#if EXTENDER_MCP23017_AANTAL != 1
  #error Deze hardwaretest vereist EXTENDER_MCP23017_AANTAL 1.
#endif

ExtenderMCP23017* extenderMCP23017 = nullptr;
Relais* opnemen = nullptr;
Relais* afspelen = nullptr;
Relais* afspelenWanneerActief = nullptr;
Relais* microfoonNaarLuidspreker = nullptr;

bool testActief = false;
bool startGelukt = false;
bool testGestopt = false;
String invoer;

void toonStartInstructies() {
  GA_SERIAL.println(F("HARDWARETEST: Sturing_ISD1820_HardwareTest"));
  GA_SERIAL.println(F("Controleer voeding/logicaniveaus, speaker en REC/P-E/P-L/FT. GEEN fysieke relais."));
  GA_SERIAL.println(F("Gebruik geen P-E/FT-jumper terwijl GPIO de signalen bestuurt."));
  GA_SERIAL.println(F("START: begeleide test starten."));
  GA_SERIAL.println(F("REC: 5 seconden opnemen; daarna PLAY: opname helemaal terugspelen."));
  GA_SERIAL.println(F("PLAYL: afspelen zolang P-L HIGH is; PLAYLSTOP: voortijdig stoppen."));
  GA_SERIAL.println(F("FT: microfoon rechtstreeks naar speaker; FTSTOP: doorluisteren stoppen."));
  GA_SERIAL.println(F("Herhaal REC/PLAY/PLAYL/FT zo vaak als nodig; wacht na PLAY tot de opname klaar is."));
  GA_SERIAL.println(F("OK of FOUT: manuele beoordeling in Serial tonen; leg resultaat ook vast in HTML."));
  GA_SERIAL.println(F("STOP: alle vier de uitgangen LOW en test beeindigen."));
}

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }

  GA_SERIAL.println(F("TEST: Sturing_ISD1820_HardwareTest"));
  extenderMCP23017 = GedeeldeBusNewComponent<ExtenderMCP23017>(&Native, GedeeldeBusComponent::DIGITAL_PINS_MCP23017, I2C_ADDRESS_EXTENDER_MCP23017_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL, EXTENDER_MCP23017_1_INTA, EXTENDER_MCP23017_1_INTB, EXTENDER_MCP23017_1_RESET);
  if (extenderMCP23017 != nullptr) opnemen = GedeeldeBusNewComponent<Relais>(extenderMCP23017, 0, HIGH);
  if (opnemen != nullptr) afspelen = GedeeldeBusNewComponent<Relais>(extenderMCP23017, 1, HIGH);
  if (afspelen != nullptr) afspelenWanneerActief = GedeeldeBusNewComponent<Relais>(extenderMCP23017, 2, HIGH);
  if (afspelenWanneerActief != nullptr) microfoonNaarLuidspreker = GedeeldeBusNewComponent<Relais>(extenderMCP23017, 3, HIGH);

  startGelukt = opnemen != nullptr && afspelen != nullptr && afspelenWanneerActief != nullptr && microfoonNaarLuidspreker != nullptr;
  if (startGelukt) {
    startGelukt = opnemen->setRelayStatusOnOff(false) && startGelukt;
    startGelukt = afspelen->setRelayStatusOnOff(false) && startGelukt;
    startGelukt = afspelenWanneerActief->setRelayStatusOnOff(false) && startGelukt;
    startGelukt = microfoonNaarLuidspreker->setRelayStatusOnOff(false) && startGelukt;
  }
  GA_SERIAL.println(startGelukt ? F("LEVENSCYCLUS: OK (geluid niet bewezen)") : F("LEVENSCYCLUS: FOUT"));
  toonStartInstructies();
}

void loop() {
  if (!GA_SERIAL.available()) return;

  invoer = GA_SERIAL.readStringUntil('\n');
  invoer.trim();
  invoer.toUpperCase();

  if (invoer == "STOP") {
    if (opnemen && !opnemen->setRelayStatusOnOff(false)) GA_SERIAL.println(F("STOP: FOUT bij REC LOW."));
    if (afspelen && !afspelen->setRelayStatusOnOff(false)) GA_SERIAL.println(F("STOP: FOUT bij PLAYE LOW."));
    if (afspelenWanneerActief && !afspelenWanneerActief->setRelayStatusOnOff(false)) GA_SERIAL.println(F("STOP: FOUT bij PLAYL LOW."));
    if (microfoonNaarLuidspreker && !microfoonNaarLuidspreker->setRelayStatusOnOff(false)) GA_SERIAL.println(F("STOP: FOUT bij FT LOW."));
    testActief = false;
    testGestopt = true;
    GA_SERIAL.println(F("STOP: alle uitgangen LOW gevraagd. Bij fout de voeding controleren; herstart voor nieuwe test."));
    return;
  }

  if (testGestopt || !startGelukt) {
    GA_SERIAL.println(F("Test niet beschikbaar. Controleer de levenscyclus of herstart Arduino."));
    return;
  }

  if (invoer == "START") {
    testActief = true;
    GA_SERIAL.println(F("START: bevestigd. REC, PLAY, PLAYL of FT invoeren."));
    return;
  }

  if (!testActief) {
    GA_SERIAL.println(F("Eerst START invoeren."));
    return;
  }

  if (invoer == "REC") {
    if (!afspelen->setRelayStatusOnOff(false) || !afspelenWanneerActief->setRelayStatusOnOff(false) || !microfoonNaarLuidspreker->setRelayStatusOnOff(false)) {
      GA_SERIAL.println(F("OPNAME: FOUT bij uitschakelen van eerdere afspeel-/doorluisterfunctie."));
      return;
    }

    if (!opnemen->setRelayStatusOnOff(true)) {
      GA_SERIAL.println(F("OPNAME: FOUT (REC niet hoog)."));
      return;
    }

    GA_SERIAL.println(F("OPNAME: spreek nu 5 seconden; STOP wordt tijdens opname niet gelezen."));
    delay(5000);
    GA_SERIAL.println(opnemen->setRelayStatusOnOff(false) ? F("OPNAME: puls voltooid; geluid niet automatisch bewezen") : F("OPNAME: FOUT (REC niet laag)."));
    return;
  }

  if (invoer == "PLAY") {
    if (!afspelenWanneerActief->setRelayStatusOnOff(false) || !microfoonNaarLuidspreker->setRelayStatusOnOff(false)) {
      GA_SERIAL.println(F("AFSPELEN: FOUT bij PLAYL/FT LOW."));
      return;
    }

    if (!afspelen->setRelayStatusOnOff(true)) {
      GA_SERIAL.println(F("AFSPELEN: FOUT (P-E niet hoog)."));
      return;
    }

    delay(100);
    GA_SERIAL.println(afspelen->setRelayStatusOnOff(false) ? F("AFSPELEN: puls voltooid; hoorbare opname zelf beoordelen") : F("AFSPELEN: FOUT (P-E niet laag)."));
    return;
  }

  if (invoer == "PLAYL") {
    if (!afspelen->setRelayStatusOnOff(false) || !microfoonNaarLuidspreker->setRelayStatusOnOff(false)) {
      GA_SERIAL.println(F("PLAYL: FOUT bij PLAYE/FT LOW."));
      return;
    }

    GA_SERIAL.println(afspelenWanneerActief->setRelayStatusOnOff(true) ? F("PLAYL: P-L HIGH. Luister en voer PLAYLSTOP in tijdens het afspelen.") : F("PLAYL: FOUT bij P-L HIGH."));
    return;
  }

  if (invoer == "PLAYLSTOP") {
    GA_SERIAL.println(afspelenWanneerActief->setRelayStatusOnOff(false) ? F("PLAYLSTOP: P-L LOW. Controleer of het afspelen direct stopt.") : F("PLAYLSTOP: FOUT bij P-L LOW."));
    return;
  }

  if (invoer == "FT") {
    if (!opnemen->setRelayStatusOnOff(false) || !afspelen->setRelayStatusOnOff(false) || !afspelenWanneerActief->setRelayStatusOnOff(false)) {
      GA_SERIAL.println(F("FT: FOUT bij REC/PLAYE/PLAYL LOW."));
      return;
    }

    GA_SERIAL.println(microfoonNaarLuidspreker->setRelayStatusOnOff(true) ? F("FT: HIGH. Spreek in de microfoon en luister naar de speaker; daarna FTSTOP.") : F("FT: FOUT bij FT HIGH."));
    return;
  }

  if (invoer == "FTSTOP") {
    GA_SERIAL.println(microfoonNaarLuidspreker->setRelayStatusOnOff(false) ? F("FTSTOP: FT LOW. Controleer of doorluisteren stopt.") : F("FTSTOP: FOUT bij FT LOW."));
    return;
  }

  if (invoer == "OK" || invoer == "FOUT") {
    GA_SERIAL.print(F("MANUELE BEOORDELING: "));
    GA_SERIAL.println(invoer);
    GA_SERIAL.println(F("Leg de beoordeelde handeling en het resultaat afzonderlijk vast in het HTML-testregister."));
    return;
  }

  GA_SERIAL.println(F("Gebruik START, REC, PLAY, PLAYL, PLAYLSTOP, FT, FTSTOP, OK, FOUT of STOP."));
}
