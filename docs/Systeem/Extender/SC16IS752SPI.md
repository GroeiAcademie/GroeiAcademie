# ExtenderSC16IS752SPI

## Implementatie

- Library: **`Appnostic_SC16IS752`** (Appnostic SC16IS7XX Arduino Library), één object per UART-kanaal (`channelA`, `channelB`).
- De Extenderklasse erft de library niet, maar bevat ze als leden.

## FrameWork-integratie

Huidige overerving:

```cpp
struct ExtenderSC16IS752SPI : HardwareResourceTypeSPI
```

Naast GedeeldeBus-resourcebeheer biedt de klasse de UART-parentfuncties `GetExtenderStream()` en `StartExtenderUART()`, de eigen GPIO-registerhelpers `SC16IS752LeesRegister()` en `SC16IS752SchrijfRegister()`, de digitale parent-methoden `SetExtenderPinMode()`, `WriteExtenderPin()` en `ReadExtenderPin()`, en de `ExtenderOneWire*`-functies voor 1-Wire via UART.

## UART-kanalen

- Twee UART-kanalen (channel 0 = A, channel 1 = B). Een Sensor op een channel haalt zijn Stream met `GetExtenderStream(channel)`.
- De Sensor zet zijn eigen baud met `StartExtenderUART(channel, baud)` in zijn `Activeren()`; de Extender roept daarvoor `setBaudrate()` van de library aan.

Niet op hardware getest: de baud per channel.

## Bronbestanden

```text
src/Systeem/Extender/SC16IS752SPI.h
src/Systeem/Extender/SC16IS752SPI.cpp
```

## Uitwisselbaarheid

- **Rechtstreeks verwant:** `ExtenderSC16IS752I2C` (kanalen A/0 en B/1). `ExtenderMAX14830I2C` en `ExtenderMAX14830SPI` zijn voorwaardelijke alternatieven voor de gemeenschappelijke UART-kanalen 0 en 1.
- **Omzetten:** wijzig parenttype, componentcode, doelconstructor en configuratie; maak I²C/SPI-aansluitingen passend en controleer de bestaande kanaalclaim en UART-TX/RX.
- **Beperking:** fysiek gebruik, klok-/baudinstellingen en extra chipfuncties zijn niet automatisch gelijkwaardig. SPI heeft geen I²C-runtime-herstel.

Zie ook [`GEDEELDEBUS.md`](../GEDEELDEBUS.md#uitwisselbaarheid-van-extenders) voor de gezamenlijke uitwisselbaarheidstabel en de stappen voor een omschakeling.

## Aansluiten

- SPI: `SCLK`, `MOSI`, `MISO`, `CS`; twee UART-kanalen met TX/RX en acht GPIO.
- De bestaande Extender-constructor heeft aparte lijnen `np_IRQ` en `np_RESET`.
- Aansluitinstellingen: `EXTENDER_SC16IS752_SPI_1_CS_PIN`, `EXTENDER_SC16IS752_SPI_1_IRQ`, `EXTENDER_SC16IS752_SPI_1_RESET`, `EXTENDER_SC16IS752_SPI_2_CS_PIN`, `EXTENDER_SC16IS752_SPI_2_IRQ`, `EXTENDER_SC16IS752_SPI_2_RESET`.

**Aansluit- en gebruiksgegevens uit de bestaande FrameWork-interface:**

- **Host-SPI:** `SCK/SCLK` van de module → host `np_SCK`; host `np_MOSI` → module `MOSI/SDI/DIN`; module `MISO/SDO/DOUT` → host `np_MISO`; module `CS/SS` → de exclusieve `np_CS` van deze Extender. Verbind GND en de voeding met correcte logicaniveaus.
- De gedeelde SPI-datalijnen kunnen gedeeld worden, **CS niet** met een gelijktijdig geclaimd ander apparaat. Kies een echte geschikte native CS-pin en controleer de fysieke modulebenaming.
- Aan de UART-zijde: apparaat-`RX` → `TXA` of `TXB` van de Extender; apparaat-`TX` → `RXA` of `RXB`. De twee UART-kanalen zijn onafhankelijk. Sluit de optionele IRQ/RESET-pinnen alleen aan als je ze werkelijk in de constructor hebt opgegeven.
- De SPI-hostlijnen zijn SCK, MOSI, MISO en een eigen CS; aan de apparaatzijde zijn twee UART-paren `TXA/RXA` en `TXB/RXB` aanwezig.
- `Activeren()` initialiseert beide UART-kanalen over SPI en controleert de verbinding via `channelA.ping()`. `StartExtenderUART` accepteert uitsluitend kanalen 0 en 1 en een niet-nul baudwaarde.

**Bestaande constructor (parameter-/pinvolgorde):**

```cpp
ExtenderSC16IS752SPI(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin np_CS, HardwareResourcePin np_SCK, HardwareResourcePin np_MISO, HardwareResourcePin np_MOSI, HardwareResourcePin np_IRQ, HardwareResourcePin np_RESET, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
```

```cpp
ExtenderSC16IS752SPI(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin np_SCK, HardwareResourcePin np_MISO, HardwareResourcePin np_MOSI, uint8_t teller, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
```

**Configuratie voor deze hardware:**

- Gebruik bij een eigen configuratie `src/Configuratie/Extenders/ConfigSC16IS752SPI.h` (het aanwezige `src/Configuratie/Extenders/ConfigSC16IS752SPI_template.h` is een kopieersjabloon en wordt niet rechtstreeks ingelezen).
- Voor één aangesloten exemplaar: `EXTENDER_SC16IS752_SPI_AANTAL` op **1**, behalve wanneer de bestaande groeps-/setconfiguratie uitdrukkelijk een andere betekenis heeft.
- De bestaande configuratie bevat deze aansluitrelevante definities (gebruik de **werkelijke aangesloten** pinnen, adressen en referenties):
  - `EXTENDER_SC16IS752_SPI_AANTAL`: sjabloonwaarde `0`.
  - `EXTENDER_SC16IS752_SPI_1_CS_PIN`: sjabloonwaarde `HardwareResourcePin::NONE`.
  - `EXTENDER_SC16IS752_SPI_2_CS_PIN`: sjabloonwaarde `HardwareResourcePin::NONE`.

**Letterlijk constructorfragment uit de bestaande voorbeeldsketch** (alle gebruikte parent-objecten moeten zelf correct aangesloten en geïnitialiseerd zijn):

```cpp
ExtenderSC16IS752SPI extender(
  &Native,
  GedeeldeBusComponent::UART_SC16IS752,
  EXTENDER_SC16IS752_SPI_1_CS_PIN, HardwareResourcePin::NP_SCK, HardwareResourcePin::NP_MISO, HardwareResourcePin::NP_MOSI, EXTENDER_SC16IS752_SPI_1_IRQ, EXTENDER_SC16IS752_SPI_1_RESET
);
```

**Bestaande test-/voorbeeldsketches:**

- `examples/Systeem/GedeeldeBus/Extenders/Extender_SC16IS752_SPI/Extender_SC16IS752_SPI.ino`
- `examples/Systeem/GedeeldeBus/Extenders/Extender_SC16IS752_SPI_Test_ExtenderPins_Toegelaten/Extender_SC16IS752_SPI_Test_ExtenderPins_Toegelaten.ino`
- `examples/Systeem/GedeeldeBus/Extenders/Extender_SC16IS752_SPI_Test_ExtenderPins_Geweigerd/Extender_SC16IS752_SPI_Test_ExtenderPins_Geweigerd.ino`


## Librarydocumentatie

- Deze Extender erft geen externe library-API; ze gebruikt `Appnostic_SC16IS752` als leden.
- Deze `.md` documenteert daarom de eigen FrameWork-/hardware-integratie die momenteel in de Extender aanwezig is.

**Elektrische aansluiting en randvoorwaarden volgens NXP:**

- De SC16IS752 gebruikt bij deze variant een **SPI-hostbus in mode 0**, met eigen CS, en biedt twee onafhankelijke UART-poorten. NXP geeft maximaal 4 Mbit/s SPI voor SC16IS752; de SC16IS762 is een ander type met andere SPI-limieten.
- De UART-kant gebruikt TX/RX-logicaniveaus; RS-232 en RS-485 vereisen een geschikte aparte transceiver.

**Technische handleiding:** https://www.nxp.com/products/interfaces/ic-spi-i3c-interface-devices/bridges/dual-uart-with-ic-bus-spi-interface-64-bs-of-transmit-and-receive-fifos-irda-sir-built-in-support%3ASC16IS752_SC16IS762


**Elektrische voeding bij de losse SC16IS752:** NXP specificeert een **2,5V- of 3,3V-bedrijfsspanning** en een SPI-hostinterface in **mode 0**, maximaal **4 Mbit/s voor SC16IS752** (niet de hogere limiet van SC16IS762). Verbind de chip met een compatibele logicavoeding en zorg dat SPI-SCK, MOSI, MISO, CS en eventuele IRQ/RESET overeenkomen met de bestaande constructor. De UART-uitgangen zijn logicaniveaus; sluit geen RS-232/RS-485-bus rechtstreeks op TX/RX aan. De exacte padnummers verschillen tussen **TSSOP28 en HVQFN32** en zijn niet gelijk aan eventuele breakoutheaders.

**Fabrikantdatasheet:** https://www.nxp.com/docs/en/data-sheet/SC16IS752_SC16IS762.pdf

**Bestaand bronbestand voor de component:** `src/Systeem/Extender/SC16IS752SPI.h`

### Gelijktijdige UART-kanalen en herstel

- Loopback op UART-kanaal A en B, gelijktijdig, met korte/lange pakketten, verschillende baudrates, volle RX/FIFO-buffer, ontbrekende bytes en SPI-storing. Vergelijk exacte bytes en controleer CS, IRQ en reset na herstel.
- **Status:** fysieke proef vereist; nog geen hardwarebewijs.

## Digitale pinnen (GPIO) en 1-Wire

- GPIO0..GPIO7 (`EP_GPIO0..EP_GPIO7`) via eigen registers (IODir 0x0A, IOState 0x0B): `SC16IS752LeesRegister`, `SC16IS752SchrijfRegister`.
- 1-Wire via een UART-channel (Maxim AN214): het 1-Wire-contract (`ExtenderOneWire*`); één apparaat per channel (READ ROM). Leden: `oneWireChannel`, `oneWireReadROMsearchDone`; functie `OneWireUARTbits`.

## Kristal en RESET

- `EXTENDER_SC16IS752_BRONKLOK_HZ` (SystemConfig.h, ConfigSC16IS752SPI.h): de werkelijke kristal- of klokfrequentie aan XTAL1, standaard 14745600 Hz. Een module met een ander kristal (bv. 1843200 of 3072000 Hz) heeft deze instelling nodig, anders klopt de baud niet.
- `Activeren()` zet de RESET-pin hoog (actief laag), als ze ingesteld is.

## Controle

- `StartExtenderUART()`: `setBaudrate()` geeft geen resultaat (Appnostic_SC16IS752 1.0.0); daarom wordt daarna het scratchpadregister van het gekozen kanaal geschreven en teruggelezen. `ping()` alleen volstaat niet voor kanaal B.

## Diagnose bij een mislukte activering

- Een mislukte chipactivering roept de bestaande `FoutDiagnoseSPI()` aan, zoals bij de andere SPI-Extenders. De fout verschijnt als gewone GB-melding (GB301, met DEBUG GB304).
- De controle op de parent blijft een gewone `return false`.

## 1-Wire: leesfout bij de echo

- `OneWireUARTbits()` bewaart de waarde van `read()` als `int`; een negatieve waarde (geen geldige byte) telt als mislukte echo en niet als aanwezigheidspuls bij `ExtenderOneWireReset()`.

## CS altijd native

- De library stuurt de CS-pin zelf aan met `pinMode()` en `digitalWrite()`, buiten `WriteCS()` van het FrameWork om.
- De CS van deze Extender moet daarom een native pin van het board zijn. Een CS via een GPIO-Extender (`ConnectCStoExtender()`) werkt hier niet. Het FrameWork biedt daarvoor ook geen instelling: alleen FLASH en microSD ondersteunen een CS via een Extender.

## Baud en 1-Wire

- Na `setBaudrate()` schrijft en leest de library het scratchpad van het gekozen channel. `ping()` is daarvoor niet bruikbaar, want `ping()` test altijd channel A.
- De ingestelde baud zelf wordt niet teruggelezen. Een juiste baud hangt af van de ingestelde kristalfrequentie.
- 1-Wire via een UART-channel: vóór elke overdracht worden hoogstens 128 oude bytes weggegooid. Raakt de buffer niet leeg of mislukt een lezing, dan mislukt de 1-Wire-actie.
