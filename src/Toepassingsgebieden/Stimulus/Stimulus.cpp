#include "Stimulus.h"

// ============================================================================
// Interne hulpfuncties (D020): dienen uitsluitend als bouwsteen binnen dit
// bestand, worden door geen enkel voorbeeld of ander bronbestand aangeroepen,
// en zijn daarom niet langer publiek gedeclareerd in Stimulus.h.
// ============================================================================
static SynchronisatieProfiel MaakSynchronisatieProfielAlleSensoren(SensorMeetStatus sensor[], int aantalSensorenSimultaanTeMeten, StimulusProfiel gemetenStimulus[]);

static int  BepaalAantalSensorenSynchroon(unsigned long tijden[], int aantalSensoren, unsigned long toegestaneMarge);
static void BerekenEindStimulus(SensorMeetStatus &sensor, StimulusProfiel &gemetenStimulus);
static void InitialiseerSensorStart(unsigned long nu, SensorMeetStatus &sensor);
static int  MaakSensorMask(SensorMeetStatus sensor[], int aantalSensorenSimultaanTeMeten, bool testOpDRUKWAARDE = true);
static void ResetStimulusProfiel(StimulusProfiel &gemetenStimulus);
static void ResetSynchronisatieProfiel(SynchronisatieProfiel &synchronisatie);
static void VerwerkSensor(unsigned long nu, int sensorPin, int offsetSensor, SensorMeetStatus &sensor);

// ============================================================================
// RawAnalogRead: backend-afhankelijke ruwe sensorlezing.
// Losstaand van AnalogReadMetGekorigeerdeOffsets() omdat BepaalSensorOffsets()
// en de wegwerp-meting in MeetStimulus() ook een ruwe lezing nodig hebben, zonder offsetcorrectie.
// ============================================================================
#if ADC_BACKEND == ADC_BACKEND_ADS1115
  #include <Adafruit_ADS1X15.h>
  static Adafruit_ADS1115 ads;
  static bool ads1115Aanwezig = false;

  void InitialiseerADS1115() {
    if (!ads.begin(ADS1115_I2C_ADDRESS)) {
      ads1115Aanwezig = false;
      PrintToScreen(_LCD_ADS1115_FOUT, _LCD_ADS1115_NIET_GEVONDEN, _LCD_LEESTIJD_FEEDBACK_MS);
      return;
    }

    ads.setGain(GAIN_TWOTHIRDS);
    ads1115Aanwezig = true;
  }

  int RawAnalogRead(int sensorPin) {
    if (!ads1115Aanwezig) return 0;
    return ads.readADC_SingleEnded(sensorPin);
  }
#else
  void InitialiseerADS1115() {
  }

  int RawAnalogRead(int sensorPin) {
    return analogRead(sensorPin);
  }
#endif

const int sensorPin[4] = { PIN_SENSOR_1, PIN_SENSOR_2, PIN_SENSOR_3, PIN_SENSOR_4 };

// ============================================================================
// STATUSTELLERS EN VARIABELEN
// ============================================================================

int stimulusVersie = STIMULUS_BASIC;

unsigned long nulmetingTikTijd  = 0;
int nulmetingTikKracht          = 0;

int offsetSensor1               = 0;
int offsetSensor2               = 0;
int offsetSensor3               = 0;
int offsetSensor4               = 0;
int offsetSensorActief          = 0; 

// Deze globale uitvoerwaarden zijn alleen geldig direct na de laatste aanroep van VergelijkStimulus().
bool TijdCorrect                = false;
bool KrachtCorrect              = false;

// MARGE_FACTOR t.o.v. de nulmeting
int MARGE_FACTOR                = DEFAULT_MARGE_FACTOR;

int TIK_TEST_ACTIEVE_VINGER     = -1;

int TELLER_TIKTIJD_CORRECT      = 0;
int TELLER_TIKTIJD_TE_SNEL      = 0;
int TELLER_TIKTIJD_TE_TRAAG     = 0;
int TELLER_TIKTIJD_SYNCHROON    = 0;

int TELLER_TIKKRACHT_CORRECT    = 0;
int TELLER_TIKKRACHT_TE_ZACHT   = 0;
int TELLER_TIKKRACHT_TE_HARD    = 0;
int TELLER_TIKKRACHT_IN_BALANS  = 0;

int TELLER_INSTORTEND_CORRECT   = 0;

int TELLER_SIMULTANE_START_OK   = 0;
int TELLER_SIMULTANE_EIND_OK    = 0;

int TOEGESTANE_MARGE_TIKTIJD    = DEFAULT_TOEGESTANE_MARGE_TIKTIJD;
int TOEGESTANE_MARGE_TIKKRACHT  = DEFAULT_TOEGESTANE_MARGE_TIKKRACHT;

unsigned long TOEGESTANE_MARGE_SIMULTANE_STARTTIJD_MS = DEFAULT_TOEGESTANE_MARGE_SIMULTANE_STARTTIJD_MS;

// ============================================================================
// STIMULUS
// ============================================================================

int AnalogReadMetGekorigeerdeOffsets(int sensorPin, int offsetSensor) {
#ifdef TRACE
  int raw = RawAnalogRead(sensorPin);
  int waarde = raw - offsetSensor;

  if (waarde < 0) {
    DEBUG_PRINT("Pin=");
    DEBUG_PRINT(sensorPin);
    DEBUG_PRINT(" raw=");
    DEBUG_PRINT(raw);
    DEBUG_PRINT(" offset=");
    DEBUG_PRINT(offsetSensor);
    DEBUG_PRINT(" waarde=");
    DEBUG_PRINTLN(waarde);
  }  
#else
  int waarde = RawAnalogRead(sensorPin) - offsetSensor;
#endif

  if (waarde < 0) waarde = 0;
  return waarde;
}

static int BepaalAantalSensorenSynchroon(unsigned long tijden[], int aantalSensoren, unsigned long toegestaneMarge) {
  int grootsteAantalSensorenSynchroon = 0;

  for (int eersteSensor = 0; eersteSensor < aantalSensoren; eersteSensor++) {
    unsigned long vroegsteTijd = tijden[eersteSensor];
    unsigned long laatsteTijd  = tijden[eersteSensor];
    int aantalSensorenBinnenMarge = 0;

    for (int sensorNummer = 0; sensorNummer < aantalSensoren; sensorNummer++) {
      unsigned long verschilTijd = (tijden[sensorNummer] > tijden[eersteSensor]) ? tijden[sensorNummer] - tijden[eersteSensor] : tijden[eersteSensor] - tijden[sensorNummer];

      if (verschilTijd <= toegestaneMarge) {
        aantalSensorenBinnenMarge++;
        if (tijden[sensorNummer] < vroegsteTijd) vroegsteTijd = tijden[sensorNummer];
        if (tijden[sensorNummer] > laatsteTijd) laatsteTijd = tijden[sensorNummer];
      }
    }

    if ((laatsteTijd - vroegsteTijd) <= toegestaneMarge && aantalSensorenBinnenMarge > grootsteAantalSensorenSynchroon) grootsteAantalSensorenSynchroon = aantalSensorenBinnenMarge;
  }

  if (grootsteAantalSensorenSynchroon < 2) grootsteAantalSensorenSynchroon = 0;
  return grootsteAantalSensorenSynchroon;
}

void BepaalSensorOffsets() {
  int aantalMetingen = 0;
  int hoogsteMeting[4] = { 0, 0, 0, 0 };
  unsigned long startTijd = millis();

#ifdef TRACE
   DEBUG_PRINTLN("---------------------");
#endif

  while (millis() - startTijd < OFFSET_METING_TIJD_MS) {
    for (int sensorNummer = 0; sensorNummer < AANTAL_SENSOREN_AANWEZIG; sensorNummer++) {
      int meting = RawAnalogRead(sensorPin[sensorNummer]);
      if (aantalMetingen > 1 && meting > hoogsteMeting[sensorNummer]) { hoogsteMeting[sensorNummer] = meting; }

#ifdef TRACE
      DEBUG_PRINT("Meting sensor ");
      DEBUG_PRINT(sensorNummer + 1);
      DEBUG_PRINT(" = ");
      DEBUG_PRINTLN(meting);
#endif
    }

    aantalMetingen++;
    delay(5);
  }

  offsetSensor1 = hoogsteMeting[0] + OFFSET_VEILIGHEIDSMARGE;
  offsetSensor2 = hoogsteMeting[1] + OFFSET_VEILIGHEIDSMARGE;

  if (AANTAL_SENSOREN_AANWEZIG == 4) {
    offsetSensor3 = hoogsteMeting[2] + OFFSET_VEILIGHEIDSMARGE;
    offsetSensor4 = hoogsteMeting[3] + OFFSET_VEILIGHEIDSMARGE;
  }

#ifdef DEBUG
  DEBUG_PRINTLN("---------------------");

  DEBUG_PRINT("Offset sensor 1 = ");
  DEBUG_PRINTLN(offsetSensor1);

  DEBUG_PRINT("Offset sensor 2 = ");
  DEBUG_PRINTLN(offsetSensor2);

  if (AANTAL_SENSOREN_AANWEZIG == 4) {
    DEBUG_PRINT("Offset sensor 3 = ");
    DEBUG_PRINTLN(offsetSensor3);

    DEBUG_PRINT("Offset sensor 4 = ");
    DEBUG_PRINTLN(offsetSensor4);
  }
#endif
}

static void BerekenEindStimulus(SensorMeetStatus &sensor, StimulusProfiel &gemetenStimulus) {
  // TIKTIJD
  if (sensor.sensorGestart && sensor.eindTikTijd > sensor.startTikTijd) gemetenStimulus.TikTijd = sensor.eindTikTijd - sensor.startTikTijd;

  // TIKKRACHT
  if (sensor.aantalTikKrachtMetingen > 0) gemetenStimulus.gemiddeldeTikKracht = sensor.somVanTikKrachtMetingen / sensor.aantalTikKrachtMetingen;

  // PIEKKRACHT
  gemetenStimulus.hoogsteTikKracht = sensor.hoogsteTikKrachtMeting;

  // TIJD TOT PIEKKRACHT
  if (sensor.sensorGestart) gemetenStimulus.tijdTotPiekTikKracht = sensor.tijdHoogsteTikKracht - sensor.startTikTijd;

  // OPBOUWSNELHEID
  if (gemetenStimulus.tijdTotPiekTikKracht > 0) {
    gemetenStimulus.opbouwSnelheid = (gemetenStimulus.hoogsteTikKracht - sensor.eersteTikKrachtMeting) / gemetenStimulus.tijdTotPiekTikKracht;
  } else {
    gemetenStimulus.opbouwSnelheid = 0;
  }

  // AFBOUWSNELHEID
  unsigned long afbouwTijd = 0;

  if (gemetenStimulus.TikTijd > gemetenStimulus.tijdTotPiekTikKracht) afbouwTijd = gemetenStimulus.TikTijd - gemetenStimulus.tijdTotPiekTikKracht;

  if (afbouwTijd > 0) {
    gemetenStimulus.afbouwSnelheid = (gemetenStimulus.hoogsteTikKracht - sensor.laatsteTikKrachtMeting) / afbouwTijd;
  } else {
    gemetenStimulus.afbouwSnelheid = 0;
  }
}

int EvalueerNulmeting(unsigned long gemetenTikTijd, int gemetenGemiddeldeTikKracht,
                      bool &nulmetingGoedgekeurd, unsigned long &nulmetingTikTijd, int &nulmetingTikKracht,
                      int &herhaling, int &aantalNulmetingPogingen) {
  if (!nulmetingGoedgekeurd) {  // Als de nulmeting nog NIET is goedgekeurd (false)
    int aantalChecksGeslaagd  = 0;

    if (gemetenGemiddeldeTikKracht < TIKKRACHT_MINIMALE_COMFORT_GRENS) {
      PrintToScreen(_LCD_SCORE_TIKKRACHT, _LCD_KRACHT_TE_ZACHT, _LCD_LEESTIJD_FEEDBACK_MS);
    } else if (gemetenGemiddeldeTikKracht > TIKKRACHT_MAXIMALE_COMFORT_GRENS) {
      PrintToScreen(_LCD_SCORE_TIKKRACHT, _LCD_KRACHT_TE_HARD, _LCD_LEESTIJD_FEEDBACK_MS);
    } else {
      ++aantalChecksGeslaagd ;
    }

    if (gemetenTikTijd > MAXIMALE_TIKTIJD_MS) {
      PrintToScreen(_LCD_SCORE_TIKTIJD, _LCD_TIJD_TE_LANG, _LCD_LEESTIJD_FEEDBACK_MS);
    } else if (gemetenTikTijd < MINIMALE_TIKTIJD_MS) {
      PrintToScreen(_LCD_SCORE_TIKTIJD, _LCD_TIJD_TE_KORT, _LCD_LEESTIJD_FEEDBACK_MS);
    } else {
      ++aantalChecksGeslaagd;
    }

    if (aantalChecksGeslaagd  == 2) {
      herhaling--;  // Zorgt ervoor dat de succesvolle nulmeting NIET meetelt als TIK
      nulmetingGoedgekeurd = true;
      nulmetingTikTijd     = gemetenTikTijd;
      nulmetingTikKracht   = gemetenGemiddeldeTikKracht;
      return 1;     // Aangepast: 1 betekent OK
    } else {
      aantalNulmetingPogingen++;  // Hoogt de teller op met 1 stap

      if (aantalNulmetingPogingen >= MAX_AANTAL_POGINGEN_NULMETING) {  // GRENS BEREIKT: Stop de oneindige loop direct
        PrintToScreen(_LCD_TIJD_METEN_STOPT, _LCD_TIJD_TEVEEL_FOUT, _LCD_LEESTIJD_FEEDBACK_MS);
        return -1;  // Aangepast: -1 betekent stoppen
      } else {      // NOG POGINGEN OVER: Reset deze ronde zodat hij opnieuw mag proberen
        herhaling--;
        return 0;   // Aangepast: 0 betekent fout, maar opnieuw proberen
      }
    }
  }

  return 0;  // Fallback
}

static void InitialiseerSensorStart(unsigned long nu, SensorMeetStatus &sensor) {
  sensor.sensorGestart             = true;

  sensor.startTikTijd              = nu;
  sensor.tijdHoogsteTikKracht      = nu;

  sensor.eersteTikKrachtMeting     = sensor.actueleTikKracht;
  sensor.laatsteTikKrachtMeting    = sensor.actueleTikKracht;
  sensor.hoogsteTikKrachtMeting    = sensor.actueleTikKracht;
  sensor.somVanTikKrachtMetingen   = sensor.actueleTikKracht;
  sensor.aantalTikKrachtMetingen   = 1;
}

static int MaakSensorMask(SensorMeetStatus sensor[], int aantalSensoren, bool testOpDRUKWAARDE) {
  int sensorMask = 0b0000;

  for (int sensorNummer = 0; sensorNummer < aantalSensoren; sensorNummer++) {
    if (testOpDRUKWAARDE) {
      if (sensor[sensorNummer].actueleTikKracht > TIK_MINIMALE_DRUKWAARDE) { sensorMask |= (0b1000 >> sensorNummer); }
    } else {
      if (sensor[sensorNummer].sensorGestart && !sensor[sensorNummer].sensorKlaar) { sensorMask |= (0b1000 >> sensorNummer); }
    }
  }

  return sensorMask;
}

SynchronisatieProfiel MaakSynchronisatieProfiel(SensorMeetStatus sensor[], int sensorA, int sensorB, StimulusProfiel gemetenStimulus[]) {
  SynchronisatieProfiel synchronisatie;
  ResetSynchronisatieProfiel(synchronisatie);

  synchronisatie.verschilStartTijd = (sensor[sensorA].startTikTijd > sensor[sensorB].startTikTijd) ? sensor[sensorA].startTikTijd - sensor[sensorB].startTikTijd : sensor[sensorB].startTikTijd - sensor[sensorA].startTikTijd;
  synchronisatie.verschilTikTijd   = (gemetenStimulus[sensorA].TikTijd > gemetenStimulus[sensorB].TikTijd) ? gemetenStimulus[sensorA].TikTijd - gemetenStimulus[sensorB].TikTijd : gemetenStimulus[sensorB].TikTijd - gemetenStimulus[sensorA].TikTijd;

  synchronisatie.verschilGemiddeldeTikKracht = abs(gemetenStimulus[sensorA].gemiddeldeTikKracht - gemetenStimulus[sensorB].gemiddeldeTikKracht);
  synchronisatie.verschilHoogsteTikKracht    = abs(gemetenStimulus[sensorA].hoogsteTikKracht - gemetenStimulus[sensorB].hoogsteTikKracht);

  return synchronisatie;
}

static SynchronisatieProfiel MaakSynchronisatieProfielAlleSensoren(SensorMeetStatus sensor[], int aantalSensorenSimultaanTeMeten, StimulusProfiel gemetenStimulus[]) {
  SynchronisatieProfiel synchronisatie;
  ResetSynchronisatieProfiel(synchronisatie);

  unsigned long startTijden[4] = { 0, 0, 0, 0 };
  unsigned long eindTijden[4] = { 0, 0, 0, 0 };

  unsigned long eersteStartTijd  = sensor[0].startTikTijd;
  unsigned long laatsteStartTijd = sensor[0].startTikTijd;

  unsigned long eersteEindTijd   = sensor[0].eindTikTijd;
  unsigned long laatsteEindTijd  = sensor[0].eindTikTijd;

  unsigned long kortsteTikTijd   = gemetenStimulus[0].TikTijd;
  unsigned long langsteTikTijd   = gemetenStimulus[0].TikTijd;

  int laagsteGemiddeldeTikKracht = gemetenStimulus[0].gemiddeldeTikKracht;
  int laagsteHoogsteTikKracht    = gemetenStimulus[0].hoogsteTikKracht;

  int hoogsteGemiddeldeTikKracht = gemetenStimulus[0].gemiddeldeTikKracht;
  int hoogsteHoogsteTikKracht    = gemetenStimulus[0].hoogsteTikKracht;

  unsigned long totaleTikTijd = 0;

  for (int sensorNummer = 0; sensorNummer < aantalSensorenSimultaanTeMeten; sensorNummer++) {
    startTijden[sensorNummer] = sensor[sensorNummer].startTikTijd;
    eindTijden[sensorNummer] = sensor[sensorNummer].eindTikTijd;
    totaleTikTijd += gemetenStimulus[sensorNummer].TikTijd;

    if (sensorNummer > 0) {
      if (sensor[sensorNummer].startTikTijd < eersteStartTijd)  eersteStartTijd = sensor[sensorNummer].startTikTijd;
      if (sensor[sensorNummer].startTikTijd > laatsteStartTijd) laatsteStartTijd = sensor[sensorNummer].startTikTijd;

      if (sensor[sensorNummer].eindTikTijd < eersteEindTijd)  eersteEindTijd = sensor[sensorNummer].eindTikTijd;
      if (sensor[sensorNummer].eindTikTijd > laatsteEindTijd) laatsteEindTijd = sensor[sensorNummer].eindTikTijd;

      if (gemetenStimulus[sensorNummer].TikTijd < kortsteTikTijd) kortsteTikTijd = gemetenStimulus[sensorNummer].TikTijd;
      if (gemetenStimulus[sensorNummer].TikTijd > langsteTikTijd) langsteTikTijd = gemetenStimulus[sensorNummer].TikTijd;

      if (gemetenStimulus[sensorNummer].gemiddeldeTikKracht < laagsteGemiddeldeTikKracht) laagsteGemiddeldeTikKracht = gemetenStimulus[sensorNummer].gemiddeldeTikKracht;
      if (gemetenStimulus[sensorNummer].gemiddeldeTikKracht > hoogsteGemiddeldeTikKracht) hoogsteGemiddeldeTikKracht = gemetenStimulus[sensorNummer].gemiddeldeTikKracht;

      if (gemetenStimulus[sensorNummer].hoogsteTikKracht < laagsteHoogsteTikKracht) laagsteHoogsteTikKracht = gemetenStimulus[sensorNummer].hoogsteTikKracht;
      if (gemetenStimulus[sensorNummer].hoogsteTikKracht > hoogsteHoogsteTikKracht) hoogsteHoogsteTikKracht = gemetenStimulus[sensorNummer].hoogsteTikKracht;
    }
  }

  synchronisatie.verschilStartTijd = laatsteStartTijd - eersteStartTijd;
  synchronisatie.verschilEindTijd  = laatsteEindTijd - eersteEindTijd;
  synchronisatie.verschilTikTijd   = langsteTikTijd - kortsteTikTijd;

  synchronisatie.verschilGemiddeldeTikKracht = hoogsteGemiddeldeTikKracht - laagsteGemiddeldeTikKracht;
  synchronisatie.verschilHoogsteTikKracht    = hoogsteHoogsteTikKracht - laagsteHoogsteTikKracht;

  unsigned long gemiddeldeTikTijd = totaleTikTijd / aantalSensorenSimultaanTeMeten;
  unsigned long toegestaneMargeEindTijd = (gemiddeldeTikTijd * TOEGESTANE_MARGE_TIKTIJD) / (MARGE_FACTOR * 100UL);

  synchronisatie.aantalSensorenSynchroonStart = BepaalAantalSensorenSynchroon(startTijden, aantalSensorenSimultaanTeMeten, TOEGESTANE_MARGE_SIMULTANE_STARTTIJD_MS);
  synchronisatie.aantalSensorenSynchroonEinde = BepaalAantalSensorenSynchroon(eindTijden, aantalSensorenSimultaanTeMeten, toegestaneMargeEindTijd);

  return synchronisatie;
}

int MeetStimulus(int sensorPin, int OffsetSensor, StimulusProfiel &gemetenStimulus, int exitPin, int exitOffset, unsigned long timeoutMs) {
  int exitStatus = EXIT_STATUS_GEEN;
  SensorMeetStatus sensor = {};
  ResetStimulusProfiel(gemetenStimulus);

  // Eerste meting weggooien: kanaalwissel-artefact ligt structureel boven drempel
  RawAnalogRead(sensorPin);

  // Wanneer de meetsensor bij het binnenkomen al ingedrukt is, eerst wachten tot die volledig wordt losgelaten.
  while (AnalogReadMetGekorigeerdeOffsets(sensorPin, OffsetSensor) > TIK_MINIMALE_DRUKWAARDE);

  // Daarna pas wachten op een nieuwe geldige druk.
  unsigned long startWachtenOpActie = millis();
  unsigned long timeoutNoActionMs = (timeoutMs > EXIT_NO_ACTION_MS) ? timeoutMs : EXIT_NO_ACTION_MS;

  sensor.actueleTikKracht = 0;

  while (sensor.actueleTikKracht <= TIK_MINIMALE_DRUKWAARDE) {
    sensor.actueleTikKracht = AnalogReadMetGekorigeerdeOffsets(sensorPin, OffsetSensor);
    if (millis() - startWachtenOpActie >= timeoutNoActionMs) return EXIT_STATUS_NO_ACTION_TIMEOUT;
  }

  // Gebruik dezelfde initialisatie van SensorMeetStatus als MeetStimulusSimultaan().
  unsigned long nu = millis();
  InitialiseerSensorStart(nu, sensor);

  while (true) {
    sensor.actueleTikKracht = AnalogReadMetGekorigeerdeOffsets(sensorPin, OffsetSensor);
    nu = millis();

    // Normale stopvoorwaarde: sensor losgelaten
    if (sensor.actueleTikKracht <= TIK_MINIMALE_DRUKWAARDE) {
#ifdef DEBUG
      DEBUG_PRINT("actueleTikKracht bij loslaten = ");
      DEBUG_PRINTLN(sensor.actueleTikKracht);
#endif
      VerwerkSensor(nu, sensorPin, OffsetSensor, sensor);
      exitStatus = EXIT_STATUS_SENSOR_LOSGELATEN;
      break;
    }

    // Extra stopvoorwaarde: exitPin aangeraakt
    if (exitPin != -1 && AnalogReadMetGekorigeerdeOffsets(exitPin, exitOffset) > TIK_MINIMALE_DRUKWAARDE) {
      sensor.eindTikTijd = nu;
      sensor.sensorKlaar = true;
      exitStatus = EXIT_STATUS_EXITSENSOR_INGEDRUKT;
      break;
    }

    // Extra stopvoorwaarde: maximale meettijd bereikt
    if (nu - sensor.startTikTijd >= timeoutMs) {
      sensor.eindTikTijd = nu;
      sensor.sensorKlaar = true;
      exitStatus = EXIT_STATUS_TIMEOUT;
      break;
    }

    // Gebruik dezelfde verwerking van krachtmetingen als MeetStimulusSimultaan().
    VerwerkSensor(nu, sensorPin, OffsetSensor, sensor);
    delayMicroseconds(DELAY_US);
  }

  // Bij normaal loslaten is de eindtijd reeds door VerwerkSensor() vastgelegd. Bij exitPin en timeout wordt ze in de stopvoorwaarde vastgelegd.
  if (sensor.eindTikTijd == 0) sensor.eindTikTijd = millis();

  // Gebruik dezelfde eindberekening van het StimulusProfiel als MeetStimulusSimultaan().
  BerekenEindStimulus(sensor, gemetenStimulus);

#ifdef DEBUG
  DEBUG_PRINTLN("---------------------");

  DEBUG_PRINT("aantalTikKrachtMetingen = ");
  DEBUG_PRINTLN(sensor.aantalTikKrachtMetingen);

  DEBUG_PRINT("somVanTikKrachtMetingen = ");
  DEBUG_PRINTLN(sensor.somVanTikKrachtMetingen);

  DEBUG_PRINT("startTikTijd = ");
  DEBUG_PRINTLN(sensor.startTikTijd);

  DEBUG_PRINT("eindTikTijd = ");
  DEBUG_PRINTLN(sensor.eindTikTijd);

  DEBUG_PRINT("TikTijd = ");
  DEBUG_PRINTLN(gemetenStimulus.TikTijd);

  DEBUG_PRINT("gemiddeldeTikKracht = ");
  DEBUG_PRINTLN(gemetenStimulus.gemiddeldeTikKracht);

  DEBUG_PRINT("piekTikKracht = ");
  DEBUG_PRINTLN(gemetenStimulus.hoogsteTikKracht);

  DEBUG_PRINT("tijdTotPiekTikKracht = ");
  DEBUG_PRINTLN(gemetenStimulus.tijdTotPiekTikKracht);

  DEBUG_PRINT("opbouwSnelheid = ");
  DEBUG_PRINTLN(gemetenStimulus.opbouwSnelheid);

  DEBUG_PRINT("afbouwSnelheid = ");
  DEBUG_PRINTLN(gemetenStimulus.afbouwSnelheid);

  DEBUG_PRINT("exitStatus = ");
  DEBUG_PRINTLN(exitStatus);
#endif

  return exitStatus;
}

int MeetStimulusSimultaan(StimulusProfiel gemetenStimulus[], int aantalSensorenSimultaanTeMeten, SynchronisatieProfiel synchronisatie[], int MaskReedsActieveSensorsBijStart, int MaskGewensteActieveSensorsBijExit, unsigned long timeoutMs, bool testOpDRUKWAARDE) {
  const int offsetSensor[4] = { offsetSensor1, offsetSensor2, offsetSensor3, offsetSensor4 };
  SensorMeetStatus sensor[4] = {};

  int exitStatus = EXIT_STATUS_GEEN;
  if (aantalSensorenSimultaanTeMeten < 2 || aantalSensorenSimultaanTeMeten > AANTAL_SENSOREN_AANWEZIG || aantalSensorenSimultaanTeMeten > 4) return EXIT_STATUS_AANTAL_SENSOREN_ONGELDIG;

  // Bepaal welke maskerposities geldig zijn voor het opgegeven aantal sensoren.
  int geldigeSensorMask = 0b0000;
  for (int sensorNummer = 0; sensorNummer < aantalSensorenSimultaanTeMeten; sensorNummer++) { geldigeSensorMask |= (0b1000 >> sensorNummer); }

  // EXIT_STATUS_STARTMASK_ONGELDIG: het opgegeven startmask bevat een positie die voor deze meting niet geldig is.
  if (MaskReedsActieveSensorsBijStart < 0 || MaskReedsActieveSensorsBijStart > 0b1111 || (MaskReedsActieveSensorsBijStart & ~geldigeSensorMask) != 0) { return EXIT_STATUS_STARTMASK_ONGELDIG; }

  // Het opgegeven exitmask bevat een positie die voor deze meting niet geldig is.
  if (MaskGewensteActieveSensorsBijExit < 0 || MaskGewensteActieveSensorsBijExit > 0b1111 || (MaskGewensteActieveSensorsBijExit & ~geldigeSensorMask) != 0) { return EXIT_STATUS_EXITMASK_ONGELDIG; }

  for (int sensorNummer = 0; sensorNummer < aantalSensorenSimultaanTeMeten; sensorNummer++) ResetStimulusProfiel(gemetenStimulus[sensorNummer]);

  ResetSynchronisatieProfiel(synchronisatie[0]);

  if (aantalSensorenSimultaanTeMeten == 4) {
    ResetSynchronisatieProfiel(synchronisatie[1]);
    ResetSynchronisatieProfiel(synchronisatie[2]);
  }

  // Wacht tot minstens één van de sensoren boven de drempel komt.
  bool minstensEenSensorGestart = false;

  // Controleer of het huidige sensormask overeenkomt met het opgegeven startmask. Indien geen startmask werd opgegeven, wacht tot minstens één sensor start.
  // Het mask loopt van links naar rechts: 0b1000 = sensor 1, 0b0100 = sensor 2, 0b0010 = sensor 3, 0b0001 = sensor 4.
  // MaskReedsActieveSensorsBijStart geeft exact aan welke sensoren al actief zijn bij aanvang van de meting. vb: 0b1100 = sensor 1 en sensor 2 zijn al actief bij aanvang.
  if (MaskReedsActieveSensorsBijStart != 0) {
    for (int sensorNummer = 0; sensorNummer < aantalSensorenSimultaanTeMeten; sensorNummer++) {
      sensor[sensorNummer].actueleTikKracht = AnalogReadMetGekorigeerdeOffsets(sensorPin[sensorNummer], offsetSensor[sensorNummer]);
    }

    // controle vóór initialisatie
    if (testOpDRUKWAARDE) {
      int sensorMask = MaakSensorMask(sensor, aantalSensorenSimultaanTeMeten, testOpDRUKWAARDE);

      // EXIT_STATUS_VERKEERDE_STARTTOESTAND: het opgegeven startmask is geldig, maar de werkelijke sensortoestand is niet gelijk aan het startmask.
      if (sensorMask != MaskReedsActieveSensorsBijStart) return EXIT_STATUS_VERKEERDE_STARTTOESTAND;
    }

    unsigned long nu = millis();

    for (int sensorNummer = 0; sensorNummer < aantalSensorenSimultaanTeMeten; sensorNummer++) {
      if (sensor[sensorNummer].actueleTikKracht > TIK_MINIMALE_DRUKWAARDE) {
        InitialiseerSensorStart(nu, sensor[sensorNummer]);
        minstensEenSensorGestart = true;
      }
    }

    // controle na initialisatie
    if (!testOpDRUKWAARDE) {
      int sensorMask = MaakSensorMask(sensor, aantalSensorenSimultaanTeMeten, testOpDRUKWAARDE);

      // EXIT_STATUS_VERKEERDE_STARTTOESTAND: het opgegeven startmask is geldig, maar de werkelijke sensortoestand is niet gelijk aan het startmask.
      if (sensorMask != MaskReedsActieveSensorsBijStart) return EXIT_STATUS_VERKEERDE_STARTTOESTAND;
    }
  } else {
    unsigned long startWachtenOpActie = millis();
    unsigned long timeoutNoActionMs = (timeoutMs > EXIT_NO_ACTION_MS) ? timeoutMs : EXIT_NO_ACTION_MS;

    while (!minstensEenSensorGestart) {
      if (millis() - startWachtenOpActie >= timeoutNoActionMs) return EXIT_STATUS_NO_ACTION_TIMEOUT;

      for (int sensorNummer = 0; sensorNummer < aantalSensorenSimultaanTeMeten; sensorNummer++) {
        sensor[sensorNummer].actueleTikKracht = AnalogReadMetGekorigeerdeOffsets(sensorPin[sensorNummer], offsetSensor[sensorNummer]);
      }

      unsigned long nu = millis();

      for (int sensorNummer = 0; sensorNummer < aantalSensorenSimultaanTeMeten; sensorNummer++) {
        if (sensor[sensorNummer].actueleTikKracht > TIK_MINIMALE_DRUKWAARDE) InitialiseerSensorStart(nu, sensor[sensorNummer]);
        if (sensor[sensorNummer].sensorGestart) minstensEenSensorGestart = true;
      }
    }
  }

  unsigned long startMeting = 0;

  for (int sensorNummer = 0; sensorNummer < aantalSensorenSimultaanTeMeten; sensorNummer++) {
    if (sensor[sensorNummer].sensorGestart && (startMeting == 0 || sensor[sensorNummer].startTikTijd < startMeting)) {
      startMeting = sensor[sensorNummer].startTikTijd;
    }
  }

  // Meet vanaf het opgegeven startmask tot het gewenste exitmask bereikt is,of zonder exitmask tot alle sensoren klaar zijn, of tot een timeout optreedt.
  bool alleSensorenKlaar = false;
  int vorigSensorMask = -1;

  while (!alleSensorenKlaar) {
    unsigned long nu = millis();

    // Eerst alle actuele sensorwaarden opnieuw lezen.
    for (int sensorNummer = 0; sensorNummer < aantalSensorenSimultaanTeMeten; sensorNummer++) {
      sensor[sensorNummer].actueleTikKracht = AnalogReadMetGekorigeerdeOffsets(sensorPin[sensorNummer], offsetSensor[sensorNummer]);
    }

    // for (int sensorNummer = 0; sensorNummer < aantalSensorenSimultaanTeMeten; sensorNummer++) VerwerkSensor(nu, sensorPin[sensorNummer], offsetSensor[sensorNummer], sensor[sensorNummer]);

    if (MaskGewensteActieveSensorsBijExit != 0) { 
      // Maak het mask rechtstreeks op basis van de zojuist gelezen actuele drukwaarden.
      int sensorMask = MaakSensorMask(sensor, aantalSensorenSimultaanTeMeten, true);

#ifdef DEBUG
      if (sensorMask != vorigSensorMask) {
        vorigSensorMask = sensorMask;

        DEBUG_PRINTLN("---------------------");

        DEBUG_PRINT("MaskReedsActieveSensorsBijStart = ");
        DEBUG_PRINTLN2(MaskReedsActieveSensorsBijStart, BIN);

        DEBUG_PRINT("sensorMask = ");
        DEBUG_PRINTLN2(sensorMask, BIN);

        DEBUG_PRINT("MaskGewensteActieveSensorsBijExit = ");
        DEBUG_PRINTLN2(MaskGewensteActieveSensorsBijExit, BIN);

        for (int sensorNummer = 0; sensorNummer < aantalSensorenSimultaanTeMeten; sensorNummer++) {
          DEBUG_PRINT("Sensor ");
          DEBUG_PRINT(sensorNummer + 1);
          DEBUG_PRINT(" actueleTikKracht = ");
          DEBUG_PRINTLN(sensor[sensorNummer].actueleTikKracht);
        }
      }
#endif

      // Alleen stoppen wanneer het actuele mask 100 procent gelijk is aan het opgegeven exitmask.
      if (sensorMask == MaskGewensteActieveSensorsBijExit) {
        int ingedrukteSensorsMask = sensorMask & ~MaskReedsActieveSensorsBijStart;
        int losgelatenSensorsMask = MaskReedsActieveSensorsBijStart & ~sensorMask;

        if (ingedrukteSensorsMask != 0 && losgelatenSensorsMask == 0) {
          exitStatus = EXIT_STATUS_SENSOR_INGEDRUKT;  // Alleen één of meerdere sensoren werden ingedrukt.
        } else if (ingedrukteSensorsMask == 0 && losgelatenSensorsMask != 0) { 
          exitStatus = EXIT_STATUS_SENSOR_LOSGELATEN; // Alleen één of meerdere sensoren werden losgelaten.
        } else {
          exitStatus = EXIT_STATUS_MASK_BEREIKT;      // Er werden tegelijk sensoren ingedrukt en losgelaten.
        }

        for (int sensorNummer = 0; sensorNummer < aantalSensorenSimultaanTeMeten; sensorNummer++) {
          if (sensor[sensorNummer].sensorGestart && !sensor[sensorNummer].sensorKlaar) {
            sensor[sensorNummer].eindTikTijd = nu;
            sensor[sensorNummer].sensorKlaar = true;
          }
        }

        alleSensorenKlaar = true;
        break;
      }
    }

    // Verwerk dezelfde sensorwaarden die hierboven voor het mask gebruikt werden.
    for (int sensorNummer = 0; sensorNummer < aantalSensorenSimultaanTeMeten; sensorNummer++) VerwerkSensor(nu, sensorPin[sensorNummer], offsetSensor[sensorNummer], sensor[sensorNummer]);

    // Stop wanneer de ingestelde timeout bereikt is.
    if (nu - startMeting >= timeoutMs) {
      exitStatus = EXIT_STATUS_TIMEOUT;

      for (int sensorNummer = 0; sensorNummer < aantalSensorenSimultaanTeMeten; sensorNummer++) {
        if (sensor[sensorNummer].sensorGestart && !sensor[sensorNummer].sensorKlaar) {
          sensor[sensorNummer].eindTikTijd = nu;
          sensor[sensorNummer].sensorKlaar = true;
        } else if (!sensor[sensorNummer].sensorGestart) {
          sensor[sensorNummer].sensorKlaar = true;
        }
      }

      alleSensorenKlaar = true;
      break;
    }

    // Wanneer minstens één sensor gestart is en een andere sensor niet binnen EXIT_TIKTIJD_MS start,
    // wordt de niet gestarte sensor als klaar gemarkeerd.
    for (int sensorNummer = 0; sensorNummer < aantalSensorenSimultaanTeMeten; sensorNummer++) {
      if (!sensor[sensorNummer].sensorGestart && !sensor[sensorNummer].sensorKlaar) {
        for (int andereSensorNummer = 0; andereSensorNummer < aantalSensorenSimultaanTeMeten; andereSensorNummer++) {
          if (sensor[andereSensorNummer].sensorGestart && nu - sensor[andereSensorNummer].startTikTijd > EXIT_TIKTIJD_MS) sensor[sensorNummer].sensorKlaar = true;
        }
      }
    }

    // Wanneer een gestarte sensor niet meer wordt losgelaten, wordt de meting na EXIT_TIKTIJD_MS beëindigd.
    for (int sensorNummer = 0; sensorNummer < aantalSensorenSimultaanTeMeten; sensorNummer++) {
      if (sensor[sensorNummer].sensorGestart && !sensor[sensorNummer].sensorKlaar && nu - sensor[sensorNummer].startTikTijd > EXIT_TIKTIJD_MS) {
        sensor[sensorNummer].eindTikTijd = nu;
        sensor[sensorNummer].sensorKlaar = true;
      }
    }

    // Zonder exitmask mag de oorspronkelijke werking stoppen wanneer alle sensoren klaar zijn.
    // Met exitmask mag "alle sensoren klaar" het exacte exitmask niet omzeilen.
    if (MaskGewensteActieveSensorsBijExit == 0) {
      alleSensorenKlaar = true;

      for (int sensorNummer = 0; sensorNummer < aantalSensorenSimultaanTeMeten; sensorNummer++) {
        if (!sensor[sensorNummer].sensorKlaar) alleSensorenKlaar = false;
      }
    } else {
      alleSensorenKlaar = false;
    }

    delayMicroseconds(DELAY_US);
  }

  // Wanneer de meetlus zonder apart exitmask en zonder aparte timeout normaal eindigt.
  if (exitStatus == EXIT_STATUS_GEEN) exitStatus = EXIT_STATUS_SENSOR_LOSGELATEN;

  for (int sensorNummer = 0; sensorNummer < aantalSensorenSimultaanTeMeten; sensorNummer++) BerekenEindStimulus(sensor[sensorNummer], gemetenStimulus[sensorNummer]);

  synchronisatie[0] = MaakSynchronisatieProfielAlleSensoren(sensor, aantalSensorenSimultaanTeMeten, gemetenStimulus);

#ifdef DEBUG
  DEBUG_PRINTLN("=== SIMULTANE METING ===");

  for (int sensorNummer = 0; sensorNummer < aantalSensorenSimultaanTeMeten; sensorNummer++) {
    DEBUG_PRINT("Sensor ");
    DEBUG_PRINT(sensorNummer + 1);
    DEBUG_PRINT(" gestart = ");
    DEBUG_PRINTLN(sensor[sensorNummer].sensorGestart);

    DEBUG_PRINT(sensorNummer + 1);
    DEBUG_PRINT(" TikTijd = ");
    DEBUG_PRINTLN(gemetenStimulus[sensorNummer].TikTijd);

    DEBUG_PRINT(sensorNummer + 1);
    DEBUG_PRINT(" gemiddeldeTikKracht = ");
    DEBUG_PRINTLN(gemetenStimulus[sensorNummer].gemiddeldeTikKracht);
  }

  DEBUG_PRINT("Synchronisatie 01 verschilStartTijd = ");
  DEBUG_PRINTLN(synchronisatie[0].verschilStartTijd);

  DEBUG_PRINT("Synchronisatie 01 verschilEindTijd = ");
  DEBUG_PRINTLN(synchronisatie[0].verschilEindTijd);

  if (aantalSensorenSimultaanTeMeten == 4) {
    DEBUG_PRINT("Synchronisatie 02 verschilStartTijd = ");
    DEBUG_PRINTLN(synchronisatie[1].verschilStartTijd);

    DEBUG_PRINT("Synchronisatie 03 verschilStartTijd = ");
    DEBUG_PRINTLN(synchronisatie[2].verschilStartTijd);
  }

  DEBUG_PRINT("exitStatus = ");
  DEBUG_PRINTLN(exitStatus);
#endif

  return exitStatus;
}

void ResetAlleTellers() {
  TIK_TEST_ACTIEVE_VINGER = -1;
  offsetSensorActief = 0;

  TELLER_TIKTIJD_CORRECT     = 0;
  TELLER_TIKTIJD_TE_SNEL     = 0;
  TELLER_TIKTIJD_TE_TRAAG    = 0;
  TELLER_TIKTIJD_SYNCHROON   = 0;

  TELLER_TIKKRACHT_CORRECT   = 0;
  TELLER_TIKKRACHT_TE_ZACHT  = 0;
  TELLER_TIKKRACHT_TE_HARD   = 0;
  TELLER_TIKKRACHT_IN_BALANS = 0;

  TELLER_INSTORTEND_CORRECT  = 0;

  TELLER_SIMULTANE_START_OK  = 0;
  TELLER_SIMULTANE_EIND_OK   = 0;
}

static void ResetStimulusProfiel(StimulusProfiel &stimulus) {
  stimulus.TikTijd              = 0;
  stimulus.gemiddeldeTikKracht  = 0;
  stimulus.hoogsteTikKracht     = 0;
  stimulus.tijdTotPiekTikKracht = 0;
  stimulus.opbouwSnelheid       = 0;
  stimulus.afbouwSnelheid       = 0;
}

static void ResetSynchronisatieProfiel(SynchronisatieProfiel &synchronisatie) {
  synchronisatie.verschilStartTijd             = 0;
  synchronisatie.verschilEindTijd              = 0;
  synchronisatie.verschilTikTijd               = 0;
  synchronisatie.verschilGemiddeldeTikKracht   = 0;
  synchronisatie.verschilHoogsteTikKracht      = 0;
  synchronisatie.aantalSensorenSynchroonStart  = 0;
  synchronisatie.aantalSensorenSynchroonEinde  = 0;
}

// doelTikTijd: > 0 = expliciete milliseconden, 0 = nulmeting, -1/-2/-3 = instortende moeilijkheidsgraad.
void VergelijkStimulus(StimulusProfiel &nulmeting, StimulusProfiel &gemeten, bool &TijdCorrect, bool &KrachtCorrect, long doelTikTijd, char *instortendExtraTeken) {
  unsigned long referentieTikTijd = (doelTikTijd > INSTORTEND_TOV_NULMETING) ? doelTikTijd : nulmeting.TikTijd;
  unsigned long margeTikTijd = (referentieTikTijd * TOEGESTANE_MARGE_TIKTIJD) / (MARGE_FACTOR * 100UL);
  unsigned long minimaleTikTijd = 0, maximaleTikTijd = 0; // Ondergrens wordt later veilig op nul begrensd om unsigned-underflow te vermijden.

  TijdCorrect   = false;
  KrachtCorrect = false;

  // TIKTIJD
  if (doelTikTijd < INSTORTEND_TOV_NULMETING) {
    long int teTestenTiktijd = gemeten.TikTijd;
    int moeilijkheidsGraad = 0;

    if (doelTikTijd == INSTORTEND_MOEILIJKHEIDSGRAAD_2 || doelTikTijd == INSTORTEND_MOEILIJKHEIDSGRAAD_3) {
      moeilijkheidsGraad++;
      teTestenTiktijd += nulmeting.TikTijd;

      if (doelTikTijd == INSTORTEND_MOEILIJKHEIDSGRAAD_3) {
        moeilijkheidsGraad++;
        teTestenTiktijd += nulmeting.TikTijd;
      }
    }

    minimaleTikTijd = nulmeting.TikTijd * (INSTORTEND_MINIMALE_FACTOR + moeilijkheidsGraad);
    maximaleTikTijd = nulmeting.TikTijd * (INSTORTEND_MAXIMALE_FACTOR + moeilijkheidsGraad);

    if (teTestenTiktijd < minimaleTikTijd) {
      TELLER_TIKTIJD_TE_SNEL++;
      if (instortendExtraTeken != nullptr) *instortendExtraTeken = '<';
    } else if (teTestenTiktijd > maximaleTikTijd) {
      TELLER_TIKTIJD_TE_TRAAG++;
      if (instortendExtraTeken != nullptr) *instortendExtraTeken = '>';
    } else {
      TijdCorrect = true;
      if (instortendExtraTeken != nullptr) *instortendExtraTeken = '=';
    }
  } else {
    minimaleTikTijd = (referentieTikTijd > margeTikTijd) ? referentieTikTijd - margeTikTijd : 0;
    maximaleTikTijd = referentieTikTijd + margeTikTijd;

    if (gemeten.TikTijd < minimaleTikTijd) {
      TELLER_TIKTIJD_TE_SNEL++;
    } else if (gemeten.TikTijd > maximaleTikTijd) {
      TELLER_TIKTIJD_TE_TRAAG++;
    } else {
      TijdCorrect = true;
    }
  }

  int margeTikKracht = (nulmeting.gemiddeldeTikKracht * TOEGESTANE_MARGE_TIKKRACHT) / (MARGE_FACTOR * 100UL);

#ifdef DEBUG
  DEBUG_PRINTLN("---------------------");
  DEBUG_PRINT("TOEGESTANE_MARGE_TIKKRACHT = ");
  DEBUG_PRINTLN(TOEGESTANE_MARGE_TIKKRACHT);

  DEBUG_PRINT("gemeten = ");
  DEBUG_PRINTLN(gemeten.TikTijd);

  if (doelTikTijd < INSTORTEND_TOV_NULMETING) {
    DEBUG_PRINT("minimum = ");
    DEBUG_PRINTLN(minimaleTikTijd);

    DEBUG_PRINT("maximum = ");
    DEBUG_PRINTLN(maximaleTikTijd);
  } else {
    DEBUG_PRINT("TOEGESTANE_MARGE_TIKTIJD = ");
    DEBUG_PRINTLN(TOEGESTANE_MARGE_TIKTIJD);

    DEBUG_PRINT("MARGE_FACTOR = ");
    DEBUG_PRINTLN(MARGE_FACTOR);

    DEBUG_PRINT("referentieTikTijd = ");
    DEBUG_PRINTLN(referentieTikTijd);

    DEBUG_PRINT("margeTikTijd = ");
    DEBUG_PRINTLN(margeTikTijd);
  }

  DEBUG_PRINT("nulmeting gemiddeldeTikKracht = ");
  DEBUG_PRINTLN(nulmeting.gemiddeldeTikKracht);

  DEBUG_PRINT("margeTikKracht = ");
  DEBUG_PRINTLN(margeTikKracht);

  DEBUG_PRINT("gemeten TikTijd = ");
  DEBUG_PRINTLN(gemeten.TikTijd);

  DEBUG_PRINT("gemeten gemiddeldeTikKracht = ");
  DEBUG_PRINTLN(gemeten.gemiddeldeTikKracht);
#endif

  // TIKKRACHT
  if (gemeten.gemiddeldeTikKracht < (nulmeting.gemiddeldeTikKracht - margeTikKracht)) {
    TELLER_TIKKRACHT_TE_ZACHT++;
  } else if (gemeten.gemiddeldeTikKracht > (nulmeting.gemiddeldeTikKracht + margeTikKracht)) {
    TELLER_TIKKRACHT_TE_HARD++;
  } else {
    KrachtCorrect = true;
  }

  if (stimulusVersie == STIMULUS_EXTENDED) {
    int margePiekTikKracht = (nulmeting.hoogsteTikKracht * TOEGESTANE_MARGE_TIKKRACHT) / (MARGE_FACTOR * 100UL);
    bool piekTikKrachtCorrect = true;

    if (gemeten.hoogsteTikKracht < (nulmeting.hoogsteTikKracht - margePiekTikKracht)) {
      piekTikKrachtCorrect = false;
    } else if (gemeten.hoogsteTikKracht > (nulmeting.hoogsteTikKracht + margePiekTikKracht)) {
      piekTikKrachtCorrect = false;
    }

    unsigned long margeTijdTotPiekTikKracht = (nulmeting.tijdTotPiekTikKracht * TOEGESTANE_MARGE_TIKTIJD) / (MARGE_FACTOR * 100UL);
    bool tijdTotPiekTikKrachtCorrect = true;

    if (gemeten.tijdTotPiekTikKracht < (nulmeting.tijdTotPiekTikKracht - margeTijdTotPiekTikKracht)) {
      tijdTotPiekTikKrachtCorrect = false;
    } else if (gemeten.tijdTotPiekTikKracht > (nulmeting.tijdTotPiekTikKracht + margeTijdTotPiekTikKracht)) {
      tijdTotPiekTikKrachtCorrect = false;
    }

    int margeOpbouwSnelheid = (nulmeting.opbouwSnelheid * TOEGESTANE_MARGE_TIKKRACHT) / (MARGE_FACTOR * 100UL);
    bool opbouwSnelheidCorrect = true;

    if (gemeten.opbouwSnelheid < (nulmeting.opbouwSnelheid - margeOpbouwSnelheid)) {
      opbouwSnelheidCorrect = false;
    } else if (gemeten.opbouwSnelheid > (nulmeting.opbouwSnelheid + margeOpbouwSnelheid)) {
      opbouwSnelheidCorrect = false;
    }

    int margeAfbouwSnelheid = (nulmeting.afbouwSnelheid * TOEGESTANE_MARGE_TIKKRACHT) / (MARGE_FACTOR * 100UL);
    bool afbouwSnelheidCorrect = true;

    if (gemeten.afbouwSnelheid < (nulmeting.afbouwSnelheid - margeAfbouwSnelheid)) {
      afbouwSnelheidCorrect = false;
    } else if (gemeten.afbouwSnelheid > (nulmeting.afbouwSnelheid + margeAfbouwSnelheid)) {
      afbouwSnelheidCorrect = false;
    }

    TijdCorrect = TijdCorrect && tijdTotPiekTikKrachtCorrect;
    KrachtCorrect = KrachtCorrect && piekTikKrachtCorrect && opbouwSnelheidCorrect && afbouwSnelheidCorrect;

#ifdef DEBUG
    DEBUG_PRINTLN("---------------------");

    DEBUG_PRINT("piekTikKrachtCorrect = ");
    DEBUG_PRINTLN(piekTikKrachtCorrect);

    DEBUG_PRINT("tijdTotPiekTikKrachtCorrect = ");
    DEBUG_PRINTLN(tijdTotPiekTikKrachtCorrect);

    DEBUG_PRINT("opbouwSnelheidCorrect = ");
    DEBUG_PRINTLN(opbouwSnelheidCorrect);

    DEBUG_PRINT("afbouwSnelheidCorrect = ");
    DEBUG_PRINTLN(afbouwSnelheidCorrect);
#endif
  }

  if (TijdCorrect) TELLER_TIKTIJD_CORRECT++;
  if (KrachtCorrect) TELLER_TIKKRACHT_CORRECT++;
}

void VergelijkSynchronisatie(SynchronisatieProfiel &nulmeting, SynchronisatieProfiel &gemeten) {
  unsigned long margeStartTijd = (nulmeting.verschilStartTijd  * TOEGESTANE_MARGE_TIKTIJD) / (MARGE_FACTOR * 100UL);
  unsigned long margeTikTijd   = (nulmeting.verschilTikTijd  * TOEGESTANE_MARGE_TIKTIJD) / (MARGE_FACTOR * 100UL);

  bool synchronisatieCorrect = true;
  if (gemeten.verschilStartTijd > nulmeting.verschilStartTijd + margeStartTijd) { synchronisatieCorrect = false; }
  if (gemeten.verschilTikTijd > nulmeting.verschilTikTijd + margeTikTijd) { synchronisatieCorrect = false; }
  if (synchronisatieCorrect) { TELLER_TIKTIJD_SYNCHROON++; }

  int margeGemiddeldeTikKracht = (nulmeting.verschilGemiddeldeTikKracht  * TOEGESTANE_MARGE_TIKKRACHT) / (MARGE_FACTOR * 100UL);
  int margeHoogsteTikKracht    = (nulmeting.verschilHoogsteTikKracht  * TOEGESTANE_MARGE_TIKKRACHT) / (MARGE_FACTOR * 100UL);

  bool krachtInBalans = true;
  if (gemeten.verschilGemiddeldeTikKracht > nulmeting.verschilGemiddeldeTikKracht + margeGemiddeldeTikKracht) { krachtInBalans = false; }
  if (gemeten.verschilHoogsteTikKracht > nulmeting.verschilHoogsteTikKracht + margeHoogsteTikKracht) { krachtInBalans = false; }
  if (krachtInBalans) { TELLER_TIKKRACHT_IN_BALANS++; }

#ifdef DEBUG
  DEBUG_PRINTLN("---------------------");

  DEBUG_PRINT("verschilStartTijd nul/gemeten = ");
  DEBUG_PRINT(nulmeting.verschilStartTijd);
  DEBUG_PRINT(" / ");
  DEBUG_PRINTLN(gemeten.verschilStartTijd);

  DEBUG_PRINT("verschilTikTijd nul/gemeten = ");
  DEBUG_PRINT(nulmeting.verschilTikTijd);
  DEBUG_PRINT(" / ");
  DEBUG_PRINTLN(gemeten.verschilTikTijd);

  DEBUG_PRINT("synchronisatieCorrect = ");
  DEBUG_PRINTLN(synchronisatieCorrect);

  DEBUG_PRINT("verschilGemiddeldeTikKracht nul/gemeten = ");
  DEBUG_PRINT(nulmeting.verschilGemiddeldeTikKracht);
  DEBUG_PRINT(" / ");
  DEBUG_PRINTLN(gemeten.verschilGemiddeldeTikKracht);

  DEBUG_PRINT("verschilHoogsteTikKracht nul/gemeten = ");
  DEBUG_PRINT(nulmeting.verschilHoogsteTikKracht);
  DEBUG_PRINT(" / ");
  DEBUG_PRINTLN(gemeten.verschilHoogsteTikKracht);

  DEBUG_PRINT("krachtInBalans = ");
  DEBUG_PRINTLN(krachtInBalans);
  DEBUG_PRINTLN("---------------------");
#endif
}

static void VerwerkSensor(unsigned long nu, int sensorPin, int offsetSensor, SensorMeetStatus &sensor) {
  if (!sensor.sensorGestart && !sensor.sensorKlaar && sensor.actueleTikKracht > TIK_MINIMALE_DRUKWAARDE) InitialiseerSensorStart(nu, sensor);

  if (sensor.sensorGestart && !sensor.sensorKlaar) {
    if (sensor.actueleTikKracht > TIK_MINIMALE_DRUKWAARDE) {
      if (sensor.actueleTikKracht > sensor.hoogsteTikKrachtMeting) {
        sensor.hoogsteTikKrachtMeting = sensor.actueleTikKracht;
        sensor.tijdHoogsteTikKracht = nu;
      }

      sensor.somVanTikKrachtMetingen += sensor.actueleTikKracht;
      sensor.aantalTikKrachtMetingen++;
      sensor.laatsteTikKrachtMeting = sensor.actueleTikKracht;
    } else {
      sensor.eindTikTijd = nu;
      sensor.sensorKlaar = true;
    }
  }
}

void WachtTotAlleSensorsLosgelatenVoorTest(int aantalSensorenSimultaanTeMeten) {
  const int offsetSensor[4] = { offsetSensor1, offsetSensor2, offsetSensor3, offsetSensor4 };
  bool alleSensorsLosgelaten = false;

  while (!alleSensorsLosgelaten) {
    alleSensorsLosgelaten = true;

    for (int sensorNummer = 0; sensorNummer < aantalSensorenSimultaanTeMeten; sensorNummer++) {
      if (AnalogReadMetGekorigeerdeOffsets(sensorPin[sensorNummer], offsetSensor[sensorNummer]) > TIK_MINIMALE_DRUKWAARDE) alleSensorsLosgelaten = false;
    }

    if (WACHT_LOSLATEN_DELAY_MS > 0) delay(WACHT_LOSLATEN_DELAY_MS);  // Voorkomt onafgebroken I2C-bevraging bij ADC_BACKEND_ADS1115.
  }
}