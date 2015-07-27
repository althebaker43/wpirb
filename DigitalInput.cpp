
#include "DigitalInput.h"
#include "Packet.h"
#include <stdlib.h>
//#include <iostream>


DigitalInput::DigitalInput(
        uint32_t channel
        ) :
    Component(),
    myChannel(channel),
    myValue(false),
    myIsPinConfigured(false),
    myOutgoingPacket(new DigitalInputPacket(myChannel)),
    myTimeoutCounter(0)
{
}

DigitalInput::~DigitalInput()
{
    if (myOutgoingPacket != NULL)
    {
        delete myOutgoingPacket;
    }
}

bool
DigitalInput::Get()
{
    return myValue;
}

Packet*
DigitalInput::getNextPacket()
{
    Packet* packet = NULL;

    if (myIsPinConfigured == true)
    {
        if (myOutgoingPacket == NULL)
        {
            if (myTimeoutCounter > TIMEOUT_THRESH)
            {
                myOutgoingPacket = new DigitalInputPacket(myChannel);
                myTimeoutCounter = 0;
            }
            else
            {
                ++myTimeoutCounter;
            }
        }

        packet = myOutgoingPacket;
        myOutgoingPacket = NULL;
    }
    else
    {
        packet = new PinConfigPacket(
                myChannel,
                PinConfigPacket::DIR_INPUT
                );
        myIsPinConfigured = true;
    }

    return packet;
}

bool
DigitalInput::processPacket(
        const Packet& packet
        )
{
    if (packet.getType() != Packet::TYPE_DVALUE)
    {
        return false;
    }

    //std::cout << "Info: recieved digital value packet." << std::endl;
    const DigitalValuePacket& dValPacket = static_cast<const DigitalValuePacket&>(packet);
    myValue = dValPacket.getValue();
    myTimeoutCounter = 0;

    if (myOutgoingPacket == NULL)
    {
        myOutgoingPacket = new DigitalInputPacket(myChannel);
    }

    return true;
}
