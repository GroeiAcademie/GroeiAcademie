# Screen-foutcodes — beta v2.0.0

Dit document beschrijft uitsluitend de foutcodes die in de huidige broncode bestaan. De definities staan in `src/Language/Library_NL.h`; de Screen-lifecycle die ze zet staat in `src/Systeem/Screen/Screen.cpp`.

De v2.0.0-Screen-lifecycle gebruikt drie ernstniveaus voor deze codes:

- `ABORT`: `aanmelden()` van een concrete Screen-uitvoer mislukt;
- `FATAL`: `controleren()` mislukt of een vereiste uitvoer blijkt tijdens runtime niet beschikbaar;
- `CRITICAL`: `inpluggen()` of `activeren()` mislukt, of een concrete uitvoer kan technisch niet bruikbaar worden gemaakt.

GedeeldeBus-conflicten kunnen daarnaast hun eigen `GBxxx`-foutcodes genereren. Die behoren niet tot de Screen-codefamilies en staan daarom niet in dit document.

## CharacterScreen

| Code | Ernst | Huidige betekenis |
|---|---|---|
| `CS001` | ABORT | `CharacterScreen::aanmelden()` mislukt. |
| `CS101` | FATAL | `CharacterScreen::controleren()` mislukt zonder dat de fout al als GedeeldeBus-conflict werd gerapporteerd. |
| `CS201` | CRITICAL | `CharacterScreen::inpluggen()` mislukt. |
| `CS301` | CRITICAL | `CharacterScreen::activeren()` / `CharacterScreen::Activeren()` mislukt. In de huidige code controleert `CharacterScreen::Activeren()` daarbij of `I2C_ADDRESS_CHARACTER_SCREEN` op de I2C-bus antwoordt voordat het LCD wordt geïnitialiseerd. |
| `CS401` | FATAL | CharacterScreen is tijdens runtime niet actief/beschikbaar terwijl uitvoer via CharacterScreen nodig is. |

### Bij `CS301`

Controleer in het bijzonder:

- voeding en GND van het I2C-characterscherm;
- SDA en SCL;
- `I2C_ADDRESS_CHARACTER_SCREEN` in de configuratie;
- of het fysieke apparaat op dat adres daadwerkelijk antwoordt.

De huidige beta v2.0.0-code scant niet automatisch naar alternatieve LCD-adressen. De vroegere `CHARACTERSCREEN_I2C_ADRES_MODUS` en foutcode `CS002` behoren tot de historische Screen-architectuur en worden niet meer door de huidige code gebruikt.

## PixelScreen

| Code | Ernst | Huidige betekenis |
|---|---|---|
| `PS001` | ABORT | `PixelScreen::aanmelden()` mislukt. |
| `PS101` | FATAL | `PixelScreen::controleren()` mislukt zonder dat de fout al als GedeeldeBus-conflict werd gerapporteerd. |
| `PS201` | CRITICAL | `PixelScreen::inpluggen()` mislukt. |
| `PS301` | CRITICAL | `PixelScreen::activeren()` / `PixelScreen::Activeren()` mislukt. |
| `PS302` | CRITICAL | PixelScreen is niet gekoppeld aan een bruikbaar `Adafruit_GFX`-object. |
| `PS303` | CRITICAL | Bij `PIXEL_SCREEN_ROTATION` 1 of 3 komen de omgewisselde breedte en hoogte niet overeen met de ingestelde PixelScreen-afmetingen. |
| `PS304` | CRITICAL | Bij `PIXEL_SCREEN_ROTATION` 0 of 2 komen de niet-omgewisselde breedte en hoogte niet overeen met de ingestelde PixelScreen-afmetingen. |
| `PS305` | CRITICAL | Het berekende tekstgrid is kleiner dan `PIXELGRID_MIN_KOLOMMEN × PIXELGRID_MIN_RIJEN`. |
| `PS401` | FATAL | PixelScreen is tijdens runtime niet actief/beschikbaar terwijl uitvoer via PixelScreen nodig is. |

`PixelScreen::inpluggen()` configureert in de huidige code de controlpinnen. `PixelScreen::Activeren()` bevat momenteel nog geen volwaardige elektronische identificatietest van de ST7789; een eerdere ID-proef staat in de broncode uitgecommentarieerd. Een toekomstige DEBUG-`Diagnose()` mag daarom niet als reeds aanwezige PixelScreen-test worden gelezen.

## SerialScreen

| Code | Ernst | Huidige betekenis |
|---|---|---|
| `SS001` | ABORT | `SerialScreen::aanmelden()` mislukt. |
| `SS101` | FATAL | `SerialScreen::controleren()` mislukt zonder dat de fout al als GedeeldeBus-conflict werd gerapporteerd. |
| `SS201` | CRITICAL | `SerialScreen::inpluggen()` mislukt. |
| `SS301` | CRITICAL | `SerialScreen::activeren()` / `SerialScreen::Activeren()` mislukt. |
| `SS302` | CRITICAL | SerialScreen is na `SERIAL_CONNECT_TIMEOUT_MS` niet beschikbaar. |

## Lifecycle en fallback

`Screen` behandelt Character-, Pixel- en Serial-uitvoer als afzonderlijke children. Wanneer één gekozen uitvoer in een lifecyclefase faalt, kan die uitvoer worden verwijderd terwijl andere gekozen uitvoer blijft werken. Een bewaarde Screen-fout wordt, waar mogelijk, op een andere nog werkende uitvoer weergegeven.

Een conflict dat al door GedeeldeBus zelf is gemeld wordt niet nogmaals als een generieke `CS101`, `PS101` of `SS101` gedupliceerd.

## Historische codes

Oudere releases en beslissingsdocumenten kunnen andere Screen-codes of configuratiefuncties vermelden, onder meer `CS000`, `CS002`, `PS000`, `CharacterScreenConfigureren()`, `PixelScreenConfigureren()` en `ScreensConfigureren()`. Die zijn geen onderdeel van de huidige beta v2.0.0-Screen-API. Historische beslissingen blijven bewaard in `docs/DECISION_LOG.md` en historische releases in `CHANGELOG.md`.
