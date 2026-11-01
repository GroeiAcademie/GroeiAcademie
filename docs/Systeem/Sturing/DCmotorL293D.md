# DCmotorL293D

## Component

DC-motor via L293D. De component is header-only geïmplementeerd.

## Implementatie

- Broncode: `src/Systeem/Sturing/DCmotorL293D.h`.
- C++-component: `DCmotorL293D`, rechtstreeks afgeleid van `GedeeldeBusNode`.
- Aansluiting: Drie PWM-uitgangen van de parent; richting en vrijgave via `WriteExtenderPWM()`.
- Bestaande aansturing: `setDirectionAndSpeed(bool vooruit, uint16_t pwm)`.
- Lifecycle: de component bevat een `Activeren()`-override en een `StopSturing()`-override voor uitschakeling.
- Configuratie: `src/Configuratie/Sturingen/ConfigDCmotorL293D_template.h`; optioneel persoonlijk `ConfigDCmotorL293D.h`.

## Aandachtspunt

Controleer dat de gekozen module deze drie signalen werkelijk zo aansluit en dat de voedings- en vermogensgrenzen kloppen.

## Aansluiten

**Aansluitwijze in de bestaande FrameWork-code:**

- De signaalverbindingen worden hieronder per onderdeel gespecificeerd waar ze uit de bestaande componentinterface blijken.
- Controleer voedingsspanning, pinvolgorde en externe stroombegrenzing bij de concrete hardware-uitvoering voordat je spanning aanlegt.

- IN1, IN2 en ENABLE → de drie PWM-kanalen van de parent in de constructorvolgorde; controleer de daadwerkelijke modulelabels.
- DC-motor → de motoruitgangen van L293D, niet rechtstreeks op de controller.
- Logica- en motorvoeding conform driverbord, motorstroom en gemeenschappelijke GND.

**Voor de hardwaretest:**

- Gebruik de bestaande `.ino` uit **Testen** en de bestaande configuratiebestanden onder `src/Configuratie/`; een `*_template.h` wordt niet rechtstreeks ingelezen.
- **Een vaste Arduino-pintoewijzing is niet algemeen geldig:** de gekozen constructor, configuratie en werkelijke bekabeling moeten overeenkomen. Waar hierboven geen exact spanningsniveau of pinheader is bevestigd, mag dat niet uit deze documentatie worden afgeleid.


**Aanvulling aansluitpinnen en elektrische voorwaarden:**

- Logica VCC1, motorvoeding VCC2 en GND zijn afzonderlijke aansluitingen bij de L293D-chip; stuur IN1/IN2 met controllerkanalen en ENABLE met PWM; motor op OUT1/OUT2. Controleer of de gebruikte module al diodes/ontkoppeling heeft.
- De concrete modulevariant bepaalt de **exacte voedingsgrenzen en fysieke pinvolgorde**. Neem die gegevens niet over van een andere module zonder de werkelijke print te controleren.

**Aansluit- en gebruiksgegevens uit de bestaande FrameWork-interface:**

- **Signalen:** PCA9685-kanaal `pin1` → `IN1`, `pin2` → `IN2` en `pinEnable` → `EN/ENABLE` van de motorbrug. De drie kanaalindices zijn verschillend en liggen elk tussen 0 en 15. De motor zelf komt **uitsluitend** op de twee driveruitgangen.
- Voed de motorbruglogica en de motor via de geschikte daarvoor bedoelde voeding; verbind stuurgedeelte en driver-GND. De gekozen motorvoeding volgt niet uit een PCA9685-PWM-kanaal.
- Voor de hier geïmplementeerde motorsturing zijn **drie afzonderlijke kanalen op dezelfde PCA9685** nodig. Constructorvolgorde: `pin1` → IN1, `pin2` → IN2, `pinEnable` → ENABLE. Kanaalnummers zijn 0 tot en met 15; identieke kanalen worden bij `Activeren()` geweigerd.
- `setDirectionAndSpeed(bool forward, uint16_t pwm)` aanvaardt een PWM-waarde van **0 tot en met 4095**. De methode zet eerst ENABLE op nul, schrijft daarna de twee richtingssignalen en activeert pas vervolgens ENABLE. Na stoppen worden ENABLE, IN1 en IN2 nul.
- Het bestaande voorbeeld `examples/Systeem/GedeeldeBus/Sturingen/Sturing_DCmotorL293D_HardwareTest/Sturing_DCmotorL293D_HardwareTest.ino` gebruikt `ExtenderPCA9685` met kanalen **0, 1 en 2**. Dit zijn *voorbeeldkanalen*, geen vaste aansluitpinnen op Arduino.
- Voor deze interactieve hardwaretest: start de seriële monitor op `SERIAL_BAUDRATE`; controleer vóór de opdracht `START` de afzonderlijke motorvoeding, gemeenschappelijke GND en motoruitgangen. Voer daarna `+1` tot `+100` voor de ene richting, `-1` tot `-100` voor de andere richting en `0` voor stoppen in. `STOP` beëindigt de test; om opnieuw te testen moet je de Arduino resetten. De percentages worden in de sketch omgerekend naar 0–4095.
- De L293D-uitgangen `OUT1`/`OUT2` gaan naar de motor; de PCA9685-kanalen voeren **uitsluitend stuursignalen**. Verbind een motor nooit rechtstreeks met de PCA9685. Controleer bij gebruik van een losse L293D-chip de vereiste voeding, vrijgave en vrijloopbeveiliging aan de hand van de concrete hardware-uitvoering.
- In `Sturing_DCmotorL293D_HardwareTest.ino` wordt `DCmotorL293D sturing(&extender,0,1,2)` gebruikt. De waarden 0/1/2 zijn **PCA9685-kanaalnummers**, niet Arduino D0/D1/D2. Na `START`: `+1`–`+100`, `-1`–`-100`, `0` voor stoppen; `STOP` vereist een boardreset voor een volgende proef.

**Bestaande constructor (parameter-/pinvolgorde):**

```cpp
DCmotorL293D(GedeeldeBusNode* parent,uint8_t pin1,uint8_t pin2,uint8_t pinEnable)
```

**Configuratie voor deze hardware:**

- Gebruik bij een eigen configuratie `src/Configuratie/Sturingen/ConfigDCmotorL293D.h` (het aanwezige `src/Configuratie/Sturingen/ConfigDCmotorL293D_template.h` is een kopieersjabloon en wordt niet rechtstreeks ingelezen).
- Voor één aangesloten exemplaar: `STURING_DCMOTORL293D_AANTAL` op **1**, behalve wanneer de bestaande groeps-/setconfiguratie uitdrukkelijk een andere betekenis heeft.
- De bestaande configuratie bevat deze aansluitrelevante definities (gebruik de **werkelijke aangesloten** pinnen, adressen en referenties):
  - `STURING_DCMOTORL293D_AANTAL`: sjabloonwaarde `0`.

**Letterlijk constructorfragment uit de bestaande voorbeeldsketch** (alle gebruikte parent-objecten moeten zelf correct aangesloten en geïnitialiseerd zijn):

```cpp
DCmotorL293D sturing(&extender,0,1,2);
```

**Bestaande test-/voorbeeldsketches:**

- `examples/Systeem/GedeeldeBus/Sturingen/Sturing_DCmotorL293D_HardwareTest/Sturing_DCmotorL293D_HardwareTest.ino`
- `examples/Systeem/GedeeldeBus/Sturingen/Sturing_DCmotorL293D/Sturing_DCmotorL293D.ino`

**Elektrische aansluiting en randvoorwaarden volgens Texas Instruments:**

- Bij de **losse L293D in 16-pins PDIP**: `1,2EN` = pin 1 → PCA9685-`pinEnable`; `1A` = pin 2 → `pin1`; `2A` = pin 7 → `pin2`; motor tussen `1Y` = pin 3 en `2Y` = pin 6. Gebruik je de tweede halve brug, dan liggen `3,4EN` op pin 9, `3A` op pin 10, `4A` op pin 15 en motoruitgangen `3Y`/`4Y` op pin 11/14. Kies voor één motor één compleet brugpaar.
- De **chipvoeding** is `VCC1` = pin 16 (**5 V logica**), `VCC2` = pin 8 (**motorvoeding 4,5–36 V**) en GND = pinnen 4, 5, 12, 13. L293D bevat interne clampdiodes en is volgens TI bedoeld voor maximaal **600 mA continu per uitgang**; de aangesloten motor moet binnen de vermogens- en thermische grenzen blijven. Op een module kunnen headerbenamingen anders zijn.
- De PWM-**kanaalnummers 0/1/2** uit de voorbeeldsketch zijn **geen Arduino D0/D1/D2**. Verbind de drie fysieke PCA9685-uitgangssignalen aan de L293D.

**Technische handleiding:** https://www.ti.com/product/L293D


**Nog vóór de hardwaretest:** controleer bij een **losse L293D** de 16-pins DIP-oriëntatie (inkeping/pin 1), de afzonderlijke `VCC1`-logica en `VCC2`-motorvoeding en de gemeenschappelijke GND-pinnen. Texas Instruments specificeert **600 mA per uitgang continu** voor de L293D en geïntegreerde clampdiodes, maar dit is **geen garantie dat jouw motor minder dan 600 mA trekt bij blokkering**. Controleer stallstroom, thermische grenzen en benodigde ontkoppeling. De drie PWM-kanalen in de FrameWork besturen uitsluitend de twee richtingsingangen en ENABLE; de motor zit aan de driveruitgangen.

**Fabrikantdocumentatie:** https://www.ti.com/product/L293D

## Testen

De huidige implementatie bevat een voorbeeld voor deze Sturing in `examples/` en het script `extras/TestLibrarySturingen-v2.0.0.cmd`. 

Configuratie: `STURING_DCMOTORL293D_AANTAL`: 0 = niet opgenomen; groter dan 0 = opgenomen; het aantal objecten wordt niet begrensd.

Alle drie de stuursignalen gebruiken drie kanalen op één PCA9685.
