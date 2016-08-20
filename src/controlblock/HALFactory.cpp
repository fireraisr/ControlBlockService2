#include "HALFactory.h"
#include <stddef.h>

HALFactory::HALFactory()
{
}

HALFactory::~HALFactory()
{
    for(uint8_t i = 0u; i < MCPCHANNEL_NUMBERELEMENTS; ++i)
    {
        if(mcp23s17[i] != NULL)
        {
            mcp23s17[i]->end();
            delete mcp23s17[i];
            mcp23s17[i] = NULL;
        }
    }
}

MCP23S17PI& HALFactory::getMCP23S17(MCPAddress address, MCPChannel channel)
{
	uint8_t index = static_cast<uint8_t>(address) << 1u;
	if (channel == MCPCHANNEL_2)
	{
		index += 1u;
	}

	if (mcp23s17[index] == NULL)
	{
		mcp23s17[index] = new MCP23S17PI(MCP23S17PI::CHIPSELECT_0, index);
		mcp23s17[index]->begin();
	}
	return *mcp23s17[index];
}
