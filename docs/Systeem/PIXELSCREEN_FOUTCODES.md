# PixelScreen-foutcodes

Wanneer een fatale fout verhindert dat de geselecteerde PixelScreen-uitvoer betrouwbaar kan werken, toont de library een korte foutmelding:

```text
FATAL: PSxxx
ZOEK DIT NU OP
```

Wanneer CharacterScreen actief is, verschijnt deze melding op het CharacterScreen. Wanneer CharacterScreen niet actief is, forceert de library Serial op 115200 baud en verschijnt dezelfde melding in de Serial Monitor. `DEBUG` en `SCREEN_TYPE_SERIAL` zijn daarvoor niet vereist. Wanneer CharacterScreen actief is, wordt Serial voor deze foutmelding niet geforceerd.

De volledige betekenis, controle en oplossing staan hieronder bij de gemelde foutcode.

## Overzicht

| Foutcode | Betekenis |
|---|---|
| [PS001: PixelScreen niet gekoppeld](#ps001) | De algemene `PixelScreen`-pointer verwijst niet naar het geïnitialiseerde schermobject. |
| [PS002: Omgewisselde breedte en hoogte komen niet overeen](#ps002) | Bij rotatie 1 of 3 komen de gemeten afmetingen niet overeen met de omgewisselde ingestelde afmetingen. |
| [PS003: Niet-omgewisselde breedte en hoogte komen niet overeen](#ps003) | Bij rotatie 0 of 2 komen de gemeten afmetingen niet overeen met de ingestelde afmetingen. |
| [PS004: Tekstgrid kleiner dan 16×2](#ps004) | De resolutie en tekstgrootte leveren minder dan 16 kolommen of minder dan 2 regels op. |

<a id="ps001"></a>
## PS001 — PixelScreen niet gekoppeld

### Melding

```text
FATAL: PS001
ZOEK DIT NU OP
```

### Trigger

```cpp
if (!PixelScreen)
```

### Betekenis

PixelScreen werd geselecteerd, maar er is geen geïnitialiseerd schermobject aan de algemene `PixelScreen`-pointer gekoppeld.

### Controle

Controleer of deze stappen na de initialisatie van de concrete displaydriver worden uitgevoerd:

```cpp
pixelScreen.init(...);
pixelScreen.setRotation(...);
PixelScreen = &pixelScreen;
PixelScreenConfigureren();
```

De ontbrekende koppeling is doorgaans:

```cpp
PixelScreen = &pixelScreen;
```

### Gevolg

De library kan het geselecteerde PixelScreen niet aanspreken en schakelt de PixelScreen-uitvoer voor deze configuratie niet betrouwbaar in.

### Oplossing

Initialiseer de concrete displaydriver, stel de gewenste rotatie in, koppel het schermobject aan `PixelScreen` en roep daarna `PixelScreenConfigureren()` aan.

<a id="ps002"></a>
## PS002 — Omgewisselde breedte en hoogte komen niet overeen

### Melding

```text
FATAL: PS002
ZOEK DIT NU OP
```

### Trigger

```cpp
#if (PIXEL_SCREEN_ROTATION == 1 || PIXEL_SCREEN_ROTATION == 3)
if (PixelScreen->width() != ACTIEF_PIXEL_SCREEN_HOOGTE || PixelScreen->height() != ACTIEF_PIXEL_SCREEN_BREEDTE)
```

### Betekenis

Bij rotatie 1 of 3 verwacht de huidige configuratie dat de ingestelde breedte en hoogte omgewisseld worden. De door `PixelScreen->width()` en `PixelScreen->height()` gerapporteerde afmetingen voldoen niet aan die verwachting.

### Controle

Controleer:

- of `ACTIEF_PIXEL_SCREEN` overeenkomt met de werkelijk aangesloten displaymodule;
- of de concrete displaydriver met de juiste fysieke resolutie werd geïnitialiseerd;
- of `PIXEL_SCREEN_ROTATION` werkelijk 1 of 3 moet zijn;
- welke waarden `PixelScreen->width()` en `PixelScreen->height()` na `setRotation()` teruggeven.

Vergelijk:

```cpp
PixelScreen->width()  == ACTIEF_PIXEL_SCREEN_HOOGTE
PixelScreen->height() == ACTIEF_PIXEL_SCREEN_BREEDTE
```

### Gevolg

De library kan de beschikbare schermbreedte, schermhoogte en tekstindeling niet betrouwbaar berekenen.

### Oplossing

Corrigeer het geselecteerde schermtype, de initialisatieafmetingen of de rotatie zodat de door de driver gerapporteerde afmetingen overeenkomen met de verwachte omgewisselde afmetingen.

<a id="ps003"></a>
## PS003 — Niet-omgewisselde breedte en hoogte komen niet overeen

### Melding

```text
FATAL: PS003
ZOEK DIT NU OP
```

### Trigger

```cpp
#if (PIXEL_SCREEN_ROTATION == 0 || PIXEL_SCREEN_ROTATION == 2)
if (PixelScreen->width() != ACTIEF_PIXEL_SCREEN_BREEDTE || PixelScreen->height() != ACTIEF_PIXEL_SCREEN_HOOGTE)
```

### Betekenis

Bij rotatie 0 of 2 verwacht de huidige configuratie dat de ingestelde breedte en hoogte niet omgewisseld worden. De door `PixelScreen->width()` en `PixelScreen->height()` gerapporteerde afmetingen voldoen niet aan die verwachting.

### Controle

Controleer:

- of `ACTIEF_PIXEL_SCREEN` overeenkomt met de werkelijk aangesloten displaymodule;
- of de concrete displaydriver met de juiste fysieke resolutie werd geïnitialiseerd;
- of `PIXEL_SCREEN_ROTATION` werkelijk 0 of 2 moet zijn;
- welke waarden `PixelScreen->width()` en `PixelScreen->height()` na `setRotation()` teruggeven.

Vergelijk:

```cpp
PixelScreen->width()  == ACTIEF_PIXEL_SCREEN_BREEDTE
PixelScreen->height() == ACTIEF_PIXEL_SCREEN_HOOGTE
```

### Gevolg

De library kan de beschikbare schermbreedte, schermhoogte en tekstindeling niet betrouwbaar berekenen.

### Oplossing

Corrigeer het geselecteerde schermtype, de initialisatieafmetingen of de rotatie zodat de door de driver gerapporteerde afmetingen overeenkomen met de verwachte niet-omgewisselde afmetingen.

<a id="ps004"></a>
## PS004 — Tekstgrid kleiner dan 16×2

### Melding

```text
FATAL: PS004
ZOEK DIT NU OP
```

### Trigger

```cpp
if (ruweKolommen < PIXELGRID_MIN_KOLOMMEN || ruweRegels < PIXELGRID_MIN_RIJEN)
```

### Betekenis

De beschikbare PixelScreen-resolutie en de gekozen `PIXEL_SCREEN_TEXT_SIZE` leveren een berekend tekstgrid op dat kleiner is dan de minimale ondersteunde indeling van 16 kolommen bij 2 regels.

### Berekening

```cpp
ruweKolommen = PixelScreen->width()  / (6 * PIXEL_SCREEN_TEXT_SIZE);
ruweRegels   = PixelScreen->height() / (8 * PIXEL_SCREEN_TEXT_SIZE);
```

De minimumvoorwaarden zijn:

```cpp
ruweKolommen >= 16
ruweRegels   >= 2
```

### Controle

Controleer:

- `ACTIEF_PIXEL_SCREEN`;
- `PIXEL_SCREEN_TEXT_SIZE`;
- `PixelScreen->width()`;
- `PixelScreen->height()`;
- of de schermrotatie de beschikbare breedte en hoogte beïnvloedt zoals verwacht.

### Gevolg

De standaard PixelScreen-uitvoer kan de minimale tekstweergave van 16×2 niet garanderen.

### Oplossing

Gebruik een kleinere `PIXEL_SCREEN_TEXT_SIZE`, kies een PixelScreen met een grotere bruikbare resolutie of corrigeer de schermconfiguratie wanneer de gemeten breedte en hoogte niet kloppen.
