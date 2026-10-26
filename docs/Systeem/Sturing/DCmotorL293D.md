# DCmotorL293D

## Component

DC-motor via L293D. De huidige implementatie bevat de component als header-only codevoorstel; dit document beschrijft die bestaande implementatie, niet een gevalideerde hardwarewerking.

## Implementatie

- Broncode: `src/Systeem/Sturing/DCmotorL293D.h`.
- C++-component: `DCmotorL293D`, rechtstreeks afgeleid van `GedeeldeBusNode`.
- Aansluiting: Drie PWM-uitgangen van de parent; richting en vrijgave via `WriteExtenderPWM()`.
- Bestaande aansturing: `setDirectionAndSpeed(bool vooruit, uint16_t pwm)`.
- Lifecycle: de component bevat een `Activeren()`-override en een `StopSturing()`-override voor uitschakeling.
- Configuratie: `src/Configuratie/Sturingen/ConfigDCmotorL293D_template.h`; optioneel persoonlijk `ConfigDCmotorL293D.h`.

## Aandachtspunt

Controleer dat de gekozen module deze drie signalen werkelijk zo aansluit en dat de voedings- en vermogensgrenzen kloppen.

## Testen

De huidige implementatie bevat een voorbeeld voor deze Sturing in `examples/` en het script `extras/TestLibrarySturingen-v2.0.0.cmd`. Compilatie en hardwaregedrag zijn **niet gevalideerd**.

Configuratie: `STURING_DCMOTORL293D_AANTAL`: 0 = niet opgenomen; groter dan 0 = opgenomen; het aantal objecten wordt niet begrensd.

Alle drie de stuursignalen gebruiken drie kanalen op één PCA9685.
