# ExtenderMCP23S17

Status: experimenteel.

Library: Rob Tillaart MCP23S17 0.8.2.

De publieke API van ExtenderMCP23S17 is de API van MCP23S17 0.8.2 van Rob Tillaart. Een wissel van bibliotheek breekt sketches die deze API gebruiken.

GedeeldeBus: SPI. Per instantie is CS exclusief; SCK/MISO/MOSI zijn gedeeld. `Activeren()` voert `begin()` en `isConnected()` uit.
