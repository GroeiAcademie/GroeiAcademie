# Screen-foutcodes

Wanneer een fatale fout verhindert dat de geselecteerde CharacterScreen- of PixelScreen-uitvoer betrouwbaar kan werken, toont de library een korte foutmelding:

```text
FATAL: CSxxx
ZOEK DIT NU OP
```

of

```text
FATAL: PSxxx
ZOEK DIT NU OP
```

Voor een mislukte normale SerialScreen-verbinding gebruikt de library daarnaast:

```text
CRITICAL: SS001
ZOEK DIT NU OP
```

De twee schermtypes vallen wederzijds op elkaar terug: een PixelScreen-fout verschijnt op het CharacterScreen wanneer dat correct geconfigureerd en actief is; een CharacterScreen-fout verschijnt op het PixelScreen wanneer dát correct geconfigureerd en actief is. Is geen van beide beschikbaar, dan forceert de library Serial op 115200 baud en verschijnt dezelfde melding in de Serial Monitor. `DEBUG` en `SCREEN_TYPE_SERIAL` zijn daarvoor niet vereist. Is het andere schermtype wel beschikbaar, dan wordt Serial voor deze foutmelding niet geforceerd. Geen van beide foutmeldingsfuncties roept zelf een configuratiefunctie aan: dat voorkomt een oneindige lus (zie `docs/DECISION_LOG.md`, D022).

De volledige betekenis, controle en oplossing staan hieronder bij de gemelde foutcode.

## Overzicht

| Foutcode | Betekenis |
|---|---|
| [CS000: Screen.Inpluggen() niet aangeroepen](#cs000) | De ingebouwde characterschermhardware is niet expliciet geconfigureerd vóór de eerste `Screen.Print()`-aanroep. |
| [CS001: geen I2C-antwoord](#cs001) | Geen enkel geprobeerd I2C-adres reageerde op de handdruk-check. |
| [CS002: scherm gevonden op ander I2C-adres](#cs002) | Wordt door de huidige Screen-code niet meer uitgegeven; automatische I2C-adresmodi zijn verwijderd. |
| [PS000: Screen.Inpluggen() niet aangeroepen](#ps000) | De ingebouwde pixelschermhardware is niet expliciet geconfigureerd vóór de eerste `Screen.Print()`-aanroep. |
| [PS001: PixelScreen niet gekoppeld](#ps001) | `Screen.Pixel.gfx` verwijst niet naar het geïnitialiseerde `Screen.Pixel.display`-object. |
| [PS002: Omgewisselde breedte en hoogte komen niet overeen](#ps002) | Bij rotatie 1 of 3 komen de gemeten afmetingen niet overeen met de omgewisselde ingestelde afmetingen. |
| [PS003: Niet-omgewisselde breedte en hoogte komen niet overeen](#ps003) | Bij rotatie 0 of 2 komen de gemeten afmetingen niet overeen met de ingestelde afmetingen. |
| [PS004: Tekstgrid kleiner dan 16×2](#ps004) | De resolutie en tekstgrootte leveren minder dan 16 kolommen of minder dan 2 regels op. |
| [SS001: SerialScreen niet beschikbaar na timeout](#ss001) | De eerste normale SerialScreen-verbindingspoging werd niet binnen `SERIAL_CONNECT_TIMEOUT_MS` beschikbaar. |

<a id="cs000"></a>
## CS000: Screen.Inpluggen() niet aangeroepen

### Melding

```text
FATAL: CS000
ZOEK DIT NU OP
```

### Trigger

```cpp
if (Screen.Character.gedeeldeBus == nullptr)
```

### Betekenis

`Screen.Print()` werd aangeroepen terwijl `Screen.Inpluggen()` nog niet succesvol werd uitgevoerd voor CharacterScreen-uitvoer.

### Controle

Controleer of `setup()` één van deze aanroepen bevat:

```cpp
Screen.Aanmelden();
Screen.Controleren();
Screen.Inpluggen();
```

of

```cpp
Screen.Aanmelden();
Screen.Controleren();
Screen.Inpluggen();
```

### Gevolg

De library kan niet garanderen dat het characterscherm werkt, en toont daarom geen normale uitvoer via CharacterScreen totdat dit is opgelost.

### Oplossing

Doorloop in `setup()` eerst `Screen.Aanmelden()`, `Screen.Controleren()` en `Screen.Inpluggen()`, vóór de eerste `Screen.Print()`-aanroep.

<a id="cs001"></a>
## CS001: Geen I2C-antwoord

### Melding

```text
FATAL: CS001
ZOEK DIT NU OP
```

### Trigger

```cpp
Wire.beginTransmission(...); 
if (Wire.endTransmission() != 0)
```

### Betekenis

Het geconfigureerde `I2C_ADRES` gaf geen ACK terug. `CharacterScreen::Activeren()` controleert uitsluitend dit aangemelde adres.

### Controle

Controleer:

- de bekabeling (SDA/SCL, voeding, GND);
- of het scherm effectief een I2C-backpack heeft (geen rechtstreekse parallelle LCD);
- `I2C_ADRES` in `UserConfig.h`;
- eventueel met een apart I2C-scannerschetsje welk adres het scherm werkelijk gebruikt.

### Gevolg

De library kan het characterscherm niet aanspreken en schakelt de CharacterScreen-uitvoer voor deze configuratie niet betrouwbaar in.

### Oplossing

Herstel de I2C-verbinding of pas `I2C_ADRES` aan naar het werkelijke adres van het CharacterScreen.

<a id="cs002"></a>
## CS002: Scherm gevonden op ander I2C-adres

### Melding

```text
FATAL: CS002 0xNN
ZOEK DIT NU OP
```

`0xNN` is het effectief gevonden adres.

### Trigger

De huidige code gebruikt geen automatische I2C-adresmodus meer en geeft `CS002` niet meer uit.

### Betekenis

Het scherm werkt, maar niet op het adres dat in `UserConfig.h` staat.

### Controle

Vergelijk het gemelde `0xNN` met de huidige `I2C_ADRES`-instelling.

### Gevolg

Geen zelfherstel in modus 1: de configuratie wordt als mislukt beschouwd totdat `I2C_ADRES` overeenstemt met het werkelijke adres.

### Oplossing

Pas `I2C_ADRES` aan naar het gemelde adres en hercompileer. Overweeg modus `2` wanneer je liever geen hercompilatie wil bij een wisselend adres (zie de afweging in `docs/DECISION_LOG.md`, D023).

<a id="ps000"></a>
## PS000: Screen.Inpluggen() niet aangeroepen

### Melding

```text
FATAL: PS000
ZOEK DIT NU OP
```

### Trigger

```cpp
if (Screen.Pixel.gedeeldeBus == nullptr)
```

### Betekenis

`Screen.Print()` werd aangeroepen terwijl `Screen.Inpluggen()` nog niet succesvol werd uitgevoerd voor PixelScreen-uitvoer.

### Controle

Controleer of `setup()` de PixelScreen-configuratie rechtstreeks of via de volledige Screen-setup aanroept:

```cpp
Screen.Aanmelden();
Screen.Controleren();
Screen.Inpluggen();
```

of

```cpp
Screen.Aanmelden();
Screen.Controleren();
Screen.Inpluggen();
```

### Gevolg

De library kan niet garanderen dat het pixelscherm werkt, en toont daarom geen normale uitvoer via PixelScreen totdat dit is opgelost.

### Oplossing

Doorloop in `setup()` eerst `Screen.Aanmelden()`, `Screen.Controleren()` en `Screen.Inpluggen()`, vóór de eerste `Screen.Print()`-aanroep. De koppeling van `Screen.Pixel.gfx` aan `Screen.Pixel.display` gebeurt intern tijdens `Screen.Inpluggen()`.

<a id="ps001"></a>
## PS001: PixelScreen niet gekoppeld

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

PixelScreen werd geselecteerd, maar `Screen.Pixel.gfx` verwijst niet naar het interne `Screen.Pixel.display`-object.

### Controle

Controleer of de normale v2.0.0-PixelScreen-configuratie wordt gebruikt:

```cpp
Screen.Aanmelden();
Screen.Controleren();
Screen.Inpluggen();
```

of:

```cpp
Screen.Aanmelden();
Screen.Controleren();
Screen.Inpluggen();
```

Na een geslaagde GedeeldeBus-inplug voert `Screen` intern de initialisatie, rotatie en koppeling uit.

### Gevolg

De library kan het geselecteerde PixelScreen niet aanspreken en schakelt de PixelScreen-uitvoer voor deze configuratie niet betrouwbaar in.

### Oplossing

Doorloop `Screen.Aanmelden()`, `Screen.Controleren()` en `Screen.Inpluggen()`. `Screen::Inpluggen()` initialiseert `Screen.Pixel.display`, stelt `PIXEL_SCREEN_ROTATION` in en koppelt `Screen.Pixel.gfx` aan dat object.

<a id="ps002"></a>
## PS002: Omgewisselde breedte en hoogte komen niet overeen

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

Bij rotatie 1 of 3 verwacht de huidige configuratie dat de ingestelde breedte en hoogte omgewisseld worden. De door `Screen.Pixel.gfx->width()` en `Screen.Pixel.gfx->height()` gerapporteerde afmetingen voldoen niet aan die verwachting.

### Controle

Controleer:

- of `ACTIEF_PIXEL_SCREEN` overeenkomt met de werkelijk aangesloten displaymodule;
- of de concrete displaydriver met de juiste fysieke resolutie werd geïnitialiseerd;
- of `PIXEL_SCREEN_ROTATION` werkelijk 1 of 3 moet zijn;
- welke waarden `Screen.Pixel.gfx->width()` en `Screen.Pixel.gfx->height()` na `setRotation()` teruggeven.

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
## PS003: Niet-omgewisselde breedte en hoogte komen niet overeen

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

Bij rotatie 0 of 2 verwacht de huidige configuratie dat de ingestelde breedte en hoogte niet omgewisseld worden. De door `Screen.Pixel.gfx->width()` en `Screen.Pixel.gfx->height()` gerapporteerde afmetingen voldoen niet aan die verwachting.

### Controle

Controleer:

- of `ACTIEF_PIXEL_SCREEN` overeenkomt met de werkelijk aangesloten displaymodule;
- of de concrete displaydriver met de juiste fysieke resolutie werd geïnitialiseerd;
- of `PIXEL_SCREEN_ROTATION` werkelijk 0 of 2 moet zijn;
- welke waarden `Screen.Pixel.gfx->width()` en `Screen.Pixel.gfx->height()` na `setRotation()` teruggeven.

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
## PS004: Tekstgrid kleiner dan 16×2

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

De beschikbare PixelScreen-resolutie, buitenmarge, tekstgrootte en witruimte leveren een berekend tekstgrid op dat kleiner is dan de minimale ondersteunde indeling van 16 kolommen bij 2 regels.

### Berekening

```cpp
bruikbareBreedte = PixelScreen->width() - (2 * PIXEL_SCREEN_MARGIN);
bruikbareHoogte = PixelScreen->height() - (2 * PIXEL_SCREEN_MARGIN);
ruweKolommen = (bruikbareBreedte + PIXEL_SCREEN_CHARACTER_SPACING) / PixelScreenKarakterStap();
ruweRegels = (bruikbareHoogte + PIXEL_SCREEN_LINE_SPACING) / PixelScreenRegelStap();
```

`PixelScreenKarakterStap()` combineert de vaste karakterbreedte, `PIXEL_SCREEN_TEXT_SIZE` en `PIXEL_SCREEN_CHARACTER_SPACING`. `PixelScreenRegelStap()` combineert de vaste karakterhoogte, `PIXEL_SCREEN_TEXT_SIZE` en `PIXEL_SCREEN_LINE_SPACING`.

De minimumvoorwaarden zijn:

```cpp
ruweKolommen >= 16
ruweRegels   >= 2
```

### Controle

Controleer:

- `ACTIEF_PIXEL_SCREEN`;
- `PIXEL_SCREEN_TEXT_SIZE`;
- `PIXEL_SCREEN_MARGIN`;
- `PIXEL_SCREEN_CHARACTER_SPACING`;
- `PIXEL_SCREEN_LINE_SPACING`;
- `Screen.Pixel.gfx->width()`;
- `Screen.Pixel.gfx->height()`;
- of de schermrotatie de beschikbare breedte en hoogte beïnvloedt zoals verwacht.

### Gevolg

De standaard PixelScreen-uitvoer kan de minimale tekstweergave van 16×2 niet garanderen.

### Oplossing

Gebruik een kleinere `PIXEL_SCREEN_TEXT_SIZE`, verklein `PIXEL_SCREEN_MARGIN`, `PIXEL_SCREEN_CHARACTER_SPACING` of `PIXEL_SCREEN_LINE_SPACING`, kies een PixelScreen met een grotere bruikbare resolutie of corrigeer de schermconfiguratie wanneer de gemeten breedte en hoogte niet kloppen.
<a id="ss001"></a>
## SS001: SerialScreen niet beschikbaar na timeout

### Melding

```text
CRITICAL: SS001
ZOEK DIT NU OP
```

### Trigger

De eerste normale `Screen.Inpluggen()`-poging heeft `GA_SERIAL.begin(SERIAL_BAUDRATE)` uitgevoerd, maar de seriële interface werd niet beschikbaar binnen `SERIAL_CONNECT_TIMEOUT_MS`.

### Gedrag

De library onthoudt de mislukte poging zodat volgende `Screen.Print()`-aanroepen niet telkens opnieuw de volledige timeout doorlopen. Een reeds werkend CharacterScreen en/of PixelScreen, of hun geregistreerde callback, krijgt de `CRITICAL: SS001`-melding rechtstreeks; het mislukte Serial-pad wordt daarvoor niet opnieuw gebruikt. De toepassing kan daarna verdergaan via de overige beschikbare uitvoerkanalen.

### Controle

Controleer de seriële verbinding en, indien nodig, `SERIAL_BAUDRATE` en `SERIAL_CONNECT_TIMEOUT_MS` in `UserConfig.h`.

