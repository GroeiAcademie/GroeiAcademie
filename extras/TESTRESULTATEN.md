# Testresultaten

Dit document bevat de resultaten van alle officiële validaties vóór publicatie van een release.

## Overzicht

| Versie | Datum | Arduino LINT | Compilaties | OK | Verwachte UNO R3-geheugenbeperkingen | Onverwachte fouten | Opmerking |
|--------|-------|:------------:|------------:|---:|------------------------------------:|-------------------:|-----------|
| v2.0.0 | 26-10-2026 | GESLAAGD | 735 | 735 | 0 | 0 | GESLAAGD |


# v2.0.0 beta — actuele ontwikkelstatus

De v2.0.0-architectuur is nog in beta en wordt actief herbouwd en getest. Er wordt **nog geen definitief v2.0.0-releaseresultaat** geclaimd. Historische resultaten van v1.x blijven hieronder staan en worden niet als v2-resultaat hergebruikt.

Actuele validatiefocus:

- GedeeldeBus-lifecycle en resourceconflicten;
- Screen via `GedeeldeBusNewComponent`;
- Input via `GedeeldeBusNewComponent`, zonder publieke `InputConfigureren()`;
- 14 concrete Extenders en hun `ExtenderPins::EP_<pinnaam>`-resourcebereiken;
- RFP602 als eerste Sensor, via de ADS1115-Extender;
- DEBUG-only `Diagnose()` is als lege virtuele hook aanwezig; concrete Diagnose-tests zijn nog niet geïmplementeerd.

Elektronische functietesten per Extender en volledige praktijktesten met de beschikbare sensoren zijn nog onderdeel van de beta-validatie en worden niet als voltooid voorgesteld.

