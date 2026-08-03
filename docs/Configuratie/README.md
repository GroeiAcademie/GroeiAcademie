# Configuratie

Deze map documenteert de configuratiebestanden uit `src/Configuratie/`.

| Document | Bronbestand | Doel |
|---|---|---|
| [SystemConfig.md](SystemConfig.md) | [`../../src/Configuratie/SystemConfig.h`](../../src/Configuratie/SystemConfig.h) | officiële standaardwaarden en fallbackwerking |
| [UserConfig.md](UserConfig.md) | [`../../src/Configuratie/UserConfig_template.h`](../../src/Configuratie/UserConfig_template.h) | blijvende persoonlijke configuratie met voorrang op de standaardwaarden |
| [UserLanguage.md](UserLanguage.md) | `../../src/Language/UserExample_XX_template.h` en `../../src/Language/UserLibrary_XX_template.h` | blijvende persoonlijke teksten per gekozen taal |
| [StimulusConfig.md](StimulusConfig.md) | [`../../src/Configuratie/StimulusConfig.h`](../../src/Configuratie/StimulusConfig.h) | drempels, marges en timingwaarden voor Stimulus |

Configuratie gebeurt compile-time via `#define`-waarden. Wijzigingen vragen dus een nieuwe compilatie en upload naar het Arduino Uno R3-vormfactorbord.

## Persoonlijke configuratie

Kopieer `UserConfig_template.h` naar `UserConfig.h` in `src/Configuratie/`. Kopieer voor persoonlijke voorbeeldteksten het passende `UserExample_XX_template.h` naar `UserExample_XX.h` en voor persoonlijke libraryteksten het passende `UserLibrary_XX_template.h` naar `UserLibrary_XX.h` in `src/Language/`. De actieve bestanden staan in `.gitignore` en worden vóór de overeenkomstige officiële fallbackwaarden geladen.

De taalbestanden voor de voorbeeldprogramma's behouden hun naam en plaats:

```text
src/Language/Examples_NL.h
src/Language/Examples_DE.h
src/Language/Examples_EN.h
src/Language/Examples_FR.h
```

De wrapper waarmee de voorbeeldprogramma's de gekozen `Examples_XX.h` laden, heet:

```text
src/Configuratie/Examples.h
```

- [ExamplesConfig.h](ExamplesConfig.md) — gedeelde, per script overschrijfbare instellingen voor de examples.
