
#include "DigitalOutput.h"
#include "RedBotPacket.h"
#include <stdlib.h>


DigitalOutput::DigitalOutput(
        uint32_t channel
        ) :
    Component(),
    myChannel(channel),
    myCurrentPacket(NULL),
    myIsPinConfigured(false)
{
}

DigitalOutput::~DigitalOutput()
{
}

void
DigitalOutput::Set(
        uint32_t value
        )
{
    if (myCurrentPacket != NULL)
    {
        delete myCurrentPacket;
    }

    myCurrentPacket = new DigitalOutputPacket(
            myChannel,
            ((value == 0) ? false : true)
            );
}

Packet*
DigitalOutput::getNextPacket()
{
    Packet* packet = NULL;

    if (myIsPinConfigured == true)
    {
        packet = myCurrentPacket;
        myCurrentPacket = NULL;
    }
    else
    {
        packet = new PinConfigPacket(
                myChannel,
                PinConfigPacket::DIR_OUTPUT
                );
        myIsPinConfigured = true;
    }

    return packet;
}

bool
DigitalOutput::processPacket(
        const Packet& packet
        )
{
    return false;
}
