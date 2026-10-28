// GROEI ACADEMIE FrameWork v2.0.0 RC1
// Foutinjectie SS301: SerialScreen activeren() mislukt
// Verwachte build: normaal of DEBUG, met REDIRECT_SERIAL_CONNECT_TIMEOUT_MS
// Opstelling: board met native USB (UNO R4, ESP32-S3), LCD aangesloten, HX1838 aangesloten, GEEN Serial Monitor open
// Bewezen = de echte FrameWork-melding "CRITICAL: SS301" verschijnt op het LCD. Geen melding = NIET BEWEZEN.
// Afzonderlijk opstartscenario: de test loopt bij elke reset. Resultaatsoort: HANDMATIG CONTROLEREN (op het LCD).
// Bediening: HX1838, toets 1 = uitleg op het LCD.
// Werkwijze: sluit de Serial Monitor, druk reset, wacht SERIAL_CONNECT_TIMEOUT_MS en lees het LCD. Druk reset om te herhalen.

#include <GroeiAcademie.h>

#if !(SCREEN_OUTPUT & SCREEN_TYPE_SERIAL) || !(SCREEN_OUTPUT & SCREEN_TYPE_CHARACTER)
  #error SS301 vereist SCREEN_TYPE_SERIAL en SCREEN_TYPE_CHARACTER in SCREEN_OUTPUT_CONFIG (UserConfig.h).
#endif
#ifndef REDIRECT_SERIAL_CONNECT_TIMEOUT_MS
  #error SS301 vereist REDIRECT_SERIAL_CONNECT_TIMEOUT_MS in UserConfig.h: zonder timeout wacht SerialScreen eindeloos op de Serial Monitor.
#endif
#if !((INPUT_KANAAL_CONFIG) & INPUT_TYPE_HX1838)
  #error SS301 vereist INPUT_TYPE_HX1838 in INPUT_KANAAL_CONFIG: de Serial Monitor is tijdens deze test gesloten.
#endif
#if HX1838_TOETSENINDELING != HX1838_TOETSENINDELING_REMOTE_OK_BOVENAAN_17_TOETSEN && HX1838_TOETSENINDELING != HX1838_TOETSENINDELING_REMOTE_OK_ONDERAAN_17_TOETSEN
  #error SS301 vereist een HX1838-afstandsbediening met 17 toetsen.
#endif

void ToonUitleg() {
  if (Screen != nullptr) Screen->Print(F("SS301: Serial"), F("dicht + reset"), 3000);
}

const MappingTussenToetsaanslagEnUitTeVoerenFunctie mappingTestMenu[] = {
  {_LABEL_OPSCHRIFT_1, ToonUitleg}
};

void setup() {
  // Geen wachten op de Serial Monitor: SerialScreen moet zelf na SERIAL_CONNECT_TIMEOUT_MS opgeven.
  Screen = GedeeldeBusNewComponent<struct Screen>();
  if (Screen == nullptr) return;
  Input = GedeeldeBusNewComponent<struct Input>(INPUT_TYPE_HX1838);
}

void loop() {
  if (Input != nullptr) Input->UitVoerenFunctieVolgensMappingMetToetsAanslag(true, mappingTestMenu);
}
