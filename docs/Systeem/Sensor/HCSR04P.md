# HCSR04P

## Sensor en gedeelde implementatie

Fysiek HC-SR04P-type. Componentcode `HCSR04P` (0x91), klasse `HSR0x` met TRIG en ECHO. Voedings-/ECHO-niveaus van deze variant afzonderlijk verifiëren.

Deze sensor deelt een C++-klasse met andere uitvoeringen. De fysieke aansluitgegevens gelden alleen voor `HCSR04P` waar dat uitdrukkelijk vermeld is.

---

## Gedeelde C++-klasse `src/Systeem/Sensor/HSR0x.h`

### HSR0x

## Implementatie

- Broncode: `src/Systeem/Sensor/HSR0x.h` en `HSR0x.cpp`.
- C++-component: `HSR0x`, afgeleid van `Sensor`.
- Parent: `GedeeldeBusNode*` in de constructor.
- Componentcode: `HCSR04` (0x90), `HCSR04P` (0x91), `HYSRF05` (0x92).
- Configuratie: `src/Configuratie/Sensoren/ConfigHCSR04_template.h`, `src/Configuratie/Sensoren/ConfigHCSR04P_template.h`, `src/Configuratie/Sensoren/ConfigHYSRF05_template.h`; de optionele persoonlijke versie heet telkens zonder `_template`.
- Data-kanalen: 1.

## Uit de broncode

HC-SR04 en HC-SR04P: zelfde TRIG/ECHO-klasse. De componentcode identificeert de fysieke variant zonder een tweede klasse.

## Aansluiten

**Aansluitwijze in de bestaande FrameWork-code:**

- De signaalverbindingen worden hieronder per onderdeel gespecificeerd waar ze uit de bestaande componentinterface blijken.
- Controleer voedingsspanning, pinvolgorde en externe stroombegrenzing bij de concrete hardware-uitvoering voordat je spanning aanlegt.

- TRIG → de native Arduino-uitgang `np_TRIG`; ECHO → de native Arduino-ingang `np_ECHO`.
- Kies een interruptgeschikte ECHO-pin als niet-blokkerende pulsmeting nodig is; anders gebruikt de code `pulseIn()`.
- **Controleer ECHO-spanningsniveau** van de concrete HC-SR04/HC-SR04P/HY-SRF05-module tegen de maximale ingangsspanning van het board; waar nodig een niveauaanpassing gebruiken.

**Voor de hardwaretest:**

- Gebruik de bestaande `.ino` uit **Testen** en de bestaande configuratiebestanden onder `src/Configuratie/`; een `*_template.h` wordt niet rechtstreeks ingelezen.
- **Een vaste Arduino-pintoewijzing is niet algemeen geldig:** de gekozen constructor, configuratie en werkelijke bekabeling moeten overeenkomen. Waar hierboven geen exact spanningsniveau of pinheader is bevestigd, mag dat niet uit deze documentatie worden afgeleid.


**Aanvulling aansluitpinnen en elektrische voorwaarden:**

- Bij HC-SR04/HC-SR04P/HY-SRF05: TRIG→digitale uitgang, ECHO→digitale ingang, GND→GND en VCC→modulevoeding. **Let op:** ECHO kan bij 5V-gevoede varianten 5V uitsturen; een 3,3V-controller vereist dan een geschikte spanningsaanpassing. De HC-SR04P-variant en SRF05-MODE moeten afzonderlijk aan hun modulehandleiding worden getoetst.
- De concrete modulevariant bepaalt de **exacte voedingsgrenzen en fysieke pinvolgorde**. Neem die gegevens niet over van een andere module zonder de werkelijke print te controleren.

**Aansluit- en gebruiksgegevens uit de bestaande FrameWork-interface:**

- **TRIG** van de HC-SR04P → `np_TRIG`; **ECHO** → `np_ECHO`; beide zijn native digitale pinnen. Controleer de exacte elektrische variant voor de uitgangsspanning van ECHO.
- De gedeelde `HSR0x`-implementatie gebruikt `np_TRIG` en `np_ECHO` als afzonderlijke native digitale lijnen. De componentcode identificeert de HC-SR04P-variant.
- Controleer voor de concrete HC-SR04P de modulevoeding en het ECHO-niveau; neem de 5V-eigenschappen van HC-SR04 niet automatisch over voor elke P-module.

**Bestaande constructor (parameter-/pinvolgorde):**

```cpp
HSR0x(GedeeldeBusNode* parent, GedeeldeBusComponent component, HardwareResourcePin np_TRIG, HardwareResourcePin np_ECHO)
```

**Configuratie voor deze hardware:**

- Gebruik bij een eigen configuratie `src/Configuratie/Sensoren/ConfigHCSR04P.h` (het aanwezige `src/Configuratie/Sensoren/ConfigHCSR04P_template.h` is een kopieersjabloon en wordt niet rechtstreeks ingelezen).
- Voor één aangesloten exemplaar: `SENSOR_HCSR04P_AANTAL` op **1**, behalve wanneer de bestaande groeps-/setconfiguratie uitdrukkelijk een andere betekenis heeft.
- De bestaande configuratie bevat deze aansluitrelevante definities (gebruik de **werkelijke aangesloten** pinnen, adressen en referenties):
  - `SENSOR_HCSR04P_AANTAL`: sjabloonwaarde `1`.

**Letterlijk constructorfragment uit de bestaande voorbeeldsketch** (alle gebruikte parent-objecten moeten zelf correct aangesloten en geïnitialiseerd zijn):

```cpp
GedeeldeBusNewComponent<HSR0x>(&Native, GedeeldeBusComponent::HCSR04P, HardwareResourcePin::NP_D2, HardwareResourcePin::NP_D3);
```

**Bestaande test-/voorbeeldsketches:**

- `examples/Systeem/GedeeldeBus/Sensoren/Sensor_HCSR04P/Sensor_HCSR04P.ino`


### Praktische aansluiting en gebruik

**Aansluitvoorbeeld voor de afzonderlijk gedocumenteerde 3,3V-HC-SR04P-uitvoering:** een technische handleiding van GENIROBOT beschrijft `VCC` op **3,3 V**, `TRIG` op een digitale uitgang, `ECHO` op een digitale ingang en `GND` op massa. In de FrameWork wordt de gekozen TRIG-pin `np_TRIG` en ECHO-pin `np_ECHO`; het zijn twee **verschillende native pinnen**. `HSR0x::inpluggen()` weigert een identieke TRIG/ECHO-pin; de driver ondersteunt dus **geen eenpinsmodus**.

- Verbind bij de **feitelijk gebruikte** HC-SR04P-module pas de voeding en ECHO nadat de printopdruk of bijgeleverde datasheet de werkspanning en uitgangsniveaus bevestigt. De 3,3V-gegevens gelden voor de bovengenoemde uitvoering; een willekeurige HC-SR04P is daarmee niet bewezen identiek.

**Documentatie van deze 3,3V-uitvoering:** https://www.genirobot.co.kr/download/GENIBOT%20Technical%20Guide%20GB1-TRG0805P2.pdf (sectie HC-SR04P).


**Praktische controle van de werkelijk geleverde HC-SR04P:** de FrameWork `HSR0x`-driver vereist altijd twee **verschillende** native pinnen. Verbind de op de module gemerkte `TRIG` met `np_TRIG` en `ECHO` met `np_ECHO`, plus GND en de op de eigen print toegelaten voedingsrail. De 3,3V-GENIROBOT-uitvoering in de bovenstaande documentatie is **geen** bewijs dat iedere module met HC-SR04P-opdruk exact hetzelfde voedings- of ECHO-niveau heeft. Gebruik bij een 5V-ECHO een passende ingangsniveauaanpassing op 3,3V-controllers.

### Aansluiting van de gebruikte RCWL-9610A-ultrasoonprint

De aanwezige vierpins-ultrasoonprint is gemerkt **`RCWL-9610A design`** en draagt configuratieopschriften voor **`GPIO`, `IIC`, `UART` en `1-WIRE`** met soldeerkeuzes `M1`/`M2`.

- De bestaande `HSR0x`-route voor `HCSR04P` werkt **alleen in de afzonderlijke TRIG/ECHO-GPIO-modus**: `TRIG` → `np_TRIG`, `ECHO` → `np_ECHO`, plus correcte `VCC` en `GND`.
- **Controleer de fysieke moduskeuze `M1`/`M2` vóór aansluiting.** De aanwezigheid van GPIO, I²C, UART en 1-Wire op dezelfde print betekent niet dat de FrameWork-driver die alternatieve protocollen ondersteunt.
- De toegestane voedingsspanning en het ECHO-uitgangsniveau moeten bij deze **specifieke RCWL-9610A-uitvoering** bevestigd zijn; neem ze niet automatisch over van een standaard HC-SR04 of een andere HC-SR04P-module.

## Testen

Voorbeelden: `examples/Systeem/GedeeldeBus/Sensoren/Sensor_HCSR04/`, `examples/Systeem/GedeeldeBus/Sensoren/Sensor_HCSR04P/`, `examples/Systeem/GedeeldeBus/Sensoren/Sensor_HYSRF05/`.
Compiletests: `extras/TestLibrarySensoren-v2.0.0.cmd` en `extras/TestLibrarySensorenOngeldig-v2.0.0.cmd`.

