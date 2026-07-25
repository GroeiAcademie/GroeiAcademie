# SystemConfig.h

Bronbestand: [`../../src/Configuratie/SystemConfig.h`](../../src/Configuratie/SystemConfig.h)

Instellingen die het hele framework raken: scherm, sensorpinnen, ADC-backend en debug-uitvoer. Dit is het eerste bestand om te controleren vóór compilatie (zie `README.md`).

## DEBUG

```cpp
#define DEBUG
```

Schakelt `DEBUG_PRINT`/`DEBUG_PRINTLN`/`DEBUG_PRINTLN2` in. Zonder `#define` (dus door de regel in commentaar te zetten) worden deze macro's leeg en verdwijnt alle Serial-uitvoer, ook die van `PrintToScreen()` (zie `docs/Systeem/SCREEN.md`).

**Let op UNO R3**: de combinatie van volledige functionaliteit + uitgebreide debuguitvoer kan de beschikbare flash overschrijden. Schakel `DEBUG` uit voor een normale UNO R3-build wanneer nodig.

## Scherm

| Define | Betekenis |
|---|---|
| `I2C_ADRES` | I2C-adres van het characterscherm. Standaard `0x27`, soms `0x32` of `0x3F` afhankelijk van de I2C-backpack. |
| `ACTIEF_CHARACTER_SCREEN` | Welk schermtype actief is (bv. `SCREEN_LCD1602`). Zie `Screen.h` voor de volledige lijst ondersteunde formaten (LCD1602, LCD1604, LCD2002, LCD2004, LCD4002). |

## Sensoren

| Define | Betekenis |
|---|---|
| `AANTAL_SENSOREN_AANWEZIG` | Aantal fysiek aangesloten sensoren (2 of 4). Scenario's 1-3 gebruiken er 2, scenario 4 kan bij 4 aanwezige sensoren ook met 3 draaien. |
| `PIN_SENSOR_1` .. `PIN_SENSOR_4` | Sensorpin per kanaal. Waarde en betekenis hangen af van `ADC_BACKEND` (zie hieronder). |

## ADC_BACKEND

```cpp
#define ADC_BACKEND_NATIVE   0
#define ADC_BACKEND_ADS1115  1
#define ADC_BACKEND ADC_BACKEND_NATIVE
```

Bepaalt welke ADC de vier sensorkanalen uitleest:

| Keuze | `ADC_BACKEND` | `PIN_SENSOR_1..4` betekent | Hardware |
|---|---|---|---|
| Directe Arduino-ADC | `ADC_BACKEND_NATIVE` | Arduino-analoogpin (`A0`..`A3`) | Niets extra — H6 rechtstreeks ingestoken op H5 pin 7-10 (zie `docs/Toepassingsgebieden/Stimulus/Hardware/`) |
| ADS1115 | `ADC_BACKEND_ADS1115` | ADS1115-kanaalnummer (`0`..`3`) | ADS1115-module op H5. I2C-adres kiezen via H7. Standaard: ADDR naar GND, adres `0x48`. Vereist de optionele afhankelijkheid `Adafruit ADS1X15` (zie `README.md` — bewust niet in `library.properties`, enkel nodig bij deze keuze). |

| Define | Betekenis |
|---|---|
| `ADS1115_I2C_ADDRESS` | Standaard `0x48`, via H7: ADDR naar GND. Gereserveerd voor Stimulus zodat een eventuele tweede ADS1115 (bv. een toekomstige emotiemeetmodule) op `0x49`/`0x4A` kan zitten zonder conflict. |
| `WACHT_LOSLATEN_DELAY_MS` | Vertraging in de busy-wait-lus van `WachtTotAlleSensorsLosgelatenVoorTest()`. Bij `ADC_BACKEND_NATIVE` blijft dit `0`. Bij `ADC_BACKEND_ADS1115` staat dit op `5` ms, om te vermijden dat de I2C-bus zonder onderbreking bevraagd wordt. |

## UNO_VERSION en ADC-resolutie

```cpp
#define UNO_VERSION   3   // R3=3, R4=4
```

| Combinatie | `ADC_BITS` | `DELAY_US` | Betekenis |
|---|---|---|---|
| `ADC_BACKEND_ADS1115` | 15 | 0 | ADS1115 single-ended: 0-32767 bruikbaar bereik. `DELAY_US` niet van toepassing — de I2C-conversietijd zit in de leesfunctie zelf. |
| `UNO_VERSION == 3` | 10 | 100 | Klassieke 10-bit ADC. |
| `UNO_VERSION == 4` | 14 | 200 | UNO R4 Minima's ingebouwde 14-bit ADC. |

`ADC_MAX` en de `ADC(x)`-macro herschalen elke drempelwaarde (zie `StimulusConfig.md`) naar de actieve resolutie:

```cpp
#define ADC_MAX ((1UL << ADC_BITS) - 1)

#if ADC_BITS == 10
  #define ADC(x) (x)
#elif ADC_BITS == 14 || ADC_BITS == 15
  #define ADC(x) (((x) * ADC_MAX) / 1023L)
#endif
```

Alle drempels in `StimulusConfig.h` zijn geschreven als `ADC(...)`-uitdrukking, gebaseerd op een 10-bit referentie (0-1023). Wijzig nooit een kale drempelwaarde zonder de `ADC(...)`-wrapper — anders klopt de schaling niet meer bij `UNO_VERSION == 4` of `ADC_BACKEND_ADS1115`.

## DEBUG-macro's

```cpp
DEBUG_PRINT(x)       // Serial.print(x), enkel actief als DEBUG gedefinieerd is
DEBUG_PRINTLN(x)     // Serial.println(x)
DEBUG_PRINTLN2(x, f) // Serial.println(x, f) — bv. voor hexadecimale uitvoer
```

Merk op dat `PrintToScreen()` (zie `docs/Systeem/SCREEN.md`) zelf al naar Serial spiegelt wanneer `DEBUG` actief is — een aparte `DEBUG_PRINTLN()` vlak vóór een `PrintToScreen()`-aanroep is dan overbodige dubbele uitvoer.
