#ifndef CONTROLBLOCKCONFIGURATION_H
#define CONTROLBLOCKCONFIGURATION_H

#include <json/json.h>
#include <stdint.h>

class ControlBlockConfiguration {
public:
	static const uint8_t MAXIMUM_NUMBER_CONTROLBLOCKS = 4u;

	enum GamepadType_e
	{
		GAMEPAD_ARCADE = 0,
		GAMEPAD_MAME,
		GAMEPAD_SNES,
		GAMEPAD_NONE
	};

	enum ShutdownType_e
	{
		SHUTDOWN_DEACTIVATED = 0,
		SHUTDOWN_ACTIVATED
	};

	enum CBAddress_e
	{
		CBADDRESS_0 = 0,
		CBADDRESS_1 = 1
	};

	struct ControlBlockConfig
	{
		bool isEnabled;
		CBAddress_e sj0;
		CBAddress_e sj1;
		GamepadType_e gamepadType;
		bool secondPlayerEnabled;
	};

	ControlBlockConfiguration();
	~ControlBlockConfiguration();
	
	void initialize();
	bool isEnabled(uint8_t controlBlockIndex) const;
	CBAddress_e getAddressSJ0(uint8_t controlBlockIndex) const;
	CBAddress_e getAddressSJ1(uint8_t controlBlockIndex) const;
	GamepadType_e getGamepadType(uint8_t controlBlockIndex) const;
	bool isSecondPlayerEnabled(uint8_t controlBlockIndex) const;
	ControlBlockConfig const & getConfig(uint8_t controlBlockIndex) const;
	ShutdownType_e getShutdownActivation() const;

private:
	ControlBlockConfig controlBlockConfigs[MAXIMUM_NUMBER_CONTROLBLOCKS];
	ShutdownType_e doShutdown;

};

#endif
