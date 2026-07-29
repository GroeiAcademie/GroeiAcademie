// ============================================================================
// GroeiAcademie UNO R3/R4 Shield v0.10.1 — volledige hardwarevalidatie
// ============================================================================
// Dit is een zelfstandig hardware-validatiescript.
// Plaats voor de TFT-route ofwel de levelshifter, ofwel de vijf draadbruggen.
// Plaats nooit beide tegelijk.
// ============================================================================

#define VALIDATIE_I2C_BUS          1
#define VALIDATIE_CHARACTER_SCREEN 1
#define VALIDATIE_PIXEL_SCREEN     1
#define VALIDATIE_KEYMATRIX        1

#define VALIDATIE_ADC_ROUTE_GEEN    0
#define VALIDATIE_ADC_ROUTE_NATIVE  1
#define VALIDATIE_ADC_ROUTE_ADS1115 2
#define VALIDATIE_ADC_ROUTE VALIDATIE_ADC_ROUTE_NATIVE

#if VALIDATIE_I2C_BUS || VALIDATIE_CHARACTER_SCREEN || VALIDATIE_ADC_ROUTE == VALIDATIE_ADC_ROUTE_ADS1115
#include <Wire.h>
#endif
#if VALIDATIE_PIXEL_SCREEN
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#endif
#if VALIDATIE_CHARACTER_SCREEN
#include <LiquidCrystal_I2C.h>
#endif
#if VALIDATIE_ADC_ROUTE == VALIDATIE_ADC_ROUTE_ADS1115
#include <Adafruit_ADS1X15.h>
#endif

#define VALIDATIE_I2C_ADRES_LCD 0x27
#define VALIDATIE_I2C_ADRES_ADS1115 0x48
#define VALIDATIE_PIXEL_CS 10
#define VALIDATIE_PIXEL_DC 9
#define VALIDATIE_PIXEL_RST 8
#define VALIDATIE_PIXEL_BREEDTE 240
#define VALIDATIE_PIXEL_HOOGTE 320
#define VALIDATIE_PIXEL_ROTATION 0
#define VALIDATIE_SAMPLE_AANTAL 20
#define VALIDATIE_SAMPLE_INTERVAL_MS 20UL
#define VALIDATIE_KEYMATRIX_DUUR_MS 15000UL

#if VALIDATIE_CHARACTER_SCREEN
LiquidCrystal_I2C lcd(VALIDATIE_I2C_ADRES_LCD, 16, 2);
#endif
#if VALIDATIE_PIXEL_SCREEN
Adafruit_ST7789 pixelScreen(VALIDATIE_PIXEL_CS, VALIDATIE_PIXEL_DC, VALIDATIE_PIXEL_RST);
#endif
#if VALIDATIE_ADC_ROUTE == VALIDATIE_ADC_ROUTE_ADS1115
Adafruit_ADS1115 ads;
#endif

uint8_t aantalGeslaagd = 0;
uint8_t aantalFout = 0;
uint8_t aantalHandmatig = 0;

void PrintTitel(const __FlashStringHelper* titel) { Serial.println(); Serial.println(F("============================================================================")); Serial.println(titel); Serial.println(F("============================================================================")); }
void ResultaatGeslaagd(const __FlashStringHelper* tekst) { Serial.print(F("GESLAAGD: ")); Serial.println(tekst); aantalGeslaagd++; }
void ResultaatFout(const __FlashStringHelper* tekst) { Serial.print(F("FOUT: ")); Serial.println(tekst); aantalFout++; }
void ResultaatHandmatig(const __FlashStringHelper* tekst) { Serial.print(F("HANDMATIG CONTROLEREN: ")); Serial.println(tekst); aantalHandmatig++; }

#if VALIDATIE_I2C_BUS || VALIDATIE_CHARACTER_SCREEN || VALIDATIE_ADC_ROUTE == VALIDATIE_ADC_ROUTE_ADS1115
bool I2CAdresAanwezig(uint8_t adres) { Wire.beginTransmission(adres); return Wire.endTransmission() == 0; }
#endif

void ValideerVoorbereiding() {
  PrintTitel(F("0. Verplichte visuele en multimetercontrole"));
  Serial.println(F("Controleer eerst: geen soldeerbruggen, geen onderbroken banen en correcte connectororientatie."));
  Serial.println(F("TFT: levelshifters OF vijf draadbruggen, nooit beide tegelijk."));
  Serial.println(F("ADC: ADS1115 op H5 OF vier H6-jumpers naar A0-A3, nooit beide tegelijk."));
  Serial.println(F("SW1: maximaal een ADDR-schakelaar ON. H7: exact een voedingskeuze 5V of 3V3."));
  ResultaatHandmatig(F("meet 5V, 3V3, GND, H7-uitgang, TFT-VCC en HV/LV voordat modules geplaatst worden"));
  ResultaatHandmatig(F("meet op spanningsloze print op kortsluiting tussen 5V-GND en 3V3-GND"));
}

void ValideerI2CBus() {
#if VALIDATIE_I2C_BUS
  PrintTitel(F("1. I2C-bus"));
  uint8_t gevonden = 0;
  for (uint8_t adres = 1; adres < 127; adres++) if (I2CAdresAanwezig(adres)) { Serial.print(F("I2C-apparaat gevonden op 0x")); if (adres < 16) Serial.print('0'); Serial.println(adres, HEX); gevonden++; }
  if (gevonden) ResultaatGeslaagd(F("minstens een I2C-apparaat gevonden")); else ResultaatFout(F("geen I2C-apparaten gevonden"));
#if VALIDATIE_CHARACTER_SCREEN
  if (I2CAdresAanwezig(VALIDATIE_I2C_ADRES_LCD)) ResultaatGeslaagd(F("CharacterScreen-adres gevonden")); else ResultaatFout(F("CharacterScreen-adres niet gevonden"));
#endif
#if VALIDATIE_ADC_ROUTE == VALIDATIE_ADC_ROUTE_ADS1115
  if (I2CAdresAanwezig(VALIDATIE_I2C_ADRES_ADS1115)) ResultaatGeslaagd(F("ADS1115-adres gevonden")); else ResultaatFout(F("ADS1115-adres niet gevonden"));
#endif
#else
  PrintTitel(F("1. I2C-bus overgeslagen"));
#endif
}

void ValideerCharacterScreen() {
#if VALIDATIE_CHARACTER_SCREEN
  PrintTitel(F("2. CharacterScreen"));
  if (!I2CAdresAanwezig(VALIDATIE_I2C_ADRES_LCD)) { ResultaatFout(F("LCD niet gevonden op het ingestelde I2C-adres")); return; }
  lcd.init(); lcd.backlight(); lcd.clear(); lcd.setCursor(0, 0); lcd.print(F("GA Shield 0.10.1")); lcd.setCursor(0, 1); lcd.print(F("LCD OK?"));
  ResultaatHandmatig(F("beide LCD-regels, backlight, contrast en stabiele tekst controleren")); delay(2000);
#else
  PrintTitel(F("2. CharacterScreen overgeslagen"));
#endif
}

void ValideerPixelScreen() {
#if VALIDATIE_PIXEL_SCREEN
  PrintTitel(F("3. TFT-SPI PixelScreen"));
  Serial.println(F("Controleer vooraf: levelshifters OF vijf draadbruggen, nooit beide."));
  pixelScreen.init(VALIDATIE_PIXEL_BREEDTE, VALIDATIE_PIXEL_HOOGTE); pixelScreen.setRotation(VALIDATIE_PIXEL_ROTATION); pixelScreen.setTextWrap(false);
  const uint16_t kleuren[] = { ST77XX_RED, ST77XX_GREEN, ST77XX_BLUE, ST77XX_WHITE, ST77XX_BLACK };
  const char* namen[] = { "ROOD", "GROEN", "BLAUW", "WIT", "ZWART" };
  for (uint8_t i = 0; i < 5; i++) { pixelScreen.fillScreen(kleuren[i]); Serial.print(F("TFT kleurtest: ")); Serial.println(namen[i]); delay(700); }
  pixelScreen.fillScreen(ST77XX_BLACK); pixelScreen.setTextColor(ST77XX_WHITE); pixelScreen.setTextSize(2); pixelScreen.setCursor(10, 10); pixelScreen.println(F("GA Shield v0.10.1")); pixelScreen.println(F("TFT-SPI test")); pixelScreen.println(F("240x320 ST7789"));
  ResultaatHandmatig(F("volledige beeldvulling, kleuren, orientatie, reset, tekst en afwezigheid van gekleurde ruis controleren")); delay(2000);
#else
  PrintTitel(F("3. TFT-SPI PixelScreen overgeslagen"));
#endif
}

void ValideerKeymatrix() {
#if VALIDATIE_KEYMATRIX
  PrintTitel(F("4. Keymatrix D2-D5"));
  const uint8_t pinnen[4] = { 3, 2, 5, 4 };
  uint8_t vorigeStatus[4];
  for (uint8_t i = 0; i < 4; i++) { pinMode(pinnen[i], INPUT_PULLUP); vorigeStatus[i] = digitalRead(pinnen[i]); }
  Serial.println(F("Bedien gedurende 15 seconden alle aangesloten toetsen. Elke elektrische statuswijziging wordt gemeld."));
  unsigned long start = millis(); uint16_t wijzigingen = 0;
  while (millis() - start < VALIDATIE_KEYMATRIX_DUUR_MS) {
    for (uint8_t i = 0; i < 4; i++) { uint8_t status = digitalRead(pinnen[i]); if (status != vorigeStatus[i]) { Serial.print(F("TOETS ")); Serial.print(i + 1); Serial.print(F(" / D")); Serial.print(pinnen[i]); Serial.print(F(" -> ")); Serial.println(status ? F("HIGH") : F("LOW")); vorigeStatus[i] = status; wijzigingen++; } }
  }
  if (wijzigingen) ResultaatGeslaagd(F("minstens een keymatrix-statuswijziging gemeten")); else ResultaatFout(F("geen keymatrix-statuswijziging gemeten"));
  ResultaatHandmatig(F("controleer dat iedere bedoelde toets de verwachte lijn of combinatie activeert"));
#else
  PrintTitel(F("4. Keymatrix overgeslagen"));
#endif
}

void ValideerADC() {
#if VALIDATIE_ADC_ROUTE == VALIDATIE_ADC_ROUTE_ADS1115
  PrintTitel(F("5. ADS1115 A0-A3"));
  Serial.println(F("Voor deze run: ADS1115 op H5, geen H6-jumpers op H5 pin 7-10."));
  if (!ads.begin(VALIDATIE_I2C_ADRES_ADS1115)) { ResultaatFout(F("ADS1115 niet gevonden op het ingestelde adres")); return; }
  ads.setGain(GAIN_TWOTHIRDS);
  for (uint8_t kanaal = 0; kanaal < 4; kanaal++) { long som = 0; int16_t minimum = 32767; int16_t maximum = -32768; for (uint8_t i = 0; i < VALIDATIE_SAMPLE_AANTAL; i++) { int16_t waarde = ads.readADC_SingleEnded(kanaal); som += waarde; if (waarde < minimum) minimum = waarde; if (waarde > maximum) maximum = waarde; delay(VALIDATIE_SAMPLE_INTERVAL_MS); } Serial.print(F("ADS1115 A")); Serial.print(kanaal); Serial.print(F(": gemiddelde=")); Serial.print(som / VALIDATIE_SAMPLE_AANTAL); Serial.print(F(" min=")); Serial.print(minimum); Serial.print(F(" max=")); Serial.println(maximum); }
  ResultaatGeslaagd(F("ADS1115 en vier kanalen uitgelezen"));
  ResultaatHandmatig(F("bedien elke sensor afzonderlijk en controleer dat alleen het bedoelde kanaal duidelijk verandert"));
#elif VALIDATIE_ADC_ROUTE == VALIDATIE_ADC_ROUTE_NATIVE
  PrintTitel(F("5. Arduino A0-A3"));
  Serial.println(F("Voor deze run: geen ADS1115 op H5, vier H6-jumpers op H5 pin 7-10."));
  const uint8_t pinnen[4] = { A0, A1, A2, A3 };
  for (uint8_t kanaal = 0; kanaal < 4; kanaal++) { unsigned long som = 0; int32_t minimum = 2147483647L; int32_t maximum = -2147483647L; for (uint8_t i = 0; i < VALIDATIE_SAMPLE_AANTAL; i++) { int waarde = analogRead(pinnen[kanaal]); som += waarde; if (waarde < minimum) minimum = waarde; if (waarde > maximum) maximum = waarde; delay(VALIDATIE_SAMPLE_INTERVAL_MS); } Serial.print(F("Arduino A")); Serial.print(kanaal); Serial.print(F(": gemiddelde=")); Serial.print(som / VALIDATIE_SAMPLE_AANTAL); Serial.print(F(" min=")); Serial.print(minimum); Serial.print(F(" max=")); Serial.println(maximum); }
  ResultaatGeslaagd(F("Arduino A0-A3 uitgelezen"));
  ResultaatHandmatig(F("bedien elke sensor afzonderlijk en controleer dat alleen het bedoelde kanaal duidelijk verandert"));
#else
  PrintTitel(F("5. ADC-route overgeslagen"));
#endif
}

void ValideerNietAutomatiseerbareRoutes() {
  PrintTitel(F("6. Continuiteit, isolatie en bestukking"));
  ResultaatHandmatig(F("controleer H1-H10 pin voor pin tegen het actuele schema met continuiteitstest"));
  ResultaatHandmatig(F("controleer isolatie tussen naburige connectorpinnen en tussen niet-verbonden netten"));
  ResultaatHandmatig(F("controleer U3: SDA, SCL en TFT-RST via shifter of draadbrug, nooit beide"));
  ResultaatHandmatig(F("controleer U8: TFT SCL, SDA, CS en DC via shifter of draadbrug, nooit beide"));
  ResultaatHandmatig(F("controleer H5, H6, H7, H8 en SW1 tegen silkscreen en actuele tekeningset"));
}

void PrintSamenvatting() {
  PrintTitel(F("Validatiesamenvatting"));
  Serial.print(F("Automatisch geslaagd: ")); Serial.println(aantalGeslaagd);
  Serial.print(F("Automatische fouten: ")); Serial.println(aantalFout);
  Serial.print(F("Nog handmatig af te tekenen: ")); Serial.println(aantalHandmatig);
  if (aantalFout) Serial.println(F("EINDRESULTAAT: NIET GOEDGEKEURD.")); else Serial.println(F("EINDRESULTAAT: automatische testen zonder fout; keur pas goed nadat alle handmatige controles afgetekend zijn."));
}

void setup() {
  Serial.begin(115200); while (!Serial && millis() < 3000UL) {}
#if VALIDATIE_I2C_BUS || VALIDATIE_CHARACTER_SCREEN || VALIDATIE_ADC_ROUTE == VALIDATIE_ADC_ROUTE_ADS1115
  Wire.begin();
#endif
#if VALIDATIE_PIXEL_SCREEN
  SPI.begin();
#endif
  PrintTitel(F("GroeiAcademie Shield v0.10.1 hardwarevalidatie"));
  ValideerVoorbereiding(); ValideerI2CBus(); ValideerCharacterScreen(); ValideerPixelScreen(); ValideerKeymatrix(); ValideerADC(); ValideerNietAutomatiseerbareRoutes(); PrintSamenvatting();
}

void loop() {}
