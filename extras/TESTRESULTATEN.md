# Testresultaten

Dit document bevat de resultaten van alle officiële validaties vóór publicatie van een release.

## Overzicht

| Versie | Datum | Arduino LINT | Compilaties | OK | Verwachte UNO R3-geheugenbeperkingen | Onverwachte fouten |
|--------|-------|:------------:|------------:|---:|------------------------------------:|-------------------:|
| v1.0.0 | 31-07-2026 | ✅ (1 bekende, onschadelijke waarschuwing) | 188 | 184 | 4 | 0 |
| v0.10.1 | 27-07-2026 | ✅ | 39 | 39 | 0 | 0 |

---

# v1.0.0

**Datum**

31-07-2026

## Arduino LINT

Status

✅ Geslaagd — `0 ERRORS`, `1 WARNING`.

De waarschuwing betreft uitsluitend het automatisch ophalen van de URL uit `library.properties` (regel `LP042`), die door de beveiliging van de website werd geweigerd met `403 Forbidden`. De website zelf is voor gewone bezoekers gewoon bereikbaar; enkel geautomatiseerde verzoeken (zoals deze linter-check) worden geweerd. Dit is een bekende, onschadelijke waarschuwing en blokkeert de release niet.

## Compilatietesten

### Geteste boards

- Arduino UNO R3;
- Arduino UNO R4 Minima;
- Arduino UNO R4 WiFi;
- Wemos D1 R32 (`esp32:esp32:d1_uno32`).

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

Het totaal van 188 volgt rechtstreeks uit de opschoning van de `.ino`-configuratiearchitectuur: enkel de 5 Stimulus-scenario's worden over alle 8 mogelijke `SCREEN_OUTPUT_CONFIG`-waarden getest (`5 × 8 = 40` per board), de 5 Screen-examples en de 2 `ADC_Validatie`-scripts elk met exact hun eigen, relevante configuratie (`7 × 1 = 7` per board) — samen `47` per board, `× 4` borden `= 188`. Minder, gerichtere combinaties in plaats van overbodige herhaling ten opzichte van eerdere testruns.

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

De compiler meldt voor deze vier combinaties dat de `text section` de beschikbare programmaruimte van het board overschrijdt. Dit zijn gekende grenzen van de Arduino UNO R3 en geen onverwachte fouten in de library.

## Conclusie

- ✅ Arduino LINT succesvol uitgevoerd (1 bekende, onschadelijke waarschuwing);
- ✅ 184 compilaties succesvol uitgevoerd;
- ✅ 4 gekende UNO R3-geheugenbeperkingen exact geïdentificeerd;
- ✅ 0 onverwachte compilatiefouten;
- ✅ release gevalideerd.

---

# v0.10.1

**Datum**

27-07-2026

## Arduino LINT

Status

✅ Geslaagd

## Compilatietesten

### Geteste boards

- Arduino UNO R3;
- Arduino UNO R4 Minima;
- Arduino UNO R4 WiFi.

### Resultaat

```text
============================================================
TESTRESULTAAT
============================================================
Totaal getest : 39
OK bevonden   : 39
Mislukt       : 0
============================================================
Alle 39 testen zijn OK bevonden.
============================================================
```

## Conclusie

- ✅ Arduino LINT succesvol uitgevoerd;
- ✅ alle voorbeelden compileren succesvol;
- ✅ geen compilatiefouten gevonden;
- ✅ release gevalideerd.
