# Relais

## Component

Relais. De component is header-only geïmplementeerd.

## Implementatie

- Broncode: `src/Systeem/Sturing/Relais.h`.
- C++-component: `Relais`, rechtstreeks afgeleid van `GedeeldeBusNode`.
- Aansluiting: Native GPIO via `digitalWrite()` en `pinMode()`, of een digitaal Extender-kanaal via `WriteExtenderPin()` en `SetExtenderPinMode()`.
- Bestaande aansturing: `setRelayStatusOnOff(bool aan)`.
- Lifecycle: de component bevat een `Activeren()`-override en een `StopSturing()`-override voor uitschakeling.
- Configuratie: `src/Configuratie/Sturingen/ConfigRelais_template.h`; optioneel persoonlijk `ConfigRelais.h`.

## Aandachtspunt

Voor GPIO-Extenders zijn de bestaande Extender-GPIO-functies nodig, niet `GEDEELDE_BUS_MIXED_PINS`; actief-laag/actief-hoog en foutveiligheid moeten overeenkomen met de module.

## ISD1820

De gewone ISD1820-sketch gebruikt twee `Relais`-objecten: REC (hoog tijdens opname) en PLAYE (korte puls voor afspelen). De begeleide hardwaretest gebruikt vier `Relais`-objecten voor REC, PLAYE, PLAYL en FT. Er worden geen fysieke relaismodules gebruikt. Voorbeeld: `examples/Systeem/GedeeldeBus/Sturingen/Sturing_ISD1820/`.

## Aansluiten

**Aansluitwijze in de bestaande FrameWork-code:**

- De signaalverbindingen worden hieronder per onderdeel gespecificeerd waar ze uit de bestaande componentinterface blijken.
- Controleer voedingsspanning, pinvolgorde en externe stroombegrenzing bij de concrete hardware-uitvoering voordat je spanning aanlegt.

- Besturingsingang IN → één digitaal kanaal van de GPIO-parent (bijvoorbeeld PCF8574), zoals gekozen in de constructor.
- Relaismodule-voeding en GND volgens de module; bij een externe voeding de voorgeschreven referentie/isolatie van de module volgen.
- Contacten COM/NO/NC uitsluitend aansluiten op een **geschikte, veilig afgeschermde testbelasting**. **Geen netspanning aansluiten voor deze softwaretesten.**
- Controleer actieve-laag/actieve-hoog logica en rusttoestand vóór de belasting wordt aangesloten.

**Voor de hardwaretest:**

- Gebruik de bestaande `.ino` uit **Testen** en de bestaande configuratiebestanden onder `src/Configuratie/`; een `*_template.h` wordt niet rechtstreeks ingelezen.
- **Een vaste Arduino-pintoewijzing is niet algemeen geldig:** de gekozen constructor, configuratie en werkelijke bekabeling moeten overeenkomen. Waar hierboven geen exact spanningsniveau of pinheader is bevestigd, mag dat niet uit deze documentatie worden afgeleid.


**Aanvulling aansluitpinnen en elektrische voorwaarden:**

- IN→gekozen GPIO-Extender-uitgang; module-GND en voedingsrail conform module. Controleer of IN actief-laag of actief-hoog is en of JD-VCC/VCC galvanische scheiding mogelijk maakt. Gebruik voor netspanning uitsluitend veilig behuisde en geschikte hardware.
- De concrete modulevariant bepaalt de **exacte voedingsgrenzen en fysieke pinvolgorde**. Neem die gegevens niet over van een andere module zonder de werkelijke print te controleren.

**Aansluit- en gebruiksgegevens uit de bestaande FrameWork-interface:**

- **Stuursignaal:** één GPIO-Extender-uitgang `pin` → relaismodule-`IN`. Gemeenschappelijke GND en een voedingsrail geschikt voor de relaismodule. Gebruik `relayValueOnAtCreation` conform de fysieke actieve HIGH-/LOW-polariteit.
- De relaiscontacten (`COM`, `NO`, `NC` waar aanwezig) zijn **gescheiden lastcontacten**, geen GPIO-aansluitingen; belastingen uitsluitend binnen de concrete module-/contactwaardes aansluiten.
- De constructor krijgt `relayValueOnAtCreation`: dit is het **actieve ingangsniveau** van de aangesloten relaismodule (`HIGH` of `LOW`). Bij activeren initialiseert de code het kanaal eerst op het tegenovergestelde niveau en daarna als `OUTPUT`.
- `setRelayStatusOnOff(bool on)` schrijft het actieve niveau wanneer `on=true` en het tegenovergestelde niveau wanneer `on=false`. `StopSturing()` schrijft de uit-toestand.
- `GEDEELDE_BUS_MIXED_PINS` is niet vereist voor Relais. Gebruik op de relaiscontacten uitsluitend lasten die binnen de specificaties van de exacte relaismodule vallen; het GPIO-signaal voedt de last niet.

**Bestaande constructor (parameter-/pinvolgorde):**

```cpp
Relais(GedeeldeBusNode* parent,uint8_t pin,uint8_t relayValueOnAtCreation)
```

**Configuratie voor deze hardware:**

- Gebruik bij een eigen configuratie `src/Configuratie/Sturingen/ConfigRelais.h` (het aanwezige `src/Configuratie/Sturingen/ConfigRelais_template.h` is een kopieersjabloon en wordt niet rechtstreeks ingelezen).
- Voor één aangesloten exemplaar: `STURING_RELAIS_AANTAL` op **1**, behalve wanneer de bestaande groeps-/setconfiguratie uitdrukkelijk een andere betekenis heeft.
- De bestaande configuratie bevat deze aansluitrelevante definities (gebruik de **werkelijke aangesloten** pinnen, adressen en referenties):
  - `STURING_RELAIS_AANTAL`: sjabloonwaarde `0`.

**Letterlijk constructorfragment uit de bestaande voorbeeldsketch** (alle gebruikte parent-objecten moeten zelf correct aangesloten en geïnitialiseerd zijn):

```cpp
Relais sturing(&extender,0,LOW);
```

**Bestaande test-/voorbeeldsketches:**

- `examples/Systeem/GedeeldeBus/Sturingen/Sturing_Relais_Native_HardwareTest/Sturing_Relais_Native_HardwareTest.ino`
- `examples/Systeem/GedeeldeBus/Sturingen/Sturing_Relais_PCF8574_HardwareTest/Sturing_Relais_PCF8574_HardwareTest.ino`
- `examples/Systeem/GedeeldeBus/Sturingen/Sturing_Relais_PCF8575_HardwareTest/Sturing_Relais_PCF8575_HardwareTest.ino`
- `examples/Systeem/GedeeldeBus/Sturingen/Sturing_Relais_MCP23017_HardwareTest/Sturing_Relais_MCP23017_HardwareTest.ino`
- `examples/Systeem/GedeeldeBus/Sturingen/Sturing_Relais_MCP23S17_HardwareTest/Sturing_Relais_MCP23S17_HardwareTest.ino`
- `examples/Systeem/GedeeldeBus/Sturingen/Sturing_Relais/Sturing_Relais.ino`


### Praktische aansluiting en gebruik

**Praktisch aansluiten via een GPIO-Extender:** verbind de **logische besturingsingang** `IN` van de relaismodule met de door de constructor gekozen `pin` van de GPIO-parent. Verbind module-voeding en stuur-GND alleen volgens het concrete moduleontwerp. Stel `relayValueOnAtCreation` in op `LOW` bij een **actief-laag** module of `HIGH` bij een **actief-hoog** module. De bestaande `Activeren()` zet de uitgang eerst op het tegengestelde niveau en configureert vervolgens `OUTPUT`; `StopSturing()` schrijft eveneens de uit-toestand.

- Test met een **laagspanningslast**: contact `COM` en `NO` zijn in rust open; `COM` en `NC` zijn in rust gesloten, indien de module deze contacten aanbiedt. De contactzijde is **geen voedingspin voor de microcontroller**. Gebruik geen netspanning in deze software-/hardwaretest.
- Bij PCF8574/PCF8575-uitgangen eerst de quasi-bidirectionele uitgangsstroom en de inputstroom/polariteit van `IN` toetsen. Een losse relaisspoel mag niet direct op een PCF8574-pin: gebruik een geschikte driver en vrijloopbeveiliging of een daarvoor ontworpen relaismodule. De code bepaalt **niet** of de gebruikte module echt galvanisch geïsoleerd is.


**Let op bij een I²C-PCF8574-parent:** een PCF8574 heeft **quasi-bidirectionele** I/O; volgens de officiële chipdocumentatie zijn hoog en laag geen symmetrische vermogensuitgangen. De relaismodule moet dus een **eigen passende spoeldriver** en een stuuringang hebben die elektrisch compatibel is met de beschikbare uitgang. Een kale relaiswikkeling rechtstreeks tussen één PCF8574-pin en GND is geen toegelaten vervanging voor een relaismodule. Bij een optisch geïsoleerde module gelden de door de modulefabrikant opgegeven VCC/JD-VCC/GND-schakelingen; sluit die niet blind samen. Gebruik voor de eerste test een volledig laagspanningscircuit.

**Chipdocumentatie parent (indien PCF8574):** https://www.ti.com/product/PCF8574

### Aansluiting van de aanwezige relaisuitvoeringen

De gebruikte hardware omvat **meerdere afzonderlijke relaismodules**. Het zijn **geen onderling uitwisselbare voedings- of ingangsschakelingen**:

- Op het **16-kanaalsbord** staan relais met het opschrift **`SRD-12VDC-SL-C`**: dit beschrijft een **12V-relaisspoel**. Het bepaalt niet zonder het bordschema of de aansluiting voor de besturing 5 V, 12 V of een gescheiden voedingscircuit vereist.
- Op andere modules staan **`SRD-05VDC-SL-C`** en een vierkanaalsbord met **5V-spoelrelais**. Ook hier blijft het actieve ingangs-`HIGH`/`LOW` en de eventuele jumper tussen logica- en spoelvoeding **per print** te controleren.
- De bestaande FrameWork-`Relais`-klasse bestuurt **één relaisingang per object** via één GPIO-Extenderkanaal. Voor een bord met meerdere afzonderlijke ingangen moet elke gebruikt ingang een eigen kanaal en object krijgen, overeenkomstig de bestaande configuratie; de klasse heeft geen automatische 16-kanaalsborddetectie.
- De `COM`/`NO`/`NC`-aansluitingen behoren tot de **geschakelde lastzijde** en zijn nooit de logische `IN`- of GND-aansluiting. Verifieer altijd de echte schroefklemopschriften en contactbelastbaarheid.
- Schakel de eerste test uitsluitend met een **laagspanningslast**. Een I²C-Extenderpin mag geen losse relaiswikkeling rechtstreeks voeden; de module moet een compatibele ingangsdriver hebben.

## Testen

### Concrete aansluiting en uitvoering van de bestaande sketches

Deze gegevens gelden voor de hieronder genoemde **bestaande sketches** en hun echte constructoren.
Ze vervangen de overige technische documentatie niet. Een succesvolle functieaanroep
of levenscyclus is geen bewijs van een fysieke beweging, licht- of contactreactie.

#### `Sturing_Relais.ino`

**Exact pad:** `examples/Systeem/GedeeldeBus/Sturingen/Sturing_Relais/Sturing_Relais.ino`.

- Arduino SDA -> SDA van de I2C-Extender; Arduino SCL -> SCL; GND -> GND.
- Voed de Extender-logica op het voor board en I2C-niveau geschikte VCC/VDD.
- Verifieer fysiek I2C-adres en optionele INT/RESET-pinnen met de actieve Config*.h.
- Bij 3,3V-logica op een 5V-bus: controleer een passende I2C-niveauomzetter.
- Gebruik een LOW-actieve relaisMODULE met geschikte stuurspanning en voeding.
- Relaismodule VCC en GND volgens het bord; signaalbron en relais-GND verbinden.
- Alleen een SPANNINGSVRIJ laagspanningscontact testen: COM -> GND, NO -> meetpin.
- Geen netspanning op een Arduino- of Extender-meetpin aansluiten.
- Deze .ino schakelt niet fysiek; START/1/0/STOP zitten in de HardwareTests.
- Verwacht een relaisklik EN overgang van het terugmeldsignaal (NO open/gesloten).
- Actieve configuratie: src/Configuratie/Sturingen/ConfigRelais.h,
- STURING_RELAIS_AANTAL >= 1.
- PCF8574 P0 -> relais IN; deze .ino voert geen schakeltest uit.
- Actieve Extenderconfig: ConfigPCF8574.h (EXTENDER_PCF8574_AANTAL >= 1).

**Uitvoering:** zie de start- en loopfunctie van deze bestaande sketch;
controleer of deze sketch zelf uitvoer activeert voordat je een fysiek resultaat verwacht.

#### `Sturing_Relais_Native_HardwareTest.ino`

**Exact pad:** `examples/Systeem/GedeeldeBus/Sturingen/Sturing_Relais_Native_HardwareTest/Sturing_Relais_Native_HardwareTest.ino`.

- Gebruik een LOW-actieve relaisMODULE met geschikte stuurspanning en voeding.
- Relaismodule VCC en GND volgens het bord; signaalbron en relais-GND verbinden.
- Alleen een SPANNINGSVRIJ laagspanningscontact testen: COM -> GND, NO -> meetpin.
- Geen netspanning op een Arduino- of Extender-meetpin aansluiten.
- Na START: 1 + Enter = relais AAN; 0 + Enter = UIT; STOP schakelt UIT.
- Verwacht een relaisklik EN overgang van het terugmeldsignaal (NO open/gesloten).
- Actieve configuratie: src/Configuratie/Sturingen/ConfigRelais.h,
- STURING_RELAIS_AANTAL >= 1.
- Arduino D6 -> relais IN (actief LOW), D2 (INPUT_PULLUP) -> NO;
- COM -> GND; rust NO open: HIGH, AAN NO gesloten: LOW.

**Uitvoering:** gebruik de in de sketch beschreven START-/STOP-/Serial-commando's.
Beoordeel het werkelijk aangesloten apparaat, niet uitsluitend `RESULTAAT: OK`.

#### `Sturing_Relais_PCF8574_HardwareTest.ino`

**Exact pad:** `examples/Systeem/GedeeldeBus/Sturingen/Sturing_Relais_PCF8574_HardwareTest/Sturing_Relais_PCF8574_HardwareTest.ino`.

- Arduino SDA -> SDA van de I2C-Extender; Arduino SCL -> SCL; GND -> GND.
- Voed de Extender-logica op het voor board en I2C-niveau geschikte VCC/VDD.
- Verifieer fysiek I2C-adres en optionele INT/RESET-pinnen met de actieve Config*.h.
- Bij 3,3V-logica op een 5V-bus: controleer een passende I2C-niveauomzetter.
- Gebruik een LOW-actieve relaisMODULE met geschikte stuurspanning en voeding.
- Relaismodule VCC en GND volgens het bord; signaalbron en relais-GND verbinden.
- Alleen een SPANNINGSVRIJ laagspanningscontact testen: COM -> GND, NO -> meetpin.
- Geen netspanning op een Arduino- of Extender-meetpin aansluiten.
- Na START: 1 + Enter = relais AAN; 0 + Enter = UIT; STOP schakelt UIT.
- Verwacht een relaisklik EN overgang van het terugmeldsignaal (NO open/gesloten).
- Actieve configuratie: src/Configuratie/Sturingen/ConfigRelais.h,
- STURING_RELAIS_AANTAL >= 1.
- PCF8574 P0 -> relais IN (actief LOW), P1 -> NO; COM -> GND.
- I2C-extender: ConfigPCF8574.h (EXTENDER_PCF8574_AANTAL >= 1).

**Uitvoering:** gebruik de in de sketch beschreven START-/STOP-/Serial-commando's.
Beoordeel het werkelijk aangesloten apparaat, niet uitsluitend `RESULTAAT: OK`.

#### `Sturing_Relais_PCF8575_HardwareTest.ino`

**Exact pad:** `examples/Systeem/GedeeldeBus/Sturingen/Sturing_Relais_PCF8575_HardwareTest/Sturing_Relais_PCF8575_HardwareTest.ino`.

- Arduino SDA -> SDA van de I2C-Extender; Arduino SCL -> SCL; GND -> GND.
- Voed de Extender-logica op het voor board en I2C-niveau geschikte VCC/VDD.
- Verifieer fysiek I2C-adres en optionele INT/RESET-pinnen met de actieve Config*.h.
- Bij 3,3V-logica op een 5V-bus: controleer een passende I2C-niveauomzetter.
- Gebruik een LOW-actieve relaisMODULE met geschikte stuurspanning en voeding.
- Relaismodule VCC en GND volgens het bord; signaalbron en relais-GND verbinden.
- Alleen een SPANNINGSVRIJ laagspanningscontact testen: COM -> GND, NO -> meetpin.
- Geen netspanning op een Arduino- of Extender-meetpin aansluiten.
- Na START: 1 + Enter = relais AAN; 0 + Enter = UIT; STOP schakelt UIT.
- Verwacht een relaisklik EN overgang van het terugmeldsignaal (NO open/gesloten).
- Actieve configuratie: src/Configuratie/Sturingen/ConfigRelais.h,
- STURING_RELAIS_AANTAL >= 1.
- PCF8575 P00 -> relais IN (actief LOW), P01 -> NO; COM -> GND.
- I2C-extender: ConfigPCF8575.h (EXTENDER_PCF8575_AANTAL = 1).

**Uitvoering:** gebruik de in de sketch beschreven START-/STOP-/Serial-commando's.
Beoordeel het werkelijk aangesloten apparaat, niet uitsluitend `RESULTAAT: OK`.

#### `Sturing_Relais_MCP23017_HardwareTest.ino`

**Exact pad:** `examples/Systeem/GedeeldeBus/Sturingen/Sturing_Relais_MCP23017_HardwareTest/Sturing_Relais_MCP23017_HardwareTest.ino`.

- Arduino SDA -> SDA van de I2C-Extender; Arduino SCL -> SCL; GND -> GND.
- Voed de Extender-logica op het voor board en I2C-niveau geschikte VCC/VDD.
- Verifieer fysiek I2C-adres en optionele INT/RESET-pinnen met de actieve Config*.h.
- Bij 3,3V-logica op een 5V-bus: controleer een passende I2C-niveauomzetter.
- Gebruik een LOW-actieve relaisMODULE met geschikte stuurspanning en voeding.
- Relaismodule VCC en GND volgens het bord; signaalbron en relais-GND verbinden.
- Alleen een SPANNINGSVRIJ laagspanningscontact testen: COM -> GND, NO -> meetpin.
- Geen netspanning op een Arduino- of Extender-meetpin aansluiten.
- Na START: 1 + Enter = relais AAN; 0 + Enter = UIT; STOP schakelt UIT.
- Verwacht een relaisklik EN overgang van het terugmeldsignaal (NO open/gesloten).
- Actieve configuratie: src/Configuratie/Sturingen/ConfigRelais.h,
- STURING_RELAIS_AANTAL >= 1.
- MCP23017 GPA0 -> relais IN (actief LOW), GPA1 -> NO; COM -> GND.
- I2C-extender: ConfigMCP23017.h (EXTENDER_MCP23017_AANTAL >= 1).

**Uitvoering:** gebruik de in de sketch beschreven START-/STOP-/Serial-commando's.
Beoordeel het werkelijk aangesloten apparaat, niet uitsluitend `RESULTAAT: OK`.

#### `Sturing_Relais_MCP23S17_HardwareTest.ino`

**Exact pad:** `examples/Systeem/GedeeldeBus/Sturingen/Sturing_Relais_MCP23S17_HardwareTest/Sturing_Relais_MCP23S17_HardwareTest.ino`.

- Gebruik een LOW-actieve relaisMODULE met geschikte stuurspanning en voeding.
- Relaismodule VCC en GND volgens het bord; signaalbron en relais-GND verbinden.
- Alleen een SPANNINGSVRIJ laagspanningscontact testen: COM -> GND, NO -> meetpin.
- Geen netspanning op een Arduino- of Extender-meetpin aansluiten.
- Na START: 1 + Enter = relais AAN; 0 + Enter = UIT; STOP schakelt UIT.
- Verwacht een relaisklik EN overgang van het terugmeldsignaal (NO open/gesloten).
- Actieve configuratie: src/Configuratie/Sturingen/ConfigRelais.h,
- STURING_RELAIS_AANTAL >= 1.
- MCP23S17 GPA0 -> relais IN (actief LOW), GPA1 -> NO; COM -> GND.
- SPI-extender: SI -> MOSI, SO -> MISO, SCK -> SCK,
- CS -> EXTENDER_MCP23S17_1_CS_PIN uit ConfigMCP23S17.h.
- VDD, VSS, RESET, adresselectie A0/A1/A2 volgens de echte MCP23S17-print.
- ConfigMCP23S17.h: EXTENDER_MCP23S17_AANTAL >= 1.

**Uitvoering:** gebruik de in de sketch beschreven START-/STOP-/Serial-commando's.
Beoordeel het werkelijk aangesloten apparaat, niet uitsluitend `RESULTAAT: OK`.

### ISD1820 via de bestaande Relais-klasse

Er is geen afzonderlijke `ISD1820.md`; daarom worden de twee reeds bestaande
ISD1820-sketches hier beschreven zonder een nieuwe bestandsnaam aan te maken.

#### `Sturing_ISD1820.ino`

**Exact pad:** `examples/Systeem/GedeeldeBus/Sturingen/Sturing_ISD1820/Sturing_ISD1820.ino`.

- Arduino SDA -> SDA van de I2C-Extender; Arduino SCL -> SCL; GND -> GND.
- Voed de Extender-logica op het voor board en I2C-niveau geschikte VCC/VDD.
- Verifieer fysiek I2C-adres en optionele INT/RESET-pinnen met de actieve Config*.h.
- Bij 3,3V-logica op een 5V-bus: controleer een passende I2C-niveauomzetter.
- MCP23017 GPA0 -> ISD1820 REC, GPA1 -> P-E/PLAYE.
- Voor HardwareTest ook GPA2 -> P-L/PLAYL en GPA3 -> FT.
- MCP23017 GND -> ISD1820 GND; luidspreker -> SP+ en SP-.
- ISD1820 VCC -> de volgens de werkelijke module toegelaten voeding:
-   IC-specificatie 2,7..4,5 V; geen 5V-GPIO op ISD zonder verificatie.
- Geen fysieke relais: de bestaande Relais-klasse schakelt direct GPIO.
- Bij een 5V-I2C-host en 3,3V-MCP23017 passende niveauomzetting controleren.
- Actieve configs: src/Configuratie/Extenders/ConfigMCP23017.h en
-   src/Configuratie/Sturingen/ConfigRelais.h.
- Deze .ino neemt direct na setup 5 s op en geeft PLAYE-puls; luister.
- RESULTAAT: OK bevestigt de fysieke luidsprekerweergave NIET.

#### `Sturing_ISD1820_HardwareTest.ino`

**Exact pad:** `examples/Systeem/GedeeldeBus/Sturingen/Sturing_ISD1820_HardwareTest/Sturing_ISD1820_HardwareTest.ino`.

- Arduino SDA -> SDA van de I2C-Extender; Arduino SCL -> SCL; GND -> GND.
- Voed de Extender-logica op het voor board en I2C-niveau geschikte VCC/VDD.
- Verifieer fysiek I2C-adres en optionele INT/RESET-pinnen met de actieve Config*.h.
- Bij 3,3V-logica op een 5V-bus: controleer een passende I2C-niveauomzetter.
- MCP23017 GPA0 -> ISD1820 REC, GPA1 -> P-E/PLAYE.
- Voor HardwareTest ook GPA2 -> P-L/PLAYL en GPA3 -> FT.
- MCP23017 GND -> ISD1820 GND; luidspreker -> SP+ en SP-.
- ISD1820 VCC -> de volgens de werkelijke module toegelaten voeding:
-   IC-specificatie 2,7..4,5 V; geen 5V-GPIO op ISD zonder verificatie.
- Geen fysieke relais: de bestaande Relais-klasse schakelt direct GPIO.
- Bij een 5V-I2C-host en 3,3V-MCP23017 passende niveauomzetting controleren.
- Actieve configs: src/Configuratie/Extenders/ConfigMCP23017.h en
-   src/Configuratie/Sturingen/ConfigRelais.h.
- Hardwaretest: REC, PLAYE, PLAYL en FT in Serial handmatig doorlopen.
- Hoorbare opname/afspelen en directe microfoon-doorvoer fysiek beoordelen.
- STOP kan een al gestarte synchrone opname van 5 s niet onderbreken.



De huidige implementatie bevat een voorbeeld voor deze Sturing in `examples/` en het script `extras/TestLibrarySturingen-v2.0.0.cmd`. 

Configuratie: `STURING_RELAIS_AANTAL`: 0 = niet opgenomen; groter dan 0 = opgenomen; het aantal objecten wordt niet begrensd.

`Activeren()` ontvangt geen resultaat van `WriteExtenderPin()` of `SetExtenderPinMode()` (beide `void`); eventuele I²C-fouten worden door de bestaande Extender-runtimeafhandeling verwerkt.

## Hardwaretestbediening

De vijf `Sturing_Relais_*_HardwareTest.ino`-voorbeelden gebruiken dezelfde bediening.

- `START` + Enter bevestigt dat voeding en bedrading gecontroleerd zijn.
- `1` + Enter: relais AAN, NO-contact hoort te sluiten.
- `0` + Enter: relais UIT, NO-contact hoort te openen.
- `STOP` + Enter: relais UIT, test beëindigd; herstart de Arduino.
- Serial vermeldt de fysieke meetpin, het ruwe HIGH/LOW-meetniveau (1/0),
  de NO-contactstatus en PASS/FAIL. De functieaanroep is geen bewijs van
  fysieke contactwerking.
- Terugmelding: NO naar Native D2, COM naar GND. Alleen spanningsvrije
  testcontacten; geen externe spanning op D2.
- Het enige verschil tussen de vijf tests is de relaisaansturing: Native D6,
  PCF8574 P0, PCF8575 P0, MCP23017 GPA0 of MCP23S17 GPA0.

### Contactterugmelding en herstel

- Verifieer werkelijke contactschakeling met de bestaande NO/COM-teruglus: relais UIT = NO open, relais AAN = NO gesloten. Herhaal bij busfout, voeding uit en reset. Succes van `setRelayStatusOnOff()` zonder teruglus bewijst alleen de opdracht.
- **Status:** fysieke proef vereist; nog geen hardwarebewijs.

### ISD1820 via MCP23017 zonder fysieke relais

- Gewone test blijft `examples/Systeem/GedeeldeBus/Sturingen/Sturing_ISD1820/Sturing_ISD1820.ino`.
- Nieuwe begeleide hardwaretest: `examples/Systeem/GedeeldeBus/Sturingen/Sturing_ISD1820_HardwareTest/Sturing_ISD1820_HardwareTest.ino`.
- MCP23017 GPA0 → REC, GPA1 → P-E / PLAYE, GPA2 → P-L / PLAYL en GPA3 → FT. Alle GND-aansluitingen gemeenschappelijk. De vier `Relais`-objecten sturen rechtstreeks digitale signalen, zonder fysieke relais.
- Voor deze hardwaretest: `STURING_RELAIS_AANTAL=4` en `EXTENDER_MCP23017_AANTAL=1` (de gewone `Sturing_ISD1820.ino` blijft twee objecten gebruiken).
- `START` bevestigt. `REC` neemt vijf seconden op, `PLAY` geeft een PLAYE-puls van 100 ms en `PLAYL` houdt P-L HIGH tot `PLAYLSTOP` of `STOP`; hierdoor kan het afspelen vroegtijdig gestopt worden. `FT` schakelt microfoon-doorluistering in tot `FTSTOP` of `STOP`.
- Herhaal `REC` en `PLAY` voor meerdere proefopnames. Beoordeel afzonderlijk de afspeelkwaliteit, het afbreken via `PLAYLSTOP` en het in-/uitschakelen van FT. `OK` en `FOUT` vermelden de manuele beoordeling in Serial; leg per handeling de uitslag ook vast in het HTML-register. Een geslaagde levenscyclus bewijst geen hoorbaar resultaat.
- Tijdens de synchrone opname van vijf seconden wordt `STOP` pas nadien gelezen. Bij een GPIO-fout kan LOW niet gegarandeerd worden; onderbreek zo nodig de modulevoeding.
- Verwijder eventuele P-E/FT-jumpers van de module bij externe GPIO-bediening. Controleer dat het concrete bord zijn stuuringangen werkelijk beschikbaar heeft. Gebruik compatibele voeding en logicaniveaus; geen 5V-signalen rechtstreeks op een 3,3V-module.
