#include "../../Configuratie/SystemConfig.h"

#if EXTENDER_MCP23017_AANTAL >= 2
  #include "GedeeldeBus.h"

constexpr ExtenderMCP23017::Mcp23017Pinnen ExtenderMCP23017::ExtenderLijst[];
#endif
