# GroeiAcademie v1.2.0 — Sensoren-kernel (experimentele pilot)

**Status: PRERELEASE / experimenteel.** Dit is geen v1.2.0-releasekandidaat, geen vervanging van v1.1.0, en niets hierin is tot nu toe gecompileerd of op hardware getest. Dit document beschrijft een werkende architectuurpilot, bedoeld om mee te testen en feedback op te geven, niet om in productie te gebruiken.

## 1. Wat dit is

Een nieuwe, optionele laag bovenop de bestaande GroeiAcademie Framework-library (v1.1.0): een **Sensoren-kernel** die sensoren via een uniform plug-in-contract laat inpluggen, naar analogie van hoe het bestaande INPUT-systeem dat al voor toetsenborden doet. Doel: nieuwe sensoren toevoegen zonder telkens een nieuw, apart stuk toepassingscode te schrijven.

De volledige architecturale motivatie, alle ontwerpbeslissingen, en de technische hercontrole staan in `Kernel_Sensorlaag_Architectuur_Brainstorming_FASE_1_v1_2_0_03.md`. Dit document is bewust kort en naar buiten gericht; voor het volledige verhaal, zie die MD.

## 2. Wat al werkt (architectuur, geschreven code)

- **Kernel**: registry, scheduler, historiek per sensorinstantie, gedeelde bus-/RAM-zuinige aanpak, transactionele registratie.
- **Acht pilot-drivers**: Pulse Sensor, FSR402/RFP602 (volledig geïntegreerd in de bestaande Stimulus-toepassing), BioAmp EXG Pill, DS18B20, MAX30102, NeuroSky TGAM, INMP441, Piëzo-ademhalingsband.
- **Bestemming-module**: stuurt sensordata weg naar Serial (werkend) of naar een tweede bord via UART (minimale pilot).
- **Ontvangst-module**: minimale pilot om data van een ander bord te ontvangen.
- **Elf testsketches**, elk apart compileerbaar, onder `examples/Systeem/Sensoren/`.

## 3. Wat expliciet nog niet werkt of getest is

- **Niets is gecompileerd op `arduino-cli` of echte hardware.** Dit is de belangrijkste beperking van dit hele document.
- UNO R3 ondersteunt enkel de eenvoudige, analoge sensoren (Pulse Sensor, FSR402/RFP602, BioAmp EXG Pill, Piëzo-ademhalingsband). De overige vier pilot-drivers vereisen minimaal UNO R4, en INMP441 vereist specifiek een ESP32-bord.
- DS18B20 ondersteunt nog geen meerdere sensoren op dezelfde 1-Wire-bus.
- NeuroSky TGAM mist nog checksumvalidatie.
- INMP441 is ESP32-specifiek gecodeerd; op RP2040/STM32 geeft dit een onduidelijke compileerfout in plaats van een nette melding.
- Bestemmingen naar SD-kaart, WiFi/MQTT of Bluetooth bestaan nog niet.
- Het netwerk-/multi-persoon-concept (meerdere borden, één centrale aggregator) is enkel als architecturale visie en minimale communicatiepilot uitgewerkt, niet als volwaardige oplossing (geen `NodeId`, geen tijdssynchronisatie tussen borden).

Volledige, geprioriteerde lijst van openstaande punten: zie DEEL 22 van de architectuur-MD.

## 4. Hoe te installeren

1. Download de ZIP van de `experimenteel/v1.2.0-sensoren`-branch (niet `main`).
2. In de Arduino IDE: `Sketch → Include Library → Add .ZIP Library...`, en selecteer de gedownloade ZIP.
3. Deze versie verschijnt **niet** in de reguliere Arduino Library Manager: dat is bewust, om te vermijden dat deze experimentele code onbedoeld breed verspreid raakt.

## 5. Hoe feedback of bugs melden

Meld bevindingen via de gebruikelijke kanalen van het project (zie `CONTRIBUTING.md` in de hoofdrepository). Vermeld daarbij minstens: welke sketch, welk board, en wat je verwachtte versus wat er werkelijk gebeurde (compileerfout, verkeerd gedrag, of iets anders).

## 6. Voor bijdragers: waar we graag over meedenken

Deze pilot bevat een aantal architecturale vragen die bewust nog open staan. Wie hierover wil meedenken, is welkom via een GitHub Discussion (zie `CONTRIBUTING.md`). Concreet, in volgorde van hoe dringend we ze zelf inschatten (volledige onderbouwing: zie DEEL 22 van het brainstormdocument):

- `SENSOR_COMPONENT_*`: hoe koppel je een sensor-instantie ondubbelzinnig aan zijn fysieke pin/adres? (gedeeltelijk al opgelost, DEEL 24)
- Gedeelde bus-eigendom tussen meerdere subsystemen (I²C/SPI) — zie de v1.1.1-patch die hieruit is voortgekomen (`Systeem/GedeeldeBus/`)
- DS18B20 met meerdere sensoren op dezelfde 1-Wire-bus
- Meerdere borden die naar één centrale plek communiceren: welk NodeId-concept, welke tijdsbasis over borden heen? (DEEL 20)

Volledige technische achtergrond: `Kernel_Sensorlaag_Architectuur_Brainstorming_FASE_1_v1_2_0_03.md` in de experimentele branch.


