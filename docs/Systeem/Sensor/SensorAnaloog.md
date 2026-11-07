# SensorAnaloog

## Implementatie

- Broncode: `src/Systeem/Sensor/SensorAnaloog.h` en `SensorAnaloog.cpp`.
- C++-component: `SensorAnaloog`, afgeleid van `Sensor`.
- Parent: `GedeeldeBusNode*` in de constructor.
- Gebruikt door: `AD8232`, `BioAmpEXGPill`, `EGBOPulseSensor`, `GSRGrove`, `LM35DZ`, `MAX4466_9814`, `MF52x`, `PIEZO`, `WATER`.
- Data-kanalen: 1.

## Uit de broncode

SensorAnaloog: gemeenschappelijke basis voor een analoge Sensor met één ingang op een ADC-Extender. StartAnalogRead(), IsAnalogReadKlaar(), GetAnalogRead() en RawAnalogRead() staan hier één keer. Een afgeleide Sensor geeft alleen zijn ADC-pin en het maximale signaal van zijn uitgang (mV) door.

## Aansluiten

**Aansluitwijze in de bestaande FrameWork-code:**

- **Meetverbinding:** sensor-uitgang naar het toegewezen **ADC-Extender-kanaal**. Verbind de signaalreferentie/GND met de ADC-Extender volgens de vereisten van die Extender.
- **Voeding:** controleer de benodigde spanning en toegestane OUT-spanning van de **specifieke sensor of module** vóór aansluiten; de FrameWork-code bepaalt de elektrische grenswaarden van het moduleboard niet.

- Analoge OUT → het door de constructor gekozen kanaal van de ADC-Extender; de huidige code is niet bedoeld voor een willekeurige digitale GPIO-pin.
- Sensor-GND met ADC-Extender-GND verbinden; OUT binnen het ingestelde meetbereik en spanningslimiet houden.

**Voor de hardwaretest:**

- Gebruik de bestaande `.ino` uit **Testen** en de bestaande configuratiebestanden onder `src/Configuratie/`; een `*_template.h` wordt niet rechtstreeks ingelezen.
- **Een vaste Arduino-pintoewijzing is niet algemeen geldig:** de gekozen constructor, configuratie en werkelijke bekabeling moeten overeenkomen. Waar hierboven geen exact spanningsniveau of pinheader is bevestigd, mag dat niet uit deze documentatie worden afgeleid.


**Aanvulling aansluitpinnen en elektrische voorwaarden:**

- Dit is een generieke analoge sensorinterface, geen afzonderlijk fabrikantproduct. OUT→ADC-kanaal, GND→gemeenschappelijke analoge referentie; VCC en maximaal uitgangssignaal zijn afhankelijk van de concrete sensormodule. Geen universele 3,3V/5V-instructie mogelijk.
- De concrete modulevariant bepaalt de **exacte voedingsgrenzen en fysieke pinvolgorde**. Neem die gegevens niet over van een andere module zonder de werkelijke print te controleren.

**Aansluit- en gebruiksgegevens uit de bestaande FrameWork-interface:**

- **Analoge bekabeling voor SensorAnaloog:** het analoge meetpunt of `OUT` gaat naar het ADC-kanaal `sensorPin` van de opgegeven parent. Gebruik het **kanaal van die ADC-Extender**, niet automatisch een native Arduino-`A0`-pin. Verbind de elektrische meetreferentie met de GND van dezelfde ADC-opstelling.
- De ADC-ingang mag niet buiten zijn werkelijk ingestelde spanningsbereik worden gestuurd. Sensorvoeding en eventuele spanningsdeler horen bij het specifieke sensorbord of het losse element; een C++-kanaalnummer bepaalt geen veilige voedingsspanning.
- De constructor ontvangt `sensorPin`, `bereikMillivolt` en `HardwareResourceToegang`. Een ADC-Extender kan andere spanningsgrenzen hebben dan een native ADC; controleer daarom het effectieve bereik van de parent.
- De bestaande methodevolgorde voor asynchrone conversie is `StartAnalogRead()`, `IsAnalogReadKlaar()` en `GetAnalogRead()`; voor een rechtstreekse waarde bestaat `RawAnalogRead(int32_t&)`.

**Bestaande constructor (parameter-/pinvolgorde):**

```cpp
SensorAnaloog(GedeeldeBusNode* parent, GedeeldeBusComponent component, uint8_t sensorPin, uint16_t bereikMillivolt, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
```



## Testen

### Foutstatus en meetbereik
- `GetAnalogRead()` heeft het bestaande `int`-returncontract: bij fout levert het 0 terug, wat ook een geldige ADC-meetwaarde kan zijn. **Voor een beoordeling die fout en nul moet onderscheiden: gebruik de reeds bestaande `RawAnalogRead(int32_t& waarde)` met `bool`-resultaat**; wijzig niet stilzwijgend het contract van `GetAnalogRead()`.
- **Proef:** meet 0 V (succes met waarde 0), meet een gekende referentiespanning (succes met niet-nul), verbreek de ADC-verbinding (foutstatus via `RawAnalogRead`), test te groot meetbereik (geweigerd). Noteer de ADC-referentie, werkelijke bits, ingestelde schaal en de eenheden van de Sensor.
- **Status:** bestaande API biedt een afzonderlijke foutstatus; alle verbruikers van `GetAnalogRead()` moeten nog per gebruik gecontroleerd worden.

### ADC-time-outs en foutstatus

- De bestaande `RawAnalogRead()` wacht maximaal 150 ms. Test bij iedere ADC-configuratie trage conversie, ontbrekende READY, losgekoppelde ingang en herstel. Controleer of time-outs als fout gerapporteerd worden en niet als geldige nul. Verhoog 150 ms niet algemeen zonder concrete meetresultaten.
- **Status:** fysieke proef vereist; nog geen hardwarebewijs.
