# Testresultaten

Dit document bevat de resultaten van alle officiële validaties vóór publicatie van een release.

## Overzicht

| Versie | Datum | Arduino LINT | Compilaties | OK | Verwachte UNO R3-geheugenbeperkingen | Onverwachte fouten | Opmerking |
|--------|-------|:------------:|------------:|---:|------------------------------------:|-------------------:|-----------|
| v1.0.3 | 03-08-2026 | ✅ (overgenomen) | 188 | 184 | 4 | 0 | overgenomen van v1.0.2 — enkel documentatiecorrecties |
| v1.0.2 | 02-08-2026 | ✅ (overgenomen) | 188 | 184 | 4 | 0 | overgenomen; alleen zichtbaarheid gewijzigd |
| v1.0.1 | 01-08-2026 | ✅ (overgenomen) | 188 | 184 | 4 | 0 | overgenomen van v1.0.0 — enkel documentatiecorrecties |
| v1.0.0 | 31-07-2026 | ✅ (1 bekende, onschadelijke waarschuwing) | 188 | 184 | 4 | 0 | eerste officiële validatie |


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

✅ Geslaagd — `0 ERRORS`, `1 WARNING`.

De waarschuwing betreft uitsluitend het automatisch ophalen van de URL uit `library.properties` (regel `LP042`), die door de beveiliging van de website werd geweigerd met `403 Forbidden`. De website zelf is voor gewone bezoekers gewoon bereikbaar; enkel geautomatiseerde verzoeken (zoals deze linter-check) worden geweerd. Dit is een bekende, onschadelijke waarschuwing en blokkeert de release niet.

## Compilatietesten

### Geteste Arduino Uno R3-vormfactorborden

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

Het totaal van 188 volgt rechtstreeks uit de opschoning van de `.ino`-configuratiearchitectuur: enkel de 5 Stimulus-scenario's worden over alle 8 mogelijke `SCREEN_OUTPUT_CONFIG`-waarden getest (`5 × 8 = 40` per Arduino Uno R3-vormfactorbord), de 5 Screen-examples en de 2 `ADC_Validatie`-scripts elk met exact hun eigen, relevante configuratie (`7 × 1 = 7` per Arduino Uno R3-vormfactorbord) — samen `47` per Arduino Uno R3-vormfactorbord, `× 4` Arduino Uno R3-vormfactorborden `= 188`. Minder, gerichtere combinaties in plaats van overbodige herhaling ten opzichte van eerdere testruns.

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
