// ============================================================================
// Data: FLASH, starten, stoppen, opnieuw starten en teruglezen
// ============================================================================
// Test de volledige opnameketen van de FLASH in één sketch.
// Stap 1: StreamingStart(), metingen, StreamingStop(), de laatste meting teruglezen met StreamingRead().
// Stap 2: opnieuw StreamingStart() op dezelfde chip, metingen, StreamingStop().
// Stap 3: de laatste meting van stap 2 teruglezen met StreamingRead().
// RESULTAAT: OK alleen als de FLASH-chip startte, elke meting lukte, er geen schrijffout was en elke teruglezing de laatste meting teruggaf.
// Dat bewijst geen duurzame opslag na een herstart.
// Geeft de sensor in stap 1 en stap 2 dezelfde waarde, dan bewijst stap 3 de opslag, maar niet dat de tweede opname nieuw is.
// Aansluiting: FLASH op SPI zoals in examples/Systeem/Data/FLASH, FsrRfp via de ADS1115 zoals in Sensor_FsrRfp_FLASH.
// ============================================================================

#include <GroeiAcademie.h>

#if EXTENDER_ADS1115_AANTAL == 0
  #error Zet EXTENDER_ADS1115_AANTAL in UserConfig.h op minstens 1: FsrRfp meet via een ADS1115-Extender.
#endif

ExtenderADS1115 extenderADS1115(
  &Native, GedeeldeBusComponent::ADC_ADS1115,
  I2C_ADDRESS_EXTENDER_ADS1115_1,
  HardwareResourcePin::NP_SDA, HardwareResourcePin::NP_SCL,
  EXTENDER_ADS1115_1_ALERT_RDY,
  HardwareResourceToegang::GEDEELD
);

#if !((DATA_LAYER_CONFIG) & DATA_TYPE_FLASH_SPI)
  #error Voeg DATA_TYPE_FLASH_SPI toe aan DATA_LAYER_CONFIG voor deze test.
#endif

void ToonResultaat(const __FlashStringHelper* stap, bool gelukt) {
  GA_SERIAL.print(stap);
  GA_SERIAL.print(F(": "));
  GA_SERIAL.println(gelukt ? F("OK") : F("FOUT"));
}

void setup() {
  GA_SERIAL.begin(SERIAL_BAUDRATE);
  #if defined(DEBUG)
  while (!GA_SERIAL) { ; }
  #else
  const unsigned long startTijd = millis();
  while (!GA_SERIAL && (millis() - startTijd) < SERIAL_CONNECT_TIMEOUT_MS) { ; }
  #endif
  GA_SERIAL.println(F("TEST: FLASH_StartStopRead"));

  if (Data == nullptr) Data = GedeeldeBusNewComponent<struct Data>(DATA_TYPE_FLASH_SPI);
  ToonResultaat(F("Data aanmelden/controleren/inpluggen/activeren"), Data != nullptr);
  if (Data == nullptr) {
    GA_SERIAL.println(F("RESULTAAT: FOUT"));
    return;
  }

  sensorFsrRfpSet1 = GedeeldeBusNewComponent<struct FsrRfp>(&extenderADS1115, GedeeldeBusComponent::FsrRfp, Data);
  ToonResultaat(F("Sensor aanmelden/controleren/inpluggen/activeren"), sensorFsrRfpSet1 != nullptr);
  if (sensorFsrRfpSet1 == nullptr) {
    GA_SERIAL.println(F("RESULTAAT: FOUT"));
    return;
  }

  bool gelukt = true;
  int32_t lastMeasuredValue = 0;   // de laatste geslaagde meting
  SensorQueueOrStack readBackChannel;   // de kopie die Data vult bij het teruglezen

  // Stap 1: eerste opname.
  Data->StreamingStart();
  ToonResultaat(F("Stap 1: StreamingStart"), Data->streamingActief);
  gelukt = gelukt && Data->streamingActief;
  ToonResultaat(F("Stap 1: StreamingStart: FLASH-chip gestart"), Data->flashStreamingStarted[0]);
  gelukt = gelukt && Data->flashStreamingStarted[0];
  // Vijf metingen van sensor 1; de sensor geeft ze door aan Data.
  // Elke meting moet lukken; de laatste wordt daarna teruggelezen.
  for (uint8_t teller = 0; teller < 5; teller++) {
    if (!sensorFsrRfpSet1->RawAnalogRead(sensorFsrRfpSet1->sensorPin[0], lastMeasuredValue)) gelukt = false;
    delay(20);
  }
  Data->StreamingStop();
  ToonResultaat(F("Stap 1: StreamingStop"), !Data->streamingActief);
  gelukt = gelukt && !Data->streamingActief;
  // Na de stop moet alles in de FLASH staan, niet meer in de buffer in RAM.
  ToonResultaat(F("Stap 1: geen schrijffout"), !Data->streamingWriteFailed);
  gelukt = gelukt && !Data->streamingWriteFailed;
  // Teruglezen via Data, in een kopie van channel 1 met een andere waarde.
  // Alleen een gevonden record geeft dan de laatste meting terug.
  readBackChannel = sensorFsrRfpSet1->QueueOrStack[0];
  readBackChannel.lastValue = static_cast<int>(lastMeasuredValue) + 1;
  if (Data->StreamingRead(*sensorFsrRfpSet1, 1, readBackChannel) && readBackChannel.lastValue == static_cast<int>(lastMeasuredValue)) {
    GA_SERIAL.println(F("Stap 1: StreamingRead = laatste meting: OK"));
  } else {
    GA_SERIAL.println(F("Stap 1: StreamingRead = laatste meting: FOUT"));
    gelukt = false;
  }

  // Stap 2: opnieuw starten op dezelfde chip; de nieuwe opname komt achter de vorige.
  Data->StreamingStart();
  ToonResultaat(F("Stap 2: StreamingStart opnieuw"), Data->streamingActief);
  gelukt = gelukt && Data->streamingActief;
  ToonResultaat(F("Stap 2: StreamingStart opnieuw: FLASH-chip gestart"), Data->flashStreamingStarted[0]);
  gelukt = gelukt && Data->flashStreamingStarted[0];
  // Vijf metingen van sensor 1; de sensor geeft ze door aan Data.
  // Elke meting moet lukken; de laatste wordt daarna teruggelezen.
  for (uint8_t teller = 0; teller < 5; teller++) {
    if (!sensorFsrRfpSet1->RawAnalogRead(sensorFsrRfpSet1->sensorPin[0], lastMeasuredValue)) gelukt = false;
    delay(20);
  }
  Data->StreamingStop();
  ToonResultaat(F("Stap 2: StreamingStop"), !Data->streamingActief);
  gelukt = gelukt && !Data->streamingActief;
  // Na de stop moet alles in de FLASH staan, niet meer in de buffer in RAM.
  ToonResultaat(F("Stap 2: geen schrijffout"), !Data->streamingWriteFailed);
  gelukt = gelukt && !Data->streamingWriteFailed;

  // Stap 3: teruglezen na het stoppen; nu moet de laatste meting van stap 2 terugkomen.
  // Teruglezen via Data, in een kopie van channel 1 met een andere waarde.
  // Alleen een gevonden record geeft dan de laatste meting terug.
  readBackChannel = sensorFsrRfpSet1->QueueOrStack[0];
  readBackChannel.lastValue = static_cast<int>(lastMeasuredValue) + 1;
  if (Data->StreamingRead(*sensorFsrRfpSet1, 1, readBackChannel) && readBackChannel.lastValue == static_cast<int>(lastMeasuredValue)) {
    GA_SERIAL.println(F("Stap 3: StreamingRead = laatste meting: OK"));
  } else {
    GA_SERIAL.println(F("Stap 3: StreamingRead = laatste meting: FOUT"));
    gelukt = false;
  }

  GA_SERIAL.println(gelukt ? F("RESULTAAT: OK") : F("RESULTAAT: FOUT"));
}

void loop() {}
