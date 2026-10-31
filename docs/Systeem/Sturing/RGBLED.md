# RGBLED

## Component

RGB LED. De huidige implementatie bevat de component als header-only codevoorstel; dit document beschrijft die bestaande implementatie, niet een gevalideerde hardwarewerking.

## Implementatie

- Broncode: `src/Systeem/Sturing/RGBLED.h`.
- C++-component: `RGBLED`, rechtstreeks afgeleid van `GedeeldeBusNode`.
- Aansluiting: Drie PWM-kanalen op de parent; omkering is voorzien.
- Bestaande aansturing: `setRGBcolor(uint16_t rood, uint16_t groen, uint16_t blauw)`.
- Lifecycle: de component bevat een `Activeren()`-override en een `StopSturing()`-override voor uitschakeling.
- Configuratie: `src/Configuratie/Sturingen/ConfigRGBLED_template.h`; optioneel persoonlijk `ConfigRGBLED.h`.

## Aandachtspunt

Type RGB-module, gemeenschappelijke anode/kathode, stroombegrenzing en voedingsspanning controleren.

## Aansluiten

**Aansluitwijze in de bestaande FrameWork-code:**

- De signaalverbindingen worden hieronder per onderdeel gespecificeerd waar ze uit de bestaande componentinterface blijken.
- Controleer voedingsspanning, pinvolgorde en externe stroombegrenzing bij de concrete hardware-uitvoering voordat je spanning aanlegt.

- R, G en B → drie afzonderlijke PWM-kanalen (`red`, `green`, `blue`) op de gekozen parent.
- Bepaal **gemeenschappelijke anode of kathode** van de RGB-LED of module; de constructorparameter `commonAnode` draait PWM-waarden om.
- Gebruik per kleur de passende stroombegrenzing/driver; een kale LED niet rechtstreeks aan een vermogensuitgang zonder stroombegrenzing verbinden.

**Voor de hardwaretest:**

- Gebruik de bestaande `.ino` uit **Testen** en de bestaande configuratiebestanden onder `src/Configuratie/`; een `*_template.h` wordt niet rechtstreeks ingelezen.
- **Een vaste Arduino-pintoewijzing is niet algemeen geldig:** de gekozen constructor, configuratie en werkelijke bekabeling moeten overeenkomen. Waar hierboven geen exact spanningsniveau of pinheader is bevestigd, mag dat niet uit deze documentatie worden afgeleid.
- De aanwezigheid van deze rubriek is **geen bewijs van een geslaagde hardwaretest**.

## Testen

De huidige implementatie bevat een voorbeeld voor deze Sturing in `examples/` en het script `extras/TestLibrarySturingen-v2.0.0.cmd`. Compilatie en hardwaregedrag zijn **niet gevalideerd**.

Configuratie: `STURING_RGBLED_AANTAL`: 0 = niet opgenomen; groter dan 0 = opgenomen; het aantal objecten wordt niet begrensd.
