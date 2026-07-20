#include <Wire.h>
#include <Screen.h>

#define SERIAL_BAUDRATE              115200UL
#define LEESTIJD_TEST_MS             2000UL
#define WACHTTIJD_TUSSEN_PAGINAS_MS  1500UL

void setup() {
  Serial.begin(SERIAL_BAUDRATE);

  lcd.init();
  lcd.backlight();

  SCREEN_OUTPUT = SCREEN_TYPE_SERIAL | SCREEN_TYPE_CHARACTER;
}

void loop() {
  while (true) {
    // Deel 1
    PrintToScreen("TEST 1", "Twee regels");
    delay(LEESTIJD_TEST_MS);

    PrintToScreen("TEST 2", "+leestijd", LEESTIJD_TEST_MS);

    PrintToScreen("TEST 3", "+leestijd", LEESTIJD_TEST_MS, " +actie");
    delay(LEESTIJD_TEST_MS);

    PrintToScreen("TEST 4", "Zo meteen volgt:", LEESTIJD_TEST_MS);
    PrintToScreen("", "", 0, "enkel actie");
    delay(LEESTIJD_TEST_MS);

    PrintToScreen("TEST 5", "Eerste pagina", LEESTIJD_TEST_MS, " +actie", "DERDE REGEL", "VIERDE REGEL", WACHTTIJD_TUSSEN_PAGINAS_MS);

    // Deel 2
    PrintToScreen("TEST 6", "2de regel", 0, "+actie", "3de regel", "4de regel");
    delay(LEESTIJD_TEST_MS);

    PrintToScreen("TEST 7", "2de regel", 0, "+actie", "3de regel", "4de regel", LEESTIJD_TEST_MS);
    delay(LEESTIJD_TEST_MS);

    PrintToScreen("TEST 8", "2de regel", 0, "+actie", "3de regel", "4de regel", 0);
    delay(LEESTIJD_TEST_MS);

    PrintToScreen("TEST 9", "2de regel", 0, "+actie", "3de regel", "4de regel", LEESTIJD_TEST_MS);
    delay(LEESTIJD_TEST_MS);

    PrintToScreen("TEST 10", "2de regel", LEESTIJD_TEST_MS, "+actie", "3de regel", "4de regel", 0);
    delay(LEESTIJD_TEST_MS);

    PrintToScreen("TEST 11", "2de regel", LEESTIJD_TEST_MS, "+actie", "3de regel", "4de regel", LEESTIJD_TEST_MS);
    delay(LEESTIJD_TEST_MS);
  }
}