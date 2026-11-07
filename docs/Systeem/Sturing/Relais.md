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
