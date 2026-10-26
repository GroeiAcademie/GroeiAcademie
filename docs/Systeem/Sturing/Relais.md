# Relais

## Component

Relais. De huidige implementatie bevat de component als header-only codevoorstel; dit document beschrijft die bestaande implementatie, niet een gevalideerde hardwarewerking.

## Implementatie

- Broncode: `src/Systeem/Sturing/Relais.h`.
- C++-component: `Relais`, rechtstreeks afgeleid van `GedeeldeBusNode`.
- Aansluiting: Digitaal parentkanaal via `WriteExtenderPin()` en `SetExtenderPinMode()`.
- Bestaande aansturing: `setRelayStatusOnOff(bool aan)`.
- Lifecycle: de component bevat een `Activeren()`-override en een `StopSturing()`-override voor uitschakeling.
- Configuratie: `src/Configuratie/Sturingen/ConfigRelais_template.h`; optioneel persoonlijk `ConfigRelais.h`.

## Aandachtspunt

De digitale parentfuncties zijn afhankelijk van `GEDEELDE_BUS_MIXED_PINS`; actief-laag/actief-hoog en foutveiligheid moeten overeenkomen met de module.

## Testen

De huidige implementatie bevat een voorbeeld voor deze Sturing in `examples/` en het script `extras/TestLibrarySturingen-v2.0.0.cmd`. Compilatie en hardwaregedrag zijn **niet gevalideerd**.

Configuratie: `STURING_RELAIS_AANTAL`: 0 = niet opgenomen; groter dan 0 = opgenomen; het aantal objecten wordt niet begrensd.

`Activeren()` ontvangt geen resultaat van `WriteExtenderPin()` of `SetExtenderPinMode()` (beide `void`); eventuele I²C-fouten worden door de bestaande Extender-runtimeafhandeling verwerkt.
