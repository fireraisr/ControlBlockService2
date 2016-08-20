#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>

#include "ControlBlockConfiguration.h"

ControlBlockConfiguration::ControlBlockConfiguration() :
	doShutdown(SHUTDOWN_ACTIVATED)
{
	for (uint8_t index = 0u; index < MAXIMUM_NUMBER_CONTROLBLOCKS; index++)
	{
		controlBlockConfigs[index].isEnabled = false;
	}
}

ControlBlockConfiguration::~ControlBlockConfiguration() 
{
}

void ControlBlockConfiguration::initialize() 
{
	try {
		Json::Value root;   // will contains the root value after parsing.
		Json::Reader reader;

		std::ifstream t("/etc/controlblockconfig.cfg");
		std::string config_doc((std::istreambuf_iterator<char>(t)),
		                 		std::istreambuf_iterator<char>());

		bool parsingSuccessful = reader.parse( config_doc, root );
		if ( !parsingSuccessful ) {
		    // report to the user the failure and their locations in the document.
		    std::cout  << "Failed to parse configuration\n"
		               << reader.getFormattedErrorMessages();
		    return;
		}

		const Json::Value controlBlocks = root["controlblocks"];
		// Iterate over the sequence elements.
		for ( uint32_t index = 0u; index < controlBlocks.size(); index++ )
		{
			if (controlBlocks[index]["enabled"] != NULL)
			{
				controlBlockConfigs[index].isEnabled = controlBlocks[index]["enabled"].asBool();
			}
			else
			{
				controlBlockConfig[index].isEnabled = false;
			}
			if (controlBlocks[index]["sj0"] != NULL)
			{
				controlBlockConfigs[index].sj0 = controlBlocks[index]["sj0"].asInt();
			}
			else
			{
				controlBlockConfig[index].sj0 = 0;
			}
			if (controlBlocks[index]["sj1"] != NULL)
			{
				controlBlockConfigs[index].sj1 = controlBlocks[index]["sj1"].asInt();
			}
			else
			{
				controlBlockConfig[index].sj1 = 0;
			}
			if (controlBlocks[index]["gamepadtype"] != NULL)
			{
				std::string configvalue = controlBlocks[index]["gamepadtype"].asString();
				if (configvalue.compare("arcade") == 0) {
					controlBlockConfig[index].gamepadType = GAMEPAD_ARCADE;
				} else if (configvalue.compare("mame") == 0) {
					controlBlockConfig[index].gamepadType = GAMEPAD_MAME;
				} else if (configvalue.compare("snes") == 0) {
					controlBlockConfig[index].gamepadType = GAMEPAD_SNES;
				} else if (configvalue.compare("none") == 0) {
					controlBlockConfig[index].gamepadType = GAMEPAD_NONE;
				}
			}
			else
			{
				controlBlockConfig[index].gamepadType = GAMEPAD_NONE;
			}
			if (controlBlocks[index]["enableSecondPlayer"] != NULL)
			{
				controlBlockConfigs[index].secondPlayerEnabled = controlBlocks[index]["enableSecondPlayer"].asBool();
			}
			else
			{
				controlBlockConfig[index].secondPlayerEnabled = false;
			}
		}

		bool configboolean = root["powerswitch"]["activated"].asBool();
		if (configboolean) {
			doShutdown = SHUTDOWN_ACTIVATED;
		} else {
			doShutdown = SHUTDOWN_DEACTIVATED;
		}
		std::cout << "Read configuration: doShutdown = " << doShutdown << std::endl;
	} catch (int errno) {
		std::cout << "Error while initializing ControlBlockConfiguration instance. Error number: " << errno << std::endl;
	}
}

bool ControlBlockConfiguration::isEnabled(uint8_t controlBlockIndex) const
{
	return controlBlockConfigs[controlBlockIndex].isEnabled;
}

ControlBlockConfiguration::CBAddress_e ControlBlockConfiguration::getAddressSJ0(uint8_t controlBlockIndex) const
{
	return controlBlockConfigs[controlBlockIndex].sj0;
}

ControlBlockConfiguration::CBAddress_e ControlBlockConfiguration::getAddressSJ1(uint8_t controlBlockIndex) const
{
	return controlBlockConfigs[controlBlockIndex].sj1;
}

ControlBlockConfiguration::GamepadType_e ControlBlockConfiguration::getGamepadType(uint8_t controlBlockIndex) const  {
	return controlBlockConfigs[controlBlockIndex].gamepadType;
}

bool ControlBlockConfiguration::isSecondPlayerEnabled(uint8_t controlBlockIndex) const
{
	return controlBlockConfigs[controlBlockIndex].secondPlayerEnabled;
}

ControlBlockConfiguration::ControlBlockConfig const & ControlBlockConfiguration::getConfig(uint8_t controlBlockIndex) const
{
	return controlBlockConfigs[controlBlockIndex];
}

ControlBlockConfiguration::ShutdownType_e ControlBlockConfiguration::getShutdownActivation() const {
	return doShutdown;
}
