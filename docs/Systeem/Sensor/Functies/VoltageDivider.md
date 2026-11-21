# VoltageDivider

Weerstand van een sensor in een spanningsdeler: voeding, serieweerstand, meetpunt, sensor, GND. Gebruikt door MF52x.

- `static bool CalculateSensorResistanceOhm(int32_t rawValue, uint32_t seriesResistorOhm, float& sensorResistanceOhm)`: R = Rserie x raw / (ADC_MAX - raw).
- `rawValue` moet herschaald zijn zodat ADC_MAX overeenkomt met de voedingsspanning van de deler. Bij `rawValue <= 0`, `rawValue >= ADC_MAX` of `seriesResistorOhm == 0` geeft de functie `false` en zet ze `sensorResistanceOhm` op `NAN`.
- Zit de sensor aan de voedingskant, dan is de formule omgekeerd; dat hangt van de module af.

Bestanden: `src/Systeem/Sensor/Functies/VoltageDivider.h` en `src/Systeem/Sensor/Functies/VoltageDivider.cpp`.
