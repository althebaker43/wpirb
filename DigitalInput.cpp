
#include "DigitalInput.h"
#include "Packet.h"
#include <stdlib.h>


DigitalInput::DigitalInput(
        uint32_t channel
        ) :
    Input(channel),
    myIsPinConfigured(false)
{
}

DigitalInput::~DigitalInput()
{
}

Packet*
DigitalInput::getNextPacket()
{
    Packet* packet = NULL;

    if (myIsPinConfigured == true)
    {
        packet = getNextPacketIfTimedOut();
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
