# UserExample_XX.h en UserLibrary_XX.h

Brontemplates voor persoonlijke voorbeeldteksten:

```text
src/Language/UserExample_NL_template.h
src/Language/UserExample_DE_template.h
src/Language/UserExample_EN_template.h
src/Language/UserExample_FR_template.h
```

Brontemplates voor persoonlijke libraryteksten:

```text
src/Language/UserLibrary_NL_template.h
src/Language/UserLibrary_DE_template.h
src/Language/UserLibrary_EN_template.h
src/Language/UserLibrary_FR_template.h
```

Kopieer uitsluitend de templates van de gekozen taal en verwijder `_template` uit de bestandsnaam, bijvoorbeeld:

```text
UserExample_NL_template.h -> UserExample_NL.h
UserLibrary_NL_template.h -> UserLibrary_NL.h
```

De twee officiële taalgroepen blijven afzonderlijk bestaan:

```text
src/Language/Examples_XX.h
src/Language/Library_XX.h
```

- `Examples_XX.h` bevat de standaardteksten voor de voorbeeldprogramma's;
- `Library_XX.h` bevat de standaardteksten voor de librarycode onder `src/`;
- `UserExample_XX.h` bevat persoonlijke aanpassingen voor de voorbeeldprogramma's;
- `UserLibrary_XX.h` bevat persoonlijke aanpassingen voor de librarycode onder `src/`.

De wrapper `src/Configuratie/Examples.h` laadt voor de gekozen taal eerst een eventueel `UserExample_XX.h` en daarna `Examples_XX.h`. De librarycode laadt eerst een eventueel `UserLibrary_XX.h` en daarna `Library_XX.h`.

Iedere officiële tekstdefinitie staat afzonderlijk onder `#ifndef`. Daardoor krijgt een tekst uit het actieve gebruikersbestand voorrang en worden ontbrekende teksten uit het overeenkomstige officiële taalbestand aangevuld.

Je mag in de actieve gebruikersbestanden alle teksten verwijderen die je niet persoonlijk wilt aanpassen.

## Actieve gebruikersbestanden

```text
src/Language/UserExample_NL.h
src/Language/UserExample_DE.h
src/Language/UserExample_EN.h
src/Language/UserExample_FR.h

src/Language/UserLibrary_NL.h
src/Language/UserLibrary_DE.h
src/Language/UserLibrary_EN.h
src/Language/UserLibrary_FR.h
```

Alle actieve bestanden vallen onder deze regels in `.gitignore`:

```text
src/Language/UserExample_*.h
src/Language/UserLibrary_*.h
```

Gebruik uitsluitend de actieve bestanden die overeenkomen met de taalkeuze in `UserConfig.h`. `.gitignore` beschermt de bestanden bij Git-gebruik; verwijder of vervang je de volledige librarymap handmatig, maak dan eerst een reservekopie.

De Duitse, Engelse en Franse officiële bestanden bevatten in deze basisrelease nog dezelfde Nederlandstalige tekstwaarden als de Nederlandse bestanden. De technische taalkeuze en de persoonlijke overrides werken wel per taalbestand; inhoudelijke vertaling van die waarden is een afzonderlijke taak.
