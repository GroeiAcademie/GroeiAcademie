#ifndef CONFIG_MCU6701_H
#define CONFIG_MCU6701_H
// CJMCU-6701: OUT (analoog via ADC-parent) of MCP3201 (SPI met CS, SCK, MISO).
// Kies de uitleesroute bij het construeren van de Sensor; geen MOSI naar de MCP3201 nodig.
// Het schema gebruikt VREF=4.096V voor MCP3201. Controleer 5V- versus 3.3V-logicaniveaus.
#ifndef SENSOR_MCU6701_AANTAL
// #define SENSOR_MCU6701_AANTAL 1 // standaard: 0
#endif
#ifndef SENSOR_MCU6701_ADC_BEREIK_MILLIVOLT
// #define SENSOR_MCU6701_ADC_BEREIK_MILLIVOLT 5000 // standaard 5000 mV, fysiek OUT-bereik verifiëren
#endif
#endif
