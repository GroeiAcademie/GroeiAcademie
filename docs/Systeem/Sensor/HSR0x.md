# HSR0x

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
- De aanwezigheid van deze rubriek is **geen bewijs van een geslaagde hardwaretest**.

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_HCSR04/`, `examples/Systeem/GedeeldeBus/Sensoren/Sensor_HCSR04P/`, `examples/Systeem/GedeeldeBus/Sensoren/Sensor_HYSRF05/`.
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.
Dit document bevestigt geen geslaagde hardwaretest.
