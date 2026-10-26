// ============================================================================
// N8: PixelScreen timing meten
// ============================================================================
// Doel:
// 1. Pixel.Clear() afzonderlijk meten.
// 2. Pixel.Print() afzonderlijk meten, zonder Clear().
// 3. Volledige Screen->Print() meten.
//
// Gebaseerd op de publieke Screen/Pixel-interface en de bestaande
// Default_PixelScreen-example van GROEI ACADEMIE FrameWork v2.0.0.
// Wijzigt geen librarycode.
// ============================================================================

#include <Screen.h>
#include <Configuratie/ExamplesConfig.h>

#if !((SCREEN_OUTPUT_CONFIG) & SCREEN_TYPE_PIXELS)
  #error Stel SCREEN_OUTPUT_CONFIG in UserConfig.h of SystemConfig.h in op minstens SCREEN_TYPE_PIXELS.
#else

static const uint16_t AANTAL_METINGEN = 20;

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && (millis() - startTijd) < SERIAL_CONNECT_TIMEOUT_MS) { ; }

  Screen = GedeeldeBusNewComponent<struct Screen>(SCREEN_TYPE_PIXELS);
  if (Screen == nullptr) {
    GA_SERIAL.println(F("FOUT: PixelScreen kon niet worden aangemaakt."));
    while (true) { ; }
  }

  GA_SERIAL.println(F("N8 PIXELSCREEN TIMING"));
  GA_SERIAL.println(F("---------------------"));
  GA_SERIAL.print(F("Aantal metingen per test: "));
  GA_SERIAL.println(AANTAL_METINGEN);

  // Eerst eenmaal tekenen zodat initialisatiekosten niet in de timing zitten.
  Screen->Print("N8 warming-up", "even geduld");
  delay(250);

  unsigned long totaalClear = 0;
  unsigned long langsteClear = 0;

  for (uint16_t teller = 0; teller < AANTAL_METINGEN; teller++) {
    const unsigned long startMicros = micros();
    Screen->Pixel.Clear();
    const unsigned long duurMicros = micros() - startMicros;

    totaalClear += duurMicros;
    if (duurMicros > langsteClear) langsteClear = duurMicros;
  }

  unsigned long totaalTekst = 0;
  unsigned long langsteTekst = 0;

  for (uint16_t teller = 0; teller < AANTAL_METINGEN; teller++) {
    Screen->Pixel.SetCursor(0, 0);

    const unsigned long startMicros = micros();
    Screen->Pixel.Print("ABCDEFGHIJKLMNOPQRST");
    const unsigned long duurMicros = micros() - startMicros;

    totaalTekst += duurMicros;
    if (duurMicros > langsteTekst) langsteTekst = duurMicros;
  }

  unsigned long totaalVolledig = 0;
  unsigned long langsteVolledig = 0;

  for (uint16_t teller = 0; teller < AANTAL_METINGEN; teller++) {
    const unsigned long startMicros = micros();
    Screen->Print("ABCDEFGHIJKLMNOPQRST", "ABCDEFGHIJKLMNOPQRST");
    const unsigned long duurMicros = micros() - startMicros;

    totaalVolledig += duurMicros;
    if (duurMicros > langsteVolledig) langsteVolledig = duurMicros;
  }

  GA_SERIAL.println(F("RESULTATEN"));
  GA_SERIAL.println(F("----------"));
  GA_SERIAL.print(F("Pixel.Clear gemiddeld_us="));
  GA_SERIAL.println(totaalClear / AANTAL_METINGEN);
  GA_SERIAL.print(F("Pixel.Clear langste_us="));
  GA_SERIAL.println(langsteClear);
  GA_SERIAL.print(F("Pixel.Print 20 tekens gemiddeld_us="));
  GA_SERIAL.println(totaalTekst / AANTAL_METINGEN);
  GA_SERIAL.print(F("Pixel.Print 20 tekens langste_us="));
  GA_SERIAL.println(langsteTekst);
  GA_SERIAL.print(F("Screen->Print 2x20 gemiddeld_us="));
  GA_SERIAL.println(totaalVolledig / AANTAL_METINGEN);
  GA_SERIAL.print(F("Screen->Print 2x20 langste_us="));
  GA_SERIAL.println(langsteVolledig);

  GA_SERIAL.println(F("KLAAR"));
}

void loop() {
}

#endif
