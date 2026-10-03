# GedeeldeBus

## Doel

GedeeldeBus is in beta v2.0.0 de neutrale lifecycle- en resourcebeheerlaag van het GroeiAcademie FrameWork. Ze vervangt niet de subsystemen zelf. Screen, Input, Stimulus, Extenders en Sensoren blijven afzonderlijk testbaar.

## Boom

```text
Native
├─ Screen
├─ Input
├─ Extenders
└─ Sensoren
```

Een Extender kan zelf children hebben. Een Sensor kan rechtstreeks onder Native hangen of op een Extender worden ingeplugd.

## Lifecycle

```text
aanmelden() → controleren() → inpluggen() → activeren()
```

De normale helper is:

```cpp
T* component = GedeeldeBusNewComponent<T>(...);
```

Bij falen geeft deze `nullptr` terug.

## Resourcecontrole

GedeeldeBus onderscheidt:

- Native resources via `HardwareResourcePin`;
- gedeelde busidentiteiten zoals I2C SDA/SCL en SPI SCK/MISO/MOSI;
- I2C-adressen;
- resources op een concrete Extender via diens `ExtenderPins`.

`HardwareResourcePin` bevat uitsluitend resources van de Arduino Uno R3-vormfactor. Extender-interne resources blijven eigendom van de concrete Extender.

## ExtenderPins

Alle `ExtenderPins`-waarden gebruiken:

```text
EP_<pinnaam>
```

De prefix maakt duidelijk dat het om een resource **op de Extender** gaat en voorkomt botsingen met globale macro's.

## Inpluggen

`inpluggen()` is onderdeel van iedere normale start. Het controleert alleen wat minimaal nodig is om de hardware bruikbaar te verklaren en om naar `activeren()` te mogen doorgaan.

## Diagnose

Uitgebreide elektronische functietesten horen niet in `inpluggen()`. Daarvoor is de DEBUG-only interface voorzien:

```cpp
#ifdef DEBUG
  virtual void Diagnose() {}
#endif
```

Deze is momenteel bewust een dummy in de basis en kan later per concrete component worden overschreven.

## Testinterpretatie

Een succesvolle GedeeldeBus-lifecycle bewijst:

- dat de resourceconfiguratie voldoende geldig is;
- dat geen gedetecteerd resourceconflict de component blokkeert;
- dat de minimale inplugcontrole slaagt;
- dat de component geactiveerd kan worden.

Ze bewijst **niet** automatisch dat iedere GPIO, ADC-ingang, UART, interruptlijn, multiplexeruitgang of ander elektrisch kanaal functioneel is doorgemeten. Dat behoort tot Diagnose en de latere praktijktest.
