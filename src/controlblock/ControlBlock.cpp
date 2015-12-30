#include "ControlBlock.h"

#include "ArcadeGamepad.h"
#include "SNESGamepad.h"
#include "MAMEGamepad.h"

ControlBlock::ControlBlock() : configuration(new ControlBlockConfiguration())
{
    std::map <ControlBlockConfiguration::ShutdownType_e, PowerSwitch::ShutdownActivated_e>
        switchMapping;
    switchMapping[ControlBlockConfiguration::SHUTDOWN_ACTIVATED] = PowerSwitch::SHUTDOWN_ACTIVATED;
    switchMapping[ControlBlockConfiguration::SHUTDOWN_DEACTIVATED] = PowerSwitch::SHUTDOWN_DEACTIVATED;

    configuration->initialize();

    // if(configuration->getGamepadType() == ControlBlockConfiguration::GAMEPAD_SNES)
    // {
    //     // DigitalIn::getInstance().setMode(DigitalIn::DI_MODE_SNES);
    //     // DigitalOut::getInstance().setMode(DigitalOut::DO_MODE_SNES);
    // }
    // else
    // {
    //     // DigitalIn::getInstance().setMode(DigitalIn::DI_MODE_ALLIN);
    //     // DigitalOut::getInstance().setMode(DigitalOut::DO_MODE_ONLYPOWERSWITCH);
    // }

    powerSwitch = new PowerSwitch(switchMapping[configuration->getShutdownActivation()]);
    
    for(uint8_t counter = 0; counter < NUMGAMEPADS; counter++)
    {
        if(configuration->getGamepadType() == ControlBlockConfiguration::GAMEPAD_ARCADE)
        {
            gamepads[counter] = new ArcadeGamepad();
        }
        else if(configuration->getGamepadType() == ControlBlockConfiguration::GAMEPAD_SNES)
        {
            gamepads[counter] = new SNESGamepad();
        }
        else if(configuration->getGamepadType() == ControlBlockConfiguration::GAMEPAD_MAME)
        {
            gamepads[counter] = new MAMEGamepad();
        }
        else
        {
            std::cout << "Error while configuring gamepad type ..." << std::cout;
            throw 1;
        }
        gamepads[counter]->initialize(counter == 0 ? InputDevice::CHANNEL_1 : InputDevice::CHANNEL_2);
    }
}

ControlBlock::~ControlBlock()
{
    for(uint8_t counter = 0; counter < NUMGAMEPADS; counter++)
    {
        delete gamepads[counter];
        gamepads[counter] = 0;
    }
    delete powerSwitch;
    delete configuration;
}

void ControlBlock::update()
{
    try
    {
        for(uint8_t counter = 0; counter < NUMGAMEPADS; counter++)
        {
            gamepads[counter]->update();
        }
    }
    catch(int errno)
    {
        std::cout << "Error while updating the gamepad devices. Error number: " << errno << std::endl;
    }
    try
    {
        powerSwitch->update();
    }
    catch(int errno)
    {
        std::cout << "Error while updating the power switch instance. Error number: " << errno << std::endl;
    }
}