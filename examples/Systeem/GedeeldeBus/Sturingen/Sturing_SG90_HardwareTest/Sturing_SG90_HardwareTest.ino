// ============================================================================
// Sturing: SG90_HardwareTest
// ============================================================================
// Begeleide hardwaretest voor SG90. Elke beweging vereist eerst bevestiging via Serial.
// Pinnen zijn afgeleid uit de bestaande constructoren en de boardmapping in src/Configuratie/UserConfig.h en SystemConfig.h.
// PCA9685 kanaal 0 -> SG90 servosignaal; servo-voeding geschikt voor het model, GND gemeenschappelijk.
// I2C-aansluiting: SDA/SCL en GND op dezelfde bus; modulevoeding volgens het werkelijke Extender-bord.
// UNO R4 Minima/WiFi en UNO Q: SDA/SCL op de gelijknamige boardaansluitingen.
// ESP32 D1 UNO R32: SDA=GPIO21, SCL=GPIO22.
// ESP32-S3 ARDI-32: SDA=GPIO38, SCL=GPIO39; ESP32-S3 DEV: SDA=GPIO8, SCL=GPIO9.
// RP2040 Cytron Maker UNO: SDA=GPIO20, SCL=GPIO21.
// STM32 F401RE Nucleo: SDA=PB9, SCL=PB8.
// ============================================================================

// ============================================================================
// CONCREET AANSLUITEN EN TESTEN
// ============================================================================
// Arduino SDA -> SDA van de I2C-Extender; Arduino SCL -> SCL; GND -> GND.
// Voed de Extender-logica op het voor board en I2C-niveau geschikte VCC/VDD.
// Verifieer fysiek I2C-adres en optionele INT/RESET-pinnen met de actieve Config*.h.
// Bij 3,3V-logica op een 5V-bus: controleer een passende I2C-niveauomzetter.
// PCA9685 VCC is de logica-/I2C-voeding; V+ is een APARTE servo-/lastvoeding.
// PCA9685 OE moet een gedefinieerd niveau hebben; controleer de concrete breakout.
// Actieve Extenderconfig: src/Configuratie/Extenders/ConfigPCA9685.h
//   EXTENDER_PCA9685_AANTAL >= 1, I2C_ADDRESS_EXTENDER_PCA9685_1 = fysiek adres.
// Externe servovoeding (+) -> PCA9685 V+; voeding (-) -> PCA9685 GND.
// Servostekker: GND (meestal bruin/zwart) -> GND, + (meestal rood) -> V+,
//   signaal (meestal oranje/geel/wit) -> signaalcontact van gekozen kanaal.
// Laat de servo onbelast en vrij bewegen; gebruik Arduino 5V niet als motorvoeding.
// Extender-PWM-frequentie wordt alleen geconfigureerd wanneer
//   EXTENDER_PCA9685_1_PWM_FREQUENTIE in ConfigPCA9685.h is gedefinieerd.
//   Voor een servo-test circa 50 Hz instellen/nameten; geen automatische default aannemen.
// MIN_PWM en MAX_PWM uit ConfigSG90.h zijn 12-bit-ticks, geen graden of microseconden:
//   controleer de daadwerkelijke pulsbreedten en kalibreer zonder eindstopbelasting.
// Een STOP schakelt PWM uit, niet de externe servovoeding of het houdkoppel.
// PCA9685 signaalkanaal 0 -> SG90-signaaldraad.
// Actieve config: ConfigSG90.h (STURING_SG90_AANTAL >= 1).
// Serietests 45, 105, 90, 135 graden + start/pauze/hervat/stop.
// Controleer fysieke hoek, trillingen, stroom en vrije mechanische uitslag.
// ============================================================================

#include <GroeiAcademie.h>
#if STURING_SG90_AANTAL == 0
  #error Zet STURING_SG90_AANTAL op minstens 1 voor deze test.
#endif
#if EXTENDER_PCA9685_AANTAL == 0
  #error Schakel de vereiste Extender in voor deze Sturing-test.
#endif

ExtenderPCA9685* extenderPCA9685 = nullptr;
SG90* sg90 = nullptr;

ServomotorBesturing* motorBesturing = nullptr;

bool testActief = false;
bool startGelukt = false;
bool testGestopt = true;
String invoer;
uint8_t testStap = 0;
uint8_t testFase = 0;
uint16_t kalibratiePulsbreedteUS = 1500;
uint16_t kalibratieMinimumUS = 0;
uint16_t kalibratieMaximumUS = 0;
bool kalibratieMinimumBevestigd = false;
bool kalibratieMaximumBevestigd = false;
unsigned long testStartMillis = 0;

void toonStartInstructies() {
  GA_SERIAL.println(F("HARDWARETEST: Sturing_SG90_HardwareTest"));
  GA_SERIAL.println(F("BEGELEIDE SERVOTEST. Controleer voeding, kalibratie en mechanische hoekgrenzen."));
  GA_SERIAL.println(F("STOP schakelt het PWM-signaal uit; de servo houdt zijn positie dan niet gegarandeerd vast."));
  GA_SERIAL.println(F("KIES EEN TEST:"));
  GA_SERIAL.println(F("0 = Kalibratie"));
  GA_SERIAL.println(F("1 = PositioneerAbsoluutNaarHoek"));
  GA_SERIAL.println(F("2 = DraaiRelatiefOverHoek"));
  GA_SERIAL.println(F("3 = PositioneerNaarMiddenpositie"));
  GA_SERIAL.println(F("4 = StartMotor"));
  GA_SERIAL.println(F("5 = PauzeerMotor / HervatMotor"));
  GA_SERIAL.println(F("6 = SchakelMotorIn / SchakelMotorUit"));
  GA_SERIAL.println(F("Typ 0 t/m 6 + Enter. STOP stopt de huidige test en keert terug naar het menu."));
  GA_SERIAL.println(F("Controleer de beweging fysiek; een geslaagde functieaanroep bewijst geen motorbeweging."));
}

void toonTestInstructies() {
  GA_SERIAL.println();

  switch (testStap) {
    case 0:  GA_SERIAL.println(F("TEST 0: KALIBRATIE VAN DE SG90"));
             GA_SERIAL.println(F("WAT GAAN WE DOEN? Je kiest zelf of je de minimale of de maximale pulsbreedte kalibreert."));
             GA_SERIAL.println(F("WAT MOET JE DOEN? Haal de mechanische belasting weg en zorg dat de servoarm vrij kan bewegen."));
             GA_SERIAL.println(F("Na de start kies je MIN of MAX. We vertrekken van de huidige ingestelde waarde voor die grens."));
             GA_SERIAL.println(F("Je verandert uitsluitend de gekozen grens. De andere pulsgrens blijft behouden."));
             GA_SERIAL.println(F("Met + of - verander je de gekozen pulsbreedte telkens met 10 microseconden."));
             GA_SERIAL.println(F("WAT MOET JE ZIEN? De servo reageert op iedere nieuwe pulsbreedte. De puls wordt telkens automatisch uitgeschakeld."));
             GA_SERIAL.println(F("Ga alleen verder zolang de servo soepel beweegt. Bij brommen, schokken, blokkeren of weerstand typ je onmiddellijk STOP."));
             GA_SERIAL.println(F("Met MIN bevestig je MIN. Met MAX bevestig je MAX. Beide grenzen worden apart bevestigd."));
             GA_SERIAL.println(F("Pas nadat MIN en MAX bevestigd zijn, sluit OK de kalibratie af en verschijnen de #define-regels voor ConfigSG90.h."));
             GA_SERIAL.println(F("Met STOP breek je de test af zonder de kalibratie toe te passen. De bevestigde waarden en configuratieregels blijven zichtbaar."));
             GA_SERIAL.println(F("De grenzen blijven tot een herstart in het geheugen. Voor blijvend gebruik pas je de getoonde #define-regels aan en compileer je opnieuw."));
             GA_SERIAL.println(F("De softwaregrenzen van 750 tot 2250 microseconden garanderen niet dat jouw servo dat volledige bereik veilig kan bewegen."));
             break;
    case 1:  GA_SERIAL.println(F("TEST 1: SERVO NAAR 45 GRADEN"));
             GA_SERIAL.println(F("WAT GAAN WE DOEN? We geven de servo een absolute doelpositie van 45 graden."));
             GA_SERIAL.println(F("WAT MOET JE ZIEN? De servoarm beweegt naar de bij 45 graden horende positie, tenzij hij daar al staat."));
             GA_SERIAL.println(F("WANNEER GESLAAGD? De servo reageert zonder te blokkeren, voortdurend te brommen of ongewoon te trillen."));
             break;
    case 2:  GA_SERIAL.println(F("TEST 2: VAN 45 NAAR 105 GRADEN"));
             GA_SERIAL.println(F("WAT GAAN WE DOEN? We geven eerst 45 graden op en daarna onmiddellijk een relatieve opdracht van 60 graden erbij."));
             GA_SERIAL.println(F("WAT MOET JE ZIEN? De uiteindelijke doelpositie is 105 graden. De tussenbeweging naar 45 graden hoeft niet zichtbaar te zijn."));
             GA_SERIAL.println(F("WANNEER GESLAAGD? De servo reageert op de nieuwe doelpositie zonder te blokkeren of ongewoon te trillen."));
             break;
    case 3:  GA_SERIAL.println(F("TEST 3: SERVO NAAR HET MIDDEN"));
             GA_SERIAL.println(F("WAT GAAN WE DOEN? We vragen de middenpositie van 90 graden."));
             GA_SERIAL.println(F("WAT MOET JE ZIEN? De servoarm beweegt naar de bij 90 graden horende positie, tenzij hij daar al staat."));
             GA_SERIAL.println(F("WANNEER GESLAAGD? De servo reageert zonder te blokkeren of voortdurend te brommen."));
             break;
    case 4:  GA_SERIAL.println(F("TEST 4: MOTORSIGNAAL OPNIEUW STARTEN"));
             GA_SERIAL.println(F("WAT GAAN WE DOEN? We geven 90 graden op, schakelen het PWM-signaal uit en schakelen dat signaal opnieuw in."));
             GA_SERIAL.println(F("WAT MOET JE ZIEN? De servo kan naar 90 graden bewegen, maar hoeft bij het opnieuw inschakelen niet zichtbaar te draaien."));
             GA_SERIAL.println(F("WANNEER GESLAAGD? Het PWM-signaal wordt succesvol opnieuw ingeschakeld en de servo reageert zonder ongewoon gedrag."));
             break;
    case 5:  GA_SERIAL.println(F("TEST 5: PAUZEREN EN HERVATTEN"));
             GA_SERIAL.println(F("WAT GAAN WE DOEN? We sturen de servo naar 135 graden en onderbreken daarna het PWM-signaal."));
             GA_SERIAL.println(F("WAT MOET JE ZIEN? De servo gaat naar de doelpositie. Na de pauze is er geen houdsignaal en kan de arm losser worden."));
             GA_SERIAL.println(F("Daarna vraagt het programma je om nogmaals 1 te typen. Pas dan wordt dezelfde doelpositie opnieuw aangestuurd."));
             GA_SERIAL.println(F("WANNEER GESLAAGD? De PWM-aansturing stopt tijdens de pauze en komt na jouw bevestiging terug."));
             break;
    case 6:  GA_SERIAL.println(F("TEST 6: MOTORSIGNAAL IN- EN UITSCHAKELEN"));
             GA_SERIAL.println(F("WAT GAAN WE DOEN? We geven 90 graden op, schakelen het PWM-signaal uit en weer in, en schakelen het na de test opnieuw uit."));
             GA_SERIAL.println(F("WAT MOET JE ZIEN? De servo kan naar 90 graden bewegen, maar bij het in- en uitschakelen hoeft geen draaiing zichtbaar te zijn."));
             GA_SERIAL.println(F("WANNEER GESLAAGD? De functieaanroepen slagen en de servo reageert zonder ongewoon geluid of trilling."));
             break;
    default: GA_SERIAL.println(F("Ongeldige testkeuze.")); return;
  }

  if (testStap >= 1 && testStap <= 6) GA_SERIAL.println(F("VOORBEREIDING: de servo gaat eerst naar 0 graden. Daarna wacht het programma 1,5 seconde voordat de gekozen test start."));

  GA_SERIAL.println();
  GA_SERIAL.println(F("1 = Start de test."));
}

void toonFunctieResultaat(bool ok) { GA_SERIAL.println(ok ? F("FUNCTIEAANROEP: OK (geen fysieke positiefeedback)") : F("FUNCTIEAANROEP: FOUT")); }

void rondTestAf(bool ok) {
  if (testStap == 6 && ok) {
    ok = motorBesturing->SchakelMotorUit();
    GA_SERIAL.println(F("SchakelMotorUit(): PWM uitgeschakeld."));
    toonFunctieResultaat(ok);
  }

  motorBesturing->StopMotor();
  GA_SERIAL.println(F("MOTOR: STOP aangeroepen. PWM-signaal uit; servo kan kracht verliezen."));
  toonFunctieResultaat(ok);
  testActief = false;
  testGestopt = false;
  testFase = 3;

  if (!ok) GA_SERIAL.println(F("TEST GESTOPT NA FOUT. Controleer de hardware voordat je verdergaat."));
  else GA_SERIAL.println(F("TEST UITGEVOERD. Controleer of de servo heeft gedaan wat hierboven beschreven staat."));

  GA_SERIAL.println(F("0 = Terug naar het hoofdmenu."));
}

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  while (!GA_SERIAL) { ; }
  GA_SERIAL.println(F("TEST: Sturing_SG90_HardwareTest"));
  extenderPCA9685 = GedeeldeBusNewComponent<ExtenderPCA9685>(&Native, GedeeldeBusComponent::PWM_PCA9685, I2C_ADDRESS_EXTENDER_PCA9685_1, HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL);
  #if defined(STURING_SG90_MIN_PWM) && defined(STURING_SG90_MAX_PWM)
  if (extenderPCA9685 != nullptr) sg90 = GedeeldeBusNewComponent<SG90>(extenderPCA9685, 0);
#else
  if (extenderPCA9685 != nullptr) sg90 = GedeeldeBusNewComponent<SG90>(extenderPCA9685, 0, 205, 410);
#endif
  if (sg90 != nullptr) motorBesturing = new ServomotorBesturing(*sg90);
  startGelukt = sg90 != nullptr && motorBesturing != nullptr;
  if (startGelukt) motorBesturing->StopMotor();
  GA_SERIAL.println(startGelukt ? F("LEVENSCYCLUS: OK. BEGINSTATUS: PWM UIT.") : F("LEVENSCYCLUS: FOUT. GEEN TEST UITVOEREN."));
  if (startGelukt) toonStartInstructies();
}

void loop() {
  if (GA_SERIAL.available()) {
    invoer = GA_SERIAL.readStringUntil('\n');
    invoer.trim();
    invoer.toUpperCase();

    if (invoer == "STOP") {
      if (startGelukt) motorBesturing->StopMotor();

      if (startGelukt && !testGestopt && testStap == 0 && testFase >= 4 && testFase <= 8) {
        GA_SERIAL.println(F("KALIBRATIE GESTOPT. Bevestigde grenzen zijn niet toegepast."));
        GA_SERIAL.println(F("KALIBRATIERESULTAAT SG90"));
        GA_SERIAL.print(F("MIN = ")); GA_SERIAL.print(kalibratieMinimumUS); GA_SERIAL.println(kalibratieMinimumBevestigd ? F(" us (bevestigd)") : F(" us (niet bevestigd)"));
        GA_SERIAL.print(F("MAX = ")); GA_SERIAL.print(kalibratieMaximumUS); GA_SERIAL.println(kalibratieMaximumBevestigd ? F(" us (bevestigd)") : F(" us (niet bevestigd)"));

        uint16_t frequentie = 0;
        if (extenderPCA9685->ReadExtenderPWMFrequency(frequentie) && frequentie != 0) {
          GA_SERIAL.println(F("Voor ConfigSG90.h (bevestigde waarden, niet toegepast):"));
          GA_SERIAL.print(F("#define STURING_SG90_MIN_PWM "));
          GA_SERIAL.println(kalibratieMinimumBevestigd ? (uint32_t(kalibratieMinimumUS) * frequentie * 4096ULL + 500000ULL) / 1000000ULL : sg90->minimumPWM);
          GA_SERIAL.print(F("#define STURING_SG90_MAX_PWM "));
          GA_SERIAL.println(kalibratieMaximumBevestigd ? (uint32_t(kalibratieMaximumUS) * frequentie * 4096ULL + 500000ULL) / 1000000ULL : sg90->maximumPWM);
        } else {
          GA_SERIAL.println(F("PWM-frequentie niet beschikbaar. Configuratieregels kunnen niet berekend worden."));
        }
      }

      testActief = false;

      if (startGelukt && !testGestopt) {
        testFase = 3;
        GA_SERIAL.println(F("STOP: PWM uit. De gekozen test is gestopt."));
        GA_SERIAL.println(F("0 = Terug naar het hoofdmenu."));
      } else {
        GA_SERIAL.println(F("STOP: PWM uit."));
      }

      return;
    }

    if (!startGelukt) {
      GA_SERIAL.println(F("LEVENSCYCLUS: FOUT. GEEN TEST UITVOEREN."));
      return;
    }

    if (testGestopt) {
      const int gekozenTest = invoer.toInt();
      if (gekozenTest < 0 || gekozenTest > 6 || String(gekozenTest) != invoer) {
        GA_SERIAL.println(F("Ongeldige keuze. Typ 0 t/m 6 + Enter, of STOP."));
        return;
      }

      testStap = static_cast<uint8_t>(gekozenTest);
      testGestopt = false;
      testActief = false;
      testFase = 0;
      toonTestInstructies();
      return;
    }

    if (!testActief && testFase == 3) {
      if (invoer == "0") {
        testGestopt = true;
        toonStartInstructies();
      } else {
        GA_SERIAL.println(F("0 = Terug naar het hoofdmenu."));
      }

      return;
    }

    if (!testActief && testFase == 0) {
      if (invoer != "1") {
        GA_SERIAL.println(F("1 = Start de test."));
        return;
      }

      if (testStap == 0) {
        uint16_t frequentie = 0;
        if (!extenderPCA9685->ReadExtenderPWMFrequency(frequentie) || frequentie == 0) {
          GA_SERIAL.println(F("TEST 0: PWM-frequentie niet beschikbaar. Kalibratie niet gestart."));
          rondTestAf(false);
          return;
        }

        kalibratieMinimumUS = static_cast<uint16_t>((uint32_t(sg90->minimumPWM) * 1000000UL + uint32_t(frequentie) * 2048UL) / (uint32_t(frequentie) * 4096UL));
        kalibratieMaximumUS = static_cast<uint16_t>((uint32_t(sg90->maximumPWM) * 1000000UL + uint32_t(frequentie) * 2048UL) / (uint32_t(frequentie) * 4096UL));
        kalibratieMinimumBevestigd = false;
        kalibratieMaximumBevestigd = false;
        testActief = true;
        testFase = 4;
        GA_SERIAL.println(F("TEST 0: Welke grens wil je kalibreren?"));
        GA_SERIAL.print(F("Huidige MIN = ")); GA_SERIAL.print(kalibratieMinimumUS); GA_SERIAL.print(F(" us (")); GA_SERIAL.print(sg90->minimumPWM); GA_SERIAL.println(F(" ticks)."));
        GA_SERIAL.print(F("Huidige MAX = ")); GA_SERIAL.print(kalibratieMaximumUS); GA_SERIAL.print(F(" us (")); GA_SERIAL.print(sg90->maximumPWM); GA_SERIAL.println(F(" ticks)."));
        GA_SERIAL.println(F("MIN = Kalibreer alleen de minimale pulsbreedte vanaf de huidige MIN."));
        GA_SERIAL.println(F("MAX = Kalibreer alleen de maximale pulsbreedte vanaf de huidige MAX."));
        GA_SERIAL.println(F("MIN = Kies MIN of bevestig MIN. MAX = Kies MAX of bevestig MAX."));
        GA_SERIAL.println(F("OK sluit de kalibratie pas af nadat beide grenzen bevestigd zijn. STOP breekt de test af."));
        return;
      }

      

      GA_SERIAL.print(F("START TEST ")); GA_SERIAL.println(testStap);
      const bool ok = motorBesturing->PositioneerAbsoluutNaarHoek(0);
      toonFunctieResultaat(ok);

      if (!ok) {
        rondTestAf(false);
        return;
      }

      testActief = true;
      testFase = 9;
      testStartMillis = millis();
      GA_SERIAL.println(F("VOORBEREIDING: 0 graden opgedragen. De gekozen test start na 1,5 seconde."));
      return;
    }

    

    if (testStap == 0) {
      if (testFase == 4 && invoer != "OK") {
        if (invoer != "MIN" && invoer != "MAX") {
          GA_SERIAL.println(F("Kies eerst MIN of MAX. Met STOP beëindig je de test."));
          return;
        }

        if (invoer == "MIN") kalibratieMinimumBevestigd = false;
        else kalibratieMaximumBevestigd = false;

        kalibratiePulsbreedteUS = invoer == "MIN" ? kalibratieMinimumUS : kalibratieMaximumUS;
        if (kalibratiePulsbreedteUS < 750 || kalibratiePulsbreedteUS > 2250) {
          GA_SERIAL.println(F("TEST 0: de huidige grens ligt buiten het toegestane kalibratiebereik van 750 tot 2250 us."));
          GA_SERIAL.println(F("De servo krijgt geen puls. Typ STOP om de kalibratie te beëindigen."));
          return;
        }

        const bool ok = motorBesturing->CalibrationServomotor(kalibratiePulsbreedteUS);
        toonFunctieResultaat(ok);

        if (!ok) {
          rondTestAf(false);
          return;
        }

        testFase = invoer == "MIN" ? 7 : 8;
        testStartMillis = millis();
        GA_SERIAL.print(F("TEST 0: ")); GA_SERIAL.print(invoer == "MIN" ? F("MIN") : F("MAX")); GA_SERIAL.print(F(" begint bij ")); GA_SERIAL.print(kalibratiePulsbreedteUS);
        GA_SERIAL.println(F(" us. De andere grens blijft behouden."));
        GA_SERIAL.println(F("+ = 10 us hoger; - = 10 us lager. Met MIN bevestig je MIN en met MAX bevestig je MAX."));
        GA_SERIAL.println(F("Iedere puls stopt automatisch na 600 ms. Bij blokkeren of brommen: STOP."));
        return;
      }

      if (invoer == "+" || invoer == "-") {
        if (testFase != 5 && testFase != 6 && testFase != 7 && testFase != 8) {
          GA_SERIAL.println(F("Kies eerst MIN of MAX."));
          return;
        }

        const int32_t microseconden = int32_t(kalibratiePulsbreedteUS) + (invoer == "+" ? 10 : -10);
        if (microseconden < 750 || microseconden > 2250) {
          GA_SERIAL.println(F("TEST 0: softwaregrens 750 tot 2250 us bereikt. Er wordt geen puls gestuurd."));
          return;
        }

        const bool ok = motorBesturing->CalibrationServomotor(static_cast<uint16_t>(microseconden));
        toonFunctieResultaat(ok);

        if (!ok) {
          rondTestAf(false);
          return;
        }

        kalibratiePulsbreedteUS = static_cast<uint16_t>(microseconden);
        testFase = (testFase == 5 || testFase == 7) ? 7 : 8;
        testStartMillis = millis();
        GA_SERIAL.print(F("TEST 0: gekozen pulsbreedte = ")); GA_SERIAL.print(kalibratiePulsbreedteUS);
        GA_SERIAL.println(F(" us. Bevestig met MIN of MAX volgens de gekozen grens. STOP breekt de test af."));
        return;
      }

      if (invoer == "MIN" && (testFase == 5 || testFase == 7)) {
        if (kalibratiePulsbreedteUS >= kalibratieMaximumUS) {
          GA_SERIAL.println(F("TEST 0: MIN moet kleiner zijn dan MAX. Wijzig MIN voordat je bevestigt."));
          return;
        }

        motorBesturing->StopMotor();
        kalibratieMinimumUS = kalibratiePulsbreedteUS;
        kalibratieMinimumBevestigd = true;
        testFase = 4;
        GA_SERIAL.print(F("MIN bevestigd: ")); GA_SERIAL.print(kalibratieMinimumUS); GA_SERIAL.println(F(" us."));
        GA_SERIAL.println(kalibratieMaximumBevestigd ? F("MIN en MAX bevestigd. Typ OK om de kalibratie af te ronden.") : F("Kies MAX om de andere grens te kalibreren."));
        return;
      }

      if (invoer == "MAX" && (testFase == 6 || testFase == 8)) {
        if (kalibratiePulsbreedteUS <= kalibratieMinimumUS) {
          GA_SERIAL.println(F("TEST 0: MAX moet groter zijn dan MIN. Wijzig MAX voordat je bevestigt."));
          return;
        }

        motorBesturing->StopMotor();
        kalibratieMaximumUS = kalibratiePulsbreedteUS;
        kalibratieMaximumBevestigd = true;
        testFase = 4;
        GA_SERIAL.print(F("MAX bevestigd: ")); GA_SERIAL.print(kalibratieMaximumUS); GA_SERIAL.println(F(" us."));
        GA_SERIAL.println(kalibratieMinimumBevestigd ? F("MIN en MAX bevestigd. Typ OK om de kalibratie af te ronden.") : F("Kies MIN om de andere grens te kalibreren."));
        return;
      }

      if (invoer == "OK") {
        if (testFase != 4 || !kalibratieMinimumBevestigd || !kalibratieMaximumBevestigd) {
          GA_SERIAL.println(F("TEST 0: bevestig eerst MIN met MIN en MAX met MAX. Daarna sluit OK de kalibratie af."));
          return;
        }

        const bool ok = motorBesturing->SetPulsLimitsMS(kalibratieMinimumUS, kalibratieMaximumUS);
        toonFunctieResultaat(ok);

        if (!ok) {
          GA_SERIAL.println(F("TEST 0: pulsgrenzen niet toegepast. Controleer MIN en MAX."));
          return;
        }

        GA_SERIAL.println(F("KALIBRATIERESULTAAT SG90"));
        GA_SERIAL.print(F("MIN = ")); GA_SERIAL.print(kalibratieMinimumUS); GA_SERIAL.println(F(" us"));
        GA_SERIAL.print(F("MAX = ")); GA_SERIAL.print(kalibratieMaximumUS); GA_SERIAL.println(F(" us"));
        GA_SERIAL.println(F("Voor ConfigSG90.h:"));
        GA_SERIAL.print(F("#define STURING_SG90_MIN_PWM ")); GA_SERIAL.println(sg90->minimumPWM);
        GA_SERIAL.print(F("#define STURING_SG90_MAX_PWM ")); GA_SERIAL.println(sg90->maximumPWM);
        rondTestAf(true);
        return;
      }

      GA_SERIAL.println(F("TEST 0: kies of bevestig MIN/MAX, gebruik + of -, of rond met OK af nadat beide bevestigd zijn. STOP breekt af."));
      return;
    }
    if (testStap == 5 && testActief && testFase == 1 && invoer == "1") {
      const bool ok = motorBesturing->HervatMotor();
      GA_SERIAL.println(F("HervatMotor(): PWM voor bewaarde doelhoek opnieuw ingeschakeld."));
      toonFunctieResultaat(ok);

      if (!ok) {
        rondTestAf(false);
        return;
      }

      testFase = 2;
      testStartMillis = millis();
      return;
    }

    GA_SERIAL.println(F("TEST BEZIG. Gebruik STOP, of rond de huidige test af."));
  }

  if (!testActief) return;

  if (testStap == 0) {
    if ((testFase == 7 || testFase == 8) && millis() - testStartMillis >= 600UL) {
      motorBesturing->StopMotor();
      testFase = testFase == 7 ? 5 : 6;
      GA_SERIAL.println(F("TEST 0: PWM uit. Beoordeel de beweging. Bevestig MIN met MIN of MAX met MAX. STOP breekt af."));
    }

    return;
  }

  if (testFase == 9) {
    if (millis() - testStartMillis < 1500UL) return;

    bool ok = false;
    switch (testStap) {
      case 1: ok = motorBesturing->PositioneerAbsoluutNaarHoek(45); break;
      case 2: ok = motorBesturing->PositioneerAbsoluutNaarHoek(45) && motorBesturing->DraaiRelatiefOverHoek(60); break;
      case 3: ok = motorBesturing->PositioneerNaarMiddenpositie(); break;
      case 4: ok = motorBesturing->PositioneerNaarMiddenpositie() && motorBesturing->StopMotor() && motorBesturing->StartMotor(); break;
      case 5: ok = motorBesturing->PositioneerAbsoluutNaarHoek(135); break;
      case 6: ok = motorBesturing->PositioneerNaarMiddenpositie() && motorBesturing->StopMotor() && motorBesturing->SchakelMotorIn(); break;
    }

    if (!ok) {
      rondTestAf(false);
      return;
    }

    testFase = 0;
    testStartMillis = millis();
    GA_SERIAL.println(F("FUNCTIEAANROEP: OK. Controleer de reactie van de servo."));
    return;
  }

  if (testStap == 5 && testFase == 1) return;

  if (testStap == 5 && testFase == 0 && millis() - testStartMillis >= 500UL) {
    const bool ok = motorBesturing->PauzeerMotor();
    GA_SERIAL.println(F("PauzeerMotor(): PWM uit. De fysieke stand is niet gemeten."));
    toonFunctieResultaat(ok);

    if (!ok) {
      rondTestAf(false);
      return;
    }

    testFase = 1;
    GA_SERIAL.println(F("Motor heeft geen houdsignaal. Typ 1 + Enter om de doelhoek opnieuw aan te sturen."));
    return;
  }

  if (millis() - testStartMillis >= 2000UL) rondTestAf(true);
}
