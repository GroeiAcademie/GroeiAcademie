# NeuroSkyTGAM

## Implementatie

- Broncode: `src/Systeem/Sensor/NeuroSkyTGAM.h` en `NeuroSkyTGAM.cpp`.
- C++-component: `NeuroSkyTGAM`, afgeleid van `Sensor`.
- Parent: `GedeeldeBusNode*` in de constructor.
- Componentcode: `NeuroSkyTGAM` (0xA7).
- Configuratie: `src/Configuratie/Sensoren/ConfigNeuroSkyTGAM_template.h`; de optionele persoonlijke versie heet telkens zonder `_template`.
- Data-kanalen: 1.

## Aansluiten

**Aansluitwijze in de bestaande FrameWork-code:**

- **Seriële verbinding:** UART-**TX van de sensor → RX** van de gekozen seriële poort; eventueel sensor-RX ← TX van dezelfde poort.
- **Voeding:** aansluitspanning en UART-logicaniveau volgens de exacte module; gebruik indien nodig een niveauaanpassing.

- UART-TX van TGAM → RX van de geselecteerde seriële interface; alleen TX/RX verbinden die de gebruikte module daadwerkelijk aanbiedt.
- De TGAM gebruikt lage logicaniveaus; nooit zonder gecontroleerde niveauaanpassing op een 5V-uitgang aansluiten.
- Bij lichaamscontact uitsluitend de door de fabrikant voorgeschreven, elektrisch veilige/isolatiegeschikte opstelling gebruiken.

**Voor de hardwaretest:**

- Gebruik de bestaande `.ino` uit **Testen** en de bestaande configuratiebestanden onder `src/Configuratie/`; een `*_template.h` wordt niet rechtstreeks ingelezen.
- **Een vaste Arduino-pintoewijzing is niet algemeen geldig:** de gekozen constructor, configuratie en werkelijke bekabeling moeten overeenkomen. Waar hierboven geen exact spanningsniveau of pinheader is bevestigd, mag dat niet uit deze documentatie worden afgeleid.
- De aanwezigheid van deze rubriek is **geen bewijs van een geslaagde hardwaretest**.

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_NeuroSkyTGAM/`.
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.
Dit document bevestigt geen geslaagde hardwaretest.
