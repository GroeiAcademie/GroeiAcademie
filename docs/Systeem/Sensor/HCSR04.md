# HCSR04

## Sensor en gedeelde implementatie

Fysiek HC-SR04-type. Componentcode `HCSR04` (0x88), klasse `HSR0x` met TRIG en ECHO.

Deze sensor deelt een C++-klasse met andere uitvoeringen. De fysieke aansluitgegevens gelden alleen voor `HCSR04` waar dat uitdrukkelijk vermeld is.

---

## Gedeelde C++-klasse `src/Systeem/Sensor/HSR0x.h`

### HSR0x

## Implementatie

- Broncode: `src/Systeem/Sensor/HSR0x.h` en `HSR0x.cpp`.
- C++-component: `HSR0x`, afgeleid van `Sensor`.
- Parent: `GedeeldeBusNode*` in de constructor.
- Componentcode: `HCSR04` (0x88), `HCSR04P` (0x89), `HYSRF05` (0x8C).
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

- **TRIG** van de HC-SR04 → de native output `np_TRIG`; **ECHO** → de native input `np_ECHO`; gemeenschappelijke GND en de juiste modulevoeding. ECHO moet binnen het toegelaten logicaniveau van de controller blijven.
- `HSR0x` gebruikt twee native digitale pinnen: `np_TRIG` als pulsuitgang en `np_ECHO` als pulsingang. Een interruptgeschikte ECHO-pin maakt de interrupt-route mogelijk; de bestaande code heeft ook een `pulseIn()`-pad.
- Controleer expliciet het ECHO-spanningsniveau. Een 5V-ECHO mag niet rechtstreeks op een uitsluitend 3,3V-tolerante ingang worden aangesloten.

**Bestaande constructor (parameter-/pinvolgorde):**

```cpp
HSR0x(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin np_TRIG, HardwareResourcePin np_ECHO)
```

**Configuratie voor deze hardware:**

- Gebruik bij een eigen configuratie `src/Configuratie/Sensoren/ConfigHCSR04.h` (het aanwezige `src/Configuratie/Sensoren/ConfigHCSR04_template.h` is een kopieersjabloon en wordt niet rechtstreeks ingelezen).
- Voor één aangesloten exemplaar: `SENSOR_HCSR04_AANTAL` op **1**, behalve wanneer de bestaande groeps-/setconfiguratie uitdrukkelijk een andere betekenis heeft.
- De bestaande configuratie bevat deze aansluitrelevante definities (gebruik de **werkelijke aangesloten** pinnen, adressen en referenties):
  - `SENSOR_HCSR04_AANTAL`: sjabloonwaarde `1`.

**Letterlijk constructorfragment uit de bestaande voorbeeldsketch** (alle gebruikte parent-objecten moeten zelf correct aangesloten en geïnitialiseerd zijn):

```cpp
GedeeldeBusNewComponent<HSR0x>(&Native, GedeeldeBusComponent::HCSR04, HardwareResourcePin::NP_D2, HardwareResourcePin::NP_D3);
```

**Bestaande test-/voorbeeldsketches:**

- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_HCSR04/Sensor_HCSR04.ino`

**Elektrische aansluiting en randvoorwaarden volgens ELECFREAKS (HC-SR04-uitvoering):**

- Bij de door ELECFREAKS gedocumenteerde **HC-SR04** zijn de connectorcontacten in volgorde `VCC`, `TRIG`, `ECHO`, `GND`; `VCC` = **5 V**, `GND` = massa, `TRIG` krijgt een HIGH-puls van minstens **10 µs**, en `ECHO` is de meetpulsuitgang. `TRIG` gaat naar de door de bestaande FrameWorkconstructor gekozen triggeruitgang en `ECHO` naar de gekozen echo-ingang.
- De beschreven module gebruikt TTL-signalen met een **5V-ECHO-signaal**: sluit ECHO op een 3,3V-host niet zonder passende niveauaanpassing aan. ELECFREAKS beveelt een meetinterval van **meer dan 60 ms** aan om onderlinge echo-interferentie te beperken. Dit geldt voor deze HC-SR04-uitvoering; niet automatisch voor HC-SR04P of een I²C-Qwiic-print.

**Technische handleiding:** https://www.elecfreaks.com/download/HC-SR04.pdf

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_HCSR04/`, `examples/Systeem/GedeeldeBus/Sensoren/Sensor_HCSR04P/`, `examples/Systeem/GedeeldeBus/Sensoren/Sensor_HYSRF05/`.
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.

## Native meetvoorbeeld

`examples/Systeem/GedeeldeBus/Sensoren/Sensor_HCSR04/Sensor_HCSR04.ino` voert na de lifecycle daadwerkelijk uitlezingen uit via de bestaande Sensor-API.
Gemeten of weergegeven in dit voorbeeld: **Afstand (mm)**.
De seriële uitvoer wordt beperkt; de Sensor-uitlezing blijft in `loop()` plaatsvinden.
Dit bevestigt bij geslaagde hardwaretests de ruwe of rechtstreeks beschikbare meetwaarde, niet automatisch alle fysische kanalen, kalibratie of nauwkeurigheid.
Compilatie en fysieke meting zijn nog niet bevestigd.

## Hulppinnen en routes via Extenders

- Via een CD74HC4067: TRIG en ECHO zijn twee kanalen; `LeesHoofdwaarde()` kiest TRIG, geeft de puls op Z, kiest ECHO en meet met `pulseIn` op Z. Pull-down op TRIG aanbevolen.
- Via MCP23017 of MCP23S17: TRIG via `WriteExtenderPin` (10 µs hoog), ECHO via interrupt-on-change van de MCP en de INTA-uitgang op een native interruptpin. MCP23S17: constructor met `np_INTA`.
- Korte afstanden (te meten grens): na de eerste ECHO-flank moet de lus INTA vrijgeven met een busopdracht. Een ECHO die korter is dan die busopdracht geeft geen tweede interrupt, dan is er geen meting (false, geen foute waarde).
  De werkelijke ondergrens hangt af van de bussnelheid en de Extender; MCP23017 (I2C) en MCP23S17 (SPI) moeten apart gemeten worden.
- Minstens 60 ms tussen twee metingen (fabrikant).
- Leden: `echoStartEdgeSeenViaINTA`.

## ECHO via een MCP23017 of MCP23S17

- ECHO mag op elke pin van de MCP, ook op GPB0..GPB7: de HSR0x zet IOCON.MIRROR aan, zodat de interrupt ook op INTA komt.
