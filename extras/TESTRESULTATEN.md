# Testresultaten

Dit document bevat de resultaten van alle officiële validaties vóór publicatie van een release.

## Overzicht

| Versie | Datum | Arduino LINT | Compilaties | OK | Verwachte -geheugenbeperkingen | Onverwachte fouten | Opmerking |
|--------|-------|:------------:|------------:|---:|------------------------------------:|-------------------:|-----------|
| v2.0.0 | 05-11-2026 | GESLAAGD | 1530 | 1530 | 0 | 0 | GESLAAGD |

## Ontwikkelstatus en reikwijdte van de metingen

De tabel hierboven bevat een historische meting van 5 november 2026. De 1530 geslaagde compilaties bewijzen niet dat de later gewijzigde drivers en testsketches uit RUN166 met diezelfde testuitvoering zijn gevalideerd. De v2.0.0-architectuur is nog in beta en wordt actief herbouwd en getest. Er wordt **nog geen definitief v2.0.0-releaseresultaat** geclaimd. Historische resultaten van v1.x blijven hieronder staan en worden niet als v2-resultaat hergebruikt.

Actuele validatiefocus:

- GedeeldeBus-lifecycle en resourceconflicten;
- Screen via `GedeeldeBusNewComponent`;
- Input via `GedeeldeBusNewComponent`, zonder publieke `InputConfigureren()`;
- 16 concrete Extenders en hun `ExtenderPins::EP_<pinnaam>`-resourcebereiken;
- de huidige Sensorimplementaties, waaronder FsrRfp via een ADC-Extender en de recent gewijzigde meerfasige I²C-sensorlezingen;
- DEBUG-only `Diagnose()` is als lege virtuele hook aanwezig; concrete Diagnose-tests zijn nog niet geïmplementeerd.

Elektronische functietesten per Extender en volledige praktijktesten met de beschikbare sensoren zijn nog onderdeel van de beta-validatie en worden niet als voltooid voorgesteld.

