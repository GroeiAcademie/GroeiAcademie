# MR60BHA2

## Implementatie

- Broncode: `src/Systeem/Sensor/MR60BHA2.h` en `MR60BHA2.cpp`.
- C++-component: `MR60BHA2`, afgeleid van `Sensor`.
- Parent: `GedeeldeBusNode*` in de constructor.
- Componentcode: `MR60BHA2` (0xA6).
- Configuratie: `src/Configuratie/Sensoren/ConfigMR60BHA2_template.h`; de optionele persoonlijke versie heet telkens zonder `_template`.
- Data-kanalen: 1.

## Uit de broncode

Seeed MR60BHA2: UART-frame 0x01, ID(2), lengte(2), type(2), checksum, payload, checksum. De officiele ESP32-only library is niet vereist. Stream laat zowel native HardwareSerial als een UART-Extender toe.

## Aansluiten

**Aansluitwijze in de bestaande FrameWork-code:**

- **Seriële verbinding:** UART-**TX van de sensor → RX** van de gekozen seriële poort; eventueel sensor-RX ← TX van dezelfde poort.
- **Voeding:** aansluitspanning en UART-logicaniveau volgens de exacte module; gebruik indien nodig een niveauaanpassing.

- UART-TX van de radarsensor → RX van de gekozen native HardwareSerial of UART-Extender; UART-RX van de sensor ← TX.
- Voeding, signaalniveau en seriële instellingen volgens het concrete Seeed-moduleboard; de code verwerkt UART-frames.

**Voor de hardwaretest:**

- Gebruik de bestaande `.ino` uit **Testen** en de bestaande configuratiebestanden onder `src/Configuratie/`; een `*_template.h` wordt niet rechtstreeks ingelezen.
- **Een vaste Arduino-pintoewijzing is niet algemeen geldig:** de gekozen constructor, configuratie en werkelijke bekabeling moeten overeenkomen. Waar hierboven geen exact spanningsniveau of pinheader is bevestigd, mag dat niet uit deze documentatie worden afgeleid.


**Aanvulling aansluitpinnen en elektrische voorwaarden:**

- UART TX→RX en RX←TX; GND→GND; VCC→voeding volgens de eigen MR60BHA2-module. Eventuele status- of triggerlijnen uitsluitend aansluiten als de module ze aanbiedt. UART-logicaniveau controleren.
- De concrete modulevariant bepaalt de **exacte voedingsgrenzen en fysieke pinvolgorde**. Neem die gegevens niet over van een andere module zonder de werkelijke print te controleren.

**Aansluit- en gebruiksgegevens uit de bestaande FrameWork-interface:**

- **UART-bekabeling voor MR60BHA2:** `TX` van de sensor → `RX` van de gekozen host-/Extender-UART, en `RX` van de sensor → `TX` van die UART indien de gebruikte interface beide richtingen nodig heeft. `GND` moet gemeenschappelijk zijn.
- Controleer de UART-logicaniveaus én de vereiste baudrate van de fysieke module; een UART-pin is geen RS-232-niveau. Kies exact de door de constructor ondersteunde native seriële poort of UART-Extender-kanaal.
- De bestaande constructors ondersteunen de seriële poort met native TX/RX of, waar beschikbaar, een kanaal van een UART-Extender.
- Kruis de UART-signalen (sensor-TX naar host-RX) en controleer baudrate, voedingsspanning en elektrisch UART-niveau van de concrete radarmodule.

**Bestaande constructor (parameter-/pinvolgorde):**

```cpp
MR60BHA2(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareSerial* poort, HardwareResourcePin np_TX, HardwareResourcePin np_RX)
```

```cpp
MR60BHA2(GedeeldeBusNode* parent, GedeeldeBusComponent component, Stream* poort, HardwareResourcePin np_TX, HardwareResourcePin np_RX)
```

```cpp
MR60BHA2(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t channel)
```

**Configuratie voor deze hardware:**

- Gebruik bij een eigen configuratie `src/Configuratie/Sensoren/ConfigMR60BHA2.h` (het aanwezige `src/Configuratie/Sensoren/ConfigMR60BHA2_template.h` is een kopieersjabloon en wordt niet rechtstreeks ingelezen).
- Voor één aangesloten exemplaar: `SENSOR_MR60BHA2_AANTAL` op **1**, behalve wanneer de bestaande groeps-/setconfiguratie uitdrukkelijk een andere betekenis heeft.
- De bestaande configuratie bevat deze aansluitrelevante definities (gebruik de **werkelijke aangesloten** pinnen, adressen en referenties):
  - `SENSOR_MR60BHA2_AANTAL`: uitgecommentarieerd sjabloonvoorstel `1`; zonder actieve override geldt de fallback `0` uit `SystemConfig.h`.

**Letterlijk constructorfragment uit de bestaande voorbeeldsketch** (alle gebruikte parent-objecten moeten zelf correct aangesloten en geïnitialiseerd zijn):

```cpp
GedeeldeBusNewComponent<MR60BHA2>(
      &Native, GedeeldeBusComponent::MR60BHA2, &Serial1, HardwareResourcePin::NP_D1, HardwareResourcePin::NP_D0);
```

**Bestaande test-/voorbeeldsketches:**

- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_MR60BHA2/Sensor_MR60BHA2.ino`

**Elektrische aansluiting en randvoorwaarden volgens Seeed Studio (MR60BHA2-kit):**

- De fabrikant beschrijft de **MR60BHA2-radarmodule** en een afzonderlijke complete kit met XIAO ESP32C6. De huidige FrameWorkconstructor gebruikt een **seriële poort**; verwar die UART-verbinding niet met de voeding/headers van de volledige kit.
- Bij toepassing van de complete Seeed-kit is de officiële kitvoeding **5 V / 1 A**. Dit is **geen bevestigde voedingswaarde van een losse MR60BHA2-module**. Gebruik de seriële TX/RX-pinout van de werkelijk gemonteerde module.

**Technische handleiding:** https://wiki.seeedstudio.com/getting_started_with_mr60bha2_mmwave_kit/

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_MR60BHA2/`.
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.

