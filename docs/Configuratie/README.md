# Configuratie

Deze map documenteert de configuratiebestanden uit `src/Configuratie/`.

| Document | Bronbestand | Doel |
|---|---|---|
| [SystemConfig.md](SystemConfig.md) | [`../../src/Configuratie/SystemConfig.h`](../../src/Configuratie/SystemConfig.h) | algemene systeemkeuzes, scherm, board, ADC-backend en debug |
| [StimulusConfig.md](StimulusConfig.md) | [`../../src/Configuratie/StimulusConfig.h`](../../src/Configuratie/StimulusConfig.h) | drempels, marges en timingwaarden voor Stimulus |


Configuratie gebeurt compile-time via `#define`-waarden. Wijzigingen in deze bestanden vragen dus een nieuwe compilatie en upload naar het Arduino-board.
