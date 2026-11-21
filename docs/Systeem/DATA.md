# Data

Status: v2.0.0 in ontwikkeling.

## Structuur

`DATA_TYPE_NONE` is de oude werkwijze zonder Data-class. De sensor geeft zijn gemeten waarde rechtstreeks terug aan de toepassing.

Wanneer een ander Data-type actief is, is Data een top-level GedeeldeBus-component onder `Native`. De gewone toepassing maakt alleen `Data` aan. `Data` maakt zelf de geconfigureerde concrete media aan en voert hun GedeeldeBus-lifecycle uit.

```text
DATA_TYPE_NONE
└─ sensor -> toepassing

Native
└─ Data
   ├─ DIRECT
   ├─ EEPROM   : NATIVE
   ├─ EEPROM   : I2C
   ├─ FLASH    : SPI
   ├─ FRAM     : I2C
   ├─ microSD  : SPI
   └─ NRF24L01 : SPI
```

Per medium is er één bestand met alle aansluitingen van dat medium: `EEPROM.h/.cpp`, `FLASH.h/.cpp`, `FRAM.h/.cpp`, `microSD.h/.cpp` en `NRF24L01.h/.cpp`. `RedirectSerial.h/.cpp` bevat de optionele omleiding van `GA_SERIAL` naar microSD.

`DIRECT` is geen opslagmedium, maar wel een kind van Data (`struct DataDIRECT`, component `DATA_DIRECT`, zonder claims). `DIRECT` gebruikt de Data-class voor real-time doorgeven. Elk channel van een sensor bewaart zijn eigen laatst gemeten waarde (`lastValue`). Met een QueueOrStack-grootte groter dan `0` bewaart Data daarnaast waarden in RAM per sensor-channel (FIFO: oudste eerst, of LIFO: nieuwste eerst; de sensor kiest per channel). Dit werkt ook wanneer andere Data-types dan DIRECT zijn geselecteerd.

## Configuratie

`DATA_LAYER_CONFIG` bepaalt welke Data-werking in deze build aanwezig is.

```cpp
#define DATA_LAYER_CONFIG DATA_TYPE_NONE
#define DATA_LAYER_CONFIG DATA_TYPE_DIRECT
#define DATA_LAYER_CONFIG (DATA_TYPE_EEPROM_I2C | DATA_TYPE_FLASH_SPI | DATA_TYPE_FRAM_I2C | DATA_TYPE_MICROSD_SPI)
```

Eén bit staat voor een medium samen met zijn aansluiting. Gebruik altijd de naam, nooit het getal. De huidige typewaarden zijn:

```cpp
#define DATA_TYPE_NONE          0x0001
#define DATA_TYPE_DIRECT        0x0002
#define DATA_TYPE_EEPROM_NATIVE 0x0004
#define DATA_TYPE_EEPROM_I2C    0x0008
#define DATA_TYPE_EEPROM_SPI    0x0010   // nog niet ondersteund in v2.0.0
// 0x0020 vrij: FLASH via I2C
#define DATA_TYPE_FLASH_SPI     0x0040
#define DATA_TYPE_FRAM_I2C      0x0080
#define DATA_TYPE_FRAM_SPI      0x0100   // nog niet ondersteund in v2.0.0
// 0x0200 vrij: microSD via I2C
#define DATA_TYPE_MICROSD_SPI   0x0400
// 0x0800 vrij: NRF24L01 native
// 0x1000 vrij: NRF24L01 via I2C
#define DATA_TYPE_NRF24L01_SPI  0x2000
```

`DATA_TYPE_NONE` kan niet met een ander Data-type worden gecombineerd. `DATA_TYPE_EEPROM_SPI` en `DATA_TYPE_FRAM_SPI` geven een compileerfout zolang ze niet ondersteund zijn. `typesActief` in Data is daarom 16 bits.

## NONE

`DATA_TYPE_NONE` gebruikt in de gewone sensorroute geen Data-class. Uitzondering: `REDIRECT_SERIAL_TO_MICROSD` maakt via `GA_SERIAL.begin()` een eigen Data/microSD-route beschikbaar. Voor FsrRfp blijft de rechtstreekse route behouden:

```text
RawAnalogRead(...)
-> gemeten waarde
-> return naar toepassing
```

## DIRECT

`DATA_TYPE_DIRECT` gebruikt de Data-class:

```text
FsrRfp
-> WriteQueueOrStack(sensorNummer, ruweData)   (overgeërfd van Sensor, logica in Data)
-> ReadQueueOrStackStatus(sensorNummer)        1 = waarde klaar en vol, 0 = waarde klaar, -1 = leeg, -2 = sensor bestaat niet
-> ReadQueueOrStackValue(sensorNummer)         volgende waarde; leeg = lastValue van dat channel
-> toepassing
```

Voor FsrRfp bepaalt:

```cpp
FSR_RFP_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1
```

en per channel `FSR_RFP_CHANNEL_1..4_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1` (standaard gelijk aan de waarde hierboven) of een QueueOrStack gebruikt wordt. De QueueOrStack hangt niet af van DIRECT: hij werkt bij elk Data-type zodra de grootte groter is dan 0.

```text
0   = geen QueueOrStack; ReadQueueOrStackValue() geeft lastValue van dat channel terug
> 0 = RAM-QueueOrStack; ReadQueueOrStackValue() haalt de volgende waarde uit de QueueOrStack van dat channel
```

De QueueOrStack-grootte en de keuze FIFO of LIFO horen bij de sensor. Voor andere sensoren wordt hun eigen sensorconfiguratie gebruikt.

Test op `ReadQueueOrStackStatus(...) >= 0` om te weten of er een waarde klaar is: `1` betekent dat de QueueOrStack vol is en dat de volgende `WriteQueueOrStack(...)` de oudste waarde overschrijft.

Niet als afzonderlijke QueueOrStack-bewerking voorzien: een waarde lezen zonder ze weg te nemen (peek), de QueueOrStack expliciet leegmaken en RAM-inhoud behouden na stroomuitval. `int`, `float` en `uint32_t` zijn wél ondersteund; gebruik voor de laatste twee `ReadQueueOrStackFloat()` en `ReadQueueOrStackUInt32()`. Een lege QueueOrStack geeft via de passende typefunctie de laatst bewaarde waarde terug. `ReadQueueOrStackValue()` is de `int`-interface; bij `float` en `uint32_t` geeft die een geconverteerde laatste waarde terug, zonder de getypeerde buffer leeg te maken.

## EEPROM

Beide aansluitingen staan in `EEPROM.h/.cpp` en gebruiken dezelfde component `DATA_EEPROM`.

### EEPROM NATIVE

`DATA_TYPE_EEPROM_NATIVE` gebruikt de native EEPROM van het board via `EEPROM.h` van de core (struct `DataEEPROMviaNative`, geen claims).

```cpp
DATA_LAYER_EEPROM_NATIVE_SIZE     // bytes voor Data; standaard 0, verplicht > 0 wanneer EEPROM NATIVE aan staat
HX1838_EEPROM_NATIVE_SIZE         // vast: 3 + 21 = 24 wanneer HX1838 de EEPROM gebruikt, anders 0
```

Het blok van HX1838 staat vooraan (adres 0 tot `HX1838_EEPROM_NATIVE_SIZE`). Data begint daarna. `HX1838_EEPROM_NATIVE_SIZE` is de grootste indeling die in EEPROM-modus mag (3 bytes kop + 21 toetsen), zodat Data altijd op hetzelfde adres begint.

```cpp
uint8_t Data->ReadNativeEEPROM(uint16_t adres);           // adres 0 = eerste byte na het blok van HX1838
void    Data->WriteNativeEEPROM(uint16_t adres, uint8_t waarde);
```

Buiten `DATA_LAYER_EEPROM_NATIVE_SIZE` geeft lezen 0 en doet schrijven niets.

- R4, STM32: `update()`, dat alleen schrijft wanneer de waarde verandert.
- ESP32 en RP2040: Input en Data roepen `EEPROM.begin()` aan met dezelfde som (`HX1838_EEPROM_NATIVE_SIZE + DATA_LAYER_EEPROM_NATIVE_SIZE`). Na elke schrijfactie die iets verandert, volgt meteen `commit()`, want een volgende `begin()` leest het geheugen opnieuw in.
- UNO Q: geen `EEPROM.h` in de core. Het medium wordt niet aangemaakt; lezen geeft 0.

### EEPROM I2C

EEPROM I2C gebruikt de library `I2C_EEPROM` van Rob Tillaart (struct `DataEEPROMviaI2C`).

```cpp
DATA_LAYER_EEPROM_I2C_AANTAL
DATA_LAYER_EEPROM_I2C_1_I2C_ADDRESS
DATA_LAYER_EEPROM_I2C_2_I2C_ADDRESS
```

SDA en SCL komen uit de boardconfiguratie. De chips staan in `Data->EEPROMviaI2C[]`.

## FLASH

FLASH gebruikt SPI en de library `Adafruit SPIFlash`, zonder cache en zonder bestandssysteem: de struct `DataFLASHviaSPI` erft van `Adafruit_SPIFlashBase`. Het transport is eigen code (`TransportFLASHviaSPI`): het schakelt CS en gebruikt de gedeelde SPI-bus met `beginTransaction()`/`endTransaction()`. CS via PCF8574, PCF8575 of MCP23017 is al geïmplementeerd. Bij activeren wordt de JEDEC-ID teruggelezen als controle.

```cpp
DATA_LAYER_FLASH_SPI_AANTAL
DATA_LAYER_FLASH_SPI_1_CS_PIN
DATA_LAYER_FLASH_SPI_2_CS_PIN
```

SCK, MISO en MOSI komen uit de boardconfiguratie. De chips staan in `Data->FLASHviaSPI[]`.

### FLASH-buffer

```cpp
#define DATA_LAYER_FLASH_SPI_1_BUFFER_SIZE 0     // zo ook _2_
```

- `0` (standaard): elke `StreamingWrite()` schrijft meteen naar FLASH. Er wordt geen buffer gecompileerd.
- `sizeof(int)` (`4` op de ondersteunde 32-bit boards) t/m `256` is als buffergrootte toegestaan. **Vanaf 15 bytes** kan een volledig GB-record in de buffer: de records worden in RAM verzameld en weggeschreven wanneer het volgende record niet meer past, bij `StreamingStop()` en bij het afmelden van Data. Bij 4 t/m 14 bytes past geen volledig record en wordt rechtstreeks naar FLASH geschreven. Bij `StreamingStart()` probeert Data eerst een nog wachtende buffer op **dezelfde chip** weg te schrijven; bij een mislukte flush start die chip niet en blijft de buffer voor een volgende poging behouden. `256` bytes is één FLASH-pagina.
- Waarom: een FLASH-schrijfactie heeft vaste overhead, ook bij één meting. Een record is 15 bytes (GB-record): in een buffer van 256 bytes passen 17 volledige records (255 bytes). Die 17 records kunnen samen in één `writeBuffer()` worden doorgegeven, in plaats van per record. De werkelijke tijdswinst hangt af van de chip en het transport.
- Kleiner dan één `int` (maar groter dan 0) geeft een compileerfout.
- `StreamingRead()` probeert eerst de buffer weg te schrijven. Lukt dat niet, dan zoekt de functie eerst in de resterende RAM-buffer, van nieuw naar oud, en daarna in FLASH. Ook zonder actieve streaming kan zij oude records rechtstreeks uit FLASH lezen, inclusief na een herstart.
- Nadeel: bij stroomuitval gaat wat nog in de buffer zit verloren (maximaal de ingestelde buffergrootte). Er is één statische RAM-buffer per geconfigureerde FLASH-plaats. Wordt een chip afgemeld of vernietigd, dan vervallen de wachtende records van zijn plaats; een vervangende chip krijgt die niet.
- EEPROM I2C en FRAM hebben in Data geen afzonderlijke STREAMING-/RECORDING-buffer; hun geërfde library-API blijft beschikbaar. microSD gebruikt de buffering van SdFat zelf, niet de RAM-buffer van FLASH.

Voor continue sensordata geldt:

```text
STREAMING
├─ microSD
└─ FLASH

RECORDING
└─ microSD
```

EEPROM en FRAM blijven beschikbaar als Data-media met hun geërfde library-API, maar worden niet gebruikt voor de bestaande continue STREAMING-/RECORDING-routes. Bij combinaties met meerdere opslaggebruikers wordt `DATA_LAYER_EXCLUSIEVE_OPSLAG_ACTIEF` door `SystemConfig.h` afgeleid; dit is geen instelling die de gebruiker zelf zet.

## FRAM

FRAM I2C (`DATA_TYPE_FRAM_I2C`, bestand `FRAM.h/.cpp`, struct `DataFRAMviaI2C`) gebruikt I2C en de library `FRAM_I2C` van Rob Tillaart. De concrete libraryclass is `FRAM`.

De huidige referentie is de Fujitsu MB85RC256V van 32 KB. Volgens de officiële librarydocumentatie gebruikt dit type de class `FRAM`, heeft het standaard I2C-adres `0x50` en gebruikt het 16-bits geheugenadressen.

```cpp
DATA_LAYER_FRAM_I2C_AANTAL
DATA_LAYER_FRAM_I2C_1_I2C_ADDRESS
DATA_LAYER_FRAM_I2C_2_I2C_ADDRESS
```

SDA en SCL komen uit de boardconfiguratie. `DataFRAMviaI2C` behoudt de volledige geërfde `FRAM`-API voor lezen, schrijven, metadata en foutstatus. De chips staan in `Data->FRAMviaI2C[]`.

FRAM I2C is in deze stap alleen als Data-medium geïntegreerd. De bestaande STREAMING- en RECORDING-routing wordt niet uitgebreid.

## microSD

microSD gebruikt SPI en `SdFat` (struct `DataMICROSDviaSPI`, erft van `SdFat32`: FAT16 en FAT32). De library is de "SdFat - Adafruit Fork" (`SdFat_Adafruit_Fork.h`): Adafruit SPIFlash brengt die mee, en twee SdFat-versies in één programma botsen. Bestanden zijn `File32`. SdFat geeft een foutcode terug (`sdErrorCode()`), die mee naar de foutdiagnose gaat.

Bij CS via een Extender kan de `void`-callback `sdCsWrite()` een mislukte CS-schakeling niet rechtstreeks aan SdFat terugmelden; de bestaande beperkingen worden verderop beschreven.

```cpp
DATA_LAYER_MICROSD_SPI_AANTAL
DATA_LAYER_MICROSD_SPI_1_CS_PIN
DATA_LAYER_MICROSD_SPI_2_CS_PIN
```

SCK, MISO en MOSI komen uit de boardconfiguratie. De kaarten staan in `Data->MICROSDviaSPI[]`. Een kaart van 64 GB of meer moet in FAT32 geformatteerd zijn.

### CS via een Extender

FLASH en microSD kunnen CS op een Extender hebben (PCF8574, PCF8575 of MCP23017), met SCK, MISO en MOSI op het board. Per chip drie instellingen:

```cpp
#define DATA_LAYER_FLASH_SPI_1_CS_EXTENDER GedeeldeBusComponent::DIGITAL_PINS_PCF8574   // welke Extender
#define DATA_LAYER_FLASH_SPI_1_CS_EXTENDER_I2C_ADDRESS 0x20                           // I2C-adres van die Extender
#define DATA_LAYER_FLASH_SPI_1_CS_PIN ExtenderPCF8574::ExtenderPins::EP_P0             // CS-pin op die Extender
```

Zo ook `_2_` en `MICROSD_SPI`. Data zoekt de Extender in de boom op component en adres; de Extender moet dus vóór `Data` aangemaakt en geactiveerd zijn. Niet gevonden: GB108 (CS ontbreekt). `GEDEELDE_BUS_MIXED_PINS` wordt dan automatisch `1`.

- FLASH: het transport schakelt CS via de Extender.
- microSD: SdFat schakelt CS via `sdCsInit()`/`sdCsWrite()` wanneer CS op een Extender staat. Gebruik daarvoor **`SD_CHIP_SELECT_MODE=1`** in `SdFatConfig.h` van "SdFat - Adafruit Fork". Bij mode 0 blokkeert de FrameWork-code deze Extender-configuratie met een `#error`. De standaardwaarde in de onderzochte fork 2.3.103 is 0. Stel mode 1 in de geïnstalleerde fork in; controleer dit opnieuw bij een library-update. Een native CS-pin gebruikt de gewone SdFat-route.
**CS-fouten bij microSD via een Extender (SdFat - Adafruit Fork 2.3.103):** `sdCsWrite()` heeft een `void`-resultaat en wordt door SdFat tijdens selectie en vrijgave aangeroepen. Een mislukte GPIO-schrijfactie wordt door de bestaande GedeeldeBus-Extender verwerkt; de callback probeert hetzelfde CS-niveau maximaal eenmaal opnieuw. Deze herhaalpoging is geen bevestiging dat de kaart correct geselecteerd of vrijgegeven werd. SdFat kan onmiddellijk daarna SPI-data versturen zonder deze fout te kennen. Bij een aanhoudende CS- of busfout kan de FrameWork-code een veilige voortzetting van de SD-overdracht niet garanderen; gegevensverlies of een onvolledige schrijfactie blijft mogelijk. Er wordt niet onvoorwaardelijk HIGH uitgegeven vóór iedere LOW-opdracht, omdat SdFat CS bewust laag houdt gedurende lopende overdrachten. Voor een harde stop vóór iedere volgende SPI-byte is een afzonderlijk foutdoorgevingsmechanisme nodig, binnen het huidige `void`-contract niet beschikbaar.

- Snelheid: elke CS-wissel via een I2C-Extender kost ongeveer 0,05 ms (400 kHz) tot 0,2 ms (100 kHz) (berekend). CS op het board blijft de standaard.

Alternatief (niet gekozen): de originele `SdFat` van Bill Greiman (`#include <SdFat.h>`). Die is de bron van de fork, met dezelfde klassen (`SdFat32`, `File32`, `SdSpiConfig`). Waarom niet: `Adafruit SPIFlash` vereist de fork, en twee SdFat-versies in één programma geven compileerfouten wanneer FLASH en microSD samen gebruikt worden. Overschakelen is mogelijk door in `microSD.h` de include te wijzigen en in `library.properties` `SdFat - Adafruit Fork` door `SdFat` te vervangen; FLASH en microSD samen werken dan niet meer. Het officiële Adafruit-voorbeeld `SdFat_Flash_and_SDcard` gebruikt dezelfde combinatie als de library (fork + Adafruit SPIFlash).

Op de UNO R4 definieert SdFat een eigen `F()`-macro wanneer er nog geen is. De core van de R4 heeft `F()` als functie (en doet zelf `#undef F` in `Arduino.h`). Daarom staat in `microSD.h` na de include van SdFat `#undef F`, alleen op `ARDUINO_ARCH_RENESAS`. Bij elke upgrade van SdFat of van de R4-core opnieuw testen.

Sensordata-recording op microSD schrijft naar `SENSOR.DAT`, geopend met `O_APPEND`. Een mislukte of onderbroken `write()` kan een onvolledig record achterlaten; `O_APPEND` garandeert niet dat de bestandinhoud intact of correct uitgelijnd blijft. Bij teruglezen zoekt `RecordRead()` daarom byte voor byte opnieuw naar een geldig record. Een mislukte `write()` of `sync()` zet `recordingWriteFailed`, zodat `RecordingStop()` false geeft. Dit bewijst niet dat een kaart elke geschreven byte duurzaam heeft opgeslagen.

Met een DS3231-klok (`SENSOR_DS3231_AANTAL 1`) krijgen `SENSOR.DAT` en elk ander bestand automatisch de echte datum en tijd als aanmaak- en wijzigingsdatum. Het recordformaat verandert niet. Zie `docs/Systeem/Sensor/DS3231.md`.

Records bewaren de componentcode van de sensor. Opnames met andere componentcodes dan de huidige zijn niet terug te lezen.

## NRF24L01

De NRF24L01 is een radiomodule (2,4 GHz) via SPI (struct `DataNRF24L01viaSPI`, erft van `RF24`). Hij is een streamingmedium: na `StreamingStart()` wordt elk GB-record dat naar FLASH of `SENSOR.DAT` gaat, ook als één pakket van 15 bytes draadloos verstuurd. Een tweede Arduino ontvangt de records (voorbeeld `NRF24L01_Ontvanger`).

```cpp
#define DATA_LAYER_CONFIG DATA_TYPE_NRF24L01_SPI
DATA_LAYER_NRF24L01_SPI_AANTAL          // 0 of 1
DATA_LAYER_NRF24L01_SPI_1_CS_PIN        // CSN: selecteert de module
DATA_LAYER_NRF24L01_SPI_1_CE_PIN        // CE: zet zenden en ontvangen aan of uit
DATA_LAYER_NRF24L01_SPI_1_RF_CHANNEL    // radiokanaal 0..125 (standaard 76)
DATA_LAYER_NRF24L01_SPI_1_ADRES         // precies 5 tekens (standaard "GROEI")
```

- SCK, MISO en MOSI komen uit de boardconfiguratie. De module staat in `Data->NRF24L01viaSPI[]`.
- CS kan niet via een Extender: de RF24-library schakelt CSN en CE zelf.
- Best-effort: geen ontvangstbevestiging (auto-ack uit) en dus **geen ontvangstgarantie**. Een meting wacht nooit op een ontvanger; een verloren pakket wordt niet opnieuw verstuurd en de zender merkt het niet. De terugkeerwaarde van `RF24::write()` wordt niet gebruikt.
- `RF24::write()` wacht tot het pakket verzonden is (berekend: minder dan 1 ms voor 15 bytes, niet gemeten). De hardwaretest meet de gemiddelde en de maximale duur bij langdurige belasting.
- Voeding 3,3 V met een condensator van 10 µF tot 100 µF over de voeding: de module trekt korte stroompieken.
- Externe library: `RF24`.

## HC-05

De HC-05 (Bluetooth) gebruikt een seriële UART en heeft geen eigen Data-klasse. Het voorbeeld `examples/Systeem/Data/HC05/` gebruikt `GA_SENSOR_SERIAL` met de hieronder vermelde boardpinnen. Ook een geschikte UART-Extender biedt de benodigde seriële interface via `GetExtenderStream()` en `StartExtenderUART()`; het meegeleverde HC-05-voorbeeld demonstreert de native UART-route.

## RAM-QueueOrStack

De QueueOrStack is eigen code in Data (geen externe library). Voor gewone sensorkanalen reserveert `Sensor::activeren()` automatisch de RAM-buffers wanneer Data actief is; bij FsrRfp blijven de bestaande tabellen `ruweDataQueueOrStackChannel1..4` van de sensor zelf (met set 2 is de gereserveerde grootte per channel de grootste van beide sets). Is de QueueOrStack vol, dan wordt de oudste waarde overschreven.

De QueueOrStack-grootte wordt per sensor geconfigureerd, los van DIRECT. Voor FsrRfp:

```cpp
FSR_RFP_DATA_LAYER_QUEUE_OR_STACK_SIZE_SET_1
```

Gelijktijdige toegang vanuit twee taken of cores vraagt een aparte analyse en synchronisatie; ook FIFO is niet vanzelf veilig.

De QueueOrStack bestaat alleen per sensor-channel. Een tweede QueueOrStack per medium bestaat niet: de oude, nooit gebruikte instellingen `DATA_LAYER_<MEDIUM>_<NR>_QUEUE_OR_STACK_SIZE` zijn verwijderd. Voor FLASH is er in de plaats een schrijfbuffer (zie "FLASH-buffer").

`DATA_QUEUE_OR_STACK_ACTIEF` wordt niet door de gebruiker ingesteld. `SystemConfig.h` maakt de compile-time waarde **1 wanneer `DATA_LAYER_CONFIG != DATA_TYPE_NONE`**, anders 0. De QueueOrStack-grootte bepaalt apart of voor een specifiek channel werkelijk een RAM-buffer wordt gereserveerd. Voor automatische sensorbuffers gebruikt `DATA_LAYER_SENSOR_QUEUE_OR_STACK_SIZE` standaard 16 elementen, of 32 op ESP32-S3 Ardi-32 en ESP32-S3 DEV. Met `DATA_LAYER_SENSOR_QUEUE_OR_STACK_SIZE_FOR(component, channel)` is de grootte per sensor en channel in te stellen (0 = alleen `lastValue`).

## Sensorgrens

Een sensor maakt geen medium aan en hoeft niet te weten of zijn data via DIRECT, STREAMING of RECORDING wordt afgehandeld. Wanneer Data actief is, krijgt de sensor bij creatie een pointer naar `Data` mee.

Voor FsrRfp komt elke meting in `lastValue` (en, bij een grootte > 0, in de QueueOrStack) van het juiste channel:

```cpp
WriteQueueOrStack(sensorNummer, ruweData);   // overgeërfd van Sensor
```

Bij `DATA_TYPE_NONE` is er geen Data-class in de sensorroute en blijft de directe returnwaarde van `RawAnalogRead(...)` actief.

## Recordformaat en getypeerde waarden

`StreamingWrite()` schrijft hetzelfde **GB-record van 15 bytes** naar FLASH, `SENSOR.DAT` op microSD en (indien geconfigureerd) de NRF24L01: twee magic bytes `GB`, twee bytes componentcode, vier bytes vaste route-identiteit, één kanaalnummer, één waardetype (`0=int32`, `1=float32`, `2=uint32`), vier waardebytes en één XOR-controlebyte. Het record heeft geen tijdstempel. `RecordRead()` kan oudere GA-records van 12 bytes herkennen en overslaan, maar gebruikt hun tijdelijke runtime-ID niet als geldige identificatie voor de actuele sensor.

Een sensor bewaart de actuele waarden getypeerd. `ReadQueueOrStackFloat()` en `ReadQueueOrStackUInt32()` lezen de bijbehorende QueueOrStack. Voor opgeslagen waarden zijn er getypeerde sensoroverloads `StreamingRead(uint8_t numberChannel, float& value)` en `StreamingRead(uint8_t numberChannel, uint32_t& value)`, en dezelfde twee vormen van `RecordRead()`. Deze geven `true` wanneer een geldig record van het gevraagde type gevonden is. De bestaande `int`-vorm retourneert de gevonden integerwaarde of de laatste integerwaarde van het channel als er geen match is; voor een niet-integer-type geeft die vorm geen getypeerde waarde terug.

## Data-interface

```cpp
Data->StreamingStart();
Data->StreamingWrite(sensor, numberChannel, channel);   // getypeerd record met sensor- en kanaalidentiteit
bool gevonden = Data->StreamingRead(sensor, numberChannel, channel); // gelezen GB-record uit FLASH; false = geen match
Data->StreamingStop();

Data->RecordingStart();
// Tijdens een opname (na RecordingStart()) schrijft StreamingWrite() ook naar SENSOR.DAT op microSD; er is geen aparte RecordingWrite().
bool gevondenOpKaart = Data->RecordRead(sensor, numberChannel, channel); // uit SENSOR.DAT; false = geen match of opname actief
Data->RecordingStop();

sensor->StreamingRead(sensorNummer); // int-interface; bij ontbreken van Data/kanaal: 0
sensor->RecordRead(sensorNummer);    // int-interface; bij ontbreken van Data/kanaal: 0
```

`StreamingRead()` en `RecordRead()` lezen per component, vaste route-identiteit en kanaal. De vaste route-identiteit wordt bij `Sensor::activeren()` berekend op basis van de component, pinclaims en ouderketen; ze is niet de tijdelijke GedeeldeBus-runtime-ID. Bij meerdere FLASH-chips of microSD-kaarten doorloopt Data de plaatsen in hun geconfigureerde volgorde: de eerste plaats met een geldig passend record wordt gebruikt. Er is geen tijdsvergelijking tussen records op verschillende media.

Per medium houdt Data een lijst van zijn chips bij: `Data->EEPROMviaNative`, `Data->EEPROMviaI2C[]`, `Data->FLASHviaSPI[]`, `Data->FRAMviaI2C[]`, `Data->MICROSDviaSPI[]`. Een lege plaats is `nullptr`. Elke chip wist bij het verwijderen zelf zijn plaats in de lijst (destructor).

Data faalt alleen wanneer geen enkel kind overblijft. Een medium dat niet werkt, wordt afgemeld; de andere media blijven werken. DIRECT is ook een kind.

microSD roept `sync()` aan na `DATA_LAYER_MICROSD_SPI_FLUSH_NA_AANTAL_REGEL` geschreven records (standaard 50) of wanneer bij een nieuwe `StreamingWrite()` minstens `DATA_LAYER_MICROSD_SPI_FLUSH_NA_MS` milliseconden sinds de vorige `sync()` zijn verlopen (standaard 5000 ms). Zonder nieuwe metingen volgt er geen tijdgestuurde `sync()`. Roep `RecordingStop()` aan voor je de stroom onderbreekt; onverwachte stroomuitval blijft een risico.

De sensor gebruikt STREAMING en RECORDING niet rechtstreeks. De toepassing kiest die werking. De concrete media behouden daarnaast hun volledige geërfde library-API.

## Sensorvoorbeelden

```text
Sensor_FsrRfp.ino
-> DATA_TYPE_NONE
-> oude werking zonder Data-class

Sensor_FsrRfp_DIRECT.ino
-> DATA_TYPE_DIRECT
-> ReadQueueOrStackValue(sensorNummer)
-> optionele QueueOrStack per channel

Sensor_FsrRfp_FLASH.ino
-> DATA_TYPE_FLASH_SPI
-> Data STREAMING naar FLASH
-> sensorFsrRfpSet1->StreamingRead(sensorNummer)

Sensor_FsrRfp_MICROSD.ino
-> DATA_TYPE_MICROSD_SPI
-> Data RECORDING naar microSD
```

## Media-tests

De gewone media-validatietests maken `Data` aan en gebruiken daarna het door `Data` aangemaakte concrete child-object. Ze maken hetzelfde medium niet nogmaals aan.

```text
examples/Systeem/Data/EEPROM/EEPROM.ino      (EEPROM I2C en/of EEPROM NATIVE)
examples/Systeem/Data/FLASH/FLASH.ino
examples/Systeem/Data/FRAM/FRAM.ino
examples/Systeem/Data/microSD/microSD.ino
```

De afzonderlijke FRAM-stresstest staat in:

```text
extras/stress/Test_FRAM/Test_FRAM.ino
```

## Serial redirect

Alle seriële frameworkuitvoer loopt via `GA_SERIAL`.

```cpp
REDIRECT_SERIAL_NONE
REDIRECT_SERIAL_TO_MICROSD
REDIRECT_SERIAL_FILE "serial.log"
```

`REDIRECT_SERIAL_CONNECT_TIMEOUT_MS` is verplicht bij `REDIRECT_SERIAL_TO_MICROSD` (anders `#error`). Het is een schakelaar zonder waarde: `SerialOutput::Activeren()` wacht dan maximaal `SERIAL_CONNECT_TIMEOUT_MS` op Serial in plaats van eindeloos.

```cpp
#define REDIRECT_SERIAL REDIRECT_SERIAL_TO_MICROSD
#define REDIRECT_SERIAL_CONNECT_TIMEOUT_MS
```

`REDIRECT_SERIAL_NONE` betekent geen redirect. `GA_SERIAL` blijft de bestaande `Serial` of `Serial0` gebruiken.

`REDIRECT_SERIAL_TO_MICROSD` maakt bij `GA_SERIAL.begin(...)` de noodzakelijke Data/microSD-route automatisch beschikbaar wanneer `Data` nog niet bestaat. De toepassing hoeft daarvoor niet zelf `DATA_TYPE_MICROSD_SPI` aan `DATA_LAYER_CONFIG` toe te voegen. De uitvoer wordt via de eerste microSD van `Data` toegevoegd aan `REDIRECT_SERIAL_FILE`; er wordt niet tegelijk naar de fysieke Serial geschreven. Bij een onbeschikbare kaart kan `RedirectSerial::write()` 0 bytes schrijven. Een regeleinde triggert `FileSerial.flush()`; dit is geen garantie tegen stroomuitval.


Bij `REDIRECT_SERIAL_TO_MICROSD` moet `GA_SERIAL.begin()` worden aangeroepen vóór `GedeeldeBusNewComponent<struct Screen>()`.


### microSD: controle bij initialisatie en grenzen van de CS-callback

Voor CS via een GPIO-Extender controleert `DataMICROSDviaSPI::Activeren()` vóór `SdFat32::begin()` of `WriteCS(HIGH)` is gelukt en of er een vrije koppeling bestaat in `linkCSviaExtender[]`. Als een van beide voorwaarden ontbreekt, wordt de activering geweigerd. Een ontbrekende koppeling mag niet stilzwijgend terugvallen op een native chipselect-pin.

De geïnstalleerde `SdFat - Adafruit Fork` 2.3.103 gebruikt bij `SD_CHIP_SELECT_MODE=1` de bestaande callbacks `void sdCsInit(SdCsPin_t)` en `void sdCsWrite(SdCsPin_t, bool)`. Bij het omschakelen van de chipselect tijdens kaarttransacties kan `sdCsWrite()` geen succes- of foutwaarde teruggeven. Het FrameWork herhaalt een gemelde mislukte schakeling maximaal eenmaal, maar kan **niet garanderen dat SdFat bij een tweede fout ophoudt met SPI-data versturen**. Dit is een harde integratiebeperking; een geslaagde `SdFat32`-aanroep is geen bewijs dat bij een CS-storing alle overgebrachte gegevens betrouwbaar zijn.

Voor opslag die geen gegevensverlies mag tolereren: CS via een directe geschikte native uitgang gebruiken zolang er geen door de bibliotheek ondersteund, foutdoorgevend selectiemechanisme beschikbaar is. De broncode van de externe SdFat-bibliotheek is niet aangepast. Foutinjectie vereist fysieke validatie; bij een onzekere CS-toestand niet blijven schrijven.

## Foutafhandeling, teruglezen en herstel

- **Mislukte microSD-write:** `StreamingWrite()` controleert de returnwaarde van `write()` naar `SENSOR.DAT`. Een mislukte write telt niet mee in `aantalRegelsSindsFlush`, zet `recordingWriteFailed` en maakt `RecordingStop()` onwaar. Het ontbrekende record wordt niet automatisch opnieuw geschreven.

### Beschadigde records en buffergedrag

- **microSD, `RecordRead()` na een onvolledig record:** eerst de checksum. Is die fout, dan zoekt `RecordRead()` één byte na het begin van die header opnieuw. Ook zonder geldige header gaat het zoeken byte per byte verder. Zo worden de records na een onderbroken write weer gevonden. Een onvolledig record blijft wel in het bestand staan.
- **FLASH, mislukte flush:** de nieuwste records blijven in de buffer in RAM. `StreamingRead()` zoekt daar eerst, van nieuw naar oud. Bij `StreamingStart()` blijft het schrijfadres staan, zodat een volgende start opnieuw op de juiste plaats probeert.
- **microSD, tijdgrens van de flush:** `DATA_LAYER_MICROSD_SPI_FLUSH_NA_MS` wordt alleen nagekeken bij een nieuwe write. Zonder nieuwe metingen volgt geen tijdgestuurde `sync()`. `DATA_LAYER_MICROSD_SPI_FLUSH_NA_AANTAL_REGEL=1` vraagt een `sync()` na ieder geslaagd record en beperkt zo het risico op verlies van wachtende gegevens, maar biedt geen garantie bij stroomuitval of kaartfouten; de kaart wordt trager en kan sneller slijten.
- **Teruglezen raakt de live waarde niet meer:** `StreamingRead()` en `RecordRead()` van een sensor werken op een kopie van het channel. De live waarde en de nog niet gelezen QueueOrStack blijven zoals ze waren.
- **Lege QueueOrStack:** is de buffer van een channel leeg, dan geeft het lezen de laatst bewaarde waarde (`lastValue`), niet "niets". Een nieuwe meting herken je aan `ReadQueueOrStackStatus() >= 0`; -1 betekent een lege buffer. Dat geldt ook voor de ruwe waarde van BioAmpEXGPill en voor de bewaarde waarden van MAX30102 en NeuroSkyTGAM.
- **EEG-kanalen:** de EEG-banden van BioAmpEXGPill en de berekende ruwe-golfbanden van NeuroSkyTGAM gaan alleen naar Data bij een aanroep van de functie die ze berekent. Zonder die aanroep blijft het channel op zijn vorige waarde.
- **`RecordingStop()` geeft `bool` terug:** true vereist een daadwerkelijk lopende opname, zonder eerdere schrijf-/syncfouten, en het succesvol wegschrijven en sluiten van alle open bestanden. `flush()` van SdFat geeft hier geen resultaat, daarom gebruikt Data `sync()`.
- **Uitgeschakelde microSD bij `RecordingStop()`:** geen `sync()` of `close()`, want die schrijven naar de kaart. Het bestandsobject wordt alleen in het geheugen losgelaten, en `RecordingStop()` geeft false. In "SdFat - Adafruit Fork" 2.3.103 doet die toewijzing geen kaarttoegang.
- **Opnieuw starten na `RecordingStop()`:** `StreamingStart()` start de microSD-opname opnieuw als de streaming via een ander medium nog actief is. `StreamingStop()` beëindigt ook een lopende microSD-opname.
- **microSD met CS via een Extender:** mislukt het schakelen van de CS twee keer na elkaar, dan zet de library de kaart uit (`actief` false). Data schrijft er dan niet meer naar. Een SPI-component heeft geen herstelpad: de kaart blijft uit tot een herstart. De SPI-overdracht die al liep, kan niemand stoppen.
- **Testvoorbeelden:** `examples/Systeem/Data/FLASH/FLASH_StartStopRead` en `examples/Systeem/Data/microSD/microSD_StartStopRead` doen starten, stoppen, opnieuw starten en teruglezen na elkaar.

### HC-05-aansluitingen

- Het voorbeeld `examples/Systeem/Data/HC05/` gebruikt `GA_SENSOR_SERIAL` en zijn pinnen, dezelfde UART als de sensoren met een seriële poort. Het voorbeeld heeft boardafhankelijke UART-instellingen.
- HC-05 TXD gaat naar `GA_SENSOR_SERIAL_PIN_RX`, HC-05 RXD naar `GA_SENSOR_SERIAL_PIN_TX`:
  - UNO R4, UNO Q, ESP32-S3 Ardi-32 en RP2040: D0 (RX) en D1 (TX);
  - ESP32 D1 R32 en ESP32-S3 DEV: D2 (RX) en D3 (TX), want D0 en D1 dragen `GA_SERIAL`;
  - Nucleo F401RE: D2 (RX) en D8 (TX); het voorbeeld maakt daar zelf `Serial1` aan.
- De RXD van de HC-05 verwacht 3,3 V: op een board met 5 V-logica via een spanningsdeler.
- UNO Q: bij elke reset stuurt Zephyr een opstartmelding over `Serial1`; die komt ook op de gsm.

### Start, schrijffouten en CS-fouten

- **Alleen een gestarte FLASH-chip krijgt records:** een FLASH-chip krijgt pas records na een geslaagde reservering, het wegschrijven van een eventuele eigen wachtbuffer en de controle van de hele chip (`flashStreamingStarted`). De code wist geen bestaande sectoren: de nieuwe schrijfpositie ligt na de laatst gebruikte sector. Bij een volle chip of leesfout start die chip niet. Dit geldt met en zonder `DATA_LAYER_EXCLUSIEVE_OPSLAG_ACTIEF`.
- **Een FLASH-chip die niet startte:** houdt een ander medium (microSD of nRF24L01) de streaming actief, dan probeert elke volgende `StreamingStart()` die chip opnieuw. Daarbij wordt telkens de hele chip gecontroleerd; bij een grote chip kan dat seconden duren.
- **`RecordingStop()` geeft false als:**
  - een write van deze opname mislukte;
  - het opnamebestand niet openging;
  - een tussentijdse `sync()` mislukte (`recordingWriteFailed`).
- **Tussentijds wegschrijven:** `StreamingWrite()` gebruikt daarvoor nu `sync()` in plaats van `flush()`, want `flush()` geeft geen resultaat. Een nieuwe `RecordingStart()` begint zonder fout.
- **Wat true nu betekent:** true betekent dat elke write van deze opname lukte, en dat alle open opnamebestanden met `sync()` weggeschreven en daarna gesloten zijn. Wat de kaart fysiek bewaard heeft, bewijst alleen een hardwaretest.
- **`RecordingStop()` en `close()`:** valt de CS uit tijdens `sync()`, dan volgt geen `close()` meer. Bestaat het kaartobject niet meer, dan volgen geen `sync()` en geen `close()`. In beide gevallen wordt het bestandsobject alleen in het geheugen losgelaten, en `RecordingStop()` geeft false.
- **microSD met CS via een Extender, fout tijdens de start:** mislukt de CS twee keer na elkaar tijdens `SdFat32::begin()`, dan mislukt de activering, ook als `begin()` true gaf (`chipSelectWriteFailed`). Een CS-lijn die fysiek vastzit, herstelt geen software; dat blijft een hardwaretest.
- **De voorbeelden `FLASH_StartStopRead` en `microSD_StartStopRead`:**
  - "RESULTAAT: OK" verschijnt alleen als elke meting lukte en elke teruglezing via Data de laatste meting teruggaf.
  - Bij de FLASH kan een teruggelezen record na een mislukte flush ook uit de buffer in RAM komen.
  - Geeft de sensor in de twee opnames dezelfde waarde, dan bewijst de laatste teruglezing niet dat de tweede opname nieuw is.

### Resultaat van RecordingStop en FLASH-streaming

- **`RecordingStop()` geeft true alleen als:**
  - er een opname liep;
  - elke geconfigureerde kaart volledig deelnam;
  - elke write en `sync()` lukte;
  - alle open bestanden gesloten zijn.
- **`RecordingStop()` geeft false bij:**
  - een stop zonder lopende opname, ook een tweede `RecordingStop()` na elkaar;
  - een kaart die bij `RecordingStart()` ontbrak of niet startte, ook door exclusieve toegang van een andere node. Geconfigureerd betekent: elke kaart volgens `DATA_LAYER_MICROSD_SPI_AANTAL`. Een kaart die je niet gebruikt, haal je uit dat aantal;
  - een CS-fout tijdens `close()`.
- **`streamingWriteFailed` (FLASH) wordt true als:**
  - een geconfigureerde FLASH-chip (volgens `DATA_LAYER_FLASH_SPI_AANTAL`) ontbreekt of niet start;
  - een record na een mislukte flush nog steeds niet in de buffer past;
  - de chip vol is;
  - een write of een flush van de buffer mislukt;
  - een gestarte chip uitvalt, gezien bij een nieuw record of bij `StreamingStop()`. Een tijdelijke uitval die herstelt zonder dat de software ze waarneemt, valt hier niet onder; de vlag garandeert dus geen detectie van iedere fysieke uitval.
- **Wanneer de vlag terug false wordt:** bij `StreamingStart()` die een nieuwe streaming begint vanuit `streamingActief == false`. Een nieuwe poging terwijl de streaming via een ander medium al actief is, wist de eerdere fout niet: er kunnen records ontbreken.
- **Wat false betekent:** er werd geen schrijffout en geen mislukte start vastgesteld. Dat bewijst geen duurzame opslag na een herstart.
- **`FLASH_StartStopRead`** meldt "OK" alleen als:
  - de chip startte (`flashStreamingStarted[0]`);
  - er na elke stop geen schrijffout was;
  - elke teruglezing de laatste meting gaf.
- **`StartFlashStreaming()`** geeft false bij een ongeldige chipindex.
- **`SD_CHIP_SELECT_MODE`:** bij microSD-CS via een Extender gebruikt het FrameWork de mode-1-callbacks (`sdCsInit()` en `sdCsWrite()`). Dit is een instelling van de geïnstalleerde SdFat-fork, niet van de aangesloten kaart. Mode 0 wordt in deze configuratie door `microSD.cpp` afgewezen.

### Verdwenen opslagmedia en overgeslagen records

- **microSD:** een geconfigureerde kaart die tijdens een lopende opname niet toegankelijk is, krijgt het record niet. Dat geldt voor een ontbrekende kaart, een kaart met `actief` false of een kaart zonder eigendom. Zo'n overgeslagen record zet nu `recordingWriteFailed`, en `RecordingStop()` geeft dan false, ook als de kaart bij de stop weer in orde is.
- **FLASH, tijdens een record:** een gestarte chip die verdwenen is (`nullptr`), zet `streamingWriteFailed`. Een chip die nooit startte, wordt voor dat record overgeslagen; de mislukte start is al via `streamingWriteFailed` gemeld.
- **FLASH, bij `StreamingStop()`:** ook een gestarte chip die zijn eigendom verloor (alleen met `DATA_LAYER_EXCLUSIEVE_OPSLAG_ACTIEF`), zet `streamingWriteFailed`. Zijn wachtende records worden dan niet weggeschreven.
- **FLASH, een chip wordt afgemeld:** de destructor van de chip zet `flashStreamingStarted` van zijn eigen plaats op false, en `streamingWriteFailed` op true als hij gestart was. Dat gebeurt alleen voor de plaats waar hij zelf staat.
  - Wachtende records in de buffer in RAM van een chip die verdwijnt, vervallen. Ze komen niet op een vervangende chip terecht. `DiscardBufferFLASH()` zet `streamingWriteFailed` als er nog records in die buffer stonden; bij een lege buffer verandert de functie deze vlag niet.
  - Een nieuwe chip die op dezelfde plaats aanmeldt, krijgt pas records na zijn eigen geslaagde start, met de controle van de hele chip.
  - Loopt de streaming nog via een ander medium, dan gebeurt die start bij de volgende `StreamingStart()`.
