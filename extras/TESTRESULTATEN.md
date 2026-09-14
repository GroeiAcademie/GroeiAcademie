# Testresultaten

Dit document bevat de resultaten van alle officiële validaties vóór publicatie van een release.

## Overzicht

| Versie | Datum | Arduino LINT | Compilaties | OK | Verwachte UNO R3-geheugenbeperkingen | Onverwachte fouten | Opmerking |
|--------|-------|:------------:|------------:|---:|------------------------------------:|-------------------:|-----------|
| v1.1.2 | nog uit te voeren | NOG UIT TE VOEREN | - | - | - | - | validatie van de v1.1.2-wijzigingen nog niet uitgevoerd |
| v1.1.1 | 25-08-2026 | GESLAAGD | 359 | 358 | 1 | 0 | GESLAAGD |
| v1.1.0 | 21-08-2026 | GESLAAGD | 460 | 456 | 4 | 0 | GESLAAGD |
| v1.0.4 | 04-08-2026 | ✅ (Alle onverwachte controles zijn OK bevonden.) | 188 | 184 | 4 | 0 | GESLAAGD |
| v1.0.3 | 03-08-2026 | ✅ (overgenomen) | 188 | 184 | 4 | 0 | overgenomen van v1.0.2: enkel documentatiecorrecties |
| v1.0.2 | 02-08-2026 | ✅ (overgenomen) | 188 | 184 | 4 | 0 | overgenomen; alleen zichtbaarheid gewijzigd |
| v1.0.1 | 01-08-2026 | ✅ (overgenomen) | 188 | 184 | 4 | 0 | overgenomen van v1.0.0: enkel documentatiecorrecties |
| v1.0.0 | 31-07-2026 | ✅ (1 bekende, onschadelijke waarschuwing) | 188 | 184 | 4 | 0 | eerste officiële validatie |


# v1.1.2

De broncode, testscripts en documentatie zijn voor v1.1.2 bijgewerkt. De officiële releasevalidatie is in dit pakket nog niet opnieuw uitgevoerd; daarom worden geen v1.1.1-compilatieresultaten als v1.1.2-resultaat overgenomen. Voer vóór publicatie de gebruikelijke releasecycli en Arduino LINT uit en vul daarna bovenstaande overzichtsregel aan.

## Bevestigde hardwarestatus

De volgende fysieke boardtests zijn bevestigd en staan los van de nog uit te voeren volledige v1.1.2-releasecompilatie:

- Arduino UNO R3: **geïmplementeerd en getest**;
- Arduino UNO R4 Minima: **geïmplementeerd en getest**;
- Arduino UNO R4 WiFi: **geïmplementeerd en getest**;
- Arduino UNO Q: **geïmplementeerd en getest**;
- Cytron Maker UNO RP2040: **geïmplementeerd en getest**;
- Paradisetronic ESP32-S3 UNO (ESP32-S3-WROOM-1): **geïmplementeerd en getest**;
- SB Components Ardi-32 (ESP32-S3-WROOM-1): **geïmplementeerd en getest**;
- STM32F4 Nucleo-F401RE: **geïmplementeerd en getest**;
- TTGO D1 R32 (ESP32-WROOM-32U): **geïmplementeerd en getest**;
- WeMos D1 R32 (ESP32-WROOM-32U): **geïmplementeerd en getest**.

Paradisetronic ESP32-S3 UNO (ESP32-S3-WROOM-1) is op echte hardware werkend bevestigd met de Arduino IDE-instellingen die in `docs/HARDWARE_SUPPORT.md` zijn vastgelegd. De seriële route gebruikt op `BOARD_ESP32S3_ARDI32` en `BOARD_ESP32S3_DEV` `GA_SERIAL` → `Serial0`.

In de dependencycontrole delen WeMos D1 R32 (ESP32-WROOM-32U) en TTGO D1 R32 (ESP32-WROOM-32U) de technische aanduiding `ESP32-WROOM-32U`.


# v1.1.1

## Handmatige HX1838-hardwarevalidatie

Voor v1.1.1 is de released HX1838-route met `HX1838_BRON_CODES_DEFINE` op de geteste hardwareopstelling werkend bevestigd:

- `HX1838_BRON_CODES = HX1838_BRON_CODES_DEFINE`;
- `HX1838_ONTVANGER_PIN = ARDUINO_UNO_SHIELD_PIN_D12`;
- `HX1838_USE_TINYIRRECEIVER_INSTEAD_OF_IRREMOTE = 1`: TinyIRReceiver: **WERKT**;
- `HX1838_USE_TINYIRRECEIVER_INSTEAD_OF_IRREMOTE = 0`: IRremote: **WERKT**.

De EEPROM-gebaseerde HX1838-routes blijven in v1.1.1 experimenteel.

De volledige v1.1.1-releasevalidatie met de vier compilecycli is op dit moment nog niet als definitief releaseresultaat in de overzichtstabel opgenomen. De totaalaantallen worden pas na uitvoering van de bijgewerkte testmatrix vastgelegd.

## Arduino LINT

Status

✅ Geslaagd: alle onverwachte controles zijn OK bevonden.


## Compilatietesten

- totaal: 359
- OK: 358
- verwachte UNO R3-geheugenbeperkingen: 1
- onverwachte fouten: 0

**Eindstatus: GESLAAGD**

# v1.1.0

## Arduino LINT

Status

✅ Geslaagd: alle onverwachte controles zijn OK bevonden.

## Compilatietesten

- totaal: 460
- OK: 456
- verwachte UNO R3-geheugenbeperkingen: 4
- onverwachte fouten: 0

**Eindstatus: GESLAAGD**

# v1.0.4

## Arduino LINT

Status

**Datum**

04-08-2026

## Arduino LINT

Status

✅ Geslaagd: alle onverwachte controles zijn OK bevonden.

## Compilatietesten

- totaal: 188
- OK: 184
- verwachte UNO R3-geheugenbeperkingen: 4
- onverwachte fouten: 0

**Eindstatus: GESLAAGD**

---


# v1.0.3

v1.0.3 bevat uitsluitend documentatiecorrecties en gebruikt ongewijzigd de codebasis van v1.0.2.

---


# v1.0.2

v1.0.2 wijzigt uitsluitend de zichtbaarheid van twaalf interne hulpfuncties. De functionele implementatie bleef ongewijzigd. De bestaande compilatie- en testresultaten van v1.0.0 bleven van toepassing. Dit was een bewuste, eenmalige correctie van het te ruime v1.0.0-API-oppervlak, uitgevoerd terwijl er nog geen gekende externe gebruikers waren (zie `docs/DECISION_LOG.md`, D020).

---

# v1.0.1

v1.0.1 bevat uitsluitend documentatiecorrecties en gebruikt ongewijzigd dezelfde codebasis en testresultaten als v1.0.0.

---

# v1.0.0

**Datum**

31-07-2026

## Arduino LINT

Status

✅ Geslaagd: `0 ERRORS`, `1 WARNING`.

De waarschuwing betreft uitsluitend het automatisch ophalen van de URL uit `library.properties` (regel `LP042`), die door de beveiliging van de website werd geweigerd met `403 Forbidden`. De website zelf is voor gewone bezoekers gewoon bereikbaar; enkel geautomatiseerde verzoeken (zoals deze linter-check) worden geweerd. Dit is een bekende, onschadelijke waarschuwing en blokkeert de release niet.

## Compilatietesten

### Geteste Arduino Uno R3-vormfactorborden

- Arduino UNO R3;
- Arduino UNO R4 Minima;
- Arduino UNO R4 WiFi;
- WeMos D1 R32 (ESP32-WROOM-32U) (`esp32:esp32:d1_uno32`).

### Resultaat

```text
============================================================
TESTRESULTAAT
============================================================
Totaal getest                         : 188
OK bevonden                           : 184
Verwachte UNO R3-geheugenbeperkingen  : 4
Onverwacht mislukt                    : 0
Arduino LINT                          : GESLAAGD
============================================================
Alle onverwachte controles zijn OK bevonden.
4 gekende UNO R3-geheugenbeperkingen zijn afzonderlijk geregistreerd.
============================================================
```

Het totaal van 188 volgt rechtstreeks uit de opschoning van de `.ino`-configuratiearchitectuur: enkel de 5 Stimulus-scenario's worden over alle 8 mogelijke `SCREEN_OUTPUT_CONFIG`-waarden getest (`5 × 8 = 40` per Arduino Uno R3-vormfactorbord), de 5 Screen-examples en de 2 `ADC_Backend`-scripts elk met exact hun eigen, relevante configuratie (`7 × 1 = 7` per Arduino Uno R3-vormfactorbord): samen `47` per Arduino Uno R3-vormfactorbord, `× 4` Arduino Uno R3-vormfactorborden `= 188`. Minder, gerichtere combinaties in plaats van overbodige herhaling ten opzichte van eerdere testruns.

### Vier verwachte UNO R3-geheugenbeperkingen

Alle vier horen bij:

```text
examples/Toepassingsgebieden/Stimulus/Tik_Enkele_Samen_Instortend_Cocktail
```

met Arduino UNO R3 en:

```text
SCREEN_OUTPUT_CONFIG=4
SCREEN_OUTPUT_CONFIG=5
SCREEN_OUTPUT_CONFIG=6
SCREEN_OUTPUT_CONFIG=7
```

De compiler meldt voor deze vier combinaties dat de `text section` de beschikbare programmaruimte van het Arduino Uno R3-vormfactorbord overschrijdt. Dit zijn gekende grenzen van de Arduino UNO R3 en geen onverwachte fouten in de library.

## Conclusie

- ✅ Arduino LINT succesvol uitgevoerd (1 bekende, onschadelijke waarschuwing);
- ✅ 184 compilaties succesvol uitgevoerd;
- ✅ 4 gekende UNO R3-geheugenbeperkingen exact geïdentificeerd;
- ✅ 0 onverwachte compilatiefouten;
- ✅ release gevalideerd.
