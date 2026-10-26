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

## Testen

De huidige implementatie bevat een voorbeeld voor deze Sturing in `examples/` en het script `extras/TestLibrarySturingen-v2.0.0.cmd`. Compilatie en hardwaregedrag zijn **niet gevalideerd**.

Configuratie: `STURING_RGBLED_AANTAL`: 0 = niet opgenomen; groter dan 0 = opgenomen; het aantal objecten wordt niet begrensd.
