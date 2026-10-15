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
   ├─ EEPROM   : I2C
   ├─ FLASH    : SPI
   ├─ FRAM     : I2C
   └─ microSD  : SPI
```

`DIRECT` is geen opslagmedium. `DIRECT` gebruikt de Data-class voor real-time doorgeven. Elk channel van een sensor bewaart zijn eigen laatst gemeten waarde (`lastValue`). Met een grootte groter dan `0` worden de waarden bovendien via een RAM-QueueOrStack per channel (FIFO: oudste eerst, of LIFO: nieuwste eerst; de sensor kiest per channel) doorgegeven.

## Configuratie

`DATA_LAYER_CONFIG` bepaalt welke Data-werking in deze build aanwezig is.

```cpp
#define DATA_LAYER_CONFIG DATA_TYPE_NONE
#define DATA_LAYER_CONFIG DATA_TYPE_DIRECT
#define DATA_LAYER_CONFIG DATA_TYPE_EEPROM | DATA_TYPE_FLASH | DATA_TYPE_FRAM | DATA_TYPE_MICROSD
```

De huidige typewaarden zijn:

```cpp
#define DATA_TYPE_NONE     0x01
#define DATA_TYPE_DIRECT   0x02
#define DATA_TYPE_EEPROM   0x04
#define DATA_TYPE_FLASH    0x08
#define DATA_TYPE_FRAM     0x10
#define DATA_TYPE_MICROSD  0x20
```

`DATA_TYPE_NONE` kan niet met een ander Data-type worden gecombineerd.

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
RFP602_DATA_LAYER_DIRECT_QUEUE_OR_STACK_SIZE
```

en per channel `RFP602_CHANNEL_1..4_DATA_LAYER_DIRECT_QUEUE_OR_STACK_SIZE` (standaard gelijk aan de waarde hierboven) of DIRECT een QueueOrStack gebruikt.

```text
0   = geen QueueOrStack; ReadQueueOrStackValue() geeft lastValue van dat channel terug
> 0 = RAM-QueueOrStack; ReadQueueOrStackValue() haalt de volgende waarde uit de QueueOrStack van dat channel
```

De QueueOrStack-grootte en de keuze FIFO of LIFO horen bij de sensor. Voor andere sensoren wordt hun eigen sensorconfiguratie gebruikt.

## EEPROM

EEPROM gebruikt I2C en de library `I2C_EEPROM` van Rob Tillaart.

```cpp
DATA_LAYER_EEPROM_AANTAL
DATA_LAYER_EEPROM_1_I2C_ADDRESS
DATA_LAYER_EEPROM_2_I2C_ADDRESS
```

SDA en SCL komen uit de boardconfiguratie.

## FLASH

FLASH gebruikt SPI en de huidige kandidaat-library `SPIMemory`. De concrete libraryclass is `SPIFlash`. De librarykeuze moet op de volledige bestaande boardmatrix getest worden. Er worden geen boards op voorhand uitgesloten.

```cpp
DATA_LAYER_FLASH_AANTAL
DATA_LAYER_FLASH_1_CS_PIN
DATA_LAYER_FLASH_2_CS_PIN
```

SCK, MISO en MOSI komen uit de boardconfiguratie.

Voor continue sensordata geldt:

```text
STREAMING
├─ microSD
└─ FLASH

RECORDING
└─ microSD
```

EEPROM blijft beschikbaar als Data-medium, maar wordt niet gebruikt voor continue sensorstromen.

## FRAM_I2C

FRAM_I2C gebruikt I2C en de library `FRAM_I2C` van Rob Tillaart. De concrete libraryclass is `FRAM`.

De huidige referentie is de Fujitsu MB85RC256V van 32 KB. Volgens de officiële librarydocumentatie gebruikt dit type de class `FRAM`, heeft het standaard I2C-adres `0x50` en gebruikt het 16-bits geheugenadressen.

```cpp
DATA_LAYER_FRAM_AANTAL
DATA_LAYER_FRAM_1_I2C_ADDRESS
DATA_LAYER_FRAM_2_I2C_ADDRESS
```

SDA en SCL komen uit de boardconfiguratie. `FRAM_I2C` behoudt de volledige geërfde `FRAM`-API voor lezen, schrijven, metadata en foutstatus.

FRAM_I2C is in deze stap alleen als Data-medium geïntegreerd. De bestaande STREAMING- en RECORDING-routing wordt niet uitgebreid.

## microSD

microSD gebruikt SPI en de officiële Arduino library `SD`.

```cpp
DATA_LAYER_MICROSD_AANTAL
DATA_LAYER_MICROSD_1_CS_PIN
DATA_LAYER_MICROSD_2_CS_PIN
```

SCK, MISO en MOSI komen uit de boardconfiguratie.

Voor RFP602 bepaalt:

```cpp
RFP602_DATA_RECORD_FILE
```

het bestand voor sensordata-recording.

## RAM-QueueOrStack

De QueueOrStack is eigen code in Data (geen externe library). Het geheugen per channel wordt voorlopig door de sensor gereserveerd (RFP602: `ruweDataQueueOrStackchannel1..4`). Is de QueueOrStack vol, dan wordt de oudste waarde overschreven.

Voor DIRECT wordt de QueueOrStack-grootte per sensor geconfigureerd. Voor RFP602:

```cpp
RFP602_DATA_LAYER_DIRECT_QUEUE_OR_STACK_SIZE
```

De bestaande media-instellingen per concrete Data-instantie staan nog in de config, maar zijn **nog niet in gebruik**: geen enkele code leest ze.

```cpp
DATA_LAYER_EEPROM_1_QUEUE_OR_STACK_SIZE
DATA_LAYER_EEPROM_2_QUEUE_OR_STACK_SIZE
DATA_LAYER_FRAM_1_QUEUE_OR_STACK_SIZE
DATA_LAYER_FRAM_2_QUEUE_OR_STACK_SIZE
DATA_LAYER_MICROSD_1_QUEUE_OR_STACK_SIZE
DATA_LAYER_MICROSD_2_QUEUE_OR_STACK_SIZE
DATA_LAYER_FLASH_1_QUEUE_OR_STACK_SIZE
DATA_LAYER_FLASH_2_QUEUE_OR_STACK_SIZE
```

`DATA_QUEUE_OR_STACK_ACTIEF` wordt niet door de gebruiker ingesteld. `SystemConfig.h` leidt deze compile-time waarde automatisch af uit de ingestelde QueueOrStack-groottes.

## Sensorgrens

Een sensor maakt geen medium aan en hoeft niet te weten of zijn data via DIRECT, STREAMING of RECORDING wordt afgehandeld. Wanneer Data actief is, krijgt de sensor bij creatie een pointer naar `Data` mee.

Voor RFP602:

```cpp
Data->SensorWrite(ruweData);
Data->SensorRead();
```

Bij `DATA_TYPE_NONE` is er geen Data-class in de sensorroute en blijft de directe returnwaarde van `RawAnalogRead(...)` actief.

## Data-interface

```cpp
Data->SensorWrite(...);
Data->SensorRead();

Data->StreamingStart();
Data->StreamingWrite(...);
Data->StreamingRead();
Data->StreamingStop();

Data->RecordingStart();
Data->RecordingWrite(...);
Data->RecordRead();
Data->RecordingStop();
```

Data faalt alleen wanneer geen enkel gevraagd medium overblijft. Een medium dat niet werkt, wordt afgemeld; de andere media blijven werken. DIRECT (RAM-QueueOrStack) is geen medium op de bus en blijft altijd over.

microSD schrijft na `DATA_LAYER_MICROSD_FLUSH_NA_AANTAL_REGEL` regels, of na `DATA_LAYER_MICROSD_FLUSH_NA_MS` milliseconden sinds de vorige flush, weg naar de kaart. Roep `RecordingStop()` aan voor je de stroom onderbreekt.

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
-> DATA_TYPE_FLASH
-> Data STREAMING naar FLASH

Sensor_RFP602_MICROSD.ino
-> DATA_TYPE_MICROSD
-> Data RECORDING naar microSD
```

## Media-tests

De gewone media-validatietests maken `Data` aan en gebruiken daarna het door `Data` aangemaakte concrete child-object. Ze maken hetzelfde medium niet nogmaals aan.

```text
examples/Systeem/Data/EEPROM/EEPROM.ino
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

`REDIRECT_SERIAL_TO_MICROSD` maakt de noodzakelijke Data/microSD-route automatisch beschikbaar. De toepassing hoeft daarvoor niet zelf `DATA_TYPE_MICROSD` aan `DATA_LAYER_CONFIG` toe te voegen en hoeft niet zelf `Data` aan te maken. De output wordt via de door `Data` aangemaakte microSD-route naar `REDIRECT_SERIAL_FILE` geschreven. Er wordt niet tegelijk naar de fysieke Serial geschreven.


Bij REDIRECT_SERIAL_TO_MICROSD moet GA_SERIAL.begin() aangeroepen worden vóór GedeeldeBusNewComponent<struct Screen>().
