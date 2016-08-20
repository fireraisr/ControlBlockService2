#ifndef HALFACTORY_H
#define HALFACTORY_H

#include "mcp23s17pi.h"

class HALFactory {
public:
	enum MCPChannel
	{
		MCPCHANNEL_1 = 0,
		MCPCHANNEL_2,
		MCPCHANNEL_NUMBERELEMENTS
	};

	enum MCPAddress
	{
		ADDRESS_00 = 0,
		ADDRESS_01,
		ADDRESS_10,
		ADDRESS_11
	};

	~HALFactory();

	static HALFactory& getInstance()
	{
		static HALFactory halFactory = HALFactory();
		return halFactory;
	}

	MCP23S17PI& getMCP23S17(MCPAddress address, MCPChannel channel);

private:
	HALFactory();

	MCP23S17PI* mcp23s17[MCPCHANNEL_NUMBERELEMENTS];
};

#endif  // HALFACTORY_H
