# StimulusConfig.h

Bronbestand: [`../../src/Configuratie/StimulusConfig.h`](../../src/Configuratie/StimulusConfig.h)

Instellingen specifiek voor de Stimulus-toepassing (VINGERTEST): meetversie, marges, drempels en timeouts. Deze waarden zijn softwareconfiguratie. Ze zijn geen universele fysiologische normen. Waar een define via `ADC(...)` geschreven is, herschaalt die automatisch mee met `ADC_BACKEND`/`UNO_VERSION` uit `SystemConfig.h` — zie `SystemConfig.md`.

## Stimulusversie

```cpp
#define STIMULUS_BASIC     1  // basismeting: tiktijd en gemiddelde tikkracht
#define STIMULUS_EXTENDED  2  // uitgebreide meting: ook piekkracht, tijd tot piek, opbouw- en afbouwsnelheid
```

## Marge t.o.v. de nulmeting

`DEFAULT_MARGE_FACTOR` bepaalt hoe een percentagemarge verdeeld wordt over onder- en bovengrens:

| Waarde | Betekenis (bij 30%-marge) |
|---|---|
| `1` | ±30% voor zowel onder- als bovengrens |
| `2` | 30% totaal, dus ±15% |
| `4` | 30% totaal, dus ±7,5% |

Bijhorende standaardwaarden (zie `Stimulus.cpp`):

| Define | Betekenis |
|---|---|
| `DEFAULT_TOEGESTANE_MARGE_TIKTIJD` | Standaardmarge (%) op de tiktijd, 30 |
| `DEFAULT_TOEGESTANE_MARGE_TIKKRACHT` | Standaardmarge (%) op de tikkracht, 25 |
| `DEFAULT_TOEGESTANE_MARGE_SIMULTANE_STARTTIJD_MS` | Toegestane tijdsmarge (ms) om metingen als "gelijktijdig gestart" te beschouwen, 100 |

## Instortende moeilijkheidsgraad (stap 3, de lange aanhoudtik)

```cpp
#define INSTORTEND_MINIMALE_FACTOR  3  // kortste toegelaten duur (veelvoud van de nulmeting)
#define INSTORTEND_MAXIMALE_FACTOR  7  // langste toegelaten duur (veelvoud van de nulmeting)
```

**Let op**: `INSTORTEND_MINIMALE_FACTOR + INSTORTEND_MAXIMALE_FACTOR` moet een **even** getal zijn — dit is een harde vereiste van het algoritme, geen stijlkeuze.

```cpp
#define INSTORTEND_TOV_NULMETING          0
#define INSTORTEND_MOEILIJKHEIDSGRAAD_1  -1
#define INSTORTEND_MOEILIJKHEIDSGRAAD_2  -2
#define INSTORTEND_MOEILIJKHEIDSGRAAD_3  -3
```

Gebruikt als `doelTikTijd`-parameter in `VergelijkStimulus()`: `> 0` = expliciete milliseconden, `0` = nulmeting, `-1`/`-2`/`-3` = oplopende moeilijkheidsgraad.

## Drukdrempels (alle via `ADC(...)`, dus resolutie-onafhankelijk)

| Define | Bereik (10-bit referentie) | Betekenis |
|---|---|---|
| `TIK_MINIMALE_DRUKWAARDE` | `ADC(50)` | 0-50: geen druk, filtert elektronische ruis op de sensoren |
| `TIKKRACHT_MINIMALE_COMFORT_GRENS` | `ADC(200)` | 200-499: lichte druk, minimum voor een bruikbare stimulus |
| `TIKKRACHT_MAXIMALE_COMFORT_GRENS` | `ADC(800)` | 800-1023: stevige druk, maximum om blessures/blauwe plekken te voorkomen |
| `SCENARIO_GEWENSTE_TIKKRACHT_ADC` | `ADC(400)` | Gewenste tikkracht — voor later, niet actief gebruikt |

## Offsetbepaling

| Define | Betekenis |
|---|---|
| `OFFSET_METING_TIJD_MS` | Duur (ms) van de nulmeting om de sensoroffset te bepalen, 2000 |
| `OFFSET_VEILIGHEIDSMARGE` | `ADC(5)` — veiligheidsmarge bovenop de gemeten offset |
| `MAX_AANTAL_POGINGEN_NULMETING` | Aantal toegestane pogingen voor een geldige nulmeting, 3 |

## Timeouts en achterdeur

| Define | Betekenis |
|---|---|
| `EXIT_TIKTIJD_MS` | 3000 — achterdeur: na deze tijd kan de oefening verlaten worden |
| `EXIT_NO_ACTION_MS` | 9000 — timeout bij geen enkele actie |
| `MINIMALE_TIKTIJD_MS` | 100 — Fysiologisch onderbouwde ondergrens nog te bepalen |
| `MAXIMALE_TIKTIJD_MS` | 2000 — Fysiologisch onderbouwde bovengrens nog te bepalen |


