# Sensorinventaris

Dit document toont welke sensoren en modules al ter beschikking zijn voor het GroeiAcademie FrameWork. Het is een **inventaris van beschikbare voorraad, geen gevalideerd schema**. Zie [MODULES.md](MODULES.md) voor de NLP-toepassingsgebieden die deze sensoren zouden kunnen dienen, en voor de vereisten waaraan een schema moet voldoen vóór het als referentieontwerp wordt opgenomen.

Meerdere sensoren hieronder meten hetzelfde of een overlappend signaal. Er is nog geen keuze gemaakt voor een "officieel ondersteunde" sensor per signaal, zoals `docs/HARDWARE_SUPPORT.md` dat wel al doet voor Arduino Uno R3-vormfactorborden. Deze inventaris toont dus alle beschikbare opties naast elkaar, niet een voorkeur.

## Aanraking, capacitieve bediening en gebruikersinput

* HW-139, 1-kanaals capacitieve touch-sensor, aanraakschakelaar (meet veranderingen in capaciteit, verstoord door je vinger, stuurt een digitaal signaal 0 of 1)
* TTP223, 1-kanaals capacitieve touch-sensor module (1x1), aanraakschakelaar
* TTP224, 1-kanaals capacitieve touch-sensor module (1x4), 4 aanraakvlakken
* TTP229, 16-kanaals capacitieve touch-sensor module (4x4), vaak gebruikt als digitaal keypad

Toepasbaar voor stimuli, gebruikersinput, keuzes, bevestigingen en het registreren van aanraking. Geen fysiologische meting, maar een alternatieve inputmethode naast het bestaande `PIN_TOETS`-systeem.

## Ademhaling

* Piëzo-ademhalingsband, mechanische ademhalingsmeting via rekstrook (×2 nodig: borst + buik, voor onderscheid tussen tempo en type ademhaling)
* MR60BHA2, contactloze ademhaling en hartslag via 60 GHz mmWave (Human Breathing and Heartbeat Sensor, 60GHz mm Wave)
* SDP810-125PA, drukverschillen en luchtstroom (Druksensor ±0018PSI, ±0125kPa)
* INMP441, ademgeluid via digitale microfoon (Omnidirectionele microfoonmodule MEMS, I2S-interface, ESP32-module)
* MAX4466, microfoon met instelbare versterking (Hoogwaardige elektro-microfoonversterker met instelbare versterkingsfactor)
* MAX9814, microfoon met automatische versterkingsregeling (Hoogwaardige microfoonversterker met AGC en geïntegreerde elektretmicrofoon)
* BME680, temperatuur, druk, vochtigheid en luchtkwaliteit (4-in-1 atmosferische sensor met IAQ-index)
* BME688, temperatuur, druk, vochtigheid en VOC (4-in-1 sensor met gas, druk, vochtigheid en temperatuur)
* SCD41, CO₂, temperatuur en vochtigheid (Gassensormodule, detecteert CO2 via I2C)
* MPU6050, MPU6500 en ADXL345, borst- en buikbeweging (zie Beweging, houding en lichaamsbeweging)

Hiermee kunnen onder meer ademfrequentie, ritme, fase, diepte, luchtstroom en borst- of buikbeweging worden onderzocht.

## Aanwezigheid, afstand en locatie

* HC-SR04, ultrasone sensor voor contactloze afstandsmetingen
* HC-SR04P, ultrasone sensor voor contactloze afstandsmetingen
* HY-SRF05, ultrasone sensor voor contactloze afstandsmetingen
* VL53L0X, laserbereiksensor 940 nm ToF-module, anti-zonlicht (200 cm)
* VL6180X, laserbereiksensor 940 nm ToF-module, anti-zonlicht (50 cm)
* HW-201, IR infrarood obstakelvermijdingssensor
* HC-SR501, PIR bewegingssensor infrarood, menselijke aanwezigheidsdetectie
* LD2410D, menselijke aanwezigheidsdetectie sensormodule
* LD2450, menselijke aanwezigheidsdetectie en locatiebepaling sensormodule

Toepasbaar voor aanwezigheid, afstand, beweging, lichaamshouding en locatie in een ruimte.

## Beweging, houding en lichaamsbeweging

* ADXL345, 3-assige digitale zwaartekrachtsensor en acceleratiemodule (GY-291)
* MPU6050, accelerometer + 3-axis analoge gyrosensor
* MPU6500, accelerometer + 6-axis analoge gyrosensor
* OT3686, optische snelheidssensor
* PIEZO-01, vibratiesensormodule met piezoelektrisch element voor impact en vibratie

Toepasbaar voor borst- en buikbeweging, lichaamshouding, beweging, tremor, reactietijd en bewegingsartefacten.

## Bio-elektrische en biopotentiële signalen

* BioAmp EXG Pill (BIOAMPEXPPILL), signalen van publicatiekwaliteit, ECG, EEG, EMG en EOG
* AD8232, ECG-meetmodule, hartslagmeter-sensorkit met klinische nauwkeurigheid
* NeuroSky TGAM, brainwave sensor module, EEG

Referentie-/validatie-apparaten (geen kandidaat-sensoren voor integratie — dienen om te controleren of eigen hardware zoals BioAmp/NeuroSky TGAM dezelfde brainwave-patronen kan tonen als een gevestigd consumentenapparaat):

* NeuroSky MindWave Mobile 2, EEG
* Mindfield eSense, zie Hartslag, HRV en hartcoherentie
* MindFlex, EEG-gebaseerde toepassing (MindFlex Mattel)

Belangrijk onderscheid:

* EEG, elektrische hersenactiviteit
* EMG, spieractiviteit, spierspanning
* ECG, elektrische activiteit van het hart
* EOG, elektrische signalen rond oogbewegingen

## Druk, kracht en aanraking

* FSR400-402, membraan druksensor module voor krachtdetectieweerstand
* RFP-602, dunne-film druksensor, flexibele kracht 50G
* FSR400, krachtgevoelige weerstand 0,5 inch, druk meten via membranen
* FSR402, krachtgevoelige weerstand 0,5 inch, voor menselijke aanraakbediening, 50gr
* HX711 met loadcells van 1 kg en 5 kg, AD load cell module en gewichtssensor voor elektronische weegschaal

Toepasbaar voor druk, kracht, grijpkracht, voetdruk, aanraking, houding en verandering in spierspanning via uitgeoefende kracht.

## Hartslag, HRV en hartcoherentie

* AD8232, ECG (zie Bio-elektrische en biopotentiële signalen)
* Pulse Sensor / EGBO, optische hartslagsensor (PPG) van Pulsesensor.com
* MAX30102, sensormodule voor hartslag (HR) en bloedzuurstofsaturatie (SpO2) via PPG, I2C
* MR60BHA2, contactloze hartslag (zie Ademhaling)
* Mindfield eSense (MINDFIELD-ESENSE), beheers stressreactie met real-time biofeedback, gekoppeld aan EDA

Hiermee kunnen hartslag, hartslagvariabiliteit, coherentie en herstelreacties worden onderzocht.

## Huidgeleiding en autonome activatie

* Grove GSR (GSR-Grove), elektrische geleidbaarheid huid om emotionele stress en sympathische zenuwactiviteit te detecteren via zweetklieren
* MCU-6701, GSR huidsensor, analoge SPI meting van EDA/GSR galvanische huidreactie
* Mindfield eSense, zie Hartslag, HRV en hartcoherentie

Toepasbaar voor elektrodermale activiteit, zweetklieractiviteit en veranderingen in sympathische activatie.

## Huidtemperatuur en omgevingsmetingen

* DS18B20, 12-bit digitale temperatuursensor
* LM35DZ, temperatuursensor voor analoge temperatuurmeting in graden Celsius
* LM75A, digitale temperatuursensor met hoge nauwkeurigheid, I2C/IIC-interface
* MF52A, B3950 10K NTC-thermistor, zeer nauwkeurige temperatuursensor
* MF52D, B3950 10K NTC-thermistor, zeer nauwkeurige temperatuursensor
* AHT10, hoge precisie digitale temperatuur- en vochtigheidssensor, I2C
* BME680, zie Ademhaling
* BME688, zie Ademhaling
* BMP280, hoge precisie digitale sensor voor temperatuur en luchtdruk, I2C & SPI
* DHT22, temperatuur en vochtigheid
* SCD41, zie Ademhaling

Voor huidtemperatuur moet nog per sensor worden bepaald welke werkelijk geschikt is voor direct huidcontact en welke vooral de omgevingstemperatuur meten.

## Licht en optische metingen

* LM3914, in jouw lijst aangeduid als lichtsensor
* MAX30102, optische PPG (zie Hartslag, HRV en hartcoherentie)
* VL53L0X en VL6180X, optische afstand (zie Aanwezigheid, afstand en locatie)
* OV7670, camera (zie Visuele observatie en pupilgedrag)
* AS608, vingerafdruk (zie Overige sensoren)

Bij de LM3914 moet nog worden gecontroleerd welke exacte module je bezit: de LM3914 zelf is normaal een display-driver en geen lichtsensor.

## Omgevingsfactoren

* BME680, temperatuur, druk, vochtigheid en luchtkwaliteit (zie Ademhaling en Huidtemperatuur en omgevingsmetingen)
* BME688, temperatuur, druk, vochtigheid en VOC (zie Ademhaling en Huidtemperatuur en omgevingsmetingen)
* SCD41, CO₂, temperatuur en vochtigheid (zie Ademhaling en Huidtemperatuur en omgevingsmetingen)

Meet de omstandigheden van de ruimte, niet van de student zelf. Nodig om te weten of een meetverschil tussen twee sessies door de student komt, of door bijvoorbeeld een warmere kamer die dag.

## Overige sensoren

* AS608, optische vingerafdruksensor/module voor het scannen, registreren en verifiëren van vingerafdrukken
* WATER-SENSOR, waterdetectie
* HX711, loadcellversterker en gewichtsmeting (display, sensor, of volledige set met 1kg/5kg loadcell)
* AHT10 en DHT22, temperatuur en vochtigheid
* BMP280, luchtdruk en temperatuur

## Stem en ademgeluid

* INMP441, zie Ademhaling
* MAX4466, zie Ademhaling
* MAX9814, zie Ademhaling

Mogelijke stemparameters:

* toonhoogte;
* intensiteit;
* spreektempo;
* pauzes;
* ritme;
* stemtrilling;
* ademgeluid;
* veranderingen in stemkwaliteit.

## Visuele observatie en pupilgedrag

* OV7670, CMOS VGA-cameramodule 640x480, 0.3MP, I2C-interface

Mogelijke toepassingen:

* oogbewegingen;
* pupilverwijding;
* knippergedrag;
* gelaatsbeweging;
* lichaamshouding.

De OV7670 alleen garandeert nog niet dat pupil- of oogmetingen voldoende betrouwbaar uitgevoerd kunnen worden. Daarvoor zijn ook belichting, optiek en beeldverwerking bepalend.
