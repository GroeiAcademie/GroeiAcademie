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
   └─ microSD  : SPI
```

Per medium is er één bestand met alle aansluitingen van dat medium: `EEPROM.h/.cpp`, `FLASH.h/.cpp`, `FRAM.h/.cpp` en `microSD.h/.cpp`.

`DIRECT` is geen opslagmedium, maar wel een kind van Data (`struct DataDIRECT`, component `DATA_DIRECT`, zonder claims). `DIRECT` gebruikt de Data-class voor real-time doorgeven. Elk channel van een sensor bewaart zijn eigen laatst gemeten waarde (`lastValue`). Met een grootte groter dan `0` worden de waarden bovendien via een RAM-QueueOrStack per channel (FIFO: oudste eerst, of LIFO: nieuwste eerst; de sensor kiest per channel) doorgegeven.

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
```

`DATA_TYPE_NONE` kan niet met een ander Data-type worden gecombineerd. `DATA_TYPE_EEPROM_SPI` en `DATA_TYPE_FRAM_SPI` geven een compileerfout zolang ze niet ondersteund zijn. `typesActief` in Data is daarom 16 bits.

## NONE

`DATA_TYPE_NONE` gebruikt geen Data-class. Voor RFP602 blijft de bestaande route behouden:

```text
RawAnalogRead(...)
-> gemeten waarde
-> return naar toepassing
```

## DIRECT

`DATA_TYPE_DIRECT` gebruikt de Data-class:

```text
RFP602
-> WriteQueueOrStack(sensorNummer, ruweData)   (overgeërfd van Sensor, logica in Data)
-> ReadQueueOrStackStatus(sensorNummer)        0 = waarde klaar, -1 = leeg, -2 = sensor bestaat niet
-> ReadQueueOrStackValue(sensorNummer)         volgende waarde; leeg = lastValue van dat channel
-> toepassing
```

Voor RFP602 bepaalt:

```cpp
RFP602_DATA_LAYER_QUEUE_OR_STACK_SIZE
```

en per channel `RFP602_CHANNEL_1..4_DATA_LAYER_QUEUE_OR_STACK_SIZE` (standaard gelijk aan de waarde hierboven) of een QueueOrStack gebruikt wordt. De QueueOrStack hangt niet af van DIRECT: hij werkt bij elk Data-type zodra de grootte groter is dan 0.

```text
0   = geen QueueOrStack; ReadQueueOrStackValue() geeft lastValue van dat channel terug
> 0 = RAM-QueueOrStack; ReadQueueOrStackValue() haalt de volgende waarde uit de QueueOrStack van dat channel
```

De QueueOrStack-grootte en de keuze FIFO of LIFO horen bij de sensor. Voor andere sensoren wordt hun eigen sensorconfiguratie gebruikt.

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

- R3, R4, STM32: `update()`, dat alleen schrijft wanneer de waarde verandert.
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

FLASH gebruikt SPI en de library `Adafruit SPIFlash`, zonder cache en zonder bestandssysteem: de struct `DataFLASHviaSPI` erft van `Adafruit_SPIFlashBase`. Het transport is eigen code (`TransportFLASHviaSPI`): het schakelt CS en gebruikt de gedeelde SPI-bus met `beginTransaction()`/`endTransaction()`. Daar komt later CS via een Extender bij (O8). Bij activeren wordt de JEDEC-ID teruggelezen als controle.

```cpp
DATA_LAYER_FLASH_SPI_AANTAL
DATA_LAYER_FLASH_SPI_1_CS_PIN
DATA_LAYER_FLASH_SPI_2_CS_PIN
```

SCK, MISO en MOSI komen uit de boardconfiguratie. De chips staan in `Data->FLASHviaSPI[]`.

### FLASH-buffer

```cpp
#define DATA_LAYER_FLASH_SPI_1_BUFFER_SIZE 0     // naam voorlopig; zo ook _2_
```

- `0` (standaard): elke `StreamingWrite()` schrijft meteen naar FLASH. Er wordt geen buffer gecompileerd.
- minstens `sizeof(int)` (`2` op de UNO R3, `4` op de 32-bit boards) t/m `256`: de metingen worden in RAM verzameld en in één keer geschreven wanneer de buffer vol is, bij `StreamingStop()` en bij het afmelden van Data. Bij `StreamingStart()` vervalt wat nog in de buffer zat. `256` is één pagina en geeft de grootste winst.
- Waarom: elke schrijfactie naar FLASH is een volledige "page program" (typisch rond 0,7 ms, aanname uit datasheets), ook voor één meting. Met een buffer van één pagina gebeurt dat één keer per 64 metingen van 4 bytes.
- Kleiner dan één `int` (maar groter dan 0) geeft een compileerfout.
- `StreamingRead()` geeft ook de laatste meting terug als die nog in de buffer wacht.
- Nadeel: bij stroomuitval gaat wat nog in de buffer zit verloren (maximaal de buffergrootte). Op de UNO R3 kost een buffer van 256 bytes te veel RAM.
- EEPROM I2C, FRAM en microSD hebben geen buffer: EEPROM I2C zit nog in geen enkele Data-route, FRAM schrijft zonder wachttijd, en SdFat heeft zelf een buffer van 512 bytes.

Voor continue sensordata geldt:

```text
STREAMING
├─ microSD
└─ FLASH

RECORDING
└─ microSD
```

EEPROM blijft beschikbaar als Data-medium, maar wordt niet gebruikt voor continue sensorstromen.

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

```cpp
DATA_LAYER_MICROSD_SPI_AANTAL
DATA_LAYER_MICROSD_SPI_1_CS_PIN
DATA_LAYER_MICROSD_SPI_2_CS_PIN
```

SCK, MISO en MOSI komen uit de boardconfiguratie. De kaarten staan in `Data->MICROSDviaSPI[]`. Een kaart van 64 GB of meer moet in FAT32 geformatteerd zijn.

### CS via een Extender

FLASH en microSD kunnen CS op een Extender hebben (PCF8574, PCF8575 of MCP23017), met SCK, MISO en MOSI op het board. Per chip drie instellingen (namen voorlopig):

```cpp
#define DATA_LAYER_FLASH_SPI_1_CS_EXTENDER GedeeldeBusComponent::DIGITAL_PINS_PCF8574   // welke Extender
#define DATA_LAYER_FLASH_SPI_1_CS_EXTENDER_I2C_ADDRESS 0x20                           // I2C-adres van die Extender
#define DATA_LAYER_FLASH_SPI_1_CS_PIN ExtenderPCF8574::ExtenderPins::EP_P0             // CS-pin op die Extender
```

Zo ook `_2_` en `MICROSD_SPI`. Data zoekt de Extender in de boom op component en adres; de Extender moet dus vóór `Data` aangemaakt en geactiveerd zijn. Niet gevonden: GB108 (CS ontbreekt). `GEDEELDE_BUS_GEMENGDE_PINNEN` wordt dan automatisch `1`.

- FLASH: het transport schakelt CS via de Extender.
- microSD: SdFat schakelt CS zelf met `sdCsInit()`/`sdCsWrite()`. Die functies kan de library alleen vervangen wanneer in `SdFatConfig.h` van "SdFat - Adafruit Fork" `SD_CHIP_SELECT_MODE` op `1` of `2` staat (standaard `0`). Staat het op `0`, dan geeft de compilatie een duidelijke `#error`. Pas die ene regel in `SdFatConfig.h` aan, of geef `-DSD_CHIP_SELECT_MODE=1` mee als compileerflag.
- Snelheid: elke CS-wissel via een I2C-Extender kost ongeveer 0,05 ms (400 kHz) tot 0,2 ms (100 kHz) (berekend). CS op het board blijft de standaard.
- Op de UNO R3 (testboard) past dit niet in het RAM.

Alternatief (niet gekozen): de originele `SdFat` van Bill Greiman (`#include <SdFat.h>`). Die is de bron van de fork, met dezelfde klassen (`SdFat32`, `File32`, `SdSpiConfig`). Waarom niet: `Adafruit SPIFlash` vereist de fork, en twee SdFat-versies in één programma geven compileerfouten wanneer FLASH en microSD samen gebruikt worden. Overschakelen is mogelijk door in `microSD.h` de include te wijzigen en in `library.properties` `SdFat - Adafruit Fork` door `SdFat` te vervangen; FLASH en microSD samen werken dan niet meer. Het officiële Adafruit-voorbeeld `SdFat_Flash_and_SDcard` gebruikt dezelfde combinatie als de library (fork + Adafruit SPIFlash).

Op de UNO R4 definieert SdFat een eigen `F()`-macro wanneer er nog geen is. De core van de R4 heeft `F()` als functie (en doet zelf `#undef F` in `Arduino.h`). Daarom staat in `microSD.h` na de include van SdFat `#undef F`, alleen op `ARDUINO_ARCH_RENESAS`. Bij elke upgrade van SdFat of van de R4-core opnieuw testen.

Voor RFP602 bepaalt:

```cpp
RFP602_DATA_RECORD_FILE
```

het bestand voor sensordata-recording.

## RAM-QueueOrStack

De QueueOrStack is eigen code in Data (geen externe library). Het geheugen per channel wordt voorlopig door de sensor gereserveerd (RFP602: `ruweDataQueueOrStackchannel1..4`). Is de QueueOrStack vol, dan wordt de oudste waarde overschreven.

De QueueOrStack-grootte wordt per sensor geconfigureerd, los van DIRECT. Voor RFP602:

```cpp
RFP602_DATA_LAYER_QUEUE_OR_STACK_SIZE
```

Gelijktijdige toegang vanuit twee taken of cores vraagt een aparte analyse en synchronisatie; ook FIFO is niet vanzelf veilig.

De QueueOrStack bestaat alleen per sensor-channel. Een tweede QueueOrStack per medium bestaat niet: de oude, nooit gebruikte instellingen `DATA_LAYER_<MEDIUM>_<NR>_QUEUE_OR_STACK_SIZE` zijn verwijderd. Voor FLASH is er in de plaats een schrijfbuffer (zie "FLASH-buffer").

`DATA_QUEUE_OR_STACK_ACTIEF` wordt niet door de gebruiker ingesteld. `SystemConfig.h` leidt deze compile-time waarde automatisch af uit de ingestelde QueueOrStack-groottes.

## Sensorgrens

Een sensor maakt geen medium aan en hoeft niet te weten of zijn data via DIRECT, STREAMING of RECORDING wordt afgehandeld. Wanneer Data actief is, krijgt de sensor bij creatie een pointer naar `Data` mee.

Voor RFP602 komt elke meting in `lastValue` (en, bij een grootte > 0, in de QueueOrStack) van het juiste channel:

```cpp
WriteQueueOrStack(sensorNummer, ruweData);   // overgeërfd van Sensor
```

Bij `DATA_TYPE_NONE` is er geen Data-class in de sensorroute en blijft de directe returnwaarde van `RawAnalogRead(...)` actief.

## Data-interface

```cpp
Data->StreamingStart();
Data->StreamingWrite(...);
Data->StreamingRead(channel);        // SensorQueueOrStack&: laatste waarde uit FLASH, anders lastValue van dit channel
Data->StreamingStop();

Data->RecordingStart();
Data->RecordingWrite(...);
Data->RecordRead(channel);           // SensorQueueOrStack&: eerste waarde uit het logbestand, anders lastValue van dit channel
Data->RecordingStop();

sensor->StreamingRead(sensorNummer); // zelfde, via de sensor; sensor bestaat niet = 0
sensor->RecordRead(sensorNummer);
```

`StreamingRead` leest vandaag de laatst geschreven waarde over alle channels heen (één gemengde stroom). Per channel lezen hoort bij O43.

Per medium houdt Data een lijst van zijn chips bij: `Data->EEPROMviaNative`, `Data->EEPROMviaI2C[]`, `Data->FLASHviaSPI[]`, `Data->FRAMviaI2C[]`, `Data->MICROSDviaSPI[]`. Een lege plaats is `nullptr`. Elke chip wist bij het verwijderen zelf zijn plaats in de lijst (destructor).

Data faalt alleen wanneer geen enkel kind overblijft. Een medium dat niet werkt, wordt afgemeld; de andere media blijven werken. DIRECT is ook een kind.

microSD schrijft na `DATA_LAYER_MICROSD_SPI_FLUSH_NA_AANTAL_REGEL` regels, of na `DATA_LAYER_MICROSD_SPI_FLUSH_NA_MS` milliseconden sinds de vorige flush, weg naar de kaart. Roep `RecordingStop()` aan voor je de stroom onderbreekt.

De sensor gebruikt STREAMING en RECORDING niet rechtstreeks. De toepassing kiest die werking. De concrete media behouden daarnaast hun volledige geërfde library-API.

## Sensorvoorbeelden

```text
Sensor_RFP602.ino
-> DATA_TYPE_NONE
-> oude werking zonder Data-class

Sensor_RFP602_DIRECT.ino
-> DATA_TYPE_DIRECT
-> ReadQueueOrStackValue(sensorNummer)
-> optionele QueueOrStack per channel

Sensor_RFP602_FLASH.ino
-> DATA_TYPE_FLASH_SPI
-> Data STREAMING naar FLASH
-> sensorRFP602->StreamingRead(sensorNummer)

Sensor_RFP602_MICROSD.ino
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

`REDIRECT_SERIAL_NONE` betekent geen redirect. `GA_SERIAL` blijft de bestaande `Serial` of `Serial0` gebruiken.

`REDIRECT_SERIAL_TO_MICROSD` maakt de noodzakelijke Data/microSD-route automatisch beschikbaar. De toepassing hoeft daarvoor niet zelf `DATA_TYPE_MICROSD_SPI` aan `DATA_LAYER_CONFIG` toe te voegen en hoeft niet zelf `Data` aan te maken. De output wordt via de door `Data` aangemaakte microSD-route naar `REDIRECT_SERIAL_FILE` geschreven. Er wordt niet tegelijk naar de fysieke Serial geschreven.


Bij REDIRECT_SERIAL_TO_MICROSD moet GA_SERIAL.begin() aangeroepen worden vóór GedeeldeBusNewComponent<struct Screen>().
