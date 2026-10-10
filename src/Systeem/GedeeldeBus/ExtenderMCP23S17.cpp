#include "../../Configuratie/SystemConfig.h"

#if EXTENDER_MCP23S17_AANTAL >= 2
  #include "GedeeldeBus.h"

constexpr ExtenderMCP23S17::Mcp23s17Pinnen ExtenderMCP23S17::ExtenderLijst[];
#endif
