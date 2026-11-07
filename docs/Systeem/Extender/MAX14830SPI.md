# ExtenderMAX14830SPI

## Implementatie

- Library: **geen externe library in de huidige Extenderklasse**.
- Er is in de huidige Extenderklasse geen externe library-API die integraal wordt geërfd.

## FrameWork-integratie

Huidige overerving:

```cpp
struct ExtenderMAX14830SPI : HardwareResourceTypeSPI
```

De huidige klasse voegt voornamelijk GedeeldeBus-/resource-integratie toe; er is geen aanvullende publieke chipspecifieke API geïnventariseerd die hier apart moet worden gedocumenteerd.

## UART-kanalen en klok

- Vier UART-kanalen (channel 0 tot en met 3). Een Sensor op een channel haalt zijn Stream met `GetExtenderStream(channel)`; dat kan al in zijn constructor, de Stream controleert zelf bij elke oproep of de Extender actief is.
- De Sensor zet zijn eigen baud met `StartExtenderUART(channel, baud)` in zijn `Activeren()`. De Extender rekent de deler uit volgens de datasheet: `D = fREF / (16 x baud)`, `DIV = TRUNC(D)` in DIVLSB/DIVMSB en `FRACT = ROUND(16 x (D - DIV))` in BRGConfig[3:0]. Is de baud te hoog voor factor 16, dan kiest de Extender de 2x-modus (factor 8) of de 4x-modus (factor 4). Een baud die met de klok niet haalbaar is, wordt geweigerd.
- `Activeren()` stelt de klokbron in (CLKSource, via UART0) en wacht bij een kristal maximaal 20 ms op CLKREADY.

| Instelling | Standaard | Betekenis |
|---|---|---|
| `EXTENDER_MAX14830_BRONKLOK_HZ` | `0` | Werkelijke frequentie van het kristal of de externe klok in Hz. `0` = onbekend: geen UART-start |
| `EXTENDER_MAX14830_BRONKLOK_KRISTAL` | `1` | `1` = kristal aan XIN/XOUT, `0` = externe klok aan XIN |
| `EXTENDER_MAX14830_PLL_VOORDELER` | `0` | `0` = PLL uit; anders 1 tot en met 63 |
| `EXTENDER_MAX14830_PLL_VERMENIGVULDIGER` | `0` | `0` = PLL uit; anders 6, 48, 96 of 144. De klok na de PLL mag niet hoger zijn dan 96 MHz |

Voorbeeld: met een kristal van 3,6864 MHz zijn 9600, 57600, 115200, 460800 en 921600 baud exact; 256000 baud (LD2450) wordt 254234 baud (-0,69 %), zonder PLL.

Niet op hardware getest: klokbron, PLL en baud.

## Bronbestanden

```text
src/Systeem/Extender/MAX14830SPI.h
src/Systeem/Extender/MAX14830SPI.cpp
```

## Uitwisselbaarheid

- **Rechtstreeks verwant:** `ExtenderMAX14830I2C` met vier UART-kanalen en hetzelfde UART-parent-contract. `ExtenderSC16IS752I2C` en `ExtenderSC16IS752SPI` zijn alleen voorwaardelijk uitwisselbaar voor kanalen 0 en 1.
- **Omzetten:** vervang parenttype/componentcode, doelconstructor en configuratie; bij I²C verandert SPI-CS in I²C-adres en SDA/SCL. Controleer kanaalclaims, UART-TX/RX en de werkelijke bronklok.
- **Beperking:** het UART-contract garandeert geen identiek klok-/baudgedrag; SC16IS752 heeft slechts twee kanalen. SPI heeft niet hetzelfde I²C-runtime-herstel.

Zie ook [`GEDEELDEBUS.md`](../GEDEELDEBUS.md#uitwisselbaarheid-van-extenders) voor de gezamenlijke uitwisselbaarheidstabel en de stappen voor een omschakeling.

## Aansluiten

- SPI: `SCLK`, `MOSI`, `MISO`, `CS`; de chip biedt vier UART-kanalen met TX/RX en 16 GPIO.
- De bestaande Extender-constructor heeft een optionele interruptlijn `np_IRQ`.
- Aansluitinstellingen: `EXTENDER_MAX14830_SPI_1_CS_PIN`, `EXTENDER_MAX14830_SPI_1_IRQ`, `EXTENDER_MAX14830_SPI_2_CS_PIN`, `EXTENDER_MAX14830_SPI_2_IRQ`.

**Aansluit- en gebruiksgegevens uit de bestaande FrameWork-interface:**

- **Host-SPI:** `SCK/SCLK` van de module → host `np_SCK`; host `np_MOSI` → module `MOSI/SDI/DIN`; module `MISO/SDO/DOUT` → host `np_MISO`; module `CS/SS` → de exclusieve `np_CS` van deze Extender. Verbind GND en de voeding met correcte logicaniveaus.
- De gedeelde SPI-datalijnen kunnen gedeeld worden, **CS niet** met een gelijktijdig geclaimd ander apparaat. Kies een echte geschikte native CS-pin en controleer de fysieke modulebenaming.
- Aan de seriële apparaatzijde: `TX0`–`TX3` van de Extender → `RX` van elk aangesloten UART-apparaat, en de bijbehorende `RX0`–`RX3` ← `TX` van dat apparaat. De bronklok en PLL-definities in de bestaande configuratie moeten overeenkomen met de fysieke oscillator.
- Deze vierkanaals UART-Extender gebruikt SPI voor de host en afzonderlijke TX/RX-lijnen voor de aangesloten UART-apparaten. Elke host-SPI-instantie heeft een eigen CS nodig.
- `MAX14830StartUART(channel, baud)` ondersteunt alleen UART-kanalen 0–3; de baudgenerator vereist de geconfigureerde `EXTENDER_MAX14830_BRONKLOK_HZ` en eventuele PLL-parameters.

**Bestaande constructor (parameter-/pinvolgorde):**

```cpp
ExtenderMAX14830SPI(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin np_CS, HardwareResourcePin np_SCK, HardwareResourcePin np_MISO, HardwareResourcePin np_MOSI, HardwareResourcePin np_IRQ, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
```

```cpp
ExtenderMAX14830SPI(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin np_SCK, HardwareResourcePin np_MISO, HardwareResourcePin np_MOSI, uint8_t teller, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
```

**Configuratie voor deze hardware:**

- Gebruik bij een eigen configuratie `src/Configuratie/Extenders/ConfigMAX14830SPI.h` (het aanwezige `src/Configuratie/Extenders/ConfigMAX14830SPI_template.h` is een kopieersjabloon en wordt niet rechtstreeks ingelezen).
- Voor één aangesloten exemplaar: `EXTENDER_MAX14830_SPI_AANTAL` op **1**, behalve wanneer de bestaande groeps-/setconfiguratie uitdrukkelijk een andere betekenis heeft.
- De bestaande configuratie bevat deze aansluitrelevante definities (gebruik de **werkelijke aangesloten** pinnen, adressen en referenties):
  - `EXTENDER_MAX14830_BRONKLOK_HZ`: sjabloonwaarde `0`.
  - `EXTENDER_MAX14830_BRONKLOK_KRISTAL`: sjabloonwaarde `1`.
  - `EXTENDER_MAX14830_PLL_VOORDELER`: sjabloonwaarde `0`.
  - `EXTENDER_MAX14830_PLL_VERMENIGVULDIGER`: sjabloonwaarde `0`.
  - `EXTENDER_MAX14830_SPI_AANTAL`: sjabloonwaarde `0`.
  - `EXTENDER_MAX14830_SPI_1_CS_PIN`: sjabloonwaarde `HardwareResourcePin::NONE`.
  - `EXTENDER_MAX14830_SPI_2_CS_PIN`: sjabloonwaarde `HardwareResourcePin::NONE`.

**Letterlijk constructorfragment uit de bestaande voorbeeldsketch** (alle gebruikte parent-objecten moeten zelf correct aangesloten en geïnitialiseerd zijn):

```cpp
ExtenderMAX14830SPI extender(
  &Native,
  GedeeldeBusComponent::UART_MAX14830,
  EXTENDER_MAX14830_SPI_1_CS_PIN, HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, EXTENDER_MAX14830_SPI_1_IRQ
);
```

**Bestaande test-/voorbeeldsketches:**

- `examples/Systeem/GedeeldeBus/Extenders/Extender_MAX14830_SPI/Extender_MAX14830_SPI.ino`
- `examples/Systeem/GedeeldeBus/Extenders/Extender_MAX14830_SPI_Test_ExtenderPins_Toegelaten/Extender_MAX14830_SPI_Test_ExtenderPins_Toegelaten.ino`
- `examples/Systeem/GedeeldeBus/Extenders/Extender_MAX14830_SPI_Test_ExtenderPins_Geweigerd/Extender_MAX14830_SPI_Test_ExtenderPins_Geweigerd.ino`


## Librarydocumentatie

- Deze Extender erft in de huidige implementatie geen externe library-API.
- Deze `.md` documenteert daarom de eigen FrameWork-/hardware-integratie die momenteel in de Extender aanwezig is.

**Elektrische aansluiting en randvoorwaarden volgens Analog Devices:**

- De MAX14830 zet SPI om in vier afzonderlijke UART-kanalen, met TX/RX per kanaal. SPI aan de hostzijde en asynchrone UART aan de apparaatkant mogen niet verwisseld worden; RS-232/RS-485 vereist een afzonderlijke niveau-/bustransceiver.
- Gebruik een eigen chipselect en controleer de fysieke klokbron en de daaraan gekoppelde bestaande baudrate-/PLL-configuratie.

**Technische handleiding:** https://www.analog.com/en/products/max14830.html


**Voeding en logicaniveaus voor de losse MAX14830 (Analog Devices):** `VA` **2,35–3,6 V** (analoge voeding), `VL` **1,71–3,6 V** (host-SPI-interface) en `VEXT` **1,71–3,6 V** (UART-interface) zijn **afzonderlijke** spanningsdomeinen. Host-SCK/MOSI/CS en MISO moeten elektrisch compatibel zijn met `VL`; UART-TX/RX met `VEXT`. Verbind een 5V-host niet rechtstreeks omdat de module toevallig een SPI-header heeft. Sluit de oscillator en eventuele regelaar af volgens de concrete chip- of breakoutuitvoering. UART is niet hetzelfde als RS-232/RS-485: gebruik daarvoor transceivers.

**Fabrikantdatasheet:** https://www.analog.com/media/en/technical-documentation/data-sheets/MAX14830.pdf

**Bestaand bronbestand voor de component:** `src/Systeem/Extender/MAX14830SPI.h`

### UART-buffer- en vierkanaalsproef
- **Proef:** configureer alle vier UART-kanalen met de werkelijk aangesloten bronklok; zend/ontvang simultaan verschillende bytestrings op ieder kanaal. Test een onvolledig pakket, hoge datarate, overvolle RX, een lege TX-buffer, andere baudrate en opnieuw verbinden na SPI-/CS-uitval.
- **Bewijs:** vergelijk per kanaal verzonden en ontvangen bytes, volgorde, verlies en foutrespons; alleen een werkende kanaal-0-demo is onvoldoende. **Status:** hardwareproef open.

### Uitwisselbaarheid met SC16IS752

- Bij vervanging door SC16IS752 blijven slechts twee UART-kanalen beschikbaar in plaats van vier. Controleer per client het gebruikte kanaal, de UART-stream, baudrate en buffergrootte. Een wissel tussen I²C en SPI vereist ook andere bedrading en pinclaims.
- **Status:** fysieke proef vereist; nog geen hardwarebewijs.
