# Beveiligingsbeleid

## Ondersteunde versies

Zolang GroeiAcademie Framework zich in actieve, pre-1.0-ontwikkeling bevindt, wordt enkel de meest recente release ondersteund met beveiligingscorrecties.

## Wat hier gemeld wordt

Dit beleid dekt kwetsbaarheden in de **software** van het framework: bijvoorbeeld fouten die tot onbetrouwbare metingen, onverwacht gedrag, of — bij toekomstige netwerkverbonden platformen zoals ESP32 — onveilige communicatie kunnen leiden.

Voor **hardwareveiligheid** (bedrading, voeding, sensorplaatsing) verwijzen we naar `DISCLAIMER.md`. Voor gewone bugs zonder veiligheids- of privacy-impact: gebruik gewoon een publieke issue, zie `CONTRIBUTING.md`.

## Een kwetsbaarheid melden

Meld kwetsbaarheden met mogelijke veiligheids- of privacy-impact **niet** via een publieke issue, maar rechtstreeks via **info@groeiacademie.be**, met:

- een omschrijving van het probleem en de mogelijke impact
- stappen om het te reproduceren
- de betrokken versie(s), board(s) en configuratie

## Wat je mag verwachten

- Ontvangstbevestiging binnen een redelijke termijn
- Openheid over de voortgang van het onderzoek
- Vermelding in `CHANGELOG.md` en, indien gewenst, in `AUTHORS.md` na oplossing, tenzij je anoniem wil blijven

## Toekomstige netwerkverbonden platformen

Zodra ESP32 of andere netwerkverbonden boards officieel ondersteund worden (zie `docs/HARDWARE_SUPPORT.md`), wordt dit beleid uitgebreid met richtlijnen specifiek voor netwerkcommunicatie en data-opslag.
