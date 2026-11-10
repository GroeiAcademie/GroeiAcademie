# HCSR04

## Sensor en gedeelde implementatie

Fysiek HC-SR04-type. Componentcode `HCSR04` (0x90), klasse `HSR0x` met TRIG en ECHO.

Deze sensor deelt een C++-klasse met andere uitvoeringen. De fysieke aansluitgegevens gelden alleen voor `HCSR04` waar dat uitdrukkelijk vermeld is.

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
  - `SENSOR_HCSR04_AANTAL`: uitgecommentarieerd sjabloonvoorstel `1`; zonder actieve override geldt de fallback `0` uit `SystemConfig.h`.

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

