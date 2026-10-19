# GedeeldeBus

## Status

Deze pagina beschrijft de actuele implementatie in `src/Systeem/GedeeldeBus/GedeeldeBus.h/.cpp` en de 16 concrete Extenderbestanden van beta v2.0.0.

## Doel

GedeeldeBus verzorgt de gemeenschappelijke componentboom, lifecycle, businitialisatie en resourceconflictcontrole. Screen, Input, Sensoren en concrete Extenders blijven hun eigen hardware- en functielogica bezitten.

## Bestanden

```text
src/Systeem/GedeeldeBus/
├── GedeeldeBus.h
├── GedeeldeBus.cpp
├── ExtenderADS1115.h/.cpp
├── ExtenderADS1158.h/.cpp
├── ExtenderADS7828.h/.cpp
├── ExtenderADS7953.h/.cpp
├── ExtenderCD74HC4067.h/.cpp
├── ExtenderDS2482v800.h/.cpp
├── ExtenderMAX14830I2C.h/.cpp
├── ExtenderMAX14830SPI.h/.cpp
├── ExtenderMCP23017.h/.cpp
├── ExtenderMCP23S17.h/.cpp
├── ExtenderPCA9685.h/.cpp
├── ExtenderPCF8574.h/.cpp
├── ExtenderPCF8575.h/.cpp
├── ExtenderSC16IS752I2C.h/.cpp
├── ExtenderSC16IS752SPI.h/.cpp
└── ExtenderTCA9548A.h/.cpp
```

## Boom

`Native` is de statische rootnode. `GedeeldeBusNode` bevat `parent`, `firstChild`, `nextChild` en `prevChild`; er is geen centrale node-array en geen vast maximum. Iedere node krijgt een uniek `id` via `volgendeGedeeldeBusId`.

`GedeeldeBusNode` wordt gebruikt voor root, parents en leaves. De rol wordt niet door een aparte subclass-hiërarchie afgedwongen, maar door de concrete component en de parent/childrelatie.

## Lifecycle

`GedeeldeBusNewComponent<T>(...)` voert uit:

```text
new T(...)
→ componentCreated = true
→ aanmelden()
→ controleren()
→ inpluggen()
→ activeren()
```

Bij een fout wordt `nullptr` teruggegeven en het dynamisch aangemaakte object opgeruimd. Na de cyclus worden tijdelijke conflictmeldingen weergegeven en verwijderd.

`activeren()` is de generieke lifecyclemethode. Deze zorgt dat de parent eerst actief is en roept vervolgens de concrete virtuele hook `Activeren()` aan.

`afmelden()` weigert wanneer de node nog children heeft. Dynamisch aangemaakte componenten worden verwijderd; statische componenten niet.

## Huidige betekenis per stap

- `aanmelden()`: node aan de boom koppelen en claims beschikbaar maken.
- `controleren()`: configuratie/resourceconflicten controleren.
- `inpluggen()`: basisvoorwaarden voor de component/bus uitvoeren; de concrete inhoud verschilt per component.
- `activeren()`: via `Activeren()` de component operationeel maken.

De code bevat nog component-specifieke hardwarecontroles in verschillende stappen. Bijvoorbeeld: `CharacterScreen::Activeren()` controleert het I2C-adres; `HardwareResourceTypeI2C::inpluggen()` initialiseert de bus; `PixelScreen::inpluggen()` configureert CS/DC/RST.

## Diagnose

In `GedeeldeBusNode` staat momenteel:

```cpp
#ifdef DEBUG
  virtual void Diagnose() {}
#endif
```

Dit is nog een dummy. Er zijn in de huidige code geen concrete `Diagnose()`-overrides. Daardoor heeft een releasebuild geen Diagnose-interface en voert een DEBUG-build nog geen uitgebreide elektronische componenttests uit.

## Foutcodes

De GedeeldeBus-codes staan in `src/Language/Library_*.h`. Het niveau volgt `SCREEN_FOUTCODES.md`: `x0xx` ABORT, `x1xx` FATAL, `x2xx`/`x3xx` CRITICAL.

| Code | Niveau | Stap | Betekenis | Getoonde waarde | Build |
|---|---|---|---|---|---|
| GB001 | ABORT | `aanmelden()` | GEDEELD-claim zonder geldige identiteit | | gedefinieerd, wordt in de code nog nergens gemeld |
| GB101 | FATAL | `controleren()` | minstens een claim EXCLUSIEF, zelfde resource | pin | altijd |
| GB102 | FATAL | `controleren()` | zelfde I2C-adres, zelfde bus | I2C-adres | altijd |
| GB103 | FATAL | `controleren()` | twee GEDEELD-claims, andere identiteit | pin | altijd |
| GB104 | FATAL | `controleren()` | parent is exclusief al bezet door een andere child | geen | altijd |
| GB105 | FATAL | `controleren()` | binnen een component twee EXCLUSIEF-claims op dezelfde resource | pin | altijd |
| GB106 | FATAL | `controleren()` | binnen een component EXCLUSIEF- en GEDEELD-claim op dezelfde resource | pin | altijd |
| GB107 | FATAL | `controleren()` | binnen een component twee GEDEELD-claims, andere identiteit | pin | altijd |
| GB108 | FATAL | `controleren()` | verplichte resource ontbreekt (CS van een SPI-component, DC van een PixelScreen) | pin | altijd |
| GB109 | FATAL | `controleren()` | I2C-adres niet toegestaan voor dit hardwaretype | | gedefinieerd, wordt in de code nog nergens gemeld |
| GB110 | FATAL | `controleren()` | ongeldig I2C-adres | I2C-adres | altijd |
| GB111 | FATAL | `controleren()` | parent/extender biedt gevraagde resource niet aan | ExtenderPin | altijd |
| GB201 | CRITICAL | `inpluggen()` | inpluggen mislukt | I2C-adres | standaard |
| GB202 | CRITICAL | `inpluggen()` | bus start niet | I2C-adres | DEBUG |
| GB301 | CRITICAL | `activeren()` | activeren mislukt | I2C-adres of CS-pin | standaard |
| GB302 | CRITICAL | `activeren()` | geen antwoord op I2C-adres | I2C-adres | DEBUG |
| GB303 | CRITICAL | `activeren()` | antwoord, maar initialisatie van de library mislukt | foutcode van de library | DEBUG |
| GB304 | CRITICAL | `activeren()` | SPI-component antwoordt niet geldig | CS-pin | DEBUG |
| GB305 | CRITICAL | `activeren()` | I2C-busfout of timeout | I2C-adres | DEBUG |
| GB306 | CRITICAL | `activeren()` | verkeerde chip, teruglezen klopt niet | I2C-adres | DEBUG |
| GB400 | volgens de foutladder | runtime | communicatie met component mislukt | I2C-adres (+ kanaal achter een TCA9548A) | altijd |
| GB401 | huidig foutniveau | runtime, herstelpoging | heractiveren mislukt | I2C-adres | standaard |
| GB402 | huidig foutniveau | runtime, herstelpoging | geen antwoord op I2C-adres | I2C-adres | DEBUG |
| GB403 | huidig foutniveau | runtime, herstelpoging | antwoord, maar initialisatie van de library mislukt | I2C-adres | DEBUG |
| GB405 | huidig foutniveau | runtime, herstelpoging | I2C-busfout of timeout | I2C-adres | DEBUG |
| GB406 | huidig foutniveau | runtime, herstelpoging | verkeerde chip, teruglezen klopt niet | I2C-adres | DEBUG |
| GB407 | SUCCESS | runtime | dit component zelf is hersteld (niet: de hele tak eronder) | I2C-adres | altijd |
| GB408 | volgens de foutladder van de sensor of het kanaal | runtime, meting | sensor of kanaal kan niet meten door een storing van zijn Extender (alleen met een Sensor- of kanaal-override; die ladder vervangt dan die van de Extender voor deze meting) | I2C-adres van de Extender (+ kanaal) | altijd |
| GB409 | huidig foutniveau | runtime, herstelpoging | busherstel mislukt: SDA blijft laag, de bus blijft vast (geen `activeren()` in die poging) | I2C-adres | altijd |
| GB410 | SUCCESS | runtime, meting | de functionele meting (Sensor- of kanaalladder) is hersteld; zegt niet dat de Extender net opnieuw geactiveerd is (dat is GB407) | I2C-adres van de Extender (+ kanaal) | altijd |

Opmerkingen:

- Een standaardbuild meldt bij `inpluggen()` en `activeren()` alleen GB201 en GB301. Een DEBUG-build vervangt die door de code met de oorzaak (GB202, GB302 tot GB306). Met TRACE komt per component een extra TRACE-regel met de details.
- GB202 kan alleen op ESP32 afgaan: daar geeft `Wire.begin()` een resultaat terug. Op de andere cores is `Wire.begin()` `void`.
- GB303 bewaart de foutcode van de library in 1 byte. Een signed libraryfout zoals -102 (ADS1X15) verschijnt daardoor als 0x9A. TRACE toont de oorspronkelijke waarde -102.
- Ontbreken bij een PixelScreen zowel CS als DC, dan komt GB108 twee keer: één keer voor CS en één keer voor DC.
- Paren: GB400 (communicatie faalt) en GB407 (component hersteld); GB408 (meting faalt) en GB410 (meting hersteld).
- GB400 tot GB408 hebben geen vast niveau: de code zegt WAT er gebeurde, het foutniveau (WARNING tot FATAL) HOE ERNSTIG het nu is. Zie "Runtime-herstel" hieronder. GB402 tot GB406 spiegelen GB302 tot GB306 (zelfde laatste cijfer) en komen alleen bij de eerste mislukte herstelpoging.

## Runtime-herstel (D042)

Een los contact tijdens het draaien maakt een I2C-component niet meer voor altijd onbruikbaar.

| Stap | Wat er gebeurt |
|---|---|
| Elke handeling | `ComponentBeschikbaarTijdensRuntime()` vooraf, één `ResultaatComponentToegangTijdensRuntimeVerwerken(gelukt)` achteraf. Eén functionele handeling = één resultaat: één keypadscan, één sensormeting (ook niet-blokkerend: start, klaar en waarde samen), één Extender-pinread of -pinwrite, één LCD-melding. Niet elke library-oproep apart: een geslaagde write mag een mislukte read niet wegpoetsen. |
| Fout | `aantalOpeenvolgendeFouten` +1. Een foutniveau komt er na het aantal fouten (`RUNTIME_<NIVEAU>_OPEENVOLGENDE_FOUTEN_NA_AANTAL`) OF na de tijd sinds de eerste fout (`RUNTIME_<NIVEAU>_SINDS_EERSTE_FOUT_NA_MS`), wat eerst bereikt is, nooit hoger dan `RUNTIME_HOOGST_TOEGELATEN_FOUTNIVEAU`. Alleen een hoger foutniveau geeft een melding (GB400). |
| Vanaf `RUNTIME_COMPONENT_UITSCHAKELEN_VANAF_FOUTNIVEAU` | `actief = false` voor het component en alle onderliggende nodes. Lager: het component blijft actief en elke toegang probeert opnieuw. |
| Uitgeschakeld | Bij de volgende toegang, maar nooit vaker dan elke `RUNTIME_PAUZE_TUSSEN_HERSTELPOGINGEN_MS`, een herstelpoging (er is geen herstel op eigen initiatief): de bus vrijmaken, alleen als SDA laag hangt (`VastgelopenGedeeldeBusVrijmaken()`; SDA hoog = de bus wordt niet aangeraakt). Blijft SDA laag: GB409, geen `activeren()` in die poging. Anders de bestaande `activeren()`. Mislukt die: tweede trap in dezelfde poging. Eerst SDA opnieuw beoordelen met `VastgelopenGedeeldeBusVrijmaken()` (de eerste `activeren()` kan de bus zelf laten vastlopen; blijft SDA laag: GB409), dan `Wire.end()`, Wire opnieuw initialiseren en nog één `activeren()`. Reden: na een timeout breekt de Wire van de UNO R4 de lopende transactie niet af (`setWireTimeout()` negeert de reset, nagekeken in Wire.cpp), waarna elke transactie naar elk adres faalt, ook met SDA hoog. Dat `Wire.end()` de driver sluit (`R_IIC_MASTER_Close`) staat in de code; dat de chip daarmee echt vrijkomt, moet de hardwaretest bewijzen. Na een busherstel initialiseert Wire zo twee keer na elkaar (bewust). Met TRACE: `herstel trap 2, SDA hoog: Wire opnieuw` of `herstel trap 2, SDA laag: busherstel`. Een mislukte poging telt als fout: de ladder klimt verder op aantal en tijd, tot het hoogst toegelaten foutniveau. Melding de eerste keer en bij elk hoger foutniveau, met de oorzaak als code (GB401, DEBUG GB402 tot GB406, of GB409). |
| Hersteld | Melding GB407 (dit component zelf), daarna alle onderliggende nodes opnieuw `activeren()` (bv. RFP602 zet opnieuw de gain), daarna toestand terug op 0, altijd, ook als een onderliggende node faalt. Is de parent van een node niet actief: geen eigen melding (de parent meldde al). Mislukt zijn eigen `Activeren()`: een I2C-node zet een stap op zijn eigen ladder; een node zonder eigen toestand meldt GB401 met het foutniveau en het I2C-adres van de I2C-node erboven die herstelt (de fout hoort bij de parent; nagekeken: geen `Activeren()` raakt de resultaatfunctie van de parent). Zo'n node blijft uit; alleen een node met een eigen herstelpad (I2C, of een sensor zoals RFP602) probeert later opnieuw. |

**Meldingen:** GB400 tot GB410 gaan alleen naar Serial (of de redirect ervan), nooit naar het Character- of pixelscherm. Een melding op het scherm overschreef de opdracht van de schets (niets tekent ze terug) en testte het LCD via I2C midden in de afhandeling van een I2C-fout, waardoor een fout van de PCF8574 ook het LCD liet falen (cascade). Verloor het LCD zelf zijn stroom, dan is het leeg tot de volgende `Print()` van de schets.

**Regel:** code boven een component met runtime-herstel mag niet vooraf op `actief` blokkeren. Alleen `ComponentBeschikbaarTijdensRuntime()` beslist of de component bruikbaar is of eerst hersteld moet worden. (Gevonden in de test: Input testte `->actief`, waardoor een uitgeschakelde PCF8574 nooit meer een herstelpoging kreeg.)

**Runtime-herstel en herstel na een afwezige opstart zijn twee verschillende modellen:**

| Situatie | Herstel |
|---|---|
| Uitval tijdens runtime, na een geslaagde opstart | ✔ via `ComponentBeschikbaarTijdensRuntime()` |
| Afwezig bij de opstart: Input, LCD, Serial | ✖ bestaande beperking |
| Afwezig bij de opstart: RFP602 | ✔ via zijn eigen herstelpad |

Herstel na een afwezige opstart is dus niet generiek voor alle subsystemen. RFP602 vormt hier bewust de uitzondering doordat `OpnieuwAanmeldenNaPauze()` de volledige levenscyclus opnieuw doorloopt. Testen: Input, LCD en Serial alleen op uitval tijdens runtime; RFP602 op beide. Komen Input, LCD of Serial na afwezigheid bij de opstart niet terug, dan is dat geen fout: het is niet beloofd.

**Wat vanzelf werkt:** alles wat via het framework loopt: `ReadExtenderPin()`, `WriteExtenderPin()` en `SetExtenderPinMode()` van PCF8574, PCF8575 en MCP23017 (met `GEDEELDE_BUS_MIXED_PINS`), de ADS1115 via RFP602, Input PCF8574 en het LCD (vóór elke melding één test, omdat `LiquidCrystal_I2C` geen foutcode teruggeeft).

**Eigen oproepen in een sketch** (bv. `setPWM()` van de PCA9685): roep zelf `ComponentBeschikbaarTijdensRuntime()` vooraf en `ResultaatComponentToegangTijdensRuntimeVerwerken()` achteraf op. Voorbeeld: `examples/Systeem/GedeeldeBus/Extenders/Extender_PCA9685`.

**Niet in v2.0.0:** FRAM en EEPROM via I2C (Data leest of schrijft ze tijdens runtime nog niet), ADS7828, ADS1158, ADS7953, MAX14830 en SC16IS752 (nog geen runtime-functies), SPI-componenten (een ontbrekende chip is via SPI niet te zien).

**Overrides** (alleen wie ze zet, betaalt ze; volgorde: exemplaar, type, algemeen):

```cpp
// waarde: { {5 aantallen}, {5 tijden in ms}, hoogst toegelaten foutniveau, uitschakelen vanaf foutniveau }
#define EXTENDER_PCF8574_RUNTIME_DREMPELS_FOUTNIVEAU   { {1, 3, 5, 10, 20}, {0, 100, 500, 2000, 10000}, ScreenData::TYPE_CRITICAL, ScreenData::TYPE_CRITICAL }
#define EXTENDER_ADS1115_2_RUNTIME_DREMPELS_FOUTNIVEAU { {1, 5, 10, 15, 20}, {0, 100, 500, 2000, 10000}, ScreenData::TYPE_CRITICAL, ScreenData::TYPE_CRITICAL }
#define RFP602_RUNTIME_DREMPELS_FOUTNIVEAU             { {2, 4, 6, 8, 10},   {0, 0, 0, 0, 0},             ScreenData::TYPE_FAULT,    ScreenData::TYPE_FAULT }
#define RFP602_CHANNEL_2_RUNTIME_DREMPELS_FOUTNIVEAU   { {1, 1, 2, 2, 3},    {0, 0, 0, 0, 0},             ScreenData::TYPE_FATAL,    ScreenData::TYPE_FATAL }
```

- Per type: ADS1115, MCP23017, PCF8574 (ook die van Input), PCF8575, TCA9548A. Per exemplaar (`_1_`, `_2_`, op het I2C-adres `I2C_ADDRESS_EXTENDER_<TYPE>_<N>`): ADS1115, MCP23017, PCF8574, PCF8575.
- Eén effectieve ladder per meting: kanaal-override, anders Sensor-override, anders de ladder van de Extender zelf (exemplaar, type, algemeen). Met een Sensor- of kanaal-override telt de meting alleen op die ladder (GB408), niet ook op die van de Extender.
- Bereikt die ladder het uitschakelniveau, dan gaat de Extender zelf uit (herstel gebeurt op chipniveau), en daarmee alle kanalen en sensoren op die Extender. De toestand gaat over naar de Extender en wordt samengevoegd met wat die zelf al opbouwde (bv. door een kanaal zonder override): hoogste foutniveau, hoogste aantal fouten (niet opgeteld: zelfde storing), oudste eerste fout. Had de Extender nog geen fout, dan neemt hij de toestand gewoon over. Daarna wordt het foutniveau begrensd op het maximum van de Extender (aantal en oudste eerste fout blijven); de Extender herstelt vanaf dan volgens zijn eigen drempels. Na dat chipherstel komt alleen GB407, geen GB410: de Sensorladder ging op in de herstelpogingen van de Extender.
- Staat de Extender op "nooit automatisch uitschakelen" (zijn uitschakelniveau boven zijn maximum), dan kan geen Sensor- of kanaalladder hem uitschakelen of zijn herstelpad starten: alleen GB408. Gevolg: zolang de storing duurt, kost elke toegang de Wire-timeout en is er geen busherstel. Let op: een tolerant ingesteld kanaal beschermt de chip niet tegen een strenger ander kanaal op dezelfde chip.
- Bij het compileren gecontroleerd (`static_assert`, ook voor elke override): aantallen 1 tot 255 en oplopend, de tijden die niet 0 zijn oplopend als één reeks, ook over een 0 heen (0 = niet op tijd; `0, 100, 0, 500, 1000` ✔, `100, 0, 50, 500, 1000` ✖), beide foutniveaus `ScreenData::TYPE_WARNING` tot `ScreenData::TYPE_FATAL`.
- Uitschakelniveau hoger dan het hoogst toegelaten foutniveau mag en betekent: wel melden, nooit automatisch uitschakelen (bv. voor een optioneel component).
- Het runtime-herstel veronderstelt sequentiële toegang: niet vanuit interrupts of callbacks (de herstelcontext is gedeeld).
- Elke sensor die tijdens runtime uitgeschakeld kan worden, heeft een eigen herstelpad nodig (RFP602: `OpnieuwAanmeldenNaPauze()`, voor blokkerend en niet-blokkerend lezen).
- De Wire-timeout (`I2C_TIMEOUT_PER_TRANSACTIE_US`, standaard 25 ms) wordt gezet op AVR en UNO R4 (nagekeken in hun Wire.h). Andere cores houden hun eigen standaard.
- Geheugen per I2C-component: 11 bytes op AVR, 12 op 32-bit (twee aparte tijdvelden, zodat ook een uitgeschakeld component verder klimt).

## Native resources

`HardwareResourcePin` gebruikt `NP_D0..NP_D13`, `NP_A0..NP_A5`, `NP_SDA`, `NP_SCL`, `NP_MISO`, `NP_MOSI`, `NP_SCK` en `NP_SS` voor de echte native pinnen. `CUSTOM` en `NONE` blijven zonder `NP_`, omdat dit speciale waarden zijn en geen pinnen. `NativeArduinoPinVan()` vertaalt de native pinwaarden naar de boardmapping in `SystemConfig.h`.

ExtenderPins zijn geen `HardwareResourcePin` en worden niet via `NativeArduinoPinVan()` vertaald.

De waarden van `HardwareResourceType` gebruiken overal `RT_`. De vijf groepswaarden in `GedeeldeBusComponent` zijn `GC_INPUT`, `GC_SCREEN`, `GC_DATA`, `GC_EXTENDER` en `GC_SENSOR`; concrete componentwaarden, zoals `INPUT_PCF8574`, `ADC_ADS1115` en `RFP602`, behouden hun eigen naam.

## Claims en conflicten

`BezettingPinnen` bevat gedeelde en exclusieve claims. I2C en SPI gebruiken daarnaast `HardwareResourceIdentiteit` voor de gedeelde lijnen.

De conflictvergelijking werkt alleen binnen hetzelfde resourcegebied. `firstExtenderNode()` bepaalt of twee nodes Native resources of resources van dezelfde Extender gebruiken.

**Gemengde pinnen** (`GEDEELDE_BUS_MIXED_PINS`; standaard `0`): een component mag per exclusieve pin een eigen gebied hebben, het board of een Extender. Voorbeeld: een SPI-chip met SCK, MISO en MOSI op het board en CS op een PCF8574. `ownerClaim` geeft per exclusieve claim de Extender (`nullptr` = het gewone gebied, zoals hierboven); `GetOwnerClaim()` geeft het gebied van één claim. De conflictcontrole vergelijkt dan per claim, en een pin op een eigen Extender moet op die Extender bestaan (GB111). Extenders met digitale pinnen (PCF8574, PCF8575, MCP23017) bieden daarvoor `SetExtenderPinMode()`, `WriteExtenderPin()` en `ReadExtenderPin()`. Staat de instelling op `0`, dan wordt niets hiervan gecompileerd en blijft het geheugengebruik gelijk. Een Data-chip met CS via een Extender zet de instelling automatisch op `1`. Sensoren kunnen dit gebruiken door `ownerClaim` te vullen; elke exclusieve pin kan naar een andere Extender wijzen. Voorbeeld: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_MixedPins/Sensor_MixedPins.ino` (2 pinnen op het board, 2 op een PCF8574). RFP602 doet dat nog niet, en de ADC-routes blijven per backend gescheiden.

I2C-adressen worden intern met bit `0x80` gemarkeerd en als exclusieve claim behandeld. Daardoor kan de generieke claimvergelijking pinnen en adressen onderscheiden.

## I2C en SPI

`HardwareResourceTypeI2C`:

- gedeelde claims: SDA, SCL;
- exclusieve claim: I2C-adres;
- `inpluggen()` initialiseert `Wire` via `InitialiserenGedeeldeBus()`;
- bevat generieke byte-/register-read/write helpers.

`HardwareResourceTypeSPI`:

- gedeelde claims: SCK, MISO, MOSI;
- exclusieve claim: CS;
- `inpluggen()` vereist CS en initialiseert `SPI`;
- bevat `Transfer()`.

## Businitialisatie

`InitialiserenGedeeldeBus()` bewaart aparte interne flags voor I2C en SPI. `ResettenGedeeldeBus()` zet beide flags terug op `false`. Op `BOARD_ESP32S3_ARDI32` en `BOARD_ESP32S3_DEV` wordt I2C expliciet gestart met de geconfigureerde SDA/SCL-pinnen; andere boards gebruiken `Wire.begin()`.

## Extenders en ExtenderPins

Alle 16 concrete Extenders staan in afzonderlijke `.h/.cpp`-bestanden. Iedere waarde in `enum class ExtenderPins` begint met `EP_`.

Alleen Extenders die compile-time effectief gebruikt worden, worden mee gecompileerd. Een `EXTENDER_<NAAM>_AANTAL` groter dan 0 activeert de overeenkomstige Extender. `ExtenderADS1115` wordt daarnaast mee gecompileerd wanneer `ADC_BACKEND == ADC_BACKEND_ADS1115`, omdat `ADC_ADS1115` ervan erft. `ExtenderPCF8574` wordt daarnaast mee gecompileerd wanneer `INPUT_KANAAL_CONFIG` `INPUT_TYPE_PCF8574` bevat, omdat `InputPCF8574` dezelfde codebasis gebruikt. Niet gebruikte Extenderheaders worden niet geïncludeerd; de afzonderlijke Extender-`.cpp` levert alleen code wanneer de daarin benodigde meervoudige `ExtenderLijst` werkelijk nodig is.

| Extender | actuele `ExtenderPins` uit de header |
|---|---|
| `ExtenderADS1115` | `EP_AIN0..EP_AIN3` |
| `ExtenderADS1158` | `EP_AIN0..EP_AIN15`, `EP_GPIO0..EP_GPIO7` |
| `ExtenderADS7828` | `EP_CH0..EP_CH7` |
| `ExtenderADS7953` | `EP_CH0..EP_CH15`, `EP_GPIO0..EP_GPIO3` |
| `ExtenderCD74HC4067` | `EP_Y0..EP_Y15` |
| `ExtenderDS2482v800` | `EP_IO0..EP_IO7` |
| `ExtenderMAX14830I2C` | `EP_UART0..EP_UART3`, `EP_GPIO0..EP_GPIO15` |
| `ExtenderMAX14830SPI` | `EP_UART0..EP_UART3`, `EP_GPIO0..EP_GPIO15` |
| `ExtenderMCP23017` | `EP_GPA0..EP_GPA7`, `EP_GPB0..EP_GPB7` |
| `ExtenderMCP23S17` | `EP_GPA0..EP_GPA7`, `EP_GPB0..EP_GPB7` |
| `ExtenderPCA9685` | `EP_PWM0..EP_PWM15` |
| `ExtenderPCF8574` | `EP_P0..EP_P7` |
| `ExtenderPCF8575` | `EP_P00..EP_P07`, `EP_P10..EP_P17` |
| `ExtenderSC16IS752I2C` | `EP_CHANNEL_A`, `EP_CHANNEL_B`, `EP_GPIO0..EP_GPIO7` |
| `ExtenderSC16IS752SPI` | `EP_CHANNEL_A`, `EP_CHANNEL_B`, `EP_GPIO0..EP_GPIO7` |
| `ExtenderTCA9548A` | `EP_CH0..EP_CH7` |

De concrete controlpinnen van een Extender, zoals CS, IRQ, RESET, START, PWDN of de selectorlijnen van de CD74HC4067, blijven Native `HardwareResourcePin`-claims wanneer ze aan het Arduino-board gekoppeld zijn.

## ADC-parents

`ADC_NATIVE` en `ADC_ADS1115` zijn statische parents voor Sensoren. Welke van beide wordt aangemaakt is compile-time afhankelijk van `ADC_BACKEND`.

## Screen, Input en Sensor

`GedeeldeBus.h` definieert de generieke childtypes die deze subsystemen gebruiken:

- `CharacterScreen`, `PixelScreen`, `SerialOutput`;
- `InputDigital`, `InputPCF8574`, `InputHX1838`;
- `Sensor`.

De volledige Screen- en Inputlogica staat niet in GedeeldeBus maar in `src/Systeem/Screen/` en `src/Systeem/Input/`. Concrete Sensorcode staat onder `src/Systeem/Sensor/`.

## Tests die nu aanwezig zijn

Onder `examples/Systeem/GedeeldeBus/Extenders/` bestaan per Extender:

- `Extender_<naam>.ino`: basis lifecycle/resource-test;
- `Extender_<naam>_Test_ExtenderPins_Toegelaten.ino`;
- `Extender_<naam>_Test_ExtenderPins_Geweigerd.ino`.

Onder `examples/Systeem/GedeeldeBus/Sensoren/` staan `Sensor_RFP602.ino`, `Sensor_RFP602_DIRECT.ino`, `Sensor_RFP602_FLASH.ino` en `Sensor_RFP602_MICROSD.ino`.

Deze tests moeten niet worden geïnterpreteerd als een volledige elektronische functietest van alle chipfuncties. Zie `extras/TESTEN.md`.
