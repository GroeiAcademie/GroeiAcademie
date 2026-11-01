# DCmotorL298N

## Component

DC-motor via L298N. De component is header-only geïmplementeerd.

## Implementatie

- Broncode: `src/Systeem/Sturing/DCmotorL298N.h`.
- C++-component: `DCmotorL298N`, rechtstreeks afgeleid van `GedeeldeBusNode`.
- Aansluiting: Drie PWM-uitgangen van de parent; richting en vrijgave via `WriteExtenderPWM()`.
- Bestaande aansturing: `setDirectionAndSpeed(bool vooruit, uint16_t pwm)`.
- Lifecycle: de component bevat een `Activeren()`-override en een `StopSturing()`-override voor uitschakeling.
- Configuratie: `src/Configuratie/Sturingen/ConfigDCmotorL298N_template.h`; optioneel persoonlijk `ConfigDCmotorL298N.h`.

## Aandachtspunt

Controleer de jumper-/EN-aansluiting, voeding en motorstroom van de concrete driver.

## Aansluiten

**Aansluitwijze in de bestaande FrameWork-code:**

- De signaalverbindingen worden hieronder per onderdeel gespecificeerd waar ze uit de bestaande componentinterface blijken.
- Controleer voedingsspanning, pinvolgorde en externe stroombegrenzing bij de concrete hardware-uitvoering voordat je spanning aanlegt.

- IN1, IN2 en EN(A/B) → de drie PWM-kanalen van de parent in de constructorvolgorde; bij PWM-sturing de eventuele EN-jumper controleren.
- Motor → één motoruitgangspaar van L298N, niet rechtstreeks op Arduino/Extender.
- Motorspanning, stroom en logicavoeding volgens de gebruikte L298N-module; massa van voeding en besturing gemeenschappelijk.

**Voor de hardwaretest:**

- Gebruik de bestaande `.ino` uit **Testen** en de bestaande configuratiebestanden onder `src/Configuratie/`; een `*_template.h` wordt niet rechtstreeks ingelezen.
- **Een vaste Arduino-pintoewijzing is niet algemeen geldig:** de gekozen constructor, configuratie en werkelijke bekabeling moeten overeenkomen. Waar hierboven geen exact spanningsniveau of pinheader is bevestigd, mag dat niet uit deze documentatie worden afgeleid.


**Aanvulling aansluitpinnen en elektrische voorwaarden:**

- Motorvoeding op de motorvoedingsklem, GND gemeenschappelijk met controller, OUT1/OUT2→motor, IN1/IN2→stuursignalen, ENA→PWM indien jumper verwijderd. **5V-regelaar en 5V-pen op L298N-modules zijn variantafhankelijk; niet zonder moduleschema als ingang/uitgang gebruiken.**
- De concrete modulevariant bepaalt de **exacte voedingsgrenzen en fysieke pinvolgorde**. Neem die gegevens niet over van een andere module zonder de werkelijke print te controleren.

**Aansluit- en gebruiksgegevens uit de bestaande FrameWork-interface:**

- **Signalen:** PCA9685-kanaal `pin1` → `IN1`, `pin2` → `IN2` en `pinEnable` → `EN/ENABLE` van de motorbrug. De drie kanaalindices zijn verschillend en liggen elk tussen 0 en 15. De motor zelf komt **uitsluitend** op de twee driveruitgangen.
- Voed de motorbruglogica en de motor via de geschikte daarvoor bedoelde voeding; verbind stuurgedeelte en driver-GND. De gekozen motorvoeding volgt niet uit een PCA9685-PWM-kanaal.
- De bestaande constructor gebruikt drie **verschillende** PCA9685-kanalen (`pin1`, `pin2`, `pinEnable`) op dezelfde parent: IN1, IN2, EN. Bij `Activeren()` moeten alle drie binnen 0–15 liggen.
- `setDirectionAndSpeed(bool forward, uint16_t pwm)` accepteert 0–4095, schakelt EN eerst uit, legt de richting vast en schrijft vervolgens de EN-PWM. `StopSturing()` schrijft drie keer nul. De motor wordt op de bruguitgangen aangesloten, niet op een PWM-kanaal.
- Gebruik voor de interactieve test `examples/Systeem/GedeeldeBus/Sturingen/Sturing_DCmotorL298N_HardwareTest/Sturing_DCmotorL298N_HardwareTest.ino`; de exacte seriële opdrachten staan in de sketch. Controleer bij de gebruikte L298N-module de EN-jumper: de module moet PWM op EN kunnen ontvangen.

**Bestaande constructor (parameter-/pinvolgorde):**

```cpp
DCmotorL298N(GedeeldeBusNode* parent,uint8_t pin1,uint8_t pin2,uint8_t pinEnable)
```

**Configuratie voor deze hardware:**

- Gebruik bij een eigen configuratie `src/Configuratie/Sturingen/ConfigDCmotorL298N.h` (het aanwezige `src/Configuratie/Sturingen/ConfigDCmotorL298N_template.h` is een kopieersjabloon en wordt niet rechtstreeks ingelezen).
- Voor één aangesloten exemplaar: `STURING_DCMOTORL298N_AANTAL` op **1**, behalve wanneer de bestaande groeps-/setconfiguratie uitdrukkelijk een andere betekenis heeft.
- De bestaande configuratie bevat deze aansluitrelevante definities (gebruik de **werkelijke aangesloten** pinnen, adressen en referenties):
  - `STURING_DCMOTORL298N_AANTAL`: sjabloonwaarde `0`.

**Letterlijk constructorfragment uit de bestaande voorbeeldsketch** (alle gebruikte parent-objecten moeten zelf correct aangesloten en geïnitialiseerd zijn):

```cpp
DCmotorL298N sturing(&extender,3,4,5);
```

**Bestaande test-/voorbeeldsketches:**

- `examples/Systeem/GedeeldeBus/Sturingen/Sturing_DCmotorL298N_HardwareTest/Sturing_DCmotorL298N_HardwareTest.ino`
- `examples/Systeem/GedeeldeBus/Sturingen/Sturing_DCmotorL298N/Sturing_DCmotorL298N.ino`

**Elektrische aansluiting en randvoorwaarden volgens STMicroelectronics:**

- Bij een L298N-driver worden de twee FrameWork-richtingssignalen op **IN1/IN2** van één motorbrug gezet en het derde PWM-signaal op de bijbehorende **ENA**. De motor komt op de twee motoruitgangen van diezelfde brug. Houd logica- en motorvoeding volgens de werkelijke L298N-module afzonderlijk.
- De L298-chip is een tweekanaals full-bridge-driver; de concrete module kan daarnaast een **5V-regelaar, jumper en externe vrijloopdiodes** bevatten. Hun werking kan niet uit de C++-constructor worden afgeleid; raadpleeg de printlabels.

**Technische handleiding:** https://www.st.com/en/motor-drivers/l298.html

## Testen

De huidige implementatie bevat een voorbeeld voor deze Sturing in `examples/` en het script `extras/TestLibrarySturingen-v2.0.0.cmd`. 

Configuratie: `STURING_DCMOTORL298N_AANTAL`: 0 = niet opgenomen; groter dan 0 = opgenomen; het aantal objecten wordt niet begrensd.

Alle drie de stuursignalen gebruiken drie kanalen op één PCA9685.
