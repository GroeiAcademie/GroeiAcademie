# Architectuur

## Status en bron van waarheid

Dit document beschrijft de actuele broncode van **beta v2.0.0**. Voor de technische architectuur geldt de code onder `src/` als bron van waarheid. Historische release-informatie blijft in `CHANGELOG.md`, `docs/DECISION_LOG.md` en `extras/TESTRESULTATEN.md` staan.

## Eén geïntegreerde library

GroeiAcademie FrameWork wordt als één samenwerkende Arduino-library ontwikkeld. Functionele onderdelen worden niet zonder expliciete reden opgesplitst in afzonderlijke libraries.

## Huidige hoofdstructuur

```text
GroeiAcademie/
├── src/
│   ├── Configuratie/
│   │   ├── Extenders/
│   │   │   ├── ConfigADS1115.h
│   │   │   ├── ConfigADS1115_template.h
│   │   │   ├── ConfigADS1158.h
│   │   │   ├── ConfigADS1158_template.h
│   │   │   ├── ConfigADS7828.h
│   │   │   ├── ConfigADS7828_template.h
│   │   │   ├── ConfigADS7953.h
│   │   │   ├── ConfigADS7953_template.h
│   │   │   ├── ConfigCD74HC4067.h
│   │   │   ├── ConfigCD74HC4067_template.h
│   │   │   ├── ConfigDS2482v800.h
│   │   │   ├── ConfigDS2482v800_template.h
│   │   │   ├── ConfigMAX14830I2C.h
│   │   │   ├── ConfigMAX14830I2C_template.h
│   │   │   ├── ConfigMAX14830SPI.h
│   │   │   ├── ConfigMAX14830SPI_template.h
│   │   │   ├── ConfigMCP23017.h
│   │   │   ├── ConfigMCP23017_template.h
│   │   │   ├── ConfigMCP23S17.h
│   │   │   ├── ConfigMCP23S17_template.h
│   │   │   ├── ConfigPCA9685.h
│   │   │   ├── ConfigPCA9685_template.h
│   │   │   ├── ConfigPCF8574.h
│   │   │   ├── ConfigPCF8574_template.h
│   │   │   ├── ConfigPCF8575.h
│   │   │   ├── ConfigPCF8575_template.h
│   │   │   ├── ConfigSC16IS752I2C.h
│   │   │   ├── ConfigSC16IS752I2C_template.h
│   │   │   ├── ConfigSC16IS752SPI.h
│   │   │   ├── ConfigSC16IS752SPI_template.h
│   │   │   ├── ConfigTCA9548A.h
│   │   │   └── ConfigTCA9548A_template.h
│   │   ├── Sensoren/
│   │   │   ├── ConfigRFP602.h
│   │   │   └── ConfigRFP602_template.h
│   │   ├── Sturingen/
│   │   │   ├── ConfigA4988_template.h
│   │   │   ├── ConfigDCmotorL293D_template.h
│   │   │   ├── ConfigDCmotorL298N_template.h
│   │   │   ├── ConfigMG996R_template.h
│   │   │   ├── ConfigRelais_template.h
│   │   │   ├── ConfigRGBLED_template.h
│   │   │   ├── ConfigSG90_template.h
│   │   │   ├── ConfigULN2003_template.h
│   │   │   └── ConfigWS2812_template.h
│   │   ├── Examples.h
│   │   ├── ExamplesConfig.h
│   │   ├── StimulusConfig.h
│   │   ├── SystemConfig.h
│   │   ├── UserConfig.h
│   │   └── UserConfig_template.h
│   ├── Language/
│   │   ├── Examples_DE.h
│   │   ├── Examples_EN.h
│   │   ├── Examples_FR.h
│   │   ├── Examples_NL.h
│   │   ├── Library_DE.h
│   │   ├── Library_EN.h
│   │   ├── Library_FR.h
│   │   ├── Library_NL.h
│   │   ├── UserExample_DE_template.h
│   │   ├── UserExample_EN_template.h
│   │   ├── UserExample_FR_template.h
│   │   ├── UserExample_NL_template.h
│   │   ├── UserLibrary_DE_template.h
│   │   ├── UserLibrary_EN_template.h
│   │   ├── UserLibrary_FR_template.h
│   │   └── UserLibrary_NL_template.h
│   ├── Systeem/
│   │   ├── Data/
│   │   │   ├── Data.cpp
│   │   │   ├── Data.h
│   │   │   ├── EEPROM.cpp
│   │   │   ├── EEPROM.h
│   │   │   ├── FLASH.cpp
│   │   │   ├── FLASH.h
│   │   │   ├── FRAM.cpp
│   │   │   ├── FRAM.h
│   │   │   ├── microSD.cpp
│   │   │   ├── microSD.h
│   │   │   ├── RedirectSerial.cpp
│   │   │   └── RedirectSerial.h
│   │   ├── GedeeldeBus/
│   │   │   ├── GedeeldeBus.cpp
│   │   │   └── GedeeldeBus.h
│   │   ├── Extender/
│   │   │   ├── ADS1115.cpp
│   │   │   ├── ADS1115.h
│   │   │   ├── ADS1158.cpp
│   │   │   ├── ADS1158.h
│   │   │   ├── ADS7828.cpp
│   │   │   ├── ADS7828.h
│   │   │   ├── ADS7953.cpp
│   │   │   ├── ADS7953.h
│   │   │   ├── CD74HC4067.cpp
│   │   │   ├── CD74HC4067.h
│   │   │   ├── DS2482v800.cpp
│   │   │   ├── DS2482v800.h
│   │   │   ├── MAX14830I2C.cpp
│   │   │   ├── MAX14830I2C.h
│   │   │   ├── MAX14830SPI.cpp
│   │   │   ├── MAX14830SPI.h
│   │   │   ├── MCP23017.cpp
│   │   │   ├── MCP23017.h
│   │   │   ├── MCP23S17.cpp
│   │   │   ├── MCP23S17.h
│   │   │   ├── PCA9685.cpp
│   │   │   ├── PCA9685.h
│   │   │   ├── PCF8574.cpp
│   │   │   ├── PCF8574.h
│   │   │   ├── PCF8575.cpp
│   │   │   ├── PCF8575.h
│   │   │   ├── SC16IS752I2C.cpp
│   │   │   ├── SC16IS752I2C.h
│   │   │   ├── SC16IS752SPI.cpp
│   │   │   ├── SC16IS752SPI.h
│   │   │   ├── TCA9548A.cpp
│   │   │   └── TCA9548A.h
│   │   ├── Sensor/
│   │   │   ├── RFP602.cpp
│   │   │   └── RFP602.h
│   │   ├── Sturing/
│   │   │   ├── A4988.h
│   │   │   ├── DCmotorL293D.h
│   │   │   ├── DCmotorL298N.h
│   │   │   ├── MG996R.h
│   │   │   ├── Relais.h
│   │   │   ├── RGBLED.h
│   │   │   ├── SG90.h
│   │   │   ├── ULN2003.h
│   │   │   └── WS2812.h
│   │   ├── Input/
│   │   │   ├── Input.cpp
│   │   │   ├── Input.h
│   │   │   └── InputTypes.h
│   │   └── Screen/
│   │       ├── Screen.cpp
│   │       ├── Screen.h
│   │       └── ScreenTypes.h
│   ├── Toepassingsgebieden/
│   │   └── Stimulus/
│   │       ├── Stimulus.cpp
│   │       └── Stimulus.h
│   ├── Uitbreidingskaarten/
│   ├── Data.h
│   ├── GroeiAcademie.h
│   ├── Input.h
│   ├── Screen.h
│   ├── Stimulus.h
│   └── SystemConfig.h
├── examples/
│   ├── Systeem/
│   │   ├── Data/
│   │   │   ├── EEPROM/
│   │   │   ├── FLASH/
│   │   │   ├── FRAM/
│   │   │   └── microSD/
│   │   ├── GedeeldeBus/
│   │   │   ├── Extenders/
│   │   │   │   ├── Extender_ADS1115/
│   │   │   │   ├── Extender_ADS1115_Meetkwaliteit/
│   │   │   │   ├── Extender_ADS1115_Test_ExtenderPins_Geweigerd/
│   │   │   │   ├── Extender_ADS1115_Test_ExtenderPins_Toegelaten/
│   │   │   │   ├── Extender_ADS1158/
│   │   │   │   ├── Extender_ADS1158_Test_ExtenderPins_Geweigerd/
│   │   │   │   ├── Extender_ADS1158_Test_ExtenderPins_Toegelaten/
│   │   │   │   ├── Extender_ADS7828/
│   │   │   │   ├── Extender_ADS7828_Test_ExtenderPins_Geweigerd/
│   │   │   │   ├── Extender_ADS7828_Test_ExtenderPins_Toegelaten/
│   │   │   │   ├── Extender_ADS7953/
│   │   │   │   ├── Extender_ADS7953_Test_ExtenderPins_Geweigerd/
│   │   │   │   ├── Extender_ADS7953_Test_ExtenderPins_Toegelaten/
│   │   │   │   ├── Extender_CD74HC4067/
│   │   │   │   ├── Extender_CD74HC4067_Test_ExtenderPins_Geweigerd/
│   │   │   │   ├── Extender_CD74HC4067_Test_ExtenderPins_Toegelaten/
│   │   │   │   ├── Extender_DS2482v800/
│   │   │   │   ├── Extender_DS2482v800_Test_ExtenderPins_Geweigerd/
│   │   │   │   ├── Extender_DS2482v800_Test_ExtenderPins_Toegelaten/
│   │   │   │   ├── Extender_MAX14830_I2C/
│   │   │   │   ├── Extender_MAX14830_I2C_Test_ExtenderPins_Geweigerd/
│   │   │   │   ├── Extender_MAX14830_I2C_Test_ExtenderPins_Toegelaten/
│   │   │   │   ├── Extender_MAX14830_SPI/
│   │   │   │   ├── Extender_MAX14830_SPI_Test_ExtenderPins_Geweigerd/
│   │   │   │   ├── Extender_MAX14830_SPI_Test_ExtenderPins_Toegelaten/
│   │   │   │   ├── Extender_MCP23017/
│   │   │   │   ├── Extender_MCP23017_Test_ExtenderPins_Geweigerd/
│   │   │   │   ├── Extender_MCP23017_Test_ExtenderPins_Toegelaten/
│   │   │   │   ├── Extender_MCP23S17/
│   │   │   │   ├── Extender_PCA9685/
│   │   │   │   ├── Extender_PCA9685_Test_OE_Pin/
│   │   │   │   ├── Extender_PCF8574/
│   │   │   │   ├── Extender_PCF8574_Test_ExtenderPins_Geweigerd/
│   │   │   │   ├── Extender_PCF8574_Test_ExtenderPins_Toegelaten/
│   │   │   │   ├── Extender_PCF8575/
│   │   │   │   ├── Extender_PCF8575_Test_ExtenderPins_Geweigerd/
│   │   │   │   ├── Extender_PCF8575_Test_ExtenderPins_Toegelaten/
│   │   │   │   ├── Extender_SC16IS752_I2C/
│   │   │   │   ├── Extender_SC16IS752_I2C_Test_ExtenderPins_Geweigerd/
│   │   │   │   ├── Extender_SC16IS752_I2C_Test_ExtenderPins_Toegelaten/
│   │   │   │   ├── Extender_SC16IS752_SPI/
│   │   │   │   ├── Extender_SC16IS752_SPI_Test_ExtenderPins_Geweigerd/
│   │   │   │   ├── Extender_SC16IS752_SPI_Test_ExtenderPins_Toegelaten/
│   │   │   │   ├── Extender_TCA9548A/
│   │   │   │   ├── Extender_TCA9548A_Test_ExtenderPins_Geweigerd/
│   │   │   │   └── Extender_TCA9548A_Test_ExtenderPins_Toegelaten/
│   │   │   ├── Sensoren/
│   │   │   │   ├── Sensor_MixedPins/
│   │   │   │   ├── Sensor_RFP602/
│   │   │   │   ├── Sensor_RFP602_DIRECT/
│   │   │   │   ├── Sensor_RFP602_FLASH/
│   │   │   │   ├── Sensor_RFP602_MICROSD/
│   │   │   │   ├── Sensor_RFP602_Test_ADC_Bezet/
│   │   │   │   ├── Sensor_RFP602_Test_EigenBuffers/
│   │   │   │   ├── Sensor_RFP602_Test_Hardwarefout/
│   │   │   │   ├── Sensor_RFP602_Test_OngeldigePin/
│   │   │   │   ├── Sensor_RFP602_Test_Timeout_Conversie/
│   │   │   │   ├── Sensor_RFP602_Test_VerkeerdeParent/
│   │   │   │   ├── Sensor_RFP602_Test_ZonderGet/
│   │   │   │   └── Sensor_RFP602_TweeSetsOpTweeADS1115/
│   │   │   └── Sturingen/
│   │   │       ├── Sturing_A4988/
│   │   │       ├── Sturing_DCmotorL293D/
│   │   │       ├── Sturing_DCmotorL298N/
│   │   │       ├── Sturing_MG996R/
│   │   │       ├── Sturing_Relais/
│   │   │       ├── Sturing_RGBLED/
│   │   │       ├── Sturing_SG90/
│   │   │       ├── Sturing_ULN2003/
│   │   │       └── Sturing_WS2812/
│   │   ├── Input/
│   │   │   ├── InputkanalenHX1838/
│   │   │   ├── InputkanalenHX1838metArgumenten/
│   │   │   ├── InputkanalenHX1838UserDefined/
│   │   │   ├── InputkanalenPCF8574/
│   │   │   ├── InputkanalenPCF8574metArgumenten/
│   │   │   ├── InputkanalenPCF8574UserDefinedDirect/
│   │   │   └── InputkanalenPCF8574UserDefinedMatrix/
│   │   ├── Screen/
│   │   │   ├── Callback_CharacterScreen/
│   │   │   ├── Callback_PixelScreen/
│   │   │   ├── Default_CharacterScreen/
│   │   │   ├── Default_CharacterScreen_PixelScreen/
│   │   │   └── Default_PixelScreen/
│   │   └── Tools/
│   │       └── I2C/
│   │           ├── I2C_DiagnoseAangemeldeDevices/
│   │           ├── I2C_DiagnoseDS2482v800/
│   │           ├── I2C_DiagnoseTCA9548A/
│   │           ├── I2C_MetenHerstelEnBlokkeertijd/
│   │           ├── I2C_MetenReactietijdEnBetrouwbaarheid/
│   │           ├── I2C_OpsporenNietAangemeldeDevices/
│   │           ├── I2C_Scan/
│   │           ├── I2C_TestenKloksnelhedenAangemeldeDevices/
│   │           └── I2C_TotaleDiagnoseAlleDevices/
│   └── Toepassingsgebieden/
│       └── Stimulus/
│           ├── Scenario1_EnkelTik/
│           ├── Scenario2_Simultaan/
│           ├── Scenario3_Ineenstortend/
│           ├── Scenario4_Cocktail/
│           └── Tik_Enkele_Samen_Instortend_Cocktail/
├── docs/
│   ├── Configuratie/
│   │   ├── ExamplesConfig.md
│   │   ├── README.md
│   │   ├── StimulusConfig.md
│   │   ├── SystemConfig.md
│   │   ├── UserConfig.md
│   │   └── UserLanguage.md
│   ├── Systeem/
│   │   ├── Extender/
│   │   │   ├── ADS1115.md
│   │   │   ├── ADS1158.md
│   │   │   ├── ADS7828.md
│   │   │   ├── ADS7953.md
│   │   │   ├── CD74HC4067.md
│   │   │   ├── DS2482v800.md
│   │   │   ├── MAX14830I2C.md
│   │   │   ├── MAX14830SPI.md
│   │   │   ├── MCP23017.md
│   │   │   ├── MCP23S17.md
│   │   │   ├── PCA9685.md
│   │   │   ├── PCF8574.md
│   │   │   ├── PCF8575.md
│   │   │   ├── SC16IS752I2C.md
│   │   │   ├── SC16IS752SPI.md
│   │   │   └── TCA9548A.md
│   │   ├── Sensor/
│   │   │   └── RFP602.md
│   │   ├── Sturing/
│   │   │   ├── A4988.md
│   │   │   ├── DCmotorL293D.md
│   │   │   ├── DCmotorL298N.md
│   │   │   ├── MG996R.md
│   │   │   ├── Relais.md
│   │   │   ├── RGBLED.md
│   │   │   ├── SG90.md
│   │   │   ├── ULN2003.md
│   │   │   └── WS2812.md
│   │   ├── DATA.md
│   │   ├── GEDEELDEBUS.md
│   │   ├── INPUT.md
│   │   ├── SCREEN.md
│   │   └── SCREEN_FOUTCODES.md
│   ├── Toepassingsgebieden/
│   │   ├── Stimulus/
│   │   │   └── README.md
│   │   ├── MODULES.md
│   │   ├── SENSOR_INVENTARIS.md
│   │   └── VEILIGHEID.md
│   ├── Uitbreidingskaarten/
│   │   └── Stimulus Shield v2.0.0/
│   │       ├── Handleiding-Stimulus-Shield-(GroeiAcademie-FrameWork)-v2.0.0.md
│   │       └── Stimulus-Shield-(GroeiAcademie-FrameWork)-v2.0.0.md
│   ├── Pilots/
│   │   └── v1_2_0_sensoren_pilot.md
│   ├── ARCHITECTURE.md
│   ├── COMMUNITY_GUIDE.md
│   ├── DECISION_LOG.md
│   ├── DESIGN_PHILOSOPHY.md
│   ├── HARDWARE_SUPPORT.md
│   ├── PROJECT_VALUES.md
│   ├── RESEARCH_PHILOSOPHY.md
│   ├── ROADMAP.md
│   ├── SCIENTIFIC_INTEGRITY.md
│   └── TERMINOLOGY.md
├── extras/
├── .github/
├── .gitattributes
├── .gitignore
├── AUTHORS.md
├── CHANGELOG.md
├── CITATION.cff
├── CODE_OF_CONDUCT.md
├── CONTRIBUTING.md
├── CONTRIBUTORS.md
├── DISCLAIMER.md
├── GOVERNANCE.md
├── keywords.txt
├── library.properties
├── LICENSE
├── LICENSE.md
├── LICENSE_KEUZEGIDS.md
├── oproep-sponsors-contributors.md
├── PROJECT_CONSTITUTION.md
├── README.md
├── SECURITY.md
├── SPONSORS.md
└── TRADEMARKS.md
```

`src/Uitbreidingskaarten/` is momenteel een gereserveerde broncodemap. De hardwaredocumentatie van de Stimulus Shield staat wel onder `docs/Uitbreidingskaarten/`.

## Publieke headers

Gebruikers kunnen de volledige library opnemen met:

```cpp
#include <GroeiAcademie.h>
```

`src/GroeiAcademie.h` neemt momenteel `Data.h`, `Screen.h`, `Stimulus.h` en `Input.h` op. Daarnaast zijn de publieke moduleheaders rechtstreeks beschikbaar:

```cpp
#include <Data.h>
#include <Input.h>
#include <Screen.h>
#include <Stimulus.h>
#include <SystemConfig.h>
```

De headers direct onder `src/` leiden door naar de interne moduleheaders. GedeeldeBus en de concrete Sensor-, Extender- en Sturingheaders zijn in de huidige code interne systeemheaders; voorbeelden die een concrete Sensor rechtstreeks testen nemen die interne header expliciet op.

## Documentatie van componenten

De actuele componentdocumentatie is onderverdeeld volgens de broncode:

- [Extenders](Systeem/Extender/): 16 componentdocumenten, met korte bestandsnamen zoals `ADS1115.md`.
- [Sensoren](Systeem/Sensor/RFP602.md): `RFP602.md`.
- [Sturingen](Systeem/Sturing/): negen componentdocumenten voor A4988, DCmotorL293D, DCmotorL298N, MG996R, Relais, RGBLED, SG90, ULN2003 en WS2812.

De Sturingen zijn nog ongetest; hun documentatie beschrijft uitsluitend de bestaande codevoorstellen.

## Architectuurgrens van GedeeldeBus

GedeeldeBus is de gemeenschappelijke boom- en resourcebeheerlaag. De generieke kern staat in `src/Systeem/GedeeldeBus/GedeeldeBus.h/.cpp`. Concrete Extenders staan in `src/Systeem/Extender/`, Sensoren in `src/Systeem/Sensor/` en Sturingen in `src/Systeem/Sturing/`. Ze gebruiken de bestaande generieke GedeeldeBus-basis.

De actuele boom gebruikt één `GedeeldeBusNode`-structuur voor root-, parent- en childnodes. `Native` is de root. Top-level componenten hangen aan `Native`; een Sensor kan als child van een ADC-extender hangen; Input en Screen maken zelf children aan voor hun concrete kanalen.

De code gebruikt geen centrale array met een vast maximum. Elke node bevat `parent`, `firstChild`, `nextChild` en `prevChild` en krijgt bij constructie een uniek `id` via `volgendeGedeeldeBusId`.

## Data

Data is een top-level GedeeldeBus-component onder `Native`. De Data-componentcodes staan vóór het Extender-bereik: `GC_DATA` (`0x14`), `DATA_DIRECT` (`0x15`), `DATA_EEPROM` (`0x16`), `DATA_FLASH` (`0x17`), `DATA_FRAM` (`0x18`) en `DATA_MICROSD` (`0x19`), in de volgorde van de Data-bits; `0x1A` t/m `0x1F` zijn vrij; `0x20` t/m `0x7F` blijft het functionele Extender-bereik. De gewone toepassing maakt `Data` aan; `Data` maakt zelf de geselecteerde concrete media aan en voert hun lifecycle uit.

```text
DATA_TYPE_NONE
└─ sensor -> toepassing

Native
└─ Data
   ├─ DIRECT
   ├─ EEPROM   : I2C
   ├─ FRAM     : I2C
   ├─ FLASH    : SPI
   └─ microSD  : SPI
```

`DATA_LAYER_CONFIG` bepaalt welke types gecompileerd zijn. `DATA_TYPE_NONE` behoudt de oude route zonder Data-class. Net als bij Input kan bij `GedeeldeBusNewComponent<struct Data>(...)` een subset als objectfilter worden meegegeven. `DIRECT` is geen opslagmedium. Zonder QueueOrStack bewaart DIRECT per channel de laatst gemeten waarde; met de sensor-specifieke QueueOrStack (FIFO of LIFO) loopt de real-time gegevensstroom via eigen code in Data. EEPROM, FRAM_I2C, FLASH en microSD zijn concrete children van Data.

Sensoren maken geen media aan. Een sensor krijgt bij creatie een pointer naar `Data` mee en levert alleen ruwe data aan via `WriteQueueOrStack(...)`, die elke sensor van `Sensor` erft: de waarde komt in `lastValue` (en eventueel de QueueOrStack) van het juiste channel. Lezen gebeurt per channel via `ReadQueueOrStackStatus(...)`, `ReadQueueOrStackValue(...)`, `StreamingRead(...)` en `RecordRead(...)`, die elke sensor eveneens van `Sensor` erft. Elk medium staat in één bestand met al zijn aansluitingen (`EEPROM`, `FLASH`, `FRAM`, `microSD`); DIRECT is een kind van Data (`DataDIRECT`). STREAMING, RECORDING, mediumkeuze, QueueOrStack en eventuele metadata blijven verantwoordelijkheid van `Data` en de applicatie.

De publieke Data-interface voor STREAMING en RECORDING gebruikt de vastgelegde namen `StreamingStart(...)`, `StreamingWrite(...)`, `StreamingRead(...)`, `StreamingStop(...)`, `RecordingStart(...)`, `RecordingWrite(...)`, `RecordRead(...)` en `RecordingStop(...)`. Deze functies horen bij `Data`; ze horen niet bij de sensor. Concrete media behouden daarnaast de volledige API die zij van hun gebruikte library erven.

Voor continue sensordata gebruikt STREAMING in deze fase microSD of FLASH. RECORDING gebruikt microSD. EEPROM blijft als Data-medium beschikbaar, maar niet voor continue sensorstromen.

Serial-redirect grijpt centraal aan op `GA_SERIAL`. `REDIRECT_SERIAL_NONE` behoudt de gewone `Serial`/`Serial0`-route. `REDIRECT_SERIAL_TO_MICROSD` gebruikt via `RedirectSerial` het door Data aangemaakte microSD-medium en schrijft de seriële bytes naar `REDIRECT_SERIAL_FILE`, standaard `"serial.log"`. De Serial-redirect gebruikt een eigen bestandshandle en blijft gescheiden van sensordata-recording.

Zie [Data](Systeem/DATA.md).

## Lifecycle

De generieke lifecycle bestaat uit:

```text
aanmelden()
→ controleren()
→ inpluggen()
→ activeren()
```

`GedeeldeBusNewComponent<T>(...)` maakt een object met `new`, zet `componentCreated = true` en voert exact deze vier stappen uit. Wanneer een stap faalt, wordt de tijdelijke conflictinformatie afgehandeld en wordt het object afgemeld/verwijderd of rechtstreeks verwijderd. Bij succes wordt de pointer teruggegeven.

`afmelden()` verwijdert een node uit de boom. Een node met nog children kan niet worden afgemeld. Alleen objecten die door `GedeeldeBusNewComponent()` als dynamisch component zijn aangemaakt worden via `delete this` verwijderd; statisch aangemaakte objecten niet.

De basisstatus in `GedeeldeBusNode` bestaat uit `aangemeld`, `gecontroleerd`, `ingeplugd`, `actief`, `activerenBezig` en `conflictGevonden`.

### Diagnose

De code bevat momenteel uitsluitend deze voorbereide DEBUG-hook:

```cpp
#ifdef DEBUG
  virtual void Diagnose() {}
#endif
```

Er zijn in de huidige code nog geen concrete `Diagnose()`-overrides. Uitgebreide elektronische functietesten zijn dus nog niet geïmplementeerd als componentdiagnose.

## Resourcecontrole

### Native resources

`HardwareResourcePin` gebruikt voor de fysieke resources van de UNO-vormfactor de prefix `NP_`: `NP_D0..NP_D13`, `NP_A0..NP_A5`, `NP_SDA`, `NP_SCL`, `NP_MISO`, `NP_MOSI`, `NP_SCK` en `NP_SS`. `CUSTOM` en `NONE` blijven zonder `NP_`: dit zijn speciale waarden en geen fysieke pinnen.

`NativeArduinoPinVan()` vertaalt zo'n logische resource naar het board-specifieke Arduino-pinnummer uit `SystemConfig.h`. ExtenderPins gaan nooit door deze functie.

De waarden van `HardwareResourceType` gebruiken de prefix `RT_`, bijvoorbeeld `RT_I2C`, `RT_SPI`, `RT_GPIO` en `RT_INTERRUPT`. De groepswaarden binnen `GedeeldeBusComponent` omvatten onder meer `GC_INPUT`, `GC_SCREEN`, `GC_EXTENDER`, `GC_SENSOR` en `GC_DATA`; concrete componentwaarden behouden hun eigen naam.

### Gedeelde en exclusieve claims

`BezettingPinnen` bewaart gedeelde en exclusieve claims afzonderlijk. Voor gedeelde I2C- en SPI-lijnen gebruikt GedeeldeBus bovendien `HardwareResourceIdentiteit`, zodat twee claims op dezelfde fysieke pin alleen compatibel zijn wanneer hun protocolrol overeenkomt (`I2C_SDA`, `I2C_SCL`, `SPI_SCK`, `SPI_MISO`, `SPI_MOSI`).

I2C-adressen worden intern als exclusieve claim in hetzelfde mechanisme behandeld. In `HardwareResourceTypeI2C` wordt het adres opgeslagen als `adres | 0x80`, zodat het niet met normale pinwaarden samenvalt.

### Resourcegebied per Extender

`firstExtenderNode()` bepaalt in welk resourcegebied een node zit. Claims worden alleen rechtstreeks vergeleken wanneer ze tot hetzelfde resourcegebied behoren. Native claims vergelijken dus met Native claims; children van dezelfde Extender vergelijken binnen die Extender.

**Gemengde pinnen** (`GEDEELDE_BUS_MIXED_PINS`; standaard `0`): een component mag per exclusieve pin een eigen gebied hebben, het board of een Extender. Voorbeeld: een SPI-chip met SCK, MISO en MOSI op het board en CS op een PCF8574. `ownerClaim` geeft per exclusieve claim de Extender (`nullptr` = het gewone gebied, zoals hierboven); `GetOwnerClaim()` geeft het gebied van één claim. De conflictcontrole vergelijkt dan per claim, en een pin op een eigen Extender moet op die Extender bestaan (GB111). Extenders met digitale pinnen (PCF8574, PCF8575, MCP23017) bieden daarvoor `SetExtenderPinMode()`, `WriteExtenderPin()` en `ReadExtenderPin()`. Staat de instelling op `0`, dan wordt niets hiervan gecompileerd en blijft het geheugengebruik gelijk. Een Data-chip met CS via een Extender zet de instelling automatisch op `1`. Sensoren kunnen dit gebruiken door `ownerClaim` te vullen; elke exclusieve pin kan naar een andere Extender wijzen. Voorbeeld: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_MixedPins/Sensor_MixedPins.ino` (2 pinnen op het board, 2 op een PCF8574). RFP602 doet dat nog niet, en de ADC-routes blijven per backend gescheiden.

## I2C en SPI

`HardwareResourceTypeI2C` en `HardwareResourceTypeSPI` zijn de twee generieke tussenlagen voor concrete buscomponenten.

- I2C claimt SDA en SCL gedeeld en het adres exclusief; `inpluggen()` initialiseert I2C via `InitialiserenGedeeldeBus(GedeeldeBusType::I2C)`.
- SPI claimt SCK, MISO en MOSI gedeeld en CS exclusief; `inpluggen()` vereist een geldige CS en initialiseert SPI.
- `InitialiserenGedeeldeBus()` bewaakt met interne flags dat `Wire.begin()` en `SPI.begin()` niet telkens opnieuw worden uitgevoerd.

## Concrete Extenders

De huidige code bevat 16 concrete Extenders. Alleen een Extender die compile-time effectief nodig is, wordt mee gecompileerd. Een `EXTENDER_<NAAM>_AANTAL` groter dan 0 activeert de overeenkomstige Extender; daarnaast gebruikt Input `ExtenderPCF8574` wanneer `INPUT_KANAAL_CONFIG` `INPUT_TYPE_PCF8574` bevat.


1. `ExtenderADS1115`
2. `ExtenderADS1158`
3. `ExtenderADS7828`
4. `ExtenderADS7953`
5. `ExtenderCD74HC4067`
6. `ExtenderDS2482v800`
7. `ExtenderMAX14830I2C`
8. `ExtenderMAX14830SPI`
9. `ExtenderMCP23017`
10. `ExtenderPCF8574`
11. `ExtenderPCF8575`
12. `ExtenderSC16IS752I2C`
13. `ExtenderSC16IS752SPI`
14. `ExtenderTCA9548A`

Elke concrete Extender heeft een eigen `.h/.cpp`. Elke `enum class ExtenderPins` gebruikt de vaste naamconventie `EP_<pinnaam>`.

De actuele resources uit de code zijn:

| Extender | `ExtenderPins` |
|---|---|
| ADS1115 | `EP_AIN0..EP_AIN3` |
| ADS1158 | `EP_AIN0..EP_AIN15`, `EP_GPIO0..EP_GPIO7` |
| ADS7828 | `EP_CH0..EP_CH7` |
| ADS7953 | `EP_CH0..EP_CH15`, `EP_GPIO0..EP_GPIO3` |
| CD74HC4067 | `EP_Y0..EP_Y15` |
| DS2482v800 | `EP_IO0..EP_IO7` |
| MAX14830 I2C/SPI | `EP_UART0..EP_UART3`, `EP_GPIO0..EP_GPIO15` |
| MCP23017 | `EP_GPA0..EP_GPA7`, `EP_GPB0..EP_GPB7` |
| PCF8574 | `EP_P0..EP_P7` |
| PCF8575 | `EP_P00..EP_P07`, `EP_P10..EP_P17` |
| SC16IS752 I2C/SPI | `EP_CHANNEL_A`, `EP_CHANNEL_B`, `EP_GPIO0..EP_GPIO7` |
| TCA9548A | `EP_CH0..EP_CH7` |

De fabrikant-/functiebenaming blijft in de comments naast elke enumwaarde staan.

## ADC-laag en Sensor

Er is geen aparte ADC-laag meer: een ADC is een gewone Extender. De `.ino` maakt de ADC-Extender aan (nu: `ExtenderADS1115`, componentcode `ADC_ADS1115`) en geeft hem als parent aan de Sensor door, zoals bij elke Extender. Welke ADC-Extenders gecompileerd worden, volgt uit `EXTENDER_<NAAM>_AANTAL`.

`Sensor` is momenteel een tussenstruct boven `GedeeldeBusNode`. `RFP602` staat onder `src/Systeem/Sensor/` en leest analoge waarden via het bestaande generieke ADC-parentcontract (`StartExtenderReadAnalog`, `IsExtenderReadAnalogReady`, `GetExtenderReadAnalogValue`). De concrete ADC-implementatie blijft bij de Extender. Eén `RFP602`-object beheert maximaal vier kanalen op één ADC-Extender; de kanalen komen uit de constructor of anders uit `RFP602_ADC_PIN_SENSOR_1_SET_1` tot en met `RFP602_ADC_PIN_SENSOR_4_SET_1`. Set 1 en set 2 worden onderscheiden via de parameter `set`, `RFP602_AANTAL_SENSOREN_BESCHIKBAAR_SET_2`, `sensorRFP602set1` en `sensorRFP602set2`; elke set gebruikt een eigen ADS1115 en elk object eigen buffers.

## Screen

`Screen` is een `GedeeldeBusNode` met runtime-selectie via `typesActief`. De concrete children zijn, afhankelijk van compile-time `SCREEN_OUTPUT`, `SerialOutput`, `CharacterScreen` en `PixelScreen`.

De constructors ondersteunen de compile-time standaardselectie, een runtime-subset en optioneel Character-/Pixelcallbacks. `Screen::aanmelden()`, `controleren()`, `inpluggen()`, `Activeren()` en `afmelden()` sturen de lifecycle door naar de geselecteerde children.

De concrete hardwarecontrole is niet overal in dezelfde lifecyclefase geplaatst. Zo controleert `CharacterScreen::Activeren()` momenteel of het I2C-adres antwoordt; `PixelScreen::inpluggen()` configureert de controlpinnen en `PixelScreen::Activeren()` retourneert in de huidige code nog `true` terwijl een uitgebreidere ID-controle uitgecommentarieerd staat. Dit is de actuele implementatie.

Zie [Systeem/SCREEN.md](Systeem/SCREEN.md).

## Input

`Input` is eveneens een `GedeeldeBusNode` met `typesActief`. Geldige compile-time waarden zijn exact:

```text
INPUT_TYPE_NONE
INPUT_TYPE_PCF8574
INPUT_TYPE_HX1838
INPUT_TYPE_PCF8574 | INPUT_TYPE_HX1838
```

Rechtstreeks gelezen digitale pinnen bestaan in v2.0.0 niet meer: het keypad loopt via de PCF8574, zodat acht native pinnen vrij blijven. De runtime-subset moet volledig binnen `INPUT_KANAAL_CONFIG` vallen; dit wordt in `Input::aanmelden()` gecontroleerd.

De concrete children zijn `InputPCF8574` en `InputHX1838`. De vroegere publieke `InputConfigureren()`-stap bestaat niet meer. `Input::inpluggen()` beslist eerst welke geselecteerde kanalen fysiek/minimaal bruikbaar zijn: PCF8574 moet zijn `begin(0xFF)`-I2C-transactie succesvol uitvoeren; TinyIRReceiver moet zijn interruptkoppeling kunnen initialiseren; IRremote wordt gestart en met de bestaande korte `isIdle()`-stabiliteitscontrole gecontroleerd. Een falend kanaal wordt vóór de overlevingscontrole afgemeld en op `nullptr` gezet. `Input::Activeren()` werkt daarna alleen met de overgebleven kanalen en verwerkt de HX1838-mapping- of kalibratiebron.

Zie [Systeem/INPUT.md](Systeem/INPUT.md).

## Stimulus

`Stimulus` blijft een toepassingsmodule onder `src/Toepassingsgebieden/Stimulus/`. De directe sensoruitlezing loopt in de huidige code via de globale pointer `sensorRFP602set1`; de voorbeelden maken `Screen`, `Input` en `sensorRFP602set1` via `GedeeldeBusNewComponent()` aan voordat de Stimuluslogica wordt gebruikt.

## Configuratiemodel

`src/Configuratie/SystemConfig.h` bevat vaste keuzewaarden, automatische boarddetectie, standaardwaarden, boardmapping en compile-time validaties. `UserConfig.h` wordt, tenzij `GROEIACADEMIE_IGNORE_USER_CONFIG` is gedefinieerd, vroeg in `SystemConfig.h` geladen wanneer het bestand bestaat.

De laadvolgorde is in de code:

```text
vaste keuzewaarden die vóór UserConfig nodig zijn
→ eventueel UserConfig.h
→ overige defaults
→ boardmapping
→ validaties en afgeleide configuratie
```

`ExamplesConfig.h` bevat gedeelde instellingen voor examples. `StimulusConfig.h` bevat Stimulusgrenzen.

## Verantwoordelijkheden

### Configuratie

`src/Configuratie/` bevat vaste keuzewaarden, gebruikersconfiguratie, boardkeuze en -mapping, ADC-backend, Screen-, Input-, Extender- en Sensorconfiguratie, voorbeeldinstellingen en Stimulusgrenzen.

### Systeem

`src/Systeem/` bevat frameworkbrede voorzieningen. De huidige systeemdomeinen zijn `GedeeldeBus`, `Screen`, `Input` en `Sensor`. GedeeldeBus beheert de generieke boom/lifecycle/resources; concrete Screen-, Input-, Sensor- en Extenderlogica blijft bij de betreffende component.

### Toepassingsgebieden

`src/Toepassingsgebieden/` bevat modules rond een concreet leer-, meet- of onderzoeksdoel. De huidige module is `Stimulus`.

### Gereserveerde domeinen

`src/Uitbreidingskaarten/` is momenteel een gereserveerde broncodemap. `docs/Uitbreidingskaarten/` bevat wel hardwaredocumentatie, waaronder de Stimulus Shield v2.0.0.

## Afhankelijkheden

`library.properties` declareert momenteel:

- LiquidCrystal I2C;
- Adafruit GFX Library;
- Adafruit ST7735 and ST7789 Library;
- ADS1X15 van Rob Tillaart;
- PCF8574 (>=0.4.0);
- FRAM_I2C van Rob Tillaart;
- IRremote.

Niet iedere build gebruikt alle dependencycode. Preprocessorconfiguratie bepaalt welke subsystemen werkelijk worden gecompileerd.

## Samenwerking tussen modules

Screen en Input zijn frameworkbrede systeemlagen. GedeeldeBus verzorgt hun resourceboom en lifecycle, maar de concrete Screen- en Inputlogica blijft in de eigen module. Stimulus gebruikt de systeemlagen en `RFP602`, maar GedeeldeBus bevat geen Stimulus-algoritmen.

## Elektronische documentatie

Elektronische schema's worden per toepassingsgebied of uitbreidingskaart beschreven. De index staat in [Toepassingsgebieden/MODULES.md](Toepassingsgebieden/MODULES.md). De beschikbare sensoren en modules staan in [Toepassingsgebieden/SENSOR_INVENTARIS.md](Toepassingsgebieden/SENSOR_INVENTARIS.md).

Een geldig schema vermeldt ten minste boardvariant, pinbezetting, voeding/massa, componentwaarden, sensorvariant, externe modules/adressen, kalibratievoorwaarden en beperkingen.

## Testarchitectuur

De huidige examples onder `examples/Systeem/GedeeldeBus/Extenders/` bevatten per Extender een basisvoorbeeld en aparte `ExtenderPins`-tests voor toegelaten en geweigerde resources. `examples/Systeem/GedeeldeBus/Sensoren/Sensor_RFP602/` bevat de eerste Sensor-lifecycletest.

Deze tests bewijzen niet automatisch de volledige elektronische werking van elk kanaal. Zie `extras/TESTEN.md` voor het onderscheid tussen lifecycle-/resourcecontrole, elektronische functietests en praktijktests.

## Geheugen en timing

UNO R3 blijft een belangrijke ondergrens. Nieuwe code wordt beoordeeld op flash, SRAM, blokkerende wachttijden en samplegedrag. DEBUG- en TRACE-functionaliteit moet uitschakelbaar blijven. `Diagnose()` bestaat uitsluitend in DEBUG-builds.
