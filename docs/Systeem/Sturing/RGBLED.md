# RGBLED

## Component

RGB LED. De component is header-only geïmplementeerd.

## Implementatie

- Broncode: `src/Systeem/Sturing/RGBLED.h`.
- C++-component: `RGBLED`, rechtstreeks afgeleid van `GedeeldeBusNode`.
- Aansluiting: Drie PWM-kanalen op de parent; omkering is voorzien.
- Bestaande aansturing: `setRGBcolor(uint16_t rood, uint16_t groen, uint16_t blauw)`.
- Lifecycle: de component bevat een `Activeren()`-override en een `StopSturing()`-override voor uitschakeling.
- Configuratie: `src/Configuratie/Sturingen/ConfigRGBLED_template.h`; optioneel persoonlijk `ConfigRGBLED.h`.

## Aandachtspunt

Type RGB-module, gemeenschappelijke anode/kathode, stroombegrenzing en voedingsspanning controleren.

## Aansluiten

**Aansluitwijze in de bestaande FrameWork-code:**

- De signaalverbindingen worden hieronder per onderdeel gespecificeerd waar ze uit de bestaande componentinterface blijken.
- Controleer voedingsspanning, pinvolgorde en externe stroombegrenzing bij de concrete hardware-uitvoering voordat je spanning aanlegt.

- R, G en B → drie afzonderlijke PWM-kanalen (`red`, `green`, `blue`) op de gekozen parent.
- Bepaal **gemeenschappelijke anode of kathode** van de RGB-LED of module; de constructorparameter `commonAnode` draait PWM-waarden om.
- Gebruik per kleur de passende stroombegrenzing/driver; een kale LED niet rechtstreeks aan een vermogensuitgang zonder stroombegrenzing verbinden.

**Voor de hardwaretest:**

- Gebruik de bestaande `.ino` uit **Testen** en de bestaande configuratiebestanden onder `src/Configuratie/`; een `*_template.h` wordt niet rechtstreeks ingelezen.
- **Een vaste Arduino-pintoewijzing is niet algemeen geldig:** de gekozen constructor, configuratie en werkelijke bekabeling moeten overeenkomen. Waar hierboven geen exact spanningsniveau of pinheader is bevestigd, mag dat niet uit deze documentatie worden afgeleid.


**Aanvulling aansluitpinnen en elektrische voorwaarden:**

- R/G/B→drie afzonderlijke PWM-kanalen met passende stroombegrenzing of drivers; gemeenschappelijke aansluiting naar VCC bij anodevariant of naar GND bij kathodevariant. Vereiste serieweerstand en mogelijke extra transistorsturing afhankelijke van LED en stuurkanaal.
- De concrete modulevariant bepaalt de **exacte voedingsgrenzen en fysieke pinvolgorde**. Neem die gegevens niet over van een andere module zonder de werkelijke print te controleren.

**Aansluit- en gebruiksgegevens uit de bestaande FrameWork-interface:**

- **Kleurkanalen:** PCA9685-kanalen `red`, `green`, `blue` → de drie LED-kleurpaden via **geschikte stroombegrenzing**. Sluit de gemeenschappelijke anode/kathode overeenkomstig de feitelijke LED en constructorparameter `commonAnode` aan.
- LED-stroom mag niet rechtstreeks uit een niet daarvoor gedimensioneerde controllerpin komen.
- Constructorvolgorde: `red`, `green`, `blue`; dit moeten drie verschillende kanalen 0–15 van één PCA9685 zijn. Sluit de drie kleurkanalen met geschikte stroombegrenzing aan.
- `setRGBcolor(uint16_t red, uint16_t green, uint16_t blue)` accepteert voor elke kleur 0–4095. Met constructorparameter `commonAnode=true` keert de code de PWM-waarden om (`4095 - waarde`). Controleer dus vóór het inschakelen of de LED gemeenschappelijke anode of kathode heeft.
- Bij activeren en stoppen wordt de toestand voor **uit** volgens die gekozen polariteit naar elk van de drie kanalen geschreven.

**Bestaande constructor (parameter-/pinvolgorde):**

```cpp
RGBLED(GedeeldeBusNode* parent,uint8_t red,uint8_t green,uint8_t blue,bool commonAnode=false)
```

**Configuratie voor deze hardware:**

- Gebruik bij een eigen configuratie `src/Configuratie/Sturingen/ConfigRGBLED.h` (het aanwezige `src/Configuratie/Sturingen/ConfigRGBLED_template.h` is een kopieersjabloon en wordt niet rechtstreeks ingelezen).
- Voor één aangesloten exemplaar: `STURING_RGBLED_AANTAL` op **1**, behalve wanneer de bestaande groeps-/setconfiguratie uitdrukkelijk een andere betekenis heeft.
- De bestaande configuratie bevat deze aansluitrelevante definities (gebruik de **werkelijke aangesloten** pinnen, adressen en referenties):
  - `STURING_RGBLED_AANTAL`: sjabloonwaarde `0`.

**Letterlijk constructorfragment uit de bestaande voorbeeldsketch** (alle gebruikte parent-objecten moeten zelf correct aangesloten en geïnitialiseerd zijn):

```cpp
RGBLED sturing(&extender,0,1,2);
```

**Bestaande test-/voorbeeldsketches:**

- `examples/Systeem/GedeeldeBus/Sturingen/Sturing_RGBLED_HardwareTest/Sturing_RGBLED_HardwareTest.ino`
- `examples/Systeem/GedeeldeBus/Sturingen/Sturing_RGBLED/Sturing_RGBLED.ino`


### Praktische aansluiting en gebruik

**Aansluitvoorbeeld voor een losse RGB-LED met gemeenschappelijke kathode:** verbind de gemeenschappelijke kathode met GND; verbind elke kleur-anode **via een eigen stroombegrenzende weerstand of geschikte driver** met respectievelijk de PCA9685-uitgangskanalen `red`, `green` en `blue`. Gebruik in de constructor `commonAnode=false`. Voor een gemeenschappelijke anode verbind je de gemeenschappelijke aansluiting met de passende positieve LED-rail en moeten de kleurkanalen via een geschikte **stroomafvoerende** driver worden gestuurd; gebruik dan `commonAnode=true` voor de PWM-inversie in de bestaande code.

- Een PCA9685 is een **PWM-driver, geen universele vermogensvoeding** voor RGB-strips. Controleer de maximaal toegelaten PWM-uitgangsstroom, de benodigde kleurweerstanden en het stroompad; gebruik voor hoogvermogen-LED's of strips afzonderlijke MOSFET-/transistordrivers. De drie kleurkanalen in de FrameWork moeten **verschillend zijn en tussen 0 en 15 liggen**. De PWM-frequentie is gedeeld met andere kanalen op dezelfde PCA9685.
- De fysieke leadvolgorde en de benodigde stroombegrenzing zijn **niet universeel**. Ter illustratie toont Adafruit een specifieke gemeenschappelijke-kathode-RGB-LED met afzonderlijke serieweerstanden; de fysieke pootvolgorde van dat voorbeeld geldt alleen voor het beschreven LED-type.

**Aansluitprincipe voor een losse RGB-LED:** https://learn.adafruit.com/adafruit-arduino-lesson-3-rgb-leds/overview


**Controleer vóór bedrading het type van de fysieke LED.** Bij gemeenschappelijke **kathode** gaan de drie afzonderlijk via weerstand/driver begrensde kleuringangen naar de drie gekozen PCA9685-kanalen; bij gemeenschappelijke **anode** moeten de drie kanalen via een geschikte stroomafvoerende schakeling werken. De instelling `commonAnode` in de bestaande constructor verandert uitsluitend het **softwarematige PWM-gedrag**, niet de hardware en niet de stroombegrenzing. Een losse LED en een 12/24V-RGB-strip zijn niet onderling uitwisselbaar.

**LED-aansluitprincipe (voorbeeldproduct, niet universele pinvolgorde):** https://learn.adafruit.com/adafruit-arduino-lesson-3-rgb-leds/overview

### Aansluiting van de gebruikte vierpins-RGBLED-module

De aanwezige kleine **RGBLED-print** is geen WS2812-matrix. De aansluitingen op de print zijn gemarkeerd **`−`, `R`, `G`, `B`**; de drie zichtbare weerstandscodes **`151`** duiden op **150 Ω per kleurpad**. Dit is een uitvoering met **gemeenschappelijke kathode (`−`)**.

- Print-`−` → GND van de LED-/PCA9685-opstelling; print-`R` → PCA9685-kanaal `red`; print-`G` → kanaal `green`; print-`B` → kanaal `blue` uit de bestaande constructor.
- Gebruik voor deze **gemeenschappelijke-kathode-uitvoering** `commonAnode=false`; de drie kanaalnummers moeten afzonderlijk gekozen zijn, elk **0–15**. De bestaande `RGBLED`-klasse vraagt drie PWM-kanalen van **één PCA9685**, geen seriële WS2812-data-aansluiting.
- De print bevat al drie serieweerstanden, maar dat ontslaat niet van controle op de **LED-stroom, gekozen PWM-uitgangsspanning en stroombelastbaarheid van de PCA9685**. Gebruik waar nodig een geschikte externe LED-driver in plaats van rechtstreeks te overbelasten.
- De aanduidingen op de **print** bepalen de aansluiting. Neem geen pootvolgorde over uit een foto van een losse vierpotige RGB-led of uit de WS2812-documentatie.

## Testen

### Concrete aansluiting en uitvoering van de bestaande sketches

Deze gegevens gelden voor de hieronder genoemde **bestaande sketches** en hun echte constructoren.
Ze vervangen de overige technische documentatie niet. Een succesvolle functieaanroep
of levenscyclus is geen bewijs van een fysieke beweging, licht- of contactreactie.

#### `Sturing_RGBLED.ino`

**Exact pad:** `examples/Systeem/GedeeldeBus/Sturingen/Sturing_RGBLED/Sturing_RGBLED.ino`.

- Arduino SDA -> SDA van de I2C-Extender; Arduino SCL -> SCL; GND -> GND.
- Voed de Extender-logica op het voor board en I2C-niveau geschikte VCC/VDD.
- Verifieer fysiek I2C-adres en optionele INT/RESET-pinnen met de actieve Config*.h.
- Bij 3,3V-logica op een 5V-bus: controleer een passende I2C-niveauomzetter.
- PCA9685 VCC is de logica-/I2C-voeding; V+ is een APARTE servo-/lastvoeding.
- PCA9685 OE moet een gedefinieerd niveau hebben; controleer de concrete breakout.
- Actieve Extenderconfig: src/Configuratie/Extenders/ConfigPCA9685.h
- EXTENDER_PCA9685_AANTAL >= 1, I2C_ADDRESS_EXTENDER_PCA9685_1 = fysiek adres.
- PCA9685 kanaal 0 -> R, kanaal 1 -> G, kanaal 2 -> B.
- Per afzonderlijke LED-kleur een passende SERIEWEERSTAND plaatsen.
- De bestaande RGBLED-constructor gebruikt commonAnode=false (standaard):
- gemeenschappelijke kathode -> GND; geen gemeenschappelijke anode aansluiten
- zonder de polariteit en de bestaande constructor te herzien.
- Controleer met R/G/B-commando of de overeenkomstige LED-kleur oplicht.
- Actieve configuratie: src/Configuratie/Sturingen/ConfigRGBLED.h,
- STURING_RGBLED_AANTAL >= 1.
- Deze .ino controleert alleen componentaanmaak; geen kleurtest.

**Uitvoering:** zie de start- en loopfunctie van deze bestaande sketch;
controleer of deze sketch zelf uitvoer activeert voordat je een fysiek resultaat verwacht.

#### `Sturing_RGBLED_HardwareTest.ino`

**Exact pad:** `examples/Systeem/GedeeldeBus/Sturingen/Sturing_RGBLED_HardwareTest/Sturing_RGBLED_HardwareTest.ino`.

- Arduino SDA -> SDA van de I2C-Extender; Arduino SCL -> SCL; GND -> GND.
- Voed de Extender-logica op het voor board en I2C-niveau geschikte VCC/VDD.
- Verifieer fysiek I2C-adres en optionele INT/RESET-pinnen met de actieve Config*.h.
- Bij 3,3V-logica op een 5V-bus: controleer een passende I2C-niveauomzetter.
- PCA9685 VCC is de logica-/I2C-voeding; V+ is een APARTE servo-/lastvoeding.
- PCA9685 OE moet een gedefinieerd niveau hebben; controleer de concrete breakout.
- Actieve Extenderconfig: src/Configuratie/Extenders/ConfigPCA9685.h
- EXTENDER_PCA9685_AANTAL >= 1, I2C_ADDRESS_EXTENDER_PCA9685_1 = fysiek adres.
- PCA9685 kanaal 0 -> R, kanaal 1 -> G, kanaal 2 -> B.
- Per afzonderlijke LED-kleur een passende SERIEWEERSTAND plaatsen.
- De bestaande RGBLED-constructor gebruikt commonAnode=false (standaard):
- gemeenschappelijke kathode -> GND; geen gemeenschappelijke anode aansluiten
- zonder de polariteit en de bestaande constructor te herzien.
- Controleer met R/G/B-commando of de overeenkomstige LED-kleur oplicht.
- Actieve configuratie: src/Configuratie/Sturingen/ConfigRGBLED.h,
- STURING_RGBLED_AANTAL >= 1.
- Na START R/G/B met waarden 0..4095 instellen; controleer kleur en UIT.

**Uitvoering:** gebruik de in de sketch beschreven START-/STOP-/Serial-commando's.
Beoordeel het werkelijk aangesloten apparaat, niet uitsluitend `RESULTAAT: OK`.

**Officiële hardwarehandleiding (controleer de eigen modulevariant):**

- https://learn.adafruit.com/16-channel-pwm-servo-driver/hooking-it-up



De huidige implementatie bevat een voorbeeld voor deze Sturing in `examples/` en het script `extras/TestLibrarySturingen-v2.0.0.cmd`. 

Configuratie: `STURING_RGBLED_AANTAL`: 0 = niet opgenomen; groter dan 0 = opgenomen; het aantal objecten wordt niet begrensd.

## Uitval van I²C en veilige eindtoestand (RC1)

Deze Sturing gebruikt een PCA9685-parent. `StopSturing()` kan de output niet betrouwbaar wijzigen zolang de I²C-bus onbereikbaar is. Een softwarematig `RESULTAAT: OK` of een geslaagde aanroep bewijst niet dat de aangesloten actuator/uitgang werkelijk gestopt is.

- Optionele OE-bedrading van de PCA9685 geeft onafhankelijk van I²C een gezamenlijke uitgangsvrijgave zolang het board en de OE-besturing functioneren. Zie `docs/Systeem/Extender/PCA9685.md` voor de noodzakelijke externe opstart-/resetbeveiliging en de afhankelijkheid van MODE2.OUTNE.
- De **voeding van de aangesloten actuator** blijft een afzonderlijke veiligheidsvoorziening. Gebruik waar de toepassing dat vereist een apart hardwarematig uitschakelpad en controleer de eindtoestand met een fysieke meting.
- RC1-hardwaretest: tijdens een actieve beweging/uitgang de I²C-bus tijdelijk onderbreken, veilig uitschakelen, de uitgang en voeding afzonderlijk meten, vervolgens herstellen. Herhaal met reset van de MCU. **Nog niet fysiek bewezen.**

### Gedeeltelijke kleurupdate
- `setRGBcolor()` schakelt bij een gemelde fout op R/G/B nu alle drie de kanalen via het bestaande `StopSturing()` uit (best effort), in plaats van een gedeeltelijke kleurupdate ongemerkt te laten staan.
- **Proef:** kleurinstelling in normale toestand; ontkoppel I²C gecontroleerd tijdens de tweede/derde kanaalupdate; controleer de retourwaarde en fysiek de drie PWM-uitgangen. **Grenzen:** bij volledig busverlies is de stop niet gegarandeerd; gebruik hardwarematig OE/voedingsonderbreking als deze eis geldt.
- **Status:** code aangepast, compileer- en hardwaretest open.
