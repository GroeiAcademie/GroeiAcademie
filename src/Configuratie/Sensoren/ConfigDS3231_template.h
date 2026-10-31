#ifndef CONFIG_DS3231_H
#define CONFIG_DS3231_H
#ifndef SENSOR_DS3231_AANTAL
// #define SENSOR_DS3231_AANTAL 1  // standaard waarde: 0
#endif
#ifndef I2C_ADDRESS_SENSOR_DS3231_1
// #define I2C_ADDRESS_SENSOR_DS3231_1 0x68  // standaard waarde: 0x68 (vast adres van de DS3231)
#endif
// Let op: de MPU6050 en de MPU6500 gebruiken met AD0 laag ook 0x68 (AD0 hoog = 0x69, of via TCA9548).
// De AT24C32-EEPROM op de meeste DS3231-modules zit op 0x57.
#endif
