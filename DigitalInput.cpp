
#include "DigitalInput.h"
#include "Packet.h"
#include <stdlib.h>


DigitalInput::DigitalInput(
        uint32_t channel
        ) :
    Component(),
    myChannel(channel),
    myValue(false),
    myIsPinConfigured(false),
    myOutgoingPacket(new DigitalInputPacket(myChannel))
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
    const DigitalValuePacket& dValPacket = static_cast<const DigitalValuePacket&>(packet);
    myValue = dValPacket.getValue();

    if (myOutgoingPacket == NULL)
    {
        myOutgoingPacket = new DigitalInputPacket(myChannel);
    }

    return true;
}
