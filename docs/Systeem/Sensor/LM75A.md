# LM75A

## Implementatie

- Broncode: `src/Systeem/Sensor/LM75A.h` en `LM75A.cpp`.
- C++-component: `LM75A`, afgeleid van `Sensor`.
- Parent: `HardwareResourceTypeI2C*` in de constructor.
- Componentcode: `LM75A` (0xD6).
- Configuratie: `src/Configuratie/Sensoren/ConfigLM75A_template.h`; de optionele persoonlijke versie heet telkens zonder `_template`.
- Data-kanalen: 1.
- Externe library: `M2M_LM75A.h`.

## Aansluiten

**Aansluitwijze in de bestaande FrameWork-code:**

- **I²C:** module-**SDA → SDA** en module-**SCL → SCL** van de geselecteerde I²C-bus of het juiste TCA9548A-kanaal.
- **Voeding:** module-VCC en GND verbinden volgens de **specificatie van het concrete breakoutboard** en de logicaspanning van het gebruikte Arduino-board. Controleer level shifting en I²C-pull-ups op de module.
- **Adres:** de I²C-`HardwareResourceTypeI2C`-parent moet hetzelfde hardwareadres gebruiken als de fysieke module. Controleer eventuele adresselectiepinnen of soldeerbruggen.

**Voor de hardwaretest:**

- Gebruik de bestaande `.ino` uit **Testen** en de bestaande configuratiebestanden onder `src/Configuratie/`; een `*_template.h` wordt niet rechtstreeks ingelezen.
- **Een vaste Arduino-pintoewijzing is niet algemeen geldig:** de gekozen constructor, configuratie en werkelijke bekabeling moeten overeenkomen. Waar hierboven geen exact spanningsniveau of pinheader is bevestigd, mag dat niet uit deze documentatie worden afgeleid.


**Aanvulling aansluitpinnen en elektrische voorwaarden:**

- SDA→SDA, SCL→SCL, GND→GND, VCC→gespecificeerde spanning; A0/A1/A2 selecteren I²C-adres 0x48–0x4F. OS is optioneel voor alarm; I²C-pull-ups zijn afhankelijk van breakout en bus.
- De concrete modulevariant bepaalt de **exacte voedingsgrenzen en fysieke pinvolgorde**. Neem die gegevens niet over van een andere module zonder de werkelijke print te controleren.

**Aansluit- en gebruiksgegevens uit de bestaande FrameWork-interface:**

- **I²C-bekabeling voor LM75A:** `SDA` van de module → `SDA` van de I²C-parent; `SCL` → `SCL` van dezelfde parent; `GND` → gemeenschappelijke massa. De parent krijgt het werkelijk geselecteerde I²C-adres (of de juiste TCA9548A-tak).
- De voedingspin van de module gaat naar een geschikte voedingsrail volgens de **concrete chip- of breakoutvariant**. Een 5V-controller maakt niet automatisch alle sensor-I/O 5V-tolerant; controleer de aanwezige pull-ups en eventuele levelshifters.
- `LM75A` is een I²C-sensor met `HardwareResourceTypeI2C*`-parent; uitlezen gaat via `LeesHoofdwaarde(float&)`.
- De werkelijke adrespinnen van de concrete module moeten overeenkomen met het I²C-adres dat bij de parent is ingesteld.

**Bestaande constructor (parameter-/pinvolgorde):**

```cpp
LM75A(HardwareResourceTypeI2C* parent, GedeeldeBusComponent component)
```

**Configuratie voor deze hardware:**

- Gebruik bij een eigen configuratie `src/Configuratie/Sensoren/ConfigLM75A.h` (het aanwezige `src/Configuratie/Sensoren/ConfigLM75A_template.h` is een kopieersjabloon en wordt niet rechtstreeks ingelezen).
- Voor één aangesloten exemplaar: `SENSOR_LM75A_AANTAL` op **1**, behalve wanneer de bestaande groeps-/setconfiguratie uitdrukkelijk een andere betekenis heeft.
- De bestaande configuratie bevat deze aansluitrelevante definities (gebruik de **werkelijke aangesloten** pinnen, adressen en referenties):
  - `SENSOR_LM75A_AANTAL`: uitgecommentarieerd sjabloonvoorstel `1`; zonder actieve override geldt de fallback `0` uit `SystemConfig.h`.
  - `I2C_ADDRESS_SENSOR_LM75A_1`: sjabloonwaarde `I2C_ADDRESS_NONE`.

**Letterlijk constructorfragment uit de bestaande voorbeeldsketch** (alle gebruikte parent-objecten moeten zelf correct aangesloten en geïnitialiseerd zijn):

```cpp
GedeeldeBusNewComponent<LM75A>(&apparaat,GedeeldeBusComponent::LM75A);
```

**Bestaande test-/voorbeeldsketches:**

- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_LM75A/Sensor_LM75A.ino`

**Elektrische aansluiting en randvoorwaarden volgens Texas Instruments:**

- De LM75A gebruikt I²C met instelbare adrespinnen en een optionele overtemperatuur-uitgang O.S. De chipvoeding volgens TI bedraagt **2,7–5,5 V**; het gekozen breakoutboard kan eigen pull-ups hebben.

**Technische handleiding:** https://www.ti.com/product/LM75A

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_LM75A/`.
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.

