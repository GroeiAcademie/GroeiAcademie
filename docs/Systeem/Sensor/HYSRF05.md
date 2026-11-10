# HYSRF05

## Sensor en gedeelde implementatie

Fysiek HY-SRF05-type. Componentcode `HYSRF05` (0x92), klasse `HSR0x`. Mogelijke MODE-variant fysiek controleren.

Deze sensor deelt een C++-klasse met andere uitvoeringen. De fysieke aansluitgegevens gelden alleen voor `HYSRF05` waar dat uitdrukkelijk vermeld is.

---

## Gedeelde C++-klasse `src/Systeem/Sensor/HSR0x.h`

### HSR0x

## Implementatie

- Broncode: `src/Systeem/Sensor/HSR0x.h` en `HSR0x.cpp`.
- C++-component: `HSR0x`, afgeleid van `Sensor`.
- Parent: `GedeeldeBusNode*` in de constructor.
- Componentcode: `HCSR04` (0x90), `HCSR04P` (0x91), `HYSRF05` (0x92).
- Configuratie: `src/Configuratie/Sensoren/ConfigHCSR04_template.h`, `src/Configuratie/Sensoren/ConfigHCSR04P_template.h`, `src/Configuratie/Sensoren/ConfigHYSRF05_template.h`; de optionele persoonlijke versie heet telkens zonder `_template`.
- Data-kanalen: 1.

## Uit de broncode

HC-SR04 en HC-SR04P: zelfde TRIG/ECHO-klasse. De componentcode identificeert de fysieke variant zonder een tweede klasse.

## Aansluiten

**Aansluitwijze in de bestaande FrameWork-code:**

- De signaalverbindingen worden hieronder per onderdeel gespecificeerd waar ze uit de bestaande componentinterface blijken.
- Controleer voedingsspanning, pinvolgorde en externe stroombegrenzing bij de concrete hardware-uitvoering voordat je spanning aanlegt.

- TRIG → de native Arduino-uitgang `np_TRIG`; ECHO → de native Arduino-ingang `np_ECHO`.
- Kies een interruptgeschikte ECHO-pin als niet-blokkerende pulsmeting nodig is; anders gebruikt de code `pulseIn()`.
- **Controleer ECHO-spanningsniveau** van de concrete HC-SR04/HC-SR04P/HY-SRF05-module tegen de maximale ingangsspanning van het board; waar nodig een niveauaanpassing gebruiken.

**Voor de hardwaretest:**

- Gebruik de bestaande `.ino` uit **Testen** en de bestaande configuratiebestanden onder `src/Configuratie/`; een `*_template.h` wordt niet rechtstreeks ingelezen.
- **Een vaste Arduino-pintoewijzing is niet algemeen geldig:** de gekozen constructor, configuratie en werkelijke bekabeling moeten overeenkomen. Waar hierboven geen exact spanningsniveau of pinheader is bevestigd, mag dat niet uit deze documentatie worden afgeleid.


**Aanvulling aansluitpinnen en elektrische voorwaarden:**

- Bij HC-SR04/HC-SR04P/HY-SRF05: TRIG→digitale uitgang, ECHO→digitale ingang, GND→GND en VCC→modulevoeding. **Let op:** ECHO kan bij 5V-gevoede varianten 5V uitsturen; een 3,3V-controller vereist dan een geschikte spanningsaanpassing. De HC-SR04P-variant en SRF05-MODE moeten afzonderlijk aan hun modulehandleiding worden getoetst.
- De concrete modulevariant bepaalt de **exacte voedingsgrenzen en fysieke pinvolgorde**. Neem die gegevens niet over van een andere module zonder de werkelijke print te controleren.

**Aansluit- en gebruiksgegevens uit de bestaande FrameWork-interface:**

- Gebruik in de huidige `HSR0x`-klasse twee native lijnen: `TRIG` → `np_TRIG` en `ECHO` → `np_ECHO`. Stem de aansluitmodus van de fysieke HY-SRF05-module af op deze tweepins-aansturing.
- De gegroepeerde `HSR0x`-driver gebruikt de eigen fysieke HYSRF05-componentcode en de twee constructorpinnen `np_TRIG` en `np_ECHO`.
- Controleer de daadwerkelijke modus/pinbezetting van de gebruikte HY-SRF05-print en het ECHO-niveau; een andere modus kan een andere aansluiting vereisen.

**Bestaande constructor (parameter-/pinvolgorde):**

```cpp
HSR0x(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin np_TRIG, HardwareResourcePin np_ECHO)
```

**Configuratie voor deze hardware:**

- Gebruik bij een eigen configuratie `src/Configuratie/Sensoren/ConfigHYSRF05.h` (het aanwezige `src/Configuratie/Sensoren/ConfigHYSRF05_template.h` is een kopieersjabloon en wordt niet rechtstreeks ingelezen).
- Voor één aangesloten exemplaar: `SENSOR_HYSRF05_AANTAL` op **1**, behalve wanneer de bestaande groeps-/setconfiguratie uitdrukkelijk een andere betekenis heeft.
- De bestaande configuratie bevat deze aansluitrelevante definities (gebruik de **werkelijke aangesloten** pinnen, adressen en referenties):
  - `SENSOR_HYSRF05_AANTAL`: uitgecommentarieerd sjabloonvoorstel `1`; zonder actieve override geldt de fallback `0` uit `SystemConfig.h`.

**Letterlijk constructorfragment uit de bestaande voorbeeldsketch** (alle gebruikte parent-objecten moeten zelf correct aangesloten en geïnitialiseerd zijn):

```cpp
GedeeldeBusNewComponent<HSR0x>(&Native, GedeeldeBusComponent::HYSRF05, HardwareResourcePin::NP_D2, HardwareResourcePin::NP_D3);
```

**Bestaande test-/voorbeeldsketches:**

- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_HYSRF05/Sensor_HYSRF05.ino`


### Praktische aansluiting en gebruik

**Aansluiting voor een SRF05 in afzonderlijke TRIG/ECHO-modus:** de oorspronkelijke SRF05-maker Robot Electronics documenteert `VCC`, `GND`, `TRIG`, `ECHO` en een aparte `MODE`-aansluiting. Laat bij deze SRF05-uitvoering `MODE` **niet aangesloten** om afzonderlijke TRIG en ECHO te gebruiken. Verbind TRIG met `np_TRIG`, ECHO met `np_ECHO`, plus voeding en massa volgens de werkelijke module. De bestaande `HSR0x`-code accepteert **geen** TRIG=ECHO, zodat de door die maker beschreven eenpinsmodus (MODE naar GND) niet met deze FrameWorkdriver overeenkomt.

- Robot Electronics beschrijft een triggerpuls van circa **10 µs** en minstens **50 ms** tussen twee triggers. Deze documentatie betreft de echte SRF05 van die fabrikant; een als **HY-SRF05** verkochte module kan een andere pinbezetting hebben. De werkelijke voedingsspanning en ECHO-logicaniveaus moeten op het fysieke HY-SRF05-model worden gecontroleerd, zeker bij 3,3V-hosts.

**Technische handleiding SRF05 (alleen bij overeenkomstige module):** https://www.robot-electronics.co.uk/htm/srf05tech.htm


**Concrete aansluiting voor de als HY-SRF05 aangeboden OTRONIC-module:** de vijf op de print benoemde aansluitingen zijn `VCC` → **5 V**, `TRIG` → de native digitale uitgang `np_TRIG`, `ECHO` → de native digitale ingang `np_ECHO`, `OUT` → **niet aansluiten**, `GND` → gemeenschappelijke massa. Deze `OUT` is bij de genoemde HY-SRF05-uitvoering **niet** hetzelfde als de `MODE`-aansluiting van de oorspronkelijke Robot Electronics SRF05. Verbind deze twee verschillende module-uitvoeringen dus niet volgens dezelfde vijfde-pin-instructie. De `ECHO`-lijn van de 5V-uitvoering mag niet zonder geschikte spanningsaanpassing op een uitsluitend 3,3V-tolerante ingang worden gezet. De bij deze uitvoering benodigde TRIG-puls is minimaal 10 µs.

De bestaande `HSR0x.cpp` geeft **na 30.000 µs zonder voltooide ECHO-puls** geen geldige meting terug (ook in de `pulseIn()`-terugval). OTRONIC beschrijft bij zijn HY-SRF05 daarentegen een mogelijke ECHO-timeout van circa **200 ms** wanneer geen echo terugkomt. Een mislukte afstandsmeting zonder reflectie is dus niet hetzelfde als een bewezen bedradingfout; verleng niet stilzwijgend de FrameWork-timing.

**Documentatie van de benoemde HY-SRF05-uitvoering:** https://www.otronic.nl/nl/hy-srf05-ultrasone-afstandssensormodule-5-pins

### Aansluiting van de gebruikte vijfpins-SRF05-print

De aanwezige vijfpins-ultrasoonprint heeft de vijf aansluitlabels **`VCC`, `TRIG`, `ECHO`, `OUT`, `GND`**. Dit is niet hetzelfde als een RCWL-9610A-print met vier contacten.

- Verbind `TRIG` → de `np_TRIG`-uitgang en `ECHO` → de `np_ECHO`-ingang uit de bestaande `HSR0x`-constructor; `GND` → gemeenschappelijke GND en `VCC` → de bij **deze concrete print** toegelaten voeding.
- Laat het extra contact **`OUT` ongemoeid** voor de gewone gescheiden TRIG/ECHO-metingen; neem niet aan dat `OUT` dezelfde functie heeft als de `MODE`-pin in een ander SRF05-schema.
- Controleer of `ECHO` voor de gekozen controller een passende spanningsniveauschakeling nodig heeft. De bestaande driver vraagt **twee afzonderlijke** native digitale pinnen.

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_HCSR04/`, `examples/Systeem/GedeeldeBus/Sensoren/Sensor_HCSR04P/`, `examples/Systeem/GedeeldeBus/Sensoren/Sensor_HYSRF05/`.
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.

