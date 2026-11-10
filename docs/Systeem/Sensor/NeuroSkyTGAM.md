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


**Aanvulling aansluitpinnen en elektrische voorwaarden:**

- TGAM TX→UART RX; GND→systeemreferentie uitsluitend volgens de officiële TGAM-modulehandleiding. EEG-elektrode-, referentie- en voeding/aardingsverbindingen mogen niet uit een willekeurige UART-aansluittabel worden afgeleid; modulevariant en veilige voeding vereist.
- De concrete modulevariant bepaalt de **exacte voedingsgrenzen en fysieke pinvolgorde**. Neem die gegevens niet over van een andere module zonder de werkelijke print te controleren.

**Aansluit- en gebruiksgegevens uit de bestaande FrameWork-interface:**

- **UART-bekabeling voor NeuroSkyTGAM:** `TX` van de sensor → `RX` van de gekozen host-/Extender-UART, en `RX` van de sensor → `TX` van die UART indien de gebruikte interface beide richtingen nodig heeft. `GND` moet gemeenschappelijk zijn.
- Controleer de UART-logicaniveaus én de vereiste baudrate van de fysieke module; een UART-pin is geen RS-232-niveau. Kies exact de door de constructor ondersteunde native seriële poort of UART-Extender-kanaal.
- De driver gebruikt een seriële `Stream` en bevat aparte `LeesMeditatie(float&)` en `LeesEEGBanden(uint32_t banden[8])` naast `LeesHoofdwaarde(float&)`.
- De sensor-TX/RX-verbinding moet naar een passende UART-poort met correcte spanningsniveaus; hou lichaamssensorverbindingen galvanisch veilig ten opzichte van USB/netvoeding.

**Bestaande constructor (parameter-/pinvolgorde):**

```cpp
NeuroSkyTGAM(GedeeldeBusNode* parent, GedeeldeBusComponent component, Stream& serielePoort, HardwareResourcePin np_TX, HardwareResourcePin np_RX)
```

```cpp
NeuroSkyTGAM(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t channel)
```

**Configuratie voor deze hardware:**

- Gebruik bij een eigen configuratie `src/Configuratie/Sensoren/ConfigNeuroSkyTGAM.h` (het aanwezige `src/Configuratie/Sensoren/ConfigNeuroSkyTGAM_template.h` is een kopieersjabloon en wordt niet rechtstreeks ingelezen).
- Voor één aangesloten exemplaar: `SENSOR_NEUROSKYTGAM_AANTAL` op **1**, behalve wanneer de bestaande groeps-/setconfiguratie uitdrukkelijk een andere betekenis heeft.
- De bestaande configuratie bevat deze aansluitrelevante definities (gebruik de **werkelijke aangesloten** pinnen, adressen en referenties):
  - `SENSOR_NEUROSKYTGAM_AANTAL`: uitgecommentarieerd sjabloonvoorstel `1`; zonder actieve override geldt de fallback `0` uit `SystemConfig.h`.

**Letterlijk constructorfragment uit de bestaande voorbeeldsketch** (alle gebruikte parent-objecten moeten zelf correct aangesloten en geïnitialiseerd zijn):

```cpp
GedeeldeBusNewComponent<NeuroSkyTGAM>(&Native, GedeeldeBusComponent::NeuroSkyTGAM, Serial1, HardwareResourcePin::NP_D1, HardwareResourcePin::NP_D0);
```

**Bestaande test-/voorbeeldsketches:**

- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_NeuroSkyTGAM/Sensor_NeuroSkyTGAM.ino`

**Elektrische aansluiting en randvoorwaarden volgens NeuroSky:**

- De **NeuroSky TGAM** is een EEG-sensormodule met seriële gegevensuitvoer. De bestaande FrameWorkconstructor vraagt een `Stream` en heeft afzonderlijke gereserveerde TX/RX-pinnen, of vraagt een kanaal van een bestaande UART-Extender. **De TGAM-seriële uitgang hoort op de ontvangende UART-ingang van de host**, met gemeenschappelijke signaalmassa.
- NeuroSky beschrijft verwerking van EEG-banden, signaalkwaliteit, aandacht, meditatie en knippersignalen. De fabrikantproductpagina specificeert hier **geen universele breakoutconnector, vaste voedingsspanning of pinvolgorde voor elk TGAM-board**. Gebruik de concrete fabrikantmoduleaansluiting, vooral bij elektrodecontact met het lichaam uitsluitend galvanisch veilig/batterijgevoed.

**Technische handleiding:** https://store.neurosky.com/products/eeg-tgam

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_NeuroSkyTGAM/`.
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.


### Checksumcontrole en herstel van UART-pakketten

- Injecteer een afgebroken UART-pakket, verkeerd checksumbyte en een nieuwe geldige frame. Controleer dat ongeldige frames niet als nieuwe meting verschijnen en dat het eerstvolgende geldige frame correct wordt verwerkt.
- **Status:** fysieke proef vereist; nog geen hardwarebewijs.
