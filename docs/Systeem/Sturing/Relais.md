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

## ISD1820

De ISD1820 (geluidsfragment opnemen en afspelen) wordt bediend met twee keer `Relais`: REC (hoog zolang er opgenomen wordt) en PLAYE (een korte puls speelt het fragment af). Voorbeeld: `examples/Systeem/GedeeldeBus/Sturingen/Sturing_ISD1820/`.

## Aansluiten

**Aansluitwijze in de bestaande FrameWork-code:**

- De signaalverbindingen worden hieronder per onderdeel gespecificeerd waar ze uit de bestaande componentinterface blijken.
- Controleer voedingsspanning, pinvolgorde en externe stroombegrenzing bij de concrete hardware-uitvoering voordat je spanning aanlegt.

- Besturingsingang IN → één digitaal kanaal van de GPIO-parent (bijvoorbeeld PCF8574), zoals gekozen in de constructor.
- Relaismodule-voeding en GND volgens de module; bij een externe voeding de voorgeschreven referentie/isolatie van de module volgen.
- Contacten COM/NO/NC uitsluitend aansluiten op een **geschikte, veilig afgeschermde testbelasting**. **Geen netspanning aansluiten voor deze softwaretesten.**
- Controleer actieve-laag/actieve-hoog logica en rusttoestand vóór de belasting wordt aangesloten.

**Voor de hardwaretest:**

- Gebruik de bestaande `.ino` uit **Testen** en de bestaande configuratiebestanden onder `src/Configuratie/`; een `*_template.h` wordt niet rechtstreeks ingelezen.
- **Een vaste Arduino-pintoewijzing is niet algemeen geldig:** de gekozen constructor, configuratie en werkelijke bekabeling moeten overeenkomen. Waar hierboven geen exact spanningsniveau of pinheader is bevestigd, mag dat niet uit deze documentatie worden afgeleid.
- De aanwezigheid van deze rubriek is **geen bewijs van een geslaagde hardwaretest**.

## Testen

De huidige implementatie bevat een voorbeeld voor deze Sturing in `examples/` en het script `extras/TestLibrarySturingen-v2.0.0.cmd`. Compilatie en hardwaregedrag zijn **niet gevalideerd**.

Configuratie: `STURING_RELAIS_AANTAL`: 0 = niet opgenomen; groter dan 0 = opgenomen; het aantal objecten wordt niet begrensd.

`Activeren()` ontvangt geen resultaat van `WriteExtenderPin()` of `SetExtenderPinMode()` (beide `void`); eventuele I²C-fouten worden door de bestaande Extender-runtimeafhandeling verwerkt.
