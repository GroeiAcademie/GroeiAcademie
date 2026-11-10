# HCSR501

## Implementatie

- Broncode: `src/Systeem/Sensor/HCSR501.h` (geen `.cpp`).
- C++-component: `HCSR501`, afgeleid van `SensorDigitaal`.
- Parent: `GedeeldeBusNode*` in de constructor.
- Componentcode: `HCSR501` (0x81).
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
  - `SENSOR_HCSR501_AANTAL`: uitgecommentarieerd sjabloonvoorstel `1`; zonder actieve override geldt de fallback `0` uit `SystemConfig.h`.

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

