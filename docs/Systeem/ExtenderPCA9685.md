# ExtenderPCA9685

Status: experimenteel.

Library: Rob Tillaart PCA9685_RT 0.7.4.

GedeeldeBus: I2C. `Activeren()` voert `begin()` en `isConnected()` uit. De 16 PWM-kanalen zijn beschikbaar via de geërfde PCA9685-interface.


De PCA9685 antwoordt standaard ook op het All Call-adres 0x70. Gebruik je ook een TCA9548A op 0x70, kies dan een ander TCA-adres of zet All Call uit.
