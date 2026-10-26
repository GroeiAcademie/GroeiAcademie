# DCmotorL298N

## Component

DC-motor via L298N. De huidige implementatie bevat de component als header-only codevoorstel; dit document beschrijft die bestaande implementatie, niet een gevalideerde hardwarewerking.

## Implementatie

- Broncode: `src/Systeem/Sturing/DCmotorL298N.h`.
- C++-component: `DCmotorL298N`, rechtstreeks afgeleid van `GedeeldeBusNode`.
- Aansluiting: Drie PWM-uitgangen van de parent; richting en vrijgave via `WriteExtenderPWM()`.
- Bestaande aansturing: `setDirectionAndSpeed(bool vooruit, uint16_t pwm)`.
- Lifecycle: de component bevat een `Activeren()`-override en een `StopSturing()`-override voor uitschakeling.
- Configuratie: `src/Configuratie/Sturingen/ConfigDCmotorL298N_template.h`; optioneel persoonlijk `ConfigDCmotorL298N.h`.

## Aandachtspunt

Controleer de jumper-/EN-aansluiting, voeding en motorstroom van de concrete driver.

## Testen

De huidige implementatie bevat een voorbeeld voor deze Sturing in `examples/` en het script `extras/TestLibrarySturingen-v2.0.0.cmd`. Compilatie en hardwaregedrag zijn **niet gevalideerd**.

Configuratie: `STURING_DCMOTORL298N_AANTAL`: 0 = niet opgenomen; groter dan 0 = opgenomen; het aantal objecten wordt niet begrensd.

Alle drie de stuursignalen gebruiken drie kanalen op één PCA9685.
