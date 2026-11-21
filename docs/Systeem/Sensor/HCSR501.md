# HCSR501

## Implementatie

- Broncode: `src/Systeem/Sensor/HCSR501.h` (geen `.cpp`).
- C++-component: `HCSR501`, afgeleid van `SensorDigitaal`.
- Parent: `GedeeldeBusNode*` in de constructor.
- Componentcode: `HCSR501` (0x8A).
- Configuratie: `src/Configuratie/Sensoren/ConfigHCSR501_template.h`; de optionele persoonlijke versie heet telkens zonder `_template`.
- Data-kanalen: 1 (via SensorDigitaal).

## Uit de broncode

HC-SR501 heeft geen ID of busprotocol. Dit controleert alleen de geconfigureerde ingang.

## Aansluiten

**Aansluitwijze in de bestaande FrameWork-code:**

- **Digitale verbinding:** module-OUT naar de gekozen digitale ingang (native Arduino of de door de constructor ondersteunde GPIO-Extender).
- **Voeding/GND:** volgens de concrete module; controleer de actieve uitgangspolariteit en spanningscompatibiliteit.

- Digitale OUT → gekozen digitale ingang (native of ondersteunde GPIO-Extender).
- Voed volgens de specificaties van de gebruikte PIR-module; beweging of stabilisatietijd niet verwarren met elektrische fout.

**Voor de hardwaretest:**

- Gebruik de bestaande `.ino` uit **Testen** en de bestaande configuratiebestanden onder `src/Configuratie/`; een `*_template.h` wordt niet rechtstreeks ingelezen.
- **Een vaste Arduino-pintoewijzing is niet algemeen geldig:** de gekozen constructor, configuratie en werkelijke bekabeling moeten overeenkomen. Waar hierboven geen exact spanningsniveau of pinheader is bevestigd, mag dat niet uit deze documentatie worden afgeleid.


**Aanvulling aansluitpinnen en elektrische voorwaarden:**

- OUT→digitale ingang, GND→GND, VCC→de ingangsspanning volgens de gebruikte HC-SR501-print. Stel gevoeligheid/tijdsvertraging op de module in; controleer de pinvolgorde op de eigen print vóór aansluiten.
- De concrete modulevariant bepaalt de **exacte voedingsgrenzen en fysieke pinvolgorde**. Neem die gegevens niet over van een andere module zonder de werkelijke print te controleren.

**Aansluit- en gebruiksgegevens uit de bestaande FrameWork-interface:**

- **Digitale bekabeling voor HCSR501:** `OUT` van de module → de in de constructor toegewezen native ingang `np_OUT`; `GND` naar gemeenschappelijke massa, voeding volgens de fysieke module. De code verwacht een digitale HIGH/LOW-toestand, geen analoge ADC-waarde.
- Controleer vóór de verbinding het **OUT-logicaniveau** en de ingangstolerantie van het Arduino-board; zo nodig een passende levelshifter of pull-up gebruiken.
- De digitale PIR-uitgang gaat naar de gekozen `np_OUT`-ingang; voeding en eventuele instelpotmeters voor gevoeligheid en nalooptijd zijn moduleafhankelijk.
- De klasse erft de digitale meetroute van `SensorDigitaal`; bij testen moet de waargenomen uitgangspolariteit worden vergeleken met de gekozen module-instelling.

**Bestaande constructor (parameter-/pinvolgorde):**

```cpp
HCSR501(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin np_OUT)
```

**Configuratie voor deze hardware:**

- Gebruik bij een eigen configuratie `src/Configuratie/Sensoren/ConfigHCSR501.h` (het aanwezige `src/Configuratie/Sensoren/ConfigHCSR501_template.h` is een kopieersjabloon en wordt niet rechtstreeks ingelezen).
- Voor één aangesloten exemplaar: `SENSOR_HCSR501_AANTAL` op **1**, behalve wanneer de bestaande groeps-/setconfiguratie uitdrukkelijk een andere betekenis heeft.
- De bestaande configuratie bevat deze aansluitrelevante definities (gebruik de **werkelijke aangesloten** pinnen, adressen en referenties):
  - `SENSOR_HCSR501_AANTAL`: sjabloonwaarde `1`.

**Letterlijk constructorfragment uit de bestaande voorbeeldsketch** (alle gebruikte parent-objecten moeten zelf correct aangesloten en geïnitialiseerd zijn):

```cpp
GedeeldeBusNewComponent<HCSR501>(&Native,GedeeldeBusComponent::HCSR501, HardwareResourcePin::NP_D2);
```

**Bestaande test-/voorbeeldsketches:**

- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_HCSR501/Sensor_HCSR501.ino`

**Elektrische aansluiting en randvoorwaarden volgens Adafruit (PIR-breakout-handleiding):**

- Voor de **door Adafruit beschreven PIR-module**: de drie aansluitingen zijn voeding, massa en digitaal uitgangssignaal. `OUT` gaat naar de ingang van de bestaande `HCSR501`-FrameWorkconstructor; `GND` gaat naar de gemeenschappelijke massa. **De pinvolgorde is moduleafhankelijk** en dient letterlijk met de connectoropdruk vergeleken te worden.
- Adafruit vermeldt dat PIR-printen vaak op 3–5 V werken maar bij andere varianten ook hogere voedingsspanningen kunnen vereisen. Dit is **geen bevestiging van een specifiek HCSR501-breakoutmodel**: sluit uitsluitend aan volgens de werkelijk aanwezige pinopdruk.

**Technische handleiding:** https://learn.adafruit.com/pir-passive-infrared-proximity-motion-sensor/connecting-to-a-pir

### Herkennen van de gebruikte PIR-print

De aanwezige PIR-uitvoering heeft een witte **Fresnel-lens**, een controllerprint met **jumper** en afzonderlijke **afregelcomponenten**. De PIR-lens is geen connector: de digitale signaal-, voedings- en massapinnen bevinden zich aan de print.

- Sluit uitsluitend de op de concrete print gemarkeerde **`OUT`** aan op de native digitale ingang `np_OUT` uit de bestaande `HCSR501`-constructor; de print moet passende voedingsspanning en gemeenschappelijke GND krijgen.
- Jumper en afregeling kunnen detectiemodus en vertraging beïnvloeden; lees de werkelijke printinstellingen af voordat je het waargenomen digitale uitgangsgedrag aan de FrameWork toeschrijft.
- De vorm met witte lens alleen is **geen bewijs** voor een identieke pinvolgorde of elektrisch identieke HC-SR501-versie.

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_HCSR501/`.
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.

### Aanvullende Sensor × Extender-testvoorbeelden

De onderstaande `.ino`-bestanden behoren tot de huidige v2.0.0-voorbeelden. Hun aanwezigheid is gecontroleerd; compilatie en fysieke hardwarevalidatie zijn hier nog **niet** aangetoond.

- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_HCSR501_MCP23017/Sensor_HCSR501_MCP23017.ino`
- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_HCSR501_MCP23S17/Sensor_HCSR501_MCP23S17.ino`
- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_HCSR501_PCF8574/Sensor_HCSR501_PCF8574.ino`
- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_HCSR501_PCF8575/Sensor_HCSR501_PCF8575.ino`

### Functionele Sensor × Extender-voorbeelden

De onderstaande voorbeelden bevatten **meetaanroepen na de activering**. Een getoonde waarde is geen bewijs van meetnauwkeurigheid of van een geslaagde hardwaretest.

- **Fysieke test:** Beweeg voor de PIR; controleer 0/1 en terugkeer naar rust.
- **Huidige reikwijdte:** de `.ino` gebruikt uitsluitend reeds bestaande FrameWork-functies; niet-beschikbare meetkanalen, kalibraties of externe functies zijn niet toegevoegd.

**Voorbeelden:**

- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_HCSR501_MCP23017/Sensor_HCSR501_MCP23017.ino`
- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_HCSR501_MCP23S17/Sensor_HCSR501_MCP23S17.ino`
- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_HCSR501_PCF8574/Sensor_HCSR501_PCF8574.ino`
- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_HCSR501_PCF8575/Sensor_HCSR501_PCF8575.ino`

**Validatie:** Arduino-compilatie, fysieke test, kalibratie en meetkwaliteitsvalidatie zijn hier nog niet aangetoond.

## Native meetvoorbeeld

`examples/Systeem/GedeeldeBus/Sensoren/Sensor_HCSR501/Sensor_HCSR501.ino` voert na de lifecycle daadwerkelijk uitlezingen uit via de bestaande Sensor-API.
Gemeten of weergegeven in dit voorbeeld: **Bewegingsstatus (0/1)**.
De seriële uitvoer wordt beperkt; de Sensor-uitlezing blijft in `loop()` plaatsvinden.
Dit bevestigt bij geslaagde hardwaretests de ruwe of rechtstreeks beschikbare meetwaarde, niet automatisch alle fysische kanalen, kalibratie of nauwkeurigheid.
Compilatie en fysieke meting zijn nog niet bevestigd.

## Hulppinnen en routes via Extenders

- OUT via een GPIO-Extender (PCF8574, PCF8575, MCP23017, MCP23S17, ADS1158, SC16IS752, MAX14830) of via een ADC-kanaal met een drempel (ADS1115, ADS7828, ADS7953, CD74HC4067): zie SensorDigitaal.
- Instellingen (SystemConfig.h, ConfigHCSR501.h): `SENSOR_HCSR501_ADC_MEASUREMENT_RANGE_MILLIVOLT` (2500), `SENSOR_HCSR501_ANALOG_THRESHOLD_FOR_HIGH_MILLIVOLT` (1500).
- Het standaardbereik van 2500 mV wordt aanvaard door ADS1115, ADS1158, ADS7828 (interne referentie) en ADS7953; via een CD74HC4067 op Native moet `EXTENDER_CD74HC4067_NATIVE_REFERENTIE_MILLIVOLT` ingesteld zijn.
- Het werkelijke meetbereik hangt van de ADC af: ADS1115 kiest 4096 mV, ADS1158 meet tot ongeveer 5333 mV (bij 5000 mV referentie), ADS7828 en ADS7953 meten tot 2500 mV; de drempel wordt met dat werkelijke bereik berekend.
- Boven het werkelijke bereik geeft de ADC zijn maximale waarde. De toegelaten ingangsspanning staat in de datasheet van de ADC; een module op 5 V aan een ADC op 3,3 V is niet toegelaten.
